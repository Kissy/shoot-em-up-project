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

#include "boost/function.hpp"
#include "Proto/Common/System.pb.h"

#include "Errors.h"
#include "System/Definitions.h"
#include "Property/IProperty.h"

class ISystemScene;

/**
 * <c>ISystem</c> is an interface class designed to be a method for adding functionality to the
 *  framework.  By default the framework does not have functionality for things like graphics,
 *  physics, etc.
 */
class ISystem : public IProperty {

    public:

        /**
         * Constructor.
         */
        ISystem(void);

        /**
         * Destructor.
         * Interface have virtual destructors.
         */
        virtual ~ISystem(void);

        /**
         * Gets the system type for this system.
         * This must function even w/o initialization.
         *
         * @return  The type of the system.
         */
        virtual System::Type GetSystemType(void) = 0;
        
        /**
         * Creates a system scene for containing system objects.
         *
         * @return  The newly create system scene.
         */
        ISystemScene* CreateScene(void);

        /**
         * Destroys a system scene.
         *
         * @param   pSystemScene    The scene to destroy. Any objects within are destroyed.
         * @return  An error code.
         */
        Error DestroyScene(ISystemScene* pSystemScene);
        
        /**
         * Gets the name of the system.
         *
         * @return  The name of the system.
         */
        const char* GetName(void);

        /**
         * Returns the CPU Utilization.
         *
         * @return  CPU Utilization (0-100f)
         */
        virtual f32 GetCPUUsage(void) {
            return 0;
        }

    protected:
        
        typedef boost::function<ISystemScene*(ISystem* pSystem)> SceneFactory;

        SceneFactory                            m_SceneFactory;

};