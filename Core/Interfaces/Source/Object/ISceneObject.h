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

#include <queue>

#include "Proto/Common/Object.pb.h"

#include "DataTypes.h"
#include "System/Definitions.h"

/**
 * <c>IGenericScene</c> is an interface for providing generic scene related functionality.  Any
 *  objects that modify scene information are required to implement this class.
 */
class ISceneObject {
public:

    typedef std::queue<ObjectProto>* ObjectProtoQueue;

    /**
     * Gets create objects.
     *
     * @return  null if it fails, else the create objects.
     */
    virtual ObjectProtoQueue getCreateObjects(void) = 0;

    /**
     * Gets destroy objects.
     *
     * @return  null if it fails, else the destroy objects.
     */
    virtual ObjectProtoQueue getDestroyObjects(void) = 0;

};
