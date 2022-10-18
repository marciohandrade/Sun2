#include "stdafx.h"

//////////////////////////////////////////////////////////////////////////
#include <PacketStruct_AG.h>
#include <MapInfoParser.h>
#include <ServerSession.h>

//////////////////////////////////////////////////////////////////////////
#include "DungeonSSQ.h"

#include <ServerSessions/ServerSessionManager.h>
#include <Zone/WarControlManager.h>

//==================================================================================================
//  <DungeonSSQ>
//
DungeonSSQ::DungeonSSQ()
    : InstanceDungeon(INDUN_KIND_SS_QUEST)
    , m_bCloseState(FALSE)
{
}

DungeonSSQ::~DungeonSSQ()
{
}

RC::eROOM_RESULT
DungeonSSQ::SetBaseRoom(User* pUser, BASE_ROOMINFO& baseRoomInfo, MAPCODE mapCode)
{
    const sMAPINFO* pInfo = MapInfoParser::Instance()->FindMapInfo(mapCode);
    if(!(pInfo && pInfo->byMKind == eZONETYPE_INSTANCE))
        return RC::RC_ROOM_INVALID_MAPCODE;

    baseRoomInfo.m_ClassLimit = pInfo->byClass;
    baseRoomInfo.m_MinLV      = (BYTE)pInfo->minLV;
    baseRoomInfo.m_MaxLV      = (BYTE)pInfo->maxLV;

    return RC::RC_ROOM_SUCCESS;
}

RC::eROOM_RESULT
DungeonSSQ::CanCreate(User* pUser, MAPCODE mapCode, eZONETYPE eZoneType, eZONEPUBLIC ePublic,
                      BASE_ROOMINFO& IN baseRoomInfo, BOOL checkOpenTime)
{
    __UNUSED((pUser, mapCode, eZoneType, ePublic, baseRoomInfo, checkOpenTime));
    //  (WAVERIX)(080805) - instance dungeon이지만 해당 맵은 그 시점 하나만 존재해야 한다.
    //  SSQControl command를 통해 확인할 필요가 있다.
    //  - 하지만 SSQUpdateCtrl에서 미리 확인하는 편이 복잡도가 감소하므로 그곳에서 처리하기로 한다.
    return RC::RC_ROOM_SUCCESS;
}

RC::eROOM_RESULT
DungeonSSQ::Create(User* pUser, // Not Used
                   MAPCODE mapCode, KEYTYPE instanceDungeonKey,
                   ServerSession* pServerSession, BOOL UseChargeItem)
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
        BASE_ROOMINFO roomInfo;

        RC::eROOM_RESULT rt = SetBaseRoom(pUser, roomInfo, mapCode);
        IfFailThenBreak(RC::RC_ROOM_SUCCESS == rt, rt);

        rt = CanCreate(pUser, mapCode, eZONETYPE_INSTANCE,
                       eZONEPUBLIC_PUBLIC, roomInfo, (!UseChargeItem));
        IfFailThenBreak(RC::RC_ROOM_SUCCESS == rt, rt);

        // 정보 복사 -----------------------------------------------------
        RoomInterface::Create(pUser, instanceDungeonKey, mapCode,
                              eZONEPUBLIC_PUBLIC, "", "", roomInfo, pServerSession);

        On_create();

        return RC::RC_ROOM_SUCCESS;
    }
    FLOWCONTROL_END;

    return (RC::eROOM_RESULT)flow.GetErrorCode();
}

VOID
DungeonSSQ::On_create()
{
    m_bCloseState = FALSE;
    RoomInterface::On_create();

    WarControlManager::OnMsg(SSQCtrlCmd::Created(), this);
}

VOID
DungeonSSQ::On_destroy()
{
    //  앞서서 Agent 는 해당 서버로 해당 존을 제거하는 명령을 보낸 상황이다.
    //  잘 살펴보고 아래의 루틴을 수정하기로 한다.
    //  (080725)(NOTE)
    //  - BattleServer 종료시, 유저들이 0가 되므로 ZoneManager::Update에서 해당 room의 제거가 가능하다.
    //  - 하지만 DungeonSSQ는 그렇지 못하는 문제가 있다. 방안을 강구하자.
    WarControlManager::OnMsg(SSQCtrlCmd::Destroy(), this);

    if(!IsReusedServerSession())
        ServerSessionManager::Instance()->Decr_LOF_BattleServer(GetLinkedServer()->GetSessionIndex());

    RoomInterface::On_destroy();
}

VOID
DungeonSSQ::On_enteruser(User* pUser)
{
    InstanceDungeon::On_enteruser(pUser);
    WarControlManager::OnMsg(SSQCtrlCmd::UserState::Enter(pUser), this);
}

VOID
DungeonSSQ::On_leaveuser(User* pUser)
{
    WarControlManager::OnMsg(SSQCtrlCmd::UserState::Leave(pUser), this);
    InstanceDungeon::On_leaveuser(pUser);
}

VOID
DungeonSSQ::SendJoinNAKPacket(User* pUser)
{
    MSG_CG_ZONE_EX_SSQ_JOIN_ACK msgNAK;
    msgNAK.SetFailResult(0, 0);
    pUser->SendPacket(&msgNAK, sizeof(msgNAK));

    pUser->SetDisconnectCode(RC::RC_DISCONNECT_DBP_CONNETION_ERROR);
    pUser->DisconnectUser();
}

VOID
DungeonSSQ::SendJoinACKPacket(User* pUser)
{
    MSG_CG_ZONE_EX_SSQ_JOIN_ACK msgACK;
    msgACK.SetSuccessResult(GetMapCode());
    pUser->SendPacket(&msgACK, msgACK.GetSize());
}


BOOL
DungeonSSQ::Process()
{
    // 종료 조건을 만들어라... 글구... RoomInterface::Process 개선 방안을 생각해 보자...
    // 하지만 도저히 시간이 촉박하다. 넘어가자... 힘들다.
    // mammoth object & garbage fields(unused..) 잘 못 파악하면 잘못 사용하게 되잖아?
    return !m_bCloseState;
}


