#include "StdAfx.h"
#include ".\gameserverex.h"

//PARSER
#include "GameScriptParserManager.h"
#include <SunEventInfoParser.h>
#include <PacketStruct_ServerCommon.h>
#ifdef _APPLY_NHN_FILTERFING
    #include <IAF_Export.h>	//lib 사용	
#endif
//custom
#include "ServerOptionParser.h"
#include "ObjectFactory.h"
#include "GamePartyManager.h"
#include "SummonManager.h"
#include "StatusFactory.h"
#include "StatisticsManager.h"
#include "GameZoneManager.h"
#include "UnitGroupManager.h"
#include "QuestManager_Concrete.h"
#include "DropManager.h"
#include "PacketHandler/PacketHandler.h"
#include "ResrcManager.h"
#include "TradeManager.h"
#include "GameGuildManager.h"
#include "RatioManager.h"
#include "FunctionalRestrictManager.h"
#include "PeakTimeEvent.h"
#include "MovementChecker.h"
#include <EventManager.h>
#include <AppliedItemDependencyChain.h>
#include <AppliedNpcDependencyChain.h>

//#endif //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__

#include <ProhibitionWordParser.h>
#include <World/GameWarControlManager.h>
#include "GameRankingManager.h"

static VOID		_StartManagerOnScriptPreLoad()
{
	AppliedNpcDependencyChain::CreateInstance();
	if( !AppliedNpcDependencyChain::GetWorkKey() )
	{
		AppliedNpcDependencyChain* pWorkNode = AppliedNpcDependencyChain::Instance();
		WORK_KEY wKey = WorkLoadManager::RegisterTask( pWorkNode );
		AppliedNpcDependencyChain::SetWorkKey( wKey );
	}
}

static VOID		_StartManagerOnScriptLoaded()
{
	if( !EventManager::GetWorkKey() )
	{
		IWorkNode* pWorkNode = new EventManager();
		WORK_KEY wKey = WorkLoadManager::RegisterTask( pWorkNode );
		EventManager::SetWorkKey( wKey );
	}

	AppliedItemDependencyChain::CreateInstance();
	if( !AppliedItemDependencyChain::GetWorkKey() )
	{
		AppliedItemDependencyChain* pWorkNode = AppliedItemDependencyChain::Instance();
		WORK_KEY wKey = WorkLoadManager::RegisterTask( pWorkNode );
		AppliedItemDependencyChain::SetWorkKey( wKey );
	}

	MovementChecker::PreCalcScript();

	if( !GameWarControlManager::GetWorkKey() )
	{
		GameWarControlManager* pWorkNode = new GameWarControlManager();
		const WORK_KEY wKey = WorkLoadManager::RegisterTask( pWorkNode );
		GameWarControlManager::SetWorkKey( wKey );
	}

	HighLoadBalancer::CreateInstance();
    // CHANGES: f110511.1L, shared logic, _NA002635_GP_DP_PROTOCOL_UINIFICATION
#ifdef _NA_008012_20150130_RANKING_SYSTEM
    g_GameRankingMgr.OnScriptsLoaded();
#endif // _NA_008012_20150130_RANKING_SYSTEM
    // CHANGES: f110809.1L
    nsAI::NpcHeartbeat::OnAiScriptLoaded();
};

BOOL GameServerEx::ParserInit()
{
	SERVER_POOL_OPTION& stServerPoolOptionInfo = ServerOptionParser::Instance()->GetPoolInfo();

	GameScriptParserManager::Instance()->RegisterParser();
	GameScriptParserManager::Instance()->InitParserPool( (VOID*)&stServerPoolOptionInfo );

	_StartManagerOnScriptPreLoad();

	#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
		return TRUE;
	#else
		GameScriptParserManager::Instance()->Load();
	#endif

	_StartManagerOnScriptLoaded();

	return TRUE;
}

void GameServerEx::ParserRelease()
{
	ServerOptionParser::Instance()->Release();			ServerOptionParser::DestroyInstance();

	GameScriptParserManager::Instance()->Release();
	GameScriptParserManager::Instance()->DestroyInstanceEx();
	GameScriptParserManager::DestroyInstance();
}


#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__

VOID	GameServerEx::ReadyToStartServer()
{
	g_pGameServer->LogicInit();

	ScriptDataManager::RegisterCallbackAllDataLoaded( GameServerSyncWithMasterServer );

#ifdef _APPLY_NHN_FILTERFING
    static BOOLEAN s_bInited = 0;
    if(s_bInited == 0)
    {
        int result = IAF::LoadAbuseDB();
        // error handling?
        if(result != 0)
        {
            SUNLOG(eCRITICAL_LOG, "Fail to load IAF LoadAbuseDB %d", result);
            return;
        }
    }
#endif

	// 현 시점 PHASE_EXCHANGE일 것이다.
	// ?? 마스터가 끊어진 상황이 존재하는 것인가. 이후 처리를 확인할 것.
	ServerSession* pSendSession = g_pGameServer->GetMasterServerSession();
	if( pSendSession )
	{
		MSG_SERVERCOMMON_DATA_TRANSFER_SENDDATA_NTF msgNTF;
		pSendSession->Send( (BYTE*)&msgNTF, sizeof(msgNTF) );
	}

	_StartManagerOnScriptLoaded();
}

VOID	GameServerEx::GameServerSyncWithMasterServer()
{
	// 현 시점 PHASE_EXCHANGE일 것이다.
	ServerSession* pSendSession = g_pGameServer->GetMasterServerSession();
	if( pSendSession )
	{
		MSG_SERVERCOMMON_DATA_TRANSFER_SENDDATA_NTF msgNTF;
		pSendSession->Send( (BYTE*)&msgNTF, sizeof(msgNTF) );
	}
}

#endif //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
