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


//��������������������������������������������������������������������������������������������������������������������������������������������������
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
//����������������������������������������������������������������������������������������������
//    Instance Dungeon�� ���⿡ �߰����� �� ��.

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

//    Instance Dungeon�� ���⿡ �߰��� ��
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

//��������������������������������������������������������������������������������������������������������������������������������������������������
//  <MoveZone Policyment>
//  -  ZoneCommon.h�� �����ϴ� MOVE_ZONE_CONTROL_LIST()�� ����� ����� �����Ѵ�.
//  -  �̵� ���� �˻� �ʴ���...

const RoomArgument&
RoomArgument::GetEmpty()
{
    static RoomArgument emptyArgument;
    return emptyArgument;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������
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
    ZoneInterface* const _FromZonePtr,  // FromKey�� ���� Zone
    ZoneInterface* const _ToZonePtr,    // ToKey�� ���� Zone
    const eZONETYPE _FromZoneTypeByObj, // FromZone�� ���� Ÿ��
    const eZONETYPE _ToZoneTypeByObj)    // ToZone�� ���� Ÿ��
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

//����������������������������������������������������������������������������������
template<>
class CheckMoveZonePolicy_Impl<MZTF_CHECK_SKIP> : public CheckMoveZonePolicy
{
private:
    static const eMOVE_ZONE_TYPE_FILTER  movetype = MZTF_CHECK_SKIP;
public:
    virtual RC::eROOM_RESULT EnableCheck()
    {
        // Ư���� ������ ���� ��� Ű�� ã�� �����͸� �����ϰ� ó���� �����Ѵ�.
        return ValidCheck() ? RC::RC_ROOM_SUCCESS : RC::RC_ROOM_NOT_EXIST_ROOM;
    }
};
//����������������������������������������������������������������������������������
template<>
class CheckMoveZonePolicy_Impl<MZTF_STATIC_TO_STATIC> : public CheckMoveZonePolicy
{
private:
    static const eMOVE_ZONE_TYPE_FILTER movetype = MZTF_STATIC_TO_STATIC;
public:
    virtual RC::eROOM_RESULT EnableCheck()
    {
        //    (NOTE)    -    �̵����� : FIELD<->FIELD, FIELD<->VILLAGE (Ʃ�丮�� ����)
        //    MapEnterencePortal�� ���� ������ �־������ ã������ ������ �ֱ���.
        //    �������� �Ϲ�ȭ ��Ƴ�... ��...
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
//����������������������������������������������������������������������������������
template<>
class CheckMoveZonePolicy_Impl<MZTF_ROOM_TO_STATIC> : public CheckMoveZonePolicy
{
private:
    static const eMOVE_ZONE_TYPE_FILTER  movetype = MZTF_ROOM_TO_STATIC;
public:
    virtual RC::eROOM_RESULT EnableCheck()
    {
        //    (NOTE)    -    ���� ROOM->VILLAGE+FIELD+TUTORIAL
        if(!ValidCheck())
            return RC::RC_ROOM_NOT_EXIST_ROOM;

        //    Ʈ���ſ� ���� �����ϴ� ��� RoomType�� Mission|Instance�� �ش�ȴ�.
        if(Args->ToZoneType != Args->ToZonePtr->GetType())
            return RC::RC_ROOM_NOT_EXIST_ROOM;

        const eZONETYPE ptrZoneType = Args->FromZonePtr->GetType();
        if(Args->FromZoneType != ptrZoneType &&    // Ʈ���ſ� ���� ��� ���� ó��
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

        //  (WAVERIX) (081021) (BUG-FIX) 'Ÿ���� ���'���� 'HUNTING_FIELD'�� ���� �ȵǴ� ���� ����
        //  - �˻����� ����: FromZoneType --> FromZoneTypeByObj 
        switch(Args->ToZoneTypeByObj)
        {
        case eZONETYPE_DOMINATION_FIELD:
        case eZONETYPE_FIELD:
            if(!(eZONETYPE_INSTANCE == Args->FromZoneTypeByObj ||
                 eZONETYPE_MISSION == Args->FromZoneTypeByObj))
                return RC::RC_ROOM_NOT_EXIST_ROOM;
            break;
        case eZONETYPE_TUTORIAL_FIELD:
			// ��ž���� ���ҵ帶���� ������ �� ���� ���� ����(�Ϻ��� ���ҵ帶������ ��Ʋ��â ���� ����)
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
//����������������������������������������������������������������������������������
template<>
class CheckMoveZonePolicy_Impl<MZTF_VILLAGE_TO_LOBBY> : public CheckMoveZonePolicy
{
private:
    static const eMOVE_ZONE_TYPE_FILTER  movetype = MZTF_VILLAGE_TO_LOBBY;
public:
    virtual RC::eROOM_RESULT EnableCheck()
    {
        //    (NOTE)    -    field���� lobby�� �̵��� ���� (TUTORIAL_FIELD)
        //    -    �ϴ� ��� STATIC(VILLAGE+FIELD)���� �̵� �����ϰ� �Ǿ���.
        if(!ValidCheck())
            return RC::RC_ROOM_NOT_EXIST_ROOM;

        if(Args->ToZoneType == Args->ToZonePtr->GetType())
        {    // ToZoneTypeByObj ... �̰��� �� �ָ�...
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
                // ��õ������ �κ�� �̵��� �� ����
                return RC::RC_ROOM_NOT_EXIST_ROOM;
            }
        }

        return RC::RC_ROOM_NOT_EXIST_ROOM;
    }
};
//����������������������������������������������������������������������������������
template<>
class CheckMoveZonePolicy_Impl<MZTF_LOBBY_TO_VILLAGE> : public CheckMoveZonePolicy
{
private:
    static const eMOVE_ZONE_TYPE_FILTER  movetype = MZTF_LOBBY_TO_VILLAGE;
public:
    virtual RC::eROOM_RESULT EnableCheck()
    {
        //    (NOTE)    -    LOBBY���� FIELD�� �̵��� ���� (TUTORIAL_FIELD)
        //    -    (Exception)
        //    -        LOBBY�� GetMapCode�� ���� ������ ROOM�� MapCode�� �����ϰ� �ִ�.
        //    -        �Ƹ� ���⼭ ������ �߻��ϴ� ���� ��...
        if(!ValidCheck())
            return RC::RC_ROOM_NOT_EXIST_ROOM;

        if(Args->FromZoneType == Args->FromZonePtr->GetType())
        {    // FromZoneTypeByObj ... �̰��� �� �ָ�...
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
//����������������������������������������������������������������������������������
template<>
class CheckMoveZonePolicy_Impl<MZTF_VILLAGE_TO_HUNTING> : public CheckMoveZonePolicy
{
private:
    static const eMOVE_ZONE_TYPE_FILTER  movetype = MZTF_VILLAGE_TO_HUNTING;
public:
    virtual RC::eROOM_RESULT EnableCheck()
    {
        //    (NOTE)    -    �������� �ٷ� HUNTING�� ����
        if(!ValidCheck())
            return RC::RC_ROOM_NOT_EXIST_ROOM;

        if(eZONETYPE_VILLAGE == Args->FromZoneType && eZONETYPE_HUNTING == Args->ToZoneType)
            return RC::RC_ROOM_SUCCESS;

        return RC::RC_ROOM_NOT_EXIST_ROOM;
    }
};
//����������������������������������������������������������������������������������
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
//����������������������������������������������������������������������������������
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
//����������������������������������������������������������������������������������
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
//����������������������������������������������������������������������������������
template<>
class CheckMoveZonePolicy_Impl<MZTF_STATIC_TO_ROOM> : public CheckMoveZonePolicy
{
private:
    static const eMOVE_ZONE_TYPE_FILTER  movetype = MZTF_STATIC_TO_ROOM;
public:
    virtual RC::eROOM_RESULT EnableCheck()
    {
        //  (090215) (NOTE) ��Ż ������ �����ϴ� ������
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
        // Character�� �⺻ ���� ����(���� �̼�, ���� ����Ʈ ���)�� GameServer���� �˻�� ����
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
//����������������������������������������������������������������������������������
template<>
class CheckMoveZonePolicy_Impl<MZTF_STATIC_TO_ROOM_FOR_UCUSER> : public CheckMoveZonePolicy
{
private:
    static const eMOVE_ZONE_TYPE_FILTER  movetype = MZTF_STATIC_TO_ROOM_FOR_UCUSER;
public:
    virtual RC::eROOM_RESULT EnableCheck()
    {
        //    (NOTE)    -    ������ ������ ���� ��û�� ó���Ǵ� ��ƾ
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


//����������������������������������������������������������������������������������
//    <MoveZone Policyment Code-Generations>
#define MOVE_ZONE_CTRL_NODE_NAME(mztf)    sCheckMoveZonePolicy_##mztf
//#define MOVE_ZONE_CTRL_NODE(eMOVE_ZONE_TYPE_FILTER)

#undef MOVE_ZONE_CTRL_NODE
#define MOVE_ZONE_CTRL_NODE(mztf)    \
    static CheckMoveZonePolicy_Impl<mztf>        MOVE_ZONE_CTRL_NODE_NAME(mztf);

MOVE_ZONE_CONTROL_LIST();    // declare module variables


//��������������������������������������������������������������������������������������������������������������������������������������������������


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

    // (WAVERIX) (090601) (BUG-FIX) GameServer�� ������ �޾� �̵�ó���� �ϴ� ���, (case Lobby)
    // pFromZone, pToZone�� null�� �� �ִ�.
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
        BOOST_STATIC_ASSERT(INDUN_KIND_MAX == 7);    // �߰� Ÿ�� ����� �ϴ� ���� �߰��� ��
        InstanceDungeon* const pIndun = InstanceDungeon_Info::downcast(pInterface);
        ZoneSubType = (WORD)pIndun->GetDungeonType();
        switch(pIndun->GetDungeonType())
        {
        case INDUN_KIND_TOP_OF_TRIAL:
        case INDUN_KIND_100LEVEL_QUEST:        // 100���� ü������, ���� �Ϲ����� ����Ʈ ������ Ȱ�� ����
        case INDUN_KIND_SS_QUEST:            // Ÿ���� ���
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



