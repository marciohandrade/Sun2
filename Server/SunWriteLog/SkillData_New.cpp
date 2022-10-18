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
	//���̳ʸ�
	if(SKILLDATA_LAST_VERSION >= 1)
	{
		_CHECK_LOG_SIZE_AND_WRITE( cSize, m_SkillLevel, rArchive);	//��ų����
		_CHECK_LOG_SIZE_AND_WRITE( cSize, m_SkillUseCnt, rArchive);	//��ų ���ȸ��
	}

	//�ؽ�Ʈ �߰�. �������� �ý�Ʈ �α� �� �����.
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
	//���̳ʸ�
	if(m_ReadVersion >= 1)
	{
		LOG_READ( cSize, m_SkillLevel, rArchive );		//��ų����
		LOG_READ( cSize, m_SkillUseCnt, rArchive );		//��ų ���ȸ��
	}
}

