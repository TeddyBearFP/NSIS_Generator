#pragma once

#ifndef TBLOG_H
#define TBLOG_H

#if defined( TBLOG_LIBRARY )
#define TBLOG_API __declspec( dllexport )
#include <guiddef.h>
#else
#define TBLOG_API __declspec( dllexport )
#endif

enum LogLevels
{
    LOGALL,
    LOGTRACE,
    LOGDEBUG,
    LOGINFO,
    LOGWARN,
    LOGERROR,
    LOGFATAL,
    LOGOFF
};

enum RequestType
{
    LogTimeStamp,
    LogFileName
};

extern "C" TBLOG_API void SetLogPath( char* path );

extern "C" TBLOG_API void SetLogLevel( LogLevels level = LOGINFO );

extern "C" TBLOG_API void Logger( LogLevels level, const char* message, ... );

#endif // TBLOG_H
