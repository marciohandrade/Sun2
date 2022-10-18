// wzarchive.h

#ifndef _PROGRAMCOMMON_WZARCHIVE_H_
#define _PROGRAMCOMMON_WZARCHIVE_H_

#include "discaccess.h"
#include "wzlist.h"

// Chunk ����
#pragma pack (1)

struct St_ChunkInfo
{
    unsigned short m_nID;
    unsigned int m_nSize;
};

#pragma pack ()

// Chunk ���� ����
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
    // ������/�Ҹ���
    CWzArchive( void );
    ~CWzArchive( void );

    // DiscAccess ������ ����/���
    void SetDiscAccess( CDiscAccess* pDiscAccess );
    void GetDiscAccess( CDiscAccess** ppDiscAccess ); 

    // ���� �ε�(���� ���� ����)
    BOOL LoadFile( const char* lpszFileName );
    // ���� �ε�(�ܺο��� ���� ����)
    BOOL LoadFile( const char* lpszFileName, BYTE* pbyBuffer, int iSize );

    // ���� ����(���ο��� ����)
    BOOL SaveFile( const char* lpszFileName );
    // ���� ����(�ܺο��� ���� ����)
    BOOL SaveFile( const char* lpszFileName, DA_FILE dfHandle );

    // ���� �ݱ�
    void Close( void );

    // ���� ����/���
    void SetVersion( DWORD dwVersion );
    DWORD GetVersion( void ) const;

    // ���� ���ΰ�?
    BOOL IsStoring( void ) const;

    // �б�/����
    UINT Read( void* lpBuffer, UINT iSize );
    UINT Write( const void* lpBuffer, UINT iSize );

    // �̸� �б�/����
    UINT ReadName( char* lpszName, char** ppNameAlloc = NULL );
    UINT WriteName( const char* lpszName );

    // �ؽ�Ʈ ����
    void WriteText( const char* lpszFormat, ... );
    void WriteHexText( const void* pBuffer, int iSize );

    // ���� ������ �̵�
    DWORD Seek( LONG lDistanceToMove, ENUM_SEEK_ORIGIN eSeekOrigin );

    // ûũ ���� ����(�����)
    void SetBeginningOfChunk( unsigned short nID );
    // ûũ �� ����(�����)
    void SetEndOfChunk( void );

    // ���� ûũ �б�
    St_ChunkInfo ReadChunk( void );
    // �ش� ûũ �ǳʶٱ�
    void SkipCurrentChunk( St_ChunkInfo ci );

    // ���ϸ� ���
    const char* GetFileName( void );

    // operator >>
    template <class T>
    CWzArchive& operator >> ( T& Data );

    // operator <<
    template <class T>
    CWzArchive& operator << ( const T& Data );

private:
    // ����Ÿ �ʱ�ȭ
    void Clear( void );

    // ���� DiscAccess ����/����
    void CreateDiscAccess( void );
    void ReleaseDiscAccess( void );

private:
    CDiscAccess* m_pDiscAccess;
    BOOL m_bDiscAccessGenerated;

    CWzList<St_ChunkStack> m_lstChunkStack;

    char m_szFileName[MAX_PATH];
    DA_FILE m_dfFile;

    DWORD m_dwVersion;      // ���� ����
    BOOL m_bStoring;        // �������ΰ�?
    BOOL m_bInnerData;      // ���� ����Ÿ ��� ����
    BYTE* m_pBuffer;        // ���� ����
    int m_iBufferSize;      // ���� ũ��
    int m_iPosInBuffer;     // ���ۻ��� ���� ��ġ
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