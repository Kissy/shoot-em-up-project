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

#include "Universal.h"
#include "Object/ISceneObject.h"
#include "Manager/SystemManager.h"


////////////////////////////////////////////////////////////////////////////////////////////////
// UScene
////////////////////////////////////////////////////////////////////////////////////////////////

UScene::UScene(IChangeManager* pSceneCCM, IChangeManager* pObjectCCM) 
    : m_pSceneCCM(pSceneCCM)
    , m_pObjectCCM(pObjectCCM) {

}


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
    for (ObjectLinksIt it = m_ObjectLinks.begin(); it != m_ObjectLinks.end(); it++) {
        const ObjectLinkData& old = *it;
        m_pObjectCCM->Unregister(old.pSubject, old.pObserver);
    }

    m_ObjectLinks.clear();
    //
    // Get rid of all the objects.
    //
    Objects Objs = m_Objects;

    for (ObjectsIt it = Objs.begin(); it != Objs.end(); it++) {
        DestroyObject(*it);
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


UObject* UScene::createObject(const ObjectProto* objectProto) {
    //
    // Create the new object.
    //
    UObject* pObject = new UObject(this, objectProto->name().c_str());
    ASSERT(pObject != NULL);
    pObject->m_pObjectCCM = m_pObjectCCM;
    //
    // Add the object to the collection.
    //
    m_Objects.push_back(pObject);
    //
    // Added systems extension.
    //
    for (ObjectProto_SystemObjectProto objectProto : objectProto->systemobjects()) {
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


Error UScene::DestroyObject(UObject* pObject) {
    ASSERT(pObject != NULL);
    m_Objects.remove(pObject);
    delete pObject;
    return Errors::Success;
}


UObject* UScene::FindObject(const char* pszName) {
    UObject* pObject = NULL;

    for (UScene::ObjectsConstIt it = m_Objects.begin(); it != m_Objects.end(); it++) {
        if (strcmp(pszName, (*it)->GetName()) == 0) {
            pObject = *it;
            break;
        }
    }

    return pObject;
}


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


Error UScene::ChangeOccurred(ISubject* pSubject, System::Changes::BitMask ChangeType) {
    switch (ChangeType) {
        case System::Changes::Generic::CreateObject: {
            ISceneObject* pScene = dynamic_cast<ISceneObject*>(pSubject);
            ISceneObject::ObjectProtoQueue* objectsToCreate = pScene->getCreateObjects();
            while (!objectsToCreate->empty()) {
                const ObjectProto objectProto = objectsToCreate->back();
                ASSERT(FindObject(objectProto.name().c_str()) == NULL);
                UObject* pObject = createObject(&objectProto);
                ASSERT(pObject != NULL);
                objectsToCreate->pop();
            }
            break;
        }

        case System::Changes::Generic::DeleteObject: {
            ISceneObject* pScene = dynamic_cast<ISceneObject*>(pSubject);
            ISceneObject::ObjectProtoQueue* objectsToDestroy = pScene->getDeleteObjects();
            while (!objectsToDestroy->empty()) {
                const ObjectProto objectProto = objectsToDestroy->back();
                UObject* pObject = FindObject(objectProto.name().c_str());
                ASSERT(pObject != NULL);
                DestroyObject(pObject);
                objectsToDestroy->pop();
            }
            break;
        }
    }

    return Errors::Success;
}


////////////////////////////////////////////////////////////////////////////////////////////////
// UObject
////////////////////////////////////////////////////////////////////////////////////////////////

UObject::UObject(UScene* pScene, const char* pszName)
    : m_pScene(pScene)
    , m_pGeometryObject(NULL) {
    //
    // Copy the name.
    //
    SetName(pszName);
}


UObject::~UObject(void) {
    //
    // Remove all the extensions.
    //
    SystemObjects SysObjs = m_ObjectExtensions;

    for (SystemObjectsIt it = SysObjs.begin(); it != SysObjs.end(); it++) {
        Unextend(it->second->GetSystemScene());
    }

    m_ObjectExtensions.clear();
}


ISystemObject* UObject::Extend(ISystemScene* pSystemScene, const char* pszSystemObjectType) {
    ASSERT(pSystemScene != NULL);
    ASSERT(m_ObjectExtensions.find(pSystemScene->GetSystemType()) == m_ObjectExtensions.end());
    ISystemObject* pSystemObject = NULL;
    //
    // Create the system object.
    //
    pSystemObject = pSystemScene->CreateObject(m_sName.c_str(), pszSystemObjectType);
    ASSERT(pSystemObject != NULL);
    Extend(pSystemObject);
    return pSystemObject;
}


bool UObject::Extend(ISystemObject* pSystemObject) {
    // 
    // If the object is already extended, do nothing
    // 
    if (m_ObjectExtensions.find(pSystemObject->GetSystemType()) != m_ObjectExtensions.end()) {
        return false;
    }

    //
    // Set this as the parent.
    //
    pSystemObject->SetParentObject(this);

    //
    // Get the changes this object will make and is looking for.
    //
    System::Changes::BitMask SysObjPotentialChanges = pSystemObject->GetPotentialSystemChanges();
    System::Changes::BitMask SysObjDesiredChanges = pSystemObject->GetDesiredSystemChanges();;

    //
    // Register each object with scenes that care about the object's changes.
    //
    UScene::SystemScenes pScenes = m_pScene->GetSystemScenes();
    for (UScene::SystemScenesIt it = pScenes.begin(); it != pScenes.end(); it++) {
        ISystemScene* pScene = it->second;

        if (pSystemObject->GetPotentialSystemChanges() & pScene->GetDesiredSystemChanges()) {
            m_pObjectCCM->Register(pSystemObject, pScene->GetDesiredSystemChanges(), pScene);
        }
    }

    //
    // Register each of the systems with each other.
    //
    System::Changes::BitMask Changes = pSystemObject->GetDesiredSystemChanges();
    for (std::map<System::Type, ISystemObject*>::iterator it = m_ObjectExtensions.begin();
            it != m_ObjectExtensions.end(); it++) {
        ISystemObject* pObj = it->second;

        if (pObj->GetPotentialSystemChanges() & SysObjDesiredChanges) {
            m_pObjectCCM->Register(pObj, Changes, pSystemObject);
        }

        if (SysObjPotentialChanges & pObj->GetDesiredSystemChanges()) {
            m_pObjectCCM->Register(pSystemObject, pObj->GetDesiredSystemChanges(), pObj);
        }
    }

    //
    // Add the system object to the list.
    //
    System::Type SystemType = pSystemObject->GetSystemType();
    m_ObjectExtensions[ SystemType ] = pSystemObject;

    //
    // Set up the speed path for the geometry and graphics objects.
    //
    if (SystemType == System::Types::Physic) {
        m_pGeometryObject = dynamic_cast<IGeometryObject*>(pSystemObject);
        ASSERT(m_pGeometryObject != NULL);
    }

    return true;
}


void UObject::Unextend(ISystemScene* pSystemScene) {
    ASSERT(pSystemScene != NULL);
    //
    // Get the iterator for the object.
    //
    System::Type SystemType = pSystemScene->GetSystem()->GetSystemType();
    SystemObjectsIt SysObjIt = m_ObjectExtensions.find(SystemType);
    ASSERTMSG(SysObjIt != m_ObjectExtensions.end(),
              "The object to delete doesn't exist in the scene.");
    ISystemObject* pSystemObject = SysObjIt->second;

    //
    // Go through all the other systems and unregister them with this as subject and observer.
    //  The CCM should know if the objects are registered or not, and if not won't do anything.
    //
    for (std::map<System::Type, ISystemObject*>::iterator it = m_ObjectExtensions.begin();
            it != m_ObjectExtensions.end(); it++) {
        ISystemObject* pObj = it->second;

        //
        // Make sure he system object is not unregistering as an observer of itself.
        //
        if (pSystemObject != pObj) {
            m_pObjectCCM->Unregister(pObj, pSystemObject);
            m_pObjectCCM->Unregister(pSystemObject, pObj);
        }
    }

    //
    // Unregister each object with scenes that cared about the object's changes.
    //
    UScene::SystemScenes pScenes = m_pScene->GetSystemScenes();
    for (UScene::SystemScenesIt it = pScenes.begin(); it != pScenes.end(); it++) {
        ISystemScene* pScene = it->second;

        if (pSystemObject->GetPotentialSystemChanges() & pScene->GetDesiredSystemChanges()) {
            m_pObjectCCM->Unregister(pSystemObject,  pScene);
        }
    }

    //
    // Destroy the object.
    //
    pSystemScene->DestroyObject(pSystemObject);
    //
    // Remove the object from the map.
    //
    m_ObjectExtensions.erase(SysObjIt);
}


const UObject::SystemObjects& UObject::GetExtensions(void) {
    return m_ObjectExtensions;
}


ISystemObject* UObject::GetExtension(System::Type SystemType) {
    ISystemObject* pSystemObject = nullptr;

    SystemObjectsConstIt it = m_ObjectExtensions.find(SystemType);
    if (it != m_ObjectExtensions.end()) {
        pSystemObject = it->second;
    }

    return pSystemObject;
}

/**
 * @inheritDoc
 */
void UObject::update(const ObjectProto* objectProto) {
    //
    // Update systems extension.
    //
    for (auto objectProto : objectProto->systemobjects()) {
        ISystem* m_pSystem = Singletons::SystemManager.Get(objectProto.systemtype());
        ASSERTMSG1(m_pSystem != NULL, "Unable to get system %s.", objectProto.systemtype());

        if (m_pSystem != NULL) {
            ISystemObject* systemObject = GetExtension(m_pSystem->GetSystemType());
            ASSERTMSG1(systemObject != nullptr, "Unable to find a scene for the system %s.", m_pSystem->GetSystemType());
            systemObject->setProperties(objectProto.properties());
        }
    }
}

System::Changes::BitMask UObject::GetPotentialSystemChanges(void) {
    return System::Changes::Generic::All | System::Changes::Physic::Position;
}


Error UObject::ChangeOccurred(ISubject* pSubject, System::Changes::BitMask ChangeType) {
    UNREFERENCED_PARAM(pSubject);

    if (ChangeType & (System::Changes::Generic::All | System::Changes::Physic::Position)) {
        ChangeType &= (System::Changes::Generic::All | System::Changes::Physic::Position);
        //
        // Post the pertinent changes made by the extension to the scene CCM.
        //
        PostChanges(ChangeType);
    }

    return Errors::Success;
}


const Math::Vector3*
UObject::GetPosition(
    void
) {
    ASSERT(m_pGeometryObject != NULL);
    return m_pGeometryObject->GetPosition();
}


const Math::Quaternion*
UObject::GetOrientation(
    void
) {
    ASSERT(m_pGeometryObject != NULL);
    return m_pGeometryObject->GetOrientation();
}


const Math::Vector3*
UObject::GetScale(
    void
) {
    ASSERT(m_pGeometryObject != NULL);
    return m_pGeometryObject->GetScale();
}
