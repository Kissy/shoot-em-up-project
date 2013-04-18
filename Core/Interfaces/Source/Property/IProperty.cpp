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

#include "Errors.h"
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
void IProperty::setProperties(const ProtoPropertyList &properties) {
    for (ProtoPropertyList::const_iterator property = properties.begin(); property != properties.end(); property++) {
        setProperty(*property);
    }
};

/**
 * @inheritDoc
 */
void IProperty::setProperty(const PropertyProto &property) {
    PropertySetters::iterator propertySetterIterator = m_propertySetters.find(property.name());
    if (propertySetterIterator == m_propertySetters.end()) {
        //ASSERTMSG1(false, "Parser could not find the property named %s in the list given by the system.", prop->name().c_str());
        return;
    }

    propertySetterIterator->second(property.value());
};

/**
 * @inheritDoc
 */
Error IProperty::initialize(void) {
    ASSERT(!m_bInitialized);
    m_bInitialized = true;
    return Errors::Success;
};
