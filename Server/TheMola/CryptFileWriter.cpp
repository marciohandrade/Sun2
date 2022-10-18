
#include "Structure.h"
#include "Crypt.RndKey.h"
#include "CryptFileWriter.h"
#include <stdio.h>
#include <stdlib.h>

CryptFileWriter::~CryptFileWriter()
{
	if( m_pChunk )
	{
		delete [] m_pChunk;
		m_pChunk = NULL;
	}
}

//BYTE __buffer[5000];

BOOL	CryptFileWriter::Create( const CHAR* pFileName, const DWORD dwKey, const DWORD dwMaxCount )
{
	if( !pFileName )
		return FALSE;

	OFSTRUCT pOF;
	HANDLE hFILE = (HANDLE)(__int64)OpenFile( pFileName, &pOF, OF_WRITE|OF_CREATE );

	return Create( hFILE, dwKey, dwMaxCount );
}

BOOL	CryptFileWriter::Create( const HANDLE hFD, const DWORD dwKey, const DWORD dwMaxCount )
{
	if( hFD == INVALID_HANDLE_VALUE )
		return FALSE;

	SetFD( hFD );

	SetFilePointer( hFD, 0, NULL, FILE_BEGIN );
	DWORD dwFixCount = 0;
	if( !dwMaxCount )
	{
		DWORD dwFileSize = GetFileSize( GetFD(), NULL );
		dwFixCount = (DWORD)(dwFileSize/TOKEN_SIZE);
		DWORD dwRemain = (DWORD)(dwFileSize%TOKEN_SIZE);
		if( dwRemain )
			++dwFixCount;
	}
	else
		dwFixCount = dwMaxCount;

	m_dwKey = m_dwLKey = dwKey;

	DWORD file_size = ( (DWORD)sizeof(CRYPT_FILE_TOKEN) ) * dwFixCount;	//32B*count
	file_size = BUF_ALIGN( file_size, MIN_FILE_DATA_SIZE );
	file_size += sizeof(CRYPT_FILE_HEADER);
	//DWORD size = (DWORD)(sizeof(CRYPT_FILE_CHUNK)/sizeof(DWORD));

	//m_pChunk = (CRYPT_FILE_CHUNK_RENEW*)malloc(file_size);
	m_pChunk = (CRYPT_FILE_CHUNK_RENEW*)new BYTE[file_size];

	DWORD* pPtr = (DWORD*)m_pChunk;

	DWORD divide_size = (DWORD) (file_size/sizeof(DWORD)) ;
	for( DWORD i=1 ; i<=divide_size ; ++i )
	{
		m_dwLKey = RANDOMKEY::GetDBProxyKey(m_dwLKey);
		*pPtr = m_dwLKey;
		pPtr++;
	}

	m_pChunk->Header.byW = 'W';
	m_pChunk->Header.byZ = 'z';
	m_pChunk->Header.byC = 'C';
	m_pChunk->Header.byF = 'F';
	m_pChunk->Header.INFO.dwRndKey = m_dwKey^m_dwLKey;
	//m_pChunk->Header.INFO.wJumpIndex = 0;
	m_dwCount = 0;
	m_dwMaxCount = dwFixCount;
	m_dwSize = 0;
	m_pLastINFO = &(m_pChunk->Header.INFO);

	//..wSize;
	return TRUE;
}


//+Decrypt
INT		CryptFileWriter::Read( BYTE* OUT pBuffer, const DWORD rLen )
{
//	DWORD dwKey = m_pChunk->Header.dwRndKey^m_dwKey;
	return 0;
}

//+Encrypt
INT		CryptFileWriter::Write( BYTE* IN pBuffer, const DWORD wLen )
{
	if( wLen > TOKEN_SIZE || !pBuffer )
		return -1;

	DWORD count = m_dwCount++;

	//32B
	CRYPT_FILE_TOKEN* pInfo = (CRYPT_FILE_TOKEN*)m_pChunk->KeyData;
	pInfo = pInfo+count;

	//pInfo->INFO.wJumpIndex = (WORD)m_dwCount;
	DWORD dwRndKey = pInfo->INFO.dwRndKey = RANDOMKEY::GetDBProxyKey(m_dwLKey);
	memcpy( pInfo->TOKEN, pBuffer, wLen );

	DWORD size = (DWORD)(sizeof(CRYPT_FILE_TOKEN)/sizeof(DWORD));
	DWORD* pdwInfo = (DWORD*)pInfo;
	for( DWORD i=0 ; i<size ; ++i )
	{
		*pdwInfo ^= m_dwLKey;
		pdwInfo++;
	}

	m_dwSize += (DWORD)wLen;
	m_dwLKey = dwRndKey;

	return 0;
}

BOOL	CryptFileWriter::Save( BYTE* IN pBuffer, DWORD IN dwLen )
{
	if( pBuffer )
	{
		CRYPT_FILE_HEADER* pHeader = (CRYPT_FILE_HEADER*)pBuffer;
		pHeader->INFO.dwSize = m_dwSize;

		DWORD file_size = ( (DWORD)sizeof(CRYPT_FILE_TOKEN) ) * m_dwMaxCount;	//32B*count
		file_size = BUF_ALIGN( file_size, MIN_FILE_DATA_SIZE );
		file_size += sizeof(CRYPT_FILE_HEADER);

		return CryptFile::Save( pBuffer, file_size );
	}
	return FALSE;
}

