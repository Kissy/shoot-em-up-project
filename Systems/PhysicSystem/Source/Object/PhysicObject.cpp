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
#include "Object/PhysicObject.h"

/**
 * @inheritDoc
 */
PhysicObject::PhysicObject(ISystemScene* pSystemScene, const char* pszName) : ISystemObject(pSystemScene, pszName)
    , m_position(Math::Vector3::Zero) {
    m_propertySetters["Position"] = boost::bind(&PhysicObject::setPosition, this, _1);
    m_propertyGetters["Position"] = boost::bind(&PhysicObject::getPosition, this, _1);
}

/**
 * @inheritDoc
 */
PhysicObject::~PhysicObject(void) {

}

/**
 * @inheritDoc
 */
void PhysicObject::setPosition(ProtoStringList values) {
    ProtoStringList::const_iterator value = values.begin();
    m_position.x = boost::lexical_cast<f32>(*(value++));
    m_position.y = boost::lexical_cast<f32>(*(value++));
    m_position.z = boost::lexical_cast<f32>(*value);
    PostChanges(System::Changes::Physic::Position);
}

/**
 * @inheritDoc
 */
void PhysicObject::getPosition(ProtoStringList* values) {
    std::string* value = nullptr;
    value = values->Add();
    value->append(boost::lexical_cast<std::string>(m_position.x));
    value = values->Add();
    value->append(boost::lexical_cast<std::string>(m_position.y));
    value = values->Add();
    value->append(boost::lexical_cast<std::string>(m_position.z));
}

