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

#ifdef _MSC_VER
#pragma warning( disable : 4996 )
#endif

#include "Assert.h"
#include "Logger.h"

#ifdef LOGGER_ENABLED

#include <string>

#include <boost/iostreams/device/file.hpp>
#include <boost/iostreams/stream.hpp>

#define MAX_STRING_LENGTH 2048

/**
 * The log file names.
 */
std::string s_logFileNames[ LogType::e_LogTypeCount ] = {
    "Debug_Common.log",
    "Debug_Graphic.log",
    "Debug_Input.log",
    "Debug_Network.log",
    "Debug_Physic.log"
};

COMPILE_ASSERT(sizeof(s_logFileNames) / sizeof(s_logFileNames[ 0 ]) == LogType::e_LogTypeCount);

static Log::Logger* s_logger = NULL;

/**
 * @inheritDoc
 */
Log::Logger::Logger(bool bLogging) {
    m_bLogging = bLogging;

    if (m_bLogging) {
        for (u8 Index = 0; Index < LogType::e_LogTypeCount; Index++) {
            std::string fileName = "logs/" + s_logFileNames[Index];
            m_logFiles[Index] = new std::ostream(new boost::iostreams::stream_buffer<boost::iostreams::file_sink>(fileName));
        }
    }
}

/**
 * @inheritDoc
 */
Log::Logger::~Logger() {
    
}

/**
 * @inheritDoc
 */
void Log::Init(Logger* p_logger) {
    s_logger = p_logger;
}

/**
 * @inheritDoc
 */
void Log::Startup(bool bLogging) {
    s_logger = new Logger(bLogging);
}

/**
 * @inheritDoc
 */
void Log::Shutdown(void) {
    // Release s_Debugger resources
    if (s_logger) {
        delete s_logger;
        s_logger = NULL;
    }
}

/**
 * @inheritDoc
 */
Log::Logger* Log::GetLogger(void) {
    return s_logger;
}

/**
 * @inheritDoc
 */
void Log::Logger::Log(LogType::LogType Type, const char* Format, va_list ArgList) {
    if (!m_bLogging) {
        return;
    }

    boost::lock_guard<boost::mutex> lock(m_logMutex);
    
    char message[MAX_STRING_LENGTH];
    vsnprintf(message, sizeof(message), Format, ArgList);
    
    // Output to file
    (*m_logFiles[Type]) << message << std::endl;
    m_logFiles[Type]->flush();
}

#endif