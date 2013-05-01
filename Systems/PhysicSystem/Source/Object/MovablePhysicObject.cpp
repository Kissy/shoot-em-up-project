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

#include "Scene.h"
#include "PhysicObject.h"
#include "Object/MovablePhysicObject.h"

const int MovablePhysicObject::m_speed_multiplier = 4;

/**
 * @inheritDoc
 */
MovablePhysicObject::MovablePhysicObject(ISystemScene* pSystemScene, const char* pszName) : PhysicObject(pSystemScene, pszName) {
    
}

/**
 * @inheritDoc
 */
MovablePhysicObject::~MovablePhysicObject(void) {
    
}

/**
 * @inheritDoc
 */
Error MovablePhysicObject::initialize(void) {
    ASSERT(!m_bInitialized);
    
    m_bInitialized = true;
    return Errors::Success;
}

/**
 * @inheritDoc
 */
Error MovablePhysicObject::ChangeOccurred(ISubject* pSubject, System::Changes::BitMask ChangeType) {
    ASSERT(m_bInitialized);

    if (ChangeType & System::Changes::Input::Velocity) {
        const Math::Vector3* velocity = dynamic_cast<IMoveObject*>(pSubject)->GetVelocity();
        m_velocity.x = velocity->x;
        m_velocity.y = velocity->y;
        m_velocity.z = velocity->z;
    }
    if (ChangeType & System::Changes::Physic::Position) {
        const Math::Vector3* position = dynamic_cast<IGeometryObject*>(pSubject)->GetPosition();
        m_position.x = position->x;
        m_position.y = position->y;
        m_position.z = position->z;
    }

    return Errors::Success;
}

/**
 * @inheritDoc
 */
void MovablePhysicObject::Update(f32 DeltaTime) {
    ASSERT(m_bInitialized);

    if (m_velocity != Math::Vector3::Zero) {
        m_position.x += m_velocity.x * m_speed_multiplier;
        m_position.y += m_velocity.y * m_speed_multiplier;
        m_position.z += m_velocity.z * m_speed_multiplier;
        PostChanges(System::Changes::Physic::Position);
    }
}