// Copyright © 2008-2009 Intel Corporation
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

#include "Interface.h"

#include "Scene.h"
#include "Object/PhysicObject.h"
#include "Task.h"
#include "Collision.h"
#include "System.h"

#include "Collision.h"

extern ManagerInterfaces    g_Managers;

// Grain sizes of the parallel jobs. The less is the grainsize the better is
// the load balance, and the higher is the parallel overhead.
static const u32    PhysicSystemTaskGrainSize = 8;

///////////////////////////////////////////////////////////////////////////////
// HavokPhysicsTask - Constructor
PhysicTask::PhysicTask(ISystemScene* pScene) : ISystemTask(pScene) {

}


///////////////////////////////////////////////////////////////////////////////
// ~HavokPhysicsTask - Destructor
PhysicTask::~PhysicTask(void) {
    
}

///////////////////////////////////////////////////////////////////////////////
// Update - Update the system (this is were all the work gets done)
void PhysicTask::Update(f32 DeltaTime) {
    //
    // Make sure that the time step is greater than 0.
    //
    if (DeltaTime > 0.0f) {
        if (DeltaTime > 0.04f) {
            DeltaTime = 0.04f;
        }

        m_pSystemScene->Update(DeltaTime);
    }
}