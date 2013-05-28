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

#include "Interface.h"

#include "Scene.h"
#include "Object/Object.h"
#include "Object/ReplicableNetworkObject.h"
#include "Proto/Server/DownstreamMessage.pb.h"
#include "Proto/Message.pb.h"

/**
 * @inheritDoc
 */
ReplicableNetworkObject::ReplicableNetworkObject(ISystemScene* pSystemScene, IEntity* entity) 
    : NetworkObject(pSystemScene, entity)
    , m_dirty(true)
    , m_velocity(Math::Vector4::Zero)
    , m_orientation(Math::Quaternion::Zero) {
}

/**
 * @inheritDoc
 */
ReplicableNetworkObject::~ReplicableNetworkObject(void) {
    
}

/**
 * @inheritDoc
 */
Error ReplicableNetworkObject::initialize(void) {
    ASSERT(!m_bInitialized);
    
    m_bInitialized = true;
    return Errors::Success;
}

/**
 * @inheritDoc
 */
Error ReplicableNetworkObject::ChangeOccurred(ISubject* pSubject, System::Changes::BitMask ChangeType) {
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
void ReplicableNetworkObject::Update(f32 DeltaTime) {
    ASSERT(m_bInitialized);

    // Send the packet everytime it's dirty
    if (m_dirty) {
        m_dirty = false;
        
        Proto::ObjectUpdated objectUpdated;
        Proto::Object* object = objectUpdated.add_objects();
        object->set_id(m_entity->getId());
        object->set_name(m_entity->getName());
        object->set_template_("ShotTemplate");
        Proto::SystemObject* systemObject = object->add_systemobjects();
        systemObject->set_type("Movable");
        systemObject->set_systemtype(Proto::SystemType::Physic);
        Proto::Property* orientationProperty = systemObject->add_properties();
        orientationProperty->set_name("Orientation");
        getQuaternion(&m_orientation, orientationProperty->mutable_value());
        Proto::Property* positionProperty = systemObject->add_properties();
        positionProperty->set_name("Position");
        getVector3(&m_position, positionProperty->mutable_value());

        std::string data;
        objectUpdated.AppendToString(&data);
        DownstreamMessageProto downstreamMessage;
        downstreamMessage.set_type(DownstreamMessageProto::PLAYER_SHOT);
        downstreamMessage.set_data(data);
        reinterpret_cast<NetworkSystem*>(GetSystemScene()->GetSystem())->getNetworkService()->send(downstreamMessage);
    }
}