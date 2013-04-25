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

#pragma once

#include "Observer/IObserver.h"
#include "System/ISystemScene.h"

#include "Object/IGeometryObject.h"

class UObject;
class IChangeManager;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>
/// Implements a universal scene for holding all the scenes of the different systems and acts as an
//   interface into the CMM.
/// </summary>
////////////////////////////////////////////////////////////////////////////////////////////////////
class UScene : public IObserver {
public:

    // TODO move
    typedef std::map<System::Type, ISystem*>                            Systems;
    typedef Systems::iterator                                           SystemsIt;
    typedef Systems::const_iterator                                     SystemsConstIt;

    typedef std::map<System::Type, ISystemScene*>                       SystemScenes;
    typedef SystemScenes::iterator                                      SystemScenesIt;
    typedef SystemScenes::const_iterator                                SystemScenesConstIt;

    typedef std::list<UObject*>                                         Objects;
    typedef Objects::iterator                                           ObjectsIt;
    typedef Objects::const_iterator                                     ObjectsConstIt;

    struct ObjectLinkData {
        ISubject*               pSubject;
        IObserver*              pObserver;
    };
    typedef std::list<ObjectLinkData>       ObjectLinks;
    typedef ObjectLinks::iterator           ObjectLinksIt;

public:

    /// <summary>
    ///   Constructor.
    /// </summary>
    UScene(IChangeManager* pUSceneCCM, IChangeManager* pUObjectCCM);

    /// <summary>
    ///   Destructor.
    /// </summary>
    ~UScene(void);

    /// <summary>
    ///   Creates a new ISystemScene for the passed in ISystem.
    /// </summary>
    /// <param name="pSystem">The system that the scene will be created from.</param>
    /// <returns>The newly ISystemScene.</returns>
    ISystemScene* Extend(ISystem* pSystem);

    /// <summary>
    ///   Destroys a ISystemScene removing it from the UScene.
    /// </summary>
    /// <param name="pScene">The system scene to destroy.</param>
    /// <returns>An error code.</returns>
    Error Unextend(ISystemScene* pScene);

    /// <summary>
    ///   Returns a collection of system scenes.
    /// </summary>
    /// <returns>The system scene collection.</returns>
    const SystemScenes& GetSystemScenes(void) const {
        return m_SystemScenes;
    }

    /**
     * Creates a new UObject that gets attached to this scene.
     * The object will be extended for each systems.
     *
     * @param   objectProto The object definition to create.
     *
     * @return  The newly created object that has consequently been added to the scene.
     */
    UObject* createObject(const ObjectProto* objectProto);

    /// <summary>
    ///   Destroys a UObject removing it from the scene.  It also deletes it's CCM.
    /// </summary>
    /// <param name="pObject">The object to remove from the scene and destroy.</param>
    /// <returns>An error code.</returns>
    Error DestroyObject(UObject* pObject);

    /// <summary>
    ///   Returns a collection of system scenes.
    /// </summary>
    /// <returns>The system scene collection.</returns>
    const Objects& GetObjects(void) const {
        return m_Objects;
    }

    UObject* FindObject(const char* pszName);

    void CreateObjectLink(ISystemObject* pSubject, ISystemObject* pObserver);

    void CreateObjectLink(UObject* pSubject, ISystemObject* pObserver);

    /// <summary cref="IObserver::ChangeOccurred">
    ///   Implementation of the IObserver ChangeOccurred function.
    /// </summary>
    virtual Error ChangeOccurred(ISubject* pSubject, System::Changes::BitMask SystemChanges);

protected:

    IChangeManager*                         m_pSceneCCM;
    IChangeManager*                         m_pObjectCCM;

    SystemScenes                            m_SystemScenes;
    Objects                                 m_Objects;
    ObjectLinks                             m_ObjectLinks;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>
/// Implements a universal object that can be extended to add functionality.  By
/// default this universal object does not have any system functionality.
/// </summary>
////////////////////////////////////////////////////////////////////////////////////////////////////

class UObject : public ISubject, public IObserver, public IGeometryObject {
    friend class UScene;

protected:

    //
    // Constructor.
    //  pszName - the name of this object.
    //
    UObject(UScene* pScene, const char* pszName = "");

    //
    // Destructor.
    //
    ~UObject(void);

public:

    typedef std::map<System::Type, ISystemObject*>      SystemObjects;
    typedef SystemObjects::iterator                     SystemObjectsIt;
    typedef SystemObjects::const_iterator               SystemObjectsConstIt;

    //
    // Gets the name of the object.
    //   return - the name of the object.
    //
    const char* GetName(void) {
        return m_sName.c_str();
    }

    //
    // Sets the name of the object.
    //  pszName - the new name of the object.
    //
    void SetName(const char* pszName) {
        m_sName = pszName;
    }

    //
    // Used to extend the objects functionality for a given system.
    //   return - the newly created system object.
    //
    ISystemObject* Extend(ISystemScene* pSystemScene, const char* pszSystemObjectType);

    /// <summary>
    ///   Used to extend the objects functionality for a given system.
    /// </summary>
    /// <param name="pSystemObject">A pointer to an already created object.</param>
    /// <returns>true is successful, false otherwise.</returns>
    bool Extend(ISystemObject* pSystemObject);

    //
    // Used to unextend the objects functionality for a given system.
    //
    void Unextend(ISystemScene* pSystemScene);

    /// <summary>
    ///   Returns the system objects map for this UObject.
    /// </summary>
    /// <returns>A constant reference to the system object mapping.</returns>
    const SystemObjects& GetExtensions(void);

    //
    // Gets the specified system object.
    //  System - the system type of the object.
    //   return - this UOBject's system object for the specified type.
    //
    ISystemObject* GetExtension(System::Type System);

    /// <summary cref="IObserver::ChangeOccurred">
    ///   Implementation of the IObserver ChangeOccurred function.
    /// </summary>
    virtual Error ChangeOccurred(ISubject* pSubject, System::Changes::BitMask SystemChanges);

    /// <summary cref="ISubject::GetPotentialSystemChanges">
    ///   Implementation of the ISubject GetPotentialSystemChanges function.
    /// </summary>
    virtual System::Changes::BitMask GetPotentialSystemChanges(void);

    /// <summary cref="IGeometryObject::GetPosition">
    ///   Implementation of the IGeometryObject GetPosition function.
    /// </summary>
    virtual const Math::Vector3* GetPosition(void);

    /// <summary cref="IGeometryObject::GetOrientation">
    ///   Implementation of the IGeometryObject GetOrientation function.
    /// </summary>
    virtual const Math::Quaternion* GetOrientation(void);

    /// <summary cref="IGeometryObject::GetScale">
    ///   Implementation of the IGeometryObject GetScale function.
    /// </summary>
    virtual const Math::Vector3* GetScale(void);

protected:

    UScene*                                             m_pScene;
    IChangeManager*                                     m_pObjectCCM;
    std::string                                         m_sName;

    SystemObjects                                       m_ObjectExtensions;
    IGeometryObject*                                    m_pGeometryObject;
};
