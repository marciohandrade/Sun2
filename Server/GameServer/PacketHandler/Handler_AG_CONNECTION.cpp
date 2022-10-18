#include "StdAfx.h"
#include ".\Handler_AG_CONNECTION.h"
#include "GameServerEx.h"
#include <PacketStruct_AG.h>
#include <PacketStruct_DG.h>
#include <Protocol_AG.h>
#include <Protocol_DG.h>
#include <ServerSession.h>
#include "PlayerManager.h"
#include "Player.h"
#include "ItemManager.h"

#include <PacketStruct_GZ.h>
#include <Protocol_AZ.h>
#include "GameGuildManager.h"

// _NA_0_20091118_GAMEROOM_BATTLE_RESULT
#include "GameBattleResult.h"
#include "GameZone.h"

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#include "GameInstanceDungeon.h"
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND


Handler_AG_CONNECTION::Handler_AG_CONNECTION(void)
{
}

Handler_AG_CONNECTION::~Handler_AG_CONNECTION(void)
{
}

//--------------------------------------------------------------------------------------------------
// changes, f100819.1L
const nsPacket::PacketHandlerInfo Handler_AG_CONNECTION::GetPacketHandlerNodes()
{
#define HANDLER_NODE_INFO(c, p, phase) { MAKEWORD(c, p), phase, Handler_AG_CONNECTION::On##p }
#define HANDLER_NODE_INFO_EOR() { 0, static_cast<ePHASE_SHIFT>(0), NULL }

    static const nsPacket::PacketHandlerNode handlers_static_[] =
    {
        // CONNECTION
        HANDLER_NODE_INFO(AG_CONNECTION, AG_CONNECTION_GROUND_CMD, PHASE_SERVICE),
        HANDLER_NODE_INFO(AG_CONNECTION, AG_CONNECTION_DBCONNECT_UPDATE_CMD, PHASE_SERVICE),
	    HANDLER_NODE_INFO(AG_CONNECTION, AG_CONNECTION_DISCONNECT_CMD, PHASE_SERVICE),
	    HANDLER_NODE_INFO(AG_CONNECTION, AG_WORLDSERVER_INFO_CMD, PHASE_SERVICE),
	    HANDLER_NODE_INFO(AG_CONNECTION, AG_CONNECTION_SERVER_INFO_CMD, PHASE_SERVICE),
        HANDLER_NODE_INFO(AG_CONNECTION, AG_WORLDSERVER_ABUSING_USER_VERIFY_QUERY_CMD, PHASE_SERVICE),
    #ifdef __NA001334_090420_SERVER_SELECT_FACILITY__
        HANDLER_NODE_INFO(AG_CONNECTION, AG_CONNECTION_SERVER_SELECT_CONTROL_SYN, PHASE_SERVICE),
    #endif
	    //HANDLER_NODE_INFO(AG_CONNECTION, AG_CONNECTION_UNREGISTER_CMD, PHASE_SERVICE),
        HANDLER_NODE_INFO_EOR() // end of records
    };
    //
    BOOST_STATIC_ASSERT(_countof(handlers_static_) >= 1);
    nsPacket::PacketHandlerInfo return_value = { handlers_static_, _countof(handlers_static_) - 1 };
    //
    return return_value;
};
//--------------------------------------------------------------------------------------------------

Handler_AG_CONNECTION_IMPL(AG_CONNECTION_GROUND_CMD)
{
    __UNUSED((pServerSession, pMsg, wSize));
}

Handler_AG_CONNECTION_IMPL(AG_CONNECTION_DBCONNECT_UPDATE_CMD)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    //_NA000000_DBQUERY_REVERSAL_STATE_PREVENTION
    const MSG_AG_CONNECTION_DBCONNECT_UPDATE_CMD* recv_msg = \
        static_cast<MSG_AG_CONNECTION_DBCONNECT_UPDATE_CMD*>(pMsg);
    g_pGameServer->ChangeDBConnectorIndex(recv_msg->db_connector_index_);
    ;
    MSG_DG_CONNECTION_DBCONNECT_UPDATE_CMD msg_cmd;
    msg_cmd.db_connector_index_ = recv_msg->db_connector_index_;
    g_pGameServer->SendToServer(GAME_DBPROXY, &msg_cmd, sizeof(msg_cmd));
}

