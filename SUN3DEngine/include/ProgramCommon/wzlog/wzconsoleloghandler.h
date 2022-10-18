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

    // �ܼ� ����/�ݱ�
    virtual bool Open( void* param = 0 );
    virtual void Close( void );

    // �α� ����
    virtual void Write( WZLOG_LEVEL level, const char* fmt, ... );
    virtual void WriteResult( void );

    // �α� Ÿ��
    virtual WZLOG_TYPE GetType( void );

private:
    HANDLE m_handle;    // �ܼ� �ڵ�
};

//------------------------------------------------------------------------------
/**
*/
inline WZLOG_TYPE WzConsoleLogHandler::GetType( void )
{
    return WZLGT_CONSOLE;
}

#endif // _PROGRAMCOMMON_WZCONSOLELOGHANDLER_H_