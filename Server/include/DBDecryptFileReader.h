#pragma once

//=============================================================================================================================
/// DBDecryptFileReader class
/**
	@author	Lee Eun-pa <waverix@webzen.co.kr>
	@since	2006. 05. 15
	@remark
	- Webzen Crypt File
	@note
	- 파일에 대한 암호화/복호화 라이브러리(TheMOLA)에
	  기본적으로 포함되어 있는 DBProxy를 위한 암호화 파일 Reader
	@history 
	- 
*/
//=============================================================================================================================


#include "CryptFileReader.h"

class DBDecryptFileReader : public CryptFileReader
{
public:
	DBDecryptFileReader() { _InitDBInfo(); }
	~DBDecryptFileReader() { _InitDBInfo(); }

	BOOL	ReadStart( const CHAR* pFileName, QueryKey IN dbp = "DBP" );
	BOOL	ReadStartFromBuffer( const BYTE* pSTREAM, QueryKey IN dbp = "DBP" );
	VOID	ReadEnd();

	const CHAR*	GetIdentification( DWORD dwServerMode ) {
		if( dwServerMode && dwServerMode<(4+1) )
			return ID_PWD[dwServerMode].m_pUserID;
		else
			return NULL;
	}
	const CHAR*	GetPassword( DWORD dwServerMode )
	{
		if( dwServerMode && dwServerMode<(4+1) )
			return ID_PWD[dwServerMode].m_pUserPWD;
		else
			return NULL;
	}

#ifdef __WAVERIX_THE_MOLA_UPDATE_VERSION2__
public:
	virtual BOOL	ILoad( eSCRIPT_CODE_TEMP scriptCode, BYTE* pSTREAM, DWORD dwSize ){ return FALSE; }
#endif //__WAVERIX_THE_MOLA_UPDATE_VERSION2__


private:
	VOID		_InitDBInfo()
	{
		memset( ID_PWD, 0, sizeof(ID_PWD) );
	}

	BOOL	_GetIdentification( BYTE* IN OUT pID );
	BOOL	_GetPassword( BYTE* IN OUT pPWD );

private:
	struct AuthInfoBlock
	{
		CHAR		m_pUserID[MAX_ID_SIZE+1];
		CHAR		m_pUserPWD[MAX_PWD_SIZE+1];
	} ID_PWD[4+1];
};
