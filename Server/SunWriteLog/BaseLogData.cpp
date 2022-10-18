#include "stdafx.h"
#include ".\baselogdata.h"
#include <LogTimeFormat.h>
//#include "afxcom_.h"

#ifndef __NA_963_LOG_LIB_VERSION_REFACTORING //노디파인

BaseLogData::BaseLogData(VOID)
{
	//멤버변수 초기화.
}

BaseLogData::~BaseLogData(VOID)
{
}

VOID BaseLogData::AddAddInfo( TCHAR * tszAddInfo )
{
	// 128일때 기록못함
	// 128에 '\0' 들어가기 때문
	if( MAX_ADDINFO_SIZE-1 < m_byAddInfoCount ) return;

	_sntprintf( &m_szAddInfo[m_byAddInfoCount], (MAX_ADDINFO_SIZE-m_byAddInfoCount), tszAddInfo );
	m_szAddInfo[MAX_ADDINFO_SIZE]='\0';
	m_byAddInfoCount += (int)_tcslen(&m_szAddInfo[m_byAddInfoCount]);
	if( m_byAddInfoCount > MAX_ADDINFO_SIZE )
		m_byAddInfoCount = MAX_ADDINFO_SIZE;
}


VOID	BaseLogData::Init()
{
	//구조체 관련 초기화
	//m_BaseData.m_nLogType		= 0;
	m_szWriteTime[0] = '\0';
	m_szAccoutID[0]	= '\0';
	m_szCharName[0]	= '\0';
	m_szUserIP[0]	= '\0';
	m_szAddInfo[0]	= '\0';

	m_byBaseTimeLen = 0;
	m_byBaseIDLen = 0;
	m_byBaseNameLen = 0;
	m_byBaseIPLen = 0;

	// addinfo 초기화
	m_byAddInfoCount	= 0;
	m_szAddInfo[0]	= '\0';
}

/*
VOID BaseLogData::SetBaseData( const int nLogType, const TCHAR * szAccountID, 
							  const TCHAR * szCharName, const TCHAR* szUserIP )
{
	SetLogType( nLogType );
	SetAccountID( szAccountID );
	SetCharName( szCharName );
	SetUserIP( szUserIP );
}
*/

VOID	BaseLogData::SetWriteTime()
{
	memset( m_szWriteTime, 0, (MAX_STRING_TIME_SIZE_V1+1) * sizeof(TCHAR) );
	m_byBaseTimeLen = (BYTE)CLogTimeFormat::GetTime( 3, m_szWriteTime, MAX_STRING_TIME_SIZE_V1 * sizeof(TCHAR) );
	if( m_byBaseTimeLen > MAX_STRING_TIME_SIZE_V1)
		m_byBaseTimeLen = MAX_STRING_TIME_SIZE_V1;
}

VOID	BaseLogData::SetAccountID( const TCHAR* pszID, DWORD MaxSize )
{
	if( !pszID )
	{
		m_szAccoutID[0] = '\0';
		return;
	}

	ASSERT( MAX_ACCOUT_ID_SIZE_V1 == MaxSize);
	
	_tcsncpy( m_szAccoutID, pszID, MAX_ACCOUT_ID_SIZE_V1 );	
	m_szAccoutID[MAX_ACCOUT_ID_SIZE_V1] = '\0';
	m_byBaseIDLen = (BYTE)_tcslen(m_szAccoutID);
	if( m_byBaseIDLen > MAX_ACCOUT_ID_SIZE_V1 )
		m_byBaseIDLen = MAX_ACCOUT_ID_SIZE_V1;


}

VOID	BaseLogData::SetCharName( const TCHAR* pszName, DWORD MaxSize )
{
	if( !pszName )
	{
		m_szCharName[0] = '\0';
		return;
	}

	ASSERT( MAX_CHAR_NAME_SIZE_V1 == MaxSize );
		
	
	_tcsncpy( m_szCharName, pszName, MAX_CHAR_NAME_SIZE_V1 );
	m_szCharName[MAX_CHAR_NAME_SIZE_V1] = '\0';
	m_byBaseNameLen = (BYTE)_tcslen( m_szCharName );
	if( m_byBaseNameLen > MAX_CHAR_NAME_SIZE_V1)
		m_byBaseNameLen = MAX_CHAR_NAME_SIZE_V1;
}