//////////////////////////////////////////////////////////////////////////////////////////////
//
// 클라이언트와 Agent사이 접속이 끊었졌을 때
//
//////////////////////////////////////////////////////////////////////////////////////////////
Handler_AG_CONNECTION_IMPL( AG_CONNECTION_DISCONNECT_CMD )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	__UNUSED(wSize);
	MSG_AG_CONNECTION_DISCONNECT_CMD * pRecvMsg = (MSG_AG_CONNECTION_DISCONNECT_CMD *)pMsg;
	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	if (pPlayer == NULL)
	{
		// Agent랑 Game Server랑 끊어져서 연결되었을 때 game server에서는 유저가 없다.
		SUNLOG( eCRITICAL_LOG,  "[AG_CONNECTION_DISCONNECT_CMD] No such Player(%d) in GameServer", pRecvMsg->m_dwKey );
		return;
	}


// _NA_0_20091118_GAMEROOM_BATTLE_RESULT 
    GameZone * pEnterZone = NULL;
    MAPCODE EnterFieldCode = 0;
    eZONE_STATE eRoomState = pPlayer->GetGameZone( pEnterZone, EnterFieldCode );
    if( eRoomState & ePRS_AT_ZONE )
    {
        GameBattleResult* battle_result = pEnterZone->GetGameBattleResult();
        bool abnormal_exit = pRecvMsg->m_AbNormalExit;
        if (battle_result /*&& pRecvMsg->m_AbNormalExit*/) // 정상이든 비정상이든 Leave시킨다. arycoat 2012.02.01
            battle_result->Leave(pPlayer, abnormal_exit);
    }

	g_GameGuildManager.DestroyMember( pPlayer->GetCharInfo()->m_GuildGuid, pPlayer->GetCharGuid() );

	MSG_GZ_GUILD_LOGOUT_CHAR_SYN	synMsg;
	synMsg.m_GuildGuid = pPlayer->GetCharInfo()->m_GuildGuid;
	synMsg.m_CharGuid = pPlayer->GetCharGuid();
	pPlayer->SendToGuildServer( &synMsg, sizeof(synMsg) );

	SUNLOG( eFULL_LOG, "[AG_CONNECTION_DISCONNECT_CMD] UserKey(%d) 접속해제", pPlayer->GetUserKey() );
	PlayerManager::Instance()->Disconnect( pPlayer );
}

// 월드서버와 Agent간의 연결상태를 알려준다.
Handler_AG_CONNECTION_IMPL( AG_WORLDSERVER_INFO_CMD )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AG_WORLDSERVER_INFO_CMD* pRecvMsg = (MSG_AG_WORLDSERVER_INFO_CMD*)pMsg;
	g_pGameServer->SetConnectToWorldServer( pRecvMsg->m_bIsConnectedWorld );
}

Handler_AG_CONNECTION_IMPL(AG_CONNECTION_SERVER_INFO_CMD)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    // (CHANGES) (f100424.3L) add channel id to synchronize or debug
    const MSG_AG_CONNECTION_SERVER_INFO_CMD* recv_msg = \
        static_cast<MSG_AG_CONNECTION_SERVER_INFO_CMD*>(pMsg);
    assert(sizeof(*recv_msg) == wSize);
    g_pGameServer->SetPVPMode(recv_msg->pvp_enabled_ != false);

    if (g_pGameServer->IsServerInfoFromFile())
    {
        SERVER_KEY change_key = g_pGameServer->GetKey();
        change_key.m_ChannelID = recv_msg->channel_id_;
        g_pGameServer->SetKey(change_key);
        g_pGameServer->UpdateAppTitle();
    }
    else
    {
        const SERVER_KEY server_key = g_pGameServer->GetKey();
        if (!FlowControl::FCAssert(server_key.m_ChannelID == recv_msg->channel_id_ && \
                                   "must be modified the server key values " \
                                   "of the T_ServerInfoList table of the WOPS_RTTDB"))
        {
            __debugbreak();
        }
    }
}

