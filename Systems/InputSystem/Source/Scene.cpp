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

#include <boost/functional/factory.hpp>
#include <OISB.h>

#include "Interface.h"

#include "Manager/IServiceManager.h"
#include "System.h"
#include "Scene.h"
#include "Task.h"
#include "Object/Object.h"
#include "Object/PlayerObject.h"
#include "Object/ConnectInputObject.h"

extern IServiceManager* g_serviceManager;

/**
 * @inheritDoc
 */
InputScene::InputScene(ISystem* pSystem)
    : ISystemScene(pSystem)
    , ISceneObject() {
    m_TaskFactory = boost::factory<InputTask*>();

    m_ObjectFactories["Player"] = boost::factory<PlayerInputObject*>();
    m_ObjectFactories["Connect"] = boost::factory<ConnectInputObject*>();
    
    m_defaultSchema = OISB::System::getSingleton().getDefaultActionSchemaAutoCreate();
    m_quitInputAction = m_defaultSchema->createAction<OISB::TriggerAction>("Exit");
}

/**
 * @inheritDoc
 */
InputScene::~InputScene(void) {
    
}

/**
 * @inheritDoc
 */
Error InputScene::initialize(void) {
    ASSERT(!m_bInitialized);
    
    m_quitInputAction->bind("Keyboard/ECHAP");
    
    m_bInitialized = true;
    return Errors::Success;
}

/**
 * @inheritDoc
 */
void InputScene::Update(f32 DeltaTime) {
    if (m_quitInputAction->isActive()) {
        g_serviceManager->getRuntimeService()->setStatus(IRuntimeService::Status::Quit);
        return;
    }

    for (auto object : m_pObjects) {
        InputObject* pObject = static_cast<InputObject*>(object.second);
        pObject->Update(DeltaTime);
    }
}

/**
 * @inheritDoc
 */
void InputScene::queueDeleteObject(Proto::Object object) {
    m_deleteObjectQueue->push_back(object);
    PostChanges(System::Changes::Generic::DeleteObject);
}
