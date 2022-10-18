#pragma once

#include "WriteLogMng.h"
#include "BaseLogData.h"

#define	MAX_TEMPINFO_SIZE	32

class DLL_DECLARE BaseLog
{
public:
	BaseLog();
    virtual ~BaseLog() {}

protected:
	//int				m_nAddInfoCount;
	BOOL		 	m_bRun;
	BOOL			m_bWriteText;
	CWriteLogMng	m_LogMng;
	//TCHAR			m_szAddInfo[MAX_ADDINFO_SIZE+1];

public:
	BOOL	Init( BYTE byLogOption, TCHAR* pszLogFilePrefix, TCHAR* pszBinDirName, TCHAR* pszTextDirName );
//	VOID	WriteBaseData( BaseLogData* pData, const int nLogType, const TCHAR* szAccountID, const TCHAR* szCharName, const TCHAR* szUserIP );
	BOOL	IsRun(){ return m_bRun; }


	//VOID	InitAddInfo();
	//VOID	AddAddInfo( TCHAR * tszAddInfo );
};
