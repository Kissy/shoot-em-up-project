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
#include <include/cef_client.h>
#pragma warning( push, 0 )
#include <Ogre.h>
#pragma warning( pop )

#include "Interface.h"

#include "Manager/IServiceManager.h"
#include "System.h"
#include "Scene.h"
#include "Task.h"
#include "Object/Object.h"
#include "Object/ImageObject.h"
#include "Object/LightGraphicObject.h"
#include "Object/ParticleGraphicObject.h"
#include "Object/CameraGraphicObject.h"
#include "Object/MeshGraphicObject.h"
#include "Browser/BrowserClient.h"

extern IServiceManager* g_serviceManager;

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
GraphicScene::GraphicScene(ISystem* pSystem) 
    : ISystemScene(pSystem)
    , m_pSceneManager(NULL)
    , m_pRootNode(NULL)
    , m_FogMode(Ogre::FOG_NONE) {
    m_TaskFactory = boost::factory<GraphicTask*>();
    
    m_propertySetters["AmbientLight"] = boost::bind(&GraphicScene::setAmbientLight, this, _1);
    
    m_ObjectFactories["Particle"] = boost::factory<ParticleGraphicObject*>();
    m_ObjectFactories["Camera"] = boost::factory<CameraGraphicObject*>();
    m_ObjectFactories["Light"] = boost::factory<LightGraphicObject*>();
    m_ObjectFactories["Image"] = boost::factory<ImageGraphicObject*>();
    m_ObjectFactories["Mesh"] = boost::factory<MeshGraphicObject*>();
}

/**
 * @inheritDoc
 */
GraphicScene::~GraphicScene(void) {
    reinterpret_cast<GraphicSystem*>(m_pSystem)->getRoot()->destroySceneManager(m_pSceneManager);
}

/**
 * @inheritDoc
 */
Error GraphicScene::initialize(void) {
    ASSERT(!m_bInitialized);

    m_pSceneManager = reinterpret_cast<GraphicSystem*>(m_pSystem)->getRoot()->createSceneManager(Ogre::ST_GENERIC);
    ASSERT(m_pSceneManager != NULL);
    if (!m_pSceneManager) {
        return Errors::Failure;
    }

    m_pRootNode = m_pSceneManager->getRootSceneNode();
    ASSERT(m_pRootNode != NULL);
    //m_pRootNode->hideBoundingBox(true);

    m_pSceneManager->setAmbientLight(m_ambientLight);
    m_pSceneManager->setSkyBox(true, "nebula");
    m_pSceneManager->addRenderQueueListener(GetSystem<GraphicSystem>()->getOverlaySystem());
    
    m_renderHandler = new RenderHandler();

    {
        CefWindowInfo window_info;
        window_info.SetTransparentPainting(true);
        CefBrowserSettings browserSettings;

        // in linux set a gtk widget, in windows a hwnd. If not available set nullptr - may cause some render errors, in context-menu and plugins.
        window_info.SetAsOffScreen(nullptr);

        m_browserClient = new BrowserClient(m_renderHandler);
        m_browser = CefBrowserHost::CreateBrowserSync(window_info, m_browserClient.get(), "file://D:/My Documents/GitHub/shoot-em-up-project/Assets/Media/Gui/index.html", browserSettings);
        
        // inject user-input by calling
        // browser->GetHost()->SendKeyEvent(...);
        // browser->GetHost()->SendMouseMoveEvent(...);
        // browser->GetHost()->SendMouseClickEvent(...);
        // browser->GetHost()->SendMouseWheelEvent(...);
    }
    
    m_bInitialized = true;
    return Errors::Success;
}

/**
 * @inheritDoc
 */
void GraphicScene::Update(f32 DeltaTime) {
    m_bPause = g_serviceManager->getRuntimeService()->isPaused();
    m_fDeltaTime = DeltaTime;

    u32 size = (u32)m_pObjects.size();
    if (g_serviceManager->getTaskManager() != NULL && UpdateGrainSize < size) {
        g_serviceManager->getTaskManager()->ParallelFor(m_pSystemTask, UpdateCallback, this, 0, size, UpdateGrainSize);
    } else {
        ProcessRange(0, size);
    }
}

/**
 * @inheritDoc
 */
void GraphicScene::UpdateCallback(void* param, u32 begin, u32 end) {
    //ASSERT (dynamic_cast<OGREGraphicsScene*>(param));
    GraphicScene* pThis = static_cast<GraphicScene*>(param);
    pThis->ProcessRange(begin, end);
}

/**
 * @inheritDoc
 */
void GraphicScene::ProcessRange(u32 begin, u32 end) {
    ObjectsList::const_iterator start = m_pObjects.begin() + begin;
    for (ObjectsList::const_iterator iterator = start; iterator < start + end; iterator++) {
        GraphicObject* pObject = reinterpret_cast<GraphicObject*>(iterator->second);

        // Update objects based on paused state
        // TODO maybe not pause some objects ?
        if (!m_bPause) {
            // Process this object
            pObject->Update(m_fDeltaTime);
        }
    }
}

/**
 * @inheritDoc
 */
void GraphicScene::setAmbientLight(Proto::RepeatedString* values) {
    auto value = values->begin();
    m_ambientLight.r = boost::lexical_cast<f32>(*value);
    m_ambientLight.g = boost::lexical_cast<f32>(*(++value));
    m_ambientLight.b = boost::lexical_cast<f32>(*(++value));
}
