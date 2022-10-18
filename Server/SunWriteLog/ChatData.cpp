#include "stdafx.h"
#include ".\chatdata.h"

#ifndef __NA_963_LOG_LIB_VERSION_REFACTORING //노디파인

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
	ZeroMemory( m_szListenCharName, (MAX_CHAR_NAME_SIZE_V1 + 1) * sizeof(TCHAR)  );
	ZeroMemory( m_szContext, (MAX_CHAT_CONTEXT_SIZE + 1) * sizeof(TCHAR)  );
	m_MapCode = 0;
	m_RoomNo = 0;	

	m_byNameLen = 0;
	m_byTextLen = 0;
}

VOID	ChatData::SetListenCharName( TCHAR* pszName, DWORD MaxSize )
{
	if( !pszName )
		return;

	ASSERT( MAX_CHAR_NAME_SIZE_V1 == MaxSize );
	
	_tcsncpy( m_szListenCharName, pszName, MAX_CHAR_NAME_SIZE_V1 );
	m_szListenCharName[MAX_CHAR_NAME_SIZE_V1] ='\0';
	m_byNameLen = (BYTE)_tcslen( m_szListenCharName );
	if( m_byNameLen > MAX_CHAR_NAME_SIZE_V1)
		m_byNameLen = MAX_CHAR_NAME_SIZE_V1;
}

VOID	ChatData::SetContext( TCHAR* pszContext, DWORD MaxSize )
{
	if( !pszContext )
		return;

	ASSERT( MAX_CHAT_CONTEXT_SIZE == MaxSize );
	
	_tcsncpy( m_szContext, pszContext, MAX_CHAT_CONTEXT_SIZE );
	m_szContext[MAX_CHAT_CONTEXT_SIZE] = '\0';
	m_byTextLen = (BYTE)_tcslen( m_szContext );
	if( m_byTextLen > MAX_CHAT_CONTEXT_SIZE )
		m_byTextLen = MAX_CHAT_CONTEXT_SIZE;
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
BaseLogData::Serialize(rArchive, bAddText);

	rArchive << (DWORD)CHATDATA_LAST_VERSION;
	//바이너리
	switch(CHATDATA_LAST_VERSION)
	{
	case 1:
		rArchive << m_Channel;
		rArchive << m_UserGuid;
		rArchive << m_ListenUserGuid;
		rArchive << m_byNameLen;
		rArchive.Write(m_szListenCharName, m_byNameLen);
		rArchive << m_byTextLen;
		rArchive.Write(m_szContext, m_byTextLen);
		rArchive << m_MapCode;
		rArchive << m_RoomNo;	
		break;
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
	BaseLogData::Read(rArchive);

	rArchive >> m_ReadVersion;
	//바이너리
	switch(m_ReadVersion)
	{
	case 1:
		rArchive >> m_Channel;
		rArchive >> m_UserGuid;
		rArchive >> m_ListenUserGuid;
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
		rArchive >> m_MapCode;
		rArchive >> m_RoomNo;	
		break;
	}
}

#endif