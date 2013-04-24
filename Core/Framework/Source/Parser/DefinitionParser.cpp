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

    const ProtoPropertyList& properties = m_gdProto.properties();
    for (ProtoPropertyList::const_iterator propertiesIt = properties.begin(); propertiesIt != properties.end(); propertiesIt++) {
        // Environment properties only have one value
        Singletons::EnvironmentManager.Variables().Add(propertiesIt->name().c_str(), propertiesIt->value().Get(0).c_str());
    }
}

/**
 * @inheritDoc
 */
void DefinitionParser::ParseSystems(void) {
    ASSERT(m_gdProto.systems_size() > 0);

    const ProtoSystemList& systems = m_gdProto.systems();
    for (ProtoSystemList::const_iterator systemsIt = systems.begin(); systemsIt != systems.end(); systemsIt++) {
        Singletons::PlatformManager.FileSystem().LoadSystemLibrary(systemsIt->type(), &m_pSystem);
        ASSERT(m_pSystem != NULL);

        // Get the default properties from system, then Initialize it
        m_pSystem->setProperties(systemsIt->properties());
        m_pSystem->initialize();
        ASSERTMSG1(strcmp(SystemProto::Type_Name(systemsIt->type()).c_str(), m_pSystem->GetName()) == 0,
                   "Parser identified an incorrect system type. It should be %s.", m_pSystem->GetName());
    }
}

/**
 * @inheritDoc
 */
void DefinitionParser::ParseScene(std::string sScene) {
    const ProtoSceneList& scenes = m_gdProto.scenes();
    ProtoSceneList::const_iterator scenesIt = std::find(scenes.begin(), scenes.end(), sScene);
    if (scenesIt == scenes.end()) {
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
    SceneDefinitionProto sceneDefinitionProto;
    std::string sceneProtoFile = *scenesIt + ".sdf.bin";
    Error result = Singletons::PlatformManager.FileSystem().LoadProto(sceneProtoFile.c_str(), &sceneDefinitionProto);
    ASSERT(result == Errors::Success);

    //
    // Initialize the System scenes.
    //
    for (ProtoSystemPropertiesList::const_iterator systemPropertiesIt = sceneDefinitionProto.systems().begin(); systemPropertiesIt != sceneDefinitionProto.systems().end(); systemPropertiesIt++) {
        m_pSystem = Singletons::SystemManager.Get(systemPropertiesIt->type());
        ASSERTMSG1(m_pSystem != NULL, "Parser was unable to get system %s.", SystemProto::Type_Name(systemPropertiesIt->type()));

        if (m_pSystem != NULL) {
            UScene::SystemScenesConstIt it = m_pScene->GetSystemScenes().find(m_pSystem->GetSystemType());
            ASSERTMSG1(it != m_pScene->GetSystemScenes().end(), "Parser was unable to find a scene for system %s.", SystemProto::Type_Name(systemPropertiesIt->type()));
            m_pSystemScene = it->second;
            ASSERT(m_pSystemScene != NULL);
            // Initialize system scene properties
            m_pSystemScene->setProperties(systemPropertiesIt->properties());
            m_pSystemScene->initialize();
        }
    }

    //
    // Initialize the scene objects.
    //
    for (ProtoObjectList::const_iterator objectsIt = sceneDefinitionProto.objects().begin(); objectsIt != sceneDefinitionProto.objects().end(); objectsIt++) {
        //
        // Create the object.
        //
        m_pUObject = m_pScene->CreateObject(objectsIt->name().c_str());
        ASSERT(m_pUObject != NULL);

        //
        // Added systems extension.
        //
        for (ProtoObjectPropertiesList::const_iterator objectPropertiesIt = objectsIt->systemobjects().begin(); objectPropertiesIt != objectsIt->systemobjects().end(); objectPropertiesIt++) {
            m_pSystem = Singletons::SystemManager.Get(objectPropertiesIt->systemtype());
            ASSERTMSG1(m_pSystem != NULL, "Parser was unable to get system %s.", objectPropertiesIt->systemtype());

            if (m_pSystem != NULL) {
                UScene::SystemScenesConstIt it = m_pScene->GetSystemScenes().find(m_pSystem->GetSystemType());
                ASSERTMSG1(it != m_pScene->GetSystemScenes().end(),
                            "Parser was unable to find a scene for the system %s.", m_pSystem->GetSystemType());
                //
                // Create object.
                //
                m_pSystemObject = m_pUObject->Extend(it->second, objectPropertiesIt->type().c_str());
                ASSERT(m_pSystemObject != NULL);

                if (m_pSystemObject != NULL) {
                    m_pSystemObject->setProperties(objectPropertiesIt->properties());
                    m_pSystemObject->initialize();
                }
            }
        }
    }

    const UScene::SystemScenes Scenes = m_pScene->GetSystemScenes();

    for (UScene::SystemScenesConstIt it = Scenes.begin(); it != Scenes.end(); it++) {
        it->second->GlobalSceneStatusChanged(ISystemScene::PostLoadingObjects);
    }

    //
    // Initialize the links.
    //
    for (ProtoLinksList::const_iterator linksIt = sceneDefinitionProto.links().begin(); linksIt != sceneDefinitionProto.links().end(); linksIt++) {
        UObject* pSubject = m_pScene->FindObject(linksIt->subject().c_str());
        UObject* pObserver = m_pScene->FindObject(linksIt->observer().c_str());;
        std::string sSystemSubject = SystemProto::Type_Name(linksIt->subjectsystemtype());
        std::string sSystemObserver = SystemProto::Type_Name(linksIt->observersystemtype());
        ISystemObject* pSystemSubject = NULL;
        ISystemObject* pSystemObserver = NULL;
        ISystem* pSystem;

        //
        // Get the extension for the subject.
        //
        if (!sSystemSubject.empty()) {
            pSystem = Singletons::SystemManager.Get(sSystemSubject.c_str());
            pSystemSubject = pSubject->GetExtension(pSystem->GetSystemType());
        }

        //
        // Get the extension for the object.
        //
        pSystem = Singletons::SystemManager.Get(sSystemObserver.c_str());
        pSystemObserver = pObserver->GetExtension(pSystem->GetSystemType());

        //
        // Call the scene to register the links.
        //
        if (pSystemSubject != NULL) {
            m_pScene->CreateObjectLink(pSystemSubject, pSystemObserver);
        } else {
            m_pScene->CreateObjectLink(pSubject, pSystemObserver);
        }
    }
}

std::string DefinitionParser::StartupScene(void) {
    return m_gdProto.startupscene();
}
