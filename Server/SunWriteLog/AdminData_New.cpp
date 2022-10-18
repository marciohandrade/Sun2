#include "StdAfx.h"
#include ".\admindata.h"

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
	ZeroMemory( m_szToCharName, (MAX_MONSTERNAME_LENGTH + 1) * sizeof(TCHAR) );

	m_byNameLen = 0;
}

VOID	AdminData::SetToCharName( const TCHAR* pszCharName, DWORD MaxSize )
{
	if( !pszCharName )
		return;

	ASSERT( MAX_MONSTERNAME_LENGTH <= MaxSize );

	_tcsncpy( m_szToCharName, pszCharName, MAX_MONSTERNAME_LENGTH );	
	m_szToCharName[MAX_MONSTERNAME_LENGTH] = '\0';
	m_byNameLen =  (BYTE)_tcslen(m_szToCharName);
	if( m_byNameLen > MAX_MONSTERNAME_LENGTH )
		m_byNameLen = MAX_MONSTERNAME_LENGTH;
}

VOID AdminData::Serialize( CLogArchive& rArchive, BOOL bAddText)
{
	BYTE cSize = 0;

	BaseLogData::Serialize(rArchive, bAddText);

	rArchive << (WORD)ADMINDATA_LAST_VERSION;
	//바이너리
	if(ADMINDATA_LAST_VERSION >= 1)
	{	
		_CHECK_LOG_SIZE_AND_WRITE( cSize, m_MapCode, rArchive);
		_CHECK_LOG_SIZE_AND_WRITE( cSize, m_MapX, rArchive);
		_CHECK_LOG_SIZE_AND_WRITE( cSize, m_MapY, rArchive);
		_CHECK_LOG_SIZE_AND_WRITE( cSize, m_MapZ, rArchive);		

		rArchive << m_byNameLen;
		rArchive.Write(m_szToCharName, m_byNameLen);					//상대방 캐릭터 명.		
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
	BYTE cSize = 0;
	BaseLogData::Read(rArchive);

	rArchive >> m_ReadVersion;
	//바이너리
	if(m_ReadVersion >= 1)
	{
		LOG_READ( cSize, m_MapCode, rArchive );
		LOG_READ( cSize, m_MapX, rArchive );
		LOG_READ( cSize, m_MapY, rArchive );
		LOG_READ( cSize, m_MapZ, rArchive );
		
		rArchive >> m_byNameLen;
		rArchive.Read(m_szToCharName, m_byNameLen);					//상대방 캐릭터 명.
		if( m_byNameLen < sizeof(m_szToCharName) )
			m_szToCharName[m_byNameLen] = '\0';
		else
			ASSERT(FALSE);		
	}
}

