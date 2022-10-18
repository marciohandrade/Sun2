#pragma once


#include "WzLogHandler.h"


namespace WzLog
{
    class WzConsoleLogHandler
        : public WzLogHandler
    {
    public:
        WzConsoleLogHandler();

        void SetFilter( WZLOG_TYPE LogLevel, BOOL bEnable ) { m_LevelFilter[LogLevel] = bEnable; }  // override
        BOOL GetFilter( WZLOG_TYPE LogLevel ) { return m_LevelFilter[LogLevel]; } // override

        void ToggleConsole();
        BOOL IsShow();
        void ShowConsole( BOOL bShow );

    private:
        BOOL m_LevelFilter[WZNUM_LOGLVL];
        HANDLE m_ConsoleHandle;

        BOOL OnInitialize(); // override
        void OnRelease(); // override
        void WriteLog( WZLOG_TYPE level, const char* message ); // override

        HANDLE& GetConsoleHandle() { return m_ConsoleHandle; }

        void InitializeConsole();
        void ReleaseConsole();
    };
}