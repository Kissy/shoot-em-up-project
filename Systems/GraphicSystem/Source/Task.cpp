// Copyright © 2008-2009 Intel Corporation
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

#include <SDL.h>

#include "Interface.h"

#include "Scene.h"
#include "Task.h"

#include "Object/Object.h"

#include "IttNotify.h"


extern ManagerInterfaces   g_Managers;
__ITT_DEFINE_STATIC_EVENT(g_tpeRendering, "Graphics: Rendering", 19);


/**
 * @inheritDoc
 */
GraphicTask::GraphicTask(ISystemScene* pScene) : ISystemTask((ISystemScene*)pScene) {
}

/**
 * @inheritDoc
 */
GraphicTask::~GraphicTask(void) {

}

/**
 * @inheritDoc
 */
void GraphicTask::Update(f32 DeltaTime) {
    SDL_Surface* screen = static_cast<GraphicSystem*> (m_pSystemScene->GetSystem())->GetScreen();
    lockScreen(screen);

    //
    // Update objects for next frame
    //
    m_pSystemScene->Update(DeltaTime);
    
    unlockScreen(screen);
    SDL_UpdateRect(screen, 0, 0, 800, 600);
}

/**
 * @inheritDoc
 */
bool GraphicTask::lockScreen(SDL_Surface* screen) {
    if (SDL_MUSTLOCK(screen)) {
        if (SDL_LockSurface(screen) < 0) {
            return false;
        }
    }

    return true;
}

/**
 * @inheritDoc
 */
void GraphicTask::unlockScreen(SDL_Surface* screen) {
    if (SDL_MUSTLOCK(screen)) {
        SDL_UnlockSurface(screen);
    }
}
