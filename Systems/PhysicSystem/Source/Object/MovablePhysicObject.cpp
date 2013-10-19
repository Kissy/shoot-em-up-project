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

#include <boost/bind.hpp>

#include "Scene.h"
#include "PhysicObject.h"
#include "Generic/IEntity.h"
#include "Object/MovablePhysicObject.h"

/**
 * @inheritDoc
 */
MovablePhysicObject::MovablePhysicObject(ISystemScene* pSystemScene, IEntity* entity) 
    : PhysicObject(pSystemScene, entity)
    , m_constraint_position(true)
    , m_speed_multiplier(40) {
    m_propertySetters["SpeedMultiplier"] = boost::bind(&IProperty::setSimpleType<f32>, this, System::Changes::None, &m_speed_multiplier, _1);
    m_propertyGetters["SpeedMultiplier"] = boost::bind(&IProperty::getSimpleType<f32>, this, &m_speed_multiplier, _1);

    m_propertySetters["ConstraintPosition"] = boost::bind(&IProperty::setSimpleType<bool>, this, System::Changes::None, &m_constraint_position, _1);
    m_propertyGetters["ConstraintPosition"] = boost::bind(&IProperty::getSimpleType<bool>, this, &m_constraint_position, _1);
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
            m_rotation = m_velocity.w * m_speed_multiplier * DeltaTime * 10;
            Math::Quaternion additionalOrientation;
            additionalOrientation.Set(Math::Vector3::UnitZ, Math::Angle::Deg2Rad(m_rotation));
            m_orientation *= additionalOrientation;
            modified |= System::Changes::Physic::Orientation;
        }

        // Position
        if (m_velocity != Math::Vector3::Zero) {
            Math::Vector3 normalizedVelocity = Math::Vector3(m_velocity.x, m_velocity.y, m_velocity.z);
            m_orientation.Rotate(normalizedVelocity);
            normalizedVelocity.Normalize();
            m_position.x += normalizedVelocity.x * m_speed_multiplier * DeltaTime;
            m_position.y += normalizedVelocity.y * m_speed_multiplier * DeltaTime;
            m_position.z += normalizedVelocity.z * m_speed_multiplier * DeltaTime;

            m_position.x = (m_constraint_position && m_position.x > 35) ? 35 : ((m_constraint_position && m_position.x < -35) ? -35 : m_position.x);
            m_position.y = (m_constraint_position && m_position.y > 25) ? 25 : ((m_constraint_position && m_position.y < -25) ? -25 : m_position.y);
            
            modified |= System::Changes::Physic::Position;
        }
    }

    PostChanges(modified);
}