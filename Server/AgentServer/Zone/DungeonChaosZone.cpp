#include "stdafx.h"

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#include "DungeonChaosZone.h"
#include <ServerSessions/ServerSessionManager.h>
#include <MapInfoParser.h>
#include <ChaosZoneSystem/PacketStruct_ChaosZoneSystem.h>
#include <ChaosZoneInfoParser.h>
#ifdef _NA_008334_20150608_SONNENSCHEIN
#include "ChaosZoneSystem/SonnenScheinManager.h"
#include <AgentParty/AgentPartyManager.h>
#endif //_NA_008334_20150608_SONNENSCHEIN

RC::eROOM_RESULT DungeonChaosZone::CanCreate(User* user, MAPCODE map_code, 
                                             eZONETYPE zone_type, eZONEPUBLIC zone_public, 
                                             BASE_ROOMINFO& IN base_room_info, BOOL check_open_time)
{
    return RC::RC_ROOM_SUCCESS;
}

RC::eROOM_RESULT DungeonChaosZone::Create(User* user, MAPCODE map_code, KEYTYPE instance_dungeon_key, 
                                                ServerSession* server_session, BOOL use_charge_item)
{
    //  (WAVERIX)(080220)
    //  - 1. User에 의해 생성되는 것이 아니라 시간에 의해 생성된다. pUser = NULL
    //  - 2. baseRoomInfo... 취할 것 있으면 취하자.
    //  - 3. mapCode, eZoneType : SSQ
    //  - 4. ePublic = PUBLIC, checkOpenTime<-Check!
    //////////////////////////////////////////////////////////////////////////
    //    <LOGIC CONTROL>
    FlowControl flow;
    FLOWCONTROL_START()
    {
        BASE_ROOMINFO room_Info;

        RC::eROOM_RESULT rt = SetBaseRoom(user, room_Info, map_code);
        IfFailThenBreak(RC::RC_ROOM_SUCCESS == rt, rt);

        rt = CanCreate(user, map_code, eZONETYPE_INSTANCE,
            eZONEPUBLIC_PUBLIC, room_Info, (!use_charge_item));
        IfFailThenBreak(RC::RC_ROOM_SUCCESS == rt, rt);

        // 정보 복사 -----------------------------------------------------
        RoomInterface::Create(user, instance_dungeon_key, map_code,
            eZONEPUBLIC_PUBLIC, "", "", room_Info, server_session);

#ifdef _NA_008334_20150608_SONNENSCHEIN
        SetMaster(user);

        //ChaosZone Type을 기억시켜둔다 (어떤카오스존인지 구분할 수있게)
        const ChaosZoneInfo* const parser_info = ChaosZoneInfoParser::Instance()->FindData(map_code);
        if (parser_info != NULL) 
        {
            SetChaosZoneType((ChaosZoneType)parser_info->kind_of_room);
        }
#endif //_NA_008334_20150608_SONNENSCHEIN

        On_create();

        return RC::RC_ROOM_SUCCESS;
    }
    FLOWCONTROL_END;

    return (RC::eROOM_RESULT)flow.GetErrorCode();
}

RC::eROOM_RESULT DungeonChaosZone::SetBaseRoom(User* user, BASE_ROOMINFO& base_room_info, MAPCODE map_code)
{
    const sMAPINFO* map_info = MapInfoParser::Instance()->FindMapInfo(map_code);
    if(!(map_info && map_info->byMKind == eZONETYPE_INSTANCE))
        return RC::RC_ROOM_INVALID_MAPCODE;

    base_room_info.m_ClassLimit = map_info->byClass;
    base_room_info.m_MinLV      = static_cast<BYTE>(map_info->minLV);
    base_room_info.m_MaxLV      = static_cast<BYTE>(map_info->maxLV);

    return RC::RC_ROOM_SUCCESS;
}

void DungeonChaosZone::SendJoinNAKPacket(User* user)
{
    MSG_CG_ZONE_INSTANCE_DUNGEON_JOIN_NAK nak_msg;
    user->SendPacket(&nak_msg, sizeof(nak_msg));

    user->SetDisconnectCode(RC::RC_DISCONNECT_DBP_CONNETION_ERROR);
    user->DisconnectUser();
}

void DungeonChaosZone::SendJoinACKPacket(User* user)
{
    const ChaosZoneInfo* const parser_info = ChaosZoneInfoParser::Instance()->FindData(GetMapCode());
    if (parser_info == NULL)
    {
        return;
    }

    MSG_CG_CHAOS_ZONE_ZONE_JOIN_ACK ack_msg;
    ack_msg.map_code = GetMapCode();
    ack_msg.chaos_zone_type = static_cast<ChaosZoneType>(parser_info->kind_of_room);
    user->SendPacket(&ack_msg, sizeof(ack_msg));
}

#ifdef _NA_008334_20150608_SONNENSCHEIN
VOID DungeonChaosZone::On_destroy()
{
    InstanceDungeon::On_destroy();
}

VOID DungeonChaosZone::On_leaveuser( User* user )
{
    if (GetChaosZoneType() == eCHAOSZONE_TYPE_SONNENSCHEIN)
    {
        BOOL is_master = GetMaster() == user;

        //sonnenschein은 '방 인원이 0명일 경우 방 소멸' 로직을 거치지 않아야 한다
        AgentPartyManager * const party_manager = AgentPartyManager::Instance();
        WORD party_key = user->GetPartyState().GetPartyKey();
        if (user->GetPartyState().IsPartyMember())
        {
            BaseParty* party = const_cast<BaseParty*>(party_manager->FindParty(party_key));
            if (party)
            {
                if (party->GetType() != ePARTY_ROOM)
                {
                    if(GetMaster() == user) {
                        VestUserWithMaster(GetFirstUser());
                    }
                }
                party_manager->LeaveZone(user, this);
            }
            else
            {
                SUNLOG(eCRITICAL_LOG, __FUNCTION__" : Invalid party state : %d", party_key);
            }
        }

        if(is_master)
        {
            AgentPartyManager* const party_manager = AgentPartyManager::Instance();

            party_manager->ChangeMaster_inInstanceDungeon(this, user);
        }
    }
    else {
        InstanceDungeon::On_leaveuser(user);
    }
}

#endif //_NA_008334_20150608_SONNENSCHEIN

#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
