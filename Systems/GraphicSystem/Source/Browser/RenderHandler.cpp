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

#include <Overlay/OgreOverlayManager.h>
#include <Overlay/OgreOverlayContainer.h>
#include <OgreTexture.h>
#include <OgreMaterial.h>
#include <OgreTechnique.h>
#include <OgreHardwarePixelBuffer.h>
#include <OgreTextureManager.h>
#include <OgreMaterialManager.h>

#include "RenderHandler.h"

/**
 * @inheritDoc
 */
RenderHandler::RenderHandler() {
    m_texture = Ogre::TextureManager::getSingleton().createManual("HUDTexture", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
       Ogre::TEX_TYPE_2D, 500, 500, 0, Ogre::PF_BYTE_BGRA, Ogre::TU_DYNAMIC);

    Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().create("HUDMaterial", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    material->getTechnique(0)->getPass(0)->createTextureUnitState("HUDTexture");
    material->getTechnique(0)->getPass(0)->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);

    Ogre::OverlayManager& overlayManager = Ogre::OverlayManager::getSingleton();
    m_overlay = overlayManager.create("OverlayName");

    Ogre::OverlayContainer* panel = static_cast<Ogre::OverlayContainer*>(overlayManager.createOverlayElement("Panel", "PanelName"));
    panel->setPosition(0.0, 0.0);
    panel->setDimensions(1.0, 1.0);
    panel->setMaterialName("HUDMaterial");

    m_overlay->add2D(panel);
    m_overlay->show();
}

/**
 * @inheritDoc
 */
RenderHandler::~RenderHandler() {
}

/**
 * @inheritDoc
 */
bool RenderHandler::GetViewRect(CefRefPtr<CefBrowser> browser, CefRect &rect) {
    rect = CefRect(0, 0, m_texture->getWidth(), m_texture->getHeight());
    return true;
}

/**
 * @inheritDoc
 */
void RenderHandler::OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList &dirtyRects, const void *buffer, int width, int height) {
    const Ogre::HardwarePixelBufferSharedPtr textureBuffer = m_texture->getBuffer();
    const Ogre::PixelBox srcBox = Ogre::PixelBox(Ogre::Box(0, 0, 0, width, height, 1), Ogre::PF_BYTE_BGRA, const_cast<void *>(buffer));

    for(CefRect dirtyRect : dirtyRects) {
        const Ogre::Box destBox(dirtyRect.x, dirtyRect.y, 0, dirtyRect.width, dirtyRect.height, 1);
        textureBuffer->blitFromMemory(srcBox.getSubVolume(destBox), destBox);
    }
}
