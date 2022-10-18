#include "StdAfx.h"
#include ".\handler_dbproxy.h"
#include <PacketStruct_DP.h>
#include <PacketStruct_GP.h>
#include "PortalServer.h"
#include "PortalRankingManager.h"


Handler_DBProxy_IMPL(DP_RANKING_INFO_REQ_ACK)
{
	MSG_DP_RANKING_INFO_REQ_ACK * pMsg = (MSG_DP_RANKING_INFO_REQ_ACK*)pRecvMsg;
	
	DISPMSG( "Ranking Stream Received(Type:%u,No:%u,Page:%u,Size:%u)\n", 
		pMsg->m_Type, pMsg->m_MissionNo, pMsg->m_Page, pMsg->m_Size );
	g_RankingMgr.Serialize( (eRANKING_TYPE)pMsg->m_Type, pMsg->m_MissionNo, pMsg->m_Page, pMsg->m_pBUFFER, pMsg->m_Size, RANKING_SERIALIZE_SAVE );
	
}	
Handler_DBProxy_IMPL(DP_RANKING_INFO_REQ_END_NTF)
{
	MSG_DP_RANKING_INFO_REQ_END_NTF * pMsg = (MSG_DP_RANKING_INFO_REQ_END_NTF*)pRecvMsg;

	DISPMSG( "Ranking Stream Received Successfully....Status is Normal and ServerSide Listen\n" );
	g_RankingMgr.SetStatus( eRANKING_STATUS_NORMAL );
	g_PortalServer.Listen( SERVER_IOHANDLER );
}

Handler_DBProxy_IMPL(DP_RANKING_INFO_REQ_NAK)
{
	MSG_DP_RANKING_INFO_REQ_NAK * pMsg = (MSG_DP_RANKING_INFO_REQ_NAK*)pRecvMsg;

	SUNLOG( eCRITICAL_LOG, _T("DP_RANKING_INFO_REQ_NAK Error[%u] can't receive Ranking Info from DBProxy"), pMsg->m_byErrorCode );
}



Handler_DBProxy_IMPL(DP_RANKING_MISSION_CLEAR_ACK)
{
	MSG_DP_RANKING_MISSION_CLEAR_ACK * pMsg = (MSG_DP_RANKING_MISSION_CLEAR_ACK *)pRecvMsg;
	
	// 등수에 들어가면 모든 서버에 SUCCESS BRD
	// 등수에 들어가지 못하면 그 서버에만 FAILED BRD

	ServerSession * pGameServer = g_PortalServer.FindSession(pMsg->m_dwSessionIndex);
	ASSERT( pGameServer && pGameServer->GetServerType() == GAME_SERVER );

	WORD Ranking = (WORD)g_RankingMgr.OnRegisterMissionRecordOnTodayweek( TRUE, pMsg->m_MissionNo, pMsg->m_Key, pMsg->m_NewKey );
	if( 0 == Ranking )
	{
		if( pGameServer )
		{
			MSG_GP_RANKING_MISSION_FAILED_NAK msg;
			msg.m_dwKey			= pMsg->m_dwKey;
			msg.m_Key			= pMsg->m_Key;
			msg.m_MissionNo		= pMsg->m_MissionNo;
			msg.m_dwErrorCode	= RC::RC_RANKING_DONT_INCLUDE_RANKING;
			pGameServer->SendPacket( &msg, sizeof(msg) );
		}
	}
	else
	{
		if( pGameServer )
		{
			MSG_GP_RANKING_MISSION_SUCCESS_ACK amsg;
			amsg.m_dwKey		= pMsg->m_dwKey;
			amsg.m_Key			= pMsg->m_Key;
			amsg.m_NewKey		= pMsg->m_NewKey;
			amsg.m_MissionNo	= pMsg->m_MissionNo;
			amsg.m_Ranking		= Ranking;
			pGameServer->SendPacket( &amsg, sizeof(amsg) );
		}
		MissionRankingRecord * pRecord = g_RankingMgr.FindMissionRecord( eRANKING_TODAYWEEK, pMsg->m_MissionNo, pMsg->m_NewKey );
		ASSERT(pRecord);
		if( pRecord )
		{
			MSG_GP_RANKING_MISSION_SUCCESS_BRD msg;
			msg.m_NewKey = pMsg->m_NewKey;
			msg.m_RankingInfo.m_Info.m_MissionNo = pMsg->m_MissionNo;
			pRecord->Serialize( msg.m_RankingInfo, SERIALIZE_LOAD );
			g_PortalServer.SendToSessionType( GAME_SERVER, &msg, sizeof(msg), pMsg->m_dwSessionIndex );
		}
	}
}

Handler_DBProxy_IMPL(DP_RANKING_MISSION_CLEAR_NAK)
{
	MSG_DP_RANKING_MISSION_CLEAR_NAK * pMsg = (MSG_DP_RANKING_MISSION_CLEAR_NAK *)pRecvMsg;

	g_RankingMgr.OnRegisterMissionRecordOnTodayweek( FALSE, 0, pMsg->m_Key, 0 );

	// 게임서버로 포워딩
	ServerSession * pGameServer = g_PortalServer.FindSession(pMsg->m_dwSessionIndex);
	ASSERT( pGameServer && pGameServer->GetServerType() == GAME_SERVER );
	if( pGameServer )
	{
		MSG_GP_RANKING_MISSION_FAILED_NAK msg;
		msg.m_dwKey			= pMsg->m_dwKey;
		msg.m_Key			= pMsg->m_Key;
		msg.m_MissionNo		= pMsg->m_MissionNo;
		msg.m_dwErrorCode	= pMsg->m_byErrorCode;
		pGameServer->SendPacket( &msg, sizeof(msg) );
	}
}


