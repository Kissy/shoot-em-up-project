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

#include "boost/functional/factory.hpp"

#include "BaseTypes.h"
#include "Interface.h"

#include "System.h"
#include "Scene.h"
#include "Task.h"
#include "Object/Object.h"

extern ManagerInterfaces       g_Managers;

// We use SM2.0 instancing. Since we do normal mapping on the instanced geometry, we
// need to pass both the world matrix and the inverse world matrix for each instance.
// Given our limit of 256 float4 constants, we set the Objects per batch to 40.
// 40 float3x3 world matrices           = 120 float3/4 constants
// 40 float3x3 world inverse matrices   = 120 float3/4 constants
// we have used 240 float4 constants. The rest 16 are saved to pass other information
// to the instancing shader.
static const u32 maxObjectsPerBatch = 40;
static const u32 UpdateGrainSize = 120;

DEFINE_SPIN_MUTEX(GraphicScene::m_mutex);

// Prototypes
void ProcessObjects(void* Data);

/**
 * @inheritDoc
 */
GraphicScene::GraphicScene(ISystem* pSystem) : ISystemScene(pSystem) {
    
    m_TaskFactory = boost::factory<GraphicTask*>();

    //m_ObjectFactories["Camera"] = boost::factory<GraphicObjectCamera*>();
}

/**
 * @inheritDoc
 */
GraphicScene::~GraphicScene(void) {

}

/**
 * @inheritDoc
 */
Error GraphicScene::initialize(void) {
    ASSERT(!m_bInitialized);

    return Errors::Success;
}

/**
 * @inheritDoc
 */
void GraphicScene::Update(f32 DeltaTime) {
    SDL_Surface* screen = static_cast<GraphicSystem*> (m_pSystem)->GetScreen();

    memset(screen->pixels, SDL_MapRGB(screen->format, 50, 0, 0), 800 * 600);
}
