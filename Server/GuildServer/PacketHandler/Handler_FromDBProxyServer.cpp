#include "stdafx.h"
#include "Handler_FromDBProxyServer.h"
#include "PacketHandler.Shared.h"

#include <GuildServer.h>

#include <PacketStruct_DZ.h>
#include <PacketStruct_GZ.h>
#include <PacketStruct_WZ.h>

#include "DominationManager.h"

// GUILD
#include <Guild/GuildManager.h>
#include <Guild/Guild.h>
#include <Guild/GuildMember.h>
#include <Guild/GuildRelation.h>
#include <Guild/GuildSmallContentEntry.h>


#define ADD_HANDLER_INFO_EOR()	{ MAKEWORD(0, 0), PHASE_NOACTION, (PH_fnHandler)NULL }
#define ADD_HANDLER_INFO( c, p, accptablePHASE )		\
	{ MAKEWORD(c, p), accptablePHASE, (PH_fnHandler)&Handler_FromDBProxyServer::On##p }

sPACKET_HANDLER_INFO*	Handler_FromDBProxyServer::GetHandlerInfo()
{
	static sPACKET_HANDLER_INFO s_pHandlerInfo[] =
	{
		//
		ADD_HANDLER_INFO( DZ_GUILD, DZ_GUILD_SELECT_ACK, PHASE_UNLIMITED ),
		ADD_HANDLER_INFO( DZ_GUILD, DZ_GUILD_CREATE_ACK, PHASE_UNLIMITED ),
		ADD_HANDLER_INFO( DZ_GUILD, DZ_GUILD_DESTROY_ACK, PHASE_UNLIMITED ),
		ADD_HANDLER_INFO( DZ_GUILD, DZ_GUILD_JOIN_ACK, PHASE_UNLIMITED ),
		ADD_HANDLER_INFO( DZ_GUILD, DZ_GUILD_WITHDRAW_ACK, PHASE_UNLIMITED ),

		ADD_HANDLER_INFO( DZ_GUILD, DZ_GUILD_SELECT_NAK, PHASE_UNLIMITED ),
		ADD_HANDLER_INFO( DZ_GUILD, DZ_GUILD_CREATE_NAK, PHASE_UNLIMITED ),
		ADD_HANDLER_INFO( DZ_GUILD, DZ_GUILD_DESTROY_NAK, PHASE_UNLIMITED ),
		ADD_HANDLER_INFO( DZ_GUILD, DZ_GUILD_JOIN_NAK, PHASE_UNLIMITED ),
		ADD_HANDLER_INFO( DZ_GUILD, DZ_GUILD_WITHDRAW_NAK, PHASE_UNLIMITED ),
		ADD_HANDLER_INFO( DZ_GUILD, DZ_GUILD_KICKPLAYER_ACK, PHASE_UNLIMITED ),
		ADD_HANDLER_INFO( DZ_GUILD, DZ_GUILD_KICKPLAYER_NAK, PHASE_UNLIMITED ),
		ADD_HANDLER_INFO( DZ_GUILD, DZ_GUILD_CHANGE_NOTICE_ACK, PHASE_UNLIMITED ),
		ADD_HANDLER_INFO( DZ_GUILD, DZ_GUILD_CHANGE_NOTICE_NAK, PHASE_UNLIMITED ),

		ADD_HANDLER_INFO( DZ_GUILD, DZ_GUILD_DONATE_UP_ACK, PHASE_UNLIMITED ),
		ADD_HANDLER_INFO( DZ_GUILD, DZ_GUILD_DONATE_UP_NAK, PHASE_UNLIMITED ),
		ADD_HANDLER_INFO( DZ_GUILD, DZ_GUILD_CONVERT_UP2GP_ACK, PHASE_UNLIMITED ),
		ADD_HANDLER_INFO( DZ_GUILD, DZ_GUILD_CONVERT_UP2GP_NAK, PHASE_UNLIMITED ),
		ADD_HANDLER_INFO( DZ_GUILD, DZ_GUILD_RANKUP_ACK, PHASE_UNLIMITED ),
		ADD_HANDLER_INFO( DZ_GUILD, DZ_GUILD_RANKUP_NAK, PHASE_UNLIMITED ),

#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
		ADD_HANDLER_INFO( DZ_GUILD, DZ_GUILD_CHUNTING_PVP_RESULT_ACK, PHASE_UNLIMITED ),
#endif
		ADD_HANDLER_INFO( DZ_GUILD, DZ_GUILD_RELATION_INFO_ACK, PHASE_UNLIMITED ), 
		ADD_HANDLER_INFO( DZ_GUILD, DZ_GUILD_RELATION_INFO_NAK, PHASE_UNLIMITED ), 

        ADD_HANDLER_INFO( DZ_GUILD, DZ_GUILD_CHANGE_DUTY_ACK, PHASE_UNLIMITED ),		// 길드 직책
		ADD_HANDLER_INFO( DZ_GUILD, DZ_GUILD_CHANGE_DUTY_NAK, PHASE_UNLIMITED ),
		ADD_HANDLER_INFO( DZ_GUILD, DZ_GUILD_RIGHT_DELEGATION_ACK, PHASE_UNLIMITED ),	// 길드 위임권한 변경
		ADD_HANDLER_INFO( DZ_GUILD, DZ_GUILD_RIGHT_DELEGATION_NAK, PHASE_UNLIMITED ),
		ADD_HANDLER_INFO( DZ_GUILD, DZ_GUILD_CHANGE_MASTER_ACK, PHASE_UNLIMITED ),		// 길드 마스터 변경
		ADD_HANDLER_INFO( DZ_GUILD, DZ_GUILD_CHANGE_MASTER_NAK, PHASE_UNLIMITED ),

		ADD_HANDLER_INFO( DZ_GUILD, DZ_GUILD_CREATE_CORPS_ACK, PHASE_UNLIMITED ),		// 길드 군단 생성
		ADD_HANDLER_INFO( DZ_GUILD, DZ_GUILD_CREATE_CORPS_NAK, PHASE_UNLIMITED ),
		ADD_HANDLER_INFO( DZ_GUILD, DZ_GUILD_DESTROY_CORPS_ACK, PHASE_UNLIMITED ),		// 길드 군단 해산
		ADD_HANDLER_INFO( DZ_GUILD, DZ_GUILD_DESTROY_CORPS_NAK, PHASE_UNLIMITED ),
		ADD_HANDLER_INFO( DZ_GUILD, DZ_GUILD_CHANGE_COMMANDER_ACK, PHASE_UNLIMITED ),	// 길드 군단장 변경
		ADD_HANDLER_INFO( DZ_GUILD, DZ_GUILD_CHANGE_COMMANDER_NAK, PHASE_UNLIMITED ),
		ADD_HANDLER_INFO( DZ_GUILD, DZ_GUILD_CHANGE_CORPS_ACK, PHASE_UNLIMITED ),		// 길드 군단 이동
		ADD_HANDLER_INFO( DZ_GUILD, DZ_GUILD_CHANGE_CORPS_NAK, PHASE_UNLIMITED ),
		ADD_HANDLER_INFO( DZ_GUILD, DZ_GUILD_CREATE_REAR_CAMP_ACK, PHASE_UNLIMITED ),	// 길드 훈련캠프 생성
		ADD_HANDLER_INFO( DZ_GUILD, DZ_GUILD_CREATE_REAR_CAMP_NAK, PHASE_UNLIMITED ),
		ADD_HANDLER_INFO( DZ_GUILD, DZ_GUILD_DESTROY_REAR_CAMP_ACK, PHASE_UNLIMITED ),	// 길드 훈련캠프 해산
		ADD_HANDLER_INFO( DZ_GUILD, DZ_GUILD_DESTROY_REAR_CAMP_NAK, PHASE_UNLIMITED ),
		ADD_HANDLER_INFO( DZ_GUILD, DZ_GUILD_LOGOUT_CHAR_CMD, PHASE_UNLIMITED ),		// 길드 멤버 로그아웃
		ADD_HANDLER_INFO( DZ_GUILD, DZ_GUILD_GM_GUILD_POINT_ACK, PHASE_UNLIMITED ),
		ADD_HANDLER_INFO( DZ_GUILD, DZ_GUILD_GM_GUILD_POINT_NAK, PHASE_UNLIMITED ),

		ADD_HANDLER_INFO( DZ_GUILD, DZ_GUILD_RELATION_ACK, PHASE_UNLIMITED ),
		ADD_HANDLER_INFO( DZ_GUILD, DZ_GUILD_RELATION_NAK, PHASE_UNLIMITED ),
		ADD_HANDLER_INFO( DZ_GUILD, DZ_GUILD_RELATION_DELETE_ACK, PHASE_UNLIMITED ),
		ADD_HANDLER_INFO( DZ_GUILD, DZ_GUILD_RELATION_DELETE_NAK, PHASE_UNLIMITED ),
		ADD_HANDLER_INFO( DZ_GUILD, DZ_GUILD_RELATION_BOTH_AGAINST_ACK, PHASE_UNLIMITED ),
		ADD_HANDLER_INFO( DZ_GUILD, DZ_GUILD_RELATION_BOTH_AGAINST_NAK, PHASE_UNLIMITED ),

		ADD_HANDLER_INFO( DZ_GUILD, DZ_GUILD_RECOVERY_GUILD_ACK, PHASE_UNLIMITED ),
		ADD_HANDLER_INFO( DZ_GUILD, DZ_GUILD_RECOVERY_GUILD_NAK, PHASE_UNLIMITED ),
		ADD_HANDLER_INFO( DZ_GUILD, DZ_GUILD_DESTROY_COMPLETE_ACK, PHASE_UNLIMITED ),
		ADD_HANDLER_INFO( DZ_GUILD, DZ_GUILD_DESTROY_COMPLETE_NAK, PHASE_UNLIMITED ),

        ADD_HANDLER_INFO( DZ_GUILD, DZ_GUILD_GUILDMARK_REGISTER_ACK, PHASE_UNLIMITED ),
		ADD_HANDLER_INFO( DZ_GUILD, DZ_GUILD_GUILDMARK_REGISTER_NAK, PHASE_UNLIMITED ),
		ADD_HANDLER_INFO( DZ_GUILD, DZ_GUILD_GUILDMARK_DELETE_ACK, PHASE_UNLIMITED ),
		ADD_HANDLER_INFO( DZ_GUILD, DZ_GUILD_GUILDMARK_DELETE_NAK, PHASE_UNLIMITED ),
		ADD_HANDLER_INFO( DZ_GUILD, DZ_GUILD_WAREHOUSE_OPEN_ACK, PHASE_UNLIMITED ),
		ADD_HANDLER_INFO( DZ_GUILD, DZ_GUILD_WAREHOUSE_OPEN_NAK, PHASE_UNLIMITED ),

		ADD_HANDLER_INFO( DZ_GUILD, DZ_GUILD_NAME_CHANGE_ACK, PHASE_UNLIMITED ),
		ADD_HANDLER_INFO( DZ_GUILD, DZ_GUILD_NAME_CHANGE_NAK, PHASE_UNLIMITED ),
        //{ _KR_001385_20091013_DOMINATION_BASICINFO_SETTING
        ADD_HANDLER_INFO( DZ_GUILD, DZ_DOMINATION_INFO_ACK, PHASE_UNLIMITED ),
        ADD_HANDLER_INFO( DZ_GUILD, DZ_DOMINATION_AUCTION_INFO_ACK, PHASE_UNLIMITED ),
        ADD_HANDLER_INFO( DZ_GUILD, DZ_DOMINATION_UPDATE_INFO_ACK, PHASE_UNLIMITED ),
        ADD_HANDLER_INFO( DZ_GUILD, DZ_DOMINATION_ACCEP_HEIM_ACK, PHASE_UNLIMITED ),
        ADD_HANDLER_INFO( DZ_GUILD, DZ_DOMINATION_INIT_AUCTIONINFO_ACK, PHASE_UNLIMITED ),
        //}
#ifdef _NA_20100604_BETTER_DOMINATION_MANAGER
        ADD_HANDLER_INFO( DZ_GUILD, DZ_DOMINATION_UPDATE_GUILDINFO_ACK, PHASE_UNLIMITED ),
#endif
#ifdef _NA_20110216_INCREASE_DOMINATION_DEFFENSEWALL_POINT
        ADD_HANDLER_INFO( DZ_GUILD, DZ_DOMINATION_ADD_DEFFENSEWALL_POINT_ACK, PHASE_UNLIMITED ),
#endif
#ifdef _NA_20110406_CHANGE_RULE_FOR_DOMINATION_JOIN
        ADD_HANDLER_INFO( DZ_GUILD, DZ_DOMINATION_MEMBER_JOIN_ACK, PHASE_UNLIMITED ),
        ADD_HANDLER_INFO( DZ_GUILD, DZ_DOMINATION_MEMBER_LEAVE_ACK, PHASE_UNLIMITED ),
        ADD_HANDLER_INFO( DZ_GUILD, DZ_DOMINATION_MEMBERLIST_ANS, PHASE_UNLIMITED ),
        ADD_HANDLER_INFO( DZ_GUILD, DZ_DOMINATION_MEMBERLIST_INIT_ACK, PHASE_UNLIMITED ),

#endif
		ADD_HANDLER_INFO_EOR(),
	};

	return s_pHandlerInfo;
};

VOID	Handler_FromDBProxyServer::RegisterHandler_ALL( PACKET_HANDLER_DELEGATOR pInstance )
{
	pInstance->RegisterHandler_<GAME_DBPROXY>( GetHandlerInfo() );
	pInstance->RegisterHandler_<GAME_DBPROXY>( PacketHandlerShared::GetHandlerInfo() );
}





Handler_FromDBProxyServer_IMPL(DZ_GUILD_CREATE_ACK)
{
	MSG_DZ_GUILD_CREATE_ACK * pRecvMsg = (MSG_DZ_GUILD_CREATE_ACK *)pMsg;

	GuildServer* pGuildServer = GuildServer::GetMainFrame();

	RC::eGUILD_RESULT rc = g_GuildManager.CreateGuild( pRecvMsg->m_GuildGuid, pRecvMsg->m_szGuildName, pRecvMsg->m_byCreateCorpsNum, 
		pRecvMsg->m_MasterCharGuid, pRecvMsg->m_szMasterName, pRecvMsg->m_MasterDuty, pRecvMsg->m_byMasterClass, pRecvMsg->m_MasterLevel, pRecvMsg->m_byChaoState,
		pRecvMsg->m_nGuildPenaltyTime, pRecvMsg->m_nDestroyRequestTime, pRecvMsg->m_nMasterChangeTime );
	if( rc != RC::RC_GUILD_SUCCESS )
	{
		MSG_GZ_GUILD_CREATE_NAK nakMsg;
		nakMsg.m_dwKey = pRecvMsg->m_dwKey;
		nakMsg.m_byErrorCode = rc;
		pGuildServer->SendToSession( pRecvMsg->m_dwSessionIndex, &nakMsg, sizeof(nakMsg) );
		SUNLOG( eCRITICAL_LOG, "[Handler_FromDBProxyServer_IMPL(DZ_GUILD_CREATE_ACK)] Fail!!, GuildGuid = %u, MasterName = %s",
			pRecvMsg->m_GuildGuid, pRecvMsg->m_szMasterName );
		return;
	}

	MSG_GZ_GUILD_CREATE_ACK amsg;
	amsg.m_dwKey = pRecvMsg->m_dwKey;
	g_GuildManager.SerializeGameGuildInfo( pRecvMsg->m_GuildGuid, amsg.m_GameGuildInfo, SERIALIZE_LOAD );
	g_GuildManager.GetGuildBaseMemberInfo( pRecvMsg->m_GuildGuid, pRecvMsg->m_MasterCharGuid, amsg.m_BaseMemberInfo );
	pGuildServer->SendToSession( pRecvMsg->m_dwSessionIndex, &amsg, sizeof( amsg ) );

	MSG_WZ_GUILD_CREATE_CMD cmsg;
	g_GuildManager.SerializeBaseGuildInfo( pRecvMsg->m_GuildGuid, cmsg.m_WorldGuildInfo, SERIALIZE_LOAD );
	g_GuildManager.GetGuildBaseMemberInfo( pRecvMsg->m_GuildGuid, pRecvMsg->m_MasterCharGuid, cmsg.m_BaseMemberInfo );
	pGuildServer->ConnectSessions.SendToWorld( &cmsg, sizeof( cmsg ) );
}

Handler_FromDBProxyServer_IMPL(DZ_GUILD_DESTROY_ACK)
{
	const MSG_DZ_GUILD_DESTROY_ACK* receive_msg = static_cast<MSG_DZ_GUILD_DESTROY_ACK*>(pMsg);

#ifdef _USING_GUILD_DESTROY_LOGGING
    SUNLOG(eCRITICAL_LOG, 
        _T("|길드해산신청|Handler_FromDBProxyServer_IMPL(DZ_GUILD_DESTROY_ACK)|MSG_DZ_GUILD_DESTROY_ACK 받음|GuildGuid:%d, DestroyTime:%u|"), 
        receive_msg->m_GuildGuid, 
        receive_msg->m_dwDestroyTime);
#endif // _USING_GUILD_DESTROY_LOGGING

	GuildSmallContentsEntry::GuildPenalty::SetGuildDeleteWaitMap(
        receive_msg->m_GuildGuid, receive_msg->m_dwDestroyTime);
	GuildSmallContentsEntry::RelationSystem::RemainRelationDestroy(receive_msg->m_GuildGuid);
	
    GuildServer* guild_server = GuildServer::GetMainFrame();

	MSG_GZ_GUILD_DESTROY_WAIT_BRD brd_msg;
	brd_msg.m_dwKey	= receive_msg->m_dwKey;
	brd_msg.m_GuildGuid = receive_msg->m_GuildGuid;

#ifdef _USING_GUILD_DESTROY_LOGGING
    SUNLOG(eCRITICAL_LOG, 
        _T("|길드해산신청|Handler_FromDBProxyServer_IMPL(DZ_GUILD_DESTROY_ACK)|MSG_GZ_GUILD_DESTROY_WAIT_BRD 보냄|Key:%u, GuildGuid:%d|"), 
           brd_msg.m_dwKey, 
           brd_msg.m_GuildGuid);
#endif // _USING_GUILD_DESTROY_LOGGING

	guild_server->SendToAllGameServers(&brd_msg, sizeof(brd_msg));

	MSG_WZ_GUILD_DESTROY_WAIT_CMD cmd_msg;
	cmd_msg.m_dwKey = receive_msg->m_dwKey;
	cmd_msg.m_GuildGuid = receive_msg->m_GuildGuid;

#ifdef _USING_GUILD_DESTROY_LOGGING
    SUNLOG(eCRITICAL_LOG, 
        _T("|길드해산신청|Handler_FromDBProxyServer_IMPL(DZ_GUILD_DESTROY_ACK)|MSG_WZ_GUILD_DESTROY_WAIT_CMD 보냄|Key:%u, GuildGuid:%d|"), 
           cmd_msg.m_dwKey, 
           cmd_msg.m_GuildGuid);
#endif // _USING_GUILD_DESTROY_LOGGING

	guild_server->ConnectSessions.SendToWorld(&cmd_msg, sizeof(cmd_msg));
}

Handler_FromDBProxyServer_IMPL(DZ_GUILD_JOIN_ACK)
{
	MSG_DZ_GUILD_JOIN_ACK* pRecvMsg = (MSG_DZ_GUILD_JOIN_ACK *)pMsg;

	GuildServer* pGuildServer = GuildServer::GetMainFrame();

	// 0,0,0은 아직 길드에서 필요 하지 않으거 같아서 저렇게 넣었다.
	// 사용되는 부분을 찾아야 한다.
	RC::eGUILD_RESULT rt = g_GuildManager.JoinGuild( pRecvMsg->m_GuildGuid, pRecvMsg->m_byCorpsNum, pRecvMsg->m_CharGuid, pRecvMsg->m_tszCharName, pRecvMsg->m_Duty, pRecvMsg->m_byClass, pRecvMsg->m_Level, pRecvMsg->m_byChaoState );
	if( rt != RC::RC_GUILD_SUCCESS )
	{
		MSG_GZ_GUILD_JOIN_NAK nakMsg;
		nakMsg.m_dwKey			= pRecvMsg->m_dwKey;
        strncpy(nakMsg.m_szTargetCharName, pRecvMsg->m_tszCharName,
                _countof(nakMsg.m_szTargetCharName));
        nakMsg.m_szTargetCharName[_countof(nakMsg.m_szTargetCharName) - 1] = '\0';
		nakMsg.m_byErrorCode	= rt;
		pGuildServer->SendToSession( pRecvMsg->m_dwSessionIndex, &nakMsg, sizeof(nakMsg) );
		return;
	}

	GuildMember* pMember = g_GuildManager.FindGuildMember( pRecvMsg->m_GuildGuid, pRecvMsg->m_CharGuid );
	if( !pMember )	return;

	//게임서버로 보낸다.
	MSG_GZ_GUILD_JOIN_BRD msg;
	msg.m_dwKey		= pRecvMsg->m_dwKey;
	msg.m_GuildGuid = pRecvMsg->m_GuildGuid;
	pMember->SerializeMemberInfo( msg.m_MemberInfo, SERIALIZE_LOAD );
	pGuildServer->SendToAllGameServers( &msg, sizeof(msg) );

	//월드서버로 보낸다.
	MSG_WZ_GUILD_JOIN_GUILD_CMD cmsg;
	cmsg.m_GuildGuid = pRecvMsg->m_GuildGuid;
	pMember->SerializeBaseMemberInfo( cmsg.m_Info, SERIALIZE_LOAD );
	pGuildServer->ConnectSessions.SendToWorld( &cmsg, sizeof(cmsg) );
}

Handler_FromDBProxyServer_IMPL(DZ_GUILD_WITHDRAW_ACK)
{
	MSG_DZ_GUILD_WITHDRAW_ACK * pRecvMsg = (MSG_DZ_GUILD_WITHDRAW_ACK *)pMsg;

	TCHAR szCharName[MAX_CHARNAME_LENGTH+1];
	g_GuildManager.GetCharName( pRecvMsg->m_GuildGuid, pRecvMsg->m_CharGuid, szCharName, MAX_CHARNAME_LENGTH );
	szCharName[MAX_CHARNAME_LENGTH] = '\0';

	RC::eGUILD_RESULT rc = g_GuildManager.LeaveGuild( pRecvMsg->m_GuildGuid, pRecvMsg->m_CharGuid );
	if( rc != RC::RC_GUILD_SUCCESS )
	{
		SUNLOG( eCRITICAL_LOG, "[Handler_FromDBProxyServer_IMPL(DZ_GUILD_WITHDRAW_ACK)] Fail, GuildGuid = %d, CharGuid = %d, rc = %d", 
			pRecvMsg->m_GuildGuid, pRecvMsg->m_CharGuid, rc );
		return;
	}

	MSG_GZ_GUILD_WITHDRAW_BRD msgBRD;
	msgBRD.m_dwKey		= pRecvMsg->m_dwKey;
	msgBRD.m_GuildGuid	= pRecvMsg->m_GuildGuid;
	msgBRD.m_CharGuid	= pRecvMsg->m_CharGuid;
    strncpy(msgBRD.m_szCharName, szCharName, _countof(msgBRD.m_szCharName));
    msgBRD.m_szCharName[_countof(msgBRD.m_szCharName) - 1] = '\0';
	msgBRD.m_MemberPenaltyTime = pRecvMsg->m_MemberPenaltyTime;
	msgBRD.m_byPenaltyType = pRecvMsg->m_byPenaltyType;

	GuildServer* pGuildServer = GuildServer::GetMainFrame();
	pGuildServer->SendToAllGameServers( &msgBRD, sizeof(msgBRD) );

	// Send To WorldServer
	MSG_WZ_GUILD_WITHDRAW_GUILD_CMD msgCMD;
	msgCMD.m_GuildGuid	= pRecvMsg->m_GuildGuid;
	msgCMD.m_CharGuid	= pRecvMsg->m_CharGuid;
	pGuildServer->ConnectSessions.SendToWorld( &msgCMD, sizeof(msgCMD) );
}

Handler_FromDBProxyServer_IMPL(DZ_GUILD_SELECT_ACK)
{
	MSG_DZ_GUILD_SELECT_ACK* pRecvMsg = (MSG_DZ_GUILD_SELECT_ACK*)pMsg;

	//로그인시 SELECT 호출이면...
	if( pRecvMsg->m_bySelectType == 1 )
	{
		GuildServer* pGuildServer = GuildServer::GetMainFrame();

		// 로그인시 select ack가 왔다는 것은 길드 서버에 해당 길드 정보가 없다는 뜻이다. 길드서버에서 새로 만든다.
		g_GuildManager.SerializeGuildInfo( pRecvMsg->m_GuildInfo.m_GuildGuid, pRecvMsg->m_GuildInfo, SERIALIZE_STORE );
		g_GuildManager.SerializeCorpsInfo( pRecvMsg->m_GuildInfo.m_GuildGuid, pRecvMsg->m_CorpsTotalInfo, SERIALIZE_STORE );
		g_GuildManager.SerializeServerMemberTotalInfo( pRecvMsg->m_GuildInfo.m_GuildGuid, pRecvMsg->m_MemberTotalInfo, SERIALIZE_STORE );
		g_GuildManager.Login( pRecvMsg->m_GuildInfo.m_GuildGuid, pRecvMsg->m_LoginCharGuid );
		g_GuildManager.SetGuildNotice( pRecvMsg->m_GuildInfo.m_GuildGuid, pRecvMsg->m_szGuildNotice );
        if (GuildSmallContentsEntry::GuildPenalty::IsGuildDestroyWaitState(
                pRecvMsg->m_GuildInfo.m_GuildGuid)) {
            return;
        }

		MSG_GZ_GUILD_LOGIN_CHAR_BRD	brdMsg;
		brdMsg.m_dwKey = pRecvMsg->m_dwKey;
		g_GuildManager.SerializeGuildInfo( pRecvMsg->m_GuildInfo.m_GuildGuid, brdMsg.m_GuildInfo, SERIALIZE_LOAD );
		g_GuildManager.SerializeCorpsInfo( pRecvMsg->m_GuildInfo.m_GuildGuid, brdMsg.m_CorpsTotalInfo, SERIALIZE_LOAD );
		g_GuildManager.GetGuildMemberInfo( pRecvMsg->m_GuildInfo.m_GuildGuid, pRecvMsg->m_LoginCharGuid, brdMsg.m_MemberInfo );
		pGuildServer->SendToAllGameServers( &brdMsg, sizeof(brdMsg) );

		MSG_WZ_GUILD_LOGIN_CHAR_CMD	cmd;
		g_GuildManager.GetWorldGuildInfo( pRecvMsg->m_GuildInfo.m_GuildGuid, cmd.m_WorldGuildInfo );
		g_GuildManager.GetGuildBaseMemberInfo( pRecvMsg->m_GuildInfo.m_GuildGuid, pRecvMsg->m_LoginCharGuid, cmd.m_BaseMemberInfo  );
		pGuildServer->ConnectSessions.SendToWorld( &cmd, sizeof(cmd) ); 
	}
	else
	{
		SUNLOG( eCRITICAL_LOG, "[Handler_FromDBProxyServer_IMPL(DZ_GUILD_SELECT_ACK)] 넌뭐냐? 아직 로그인시 빼곤 예외 상황 없다." );
	}
}



Handler_FromDBProxyServer_IMPL(DZ_GUILD_CREATE_NAK)
{
	MSG_DZ_GUILD_CREATE_NAK * pRecvMsg = (MSG_DZ_GUILD_CREATE_NAK *)pMsg;

	MSG_GZ_GUILD_CREATE_NAK msg;
	msg.m_dwKey = pRecvMsg->m_dwKey;
	msg.m_byErrorCode = pRecvMsg->m_byErrorCode;
	GuildServer* pGuildServer = GuildServer::GetMainFrame();
	pGuildServer->SendToSession( pRecvMsg->m_dwSessionIndex, &msg, sizeof(msg) );
}

Handler_FromDBProxyServer_IMPL(DZ_GUILD_DESTROY_NAK)
{
	MSG_DZ_GUILD_DESTROY_NAK * pRecvMsg = (MSG_DZ_GUILD_DESTROY_NAK *)pMsg;
	GuildServer* pGuildServer = GuildServer::GetMainFrame();
	if(!pGuildServer) return;

	MSG_GZ_GUILD_DESTROY_NAK msg;
	msg.m_dwKey = pRecvMsg->m_dwKey;
	msg.m_byErrorCode = pRecvMsg->m_byErrorCode;
	pGuildServer->SendToSession(pRecvMsg->m_dwSessionIndex, &msg, sizeof(msg));
}

Handler_FromDBProxyServer_IMPL(DZ_GUILD_JOIN_NAK)
{
	MSG_DZ_GUILD_JOIN_NAK * pRecvMsg = (MSG_DZ_GUILD_JOIN_NAK *)pMsg;

	MSG_GZ_GUILD_JOIN_NAK msg;
	msg.m_dwKey = pRecvMsg->m_dwKey;
	msg.m_byErrorCode = pRecvMsg->m_byErrorCode;
    strncpy(msg.m_szTargetCharName, pRecvMsg->m_tszCharName, _countof(msg.m_szTargetCharName));
    msg.m_szTargetCharName[_countof(msg.m_szTargetCharName) - 1] = '\0';

    GuildServer* pGuildServer = GuildServer::GetMainFrame();
	pGuildServer->SendToSession( pRecvMsg->m_dwSessionIndex, &msg, sizeof(msg) );
}

Handler_FromDBProxyServer_IMPL(DZ_GUILD_WITHDRAW_NAK)
{
	MSG_DZ_GUILD_WITHDRAW_NAK * pRecvMsg = (MSG_DZ_GUILD_WITHDRAW_NAK *)pMsg;
}

Handler_FromDBProxyServer_IMPL(DZ_GUILD_SELECT_NAK)
{
	MSG_DZ_GUILD_SELECT_NAK * pRecvMsg = (MSG_DZ_GUILD_SELECT_NAK *)pMsg;
}

// 강퇴 결과
Handler_FromDBProxyServer_IMPL(DZ_GUILD_KICKPLAYER_ACK)
{
	MSG_DZ_GUILD_KICKPLAYER_ACK* pRecvMsg = (MSG_DZ_GUILD_KICKPLAYER_ACK*)pMsg;

	TCHAR szCharName[MAX_CHARNAME_LENGTH+1];
	g_GuildManager.GetCharName( pRecvMsg->m_GuildGuid, pRecvMsg->m_TargetCharGuid, szCharName, MAX_CHARNAME_LENGTH );
	szCharName[MAX_CHARNAME_LENGTH] = '\0';

	RC::eGUILD_RESULT rc = g_GuildManager.KickMember( pRecvMsg->m_GuildGuid, pRecvMsg->m_OperCharGuid, pRecvMsg->m_TargetCharGuid );
	if( rc != RC::RC_GUILD_SUCCESS )
	{
		SUNLOG( eCRITICAL_LOG, "[Handler_FromDBProxyServer_IMPL(DZ_GUILD_KICKPLAYER_ACK)] Fail, GuildGuid = %u, CharGuid = %u, Result = %d",
			pRecvMsg->m_GuildGuid, pRecvMsg->m_TargetCharGuid, rc );
		return;
	}

	MSG_GZ_GUILD_KICKPLAYER_BRD brdACK;
	brdACK.m_dwKey		= pRecvMsg->m_dwKey;
	brdACK.m_GuildGuid	= pRecvMsg->m_GuildGuid;
	brdACK.m_CharGuid	= pRecvMsg->m_TargetCharGuid;
	strncpy( brdACK.m_ptszCharName, szCharName, MAX_CHARNAME_LENGTH );
	brdACK.m_ptszCharName[MAX_CHARNAME_LENGTH]='\0';
	brdACK.m_GuildPenaltyTime	= pRecvMsg->m_GuildPenaltyTime;
	brdACK.m_MemberPenaltyTime	= pRecvMsg->m_MemberPenaltyTime;
	GuildServer* pGuildServer = GuildServer::GetMainFrame();
	pGuildServer->SendToAllGameServers( &brdACK, sizeof(brdACK) );

	MSG_WZ_GUILD_KICK_PLAYER_CMD msgCMD;
	msgCMD.m_GuildGuid		= pRecvMsg->m_GuildGuid;
	msgCMD.m_KickCharGuid	= pRecvMsg->m_TargetCharGuid;
	pGuildServer->ConnectSessions.SendToWorld(&msgCMD, sizeof(msgCMD));
}

Handler_FromDBProxyServer_IMPL(DZ_GUILD_KICKPLAYER_NAK)
{
	MSG_DZ_GUILD_KICKPLAYER_NAK* pRecvMsg = (MSG_DZ_GUILD_KICKPLAYER_NAK*)pMsg;
}

// 길드 공지 변경
Handler_FromDBProxyServer_IMPL(DZ_GUILD_CHANGE_NOTICE_ACK)
{
	PACKET_PTR_CONVERT(MSG_DZ_GUILD_CHANGE_NOTICE_ACK, pRecvMsg, pMsg);
	GuildServer* pGuildServer = GuildServer::GetMainFrame();
	if(!pGuildServer) return;

	g_GuildManager.SetGuildNotice( pRecvMsg->m_GuildGuid, pRecvMsg->m_tszGuildNotice );
	
	MSG_WZ_GUILD_CHANGE_NOTICE_CMD	msg;
	msg.m_eNoticeType = eGUILD_NOTICE_GUILD;
	msg.m_GuildGuid = pRecvMsg->m_GuildGuid;
	msg.m_wNoticeLen = (WORD)_tcslen(pRecvMsg->m_tszGuildNotice);
	strncpy(msg.m_szGuildNotice, pRecvMsg->m_tszGuildNotice, MAX_GUILDNOTICE_LENGTH );
	msg.m_szGuildNotice[MAX_GUILDNOTICE_LENGTH] = '\0';
	pGuildServer->ConnectSessions.SendToWorld(&msg, msg.GetSize());
}

Handler_FromDBProxyServer_IMPL(DZ_GUILD_CHANGE_NOTICE_NAK)
{
	PACKET_PTR_CONVERT(MSG_DZ_GUILD_CHANGE_NOTICE_NAK, pRecvMsg, pMsg);
	GuildServer* pGuildServer = GuildServer::GetMainFrame();
	if(!pGuildServer) return;

	MSG_WZ_GUILD_CHANGE_NOTICE_NAK	msg;
	//msg.m_OperCharGuid = pRecvMsg->m_OperCharGuid;
	msg.m_byErrorCode = pRecvMsg->m_byErrorCode;
	pGuildServer->ConnectSessions.SendToWorld(&msg, sizeof(msg));
}

////////////////
// 길드 포인트 관련

// UP 적립
Handler_FromDBProxyServer_IMPL(DZ_GUILD_DONATE_UP_ACK)
{
	MSG_DZ_GUILD_DONATE_UP_ACK	*pRecvMsg = (MSG_DZ_GUILD_DONATE_UP_ACK*)pMsg;

	RC::eGUILD_RESULT rc = g_GuildManager.DonateUP( pRecvMsg->m_GuildGuid, pRecvMsg->m_TotalUP, pRecvMsg->m_OperCharGuid, pRecvMsg->m_DonateUP );

	if( rc != RC::RC_GUILD_SUCCESS )
	{
		SUNLOG( eCRITICAL_LOG, "[Handler_FromDBProxyServer_IMPL(DZ_GUILD_DONATE_UP_ACK)] Error! GuildGuid = %u, UP = %d", pRecvMsg->m_GuildGuid, pRecvMsg->m_TotalUP );
		return;
	}

	MSG_GZ_GUILD_DONATE_UP_BRD	sendMsg;
	sendMsg.m_dwKey = pRecvMsg->m_dwKey;
	sendMsg.m_GuildGuid = pRecvMsg->m_GuildGuid;
	sendMsg.m_TotalUP = pRecvMsg->m_TotalUP;
	sendMsg.m_DonateUP = pRecvMsg->m_DonateUP;

	GuildServer* pGuildServer = GuildServer::GetMainFrame();
	pGuildServer->SendToAllGameServers( &sendMsg, sizeof(sendMsg) );

}

Handler_FromDBProxyServer_IMPL(DZ_GUILD_DONATE_UP_NAK)
{
	MSG_DZ_GUILD_DONATE_UP_NAK	*pRecvMsg = (MSG_DZ_GUILD_DONATE_UP_NAK*)pMsg;

	MSG_GZ_GUILD_DONATE_UP_NAK	nakMsg;
	nakMsg.m_dwKey = pRecvMsg->m_dwKey;
	nakMsg.m_byErrorCode = pRecvMsg->m_byErrorCode;

	GuildServer* pGuildServer = GuildServer::GetMainFrame();
	pGuildServer->SendToSession( pRecvMsg->m_dwSessionIndex, &nakMsg, sizeof(nakMsg) );
}

// UP->GP 변환
Handler_FromDBProxyServer_IMPL(DZ_GUILD_CONVERT_UP2GP_ACK)
{
	MSG_DZ_GUILD_CONVERT_UP2GP_ACK	*pRecvMsg = (MSG_DZ_GUILD_CONVERT_UP2GP_ACK*)pMsg;

	g_GuildManager.SetPoint( pRecvMsg->m_GuildGuid, pRecvMsg->m_TotalGP, pRecvMsg->m_RemainUP );

	MSG_GZ_GUILD_CONVERT_UP2GP_BRD	sendMsg;
	sendMsg.m_dwKey = pRecvMsg->m_dwKey;
	sendMsg.m_GuildGuid = pRecvMsg->m_GuildGuid;
	sendMsg.m_RemainUP = pRecvMsg->m_RemainUP;
	sendMsg.m_TotalGP = pRecvMsg->m_TotalGP;
	GuildServer* pGuildServer = GuildServer::GetMainFrame();
	pGuildServer->SendToAllGameServers( &sendMsg, sizeof(sendMsg) );
}

Handler_FromDBProxyServer_IMPL(DZ_GUILD_CONVERT_UP2GP_NAK)
{
	MSG_DZ_GUILD_CONVERT_UP2GP_NAK	*pRecvMsg = (MSG_DZ_GUILD_CONVERT_UP2GP_NAK*)pMsg;
	MSG_GZ_GUILD_CONVERT_UP2GP_NAK	nakMsg;
	nakMsg.m_dwKey = pRecvMsg->m_dwKey;
	nakMsg.m_byErrorCode = pRecvMsg->m_byErrorCode;

	GuildServer* pGuildServer = GuildServer::GetMainFrame();
	pGuildServer->SendToSession( pRecvMsg->m_dwSessionIndex, &nakMsg, sizeof(nakMsg) );
}

Handler_FromDBProxyServer_IMPL(DZ_GUILD_RANKUP_ACK)
{
	MSG_DZ_GUILD_RANKUP_ACK	*pRecvMsg = (MSG_DZ_GUILD_RANKUP_ACK*)pMsg;

	g_GuildManager.RankUpGuild( pRecvMsg->m_GuildGuid );

	MSG_GZ_GUILD_RANKUP_BRD	sendMsg;
	sendMsg.m_dwKey = pRecvMsg->m_dwKey;
	sendMsg.m_GuildGuid = pRecvMsg->m_GuildGuid;
	sendMsg.m_byLevel = (BYTE)pRecvMsg->m_NowLevel;
	sendMsg.m_RemainGP = pRecvMsg->m_RemainGP;
	sendMsg.m_RemainUP = pRecvMsg->m_RemainUP;

	GuildServer* pGuildServer = GuildServer::GetMainFrame();
	pGuildServer->SendToAllGameServers( &sendMsg, sizeof(sendMsg) );
}


Handler_FromDBProxyServer_IMPL(DZ_GUILD_RANKUP_NAK)
{
	MSG_DZ_GUILD_RANKUP_NAK	*pRecvMsg = (MSG_DZ_GUILD_RANKUP_NAK*)pMsg;

	g_GuildManager.SetGuildTransaction( pRecvMsg->m_GuildGuid, GUILD_TRANS_RANKUP_END );

	MSG_GZ_GUILD_RANKUP_NAK	nakMsg;
	nakMsg.m_dwKey = pRecvMsg->m_dwKey;
	nakMsg.m_byErrorCode = pRecvMsg->m_byErrorCode;

	GuildServer* pGuildServer = GuildServer::GetMainFrame();
	pGuildServer->SendToSession( pRecvMsg->m_dwSessionIndex, &nakMsg, sizeof(nakMsg) );

	DISPMSG( "DZ_GUILD_RANKUP_NAK[GuildGuid:%u][UserKey:%u]\n", pRecvMsg->m_GuildGuid, pRecvMsg->m_dwKey );
}

#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
Handler_FromDBProxyServer_IMPL(DZ_GUILD_CHUNTING_PVP_RESULT_ACK)
{
	MSG_DZ_GUILD_CHUNTING_PVP_RESULT_ACK * pRecvMsg = (MSG_DZ_GUILD_CHUNTING_PVP_RESULT_ACK *)pMsg;

	MSG_GZ_GUILD_CHUNTING_PVP_RESULT_BRD msg;
	msg.m_dwKey		= pRecvMsg->m_dwKey;
	msg.m_GuildGuid = pRecvMsg->m_GuildGuid;
	msg.m_PVPInfo	= pRecvMsg->m_PVPInfo;
	GuildServer* pGuildServer = GuildServer::GetMainFrame();
	pGuildServer->SendToAllGameServers( &msg, sizeof(msg) );

	// Send To WorldServer
	MSG_WZ_GUILD_CHUNTING_PVP_RESULT_CMD cmsg;
	cmsg.m_GuildGuid = pRecvMsg->m_GuildGuid;
	cmsg.m_PVPInfo	= pRecvMsg->m_PVPInfo;
	pGuildServer->ConnectSessions.SendToWorld( &cmsg, sizeof(cmsg) );
}
#endif



//길드 직책 ack
Handler_FromDBProxyServer_IMPL(DZ_GUILD_CHANGE_DUTY_ACK)
{
	MSG_DZ_GUILD_CHANGE_DUTY_ACK *pRecvMsg = (MSG_DZ_GUILD_CHANGE_DUTY_ACK *)pMsg;

	if( !g_GuildManager.ChangeMemberDuty( pRecvMsg->m_GuildGuid, pRecvMsg->m_TargetCharGuid, pRecvMsg->m_eTargetChangeDuty ) )
	{
		//실패했다.. NAK는 보내지 말고 실패 로그는 내부에서 찍는다.
		return;
	}


	//GAME서버로 보낸다.
	MSG_GZ_GUILD_CHANGE_DUTY_BRD gsMSG;
	gsMSG.m_dwKey			= pRecvMsg->m_dwKey;
	gsMSG.m_GuildGuid		= pRecvMsg->m_GuildGuid;
	g_GuildManager.GetCharName( pRecvMsg->m_GuildGuid, pRecvMsg->m_TargetCharGuid, gsMSG.m_szTargetCharName, MAX_CHARNAME_LENGTH );
	gsMSG.m_byDuty			= pRecvMsg->m_eTargetChangeDuty;

	//모든 게임서버가 모든 길드원의 정보를 모두 가지고 있는가?
	GuildServer* pGuildServer = GuildServer::GetMainFrame();
	pGuildServer->SendToAllGameServers( &gsMSG, sizeof(gsMSG) );

	//월드서버로 보낸다.
	MSG_WZ_GUILD_CHANGE_DUTY_CMD wsMSG;
	wsMSG.m_dwKey			= pRecvMsg->m_dwKey;
	wsMSG.m_GuildGuid		= pRecvMsg->m_GuildGuid;
	wsMSG.m_TargetCharGuid	= pRecvMsg->m_TargetCharGuid;
	wsMSG.m_eGuildDuty		= pRecvMsg->m_eTargetChangeDuty;
	pGuildServer->ConnectSessions.SendToWorld( &wsMSG, sizeof(wsMSG) );

	//트랜잭션 종료
	g_GuildManager.SetGuildTransaction( pRecvMsg->m_GuildGuid, GUILD_TRANS_CHANGE_DUTY_END );
}

//길드 직책 nak
Handler_FromDBProxyServer_IMPL(DZ_GUILD_CHANGE_DUTY_NAK)
{
	MSG_DZ_GUILD_CHANGE_DUTY_NAK *pRecvMsg = (MSG_DZ_GUILD_CHANGE_DUTY_NAK *)pMsg;

	MSG_GZ_GUILD_CHANGE_DUTY_NAK nakMsg;
	nakMsg.m_dwKey			= pRecvMsg->m_dwKey;
	nakMsg.m_byErrorCode	= RC::RC_GUILD_CHANGE_MEMBER_DUTY_FAILED;
	GuildServer* pGuildServer = GuildServer::GetMainFrame();
	pGuildServer->SendToAllGameServers( &nakMsg, sizeof(MSG_GZ_GUILD_CHANGE_DUTY_NAK) );

	//트랜잭션 종료
	g_GuildManager.SetGuildTransaction( pRecvMsg->m_GuildGuid, GUILD_TRANS_CHANGE_DUTY_END );
}

Handler_FromDBProxyServer_IMPL(DZ_GUILD_RIGHT_DELEGATION_ACK)
{
	MSG_DZ_GUILD_RIGHT_DELEGATION_ACK* pRecvMsg = (MSG_DZ_GUILD_RIGHT_DELEGATION_ACK*)pMsg;

	g_GuildManager.ChangeDelegateRight( pRecvMsg->m_GuildGuid, pRecvMsg->m_TargetGuid, pRecvMsg->m_DelegateRight );

	//위임된 권한은 지시자와 당자사만 알고 있으면 된다.
	MSG_GZ_GUILD_RIGHT_DELEGATION_BRD gsMsg;
	gsMsg.m_dwKey			= pRecvMsg->m_dwKey;
	gsMsg.m_GuildGuid		= pRecvMsg->m_GuildGuid;
	g_GuildManager.GetCharName( pRecvMsg->m_GuildGuid, pRecvMsg->m_TargetGuid, gsMsg.m_szTargetCharName, MAX_CHARNAME_LENGTH );
	CopyMemory( gsMsg.m_DelegateRight, pRecvMsg->m_DelegateRight, sizeof(gsMsg.m_DelegateRight) );
	GuildServer* pGuildServer = GuildServer::GetMainFrame();
	pGuildServer->SendToAllGameServers( &gsMsg, sizeof(gsMsg) );
}

//길드 직책 nak
Handler_FromDBProxyServer_IMPL(DZ_GUILD_RIGHT_DELEGATION_NAK)
{
	MSG_DZ_GUILD_RIGHT_DELEGATION_NAK* pRecvMsg = (MSG_DZ_GUILD_RIGHT_DELEGATION_NAK*)pMsg;

	MSG_GZ_GUILD_RIGHT_DELEGATION_NAK nakMsg;
	nakMsg.m_dwKey			= pRecvMsg->m_dwKey;
	nakMsg.m_byErrorCode	= RC::RC_GUILD_CHANGE_MEMBER_RIGHT_FAILED;
	GuildServer* pGuildServer = GuildServer::GetMainFrame();
	pGuildServer->SendToAllGameServers( &nakMsg, sizeof(MSG_GZ_GUILD_RIGHT_DELEGATION_NAK) );
}

Handler_FromDBProxyServer_IMPL(DZ_GUILD_CHANGE_MASTER_ACK)
{
	MSG_DZ_GUILD_CHANGE_MASTER_ACK* pRecvMsg = (MSG_DZ_GUILD_CHANGE_MASTER_ACK*)pMsg;
	GuildServer* pGuildServer = GuildServer::GetMainFrame();
	if( !pGuildServer )		return;

	if( !g_GuildManager.ChangeMaster( pRecvMsg->m_GuildGuid, pRecvMsg->m_OldMasterCharGuid, pRecvMsg->m_NewMasterCharGuid, pRecvMsg->m_nMasterChangeTime ) )
	{
		// 여기로 오면 안된다.. 로그는 위 함수 안에서 남긴다.
		return;
	}

	//GAME서버로 보낸다.
	MSG_GZ_GUILD_CHANGE_MASTER_BRD gsMSG;
	gsMSG.m_dwKey				= pRecvMsg->m_dwKey;
	gsMSG.m_GuildGuid			= pRecvMsg->m_GuildGuid;
	gsMSG.m_OldMasterCharGuid	= pRecvMsg->m_OldMasterCharGuid;
	g_GuildManager.GetCharName( pRecvMsg->m_GuildGuid, pRecvMsg->m_OldMasterCharGuid, gsMSG.m_szOldMasterCharName, MAX_CHARNAME_LENGTH );
	gsMSG.m_NewMasterCharGuid	= pRecvMsg->m_NewMasterCharGuid;
	g_GuildManager.GetCharName( pRecvMsg->m_GuildGuid, pRecvMsg->m_NewMasterCharGuid, gsMSG.m_szNewMasterCharName, MAX_CHARNAME_LENGTH );
	pGuildServer->SendToAllGameServers( &gsMSG, sizeof(gsMSG) );

	//월드서버로 보낸다.
	MSG_WZ_GUILD_CHANGE_MASTER_CMD wsMSG;
	wsMSG.m_dwKey			= pRecvMsg->m_dwKey;
	wsMSG.m_GuildGuid		= pRecvMsg->m_GuildGuid;
	wsMSG.m_OldMasterGuid	= pRecvMsg->m_OldMasterCharGuid;
	wsMSG.m_NewMasterGuid	= pRecvMsg->m_NewMasterCharGuid;
	pGuildServer->ConnectSessions.SendToWorld( &wsMSG, sizeof(wsMSG) );
}

//길드 직책 nak
Handler_FromDBProxyServer_IMPL(DZ_GUILD_CHANGE_MASTER_NAK)
{
	MSG_DZ_GUILD_CHANGE_MASTER_NAK* pRecvMsg = (MSG_DZ_GUILD_CHANGE_MASTER_NAK *)pMsg;

	MSG_GZ_GUILD_CHANGE_MASTER_NAK nakMsg;
	nakMsg.m_dwKey			= pRecvMsg->m_dwKey;
	nakMsg.m_byErrorCode	= pRecvMsg->m_byErrorCode;
	GuildServer* pGuildServer = GuildServer::GetMainFrame();
	pGuildServer->SendToSession( pRecvMsg->m_dwSessionIndex, &nakMsg, sizeof(nakMsg) );

}

// 길드 군단 생성
Handler_FromDBProxyServer_IMPL(DZ_GUILD_CREATE_CORPS_ACK)		
{
	MSG_DZ_GUILD_CREATE_CORPS_ACK* pRecvMsg = (MSG_DZ_GUILD_CREATE_CORPS_ACK*)pMsg;
	GuildServer* pGuildServer = GuildServer::GetMainFrame();
	if( !pGuildServer )		return;

	//현재 
	RC::eGUILD_RESULT rc = g_GuildManager.CreateCorps( pRecvMsg->m_GuildGuid, pRecvMsg->m_byCorpsNum, pRecvMsg->m_TargetCharGuid );
	if( rc != RC::RC_GUILD_SUCCESS )
	{
		MSG_GZ_GUILD_CREATE_CORPS_NAK nakMSG;
		nakMSG.m_dwKey			= pRecvMsg->m_dwKey;
		nakMSG.m_byErrorCode	= rc;
		pGuildServer->SendToSession( pRecvMsg->m_dwSessionIndex, &nakMSG, sizeof(nakMSG) );
		SUNLOG( eCRITICAL_LOG, "[DZ_GUILD_CREATE_CORPS_ACK] DB Success But GuildServer Fail, Guild = %u, CorpsNum = %d", pRecvMsg->m_GuildGuid, pRecvMsg->m_byCorpsNum );
		return;
	}

	GuildMember* pCommander = g_GuildManager.FindGuildMember( pRecvMsg->m_GuildGuid, pRecvMsg->m_TargetCharGuid );
	if( !pCommander )		return;

	//GAME서버로 보낸다. ==> 게임서버에서는 바로 유저에게 통과시킨다.
	MSG_GZ_GUILD_CREATE_CORPS_BRD brdMSG;
	brdMSG.m_dwKey			= pRecvMsg->m_dwKey;
	brdMSG.m_GuildGuid		= pRecvMsg->m_GuildGuid;
	brdMSG.m_byCorpsNum		= pRecvMsg->m_byCorpsNum;
    strncpy(brdMSG.m_szTargetCharName, pCommander->GetCharName(),
            _countof(brdMSG.m_szTargetCharName));
    brdMSG.m_szTargetCharName[_countof(brdMSG.m_szTargetCharName) - 1] = '\0';
	pGuildServer->SendToAllGameServers( &brdMSG, sizeof(brdMSG) );
}

Handler_FromDBProxyServer_IMPL(DZ_GUILD_CREATE_CORPS_NAK)
{
	MSG_DZ_GUILD_CREATE_CORPS_NAK* pRecvMsg = (MSG_DZ_GUILD_CREATE_CORPS_NAK*)pMsg;
	GuildServer* pGuildServer = GuildServer::GetMainFrame();
	if( !pGuildServer )		return;

	MSG_GZ_GUILD_CREATE_CORPS_NAK nakMSG;
	nakMSG.m_dwKey			= pRecvMsg->m_dwKey;
	nakMSG.m_byErrorCode	= pRecvMsg->m_byErrorCode;
	pGuildServer->SendToSession( pRecvMsg->m_dwSessionIndex, &nakMSG, sizeof(nakMSG) );
}

// 길드 군단 해산
Handler_FromDBProxyServer_IMPL(DZ_GUILD_DESTROY_CORPS_ACK)		
{
	MSG_DZ_GUILD_DESTROY_CORPS_ACK* pRecvMsg = (MSG_DZ_GUILD_DESTROY_CORPS_ACK*)pMsg;
	GuildServer* pGuildServer = GuildServer::GetMainFrame();
	if( !pGuildServer )		return;

	//현재 
	RC::eGUILD_RESULT rc = g_GuildManager.DestoryCorps( pRecvMsg->m_GuildGuid, pRecvMsg->m_byCorpsNum, pRecvMsg->m_OperCharGuid );

	if( rc != RC::RC_GUILD_SUCCESS )
	{
		MSG_GZ_GUILD_DESTROY_CORPS_NAK nakMSG;
		nakMSG.m_dwKey			= pRecvMsg->m_dwKey;
		nakMSG.m_byErrorCode	= rc;
		pGuildServer->SendToSession( pRecvMsg->m_dwSessionIndex, &nakMSG, sizeof(nakMSG) );
		SUNLOG( eCRITICAL_LOG, "[DZ_GUILD_DESTROY_CORPS_ACK] DB Success But GuildServer Fail, Guild = %u, CorpsNum = %d", pRecvMsg->m_GuildGuid, pRecvMsg->m_byCorpsNum );
		return;
	}

	//GAME서버로 보낸다. ==> 게임서버에는 보낼 필요가 없을거 같다.
	MSG_GZ_GUILD_DESTROY_CORPS_BRD brdMSG;
	brdMSG.m_dwKey			= pRecvMsg->m_dwKey;
	brdMSG.m_GuildGuid		= pRecvMsg->m_GuildGuid;
	brdMSG.m_byCorpsNum		= pRecvMsg->m_byCorpsNum;
	pGuildServer->SendToAllGameServers( &brdMSG, sizeof(brdMSG) );
}

Handler_FromDBProxyServer_IMPL(DZ_GUILD_DESTROY_CORPS_NAK)
{
	MSG_DZ_GUILD_DESTROY_CORPS_NAK* pRecvMsg = (MSG_DZ_GUILD_DESTROY_CORPS_NAK*)pMsg;
	GuildServer* pGuildServer = GuildServer::GetMainFrame();
	if( !pGuildServer )		return;

	MSG_GZ_GUILD_DESTROY_CORPS_NAK nakMSG;
	nakMSG.m_dwKey			= pRecvMsg->m_dwKey;
	nakMSG.m_byErrorCode	= pRecvMsg->m_byErrorCode;
	pGuildServer->SendToSession( pRecvMsg->m_dwSessionIndex, &nakMSG, sizeof(nakMSG) );
}

// 길드 군단장 변경
Handler_FromDBProxyServer_IMPL(DZ_GUILD_CHANGE_COMMANDER_ACK)	
{
	MSG_DZ_GUILD_CHANGE_COMMANDER_ACK* pRecvMsg = (MSG_DZ_GUILD_CHANGE_COMMANDER_ACK*)pMsg;
	GuildServer* pGuildServer = GuildServer::GetMainFrame();
	if( !pGuildServer )		return;

	RC::eGUILD_RESULT rc = g_GuildManager.ChangeCommander( pRecvMsg->m_GuildGuid, pRecvMsg->m_byCorpsNum, pRecvMsg->m_TargetGuid );
	if( rc != RC::RC_GUILD_SUCCESS )
	{
		MSG_GZ_GUILD_CHANGE_COMMANDER_NAK nakMSG;
		nakMSG.m_dwKey			= pRecvMsg->m_dwKey;
		nakMSG.m_byErrorCode	= rc;
		pGuildServer->SendToSession( pRecvMsg->m_dwSessionIndex, &nakMSG, sizeof(nakMSG) );
		SUNLOG( eCRITICAL_LOG, "[DZ_GUILD_CHANGE_COMMANDER_ACK] DB Success But GuildServer Fail, Guild = %u, CorpsNum = %d, CharGuid = %u, rc = %d",
			pRecvMsg->m_GuildGuid, pRecvMsg->m_byCorpsNum, pRecvMsg->m_TargetGuid, rc );
		return;
	}

	GuildMember* pMember = g_GuildManager.FindGuildMember( pRecvMsg->m_GuildGuid, pRecvMsg->m_TargetGuid );
	if( !pMember )	return;

	//GAME서버로 보낸다. 
	MSG_GZ_GUILD_CHANGE_COMMANDER_BRD brdMSG;
	brdMSG.m_dwKey			= pRecvMsg->m_dwKey;
	brdMSG.m_GuildGuid		= pRecvMsg->m_GuildGuid;
	brdMSG.m_byCorpsNum		= pRecvMsg->m_byCorpsNum;
    strncpy(brdMSG.m_szTargetCharName, pMember->GetCharName(), _countof(brdMSG.m_szTargetCharName));
    brdMSG.m_szTargetCharName[_countof(brdMSG.m_szTargetCharName) - 1] = '\0';
	pGuildServer->SendToAllGameServers( &brdMSG, sizeof(brdMSG) );

	//월드서버로 보낸다.
	MSG_WZ_GUILD_CHANGE_COMMANDER_CMD wsMSG;
	wsMSG.m_dwKey			= pRecvMsg->m_dwKey;
	wsMSG.m_GuildGuid		= pRecvMsg->m_GuildGuid;
	wsMSG.m_byCorpsNum		= pRecvMsg->m_byCorpsNum;
	wsMSG.m_TargetCharGuid  = pRecvMsg->m_TargetGuid;
	pGuildServer->ConnectSessions.SendToWorld( &wsMSG, sizeof(wsMSG) );
}

Handler_FromDBProxyServer_IMPL(DZ_GUILD_CHANGE_COMMANDER_NAK)
{
	MSG_DZ_GUILD_CHANGE_COMMANDER_NAK* pRecvMsg = (MSG_DZ_GUILD_CHANGE_COMMANDER_NAK*)pMsg;
	GuildServer* pGuildServer = GuildServer::GetMainFrame();
	if( !pGuildServer )		return;

	MSG_GZ_GUILD_CHANGE_COMMANDER_NAK nakMSG;
	nakMSG.m_dwKey			= pRecvMsg->m_dwKey;
	nakMSG.m_byErrorCode	= pRecvMsg->m_byErrorCode;
	pGuildServer->SendToSession( pRecvMsg->m_dwSessionIndex, &nakMSG, sizeof(nakMSG) );
}

// 길드 군단 이동
Handler_FromDBProxyServer_IMPL(DZ_GUILD_CHANGE_CORPS_ACK)		
{
	MSG_DZ_GUILD_CHANGE_CORPS_ACK* pRecvMsg = (MSG_DZ_GUILD_CHANGE_CORPS_ACK*)pMsg;
	GuildServer* pGuildServer = GuildServer::GetMainFrame();
	if( !pGuildServer )		return;

	//현재 
	RC::eGUILD_RESULT rc = g_GuildManager.ChangeCorps( pRecvMsg->m_GuildGuid, pRecvMsg->m_TargetGuid, pRecvMsg->m_byCorpsNum );
	if( rc != RC::RC_GUILD_SUCCESS )
	{
		MSG_GZ_GUILD_CHANGE_CORPS_NAK nakMSG;
		nakMSG.m_dwKey			= pRecvMsg->m_dwKey;
		nakMSG.m_byErrorCode	= rc;
		pGuildServer->SendToSession( pRecvMsg->m_dwSessionIndex, &nakMSG, sizeof(nakMSG) );
		SUNLOG( eCRITICAL_LOG, "[DZ_GUILD_CHANGE_CORPS_ACK] DB Success But GuildServer Fail, Guild = %u, CorpsNum = %d", pRecvMsg->m_GuildGuid, pRecvMsg->m_byCorpsNum );
		return;
	}

	GuildMember* pMember = g_GuildManager.FindGuildMember( pRecvMsg->m_GuildGuid, pRecvMsg->m_TargetGuid );
	if( !pMember )		return;

	//GAME서버로 보낸다.
	MSG_GZ_GUILD_CHANGE_CORPS_BRD brdMSG;
	brdMSG.m_dwKey			= pRecvMsg->m_dwKey;
	brdMSG.m_GuildGuid		= pRecvMsg->m_GuildGuid;
	brdMSG.m_TargetCharGuid	= pRecvMsg->m_TargetGuid;
	brdMSG.m_byCorpsNum		= pRecvMsg->m_byCorpsNum;
    strncpy(brdMSG.m_szTargetCharName, pMember->GetCharName(), _countof(brdMSG.m_szTargetCharName));
    brdMSG.m_szTargetCharName[_countof(brdMSG.m_szTargetCharName) - 1] = '\0';
	pGuildServer->SendToAllGameServers( &brdMSG, sizeof(brdMSG) );

	//월드서버로 보낸다.
	MSG_WZ_GUILD_CHANGE_CORPS_CMD wsMSG;
	wsMSG.m_dwKey			= pRecvMsg->m_dwKey;
	wsMSG.m_GuildGuid		= pRecvMsg->m_GuildGuid;
	wsMSG.m_byCorpsNum		= pRecvMsg->m_byCorpsNum;
	wsMSG.m_TargetCharGuid	= pRecvMsg->m_TargetGuid;
	pGuildServer->ConnectSessions.SendToWorld( &wsMSG, sizeof(wsMSG) );

}

Handler_FromDBProxyServer_IMPL(DZ_GUILD_CHANGE_CORPS_NAK)
{
	MSG_DZ_GUILD_CHANGE_CORPS_NAK* pRecvMsg = (MSG_DZ_GUILD_CHANGE_CORPS_NAK*)pMsg;
	GuildServer* pGuildServer = GuildServer::GetMainFrame();
	if( !pGuildServer )		return;

	MSG_GZ_GUILD_CHANGE_CORPS_NAK nakMSG;
	nakMSG.m_dwKey			= pRecvMsg->m_dwKey;
	nakMSG.m_byErrorCode	= pRecvMsg->m_byErrorCode;
	pGuildServer->SendToSession( pRecvMsg->m_dwSessionIndex, &nakMSG, sizeof(nakMSG) );
}

// 길드 훈련캠프 생성
Handler_FromDBProxyServer_IMPL(DZ_GUILD_CREATE_REAR_CAMP_ACK)	
{
	MSG_DZ_GUILD_CREATE_REAR_CAMP_ACK* pRecvMsg = (MSG_DZ_GUILD_CREATE_REAR_CAMP_ACK*)pMsg;
	GuildServer* pGuildServer = GuildServer::GetMainFrame();
	if( !pGuildServer )		return;

	RC::eGUILD_RESULT rc = g_GuildManager.CreateRearCamp( pRecvMsg->m_GuildGuid, pRecvMsg->m_byCorpsNum, pRecvMsg->m_TargetCharGuid );
	if( rc != RC::RC_GUILD_SUCCESS )
	{
		MSG_GZ_GUILD_CREATE_REAR_CAMP_NAK nakMSG;
		nakMSG.m_dwKey			= pRecvMsg->m_dwKey;
		nakMSG.m_byErrorCode	= rc;
		pGuildServer->SendToSession( pRecvMsg->m_dwSessionIndex, &nakMSG, sizeof(nakMSG) );
		SUNLOG( eCRITICAL_LOG, "[DZ_GUILD_CREATE_REAR_CAMP_ACK] DB Success But GuildServer Fail, Guild = %u, CorpsNum = %d", pRecvMsg->m_GuildGuid, pRecvMsg->m_byCorpsNum );
		return;
	}

	GuildMember* pMember = g_GuildManager.FindGuildMember( pRecvMsg->m_GuildGuid, pRecvMsg->m_TargetCharGuid );
	if( !pMember )		return;

	//GAME서버로 보낸다.
	MSG_GZ_GUILD_CREATE_REAR_CAMP_BRD brdMSG;
	brdMSG.m_dwKey			= pRecvMsg->m_dwKey;
	brdMSG.m_GuildGuid		= pRecvMsg->m_GuildGuid;
	brdMSG.m_byCorpsNum		= pRecvMsg->m_byCorpsNum;
    strncpy(brdMSG.m_szTargetCharName, pMember->GetCharName(), _countof(brdMSG.m_szTargetCharName));
    brdMSG.m_szTargetCharName[_countof(brdMSG.m_szTargetCharName) - 1] = '\0';
	pGuildServer->SendToAllGameServers( &brdMSG, sizeof(brdMSG) );

	//월드서버로 보낸다.
	MSG_WZ_GUILD_CREATE_REAR_CAMP_CMD wsMSG;
	wsMSG.m_dwKey			= pRecvMsg->m_dwKey;
	wsMSG.m_GuildGuid		= pRecvMsg->m_GuildGuid;
	wsMSG.m_byCorpsNum		= pRecvMsg->m_byCorpsNum;
	pGuildServer->ConnectSessions.SendToWorld( &wsMSG, sizeof(wsMSG) );
}

Handler_FromDBProxyServer_IMPL(DZ_GUILD_CREATE_REAR_CAMP_NAK)
{
	MSG_DZ_GUILD_CREATE_REAR_CAMP_NAK* pRecvMsg = (MSG_DZ_GUILD_CREATE_REAR_CAMP_NAK*)pMsg;
	GuildServer* pGuildServer = GuildServer::GetMainFrame();
	if( !pGuildServer )		return;

	MSG_GZ_GUILD_CREATE_REAR_CAMP_NAK nakMSG;
	nakMSG.m_dwKey			= pRecvMsg->m_dwKey;
	nakMSG.m_byErrorCode	= pRecvMsg->m_byErrorCode;
	pGuildServer->SendToSession( pRecvMsg->m_dwSessionIndex, &nakMSG, sizeof(nakMSG) );
}

// 길드 훈련캠프 해산
Handler_FromDBProxyServer_IMPL(DZ_GUILD_DESTROY_REAR_CAMP_ACK)	
{
	MSG_DZ_GUILD_DESTROY_REAR_CAMP_ACK* pRecvMsg = (MSG_DZ_GUILD_DESTROY_REAR_CAMP_ACK*)pMsg;
	GuildServer* pGuildServer = GuildServer::GetMainFrame();
	if( !pGuildServer )		return;

	RC::eGUILD_RESULT rc = g_GuildManager.DestroyRearCamp( pRecvMsg->m_GuildGuid, pRecvMsg->m_byCorpsNum, pRecvMsg->m_OperCharGuid );

	if( rc != RC::RC_GUILD_SUCCESS )
	{
		MSG_GZ_GUILD_DESTROY_REAR_CAMP_NAK nakMSG;
		nakMSG.m_dwKey			= pRecvMsg->m_dwKey;
		nakMSG.m_byErrorCode	= rc;
		pGuildServer->SendToSession( pRecvMsg->m_dwSessionIndex, &nakMSG, sizeof(nakMSG) );
		SUNLOG( eCRITICAL_LOG, "[DZ_GUILD_DESTROY_REAR_CAMP_ACK] DB Success But GuildServer Fail, Guild = %u, CorpsNum = %d", pRecvMsg->m_GuildGuid, pRecvMsg->m_byCorpsNum );
		return;
	}

	//GAME서버로 보낸다.
	MSG_GZ_GUILD_DESTROY_REAR_CAMP_BRD brdMSG;
	brdMSG.m_dwKey			= pRecvMsg->m_dwKey;
	brdMSG.m_GuildGuid		= pRecvMsg->m_GuildGuid;
	brdMSG.m_byCorpsNum		= pRecvMsg->m_byCorpsNum;
	pGuildServer->SendToAllGameServers( &brdMSG, sizeof(brdMSG) );

	//월드서버로 보낸다.
	MSG_WZ_GUILD_DESTROY_REAR_CAMP_CMD wsMSG;
	wsMSG.m_dwKey			= pRecvMsg->m_dwKey;
	wsMSG.m_GuildGuid		= pRecvMsg->m_GuildGuid;
	wsMSG.m_byCorpsNum		= pRecvMsg->m_byCorpsNum;
	pGuildServer->ConnectSessions.SendToWorld( &wsMSG, sizeof(wsMSG) );
}


Handler_FromDBProxyServer_IMPL(DZ_GUILD_DESTROY_REAR_CAMP_NAK)
{
	MSG_DZ_GUILD_DESTROY_REAR_CAMP_NAK* pRecvMsg = (MSG_DZ_GUILD_DESTROY_REAR_CAMP_NAK*)pMsg;
	GuildServer* pGuildServer = GuildServer::GetMainFrame();
	if( !pGuildServer )		return;

	MSG_GZ_GUILD_DESTROY_REAR_CAMP_NAK nakMSG;
	nakMSG.m_dwKey			= pRecvMsg->m_dwKey;
	nakMSG.m_byErrorCode	= pRecvMsg->m_byErrorCode;
	pGuildServer->SendToSession( pRecvMsg->m_dwSessionIndex, &nakMSG, sizeof(nakMSG) );
}

// 길드멤버 로그아웃
Handler_FromDBProxyServer_IMPL(DZ_GUILD_LOGOUT_CHAR_CMD)
{
	MSG_DZ_GUILD_LOGOUT_CHAR_CMD* pRecvMsg = (MSG_DZ_GUILD_LOGOUT_CHAR_CMD*)pMsg;
	GuildServer* pGuildServer = GuildServer::GetMainFrame();
	if( !pGuildServer )		return;

	//1. 길드서버의 해당 유저의 정보를 로그아웃으로 변경한다.
	RC::eGUILD_RESULT rc = g_GuildManager.Logout( pRecvMsg->m_GuildGuid, pRecvMsg->m_CharGuid );
	if( rc != RC::RC_GUILD_SUCCESS )
	{
		SUNLOG( eCRITICAL_LOG, "Handler_FromDBProxyServer_IMPL(DZ_GUILD_LOGOUT_CHAR_CMD) Fail, GuildGuid = %u, CharGuid = %u, rc = %d",
			pRecvMsg->m_GuildGuid, pRecvMsg->m_CharGuid, rc );
		return;
	}

	//2. 게임서버로 보낸다.
	MSG_GZ_GUILD_LOGOUT_CHAR_BRD	brdMsg;
	brdMsg.m_GuildGuid = pRecvMsg->m_GuildGuid;
	brdMsg.m_CharGuid = pRecvMsg->m_CharGuid;
	g_GuildManager.GetCharName( pRecvMsg->m_GuildGuid, pRecvMsg->m_CharGuid, brdMsg.m_szCharName, MAX_CHARNAME_LENGTH );
	pGuildServer->SendToAllGameServers( &brdMsg, sizeof(brdMsg) );

	//3. 월드서버로 보낸다.
	MSG_WZ_GUILD_LOGOUT_CHAR_CMD	cmdMsg;
	cmdMsg.m_GuildGuid = pRecvMsg->m_GuildGuid;
	cmdMsg.m_CharGuid = pRecvMsg->m_CharGuid;
	pGuildServer->ConnectSessions.SendToWorld( &cmdMsg, sizeof(cmdMsg) );
}

// GM명령어 길드포인트
Handler_FromDBProxyServer_IMPL(DZ_GUILD_GM_GUILD_POINT_ACK)
{
	MSG_DZ_GUILD_GM_GUILD_POINT_ACK* pRecvMsg = (MSG_DZ_GUILD_GM_GUILD_POINT_ACK*)pMsg;
	GuildServer* pGuildServer = GuildServer::GetMainFrame();
	if( !pGuildServer )		return;

	//1. 길드서버의 해당 유저의 정보를 로그아웃으로 변경한다.
	BOOL bRet = g_GuildManager.SetPoint( pRecvMsg->m_GuildGuid, pRecvMsg->m_GP, pRecvMsg->m_UP );
	if( !bRet )
	{
		SUNLOG( eCRITICAL_LOG, "Handler_FromDBProxyServer_IMPL(DZ_GUILD_GM_GUILD_POINT_ACK) Fail, GuildGuid = %u, GP = %d, UP = %d",
			pRecvMsg->m_GuildGuid, pRecvMsg->m_GP, pRecvMsg->m_UP );
		return;
	}

	//2. 게임서버로 보낸다.
	MSG_GZ_GUILD_GM_GUILD_POINT_BRD	brdMsg;
	brdMsg.m_GuildGuid = pRecvMsg->m_GuildGuid;
	brdMsg.m_UP = pRecvMsg->m_UP;
	brdMsg.m_GP = pRecvMsg->m_GP;
	pGuildServer->SendToAllGameServers( &brdMsg, sizeof(brdMsg) );
}


Handler_FromDBProxyServer_IMPL(DZ_GUILD_GM_GUILD_POINT_NAK)
{
	MSG_DZ_GUILD_GM_GUILD_POINT_NAK* pRecvMsg = (MSG_DZ_GUILD_GM_GUILD_POINT_NAK*)pMsg;

	MSG_GZ_GUILD_GM_GUILD_POINT_NAK nakMsg;
	nakMsg.m_byErrorCode = pRecvMsg->m_byErrorCode;
	GuildServer::GetMainFrame()->SendToSession( pRecvMsg->m_dwSessionIndex, &nakMsg, sizeof(nakMsg) );
}

// 군단 정보 요청 실패.
Handler_FromDBProxyServer_IMPL(DZ_GUILD_SELECT_CORPS_NAK)			
{
	MSG_DZ_GUILD_SELECT_CORPS_NAK* pRecvMsg = (MSG_DZ_GUILD_SELECT_CORPS_NAK*)pMsg;
	SUNLOG( eCRITICAL_LOG, "[Handler_FromDBProxyServer_IMPL(DZ_GUILD_SELECT_CORPS_NAK)] GuildGuid = %u", pRecvMsg->m_GuildGuid );
}

Handler_FromDBProxyServer_IMPL(DZ_GUILD_RELATION_ACK)
{
	PACKET_PTR_CONVERT(MSG_DZ_GUILD_RELATION_ACK, pRecvMsg, pMsg);
	
	GuildServer* pGuildServer = GuildServer::GetMainFrame();
	if(!pGuildServer) return;
	
	if(RC:: RC_GUILD_SUCCESS == GuildSmallContentsEntry::RelationSystem::AddRelation(
		pRecvMsg->m_GuildGuid, pRecvMsg->m_RelationGuildGuid, 
		pRecvMsg->m_eRelationType, pRecvMsg->m_tszRELGuildName))
	{

		MSG_GZ_GUILD_RELATION_BRD msgToGameServer;
		msgToGameServer.m_dwKey = pRecvMsg->m_dwKey;
		msgToGameServer.m_eRelationType = pRecvMsg->m_eRelationType;
		msgToGameServer.m_GuildGuid = pRecvMsg->m_GuildGuid;
		msgToGameServer.m_RelationGuildGuid = pRecvMsg->m_RelationGuildGuid;
		util::GUILDNAMECOPY(msgToGameServer.m_tszRelationGuildName, pRecvMsg->m_tszRELGuildName);
		
		Guild* pRequestGuild = g_GuildManager.FindGuild(pRecvMsg->m_GuildGuid);
        if(pRequestGuild) {
            util::GUILDNAMECOPY(msgToGameServer.m_tszRequestGuildName, pRequestGuild->GetGuildName());
        }
		
		pGuildServer->SendToAllGameServers(&msgToGameServer, sizeof(msgToGameServer));
	}
}

Handler_FromDBProxyServer_IMPL(DZ_GUILD_RELATION_NAK)
{
	PACKET_PTR_CONVERT(MSG_DZ_GUILD_RELATION_NAK, pRecvMsg, pMsg);
	
	GuildServer* pGuildServer = GuildServer::GetMainFrame();
	if(!pGuildServer) return;

	MSG_GZ_GUILD_RELATION_NAK msg;
	msg.m_dwKey = pRecvMsg->m_dwKey;
	msg.m_byErrorCode = pRecvMsg->m_byErrorCode;
	pGuildServer->SendToAllGameServers(&msg, sizeof(msg));
}

Handler_FromDBProxyServer_IMPL(DZ_GUILD_RELATION_DELETE_ACK)
{
	PACKET_PTR_CONVERT(MSG_DZ_GUILD_RELATION_DELETE_ACK, pRecvMsg, pMsg);

	GuildServer* pGuildServer = GuildServer::GetMainFrame();
	if(!pGuildServer) return;

	if(RC:: RC_GUILD_SUCCESS == GuildSmallContentsEntry::RelationSystem::RemoveRelation(
            pRecvMsg->m_GuildGuid, 
            pRecvMsg->m_RelGuildGuid, pRecvMsg->m_eType))
	{
		MSG_GZ_GUILD_RELATION_DELETE_BRD msgToGameServer;
		msgToGameServer.m_dwKey = pRecvMsg->m_dwKey;
		msgToGameServer.m_eType= pRecvMsg->m_eType;
		msgToGameServer.m_GuildGuid = pRecvMsg->m_GuildGuid;
		msgToGameServer.m_RelGuildGuid = pRecvMsg->m_RelGuildGuid;
		pGuildServer->SendToAllGameServers(&msgToGameServer, sizeof(msgToGameServer));
	}
}

Handler_FromDBProxyServer_IMPL(DZ_GUILD_RELATION_DELETE_NAK)
{
	PACKET_PTR_CONVERT(MSG_DZ_GUILD_RELATION_DELETE_NAK , pRecvMsg, pMsg);
		
	GuildServer* pGuildServer = GuildServer::GetMainFrame();
	if(!pGuildServer) return;

	MSG_GZ_GUILD_RELATION_DELETE_NAK msg;
	msg.m_dwKey			= pRecvMsg->m_dwKey;
	msg.m_eType			= pRecvMsg->m_eType;
	msg.m_GuildGuid		= pRecvMsg->m_GuildGuid;
	msg.m_RelGuildGuid	= pRecvMsg->m_RelGuildGuid;
	msg.m_byErrorCode	= pRecvMsg->m_byErrorCode;
	pGuildServer->SendToAllGameServers(&msg, sizeof(msg));
}

Handler_FromDBProxyServer_IMPL(DZ_GUILD_RELATION_BOTH_AGAINST_ACK)
{
	PACKET_PTR_CONVERT(MSG_DZ_GUILD_RELATION_BOTH_AGAINST_ACK, pRecvMsg, pMsg);
	
	GuildServer* pGuildServer = GuildServer::GetMainFrame();
	if(!pGuildServer) return;
	
	if(RC:: RC_GUILD_SUCCESS ==  GuildSmallContentsEntry::RelationSystem::AddRelation(
        pRecvMsg->m_GuildGuid, 
        pRecvMsg->m_RelGuildGuid, pRecvMsg->m_eRelType))
	{
		MSG_GZ_GUILD_RELATION_BOTH_AGAINST_BRD msgToGameServer;
		msgToGameServer.m_dwKey = pRecvMsg->m_dwKey;
		msgToGameServer.m_GuildGuid = pRecvMsg->m_GuildGuid;
		msgToGameServer.m_RelGuildGuid = pRecvMsg->m_RelGuildGuid;
		pGuildServer->SendToAllGameServers(&msgToGameServer, sizeof(msgToGameServer));
	}
}

Handler_FromDBProxyServer_IMPL(DZ_GUILD_RELATION_BOTH_AGAINST_NAK)
{
	PACKET_PTR_CONVERT(MSG_DZ_GUILD_RELATION_BOTH_AGAINST_NAK, pRecvMsg, pMsg);

	GuildServer* pGuildServer = GuildServer::GetMainFrame();
	if(!pGuildServer) return;

	MSG_GZ_GUILD_RELATION_BOTH_AGAINST_NAK msg;
	msg.m_dwKey			= pRecvMsg->m_dwKey;
	msg.m_GuildGuid		= pRecvMsg->m_GuildGuid;
	msg.m_RelGuildGuid	= pRecvMsg->m_RelGuildGuid;
	msg.m_byErrorCode	= pRecvMsg->m_byErrorCode;
	pGuildServer->SendToAllGameServers(&msg, sizeof(msg));
}


Handler_FromDBProxyServer_IMPL(DZ_GUILD_RELATION_INFO_ACK)
{
	PACKET_PTR_CONVERT(MSG_DZ_GUILD_RELATION_INFO_ACK, pRecvMsg, pMsg);
	GuildSmallContentsEntry::RelationSystem::SaveRelationInfo(&pRecvMsg ->m_AddInfo);

	GuildServer* pGuildServer = GuildServer::GetMainFrame();
	if(!pGuildServer) return;
	
	MSG_GZ_GUILD_RELATION_INFO_BRD SendToGameServer;
	SendToGameServer.m_dwKey = pRecvMsg->m_dwKey;
	SendToGameServer.m_AddInfo.m_GuildGuid = pRecvMsg->m_AddInfo.m_GuildGuid;
	SendToGameServer.m_byRelCnt = pRecvMsg->m_AddInfo.m_byRelationGuildCnt;
	GuildSmallContentsEntry::RelationSystem::LoadRelationInfo(&SendToGameServer.m_AddInfo);
	pGuildServer->SendToAllGameServers(&SendToGameServer, SendToGameServer.GetSize());
}

Handler_FromDBProxyServer_IMPL(DZ_GUILD_RELATION_INFO_NAK)
{
	PACKET_PTR_CONVERT(MSG_DZ_GUILD_RELATION_INFO_NAK, pRecvMsg, pMsg);

	MSG_DZ_GUILD_RELATION_INFO_NAK msg;
	msg.m_dwKey	= pRecvMsg->m_dwKey;
	msg.m_GuildGuid = pRecvMsg->m_GuildGuid;
	msg.m_byErrorCode = pRecvMsg->m_byErrorCode;
	GuildServer::GetMainFrame()->SendToAllGameServers(&msg, sizeof(msg));
}

Handler_FromDBProxyServer_IMPL(DZ_GUILD_RECOVERY_GUILD_ACK)
{
	MSG_DZ_GUILD_RECOVERY_GUILD_ACK *pRecvMsg = 
		static_cast<MSG_DZ_GUILD_RECOVERY_GUILD_ACK*>(pMsg);
	
	if(!GuildSmallContentsEntry::GuildPenalty::GuildRecovery(
        pRecvMsg->m_CharGuid, pRecvMsg->m_GuildGuid))
		return; //TODO : 로그를 남기자.

	GuildServer* pGuildServer = GuildServer::GetMainFrame();
	if(!pGuildServer) return;

	MSG_GZ_GUILD_RECOVERY_GUILD_BRD msgBRD;
	msgBRD.m_dwKey		= pRecvMsg->m_dwKey;
	msgBRD.m_GuildGuid	= pRecvMsg->m_GuildGuid;
	pGuildServer->SendToAllGameServers(&msgBRD, sizeof(msgBRD));

	MSG_WZ_GUILD_RECOVERY_GUILD_CMD msgCMD;
	msgCMD.m_dwKey		= pRecvMsg->m_dwKey;
	msgCMD.m_GuildGuid	= pRecvMsg->m_GuildGuid;
	pGuildServer->ConnectSessions.SendToWorld(&msgCMD, sizeof(msgCMD));
}

Handler_FromDBProxyServer_IMPL(DZ_GUILD_RECOVERY_GUILD_NAK)
{
	MSG_DZ_GUILD_RECOVERY_GUILD_NAK *pRecvMsg = 
		static_cast<MSG_DZ_GUILD_RECOVERY_GUILD_NAK*>(pMsg);
	
	MSG_GZ_GUILD_RECOVERY_GUILD_NAK msg;
	msg.m_dwKey			= pRecvMsg->m_dwKey;
	msg.m_CharGuid		= pRecvMsg->m_CharGuid;
	msg.m_byErrorCode	= RC::RC_GUILD_RECOVERY_FAILED;

	GuildServer *pGuildServer = GuildServer::GetMainFrame();
	pGuildServer->SendToAllGameServers(&msg, sizeof(msg));
}

Handler_FromDBProxyServer_IMPL(DZ_GUILD_DESTROY_COMPLETE_ACK)
{
	MSG_DZ_GUILD_DESTROY_COMPLETE_ACK* receive_msg = static_cast<MSG_DZ_GUILD_DESTROY_COMPLETE_ACK*>(pMsg);

#ifdef _USING_GUILD_DESTROY_LOGGING
    SUNLOG(eCRITICAL_LOG, 
        _T("|길드해산완료|Handler_FromDBProxyServer_IMPL(DZ_GUILD_DESTROY_COMPLETE_ACK)|MSG_DZ_GUILD_DESTROY_COMPLETE_ACK 받음|GuildGuid:%d, MasterCharGuid:%d|"), 
        receive_msg->m_GuildGuid, 
        receive_msg->m_MasterCharGuid);
#endif // _USING_GUILD_DESTROY_LOGGING

	g_GuildManager.DestroyGuild(receive_msg->m_GuildGuid);

	MSG_GZ_GUILD_DESTROY_BRD brd_msg;
	//(shogen)(20090819)
	//brd_msg.m_dwKey = receive_msg->m_dwKey;
	brd_msg.m_dwKey	= receive_msg->m_MasterCharGuid;
	brd_msg.m_GuildGuid = receive_msg->m_GuildGuid;

#ifdef _USING_GUILD_DESTROY_LOGGING
    SUNLOG(eCRITICAL_LOG, 
        _T("|길드해산완료|Handler_FromDBProxyServer_IMPL(DZ_GUILD_DESTROY_COMPLETE_ACK)|MSG_GZ_GUILD_DESTROY_BRD 보냄|GuildGuid:%d, MasterCharGuid:%u|"), 
        brd_msg.m_GuildGuid, 
        brd_msg.m_dwKey);
#endif // _USING_GUILD_DESTROY_LOGGING

	GuildServer* guild_server = GuildServer::GetMainFrame();
	guild_server->SendToAllGameServers(&brd_msg, sizeof(brd_msg));

	// Send To WorldServer
	MSG_WZ_GUILD_DESTROY_GUILD_CMD cmd_msg;
	cmd_msg.m_GuildGuid = receive_msg->m_GuildGuid;

#ifdef _USING_GUILD_DESTROY_LOGGING
    SUNLOG(eCRITICAL_LOG, 
        _T("|길드해산완료|Handler_FromDBProxyServer_IMPL(DZ_GUILD_DESTROY_COMPLETE_ACK)|MSG_WZ_GUILD_DESTROY_GUILD_CMD 보냄|GuildGuid:%d|"), cmd_msg.m_GuildGuid);
#endif // _USING_GUILD_DESTROY_LOGGING

	guild_server->ConnectSessions.SendToWorld(&cmd_msg, sizeof(cmd_msg));
}

Handler_FromDBProxyServer_IMPL(DZ_GUILD_DESTROY_COMPLETE_NAK)
{
	MSG_DZ_GUILD_DESTROY_COMPLETE_NAK* pRecvMsg = 
		static_cast<MSG_DZ_GUILD_DESTROY_COMPLETE_NAK*>(pMsg);
	
	MSG_GZ_GUILD_DESTROY_NAK msg;
	msg.m_dwKey = pRecvMsg->m_dwKey;
	msg.m_byErrorCode = pRecvMsg->m_byErrorCode;
	GuildServer* pGuildServer = GuildServer::GetMainFrame();
	pGuildServer->SendToSession(pRecvMsg->m_dwSessionIndex, &msg, sizeof(msg));
}


Handler_FromDBProxyServer_IMPL(DZ_GUILD_GUILDMARK_REGISTER_ACK)
{
	MSG_DZ_GUILD_GUILDMARK_REGISTER_ACK *pRecvMsg = 
		static_cast<MSG_DZ_GUILD_GUILDMARK_REGISTER_ACK*>(pMsg);

	/* 서버 마크 등록 */
	GuildSmallContentsEntry::GuildMark::GuildMarkInfo info;
	info.guild_Index = pRecvMsg->m_GuildIndex;
	info.pattern_index = pRecvMsg->m_PatternIndex;
	info.background_index = pRecvMsg->m_BackGroundIndex;
	GuildSmallContentsEntry::GuildMark::RegisterMark(info);

	GuildServer* pGuildServer = GuildServer::GetMainFrame();

	MSG_GZ_GUILD_GUILDMARK_REGISTER_BRD msgToGameServer;
	msgToGameServer.m_dwKey = pRecvMsg->m_dwKey;
	msgToGameServer.m_SlotIdx = pRecvMsg->m_SlotIdx;
	msgToGameServer.m_SlotPos = pRecvMsg->m_SlotPos;
	msgToGameServer.m_GuildIndex = pRecvMsg->m_GuildIndex;
	msgToGameServer.m_PatternIndex = pRecvMsg->m_PatternIndex;
	msgToGameServer.m_BackGroundIndex = pRecvMsg->m_BackGroundIndex;
	pGuildServer->SendToAllGameServers( &msgToGameServer, sizeof(msgToGameServer) );
	
	MSG_WZ_GUILD_GUILDMARK_REGISTER_CMD msgToWorld;
	msgToWorld.m_GuildGuid = pRecvMsg->m_GuildIndex;
	msgToWorld.m_PatternIndex = pRecvMsg->m_PatternIndex;
	msgToWorld.m_BackGroundIndex = pRecvMsg->m_BackGroundIndex;
	pGuildServer->ConnectSessions.SendToWorld( &msgToWorld, sizeof(msgToWorld) );
}

Handler_FromDBProxyServer_IMPL(DZ_GUILD_GUILDMARK_REGISTER_NAK)
{
	MSG_DZ_GUILD_GUILDMARK_REGISTER_NAK *pRecvMsg = 
		static_cast<MSG_DZ_GUILD_GUILDMARK_REGISTER_NAK*>(pMsg);

	MSG_GZ_GUILD_GUILDMARK_REGISTER_NAK msg;
	msg.m_dwKey = pRecvMsg->m_dwKey;
	msg.m_Result = pRecvMsg->m_Result;
	GuildServer* pGuildServer = GuildServer::GetMainFrame();
	pGuildServer->SendToSession(pRecvMsg->m_dwSessionIndex, &msg, sizeof(msg));
}

Handler_FromDBProxyServer_IMPL(DZ_GUILD_GUILDMARK_DELETE_ACK)
{
	MSG_DZ_GUILD_GUILDMARK_DELETE_ACK *pRecvMsg = 
		static_cast<MSG_DZ_GUILD_GUILDMARK_DELETE_ACK*>(pMsg);

	/* 서버 마크 삭제 */
	GuildSmallContentsEntry::GuildMark::GuildMarkInfo info;
	info.guild_Index = pRecvMsg->m_GuildIndex;
	info.pattern_index = pRecvMsg->m_PatternIndex;
	info.background_index = pRecvMsg->m_BackGroundIndex;
	GuildSmallContentsEntry::GuildMark::DeleteMark(info);

	GuildServer* pGuildServer = GuildServer::GetMainFrame();
	MSG_GZ_GUILD_GUILDMARK_DELETE_BRD msgToGameServer;
	msgToGameServer.m_dwKey = pRecvMsg->m_dwKey;
	msgToGameServer.m_GuildIndex = pRecvMsg->m_GuildIndex;
	msgToGameServer.m_PatternIndex = pRecvMsg->m_PatternIndex;
	msgToGameServer.m_BackGroundIndex = pRecvMsg->m_BackGroundIndex;
	pGuildServer->SendToAllGameServers( &msgToGameServer, sizeof(msgToGameServer) );

	MSG_WZ_GUILD_GUILDMARK_DELETE_CMD msgToWorld;
	msgToWorld.m_dwKey = pRecvMsg->m_dwKey;
	msgToWorld.m_GuildGuid = pRecvMsg->m_GuildIndex;
	msgToWorld.m_PatternIndex = pRecvMsg->m_PatternIndex;
	msgToWorld.m_BackGroundIndex = pRecvMsg->m_BackGroundIndex;
	pGuildServer->ConnectSessions.SendToWorld( &msgToWorld, sizeof(msgToWorld) );
}

Handler_FromDBProxyServer_IMPL(DZ_GUILD_GUILDMARK_DELETE_NAK)
{
	PACKET_PTR_CONVERT(MSG_DZ_GUILD_GUILDMARK_DELETE_NAK , pRecvMsg, pMsg);

	MSG_GZ_GUILD_GUILDMARK_DELETE_NAK msg;
	msg.m_dwKey = pRecvMsg->m_dwKey;
	msg.m_Result = pRecvMsg->m_Result;
	GuildServer* pGuildServer = GuildServer::GetMainFrame();
	pGuildServer->SendToSession(pRecvMsg->m_dwSessionIndex, &msg, sizeof(msg));
}

Handler_FromDBProxyServer_IMPL(DZ_GUILD_WAREHOUSE_OPEN_ACK)
{
	PACKET_PTR_CONVERT(MSG_DZ_GUILD_WAREHOUSE_OPEN_ACK, pRecvMsg, pMsg);
	GuildServer* pGuildServer = GuildServer::GetMainFrame();
	if(!pGuildServer) return;
    // (f100527.3L)
    if (sizeof(*pRecvMsg) < static_cast<ulong>(pRecvMsg->GetSize())) {
        SUNLOG(eCRITICAL_LOG, _T("[%s]|Required='<=%d'|Inputed=%u|Msg=corrupted packet received|"),
               __FUNCTION__, sizeof(*pRecvMsg), pRecvMsg->GetSize());
        return;
    }
	
	GUILDGUID GuildGuid = pRecvMsg->m_GuildGuid;
	CHARGUID OperCharGuid = pRecvMsg->m_OperCharGuid;
	MONEY WarehouseMoney = pRecvMsg->m_WarehouseMoney;

	BYTE SlotTabNum = pRecvMsg->m_SlotTabNum;
	BOOL IsRight = g_GuildManager.CheckRight(GuildGuid, OperCharGuid, eGUILD_RIGHTS_WAREHOUSE_OUT);

    // (f100527.3L)
    const int guild_warehouse_stream_size = pRecvMsg->m_ItemInfo.GetSize();
	//int size = sizeof(_GUILDWAREHOUSE_TOTAL_INFO) -
	//	(_GUILDWAREHOUSE_TOTAL_INFO::MAX_SLOT_NUM - pRecvMsg->m_ItemInfo.m_Count) * 
	//	sizeof(_GUILDWAREHOUSE_TOTAL_INFO::SLOT_TYPE);
	
	MSG_GZ_GUILD_WAREHOUSE_OPEN_BRD msgToGameServer;
    if (guild_warehouse_stream_size == 0) { //compile debug
        util::AssertCompileTimeVariableTypeEqual(msgToGameServer.m_ItemInfo, pRecvMsg->m_ItemInfo);
    }
	msgToGameServer.m_dwKey = pRecvMsg->m_dwKey;
	msgToGameServer.m_GuildGuid = GuildGuid;
	msgToGameServer.m_WarehouseRight = IsRight;
	msgToGameServer.m_SlotTabNum  = SlotTabNum;
	msgToGameServer.m_OperCharGuid = OperCharGuid;
	msgToGameServer.m_WarehouseMoney = WarehouseMoney;
	CopyMemory(&msgToGameServer.m_ItemInfo, &pRecvMsg->m_ItemInfo, guild_warehouse_stream_size);
	pGuildServer->SendToAllGameServers(&msgToGameServer, msgToGameServer.GetSize());
	//pGuildServer->SendToSession(pRecvMsg->m_dwSessionIndex, &msgToGameServer, msgToGameServer.GetSize());

	//MSG_WZ_GUILD_WAREHOUSE_OPEN_CMD msgToWorld;
	//msgToWorld.m_GuildGuid = GuildGuid;
	//msgToWorld.m_OpenUserKey = OperCharGuid;
	//pGuildServer->ConnectSessions.SendToWorld(&msgToWorld, sizeof(msgToWorld));
}

Handler_FromDBProxyServer_IMPL(DZ_GUILD_WAREHOUSE_OPEN_NAK)
{
	PACKET_PTR_CONVERT(MSG_DZ_GUILD_WAREHOUSE_OPEN_NAK, pRecvMsg, pMsg);
	
	GuildServer* pGuildServer = GuildServer::GetMainFrame();
	if(!pGuildServer) return;

	MSG_GZ_GUILD_WAREHOUSE_OPEN_NAK msg;
	msg.m_dwKey = pRecvMsg->m_dwKey;
	msg.m_GuildGuid = pRecvMsg->m_GuildGuid;
	msg.m_bErrorCode = pRecvMsg->m_byErrorCode;
	pGuildServer->SendToAllGameServers(&msg, sizeof(msg));
	//pGuildServer->SendToSession(pRecvMsg->m_dwSessionIndex, &msg, sizeof(msg));
	//pGuildServer->SendToSession()
}

Handler_FromDBProxyServer_IMPL(DZ_GUILD_NAME_CHANGE_ACK)
{	
	MSG_DZ_GUILD_NAME_CHANGE_ACK* pRecvMsg = (MSG_DZ_GUILD_NAME_CHANGE_ACK*)pMsg;
	GuildServer* pGuildServer = GuildServer::GetMainFrame();
	if(!pGuildServer) return;

	GUILDGUID GuildGuid = pRecvMsg->m_GuildGuid;

    //{__NA001254_20090109_RTTG_FUNC_ADD
	if( pRecvMsg->m_TargetPos == INVALID_POSTYPE_VALUE && 
		pRecvMsg->m_OperCharGuid == 0 )//INVALID_CHARGUID )
	{
		// RTTG명령이 성공 했음을 알린다.
		MSG_MZ_GUILD_NAME_CHANGE_ACK msg;
		msg.m_dwKey	= pRecvMsg->m_dwKey;
		pGuildServer->SendToSession(pRecvMsg->m_dwSessionIndex, &msg, sizeof(msg));		
	}
    //}
	if( pRecvMsg->m_TargetPos != INVALID_POSTYPE_VALUE && 
		pRecvMsg->m_OperCharGuid != 0 )// INVALID_CHARGUID )
	{
        Guild* pGuild = g_GuildManager.FindGuild(GuildGuid);
        const TCHAR* guild_name = (pGuild != NULL ? pGuild->GetGuildName() : "[Unknown]");
 
		// 아이템 사용이 성공 했음을 알린다.
		MSG_GZ_GUILD_NAME_CHANGE_ACK msg;
		msg.m_dwKey	= pRecvMsg->m_dwKey;
		msg.m_TargetPos = pRecvMsg->m_TargetPos;
        //_NA_0_20100901_GUILD_NAME_CHANGE_ITEM_LOG
        util::GUILDNAMECOPY(msg.m_tszCurrentGuildName, guild_name);
        util::GUILDNAMECOPY(msg.m_tszchangeGuildName, pRecvMsg->m_tszGuildName);
		pGuildServer->SendToSession(pRecvMsg->m_dwSessionIndex, &msg, sizeof(msg));
	}

    // 현재 길드 서버의 길드명을 바꾼다.
    g_GuildManager.ChangeGuildName(GuildGuid, pRecvMsg->m_tszGuildName);

	// 연결된 모든 게임 서버에 길드명이 바뀌었음을 알린다.
	MSG_GZ_GUILD_NAME_CHANGE_BRD msgBRD;
	msgBRD.m_GuildGuid = GuildGuid;
	util::GUILDNAMECOPY(msgBRD.m_tszGuildName, pRecvMsg->m_tszGuildName);
	pGuildServer->SendToAllGameServers(&msgBRD, sizeof(msgBRD));

	//월드 서버에 변경을 알린다.
	MSG_WZ_GUILD_NAME_CHANGE_CMD Wmsg;
	Wmsg.m_GuildGuid = GuildGuid;
	util::GUILDNAMECOPY(Wmsg.m_tszGuildName, pRecvMsg->m_tszGuildName);
	pGuildServer->ConnectSessions.SendToWorld(&Wmsg, sizeof(Wmsg));
}

Handler_FromDBProxyServer_IMPL(DZ_GUILD_NAME_CHANGE_NAK)
{
	MSG_DZ_GUILD_NAME_CHANGE_NAK* pRecvMsg = (MSG_DZ_GUILD_NAME_CHANGE_NAK*)pMsg;
	GuildServer* pGuildServer = GuildServer::GetMainFrame();
	if(!pGuildServer) return;

    //{__NA001254_20090109_RTTG_FUNC_ADD
	if( pRecvMsg->m_TargetPos == INVALID_POSTYPE_VALUE && 
		pRecvMsg->m_OperCharGuid == 0 )//INVALID_CHARGUID )
	{
		MSG_MZ_GUILD_NAME_CHANGE_NAK msg;
		msg.m_dwKey = pRecvMsg->m_dwKey;
		msg.m_byErrorCode = pRecvMsg->m_byErrorCode;
		pGuildServer->SendToSession(pRecvMsg->m_dwSessionIndex, &msg, sizeof(msg));
		return;
	}
    //}
	MSG_GZ_GUILD_NAME_CHANGE_NAK msg;
	msg.m_dwKey = pRecvMsg->m_dwKey;
	msg.m_TargetPos = pRecvMsg->m_TargetPos;
	msg.m_byErrorCode = pRecvMsg->m_byErrorCode;
	pGuildServer->SendToSession(pRecvMsg->m_dwSessionIndex, &msg, sizeof(msg));
}

/************************************************************************/
/*                  지역 점령전 시스템 작업                             */
/************************************************************************/
Handler_FromDBProxyServer_IMPL(DZ_DOMINATION_INFO_ACK)
{
    const MSG_DZ_DOMINATION_INFO_ACK* recv_msg = static_cast<MSG_DZ_DOMINATION_INFO_ACK*>(pMsg);

    DominationManager* domination_manager = DominationManager::Instance();

    if (recv_msg->m_RsultCode == RC::DOMINATION_RESULT_FAIL)
    {
        domination_manager->SetLoadDominationInfo(false);
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"]Domintion info request fail");
        return;
    }

    const BYTE number_of_records = recv_msg->m_NumRecords;
    if (number_of_records > _countof(recv_msg->m_DominationInfo)) 
    {
        assert(!"Data num is incorrect");
        SUNLOG(eCRITICAL_LOG, __FUNCTION__":Data num is incorrect : %d]",recv_msg->m_NumRecords);
        return;
    }

    for (int i = 0; i < number_of_records; ++i)
    {
        const DOMINATION_INFO& curInfo = recv_msg->m_DominationInfo[i];
        domination_manager->InsertDominationInfo(curInfo );
    }

    //load success
    domination_manager->SetLoadDominationInfo(true);
    SUNLOG(eCRITICAL_LOG, "Domination info load success : %d",number_of_records);
}

