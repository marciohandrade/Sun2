#include "stdafx.h"
#include ".\expdata.h"

#ifndef __NA_963_LOG_LIB_VERSION_REFACTORING //노디파인

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
	BaseLogData::Serialize(rArchive, bAddText);

	rArchive << (DWORD)EXPDATA_LAST_VERSION;
	//바이너리
	switch(EXPDATA_LAST_VERSION)
	{
	case 1:
		rArchive << m_NewLevel;
		rArchive << m_OldLevel;
		rArchive.Write( &m_NewExp, ITEM_DATA_EXP_SIZE_V1);
		rArchive.Write( &m_OldExp, ITEM_DATA_EXP_SIZE_V1);
		rArchive << m_DivideCount;
		break;
	case 2:
		rArchive << m_NewLevel;
		rArchive << m_OldLevel;
		rArchive.Write( &m_NewExp, ITEM_DATA_EXP_SIZE_V2);
		rArchive.Write( &m_OldExp, ITEM_DATA_EXP_SIZE_V2);
		rArchive << m_DivideCount;		
		break;
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
	BaseLogData::Read(rArchive);

	rArchive >> m_ReadVersion;
	//바이너리
	switch(m_ReadVersion)
	{
	case 1:
		rArchive >> m_NewLevel;
		rArchive >> m_OldLevel;
		rArchive.Read( &m_NewExp, ITEM_DATA_EXP_SIZE_V1);
		rArchive.Read( &m_OldExp, ITEM_DATA_EXP_SIZE_V1);
		rArchive >> m_DivideCount;
		break;
	case 2:
		rArchive >> m_NewLevel;
		rArchive >> m_OldLevel;
		rArchive.Read( &m_NewExp, ITEM_DATA_EXP_SIZE_V2);
		rArchive.Read( &m_OldExp, ITEM_DATA_EXP_SIZE_V2);
		rArchive >> m_DivideCount;		
		break;
	}
}

#endif