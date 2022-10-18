#include "stdafx.h"
#include ".\sessiondata.h"
#include "LogTimeFormat.h"

#ifndef __NA_963_LOG_LIB_VERSION_REFACTORING //노디파인

SessionData::SessionData(VOID)
{
	SetCategory( LOG_SESSION );	
}

SessionData::~SessionData(VOID)
{
}

VOID SessionData::Init()
{
	BaseLogData::Init();

	m_LogoutType	= 0;
	m_UsedTime		= 0;
	m_tszLoginTime[0] = '\0';;

	m_byTimeLen = 0;
}

VOID SessionData::GetBodyDataText( TCHAR* pszBodyData, int nSize )
{
	if( !pszBodyData )	return;

	_sntprintf( pszBodyData, nSize-1, ",%u,%u,%s", m_LogoutType, m_UsedTime, m_tszLoginTime );
	pszBodyData[nSize] = '\0';

}

VOID SessionData::SetLoginTime( const TCHAR * pszTime,  DWORD MaxSize)
{
	ASSERT( MAX_STRING_TIME_SIZE_V1 == MaxSize);

	_tcsncpy( m_tszLoginTime, pszTime, MAX_STRING_TIME_SIZE_V1 );
	m_tszLoginTime[MAX_STRING_TIME_SIZE_V1] = '\0';
	m_byTimeLen = (BYTE)_tcslen( m_tszLoginTime );
	if( m_byTimeLen > MAX_STRING_TIME_SIZE_V1)
		m_byTimeLen = MAX_STRING_TIME_SIZE_V1;
}

VOID SessionData::Serialize( CLogArchive& rArchive, BOOL bAddText)
{
	BaseLogData::Serialize(rArchive, bAddText);

	rArchive << (DWORD)SESSIONDATA_LAST_VERSION;
	//바이너리
	switch(SESSIONDATA_LAST_VERSION)
	{
	case 1:
		rArchive << m_LogoutType;									//< eLOG_SESSION_LOGOUT_TYPE 참조
		rArchive << m_byTimeLen;
		rArchive.Write( m_tszLoginTime, MAX_STRING_TIME_SIZE_V1 );
		rArchive << m_UsedTime;										//< 사용시간 (분단위)
		break;
	}

	//텍스트 추가. 스냅샷은 택스트 로그 안 남긴다.
	if( bAddText && ( m_LogType < SNAPSHOT_REGULAR || m_LogType > SNAPSHOT_MAX ) )
	{		
		GetBodyDataText( m_szBodyText, MAX_TEXT_SIZE * sizeof(TCHAR)  );
		rArchive.WriteText(m_szBodyText, (MAX_TEXT_SIZE + 1) * sizeof(TCHAR) );
	}
}

VOID SessionData::Read(CLogArchive& rArchive)
{
	BaseLogData::Read(rArchive);

	rArchive >> m_ReadVersion;
	//바이너리
	switch(m_ReadVersion)
	{
	case 1:
		rArchive >> m_LogoutType;									//< eLOG_SESSION_LOGOUT_TYPE 참조
		rArchive >> m_byTimeLen;
		rArchive.Read( m_tszLoginTime, MAX_STRING_TIME_SIZE_V1 );
		//rArchive.Read( m_tszLoginTime, m_byTimeLen );
		//if( m_byTimeLen < sizeof(m_tszLoginTime) )
		//	m_tszLoginTime[m_byTimeLen] = '\0';
		//else
		//	ASSERT(FALSE);
		rArchive >> m_UsedTime;										//< 사용시간 (분단위)
		break;
	}
}
#endif