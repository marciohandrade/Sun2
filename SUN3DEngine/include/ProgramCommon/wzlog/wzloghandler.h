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

    // 인스턴스 얻기
    static WzLogHandler* Instance( void );
    static bool IsValid( void );

    // 로그 정보 초기화
    virtual void ClearLogInfo( void );

    // 열기/닫기
    virtual bool Open( void* param );
    virtual void Close( void );

    // 로그 쓰기
    virtual void Write( WZLOG_LEVEL level, const char* fmt, ... );
    virtual void WriteResult( void );

    // 로그 보기
    virtual void ShowLog( void );

    // 로그 타입
    virtual WZLOG_TYPE GetType( void );

    // 로그 레벨 설정
    void SetLevel( int level );

protected:
    static WzLogHandler* m_instance;

    int m_level;        // 로그 레벨
    int m_numLvlLog[5]; // 각 레벨별 로그 수
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