// scriptread.h

#ifndef _PROGRAMCOMMON_WZSCRIPTREAD_H_
#define _PROGRAMCOMMON_WZSCRIPTREAD_H_

#include "discaccess.h"

enum ENUM_TOKEN_TYPE
{
    TT_ANYTYPE      = -1,
    TT_ERROR        = 0,
    TT_END,
    TT_NUMBER,
    TT_STRING,

    TT_COMMAND      = '#',
    TT_GROUP_BEGIN  = '{',
    TT_GROUP_END    = '}',
    TT_COMMA        = ',',
    TT_SEMICOLON    = ';',
};

//------------------------------------------------------------------------------
/**
    @class CScriptRead
*/
class CScriptRead
{
public:
    // 생성자/소멸자
    CScriptRead( void );
    ~CScriptRead( void );

    // DiscAccess 설정
    void SetDiscAccess( CDiscAccess* pDiscAccess );

    // 생성(파일)
    BOOL Create( const char* lpszFileName );
    // 생성(버퍼로부터)
    BOOL CreateFromBuffer( const char* pBuffer, int iBufferSize );

    // 제거
    void Destroy( void );

    // 토큰 얻기(특정 타입을 얻어야 할 경우 인자를 넣는다.)
    ENUM_TOKEN_TYPE GetToken( ENUM_TOKEN_TYPE ettTypeToGet = TT_ANYTYPE );
    // 문자열 토큰 얻기
    const char* GetTokenStringPtr( void ) const;
    // 숫자 토큰 얻기
    float GetTokenNumber( void ) const;

private:
    // 데이타 초기화
    void Clear( void );

    // 내부 DiscAccess 생성/제거
    void CreateDiscAccess( void );
    void ReleaseDiscAccess( void );

    // 바이트 하나 얻기(다음 위치로 이동)
    BYTE GetC( void );
    // 현재 위치의 바이트 하나 얻기
    BYTE GetNext( void ) const;
    // 이전 위치로 이동
    void UnGet( void );

private:
    CDiscAccess* m_pDiscAccess;
    BOOL m_bDiscAccessGenerated;

    int m_iBufferSize;  // 버퍼 크기
    BYTE* m_pbyBuffer;  // 버퍼
    int m_iBufferPos;   // 버퍼 상의 위치

    char m_lpszTokenString[1024];
    float m_fTokenNumber;
};

//------------------------------------------------------------------------------
/**
*/
inline const char* CScriptRead::GetTokenStringPtr( void ) const
{
    return m_lpszTokenString;
}

//------------------------------------------------------------------------------
/**
*/
inline float CScriptRead::GetTokenNumber( void ) const
{
    return m_fTokenNumber;
}

//------------------------------------------------------------------------------
/**
*/
inline BYTE CScriptRead::GetC( void )
{
    return ( ( m_iBufferPos < m_iBufferSize ) ? m_pbyBuffer[m_iBufferPos++] : 0xFF );
}

//------------------------------------------------------------------------------
/**
*/
inline BYTE CScriptRead::GetNext( void ) const
{
    return ( ( m_iBufferPos < m_iBufferSize ) ? m_pbyBuffer[m_iBufferPos] : 0xFF );
}

//------------------------------------------------------------------------------
/**
*/
inline void CScriptRead::UnGet( void )
{
    --m_iBufferPos; 
}

#endif // _PROGRAMCOMMON_WZSCRIPTREAD_H_