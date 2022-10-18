#include "StdAfx.h"
#include ".\dbproxyscriptparsermanager.h"

#include <RankingManager.h>
#include <ItemInfoParser.h>
#include <SunEventInfoParser.h>
#include <NewCharItemInfoParser.h>
#include <CharInfoParser.h> // NOTE: f110427.3L, added by _NA_002050_20110216_ADD_GENDER
#include <ExtraStoneOptionInfoParser.h>
#include <NPCInfoParser.h>
#include <PetInfoParser.h> // (f100603.3L) change a policy to parse the db proxy server has a pet info.

#include <QuestScriptLoader.h>
#include <MapInfoParser.h>
// (f100713.2L) import a drop information to support a changed new char information table
#include <ItemTypeList.h>
#include <SkillSystem/PerkInfoParser.h>
#include "SocketSystem/SocketItemRatioParser.h" //_NA_000251_20100727_SOCKET_SYSTEM
#ifdef _USE_MAX_CHAR_LIST_PARSER
#include <MaxCharListParser.h>
#endif
//_NA002635_GZ_DZ_PROTOCOL_UINIFICATION_IN_DBP = {
#include <GuildRankInfoParser.h>
#include <GuildRightInfoParser.h>
#include <MaxGuildMemberInfoParser.h>
#include <DominationInfoParser.h>
//}
//_NA_003923_20120130_GUILD_RENEWAL
#include <GuildInfoParser.h>
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
#include <GuildFacilityInfoParser.h>
#endif //_NA_007330_20140620_GUILD_SYSTEM_EXTENSION
#include "DBProxyServerEx.h"

// @history
// ~10/01/04
// 10/01/04|waverix|add area conquest parser for db batch unit|
// 10/01/06|waverix|add extra stone parser for SCItemSlot enable valid conversion stream|
#include "Services/PortalRankingManager.h"
#if !defined(_NA002635_GP_DP_PROTOCOL_UINIFICATION)
extern RankingManager g_RankingHelper;
#endif

#if DBPROXY_TYPE_SELECTOR == DBPROXY_TYPE_UNIT

#include "SpecialPurposeUnits/BatchUnitFactory.hxx"
#include "SpecialPurposeUnits/QuestManagerForUnit.h"
#ifdef C_NA_0_20100520_ACSYSTEM_CHANGED
#include <AchievementParser.h>
#endif

#endif // (DBPROXY_TYPE_SELECTOR == DBPROXY_TYPE_UNIT)


DBProxyScriptParserManager::DBProxyScriptParserManager()
{
}

DBProxyScriptParserManager::~DBProxyScriptParserManager()
{
}

void DBProxyScriptParserManager::InitParserPool(void* pool_option)
{
    __UNUSED(pool_option);
#if DBPROXY_TYPE_SELECTOR != DBPROXY_TYPE_ACCOUNT
    ItemInfoParser::Instance()->Init(3000);
    ExtraStoneOptionInfoParser::Instance()->Init(10);
#ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION
    MapInfoParser::Instance()->Init(10);
#endif //_NA_007128_20140326_MONDSCHIEN_DOMINATION
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    GuildFacilityInfoParser::Instance()->Init(0);
#endif //_NA_007330_20140620_GUILD_SYSTEM_EXTENSION
  #if DBPROXY_TYPE_SELECTOR == DBPROXY_TYPE_UNIT
    AreaConquestInfoParser::Instance()->Init(10);
    MapInfoParser::Instance()->Init(10);
    batch_unit::quest_script_parser_global.SetQuestInfoManager(\
        &batch_unit::quest_info_manager_global_);
    batch_unit::quest_script_parser_global.SetEventRegister(\
        &batch_unit::quest_event_register_dummy_);
    #ifdef C_NA_0_20100520_ACSYSTEM_CHANGED
        AchievementParser::Instance()->Init(100);
    #endif
        NPCInfoParser::Instance()->Init(100);
  #endif
#endif
}