Handler_DBProxy_IMPL(DP_RANKING_MISSION_REWARD_ACK)
{
	MSG_DP_RANKING_MISSION_REWARD_ACK * pMsg = (MSG_DP_RANKING_MISSION_REWARD_ACK *)pRecvMsg;

	g_RankingMgr.OnReward( TRUE, pMsg->m_MissionNo, pMsg->m_Key, pMsg->m_tszCharName );

	// 게임서버로 포워딩
	ServerSession * pGameServer = g_PortalServer.FindSession(pMsg->m_dwSessionIndex);
	ASSERT( pGameServer && pGameServer->GetServerType() == GAME_SERVER );
	if( pGameServer )
	{
		MSG_GP_RANKING_MISSION_REWARD_ACK msg;
		msg.m_dwKey		= pMsg->m_dwKey;
		msg.m_Key		= pMsg->m_Key;
		msg.m_MissionNo	= pMsg->m_MissionNo;
		_tcsncpy(msg.m_tszCharName, pMsg->m_tszCharName, MAX_CHARNAME_LENGTH);
		msg.m_tszCharName[MAX_CHARNAME_LENGTH] = '\0';
		pGameServer->SendPacket( &msg, msg.GetSize() );

		MSG_GP_RANKING_MISSION_REWARD_BRD bmsg;
		bmsg.m_dwKey		= pMsg->m_dwKey;
		bmsg.m_Key		= pMsg->m_Key;
		bmsg.m_MissionNo	= pMsg->m_MissionNo;
		_tcsncpy(bmsg.m_tszCharName, pMsg->m_tszCharName, MAX_CHARNAME_LENGTH);
		bmsg.m_tszCharName[MAX_CHARNAME_LENGTH] = '\0';
		g_PortalServer.SendToSessionType( GAME_SERVER, &bmsg, bmsg.GetSize(), pMsg->m_dwSessionIndex );
	}

}

Handler_DBProxy_IMPL(DP_RANKING_MISSION_REWARD_NAK)
{
	MSG_DP_RANKING_MISSION_REWARD_NAK * pMsg = (MSG_DP_RANKING_MISSION_REWARD_NAK *)pRecvMsg;

	SUNLOG( eCRITICAL_LOG, 
		_T("DP_RANKING_MISSION_REWARD_NAK Error[%u] can't execute Ranking Reward Action from DBProxy"), 
		pMsg->m_byErrorCode );

	g_RankingMgr.OnReward( FALSE, pMsg->m_MissionNo, pMsg->m_Key, pMsg->m_tszCharName );

	// 게임서버로 포워딩
	ServerSession * pGameServer = g_PortalServer.FindSession(pMsg->m_dwSessionIndex);
	ASSERT( pGameServer && pGameServer->GetServerType() == GAME_SERVER );
	if( pGameServer )
	{
		MSG_GP_RANKING_MISSION_REWARD_NAK nmsg;
		nmsg.m_byErrorCode	= pMsg->m_byErrorCode;
		nmsg.m_Key			= pMsg->m_Key;
		nmsg.m_MissionNo	= pMsg->m_MissionNo;
		_tcsncpy(nmsg.m_tszCharName, pMsg->m_tszCharName, MAX_CHARNAME_LENGTH);
		nmsg.m_tszCharName[MAX_CHARNAME_LENGTH] = '\0';
		pGameServer->SendPacket( &nmsg, nmsg.GetSize() );
	}
}

Handler_DBProxy_IMPL(DP_RANKING_REGISTER_MISSION_POINT_ACK)
{
	MSG_DP_RANKING_REGISTER_MISSION_POINT_ACK * pMsg = (MSG_DP_RANKING_REGISTER_MISSION_POINT_ACK *)pRecvMsg;	

	WORD Ranking = (WORD)g_RankingMgr.OnRegisterMissionRecordOnTodayweek( TRUE, pMsg->MissionNo, pMsg->TempKey, pMsg->DBKey );
	if( 0 == Ranking )
	{
		//순위권에 못들면 랭킹등록 안함.
		//SUNLOG(eCRITICAL_LOG, "[DP_RANKING_REGISTER_MISSION_POINT_ACK] - OnRegisterMissionRecordOnTodayweek Fail.");
	}
	else
	{
		MissionRankingRecord * pRecord = g_RankingMgr.FindMissionRecord( eRANKING_TODAYWEEK, pMsg->MissionNo, pMsg->DBKey );
		ASSERT(pRecord);
		if( pRecord )
		{
			MSG_GP_RANKING_MISSION_SUCCESS_BRD msg;
			msg.m_NewKey = pMsg->DBKey;
			msg.m_RankingInfo.m_Info.m_MissionNo = pMsg->MissionNo;
			pRecord->Serialize( msg.m_RankingInfo, SERIALIZE_LOAD );
			g_PortalServer.SendToSessionType( GAME_SERVER, &msg, sizeof(msg) );
		}
	}
}

Handler_DBProxy_IMPL(DP_RANKING_REGISTER_MISSION_POINT_NAK)
{
	MSG_DP_RANKING_REGISTER_MISSION_POINT_NAK * pMsg = (MSG_DP_RANKING_REGISTER_MISSION_POINT_NAK *)pRecvMsg;

	g_RankingMgr.OnRegisterMissionRecordOnTodayweek( FALSE, 0, pMsg->m_OldKey, 0 );

	// 게임서버로 포워딩 할필요 없음
	// 시퀀스 다이어그램 참고
}