#ifdef _CH_20100420_CHANGE_DOMINATION_APPLY_WAY 
Handler_FromDBProxyServer_IMPL(DZ_DOMINATION_AUCTION_INFO_ACK)
{
    const MSG_DZ_DOMINATION_AUCTION_INFO_ACK* recv_msg = 
        static_cast<MSG_DZ_DOMINATION_AUCTION_INFO_ACK*>(pMsg);

    DominationManager* domination_manager = DominationManager::Instance();

    if (recv_msg->m_RsultCode == RC::DOMINATION_RESULT_FAIL)
    {
        domination_manager->SetLoadDominationAuctionInfo( FALSE );

        SUNLOG( eCRITICAL_LOG, __FUNCTION__":Result fail" );
        return;
    }

    //전달된 데이터의 레코드수 만큼의 데이터를 저장한다. 
    const BYTE data_count = recv_msg->m_DataCount;

    if (data_count > _countof(recv_msg->m_DominationActionDBInfo))
    {
        assert(!"Over auction list");
        SUNLOG(eCRITICAL_LOG, __FUNCTION__":Over auction list : %d, %d", recv_msg->m_MapCode, data_count);
        return;
    }

    domination_manager->SetCheckActionInfo(recv_msg->m_MapCode);

    for (int i = 0; i < data_count; ++i)
    {
        const DominationAuctionInfo& info = recv_msg->m_DominationActionDBInfo[i];
        domination_manager->InsertActionInfo(info.m_ActionInfo.m_FieldCode, info);
    }
    SUNLOG(eCRITICAL_LOG,"Insert auction info : %d, %d", recv_msg->m_MapCode, data_count);
}
#else//_CH_20100420_CHANGE_DOMINATION_APPLY_WAY
Handler_FromDBProxyServer_IMPL(DZ_DOMINATION_AUCTION_INFO_ACK)
{
    const MSG_DZ_DOMINATION_AUCTION_INFO_ACK* pRecvMsg = 
        static_cast<MSG_DZ_DOMINATION_AUCTION_INFO_ACK*>(pMsg);

    GuildServer* pGuildServer = GuildServer::GetMainFrame();
    if( pGuildServer == NULL )
    {
        SUNLOG( eCRITICAL_LOG, "["__FUNCTION__"]Not Found pGuildServer\n" );
        return;
    }

    if( pRecvMsg->m_RsultCode == RC::DOMINATION_RESULT_FAIL )
    {
        DominationManager::Instance()->SetLoadDominationAuctionInfo( FALSE );

        SUNLOG( eCRITICAL_LOG, "["__FUNCTION__"]Result Fail\n" );
        return;
    }

    //전달된 데이터의 레코드수 만큼의 데이터를 저장한다. 
    const BYTE number_of_data = pRecvMsg->NumRecords;

    //절달된 패킷 내용이 최대 경매 리스트 갯수를 넘게 된다면 실패
    if (number_of_data > _countof(pRecvMsg->m_DominationActionDBInfo))
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"]Over Action List");
        return;
    }

    MAPCODE mapCode = pRecvMsg->m_MapCode;
    DominationManager::Instance()->SetCheckActionInfo(mapCode);

    for (int i = 0; i < number_of_data; ++i)
    {
        const DominationAuctionInfo& CurInfo = pRecvMsg->m_DominationActionDBInfo[i];
        DominationManager::Instance()->InsertActionInfo(CurInfo.m_ActionInfo.m_FieldCode, CurInfo);
    }
    SUNLOG(eCRITICAL_LOG,"Insert auction info : %d , %d", mapCode, number_of_data);
}
#endif//_CH_20100420_CHANGE_DOMINATION_APPLY_WAY

