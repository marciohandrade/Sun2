#include "StdAfx.h"
#include "MonsterSummonParser.h"
#include ".\NpcInfoParser.h"
#include "misc.h"

#include <iostream>


MonsterSummonParser::MonsterSummonParser() : m_pMonsterSummonHash(NULL)
{
	
}

MonsterSummonParser::~MonsterSummonParser()
{
}


VOID MonsterSummonParser::Init( DWORD dwPoolSize )
{
	m_pMonsterSummonHash	= new util::SolarHashTable<MonsterSummonScriptInfo *>;
	m_pMonsterSummonHash->Initialize( dwPoolSize );

	m_pRatioSelectHash		= new util::SolarHashTable<ReliableRatioRandomizer<MAX_MONSTER_SUMMON_IN_GROUP> *>;
	m_pRatioSelectHash->Initialize( dwPoolSize );
}

VOID MonsterSummonParser::Release()
{
	Unload();
	SAFE_DELETE( m_pMonsterSummonHash );
	SAFE_DELETE( m_pRatioSelectHash );
}

VOID MonsterSummonParser::Unload()
{
	if( m_pMonsterSummonHash )
	{
		MonsterSummonScriptInfo *pInfo = NULL ;
		for( MONSTER_SUMMON_ITER it = m_pMonsterSummonHash->begin(); it != m_pMonsterSummonHash->end(); ++it )
		{
			pInfo = *it;
			SAFE_DELETE( pInfo );
		}

		m_pMonsterSummonHash->RemoveAll();
	}

	if( m_pRatioSelectHash )
	{
		ReliableRatioRandomizer<MAX_MONSTER_SUMMON_IN_GROUP> *pRatio = NULL;
		for( SUMMON_RATIO_ITER it = m_pRatioSelectHash->begin(); it != m_pRatioSelectHash->end(); ++it )
		{
			pRatio = *it;
			SAFE_DELETE( pRatio );
		}

		m_pRatioSelectHash->RemoveAll();
	}
}


//#pragma warning ( push )
//#pragma warning ( disable : 4244)

BOOL MonsterSummonParser::LoadScript(eSCRIPT_CODE ScriptCode, BOOL bReload)
{
	switch( ScriptCode )
	{
	case SCRIPT_MONSTERSUMMON:
		return _Load( bReload);
	}

	return FALSE;
}

BOOL MonsterSummonParser::_Load(BOOL bReload)
{
    __UNUSED(bReload);
	int nRowSize = GetRowSize();
	for( int nRow = 0; nRow < nRowSize; ++nRow )
	{
		MonsterSummonScriptInfo * pInfo = new MonsterSummonScriptInfo;
		ReliableRatioRandomizer<MAX_MONSTER_SUMMON_IN_GROUP>* pRatioSelect = new ReliableRatioRandomizer<MAX_MONSTER_SUMMON_IN_GROUP>;

		pInfo->m_wMonsterGroupCode				= GetDataWORD( "wMonsterGCode", nRow);
		pInfo->m_wMonsterNum					= GetDataWORD( "wMonsterNum", nRow);
		pInfo->m_dMaxRatio						= GetDataDouble( "P_Amount", nRow);
		pInfo->m_LiveTime						= GetDataDWORD( "wTime", nRow);
		pInfo->m_LimitedLV						= GetDataBYTE( "wLvLilmit", nRow);

        double dAccumulation = 0;
		double dRatio = 0;
		TCHAR szTemp[MAX_FIELD_NAME_SIZE];
		for( DWORD i = 0; i < MAX_MONSTER_SUMMON_IN_GROUP; i++ )
		{
			_snprintf( szTemp, MAX_FIELD_NAME_SIZE, "MonsterIndex%d\0", i + 1);
			pInfo->m_pMonster[i].MonsterCode	= GetDataDWORD( szTemp, nRow);
			_snprintf( szTemp, MAX_FIELD_NAME_SIZE, "MRation%d\0", i + 1);
			dRatio								= GetDataDouble( szTemp, nRow);
			pInfo->m_pMonster[i].dSeed			= ( dRatio/ pInfo->m_dMaxRatio);
			dAccumulation						= dAccumulation + dRatio;

			if( pInfo->m_pMonster[i].MonsterCode )
				pRatioSelect->AddRatio( (DWORD)(i + 1), dRatio, pInfo->m_dMaxRatio);
		}

		if( CheckValidity( pInfo, dAccumulation) )
		{
			m_pMonsterSummonHash->Add( pInfo, pInfo->m_wMonsterGroupCode);
			m_pRatioSelectHash->Add( pRatioSelect, pInfo->m_wMonsterGroupCode);
		}
		else
		{
			SAFE_DELETE( pInfo);
			SAFE_DELETE( pRatioSelect);
		}
	}

	return TRUE;
}

