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

#include "System.h"
#include "Object/Object.h"

class ISystemScene;

/**
 * Implementation of the IGraphicsObject interface. See Interfaces\Graphics.h and Interfaces\
 * System.h for a definition of the class and its functions.
 * 
 * @sa  ISystemObject
 */
class PlayerInputObject : public InputObject, public IGeometryObject, public IMoveObject {
    
    public:

        /**
         * @inheritDoc
         */
        PlayerInputObject(ISystemScene* pSystemScene, const char* pszName);

        /**
         * @inheritDoc
         */
        ~PlayerInputObject(void);

        /**
         * @inheritDoc
         */
        Error initialize(void);

        /**
         * @inheritDoc
         */
        System::Changes::BitMask GetPotentialSystemChanges(void) {
            return System::Changes::Physic::Velocity | System::Changes::Physic::Orientation;
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
        const Math::Vector3* GetVelocity(void) {
            return &m_velocity;
        }

        /**
         * @inheritDoc
         */
        inline const Math::Vector3* GetPosition(void) {
            return NULL;
        }

        /**
         * @inheritDoc
         */
        inline const Math::Quaternion* GetOrientation(void) {
            return &m_orientation;
        }

        /**
         * @inheritDoc
         */
        inline const Math::Vector3* GetScale(void) {
            return NULL;
        }

    private:
        
        InputAction* m_upInputAction;
        InputAction* m_rightInputAction;
        InputAction* m_downInputAction;
        InputAction* m_leftInputAction;
        InputAction* m_rightRotateInputAction;
        InputAction* m_leftRotateInputAction;

        Math::Vector3       m_velocity;
        Math::Quaternion    m_orientation;

};

