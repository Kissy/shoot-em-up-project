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

#include "System/ISystem.h"
#include "Proto/Common.pb.h"

/**
 * Interface class for providing access to other systems' ISystem.
 */
class ISystemService {
public:
    /**
     * Adds a new system to the collection.  Called by the ISystem constructor.
     *
     * @param   pSystem A pointer to the system to add.
     * @return  An error code.
     */
    virtual Error add(ISystem* pSystem) = 0;

    /**
     * Removes a system from the collection.  Called by the ISystem destructor.
     *
     * @param   SystemType  The system type to remove.
     * @return  An error code.
     */
    virtual Error remove(const Proto::SystemType SystemType) = 0;

    /**
     * Gets a specific system from the collection based on its type.
     *
     * @param   SystemType  The type of system to get.
     * @return  A pointer to the system.
     */
    virtual ISystem* get(const Proto::SystemType SystemType) = 0;

    /**
     * Gets the system iterator.
     *
     * @return  null if it fails, else.
     */
    virtual std::map<Proto::SystemType, ISystem*> get(void) = 0;

};