#include "stdafx.h"
#include ".\chatdata.h"

ChatData::ChatData(VOID)
{
	SetCategory( LOG_CHAT );
}

ChatData::~ChatData(VOID)
{
}


VOID ChatData::Init()
{
	BaseLogData::Init();

	SetCategory( LOG_CHAT );	

	m_Channel = 0;
	m_UserGuid = 0;
	m_ListenUserGuid = 0;
	ZeroMemory( m_szListenCharName, (MAX_CHARNAME_LENGTH + 1) * sizeof(TCHAR)  );
	ZeroMemory( m_szContext, (MAX_CHATMSG_SIZE + 1) * sizeof(TCHAR)  );
	m_MapCode = 0;
	m_RoomNo = 0;	

	m_byNameLen = 0;
	m_byTextLen = 0;
}

VOID	ChatData::SetListenCharName( TCHAR* pszName, DWORD MaxSize )
{
	if( !pszName )
		return;

	ASSERT( MAX_CHARNAME_LENGTH == MaxSize );

	_tcsncpy( m_szListenCharName, pszName, MAX_CHARNAME_LENGTH );
	m_szListenCharName[MAX_CHARNAME_LENGTH] ='\0';
	m_byNameLen = (BYTE)_tcslen( m_szListenCharName );
	if( m_byNameLen > MAX_CHARNAME_LENGTH)
		m_byNameLen = MAX_CHARNAME_LENGTH;
}

VOID	ChatData::SetContext( TCHAR* pszContext, DWORD MaxSize )
{
	if( !pszContext )
		return;

	ASSERT( MAX_CHATMSG_SIZE == MaxSize );

	_tcsncpy( m_szContext, pszContext, MAX_CHATMSG_SIZE );
	m_szContext[MAX_CHATMSG_SIZE] = '\0';
	m_byTextLen = (BYTE)_tcslen( m_szContext );
	if( m_byTextLen > MAX_CHATMSG_SIZE )
		m_byTextLen = MAX_CHATMSG_SIZE;
}

VOID	ChatData::GetBodyDataText( TCHAR* pszBodyData, int nSize )
{
	if( !pszBodyData )	return;

	_sntprintf( pszBodyData, nSize-1, ",%d,%d,%d,%d,%u,%s,%s"
		, m_Channel 
		, m_ListenUserGuid
		, m_RoomNo
		, m_UserGuid
		, m_MapCode
		, m_szContext 
		, m_szListenCharName 
		);

	pszBodyData[nSize] = '\0';
}

VOID ChatData::Serialize( CLogArchive& rArchive, BOOL bAddText)
{
	BYTE cSize = 0;

	BaseLogData::Serialize(rArchive, bAddText);

	rArchive << (WORD)CHATDATA_LAST_VERSION;
	//바이너리
	if(CHATDATA_LAST_VERSION >= 1)
	{
		_CHECK_LOG_SIZE_AND_WRITE( cSize, m_Channel, rArchive);
		_CHECK_LOG_SIZE_AND_WRITE( cSize, m_UserGuid, rArchive);
		_CHECK_LOG_SIZE_AND_WRITE( cSize, m_ListenUserGuid, rArchive);

		rArchive << m_byNameLen;
		rArchive.Write(m_szListenCharName, m_byNameLen);
		
		rArchive << m_byTextLen;
		rArchive.Write(m_szContext, m_byTextLen);

		_CHECK_LOG_SIZE_AND_WRITE( cSize, m_MapCode, rArchive);
		_CHECK_LOG_SIZE_AND_WRITE( cSize, m_RoomNo, rArchive);		
	}

	//텍스트 추가. 스냅샷은 택스트 로그 안 남긴다.
	if( bAddText && ( m_LogType < SNAPSHOT_REGULAR || m_LogType > SNAPSHOT_MAX ) )
	{		
		GetBodyDataText( m_szBodyText, MAX_TEXT_SIZE * sizeof(TCHAR)  );
		rArchive.WriteText(m_szBodyText, (MAX_TEXT_SIZE + 1) * sizeof(TCHAR) );
	}
}

VOID ChatData::Read(CLogArchive& rArchive)
{
	BYTE cSize = 0;

	BaseLogData::Read(rArchive);

	rArchive >> m_ReadVersion;
	//바이너리
	if(m_ReadVersion >= 1)
	{
		LOG_READ( cSize, m_Channel, rArchive );
		LOG_READ( cSize, m_UserGuid, rArchive );
		LOG_READ( cSize, m_ListenUserGuid, rArchive );
		
		rArchive >> m_byNameLen;
		rArchive.Read(m_szListenCharName, m_byNameLen);
		if( m_byNameLen < sizeof(m_szListenCharName) )
			m_szListenCharName[m_byNameLen] = '\0';
		else
			ASSERT(FALSE);

		rArchive >> m_byTextLen;
		rArchive.Read(m_szContext, m_byTextLen);
		if( m_byTextLen < sizeof(m_szContext))
			m_szContext[m_byTextLen] = '\0';
		else
			ASSERT(FALSE);

		LOG_READ( cSize, m_MapCode, rArchive );
		LOG_READ( cSize, m_RoomNo, rArchive );		
	}
}

