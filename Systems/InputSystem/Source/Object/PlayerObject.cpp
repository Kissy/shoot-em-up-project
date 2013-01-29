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

#include "BaseTypes.h"
#include "Interface.h"

#include "Scene.h"
#include "Object.h"
#include "Input/InputAction.h"
#include "Object/PlayerObject.h"

/**
 * @inheritDoc
 */
PlayerInputObject::PlayerInputObject(ISystemScene* pSystemScene, const char* pszName) : InputObject(pSystemScene, pszName)
    , m_velocity(Math::Vector3::Zero) {
    
}

/**
 * @inheritDoc
 */
PlayerInputObject::~PlayerInputObject(void) {
    
}

/**
 * @inheritDoc
 */
Error PlayerInputObject::initialize(void) {
    ASSERT(!m_bInitialized);
    
    m_upInputAction = static_cast<InputSystem*>(m_pSystemScene->GetSystem())->createInputAction(SDLK_UP);
    m_rightInputAction = static_cast<InputSystem*>(m_pSystemScene->GetSystem())->createInputAction(SDLK_RIGHT);
    m_downInputAction = static_cast<InputSystem*>(m_pSystemScene->GetSystem())->createInputAction(SDLK_DOWN);
    m_leftInputAction = static_cast<InputSystem*>(m_pSystemScene->GetSystem())->createInputAction(SDLK_LEFT);

    return Errors::Success;
}

/**
 * @inheritDoc
 */
Error PlayerInputObject::ChangeOccurred(ISubject* pSubject, System::Changes::BitMask ChangeType) {
    ASSERT(m_bInitialized);

    return Errors::Success;
}

void PlayerInputObject::Update(f32 DeltaTime) {
    ASSERT(m_bInitialized);

    u32 mModified = 0;
    
    if (m_upInputAction->hasChanged()) {
        mModified |= System::Changes::Input::Velocity;
        if (m_upInputAction->isActive()) {
            m_velocity.y -= 1;
        } else {
            m_velocity.y += 1;
        }
    }    
    if (m_rightInputAction->hasChanged()) {
        mModified |= System::Changes::Input::Velocity;
        if (m_rightInputAction->isActive()) {
            m_velocity.x += 1;
        } else {
            m_velocity.x -= 1;
        }
    }    
    if (m_downInputAction->hasChanged()) {
        mModified |= System::Changes::Input::Velocity;
        if (m_downInputAction->isActive()) {
            m_velocity.y += 1;
        } else {
            m_velocity.y -= 1;
        }
    }    
    if (m_leftInputAction->hasChanged()) {
        mModified |= System::Changes::Input::Velocity;
        if (m_leftInputAction->isActive()) {
            m_velocity.x -= 1;
        } else {
            m_velocity.x += 1;
        }
    }
    
    if (mModified != 0) {
        PostChanges(mModified);
    }
}