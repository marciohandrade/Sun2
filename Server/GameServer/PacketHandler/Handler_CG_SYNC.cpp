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
                            "|MSG=WMO������ ���� Ʋ���ϴ�|WmoChecksum=[%8u:%8u]|",
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

        // ���� Ŭ���̾�Ʈó�� �ʹ� ���� ������ �ʾҴ��� üũ
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
                   "["__FUNCTION__"] [U:%d][Room:%d] ��� ������ ���� ����.",
                   pPlayer->GetUserKey(), roomKey);
            break;
        }

        // �� �κ��� ����� �ּ�ó�� ���� �� ��!!!(���ǿ��!!)
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

        // �̼� â �ٿ����� ��� ��Ŷ
        // <NOTE> �̼� �濡�� �� ������ NAKó���� �ϴ� ������ ����.
        BOOLEAN is_reward_window_open = !!pPlayer->GetMissionManager()->EnableWindowOpen();

        if(is_reward_window_open)
        {
            IfFailThenBreak(TriggerManagerEx::Check_RewardWindow(pPlayer),
                            RC::RC_ZONE_ASSERTION_FLOW_VIOLATION);
        }

        IfFailThenBreak(
            g_GameZoneManager.JoinPlayer(roomKey, pPlayer, fieldCode, areaID, &vStartPos),
            RC::RC_ZONE_MAP_PROBLEM);

        // ���� ����ġ�� ���� ����� ���ϰ� �ٸ��� ���Ǿ�� �ϴ� ��� ������ ���⼭ üũ��!! 
        pPlayer->FixLevel();

        if(is_reward_window_open)
            TriggerManagerEx::Process_RewardWindow(pPlayer);

    #if WAVERIX_CODE_BACKUP
        if(FIELD_SERVER == g_pGameServer->GetServerType())
        {   // SSQ(Ÿ���� ���) ����/ȯ�� ���� ������ ���� ó��
            ISSQHandlerInGame& rHandler = pPlayer->SSQHandler;
            if(rHandler.IsValidState())
                rHandler->Send_RewardProcess(pPlayer);
        };
    #endif

        GUILDGUID guild_guid = pPlayer->GetGuildGuid();
        if(guild_guid != 0)
        {
            // ��� ������ �α��� ��Ŷ�� ������. ��... ��Ż Ż������ ���� �� ��Ŷ ������ �Ǵµ�... ���߿� �ٸ� ��� ����� ���� �ڴ�.
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
            // ��� ���Կ�û �ý��ۿ� ���� ���Ե� ��� ĳ���� ����â���� ĳ���� ������ �ٽ� �ε����� �ʴ� ������ �ִ�.
            // �̿� ���� ��Ȳ�� ��� ������ ����� ���޵��� �����Ƿ� DBP�� ��Ŷ�� ���� ��忡 ���Ե� ĳ���� ���� Ȯ���Ѵ�.
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

        // �� �κ��� ����� �ּ�ó�� ���� �� ��!!!(���ǿ��!!)
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

        // �̼� â �ٿ����� ��� ��Ŷ
        // <NOTE> �̼� �濡�� �� ������ NAKó���� �ϴ� ������ ����.
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
// �̵�, ���� ��� ��� �̵����� Common Functions
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

    // �÷��̾ ������Ų��.
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
    case RC::RC_SYNC_PLAYER_IS_DEAD:                    // �÷��̾ �׾ �̵�����
    case RC::RC_SYNC_FIELD_IS_NULL:                     // �÷��̾��� �ʵ尪�� NULL�̴�.
    case RC::RC_SYNC_PLAYER_IS_UNMOVABLE_STATE:         // �÷��̾ �̵��� �� ���� �����̴�.
    case RC::RC_SYNC_SETPATH_ERROR:
    case RC::RC_SYNC_ROUTE_SIZE_EXCESS:                 // ��� ���� �ִ밪 �ʰ�
    case RC::RC_SYNC_INVALID_MOVESATE:                  // �̵����� �̻�(�ȱ�: �޸���)
    case RC::RC_SYNC_CUR_POSITION_IS_INVALID:           // Ŭ�� ������ ������ǥ�� ��ȿ���� �ʴ�.
    case RC::RC_SYNC_DEST_POSITION_IS_INVALID:          // ������ ��ǥ�� ��ȿ���� �ʴ�.
    case RC::RC_SYNC_JUMP_LENGTH_TOO_LONG:              // �����Ÿ��� �ʹ� �ִ�
    case RC::RC_SYNC_CUR_POSITION_IS_INVALID_NO_HACK:   // Ŭ�� ������ ������ǥ�� ��ȿ���� �ʴ�. ���� ��輱���� Ŭ�� ���� Ʋ�� ��ǥ�� ���� �� �ִ�.
        {
            // �ֺ��� ������ �˸���.
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
        return; //< �÷��̾� ������ �������� ��
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
        return; //< �÷��̾� ������ �������� ��

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
        // ���� ������ SUCTION ��ų ������̿����� STOP ��Ų��.
        BaseStatus* pStatus = NULL;
        pStatus = pPlayer->GetStatusManager()->FindStatus(eCHAR_STATE_HP_SUCTION);
        if(pStatus)     pStatus->StopStatus();

        pStatus = pPlayer->GetStatusManager()->FindStatus(eCHAR_STATE_MP_SUCTION);
        if(pStatus)     pStatus->StopStatus();
    }
#endif
}

// �÷��̾� ����
Handler_CG_SYNC_IMPL(CG_SYNC_STOP_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    MSG_CG_SYNC_STOP_SYN* pRecvMsg = static_cast<MSG_CG_SYNC_STOP_SYN*>(pMsg);

    Player* pPlayer = PlayerManager::Instance()->FindPlayerByUserKey(pRecvMsg->m_dwKey);
    //ASSERT(pPlayer);
    if(pPlayer == 0)
        return;   //< �÷��̾� ������ �������� ��

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
