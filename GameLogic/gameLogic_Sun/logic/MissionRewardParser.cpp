#include "StdAfx.h"
#include "MissionRewardParser.h"
#include <Struct.h>
#include "ItemInfoParser.h"

static VOID CheckValidItemCode( SLOTCODE code )
{
	if( 0 == code ) return;
	BASE_ITEMINFO * pInfo = ItemInfoParser::Instance()->GetItemInfo( code );
	FASSERT( pInfo && "보상아이템이 존재하지 않는 코드입니다." );

}

MissionRewardParser::MissionRewardParser():	m_pDataTable ( NULL )
{
}

MissionRewardParser::~MissionRewardParser()
{
	FASSERT( m_pDataTable == NULL);
}

VOID MissionRewardParser::Release()
{
	Unload();
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

VOID MissionRewardParser::Unload()
{
	MissionRewardInfo * pInfo = NULL;
	if( m_pDataTable == NULL )
	{
		return;
	}
	m_pDataTable->SetFirst();
	while( pInfo = m_pDataTable->GetNext() )
	{
		delete pInfo;
	}
	m_pDataTable->RemoveAll();
}

VOID MissionRewardParser::Reload()
{
	Load( m_pszFileName, TRUE );
}

MissionRewardInfo* MissionRewardParser::GetMissionReward( BYTE byMissionNo )
{
	return m_pDataTable->GetData( byMissionNo );
}

#pragma warning ( push )
#pragma warning ( disable : 4244)
BOOL MissionRewardParser::Load( char * pszFileName, BOOL bReload )
{
	if( !bReload ) strncpy( m_pszFileName, pszFileName, MAX_PATH );

	FASSERT( m_pDataTable != NULL );
	// 서버에서는 리로드 기능이 있으므로 절대 Load()에서 Unload를 호출하지 말 것!!
	// => Unload()호출은 Release()시 해주고 있음
	// Unload(); 


	util::SolarFileReader sr;
	if ( !OpenFile( sr, pszFileName, SEPERATOR_WHITESPACE ) )
	{

		FASSERT(0);
		return (FALSE);
	}

	CHAR	pszDesc[REWARD_DESC_LENGTH];

	while( TRUE )
	{
		eTOKEN_TYPE ettType = sr.GetNextTokenType();
		if ( TOKEN_END == ettType )
		{
			break;
		}

		DWORD dwMissionNo = sr.GetTokenNumber();
		FASSERT( SAFE_NUMERIC_TYPECAST(DWORD,dwMissionNo,BYTE) );
		BYTE byMissionNo = (BYTE)dwMissionNo;
		sr.GetNextTokenType(TOKEN_STRING); strncpy( pszDesc, sr.GetTokenString(), REWARD_DESC_LENGTH );

		MissionRewardInfo* pMRI = m_pDataTable->GetData( byMissionNo );
		if (!pMRI)
		{
			pMRI = new MissionRewardInfo;
			ZeroMemory(pMRI, sizeof(MissionRewardInfo));
			pMRI->m_byMissionNo = byMissionNo;
			m_pDataTable->Add( pMRI, byMissionNo );
		}

		DWORD dwClassCode = 0;
		sr.GetNextTokenType(TOKEN_NUMBER); dwClassCode = sr.GetTokenNumber();

		FASSERT( (MissionRewardInfo::CLASS_CODE_LOWER < dwClassCode) && (MissionRewardInfo::CLASS_CODE_UPPER > dwClassCode) );

		MissionRewardInfo::sCLASS* pCLASS = &(pMRI->CLASSs[dwClassCode]);
		pCLASS->m_bClassCode = (BYTE)dwClassCode;

		MissionRewardInfo::sCLASS::sREWARD* pReward		= pCLASS->REWARDs;
		MissionRewardInfo::sCLASS::sREWARD* pPreReward	= NULL;
		for( DWORD i=0 ; i< MissionRewardInfo::MISSION_REWARD_MAX_SIZE ; ++i, ++pReward )
		{
			sr.GetNextTokenType(TOKEN_NUMBER); pReward->m_bMissionNum = sr.GetTokenNumber();
			sr.GetNextTokenType(TOKEN_NUMBER); pReward->m_wRewardCode = sr.GetTokenNumber();
			if( 0 != pReward->m_bMissionNum && 0 == pReward->m_wRewardCode ||
				0 == pReward->m_bMissionNum && 0 != pReward->m_wRewardCode )
			{
				DISPMSG( "[미션보상스크립트오류:%u,%u]\n", byMissionNo, dwClassCode );
			}
			if( (pPreReward && pPreReward->m_bMissionNum >= pReward->m_bMissionNum) || 
				(pPreReward && pPreReward->m_bMissionNum == 0) ||
				(NULL == pPreReward && pReward->m_bMissionNum == 0) )
			{
				DISPMSG( "[미션보상스크립트오류:%u,%u]보상의값이이상하다\n", byMissionNo, dwClassCode );
			}


			pPreReward = pReward;
		}
	}

	sr.CloseFile();

	return TRUE;
}


#pragma warning ( pop )
