#include "StdAfx.h"
#include ".\gamescriptparsermanager.h"

#include <ItemInfoParser.h>
#include <NPCInfoParser.h>
#include <MapInfoParser.h>
#include <ShopInfoParser.h>
#include <SkillInfoParser.h>
#include <TriggerParser.h>
#include <RegenParser.h>
#include <GroupParser.h>
#include <StateInfoParser.h>
#include <ItemCompositeParser.h>
#include <MissionRewardParser.h>
#include <FormulaRatioParser.h>
#include <StyleQuickRegistInfoParser.h>
#include <AIParamParser.h>
#include <EnchantParser.h>
#include <RankOptionParser.h>
#include <SocketOptionParser.h>
#include <CHuntingRewardParser.h>
#include <ItemComposeListParser.h>
#include <ItemCrystalizeListParser.h>
#include <ExtraStoneOptionInfoParser.h>
#include <ExpValueInfoParser.h>

#include <SunEventInfoParser.h>

#ifdef C_NA_0_20100520_ACSYSTEM_CHANGED
#include <AchievementParser.h>
#else
#include <AreaConquestInfoParser.h>
#endif


#include <ArmorDamageParser.h>
#include <FatigueOptionParser.h>
#include <PKScriptParser.h>
#include <CharInfoParser.h>
#include <MonsterSummonParser.h>
#include <MapPartialLoadingParser.h>
#include <PointRankingParser.h>

//파싱기능을 하는데... Parser라는 이름을 안 붙인 것들..
#include <ItemTypeList.h>
#include <DropRatioInfoList.h>
#include <ItemTypeDropRatio.h>
#include "DropManager.h"		//위의 3개의 파일은 DropManager에서 사용된다.

#include <RewardInfoList.h>
#include "RewardManager.h"
#include "GMCmdManager.h"
#include "QuestInfoManagerEx.h"
#include "MissionRecordManager.h"
#include "GameRankingManager.h"

#include "GameServerEx.h"
#include <ProhibitionWordParser.h>
#include "ExchangeRewardInfo.h"

#include <ServerOptionParserEx.h>
#include <GameOptionParser.h>

#include <LimitedOptionParser.h>
#include <LimitedItemInfoParser.h>
#include <SetItemOptionInfoParser.h>
#include <GuildRankInfoParser.h>
#include <GuildRightInfoParser.h>
#include <SSQ/SSQInfoParser.h>
#include <PetInfoParser.h>
#include <EtherBulletInfoParser.h>
//#include <TestParser.h>
#include <LotteryInfoParser.h>

#ifdef __NA_001062_20080623_STRENGTH_PENALTY
	#include <StrengthPenaltyParser.h>
	#include <StrengthUIParser.h>
#endif
#include <CollectionParser.h>
#include <GroupInfoParser.h>
#include <RidingInfoParser.h>
#include <DominationInfoParser.h>

#ifdef _NA_1419_20100121_ACCUMULATE_POINT
    #include <AccumulatePointParser.h>
#endif

#ifdef _NA_0_20100901_SKILLTREE_SYSTEM
    #include <SkillInventoryStoreParser.h>
#endif

#ifdef _NA_0_20101015_SOLAR_SHOP_TEMP_REFACTORING
    #include <CashCategoryParser.h>
#endif

#include <SkillSystem/PerkInfoParser.h> // _NA002217_100728_EP2_PERK_SYSTEM_
#ifdef _NA_000251_20100727_SOCKET_SYSTEM
#include "SocketSystem/SocketItemRatioParser.h"
#include "SocketSystem/ZardComposeInfoParser.h"
#endif // _NA_000251_20100727_SOCKET_SYSTEM
#ifdef _NA_002253_20100811_CUBE_SYSTEM
#include "CubeSystem/CubeSystemParser.h"
#endif // _NA_002253_20100811_CUBE_SYSTEM
#ifdef _USE_MAX_CHAR_LIST_PARSER
#include "MaxCharListParser.h"
#endif
#ifdef _NA_003027_20111013_HONOR_SYSTEM
#include "TitleListParser.h"
#include "ReputeListParser.h"
#include "FameListParser.h"
#endif //_NA_003027_20111013_HONOR_SYSTEM
//_NA_20110630_BATTLE_ZONE_RENEWAL
#include "BattlezoneMissionTreeParser.h"

#ifdef _NA_002935_20110704_ITEM_SCORE
#include "ItemScoreParser.h"
#endif // _NA_002935_20110704_ITEM_SCORE
#ifdef _NA_003649_20111115_BATTLE_ZONE_HARDMODE
#include "NPCReplaceInfoParser.h"
#endif //_NA_003649_20111115_BATTLE_ZONE_HARDMODE
#ifdef _NA_003740_20111122_RANDOM_ITEM
#include "RandomItemParser.h"
#endif // _NA_003740_20111122_RANDOM_ITEM
#include "CurrencyInfoParser.h" //_NA004034_20120102_POINT_WALLET_SYSTEM
#ifdef _NA_003923_20120130_GUILD_RENEWAL
#include "GuildInfoParser.h"
#endif
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#include "ChaosZoneInfoParser.h"
#include "BattleGroundInfoParser.h"
#include "BattleGroundFlagInfoParser.h"
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#ifdef _NA_006538_BATTLE_GROUND_TIME_CONTROL_SYSTEM
#include "BattleGroundTimeInfoParser.h"
#endif // _NA_006538_BATTLE_GROUND_TIME_CONTROL_SYSTEM
#ifdef _NA_007065_20140217_CHAOSZONE_GOLDRUSH
#include "ChaosZoneSystem/GoldRushInfoParser.h"
#include "ChaosZoneSystem/GoldRushMonsterPointParser.h"
#endif //_NA_007065_20140217_CHAOSZONE_GOLDRUSH
#include "MonsterBookParser.h" //_NA_007086_20140318_MONSTERBOOK
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
#include "GuildFacilityInfoParser.h"
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
#ifdef _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
#include "AttendanceParser.h"
#endif // _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
#ifdef _NA_008012_20150204_SUN_RANKING_SYSTEM
#include "SunRankingParser.h"
#endif // _NA_008012_20150204_SUN_RANKING_SYSTEM
#ifdef _NA_008124_20150313_AWAKENING_SYSTEM
#include "AwakeningInfoParser.h"
#endif // _NA_008124_20150313_AWAKENING_SYSTEM
#ifdef _NA_008283_20150511_NOTIFICATIONS_SYSTEM
#include "NotificationInfoParser.h"
#endif // _NA_008283_20150511_NOTIFICATIONS_SYSTEM
#ifdef _NA_008404_20150712_MINIGAME_SYSTEM
#include "MiniGameParser.h"
#endif // _NA_008404_20150712_MINIGAME_SYSTEM
#ifdef _NA_008334_20150608_SONNENSCHEIN
#include "ChaosZoneSystem/SonnenScheinRewardInfoParser.h"
#endif //_NA_008334_20150608_SONNENSCHEIN
#ifdef _NA_008543_20151027_ONE_DAYS_GUILD_MISSION
#include "ChaosZoneSystem/OneDaysGuildMissioninfoParser.h"
#endif //(_NA_008543_20151027_ONE_DAYS_GUILD_MISSION)


