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

#include "Defines.h"
#include "BaseTypes.h"
#include "Interface.h"

#include "Proto.h"
#include "Universal/UScene.h"
#include "Universal/UObject.h"
#include "Debugger/Debugger.h"
#include "Manager/MemoryManager.h"
#include "Manager/SystemManager.h"
#include "Manager/PlatformManager.h"
#include "Manager/ChangeControlManager.h"
#include "Manager/EnvironmentManager.h"
#include "Manager/ServiceManager.h"
#include "Manager/TaskManager.h"
#include "Parser/DefinitionParser.h"
#include "Scheduler.h"
#include "Instrumentation.h"
#include "Framework.h"

#ifdef DEBUG_BUILD
CreateSingleton(Debugger);
#endif

CreateSingleton(MemoryManager);
CreateSingleton(EnvironmentManager);
CreateSingleton(PlatformManager);
CreateSingleton(SystemManager);
CreateSingleton(ServiceManager);
CreateSingleton(Instrumentation);

TaskManager*    g_pTaskManager = NULL;

/**
 * @inheritDoc
 */
void ExecuteFramework(void) {
#ifndef DEBUG_BUILD
    try
#endif
    {
        Framework Framework;
        if (Framework.Initialize() == Errors::Success) {
            Framework.Execute();
            Framework.Shutdown();
        }
    }
#ifndef DEBUG_BUILD
    catch (...) {
        // Display an error message.
    }
#endif
}

/**
 * @inheritDoc
 */
Framework::Framework(void) : 
    m_bExecuteLoop(true)
    , m_pScheduler(NULL)
    , m_pSceneCCM(NULL)
    , m_pObjectCCM(NULL) {
    //
    // g_pTaskManager and m_pScheduler are instantiated after the environment variables
    // in the config file are parsed
    //
    m_pSceneCCM = new ChangeManager();
    m_pObjectCCM = new ChangeManager();
}

/**
 * @inheritDoc
 */
Framework::~Framework(void) {
    delete m_pScheduler;
    delete g_pTaskManager;
    delete m_pSceneCCM;
    delete m_pObjectCCM;
}

/**
 * @inheritDoc
 */
Error Framework::Initialize(void) {
    //
    // Set the current directory to the location of the GDF.
    //
    static const char* apszFile = "Application.gdf.bin";
    static const char* apszLocations[] = { ".\\." };

    if (!Singletons::PlatformManager.FileSystem().SetCurrentDirToFileLocation(apszFile, apszLocations)) {
        ASSERT(false, "Framework could not locate the GDF file Application.gdf.");
        return Errors::File::NotFound;
    }

    //
    // Create the initial universal scene.
    //
    m_pScene = new UScene(m_pSceneCCM, m_pObjectCCM);
    ASSERT(m_pScene != NULL);

    if (m_pScene == NULL) {
        return Errors::Memory::OutOfMemory;
    }

    //
    // Instantiate the parser, parse the environment variables in the GDF.
    //
    DefinitionParser Parser(m_pScene, apszFile);
    Parser.ParseEnvironment();
    
    //
    // Init debugger
    // 
#ifdef DEBUG_BUILD
    bool debuggerActive = Singletons::EnvironmentManager.Variables().GetAsBool("Framework::DebugWindow", false);
    Singletons::Debugger.initialize(debuggerActive);
    Singletons::Debugger.setChangeManagers(m_pSceneCCM, m_pObjectCCM);
#endif
    
    //
    // Register the framework as the system access provider.  The system access provider gives the
    //  ability for systems to set the properties in other systems.
    //
    Singletons::ServiceManager.RegisterSystemAccessProvider(this);
    
    //
    // Instantiate the task manager.
    //
    g_pTaskManager = new TaskManager();
    g_pTaskManager->Init();
    
    //
    // Instantiate the scheduler.
    //
    m_pScheduler = new Scheduler(g_pTaskManager);
    ASSERT(m_pScheduler != NULL);

    if (m_pScheduler == NULL) {
        return Errors::Memory::OutOfMemory;
    }

    //
    // Complete the parsing of the GDF and the initial scene.
    //
    Parser.ParseSystems();
    m_sNextScene = Parser.StartupScene();
    Parser.ParseScene(m_sNextScene);
    
    //
    // Set the initial scene for the scheduler.
    //
    m_pScheduler->SetScene(m_pScene);

    return Errors::Success;
}


void Framework::Shutdown(void) {
    //
    // Clean debugger
    //
#ifdef DEBUG_BUILD
    Singletons::Debugger.clean();
#endif
    //
    // Get rid of the scene.
    //
    delete m_pScene;
    //
    // De-register the framework as the system access provider.
    //
    Singletons::ServiceManager.UnregisterSystemAccessProvider(this);
    //
    // Free resources used for parallel execution by the change manager.
    //
    m_pObjectCCM->ResetTaskManager();
    m_pSceneCCM->ResetTaskManager();

    //
    // Free the task manager.
    //
    if (g_pTaskManager != NULL) {
        g_pTaskManager->Shutdown();
        delete g_pTaskManager;
        g_pTaskManager = NULL;
    }
}


