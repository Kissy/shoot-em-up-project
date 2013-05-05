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

#include <boost/lexical_cast.hpp>

#include "BaseTypes.h"
#include "Interface.h"

#include "Scene.h"
#include "Object/Object.h"
#include "Object/PlayerNetworkObject.h"
#include "Proto/Server/DownstreamMessage.pb.h"
#include "Proto/Message/ObjectUpdated.pb.h"

/**
 * @inheritDoc
 */
PlayerNetworkObject::PlayerNetworkObject(ISystemScene* pSystemScene, const char* pszName) 
    : NetworkObject(pSystemScene, pszName)
    , m_dirty(true)
    , m_velocity(Math::Vector4::Zero)
    , m_orientation(Math::Quaternion::Zero)
    , m_heartbeat_delay(500000000LL) /* 500ms */ {
    m_heartbeat.stop();
}

/**
 * @inheritDoc
 */
PlayerNetworkObject::~PlayerNetworkObject(void) {
    
}

/**
 * @inheritDoc
 */
Error PlayerNetworkObject::initialize(void) {
    ASSERT(!m_bInitialized);
    
    m_bInitialized = true;
    return Errors::Success;
}

/**
 * @inheritDoc
 */
Error PlayerNetworkObject::ChangeOccurred(ISubject* pSubject, System::Changes::BitMask ChangeType) {
    ASSERT(m_bInitialized);

    if (ChangeType & System::Changes::Physic::Velocity) {
        m_velocity = *dynamic_cast<IMoveObject*>(pSubject)->getVelocity();
        m_dirty = true;
    }
    if (ChangeType & System::Changes::Physic::Position) {
        m_position = *dynamic_cast<IGeometryObject*>(pSubject)->GetPosition();
    }
    if (ChangeType & System::Changes::Physic::Orientation) {
        m_orientation = *dynamic_cast<IGeometryObject*>(pSubject)->GetOrientation();
        m_dirty = true;
    }

    return Errors::Success;
}

/**
 * @inheritDoc
 */
void PlayerNetworkObject::Update(f32 DeltaTime) {
    ASSERT(m_bInitialized);

    // Send the packet everytime it's dirty or for a heartbeat
    bool heartbeat_triggered = !m_heartbeat.is_stopped() && m_heartbeat.elapsed().wall >= m_heartbeat_delay;
    if (m_dirty || heartbeat_triggered) {
        m_dirty = false;
        m_heartbeat.stop();
        if (m_velocity != Math::Vector4::Zero) {
            m_heartbeat.start();
        }
        
        ObjectUpdatedProto objectUpdatedProto;
        ObjectProto* object = objectUpdatedProto.add_objects();
        object->set_name(GetName());
        ObjectProto_SystemObjectProto* systemObject = object->add_systemobjects();
        systemObject->set_systemtype(SystemProto_Type_Geometry);
        PropertyProto* velocityProperty = systemObject->add_properties();
        velocityProperty->set_name("Velocity");
        getVector4(&m_velocity, velocityProperty->mutable_value());
        PropertyProto* orientationProperty = systemObject->add_properties();
        orientationProperty->set_name("Orientation");
        getQuaternion(&m_orientation, orientationProperty->mutable_value());
        PropertyProto* positionProperty = systemObject->add_properties();
        positionProperty->set_name("Position");
        getVector3(&m_position, positionProperty->mutable_value());

        std::string data;
        objectUpdatedProto.AppendToString(&data);
        DownstreamMessageProto downstreamMessageProto;
        downstreamMessageProto.set_type(DownstreamMessageProto::PLAYER_MOVE);
        downstreamMessageProto.set_data(data);
        reinterpret_cast<NetworkSystem*>(GetSystemScene()->GetSystem())->getNetworkService()->send(downstreamMessageProto);
    }
}