//_KR_001385_20091105_DOMINATION_UPDATEDOMINATIONINFO
Handler_FromDBProxyServer_IMPL(DZ_DOMINATION_UPDATE_INFO_ACK)
{
    const MSG_DZ_DOMINATION_UPDATE_INFO_ACK* pRecvMsg = static_cast<MSG_DZ_DOMINATION_UPDATE_INFO_ACK*>(pMsg);

    GuildServer* pGuildServer = GuildServer::GetMainFrame();
    if( pGuildServer == NULL )
    {
        SUNLOG( eCRITICAL_LOG, "["__FUNCTION__"]Not Found pGuildServer" );
        return;
    }

    MSG_GZ_DOMINATION_UPDATE_INFO_ACK sendMsg;

    if( pRecvMsg->m_RsultCode == RC::DOMINATION_RESULT_FAIL )
    {
        sendMsg.m_ResultCode = pRecvMsg->m_RsultCode;
        sendMsg.m_DominationInfo = pRecvMsg->m_DominationInfo;
        pGuildServer->SendToAllGameServers(&sendMsg, sizeof(sendMsg));        
        SUNLOG( eCRITICAL_LOG, "["__FUNCTION__"]Data Update Result is Fail" );

        return;
    }

    const DOMINATION_INFO& dominationInfo = pRecvMsg->m_DominationInfo;

    DOMINATION_INFO* pInfo = DominationManager::Instance()->FindDominationInfo( pRecvMsg->m_DominationInfo.m_FieldCode );

    if( pInfo )
    {
        switch ( pRecvMsg->m_eSetType )
        {
        case DOMINATION_DEFFENSEWALL_SET:
        case DOMINATION_INIT:
        case DOMINATION_INFO_SET:
            (*pInfo) = pRecvMsg->m_DominationInfo;
            break;

        case DOMINATION_SUCCESS_DEFENCE:
            pInfo->m_disGuildInfo.m_GuildGuid = 0;
            break;

        case DOMINATION_SUCCESS_OFFENCE:
            pInfo->m_inGuildInfo = pRecvMsg->m_DominationInfo.m_disGuildInfo;
            pInfo->m_disGuildInfo.m_GuildGuid = 0;
            break;

        case DOMINATION_DISGUILD_SET:
            //현재 경매 정보에서 공격길드로 선정된 경매 리스트를 삭제 한다.
            DominationManager::Instance()->DeleteActionList(dominationInfo.m_FieldCode, 
                dominationInfo.m_disGuildInfo.m_GuildGuid );

            break;
        }
    }
    else
    {
        //데이터가 존재 하지 않으면 새로 만들어 준다.
        DominationManager::Instance()->InsertDominationInfo( dominationInfo );
    }

    sendMsg.m_ResultCode = pRecvMsg->m_RsultCode;
    sendMsg.m_DominationInfo = pRecvMsg->m_DominationInfo;
    sendMsg.m_eSetType = pRecvMsg->m_eSetType;
    pGuildServer->SendToAllGameServers(&sendMsg, sizeof(sendMsg));
}

