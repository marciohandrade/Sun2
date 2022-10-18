#include "stdafx.h"

//==================================================================================================
#include <PacketStruct_AG.h>
#include <PacketStruct_AW.h>
#include <MapInfoParser.h>

#include <ItemOwnFlag.h>
#include <ServerOptionParserEx.h>
#include <PacketControl/PacketControl.h>
//==================================================================================================
#include "ServerSessions/ServerSessionManager.h"
#include "ServerSessions/ServerSessionEx.h"
#include "UserSessions/User.h"

#include "ZoneInterface.h"
#include "ZoneManager.h"
#include "LobbyRoom.h"

// MapCode() 얻기 위해
#include "HuntingRoom.h"
#include "MissionRoom.h"
#include "InstanceDungeon.h"
#include "DungeonTopOfTrial.h"
#include "DungeonSSQ.h"

//==================================================================================================

ZoneInterface::ZoneInterface()
    : m_Key(0)
    , m_LinkedServerSession(0)
    , m_bReusedKey(0)
    , m_bReusedServerSession(0)
    , m_LinkPartyKey(0)
{
    ZeroMemory(__paddings_for_debug, sizeof(__paddings_for_debug));
}


ZoneInterface::~ZoneInterface()
{
}


VOID
ZoneInterface::Create()
{
    On_create();
}


VOID
ZoneInterface::Destroy()
{
    if(GetLinkedServer()) 
    {
        MSG_AG_ZONE_ROOM_DESTROY_CMD msg;
        msg.m_RoomKey = GetKey();
        SendToLinkedServer(&msg, sizeof(msg));
    }

    On_destroy();

    // 초기화
    ZONE_USER_LIST::iterator it = m_UserList.begin(),
                             itend = m_UserList.end();
    while(it!=itend)
    {
        User* const pUser = it->second;
        ++it; // for sync iterator
        DestroyUser(pUser);
    }
    m_UserList.clear();

    SetLinkedServer(NULL);

    m_LinkPartyKey = 0;
}

// (WAVERIX) (090520) (CHANGES) sMAPINFO control
BOOLEAN
ZoneInterface::SetMapCode(MAPCODE mapCode)
{
    m_MapCode = mapCode;
    if(mapCode != 0)
    {
        const sMAPINFO* pMapInfo = MapInfoParser::Instance()->FindMapInfo(mapCode);
        m_pMapInfo = pMapInfo;
    }
    else
        m_pMapInfo = 0;
    return !!m_pMapInfo;
}

void ZoneInterface::RemoveUser(const DWORD player_key)
{
    m_UserList.erase(player_key);
}

void ZoneInterface::AddUser(User* const user, const DWORD player_key)
{
#if !defined(_DEBUG)
    m_UserList.insert(ZONE_USER_LIST::value_type(player_key, user));
#else
    std::pair<ZONE_USER_LIST::iterator, bool> ret = \
        m_UserList.insert(ZONE_USER_LIST::value_type(player_key, user));
    ASSERT(ret.second == true);
#endif
}

