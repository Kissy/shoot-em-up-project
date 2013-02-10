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
#include <cstdio>

#include "DataTypes.h"

#include "boost/thread/mutex.hpp"

#define LOG_ACTUAL( x ) va_list ArgList;                                   \
                        va_start( ArgList, Format );                       \
                        Debug::GetDebugger()->Log( (x), Format, ArgList ); \
                        va_end( ArgList );

/**
 * Data structure for log files
 */
struct LogFile {
    FILE* FileHandle;
    char* FileName;
    char* SystemName;
};

/**
 * Types of log files.
 */
namespace LogType {
    enum LogType {
        e_Debug,

        e_AI,
        e_Animation,
        e_Audio,
        e_Geometry,
        e_Graphics,
        e_Input,
        e_Network,
        e_Physics,

        e_LogTypeCount,

        e_Invalid
    };
};

/**
 * Debugging Functionality.
 */
namespace Debug {

    /**
     * Debugger class.
     */
    class Debugger {
        public:

            /**
             * Constructor.
             *
             * @param	bLogging	true to logging.
             */
            Debugger(bool bLogging);

            /**
             * Destructor.
             */
            ~Debugger();

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
            boost::mutex mutex;
            bool m_bLogging;
            LogFile m_LogFiles[ LogType::e_LogTypeCount ];
    };

#ifdef DEBUG_BUILD

    /**
     * Initialises this object.
     *
     * @param [in,out]	p_Debugger	If non-null, the debugger.
     */
    void Init(Debugger* p_Debugger);

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
    Debugger* GetDebugger(void);

    /**
     * Prints the given format.
     *
     * @param	Format	Describes the format to use.
     */
    void Print(const char* Format, ...);

    /**
     * Logs the given format.
     *
     * @param	Format	Describes the format to use.
     */
    inline void Log(const char* Format, ...) {
        LOG_ACTUAL(LogType::e_Debug);
    };

    /**
     * Logs a i.
     *
     * @param	Format	Describes the format to use.
     */
    inline void LogAI(const char* Format, ...) {
        LOG_ACTUAL(LogType::e_AI);
    };

    /**
     * Logs an animation.
     *
     * @param	Format	Describes the format to use.
     */
    inline void LogAnimation(const char* Format, ...) {
        LOG_ACTUAL(LogType::e_Animation);
    };

    /**
     * Logs an audio.
     *
     * @param	Format	Describes the format to use.
     */
    inline void LogAudio(const char* Format, ...) {
        LOG_ACTUAL(LogType::e_Audio);
    };

    /**
     * Logs a geometry.
     *
     * @param	Format	Describes the format to use.
     */
    inline void LogGeometry(const char* Format, ...) {
        LOG_ACTUAL(LogType::e_Geometry);
    };

    /**
     * Logs the graphics.
     *
     * @param	Format	Describes the format to use.
     */
    inline void LogGraphics(const char* Format, ...)  {
        LOG_ACTUAL(LogType::e_Graphics);
    };

    /**
     * Logs an input.
     *
     * @param	Format	Describes the format to use.
     */
    inline void LogInput(const char* Format, ...) {
        LOG_ACTUAL(LogType::e_Input);
    };

    /**
     * Logs a network.
     *
     * @param	Format	Describes the format to use.
     */
    inline void LogNetwork(const char* Format, ...) {
        LOG_ACTUAL(LogType::e_Network);
    };

    /**
     * Logs the physics.
     *
     * @param	Format	Describes the format to use.
     */
    inline void LogPhysics(const char* Format, ...) {
        LOG_ACTUAL(LogType::e_Physics);
    };

#else  // Debugging disable, all functions will in inline and empty (aka removed)

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable: 4100 )
#endif

    /**
     * Initialises this object.
     *
     * @param [in,out]	p_Debugger	If non-null, the debugger.
     */
    inline void Init(Debug::Debugger* p_Debugger) {};

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
    inline Debug::Debugger* GetDebugger(void) { return NULL; };

    /**
     * Prints the given format.
     *
     * @param	Format	Describes the format to use.
     */
    inline void Print(const char* Format, ...) {};

    /**
     * Logs the given format.
     *
     * @param	Format	Describes the format to use.
     */
    inline void Log(const char* Format, ...) {};

    /**
     * Logs a i.
     *
     * @param	Format	Describes the format to use.
     */
    inline void LogAI(const char* Format, ...) {};

    /**
     * Logs an animation.
     *
     * @param	Format	Describes the format to use.
     */
    inline void LogAnimation(const char* Format, ...) {};

    /**
     * Logs an audio.
     *
     * @param	Format	Describes the format to use.
     */
    inline void LogAudio(const char* Format, ...) {};

    /**
     * Logs a fire.
     *
     * @param	Format	Describes the format to use.
     */
    inline void LogFire(const char* Format, ...) {};

    /**
     * Logs a geometry.
     *
     * @param	Format	Describes the format to use.
     */
    inline void LogGeometry(const char* Format, ...) {};

    /**
     * Logs the graphics.
     *
     * @param	Format	Describes the format to use.
     */
    inline void LogGraphics(const char* Format, ...) {};

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
    inline void LogPhysics(const char* Format, ...) {};

    /**
     * Logs a smoke.
     *
     * @param	Format	Describes the format to use.
     */
    inline void LogSmoke(const char* Format, ...) {};

    /**
     * Logs the trees.
     *
     * @param	Format	Describes the format to use.
     */
    inline void LogTrees(const char* Format, ...) {};

#ifdef _MSC_VER
#pragma warning( pop )
#endif

#endif
}
