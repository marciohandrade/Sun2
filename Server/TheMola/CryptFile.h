#pragma once

//=============================================================================================================================
/// CryptFile class
/**
	@author	Lee Eun-pa <waverix@webzen.co.kr>
	@since	2006. 05. 15
	@remark
	- Webzen Crypt File
	@note
	- 파일에 대한 암호화/복호화 라이브러리(TheMOLA)를 위한 기본 암호화 파일 I/O
	@history 
	- 
*/
//=============================================================================================================================


#include <windows.h>
#include <tchar.h>

#include "WzCF.Define.h"

class CryptFile
{
public:
	CryptFile() : m_hFD(INVALID_HANDLE_VALUE) {}
	~CryptFile();
public:
	BOOL	Load( BYTE* OUT pBuffer, DWORD IN OUT dwLen );
	BOOL	Save( BYTE* IN pBuffer, DWORD IN dwLen );
	VOID	Close();

public:
	HANDLE	GetFD() { return m_hFD; }
	VOID	SetFD( HANDLE IN hFD ) { m_hFD = hFD; }
private:
	HANDLE	m_hFD;
};