Error Framework::Execute(void) {
    //
    // Process the link messages in the CCMs first, for both the object and scene CCMs.
    // The link needs to be established before any other messages come through.
    //
    m_pObjectCCM->DistributeQueuedChanges(System::Types::All, System::Changes::Link | System::Changes::ParentLink);
    m_pSceneCCM->DistributeQueuedChanges(System::Types::All, System::Changes::Link | System::Changes::ParentLink);

    //
    // Distribute changes for object and scene CCMs.
    // The UObject propagates some object messages up to the scene so it needs to go first.
    //
    m_pObjectCCM->DistributeQueuedChanges(System::Types::All, System::Changes::All);
    m_pSceneCCM->DistributeQueuedChanges(System::Types::All, System::Changes::All);

    //
    // Set the runtime status to running.
    //
    Singletons::EnvironmentManager.Runtime().SetStatus(IEnvironmentManager::IRuntime::Status::Running);

    //
    // Initialize resources necessary for parallel change distribution.
    //
    m_pObjectCCM->SetTaskManager(g_pTaskManager);
    m_pSceneCCM->SetTaskManager(g_pTaskManager);

    while (m_bExecuteLoop) {
        //
        // Process any pending window messages.
        //
        Singletons::PlatformManager.WindowSystem().ProcessMessages();
        //
        // Call the scheduler to have the systems internally update themselves.
        //
        m_pScheduler->Execute();
        
        // Process first the object creation messages alone since it will 
        // generate some object messages that need to be processed by the object CCM.
        m_pSceneCCM->DistributeQueuedChanges(System::Types::All, System::Changes::Generic::CreateObject);
        //
        // Distribute changes for object and scene CCMs.  The UObject propagates some object
        // messages up to the scene CCM so it needs to go first.
        m_pObjectCCM->DistributeQueuedChanges(System::Types::All, System::Changes::All);
        m_pSceneCCM->DistributeQueuedChanges(System::Types::All, System::Changes::All ^ System::Changes::Generic::CreateObject);
        
        //
        // Check with the environment manager if there is a change in the runtime status to quit.
        //
        if (Singletons::EnvironmentManager.Runtime().GetStatus() == IEnvironmentManager::IRuntime::Status::Quit) {
            //
            // Time to quit looping.
            //
            m_bExecuteLoop = false;
        }
    }

    return Errors::Success;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// IService::ISystemAccess Implementations.

Handle Framework::GetSystem(const char* pszSystemName) {
    //
    // Get the pointer to the system from the system manager.  Handle is just a void* so it will
    //  convert with any problems.  If you're one of those guys that will think of just casting
    //  the handle back you'd better not as you'll break the threading when you try to make calls
    //  into this interface directly.
    //
    return Singletons::SystemManager.Get(pszSystemName);
}


Handle Framework::GetSystem(System::Type Type) {
    //
    // Get the pointer to the system from the system manager.  Handle is just a void* so it will
    //  convert with any problems.  If you're one of those guys that will think of just casting
    //  the handle back you'd better not as you'll break the threading when you try to make calls
    //  into this interface directly.
    //
    return Singletons::SystemManager.Get(Type);
}


Handle Framework::GetScene(const char* pszSystemName) {
    Handle hScene = NULL;
    //
    // Get the system from the system manager to get the type.
    //
    ISystem* pSystem = Singletons::SystemManager.Get(pszSystemName);

    if (pSystem != NULL) {
        //
        // Get the scene based on the type.
        //
        hScene = GetScene(pSystem->GetSystemType());
    }

    return hScene;
}


Handle Framework::GetScene(System::Type Type) {
    Handle hScene = NULL;
    //
    // Find the scene extension in the universal scene.
    //
    UScene::SystemScenes::const_iterator it = m_pScene->GetSystemScenes().find(Type);

    if (it != m_pScene->GetSystemScenes().end()) {
        hScene = it->second;
    }

    return hScene;
}


Handle Framework::GetSystemObject(const char* pszSystemName, const char* pszName) {
    Handle hObject = NULL;
    //
    // Get the system from the system manager to get the type.
    //
    ISystem* pSystem = Singletons::SystemManager.Get(pszSystemName);

    if (pSystem != NULL) {
        hObject = GetSystemObject(pSystem->GetSystemType(), pszName);
    }

    return hObject;
}


Handle Framework::GetSystemObject(System::Type Type, const char* pszName) {
    Handle hObject = NULL;
    //
    // Find the universal object in the scene.
    //
    UObject* pUObject = m_pScene->FindObject(pszName);

    if (pUObject != NULL) {
        //
        // Get the system object extension of the universal object using the system type.
        //
        ISystemObject* pObject = pUObject->GetExtension(Type);

        if (pObject != NULL) {
            hObject = pObject;
        }
    }

    return hObject;
}
