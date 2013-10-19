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

/**
 * An interface for environment runtime functionality.
 */
class IRuntimeService {
public:
    /**
     * Values that represent Status.
     */
    enum Status {
        Unknown, Running, Paused, NextScene, Quit
    };

    /**
     * Returns the status of execution.
     *
     * @return  The execution status.
     */
    virtual Status getStatus(void) = 0;

    /**
     * Sets the status of execution.
     *
     * @param   Status  The execution status.
     */
    virtual void setStatus(Status Status) = 0;

    /**
     * Query if this object is paused.
     *
     * @return  true if paused, false if not.
     */
    virtual bool isPaused(void) = 0;

    /**
     * Query if this object is quit.
     *
     * @return  true if quit, false if not.
     */
    virtual bool isQuit(void) = 0;
};