#include "stdafx.h"
#include "InstanceDungeon.h"
//==================================================================================================
#include <MapInfoParser.h>
#include <PacketStruct_AG.h>
//==================================================================================================
#include <Zone/ZoneManager.h>
#include <Zone/WarControlManager.h>

#include <ServerSessions/ServerSessionManager.h>
#include <ServerSessions/ServerSessionEx.h>

#include <AgentParty/AgentPartyManager.h>
#include <AgentParty/AgentParty.h>

//_NA_20110630_BATTLE_ZONE_RENEWAL
#include <AgentParty/AgentPartyOperator.h>

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#include <ChaosZoneInfoParser.h> 
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
//==================================================================================================
//
//

InstanceDungeon::~InstanceDungeon(void)
{
}

BOOLEAN
InstanceDungeon::RegisterRuleset(const MAPCODE map_code)
{
    //    ZoneManager::CreateInstanceDungeon 호출 이전에 설정한다.
    eZONE_RULESET& rRuleset = const_cast<eZONE_RULESET&>(Ruleset);

    MapInfoParser* const pMapInfoParser = MapInfoParser::Instance();
    const sMAPINFO* const pMapInfo = pMapInfoParser->FindMapInfo(map_code);

    rRuleset.rules
        = (pMapInfo && pMapInfo->byMKind == eZONETYPE_INSTANCE)
        ?   pMapInfo->Ruleset.rules
        :   rRuleset.ZONE_RULE_EMPTY
        ;

    m_KindOfDungeonVariation
        = (rRuleset.rules != rRuleset.ZONE_RULE_EMPTY)
        ?   eINSTANCE_DUNGEON_KIND(pMapInfo->byMType)
        :   INDUN_KIND_MAX
        ;

    return m_KindOfDungeonVariation != INDUN_KIND_MAX;
}

VOID
InstanceDungeon::On_create()
{    
    RoomInterface::On_create();

    MSG_AG_ZONE_INSTANCE_CREATE_CMD msg;
    msg.m_dwKey     = (GetMaster() ? GetMaster()->GetUserKey() : 0);
    msg.m_MapCode   = GetMapCode();
    msg.m_RoomKey   = GetKey();    
    msg.m_TypeOfIndun = GetDungeonTypeVariation();
    msg.m_RoomInfo  = *GetRoomInfo();   //< 생성할 방 정보
#ifdef _NA_004204_20120131_BATTLE_ZONE_PARTY_OBTAIN
    msg.party_zone_info_ = PartyOfTopOfTrialByCash;
#endif //_NA_004204_20120131_BATTLE_ZONE_PARTY_OBTAIN
#ifdef  _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    if(GetDungeonTypeVariation() == INDUN_KIND_CHAOS_ZONE) 
    {
        msg.party_zone_info_ = PartyOfChaosZone;
        msg.chaos_zone_type_ = ChaosZoneInfoParser::Instance()->GetType(GetMapCode());
    }
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    SendToLinkedServer(&msg, sizeof(msg));    
}

VOID
InstanceDungeon::On_destroy()
{    
    if(!IsReusedServerSession())
        ServerSessionManager::Instance()->Decr_LOF_BattleServer(GetLinkedServer()->GetSessionIndex());

    RoomInterface::On_destroy();
}

RC::eROOM_RESULT
InstanceDungeon::CanCreate(User* user, MAPCODE map_code, eZONETYPE zone_type, eZONEPUBLIC zone_public,
                           BASE_ROOMINFO& IN base_room_info, BOOL need_check_open_time)
{
    const sMAPINFO* const map_info = MapInfoParser::Instance()->FindMapInfo(map_code);
    if(map_info == NULL)
    {
        return RC::RC_ROOM_INVALID_MAPCODE;
    }

    WORD partyKey = user->GetPartyState().GetPartyKey();
    RC::eROOM_RESULT result_code = 
        AgentPartyManager::Instance()->InstanceDungeonCheck(partyKey, map_info);
    if(result_code != RC::RC_ROOM_SUCCESS)
    {
        return result_code;
    }

    if(!GetServerOptChaoStateUseZone(zone_type) && user->IsChaoState())
    {
        return RC::RC_ROOM_UNABLE_FUNCTION_FOR_CHAOSTATE;
    }

    result_code = RoomInterface::CanCreate(user,
                                           map_code, eZONETYPE_INSTANCE,
                                           zone_public, base_room_info,
                                           need_check_open_time);
    if (result_code != RC::RC_ROOM_SUCCESS)
    {
        return result_code;
    }

    return RC::RC_ROOM_SUCCESS;
}


RC::eROOM_RESULT
InstanceDungeon::Create(User* user, MAPCODE map_code, KEYTYPE instance_dungen_key,
                        ServerSession* server_session, BOOL need_check_open_time)
{
    BASE_ROOMINFO roomInfo;    
    RC::eROOM_RESULT result_code = SetBaseRoom(user, roomInfo, map_code);
    if (result_code != RC::RC_ROOM_SUCCESS)
    {
        return result_code;
    }

    result_code = CanCreate(user, map_code, eZONETYPE_INSTANCE, eZONEPUBLIC_PUBLIC, 
                            roomInfo, need_check_open_time);
    if (result_code != RC::RC_ROOM_SUCCESS)
    {
        return result_code;    
    }

    //_NA_20110630_BATTLE_ZONE_RENEWAL
    SetMaster(user);
    LinkParty(user->GetPartyState().GetPartyKey());

    RoomInterface::Create(user, instance_dungen_key, map_code, eZONEPUBLIC_PUBLIC, 
                          "", "", roomInfo, server_session);

    eROOMSTATE_TYPE type =  GetRoomState();
    SetRoomState(eROOMSTATE_GENERNAL);
    result_code = CanJoin(user, GetZonePublic(), GetRoomPWD());
    SetRoomState(type);
    if(result_code != RC::RC_ROOM_SUCCESS)
    {
        return result_code;
    }

    On_create();

    return result_code;
}



