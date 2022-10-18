#include "stdafx.h"

//==================================================================================================
//
#include <float.h>
//////////////////////////////////////////////////////////////////////////
#include <Struct.h>
#include <AIParamParser.h>
#include <ServerOptionParser.h>
#include <Global.h>
#include <PublicMath.h>
#include <NumericValues.h>
#include <PacketControl/WarMessages_Sync.h>
#include <ServerOptionParserEx.h>
#include <PacketStruct_GZ.h>
#include <PacketStruct_CG.h>
#include <PacketStruct_AG.h>
#include <ServerSession.h>
//////////////////////////////////////////////////////////////////////////
#include "FieldServer.h"
#include "Map.h"
#include "GameField.h"
#include "Player.h"
#include "PlayerManager.h"
#include "Handler_CG_SYNC.h"

#include "MissionManager.h"
#include "MovementChecker.h"
#include "GameZone.h"
#include "GameZoneManager.h"

#include "MoveStateControl.h"

#include "StatusManager.h"
#include "MissionRewardManager.h"
#include "TriggerManagerEx.h"
#include "QuestManager_Concrete.h"
#include <World/SSQ/SSQHandlerInGame.h>
#include "PlayerWings.h" //_NA001955_110210_WING_COSTUME
#include <PacketStruct_PointSystem.h>

Handler_CG_SYNC::Handler_CG_SYNC()
{
}

Handler_CG_SYNC::~Handler_CG_SYNC()
{
}

