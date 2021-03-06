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

#include "Scene.h"
#include "Object/PhysicObject.h"

/**
 * @inheritDoc
 */
PhysicObject::PhysicObject(ISystemScene* pSystemScene, IEntity* entity) 
    : ISystemObject(pSystemScene, entity)
    , IGeometryObject() {
    m_orientation.Set(Math::Vector3::UnitZ, 0);
    
    m_propertySetters["Position"] = boost::bind(&IProperty::setVector3, this, System::Changes::Physic::Position, &m_position, _1);
    m_propertyGetters["Position"] = boost::bind(&IProperty::getVector3, this, &m_position, _1);
    
    m_propertySetters["Orientation"] = boost::bind(&IProperty::setQuaternion, this, System::Changes::Physic::Orientation, &m_orientation, _1);
    m_propertyGetters["Orientation"] = boost::bind(&IProperty::getQuaternion, this, &m_orientation, _1);
}

/**
 * @inheritDoc
 */
PhysicObject::~PhysicObject(void) {

}
