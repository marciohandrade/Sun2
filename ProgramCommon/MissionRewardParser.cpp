#include "StdAfx.h"
#include "MissionRewardParser.h"
#include <Struct.h>
#include "ItemInfoParser.h"

#if SUN_CODE_BACKUP
static VOID CheckValidItemCode( SLOTCODE code )
{
	if( 0 == code ) return;
	BASE_ITEMINFO * pInfo = ItemInfoParser::Instance()->GetItemInfo( code );
	FASSERT( pInfo && "보상아이템이 존재하지 않는 코드입니다." );
}
#endif

MissionRewardParser::MissionRewardParser():	m_pDataTable ( NULL )
{
}

MissionRewardParser::~MissionRewardParser()
{
	FASSERT( m_pDataTable == NULL);
}

VOID MissionRewardParser::Release()
{
	UnLoad();
	delete m_pDataTable;
	m_pDataTable = NULL;
}

VOID MissionRewardParser::Init(DWORD dwPoolSize)
{
	FASSERT( m_pDataTable == NULL );
	m_pDataTable = new util::SolarHashTable<MissionRewardInfo*>;
	m_pDataTable->Initialize( dwPoolSize );
}

VOID MissionRewardParser::Init( DWORD dwPoolSize, char * pszPackFileName )
{
	Init( dwPoolSize );
	SetPackFileName( pszPackFileName );
}

void MissionRewardParser::UnLoad()
{
    if (m_pDataTable == NULL) {
        return;
    };
    m_pDataTable->SetFirst();
    while (MissionRewardInfo* info = m_pDataTable->GetNext()) {
        delete info;
    }
    m_pDataTable->RemoveAll();
}

VOID MissionRewardParser::Reload()
{
//	Load( m_pszFileName, TRUE );
}

MissionRewardInfo* MissionRewardParser::GetMissionReward( BYTE byMissionNo )
{
	return m_pDataTable->GetData( byMissionNo );
}

//#pragma warning ( push )
//#pragma warning ( disable : 4244)

BOOL	MissionRewardParser::_Load( BOOL bReload )
{
    __UNUSED(bReload);
	CHAR	pszDesc[REWARD_DESC_LENGTH];

	int nRowSize = GetRowSize();

	for( int nRow = 0; nRow < nRowSize; ++nRow )
	{		
		DWORD dwMissionNo = GetDataDWORD( "dwIndex", nRow );
		FASSERT( SAFE_NUMERIC_TYPECAST(DWORD,dwMissionNo,BYTE) );
		BYTE byMissionNo = (BYTE)dwMissionNo;
		strncpy( pszDesc, GetDataString( "sDesc", nRow ), REWARD_DESC_LENGTH ); // 이 코드 도대체 왜 있는거임?

		MissionRewardInfo* pMRI = m_pDataTable->GetData( byMissionNo );
		if (!pMRI)
		{
			pMRI = new MissionRewardInfo;
			ZeroMemory(pMRI, sizeof(MissionRewardInfo));
			pMRI->m_byMissionNo = byMissionNo;
			m_pDataTable->Add( pMRI, byMissionNo );
		}

		DWORD dwClassCode = GetDataDWORD( "dwClassCode", nRow );
		strncpy( pMRI->m_pszDesc, pszDesc, REWARD_DESC_LENGTH );				// 빠진거 같아서 추가함

		FASSERT( (MissionRewardInfo::CLASS_CODE_LOWER < dwClassCode) && (MissionRewardInfo::CLASS_CODE_UPPER > dwClassCode) );

		MissionRewardInfo::sCLASS* pCLASS = &(pMRI->CLASSs[dwClassCode]);
		pCLASS->m_bClassCode = (BYTE)dwClassCode;

		MissionRewardInfo::sCLASS::sREWARD* pReward		= pCLASS->REWARDs;		
		MissionRewardInfo::sCLASS::sREWARD* pPreReward	= NULL;

		TCHAR szTemp[MAX_FIELD_NAME_SIZE];
		for( DWORD i=0 ; i< MissionRewardInfo::MISSION_REWARD_MAX_SIZE ; ++i, ++pReward )
		{
			_snprintf( szTemp, MAX_FIELD_NAME_SIZE, "bMissionNum%d\0", i+1 );
			pReward->m_bMissionNum	= GetDataBYTE( szTemp, nRow );

			_snprintf( szTemp, MAX_FIELD_NAME_SIZE, "wRewardCode%d\0", i+1 );
			pReward->m_wRewardCode	= GetDataWORD( szTemp, nRow );
			
			if( 0 != pReward->m_bMissionNum && 0 == pReward->m_wRewardCode ||
				0 == pReward->m_bMissionNum && 0 != pReward->m_wRewardCode )
			{
				SUNLOG( eCRITICAL_LOG, "[미션보상스크립트오류:%u,%u]\n", byMissionNo, dwClassCode );
			}
			if( (pPreReward && pPreReward->m_bMissionNum >= pReward->m_bMissionNum) || 
				(pPreReward && pPreReward->m_bMissionNum == 0) ||
				(NULL == pPreReward && pReward->m_bMissionNum == 0) )
			{
				SUNLOG( eCRITICAL_LOG, "[미션보상스크립트오류:%u,%u]보상의값이이상하다", byMissionNo, dwClassCode );
			}

			pPreReward = pReward;
		}
	}

	return TRUE;
}

//#pragma warning ( pop )

BOOL	MissionRewardParser::LoadScript( eSCRIPT_CODE ScriptCode, BOOL bReload )
{
	switch( ScriptCode )
	{
	case SCRIPT_MISSIONREWARDINFO:
		return _Load( bReload );
	}

	return FALSE;
}

