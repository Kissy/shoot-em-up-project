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

#include "Manager/IManagerInterfaces.h"
#include "Manager/PlatformManager.h"
#include "Manager/EnvironmentManager.h"
#include "Manager/ServiceManager.h"
#include "Manager/TaskManager.h"

/**
 * The list of managers interfaces.
 */
class ManagerInterfaces : public IManagerInterfaces {
public:
    /**
     * Default constructor.
     */
    ManagerInterfaces();

    /**
     * Destructor.
     */
    ~ManagerInterfaces();

    /**
     * @inheritDoc
     */
    inline IPlatformManager* getPlatformManager(void) {
        return pPlatform;
    }

    /**
     * @inheritDoc
     */
    inline IEnvironmentManager* getEnvironmentManager(void) {
        return pEnvironment;
    }

    /**
     * @inheritDoc
     */
    inline IServiceManager* getServiceManager(void) {
        return pService;
    }

    /**
     * @inheritDoc
     */
    inline ITaskManager* getTaskManager(void) {
        return pTask;
    }
    
private:
    PlatformManager*       pPlatform;
    EnvironmentManager*    pEnvironment;
    ServiceManager*        pService;
    TaskManager*           pTask;

};