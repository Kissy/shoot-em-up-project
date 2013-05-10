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

#include "ConnectInputObject.h"

/**
 * @inheritDoc
 */
ConnectInputObject::ConnectInputObject(ISystemScene* pSystemScene, std::string id, std::string name) 
    : InputObject(pSystemScene, id, name) {
    m_propertySetters["Key"] = boost::bind(&ConnectInputObject::setKey, this, _1);
    m_propertyGetters["Key"] = boost::bind(&ConnectInputObject::getKey, this, _1);
}

/**
 * @inheritDoc
 */
ConnectInputObject::~ConnectInputObject(void) {
}

/**
 * @inheritDoc
 */
Error ConnectInputObject::initialize(void) {
    ASSERT(!m_bInitialized);
    
    m_connectInputAction = static_cast<InputSystem*>(m_pSystemScene->GetSystem())->createInputAction(m_key);
    
    m_bInitialized = true;
    return Errors::Success;
}

/**
 * @inheritDoc
 */
Error ConnectInputObject::ChangeOccurred(ISubject* pSubject, System::Changes::BitMask ChangeType) {
    ASSERT(m_bInitialized);

    return Errors::Success;
}

void ConnectInputObject::Update(f32 DeltaTime) {
    ASSERT(m_bInitialized);
    int modified = 0;

    if (m_connectInputAction->hasChanged()) {
        m_keyboardButtonData.type = 0;
        m_keyboardButtonData.down = m_connectInputAction->isActive();
        modified |= System::Changes::Input::Keyboard;
    }
    
    PostChanges(modified);
}

/**
 * @inheritDoc
 */
void ConnectInputObject::setKey(ProtoStringList* values) {
    ProtoStringList::const_iterator value = values->begin();
    std::string key = *value;
    if (key == "F1") {
        m_key = SDLK_F1;
    } else if (key == "F2") {
        m_key = SDLK_F2;
    } else if (key == "F3") {
        m_key = SDLK_F3;
    } else if (key == "F4") {
        m_key = SDLK_F4;
    }
}

/**
 * @inheritDoc
 */
void ConnectInputObject::getKey(ProtoStringList* values) {
    std::string* value = nullptr;
    value = values->Add();
    value->append(boost::lexical_cast<std::string>(m_key));
}