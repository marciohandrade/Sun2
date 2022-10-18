#include "stdafx.h"
#include ".\Handler_DG_CONNECTION.h"

#include <PacketStruct_DG.h>

#include "ObjectFactory.h"
#include "GameZoneManager.h"
#include "GameZone.h"

#include "PacketStruct_PointSystem.h"

//==================================================================================================
// changes, f100819.1L
const nsPacket::PacketHandlerInfo Handler_DG_CONNECTION::GetPacketHandlerNodes()
{
#define HANDLER_NODE_INFO(c, p, phase) { MAKEWORD(c, p), phase, Handler_DG_CONNECTION::On##p }
#define HANDLER_NODE_INFO_EOR() { 0, static_cast<ePHASE_SHIFT>(0), NULL }

    static const nsPacket::PacketHandlerNode handlers_static_[] =
    {
        HANDLER_NODE_INFO(DG_CONNECTION, DG_CONNECTION_GROUND_CMD, PHASE_SERVICE),
        HANDLER_NODE_INFO(DG_CHARINFO, DG_CHARINFO_CONNECT_CHAR_CMD, PHASE_SERVICE),
        HANDLER_NODE_INFO_EOR() // end of records
    };
    //
    BOOST_STATIC_ASSERT(_countof(handlers_static_) >= 1);
    nsPacket::PacketHandlerInfo return_value = { handlers_static_, _countof(handlers_static_) - 1 };
    //
    return return_value;
};

//==================================================================================================
//==================================================================================================

Handler_DG_CONNECTION_IMPL(DG_CONNECTION_GROUND_CMD)
{
    __UNUSED((server_session, msg, msg_size));
}

