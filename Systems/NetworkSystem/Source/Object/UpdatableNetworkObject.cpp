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
#include <boost/lexical_cast.hpp>

#include "BaseTypes.h"
#include "Interface.h"

#include "Scene.h"
#include "Object/Object.h"
#include "Object/UpdatableNetworkObject.h"
#include "Proto/Server/DownstreamMessage.pb.h"
#include "Proto/Message/ObjectUpdated.pb.h"

/**
 * @inheritDoc
 */
UpdatableNetworkObject::UpdatableNetworkObject(ISystemScene* pSystemScene, const char* pszName) : NetworkObject(pSystemScene, pszName)
    , m_position(Math::Vector3::Zero)
    , m_velocity(Math::Vector3::Zero)
    , m_orientation(Math::Quaternion::Zero) {
    m_propertySetters["Velocity"] = boost::bind(&UpdatableNetworkObject::setVelocity, this, _1);
    m_propertyGetters["Velocity"] = boost::bind(&UpdatableNetworkObject::getVelocity, this, _1);

    m_propertySetters["Orientation"] = boost::bind(&UpdatableNetworkObject::setOrientation, this, _1);
    m_propertyGetters["Orientation"] = boost::bind(&UpdatableNetworkObject::getOrientation, this, _1);

    m_propertySetters["Position"] = boost::bind(&UpdatableNetworkObject::setPosition, this, _1);
    m_propertyGetters["Position"] = boost::bind(&UpdatableNetworkObject::getPosition, this, _1);
}

/**
 * @inheritDoc
 */
UpdatableNetworkObject::~UpdatableNetworkObject(void) {
    
}

/**
 * @inheritDoc
 */
Error UpdatableNetworkObject::initialize(void) {
    ASSERT(!m_bInitialized);
    
    m_bInitialized = true;
    return Errors::Success;
}

/**
 * @inheritDoc
 */
Error UpdatableNetworkObject::ChangeOccurred(ISubject* pSubject, System::Changes::BitMask ChangeType) {
    ASSERT(m_bInitialized);

    return Errors::Success;
}

/**
 * @inheritDoc
 */
void UpdatableNetworkObject::Update(f32 DeltaTime) {
    ASSERT(m_bInitialized);

}

/**
 * @inheritDoc
 */
void UpdatableNetworkObject::setVelocity(ProtoStringList values) {
    ProtoStringList::const_iterator value = values.begin();
    m_velocity.x = boost::lexical_cast<f32>(*(value++));
    m_velocity.y = boost::lexical_cast<f32>(*(value++));
    m_velocity.z = boost::lexical_cast<f32>(*value);
    PostChanges(System::Changes::Physic::Velocity);
}

/**
 * @inheritDoc
 */
void UpdatableNetworkObject::setOrientation(ProtoStringList values) {
    ProtoStringList::const_iterator value = values.begin();
    m_orientation.x = boost::lexical_cast<f32>(*(value++));
    m_orientation.y = boost::lexical_cast<f32>(*(value++));
    m_orientation.z = boost::lexical_cast<f32>(*(value++));
    m_orientation.w = boost::lexical_cast<f32>(*value);
    PostChanges(System::Changes::Physic::Orientation);
}

/**
 * @inheritDoc
 */
void UpdatableNetworkObject::getOrientation(ProtoStringList* values) {
    std::string* value = nullptr;
    value = values->Add();
    value->append(boost::lexical_cast<std::string>(m_orientation.x));
    value = values->Add();
    value->append(boost::lexical_cast<std::string>(m_orientation.y));
    value = values->Add();
    value->append(boost::lexical_cast<std::string>(m_orientation.z));
    value = values->Add();
    value->append(boost::lexical_cast<std::string>(m_orientation.w));
}

/**
 * @inheritDoc
 */
void UpdatableNetworkObject::getVelocity(ProtoStringList* values) {
    std::string* value = nullptr;
    value = values->Add();
    value->append(boost::lexical_cast<std::string>(m_velocity.x));
    value = values->Add();
    value->append(boost::lexical_cast<std::string>(m_velocity.y));
    value = values->Add();
    value->append(boost::lexical_cast<std::string>(m_velocity.z));
}

/**
 * @inheritDoc
 */
void UpdatableNetworkObject::setPosition(ProtoStringList values) {
    ProtoStringList::const_iterator value = values.begin();
    m_position.x = boost::lexical_cast<f32>(*(value++));
    m_position.y = boost::lexical_cast<f32>(*(value++));
    m_position.z = boost::lexical_cast<f32>(*value);
    PostChanges(System::Changes::Physic::Position);
}

/**
 * @inheritDoc
 */
void UpdatableNetworkObject::getPosition(ProtoStringList* values) {
    std::string* value = nullptr;
    value = values->Add();
    value->append(boost::lexical_cast<std::string>(m_position.x));
    value = values->Add();
    value->append(boost::lexical_cast<std::string>(m_position.y));
    value = values->Add();
    value->append(boost::lexical_cast<std::string>(m_position.z));
}
