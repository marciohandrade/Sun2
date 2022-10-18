#pragma once

#include "BaseLogData.h"

class DLL_DECLARE SessionData : public BaseLogData
{
public:
	SessionData(VOID);
	~SessionData(VOID);

protected:	
	//Ver. 1
	__LOG_PROPERTY( BYTE, LogoutType );									//< eLOG_SESSION_LOGOUT_TYPE 참조
	__LOG_PROPERTY( BYTE, byTimeLen );
	__LOG_PROPERTY(WORD, ReadVersion );
	TCHAR				m_tszLoginTime[MAX_TIMEDATA_SIZE+1];
	__LOG_PROPERTY( DWORD, UsedTime );									//< 사용시간 (분단위)

protected:
	virtual	VOID	GetBodyDataText( TCHAR* pszBodyData, int nSize );

public:
	VOID			Serialize( CLogArchive& rArchive, BOOL bAddText = FALSE );
	VOID			Read(CLogArchive& rArchive);

	VOID	Init();

	//setter Method
public:
	inline VOID		SetLoginTime( const TCHAR * pszTime, DWORD MaxSize );
	inline TCHAR*	GetLoginTime(){ return m_tszLoginTime; }
};