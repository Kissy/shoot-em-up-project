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

#include "OIS.h"

#include "System.h"
#include "System/ISystemScene.h"
#include "Object/Object.h"
#include "Object/IGeometryObject.h"
#include "Object/IMouseObject.h"
#include "Object/IMoveObject.h"

class InputScene;
class InputTask;

/**
 * <c>InputMouseObject</c> Implementation of the ISystemObject interface.
 * This is the Mouse object created objects.
 * 
 * @sa  InputObject
 * @sa  IMoveObject
 */
class InputMouseObject : public InputObject, public IMouseObject, public OIS::MouseListener {

    public:

        /**
         * @inheritDoc
         */
        InputMouseObject(ISystemScene* pSystemScene, const char* pszName);

        /**
         * @inheritDoc
         */
        ~InputMouseObject(void);
        
        /**
         * @inheritDoc
         */
        Error initialize(void);
        
        /**
         * @inheritDoc
         */
        void Update(f32 DeltaTime);
        
        /**
         * @inheritDoc
         */
        System::Changes::BitMask GetPotentialSystemChanges(void) {
            return System::Changes::Input::Velocity | System::Changes::Input::Mouse;
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
        const MouseButtonData GetMouseButtonData(void) {
            return m_MouseButtonData;
        }

        /**
         * @inheritDoc
         */
        const Math::Vector3* GetMousePosition(void) {
            return &m_MousePosition;
        }
        
        /**
         * @inheritDoc
         */
        bool mouseMoved(const OIS::MouseEvent &arg);

        /**
         * @inheritDoc
         */
		bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);

        /**
         * @inheritDoc
         */
		bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);

    private:

        u32                                 m_Modified;
        MouseButtonData                     m_MouseButtonData;
        Math::Vector3                       m_MousePosition;

};

