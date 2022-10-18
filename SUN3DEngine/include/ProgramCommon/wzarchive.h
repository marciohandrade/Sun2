// wzarchive.h

#ifndef _PROGRAMCOMMON_WZARCHIVE_H_
#define _PROGRAMCOMMON_WZARCHIVE_H_

#include "discaccess.h"
#include "wzlist.h"

// Chunk 정보
#pragma pack (1)

struct St_ChunkInfo
{
    unsigned short m_nID;
    unsigned int m_nSize;
};

#pragma pack ()

// Chunk 보관 스택
struct St_ChunkStack
{
    unsigned short m_nID;
    DWORD m_dwBeginning;
};

//------------------------------------------------------------------------------
/**
    @class CWzArchive
*/
class CWzArchive
{
public:
    // 생성자/소멸자
    CWzArchive( void );
    ~CWzArchive( void );

    // DiscAccess 포인터 설정/얻기
    void SetDiscAccess( CDiscAccess* pDiscAccess );
    void GetDiscAccess( CDiscAccess** ppDiscAccess ); 

    // 파일 로딩(내부 버퍼 생성)
    BOOL LoadFile( const char* lpszFileName );
    // 파일 로딩(외부에서 버퍼 제공)
    BOOL LoadFile( const char* lpszFileName, BYTE* pbyBuffer, int iSize );

    // 파일 저장(내부에서 열기)
    BOOL SaveFile( const char* lpszFileName );
    // 파일 저장(외부에서 열린 파일)
    BOOL SaveFile( const char* lpszFileName, DA_FILE dfHandle );

    // 파일 닫기
    void Close( void );

    // 버전 설정/얻기
    void SetVersion( DWORD dwVersion );
    DWORD GetVersion( void ) const;

    // 저장 중인가?
    BOOL IsStoring( void ) const;

    // 읽기/쓰기
    UINT Read( void* lpBuffer, UINT iSize );
    UINT Write( const void* lpBuffer, UINT iSize );

    // 이름 읽기/쓰기
    UINT ReadName( char* lpszName, char** ppNameAlloc = NULL );
    UINT WriteName( const char* lpszName );

    // 텍스트 쓰기
    void WriteText( const char* lpszFormat, ... );
    void WriteHexText( const void* pBuffer, int iSize );

    // 파일 포인터 이동
    DWORD Seek( LONG lDistanceToMove, ENUM_SEEK_ORIGIN eSeekOrigin );

    // 청크 시작 설정(저장시)
    void SetBeginningOfChunk( unsigned short nID );
    // 청크 끝 설정(저장시)
    void SetEndOfChunk( void );

    // 현재 청크 읽기
    St_ChunkInfo ReadChunk( void );
    // 해당 청크 건너뛰기
    void SkipCurrentChunk( St_ChunkInfo ci );

    // 파일명 얻기
    const char* GetFileName( void );

    // operator >>
    template <class T>
    CWzArchive& operator >> ( T& Data );

    // operator <<
    template <class T>
    CWzArchive& operator << ( const T& Data );

private:
    // 데이타 초기화
    void Clear( void );

    // 내부 DiscAccess 생성/제거
    void CreateDiscAccess( void );
    void ReleaseDiscAccess( void );

private:
    CDiscAccess* m_pDiscAccess;
    BOOL m_bDiscAccessGenerated;

    CWzList<St_ChunkStack> m_lstChunkStack;

    char m_szFileName[MAX_PATH];
    DA_FILE m_dfFile;

    DWORD m_dwVersion;      // 파일 버젼
    BOOL m_bStoring;        // 저장중인가?
    BOOL m_bInnerData;      // 내부 데이타 사용 여부
    BYTE* m_pBuffer;        // 내부 버퍼
    int m_iBufferSize;      // 버퍼 크기
    int m_iPosInBuffer;     // 버퍼상의 현재 위치
};

//------------------------------------------------------------------------------
/**
*/
inline void CWzArchive::SetVersion( DWORD dwVersion )
{
    m_dwVersion = dwVersion; 
}

//------------------------------------------------------------------------------
/**
*/
inline DWORD CWzArchive::GetVersion( void ) const
{ 
    return m_dwVersion; 
}

//------------------------------------------------------------------------------
/**
*/
inline BOOL CWzArchive::IsStoring( void ) const
{
    return m_bStoring; 
}

//------------------------------------------------------------------------------
/**
*/
inline const char* CWzArchive::GetFileName( void )
{ 
    return m_szFileName; 
}

//------------------------------------------------------------------------------
/**
*/
template <class T>
inline CWzArchive& CWzArchive::operator >> ( T& Data )
{
    Read( &Data, sizeof( T ) );

    return *this;
}

//------------------------------------------------------------------------------
/**
*/
template <class T>
inline CWzArchive& CWzArchive::operator << ( const T& Data )
{
    Write( &Data, sizeof( T ) );

    return *this;
}

#endif // _PROGRAMCOMMON_WZARCHIVE_H_