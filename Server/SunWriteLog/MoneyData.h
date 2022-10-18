#pragma once

#include "baselogdata.h"

class DLL_DECLARE MoneyData : public BaseLogData
{
public:
	MoneyData(void);
	virtual ~MoneyData(void);

protected:	
	//Ver. 1
	__LOG_PROPERTY( WORD, MapCode );
	__LOG_PROPERTY( SHORT, MapX );
	__LOG_PROPERTY( SHORT, MapY );
	__LOG_PROPERTY( SHORT, MapZ );
	__LOG_PROPERTY( BYTE, byIDLen );	
	__LOG_PROPERTY( BYTE, byNameLen );
	__LOG_PROPERTY(WORD, ReadVersion );
	TCHAR 				m_szToAccountID[ID_MAX_LEN+1];			//< 파트너id
	TCHAR 				m_szToCharName[MAX_CHARNAME_LENGTH+1];			//< 파트너명
	__LOG_PROPERTY( DWORD64, Money );
	__LOG_PROPERTY( DWORD64, ToMoney );
	__LOG_PROPERTY( WORD, RoomKey );

protected:
	virtual	VOID GetBodyDataText( TCHAR* pszBodyData, int nSize );

public:
	VOID			Serialize( CLogArchive& rArchive, BOOL bAddText = FALSE );
	VOID			Read(CLogArchive& rArchive);

	VOID	Init();

	//setter Method
public:
	inline VOID		SetToAccountID( const TCHAR* pszID, DWORD MaxSize );
	inline TCHAR*	GetToAccoundID(){ return m_szToAccountID; }
	inline VOID		SetToCharName( const TCHAR* pszCharName, DWORD MaxSize );
	inline TCHAR*	GetToCharName(){ return m_szToCharName; }
};