Handler_AG_CONNECTION_IMPL( AG_WORLDSERVER_ABUSING_USER_VERIFY_QUERY_CMD )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AG_WORLDSERVER_ABUSING_USER_VERIFY_QUERY_CMD * pRecvMsg = (MSG_AG_WORLDSERVER_ABUSING_USER_VERIFY_QUERY_CMD *)pMsg;
	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey(pRecvMsg->m_dwKey);
	if(pPlayer)
	{
		SECURELOG( SECURELOG_PLAYERINFO((Player*)pPlayer, eABUSE_LOGIC_VIOLATION, pRecvMsg->m_eAbuseSubType),
			_T("[FromWorldServer] The WorldServer UserData Wrong!!"));
		return;
	}

	SUNLOG(eCRITICAL_LOG, "AG_WORLDSERVER_ABUSING_USER_VERIFY_QUERY_CMD(%u)(%u)", pRecvMsg->m_dwKey, pRecvMsg->m_CharGuid);
}


#ifdef __NA001334_090420_SERVER_SELECT_FACILITY__
Handler_AG_CONNECTION_IMPL(AG_CONNECTION_SERVER_SELECT_CONTROL_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    const MSG_AG_CONNECTION_SERVER_SELECT_CONTROL_SYN* pRecvMsg =
        static_cast<MSG_AG_CONNECTION_SERVER_SELECT_CONTROL_SYN*>(pMsg);

    MSG_AG_CONNECTION_SERVER_SELECT_CONTROL_ACK msgACK;
    BOOLEAN bValidState = TRUE, bSendResult = TRUE;
    Player* pPlayer = PlayerManager::Instance()->FindPlayerByUserKey(pRecvMsg->m_dwKey);
    if(pPlayer == 0)
    {
        msgACK.m_dwKey = pRecvMsg->m_dwKey;
        msgACK.m_Result = RC::RC_ROOM_FAILED;
        pServerSession->SendPacket(&msgACK, sizeof(msgACK));
        return;
    }

    ePLAYER_BEHAVE_STATE behave = pPlayer->GetBehave();
    if(pRecvMsg->m_Control == pRecvMsg->eControl_StartTransaction)
    {
#ifdef _NA_000000_20130508_DONOT_SERVER_SELECT_WHILE_BATTLE_GROUND_RESERVATION
        if(bValidState && behave != PLAYER_BEHAVE_IDLE_STATE &&
                          behave != PLAYER_BEHAVE_RESERVATION_BATTLE_GROUND_ZONE)
#else
        if(bValidState && behave != PLAYER_BEHAVE_IDLE_STATE)
#endif // _NA_000000_20130508_DONOT_SERVER_SELECT_WHILE_BATTLE_GROUND_RESERVATION
            bValidState = FALSE, msgACK.m_Result = RC::RC_ROOM_INVALID_PLAYER_STATE;

        if(bValidState)
        {
            GameZone* pCurrentZone = 0;
            MAPCODE currentFieldCode = 0;
            eZONE_STATE eZone_State = pPlayer->GetGameZone(pCurrentZone, currentFieldCode);
            if((eZone_State & ePRS_AT_ZONE) == 0)
                bValidState = FALSE, msgACK.m_Result = RC::RC_ROOM_ALREADY_DOING_TRANSACTION;
        }
    }
    else
    {
        bSendResult = FALSE;
        if(bValidState && behave == PLAYER_BEHAVE_SERVER_SELECTING_STATE)
            pPlayer->SetBehave(PLAYER_BEHAVE_IDLE_STATE);
    }

    if(bValidState && pRecvMsg->m_Control == pRecvMsg->eControl_StartTransaction)
    {
        pPlayer->SetBehave(PLAYER_BEHAVE_SERVER_SELECTING_STATE);
        msgACK.m_Result = RC::RC_ROOM_SUCCESS;
    }

    if(bSendResult)
        pPlayer->SendPacket(&msgACK, sizeof(msgACK));
}
#endif












