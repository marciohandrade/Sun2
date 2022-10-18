#pragma once

//=============================================================================================================================
/// CryptFileWriter class
/**
	@author	Lee Eun-pa <waverix@webzen.co.kr>
	@since	2006. 05. 15
	@remark
	- Webzen Crypt File
	@note
	- ���Ͽ� ���� ��ȣȭ/��ȣȭ ���̺귯��(TheMOLA)��
	�⺻������ ���ԵǾ� �ִ� ���� ���� Writer
	@history 
	- 
*/
//=============================================================================================================================


#include <windows.h>
#include <tchar.h>

#include "CryptFile.h"

struct sCRYPT_FILE_HEADER_INFO;
struct CRYPT_FILE_CHUNK_RENEW;

class CryptFileWriter : public CryptFile
{
public:
	CryptFileWriter() /*:
					m_dwFlag(0),
					m_dwMaxCount(0),
					m_dwIndex(0),
					m_pLastINFO(NULL),
					m_dwKey(0),
					m_dwLKey(0),*/
					{}
	~CryptFileWriter();

public:
	BOOL	Create( const CHAR* pFileName, const DWORD dwKey, const DWORD dwMaxCount );
	BOOL	Create( const HANDLE hFD, const DWORD dwKey, const DWORD dwMaxCount );
protected:
	INT		Read( BYTE* OUT pBuffer, const DWORD rLen );	//+Decrypt
	INT		Write( BYTE* IN pBuffer, const DWORD wLen );	//+Encrypt
	BOOL	Save( BYTE* IN pBuffer, DWORD IN dwLen );

	CRYPT_FILE_CHUNK_RENEW*	GetChunk() { return m_pChunk; }

private:
	DWORD						m_dwCount;
	DWORD						m_dwMaxCount;
	DWORD						m_dwSize;	// ������ ������ ũ��
	sCRYPT_FILE_HEADER_INFO*	m_pLastINFO;
	DWORD						m_dwKey;
	DWORD						m_dwLKey;
	CRYPT_FILE_CHUNK_RENEW*		m_pChunk;
};




