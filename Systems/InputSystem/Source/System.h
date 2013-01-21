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

#pragma once

#include <list>
#include <SDL_keysym.h>

#include "Errors.h"
#include "Interface.h"
#include "System/ISystem.h"
#include "Input/InputAction.h"

/**
 * Input system.
 * 
 * @sa  ISystem
 */
class InputSystem : public ISystem {

    public:

        /**
         * @inheritDoc
         */
        InputSystem(void);

        /**
         * @inheritDoc
         */
        ~InputSystem(void);

        /**
         * @inheritDoc
         */
        Error initialize(void);

        /**
         * @inheritDoc
         */
        System::Type GetSystemType(void) {
            return System::Types::Input;
        }
        
        /**
         * Create a new input action binded to a key
         */
        InputAction* createInputAction(SDLKey key);

        /**
         * Process the list of events from SDL.
         */
        void pollInputEvents(void);

    protected:

        std::list<InputAction*> m_inputActions;
        
};

