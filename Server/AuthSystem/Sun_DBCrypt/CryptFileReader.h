#pragma once

//=============================================================================================================================
/// CryptFileReader class
/**
	@author	Lee Eun-pa <waverix@webzen.co.kr>
	@since	2006. 05. 15
	@remark
	- Webzen Crypt File
	@note
	- 파일에 대한 암호화/복호화 라이브러리(TheMOLA)에
	기본적으로 포함되어 있는 범용 파일 Reader
	@history 
	- 
*/
//=============================================================================================================================


#include "CryptFile.h"


//
//
//
#ifndef __SCRIPTCODE_IPARSER_H
#define __SCRIPTCODE_IPARSER_H

typedef DWORD				SCRIPT_CODE_T;

class IParser
{
public:
	virtual BOOL	ILoad( SCRIPT_CODE_T scriptCode, BYTE* pSTREAM, DWORD dwSize ) = 0;
};

#endif //__SCRIPTCODE_IPARSER_H
//
//
//



struct sCRYPT_FILE_HEADER_INFO;
struct CRYPT_FILE_CHUNK_RENEW;

class CryptFileReader : public CryptFile
#ifdef __WAVERIX_THE_MOLA_UPDATE_VERSION2__
	, public IParser
#endif //__WAVERIX_THE_MOLA_UPDATE_VERSION2__
{
public:
	CryptFileReader() : m_pChunk(NULL) {}
	~CryptFileReader();

public:
	static BOOL		IsEncrypted( const BYTE* pszBufferHeadPtr );

	DWORD	GetSize() { return m_dwSize; }
	BOOL	Open( const CHAR* pFileName, const DWORD dwKey/*, const DWORD dwMaxCount*/ );
	BOOL	ReadEncryptedBuffer( const BYTE* pBuffer, const DWORD dwBufSize, const DWORD dwKey );
protected:
	INT		Read( BYTE* OUT pBuffer, const DWORD rLen );	//+Decrypt

	CRYPT_FILE_CHUNK_RENEW*	GetChunk() { return m_pChunk; }

#ifdef __WAVERIX_THE_MOLA_UPDATE_VERSION2__
public:
	virtual BOOL	ILoad( SCRIPT_CODE_T scriptCode, BYTE* pSTREAM, DWORD dwSize ) { return FALSE; }
#endif //__WAVERIX_THE_MOLA_UPDATE_VERSION2__

private:
	BOOL	_InitInfo( const DWORD dwKey );

private:
	DWORD						m_dwBufferSize;
	DWORD						m_dwCount;
	DWORD						m_dwMaxCount;
	DWORD						m_dwSize;	// 쓰여진 데이터 크기
	sCRYPT_FILE_HEADER_INFO*	m_pLastINFO;
	DWORD						m_dwKey;
	DWORD						m_dwLKey;
	CRYPT_FILE_CHUNK_RENEW*		m_pChunk;
};

struct CryptInfoStream
{
	static const BYTE*		GetCryptStream();
};