Handler_FromDBProxyServer_IMPL(DZ_DOMINATION_ACCEP_HEIM_ACK)
{
    const MSG_DZ_DOMINATION_ACCEP_HEIM_ACK* pRecvMsg = static_cast<MSG_DZ_DOMINATION_ACCEP_HEIM_ACK*>(pMsg);

    GuildServer* pGuildServer = GuildServer::GetMainFrame();
    if( pGuildServer == NULL )
    {
        SUNLOG( eCRITICAL_LOG, "["__FUNCTION__"]Not Found pGuildServer\n" );
        return;
    }

    MSG_GZ_DOMINATION_ACCEP_HEIM_ACK sendMsg;

    if( pRecvMsg->m_ResultCode != RC::DOMINATION_RESULT_SUCCESS )
    {
        sendMsg.m_dwKey = pRecvMsg->m_dwKey;
        sendMsg.m_ResultCode = pRecvMsg->m_ResultCode;
        sendMsg.m_DominationActionInfo = pRecvMsg->m_DominationActionInfo;
#ifdef _CH_20100420_CHANGE_DOMINATION_APPLY_WAY
        sendMsg.m_TokenPos = pRecvMsg->m_TokenPos;
        sendMsg.number_of_betting_item = pRecvMsg->number_of_betting_item;
#endif 
        SUNLOG( eCRITICAL_LOG, "["__FUNCTION__"]Result Fail\n" );
    }
    else if( pRecvMsg->m_ResultCode == RC::DOMINATION_RESULT_SUCCESS )
    {
        const DominationAuctionInfo& curActionInfo = pRecvMsg->m_DominationActionInfo;
        DominationManager::Instance()->InsertActionInfo(curActionInfo.m_ActionInfo.m_FieldCode, curActionInfo);

        sendMsg.m_dwKey = pRecvMsg->m_dwKey;
        sendMsg.m_ResultCode = pRecvMsg->m_ResultCode;
        sendMsg.m_DominationActionInfo = pRecvMsg->m_DominationActionInfo;
#ifdef _CH_20100420_CHANGE_DOMINATION_APPLY_WAY
        sendMsg.m_TokenPos = pRecvMsg->m_TokenPos;
        sendMsg.number_of_betting_item = pRecvMsg->number_of_betting_item;
#endif
        SUNLOG( eFULL_LOG, "["__FUNCTION__"]Update Success[Map:%d][Heim:%d]\n",
            pRecvMsg->m_DominationActionInfo.m_ActionInfo.m_FieldCode, pRecvMsg->m_DominationActionInfo.m_ActionInfo.m_TenderCost );
    }

    pGuildServer->SendToAllGameServers(&sendMsg, sizeof(sendMsg));
}

