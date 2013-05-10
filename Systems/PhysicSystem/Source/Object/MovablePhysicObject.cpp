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
#include "Generic/IEntity.h"
#include "Object/MovablePhysicObject.h"

const f32 MovablePhysicObject::m_speed_multiplier = 500;

/**
 * @inheritDoc
 */
MovablePhysicObject::MovablePhysicObject(ISystemScene* pSystemScene, IEntity* entity) 
    : PhysicObject(pSystemScene, entity) {
    
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

    if (ChangeType & System::Changes::Physic::Velocity) {
        m_velocity = *dynamic_cast<IMoveObject*>(pSubject)->getVelocity();
    }
    if (ChangeType & System::Changes::Physic::Position) {
        m_position = *dynamic_cast<IGeometryObject*>(pSubject)->GetPosition();
    }
    if (ChangeType & System::Changes::Physic::Orientation) {
        m_orientation = *dynamic_cast<IGeometryObject*>(pSubject)->GetOrientation();
    }

    return Errors::Success;
}

/**
 * @inheritDoc
 */
void MovablePhysicObject::Update(f32 DeltaTime) {
    ASSERT(m_bInitialized);
    u32 modified = 0;

    if (m_velocity != Math::Vector4::Zero) {
        // Rotation
        if (m_velocity.w != 0) {
            m_rotation += m_velocity.w * m_speed_multiplier * DeltaTime;
            m_rotation = fmod(m_rotation, 360.0f);
            if(m_rotation < 0) {
                m_rotation += 360.0f;
            }
            m_orientation.Set(Math::Vector3::UnitZ, Math::Angle::Deg2Rad(m_rotation));
            modified |= System::Changes::Physic::Orientation;
        }

        // Position
        Math::Vector3 normalizedVelocity = Math::Vector3(m_velocity.x, m_velocity.y, m_velocity.z);
        if (normalizedVelocity != Math::Vector3::Zero) {
            m_orientation.Rotate(normalizedVelocity);
            normalizedVelocity.Normalize();
            m_position.x += normalizedVelocity.x * m_speed_multiplier * DeltaTime;
            m_position.y += normalizedVelocity.y * m_speed_multiplier * DeltaTime;
            m_position.z += normalizedVelocity.z * m_speed_multiplier * DeltaTime;

            m_position.x = m_position.x > 780.0f ? 780.0f : (m_position.x < 40.0f ? 40.0f : m_position.x);
            m_position.y = m_position.y > 570.0f ? 570.0f : (m_position.y < 40.0f ? 40.0f : m_position.y);
        
            modified |= System::Changes::Physic::Position;
        }
    }

    PostChanges(modified);
}