RC::eROOM_RESULT
InstanceDungeon::SetBaseRoom(User* pUser, BASE_ROOMINFO& baseRoomInfo, MAPCODE mapCode)
{
    const sMAPINFO* pInfo = MapInfoParser::Instance()->FindMapInfo(mapCode);
    if(!pInfo) return RC::RC_ROOM_INVALID_MAPCODE;
    if(pInfo->byMKind != eZONETYPE_INSTANCE)
        return RC::RC_ROOM_INVALID_MAPCODE;

    baseRoomInfo.m_ClassLimit    = pInfo->byClass;
    baseRoomInfo.m_MinLV        = (BYTE)pInfo->minLV;
    baseRoomInfo.m_MaxLV        = (BYTE)pInfo->maxLV;    

    //PC방인가
    if(pUser->CanApplyPCBangBenefit(TRUE))
    {
        baseRoomInfo.m_SpecialType = eROOM_SPECIAL_PCBANG_SUPER_ROOM;
    }
    //슈퍼방장 아이템을 소유하고 있는가
    else if(pUser->GetItemOwnFlag().FindItemInfo(eITEMTYPE_SUPERMASTER))
    {
        baseRoomInfo.m_SpecialType = eROOM_SPECIAL_ITEM1_SUPER_ROOM;
    }

    return RC::RC_ROOM_SUCCESS;
}

VOID
InstanceDungeon::On_readyuser(User* pUser)
{
    RoomInterface::On_readyuser(pUser);
    pUser->SetZoneState(ePRS_BEFORE_ENTER_INSTNACE);
    pUser->SetZoneKey(GetKey());
    pUser->SetZoneType(eZONETYPE_INSTANCE);
}

VOID
InstanceDungeon::On_enteruser(User* pUser)
{
    RoomInterface::On_enteruser(pUser);
    pUser->SetZoneState(ePRS_AT_INSTANCE);
}

VOID
InstanceDungeon::On_leaveuser(User* user)
{
    BOOL is_master = GetMaster() == user;

    RoomInterface::On_leaveuser(user);

    //_NA_20110630_BATTLE_ZONE_RENEWAL
    if(is_master)
    {
        AgentPartyManager* const party_manager = AgentPartyManager::Instance();

        party_manager->ChangeMaster_inInstanceDungeon(this, user);
    }
}

RC::eROOM_RESULT
InstanceDungeon::_CanJoinEach(User * pUser, eZONEPUBLIC RoomPublic, TCHAR * pszRoomPWD)
{
    if(!GetServerOptChaoStateUseZone(GetType()) && pUser->IsChaoState())
        return RC::RC_ROOM_UNABLE_FUNCTION_FOR_CHAOSTATE;

    return RC::RC_ROOM_SUCCESS;
}

////////////////////////////////////////////////* End InstanceDungeon *////////////////////////////////////////////////////////



//==================================================================================================
//    <Milestone100LevelQuest>
//
Milestone100LevelQuest::Milestone100LevelQuest() : InstanceDungeon(INDUN_KIND_100LEVEL_QUEST)
{
}

Milestone100LevelQuest::~Milestone100LevelQuest()
{
}

RC::eROOM_RESULT
Milestone100LevelQuest::CanCreate(User* pUser, MAPCODE mapCode,
                                  eZONETYPE eZoneType, eZONEPUBLIC ePublic,
                                  BASE_ROOMINFO& IN baseRoomInfo, BOOL needCheckOpenTime)
{
    RC::eROOM_RESULT rt = InstanceDungeon::CanCreate(pUser, mapCode,
                                                     eZoneType, ePublic, baseRoomInfo,
                                                     needCheckOpenTime);
    if(rt != RC::RC_ROOM_SUCCESS)
        return rt;

    // -저주의 탑 Create 조건
    //필드 마을에서만 사용 가능
    eZONETYPE zone_type = pUser->GetZoneType();
    switch(zone_type)
    {
    case eZONETYPE_VILLAGE:
    case eZONETYPE_FIELD:
        // 생성가능
        break;
    case eZONETYPE_DOMINATION_FIELD:
    case eZONETYPE_SPA_FIELD:
        return RC::RC_ROOM_CAN_NOT_CREATE_PLACE;
    default:
        return RC::RC_ROOM_CAN_NOT_CREATE_PLACE;
    }

    return RC::RC_ROOM_SUCCESS;
}

VOID
Milestone100LevelQuest::SendJoinNAKPacket(User* pUser)
{
    MSG_CG_ZONE_EX_CAN_JOIN_100LEVEL_QUEST_MISSION_NAK msgNAK;
    pUser->SendPacket(&msgNAK, sizeof(msgNAK));

    pUser->SetDisconnectCode(RC::RC_DISCONNECT_DBP_CONNETION_ERROR);
    pUser->DisconnectUser();
}

VOID
Milestone100LevelQuest::SendJoinACKPacket(User* pUser)
{
    MSG_CG_ZONE_EX_CAN_JOIN_100LEVEL_QUEST_MISSION_ACK msgACK;
    msgACK.SetSuccessResult(GetMapCode());
    pUser->SendPacket(&msgACK, msgACK.GetSize());
}

