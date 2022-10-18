#pragma once

#include <MapInfoParser.h>
#include <ServerOptionParser.h>
#include <ServerOptionParserEx.h>

//////////////////////////////////////////////////////////////////////////
#include <ServerSessions/ServerSessionManager.h>
#include <ServerSessions/ServerSessionEx.h>

#include <UserSessions/User.h>

#include <AgentParty/AgentPartyManager.h>

#include "ZoneManager.h"
#include "Village.h"
#include "CHuntingRoom.h"
#include "HuntingField.h"
#include "HuntingRoom.h"
#include "MissionRoom.h"
#include "LobbyRoom.h"
#include "PVPRoom.h"
#include "CharSelect.h"
#include "InstanceDungeon.h"
#include "DungeonTopOfTrial.h"
#include "DungeonSSQ.h"
#include "DominationField.h"
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#include "DungeonChaosZone.h"
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
#include "SpaField.h"
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//
template< eZONETYPE _ZTYPE, typename _TYPE, DWORD _InitPool, DWORD _AddinPool >
struct zone_exinfo : zone_info< _ZTYPE, _TYPE >
{
    static const DWORD size_init_pool   = _InitPool;
    static const DWORD size_addin_pool  = _AddinPool;
};

template< eINSTANCE_DUNGEON_KIND _ZTYPE, typename _TYPE, DWORD _InitPool, DWORD _AddinPool >
struct indun_exinfo : public zone_exinfo< eZONETYPE_MAX, _TYPE, _InitPool, _AddinPool >
{
    static const eINSTANCE_DUNGEON_KIND value = _ZTYPE;
};

//////////////////////////////////////////////////////////////////////////
//    Hard-coding for intelli-sense
typedef zone_exinfo<eZONETYPE_LOBBY,       LobbyRoom,    50, 50>    LobbyRoom_Info;
typedef zone_exinfo<eZONETYPE_VILLAGE,     Village,       5,  1>    Village_Info;
typedef zone_exinfo<eZONETYPE_CHARSELECT,  CharSelect,    1,  0>    CharSelect_Info;
typedef zone_exinfo<eZONETYPE_MISSION,     MissionRoom,  50, 50>    MissionRoom_Info;
typedef zone_exinfo<eZONETYPE_HUNTING,     HuntingRoom,   1,  1>    HuntingRoom_Info;
typedef zone_exinfo<eZONETYPE_PVP,         PVPRoom,       5,  5>    PVPRoom_Info;
typedef zone_exinfo<eZONETYPE_FIELD,       HuntingField, 30, 10>    HuntingField_Info;
typedef zone_exinfo<eZONETYPE_DOMINATION_FIELD, DominationField, 30, 10>    DominationField_Info;
typedef zone_exinfo<eZONETYPE_CHUNTING,    CHuntingRoom, 50, 50>    CHuntingRoom_Info;
#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
typedef zone_exinfo<eZONETYPE_SPA_FIELD,   SpaField,      1,  1>    SpaField_Info;
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM
//    temporary type register
typedef zone_exinfo<eZONETYPE_INSTANCE,    InstanceDungeon,  0,  0> InstanceDungeon_Info;

typedef indun_exinfo<INDUN_KIND_TOP_OF_TRIAL, DungeonTopOfTrial,  10,  10> DungeonTopOfTrial_Info;
typedef indun_exinfo<INDUN_KIND_SS_QUEST,     DungeonSSQ,          5,   5> DungeonSSQ_Info;
typedef indun_exinfo<INDUN_KIND_100LEVEL_QUEST, Milestone100LevelQuest,  5,  5> Milestone100LevelQuest_Info;
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
typedef indun_exinfo<INDUN_KIND_CHAOS_ZONE, DungeonChaosZone,  10,  10> DungeonChaosZone_Info;
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND

//#define REG_ZONE_NODE(_typeinfo)
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//    Instance Dungeon擎 罹晦縑 蹺陛ж雖 蜓 匙.

