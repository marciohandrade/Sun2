#include "StdAfx.h"
#include ".\handler_gameserver.h"
#include <PacketStruct_GP.h>
#include <PacketStruct_DP.h>
#include ".\PortalServer.h"
#include <ArchiveOfStream.h>
#include "PortalRankingManager.h"

Handler_GameServer_IMPL(GP_RANKING_INFO_REQ_SYN)
{
	MSG_GP_RANKING_INFO_REQ_SYN * pMsg = (MSG_GP_RANKING_INFO_REQ_SYN *)pRecvMsg;
	
	// 이정보를 받을 시점에 이미 Ranking 정보가 Establish되어 있어야 한다.
	// Establish후 Listen

	static const int MAX_PAGE = 5;
	static MSG_GP_RANKING_INFO_REQ_ACK msg;
	for( int type = 0 ; type < eRANKING_MAX ; ++type )
	{
		for( int MissionNo = 0 ; MissionNo < MAX_MAPCODE_NUM ; ++MissionNo )
		{
			MissionRanking * pRanking = g_RankingMgr.FindMissionRanking( (eRANKING_TYPE)type, MissionNo );
			if( !pRanking ) continue;

			for( int page = 0 ; page < MAX_PAGE ; ++page )
			{
				msg.m_Type		= type;
				msg.m_MissionNo = MissionNo;
				msg.m_Page		= page;
				msg.m_Size		= msg.MAX_BUFFER_SIZE;
				ArchiveOfStream lArchive(msg.m_pBUFFER, msg.m_Size);
				pRanking->Serialize( msg.m_Page, lArchive, RANKING_SERIALIZE_LOAD );
				msg.m_Size = lArchive.GetSize();
				pServerSession->SendPacket( &msg, msg.GetSize() );
			}
		}
	}

	MSG_GP_RANKING_INFO_REQ_END_NTF ntf;
	ntf.m_SettledYear	= g_RankingMgr.GetOldSettledDate_Year();
	ntf.m_SettledMonth	= g_RankingMgr.GetOldSettledDate_Month();
	ntf.m_SettledDay	= g_RankingMgr.GetOldSettledDate_Day();
	ntf.m_PeriodDay		= g_RankingMgr.GetPeriod();
	ntf.m_SuspendTime	= g_RankingMgr.GetSuspendTime();
	pServerSession->SendPacket( &ntf, sizeof(ntf) );

	//static MSG_GP_RANKING_INFO_REQ_ACK msg;
	//msg.m_Size = MSG_GP_RANKING_INFO_REQ_ACK::MAX_BUFFER_SIZE;
	//g_RankingMgr.Serialize( msg.m_pBUFFER, msg.m_Size,  RANKING_SERIALIZE_LOAD );
	//pServerSession->SendPacket( &msg, msg.GetSize() );
}

Handler_GameServer_IMPL(GP_RANKING_MISSION_CLEAR_SYN)
{
	MSG_GP_RANKING_MISSION_CLEAR_SYN * pMsg = (MSG_GP_RANKING_MISSION_CLEAR_SYN *)pRecvMsg;

	RC::eRANKING_RESULT rt = g_RankingMgr.CanRegisterMissionRecord( pMsg->m_totalInfo );
	if( rt == RC::RC_RANKING_SUCCESS )
	{
		MSG_DP_RANKING_MISSION_CLEAR_SYN msg;
		msg.m_dwKey				= pMsg->m_dwKey;
		msg.m_Key				= pMsg->m_Key;
		msg.m_dwSessionIndex	= pServerSession->GetSessionIndex();
		memcpy( &msg.m_totalInfo, &pMsg->m_totalInfo, pMsg->m_totalInfo.GetSize() );
		if( g_PortalServer.SendDBProxy( &msg, msg.GetSize() ) )
		{
			g_RankingMgr.RegisterMissionRecordInTemp( pMsg->m_totalInfo, pMsg->m_Key );
			return;
		}
		else
		{
			rt = RC::RC_RANKING_NETWORK_ERROR_FOR_DBPROXY;	
		}
	}
	
	MSG_GP_RANKING_MISSION_FAILED_NAK nmsg;
	nmsg.m_dwKey		= pMsg->m_dwKey;
	nmsg.m_Key			= pMsg->m_Key;
	nmsg.m_MissionNo		= pMsg->m_totalInfo.m_Info.m_MissionNo;
	nmsg.m_dwErrorCode	= rt;
	pServerSession->SendPacket( &nmsg, sizeof(nmsg) );
}