VOID
ZoneInterface::ReadyUser(User* pUser, eSEND_TYPE type,
                         MAPCODE fieldCode, DWORD areaID, WzVector* pwvStartPos, bool bFirstJoin)
{
    if(GetLinkedServer())
        pUser->LinkServer(GetLinkedServer()->GetSessionIndex());

    switch(type)
    {
    case eSEND_SYN:
        {
        //#ifdef _PCBANG_POLICY_CHANGE_POST_EXPIRED
            pUser->NotifyPcBangBillingStatus(true, true);
        //#endif

            MSG_AG_ZONE_ROOM_JOIN_SYN msg;
            msg.m_dwKey       = pUser->GetUserKey();
            msg.m_byRoomType  = GetType();
            msg.m_dwPlayerKey = pUser->GetPlayerKey();
            msg.m_bySelectedCharIndex = pUser->GetSelectedCharIndex();
            msg.m_RoomKey     = GetKey();
            msg.m_dwCharGUID  = pUser->GetSelectedCharGuid();
            msg.m_dwReservedValue = pUser->GetReservedValue();
            msg.m_bySts       = pUser->GetSts();
            msg.m_PCRoomSts   = pUser->GetPCRoomSts();

            CHECK_ID_LENGTH_BUFFER(msg.m_pszUserID);
            _tcsncpy(msg.m_pszUserID, pUser->GetUserID(), _countof(msg.m_pszUserID));
            msg.m_pszUserID[_countof(msg.m_pszUserID) - 1] = '\0';

            memcpy(msg.m_pszUserIP, pUser->GetClientIP(), IP_MAX_LEN);    //< char형 사용 모듈도char로 변경할예정
            msg.m_LoginTime_t  = pUser->GetLoginTime_t();
            msg.m_bIsFirstJoin = bFirstJoin;

        #ifdef __CN_OO0510__FATIGUE_MATCHING_UPDATE
            msg.m_byAge        = pUser->GetAge();
            msg.m_byRealNameAuth = pUser->GetRealNameAuth();
        #endif //__CN_OO0510__FATIGUE_MATCHING_UPDATE

            ClosingUserTransController& rUCUControl = pUser->closing_user_trans_controller_;
            if(rUCUControl.IsTransaction() == 0)
            {
                msg.m_wFieldCode = fieldCode;
                msg.m_dwAreaID   = areaID;
                if(pwvStartPos) msg.m_vStartPos = *pwvStartPos;
                else            SetVector(&msg.m_vStartPos, .0f, .0f, .0f);
                msg.m_byApplyFieldCode = (fieldCode != 0);
            }
            else
            {
                msg.m_wFieldCode = rUCUControl.GetFieldCode();
                msg.m_dwAreaID   = 0;
                msg.m_vStartPos  = rUCUControl.GetRandomPos();
                msg.m_byApplyFieldCode = TRUE;
            }

            msg.m_GuildGuid = pUser->GetSelectedCharGuildGuid();
            SendToLinkedServer(&msg, sizeof(msg));
        }
        break;
    case eSEND_CMD:
        {
            MSG_AG_ZONE_ROOM_JOIN_CMD msg;
            msg.m_dwKey     = pUser->GetUserKey();
            msg.m_RoomKey   = GetKey();
            msg.m_wFieldCode = fieldCode;
            msg.m_dwAreaID  = areaID;

            ZeroMemory(&msg.m_vStartPos, sizeof(msg.m_vStartPos));
            if(pwvStartPos)
                msg.m_vStartPos = *pwvStartPos;

            SendToLinkedServer(&msg, sizeof(msg));
        }
        break;
    }

    eZONETYPE zoneType = GetType();
    switch(GetType())
    {
    case eZONETYPE_VILLAGE: g_ZoneManager.IncVillageUserCount(); break;
    case eZONETYPE_FIELD:   g_ZoneManager.IncHuntionFieldRoomUserCount(); break;
    case eZONETYPE_DOMINATION_FIELD:   g_ZoneManager.IncDominationFieldUserCount(); break;
#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
    case eZONETYPE_SPA_FIELD: g_ZoneManager.IncSpaFieldUserCount(); break;
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM
    case eZONETYPE_LOBBY:
        {
            LobbyRoom* pVRoom = (LobbyRoom *)this;
            switch(pVRoom->GetLobbyType())
            {
            case eZONETYPE_HUNTING: g_ZoneManager.IncHuntingLobbyRoomUserCount();   break;
            case eZONETYPE_MISSION: g_ZoneManager.IncMissionLobbyRoomUserCount();   break;
            case eZONETYPE_PVP:     g_ZoneManager.IncPVPLobbyRoomUserCount();       break;
            case eZONETYPE_CHUNTING:g_ZoneManager.IncCHuntingLobbyRoomUserCount();  break;
            }
        }
        break;
    case eZONETYPE_HUNTING:  g_ZoneManager.IncHuntingRoomUserCount();       break;
    case eZONETYPE_MISSION:  g_ZoneManager.IncMissionRoomUserCount();       break;
    case eZONETYPE_CHUNTING: g_ZoneManager.IncCHuntionRoomUserCount();      break;
    case eZONETYPE_INSTANCE: g_ZoneManager.IncInstanceDungeonUserCount();   break;
    }
    // NOTE: f110906.1L, in China version, it has sometimes occurred crash faults.
    // the major reason of crashes is the vector last pointing error in hash_map for user listing.
    // in general, the result that is based on a related analysis is ambigous.
    // it may be summarized be as follow,
    //   first, the memory invasion by other objects
    //   second, the vector assembly code inordered operation.
    static uint8_t debug_stream[sizeof(*this)];
    CopyMemory(debug_stream, this, sizeof(debug_stream));
    //
    DWORD playerKey = pUser->GetPlayerKey();
    ASSERT(FindUser(playerKey) == NULL);
    AddUser(pUser, playerKey);

    On_readyuser(pUser);

    if(type == eSEND_SYN || zoneType == eZONETYPE_CHARSELECT)
    {
        // 서버 세션이 바뀔 때 초기화 해주어야할 값들이 있다.
        if(pUser->GetItemOwnFlag().GetItemNum())
        {
            MSG_AW_ITEM_SPECIAL_ITEM_CLEAR_CMD AWMsg;
            AWMsg.m_dwCharGuid = pUser->GetSelectedCharGuid();
            pUser->SendToWorldServer(&AWMsg, sizeof(AWMsg));
        }

        pUser->GetItemOwnFlag().Init();
    }
}

