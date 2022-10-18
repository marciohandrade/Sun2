#include "stdafx.h"
#include ".\actiondata.h"

ActionData::ActionData(VOID)
{
	SetCategory( LOG_ACTION );
}

ActionData::~ActionData(VOID)
{
}

VOID ActionData::Init()
{
	BaseLogData::Init();

	m_MapCode = 0;
	m_MapX = 0;
	m_MapY = 0;
	m_MapZ = 0;
}

VOID	ActionData::GetBodyDataText( TCHAR* pszData, int nSize )
{ 
	if( !pszData )	return;

	_sntprintf( pszData, nSize-1, ",%u,%d,%d,%d"
		, m_MapCode
		, m_MapX
		, m_MapY
		, m_MapZ
		);

	pszData[nSize] = '\0';
}

VOID ActionData::Serialize( CLogArchive& rArchive, BOOL bAddText)
{	
	BYTE cSize = 0;

	BaseLogData::Serialize(rArchive, bAddText);

	rArchive << (WORD)ACTIONDATA_LAST_VERSION;	

	//바이너리
	if(ACTIONDATA_LAST_VERSION >= 1)
	{
		_CHECK_LOG_SIZE_AND_WRITE( cSize, m_MapCode, rArchive);
		_CHECK_LOG_SIZE_AND_WRITE( cSize, m_MapX, rArchive);
		_CHECK_LOG_SIZE_AND_WRITE( cSize, m_MapY, rArchive);
		_CHECK_LOG_SIZE_AND_WRITE( cSize, m_MapZ, rArchive);		
	}	

	//텍스트 추가. 스냅샷은 택스트 로그 안 남긴다.
	if( bAddText && ( m_LogType < SNAPSHOT_REGULAR || m_LogType > SNAPSHOT_MAX ) )
	{		
		GetBodyDataText( m_szBodyText, MAX_TEXT_SIZE * sizeof(TCHAR)  );		
	}	
}

VOID ActionData::Read(CLogArchive& rArchive)
{
	BYTE cSize = 0;
	BaseLogData::Read(rArchive);

	rArchive >> m_ReadVersion;
	//바이너리
	if( m_ReadVersion >= 1)
	{	
		LOG_READ( cSize, m_MapCode, rArchive );
		LOG_READ( cSize, m_MapX, rArchive );
		LOG_READ( cSize, m_MapY, rArchive );
		LOG_READ( cSize, m_MapZ, rArchive );	
	}	
}
