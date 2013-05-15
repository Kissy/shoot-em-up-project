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

#include "Proto.h"
#include "Universal/UScene.h"
#include "Universal/UObject.h"
#include "Manager/PlatformManager.h"
#include "Manager/EnvironmentManager.h"
#include "Manager/SystemManager.h"
#include "DefinitionParser.h"

/**
 * @inheritDoc
 */
DefinitionParser::DefinitionParser(UScene* pScene, std::string sGDF)
    : m_pScene(pScene) {
    Error result = Singletons::PlatformManager.FileSystem().LoadProto(sGDF.c_str(), &m_gdProto);
    ASSERT(result == Errors::Success);
}

/**
 * @inheritDoc
 */
void DefinitionParser::ParseEnvironment(void) {
    ASSERT(m_gdProto.IsInitialized());
    for (auto property : m_gdProto.properties()) {
        // Environment properties only have one value
        Singletons::EnvironmentManager.Variables().Add(property.name().c_str(), property.value().Get(0).c_str());
    }
}

/**
 * @inheritDoc
 */
void DefinitionParser::ParseSystems(void) {
    ASSERT(m_gdProto.systems_size() > 0);
    for (auto system : m_gdProto.systems()) {
        Singletons::PlatformManager.FileSystem().LoadSystemLibrary(system.type(), &m_pSystem);
        ASSERT(m_pSystem != NULL);

        // Get the default properties from system, then Initialize it
        m_pSystem->setProperties(system.properties());
        m_pSystem->initialize();
        ASSERTMSG1(system.type() == m_pSystem->GetSystemType(),
                   "Parser identified an incorrect system type. It should be %s.", Proto::SystemType_Name(m_pSystem->GetSystemType()));
    }
}

/**
 * @inheritDoc
 */
void DefinitionParser::ParseScene(std::string sScene) {
    const auto& scenes = m_gdProto.scenes();
    auto sceneIt = std::find(scenes.begin(), scenes.end(), sScene);
    if (sceneIt == scenes.end()) {
        return;
    }
    
    //
    // Create the initial scene for each system.
    //
    ISystem* pSystem = Singletons::SystemManager.GetFirst();
    while (pSystem != NULL) {
        m_pScene->Extend(pSystem);
        pSystem = Singletons::SystemManager.GetNext();
    }

    //
    // Parse the SDF file
    //
    Proto::Scene scene;
    std::string sceneProtoFile = *sceneIt + ".sdf.bin";
    Error result = Singletons::PlatformManager.FileSystem().LoadProto(sceneProtoFile.c_str(), &scene);
    ASSERT(result == Errors::Success);

    //
    // Initialize the System scenes.
    //
    for (auto system : scene.systems()) {
        m_pSystem = Singletons::SystemManager.Get(system.type());
        ASSERTMSG1(m_pSystem != NULL, "Parser was unable to get system %s.", Proto::SystemType_Name(system.type()));

        if (m_pSystem != NULL) {
            UScene::SystemScenesConstIt it = m_pScene->GetSystemScenes().find(m_pSystem->GetSystemType());
            ASSERTMSG1(it != m_pScene->GetSystemScenes().end(), "Parser was unable to find a scene for system %s.", Proto::SystemType_Name(system.type()));
            m_pSystemScene = it->second;
            ASSERT(m_pSystemScene != NULL);
            // Initialize system scene properties
            m_pSystemScene->setProperties(system.properties());
            m_pSystemScene->initialize();
        }
    }

    //
    // Initialize the scene objects.
    //
    for (auto object : scene.objects()) {
        m_pScene->createObject(&object);
    }

    //
    // Refresh all scenes
    // 
    for (auto scene : m_pScene->GetSystemScenes()) {
        scene.second->GlobalSceneStatusChanged(ISystemScene::PostLoadingObjects);
    }

    //
    // Initialize the links.
    //
    for (auto link : scene.links()) {
        UObject* pSubject = m_pScene->FindObject(link.subject().c_str());
        UObject* pObserver = m_pScene->FindObject(link.observer().c_str());

        //
        // Get the extension for the object.
        //
        ISystemObject* pSystemObserver = pSystemObserver = pObserver->GetExtension(link.observersystemtype());

        //
        // Call the scene to register the links.
        //
        if (link.subjectsystemtype() != Proto::SystemType::Null) {
            ISystemObject* pSystemSubject = pSystemSubject = pSubject->GetExtension(link.subjectsystemtype());
            m_pScene->CreateObjectLink(pSystemSubject, pSystemObserver);
        } else {
            m_pScene->CreateObjectLink(pSubject, pSystemObserver);
        }
    }
}

std::string DefinitionParser::StartupScene(void) {
    return m_gdProto.startupscene();
}
