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

#include <logog.hpp>

#undef LOGOG_LEVEL
#define LOGOG_LEVEL LOGOG_LEVEL_WARN

/**
 * Types of log files.
 */
namespace LogType {
    enum LogType {
        Common,

        Graphic,
        Input,
        Network,
        Physic,

        e_LogTypeCount,

        e_Invalid
    };
};


/**
 * LogManager class.
 */
class LogManager {
public:

    /**
     * Constructor.
     */
    LogManager();

    /**
     * Destructor.
     */
    ~LogManager();

private:
    logog::Mutex        m_logMutex;
    logog::LogFile*     m_logFiles[LogType::e_LogTypeCount];

};