Handler_FromDBProxyServer_IMPL(DZ_DOMINATION_INIT_AUCTIONINFO_ACK)
{
    const MSG_DZ_DOMINATION_INIT_AUCTIONINFO_ACK* pRecvMsg = static_cast<MSG_DZ_DOMINATION_INIT_AUCTIONINFO_ACK*>(pMsg);

    GuildServer* pGuildServer = GuildServer::GetMainFrame();
    if( pGuildServer == NULL )
    {
        SUNLOG( eCRITICAL_LOG, "["__FUNCTION__"]Not Found pGuildServer\n" );
        return;
    }

    MSG_GZ_DOMINATION_INIT_ACTIONINFO_ACK sendMsg;

    if( pRecvMsg->m_ResultCode != RC::DOMINATION_RESULT_SUCCESS )
    {
        sendMsg.m_ResultCode = pRecvMsg->m_ResultCode;
        SUNLOG( eCRITICAL_LOG, "["__FUNCTION__"]Result Fail\n" );
    }
    else if( pRecvMsg->m_ResultCode == RC::DOMINATION_RESULT_SUCCESS )
    {
        sendMsg.m_ResultCode = pRecvMsg->m_ResultCode;
        DominationManager::Instance()->ClearDominationActionInfo( pRecvMsg->m_MapCode );
        SUNLOG( eFULL_LOG, "["__FUNCTION__"]ActionInfo Init Success~!!\n" );
    }
    
    sendMsg.m_MapCode = pRecvMsg->m_MapCode;
    pGuildServer->SendToAllGameServers(&sendMsg, sizeof(sendMsg));
}

