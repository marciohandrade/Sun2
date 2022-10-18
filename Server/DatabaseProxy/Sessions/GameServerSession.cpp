#include "stdafx.h"
#include "./GameServerSession.h"

#include <Global.h>
#include <PacketStruct_GP.h>

#include "PacketHandler/PacketHandler.h"
#include "DBUser.h"

#ifdef _NA_1419_20100121_ACCUMULATE_POINT
#include "Services/AccumulatePointMatchDate.h"
#endif
#include "Services/PortalRankingManager.h"

__IMPL_CUSTOMPOOL_PTR( GameServerSession )
GameServerSession::GameServerSession()
{

}

GameServerSession::~GameServerSession()
{
}

VOID GameServerSession::OnRecv( BYTE * pMsg, WORD wSize )
{
	PacketHandler::Instance()->ParsePacket( PACKET_HANDLER_GAME, this, (MSG_BASE *)pMsg, wSize );
}

VOID GameServerSession::OnDisconnect()
{
	SUNLOG( eDEV_LOG, "[GameServerSession::OnDisconnect()] Remove RelagteUsers, ServerType = %d", GetServerType() );
    g_DBUserManager.RemoveUsersByServerIndex(GetSessionIndex(), GetServerType());
#if SUN_CODE_BACKUP
	DisconnectRelatedGame op(GetSessionIndex());
	removeRelatedUsers(op);
#endif
	ChildServerSession::OnDisconnect();
}

void GameServerSession::OnRedirect(DWORD network_index, MSG_SERVER_TYPE* const first_contact_msg)
{
    ChildServerSession::OnRedirect(network_index, first_contact_msg);
    ;{
        // CHANGES: f110617.1L
        first_contact_msg->m_byCategory = DG_CONNECTION;
        first_contact_msg->m_byProtocol = DG_CONNECTION_GROUND_CMD;
        // CHANGES: f110615.2L, added to solve high-load network bottleneck problem
        switch (first_contact_msg->m_byServerType)
        {
        case FIELD_SERVER:
        case BATTLE_SERVER:
        case GAME_SERVER:
            this->UseSendPendingBuffer(3 * 1024 * 1024);
            this->UseRecvDualBuffer(5 * 1024 * 1024);
            break;
        };
    };
#ifdef _NA_1419_20100121_ACCUMULATE_POINT
    // 게임서버의 접속시 적립 포인트 관련 정보를 보낸다.
    AccumulatePointMatchDate::Instance()->SendMatchDate(this);
#endif
#ifdef _NA_008012_20150130_RANKING_SYSTEM
#ifdef _NA002635_GP_DP_PROTOCOL_UINIFICATION_IN_DBP
    if (g_RankingMgr.enabled_service())
    {
        MSG_GP_RANKING_SESSION_CONTROL_CMD msg_cmd;
        msg_cmd.enabled_service = true;
        SendPacket(&msg_cmd, sizeof(msg_cmd));
    };
#endif
#endif // _NA_008012_20150130_RANKING_SYSTEM

}
