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

#include "Interface.h"

#include "System.h"
#include "Scene.h"
#include "Input/InputAction.h"

extern ManagerInterfaces    g_Managers;


/**
 * @inheritDoc
 */
InputSystem::InputSystem(void) : ISystem() {
    m_SceneFactory = boost::factory<InputScene*>();
}

/**
 * @inheritDoc
 */
InputSystem::~InputSystem(void) {

}

/**
 * @inheritDoc
 */
Error InputSystem::initialize(void) {
    ASSERT(!m_bInitialized);
    
    m_bInitialized = true;
    return Errors::Success;
}

/**
 * @inheritDoc
 */
InputAction* InputSystem::createInputAction(SDLKey key) {
    InputAction* inputAction = new InputAction(key);
    m_inputActions.push_back(inputAction);
    return inputAction;
}

/**
 * @inheritDoc
 */
void InputSystem::pollInputEvents(void) {
    SDL_Event event;
    std::list<InputAction*>::const_iterator inputActionIterator;
    
    for (inputActionIterator = m_inputActions.begin(); inputActionIterator != m_inputActions.end(); ++inputActionIterator) {
        (*inputActionIterator)->updatePreviousValue();
    }

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            g_Managers.pEnvironment->Runtime().SetStatus(IEnvironmentManager::IRuntime::Status::Quit);
            return;
        }

        for (inputActionIterator = m_inputActions.begin(); inputActionIterator != m_inputActions.end(); ++inputActionIterator) {
            (*inputActionIterator)->processEvent(event);
        }
    }
}

