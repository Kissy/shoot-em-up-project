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

#include "Errors.h"
#include "MathUtils.h"
#include "System/Changes.h"
#include "Property/IProperty.h"

/**
 * @inheritDoc
 */
IProperty::IProperty(void)
    : m_bInitialized(false) {
    
};

/**
 * @inheritDoc
 */
IProperty::~IProperty(void) {
    
};

/**
 * @inheritDoc
 */
void IProperty::setProperties(const ProtoPropertyList& properties) {
    for (auto property : properties) {
        setProperty(property);
    }
};

/**
 * @inheritDoc
 */
void IProperty::setProperty(const PropertyProto& property) {
    PropertySetters::iterator setter = m_propertySetters.find(property.name());
    if (setter == m_propertySetters.end()) {
        //ASSERTMSG1(false, "Parser could not find the property named %s in the list given by the system.", prop->name().c_str());
        return;
    }

    ProtoStringList proto = property.value();
    setter->second(&proto);
};

/**
 * @inheritDoc
 */
ProtoPropertyList IProperty::getProperties(void) {
    ProtoPropertyList propertyList;
    for (auto getter : m_propertyGetters) {
        PropertyProto* property = propertyList.Add();
        property->set_name(getter.first);
        getter.second(property->mutable_value());
    }
    return propertyList;
};

/**
 * @inheritDoc
 */
Error IProperty::initialize(void) {
    ASSERT(!m_bInitialized);
    m_bInitialized = true;
    return Errors::Success;
};

/**
 * @inheritDoc
 */
void IProperty::setString(System::Changes::BitMask changeType, std::string* string, ProtoStringList* values) {
    ProtoStringList::const_iterator value = values->begin();
    string->assign(*value);
}

/**
 * @inheritDoc
 */
void IProperty::getString(std::string* string, ProtoStringList* values) {
    std::string* value = nullptr;
    value = values->Add();
    value->append(*string);
}

/**
 * @inheritDoc
 */
void IProperty::setVector3(System::Changes::BitMask changeType, Math::Vector3* vector3, ProtoStringList* values) {
    ProtoStringList::const_iterator value = values->begin();
    vector3->x = boost::lexical_cast<f32>(*value);
    vector3->y = boost::lexical_cast<f32>(*(++value));
    vector3->z = boost::lexical_cast<f32>(*(++value));
    propertyChanged(changeType);
}

/**
 * @inheritDoc
 */
void IProperty::getVector3(Math::Vector3* vector3, ProtoStringList* values) {
    std::string* value = nullptr;
    value = values->Add();
    value->append(boost::lexical_cast<std::string>(vector3->x));
    value = values->Add();
    value->append(boost::lexical_cast<std::string>(vector3->y));
    value = values->Add();
    value->append(boost::lexical_cast<std::string>(vector3->z));
}

/**
 * @inheritDoc
 */
void IProperty::setQuaternion(System::Changes::BitMask changeType, Math::Quaternion* quaternion, ProtoStringList* values) {
    ProtoStringList::const_iterator value = values->begin();
    quaternion->x = boost::lexical_cast<f32>(*value);
    quaternion->y = boost::lexical_cast<f32>(*(++value));
    quaternion->z = boost::lexical_cast<f32>(*(++value));
    quaternion->w = boost::lexical_cast<f32>(*(++value));
    propertyChanged(changeType);
}

/**
 * @inheritDoc
 */
void IProperty::getQuaternion(Math::Quaternion* quaternion, ProtoStringList* values) {
    std::string* value = nullptr;
    value = values->Add();
    value->append(boost::lexical_cast<std::string>(quaternion->x));
    value = values->Add();
    value->append(boost::lexical_cast<std::string>(quaternion->y));
    value = values->Add();
    value->append(boost::lexical_cast<std::string>(quaternion->z));
    value = values->Add();
    value->append(boost::lexical_cast<std::string>(quaternion->w));
}


/**
 * @inheritDoc
 */
void IProperty::propertyChanged(System::Changes::BitMask uInChangedBits) {
    // NOOP
}