VOID
ZoneInterface::EnterUser(User* pUser)
{
    On_enteruser(pUser);
    eZONETYPE zoneType = GetType();



    MSG_AW_USER_ENTER_ZONE_SYN msg;
    msg.m_byCategory = AW_USER_STATUS;
    msg.m_byProtocol = AW_USER_ENTER_ZONE_SYN;
    msg.dwZoneKey   = GetKey();
    msg.byZoneType  = zoneType;
    msg.m_dwKey     = pUser->GetUserKey();

    msg.MapCode = 0;
    switch(zoneType)
    {
    case eZONETYPE_VILLAGE:
    case eZONETYPE_FIELD:
    case eZONETYPE_DOMINATION_FIELD:
    case eZONETYPE_SPA_FIELD:
#ifdef _NA002676_WORLD_SERVER_RENEWAL
        msg.MapCode = this->GetMapCode();
        break;
#endif
    case eZONETYPE_CHARSELECT:
        break;
    case eZONETYPE_LOBBY:
        {
            LobbyRoom* pVRoom = (LobbyRoom *)this;
            msg.MapCode = pVRoom->GetMapCode();
        }
        break;
    case eZONETYPE_HUNTING:
        {
            HuntingRoom* pMRoom = (HuntingRoom *)this;
            msg.MapCode = pMRoom->GetMapCode();
        }
        break;
    case eZONETYPE_MISSION:
        {
            MissionRoom* pMRoom = (MissionRoom *)this;
            msg.MapCode = pMRoom->GetMapCode();
        }
        break;
    case eZONETYPE_INSTANCE:
        {        
            InstanceDungeon* pDungeon = (InstanceDungeon *)this;
            msg.MapCode = pDungeon->GetMapCode();
        }
        break;
    case eZONETYPE_QUEST:
    case eZONETYPE_PVP:
    case eZONETYPE_EVENT:
    case eZONETYPE_GUILD:
    case eZONETYPE_SIEGE:
    case eZONETYPE_CHUNTING:
        {
            RoomInterface* pRoom = static_cast<RoomInterface*>(this);

            msg.MapCode = pRoom->GetMapCode();
        }
        break;
    default:
        {
#ifdef _DEBUG
            assert(0 && "ZoneInterface::EnterUser Error - invalid ZONETYPE");
#else
            MessageOut(eCRITICAL_LOG, "ZoneInterface EnterUser Error - Invalid ZONETYPE(%u)", GetType());
#endif
        }
        break;
    }

    pUser->SendToWorldServer(&msg, sizeof(msg));
}

