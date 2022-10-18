// wzconsoleloghandler.h

#ifndef _PROGRAMCOMMON_WZCONSOLELOGHANDLER_H_
#define _PROGRAMCOMMON_WZCONSOLELOGHANDLER_H_

#include "wzloghandler.h"

//------------------------------------------------------------------------------
/**
    @class WzConsoleLogHandler
*/
class WzConsoleLogHandler : public WzLogHandler
{
public:
    WzConsoleLogHandler( void );
    virtual ~WzConsoleLogHandler( void );

    // 콘솔 열기/닫기
    virtual bool Open( void* param = 0 );
    virtual void Close( void );

    // 로그 쓰기
    virtual void Write( WZLOG_LEVEL level, const char* fmt, ... );
    virtual void WriteResult( void );

    // 로그 타입
    virtual WZLOG_TYPE GetType( void );

private:
    HANDLE m_handle;    // 콘솔 핸들
};

//------------------------------------------------------------------------------
/**
*/
inline WZLOG_TYPE WzConsoleLogHandler::GetType( void )
{
    return WZLGT_CONSOLE;
}

#endif // _PROGRAMCOMMON_WZCONSOLELOGHANDLER_H_