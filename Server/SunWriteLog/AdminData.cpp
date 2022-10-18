#include "StdAfx.h"
#include ".\admindata.h"

#ifndef __NA_963_LOG_LIB_VERSION_REFACTORING //노디파인

AdminData::AdminData(void)
{
	SetCategory( LOG_ADMIN );
}

AdminData::~AdminData(void)
{
}


VOID	AdminData::GetBodyDataText( TCHAR* pszBodyData, int nSize )
{
	if( !pszBodyData )	return;

	_sntprintf( pszBodyData, nSize-1, ",%u,%d,%d,%d,%s"
		, m_MapCode
		, m_MapX
		, m_MapY
		, m_MapZ
		, m_szToCharName
		);

	pszBodyData[nSize] = '\0';
}

VOID	AdminData::Init()
{
	BaseLogData::Init();

	SetCategory( LOG_ADMIN );	

	m_MapCode = 0;
	m_MapX = 0;
	m_MapY = 0;
	m_MapZ = 0;	
	ZeroMemory( m_szToCharName, (MAX_MONSTER_NAME_SIZE_V1 + 1) * sizeof(TCHAR) );

	m_byNameLen = 0;
}

VOID	AdminData::SetToCharName( const TCHAR* pszCharName, DWORD MaxSize )
{
	if( !pszCharName )
		return;
	
	ASSERT( MAX_MONSTER_NAME_SIZE_V1 <= MaxSize );
	
	_tcsncpy( m_szToCharName, pszCharName, MAX_MONSTER_NAME_SIZE_V1 );	
	m_szToCharName[MAX_MONSTER_NAME_SIZE_V1] = '\0';
	m_byNameLen =  (BYTE)_tcslen(m_szToCharName);
	if( m_byNameLen > MAX_MONSTER_NAME_SIZE_V1 )
		m_byNameLen = MAX_MONSTER_NAME_SIZE_V1;
}

VOID AdminData::Serialize( CLogArchive& rArchive, BOOL bAddText)
{
	BaseLogData::Serialize(rArchive, bAddText);

	rArchive << (DWORD)ADMINDATA_LAST_VERSION;
	//바이너리
	switch(ADMINDATA_LAST_VERSION)
	{
	case 1:
		rArchive << m_MapCode;
		rArchive << m_MapX;
		rArchive << m_MapY;
		rArchive << m_MapZ;
		rArchive << m_byNameLen;
		rArchive.Write(m_szToCharName, m_byNameLen);					//상대방 캐릭터 명.
		break;
	}
	

	//텍스트 추가. 스냅샷은 택스트 로그 안 남긴다.
	if( bAddText && ( m_LogType < SNAPSHOT_REGULAR || m_LogType > SNAPSHOT_MAX ) )
	{		
		GetBodyDataText( m_szBodyText, MAX_TEXT_SIZE * sizeof(TCHAR)  );
		rArchive.WriteText(m_szBodyText, (MAX_TEXT_SIZE + 1) * sizeof(TCHAR) );
	}
}

VOID AdminData::Read(CLogArchive& rArchive)
{
	BaseLogData::Read(rArchive);

	rArchive >> m_ReadVersion;
	//바이너리
	switch(m_ReadVersion)
	{
	case 1:
		rArchive >> m_MapCode;
		rArchive >> m_MapX;
		rArchive >> m_MapY;
		rArchive >> m_MapZ;		
		rArchive >> m_byNameLen;
		rArchive.Read(m_szToCharName, m_byNameLen);					//상대방 캐릭터 명.
		if( m_byNameLen < sizeof(m_szToCharName) )
			m_szToCharName[m_byNameLen] = '\0';
		else
			ASSERT(FALSE);
		break;
	}
}

#endif