GameScriptParserManager::GameScriptParserManager(void)
{
}

GameScriptParserManager::~GameScriptParserManager(void)
{
}


VOID	GameScriptParserManager::InitParserPool( VOID* pPoolOption )
{
	SERVER_POOL_OPTION* pServerPoolOptionInfo = (SERVER_POOL_OPTION*)pPoolOption;
	if( !pServerPoolOptionInfo )	return;

	GMCmdManager::Instance()->Init();
	ItemInfoParser::Instance()->Init(pServerPoolOptionInfo->m_wItemInfo);
	SkillInfoParser::Instance()->Init( pServerPoolOptionInfo->m_wSkillInfo );
	NPCInfoParser::Instance()->Init( pServerPoolOptionInfo->m_wNPCInfo);
	AIParamParser::Instance()->Init(pServerPoolOptionInfo->m_wAITypeInfo);
	MapInfoParser::Instance()->Init( pServerPoolOptionInfo->m_wMapInfo );
	ShopInfoParser::Instance()->Init( pServerPoolOptionInfo->m_wShopInfo );
	GroupParser::Instance()->Init( pServerPoolOptionInfo->m_wGroupInfo );
	RegenParser::Instance()->Init( pServerPoolOptionInfo->m_wRegenInfo );

	StateInfoParser::Instance()->Init( pServerPoolOptionInfo->m_wStateInfo );
	ItemCompositeParser::Instance()->Init( pServerPoolOptionInfo->m_wItemCompInfo );
	MissionRewardParser::Instance()->Init(pServerPoolOptionInfo->m_wMissionRewardInfo);
	FormulaRatioParser::Instance()->Init(pServerPoolOptionInfo->m_wFormulaRatioInfo);
	StyleQuickRegistInfoParser::Instance()->Init(pServerPoolOptionInfo->m_wStyleQuickInfo);
	ExtraStoneOptionInfoParser::Instance()->Init( EXTRASTONEOPTIONINFO_PARSER_POOL );
	ItemComposeListParser::Instance()->Init( ITEMCOMPOSELIST_PARSER_POOL );
	ItemCrystalizeListParser::Instance()->Init( ITEMCRYSTALIZELIST_PARSER_POOL );

    ExpValueInfoParser::Instance()->Init( EXPVALUEINFO_PARSER_POOL );
	CHuntingRewardParser::Instance()->Init( CHUNTINGREWARD_PARSER_POOL );
	g_RewardManager.Init( ItemTypeList::Instance(), DropRatioInfoList::Instance() );
#ifdef C_NA_0_20100520_ACSYSTEM_CHANGED
	AchievementParser::Instance()->Init(AREACONQUESTINFO_PARSER_POOL);
#else
	AreaConquestInfoParser::Instance()->Init( AREACONQUESTINFO_PARSER_POOL );
#endif	
	ArmorDamageParser::Instance()->Init(ARMORDAMAGE_PARSER_POOL);
	MonsterSummonParser::Instance()->Init( MONSTERSUMMON_PARSER_POOL );

#ifdef __NA_001027_20080423_RANDOM_REWARD_EVENT_
	ExchangeRewardInfoParser::Instance()->Init( 50 );
#endif


#ifdef _NA_1419_20100121_ACCUMULATE_POINT
    AccumulatePointParser::Instance()->Init();
#endif

#ifdef _NA_0_20101015_SOLAR_SHOP_TEMP_REFACTORING
    CashCategoryParser::Instance()->Init();
#endif

#ifdef _NA_0_20100901_SKILLTREE_SYSTEM
    SkillInventoryStoreParser::Instance()->Init(50);
#endif
#ifdef _NA_003027_20111013_HONOR_SYSTEM
    TitleListParser::Instance()->Init();
    ReputeListParser::Instance()->Init();
    FameListParser::Instance()->Init();
#endif //_NA_003027_20111013_HONOR_SYSTEM
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    ChaosZoneInfoParser::Instance()->Init();
    BattleGroundInfoParser::Instance()->Init();
    BattleGroundFlagInfoParser::Instance()->Init(100);
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#ifdef _NA_007065_20140217_CHAOSZONE_GOLDRUSH
    GoldRushInfoParser::Instance()->Init();
    GoldRushMonsterPointParser::Instance()->Init();
#endif //_NA_007065_20140217_CHAOSZONE_GOLDRUSH
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    GuildFacilityInfoParser::Instance()->Init(0);
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
#ifdef _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
    AttendanceParser::Instance()->Init(0);
#endif // _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
#ifdef _NA_008012_20150204_SUN_RANKING_SYSTEM
    SunRankingParser::Instance()->Init(0);
#endif // _NA_008012_20150204_SUN_RANKING_SYSTEM
#ifdef _NA_008124_20150313_AWAKENING_SYSTEM
    AwakeningInfoParser::Instance()->Init(0);
#endif // _NA_008124_20150313_AWAKENING_SYSTEM
#ifdef _NA_008283_20150511_NOTIFICATIONS_SYSTEM
    NotificationInfoParser::Instance()->Init(0);
#endif // _NA_008283_20150511_NOTIFICATIONS_SYSTEM
#ifdef _NA_008404_20150712_MINIGAME_SYSTEM
    MiniGameParser::Instance()->Init(0);
#endif // _NA_008404_20150712_MINIGAME_SYSTEM
}

