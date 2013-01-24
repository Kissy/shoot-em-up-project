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

#include "BaseTypes.h"
#include "Interface.h"

#include "Scene.h"
#include "Object.h"
#include "ImageObject.h"

/**
 * @inheritDoc
 */
ImageGraphicObject::ImageGraphicObject(ISystemScene* pSystemScene, const char* pszName) : GraphicObject(pSystemScene, pszName) {
    m_image = SDL_LoadBMP("../../Assets/Media/Graphic/SpaceShip.bmp");
}

/**
 * @inheritDoc
 */
ImageGraphicObject::~ImageGraphicObject(void) {

}

/**
 * @inheritDoc
 */
Error ImageGraphicObject::initialize(void) {
    ASSERT(!m_bInitialized);

    return Errors::Success;
}

/**
 * @inheritDoc
 */
Error ImageGraphicObject::ChangeOccurred(ISubject* pSubject, System::Changes::BitMask ChangeType) {
    ASSERT(m_bInitialized);


    if (ChangeType & System::Changes::Geometry::All) {
        IGeometryObject* pGeometryObject = dynamic_cast<IGeometryObject*>(pSubject);

    }

    return Errors::Success;
}

void ImageGraphicObject::Update(f32 DeltaTime) {
    SDL_Rect position;
    position.x = 0;
    position.y = 0;
    SDL_Surface* screen = static_cast<GraphicSystem*>(GetSystemScene()->GetSystem())->GetScreen();
    SDL_BlitSurface(m_image, NULL, screen, &position);
    SDL_Flip(screen);
}