
#pragma once

#include <windows.h>
#include <tchar.h>

#include "CryptFile.h"

//
//

class CryptFileLoader
{
public:
	CryptFileLoader() { m_pFD = NULL; m_dwFlag = 0; }
	~CryptFileLoader();

protected:
	INT		Read( BYTE* OUT pBuffer, const DWORD rLen );	//+Decrypt

private:
	FILE*	m_pFD;
	DWORD	m_dwFlag;
};


class DBDecryptFileLoader : public CryptFileLoader
{
public:
	VOID	SetServiceMode( DWORD dwServiceMode ) { m_dwServiceMode = dwServiceMode; }
	DWORD	GetServiceMode() { return m_dwServiceMode; }

	BOOL	ReadStart( const CHAR* pFileName, QueryKey IN dbp = "DBP" );
	VOID	ReadEnd();

	BOOL	GetIdentification( BYTE* OUT pID );
	BOOL	GetPassword( BYTE* OUT pPWD );
private:
	DWORD	m_dwServiceMode;	// 0: 1: 2:
};


