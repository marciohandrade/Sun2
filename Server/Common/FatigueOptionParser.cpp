#include "stdafx.h"
#include "FatigueOptionParser.h"


FatigueOptionParser::FatigueOptionParser(void)
{
	ZeroMemory( &m_FatigueCommon, sizeof(m_FatigueCommon) );
	ZeroMemory( &m_FatigueApply, sizeof(m_FatigueApply) );
	m_mapFatigueType.clear();
}

FatigueOptionParser::~FatigueOptionParser(void)
{

}

_FATIGUE_TYPE*	FatigueOptionParser::GetFatigueTypeFromTime( int nPlayTime )
{
	FATIGUE_TYPE_MAP_ITER	iter;
	for( iter = m_mapFatigueType.begin(); iter != m_mapFatigueType.end(); ++iter )
	{
		_FATIGUE_TYPE*	pFatigue = iter->second;
		if( pFatigue )
		{
			if( (pFatigue->m_nStartTime <= nPlayTime) && (pFatigue->m_nEndTime > nPlayTime) )
				return pFatigue;

			if( (pFatigue->m_nStartTime <= nPlayTime) && (pFatigue->m_nEndTime == -1) )  //무한대이면...
				return pFatigue;
		}
	}

	return NULL;
}

_FATIGUE_TYPE*
FatigueOptionParser::GetFatigueType(BYTE byType)
{
    // (CHANGES) (100113) (WAVERIX) fix up invalid code rule
    FATIGUE_TYPE_MAP::iterator it = m_mapFatigueType.find(byType);
	if (it == m_mapFatigueType.end())
		return NULL;

	return it->second;
}

VOID	FatigueOptionParser::Release()
{
	FATIGUE_TYPE_MAP_ITER	iter;
	for( iter = m_mapFatigueType.begin(); iter != m_mapFatigueType.end(); ++iter )
	{
		_FATIGUE_TYPE*	pFatigue = iter->second;
		if( pFatigue )
			delete pFatigue;
	}

	m_mapFatigueType.clear();
}


BOOL		FatigueOptionParser::LoadScript( eSCRIPT_CODE ScriptCode, BOOL bReload )
{
	switch( ScriptCode )
	{
	case SCRIPT_FATIGUEOPTION:
		return _Load( bReload );
	};

	return FALSE;
}

BOOL		FatigueOptionParser::_Load( BOOL bReload )
{
	ZeroMemory( &m_FatigueCommon, sizeof(m_FatigueCommon) );	//	(WAVERIX)(081013)(ADD-IN)
	ZeroMemory( &m_FatigueApply, sizeof(m_FatigueApply) );		//	(WAVERIX)(081013)(ADD-IN)
	// COMMON
	m_FatigueCommon.m_bUse = GetINIDataBYTE( "COMMON", "USE" );
	m_FatigueCommon.m_byTypeCount = GetINIDataBYTE( "COMMON", "FATIGUE_TYPE_COUNT" );
	m_FatigueCommon.m_byInitType = GetINIDataBYTE( "COMMON", "FATIGUE_INIT_TYPE" );
	m_FatigueCommon.m_wCheckCycle = GetINIDataWORD( "COMMON", "FATIGUE_CHECK_CYCLE" );

	// APPLY
	m_FatigueApply.m_bVillageTime = GetINIDataBYTE( "FATIGUE_APPLY", "VILLAGE_TIME" );
	m_FatigueApply.m_bLobbyTime = GetINIDataBYTE( "FATIGUE_APPLY", "LOBBY_TIME" );
	m_FatigueApply.m_bSuperMaster = GetINIDataBYTE( "FATIGUE_APPLY", "SUPER_MASTER" );
	m_FatigueApply.m_bDeadTime = GetINIDataBYTE( "FATIGUE_APPLY", "DEAD_TIME" );
	m_FatigueApply.m_bInSSQField	= GetINIDataBYTE( "FATIGUE_APPLY", "IN_SSQ_FIELD" );

	// 피로도 타입
	for( BYTE byType = 0; byType < m_FatigueCommon.m_byTypeCount; byType++ )
	{
		_FATIGUE_TYPE* pType = new _FATIGUE_TYPE;
		ZeroMemory( pType, sizeof(*pType) );	//	(WAVERIX)(081013)(ADD-IN)
		TCHAR szType[MAX_PATH];
		_snprintf( szType, MAX_PATH, "FATIGUE_TYPE_%d", byType+1 );
		pType->m_byType = byType+1;
		pType->m_nStartTime = GetINIDataInt( szType, "START_TIME" );
		pType->m_nEndTime = GetINIDataInt( szType, "END_TIME" );
		pType->m_wMsgInterval = GetINIDataWORD( szType, "MESSAGE_INTERVAL" );
		ASSERT( pType->m_wMsgInterval >= 0 );
		pType->m_byItemDropRatio = GetINIDataBYTE( szType, "ITEMDROP_RATIO" );
		pType->m_byExpRatio = GetINIDataBYTE( szType, "EXP_RATIO" );
		pType->m_bItemPickUp = GetINIDataBYTE( szType, "ITEM_PICKUP" );
		pType->m_bySharedItemPickupRatio = GetINIDataBYTE( szType, "SHARE_ITEM_PICKUP_RATIO" );
		pType->m_bySharedHeimPickupRatio = GetINIDataBYTE( szType, "SHARE_HEIM_PICKUP_RATIO" );
		pType->m_bItemReward = GetINIDataBYTE( szType, "ITEM_REWARD" );
		pType->m_byHeimRewardRatio = GetINIDataBYTE( szType, "HEIM_REWARD_RATIO" );
		pType->m_byExpRewardRatio = GetINIDataBYTE( szType, "EXP_REWARD_RATIO" );
		m_mapFatigueType.insert( std::make_pair( pType->m_byType, pType ) );
	}

	return TRUE;

}
