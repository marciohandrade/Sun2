#include "stdafx.h"
#include ".\skilldata.h"

#ifndef __NA_963_LOG_LIB_VERSION_REFACTORING //노디파인

SkillData::SkillData(VOID)
{
	SetCategory( LOG_SKILL );	
}

SkillData::~SkillData(VOID)
{
}

VOID	SkillData::Init()
{
	BaseLogData::Init();

	m_SkillLevel = 0;					
	m_SkillUseCnt = 0;					
}

VOID	SkillData::GetBodyDataText( TCHAR* pszBodyData, int nSize )
{
	if( !pszBodyData )	return;

	_sntprintf( pszBodyData, nSize-1, ",%d,%u" 
		, m_SkillUseCnt 
		, m_SkillLevel
		);

	pszBodyData[nSize] = '\0';
}

VOID SkillData::Serialize( CLogArchive& rArchive, BOOL bAddText)
{
	BaseLogData::Serialize(rArchive, bAddText);

	rArchive << (DWORD)SKILLDATA_LAST_VERSION;
	//바이너리
	switch(SKILLDATA_LAST_VERSION)
	{
	case 1:
		rArchive << m_SkillLevel;						//스킬레벨
		rArchive << m_SkillUseCnt;						//스킬 사용회수
		break;
	}

	//텍스트 추가. 스냅샷은 택스트 로그 안 남긴다.
	if( bAddText && ( m_LogType < SNAPSHOT_REGULAR || m_LogType > SNAPSHOT_MAX ) )
	{		
		GetBodyDataText( m_szBodyText, MAX_TEXT_SIZE * sizeof(TCHAR)  );
		rArchive.WriteText(m_szBodyText, (MAX_TEXT_SIZE + 1) * sizeof(TCHAR) );
	}
}

VOID SkillData::Read(CLogArchive& rArchive)
{
	BaseLogData::Read(rArchive);

	rArchive >> m_ReadVersion;
	//바이너리
	switch(m_ReadVersion)
	{
	case 1:
		rArchive >> m_SkillLevel;						//스킬레벨
		rArchive >> m_SkillUseCnt;						//스킬 사용회수
		break;
	}
}

#endif