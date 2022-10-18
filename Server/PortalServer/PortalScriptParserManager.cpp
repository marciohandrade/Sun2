#include "StdAfx.h"
#include ".\portalscriptparsermanager.h"

#include "PortalRankingManager.h"
#include "PortalServer.h"

PortalScriptParserManager::PortalScriptParserManager(void)
{
}

PortalScriptParserManager::~PortalScriptParserManager(void)
{
}

VOID	PortalScriptParserManager::InitParserPool( VOID* pPoolOption )
{

}

BOOL	PortalScriptParserManager::RegisterParser()
{
	ERROR_PARSER( RegisterScriptData( SCRIPT_MISSIONRANKINGREWARD, "data\\MissionRankingReward.txt", &g_RankingMgr ) );
	ERROR_PARSER( RegisterScriptData( SCRIPT_MISSIONRANKINGLVAREA, "data\\MissionRankingLVArea.txt", &g_RankingMgr ) );

#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
	sSCRIPT_DATA_INFO_NODE pNodeTable[SCRIPT_LIST_SIZE];	
	if( !GetScriptDataNodeList( pNodeTable, SCRIPT_LIST_SIZE ) )	return FALSE;
	ScriptDataManager::RegisterDataList( pNodeTable );
	ScriptDataManager::RegisterCallbackAllDataLoaded( PortalServer::StartServer );
#endif //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__

	return TRUE;
}

VOID	PortalScriptParserManager::DestroyInstanceEx()
{
	
}