#ifdef _NA_20100604_BETTER_DOMINATION_MANAGER
Handler_FromDBProxyServer_IMPL(DZ_DOMINATION_UPDATE_GUILDINFO_ACK)
{
    const MSG_DZ_DOMINATION_UPDATE_GUILDINFO_ACK* recv_msg = 
        static_cast<MSG_DZ_DOMINATION_UPDATE_GUILDINFO_ACK*>(pMsg);

    GuildServer* guild_server = GuildServer::GetMainFrame();
    if (guild_server == NULL)
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"]Not Found pGuildServer");
        return;
    }

    //--DB 저장 실패
    if (recv_msg->db_result_ == MSG_DZ_DOMINATION_UPDATE_GUILDINFO_ACK::DB_RESULT_FAIL)
    {
        MSG_GZ_DOMINATION_UPDATE_GUILDINFO_ACK nak_msg;
        {
            nak_msg.db_result_ = MSG_DZ_DOMINATION_UPDATE_GUILDINFO_ACK::DB_RESULT_FAIL;
            nak_msg.guild_guid_ = recv_msg->guild_guid_;
            nak_msg.domination_join_count_ = 0;
            nak_msg.domination_defense_count_ = 0;
            nak_msg.domination_hunting_count_ = 0;
        }
        guild_server->SendToAllGameServers(&nak_msg, sizeof(nak_msg));
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"]DB Store Fail");
        return;        
    }

    //--길드 정보내 점령전 정보 셋팅
    Guild* request_guild = g_GuildManager.FindGuild(recv_msg->guild_guid_);
    if (request_guild == NULL) return;
    request_guild->SetDominationJointCount(recv_msg->domination_join_count_);
    request_guild->SetDominationDefenseCount(recv_msg->domination_defense_count_);
    request_guild->SetDominationHuntingCount(recv_msg->domination_hunting_count_);

    MSG_GZ_DOMINATION_UPDATE_GUILDINFO_ACK send_msg;
    {
        send_msg.db_result_ = MSG_DZ_DOMINATION_UPDATE_GUILDINFO_ACK::DB_RESULT_SUCCESS;
        send_msg.guild_guid_ = recv_msg->guild_guid_;
        send_msg.domination_join_count_ = recv_msg->domination_join_count_;
        send_msg.domination_defense_count_ = recv_msg->domination_defense_count_;
        send_msg.domination_hunting_count_ = recv_msg->domination_hunting_count_;
    }
    guild_server->SendToAllGameServers(&send_msg, sizeof(send_msg));
}
#endif