/*
VOID	BaseLogData::SetAddInfo( const TCHAR* pszAddInfo )
{
	if( !pszAddInfo )
	{
		m_szAddInfo[0] = '\0';
		return;
	}

	_tcsncpy( m_szAddInfo, pszAddInfo, MAX_ADDINFO_SIZE  );
	m_szAddInfo[MAX_ADDINFO_SIZE] = '\0';
}
*/
VOID	BaseLogData::SetUserIP( const TCHAR* pszUserIP, DWORD MaxSize )
{
	if( !pszUserIP )
	{
		m_szAddInfo[0] = '\0';
		return;
	}
	
	ASSERT( MAX_IP_ADDR_SIZE_V1 == MaxSize );		

	_tcsncpy( m_szUserIP, pszUserIP, MAX_IP_ADDR_SIZE_V1 );
	m_szAddInfo[MAX_IP_ADDR_SIZE_V1] = '\0';
	m_byBaseIPLen = (BYTE)_tcslen( m_szUserIP );
	if( m_byBaseIPLen > MAX_IP_ADDR_SIZE_V1 )
		m_byBaseIPLen = MAX_IP_ADDR_SIZE_V1;
}

//pszBaseData의 크기는 nSize의 크기보다 1크다.
VOID	BaseLogData::GetBaseDataText( TCHAR* pszBaseData, int nMaxSize )
{
	if( !pszBaseData )	return;

	int iSize = _sntprintf( pszBaseData, nMaxSize-1, "%d,%d,%s,%s,%s,%s,%s"
		, m_Category
		, m_LogType
		, m_szAccoutID
		, m_szCharName
		, m_szUserIP
		, m_szWriteTime
		, m_szAddInfo
		);

	pszBaseData[iSize] = '\0';
}

VOID BaseLogData::Serialize( CLogArchive& rArchive, BOOL bAddText)
{	
	SetWriteTime();
	
	rArchive <<	m_Category;
	rArchive <<	(DWORD)BASEDATA_LAST_VERSION;
	rArchive << m_LogType;
	//바이너리
	switch(BASEDATA_LAST_VERSION)
	{
	case 1:		
		rArchive << m_byBaseTimeLen;
		rArchive.Write( m_szWriteTime, m_byBaseTimeLen );
		
		rArchive << m_byBaseIDLen;
		rArchive.Write( m_szAccoutID, m_byBaseIDLen );
		
		rArchive << m_byBaseNameLen;
		rArchive.Write( m_szCharName, m_byBaseNameLen );
		
		rArchive << m_byBaseIPLen;
		rArchive.Write( m_szUserIP, m_byBaseIPLen );					//< 클라이언트IP

		rArchive << m_byAddInfoCount;
		rArchive.Write( m_szAddInfo, m_byAddInfoCount );
		break;
	}

	//스냅샷은 택스트 로그 안 남긴다.
	if( bAddText && ( m_LogType < SNAPSHOT_REGULAR || m_LogType > SNAPSHOT_MAX ) )
	{
		GetBaseDataText( m_szBaseText, MAX_TEXT_SIZE * sizeof(TCHAR) );
		rArchive.WriteText(m_szBaseText, (MAX_TEXT_SIZE + 1) * sizeof(TCHAR) );
	}	
}

VOID BaseLogData::Read(CLogArchive& rArchive)
{
	rArchive >> m_Category;
	rArchive >> m_BaseReadVersion;

	switch(m_BaseReadVersion)
	{
	case 1:			
		rArchive >> m_LogType;

		rArchive >> m_byBaseTimeLen;
		rArchive.Read( m_szWriteTime, m_byBaseTimeLen );
		if( m_byBaseTimeLen < sizeof(m_szWriteTime) )
			m_szWriteTime[m_byBaseTimeLen] = '\0';
		else
			ASSERT(FALSE);

		rArchive >> m_byBaseIDLen;
		rArchive.Read( m_szAccoutID, m_byBaseIDLen );
		if( m_byBaseIDLen < sizeof(m_szAccoutID) )
			m_szAccoutID[m_byBaseIDLen] = '\0';
		else
			ASSERT(FALSE);

		rArchive >> m_byBaseNameLen;
		rArchive.Read( m_szCharName, m_byBaseNameLen );
		if( m_byBaseNameLen < sizeof(m_szCharName) )
			m_szCharName[m_byBaseNameLen] = '\0';
		else
			ASSERT(FALSE);

		rArchive >> m_byBaseIPLen;
		rArchive.Read( m_szUserIP, m_byBaseIPLen );					//< 클라이언트IP
		if( m_byBaseIPLen < sizeof(m_szUserIP) )
			m_szUserIP[m_byBaseIPLen] = '\0';
		else
			ASSERT(FALSE);

		rArchive >> m_byAddInfoCount;
		rArchive.Read( m_szAddInfo, m_byAddInfoCount );
		if( m_byAddInfoCount < sizeof(m_szAddInfo) )
			m_szAddInfo[m_byAddInfoCount] = '\0';
		else
			ASSERT(FALSE);
		break;
	}
}

#endif //__NA_963_LOG_LIB_VERSION_REFACTORING