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

#include "Manager/IPlatformManager.h"
#include "Manager/IEnvironmentManager.h"
#include "Manager/IServiceManager.h"
#include "Manager/ITaskManager.h"

/**
 * The list of managers interfaces.
 */
class IManagerInterfaces {
public:
    /**
     * Gets platform manager.
     *
     * @return  null if it fails, else the platform manager.
     */
    virtual IPlatformManager* getPlatformManager(void) = 0;

    /**
     * Gets environment manager.
     *
     * @return  null if it fails, else the environment manager.
     */
    virtual IEnvironmentManager* getEnvironmentManager(void) = 0;

    /**
     * Gets service manager.
     *
     * @return  null if it fails, else the service manager.
     */
    virtual IServiceManager* getServiceManager(void) = 0;

    /**
     * Gets task manager.
     *
     * @return  null if it fails, else the task manager.
     */
    virtual ITaskManager* getTaskManager(void) = 0;
};