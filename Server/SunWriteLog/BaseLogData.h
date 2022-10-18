#pragma once

#include "LogDataStruct.h"
#include "LogArchive.h"

#define		MAX_TEXT_SIZE	256

class DLL_DECLARE BaseLogData
{
public:
	BaseLogData(VOID);
	virtual ~BaseLogData(VOID);

protected:	
	//////////////////////////////////////////////////////////////
	//Base Data	
	__LOG_PROPERTY( eLOG_CATEGORY, Category );
	__LOG_PROPERTY( int, LogType );
	
	__LOG_PROPERTY( BYTE, byBaseTimeLen);
	__LOG_PROPERTY( BYTE, byBaseIDLen);
	__LOG_PROPERTY( BYTE, byBaseNameLen);
	__LOG_PROPERTY( BYTE, byBaseIPLen);
	__LOG_PROPERTY( BYTE, byAddInfoCount);
	__LOG_PROPERTY(WORD, BaseReadVersion );

	TCHAR				m_szWriteTime[MAX_TIMEDATA_SIZE+1];	
	TCHAR				m_szAccoutID[ID_MAX_LEN+1];	
	TCHAR				m_szCharName[MAX_CHARNAME_LENGTH+1];	
	TCHAR 				m_szUserIP[MAX_IPADDRESS_SIZE+1];					//< 클라이언트IP	
	TCHAR				m_szAddInfo[MAX_ADDINFO_SIZE+1];
	//////////////////////////////////////////////////////////////
	int					m_nAddInfoCount;

	TCHAR				m_szBaseText[MAX_TEXT_SIZE+1];
	TCHAR				m_szBodyText[MAX_TEXT_SIZE+1];

protected:	
	VOID					GetBaseDataText( TCHAR* pszBaseData, int nSize );
public:

	VOID					AddAddInfo( TCHAR * tszAddInfo );
	TCHAR*					GetAddInfo(){ return m_szAddInfo; }
	//VOID					SetBaseData( const int nLogType, const TCHAR * szAccountID, 
	//								const TCHAR * szCharName, const TCHAR* szUserIP );
	inline VOID				Init();	
	inline VOID				SetAccountID( const TCHAR* pszID, DWORD MaxSize );				//사용자가 설정
	inline TCHAR*			GetAccountID(){ return m_szAccoutID; }
	inline VOID				SetCharName( const TCHAR* pszName, DWORD MaxSize );			//사용자가 설정
	inline TCHAR*			GetCharName(){ return m_szCharName; }
	//inline VOID				SetAddInfo( const TCHAR* pszAddInfo );			//사용자가 설정
	inline VOID				SetUserIP( const TCHAR* pszUserIP, DWORD MaxSize );			//사용자가 설정	
	inline TCHAR*			GetUserIP(){ return m_szUserIP; }
	inline VOID				SetWriteTime();									//자동으로 설정
	inline TCHAR*			GetWriteTime(){ return m_szWriteTime; }

	virtual VOID			Serialize( CLogArchive& rArchive, BOOL bAddText = FALSE );	
	virtual VOID			Read(CLogArchive& rArchive);
};