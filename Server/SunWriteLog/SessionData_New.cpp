#include "stdafx.h"
#include ".\sessiondata.h"
#include "LogTimeFormat.h"

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
	ASSERT( MAX_TIMEDATA_SIZE == MaxSize);

	_tcsncpy( m_tszLoginTime, pszTime, MAX_TIMEDATA_SIZE );
	m_tszLoginTime[MAX_TIMEDATA_SIZE] = '\0';
	m_byTimeLen = (BYTE)_tcslen( m_tszLoginTime );
	if( m_byTimeLen > MAX_TIMEDATA_SIZE)
		m_byTimeLen = MAX_TIMEDATA_SIZE;
}

VOID SessionData::Serialize( CLogArchive& rArchive, BOOL bAddText)
{
	BYTE cSize = 0;
	BaseLogData::Serialize(rArchive, bAddText);

	rArchive << (WORD)SESSIONDATA_LAST_VERSION;
	//바이너리
	if(SESSIONDATA_LAST_VERSION >= 1)
	{
		_CHECK_LOG_SIZE_AND_WRITE( cSize, m_LogoutType, rArchive);	//< eLOG_SESSION_LOGOUT_TYPE 참조
		
		rArchive << m_byTimeLen;
		rArchive.Write( m_tszLoginTime, m_byTimeLen );

		_CHECK_LOG_SIZE_AND_WRITE( cSize, m_UsedTime, rArchive);	//< eLOG_SESSION_LOGOUT_TYPE 참조
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
	BYTE cSize = 0;

	BaseLogData::Read(rArchive);

	rArchive >> m_ReadVersion;
	//바이너리
	if(m_ReadVersion >= 1)
	{
		LOG_READ( cSize, m_LogoutType, rArchive );					//< eLOG_SESSION_LOGOUT_TYPE 참조
		
		rArchive >> m_byTimeLen;		
		rArchive.Read( m_tszLoginTime, m_byTimeLen );
		if( m_byTimeLen < sizeof(m_tszLoginTime) )
			m_tszLoginTime[m_byTimeLen] = '\0';
		else
			ASSERT(FALSE);
		
		LOG_READ( cSize, m_UsedTime, rArchive );					//< 사용시간 (분단위)
	}
}

