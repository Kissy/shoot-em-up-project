// Copyright � 2008-2009 Intel Corporation
// All Rights Reserved
//
// Permission is granted to use, copy, distribute and prepare derivative works of this
// software for any purpose and without fee, provided, that the above copyright notice
// and this statement appear in all copies.  Intel makes no representations about the
// suitability of this software for any purpose.  THIS SOFTWARE IS PROVIDED "AS IS."
// INTEL SPECIFICALLY DISCLAIMS ALL WARRANTIES, EXPRESS OR IMPLIED, AND ALL LIABILITY,
// INCLUDING CONSEQUENTIAL AND OTHER INDIRECT DAMAGES, FOR THE USE OF THIS SOFTWARE,
// INCLUDING LIABILITY FOR INFRINGEMENT OF ANY PROPRIETARY RIGHTS, AND INCLUDING THE
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  Intel does not
// assume any responsibility for any errors which may appear in this software nor any
// responsibility to update it.

#include "BaseTypes.h"
#include "Interface.h"

#include "Collision.h"
#include "Scene.h"
#include "Object/Object.h"


//
// global variables
//
extern ManagerInterfaces    g_Managers;


//
// local prototypes
//
static void ProcessColl(CollData& Data, HavokPhysicsScene* pScene);
static void LineTest(const Coll::Request& Request, Coll::Result* Result, HavokPhysicsScene* pScene);


///////////////////////////////////////////////////////////////////////////////
// HavokCollisionService - Default constructor
HavokCollisionService::HavokCollisionService(
    void
) {
    g_Managers.pService->RegisterCollisionProvider(this);
    // Setup collision workspace
    m_HandleCount = 0;
    m_CollData.clear();
    // Setup pending request
    m_PendingRequests.clear();
    // Setup pending results
    m_PendingResults.clear();
    // Setup dead (used) results
    m_DeadHandles.clear();
}


///////////////////////////////////////////////////////////////////////////////
// ~HavokCollisionService - Default destructor
HavokCollisionService::~HavokCollisionService(
    void
) {
    g_Managers.pService->UnregisterCollisionProvider(this);
}

///////////////////////////////////////////////////////////////////////////////
// ProcessRequests - Process all requested collisions
void
HavokCollisionService::ProcessRequests(
    HavokPhysicsScene* pScene
) {
    // Delete all dead entries
    {
        SCOPED_SPIN_LOCK_CONFLICT(m_DeadHandlesLock);
        SCOPED_SPIN_LOCK_CONFLICT(m_PendingResultsLock);

        if (!m_DeadHandles.empty()) {
            std::vector<Coll::Handle>::iterator it;

            for (it = m_DeadHandles.begin(); it != m_DeadHandles.end(); it++) {
                m_CollData.erase(*(it));
                m_PendingResults.erase(*(it));
            }

            m_DeadHandles.clear();
        }
    }
    // Add all new request
    {
        SCOPED_SPIN_LOCK(m_PendingRequestsLock);

        if (!m_PendingRequests.empty()) {
            std::vector<Coll::Request>::iterator it;

            for (it = m_PendingRequests.begin(); it != m_PendingRequests.end(); it++) {
                // Build an entry
                CollData Data;
                memset(&Data, 0, sizeof(Data));
                Data.m_Request = (*it);
                m_CollData[(*it).m_Handle ] = Data;
            }

            m_PendingRequests.clear();
        }
    }

    // Process all outstanding requests
    if (!m_CollData.empty()) {
        u32 size = (u32)m_CollData.size();
        UNREFERENCED_PARAM(size);
        std::map<Coll::Handle, CollData>::iterator it;

        for (it = m_CollData.begin(); it != m_CollData.end(); it++) {
            ProcessColl((*it).second, pScene);
            // Store result
            {
                SCOPED_SPIN_LOCK(m_PendingResultsLock);
                m_PendingResults[(*it).second.m_Request.m_Handle ] = (*it).second.m_Result;
            }
        }
    }
}


///////////////////////////////////////////////////////////////////////////////
// Test - Requests a collision test
Coll::Handle
HavokCollisionService::Test(
    const Coll::Request& Request
) {
    Coll::Handle Handle = Coll::InvalidHandle;
    // Create a new entry
    Coll::Request PendingRequest = Request;
    // Store data to run the test later
    {
        SCOPED_SPIN_LOCK(m_PendingRequestsLock);
        Handle = GetNextHandle();
        PendingRequest.m_Handle = Handle;
        m_PendingRequests.push_back(PendingRequest);
    }
    // Return the handle
    return Handle;
}


///////////////////////////////////////////////////////////////////////////////
// LineTest - Requests a collision line test
Coll::Handle
HavokCollisionService::LineTest(
    const Math::Vector3& Start,
    const Math::Vector3& End,
    Coll::Request& Request
) {
    // Build request
    Request.m_Type      = Coll::e_LineTest;
    Request.m_Position0 = Start;
    Request.m_Position1 = End;
    // Register the test
    return Test(Request);
}


///////////////////////////////////////////////////////////////////////////////
// Finalize - Gets results for the given handle
bool
HavokCollisionService::Finalize(
    Coll::Handle Handle,
    Coll::Result* pResult
) {
    ASSERT(pResult != NULL);
    ASSERT(Handle != Coll::InvalidHandle);

    // Return if the handle isn't valid
    if (Handle == Coll::InvalidHandle) {
        return false;
    }

    // Finalized will be returned if the test has completed
    bool Finalized = false;
    // Get the data for the given handle
    bool ResultAvailable = false;
    Coll::Result PendingResult;
    memset(&PendingResult, 0, sizeof(PendingResult));
    {
        SCOPED_SPIN_LOCK(m_PendingResultsLock);
        std::map<Coll::Handle, Coll::Result>::iterator it = m_PendingResults.find(Handle);

        if (it != m_PendingResults.end()) {
            ResultAvailable = true;
            PendingResult = (*it).second;
        }
    }

    if (ResultAvailable) {
        // Get store the result
        Finalized = true;

        if (pResult) {
            *pResult = PendingResult;
        }

        // Mark for deletion
        {
            SCOPED_SPIN_LOCK(m_DeadHandlesLock);
            m_DeadHandles.push_back(Handle);
        }
    }

    return Finalized;
}


///////////////////////////////////////////////////////////////////////////////
// GetNextHandle - Returns the next unique handle
Coll::Handle
HavokCollisionService::GetNextHandle(
    void
) {
    m_HandleCount = (m_HandleCount + 1) % 0xFFFFFFFE;
    return m_HandleCount;
}


///////////////////////////////////////////////////////////////////////////////
// ProcessColl - Process and individual collision
static void
ProcessColl(
    CollData& Data,
    HavokPhysicsScene* pScene
) {
    // Process the collision (if not already processed)
    if (!Data.m_Result.m_Finalized) {
        switch (Data.m_Request.m_Type) {
            case Coll::e_LineTest:
                LineTest(Data.m_Request, &Data.m_Result, pScene);
                break;

            default:
                ASSERT(false);    // Unsupported collision type
                break;
        }
    }
}


///////////////////////////////////////////////////////////////////////////////
// LineTest - Initiate a collision line test
static void
LineTest(
    const Coll::Request& Request,
    Coll::Result* Result,
    HavokPhysicsScene* pScene
) {
    ASSERT(Result);
   
    // Mark is as finalized
    Result->m_Finalized = true;
}

