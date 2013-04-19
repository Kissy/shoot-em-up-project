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
PlayerNetworkObject::PlayerNetworkObject(ISystemScene* pSystemScene, const char* pszName) : NetworkObject(pSystemScene, pszName)
    , m_position(Math::Vector3::Zero)
    , m_velocity(Math::Vector3::Zero)
    , m_heartbeat_delay(1000000000LL) /* 1s */ {
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
void PlayerNetworkObject::Update(f32 DeltaTime) {
    ASSERT(m_bInitialized);

    if (m_velocity != Math::Vector3::Zero) {
        if (m_heartbeat.is_stopped() || m_heartbeat.elapsed().wall >= m_heartbeat_delay) {
            m_heartbeat.stop();
            m_heartbeat.start();

            ObjectUpdatedProto objectUpdatedProto;
            ObjectProto* object = objectUpdatedProto.add_objects();
            object->set_name(GetName());
            ObjectProto_SystemObjectProto* systemObject = object->add_systemobjects();
            systemObject->set_systemtype(SystemProto_Type_Geometry);
            PropertyProto* velocityProperty = systemObject->add_properties();
            velocityProperty->set_name("Velocity");
            velocityProperty->add_value(boost::lexical_cast<std::string>(m_velocity.x));
            velocityProperty->add_value(boost::lexical_cast<std::string>(m_velocity.y));
            velocityProperty->add_value(boost::lexical_cast<std::string>(m_velocity.z));
            PropertyProto* positionProperty = systemObject->add_properties();
            positionProperty->set_name("Position");
            positionProperty->add_value(boost::lexical_cast<char*>(m_position.x));
            positionProperty->add_value(boost::lexical_cast<char*>(m_position.y));
            positionProperty->add_value(boost::lexical_cast<char*>(m_position.z));

            std::string data;
            objectUpdatedProto.AppendToString(&data);
            DownstreamMessageProto* downstreamMessageProto = new DownstreamMessageProto();
            downstreamMessageProto->set_type(DownstreamMessageProto::PLAYER_MOVE);
            downstreamMessageProto->set_data(data);
            reinterpret_cast<NetworkTask*>(GetSystemScene()->GetSystemTask())->queueMessage(downstreamMessageProto);
        }
    } else if (!m_heartbeat.is_stopped()) {
        m_heartbeat.stop();
    }
}