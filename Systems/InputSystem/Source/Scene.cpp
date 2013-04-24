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
#include <SDL.h>

#include "BaseTypes.h"
#include "Interface.h"

#include "System.h"
#include "Scene.h"
#include "Task.h"
#include "Object/Object.h"
#include "Object/PlayerObject.h"


extern ManagerInterfaces   g_Managers;


/**
 * @inheritDoc
 */
InputScene::InputScene(ISystem* pSystem) : ISystemScene(pSystem) {
    m_TaskFactory = boost::factory<InputTask*>();

    m_ObjectFactories["Player"] = boost::factory<PlayerInputObject*>();
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
    
    quitInputAction = static_cast<InputSystem*>(m_pSystem)->createInputAction(SDLK_ESCAPE);
    
    m_bInitialized = true;
    return Errors::Success;
}

/**
 * @inheritDoc
 */
void InputScene::Update(f32 DeltaTime) {
    if (quitInputAction->isActive()) {
        g_Managers.pEnvironment->Runtime().SetStatus(IEnvironmentManager::IRuntime::Status::Quit);
        return;
    }

    ObjectsList Objects = m_pObjects;

    for (ObjectsList::iterator it = Objects.begin(); it != Objects.end(); it++) {
        InputObject* pObject = static_cast<InputObject*>(*it);
        pObject->Update(DeltaTime);
    }
}