// NOTE: f110426.2L, added to prevent connectless db user problems.
//  MSG_AG_ZONE_ROOM_JOIN_SYN(A->G) ->
//  DG_CHARINFO_CONNECT_CHAR_CMD(G->D) ->
//  DG_CHARINFO_CONNECT_CHAR_CMD(D->G) ->
Handler_DG_CONNECTION_IMPL(DG_CHARINFO_CONNECT_CHAR_CMD)
{
    const MSG_DG_CHARINFO_CONNECT_CHAR_CMD* base_msg =
        static_cast<MSG_DG_CHARINFO_CONNECT_CHAR_CMD*>(msg);
    const MSG_AG_ZONE_ROOM_JOIN_SYN* recv_msg = &base_msg->forward_msg;
    PlayerManager* const player_manager = PlayerManager::Instance();
    const DWORD user_key = recv_msg->m_dwKey;
    Player* const check_player = player_manager->FindPlayerByUserKey(user_key);
    if (FlowControl::FCAssert(check_player == NULL) == false)
    {
        SUNLOG(eCRITICAL_LOG, "|[%s]|Player already exist|%d, %s|",
               __FUNCTION__, user_key, check_player->GetCharName());
        return;
    }
    struct RollbackConnectStatus {
        RollbackConnectStatus(const MSG_AG_ZONE_ROOM_JOIN_SYN* recv_msg,
                              ServerSession* server_session)
            : recv_msg_(recv_msg), server_session_(server_session), rollback_(true)
        {}
        ~RollbackConnectStatus()
        {
            if (rollback_ == false) {
                return;
            };
            //
            MSG_DG_CHARINFO_DISCONNECT_CMD msg_cmd;
            msg_cmd.m_dwKey = recv_msg_->m_dwKey;
            msg_cmd.m_ChannelID = g_pGameServer->GetKey().GetChannelID();
            server_session_->SendPacket(&msg_cmd, sizeof(msg_cmd));
        }

        const MSG_AG_ZONE_ROOM_JOIN_SYN* recv_msg_;
        ServerSession* server_session_;
        bool rollback_;
    } rollback(recv_msg, server_session);
    //
    ServerSession* const agent_server = g_pGameServer->GetSession(AGENT_SERVER);
    if (agent_server == NULL ||
        agent_server->IsConnected() == false ||
        agent_server->GetSessionIndex() != base_msg->session_index)
    {
        return;
    };

    GameZone* const game_zone = g_GameZoneManager.GetZone(recv_msg->m_RoomKey);
    if (game_zone == NULL)
    {
        SUNLOG(eCRITICAL_LOG, "|[%s]|can't find room (%d)", __FUNCTION__, recv_msg->m_RoomKey);
        return;
    }
    Player* const creating_player = static_cast<Player*>(
        ObjectFactory::Instance()->AllocObject(PLAYER_OBJECT));
    if (creating_player) // assertion
    {
        ASSERT(agent_server->GetSessionIndex() == base_msg->session_index);
        // 플레이어 생성
        creating_player->Create(agent_server,
            user_key, recv_msg->m_pszUserID, recv_msg->m_pszUserIP, recv_msg->m_LoginTime_t);
        creating_player->SetObjectKey(recv_msg->m_dwPlayerKey);
        creating_player->SetReservedValue(recv_msg->m_dwReservedValue);
        creating_player->SetSts(recv_msg->m_bySts);
        creating_player->SetPCRoomSts(recv_msg->m_PCRoomSts);
        // CHANGES: f090622, lst1024, added a missing operation on China version
        player_manager->AddPlayer(creating_player);

        // 순서 주의 : PlayerManager::AddPlayer() 함수내부에서 Player->Init()호출로 인해 특정 값이 리셋된다.
#ifdef __CN_OO0510__FATIGUE_MATCHING_UPDATE
        creating_player->SetAge(recv_msg->m_byAge);
        creating_player->SetRealNameAuth(recv_msg->m_byRealNameAuth);
#endif
    };

    // 필드코드, 영역아이디 셋팅
    FIELDCODE entry_field_code = game_zone->GetEntryFieldCode();
    DWORD entry_area_id = game_zone->GetEntryAreaID();

    const eZONETYPE zone_type = game_zone->GetZoneType();
    if ((recv_msg->m_byApplyFieldCode) ||
        (zone_type == eZONETYPE_CHUNTING ||
         zone_type == eZONETYPE_PVP)) // CHANGES: f091207.x, waverix, PvP에서도 위치 선택이 가능하도록 추가
    {
        if (recv_msg->m_wFieldCode) {
            entry_field_code = recv_msg->m_wFieldCode;
        };
        if (recv_msg->m_dwAreaID) {
            entry_area_id = recv_msg->m_dwAreaID;
        };
    }
    creating_player->UnexpectedUserEnterRoom() = (recv_msg->m_byApplyFieldCode != false);

    g_GameZoneManager.ReadyPlayer(recv_msg->m_RoomKey, creating_player,
        entry_field_code, entry_area_id, const_cast<WzVector*>(&recv_msg->m_vStartPos));

    if (recv_msg->m_bIsFirstJoin) {
        creating_player->SetState(PLAYER_CHARINFO_FIRST_REQUESTED);
    }
    else {
        creating_player->SetState(PLAYER_CHARINFO_REQUESTED);
    };
    creating_player->Rider.SyncPrepareRidingStatus(recv_msg->m_RoomKey);

    // ??
    //g_GameGuildManager.DestroyMember(creating_player->GetCharInfo()->m_GuildGuid, creating_player->GetCharGuid());
    ;{
        MSG_DG_CHARINFO_QUERY_SYN msg_charinfo_req;
        creating_player->SendToGameDBPServer(&msg_charinfo_req, sizeof(msg_charinfo_req));
        //
        MSG_DG_COLLECT_LIST_LOAD_SYN msg_collect_list_syn;
        creating_player->SendToGameDBPServer(&msg_collect_list_syn, sizeof(msg_collect_list_syn));

#ifdef _NA_003027_20111013_HONOR_SYSTEM
        MSG_DG_HONORTITLES_SELECT_SYN msg_honortitle_syn;
        creating_player->SendToGameDBPServer(&msg_honortitle_syn, sizeof(msg_honortitle_syn));

        MSG_DG_FAME_REPUTE_SELECT_SYN msg_fame_repute_syn;
        creating_player->SendToGameDBPServer(&msg_fame_repute_syn, sizeof(msg_fame_repute_syn));

        MSG_DG_HONORSYSTEM_DAILYQUEST_SELECT_SYN msg_dailyquest_syn;
        creating_player->SendToGameDBPServer(&msg_dailyquest_syn, sizeof(msg_dailyquest_syn));
#endif //_NA_003027_20111013_HONOR_SYSTEM

#ifdef _NA004034_20120102_POINT_WALLET_SYSTEM
        MSG_DG_POINTWALLET_SELECT_SYN msg_point_select_syn;
        creating_player->SendToGameDBPServer(&msg_point_select_syn, sizeof(msg_point_select_syn));
#endif //_NA004034_20120102_POINT_WALLET_SYSTEM
    };

    rollback.rollback_ = false;
}

//==================================================================================================
//==================================================================================================
