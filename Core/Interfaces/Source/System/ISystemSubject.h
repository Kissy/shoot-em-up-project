﻿// Copyright � 2008-2009 Intel Corporation
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

#include "Errors.h"
#include "System/Changes.h"
#include "System/Definitions.h"
#include "Generic/IProperty.h"
#include "Generic/ISubject.h"
#include "Generic/IObserver.h"

/**
 * The Subject implementation.
 * 
 * @sa  ISubject
 */
class ISystemSubject : public ISubject, public IProperty, public IObserver {
public :
        
    /**
     * @inheritDoc
     */
    ISystemSubject(void);
        
    /**
     * @inheritDoc
     */
    virtual ~ISystemSubject(void);
        
    /**
     * Gets the system type for this system object.
     * This is a shortcut to getting the system type w/o having to go the system first.
     *
     * @return  The type of the system. 
     */
    virtual System::Type GetSystemType(void) = 0;
        
    /**
     * Returns a bit mask of System Changes that this scene wants to receive changes for.  Used
     *  to inform the change control manager if this scene should be informed of the change.
     *
     * @return  A System::Changes::BitMask.
     */
    virtual System::Changes::BitMask GetDesiredSystemChanges(void) = 0;

    /**
     * Update the system object.
     *
     * @param   DeltaTime   Time of the delta.
     */
    virtual void Update(f32 DeltaTime) = 0;
    
    /**
     * @inheritDoc
     */
    void propertyChanged(System::Changes::BitMask uInChangedBits);

};