BOOL	GameScriptParserManager::RegisterParser()
{

	ERROR_PARSER( RegisterScriptData( SCRIPT_SERVER_OPTION, "data\\ServerOptionEx.ini", ServerOptionParserEx::Instance() ) );
	ERROR_PARSER( RegisterScriptData( SCRIPT_GAME_OPTION, "data\\GameOption.ini", GameOptionParser::Instance() ) );
#if !defined(_NA000000_GM_COMMAND_CHANGE_CONCEPT_)
	ERROR_PARSER( RegisterScriptData( SCRIPT_DEVCOMMAND, "data\\DevCommand.ini", GMCmdManager::Instance() ) );
	ERROR_PARSER( RegisterScriptData( SCRIPT_GMCOMMAND, "data\\GMCommand.ini", GMCmdManager::Instance() ) );
#endif
	ERROR_PARSER( RegisterScriptData( SCRIPT_ACCESSORYITEMINFO, "data\\AccessoryItemInfo.txt", ItemInfoParser::Instance() ) );
	ERROR_PARSER( RegisterScriptData( SCRIPT_ARMORITEMINFO, "data\\ArmorItemInfo.txt", ItemInfoParser::Instance() ) );
	ERROR_PARSER( RegisterScriptData( SCRIPT_WASTEITEMINFO, "data\\WasteItemInfo.txt", ItemInfoParser::Instance() ) );
	ERROR_PARSER( RegisterScriptData( SCRIPT_WEAPONITEMINFO, "data\\WeaponItemInfo.txt", ItemInfoParser::Instance() ) );
	ERROR_PARSER( RegisterScriptData( SCRIPT_CHARGEITEMINFO, "data\\ChargeItemInfo.txt", ItemInfoParser::Instance() ) );

#ifdef _NA_0_20100901_SKILLTREE_SYSTEM
    ERROR_PARSER(RegisterScriptData( SCRIPT_SKILL_INVEN_STORE, "data\\SkillInventoryStore.txt", SkillInventoryStoreParser::Instance() ) );
#endif

    ERROR_PARSER( RegisterScriptData( SCRIPT_SKILLINFO, "data\\SkillInfo.txt", SkillInfoParser::Instance() ) );
	ERROR_PARSER( RegisterScriptData( SCRIPT_STYLEINFO, "data\\StyleInfo.txt", SkillInfoParser::Instance() ) );

#ifdef _NA002217_100728_EP2_PERK_SYSTEM_
    ERROR_PARSER(RegisterScriptData(\
                 SCRIPT_PERKINFO, "data\\PerkInfo.txt", PerkInfoParser::Instance()));
#endif
	ERROR_PARSER( RegisterScriptData( SCRIPT_NPCINFO, "data\\NPCInfo.txt", NPCInfoParser::Instance() ) );
	ERROR_PARSER( RegisterScriptData( SCRIPT_NPCEXTRAINFO, "data\\NPCExtraInfo.txt", NPCInfoParser::Instance() ) );

	ERROR_PARSER( RegisterScriptData( SCRIPT_AICOMMONINFO, "data\\AICommonInfo.txt", AIParamParser::Instance() ) );
	ERROR_PARSER( RegisterScriptData( SCRIPT_AITYPEINFO, "data\\AITypeInfo.txt", AIParamParser::Instance() ) );

	ERROR_PARSER( RegisterScriptData( SCRIPT_WORLD_SUB_INFO, "data\\Worldsubinfo.txt", MapInfoParser::Instance() ) );

	ERROR_PARSER( RegisterScriptData( SCRIPT_WORLD, "data\\World.txt", MapInfoParser::Instance() ) );

	ERROR_PARSER( RegisterScriptData( SCRIPT_FIELD, "data\\Field.txt", MapInfoParser::Instance(), SEPERATOR_WHITESPACE, NULL, TRUE ));
	ERROR_PARSER( RegisterScriptData( SCRIPT_FIELDWMO, "data\\Fieldwmo.txt", MapInfoParser::Instance(), SEPERATOR_WHITESPACE, NULL, TRUE ));
	ERROR_PARSER( RegisterScriptData( SCRIPT_FIELDWUT, "data\\Fieldwut.txt", MapInfoParser::Instance(), SEPERATOR_WHITESPACE, NULL, TRUE ));

	ERROR_PARSER( RegisterScriptData( SCRIPT_MAPENTERANCEPORTAL, "data\\MapEnterancePortal.txt", MapInfoParser::Instance() ) );

	ERROR_PARSER( RegisterScriptData( SCRIPT_GROUP, "data\\Group.txt", GroupParser::Instance() ) ); //의존성 NPCInfo

	ERROR_PARSER( RegisterScriptData( SCRIPT_REGEN, "data\\Regen.txt", RegenParser::Instance() ) );	// 의존성 Group
	ERROR_PARSER( RegisterScriptData( SCRIPT_REGEN_POS, "data\\Regen_pos.txt", RegenParser::Instance() ) );

	ERROR_PARSER( RegisterScriptData( SCRIPT_LIMITED_ITEM_INFO, "data\\Limitediteminfo.txt", LimitedItemInfoParser::Instance() ) );	

	ERROR_PARSER( RegisterScriptData( SCRIPT_DROPRATIOINFO, "data\\DropRatioInfo.txt", &g_DropManager) );
	ERROR_PARSER( RegisterScriptData( SCRIPT_ITEMTYPEDROPRATIO, "data\\ItemTypeDropRatio.txt", &g_DropManager) );
	ERROR_PARSER( RegisterScriptData( SCRIPT_ITEMTYPELIST, "data\\ItemTypeList.txt", &g_DropManager ) );

#ifdef _NA_008012_20150130_RANKING_SYSTEM
	ERROR_PARSER( RegisterScriptData( SCRIPT_MISSIONRANKINGREWARD, "data\\MissionRankingReward.txt", &g_GameRankingMgr) );
	ERROR_PARSER( RegisterScriptData( SCRIPT_MISSIONRANKINGLVAREA, "data\\MissionRankingLVArea.txt", &g_GameRankingMgr) );
#endif // _NA_008012_20150130_RANKING_SYSTEM

	ERROR_PARSER( RegisterScriptData( SCRIPT_SHOPINFO, "data\\ShopInfo.txt", ShopInfoParser::Instance() ) );
	ERROR_PARSER( RegisterScriptData( SCRIPT_STATEINFO, "data\\StateInfo.txt", StateInfoParser::Instance() ) );
	ERROR_PARSER( RegisterScriptData( SCRIPT_ITEMCOMPOSITEINFO, "data\\ItemCompositeInfo.txt", ItemCompositeParser::Instance() ) );
#ifdef _NA_001699_20110104_SOCKET_OPTION_POINT
    ERROR_PARSER(RegisterScriptData(SCRIPT_ZARD_OPTION_INFO, _T("data\\ZardOptionInfo.txt"), SocketOptionParser::Instance()));
    ERROR_PARSER(RegisterScriptData(SCRIPT_ZARD_POINT_INFO, _T("data\\ZardPointInfo.txt"), SocketOptionParser::Instance()));
#else
    ERROR_PARSER( RegisterScriptData( SCRIPT_SOCKETOPTIONINFO, "data\\SocketOptionInfo.txt", SocketOptionParser::Instance() ) );
#endif // _NA_001699_20110104_SOCKET_OPTION_POINT
	ERROR_PARSER( RegisterScriptData( SCRIPT_MISSIONREWARDINFO, "data\\MissionRewardInfo.txt", MissionRewardParser::Instance() ) );
	ERROR_PARSER( RegisterScriptData( SCRIPT_FORMULARATIO, "data\\FormulaRatio.txt", FormulaRatioParser::Instance() ) );
	ERROR_PARSER( RegisterScriptData( SCRIPT_STYLEQUICKREGISTINFO, "data\\StyleQuickRegistInfo.txt", StyleQuickRegistInfoParser::Instance() ) );
	ERROR_PARSER( RegisterScriptData( SCRIPT_ENCHANT, "data\\Enchant.txt", EnchantParser::Instance() ) );
	ERROR_PARSER( RegisterScriptData( SCRIPT_EXTRASTONEOPTIONINFO, "data\\ExtraStoneOptionInfo.txt", ExtraStoneOptionInfoParser::Instance() ) );
	ERROR_PARSER( RegisterScriptData( SCRIPT_ITEMCOMPOSELIST, "data\\ItemComposeList.txt", ItemComposeListParser::Instance() ) );
	ERROR_PARSER( RegisterScriptData( SCRIPT_ITEMCRYSTALIZELIST, "data\\ItemCrystalizeList.txt", ItemCrystalizeListParser::Instance() ) );
	ERROR_PARSER( RegisterScriptData( SCRIPT_EXPVALUEINFO, "data\\ExpValueInfo.txt", ExpValueInfoParser::Instance() ) );
	ERROR_PARSER( RegisterScriptData( SCRIPT_VSHUNTINGREWARDINFO, "data\\VSHuntingRewardInfo.txt", CHuntingRewardParser::Instance() ) );
	ERROR_PARSER( RegisterScriptData( SCRIPT_REWARDINFOLIST, "data\\RewardInfoList.txt", RewardInfoList::Instance() ) );
	ERROR_PARSER( RegisterScriptData( SCRIPT_SUNEVENTINFO, "data\\SunEventInfo.txt", SunEventInfoParser::Instance() ) );
	ERROR_PARSER( RegisterScriptData( SCRIPT_ARMORTYPEDAMAGEINFO, "data\\ArmorTypeDamageInfo.txt", ArmorDamageParser::Instance() ) );
	ERROR_PARSER( RegisterScriptData( SCRIPT_FATIGUEOPTION, "data\\FatigueOption.ini", FatigueOptionParser::Instance() ) );
	ERROR_PARSER( RegisterScriptData( SCRIPT_PKSCRIPT, "data\\PKScript.txt", PKScriptParser::Instance() ) );
	ERROR_PARSER( RegisterScriptData( SCRIPT_RANKOPTIONINFO, "data\\RankOptionInfo.txt", RankOptionParser::Instance() ) );
	ERROR_PARSER( RegisterScriptData( SCRIPT_INITCHARINFO, "data\\InitCharInfoList.txt", CharInfoParser::Instance() ) );
	ERROR_PARSER( RegisterScriptData( SCRIPT_MONSTERSUMMON, "data\\MonsterSummon.txt", MonsterSummonParser::Instance() ) );
	ERROR_PARSER( RegisterScriptData( SCRIPT_MAP_PARTIAL_LOADING, "data\\MapPartialLoading.ini", MapPartialLoadingParser::Instance() ) );
	ERROR_PARSER( RegisterScriptData( SCRIPT_FILTER, "data\\Filter.txt", ProhibitionWordParser::Instance() ) );
	ERROR_PARSER( RegisterScriptData( SCRIPT_FILTERNAME, "data\\FilterName.txt", ProhibitionWordParser::Instance() ) );
#ifdef _NA_008012_20150130_RANKING_SYSTEM
	ERROR_PARSER( RegisterScriptData( SCRIPT_POINT_RANKING, "data\\PointRankingRule.txt", PointRankingParser::Instance() ) );
#endif // _NA_008012_20150130_RANKING_SYSTEM

#ifdef __NA_001027_20080423_RANDOM_REWARD_EVENT_
	ERROR_PARSER( RegisterScriptData( SCRIPT_EXCHANGE_REWARD_INFO, "data\\ExchangeRewardInfo.txt", ExchangeRewardInfoParser::Instance() ) );
#endif

	ERROR_PARSER( RegisterScriptData( SCRIPT_GAMBLE_OPTION, "data\\Gambleoptionlist.txt", LimitedOptionParser::Instance() ) );	
	ERROR_PARSER( RegisterScriptData( SCRIPT_SETITEM_OPTION_INFO, "data\\SetItemOptionInfo.txt", SetItemOptionInfoParser::Instance() ) );
	ERROR_PARSER( RegisterScriptData( SCRIPT_GUILD_RANK_INFO, "data\\GuildRankInfo.txt", GuildRankInfoParser::Instance() ) );
	ERROR_PARSER( RegisterScriptData( SCRIPT_GUILD_RIGHT_INFO, "data\\GuildRightInfo.txt", GuildRightInfoParser::Instance() ) );
	ERROR_PARSER( RegisterScriptData( SCRIPT_FATE_ITEM_INFO, "data\\Luckyiteminfo.txt", FateItemInfoParser::Instance() ) );	

	ERROR_PARSER( RegisterScriptData( SCRIPT_EVENTREWARD_INFO, "data\\EventRewardInfo.txt", SunEventInfoParser::Instance() ) );
	ERROR_PARSER( RegisterScriptData( SCRIPT_PET_INFO, "data\\PetManageInfo.txt", PetInfoParser::Instance() ) );
	ERROR_PARSER( RegisterScriptData( SCRIPT_ETHER_BULLET_INFO, "data\\EtherBulletInfo.txt", EtherBulletInfoParser::Instance() ) );
	ERROR_PARSER( RegisterScriptData( SCRIPT_LOTTO_INFO, "data\\LotteryInfo.txt", LotteryInfoParser::Instance()) );
//더이상 사용하지 않거나 그런것들..
/*x*/	ERROR_PARSER( RegisterScriptData( SCRIPT_QUESTINFO, "data\\QuestInfo.txt", &g_QuestInfoManager) );
#ifdef C_NA_0_20100520_ACSYSTEM_CHANGED
	ERROR_PARSER( RegisterScriptData( SCRIPT_ACHIEVEMENT_INFO, "data\\ACScript.txt", AchievementParser::Instance() ) );
#else
	ERROR_PARSER( RegisterScriptData( SCRIPT_ACSCRIPT, "data\\ACScript.txt", AreaConquestInfoParser::Instance() ) );
#endif	
/*x*/	
		//ERROR_PARSER( RegisterScriptData( SCRIPT_COMPOSITE_NPC), NULL),
		//ERROR_PARSER( RegisterScriptData( SCRIPT_INITCHARINFOLIST), NULL),
		//ERROR_PARSER( RegisterScriptData( SCRIPT_ITEMCOMPOSITERESULTINFO), NULL),
		//ERROR_PARSER( RegisterScriptData( SCRIPT_MATCHINFO), NULL),
		//ERROR_PARSER( RegisterScriptData( SCRIPT_REQUIREFIELDLEVELINFO), NULL),
		//ERROR_PARSER( RegisterScriptData( SCRIPT_SHOUTRANGE), NULL),

	ERROR_PARSER( RegisterScriptData( SCRIPT_SSQINFO, "data\\SSQInfo.txt", SSQInfoParser::Instance() ) );
	ERROR_PARSER( RegisterScriptData( SCRIPT_SSQPATTERN, "data\\SSQPattern.txt", SSQInfoParser::Instance() ) );

#ifdef __NA_001062_20080623_STRENGTH_PENALTY
	ERROR_PARSER( RegisterScriptData( SCRIPT_STRENGTH_PENALTY, "data\\StrengthPenalty.txt", StrengthPenaltyParser::Instance() ) );
#endif
	ERROR_PARSER( RegisterScriptData(SCRIPT_COLLECT, "data\\CollectInfo.txt", CollectionParser::Instance() ) );
	ERROR_PARSER( RegisterScriptData(SCRIPT_GROUPINFO, "data\\GroupInfo.txt", GroupInfoParser::Instance() ) );
    ERROR_PARSER(RegisterScriptData(SCRIPT_RIDING_INFO, "data\\RidingInfo.txt", RidingInfoParser::Instance()));
    //_KR_001385_20090907_DOMINATION_PARSER
    ERROR_PARSER(RegisterScriptData(SCRIPT_DOMINATIONINFO, "data\\DominationInfo.txt", DominationInfoParser::Instance()));
    ERROR_PARSER(RegisterScriptData(SCRIPT_DOMINATION_EVENT_INFO, "data\\DominationTimeInfo.txt", DominationInfoParser::Instance()));
#ifdef _NA_1419_20100121_ACCUMULATE_POINT
    ERROR_PARSER(RegisterScriptData(SCRIPT_ACCUMULATE_POINT, "data\\AccumulatePoint.txt", AccumulatePointParser::Instance()));
#endif

#ifdef _NA_0_20101015_SOLAR_SHOP_TEMP_REFACTORING
    ERROR_PARSER(RegisterScriptData(SCRIPT_CASHCATEGORY, "data\\CashCategory.txt", CashCategoryParser::Instance()));
#endif

#ifdef _NA_000251_20100727_SOCKET_SYSTEM
    ERROR_PARSER(RegisterScriptData(SCRIPT_SOCKET_ITEM_RATIO_INFO, _T("data\\SocketItemRatio.txt"), SocketItemRatioParser::Instance()));
    ERROR_PARSER(RegisterScriptData(SCRIPT_ZARD_COMPOSE_INFO, _T("data\\ZardComposeinfo.txt"), ZardComposeInfoParser::Instance()));
#endif // _NA_000251_20100727_SOCKET_SYSTEM

#ifdef _NA_002253_20100811_CUBE_SYSTEM
    ERROR_PARSER(RegisterScriptData(SCRIPT_CUBE_COMPOSE, _T("data\\CubeComposeInfo.txt"), CubeSystemParser::Instance()));
    ERROR_PARSER(RegisterScriptData(SCRIPT_CUBE_DECOMPOSE, _T("data\\CubeDecompositionInfo.txt"), CubeSystemParser::Instance()));
#endif // _NA_002253_20100811_CUBE_SYSTEM
#ifdef _USE_MAX_CHAR_LIST_PARSER
    ERROR_PARSER(RegisterScriptData(SCRIPT_MAX_CHAR_LIST, _T("data\\MaxCharList.txt"), MaxCharListParser::Instance()));
#endif
#ifdef _NA000000_GM_COMMAND_CHANGE_CONCEPT_
    ERROR_PARSER(RegisterScriptData(SCRIPT_GMRIGHTS, "data\\GMRights.txt", GMCmdManager::Instance()));
#endif
#ifdef _NA_003027_20111013_HONOR_SYSTEM
    ERROR_PARSER(RegisterScriptData(SCRIPT_TITLE_LIST, "data\\TitleList.txt", TitleListParser::Instance()));
    ERROR_PARSER(RegisterScriptData(SCRIPT_REPUTE_LIST, "data\\ReputeList.txt", ReputeListParser::Instance()));
    ERROR_PARSER(RegisterScriptData(SCRIPT_FAME_LIST, "data\\FameList.txt", FameListParser::Instance()));
#endif //_NA_003027_20111013_HONOR_SYSTEM
#ifdef _NA_002935_20110704_ITEM_SCORE
    ERROR_PARSER(RegisterScriptData(SCRIPT_ITEM_SCORE_COMMON, _T("data\\ItemScoreInfo.txt"), ItemScoreParser::Instance()));
#endif // _NA_002935_20110704_ITEM_SCORE

    //_NA_20110630_BATTLE_ZONE_RENEWAL
    ERROR_PARSER(RegisterScriptData(SCRIPT_BATTLEZONE_TREE, "data\\BattlezoneMissiontree.txt", BattlezoneMissionTreeParser::Instance()));

#ifdef _NA_003649_20111115_BATTLE_ZONE_HARDMODE
    ERROR_PARSER(RegisterScriptData( SCRIPT_NPCREPLACE_INFO, "data\\NPCReplaceInfo.txt", NPCReplaceInfoParser::Instance()));
#endif
#ifdef _NA_003740_20111122_RANDOM_ITEM
    ERROR_PARSER(RegisterScriptData(SCRIPT_RANDOM_ITEM_VALUE, _T("data\\RandomItemValue.txt"), RandomItemParser::Instance()));
    ERROR_PARSER(RegisterScriptData(SCRIPT_RANDOM_ITEM_OPTION_TYPE, _T("data\\RandomItemOptionType.txt"), RandomItemParser::Instance()));
    ERROR_PARSER(RegisterScriptData(SCRIPT_RANDOM_ITEM_INFO, _T("data\\RandomItemInfo.txt"), RandomItemParser::Instance()));
    ERROR_PARSER(RegisterScriptData(SCRIPT_RANDOM_ITEM_KEY, _T("data\\RandomItemKey.txt"), RandomItemParser::Instance()));
#endif
#ifdef _NA004034_20120102_POINT_WALLET_SYSTEM
    ERROR_PARSER(RegisterScriptData(SCRIPT_CURRENCY_INFO, _T("data\\CurrencyInfo.txt"), CurrencyInfoParser::Instance()));
    // 아래처럼 호출하면 명시적으로 DestroyInstance() 호출하지 않아도 자동으로 인스턴스를 제거해 줍니다.
    // 앞으로 파서를 등록할때는 이 버전을 사용하시기 바랍니다. (hukim)
    //if (RegisterScript<CurrencyInfoParser>(SCRIPT_CURRENCY_INFO, _T("data\\CurrencyInfo.txt")) == false)
    //{
    //    return FALSE;
    //}
#endif //_NA004034_20120102_POINT_WALLET_SYSTEM

#ifdef _NA_003923_20120130_GUILD_RENEWAL
    ERROR_PARSER(RegisterScript<GuildInfoParser>(SCRIPT_GUILDINFO, _T("data\\GuildInfo.txt")));
#endif
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    ERROR_PARSER(RegisterScript<ChaosZoneInfoParser>(SCRIPT_CHAOSZONE_INFO, _T("data\\ChaosZoneInfo.txt")));
    ERROR_PARSER(RegisterScript<BattleGroundInfoParser>(SCRIPT_BATTLE_GROUND_INFO, _T("data\\BattleGroundInfo.txt")));
    ERROR_PARSER(RegisterScript<BattleGroundFlagInfoParser>(SCRIPT_BATTLE_GROUND_FLAG_INFO, _T("data\\BattleGroundFlagInfo.txt")));
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#ifdef _NA_006538_BATTLE_GROUND_TIME_CONTROL_SYSTEM
    ERROR_PARSER(RegisterScript<BattleGroundTimeInfoParser>(SCRIPT_BATTLE_GROUND_TIME_INFO, _T("data\\BattleGroundTimeInfo.txt")));
#endif // _NA_006538_BATTLE_GROUND_TIME_CONTROL_SYSTEM
#ifdef _NA_007065_20140217_CHAOSZONE_GOLDRUSH
    ERROR_PARSER(RegisterScript<GoldRushInfoParser>(SCRIPT_GOLDRUSH_INFO, _T("data\\GoldRushInfo.txt")));
    ERROR_PARSER(RegisterScript<GoldRushMonsterPointParser>(SCRIPT_GOLDRUSH_MONSTERPOINT, _T("data\\GoldRushMonsterPoint.txt")));
#endif //_NA_007065_20140217_CHAOSZONE_GOLDRUSH
#ifdef _NA_007086_20140318_MONSTERBOOK
    ERROR_PARSER( RegisterScriptData( SCRIPT_MONSTER_BOOK, "data\\MonsterBook.txt", MonsterBookParser::Instance() ) );
#endif //_NA_007086_20140318_MONSTERBOOK
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    ERROR_PARSER( RegisterScriptData( SCRIPT_GUILD_FACILITY_INFO, "data\\GuildFacilityInfo.txt", GuildFacilityInfoParser::Instance() ) );
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
#ifdef _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
    ERROR_PARSER( RegisterScriptData( SCRIPT_ATTENDANCE, "data\\Attendance.txt", AttendanceParser::Instance() ) );
#endif // _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
#ifdef _NA_008012_20150204_SUN_RANKING_SYSTEM
    ERROR_PARSER( RegisterScriptData( SCRIPT_SUN_RANKING, "data\\SunRanking.ini", SunRankingParser::Instance() ) );
#endif // _NA_008012_20150204_SUN_RANKING_SYSTEM
#ifdef _NA_008124_20150313_AWAKENING_SYSTEM
    ERROR_PARSER( RegisterScriptData(SCRIPT_AWAKENING_INFO, "data\\AwakeningInfo.txt",AwakeningInfoParser::Instance() ) );
#endif // _NA_008124_20150313_AWAKENING_SYSTEM
#ifdef _NA_008283_20150511_NOTIFICATIONS_SYSTEM
    ERROR_PARSER( RegisterScriptData(SCRIPT_NOTIFICATION_INFO, "data\\NotificationInfo.txt",NotificationInfoParser::Instance() ) );
#endif // _NA_008283_20150511_NOTIFICATIONS_SYSTEM
#ifdef _NA_008404_20150712_MINIGAME_SYSTEM
    ERROR_PARSER( RegisterScriptData(SCRIPT_MINIGAME, "data\\MiniGame.ini",MiniGameParser::Instance() ) );
#endif // _NA_008404_20150712_MINIGAME_SYSTEM
#ifdef _NA_008334_20150608_SONNENSCHEIN
    ERROR_PARSER( RegisterScriptData(SCRIPT_SONNENSCHEIN_REWARDINFO, "data\\SonnenScheinReward.txt", SonnenScheinRewardInfoParser::Instance()));
#endif //_NA_008334_20150608_SONNENSCHEIN
#ifdef _NA_008543_20151027_ONE_DAYS_GUILD_MISSION
	ERROR_PARSER( RegisterScriptData(SCRIPT_ONEDAYSBUILDMISSION, "data\\OneDaysGuildmissionInfo.txt", OneDaysGuildMissionInfoParser::Instance()));
#endif //_NA_008543_20151027_ONE_DAYS_GUILD_MISSION

#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
	sSCRIPT_DATA_INFO_NODE pNodeTable[SCRIPT_LIST_SIZE];	
	if( !GetScriptDataNodeList( pNodeTable, SCRIPT_LIST_SIZE ) )
	{
		SUNLOG( eCRITICAL_LOG, "GetScriptDataNodeList Error!" );
		return FALSE;
	}
	ScriptDataManager::RegisterDataList( pNodeTable );
	ScriptDataManager::RegisterCallbackAllDataLoaded( &GameServerEx::ReadyToStartServer );
#endif //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__

	SUNLOG( eFULL_LOG, "Parser Register Complete" );

	return TRUE;

}

