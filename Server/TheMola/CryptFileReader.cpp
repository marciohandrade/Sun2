
#include "Structure.h"
#include "CryptFile.h"
#include "CryptFileReader.h"
#include <stdio.h>
#include <stdlib.h>

CryptFileReader::~CryptFileReader()
{
	m_dwCount		= 0;
	m_dwMaxCount	= 0;
	m_dwSize		= 0;
	m_pLastINFO		= NULL;
	m_dwKey			= 0;
	m_dwLKey		= 0;
	if( m_pChunk )
	{
		memset( m_pChunk, 0, m_dwBufferSize );
		m_dwBufferSize = 0;
		delete [] m_pChunk;
		m_pChunk = NULL;
	}
}

BOOL	CryptFileReader::Open( const CHAR* pFileName, const DWORD dwKey )
{
	if( !pFileName )
		return FALSE;

	OFSTRUCT pOF;
	HANDLE hFILE = (HANDLE)(__int64)OpenFile( pFileName, &pOF, OF_READ );

	if( hFILE == INVALID_HANDLE_VALUE )
		return FALSE;

	SetFD( hFILE );

	DWORD dwFileSize = GetFileSize( GetFD(), NULL );

	//m_pChunk = (CRYPT_FILE_CHUNK_RENEW*)malloc( dwFileSize );
	m_pChunk = (CRYPT_FILE_CHUNK_RENEW*)new BYTE[dwFileSize];
	m_dwBufferSize = dwFileSize;

	// <NOTE>
	//   HEADER + DATA[] 로 계산할 것
	//

	if( Load( (BYTE*)GetChunk(), dwFileSize ) == FALSE )
	{
		printf( "Can't load file\n" );
		return FALSE;
	}

	if( !_InitInfo( dwKey ) )
	{
		printf( "File Incorrupt\n" );
		return FALSE;
	}	

	return TRUE;
}

BOOL	CryptFileReader::_InitInfo( const DWORD dwKey )
{
	m_dwKey = m_dwLKey = dwKey;

	if( !IsEncrypted( (BYTE*)&(m_pChunk->Header) ) )
	{
		return FALSE;
	}

	m_dwLKey = m_pChunk->Header.INFO.dwRndKey^m_dwKey;
	//m_pChunk->Header.INFO.wJumpIndex = 0;
	m_dwCount = 0;
	//m_dwMaxCount = dwMaxCount;
	m_dwSize = m_pChunk->Header.INFO.dwSize;
	m_pLastINFO = &(m_pChunk->Header.INFO);

	//..wSize;
	return TRUE;
}

BOOL	CryptFileReader::ReadEncryptedBuffer( const BYTE* pBuffer, const DWORD dwBufSize, const DWORD dwKey )
{
	if( !pBuffer )
		return FALSE;

	m_pChunk = (CRYPT_FILE_CHUNK_RENEW*)new BYTE[dwBufSize];
	memcpy( m_pChunk, pBuffer, dwBufSize );		//원본 보호 목적
	m_dwBufferSize = dwBufSize;

	if( !_InitInfo( dwKey ) )
	{
		printf( "Invalid Stream\n" );
		return FALSE;
	}

	return TRUE;
}

BOOL	CryptFileReader::IsEncrypted( const BYTE* pszBufferHeadPtr )
{
	DWORD dwResult = 0;
	BOOL bChk = FALSE;
	CRYPT_FILE_HEADER* pHeader = (CRYPT_FILE_HEADER*)pszBufferHeadPtr;

	bChk = pHeader->byW == 'W';
	dwResult |= bChk<<0;
	bChk = pHeader->byZ == 'z';
	dwResult |= bChk<<1;
	bChk = pHeader->byC == 'C';
	dwResult |= bChk<<2;
	bChk = pHeader->byF == 'F';
	dwResult |= bChk<<3;

	if( dwResult != 0xF )
		return FALSE;
	return TRUE;
}

//BOOL	CryptFileReader::IsEncrypted()
//{
//	DWORD dwResult = 0;
//	BOOL bChk = FALSE;
//	bChk = m_pChunk->Header.byW == 'W';
//	dwResult |= bChk<<0;
//	bChk = m_pChunk->Header.byZ == 'z';
//	dwResult |= bChk<<1;
//	bChk = m_pChunk->Header.byC == 'C';
//	dwResult |= bChk<<2;
//	bChk = m_pChunk->Header.byF == 'F';
//	dwResult |= bChk<<3;
//
//	if( dwResult != 0xF )
//		return FALSE;
//	return TRUE;
//}


//+Decrypt
INT		CryptFileReader::Read( BYTE* OUT pBuffer, const DWORD rLen )
{
	if( rLen > TOKEN_SIZE || !pBuffer )
		return -1;

	DWORD count = m_dwCount++;

	//32B
	CRYPT_FILE_TOKEN* pInfo = (CRYPT_FILE_TOKEN*)m_pChunk->KeyData;
	pInfo = pInfo+count;

	//pInfo->INFO.wJumpIndex = (WORD)m_dwCount;

	DWORD size = (DWORD)(sizeof(CRYPT_FILE_TOKEN)/sizeof(DWORD));
	DWORD* pdwInfo = (DWORD*)pInfo;
	for( DWORD i=0 ; i<size ; ++i )
	{
		*pdwInfo ^= m_dwLKey;
		pdwInfo++;
	}

	memcpy( pBuffer, pInfo->TOKEN, rLen );

	m_dwLKey = pInfo->INFO.dwRndKey;

	return 0;
}

#include "WzCF.Block.cxx"

const BYTE* CryptInfoStream::GetCryptStream()
{
	return __CRYPTSTREAM;
}

