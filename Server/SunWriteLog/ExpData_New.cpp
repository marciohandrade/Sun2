#include "stdafx.h"
#include ".\expdata.h"

ExpData::ExpData(VOID)
{
	SetCategory( LOG_EXP );
}

ExpData::~ExpData(VOID)
{
}

VOID	ExpData::Init()
{
	BaseLogData::Init();

	m_NewLevel		= 0;
	m_OldLevel		= 0;
	m_NewExp		= 0;
	m_OldExp		= 0;
	m_DivideCount	= 0;
}

VOID	ExpData::GetBodyDataText( TCHAR* pszBodyData, int nSize )
{
	if( !pszBodyData )	return;

	_sntprintf( pszBodyData, nSize-1, ",%d,%d,%d,%d,%d" 
		, m_DivideCount
		, m_NewExp
		, m_NewLevel
		, m_OldExp
		, m_OldLevel
		);

	pszBodyData[nSize] = '\0';
}

VOID ExpData::Serialize( CLogArchive& rArchive, BOOL bAddText)
{
	BYTE cSize = 0;

	BaseLogData::Serialize(rArchive, bAddText);

	rArchive << (WORD)EXPDATA_LAST_VERSION;
	//바이너리
	if(EXPDATA_LAST_VERSION >= 1)
	{
		_CHECK_LOG_SIZE_AND_WRITE( cSize, m_NewLevel, rArchive);
		_CHECK_LOG_SIZE_AND_WRITE( cSize, m_OldLevel, rArchive);
		_CHECK_LOG_SIZE_AND_WRITE( cSize, m_NewExp, rArchive);
		_CHECK_LOG_SIZE_AND_WRITE( cSize, m_OldExp, rArchive);
		_CHECK_LOG_SIZE_AND_WRITE( cSize, m_DivideCount, rArchive);
	}

	//텍스트 추가. 스냅샷은 택스트 로그 안 남긴다.
	if( bAddText && ( m_LogType < SNAPSHOT_REGULAR || m_LogType > SNAPSHOT_MAX ) )
	{		
		GetBodyDataText( m_szBodyText, MAX_TEXT_SIZE * sizeof(TCHAR)  );
		rArchive.WriteText(m_szBodyText, (MAX_TEXT_SIZE + 1) * sizeof(TCHAR) );
	}	
}

VOID ExpData::Read(CLogArchive& rArchive)
{
	BYTE cSize = 0;

	BaseLogData::Read(rArchive);

	rArchive >> m_ReadVersion;
	//바이너리
	if(m_ReadVersion >= 1)
	{
		LOG_READ( cSize, m_NewLevel, rArchive );
		LOG_READ( cSize, m_OldLevel, rArchive );
		LOG_READ( cSize, m_NewExp, rArchive );
		LOG_READ( cSize, m_OldExp, rArchive );
		LOG_READ( cSize, m_DivideCount, rArchive );
	}
}

