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

#include <cstring>
#include <vector>

#include "Defines.h"
#include "DataTypes.h"
#include "Singleton.h"
#include "Manager/IEnvironmentManager.h"

/**
 * A singleton for getting and setting information about the environment.
 * 
 * @sa  IEnvironment
 * @sa  IEnvironment::IRuntime
 * @sa  Singleton
 */
class EnvironmentManager : public IEnvironmentManager, public IEnvironmentManager::IRuntime, public Singleton {
    public:

        /// <summary>
        ///   Constructor.
        /// </summary>
        EnvironmentManager(void);

        /// <summary>
        ///   Destructor.
        /// </summary>
        ~EnvironmentManager(void);


    public:

        ////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>
        ///   Environment variable functionality.
        /// </summary>
        ////////////////////////////////////////////////////////////////////////////////////////////////

        class Variables : public IEnvironmentManager::IVariables {
                friend class EnvironmentManager;

            public:

                /// <summary>
                ///   Adds a new environment variable.
                /// </summary>
                /// <param name="pszName">The name of the variable.</param>
                /// <param name="pszValue">The value of the variable.</param>
                void Add(In char* pszName, In char* pszValue);

                /// <summary>
                ///   Sets new value for environment variable.
                /// </summary>
                /// <param name="pszName">The name of the variable.</param>
                /// <param name="pszValue">The new value of the variable.</param>
                void SetValue(In char* pszName, In char* pszValue);

                /// <summary cref="IEnvironment::IVariables::GetAsString">
                ///   Implementation of IEnvironment::IVariables::GetAsString.
                /// </summary>
                virtual const char* GetAsString(In char* pszName, In char* pszDefaultValue = "");

                /// <summary cref="IEnvironment::IVariables::IsString">
                ///   Implementation of IEnvironment::IVariables::IsString.
                /// </summary>
                virtual bool IsString(In char* pszName, In char* pszCompareValue);

                /// <summary cref="IEnvironment::IVariables::GetAsBool">
                ///   Implementation of IEnvironment::IVariables::GetAsBool.
                /// </summary>
                virtual bool GetAsBool(In char* pszName, In bool bDefaultValue = false);

                /// <summary cref="IEnvironment::IVariables::GetAsInt">
                ///   Implementation of IEnvironment::IVariables::GetAsInt.
                /// </summary>
                virtual i32 GetAsInt(In char* pszName, In i32 DefaultValue = 0);

                /// <summary cref="IEnvironment::IVariables::GetAsFloat">
                ///   Implementation of IEnvironment::IVariables::GetAsFloat.
                /// </summary>
                virtual f32 GetAsFloat(In char* pszName, In f32 DefaultValue = 0.0f);


            protected:

                /// <summary>
                ///   Constructor.
                /// </summary>
                /// <remarks>Only accessible via the EnvironmentManager class.</remarks>
                Variables(void);

                /// <summary>
                ///   Gets the value of a variable.
                /// </summary>
                /// <param name="pszName">The name of the variable.</param>
                /// <param name="pszValue">The returned value of the variable.</param>
                /// <returns>true if the variable exists, false otherwise.</returns>
                bool GetValue(In char* pszName, Out const char*& pszValue);


            protected:

                struct Pair {
                    std::string                             sName;
                    std::string                             sValue;
                };

                typedef std::vector<Pair>                   Pairs;
                typedef Pairs::iterator                     PairsIt;

                Pairs                                       m_Pairs;
        };


        ////////////////////////////////////////////////////////////////////////////////////////////////
        // IEnvironment::IRuntime functionality.

        /// <summary cref="IEnvironment::IRuntime::GetStatus">
        ///   Implementation of IEnvironment::IRuntime::GetStatus.
        /// </summary>
        virtual IEnvironmentManager::IRuntime::Status GetStatus(void);

        /// <summary cref="IEnvironment::IRuntime::SetStatus">
        ///   Implementation of IEnvironment::IRuntime::SetStatus.
        /// </summary>
        virtual void SetStatus(IEnvironmentManager::IRuntime::Status Status);


    protected:

        Variables                                       m_Variables;

        IEnvironmentManager::IRuntime::Status           m_RuntimeStatus;


    public:

        /// <summary cref="IEnvironment::Variables">
        ///   Implementation of IEnvironment::Variables.
        /// </summary>
        virtual Variables& Variables(void) {
            return m_Variables;
        }

        /// <summary cref="IEnvironment::Runtime">
        ///   Implementation of IEnvironment::Runtime.
        /// </summary>
        virtual IEnvironmentManager::IRuntime& Runtime(void) {
            return *this;
        }
};


DeclareSingleton(EnvironmentManager);
