#pragma once

#include "WzLogDefine.h"
#include "wzsingleton.h"


namespace WzLog
{
    class WzLogHandler;

    class WzLogSystem
        : public CWzSingleton<WzLogSystem>
    {
    private:
        WzLogHandler* m_DefaultFileLogHandler;
        WzLogHandler** m_LogHandlerArray;

        size_t m_HandlerCount;
        size_t m_MaxHandlerCount;

    public:
        WzLogSystem();
        ~WzLogSystem();

        void Initialize();
        void Release();

        void AddLogHandler( WzLogHandler* pHandler );

        WzLogHandler* GetDefaultLogHanlder() { return m_DefaultFileLogHandler; }

        void WriteLog( WZLOG_TYPE LogType, const char* Message );
    };
};

// 시스템 초기화
extern void InitializeWzLogSystem();
extern void ReleaseWzLogSystem();
extern void AddWzLogHandler( WzLog::WzLogHandler* pLogHandler );
extern void OutputWzLog( WZLOG_TYPE LogType, const char* fmt, ... );