#ifdef _NA_20110216_INCREASE_DOMINATION_DEFFENSEWALL_POINT
Handler_FromDBProxyServer_IMPL(DZ_DOMINATION_ADD_DEFFENSEWALL_POINT_ACK)
{
    const MSG_DZ_DOMINATION_ADD_DEFFENSEWALL_POINT_ACK* recv_msg = 
        static_cast<MSG_DZ_DOMINATION_ADD_DEFFENSEWALL_POINT_ACK*>(pMsg);

    GuildServer* const guild_server = GuildServer::GetMainFrame();

    if (recv_msg->result_value_ == false)
    {
        MSG_GZ_DOMINATION_ADD_DEFFENSEWALL_POINT_ACK nak_msg;
        nak_msg.db_result_ = recv_msg->result_value_;
        nak_msg.domination_info_ = recv_msg->domination_info_;
        nak_msg.item_code = recv_msg->item_code_;
        nak_msg.m_dwKey = recv_msg->m_dwKey;

        guild_server->SendToAllGameServers(&nak_msg, sizeof(nak_msg));        
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"]Update Failed to domination info[map:%d]",
               recv_msg->domination_info_.m_FieldCode);

        return;
    }

    DominationManager* const domination_manager = DominationManager::Instance();

    DOMINATION_INFO* info = 
        domination_manager->FindDominationInfo(recv_msg->domination_info_.m_FieldCode);
    if (info == NULL)
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"]Not found domination info[map:%d]",
               recv_msg->domination_info_.m_FieldCode);
        return;
    }

    *(info) = recv_msg->domination_info_;

    MSG_GZ_DOMINATION_ADD_DEFFENSEWALL_POINT_ACK ack_msg;
    ack_msg.db_result_ = recv_msg->result_value_;
    ack_msg.domination_info_ = recv_msg->domination_info_;
    ack_msg.item_code = recv_msg->item_code_;
    ack_msg.m_dwKey = recv_msg->m_dwKey;
    guild_server->SendToAllGameServers(&ack_msg, sizeof(ack_msg));
}
#endif//_NA_20110216_INCREASE_DOMINATION_DEFFENSEWALL_POINT

