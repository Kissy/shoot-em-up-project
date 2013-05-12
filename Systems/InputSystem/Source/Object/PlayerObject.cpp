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

#include "Interface.h"

#include "Scene.h"
#include "Object.h"
#include "Object/PlayerObject.h"

/**
 * @inheritDoc
 */
PlayerInputObject::PlayerInputObject(ISystemScene* pSystemScene, IEntity* entity) 
    : InputObject(pSystemScene, entity) {
    
    InputScene* inputScene = reinterpret_cast<InputScene*>(pSystemScene);
    m_upInputAction = inputScene->getDefaultSchema()->createAction<OISB::TriggerAction>(entity->getName() + "_Up");
    m_rightInputAction = inputScene->getDefaultSchema()->createAction<OISB::TriggerAction>(entity->getName() + "_R");
    m_downInputAction = inputScene->getDefaultSchema()->createAction<OISB::TriggerAction>(entity->getName() + "_Down");
    m_leftInputAction = inputScene->getDefaultSchema()->createAction<OISB::TriggerAction>(entity->getName() + "_E");
    m_rightRotateInputAction = inputScene->getDefaultSchema()->createAction<OISB::TriggerAction>(entity->getName() + "_Right");
    m_leftRotateInputAction = inputScene->getDefaultSchema()->createAction<OISB::TriggerAction>(entity->getName() + "_Left");
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
    
    m_upInputAction->bind("Keyboard/HAUT");
    m_rightInputAction->bind("Keyboard/R");
    m_downInputAction->bind("Keyboard/BAS");
    m_leftInputAction->bind("Keyboard/E");
    m_rightRotateInputAction->bind("Keyboard/DROITE");
    m_leftRotateInputAction->bind("Keyboard/GAUCHE");

    m_bInitialized = true;
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
        mModified |= System::Changes::Physic::Velocity;
        m_velocity.y += m_upInputAction->isActive() ? -1 : 1;
    }
    if (m_rightInputAction->hasChanged()) {
        mModified |= System::Changes::Physic::Velocity;
        m_velocity.x += m_rightInputAction->isActive() ? 1 : -1;
    }
    if (m_downInputAction->hasChanged()) {
        mModified |= System::Changes::Physic::Velocity;
        m_velocity.y += m_downInputAction->isActive() ? 1 : -1;
    }
    if (m_leftInputAction->hasChanged()) {
        mModified |= System::Changes::Physic::Velocity;
        m_velocity.x += m_leftInputAction->isActive() ? -1 : 1;
    }
    if (m_rightRotateInputAction->hasChanged()) {
        mModified |= System::Changes::Physic::Velocity;
        m_velocity.w += m_rightRotateInputAction->isActive() ? -1 : 1;
    }
    if (m_leftRotateInputAction->hasChanged()) {
        mModified |= System::Changes::Physic::Velocity;
        m_velocity.w += m_leftRotateInputAction->isActive() ? 1 : -1;
    }
    
    if (mModified != 0) {
        PostChanges(mModified);
    }
}