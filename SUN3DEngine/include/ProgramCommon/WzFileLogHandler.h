#pragma once

#include "WzLogHandler.h"

namespace WzLog
{
    class WzFileLogHandler
        : public WzLogHandler
    {
    private:
        HANDLE m_FileHandle;
        char m_LogFilePath[1024];
        BOOL m_LevelFilter[WZNUM_LOGLVL];

    public:
        WzFileLogHandler();

        void SetLogFilePath( const char* LogFilePath );
        void SetFilterLevel( int Level );
        void SetFilter( WZLOG_TYPE LogLevel, BOOL bEnable ) { m_LevelFilter[LogLevel] = bEnable; }  // override

    private:
        BOOL OnInitialize(); // override
        void OnRelease(); // override
        void WriteLog( WZLOG_TYPE level, const char* message ); // override
    };
}