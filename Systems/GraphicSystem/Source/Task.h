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

#pragma warning( push, 0 )
// Temporarily switching warning level to 0 to ignore warnings in extern/Ogre
#include "Ogre.h"
#pragma warning( pop )

#include "System/ISystemScene.h"

class GraphicScene;

/**
 * Implementation of the ISystemTask interface for OGRE graphics. See Interfaces\System.h for a
 * definition of the class and its functions.
 *
 * @sa  ISystemTask
 */
class GraphicTask : public ISystemTask {
public:

    /**
     * @inheritDoc
     */
    GraphicTask(ISystemScene* pScene);

    /**
     * @inheritDoc
     */
    ~GraphicTask(void);

    /**
     * @inheritDoc
     */
    void Update(f32 DeltaTime);

    /**
     * @inheritDoc
     */
    bool IsPrimaryThreadOnly(void) {
        return false;
    };

    /**
     * @inheritDoc
     */
    System::Type GetSystemType(void) {
        return System::Types::Graphic;
    }

private:
    Ogre::Root*                         m_pRoot;

};
