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

#include <boost/bind.hpp>
#pragma warning( push, 0 )
#include <OgreSceneNode.h>
#pragma warning( pop )

#include "Scene.h"
#include "Object.h"
#include "Object/ImageObject.h"
#include "Object/IGeometryObject.h"

const std::string ImageGraphicObject::IMAGE_BASE_PATH = "../../Assets/Media/Graphic/";

/**
 * @inheritDoc
 */
ImageGraphicObject::ImageGraphicObject(ISystemScene* pSystemScene, IEntity* entity) 
    : GraphicObject(pSystemScene, entity) {
    /*m_position = new SDL_Rect();
    m_position->x = 0;
    m_position->y = 0;*/

    m_propertySetters["Image"] = boost::bind(&IProperty::setString, this, System::Changes::None, &m_image, _1);
    m_propertyGetters["Image"] = boost::bind(&IProperty::getString, this, &m_image, _1);
}

/**
 * @inheritDoc
 */
ImageGraphicObject::~ImageGraphicObject(void) {
    /*delete m_position;
    if (m_bInitialized) {
        SDL_FreeSurface(m_sourceImage);
        SDL_FreeSurface(m_displayImage);
    }*/
}

/**
 * @inheritDoc
 */
Error ImageGraphicObject::initialize(void) {
    ASSERT(!m_bInitialized);
    
    std::string image = IMAGE_BASE_PATH + m_image;
    /*m_sourceImage = IMG_Load(image.c_str());
    m_displayImage = SDL_ConvertSurface(m_sourceImage, m_sourceImage->format, m_sourceImage->flags);*/
    
    Ogre::SceneNode* rootNode = static_cast<GraphicScene*>(m_pSystemScene)->getRootNode();
    Ogre::SceneManager* sceneManager = static_cast<GraphicScene*>(m_pSystemScene)->getSceneManager();
    Ogre::SceneNode* myNode = static_cast<Ogre::SceneNode*>(rootNode->createChild());
    Ogre::BillboardSet* mySet = sceneManager->createBillboardSet("mySet");
    Ogre::Billboard* myBillboard = mySet->createBillboard(Ogre::Vector3(0, 0, 0));
    myBillboard->setDimensions(100, 100);
    myNode->attachObject(mySet);
    
    m_bInitialized = true;
    return Errors::Success;
}

/**
 * @inheritDoc
 */
Error ImageGraphicObject::ChangeOccurred(ISubject* pSubject, System::Changes::BitMask ChangeType) {
    ASSERT(m_bInitialized);

    if (ChangeType & System::Changes::Physic::Position) {
        auto position = dynamic_cast<IGeometryObject*>(pSubject)->GetPosition();
        /*m_position->x = (Sint16) position->x - 64;
        m_position->y = (Sint16) position->y - 64;*/
    }
    if (ChangeType & System::Changes::Physic::Orientation) {
        const Math::Quaternion* orientation = dynamic_cast<IGeometryObject*>(pSubject)->GetOrientation();
        /*f32 rotation = Math::Angle::Rad2Deg(orientation->GetAngle());
        m_displayImage = SPG_Rotate(m_sourceImage, rotation);*/
    }

    return Errors::Success;
}

/**
 * @inheritDoc
 */
void ImageGraphicObject::Update(f32 DeltaTime) {
    /*SDL_Surface* screen = static_cast<GraphicSystem*>(GetSystemScene()->GetSystem())->GetScreen();
    SDL_BlitSurface(m_displayImage, NULL, screen, m_position);*/
}
