﻿// Copyright � 2008-2009 Intel Corporation
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

#include "System/ISystemScene.h"
#include "ConnectNetworkObject.h"
#include "Object/IKeyboardObject.h"

/**
 * @inheritDoc
 */
ConnectNetworkObject::ConnectNetworkObject(ISystemScene* pSystemScene, const char* pszName) : NetworkObject(pSystemScene, pszName) {
}

/**
 * @inheritDoc
 */
ConnectNetworkObject::~ConnectNetworkObject(void) {
}

/**
 * @inheritDoc
 */
Error ConnectNetworkObject::initialize(void) {
    ASSERT(!m_bInitialized);
    
    m_bInitialized = true;
    return Errors::Success;
}

/**
 * @inheritDoc
 */
Error ConnectNetworkObject::ChangeOccurred(ISubject* pSubject, System::Changes::BitMask ChangeType) {
    ASSERT(m_bInitialized);

    if (ChangeType & System::Changes::Input::Keyboard) {
        const IKeyboardObject::KeyboardButtonData* keyboardButtonData = dynamic_cast<IKeyboardObject*>(pSubject)->getKeyboardButtonData();
        if (keyboardButtonData->down) {
            DownstreamMessageProto downstreamMessageProto;
            downstreamMessageProto.set_type(DownstreamMessageProto::AUTHENTICATE);
            downstreamMessageProto.set_data("1");
            reinterpret_cast<NetworkSystem*>(GetSystemScene()->GetSystem())->getNetworkService()->send(downstreamMessageProto);
        }
    }

    return Errors::Success;
}

/**
 * @inheritDoc
 */
void ConnectNetworkObject::Update(f32 DeltaTime) {
    ASSERT(m_bInitialized);

}