#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
#define REG_ZONE_LIST() \
    REG_ZONE_NODE(LobbyRoom_Info) \
    REG_ZONE_NODE(Village_Info  ) \
    REG_ZONE_NODE(CharSelect_Info) \
    REG_ZONE_NODE(MissionRoom_Info) \
    REG_ZONE_NODE(HuntingRoom_Info) \
    REG_ZONE_NODE(PVPRoom_Info  ) \
    REG_ZONE_NODE(HuntingField_Info) \
    REG_ZONE_NODE(DominationField_Info) \
    REG_ZONE_NODE(CHuntingRoom_Info) \
    REG_ZONE_NODE(SpaField_Info)
#else
#define REG_ZONE_LIST() \
    REG_ZONE_NODE(LobbyRoom_Info) \
    REG_ZONE_NODE(Village_Info  ) \
    REG_ZONE_NODE(CharSelect_Info) \
    REG_ZONE_NODE(MissionRoom_Info) \
    REG_ZONE_NODE(HuntingRoom_Info) \
    REG_ZONE_NODE(PVPRoom_Info  ) \
    REG_ZONE_NODE(HuntingField_Info) \
    REG_ZONE_NODE(DominationField_Info) \
    REG_ZONE_NODE(CHuntingRoom_Info)
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM

//    Instance Dungeon擎 罹晦縑 蹺陛й 匙
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#define REG_INDUN_LIST()    \
    REG_ZONE_NODE(DungeonTopOfTrial_Info) \
    REG_ZONE_NODE(DungeonSSQ_Info)  \
    REG_ZONE_NODE(Milestone100LevelQuest_Info) \
    REG_ZONE_NODE(DungeonChaosZone_Info)
#else
#define REG_INDUN_LIST()    \
    REG_ZONE_NODE(DungeonTopOfTrial_Info) \
    REG_ZONE_NODE(DungeonSSQ_Info)  \
    REG_ZONE_NODE(Milestone100LevelQuest_Info)
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//  <MoveZone Policyment>
//  -  ZoneCommon.h縑 襄營ж朝 MOVE_ZONE_CONTROL_LIST()縑 蛔營脹 跡煙擊 掘⑷и棻.
//  -  檜翕 褻勒 匐餌 в渦菟...

