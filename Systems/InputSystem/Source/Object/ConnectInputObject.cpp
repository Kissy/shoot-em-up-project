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
    
    m_connectInputAction = static_cast<InputSystem*>(m_pSystemScene->GetSystem())->createInputAction(SDLK_F1);
    
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

    if (m_connectInputAction->hasChanged()) {
        m_keyboardButtonData.down = m_connectInputAction->isActive();
        PostChanges(System::Changes::Input::Keyboard);
    }
}