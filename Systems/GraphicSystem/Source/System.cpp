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
#include <boost/lexical_cast.hpp>
#include <boost/bind.hpp>
#include <SDL.h>
#include <SDL_image.h>

#include "Defines.h"
#include "BaseTypes.h"
#include "Interface.h"

#include "System.h"
#include "Scene.h"

extern ManagerInterfaces    g_Managers;

/**
 * @inheritDoc
 */
GraphicSystem::GraphicSystem(void) : ISystem() {
    m_SceneFactory = boost::factory<GraphicScene*>();
}

/**
 * @inheritDoc
 */
GraphicSystem::~GraphicSystem(void) {
    // quit listening to the RenderWindow
    if (m_bInitialized) {
        SDL_Quit();
    }
}

/**
 * @inheritDoc
 */
Error GraphicSystem::initialize(void) {
    ASSERT(!m_bInitialized);

    if (SDL_Init(SDL_INIT_VIDEO) == -1) {
        return Errors::Failure;
    }

    screen = SDL_SetVideoMode(800, 600, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    if ( screen == NULL ) {
        return Errors::Failure;
    }

    int flags = IMG_INIT_JPG | IMG_INIT_PNG;
    int initted = IMG_Init(flags);
    if((initted & flags) != flags) {
        return Errors::Failure;
    }
    
    m_bInitialized = true; 
    return Errors::Success;
}
