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

#include "Universal/UScene.h"
#include "Universal/UObject.h"
#include "Object/ISceneObject.h"
#include "Manager/SystemManager.h"

/**
 * @inheritDoc
 */
UScene::UScene(IChangeManager* pSceneCCM, IChangeManager* pObjectCCM) 
    : m_pSceneCCM(pSceneCCM)
    , m_pObjectCCM(pObjectCCM) {

}

/**
 * @inheritDoc
 */
UScene::~UScene(
    void
) {
    //
    // Send "pre-destroying objects" message to the scene extensions.
    //
    for (SystemScenesConstIt it = m_SystemScenes.begin(); it != m_SystemScenes.end(); it++) {
        it->second->GlobalSceneStatusChanged(
            ISystemScene::GlobalSceneStatus::PreDestroyingObjects
        );
    }

    //
    // Get rid of all the links.
    //
    for (auto objectLink : m_ObjectLinks) {
        m_pObjectCCM->Unregister(objectLink.pSubject, objectLink.pObserver);
    }
    m_ObjectLinks.clear();

    //
    // Get rid of all the objects.
    //
    for (auto object : m_Objects) {
        DestroyObject(object);
    }
    m_Objects.clear();

    //
    // Send "post-destroying objects" message to the scene extensions then delete the scene.
    //
    SystemScenes SysScenes = m_SystemScenes;
    for (SystemScenesIt it = SysScenes.begin(); it != SysScenes.end(); it++) {
        ISystemScene* pSystemScene = it->second;
        pSystemScene->GlobalSceneStatusChanged(
            ISystemScene::GlobalSceneStatus::PostDestroyingObjects
        );
        Unextend(pSystemScene);
    }
    m_SystemScenes.clear();
}

/**
 * @inheritDoc
 */
ISystemScene* UScene::Extend(ISystem* pSystem) {
    ASSERT(pSystem != NULL);
    //
    // Get the system's type.
    //
    System::Type SystemType = pSystem->GetSystemType();
    ASSERTMSG(m_SystemScenes.find(SystemType) == m_SystemScenes.end(),
              "The new scene to create for the selected system type already exists.");
    //
    // Have the system create it's scene.
    //
    pSystem->createScene();
    ISystemScene* pScene = pSystem->getSystemScene();
    ASSERT(pScene != NULL);

    if (pScene != NULL) {
        //
        // Create the associated task.
        pScene->createTask();
        //
        // Register all changes made by the scene.
        //
        m_pSceneCCM->Register(pScene, System::Changes::Generic::All, this);
        //
        // Add the scene to the collection.
        //
        m_SystemScenes[SystemType] = pScene;
    }

    return pScene;
}

/**
 * @inheritDoc
 */
Error UScene::Unextend(ISystemScene* pScene) {
    ASSERT(pScene != NULL);
    //
    // Get the system.
    //
    ISystem* pSystem = pScene->GetSystem();
    ASSERT(pSystem != NULL);
    //
    // Get the system's type.
    //
    System::Type SystemType = pSystem->GetSystemType();
    //
    // Find the system scene in the collection and remove it.
    //
    SystemScenesIt it = m_SystemScenes.find(SystemType);
    ASSERTMSG(it != m_SystemScenes.end(),
              "The scene to delete for its system type doesn't exist.");
    m_SystemScenes.erase(it);
    //
    // Unregister the scene from the CCM.
    //
    m_pSceneCCM->Unregister(pScene, this);
    //
    // Call the system to delete it's scene.
    //
    pSystem->DestroyScene(pScene);
    return Errors::Success;
}

/**
 * @inheritDoc
 */
