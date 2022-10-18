#include "stdafx.h"
#include ".\skilldata.h"

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
	BYTE cSize = 0;

	BaseLogData::Serialize(rArchive, bAddText);

	rArchive << (WORD)SKILLDATA_LAST_VERSION;
	//바이너리
	if(SKILLDATA_LAST_VERSION >= 1)
	{
		_CHECK_LOG_SIZE_AND_WRITE( cSize, m_SkillLevel, rArchive);	//스킬레벨
		_CHECK_LOG_SIZE_AND_WRITE( cSize, m_SkillUseCnt, rArchive);	//스킬 사용회수
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
	BYTE cSize = 0;

	BaseLogData::Read(rArchive);

	rArchive >> m_ReadVersion;
	//바이너리
	if(m_ReadVersion >= 1)
	{
		LOG_READ( cSize, m_SkillLevel, rArchive );		//스킬레벨
		LOG_READ( cSize, m_SkillUseCnt, rArchive );		//스킬 사용회수
	}
}

