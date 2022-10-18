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

    // ���� ����/�ݱ�
    virtual bool Open( void* param );
    virtual void Close( void );

    // �α� ����
    virtual void Write( WZLOG_LEVEL level, const char* fmt, ... );
    virtual void WriteResult( void );

    // �α� ����
    virtual void ShowLog( void );

    // �α� Ÿ��
    virtual WZLOG_TYPE GetType( void );

private:
    HANDLE m_handle;    // ���� �ڵ�
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