#ifdef _NA_20110406_CHANGE_RULE_FOR_DOMINATION_JOIN
Handler_FromDBProxyServer_IMPL(DZ_DOMINATION_MEMBER_JOIN_ACK)
{
    const MSG_DZ_DOMINATION_MEMBER_JOIN_ACK* recv_msg = 
        static_cast<MSG_DZ_DOMINATION_MEMBER_JOIN_ACK*>(pMsg);

    GuildServer* const guild_server = GuildServer::GetMainFrame();

    if (recv_msg->db_result_ == false)
    {
        MSG_GZ_DOMINATION_MEMBER_JOIN_ACK nak_msg;
        nak_msg.db_result_ = recv_msg->db_result_;
        nak_msg.map_code_ = recv_msg->map_code_;
        nak_msg.member_info_ = recv_msg->member_info_;
        guild_server->SendToAllGameServers(&nak_msg, sizeof(nak_msg));

        return;
    }

    DominationManager::Instance()->InsertDominationMemberInfo(recv_msg->map_code_, 
                                                              recv_msg->member_info_);
    MSG_GZ_DOMINATION_MEMBER_JOIN_ACK ack_msg;
    ack_msg.db_result_ = recv_msg->db_result_;
    ack_msg.map_code_ = recv_msg->map_code_;
    ack_msg.member_info_ = recv_msg->member_info_;
    guild_server->SendToAllGameServers(&ack_msg, sizeof(ack_msg));
}
Handler_FromDBProxyServer_IMPL(DZ_DOMINATION_MEMBER_LEAVE_ACK)
{
    const MSG_DZ_DOMINATION_MEMBER_LEAVE_ACK* recv_msg = 
        static_cast<MSG_DZ_DOMINATION_MEMBER_LEAVE_ACK*>(pMsg);

    GuildServer* const guild_server = GuildServer::GetMainFrame();

    if (recv_msg->db_result_ == false)
    {
        MSG_GZ_DOMINATION_MEMBER_LEAVE_ACK nak_msg;
        nak_msg.db_result_ = recv_msg->db_result_;
        nak_msg.map_code_ = recv_msg->map_code_;
        nak_msg.member_guid_ = recv_msg->member_guid_;
        nak_msg.requester_guid = recv_msg->requester_guid;
        guild_server->SendToAllGameServers(&nak_msg, sizeof(nak_msg));

        return;
    }

    DominationManager::Instance()->DeleteDominationMemberInfo(recv_msg->map_code_, 
                                                              recv_msg->member_guid_);
    MSG_GZ_DOMINATION_MEMBER_LEAVE_ACK ack_msg;
    ack_msg.db_result_ = recv_msg->db_result_;
    ack_msg.map_code_ = recv_msg->map_code_;
    ack_msg.member_guid_ = recv_msg->member_guid_;
    ack_msg.requester_guid = recv_msg->requester_guid;
    guild_server->SendToAllGameServers(&ack_msg, sizeof(ack_msg));
}
Handler_FromDBProxyServer_IMPL(DZ_DOMINATION_MEMBERLIST_ANS)
{
    const MSG_DZ_DOMINATION_MEMBERLIST_ANS* recv_msg = 
        static_cast<MSG_DZ_DOMINATION_MEMBERLIST_ANS*>(pMsg);

    DominationManager* const domination_manager = DominationManager::Instance();

    const BYTE data_count = recv_msg->data_count_;
    if (data_count > _countof(recv_msg->member_list_))
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__":Member list count is over");
        return;
    }

    MAPCODE map_code = recv_msg->map_code_;
    domination_manager->SetCheckMemberInfo(map_code);

    if (data_count > 0)
    {
        for (BYTE i = 0; i < data_count; ++i)
        {
            const DominationMemberInfo& member_info = recv_msg->member_list_[i];
            domination_manager->InsertDominationMemberInfo(map_code, member_info);
        }
    }
    SUNLOG(eCRITICAL_LOG,"Insert member info : %d , %d", recv_msg->map_code_, recv_msg->data_count_);
}

Handler_FromDBProxyServer_IMPL(DZ_DOMINATION_MEMBERLIST_INIT_ACK)
{
    const MSG_DZ_DOMINATION_MEMBERLIST_INIT_ACK* recv_msg = 
        static_cast<MSG_DZ_DOMINATION_MEMBERLIST_INIT_ACK*>(pMsg);

    if (recv_msg->db_result_ == false)
    {
        MSG_DZ_DOMINATION_MEMBERLIST_INIT_SYN send_msg;
        GuildServer* const guild_server = GuildServer::GetMainFrame();
        guild_server->ConnectSessions.SendToDBProxy(&send_msg, sizeof(send_msg));
    }
    SUNLOG(eCRITICAL_LOG, "Domination member list init success");
}
#endif//_NA_20110406_CHANGE_RULE_FOR_DOMINATION_JOIN
/************************************************************************/
/*                  지역 점령전 시스템 작업 END                         */
/************************************************************************/