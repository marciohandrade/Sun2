#include "stdafx.h"

#include "DungeonTopOfTrial.h"
#include "InstanceDungeon.h"

#include <AgentParty/AgentPartyManager.h>
#include <AgentParty/AgentParty.h>

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//  <DungeonTopOfTrial>
//
DungeonTopOfTrial::DungeonTopOfTrial()
    : InstanceDungeon(INDUN_KIND_TOP_OF_TRIAL)
{
}

DungeonTopOfTrial::~DungeonTopOfTrial()
{
}

RC::eROOM_RESULT
DungeonTopOfTrial::CanCreate(User* pUser, MAPCODE mapCode, eZONETYPE eZoneType, eZONEPUBLIC ePublic,
                             BASE_ROOMINFO& IN baseRoomInfo, BOOL checkOpenTime)
{
    RC::eROOM_RESULT rt = InstanceDungeon::CanCreate(pUser, mapCode, eZoneType,
                                                     ePublic, baseRoomInfo, checkOpenTime);
    if(rt != RC::RC_ROOM_SUCCESS)
        return rt;

    // -저주의 탑 Create 조건

    eZONETYPE userZoneType = pUser->GetZoneType();

    //필드 마을에서만 사용 가능
    switch(userZoneType)
    {
    case eZONETYPE_VILLAGE:
    case eZONETYPE_FIELD:
        //생성가능
        break;
    case eZONETYPE_DOMINATION_FIELD:
    case eZONETYPE_SPA_FIELD:
        //생성불가능
        return RC::RC_ROOM_CAN_NOT_CREATE_PLACE;
    default:
        return RC::RC_ROOM_CAN_NOT_CREATE_PLACE;
    }

    WORD partyKey = pUser->GetPartyState().GetPartyKey();

    rt = AgentPartyManager::Instance()->TopOfTrialCheck(partyKey);
    if(rt != RC::RC_ROOM_SUCCESS) return rt;

    return RC::RC_ROOM_SUCCESS;
}

VOID
DungeonTopOfTrial::SendJoinNAKPacket(User* pUser)
{
    MSG_CG_ZONE_INSTANCE_DUNGEON_JOIN_NAK nmsg;
    pUser->SendPacket(&nmsg, sizeof(nmsg));

    pUser->SetDisconnectCode(RC::RC_DISCONNECT_DBP_CONNETION_ERROR);
    pUser->DisconnectUser();
}

VOID
DungeonTopOfTrial::SendJoinACKPacket(User* pUser)
{
    MSG_CG_ZONE_INSTANCE_DUNGEON_JOIN_ACK amsg;
    amsg.m_MapCode = GetMapCode();
    pUser->SendPacket(&amsg, sizeof(amsg));
}