// 임시 처리
VOID
ZoneInterface::DestroyUser(User* pUser)
{
    eZONETYPE zoneType = GetType();
    switch(zoneType)
    {
    case eZONETYPE_VILLAGE: g_ZoneManager.DecVillageUserCount(); break;
    case eZONETYPE_FIELD:   g_ZoneManager.DecHuntionFieldRoomUserCount(); break;
    case eZONETYPE_DOMINATION_FIELD:   g_ZoneManager.DecDominationFieldUserCount(); break;
#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
    case eZONETYPE_SPA_FIELD: g_ZoneManager.DecSpaFieldUserCount(); break;
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM
    case eZONETYPE_LOBBY:
        {
            LobbyRoom* pVRoom = (LobbyRoom *)this;
            switch(pVRoom->GetLobbyType())
            {
            case eZONETYPE_HUNTING: g_ZoneManager.DecHuntingLobbyRoomUserCount();   break;
            case eZONETYPE_MISSION: g_ZoneManager.DecMissionLobbyRoomUserCount();   break;
            case eZONETYPE_PVP:     g_ZoneManager.DecPVPLobbyRoomUserCount();       break;
            case eZONETYPE_CHUNTING:g_ZoneManager.DecCHuntingLobbyRoomUserCount();  break;
            }
        }
        break;
    case eZONETYPE_HUNTING:  g_ZoneManager.DecHuntingRoomUserCount();   break;
    case eZONETYPE_CHUNTING: g_ZoneManager.DecCHuntingRoomUserCount();  break;
    case eZONETYPE_MISSION:  g_ZoneManager.DecMissionRoomUserCount();   break;
    case eZONETYPE_INSTANCE: g_ZoneManager.DecInstanceDungeonUserCount(); break;
    }

    MSG_AG_ZONE_ROOM_LEAVE_CMD msg;
    msg.m_dwKey = pUser->GetUserKey();
    msg.m_RoomKey = GetKey();
    SendToLinkedServer(&msg, sizeof(msg));


    MSG_AW_USER_LEAVE_ZONE_SYN wmsg;
    wmsg.m_byCategory = AW_USER_STATUS;
    wmsg.m_byProtocol = AW_USER_LEAVE_ZONE_SYN;
    wmsg.dwZoneKey  = GetKey();
    wmsg.byZoneType = zoneType;
    wmsg.m_dwKey    = pUser->GetUserKey();
    pUser->SendToWorldServer(&wmsg, sizeof(wmsg));

    // 공통 부분
    pUser->SetZoneState(ePRS_NOT_AT_ZONE);
    pUser->SetZoneKey(0);
    pUser->SetZoneType(eZONETYPE_MAX);

    // 유저 삭제
    DWORD playerKey = pUser->GetPlayerKey();
    ASSERT(FindUser(playerKey) != NULL);
    RemoveUser(playerKey);

    On_leaveuser(pUser);
}