BOOL DBProxyScriptParserManager::RegisterParser()
{
//==================================================================================================
#  if DBPROXY_TYPE_SELECTOR == DBPROXY_TYPE_ACCOUNT
//==================================================================================================
    return true;
//==================================================================================================
#elif DBPROXY_TYPE_SELECTOR == DBPROXY_TYPE_DEFAULT
//==================================================================================================

  #ifdef _NA002635_GP_DP_PROTOCOL_UINIFICATION
#ifdef _NA_008012_20150130_RANKING_SYSTEM
    ERROR_PARSER(RegisterScriptData(SCRIPT_MISSIONRANKINGREWARD,
        "data\\MissionRankingReward.txt", &g_RankingMgr));
    ERROR_PARSER(RegisterScriptData(SCRIPT_MISSIONRANKINGLVAREA,
        "data\\MissionRankingLVArea.txt", &g_RankingMgr));
#endif // _NA_008012_20150130_RANKING_SYSTEM
  #else
    ERROR_PARSER(RegisterScriptData(SCRIPT_MISSIONRANKINGLVAREA,
        "data\\MissionRankingLVArea.txt", &g_RankingHelper));
  #endif
    ERROR_PARSER(RegisterScriptData(SCRIPT_ACCESSORYITEMINFO,
        "data\\AccessoryItemInfo.txt", ItemInfoParser::Instance()));
    ERROR_PARSER(RegisterScriptData(SCRIPT_ARMORITEMINFO,
        "data\\ArmorItemInfo.txt", ItemInfoParser::Instance()));
    ERROR_PARSER(RegisterScriptData(SCRIPT_WASTEITEMINFO,
        "data\\WasteItemInfo.txt", ItemInfoParser::Instance()));
    ERROR_PARSER(RegisterScriptData(SCRIPT_WEAPONITEMINFO,
        "data\\WeaponItemInfo.txt", ItemInfoParser::Instance()));
    ERROR_PARSER(RegisterScriptData(SCRIPT_CHARGEITEMINFO,
        "data\\ChargeItemInfo.txt", ItemInfoParser::Instance()));
    //
    // (f100713.2L)
    ERROR_PARSER(RegisterScriptData(SCRIPT_ITEMTYPELIST,
        "data\\ItemTypeList.txt", ItemTypeList::Instance()));
    ERROR_PARSER(RegisterScriptData(SCRIPT_EXTRASTONEOPTIONINFO,
        "data\\ExtraStoneOptionInfo.txt", ExtraStoneOptionInfoParser::Instance()));
    ERROR_PARSER(RegisterScriptData(SCRIPT_NEW_CHARACTER_ITEM_INFO,
        "data\\NewCharacterItemInfo.txt", NewCharItemInfoParser::Instance()));
    ERROR_PARSER(RegisterScriptData(SCRIPT_INITCHARINFO,
        "data\\InitCharInfoList.txt", CharInfoParser::Instance()));
    // (f100603.3L)
    ERROR_PARSER(RegisterScriptData(SCRIPT_PET_INFO,
        "data\\PetManageInfo.txt", PetInfoParser::Instance()));
    //_KR_0_20091021_FIRENDINVITATION_EXTEND
    ERROR_PARSER(RegisterScriptData(SCRIPT_SUNEVENTINFO,
        "data\\SunEventInfo.txt", SunEventInfoParser::Instance()));
  #ifdef _NA_000251_20100727_SOCKET_SYSTEM
    // CHANGES: f110305.3L, added script loading process
    // to support socket functionality in NewCharacterItemProvier.
    ERROR_PARSER(RegisterScriptData(SCRIPT_SOCKET_ITEM_RATIO_INFO,
        "data\\SocketItemRatio.txt", SocketItemRatioParser::Instance()));
  #endif
    ERROR_PARSER(RegisterScriptData(SCRIPT_PERKINFO,
        "data\\PerkInfo.txt", PerkInfoParser::Instance()));
  #ifdef _USE_MAX_CHAR_LIST_PARSER
    ERROR_PARSER(RegisterScriptData(SCRIPT_MAX_CHAR_LIST,
        "data\\MaxCharList.txt", MaxCharListParser::Instance()));
  #endif
  #ifdef _NA002635_GZ_DZ_PROTOCOL_UINIFICATION_IN_DBP
    ERROR_PARSER(RegisterScriptData(SCRIPT_GUILD_RANK_INFO,
        "data\\GuildRankInfo.txt", GuildRankInfoParser::Instance()));
    ERROR_PARSER(RegisterScriptData(SCRIPT_GUILD_RIGHT_INFO,
        "data\\GuildRightInfo.txt", GuildRightInfoParser::Instance()));
    ERROR_PARSER(RegisterScriptData(SCRIPT_MAX_GUILD_MEMBER_INFO,
        "data\\GuildMemberInfo.txt", MaxGuildMemberInfoParser::Instance()));
    ERROR_PARSER(RegisterScriptData(SCRIPT_DOMINATIONINFO,
        "data\\DominationInfo.txt", DominationInfoParser::Instance()));
#ifdef _NA_008334_20150608_SONNENSCHEIN
    ERROR_PARSER(RegisterScriptData(SCRIPT_DOMINATION_EVENT_INFO, 
        "data\\DominationTimeInfo.txt", DominationInfoParser::Instance()));
#endif //_NA_008334_20150608_SONNENSCHEIN
  #endif

  #ifdef _NA_003923_20120130_GUILD_RENEWAL
    ERROR_PARSER(RegisterScriptData(SCRIPT_GUILDINFO,
        "data\\GuildInfo.txt", GuildInfoParser::Instance()));
  #endif

#ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION
    ERROR_PARSER(RegisterScriptData( SCRIPT_WORLD_SUB_INFO, 
        "data\\Worldsubinfo.txt", MapInfoParser::Instance()));
    ERROR_PARSER(RegisterScriptData(SCRIPT_WORLD,
        "data\\World.txt", MapInfoParser::Instance()));
#endif //_NA_007128_20140326_MONDSCHIEN_DOMINATION
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    ERROR_PARSER(RegisterScriptData(SCRIPT_GUILD_FACILITY_INFO,
        "data\\GuildFacilityInfo.txt", GuildFacilityInfoParser::Instance()));
#endif //_NA_007330_20140620_GUILD_SYSTEM_EXTENSION
  #ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
    //----------------------------------------------------------------------------------------------
    ;{
        sSCRIPT_DATA_INFO_NODE pNodeTable[SCRIPT_LIST_SIZE];
        if (!GetScriptDataNodeList(pNodeTable, SCRIPT_LIST_SIZE)) {
            return false;
        };
        ScriptDataManager::RegisterDataList(pNodeTable);
        ScriptDataManager::RegisterCallbackAllDataLoaded(DBProxyServerEx::StartServer);
    };
    //----------------------------------------------------------------------------------------------
  #endif //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__

//==================================================================================================
#elif DBPROXY_TYPE_SELECTOR == DBPROXY_TYPE_UNIT
//==================================================================================================

    ERROR_PARSER(RegisterScriptData(SCRIPT_ACCESSORYITEMINFO,
        "data\\AccessoryItemInfo.txt", ItemInfoParser::Instance()));
    ERROR_PARSER(RegisterScriptData(SCRIPT_ARMORITEMINFO,
        "data\\ArmorItemInfo.txt", ItemInfoParser::Instance()));
    ERROR_PARSER(RegisterScriptData(SCRIPT_WASTEITEMINFO,
        "data\\WasteItemInfo.txt", ItemInfoParser::Instance()));
    ERROR_PARSER(RegisterScriptData(SCRIPT_WEAPONITEMINFO,
        "data\\WeaponItemInfo.txt", ItemInfoParser::Instance()));
    ERROR_PARSER(RegisterScriptData(SCRIPT_CHARGEITEMINFO,
        "data\\ChargeItemInfo.txt", ItemInfoParser::Instance()));
    //
    // (f100713.2L)
    ERROR_PARSER(RegisterScriptData(SCRIPT_ITEMTYPELIST,
        "data\\ItemTypeList.txt", ItemTypeList::Instance()));
    ERROR_PARSER(RegisterScriptData(SCRIPT_EXTRASTONEOPTIONINFO,
        "data\\ExtraStoneOptionInfo.txt", ExtraStoneOptionInfoParser::Instance()));
    ERROR_PARSER(RegisterScriptData(SCRIPT_NEW_CHARACTER_ITEM_INFO,
        "data\\NewCharacterItemInfo.txt", NewCharItemInfoParser::Instance()));
    ERROR_PARSER(RegisterScriptData(SCRIPT_INITCHARINFO,
        "data\\InitCharInfoList.txt", CharInfoParser::Instance()));
    // (f100603.3L)
    ERROR_PARSER(RegisterScriptData(SCRIPT_PET_INFO,
        "data\\PetManageInfo.txt", PetInfoParser::Instance()));
    //_KR_0_20091021_FIRENDINVITATION_EXTEND
    ERROR_PARSER(RegisterScriptData(SCRIPT_SUNEVENTINFO,
        "data\\SunEventInfo.txt", SunEventInfoParser::Instance()));
    ERROR_PARSER(RegisterScriptData(SCRIPT_QUESTINFO,
        "data\\QuestInfo.txt", &batch_unit::quest_script_parser_global));
  #ifdef C_NA_0_20100520_ACSYSTEM_CHANGED
    ERROR_PARSER(RegisterScriptData(SCRIPT_ACHIEVEMENT_INFO,
        "data\\ACScript.txt", AchievementParser::Instance()));
  #else
    ERROR_PARSER(RegisterScriptData(SCRIPT_ACSCRIPT, "data\\ACScript.txt",
        AreaConquestInfoParser::Instance()));
  #endif
    ERROR_PARSER(RegisterScriptData(SCRIPT_WORLD_SUB_INFO,
        "data\\Worldsubinfo.txt", MapInfoParser::Instance()));
    ERROR_PARSER(RegisterScriptData(SCRIPT_WORLD,
        "data\\World.txt", MapInfoParser::Instance()));
    ERROR_PARSER(RegisterScriptData(SCRIPT_FIELD,
        "data\\Field.txt", MapInfoParser::Instance(), SEPERATOR_WHITESPACE, NULL, true));
    ERROR_PARSER(RegisterScriptData(SCRIPT_FIELDWMO,
        "data\\Fieldwmo.txt", MapInfoParser::Instance(), SEPERATOR_WHITESPACE, NULL, true));
    ERROR_PARSER(RegisterScriptData(SCRIPT_FIELDWUT,
        "data\\Fieldwut.txt", MapInfoParser::Instance(), SEPERATOR_WHITESPACE, NULL, true));

    if (BatchunitFactory::GetUnitType() == BatchunitFactory::AC_CONVERT)
    {
    #ifdef C_NA_0_20100520_ACSYSTEM_CHANGED
        ERROR_PARSER(RegisterScriptData(SCRIPT_ACSCRIPT,
            "data\\oldACScript.txt", AreaConquestInfoParser::Instance()));
        ERROR_PARSER(RegisterScriptData(SCRIPT_NPCINFO,
            "data\\NPCInfo.txt",  NPCInfoParser::Instance()));
    #endif
    }

  #ifdef _USE_MAX_CHAR_LIST_PARSER
    ERROR_PARSER(RegisterScriptData(SCRIPT_MAX_CHAR_LIST,
        "data\\MaxCharList.txt", MaxCharListParser::Instance()));
  #endif

  #ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
    #error "The batch unit is only processing on local environments"
  #endif //

//==================================================================================================
#endif //DBPROXY_TYPE_SELECTOR != DBPROXY_TYPE_ACCOUNT
//==================================================================================================

    return true;
}

