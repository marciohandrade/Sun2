#include "StdAfx.h"

#include "AgentScriptParserManager.h"
#include "MapInfoParser.h"
#include "RequireFieldLevelInfoParser.h"
#include "MatchInfoParser.h"
#include "AgentServer.h"
#include "MapPartialLoadingParser.h"
#include "ProhibitionWordParser.h"
#include "CurseTowerTimeParser.h"
#include <SkillInfoParser.h>
#include <ServerOptionParserEx.h>

#include <SunEventInfoParser.h>
#include <NPCInfoParser.h>
#include <SSQ/SSQInfoParser.h>
#include <DominationInfoParser.h>
#ifdef _NA_0_20100901_SKILLTREE_SYSTEM
#include <SkillInventoryStoreParser.h>
#endif
#include <SkillSystem/PerkInfoParser.h>
#ifdef _NA_20100307_BETAKEY_SYSTEM
#include <CharInfoParser.h>
#endif
//_NA_20110630_BATTLE_ZONE_RENEWAL
#include "BattlezoneMissionTreeParser.h"

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#include "ChaosZoneInfoParser.h"
#include "BattleGroundInfoParser.h"
#include "BattleGroundFlagInfoParser.h"
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#ifdef _NA_006538_BATTLE_GROUND_TIME_CONTROL_SYSTEM
#include "BattleGroundTimeInfoParser.h"
#endif // _NA_006538_BATTLE_GROUND_TIME_CONTROL_SYSTEM

#ifdef _NA_20110610_MOVE_DOMINATION_CHANNEL_INFO
#include "GameOptionParser.h"
#endif

AgentScriptParserManager::AgentScriptParserManager(void)
{
}

AgentScriptParserManager::~AgentScriptParserManager(void)
{
}

VOID	AgentScriptParserManager::InitParserPool( VOID* pPoolOption )
{
	MapInfoParser::Instance()->Init( MAPINFOPARSER_POOL );
	RequireFieldLevelInfoParser::Instance()->Init( REQUIREFIELDLEVELINFOPARSER_POOL );
	MatchInfoParser::Instance()->Init( MATCHINFOPARSER_POOL );
	NPCInfoParser::Instance()->Init( 100 );
	SkillInfoParser::Instance()->Init( 1000 );
#ifdef _NA_0_20100901_SKILLTREE_SYSTEM
    SkillInventoryStoreParser::Instance()->Init(20);
#endif
#ifdef _NA_20100307_BETAKEY_SYSTEM
    CharInfoParser::Instance()->Init( CHARINFO_PARSER_POOL );
#endif
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    ChaosZoneInfoParser::Instance()->Init();
    BattleGroundInfoParser::Instance()->Init();
    BattleGroundFlagInfoParser::Instance()->Init(100);
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
}