UObject* UScene::createObject(const ObjectProto* objectProto) {
    //
    // Create the new object.
    //
    UObject* pObject = new UObject(this, objectProto->name(), objectProto->name());
    ASSERT(pObject != NULL);
    //
    // Add the object to the collection.
    //
    m_Objects.push_back(pObject);
    Objects objects = m_Objects;

    //
    // Added systems extension.
    //
    for (auto objectProto : objectProto->systemobjects()) {
        ISystem* m_pSystem = Singletons::SystemManager.Get(objectProto.systemtype());
        ASSERTMSG1(m_pSystem != NULL, "Parser was unable to get system %s.", objectProto.systemtype());

        if (m_pSystem != NULL) {
            UScene::SystemScenesConstIt it = GetSystemScenes().find(m_pSystem->GetSystemType());
            ASSERTMSG1(it != GetSystemScenes().end(), "Parser was unable to find a scene for the system %s.", m_pSystem->GetSystemType());
            //
            // Create object.
            //
            ISystemObject* pSystemObject = pObject->Extend(it->second, objectProto.type().c_str());
            ASSERT(pSystemObject != NULL);

            if (pSystemObject != NULL) {
                pSystemObject->setProperties(objectProto.properties());
                pSystemObject->initialize();
            }
        }
    }
    return pObject;
}

/**
 * @inheritDoc
 */
Error UScene::DestroyObject(UObject* pObject) {
    ASSERT(pObject != NULL);
    m_Objects.remove(pObject);
    delete pObject;
    return Errors::Success;
}

/**
 * @inheritDoc
 */
UObject* UScene::FindObject(std::string id) {
    UObject* pObject = NULL;

    for (UScene::ObjectsConstIt it = m_Objects.begin(); it != m_Objects.end(); it++) {
        if (id == (*it)->getId()) {
            pObject = *it;
            break;
        }
    }

    return pObject;
}

/**
 * @inheritDoc
 */
void UScene::CreateObjectLink(ISystemObject* pSubject, ISystemObject* pObserver) {
    //
    // Register objects with the CCM.
    //
    System::Changes::BitMask Changes =
        pSubject->GetPotentialSystemChanges() & pObserver->GetDesiredSystemChanges();

    if (Changes) {
        m_pObjectCCM->Register(pSubject, Changes, pObserver);
        //
        // Hold on to the list for unregistering later.
        //
        ObjectLinkData old = { pSubject, pObserver };
        m_ObjectLinks.push_back(old);
        //
        // Inform the link requester that the link has been established.
        //
        pSubject->PostChanges(System::Changes::Link);
    }
}

/**
 * @inheritDoc
 */
void UScene::CreateObjectLink(UObject* pSubject, ISystemObject* pObserver) {
    //
    // Register objects with the CCM.
    //
    System::Changes::BitMask Changes =
        pSubject->GetPotentialSystemChanges() & pObserver->GetDesiredSystemChanges();

    if (Changes) {
        m_pObjectCCM->Register(pSubject, Changes, pObserver);
        //
        // Hold on to the list for unregistering later.
        //
        ObjectLinkData old = { pSubject, pObserver };
        m_ObjectLinks.push_back(old);
        //
        // Inform the link requester that the link has been established.
        //
        pSubject->PostChanges(System::Changes::ParentLink);
    }
}

/**
 * @inheritDoc
 */
Error UScene::ChangeOccurred(ISubject* pSubject, System::Changes::BitMask ChangeType) {
    switch (ChangeType) {
        case System::Changes::Generic::CreateObject: {
            ISceneObject* pScene = dynamic_cast<ISceneObject*>(pSubject);
            const ISceneObject::ObjectProtoQueue objectsToCreate = *pScene->getCreateObjects();
            for (auto objectProto : objectsToCreate) {
                ASSERT(FindObject(objectProto.name().c_str()) == NULL);
                UObject* pObject = createObject(&objectProto);
                ASSERT(pObject != NULL);
            }
            break;
        }

        case System::Changes::Generic::DeleteObject: {
            ISceneObject* pScene = dynamic_cast<ISceneObject*>(pSubject);
            const ISceneObject::ObjectProtoQueue objectsToDestroy = *pScene->getDeleteObjects();
            for (auto objectProto : objectsToDestroy) {
                UObject* pObject = FindObject(objectProto.name().c_str());
                ASSERT(pObject != NULL);
                DestroyObject(pObject);
            }
            break;
        }
    }

    return Errors::Success;
}