void DBProxyScriptParserManager::DestroyInstanceEx()
{
#if DBPROXY_TYPE_SELECTOR != DBPROXY_TYPE_ACCOUNT
    ItemInfoParser::DestroyInstance();
    ExtraStoneOptionInfoParser::DestroyInstance();
    // (f100603.3L)
    PetInfoParser::DestroyInstance();
  #if DBPROXY_TYPE_SELECTOR == DBPROXY_TYPE_UNIT
    AreaConquestInfoParser::DestroyInstance();
  #endif
    SunEventInfoParser::DestroyInstance(); //_KR_0_20091021_FIRENDINVITATION_EXTEND
    NewCharItemInfoParser::DestroyInstance();
    // (f100713.2L)
    ItemTypeList::Instance()->Release();
    ItemTypeList::DestroyInstance();
    PerkInfoParser::DestroyInstance();
  #ifdef _NA_000251_20100727_SOCKET_SYSTEM
    SocketItemRatioParser::DestroyInstance();
  #endif
  #ifdef _USE_MAX_CHAR_LIST_PARSER
    MaxCharListParser::DestroyInstance();
  #endif
  #ifdef _NA002635_GZ_DZ_PROTOCOL_UINIFICATION_IN_DBP
    GuildRankInfoParser::DestroyInstance();
    GuildRightInfoParser::DestroyInstance();
    MaxGuildMemberInfoParser::DestroyInstance();
    DominationInfoParser::DestroyInstance();
  #endif

#ifdef _NA_003923_20120130_GUILD_RENEWAL
    GuildInfoParser::DestroyInstance();
#endif
#ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION
    MapInfoParser::DestroyInstance();
#endif //_NA_007128_20140326_MONDSCHIEN_DOMINATION
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    GuildFacilityInfoParser::DestroyInstance();
#endif //_NA_007330_20140620_GUILD_SYSTEM_EXTENSION
#endif
}
