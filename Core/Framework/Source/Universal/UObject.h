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

class IChangeManager;
class UScene;

/**
 * Implements a universal object that can be extended to add functionality.  By default this
 * universal object does not have any system functionality.
 *
 * @sa  ISubject
 * @sa  IObserver
 * @sa  IGeometryObject
 */
class UObject : public ISubject, public IObserver {
public:

    /**
     * Constructor.
     *  pszName - the name of this object.
     *
     * @param [in,out]  pScene  If non-null, the scene.
     * @param   pszName         (Optional) the name.
     */
    UObject(UScene* pScene, const char* pszName = "");

    /**
     * Destructor.
     */
    ~UObject(void);

    typedef std::map<System::Type, ISystemObject*>      SystemObjects;
    typedef SystemObjects::iterator                     SystemObjectsIt;
    typedef SystemObjects::const_iterator               SystemObjectsConstIt;

    /**
     * Gets the name of the object.
     *   return - the name of the object.
     *
     * @return  null if it fails, else the name.
     */
    const char* GetName(void) {
        return m_sName.c_str();
    }

    /**
     * Sets the name of the object.
     *  pszName - the new name of the object.
     *
     * @param   pszName The name.
     */
    void SetName(const char* pszName) {
        m_sName = pszName;
    }

    /**
     * Used to extend the objects functionality for a given system.
     *   return - the newly created system object.
     *
     * @param [in,out]  pSystemScene    If non-null, the system scene.
     * @param   pszSystemObjectType     Type of the system object.
     *
     * @return  null if it fails, else.
     */
    ISystemObject* Extend(ISystemScene* pSystemScene, const char* pszSystemObjectType);

    /**
     * Used to extend the objects functionality for a given system.
     *
     * @param [in,out]  pSystemObject   A pointer to an already created object.
     *
     * @return  true is successful, false otherwise.
     */
    bool Extend(ISystemObject* pSystemObject);

    /**
     * Used to unextend the objects functionality for a given system.
     *
     * @param [in,out]  pSystemScene    If non-null, the system scene.
     */
    void Unextend(ISystemScene* pSystemScene);

    /**
     * Returns the system objects map for this UObject.
     *
     * @return  A constant reference to the system object mapping.
     */
    const SystemObjects& GetExtensions(void);

    /**
     * Gets the specified system object.
     *  System - the system type of the object.
     *   return - this UOBject's system object for the specified type.
     *
     * @param   System  The system.
     *
     * @return  null if it fails, else the extension.
     */
    ISystemObject* GetExtension(System::Type System);

    /**
     * Updates the object using an object proto.
     *
     * @param   objectProto The object prototype.
     */
    // TODO maybe move it to system scene instead ?
    void update(const ObjectProto* objectProto);

    /**
     * Change occurred.
     * Implementation of the IObserver ChangeOccurred function.
     *
     * @param [in,out]  pSubject    If non-null, the subject.
     * @param   SystemChanges       The system changes.
     *
     * @return  .
     */
    virtual Error ChangeOccurred(ISubject* pSubject, System::Changes::BitMask SystemChanges);

    /**
     * Gets potential system changes.
     * Implementation of the ISubject GetPotentialSystemChanges function.
     *
     * @return  The potential system changes.
     */
    virtual System::Changes::BitMask GetPotentialSystemChanges(void);

    /**
     * Sets object ccm.
     *
     * @param [in,out]  objectCCM   If non-null, the object ccm.
     */
    inline void setObjectCCM(IChangeManager* objectCCM) {
        m_pObjectCCM = objectCCM;
    }

protected:
    IChangeManager*                                     m_pObjectCCM;

    UScene*                                             m_pScene;
    std::string                                         m_sName;
    SystemObjects                                       m_ObjectExtensions;

};