BOOL	AgentScriptParserManager::RegisterParser( )
{
	ERROR_PARSER( RegisterScriptData( SCRIPT_SERVER_OPTION, "data\\ServerOptionEx.ini", ServerOptionParserEx::Instance() ) );

	ERROR_PARSER( RegisterScriptData( SCRIPT_FIELD, "data\\Field.txt", MapInfoParser::Instance(), SEPERATOR_WHITESPACE, NULL, TRUE ) );
	ERROR_PARSER( RegisterScriptData( SCRIPT_FIELDWMO, "data\\Fieldwmo.txt", MapInfoParser::Instance(), SEPERATOR_WHITESPACE, NULL, TRUE ) );
	ERROR_PARSER( RegisterScriptData( SCRIPT_FIELDWUT, "data\\Fieldwut.txt", MapInfoParser::Instance(), SEPERATOR_WHITESPACE, NULL, TRUE ) );

	ERROR_PARSER( RegisterScriptData( SCRIPT_WORLD_SUB_INFO, "data\\Worldsubinfo.txt", MapInfoParser::Instance() ) );

	ERROR_PARSER( RegisterScriptData( SCRIPT_WORLD, "data\\World.txt", MapInfoParser::Instance() ) );
	ERROR_PARSER( RegisterScriptData( SCRIPT_MAPENTERANCEPORTAL, "data\\MapEnterancePortal.txt", MapInfoParser::Instance() ) );
	ERROR_PARSER( RegisterScriptData( SCRIPT_MATCHINFO, "data\\MatchInfo.txt", MatchInfoParser::Instance() ) );
	ERROR_PARSER( RegisterScriptData( SCRIPT_REQUIREFIELDLEVELINFO, "data\\RequireFieldLevelInfo.txt", RequireFieldLevelInfoParser::Instance() ) );
	ERROR_PARSER( RegisterScriptData( SCRIPT_MAP_PARTIAL_LOADING, "data\\MapPartialLoading.ini", MapPartialLoadingParser::Instance() ) );

	ERROR_PARSER( RegisterScriptData( SCRIPT_FILTER, "data\\Filter.txt", ProhibitionWordParser::Instance() ) );
	ERROR_PARSER( RegisterScriptData( SCRIPT_FILTERNAME, "data\\FilterName.txt", ProhibitionWordParser::Instance() ) );

	ERROR_PARSER( RegisterScriptData( SCRIPT_CURSE_TOWER_TIME, "data\\CurseTowerTime.txt", CurseTowerTimeParser::Instance() ) );

	// 임시 등록
	ERROR_PARSER( RegisterScriptData( SCRIPT_SSQPATTERN, "data\\SSQPattern.txt", SSQInfoParser::Instance() ) );
#ifdef _NA_0_20100901_SKILLTREE_SYSTEM
    ERROR_PARSER(RegisterScriptData( SCRIPT_SKILL_INVEN_STORE, "data\\SkillInventoryStore.txt", SkillInventoryStoreParser::Instance() ) );
#endif
    ERROR_PARSER( RegisterScriptData( SCRIPT_SKILLINFO, "data\\SkillInfo.txt", SkillInfoParser::Instance() ) );
	ERROR_PARSER( RegisterScriptData( SCRIPT_STYLEINFO, "data\\StyleInfo.txt", SkillInfoParser::Instance() ) );
    ERROR_PARSER( RegisterScriptData( SCRIPT_NPCINFO,   "data\\NpcInfo.txt",    NPCInfoParser::Instance() ) );
    ERROR_PARSER( RegisterScriptData( SCRIPT_SSQINFO, "data\\SSQInfo.txt", SSQInfoParser::Instance() ) );
    //_KR_001385_20090907_DOMINATION_PARSER
    ERROR_PARSER(RegisterScriptData(SCRIPT_DOMINATIONINFO, "data\\DominationInfo.txt", DominationInfoParser::Instance()));
    ERROR_PARSER(RegisterScriptData(SCRIPT_DOMINATION_EVENT_INFO, "data\\DominationTimeInfo.txt", DominationInfoParser::Instance()));
#ifdef _NA002217_100728_EP2_PERK_SYSTEM_
    ERROR_PARSER(RegisterScriptData(SCRIPT_PERKINFO, "data\\PerkInfo.txt", PerkInfoParser::Instance()));
#endif
#ifdef _NA_20100307_BETAKEY_SYSTEM
    ERROR_PARSER(RegisterScriptData(SCRIPT_INITCHARINFO, "data\\InitCharInfoList.txt", CharInfoParser::Instance()));
#endif
    //_NA_20110630_BATTLE_ZONE_RENEWAL
    ERROR_PARSER(RegisterScriptData(SCRIPT_BATTLEZONE_TREE, "data\\BattlezoneMissiontree.txt", BattlezoneMissionTreeParser::Instance()));

#ifdef _NA_20110610_MOVE_DOMINATION_CHANNEL_INFO
    ERROR_PARSER(RegisterScriptData(SCRIPT_GAME_OPTION, "GameOption.ini", GameOptionParser::Instance()));
#endif
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    ERROR_PARSER(RegisterScriptData(SCRIPT_CHAOSZONE_INFO, "data\\ChaosZoneInfo.txt", ChaosZoneInfoParser::Instance()));
    ERROR_PARSER(RegisterScriptData(SCRIPT_BATTLE_GROUND_INFO, "data\\BattleGroundInfo.txt", BattleGroundInfoParser::Instance()));
    ERROR_PARSER(RegisterScriptData(SCRIPT_BATTLE_GROUND_FLAG_INFO, "data\\BattleGroundFlagInfo.txt", BattleGroundFlagInfoParser::Instance()));
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#ifdef _NA_006538_BATTLE_GROUND_TIME_CONTROL_SYSTEM
    ERROR_PARSER(RegisterScriptData(SCRIPT_BATTLE_GROUND_TIME_INFO, "data\\BattleGroundTimeInfo.txt", BattleGroundTimeInfoParser::Instance()));
#endif // _NA_006538_BATTLE_GROUND_TIME_CONTROL_SYSTEM


#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
	sSCRIPT_DATA_INFO_NODE pNodeTable[SCRIPT_LIST_SIZE];	
	if( !GetScriptDataNodeList( pNodeTable, SCRIPT_LIST_SIZE ) )	return FALSE;
	ScriptDataManager::RegisterDataList( pNodeTable );
	ScriptDataManager::RegisterCallbackAllDataLoaded( AgentServer::StartServer );
#endif //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__

	return TRUE;
}

VOID	AgentScriptParserManager::DestroyInstanceEx()
{
	MapInfoParser::Instance()->DestroyInstance();
	MatchInfoParser::Instance()->DestroyInstance();
	RequireFieldLevelInfoParser::Instance()->DestroyInstance();
	MapPartialLoadingParser::DestroyInstance();
	ProhibitionWordParser::DestroyInstance();
    NPCInfoParser::DestroyInstance();

	CurseTowerTimeParser::DestroyInstance();
	ServerOptionParserEx::DestroyInstance();

	SSQInfoParser::DestroyInstance();
	SkillInfoParser::DestroyInstance();
    DominationInfoParser::DestroyInstance();
#ifdef _NA002217_100728_EP2_PERK_SYSTEM_
    PerkInfoParser::DestroyInstance();
#endif
#ifdef _NA_0_20100901_SKILLTREE_SYSTEM
    SkillInventoryStoreParser::DestroyInstance();
#endif
    //_NA_20110630_BATTLE_ZONE_RENEWAL
    BattlezoneMissionTreeParser::DestroyInstance();

#ifdef _NA_20110610_MOVE_DOMINATION_CHANNEL_INFO
    GameOptionParser::DestroyInstance();
#endif
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    ChaosZoneInfoParser::DestroyInstance();
    BattleGroundInfoParser::DestroyInstance();
    BattleGroundFlagInfoParser::DestroyInstance();
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
}












