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

#include "ConnectInputObject.h"

/**
 * @inheritDoc
 */
ConnectInputObject::ConnectInputObject(ISystemScene* pSystemScene, const char* pszName) : InputObject(pSystemScene, pszName) {
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
    
    m_connectInputActionF1 = static_cast<InputSystem*>(m_pSystemScene->GetSystem())->createInputAction(SDLK_F1);
    m_connectInputActionF2 = static_cast<InputSystem*>(m_pSystemScene->GetSystem())->createInputAction(SDLK_F2);
    
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

    if (m_connectInputActionF1->hasChanged()) {
        m_keyboardButtonData.type = 1;
        m_keyboardButtonData.down = m_connectInputActionF1->isActive();
        modified |= System::Changes::Input::Keyboard;
    }
    if (m_connectInputActionF2->hasChanged()) {
        m_keyboardButtonData.type = 2;
        m_keyboardButtonData.down = m_connectInputActionF2->isActive();
        modified |= System::Changes::Input::Keyboard;
    }
    
    PostChanges(modified);
}