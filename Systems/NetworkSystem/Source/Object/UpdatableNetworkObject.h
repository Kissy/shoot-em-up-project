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

#pragma once

#include <boost/timer/timer.hpp>

#include "MathUtils.h"
#include "System.h"
#include "Object/Object.h"

class ISystemScene;

/**
* Implementation of the IGraphicsObject interface. See Interfaces\Graphics.h and Interfaces\
* System.h for a definition of the class and its functions.
*
* @sa  ISystemObject
*/
class UpdatableNetworkObject : public NetworkObject, public IGeometryObject, public IMoveObject {
public:

    /**
     * @inheritDoc
     */
    UpdatableNetworkObject(ISystemScene* pSystemScene, const char* pszName);

    /**
     * @inheritDoc
     */
    ~UpdatableNetworkObject(void);

    /**
     * @inheritDoc
     */
    Error initialize(void);

    /**
     * @inheritDoc
     */
    System::Changes::BitMask GetPotentialSystemChanges(void) {
        return System::Changes::Input::Velocity | System::Changes::Physic::Position;
    };

    /**
     * @inheritDoc
     */
    System::Types::BitMask GetDesiredSystemChanges(void) {
        return System::Changes::None;
    };

    /**
     * @inheritDoc
     */
    Error ChangeOccurred(ISubject* pSubject, System::Changes::BitMask ChangeType);

    /**
     * @inheritDoc
     */
    void Update(f32 DeltaTime);

    /**
     * @inheritDoc
     */
    void setVelocity(ProtoStringList values);

    /**
     * @inheritDoc
     */
    void getVelocity(ProtoStringList* values);

    /**
     * @inheritDoc
     */
    void setPosition(ProtoStringList values);

    /**
     * @inheritDoc
     */
    void getPosition(ProtoStringList* values);

    /**
     * @inheritDoc
     */
    inline const Math::Vector3* GetPosition(void) {
        return &m_position;
    }

    /**
     * @inheritDoc
     */
    inline const Math::Quaternion* GetOrientation(void) {
        return NULL;
    }

    /**
     * @inheritDoc
     */
    inline const Math::Vector3* GetScale(void) {
        return NULL;
    }

    /**
     * @inheritDoc
     */
    inline const Math::Vector3* GetVelocity(void) {
        return &m_velocity;
    }

private:
    Math::Vector3                   m_position;
    Math::Vector3                   m_velocity;

};