const RoomArgument&
RoomArgument::GetEmpty()
{
    static RoomArgument emptyArgument;
    return emptyArgument;
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//
CheckMoveZonePolicyArgs::CheckMoveZonePolicyArgs()
    : pUser(0)
    , FromZoneType(eZONETYPE_MAX)
    , ToZoneType(eZONETYPE_MAX)
    , FromZoneKey(0)
    , ToZoneKey(0)
    , FromZonePtr(0)
    , ToZonePtr(0)
    , FromZoneTypeByObj(eZONETYPE_MAX)
    , ToZoneTypeByObj(eZONETYPE_MAX)
{
}

CheckMoveZonePolicyArgs::CheckMoveZonePolicyArgs(
    User* const _pUser,
    const eZONETYPE _FromZoneType,  const eZONETYPE  _ToZoneType,
    const KEYTYPE   _FromZoneKey,   const KEYTYPE    _ToZoneKey,
    ZoneInterface* const _FromZonePtr,  // FromKey縑 渠и Zone
    ZoneInterface* const _ToZonePtr,    // ToKey縑 渠и Zone
    const eZONETYPE _FromZoneTypeByObj, // FromZone曖 褒薯 顫殮
    const eZONETYPE _ToZoneTypeByObj)    // ToZone曖 褒薯 顫殮
    : pUser(_pUser)
    , FromZoneType(_FromZoneType)
    , ToZoneType(_ToZoneType)
    , FromZoneKey(_FromZoneKey)
    , ToZoneKey(_ToZoneKey)
    , FromZonePtr(_FromZonePtr)
    , ToZonePtr(_ToZonePtr)
    , FromZoneTypeByObj(_FromZoneTypeByObj)
    , ToZoneTypeByObj(_ToZoneTypeByObj)
{
}

//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式
template<>
class CheckMoveZonePolicy_Impl<MZTF_CHECK_SKIP> : public CheckMoveZonePolicy
{
private:
    static const eMOVE_ZONE_TYPE_FILTER  movetype = MZTF_CHECK_SKIP;
public:
    virtual RC::eROOM_RESULT EnableCheck()
    {
        // か滌и 薯擒檜 橈朝 唳辦 酈煎 瓊擎 ん檣攪虜 匐隸ж堅 籀葬蒂 熱чи棻.
        return ValidCheck() ? RC::RC_ROOM_SUCCESS : RC::RC_ROOM_NOT_EXIST_ROOM;
    }
};
//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式
template<>
class CheckMoveZonePolicy_Impl<MZTF_STATIC_TO_STATIC> : public CheckMoveZonePolicy
{
private:
    static const eMOVE_ZONE_TYPE_FILTER movetype = MZTF_STATIC_TO_STATIC;
public:
    virtual RC::eROOM_RESULT EnableCheck()
    {
        //    (NOTE)    -    檜翕⑽鷓 : FIELD<->FIELD, FIELD<->VILLAGE (ぅ饜葬橡 んл)
        //    MapEnterencePortal婁 偽擎 匐隸檜 氈橫撿雖虜 瓊嬴雖朝 僥薯陛 氈捱蹂.
        //    儅陝爾棻 橾奩�� 橫滑喜... 擠...
        // _NA_008069_20150210_ADD_SPA_SYSTEM
        const DWORD GO_ACCEPT_MASK = 
            (1<<eZONETYPE_VILLAGE) | (1<<eZONETYPE_FIELD) | (1<<eZONETYPE_DOMINATION_FIELD) | (1<<eZONETYPE_SPA_FIELD);

        if(!ValidCheck())
            return RC::RC_ROOM_NOT_EXIST_ROOM;

        BOOL bMoveAccept = !!(GO_ACCEPT_MASK & (1 << Args->FromZoneType));
             bMoveAccept = bMoveAccept && (GO_ACCEPT_MASK & (1 << Args->ToZoneType));

        return bMoveAccept ? RC::RC_ROOM_SUCCESS : RC::RC_ROOM_NOT_EXIST_ROOM;
    }
};
//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式
template<>
class CheckMoveZonePolicy_Impl<MZTF_ROOM_TO_STATIC> : public CheckMoveZonePolicy
{
private:
    static const eMOVE_ZONE_TYPE_FILTER  movetype = MZTF_ROOM_TO_STATIC;
public:
    virtual RC::eROOM_RESULT EnableCheck()
    {
        //    (NOTE)    -    陝謙 ROOM->VILLAGE+FIELD+TUTORIAL
        if(!ValidCheck())
            return RC::RC_ROOM_NOT_EXIST_ROOM;

        //    お葬剪縑 曖п 犒敝ж朝 唳辦 RoomType擎 Mission|Instance陛 п渡脹棻.
        if(Args->ToZoneType != Args->ToZonePtr->GetType())
            return RC::RC_ROOM_NOT_EXIST_ROOM;

        const eZONETYPE ptrZoneType = Args->FromZonePtr->GetType();
        if(Args->FromZoneType != ptrZoneType &&    // お葬剪縑 曖и 唳辦 蕨諼 籀葬
           !(ptrZoneType == eZONETYPE_INSTANCE && Args->FromZoneType == eZONETYPE_MISSION))
            return RC::RC_ROOM_NOT_EXIST_ROOM;

        switch(Args->FromZoneType)
        {
        case eZONETYPE_HUNTING:
        case eZONETYPE_CHUNTING:
        case eZONETYPE_MISSION:
        case eZONETYPE_INSTANCE:
        case eZONETYPE_PVP:
            break;
        default:
            return RC::RC_ROOM_NOT_EXIST_ROOM;
        }

        //  (WAVERIX) (081021) (BUG-FIX) '顫塊и 餌錳'縑憮 'HUNTING_FIELD'煎 犒敝 寰腎朝 僥薯 熱薑
        //  - 匐餌褻勒 滲唳: FromZoneType --> FromZoneTypeByObj 
        switch(Args->ToZoneTypeByObj)
        {
        case eZONETYPE_DOMINATION_FIELD:
        case eZONETYPE_FIELD:
            if(!(eZONETYPE_INSTANCE == Args->FromZoneTypeByObj ||
                 eZONETYPE_MISSION == Args->FromZoneTypeByObj))
                return RC::RC_ROOM_NOT_EXIST_ROOM;
            break;
        case eZONETYPE_TUTORIAL_FIELD:
			// 盪鱉縑憮 縑模萄葆擊煎 犒敝й 熱 橈朝 ⑷鼻 熱薑(橾獄擎 縑模萄葆擊縑憮 寡ぎ襄璽 翮晦 陛棟)
            //if(eZONETYPE_MISSION != Args->FromZoneTypeByObj)
			//  return RC::RC_ROOM_NOT_EXIST_ROOM;
            break;
        case eZONETYPE_VILLAGE:
            break;
#ifdef _NA_006174_20121015_MOVE_ZONE_POLICY_AIRSHIP_FIELD_ADD
        case eZONETYPE_AIRSHIP_FIELD:
            break;
#endif // _NA_006174_20121015_MOVE_ZONE_POLICY_AIRSHIP_FIELD_ADD
        //_NA_008069_20150210_ADD_SPA_SYSTEM
        case eZONETYPE_SPA_FIELD:
            return RC::RC_ROOM_NOT_EXIST_ROOM;
        default:
            return RC::RC_ROOM_NOT_EXIST_ROOM;
        }

        return RC::RC_ROOM_SUCCESS;
    }
};
//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式
template<>
class CheckMoveZonePolicy_Impl<MZTF_VILLAGE_TO_LOBBY> : public CheckMoveZonePolicy
{
private:
    static const eMOVE_ZONE_TYPE_FILTER  movetype = MZTF_VILLAGE_TO_LOBBY;
public:
    virtual RC::eROOM_RESULT EnableCheck()
    {
        //    (NOTE)    -    field縑憮 lobby煎 檜翕紫 陛棟 (TUTORIAL_FIELD)
        //    -    橾欽 賅萇 STATIC(VILLAGE+FIELD)縑憮 檜翕 陛棟ж啪 腎歷棻.
        if(!ValidCheck())
            return RC::RC_ROOM_NOT_EXIST_ROOM;

        if(Args->ToZoneType == Args->ToZonePtr->GetType())
        {    // ToZoneTypeByObj ... 檜匙擎 ゎ陛 擁衙...
            if(Args->FromZoneType == Args->FromZoneTypeByObj)
                return RC::RC_ROOM_SUCCESS;

            if(Args->FromZoneType == eZONETYPE_FIELD)
                return RC::RC_ROOM_SUCCESS;

            if(Args->FromZoneTypeByObj == eZONETYPE_TUTORIAL_FIELD)
                return RC::RC_ROOM_SUCCESS;
            else if(Args->FromZoneTypeByObj == eZONETYPE_DOMINATION_FIELD)
                return RC::RC_ROOM_SUCCESS;
            else if(Args->FromZoneTypeByObj == eZONETYPE_SPA_FIELD) 
            {
                //_NA_008069_20150210_ADD_SPA_SYSTEM
                // 螞繭縑憮朝 煎綠煎 檜翕й 熱 橈棻
                return RC::RC_ROOM_NOT_EXIST_ROOM;
            }
        }

        return RC::RC_ROOM_NOT_EXIST_ROOM;
    }
};
//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式
template<>
class CheckMoveZonePolicy_Impl<MZTF_LOBBY_TO_VILLAGE> : public CheckMoveZonePolicy
{
private:
    static const eMOVE_ZONE_TYPE_FILTER  movetype = MZTF_LOBBY_TO_VILLAGE;
public:
    virtual RC::eROOM_RESULT EnableCheck()
    {
        //    (NOTE)    -    LOBBY縑憮 FIELD煎 檜翕紫 陛棟 (TUTORIAL_FIELD)
        //    -    (Exception)
        //    -        LOBBY曖 GetMapCode朝 褒薯 陛溥朝 ROOM曖 MapCode蒂 爾嶸ж堅 氈棻.
        //    -        嬴葆 罹晦憮 僥薯陛 嫦儅ж朝 匙橾 攪...
        if(!ValidCheck())
            return RC::RC_ROOM_NOT_EXIST_ROOM;

        if(Args->FromZoneType == Args->FromZonePtr->GetType())
        {    // FromZoneTypeByObj ... 檜匙擎 ゎ陛 擁衙...
            if(Args->ToZoneType == Args->ToZoneTypeByObj)
                return RC::RC_ROOM_SUCCESS;

            if(Args->ToZoneType == eZONETYPE_FIELD)
                return RC::RC_ROOM_SUCCESS;
            //if(ToZoneTypeByObj == eZONETYPE_TUTORIAL_FIELD)
            //    return RC::RC_ROOM_SUCCESS;
            if(Args->ToZoneType == eZONETYPE_FIELD)
                return RC::RC_ROOM_SUCCESS;
        }

        return RC::RC_ROOM_NOT_EXIST_ROOM;
    }
};
//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式
template<>
class CheckMoveZonePolicy_Impl<MZTF_VILLAGE_TO_HUNTING> : public CheckMoveZonePolicy
{
private:
    static const eMOVE_ZONE_TYPE_FILTER  movetype = MZTF_VILLAGE_TO_HUNTING;
public:
    virtual RC::eROOM_RESULT EnableCheck()
    {
        //    (NOTE)    -    葆擊縑憮 夥煎 HUNTING縑 霤陛
        if(!ValidCheck())
            return RC::RC_ROOM_NOT_EXIST_ROOM;

        if(eZONETYPE_VILLAGE == Args->FromZoneType && eZONETYPE_HUNTING == Args->ToZoneType)
            return RC::RC_ROOM_SUCCESS;

        return RC::RC_ROOM_NOT_EXIST_ROOM;
    }
};
//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式
template<>
class CheckMoveZonePolicy_Impl<MZTF_ROOM_TO_LOBBY> : public CheckMoveZonePolicy
{
private:
    static const eMOVE_ZONE_TYPE_FILTER  movetype = MZTF_ROOM_TO_LOBBY;
public:
    virtual RC::eROOM_RESULT EnableCheck()
    {
        //    (NOTE)    -    RELAY MISSION
        if(!ValidCheck())
            return RC::RC_ROOM_NOT_EXIST_ROOM;

        if(eZONETYPE_MISSION == Args->FromZonePtr->GetType() && eZONETYPE_LOBBY == Args->ToZonePtr->GetType())
        {
            return RC::RC_ROOM_SUCCESS;
        }

        return RC::RC_ROOM_NOT_EXIST_ROOM;
    }
};
//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式
template<>
class CheckMoveZonePolicy_Impl<MZTF_CHUNTING_TO_VILLAGE> : public CheckMoveZonePolicy
{
private:
    static const eMOVE_ZONE_TYPE_FILTER  movetype = MZTF_CHUNTING_TO_VILLAGE;
public:
    virtual RC::eROOM_RESULT EnableCheck()
    {
        //    (NOTE)    -    CHUNTING->VILLAGE
        if(!ValidCheck())
            return RC::RC_ROOM_NOT_EXIST_ROOM;

        if(eZONETYPE_CHUNTING == Args->FromZonePtr->GetType() && eZONETYPE_VILLAGE == Args->ToZonePtr->GetType())
        {
            return RC::RC_ROOM_SUCCESS;
        }

        return RC::RC_ROOM_NOT_EXIST_ROOM;
    }
};
//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式
template<>
class CheckMoveZonePolicy_Impl<MZTF_HUNTING_TO_HUNTING> : public CheckMoveZonePolicy
{
private:
    static const eMOVE_ZONE_TYPE_FILTER  movetype = MZTF_HUNTING_TO_HUNTING;
public:
    virtual RC::eROOM_RESULT EnableCheck()
    {
        //    (NOTE)    -    CHUNTING->VILLAGE
        if(!ValidCheck())
            return RC::RC_ROOM_NOT_EXIST_ROOM;

        if(eZONETYPE_HUNTING == Args->FromZonePtr->GetType() && eZONETYPE_HUNTING == Args->ToZonePtr->GetType())
        {
            return RC::RC_ROOM_SUCCESS;
        }

        return RC::RC_ROOM_NOT_EXIST_ROOM;
    }
};
//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式
template<>
class CheckMoveZonePolicy_Impl<MZTF_STATIC_TO_ROOM> : public CheckMoveZonePolicy
{
private:
    static const eMOVE_ZONE_TYPE_FILTER  movetype = MZTF_STATIC_TO_ROOM;
public:
    virtual RC::eROOM_RESULT EnableCheck()
    {
        //  (090215) (NOTE) ん驍 儅撩�� 殮濰ж朝 嶸盪辨
        if(!ValidCheck())
            return RC::RC_ROOM_NOT_EXIST_ROOM;

        switch(Args->FromZoneType)
        {
        case eZONETYPE_VILLAGE:
        case eZONETYPE_DOMINATION_FIELD:
        case eZONETYPE_FIELD:
            break;
        //_NA_008069_20150210_ADD_SPA_SYSTEM
        case eZONETYPE_SPA_FIELD:
            return RC::RC_ROOM_NOT_EXIST_ROOM;
        default:
            return RC::RC_ROOM_NOT_EXIST_ROOM;
        }

        switch(Args->ToZoneType)
        {
        case eZONETYPE_MISSION:
        case eZONETYPE_INSTANCE:
            break;
        default:
            return RC::RC_ROOM_INVALID_ROOM_TYPE;
        }

        const sMAPINFO* pMapInfo = Args->ToZonePtr->GetMapInfo();
        //const sMAPINFO* pMapInfo =
        //    MapInfoParser::Instance()->FindMapInfo(Args->ToZonePtr->GetMapCode());
        assert(pMapInfo);

        if(Args->ToZoneType == eZONETYPE_MISSION)
            CheckMission(pMapInfo);
        if(Args->ToZoneType == eZONETYPE_INSTANCE)
            CheckInstanceDungeon(pMapInfo);

        return RC::RC_ROOM_SUCCESS;
    }

    RC::eROOM_RESULT CheckMission(const sMAPINFO* pMapInfo)
    {
        __UNUSED(pMapInfo);
        // Character曖 晦獄 薯擒 褻勒(摹ч 嘐暮, 摹ч 蠡蝶お 蛔蛔)擎 GameServer縑憮 匐餌脹 鼻鷓
        RC::eROOM_RESULT result = RC::RC_ROOM_SUCCESS;
        result = Args->ToZonePtr->CanJoin(Args->pUser, eZONEPUBLIC_PUBLIC, "");
        return result;
    }

    RC::eROOM_RESULT CheckInstanceDungeon(const sMAPINFO* pMapInfo)
    {
        __UNUSED(pMapInfo);
        RC::eROOM_RESULT result = RC::RC_ROOM_SUCCESS;
        result = Args->ToZonePtr->CanJoin(Args->pUser, eZONEPUBLIC_PUBLIC, "");
        return result;
    }
};
//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式
template<>
class CheckMoveZonePolicy_Impl<MZTF_STATIC_TO_ROOM_FOR_UCUSER> : public CheckMoveZonePolicy
{
private:
    static const eMOVE_ZONE_TYPE_FILTER  movetype = MZTF_STATIC_TO_ROOM_FOR_UCUSER;
public:
    virtual RC::eROOM_RESULT EnableCheck()
    {
        //    (NOTE)    -    綠薑鼻 謙猿�� 犒敝 蹂羶衛 籀葬腎朝 瑞ず
        if(!ValidCheck())
            return RC::RC_ROOM_NOT_EXIST_ROOM;

        switch(Args->FromZoneType)
        {
        case eZONETYPE_VILLAGE:
        case eZONETYPE_DOMINATION_FIELD:
        case eZONETYPE_FIELD:
            break;
        //_NA_008069_20150210_ADD_SPA_SYSTEM
        case eZONETYPE_SPA_FIELD:
            return RC::RC_ROOM_NOT_EXIST_ROOM;
        default:
            return RC::RC_ROOM_NOT_EXIST_ROOM;
        }

        switch(Args->ToZoneType)
        {
        case eZONETYPE_MISSION:
        case eZONETYPE_INSTANCE:
            break;
        default:
            return RC::RC_ROOM_NOT_EXIST_ROOM;
        }

        return RC::RC_ROOM_SUCCESS;
    }
};


//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式
//    <MoveZone Policyment Code-Generations>
#define MOVE_ZONE_CTRL_NODE_NAME(mztf)    sCheckMoveZonePolicy_##mztf
//#define MOVE_ZONE_CTRL_NODE(eMOVE_ZONE_TYPE_FILTER)

#undef MOVE_ZONE_CTRL_NODE
#define MOVE_ZONE_CTRL_NODE(mztf)    \
    static CheckMoveZonePolicy_Impl<mztf>        MOVE_ZONE_CTRL_NODE_NAME(mztf);

MOVE_ZONE_CONTROL_LIST();    // declare module variables


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收


VOID
CheckMoveZonePolicy::_Clear()
{
    //m_pImpl = NULL;
    //USER.Set(NULL);
    //FromZoneType.Set(eZONETYPE_MAX);
    //ToZoneType.Set(eZONETYPE_MAX);
    //FromZoneKey.Set(0);
    //ToZoneKey.Set(0);
    //FromZonePtr.Set(NULL);
    //ToZonePtr.Set(NULL);
    //FromZoneTypeByObj.Set(eZONETYPE_MAX);
    //ToZoneTypeByObj.Set(eZONETYPE_MAX);


}


CheckMoveZonePolicy*
CheckMoveZonePolicy::_AlignInfo(const eMOVE_ZONE_TYPE_FILTER moveType,
                                User* const     pUser,
                                const eZONETYPE fromZoneType,
                                const eZONETYPE toZoneType,
                                const KEYTYPE   fromZoneKey,
                                const KEYTYPE   toZoneKey)
{
    if(!pUser)
        return NULL;

    switch(moveType)
    {
#undef MOVE_ZONE_CTRL_NODE
#define MOVE_ZONE_CTRL_NODE(mztf)    \
    case (mztf): break;

    MOVE_ZONE_CONTROL_LIST();    // case list

    default:
        SUNLOG(eCRITICAL_LOG, _T("[%s] Unhandled Check Type"), __FUNCTION__);
        return NULL;
    };

    CheckMoveZonePolicy* pInstance = NULL;
    ZoneInterface* const pFromZone = g_ZoneManager.FindZone(fromZoneKey);
    ZoneInterface* const pToZone = g_ZoneManager.FindZone(toZoneKey);

    // (WAVERIX) (090601) (BUG-FIX) GameServer曖 擬港擊 嫡嬴 檜翕籀葬蒂 ж朝 唳辦, (case Lobby)
    // pFromZone, pToZone擎 null橾 熱 氈棻.
    const sMAPINFO* pFromMapInfo = pFromZone ? pFromZone->GetMapInfo() : 0;
    eZONETYPE fromZoneTypeByObj = pFromMapInfo ? (eZONETYPE)pFromMapInfo->byMKind : eZONETYPE_MAX;

    const sMAPINFO* pToMapInfo = pToZone ? pToZone->GetMapInfo() : 0;
    eZONETYPE toZoneTypeByObj = pToMapInfo ? (eZONETYPE)pToMapInfo->byMKind : eZONETYPE_MAX;

    switch(moveType)
    {
#undef MOVE_ZONE_CTRL_NODE
#define MOVE_ZONE_CTRL_NODE(mztf)        \
    case (mztf):                        \
        pInstance = &MOVE_ZONE_CTRL_NODE_NAME(mztf);            \
        new (pInstance->Args) CheckMoveZonePolicyArgs            \
            (pUser                                                \
            , fromZoneType, toZoneType, fromZoneKey, toZoneKey    \
            , pFromZone, pToZone                                \
            , fromZoneTypeByObj, toZoneTypeByObj);                \
        break;

    MOVE_ZONE_CONTROL_LIST();    // case list
    };

    pInstance->m_pImpl = pInstance;
    return pInstance;
}

//////////////////////////////////////////////////////////////////////////
//    <ZoneKeyStreamAddin>
ZoneKeyStreamAddin::ZoneKeyStreamAddin(ZoneInterface* const pInterface)
{
    ZoneType = pInterface->GetType();
    switch(ZoneType)
    {
    case eZONETYPE_LOBBY      :
    case eZONETYPE_VILLAGE    :
    case eZONETYPE_CHARSELECT :
    case eZONETYPE_MISSION    :
    case eZONETYPE_HUNTING    :
    //case eZONETYPE_QUEST    :
    case eZONETYPE_PVP        :
    //case eZONETYPE_EVENT    :
    //case eZONETYPE_GUILD    :
    //case eZONETYPE_SIEGE    :
    case eZONETYPE_FIELD      :
    case eZONETYPE_DOMINATION_FIELD:
    case eZONETYPE_CHUNTING   :
    case eZONETYPE_TUTORIAL_FIELD:
    case eZONETYPE_INSTANCE   :
    //_NA_008069_20150210_ADD_SPA_SYSTEM
    case eZONETYPE_SPA_FIELD:
        break;
    default:
        SUNLOG(eCRITICAL_LOG, "Invalid Zone Type [%u]", ZoneType);
        ZoneType = eZONETYPE_MAX;
        break;
    }

    ZoneSubType = INDUN_KIND_MAX;
    if(eZONETYPE_INSTANCE == ZoneType)
    {
        BOOST_STATIC_ASSERT(INDUN_KIND_MAX == 7);    // 蹺陛 顫殮 襄營衛 ж欽 頂辨 蹺陛й 匙
        InstanceDungeon* const pIndun = InstanceDungeon_Info::downcast(pInterface);
        ZoneSubType = (WORD)pIndun->GetDungeonType();
        switch(pIndun->GetDungeonType())
        {
        case INDUN_KIND_TOP_OF_TRIAL:
        case INDUN_KIND_100LEVEL_QUEST:        // 100溯漣 羹檣雖機, 檜�� 橾奩瞳檣 蠡蝶お 裘戲煎 �倏� 陛棟
        case INDUN_KIND_SS_QUEST:            // 顫塊и 餌錳
        case INDUN_KIND_EXT_03:
        case INDUN_KIND_EXT_04:
        case INDUN_KIND_MAYA_SCROLL:
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
        case INDUN_KIND_CHAOS_ZONE:
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
            break;
        default:
            SUNLOG(eCRITICAL_LOG, "Invalid Zone Sub Type [%u]", ZoneSubType);
            ZoneSubType = INDUN_KIND_MAX;
            break;
        }
    }
}