Handler_GameServer_IMPL(GP_RANKING_MISSION_REWARD_SYN)
{
	MSG_GP_RANKING_MISSION_REWARD_SYN * pMsg = (MSG_GP_RANKING_MISSION_REWARD_SYN *)pRecvMsg;
	
	// 보상 여부 체크
	// DoReward
	// DB로 보상 쿼리
	RC::eRANKING_RESULT rt = RC::RC_RANKING_NETWORK_ERROR_FOR_DBPROXY;
	if( g_PortalServer.IsDBProxyConnected() )
	{
		rt = g_RankingMgr.DoReward( pMsg->m_MissionNo, pMsg->m_Key, pMsg->m_tszCharName );
		if( rt == RC::RC_RANKING_SUCCESS )
		{
			MSG_DP_RANKING_MISSION_REWARD_SYN msg;
			msg.m_dwSessionIndex = pServerSession->GetSessionIndex();
			msg.m_dwKey			= pMsg->m_dwKey;
			msg.m_Key			= pMsg->m_Key;						//< 보상받기를 원하는 지난주랭킹의 키(일련번호)
			msg.m_MissionNo	= pMsg->m_MissionNo;
			_tcsncpy(msg.m_tszCharName, pMsg->m_tszCharName, MAX_CHARNAME_LENGTH);
			msg.m_tszCharName[MAX_CHARNAME_LENGTH] = '\0';

			g_PortalServer.SendDBProxy( &msg, msg.GetSize() );

			return;
			//------------------------------------------------------------------------------------------
			//------------------------------------------------------------------------------------------
		}
	}

	MSG_GP_RANKING_MISSION_REWARD_NAK nmsg;
	nmsg.m_dwKey		= pMsg->m_dwKey;
	nmsg.m_byErrorCode	= rt;
	nmsg.m_Key			= pMsg->m_Key;						//< 보상받기를 원하는 지난주랭킹의 키(일련번호)
	nmsg.m_MissionNo	= pMsg->m_MissionNo;
	_tcsncpy(nmsg.m_tszCharName, pMsg->m_tszCharName, MAX_CHARNAME_LENGTH);
	nmsg.m_tszCharName[MAX_CHARNAME_LENGTH] = '\0';
	pServerSession->SendPacket( &nmsg, nmsg.GetSize() );
}

Handler_GameServer_IMPL(GP_RANKING_SETTLE_CMD)
{
	// 현재시간을 정산시간으로 맞추고
	g_RankingMgr.DoTestCurSettle();
	// 자동 정산됨
	//g_RankingMgr.DoSettle();

	// 중지기간을 원상태로 돌려놓는다?
	//g_RankingMgr.InitComputation( tmdate->tm_year+1900, tmdate->tm_mon+1, tmdate->tm_mday, 
	//	tmdate->tm_hour, tmdate->tm_min, tmdate->tm_sec, 1, 15 );
}

Handler_GameServer_IMPL(GP_RANKING_REGISTER_MISSION_POINT_SYN)
{
	MSG_GP_RANKING_REGISTER_MISSION_POINT_SYN * pMsg = (MSG_GP_RANKING_REGISTER_MISSION_POINT_SYN *)pRecvMsg;

	//정산중이면 등록 안함
	if( g_RankingMgr.GetStatus() != eRANKING_STATUS_NORMAL )
		return;

	MSG_DP_RANKING_REGISTER_MISSION_POINT_SYN SynMsg;
	SynMsg.m_dwSessionIndex			= pServerSession->GetSessionIndex();
	SynMsg.TotalInfo.m_Count		= 1;
	SynMsg.TotalInfo.m_Info			= pMsg->RecordInfo; 
	SynMsg.TotalInfo.m_Member[0]	= pMsg->MemberInfo;
	SynMsg.SettledStartDate			= g_RankingMgr.GetOldSettledDate();
	SynMsg.Period					= g_RankingMgr.GetPeriod();
	SynMsg.DBKey					= pMsg->DBKey;
	SynMsg.TempKey					= g_RankingMgr.RegisterMissionRecordInTemp( SynMsg.TotalInfo );
	
	g_PortalServer.SendDBProxy( &SynMsg, SynMsg.GetSize() );
}

Handler_GameServer_IMPL(GP_MISSION_RANKING_SETTLE_DATE_SYN)
{
	MSG_GP_MISSION_RANKING_SETTLE_DATE_SYN * pMsg = (MSG_GP_MISSION_RANKING_SETTLE_DATE_SYN *)pRecvMsg;

	MSG_GP_MISSION_RANKING_SETTLE_DATE_ACK Ack;
	Ack.RecordInfo			= pMsg->RecordInfo;
	Ack.MemberInfo			= pMsg->MemberInfo;
	Ack.SettledStartDate	= g_RankingMgr.GetOldSettledDate();
	Ack.Period				= g_RankingMgr.GetPeriod();

	pServerSession->SendPacket( &Ack, sizeof(Ack) );
}