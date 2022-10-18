#include "stdafx.h"
#include ".\actiondata.h"

#ifndef __NA_963_LOG_LIB_VERSION_REFACTORING //�������

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
	
	BaseLogData::Serialize(rArchive, bAddText);

	rArchive << (DWORD)ACTIONDATA_LAST_VERSION;
	//���̳ʸ�
	switch(ACTIONDATA_LAST_VERSION)
	{
	case 1:
		rArchive << m_MapCode;
		rArchive << m_MapX;
		rArchive << m_MapY;
		rArchive << m_MapZ;
		break;	
	}	

	//�ؽ�Ʈ �߰�. �������� �ý�Ʈ �α� �� �����.
	if( bAddText && ( m_LogType < SNAPSHOT_REGULAR || m_LogType > SNAPSHOT_MAX ) )
	{		
		GetBodyDataText( m_szBodyText, MAX_TEXT_SIZE * sizeof(TCHAR)  );		
	}	
}

VOID ActionData::Read(CLogArchive& rArchive)
{
	BaseLogData::Read(rArchive);

	rArchive >> m_ReadVersion;
	//���̳ʸ�
	switch(m_ReadVersion)
	{
	case 1:
		rArchive >> m_MapCode;
		rArchive >> m_MapX;
		rArchive >> m_MapY;
		rArchive >> m_MapZ;
		break;	
	}	
}
#endif