VOID
ZoneInterface::LeaveUser(User* pUser, eSEND_TYPE type, eZONETYPE toZone, DWORD toLinkServerIndex)
{
    pUser->SetMoveZoneType(GetType(), toZone, toLinkServerIndex);

    switch(GetType())
    {
    case eZONETYPE_VILLAGE: g_ZoneManager.DecVillageUserCount(); break;
    case eZONETYPE_FIELD:   g_ZoneManager.DecHuntionFieldRoomUserCount(); break;
    case eZONETYPE_DOMINATION_FIELD:   g_ZoneManager.DecDominationFieldUserCount(); break;
#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
    case eZONETYPE_SPA_FIELD: g_ZoneManager.DecSpaFieldUserCount(); break;
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM
    case eZONETYPE_LOBBY:
        {
            LobbyRoom* pVRoom = (LobbyRoom *)this;
            switch(pVRoom->GetLobbyType())
            {
            case eZONETYPE_HUNTING: g_ZoneManager.DecHuntingLobbyRoomUserCount();   break;
            case eZONETYPE_MISSION: g_ZoneManager.DecMissionLobbyRoomUserCount();   break;
            case eZONETYPE_PVP:     g_ZoneManager.DecPVPLobbyRoomUserCount();    break;
            case eZONETYPE_CHUNTING:g_ZoneManager.DecCHuntingLobbyRoomUserCount();  break;
            }
        }
        break;
    case eZONETYPE_HUNTING:  g_ZoneManager.DecHuntingRoomUserCount();     break;
    case eZONETYPE_CHUNTING: g_ZoneManager.DecCHuntingRoomUserCount();    break;
    case eZONETYPE_MISSION:  g_ZoneManager.DecMissionRoomUserCount();     break;
    case eZONETYPE_INSTANCE: g_ZoneManager.DecInstanceDungeonUserCount(); break;
    }

    switch(type)
    {
    case eSEND_SYN:
        {
            MSG_AG_ZONE_ROOM_LEAVE_SYN msg;
            msg.m_dwKey = pUser->GetUserKey();
            msg.m_byRoomType = GetType();
            msg.m_RoomKey = GetKey();
            msg.m_ToRoomType = toZone;  //나가서 가려는 방타입.
            SendToLinkedServer(&msg, sizeof(msg));
        }break;
    case eSEND_CMD:
        {
            MSG_AG_ZONE_ROOM_LEAVE_CMD msg;
            msg.m_dwKey     = pUser->GetUserKey();
            msg.m_RoomKey   = GetKey();
            SendToLinkedServer(&msg, sizeof(msg));
        }break;
    }

    MSG_AW_USER_LEAVE_ZONE_SYN msg;
    msg.m_byCategory = AW_USER_STATUS;
    msg.m_byProtocol = AW_USER_LEAVE_ZONE_SYN;
    msg.dwZoneKey   = GetKey();
    msg.byZoneType  = GetType();
    msg.m_dwKey     = pUser->GetUserKey();
    pUser->SendToWorldServer(&msg, sizeof(msg));
    // NOTE: f110906.1L, see the ZoneInterface::ReadyUser
    static uint8_t debug_stream[sizeof(*this)];
    CopyMemory(debug_stream, this, sizeof(debug_stream));
    // 유저 삭제
    ASSERT(FindUser(pUser->GetPlayerKey()) != NULL);
    RemoveUser(pUser->GetPlayerKey());

    On_leaveuser(pUser);

    pUser->UnlinkServer();

    // 공통 부분
    pUser->SetZoneState(ePRS_NOT_AT_ZONE);
    pUser->SetZoneKey(0);
    pUser->SetZoneType(eZONETYPE_MAX);

    {    //    __NA000630_070817_MOLA_5th_UPDATE
        SecurePackNode4User& rSECURE = pUser->GetSecureNode();
        sSECURE_FLAG& rSECURE_FLAG = rSECURE.SECURE_FLAG();
        //ASSERT(!rSECURE_FLAG.STATUS.IsTransaction());        // 접속이 갑자기 끊기는 경우 이 ASSERT에 걸리게 될 것이다.
        rSECURE_FLAG.STATUS.TransEnter();
    }
}

// @history
//  - waverix: move to code from header
BOOL
ZoneInterface::CanShowAtRoomList(User* pUser, eZONEPUBLIC roomPublic, TCHAR* pszRoomPWD)
{
    __TOUCHED((pUser, roomPublic, pszRoomPWD));
    return true;
}

VOID
ZoneInterface::SendToLinkedServer(MSG_BASE_FORWARD* pMsg, WORD wSize)
{
    if(m_LinkedServerSession)
    {
        m_LinkedServerSession->Send((BYTE*)pMsg, wSize);
    }
    else
    {
        ASSERT(m_LinkedServerSession);
    }
}

