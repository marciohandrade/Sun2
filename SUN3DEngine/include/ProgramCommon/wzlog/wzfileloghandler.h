// wzfileloghandler.h

#ifndef _PROGRAMCOMMON_WZFILELOGHANDLER_H_
#define _PROGRAMCOMMON_WZFILELOGHANDLER_H_

#include "wzloghandler.h"

//------------------------------------------------------------------------------
/**
    @class WzFileLogHandler
*/
class WzFileLogHandler : public WzLogHandler
{
public:
    WzFileLogHandler( void );
    virtual ~WzFileLogHandler( void );

    // 파일 열기/닫기
    virtual bool Open( void* param );
    virtual void Close( void );

    // 로그 쓰기
    virtual void Write( WZLOG_LEVEL level, const char* fmt, ... );
    virtual void WriteResult( void );

    // 로그 보기
    virtual void ShowLog( void );

    // 로그 타입
    virtual WZLOG_TYPE GetType( void );

private:
    HANDLE m_handle;    // 파일 핸들
    char m_logPath[MAX_PATH];
};

//------------------------------------------------------------------------------
/**
*/
inline WZLOG_TYPE WzFileLogHandler::GetType( void )
{
    return WZLGT_FILE;
}

#endif // _PROGRAMCOMMON_WZFILELOGHANDLER_H_