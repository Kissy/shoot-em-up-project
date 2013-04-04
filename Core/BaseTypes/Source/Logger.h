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

#include <cstdarg>
#include <boost/thread/mutex.hpp>

#include "DataTypes.h"

#define LOG_ACTUAL( x ) va_list ArgList;                                   \
                        va_start( ArgList, Format );                       \
                        Log::GetLogger()->Log( (x), Format, ArgList ); \
                        va_end( ArgList );


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
 * Debugging Functionality.
 */
namespace Log {

    /**
     * Debugger class.
     */
    class Logger {
        public:

            /**
             * Constructor.
             */
            Logger(bool bLogging);

            /**
             * Destructor.
             */
            ~Logger();

            /**
             * Prints.
             *
             * @param	Format 	Describes the format to use.
             * @param	ArgList	List of arguments.
             */
            void Print(const char* Format, va_list ArgList);

            /**
             * Logs.
             *
             * @param	Type   	The type.
             * @param	Format 	Describes the format to use.
             * @param	ArgList	List of arguments.
             */
            void Log(LogType::LogType Type, const char* Format, va_list ArgList);

        private:

            bool            m_bLogging;

            boost::mutex    m_logMutex;
            std::ostream*   m_logFiles[LogType::e_LogTypeCount];

    };

#ifdef LOGGER_ENABLED

    /**
     * Initialises this object.
     *
     * @param [in,out]	p_logger	If non-null, the logger.
     */
    void Init(Logger* p_logger);

    /**
     * Prepares this object for use.
     *
     * @param	bLogging	(optional) the logging.
     */
    void Startup(bool bLogging = false);

    /**
     * Shuts down this object and frees any resources it is using.
     */
    void Shutdown(void);

    /**
     * Gets the debugger.
     *
     * @return	null if it fails, else the debugger.
     */
    Logger* GetLogger(void);

    /**
     * Logs the given format.
     *
     * @param	Format	Describes the format to use.
     */
    inline void Log(const char* Format, ...) {
        LOG_ACTUAL(LogType::Common);
    };

    /**
     * Logs the graphics.
     *
     * @param	Format	Describes the format to use.
     */
    inline void LogGraphic(const char* Format, ...)  {
        LOG_ACTUAL(LogType::Graphic);
    };

    /**
     * Logs an input.
     *
     * @param	Format	Describes the format to use.
     */
    inline void LogInput(const char* Format, ...) {
        LOG_ACTUAL(LogType::Input);
    };

    /**
     * Logs a network.
     *
     * @param	Format	Describes the format to use.
     */
    inline void LogNetwork(const char* Format, ...) {
        LOG_ACTUAL(LogType::Network);
    };

    /**
     * Logs the physics.
     *
     * @param	Format	Describes the format to use.
     */
    inline void LogPhysic(const char* Format, ...) {
        LOG_ACTUAL(LogType::Physic);
    };

#else  // Debugging disable, all functions will in inline and empty (aka removed)

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable: 4100 )
#endif

    /**
     * Initialises this object.
     *
     * @param [in,out]	p_logger	If non-null, the debugger.
     */
    inline void Init(Log::Logger* p_logger) {};

    /**
     * Prepares this object for use.
     *
     * @param	bLogging	(optional) the logging.
     */
    inline void Startup(bool bLogging = false) {};

    /**
     * Shuts down this object and frees any resources it is using.
     */
    inline void Shutdown(void) {};

    /**
     * Gets the debugger.
     *
     * @return	null if it fails, else the debugger.
     */
    inline Log::Logger* GetLogger(void) { return NULL; };

    /**
     * Logs the given format.
     *
     * @param	Format	Describes the format to use.
     */
    inline void Log(const char* Format, ...) {};

    /**
     * Logs the graphics.
     *
     * @param	Format	Describes the format to use.
     */
    inline void LogGraphic(const char* Format, ...) {};

    /**
     * Logs an input.
     *
     * @param	Format	Describes the format to use.
     */
    inline void LogInput(const char* Format, ...) {};

    /**
     * Logs the physics.
     *
     * @param	Format	Describes the format to use.
     */
    inline void LogPhysic(const char* Format, ...) {};

#ifdef _MSC_VER
#pragma warning( pop )
#endif

#endif
}
