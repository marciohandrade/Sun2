// wzloghandler.h

#ifndef _PROGRAMCOMMON_WZLOGHANDLER_H_
#define _PROGRAMCOMMON_WZLOGHANDLER_H_

#include "wzlogtype.h"

//------------------------------------------------------------------------------
/**
    @class WzLogHandler
*/
class WzLogHandler
{
public:
    WzLogHandler( void );
    virtual ~WzLogHandler( void ) = 0;

    // �ν��Ͻ� ���
    static WzLogHandler* Instance( void );
    static bool IsValid( void );

    // �α� ���� �ʱ�ȭ
    virtual void ClearLogInfo( void );

    // ����/�ݱ�
    virtual bool Open( void* param );
    virtual void Close( void );

    // �α� ����
    virtual void Write( WZLOG_LEVEL level, const char* fmt, ... );
    virtual void WriteResult( void );

    // �α� ����
    virtual void ShowLog( void );

    // �α� Ÿ��
    virtual WZLOG_TYPE GetType( void );

    // �α� ���� ����
    void SetLevel( int level );

protected:
    static WzLogHandler* m_instance;

    int m_level;        // �α� ����
    int m_numLvlLog[5]; // �� ������ �α� ��
};

//------------------------------------------------------------------------------
/**
*/
inline WzLogHandler* WzLogHandler::Instance( void )
{
    assert( m_instance );

    return m_instance;
}

//------------------------------------------------------------------------------
/**
*/
inline bool WzLogHandler::IsValid( void )
{
    return ( m_instance != NULL );
}

//------------------------------------------------------------------------------
/**
*/
inline WZLOG_TYPE WzLogHandler::GetType( void )
{
    return WZLGT_UNKNOWN;
}

//------------------------------------------------------------------------------
/**
*/
inline void WzLogHandler::SetLevel( int level )
{
    m_level = level;
}

#endif // _PROGRAMCOMMON_WZLOGHANDLER_H_