void ZoneInterface::SendToAll(MSG_BASE* msg, WORD msg_size, DWORD except_user_key) const
{
    // CHANGES: f110608.6L, f110603.3L, this changes are to more suitable packet processing
    // in AgentServer
    if (bool need_packing = (msg_size > 120))
    {
        nsPacket::PacketInfo pack_info = nsPacket::PacketControl::Pack(msg, msg_size);
        msg = pack_info.pMsg;
        msg_size = pack_info.Size;
    };
    const bool check_skip_object = (except_user_key == 0);
    FOREACH_CONTAINER(const ZONE_USER_LIST::value_type& node, m_UserList, ZONE_USER_LIST)
    {
        User* const user = node.second;
        DWORD user_key = user->GetUserKey();
        if (!(check_skip_object || (user_key != except_user_key))) {
            continue;
        };
        user->SendPacket(msg, msg_size, false);
    };
#if SUN_CODE_BACKUP
    const BOOLEAN bKeySkip = !except_user_key;
    FOREACH_CONTAINER(const ZONE_USER_LIST::value_type& rNode, m_UserList, ZONE_USER_LIST)
    {
        User* const pUser = rNode.second;
        if (bKeySkip || pUser->GetUserKey() != except_user_key)
            pUser->SendPacket(msg, msg_size);
    }
#endif
}

VOID
ZoneInterface::On_enteruser(User* pUser)
{
    // 서버간의 이동일 경우 월드서버에 리벤지 정보를 요청한다.
    if(pUser->IsMovingBetweenServer())
    {
        MSG_AW_CHAO_REQUEST_REVENGEINFO_SYN AWMsg;
        AWMsg.m_dwCharGuid = pUser->GetSelectedCharGuid();
        pUser->SendToWorldServer(&AWMsg, sizeof(AWMsg));
    }
}

RC::eROOM_RESULT
ZoneInterface::CanJoin(User* pUser, eZONEPUBLIC roomPublic, TCHAR* pszRoomPWD)
{ 
    RC::eROOM_RESULT rc = RC::RC_ROOM_FAILED;

    rc = _CanJoinAll(pUser, roomPublic, pszRoomPWD);
    if(RC::RC_ROOM_SUCCESS != rc)
        return rc;

    rc = _CanJoinEach(pUser, roomPublic, pszRoomPWD);
    if(RC::RC_ROOM_SUCCESS != rc)
        return rc;

    return RC::RC_ROOM_SUCCESS;
}

BOOL
ZoneInterface::GetServerOptChaoStateUseZone(eZONETYPE zoneType)
{
    BOOL flag = 0;

    switch(zoneType)
    {
    case eZONETYPE_LOBBY:
    case eZONETYPE_VILLAGE:
    case eZONETYPE_MISSION:
    case eZONETYPE_HUNTING:
    case eZONETYPE_PVP:
    case eZONETYPE_CHUNTING:
        flag = ServerOptionParserEx::Instance()->GetServerOption().m_byLoobyUseConOpt;
        break;
    case eZONETYPE_INSTANCE:
        flag = ServerOptionParserEx::Instance()->GetServerOption().m_byInsDungeonUseConOpt;
        break;
    default:
        flag = FALSE;
    }

    return flag;
}

BOOL
ZoneInterface::FromZone_Check(ZONE_CHECK_TYPE checkType)
{
    if(checkType == CHECK_RETURN_PARTY)
    {
        eZONETYPE zoneType = GetType();
        if (zoneType == eZONETYPE_VILLAGE || 
            zoneType == eZONETYPE_DOMINATION_FIELD || 
            zoneType == eZONETYPE_FIELD ||
            zoneType == eZONETYPE_SPA_FIELD)
            return TRUE;
    }

    return FALSE;
}

BOOL
ZoneInterface::ToZone_Check(ZONE_CHECK_TYPE checkType)
{
    if(checkType == CHECK_RETURN_PARTY)
    {
        eZONETYPE zoneType = GetType();
        if(zoneType == eZONETYPE_MISSION ||
           zoneType == eZONETYPE_INSTANCE ||
           zoneType == eZONETYPE_HUNTING ||
           zoneType == eZONETYPE_CHUNTING)
            return TRUE;
    }

    return FALSE;
}

//--------------------------------------------------------------------------------------------------
//  @history
//      - __NA_0_20081029_PARTY_REFACTORING
//      - default adaptation
DWORD
ZoneInterface::GetLinkedServerIndex()
{
    return m_LinkedServerSession != NULL
            ?   m_LinkedServerSession->GetSessionIndex()
            :   0;
}














