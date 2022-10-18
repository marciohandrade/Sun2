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
    //  (WAVERIX)(080805) - instance dungeon������ �ش� ���� �� ���� �ϳ��� �����ؾ� �Ѵ�.
    //  SSQControl command�� ���� Ȯ���� �ʿ䰡 �ִ�.
    //  - ������ SSQUpdateCtrl���� �̸� Ȯ���ϴ� ���� ���⵵�� �����ϹǷ� �װ����� ó���ϱ�� �Ѵ�.
    return RC::RC_ROOM_SUCCESS;
}

RC::eROOM_RESULT
DungeonSSQ::Create(User* pUser, // Not Used
                   MAPCODE mapCode, KEYTYPE instanceDungeonKey,
                   ServerSession* pServerSession, BOOL UseChargeItem)
{
    //  (WAVERIX)(080220)
    //  - 1. User�� ���� �����Ǵ� ���� �ƴ϶� �ð��� ���� �����ȴ�. pUser = NULL
    //  - 2. baseRoomInfo... ���� �� ������ ������.
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

        // ���� ���� -----------------------------------------------------
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
    //  �ռ��� Agent �� �ش� ������ �ش� ���� �����ϴ� ����� ���� ��Ȳ�̴�.
    //  �� ���캸�� �Ʒ��� ��ƾ�� �����ϱ�� �Ѵ�.
    //  (080725)(NOTE)
    //  - BattleServer �����, �������� 0�� �ǹǷ� ZoneManager::Update���� �ش� room�� ���Ű� �����ϴ�.
    //  - ������ DungeonSSQ�� �׷��� ���ϴ� ������ �ִ�. ����� ��������.
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
    // ���� ������ ������... �۱�... RoomInterface::Process ���� ����� ������ ����...
    // ������ ������ �ð��� �˹��ϴ�. �Ѿ��... �����.
    // mammoth object & garbage fields(unused..) �� �� �ľ��ϸ� �߸� ����ϰ� ���ݾ�?
    return !m_bCloseState;
}