VOID	GameScriptParserManager::DestroyInstanceEx()
{
// Release는 할 필요 없다. Release는 종료시 명시적으로 스크립트 매니져에서 호출해 준다.

	GMCmdManager::DestroyInstance();
	ItemInfoParser::DestroyInstance();
	NPCInfoParser::DestroyInstance();
	MapInfoParser::DestroyInstance();
	ShopInfoParser::DestroyInstance();
	SkillInfoParser::DestroyInstance();
	RegenParser::DestroyInstance();
	GroupParser::DestroyInstance();
	StateInfoParser::DestroyInstance();
	ItemCompositeParser::DestroyInstance();
	RankOptionParser::DestroyInstance();
	SocketOptionParser::DestroyInstance();

	MissionRewardParser::DestroyInstance();
	FormulaRatioParser::DestroyInstance();
	StyleQuickRegistInfoParser::DestroyInstance();
	AIParamParser::DestroyInstance();
	EnchantParser::DestroyInstance();
	ExtraStoneOptionInfoParser::DestroyInstance();
	ItemComposeListParser::DestroyInstance();
	ItemCrystalizeListParser::DestroyInstance();
	ExpValueInfoParser::DestroyInstance();
	CHuntingRewardParser::DestroyInstance();

	RewardInfoList::DestroyInstance();
	SunEventInfoParser::DestroyInstance();
#ifdef C_NA_0_20100520_ACSYSTEM_CHANGED
	AchievementParser::DestroyInstance();
#else
	AreaConquestInfoParser::DestroyInstance();
#endif
	ArmorDamageParser::DestroyInstance();
	PKScriptParser::DestroyInstance();

	ItemTypeList::DestroyInstance();
	DropRatioInfoList::DestroyInstance();

	MonsterSummonParser::Instance()->Release();
	MonsterSummonParser::DestroyInstance();

	FatigueOptionParser::DestroyInstance();
	MapPartialLoadingParser::DestroyInstance();
	ProhibitionWordParser::DestroyInstance();
#ifdef _NA_008012_20150130_RANKING_SYSTEM
    PointRankingParser::DestroyInstance();
#endif // _NA_008012_20150130_RANKING_SYSTEM

#ifdef __NA_001027_20080423_RANDOM_REWARD_EVENT_
	ExchangeRewardInfoParser::DestroyInstance();
#endif

	LimitedItemInfoParser::DestroyInstance();
	LimitedOptionParser::DestroyInstance();

	ServerOptionParserEx::DestroyInstance();
	GameOptionParser::DestroyInstance();
	SetItemOptionInfoParser::DestroyInstance();
	GuildRankInfoParser::DestroyInstance();
	GuildRightInfoParser::DestroyInstance();
	FateItemInfoParser::DestroyInstance();
	PetInfoParser::DestroyInstance();
	EtherBulletInfoParser::DestroyInstance();
	SSQInfoParser::DestroyInstance();
	LotteryInfoParser::DestroyInstance();
#ifdef __NA_001062_20080623_STRENGTH_PENALTY
	StrengthPenaltyParser::DestroyInstance();
	StrengthUIParser::DestroyInstance();
#endif
	GroupParser::DestroyInstance();
    CollectionParser::DestroyInstance();
    GroupInfoParser::DestroyInstance();
    RidingInfoParser::DestroyInstance();
    DominationInfoParser::DestroyInstance();
#ifdef _NA_1419_20100121_ACCUMULATE_POINT
    AccumulatePointParser::DestroyInstance();
#endif

#ifdef _NA_0_20101015_SOLAR_SHOP_TEMP_REFACTORING
    CashCategoryParser::DestroyInstance();
#endif

#ifdef _NA_0_20100901_SKILLTREE_SYSTEM
    SkillInventoryStoreParser::DestroyInstance();
#endif

#ifdef _NA002217_100728_EP2_PERK_SYSTEM_
    PerkInfoParser::DestroyInstance();
#endif
#ifdef _NA_002253_20100811_CUBE_SYSTEM
    CubeSystemParser::DestroyInstance();
#endif // _NA_002253_20100811_CUBE_SYSTEM
#ifdef _NA_000251_20100727_SOCKET_SYSTEM
    SocketItemRatioParser::DestroyInstance();
    ZardComposeInfoParser::DestroyInstance();
#endif // _NA_000251_20100727_SOCKET_SYSTEM
#ifdef _USE_MAX_CHAR_LIST_PARSER
    MaxCharListParser::DestroyInstance();
#endif
    //_NA_20110630_BATTLE_ZONE_RENEWAL
    BattlezoneMissionTreeParser::DestroyInstance();

#ifdef _NA_002935_20110704_ITEM_SCORE
    ItemScoreParser::DestroyInstance();
#endif // _NA_002935_20110704_ITEM_SCORE
#ifdef _NA_003027_20111013_HONOR_SYSTEM
    TitleListParser::DestroyInstance();
    ReputeListParser::DestroyInstance();
    FameListParser::DestroyInstance();
#endif //_NA_003027_20111013_HONOR_SYSTEM
#ifdef _NA_003649_20111115_BATTLE_ZONE_HARDMODE
    NPCReplaceInfoParser::DestroyInstance();
#endif //_NA_003649_20111115_BATTLE_ZONE_HARDMODE
#ifdef _NA_003740_20111122_RANDOM_ITEM
    RandomItemParser::DestroyInstance();
#endif // _NA_003740_20111122_RANDOM_ITEM
#ifdef _NA004034_20120102_POINT_WALLET_SYSTEM
    CurrencyInfoParser::DestroyInstance();
#endif //_NA004034_20120102_POINT_WALLET_SYSTEM

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    ChaosZoneInfoParser::DestroyInstance();
    BattleGroundInfoParser::DestroyInstance();
    BattleGroundFlagInfoParser::DestroyInstance();
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#ifdef _NA_007065_20140217_CHAOSZONE_GOLDRUSH
    GoldRushInfoParser::DestroyInstance();
    GoldRushMonsterPointParser::DestroyInstance();
#endif //_NA_007065_20140217_CHAOSZONE_GOLDRUSH
#ifdef _NA_007086_20140318_MONSTERBOOK
    MonsterBookParser::DestroyInstance();
#endif //_NA_007086_20140318_MONSTERBOOK
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    GuildFacilityInfoParser::DestroyInstance();
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
#ifdef _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
    AttendanceParser::DestroyInstance();
#endif // _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
#ifdef _NA_008012_20150204_SUN_RANKING_SYSTEM
    SunRankingParser::DestroyInstance();
#endif // _NA_008012_20150204_SUN_RANKING_SYSTEM
#ifdef _NA_008124_20150313_AWAKENING_SYSTEM
    AwakeningInfoParser::DestroyInstance();
#endif // _NA_008124_20150313_AWAKENING_SYSTEM
#ifdef _NA_008283_20150511_NOTIFICATIONS_SYSTEM
    NotificationInfoParser::DestroyInstance();
#endif // _NA_008283_20150511_NOTIFICATIONS_SYSTEM
#ifdef _NA_008334_20150608_SONNENSCHEIN
    SonnenScheinRewardInfoParser::DestroyInstance();
#endif //_NA_008334_20150608_SONNENSCHEIN
#ifdef _NA_008404_20150712_MINIGAME_SYSTEM
    MiniGameParser::DestroyInstance();
#endif // _NA_008404_20150712_MINIGAME_SYSTEM
#ifdef _NA_008543_20151027_ONE_DAYS_GUILD_MISSION
	OneDaysGuildMissionInfoParser::DestroyInstance();
#endif //_NA_008543_20151027_ONE_DAYS_GUILD_MISSION

	SUNLOG( eFULL_LOG, "Parser Destroy Complete" );
}













