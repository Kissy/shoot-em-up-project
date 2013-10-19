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

#include <map>
#include <string>

#include "Proto/Common.pb.h"

/**
 * An interface for environment runtime functionality.
 */
class ISettingService {
public:    
    /**
     * Adds propertyValue.
     *
     * @param   propertyValue   The property value to add.
     */
    virtual void add(Proto::Property propertyValue) = 0;

    /**
     * Gets the given property name.
     *
     * @param   propertyName    The property name to get.
     */
    virtual Proto::Property get(std::string propertyName) = 0;

    /**
     * Gets as string.
     *
     * @param   propertyName    Name of the property.
     *
     * @return  as string.
     */
    virtual std::string getString(std::string propertyName) = 0;

    /**
     * Gets an int.
     *
     * @param   propertyName    Name of the property.
     *
     * @return  The int.
     */
    virtual int getInt(std::string propertyName) = 0;

    /**
     * Gets a bool.
     *
     * @param   propertyName    Name of the property.
     *
     * @return  true if it succeeds, false if it fails.
     */
    virtual bool getBool(std::string propertyName) = 0;

};