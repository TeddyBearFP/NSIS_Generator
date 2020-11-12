#include "tblog.h"
#include <direct.h>
#include <io.h>
#include <stdarg.h> //va_list, va_arg, va_start, va_end
#include <stdio.h>  //printf
#include <time.h>
#include <windows.h>

LogLevels logThreshold;
char folderPath[ 200 ];
char filePath[ 200 ];
bool isFirst = true;

inline static const char *GetLogLevel( int level )
{
    switch ( level )
    {
        case LogLevels::LOGTRACE:
            return "[TRACE]";
        case LogLevels::LOGDEBUG:
            return "[DEBUG]";
        case LogLevels::LOGINFO:
            return "[INFO ]";
        case LogLevels::LOGWARN:
            return "[WARN ]";
        case LogLevels::LOGERROR:
            return "[ERROR]";
        case LogLevels::LOGFATAL:
            return "[FATAL]";
    }
    return "";
}

void GetTime( RequestType request, char *temp )
{
    char currentTime[ 300 ];
    memset( currentTime, 0, sizeof( currentTime ) );

    time_t now = time( nullptr );

    SYSTEMTIME time;
    GetSystemTime( &time );

    char ms[ 10 ];
    int time_ms = time.wMilliseconds;

    if ( request == LogTimeStamp )
    {
        strftime( currentTime, 30, "%F %T", localtime( &now ) );
        sprintf( ms, "%03d", time_ms );
        strcat( currentTime, "." );
        strcat( currentTime, ms );
    }
    else
        strftime( currentTime, 30, "%Y%m", localtime( &now ) );

    strcpy( temp, currentTime );
}

extern "C" TBLOG_API void SetLogPath( char *path )
{
    memcpy( folderPath, path, sizeof( folderPath ) );
    memcpy( filePath, folderPath, sizeof( filePath ) );
    char date[ 300 ];
    GetTime( LogFileName, date );
    strcat( filePath, "\\NSISGenerator_log_" );
    strcat( filePath, date );
    strcat( filePath, ".txt" );
}

extern "C" TBLOG_API void SetLogLevel( LogLevels level )
{
    logThreshold = level;
}

extern "C" TBLOG_API void Logger( LogLevels level, const char *fmt, ... )
{
    if ( filePath[ 0 ] == '\0' )
        return;

    char message[ 300 ];
    memset( message, 0, sizeof( message ) );

    char message2[ 300 ];
    memset( message2, 0, sizeof( message2 ) );

    if ( level >= LOGTRACE )
    {
        FILE *fp = nullptr;
        if ( ( _access( folderPath, 0 ) ) == -1 )
        {
            if ( _mkdir( folderPath ) == 0 )
            {
                printf( "File add!\n" );
            }
            else
                printf( "Can't add File\n" );
        }

        GetTime( LogTimeStamp, message );
        strcat_s( message, "," );
        strcat_s( message, GetLogLevel( level ) );
        strcat_s( message, "," );

        int threadID = GetCurrentThreadId();
        char buffer[ 20 ];
        sprintf( buffer, "%d", threadID );

        strcat_s( message, buffer );
        strcat_s( message, "," );
        strcat_s( message, fmt );

        va_list args;
        va_start( args, fmt );
        vprintf( message, args ); //print
        strcat_s( message, "\n" );
        fprintf( stdout, "\n" );
        vsprintf( message2, message, args ); //save message and write file.
        va_end( args );

        if ( level >= logThreshold )
        {
            fp = fopen( filePath, "a" );
            fprintf( fp, "%s", message2 );
            fclose( fp );
        }
        fflush( stdout );
    }
}
