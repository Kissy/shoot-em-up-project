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

#include "System/Definitions.h"
#include "FrameworkAPI.h"

class Scheduler;
class ChangeManager;
class UScene;
class UObject;
class Gdf;

/**
 * Framework.
 * Responsible for tying in all the different managers and systems.  Also handles
 * loading and parsing of the global definition file (gdf).
 * 
 * @sa  IService::ISystemAccess
 * @sa  IService::ISystemAccess
 */
class Framework : public IServiceManager::ISystemAccess {
    public:

        /**
         * Default constructor.
         */
        Framework(void) throw(...);

        /**
         * Destructor.
         */
        ~Framework(void);

        /**
         * Initializes this :.
         *
         * @return  Error status.
         */
        Error Initialize(void);

        /**
         * Shuts down this : and frees any resources it is using.
         */
        void Shutdown(void);

        /**
         * Executes the framework..
         *
         * @return An error code.
         */
        Error Execute(void) throw(...);


    protected:

        /**
         * Gets a system.
         * Implementation of IService::ISystemAccess::GetSystem.
         *
         * @param   pszSystemName   Name of the system.
         * @return  The system.
         */
        virtual Handle GetSystem(const char* pszSystemName);

        /**
         * Implementation of IService::ISystemAccess::GetSystem.
         *
         * @param   Type    The type.
         * @return  The system.
         */
        virtual Handle GetSystem(System::Type Type);

        /**
         * Gets a scene.
         * Implementation of IService::ISystemAccess::GetScene.
         *
         * @param   pszSystemName   Name of the system.
         * @return  The scene.
         */
        virtual Handle GetScene(const char* pszSystemName);

        /**
         * Implementation of IService::ISystemAccess::GetScene.
         *
         * @param   Type    The type.
         * @return  The scene.
         */
        virtual Handle GetScene(System::Type Type);

        /**
         * Gets a system object.
         * Implementation of IService::ISystemAccess::GetSystemObject.
         *
         * @param   pszSystemName   Name of the system.
         * @param   pszName         The name.
         * @return  The system object.
         */
        virtual Handle GetSystemObject(const char* pszSystemName, const char* pszName);

        /**
         * Implementation of IService::ISystemAccess::GetSystemObject.
         *
         * @param   Type    The type.
         * @param   pszName The name.
         * @return  The system object.
         */
        virtual Handle GetSystemObject(System::Type Type, const char* pszName);

        /**
         * Issues all the pending property changes.  This only occurs after the scheduler has
         *  executed all the waiting items.
         *
         * @param   SystemTypes The system types of the system that can accept property changes.
         */
        void IssuePendingSystemPropertyChanges(System::Types::BitMask SystemTypes = System::Types::All);

    protected:

        Scheduler*                              m_pScheduler;
        ChangeManager*                          m_pSceneCCM;
        ChangeManager*                          m_pObjectCCM;
        UScene*                                 m_pScene;

        bool                                    m_bExecuteLoop;

    private:

        std::string                             m_sNextScene;

};
