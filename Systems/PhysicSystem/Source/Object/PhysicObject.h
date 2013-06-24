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

#pragma once

#include "System/ISystemScene.h"
#include "System/ISystemObject.h"
#include "Object/IGeometryObject.h"
#include "Object/IMoveObject.h"
#include "Generic/IEntity.h"

class HavokPhysicsSystem;
class HavokPhysicsScene;
class HavokPhysicsTask;

///////////////////////////////////////////////////////////////////////////////
/// <summary>
///   <c>HavokObject</c> Implementation of the ISystemObject interface.
///   This is the base object created by the HavokPhysics Scene.
/// </summary>
///////////////////////////////////////////////////////////////////////////////

class PhysicObject : public ISystemObject, public IGeometryObject, public IMoveObject {
public:

    /**
     * @inheritDoc
     */
    PhysicObject(ISystemScene* pSystemScene, IEntity* entity);

    /**
     * @inheritDoc
     */
    virtual ~PhysicObject(void);

    /**
     * @inheritDoc
     */
    inline Proto::SystemType GetSystemType(void) {
        return Proto::SystemType::Physic;
    }

protected:
    f32                 m_rotation;

};
