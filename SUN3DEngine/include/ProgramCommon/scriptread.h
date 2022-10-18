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
    // ������/�Ҹ���
    CScriptRead( void );
    ~CScriptRead( void );

    // DiscAccess ����
    void SetDiscAccess( CDiscAccess* pDiscAccess );

    // ����(����)
    BOOL Create( const char* lpszFileName );
    // ����(���۷κ���)
    BOOL CreateFromBuffer( const char* pBuffer, int iBufferSize );

    // ����
    void Destroy( void );

    // ��ū ���(Ư�� Ÿ���� ���� �� ��� ���ڸ� �ִ´�.)
    ENUM_TOKEN_TYPE GetToken( ENUM_TOKEN_TYPE ettTypeToGet = TT_ANYTYPE );
    // ���ڿ� ��ū ���
    const char* GetTokenStringPtr( void ) const;
    // ���� ��ū ���
    float GetTokenNumber( void ) const;

private:
    // ����Ÿ �ʱ�ȭ
    void Clear( void );

    // ���� DiscAccess ����/����
    void CreateDiscAccess( void );
    void ReleaseDiscAccess( void );

    // ����Ʈ �ϳ� ���(���� ��ġ�� �̵�)
    BYTE GetC( void );
    // ���� ��ġ�� ����Ʈ �ϳ� ���
    BYTE GetNext( void ) const;
    // ���� ��ġ�� �̵�
    void UnGet( void );

private:
    CDiscAccess* m_pDiscAccess;
    BOOL m_bDiscAccessGenerated;

    int m_iBufferSize;  // ���� ũ��
    BYTE* m_pbyBuffer;  // ����
    int m_iBufferPos;   // ���� ���� ��ġ

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