//--------------------------------------------------------------------------------------------------
// changes, f100819.1L
const nsPacket::PacketHandlerInfo Handler_CG_SYNC::GetPacketHandlerNodes()
{
#define HANDLER_NODE_INFO(c, p, phase) { MAKEWORD(c, p), phase, Handler_CG_SYNC::On##p }
#define HANDLER_NODE_INFO_EOR() { 0, static_cast<ePHASE_SHIFT>(0), NULL }

    static const nsPacket::PacketHandlerNode handlers_static_[] =
    {
        HANDLER_NODE_INFO(AG_SYNC,  AG_SYNC_PLAYER_ENTER_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(AG_SYNC,  AG_SYNC_PLAYER_WARP_SYN, PHASE_SERVICE),

        // CG_SYNC
        HANDLER_NODE_INFO(CG_SYNC,  CG_SYNC_MOVE_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_SYNC,  CG_SYNC_TARGET_MOVE_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_SYNC,  CG_SYNC_KBMOVE_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_SYNC,  CG_SYNC_STOP_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_SYNC,  CG_SYNC_FAST_KBMOVE_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_SYNC,  CG_SYNC_JUMP_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_SYNC,  CG_SYNC_JUMP_STOP_SYN, PHASE_SERVICE),
        // _NA001955_110210_WING_COSTUME
        HANDLER_NODE_INFO(CG_SYNC,  CG_SYNC_WINGS_CONTROL_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO_EOR() // end of records
    };
    //
    BOOST_STATIC_ASSERT(_countof(handlers_static_) >= 1);
    nsPacket::PacketHandlerInfo return_value = { handlers_static_, _countof(handlers_static_) - 1 };
    //
    return return_value;
};
//--------------------------------------------------------------------------------------------------


namespace util { namespace internal {
;
static void NotifyEnterFieldVersionFault(Player* player,
                                         const EngineFileChecksum& client_checksums,
                                         const EngineFileChecksum& server_checksums)
{
    MSG_CG_ETC_DEBUGINFO_CMD msg_debug;
    msg_debug.m_dwObjectKey = player->GetObjectKey();
    int written = _snprintf(msg_debug.m_pszChatMsg, _countof(msg_debug.m_pszChatMsg),
                            "|MSG=WMO버전이 서로 틀립니다|WmoChecksum=[%8u:%8u]|",
                            server_checksums.checksum_wmo, client_checksums.checksum_wmo);
    msg_debug.m_pszChatMsg[_countof(msg_debug.m_pszChatMsg) - 1] = '\0';
    msg_debug.m_byMsgLength = static_cast<BYTE>(written);
    player->SendPacket(&msg_debug, msg_debug.GetSize());
}

}}; //end of namespace
//==================================================================================================
// ZONE CONTROL SYNC
Handler_CG_SYNC_IMPL(AG_SYNC_PLAYER_ENTER_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    const MSG_AG_SYNC_PLAYER_ENTER_SYN* pRecvMsg = static_cast<MSG_AG_SYNC_PLAYER_ENTER_SYN*>(pMsg);

#ifdef __NA_PACKETFLOW_TEST
    SUNLOG(eFULL_LOG, "Handler_CG_SYNC(AG_SYNC_PLAYER_ENTER_SYN)");
#endif

    Player* pPlayer = PlayerManager::Instance()->FindPlayerByUserKey(pRecvMsg->m_dwKey);

    FlowControl flow;
    FLOWCONTROL_START()
    {
        // start LOGIC_BLOCK
        if(FALSE == flow.FCAssert(pPlayer))
        {
            SetFailFc(pPlayer, RC::RC_ZONE_FAILED);
            break;
        }

        // 가상 클라이언트처럼 너무 빨리 들어오진 않았는지 체크
        util::SimpleTimer& loadingTimer = pPlayer->GetLoadingCheckTimer();
        if(loadingTimer.GetExpireTime() && !loadingTimer.IsExpired())
        {
            const SERVER_COMMON_OPTION& option = ServerOptionParserEx::Instance()->GetServerOption();
            SECURELOG(SECURELOG_PLAYERINFO(pPlayer, eABUSE_LOGIC_VIOLATION,
                                           eABUSE_SUB_LOGIC_VIOLATION_LOADING_TIME_SHORT),
                                           "[OnAG_SYNC_PLAYER_ENTER_SYN] Loading Time(%d) is Short!",
                      option.m_wABUSE_MIN_CLIENT_LOADING_TIME - loadingTimer.GetRemainedTime());
        }
        loadingTimer.Clear();

        GameZone* pEnterZone = NULL;
        MAPCODE   fieldCode = 0;  
        WzID      areaID = 0;
        WzVector  vStartPos;

        const eZONE_STATE eRoom_State =
            pPlayer->GetGameZone(pEnterZone, fieldCode, &areaID, &vStartPos);
        IfFailThenBreak(eRoom_State != ePRS_NOT_AT_ZONE, RC::RC_ZONE_FAILED);

        const KEYTYPE roomKey = pEnterZone->GetKey();
        pEnterZone = g_GameZoneManager.GetZone(roomKey);
        IfFailThen(pEnterZone, RC::RC_ZONE_CANT_FIND_ZONE)
        {
            SUNLOG(eCRITICAL_LOG,
                   "["__FUNCTION__"] [U:%d][Room:%d] 들어 갈려는 맵이 없다.",
                   pPlayer->GetUserKey(), roomKey);
            break;
        }

        // 이 부분은 절대로 주석처리 하지 말 것!!!(주의요망!!)
        // (f100623.1L) change an interface to support a multi-checksum checker
        const EngineFileChecksum& client_checksums = pRecvMsg->checksum_list_;
        EngineFileChecksum checksums = { 0, };
        if (g_GameZoneManager.FieldVersionCheck(roomKey, fieldCode,
                &checksums, client_checksums) == false)
        {
            SUNLOG(eCRITICAL_LOG,
                   _T("|ID=%s|Msg=C/S Map(%d) Version is different|")
                   _T("|FieldCheckSum=[%8u:%8u]|WmoChecksum=[%8u:%8u]|"),
                   pPlayer->GetUserID(), fieldCode,
                   checksums.checksum_map, client_checksums.checksum_map,
                   checksums.checksum_wmo, client_checksums.checksum_wmo);
            const RC::eZONE_RESULT selected_result
                = checksums.checksum_map != client_checksums.checksum_map
                ?   RC::RC_ZONE_NOT_MATCHED_MAP_VERSION
                :   RC::RC_ZONE_NOT_MATCHED_WMO_VERSION;
            // f100916.2L, to support the agent server dummy client
            if (pRecvMsg->is_dummy_client_test_ != 1) {
                SetFailFc(FieldVersionCheck, selected_result);
                break;
            }
        }

        // 미션 창 뛰워라라는 명령 패킷
        // <NOTE> 미션 방에서 이 조건은 NAK처리를 하는 것으로 하자.
        BOOLEAN is_reward_window_open = !!pPlayer->GetMissionManager()->EnableWindowOpen();

        if(is_reward_window_open)
        {
            IfFailThenBreak(TriggerManagerEx::Check_RewardWindow(pPlayer),
                            RC::RC_ZONE_ASSERTION_FLOW_VIOLATION);
        }

        IfFailThenBreak(
            g_GameZoneManager.JoinPlayer(roomKey, pPlayer, fieldCode, areaID, &vStartPos),
            RC::RC_ZONE_MAP_PROBLEM);

        // 누적 경험치가 기존 저장된 값하고 다르게 계산되어야 하는 경우 때문에 여기서 체크함!! 
        pPlayer->FixLevel();

        if(is_reward_window_open)
            TriggerManagerEx::Process_RewardWindow(pPlayer);

    #if WAVERIX_CODE_BACKUP
        if(FIELD_SERVER == g_pGameServer->GetServerType())
        {   // SSQ(타락한 사원) 보상/환불 정보 유무에 따른 처리
            ISSQHandlerInGame& rHandler = pPlayer->SSQHandler;
            if(rHandler.IsValidState())
                rHandler->Send_RewardProcess(pPlayer);
        };
    #endif

        GUILDGUID guild_guid = pPlayer->GetGuildGuid();
        if(guild_guid != 0)
        {
            // 길드 서버로 로그인 패킷을 보낸다. 음... 포탈 탈때마다 길드로 이 패킷 보내게 되는데... 나중에 다른 방법 고려해 봐야 겠다.
            MSG_GZ_GUILD_LOGIN_CHAR_SYN msgGzSYN;
            msgGzSYN.m_GuildGuid  = guild_guid;
            msgGzSYN.m_CharGuid   = pPlayer->GetCharGuid();
            if(pPlayer->SendToGuildServer(&msgGzSYN, sizeof(msgGzSYN)) == 0)
            {
                SUNLOG(eCRITICAL_LOG,
                       "["__FUNCTION__"] Fail, Send MSG_GZ_GUILD_LOGIN_CHAR_SYN, "
                       "GuildGuid = %u, CharGuid = %u",
                       guild_guid, pPlayer->GetCharGuid());
            }
        }
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
        else
        {
            // 길드 가입요청 시스템에 의해 가입된 경우 캐릭터 선택창에서 캐릭터 정보를 다시 로드하지 않는 문제가 있다.
            // 이와 같은 상황에 길드 정보가 제대로 전달되지 않으므로 DBP에 패킷을 보내 길드에 가입된 캐릭터 인지 확인한다.
            MSG_GZ_GUILD_RELOAD_SYN sync_msg;
            sync_msg.m_dwKey = pPlayer->GetUserKey();
            sync_msg.char_guid = pPlayer->GetCharGuid();
            pPlayer->SendToGuildServer(&sync_msg, sizeof(sync_msg));
        }
#endif //_NA_007330_20140620_GUILD_SYSTEM_EXTENSION
        //__NA001277_090205_NAVI_SYSTEM_SUPPORT__
        MSG_CG_QUEST_OBJECT_NAVI_POS_CMD msgNavi;
        if(pPlayer->GetQuestManager()->MakeInfoQuestNpcNaviStream(&msgNavi))
            pPlayer->SendPacket(&msgNavi, msgNavi.GetSize());
        //
        if (client_checksums.pass_result == client_checksums.Passed_But_NotMatchedWmo) {
            util::internal::NotifyEnterFieldVersionFault(pPlayer, client_checksums, checksums);
        };

        return /*RETURN TRUE VALUE*/;
    }
    FLOWCONTROL_END;

    MSG_CG_SYNC_PLAYER_ENTER_NAK msgNAK;
    switch(flow.GetErrorCode())
    {
    case RC::RC_ZONE_FAILED:
    case RC::RC_ZONE_ASSERTION_FLOW_VIOLATION:
        msgNAK.m_dwErrorCode = RC::RC_ZONE_FAILED;
        break;
    case RC::RC_ZONE_CANT_FIND_ZONE:
    case RC::RC_ZONE_NOT_MATCHED_MAP_VERSION:
    case RC::RC_ZONE_NOT_MATCHED_WMO_VERSION: // added by _NA000000_100623_CHECK_WMO_CHECKSUM_
    case RC::RC_ZONE_MAP_PROBLEM:
        msgNAK.m_dwErrorCode = flow.GetErrorCode();
        break;
    default:
        flow.FCAssert("Unexpected flow");
        return;
    }

    if(pPlayer)
        pPlayer->SendPacket(&msgNAK, sizeof(msgNAK));

    return /*RETURN FALSE VALUE*/;
}

//==================================================================================================
// ZONE CONTROL SYNC
Handler_CG_SYNC_IMPL(AG_SYNC_PLAYER_WARP_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    const MSG_AG_SYNC_PLAYER_WARP_SYN* pRecvMsg = static_cast<MSG_AG_SYNC_PLAYER_WARP_SYN *>(pMsg);

#ifdef __NA_PACKETFLOW_TEST
    SUNLOG(eFULL_LOG, "Handler_CG_SYNC(AG_SYNC_PLAYER_WARP_SYN)");
#endif

    Player* pPlayer = PlayerManager::Instance()->FindPlayerByUserKey(pRecvMsg->m_dwKey);

    FlowControl flow;
    FLOWCONTROL_START()
    {
        // start LOGIC_BLOCK
        if(FALSE == flow.FCAssert(pPlayer))
        {
            SetFailFc(pPlayer, RC::RC_ZONE_FAILED);
            break;
        }

        GameZone* pEnterZone = NULL;
        MAPCODE fieldCode = 0;
        WzID areaID = 0;
        const eZONE_STATE eRoom_State = pPlayer->GetGameZone(pEnterZone, fieldCode, &areaID);

        IfFailThenBreak(ePRS_NOT_AT_ZONE != eRoom_State, RC::RC_ZONE_FAILED);

        const KEYTYPE roomKey = pEnterZone->GetKey();
        IfFailThenBreak(pEnterZone = g_GameZoneManager.GetZone(roomKey), RC::RC_ZONE_CANT_FIND_ZONE);

        // 이 부분은 절대로 주석처리 하지 말 것!!!(주의요망!!)
        // (f100623.1L) change an interface to support a multi-checksum checker
        const EngineFileChecksum& client_checksums = pRecvMsg->checksum_list_;
        EngineFileChecksum checksums = { 0, };
        if (g_GameZoneManager.FieldVersionCheck(roomKey, fieldCode,
                &checksums, client_checksums) == false)
        {
            SUNLOG(eCRITICAL_LOG,
                   _T("|ID=%s|Msg=C/S Map(%d) Version is different|")
                   _T("|FieldCheckSum=[%8u:%8u]|WmoChecksum=[%8u:%8u]|"),
                   pPlayer->GetUserID(), fieldCode,
                   checksums.checksum_map, client_checksums.checksum_map,
                   checksums.checksum_wmo, client_checksums.checksum_wmo);
            const RC::eZONE_RESULT selected_result
                = checksums.checksum_map != client_checksums.checksum_map
                ?   RC::RC_ZONE_NOT_MATCHED_MAP_VERSION
                :   RC::RC_ZONE_NOT_MATCHED_WMO_VERSION;
            // f100916.2L, to support the agent server dummy client
            if (pRecvMsg->is_dummy_client_test_ != 1) {
                SetFailFc(FieldVersionCheck, selected_result);
                break;
            }
        }

        // 미션 창 뛰워라라는 명령 패킷
        // <NOTE> 미션 방에서 이 조건은 NAK처리를 하는 것으로 하자.
        BOOLEAN is_reward_window_open = !!pPlayer->GetMissionManager()->EnableWindowOpen();

        if(is_reward_window_open)
        {
            IfFailThenBreak(TriggerManagerEx::Check_RewardWindow(pPlayer),
                            RC::RC_ZONE_ASSERTION_FLOW_VIOLATION);
        }

        g_GameZoneManager.WarpPlayer(roomKey, pPlayer, fieldCode, areaID);

        if(is_reward_window_open)
            TriggerManagerEx::Process_RewardWindow(pPlayer);

        if (client_checksums.pass_result == client_checksums.Passed_But_NotMatchedWmo) {
            util::internal::NotifyEnterFieldVersionFault(pPlayer, client_checksums, checksums);
        };

        return /*RETURN TRUE VALUE*/;
    }
    FLOWCONTROL_END;

    MSG_CG_SYNC_PLAYER_ENTER_NAK msgNAK;
    switch(flow.GetErrorCode())
    {
    case RC::RC_ZONE_FAILED:
    case RC::RC_ZONE_ASSERTION_FLOW_VIOLATION:
        msgNAK.m_dwErrorCode = RC::RC_ZONE_FAILED;
        break;
    case RC::RC_ZONE_CANT_FIND_ZONE:
    case RC::RC_ZONE_NOT_MATCHED_MAP_VERSION:
    case RC::RC_ZONE_NOT_MATCHED_WMO_VERSION: // added by _NA000000_100623_CHECK_WMO_CHECKSUM_
    case RC::RC_ZONE_MAP_PROBLEM:
        msgNAK.m_dwErrorCode = flow.GetErrorCode();
        break;
    default:
        flow.FCAssert("Unexpected flow");
        return;
    }

    if(pPlayer)
        pPlayer->SendPacket(&msgNAK, sizeof(msgNAK));

    return /*RETURN FALSE VALUE*/;
}


//==================================================================================================
// 이동, 점프 등등 모든 이동관련 Common Functions
//
VOID
Handler_CG_SYNC::Send_SYNC_MOVE_NAK(Player* pPlayer,
                                    RC::eSYNC_RESULT error_code, DWORD IN error_level,
                                    BOOL bThrustMove)
{
    // Common Block
    MSG_CG_SYNC_MOVE_NAK msgNAK;
    msgNAK.m_wvCurPos = *pPlayer->GetPosPtr();
    msgNAK.m_byErrorCode = error_code;
    pPlayer->SendPacket(&msgNAK, sizeof(msgNAK));

    // 플레이어를 정지시킨다.
    pPlayer->StopMoving();

    // Pre-process filter
    switch(RC::eSYNC_RESULT skip_filter = error_code)
    {
    case RC::RC_SYNC_ANIMATION_DELAY_PROCESSING:
        return;
    }

    switch(error_code)
    {
    case RC::RC_SYNC_FAILED:
        break;
    case RC::RC_SYNC_SUCCESS:
    case RC::RC_SYNC_PLAYER_IS_DEAD:                    // 플레이어가 죽어서 이동실패
    case RC::RC_SYNC_FIELD_IS_NULL:                     // 플레이어의 필드값이 NULL이다.
    case RC::RC_SYNC_PLAYER_IS_UNMOVABLE_STATE:         // 플레이어가 이동할 수 없는 상태이다.
    case RC::RC_SYNC_SETPATH_ERROR:
    case RC::RC_SYNC_ROUTE_SIZE_EXCESS:                 // 경로 개수 최대값 초과
    case RC::RC_SYNC_INVALID_MOVESATE:                  // 이동상태 이상(걷기: 달리기)
    case RC::RC_SYNC_CUR_POSITION_IS_INVALID:           // 클라가 보내준 현재좌표가 유효하지 않다.
    case RC::RC_SYNC_DEST_POSITION_IS_INVALID:          // 목적지 좌표가 유효하지 않다.
    case RC::RC_SYNC_JUMP_LENGTH_TOO_LONG:              // 점프거리가 너무 멀다
    case RC::RC_SYNC_CUR_POSITION_IS_INVALID_NO_HACK:   // 클라가 보내준 현재좌표가 유효하지 않다. 맵의 경계선에서 클라가 조금 틀린 좌표를 보낼 수 있다.
        {
            // 주변에 정지를 알린다.
            MSGSUB_SYNC_STOP_BRD msg;
            msg.m_dwObjectKey = pPlayer->GetObjectKey();
            msg.m_wvCurPos = msgNAK.m_wvCurPos;
            pPlayer->SendPacketAroundForSync(&msg);
        }
        break;
    default:
        break;
    }

    {   // Post Block
        MovementChecker& rMoveChecker = *pPlayer->GetMovementChecker();
        rMoveChecker.AbusingNotify(error_code, error_level, .0f);
    }
}


Handler_CG_SYNC_IMPL(CG_SYNC_MOVE_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    MSG_CG_SYNC_MOVE_SYN* pRecvMsg = static_cast<MSG_CG_SYNC_MOVE_SYN*>(pMsg);
    Player* pPlayer = PlayerManager::Instance()->FindPlayerByUserKey(pRecvMsg->m_dwKey);
    if (pPlayer == NULL || pPlayer->GetField() == NULL) {
        return; //< 플레이어 세션이 끊어졌을 때
    };

    BOOLEAN is_valid_size = pRecvMsg->GetPacketSize() == wSize;
    if(is_valid_size == 0)
    {
        SUNLOG(eDEV_LOG, _T("[RNDPROTOCOL_REPORT][SIZE] "__FUNCTION__""));
        return;
    }
    pRecvMsg->Decode();

    DWORD error_level = 0;
    MovementChecker& rMoveChecker = *pPlayer->GetMovementChecker();
    RC::eSYNC_RESULT rcResult = rMoveChecker.OnRecvCG_SYNC_MOVE_SYN(pRecvMsg, error_level);

    if(rcResult != RC::RC_SYNC_SUCCESS)
        Send_SYNC_MOVE_NAK(pPlayer, rcResult, error_level, TRUE); 
}

Handler_CG_SYNC_IMPL(CG_SYNC_TARGET_MOVE_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    MSG_CG_SYNC_TARGET_MOVE_SYN* pRecvMsg = (MSG_CG_SYNC_TARGET_MOVE_SYN*)pMsg;
    Player* pPlayer = PlayerManager::Instance()->FindPlayerByUserKey(pRecvMsg->m_dwKey);
    if(pPlayer == 0)
        return; //< 플레이어 세션이 끊어졌을 때

    if(!pPlayer->GetField())
    {
        SUNLOG(eCRITICAL_LOG, "[Handler_CG_SYNC_IMPL(CG_SYNC_TARGET_MOVE_SYN)] Not Exist GameField");
        return;
    }

    BOOLEAN is_valid_size = pRecvMsg->GetPacketSize() == wSize;
    if(is_valid_size == 0)
    {
        SUNLOG(eDEV_LOG, _T("[RNDPROTOCOL_REPORT][SIZE] "__FUNCTION__""));
        return;
    }

    DWORD error_level = 0;
    MovementChecker& rMoveChecker = *pPlayer->GetMovementChecker();
    RC::eSYNC_RESULT rcResult = rMoveChecker.OnRecvCG_SYNC_TARGET_MOVE_SYN(pRecvMsg, error_level);

    if(rcResult != RC::RC_SYNC_SUCCESS)
        Send_SYNC_MOVE_NAK(pPlayer, rcResult, error_level, TRUE); 
}



Handler_CG_SYNC_IMPL(CG_SYNC_KBMOVE_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    Player* const pPlayer = PlayerManager::Instance()->FindPlayerByUserKey(pMsg->m_dwKey);
    GameField* const game_field = (pPlayer == NULL) ? NULL : pPlayer->GetField();
    if (pPlayer == NULL || game_field == NULL) {
        return;
    };

    MSG_CG_SYNC_KBMOVE_SYN* pRecvMsg = (MSG_CG_SYNC_KBMOVE_SYN*)pMsg;
    BOOLEAN is_valid_size = sizeof(*pRecvMsg) == wSize;
    if(is_valid_size == 0)
    {
        SUNLOG(eDEV_LOG, _T("[RNDPROTOCOL_REPORT][SIZE] "__FUNCTION__""));
        return;
    };
    uint8_t copied_stream[sizeof(*pRecvMsg) << 1];
    CopyMemory(copied_stream, pRecvMsg, sizeof(*pRecvMsg));
    pRecvMsg = reinterpret_cast<MSG_CG_SYNC_KBMOVE_SYN*>(copied_stream);
    pRecvMsg->Decode();

#ifdef _NA_0_20110803_PLAYER_MOVEMENT_SYNC
    pPlayer->GetMovementChecker()->KeyBoard_Move(pRecvMsg);
#else
    DWORD error_level = 0;
    MovementChecker& rMoveChecker = *pPlayer->GetMovementChecker();
    RC::eSYNC_RESULT rcResult = rMoveChecker.OnRecvCG_SYNC_KBMOVE_SYN(pRecvMsg, error_level);

    if(rcResult != RC::RC_SYNC_SUCCESS)
        Send_SYNC_MOVE_NAK(pPlayer, rcResult, error_level, TRUE); 
    else
    {   // (NOTE) to support a shadow control, __NA_S00015_20080828_NEW_HOPE_SHADOW
        // 현재 유저가 SUCTION 스킬 사용중이였으면 STOP 시킨다.
        BaseStatus* pStatus = NULL;
        pStatus = pPlayer->GetStatusManager()->FindStatus(eCHAR_STATE_HP_SUCTION);
        if(pStatus)     pStatus->StopStatus();

        pStatus = pPlayer->GetStatusManager()->FindStatus(eCHAR_STATE_MP_SUCTION);
        if(pStatus)     pStatus->StopStatus();
    }
#endif
}

// 플레이어 정지
Handler_CG_SYNC_IMPL(CG_SYNC_STOP_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    MSG_CG_SYNC_STOP_SYN* pRecvMsg = static_cast<MSG_CG_SYNC_STOP_SYN*>(pMsg);

    Player* pPlayer = PlayerManager::Instance()->FindPlayerByUserKey(pRecvMsg->m_dwKey);
    //ASSERT(pPlayer);
    if(pPlayer == 0)
        return;   //< 플레이어 세션이 끊어졌을 때

    if(!pPlayer->GetField())
    {
        SUNLOG(eCRITICAL_LOG, "[Handler_CG_SYNC_IMPL(CG_SYNC_STOP_SYN)] Not Exist GameField");
        return;
    }

#ifdef _NA_0_20110803_PLAYER_MOVEMENT_SYNC
    pPlayer->GetMovementChecker()->Move_Stop(pRecvMsg);
#else
    DWORD error_level = 0;
    MovementChecker& rMoveChecker = *pPlayer->GetMovementChecker();
    RC::eSYNC_RESULT rcResult = rMoveChecker.OnRecvCG_SYNC_STOP_SYN(pRecvMsg, error_level);

    if(rcResult != RC::RC_SYNC_SUCCESS)
        Send_SYNC_MOVE_NAK(pPlayer, rcResult, error_level, TRUE);
#endif
}

Handler_CG_SYNC_IMPL(CG_SYNC_FAST_KBMOVE_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    Player* const pPlayer = PlayerManager::Instance()->FindPlayerByUserKey(pMsg->m_dwKey);
    GameField* const game_field = (pPlayer == NULL) ? NULL : pPlayer->GetField();
    if (pPlayer == NULL || game_field == NULL) {
        return;
    };
    MSG_CG_SYNC_FAST_KBMOVE_SYN* pRecvMsg = static_cast<MSG_CG_SYNC_FAST_KBMOVE_SYN*>(pMsg);
    if (sizeof(*pRecvMsg) != wSize)
    {
        SUNLOG(eDEV_LOG, _T("[RNDPROTOCOL_REPORT][SIZE] "__FUNCTION__""));
        return;
    };
#ifdef _NA000000_SPEED_HACK_PREVENTION_SUPPORT
    uint8_t copied_stream[sizeof(*pRecvMsg) << 1];
    CopyMemory(copied_stream, pRecvMsg, sizeof(*pRecvMsg));
    pRecvMsg = reinterpret_cast<MSG_CG_SYNC_FAST_KBMOVE_SYN*>(copied_stream);
    pRecvMsg->Decode();
#endif
    DWORD error_level = 0;
    MovementChecker& rMoveChecker = *pPlayer->GetMovementChecker();
    RC::eSYNC_RESULT rcResult = rMoveChecker.OnRecvCG_SYNC_FAST_KBMOVE_SYN(pRecvMsg, error_level);

    if(rcResult != RC::RC_SYNC_SUCCESS)
        Send_SYNC_MOVE_NAK(pPlayer, rcResult, error_level, TRUE); 
}


Handler_CG_SYNC_IMPL(CG_SYNC_JUMP_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    MSG_CG_SYNC_JUMP_SYN* pRecvMsg = (MSG_CG_SYNC_JUMP_SYN*)pMsg;
    PlayerManager* pPlayerManager = PlayerManager::Instance();

    //////////////////////////////////////////////////////////////////////////
    // <PATTERN>
    Player* pPlayer = pPlayerManager->FindPlayerByUserKey(pRecvMsg->m_dwKey);
    if(!pPlayer || (sizeof(*pRecvMsg) != wSize))
        return;

    if(!pPlayer->GetField())
    {
        SUNLOG(eCRITICAL_LOG, "[Handler_CG_SYNC_IMPL(CG_SYNC_JUMP_SYN)] Not Exist GameField");
        return;
    }

    DWORD error_level = 0;
    MovementChecker& rMoveChecker = *pPlayer->GetMovementChecker();
    RC::eSYNC_RESULT rcResult = rMoveChecker.OnRecvCG_SYNC_JUMP_SYN(pRecvMsg, error_level);

    if(rcResult != RC::RC_SYNC_SUCCESS)
    {
        MSG_CG_SYNC_JUMP_NAK msgNAK;
        msgNAK.m_wvCurPos = *pPlayer->GetPosPtr();
        pPlayer->SendPacket(&msgNAK, sizeof(msgNAK));

        rMoveChecker.AbusingNotify(rcResult, 0, .0f);
    }
}

Handler_CG_SYNC_IMPL(CG_SYNC_JUMP_STOP_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    MSG_CG_SYNC_JUMP_STOP_SYN* pRecvMsg = (MSG_CG_SYNC_JUMP_STOP_SYN *)pMsg;

    Player* pPlayer = PlayerManager::Instance()->FindPlayerByUserKey(pRecvMsg->m_dwKey);
    if(pPlayer == 0)
        return ;

    if(!pPlayer->GetField())
    {
        SUNLOG(eCRITICAL_LOG, "[Handler_CG_SYNC_IMPL(CG_SYNC_JUMP_STOP_SYN)] Not Exist GameField");
        return;
    }

    DWORD error_level = 0;
    MovementChecker& rMoveChecker = *pPlayer->GetMovementChecker();
    RC::eSYNC_RESULT rcResult = rMoveChecker.OnRecvCG_SYNC_JUMP_STOP_SYN(pRecvMsg, error_level);

    if(rcResult != RC::RC_SYNC_SUCCESS)
    {
        MSG_CG_SYNC_JUMP_STOP_NAK msgNAK;
        msgNAK.m_byErrorCode = rcResult;
        pPlayer->SendPacket(&msgNAK, sizeof(msgNAK));

        rMoveChecker.AbusingNotify(rcResult, error_level, .0f);
    }
}

//
//==================================================================================================

// CHANGES: f110315.2L, _NA001955_110210_WING_COSTUME
Handler_CG_SYNC_IMPL(CG_SYNC_WINGS_CONTROL_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));
#if !defined(_NA001955_110210_WING_COSTUME)
    ASSERT(!"Currently not supported");
    return;
#endif

#ifdef _NA001955_110210_WING_COSTUME
    //
    using ns_object::PlayerWings;
    //
    const MSG_CG_SYNC_WINGS_CONTROL_SYN* recv_msg = \
        static_cast<MSG_CG_SYNC_WINGS_CONTROL_SYN*>(pMsg);
    if (sizeof(*recv_msg) != wSize) { // pass
        assert(sizeof(*recv_msg) != wSize);
        return;
    }
    //
    const DWORD user_key = recv_msg->m_dwKey;
    Player* const player = PlayerManager::Instance()->FindPlayerByUserKey(user_key);
    if (player == NULL || player->GetField() == NULL) { // low level
        return;
    }
    //
    MSG_CG_SYNC_WINGS_CONTROL_CMD msg_brd;
    const WingControl& header = recv_msg->header;
    bool success = (header.request < header.eReq_Counts);
    if (success == false)
    {
        msg_brd.header = header;
        msg_brd.data.Clear();
        player->SendPacket(&msg_brd, msg_brd.GetSize());
        return;
    }
    //
    assert(success != 0);
    //
    IPlayerWingsD& wings = player->Wings;
    PlayerWings::eResult notify_result = PlayerWings::eResult_Fail;
    //
    switch (header.request)
    {
    case header.eReq_SpreadWings:
        success = PlayerWings::SpreadWings(player, header,
            &msg_brd.header, &msg_brd.data, &notify_result);
        break;
    case header.eReq_FurlWings:
        success = PlayerWings::FurlWings(player, header,
            &msg_brd.header, &msg_brd.data, &notify_result);
        break;
    default:
        success = false;
        if (WingControl* const result_header = &msg_brd.header)
        {
            result_header->Clear();
            msg_brd.data.Clear();
            result_header->request = header.request;
            result_header->args = header.args;
            result_header->result = header.eRet_Failed;
            result_header->result_code = header.eRetCode_Unknown;
        };
        break;
    }
    //
    if (notify_result != ns_object::PlayerWings::eResult_1toN) {
        player->SendPacket(&msg_brd, msg_brd.GetSize());
    }
    else {
        player->SendPacketAround(&msg_brd, msg_brd.GetSize());
    }
#endif
}

//==================================================================================================