//#pragma warning ( pop )

BOOL MonsterSummonParser::CheckValidity( MonsterSummonScriptInfo *pMonsterSummonInfo, double dAccumulation )
{
	if( pMonsterSummonInfo->m_dMaxRatio == 0)
	{
		SUNLOG(eCRITICAL_LOG, "MonsterSummon 스크립트의 %u 몬스터 그룹코드에서 확률의 합이 0이다. ");
		return FALSE;
	}

	if( dAccumulation > pMonsterSummonInfo->m_dMaxRatio )
	{
		SUNLOG(eCRITICAL_LOG, "MonsterSummon 스크립트 : %u 그룹코드에서 확률의 합이 MAX를 넘었다.", pMonsterSummonInfo->m_wMonsterGroupCode );
		return FALSE;
	}

	//소환할 몬스터가 스크립트에 존재하는가?
	for(DWORD i = 0; i < MAX_MONSTER_SUMMON_IN_GROUP; i++)
	{		
		if( pMonsterSummonInfo->m_pMonster[i].MonsterCode == 0 )	continue;

		if( !NPCInfoParser::Instance()->GetNPCInfo( pMonsterSummonInfo->m_pMonster[i].MonsterCode ) )
		{
			SUNLOG(eCRITICAL_LOG, "MonsterSummon 스크립트 그룹코드 : %u  Index : %u  없는 몬스터."
				, pMonsterSummonInfo->m_wMonsterGroupCode, i+1 );
			return FALSE;
		}
	}

	//소환할 몬스터가 0일 수는 없다.
	if( pMonsterSummonInfo->m_wMonsterNum == 0)
		return FALSE;

	return TRUE;
}

WORD MonsterSummonParser::GetMonsterSummonNum( WORD GroupCode )
{
	MonsterSummonScriptInfo *pInfo = m_pMonsterSummonHash->GetData( GroupCode );
	if( !pInfo )
	{
		SUNLOG(eCRITICAL_LOG, "MonsterSummon 스크립트가 없거나 잘못된 그룹코드: %u", GroupCode);
		return 0;
	}	
	
	return pInfo->m_wMonsterNum;
}

DWORD MonsterSummonParser::GetMonsterLiveTime( WORD GroupCode ) const
{
	MonsterSummonScriptInfo *pInfo = m_pMonsterSummonHash->GetData( GroupCode );
	if( !pInfo )
	{
		SUNLOG(eCRITICAL_LOG, "MonsterSummon 스크립트가 없거나 잘못된 그룹코드: %u", GroupCode);
		return 0;
	}	

	return pInfo->m_LiveTime;
}

const MonsterSummonScriptInfo* MonsterSummonParser::GetMonsterSummonScriptInfo(WORD GroupCode) const
{
	MonsterSummonScriptInfo *pInfo = m_pMonsterSummonHash->GetData( GroupCode );
	if( !pInfo )
	{
		SUNLOG(eCRITICAL_LOG, "MonsterSummon 스크립트가 없거나 잘못된 그룹코드: %u", GroupCode);
		return 0;
	}	

	return pInfo;
}


CODETYPE MonsterSummonParser::GetRandomMonsterCode( WORD GroupCode )
{
	MonsterSummonScriptInfo *pInfo = m_pMonsterSummonHash->GetData( GroupCode );
	if( !pInfo )
	{
		SUNLOG(eCRITICAL_LOG, "MonsterSummon 스크립트가 없거나 잘못된 그룹코드: %u", GroupCode);
		return FALSE;
	}
	ReliableRatioRandomizer<MAX_MONSTER_SUMMON_IN_GROUP> * pRatioSelect = NULL; 
	pRatioSelect = m_pRatioSelectHash->GetData( GroupCode );
	if( pRatioSelect == NULL)
	{
		return 0;
	}	

	CODETYPE MonsterCode = 0;

	DWORD RandomKey = pRatioSelect->RandomizedKey();	

	if( RandomKey == 0 || RandomKey > MAX_MONSTER_SUMMON_IN_GROUP )
	{
		MonsterCode = 0;
	}
	else
	{
		MonsterCode = pInfo->m_pMonster[RandomKey - 1].MonsterCode;
	}
	
	return MonsterCode;
}