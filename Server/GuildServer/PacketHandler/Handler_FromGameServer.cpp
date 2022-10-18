#include "stdafx.h"
#include "Handler_FromGameServer.h"
#include "PacketHandler.Shared.h"
#include <PacketStruct_DZ.h>
#include <PacketStruct_GZ.h>
#include <PacketStruct_WZ.h>

#include <GuildRankInfoParser.h>
#include <GuildServer.h>
#include "DominationManager.h"

// GUILD
#include <Guild/GuildManager.h>
#include <Guild/Guild.h>
#include <Guild/GuildMember.h>

#include <Guild/GuildRelation.h>
#include <Guild/GuildSmallContentEntry.h>
//
//


#define ADD_HANDLER_INFO_EOR()	{ MAKEWORD(0, 0), PHASE_NOACTION, (PH_fnHandler)NULL }
#define ADD_HANDLER_INFO( c, p, accptablePHASE )		\
	{ MAKEWORD(c, p), accptablePHASE, (PH_fnHandler)&Handler_FromGameServer::On##p }

sPACKET_HANDLER_INFO*	Handler_FromGameServer::GetHandlerInfo()
{
	static sPACKET_HANDLER_INFO s_pHandlerInfo[] =
	{
		//
		//ADD_HANDLER_INFO(AM_OPERATION, AM_OPERATION_USERCOUNT_SYN, PHASE_UNLIMITED),
		ADD_HANDLER_INFO( GZ_GUILD, GZ_GUILD_CREATE_SYN, PHASE_UNLIMITED),
		ADD_HANDLER_INFO( GZ_GUILD, GZ_GUILD_DESTROY_SYN, PHASE_UNLIMITED),
		ADD_HANDLER_INFO( GZ_GUILD, GZ_GUILD_JOIN_SYN, PHASE_UNLIMITED),
		ADD_HANDLER_INFO( GZ_GUILD, GZ_GUILD_WITHDRAW_SYN, PHASE_UNLIMITED),
		ADD_HANDLER_INFO( GZ_GUILD, GZ_GUILD_KICKPLAYER_SYN, PHASE_UNLIMITED),
		ADD_HANDLER_INFO( GZ_GUILD, GZ_GUILD_DONATE_UP_SYN, PHASE_UNLIMITED),
		ADD_HANDLER_INFO( GZ_GUILD, GZ_GUILD_CONVERT_UP2GP_SYN, PHASE_UNLIMITED),
		ADD_HANDLER_INFO( GZ_GUILD, GZ_GUILD_RANKUP_SYN, PHASE_UNLIMITED),

#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
		ADD_HANDLER_INFO( GZ_GUILD, GZ_GUILD_CHUNTING_PVP_RESULT_CMD, PHASE_UNLIMITED),
#endif
		ADD_HANDLER_INFO( GZ_GUILD, GZ_GUILD_CHANGE_DUTY_SYN, PHASE_UNLIMITED ),				//��� ��å ����	
		ADD_HANDLER_INFO( GZ_GUILD, GZ_GUILD_RIGHT_DELEGATION_LIST_SYN, PHASE_UNLIMITED ),		//��� ���� ���� ��� ��û	
		ADD_HANDLER_INFO( GZ_GUILD, GZ_GUILD_RIGHT_DELEGATION_SYN, PHASE_UNLIMITED ),			//��� ���� ����
		ADD_HANDLER_INFO( GZ_GUILD, GZ_GUILD_CHANGE_MASTER_SYN, PHASE_UNLIMITED ),				//����庯��

        ADD_HANDLER_INFO( GZ_GUILD, GZ_GUILD_CREATE_CORPS_SYN, PHASE_UNLIMITED ),					//��� ���� ����
		ADD_HANDLER_INFO( GZ_GUILD, GZ_GUILD_DESTROY_CORPS_SYN, PHASE_UNLIMITED ),					//��� ���� �ػ�
		ADD_HANDLER_INFO( GZ_GUILD, GZ_GUILD_EMPTY_COMMANDER_INFO_SYN, PHASE_UNLIMITED ),			//��� �� ������ ���� ��û.
		ADD_HANDLER_INFO( GZ_GUILD, GZ_GUILD_CHANGE_COMMANDER_SYN, PHASE_UNLIMITED ),				//��� ������ ����.
		ADD_HANDLER_INFO( GZ_GUILD, GZ_GUILD_CHANGE_CORPS_SYN, PHASE_UNLIMITED ),					//��� ���� ����.
		ADD_HANDLER_INFO( GZ_GUILD, GZ_GUILD_CREATE_REAR_CAMP_SYN, PHASE_UNLIMITED ),				//��� ����ķ�� ����.
		ADD_HANDLER_INFO( GZ_GUILD, GZ_GUILD_DESTROY_REAR_CAMP_SYN, PHASE_UNLIMITED ),				//��� ����ķ�� �ػ�.
		ADD_HANDLER_INFO( GZ_GUILD, GZ_GUILD_INVITE_SYN, PHASE_UNLIMITED ),							//��� �ʴ�.
		ADD_HANDLER_INFO( GZ_GUILD, GZ_GUILD_LOGIN_CHAR_SYN, PHASE_UNLIMITED ),						//��� ��� �α���.
		ADD_HANDLER_INFO( GZ_GUILD, GZ_GUILD_LOGOUT_CHAR_SYN, PHASE_UNLIMITED ),					//��� ��� �α׾ƿ�.
		ADD_HANDLER_INFO( GZ_GUILD, GZ_GUILD_CHAR_LEVELUP_SYN, PHASE_UNLIMITED ),					//���� ������.
		ADD_HANDLER_INFO( GZ_GUILD, GZ_GUILD_TOTALINFO_REQ_SYN, PHASE_UNLIMITED ),					//���� ������û.
		ADD_HANDLER_INFO( GZ_GUILD, GZ_GUILD_GM_GUILD_POINT_SYN, PHASE_UNLIMITED ),					//���� ����Ʈ ���� ��û.
		ADD_HANDLER_INFO( GZ_GUILD, GZ_GUILD_CHANGE_CHAOSTATE_SYN, PHASE_UNLIMITED),				//���� ī������ ����.
		ADD_HANDLER_INFO( GZ_GUILD, GZ_GUILD_ENTER_SERVER_SYN, PHASE_UNLIMITED),					//������ ���Ӽ������� �̵��� ��.
		ADD_HANDLER_INFO( GZ_GUILD, GZ_GUILD_PRE_LOGIN_CMD, PHASE_UNLIMITED),						//������ �α��� ���� �غ�.

		ADD_HANDLER_INFO( GZ_GUILD, GZ_GUILD_RELATION_SYN, PHASE_UNLIMITED),
		ADD_HANDLER_INFO( GZ_GUILD, GZ_GUILD_RELATION_DELETE_SYN, PHASE_UNLIMITED),
		ADD_HANDLER_INFO( GZ_GUILD, GZ_GUILD_RELATION_BOTH_AGAINST_SYN, PHASE_UNLIMITED),
		ADD_HANDLER_INFO( GZ_GUILD, GZ_GUILD_RELATION_ALLIENCE_SYN, PHASE_UNLIMITED),

        ADD_HANDLER_INFO( GZ_GUILD, GZ_GUILD_RECOVERY_GUILD_SYN, PHASE_UNLIMITED),
		ADD_HANDLER_INFO( GZ_GUILD, GZ_GUILD_GUILDMARK_REGISTER_SYN, PHASE_UNLIMITED),
		ADD_HANDLER_INFO( GZ_GUILD, GZ_GUILD_GUILDMARK_DELETE_SYN, PHASE_UNLIMITED),

        ADD_HANDLER_INFO( GZ_GUILD, GZ_GUILD_WAREHOUSE_OPEN_SYN, PHASE_UNLIMITED),
		ADD_HANDLER_INFO( GZ_GUILD, GZ_GUILD_WAREHOUSE_CLOSE_SYN, PHASE_UNLIMITED),
		ADD_HANDLER_INFO( GZ_GUILD, GZ_GUILD_RELATION_INFO_SYN, PHASE_UNLIMITED),
		ADD_HANDLER_INFO( GZ_GUILD, GZ_GUILD_NAME_CHANGE_SYN, PHASE_UNLIMITED),
        //{ __KR_001385_20090907_DOMINATION_PACKET
        ADD_HANDLER_INFO( GZ_GUILD, GZ_DOMINATION_INFO_SYN, PHASE_UNLIMITED),
        ADD_HANDLER_INFO( GZ_GUILD, GZ_DOMINATION_AUCTION_INFO_SYN, PHASE_UNLIMITED),
        ADD_HANDLER_INFO( GZ_GUILD, GZ_DOMINATION_UPDATE_INFO_SYN, PHASE_UNLIMITED),
        ADD_HANDLER_INFO( GZ_GUILD, GZ_DOMINATION_ACCEP_HEIM_SYN, PHASE_UNLIMITED),
        ADD_HANDLER_INFO( GZ_GUILD, GZ_DOMINATION_INIT_ACTIONINFO_SYN, PHASE_UNLIMITED),
        ADD_HANDLER_INFO( GZ_GUILD, GZ_DOMINATION_RETURNHIME_SYN, PHASE_UNLIMITED),
        ADD_HANDLER_INFO( GZ_GUILD, GZ_DOMINATION_BATTING_HEIM_SYN, PHASE_UNLIMITED),
        //}
#ifdef _NA_20100604_BETTER_DOMINATION_MANAGER
        ADD_HANDLER_INFO( GZ_GUILD, GZ_DOMINATION_UPDATE_GUILDINFO_SYN, PHASE_UNLIMITED),
#endif
        //_NA_20100913_GUILD_WAREHOUSE_GM_COMMAND
        ADD_HANDLER_INFO(GZ_GUILD, GZ_GUILD_GM_WAREHOUSE_FORCE_CLOSE_CMD, PHASE_UNLIMITED),         // GM ��ɿ� ���� ��� â�� ���� �ݱ�
#ifdef _NA_20110216_INCREASE_DOMINATION_DEFFENSEWALL_POINT
        ADD_HANDLER_INFO(GZ_GUILD, GZ_DOMINATION_ADD_DEFFENSEWALL_POINT_SYN, PHASE_UNLIMITED),
#endif
#ifdef _NA_20110406_CHANGE_RULE_FOR_DOMINATION_JOIN
        ADD_HANDLER_INFO(GZ_GUILD, GZ_DOMINATION_MEMBER_JOIN_SYN, PHASE_UNLIMITED),
        ADD_HANDLER_INFO(GZ_GUILD, GZ_DOMINATION_MEMBER_LEAVE_SYN, PHASE_UNLIMITED),
        ADD_HANDLER_INFO(GZ_GUILD, GZ_DOMINATION_MEMBERLIST_REQ, PHASE_UNLIMITED),
        ADD_HANDLER_INFO(GZ_GUILD, GZ_DOMINATION_MEMBERLIST_INIT_CMD, PHASE_UNLIMITED),
#endif
		ADD_HANDLER_INFO_EOR(),
	};

	return s_pHandlerInfo;
};

VOID	Handler_FromGameServer::RegisterHandler_ALL( PACKET_HANDLER_DELEGATOR pInstance )
{
	pInstance->RegisterHandler_<GAME_SERVER>( GetHandlerInfo() );
	pInstance->RegisterHandler_<GAME_SERVER>( PacketHandlerShared::GetHandlerInfo() );
}





Handler_FromGameServer_IMPL(GZ_GUILD_CREATE_SYN)
{
	MSG_GZ_GUILD_CREATE_SYN * pRecvMsg = (MSG_GZ_GUILD_CREATE_SYN *)pMsg;

	RC::eGUILD_RESULT rt = g_GuildManager.CanCreateGuild( pRecvMsg->m_MasterCharGuid, pRecvMsg->m_MasterLV );

	if( rt == RC::RC_GUILD_SUCCESS )
	{
		MSG_DZ_GUILD_CREATE_SYN msg;
		msg.m_dwKey				= pRecvMsg->m_dwKey;
		msg.m_dwSessionIndex	= pServerSession->GetSessionIndex();
		msg.m_MasterCharGuid	= pRecvMsg->m_MasterCharGuid;
        _tcsncpy(msg.m_tszGuildName, pRecvMsg->m_tszGuildName, _countof(msg.m_tszGuildName));
        msg.m_tszGuildName[_countof(msg.m_tszGuildName) - 1] = _T('\0');
        _tcsncpy(msg.m_szMasterCharName, pRecvMsg->m_tszMasterCharName,
                 _countof(msg.m_szMasterCharName));
        msg.m_szMasterCharName[_countof(msg.m_szMasterCharName) - 1] = _T('\0');
		msg.m_byClass = pRecvMsg->m_byMasterClass;
		msg.m_Level	= pRecvMsg->m_MasterLV;
		GuildServer::GetMainFrame()->ConnectSessions.SendToDBProxy( &msg, sizeof(msg) );
	}
	else
	{
		MSG_GZ_GUILD_CREATE_NAK msg;
		msg.m_dwKey			= pRecvMsg->m_dwKey;
		msg.m_byErrorCode = rt;
		pServerSession->SendPacket( &msg, sizeof(msg) );
	}


}

Handler_FromGameServer_IMPL(GZ_GUILD_DESTROY_SYN)
{
	const MSG_GZ_GUILD_DESTROY_SYN* receive_msg = static_cast<MSG_GZ_GUILD_DESTROY_SYN*>(pMsg);
    
#ifdef _USING_GUILD_DESTROY_LOGGING
    SUNLOG(eCRITICAL_LOG, 
        _T("|����ػ��û|Handler_FromGameServer_IMPL(GZ_GUILD_DESTROY_SYN)|MSG_GZ_GUILD_DESTROY_SYN ����|GuildGuid:%d, MasterCharGuid:%d|"), 
        receive_msg->m_GuildGuid, 
        receive_msg->m_MasterCharGuid);
#endif // _USING_GUILD_DESTROY_LOGGING

    const RC::eGUILD_RESULT result = g_GuildManager.CanDestroyGuild(receive_msg->m_GuildGuid,
                                                                    receive_msg->m_MasterCharGuid);
	if (result == RC::RC_GUILD_SUCCESS)
	{
		MSG_DZ_GUILD_DESTROY_SYN msg;
		msg.m_dwKey	= receive_msg->m_dwKey;
		msg.m_dwSessionIndex = pServerSession->GetSessionIndex();
		msg.m_MasterCharGuid = receive_msg->m_MasterCharGuid;
		msg.m_GuildGuid	= receive_msg->m_GuildGuid;
        msg.m_DestroyWaitTime = receive_msg->m_DestroyWaitTime;        

#ifdef _USING_GUILD_DESTROY_LOGGING
        SUNLOG(eCRITICAL_LOG, 
            _T("|����ػ��û|Handler_FromGameServer_IMPL(GZ_GUILD_DESTROY_SYN)|MSG_DZ_GUILD_DESTROY_SYN ����|Key:%u, SessionIndex:%u, MasterCharGuid:%u, GuildGuid:%d, DestroyWaitTime:%u|"), 
            msg.m_dwKey, 
            msg.m_dwSessionIndex, 
            msg.m_MasterCharGuid, 
            msg.m_GuildGuid, 
            msg.m_DestroyWaitTime);
#endif // _USING_GUILD_DESTROY_LOGGING

		GuildServer::GetMainFrame()->ConnectSessions.SendToDBProxy(&msg, sizeof(msg));
	}
	else
	{
		MSG_GZ_GUILD_DESTROY_NAK nak_msg;
		nak_msg.m_dwKey	= receive_msg->m_dwKey;
		nak_msg.m_byErrorCode = result;

#ifdef _USING_GUILD_DESTROY_LOGGING
        SUNLOG(eCRITICAL_LOG, 
            _T("|����ػ��û|Handler_FromGameServer_IMPL(GZ_GUILD_DESTROY_SYN)|MSG_GZ_GUILD_DESTROY_NAK ����|Key:%u, ErrorCode:%d|"), 
            nak_msg.m_dwKey, 
            nak_msg.m_byErrorCode);
#endif // _USING_GUILD_DESTROY_LOGGING

		pServerSession->SendPacket(&nak_msg, sizeof(nak_msg));
	}
}

//Ż��.
Handler_FromGameServer_IMPL(GZ_GUILD_WITHDRAW_SYN)
{
	MSG_GZ_GUILD_WITHDRAW_SYN * pRecvMsg = (MSG_GZ_GUILD_WITHDRAW_SYN *)pMsg;

	RC::eGUILD_RESULT rt = g_GuildManager.CanLeaveGuild( pRecvMsg->m_GuildGuid, pRecvMsg->m_CharGuid );
	if( rt != RC::RC_GUILD_SUCCESS )
	{
		MSG_GZ_GUILD_WITHDRAW_NAK msgNAK;
		msgNAK.m_dwKey			= pRecvMsg->m_dwKey;
		msgNAK.m_byErrorCode	= rt;
		pServerSession->SendPacket( &msgNAK, sizeof(msgNAK) );
		return;
	}

	MSG_DZ_GUILD_WITHDRAW_SYN msgSYN;
	msgSYN.m_dwKey			= pRecvMsg->m_dwKey;
	msgSYN.m_dwSessionIndex	= pServerSession->GetSessionIndex();
	msgSYN.m_GuildGuid		= pRecvMsg->m_GuildGuid;
	msgSYN.m_CharGuid		= pRecvMsg->m_CharGuid;
	msgSYN.m_byPenaltyType	= 1; // �ڱ��ڽ� Ż��
	if( !GuildServer::GetMainFrame()->ConnectSessions.SendToDBProxy( &msgSYN, sizeof(msgSYN) ) )
	{
		MSG_GZ_GUILD_WITHDRAW_NAK msgNAK;
		msgNAK.m_dwKey			= pRecvMsg->m_dwKey;
		msgNAK.m_byErrorCode	= RC::RC_GUILD_FAILED_BY_NOT_CONNECT_SERVER;
		pServerSession->SendPacket( &msgNAK, sizeof(msgNAK) );
		return;
	}
}

// ���� ����
Handler_FromGameServer_IMPL(GZ_GUILD_KICKPLAYER_SYN)
{
	MSG_GZ_GUILD_KICKPLAYER_SYN* pRecvMsg = (MSG_GZ_GUILD_KICKPLAYER_SYN*)pMsg;

	if( _tcslen( pRecvMsg->m_ptszCharName ) > MAX_CHARNAME_LENGTH )
	{
		MSG_GZ_GUILD_KICKPLAYER_NAK nakMsg;
		nakMsg.m_dwKey			= pRecvMsg->m_dwKey;
		nakMsg.m_dwErrorCode	= RC::RC_GUILD_FAILED;
		pServerSession->SendPacket( &nakMsg, sizeof(nakMsg) );
		DISPMSG( "KICKPLAYER Error - KickCharName Overflow[GuildGuid:%u]\n", pRecvMsg->m_GuildGuid );
		return;
	}

	RC::eGUILD_RESULT rt = g_GuildManager.CanKickMember( pRecvMsg->m_GuildGuid, pRecvMsg->m_OperCharGuid, pRecvMsg->m_ptszCharName );
	if( rt != RC::RC_GUILD_SUCCESS )
	{
		MSG_GZ_GUILD_KICKPLAYER_NAK nakMsg;
		nakMsg.m_dwKey			= pRecvMsg->m_dwKey;
		_tcsncpy( nakMsg.m_ptszCharName, pRecvMsg->m_ptszCharName, MAX_CHARNAME_LENGTH );
		nakMsg.m_ptszCharName[MAX_CHARNAME_LENGTH] = '\0';
		nakMsg.m_dwErrorCode	= rt;
		pServerSession->SendPacket( &nakMsg, sizeof(nakMsg) );
		return;
	}

	GuildMember* pOperMember = g_GuildManager.FindGuildMember(pRecvMsg->m_GuildGuid,pRecvMsg->m_OperCharGuid);
	if( !pOperMember ) return;

	GuildMember* pTargetMember = g_GuildManager.FindGuildMemberByName( pRecvMsg->m_GuildGuid, pRecvMsg->m_ptszCharName );
	if( !pTargetMember ) return;		//�� �׳� �����ϳĸ�... ������ �����Ǿ��� �����̴�. �ݵ�� �����Ѵ�. 

	MSG_DZ_GUILD_KICKPLAYER_SYN msg;
	msg.m_dwKey = pRecvMsg->m_dwKey;
	msg.m_GuildGuid	= pRecvMsg->m_GuildGuid;
    msg.m_OperCharGuid = pOperMember->GetCharGuid();
	msg.m_TargetCharGuid = pTargetMember->GetCharGuid();
	msg.m_byCorpsNum = pOperMember->GetCorpsNumber();
	msg.m_dwSessionIndex = pServerSession->GetSessionIndex();
	if( !GuildServer::GetMainFrame()->ConnectSessions.SendToDBProxy( &msg, sizeof(msg) ) )
	{
		MSG_GZ_GUILD_KICKPLAYER_NAK nakMsg;
		nakMsg.m_dwKey			= pRecvMsg->m_dwKey;
		_tcsncpy( nakMsg.m_ptszCharName, pRecvMsg->m_ptszCharName, MAX_CHARNAME_LENGTH );
		nakMsg.m_ptszCharName[MAX_CHARNAME_LENGTH] = '\0';
		nakMsg.m_dwErrorCode	= RC::RC_GUILD_FAILED_BY_NOT_CONNECT_SERVER;
		pServerSession->SendPacket( &nakMsg, sizeof(nakMsg) );
		return;
	}
}


////////////////////
// ��� ����Ʈ ����

// UP ����
Handler_FromGameServer_IMPL(GZ_GUILD_DONATE_UP_SYN)
{
	MSG_GZ_GUILD_DONATE_UP_SYN* pRecvMsg = (MSG_GZ_GUILD_DONATE_UP_SYN*)pMsg;

	RC::eGUILD_RESULT rt = g_GuildManager.CanDonateUP( pRecvMsg->m_GuildGuid, pRecvMsg->m_CharGuid );
	if( rt != RC::RC_GUILD_SUCCESS )
	{
		MSG_GZ_GUILD_DONATE_UP_NAK nakMsg;
		nakMsg.m_dwKey			= pRecvMsg->m_dwKey;
		nakMsg.m_GuildGuid		= pRecvMsg->m_GuildGuid;
		nakMsg.m_byErrorCode	= rt;
		pServerSession->SendPacket( &nakMsg, sizeof(nakMsg) );
		return;
	}

	MSG_DZ_GUILD_DONATE_UP_SYN sendMsg;
	sendMsg.m_dwKey = pRecvMsg->m_dwKey;
	sendMsg.m_dwSessionIndex = pServerSession->GetSessionIndex();	// ���Ӽ����� ������ �����Ѵ�.
	sendMsg.m_GuildGuid = pRecvMsg->m_GuildGuid;
	sendMsg.m_CharGuid = pRecvMsg->m_CharGuid;
	sendMsg.m_GuildUP = pRecvMsg->m_DonateUP;
	if( !GuildServer::GetMainFrame()->ConnectSessions.SendToDBProxy( &sendMsg, sizeof(sendMsg) ) )
	{
		SUNLOG( eCRITICAL_LOG, "DZ_GUILD_DONATE_UP Error - Can't send Packet![GuildGuid:%u]\n", pRecvMsg->m_GuildGuid );
		MSG_DZ_GUILD_DONATE_UP_NAK nakMsg;
		nakMsg.m_GuildGuid = pRecvMsg->m_GuildGuid;
		nakMsg.m_byErrorCode = RC::RC_GUILD_FAILED;
		pServerSession->SendPacket( &nakMsg, sizeof(nakMsg) );
		return;
	}
}

// UP->GP ��ȯ
Handler_FromGameServer_IMPL(GZ_GUILD_CONVERT_UP2GP_SYN)
{
	MSG_GZ_GUILD_CONVERT_UP2GP_SYN* pRecvMsg = (MSG_GZ_GUILD_CONVERT_UP2GP_SYN*)pMsg;

	RC::eGUILD_RESULT rt = g_GuildManager.CanConvertUP2GP( pRecvMsg->m_GuildGuid, pRecvMsg->m_OperCharGuid, pRecvMsg->m_ConvertGP );
	if( rt != RC::RC_GUILD_SUCCESS )
	{
		// �⺻���� ���� �˻�� GameServer���� �ߴٰ� �Ǵ��ϹǷ� ���� �˻����� �ʰ� �ٷ� DBProxy�� ������
		MSG_GZ_GUILD_CONVERT_UP2GP_NAK nakMsg;
		nakMsg.m_dwKey = pRecvMsg->m_dwKey;
		nakMsg.m_GuildGuid = pRecvMsg->m_GuildGuid;
		nakMsg.m_byErrorCode = rt;
		pServerSession->SendPacket( &nakMsg, sizeof(nakMsg) );
		return;
	}

	MSG_DZ_GUILD_CONVERT_UP2GP_SYN sendMsg;
	sendMsg.m_dwKey = pRecvMsg->m_dwKey;
	sendMsg.m_dwSessionIndex = pServerSession->GetSessionIndex();
	sendMsg.m_GuildGuid = pRecvMsg->m_GuildGuid;
	sendMsg.m_ConverGP = pRecvMsg->m_ConvertGP;
	if( !GuildServer::GetMainFrame()->ConnectSessions.SendToDBProxy( &sendMsg, sizeof(sendMsg) ) )
	{
		MSG_GZ_GUILD_CONVERT_UP2GP_NAK nakMsg;
		nakMsg.m_dwKey = pRecvMsg->m_dwKey;
		nakMsg.m_GuildGuid = pRecvMsg->m_GuildGuid;
		nakMsg.m_byErrorCode = RC::RC_GUILD_FAILED_BY_NOT_CONNECT_SERVER;
		pServerSession->SendPacket( &nakMsg, sizeof(nakMsg) );
		return;
	}
}

// ��� ��ũ��
Handler_FromGameServer_IMPL(GZ_GUILD_RANKUP_SYN)
{
	MSG_GZ_GUILD_RANKUP_SYN*	pRecvMsg = (MSG_GZ_GUILD_RANKUP_SYN*)pMsg;
	GPTYPE						UseGP = 0;

	GuildRankInfo RankInfo;
	RC::eGUILD_RESULT rt = g_GuildManager.CanRankUp( pRecvMsg->m_GuildGuid, pRecvMsg->m_OperCharGuid, RankInfo );

	if( rt != RC::RC_GUILD_SUCCESS )
	{
		MSG_GZ_GUILD_RANKUP_NAK msg;
		msg.m_dwKey			= pRecvMsg->m_dwKey;
		msg.m_GuildGuid		= pRecvMsg->m_GuildGuid;
		msg.m_byErrorCode	= rt;
		pServerSession->SendPacket( &msg, sizeof(msg) );
		SUNLOG( eFULL_LOG, "[GZ_GUILD_RANKUP_SYN] RankUp Fail [GuildGuid:%u][ErrorCode:%u]", pRecvMsg->m_GuildGuid, rt );
		return;
	}

	// ���� ���Ӽ������� üũ�ϰ� �������� ���⼭ üũ�Ѵ�.
	MSG_DZ_GUILD_RANKUP_SYN sendMsg;
	sendMsg.m_dwSessionIndex = pServerSession->GetSessionIndex();
	sendMsg.m_dwKey = pRecvMsg->m_dwKey;
	sendMsg.m_GuildGuid = pRecvMsg->m_GuildGuid;
	sendMsg.m_UpdateGrade = RankInfo.m_byLevel;
	//DB�� ������ ���� ��ũ���� �ʿ��� �����̴�.
	sendMsg.m_UseUP = RankInfo.m_UP;
	sendMsg.m_UseGP = RankInfo.m_GP;

	if( !GuildServer::GetMainFrame()->ConnectSessions.SendToDBProxy( &sendMsg, sizeof(sendMsg) ) )
	{
		MSG_GZ_GUILD_RANKUP_NAK nakMsg;
		nakMsg.m_GuildGuid = pRecvMsg->m_GuildGuid;
		nakMsg.m_byErrorCode = RC::RC_GUILD_FAILED_BY_NOT_CONNECT_SERVER;
		pServerSession->SendPacket( &nakMsg, sizeof(nakMsg) );
		return;
	}
}


// ���� ī������ ����
Handler_FromGameServer_IMPL(GZ_GUILD_CHANGE_CHAOSTATE_SYN)
{
	MSG_GZ_GUILD_CHANGE_CHAOSTATE_SYN* pRecvMsg = (MSG_GZ_GUILD_CHANGE_CHAOSTATE_SYN*)pMsg;

	RC::eGUILD_RESULT rc = g_GuildManager.ChangeChaoState( pRecvMsg->m_GuildGuid, pRecvMsg->m_CharGuid, pRecvMsg->m_byChaoState );
	if( rc != RC::RC_GUILD_SUCCESS )
	{
		SUNLOG( eCRITICAL_LOG, "[Handler_FromGameServer_IMPL(GZ_GUILD_CHANGE_CHAOSTATE_SYN)] Fail, GuildGuid = %u, CharGuid = %d, ChaoState = %d",
								pRecvMsg->m_GuildGuid, pRecvMsg->m_CharGuid, pRecvMsg->m_byChaoState );
		return;
	}

	MSG_GZ_GUILD_CHANGE_CHAOSTATE_BRD brdMsg;
	brdMsg.m_GuildGuid = pRecvMsg->m_GuildGuid;
	g_GuildManager.GetCharName( pRecvMsg->m_GuildGuid, pRecvMsg->m_CharGuid, brdMsg.m_szCharName, MAX_CHARNAME_LENGTH );
	brdMsg.m_byChaoState = pRecvMsg->m_byChaoState;
	GuildServer::GetMainFrame()->SendToAllGameServers( &brdMsg, sizeof(brdMsg) );
}

Handler_FromGameServer_IMPL(GZ_GUILD_ENTER_SERVER_SYN)
{
	MSG_GZ_GUILD_ENTER_SERVER_SYN* pRecvMsg = (MSG_GZ_GUILD_ENTER_SERVER_SYN*)pMsg;

	//Invite�� Join�̳� ������ �Ȱ���.
	RC::eGUILD_RESULT rt = g_GuildManager.CanMoveServer( pRecvMsg->m_GuildGuid, pRecvMsg->m_CharGuid );
	if( rt != RC::RC_GUILD_SUCCESS )
	{
		MSG_GZ_GUILD_ENTER_SERVER_NAK nakMsg;
		nakMsg.m_byErrorCode = rt;
		pServerSession->SendPacket( &nakMsg, sizeof(nakMsg) );
		return;
	}

	// �ش� ���Ӽ����� ��� ������ ������.
	MSG_GZ_GUILD_ENTER_SERVER_ACK ackMsg;
	ackMsg.m_dwKey = pRecvMsg->m_dwKey;
	g_GuildManager.SerializeGameGuildInfo( pRecvMsg->m_GuildGuid, ackMsg.m_GameGuildInfo, SERIALIZE_LOAD );
	g_GuildManager.GetGuildBaseMemberInfo( pRecvMsg->m_GuildGuid, pRecvMsg->m_CharGuid, ackMsg.m_BaseMemberInfo );
	pServerSession->SendPacket( &ackMsg, sizeof(ackMsg) );
}

Handler_FromGameServer_IMPL(GZ_GUILD_PRE_LOGIN_CMD)
{
	MSG_GZ_GUILD_PRE_LOGIN_CMD* pRecvMsg = (MSG_GZ_GUILD_PRE_LOGIN_CMD*)pMsg;

	Guild* pGuild = g_GuildManager.FindGuild( pRecvMsg->m_GuildGuid );
	if( !pGuild )	return;

	GuildMember* pMember = pGuild->FindMember( pRecvMsg->m_CharGuid );
	if( !pMember )	return;

	if( pMember->IsLogIn() == TRUE )
	{
		SUNLOG( eCRITICAL_LOG, "[Handler_FromGameServer_IMPL(GZ_GUILD_PRE_LOGIN_CMD)] Already Loin Char And Init GuildGuid = %u, CharGuid = %u",
			pRecvMsg->m_GuildGuid, pRecvMsg->m_CharGuid );

		pMember->SetLogin( FALSE );
	}
}

#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
Handler_FromGameServer_IMPL(GZ_GUILD_CHUNTING_PVP_RESULT_CMD)
{
	MSG_GZ_GUILD_CHUNTING_PVP_RESULT_CMD * pRecvMsg = (MSG_GZ_GUILD_CHUNTING_PVP_RESULT_CMD *)pMsg;

	MSG_DZ_GUILD_CHUNTING_PVP_RESULT_SYN msg;
	msg.m_dwKey = pRecvMsg->m_dwKey;
	msg.m_dwSessionIndex = pServerSession->GetSessionIndex();

	RC::eGUILD_RESULT rt = g_GuildManager.ChuntingPVPResult( pRecvMsg->m_WinGuildGuid, pRecvMsg->m_LoseGuildGuid, pRecvMsg->m_DisGuildGuid );
	if( rt == RC::RC_GUILD_SUCCESS )
	{
		Guild *pWinGuild = g_GuildManager.FindGuild(pRecvMsg->m_WinGuildGuid);
		if( pWinGuild )
		{
			msg.m_GuildGuid = pRecvMsg->m_WinGuildGuid;
			msg.m_PVPInfo = *pWinGuild->GetChuntingPVPInfo();
			GuildServer::GetMainFrame()->ConnectSessions.SendToDBProxy( &msg, sizeof(msg) );
		}

		if( pRecvMsg->m_DisGuildGuid )	msg.m_GuildGuid = pRecvMsg->m_DisGuildGuid;
		else							msg.m_GuildGuid = pRecvMsg->m_LoseGuildGuid;
		
		Guild *pLoseGuild = g_GuildManager.FindGuild(msg.m_GuildGuid);
		if( pLoseGuild )
		{
			msg.m_PVPInfo = *pLoseGuild->GetChuntingPVPInfo();
			GuildServer::GetMainFrame()->ConnectSessions.SendToDBProxy( &msg, sizeof(msg) );
		}
	}
}
#endif

// ��� ��å ����
Handler_FromGameServer_IMPL(GZ_GUILD_CHANGE_DUTY_SYN)
{
	MSG_GZ_GUILD_CHANGE_DUTY_SYN *pRecvMsg = (MSG_GZ_GUILD_CHANGE_DUTY_SYN *)pMsg;

	GuildMember* pTargerMember = g_GuildManager.FindGuildMemberByName( pRecvMsg->m_GuildGuid, pRecvMsg->m_szTargetCharName );
	if( !pTargerMember )
	{
		//Ŭ���̾�Ʈ���� �׷� Ÿ�ٸ�� ���ٰ� NAK������.
		MSG_GZ_GUILD_CHANGE_DUTY_NAK nakMSG;
		nakMSG.m_dwKey			= pRecvMsg->m_dwKey;
		nakMSG.m_byErrorCode	= RC::RC_GUILD_NOT_GUILD_MEMBER;
		pServerSession->SendPacket(&nakMSG, sizeof(nakMSG));
		return;
	}

	// 1. ���⼭ ����� ������ üũ�Ѵ�.
	RC::eGUILD_RESULT rt = g_GuildManager.CanChangeDuty( pRecvMsg->m_GuildGuid, pRecvMsg->m_OperCharGuid, 
		pTargerMember->GetCharGuid(), (eGUILD_DUTY)pRecvMsg->m_byDuty );
	
	//��� ���� DB���� ���� ACK�� �ް� �����Ѵ�. 
	if(rt != RC::RC_GUILD_SUCCESS)
	{
		MSG_GZ_GUILD_CHANGE_DUTY_NAK nakMSG;
		nakMSG.m_dwKey			= pRecvMsg->m_dwKey;
		nakMSG.m_byErrorCode	= rt;
		pServerSession->SendPacket(&nakMSG, sizeof(nakMSG));
		SUNLOG( eFULL_LOG, "[Handler_FromGameServer_IMPL(GZ_GUILD_CHANGE_DUTY_SYN)] Fail, [GUILDGUID:%d][OPERGUID:%d][Target:%s][ChangeDuty:%d] rt = %d",
			pRecvMsg->m_GuildGuid, pRecvMsg->m_OperCharGuid, pRecvMsg->m_szTargetCharName, pRecvMsg->m_byDuty, rt );
		return;
	}

	//DB�� ����
	MSG_DZ_GUILD_CHANGE_DUTY_SYN sndMSG;
	sndMSG.m_dwKey			= pRecvMsg->m_dwKey;
	sndMSG.m_dwSessionIndex = pServerSession->GetSessionIndex();
	sndMSG.m_GuildGuid		= pRecvMsg->m_GuildGuid;
	sndMSG.m_OperGuid		= pRecvMsg->m_OperCharGuid;
	sndMSG.m_TargetGuid		= pTargerMember->GetCharGuid();
	sndMSG.m_eNewDuty		= (eGUILD_DUTY)pRecvMsg->m_byDuty;
	sndMSG.m_eOldDuty		= pTargerMember->GetDuty();

	if( !GuildServer::GetMainFrame()->ConnectSessions.SendToDBProxy(&sndMSG, sizeof(sndMSG)) )
	{
		MSG_GZ_GUILD_CHANGE_DUTY_NAK nakMSG;
		nakMSG.m_dwKey			= pRecvMsg->m_dwKey;
		nakMSG.m_byErrorCode	= RC::RC_GUILD_FAILED_BY_NOT_CONNECT_SERVER;
		pServerSession->SendPacket(&nakMSG, sizeof(nakMSG));
		SUNLOG( eCRITICAL_LOG, "[Handler_FromGameServer_IMPL(GZ_GUILD_CHANGE_DUTY_SYN)] Can't send Packet![GuildGuid:%u]\n", pRecvMsg->m_GuildGuid );			
		return;
	}
}

//��� ���� ���� ��� ��û	
Handler_FromGameServer_IMPL(GZ_GUILD_RIGHT_DELEGATION_LIST_SYN)
{
	MSG_GZ_GUILD_RIGHT_DELEGATION_LIST_SYN* pRecvMsg = (MSG_GZ_GUILD_RIGHT_DELEGATION_LIST_SYN*)pMsg;
	MSG_GZ_GUILD_RIGHT_DELEGATION_LIST_NAK		nakMSG;
	nakMSG.m_dwKey = pRecvMsg->m_dwKey;

	//1. ��尡 �ִ��� üũ
	Guild* pGuild = g_GuildManager.FindGuild( pRecvMsg->m_GuildGuid );
	if( !pGuild )
	{
		nakMSG.m_byErrorCode = RC::RC_GUILD_NOT_EXIST_GUILD;
		pServerSession->SendPacket(&nakMSG, sizeof(nakMSG));
		return;
	}

	//2. ����Ʈ ��û�ڰ� ����Ʈ�� ��û�ص� �Ǵ� ������� üũ�Ѵ�.
	if( !g_GuildManager.CheckRight( pRecvMsg->m_GuildGuid, pRecvMsg->m_OperCharGuid, eGUILD_RIGHTS_VIEW_RIGHT ) )
	{
		nakMSG.m_byErrorCode = RC::RC_GUILD_HAVE_NOT_RIGHT;
		pServerSession->SendPacket(&nakMSG, sizeof(nakMSG));
		return;
	}

	//3. �������Ӵ���� ���Ӵ���� �Ǵ��� üũ.
	RC::eGUILD_RESULT rc = g_GuildManager.IsDelegateChar( pRecvMsg->m_GuildGuid, pRecvMsg->m_szTargetCharName );	
	if( rc != RC::RC_GUILD_SUCCESS )
	{
		nakMSG.m_byErrorCode = rc;
		pServerSession->SendPacket(&nakMSG, sizeof(nakMSG));
		return;
	}

	//4. �����Ҽ� �ִ� ���� ����Ʈ�� �ҷ��´�. �ҷ��ö��� ���� �ش� ������ ���ӵ� ���ѱ��� ���� �����´�.
	MSG_GZ_GUILD_RIGHT_DELEGATION_LIST_ACK			Ack;
	Ack.m_dwKey = pRecvMsg->m_dwKey;
	Ack.m_GuildGuid = pRecvMsg->m_GuildGuid;
	g_GuildManager.GetDelegateRight( pRecvMsg->m_GuildGuid, pRecvMsg->m_szTargetCharName, Ack.m_DelegateRight );
	strncpy( Ack.m_szTargetCharName, pRecvMsg->m_szTargetCharName, MAX_CHARNAME_LENGTH );
	pServerSession->SendPacket( &Ack, sizeof(Ack) );
	
}

//��� ���� ����
Handler_FromGameServer_IMPL(GZ_GUILD_RIGHT_DELEGATION_SYN)
{
	MSG_GZ_GUILD_RIGHT_DELEGATION_SYN*		pRecvMsg = (MSG_GZ_GUILD_RIGHT_DELEGATION_SYN*)pMsg;

	RC::eGUILD_RESULT rc = g_GuildManager.CanRightDelegate( pRecvMsg->m_GuildGuid, pRecvMsg->m_OperCharGuid, pRecvMsg->m_szTargetCharName, pRecvMsg->m_DelegateRight );
	if( rc != RC::RC_GUILD_SUCCESS )
	{
		MSG_GZ_GUILD_RIGHT_DELEGATION_NAK		nakMSG;
		nakMSG.m_byErrorCode = RC::RC_GUILD_INVALID_DELEGATE_RIGHT;
		pServerSession->SendPacket(&nakMSG, sizeof(nakMSG));
		return;
	}

	GuildMember* pTargetMember = g_GuildManager.FindGuildMemberByName( pRecvMsg->m_GuildGuid, pRecvMsg->m_szTargetCharName );
	if( !pTargetMember )
		return;					//������ ����....

	//5. DB�� ������.
	MSG_DZ_GUILD_RIGHT_DELEGATION_SYN	synMsg;
	synMsg.m_dwKey			= pRecvMsg->m_dwKey;
	synMsg.m_dwSessionIndex = pServerSession->GetSessionIndex();
	synMsg.m_GuildGuid		= pRecvMsg->m_GuildGuid;
	synMsg.m_TargetGuid		= pTargetMember->GetCharGuid();
	CopyMemory( synMsg.m_DelegateRight, pRecvMsg->m_DelegateRight, sizeof(synMsg.m_DelegateRight) );
	if( !GuildServer::GetMainFrame()->ConnectSessions.SendToDBProxy( &synMsg, sizeof(synMsg) ) )
	{
		MSG_GZ_GUILD_RIGHT_DELEGATION_NAK		nakMSG;
		nakMSG.m_byErrorCode = RC::RC_GUILD_FAILED_BY_NOT_CONNECT_SERVER;
		pServerSession->SendPacket(&nakMSG, sizeof(nakMSG));
		return;
	}
}

//����� ����
Handler_FromGameServer_IMPL(GZ_GUILD_CHANGE_MASTER_SYN)
{
	MSG_GZ_GUILD_CHANGE_MASTER_SYN*		pRecvMsg = (MSG_GZ_GUILD_CHANGE_MASTER_SYN*)pMsg;

	RC::eGUILD_RESULT	rc = g_GuildManager.CanChangeMaster( pRecvMsg->m_GuildGuid, pRecvMsg->m_OperCharGuid, pRecvMsg->m_szTargetCharName );
	if( rc != RC::RC_GUILD_SUCCESS )
	{
		MSG_GZ_GUILD_CHANGE_MASTER_NAK		nakMSG;
		nakMSG.m_dwKey = pRecvMsg->m_dwKey;
		nakMSG.m_byErrorCode = rc;
		pServerSession->SendPacket(&nakMSG, sizeof(nakMSG));
		return;
	}

	GuildMember* pTargetMember = g_GuildManager.FindGuildMemberByName( pRecvMsg->m_GuildGuid, pRecvMsg->m_szTargetCharName );
	if( !pTargetMember )		return;

	//5. DB�� ������.
	MSG_DZ_GUILD_CHANGE_MASTER_SYN	synMsg;
	synMsg.m_dwKey			= pRecvMsg->m_dwKey;
	synMsg.m_dwSessionIndex = pServerSession->GetSessionIndex();
	synMsg.m_GuildGuid		= pRecvMsg->m_GuildGuid;
	synMsg.m_OperCharGuid	= pRecvMsg->m_OperCharGuid;
	synMsg.m_TargetCharGuid = pTargetMember->GetCharGuid();
	if( !GuildServer::GetMainFrame()->ConnectSessions.SendToDBProxy( &synMsg, sizeof(synMsg) ) )
	{
		MSG_GZ_GUILD_CHANGE_MASTER_NAK		nakMSG;
		nakMSG.m_byErrorCode = RC::RC_GUILD_FAILED;
		pServerSession->SendPacket(&nakMSG, sizeof(nakMSG));
		return;
	}
}

//��� ���� ����
Handler_FromGameServer_IMPL(GZ_GUILD_CREATE_CORPS_SYN)
{
	MSG_GZ_GUILD_CREATE_CORPS_SYN *pRecvMsg = (MSG_GZ_GUILD_CREATE_CORPS_SYN *)pMsg;

	BYTE byCorpsNum = 0;
	RC::eGUILD_RESULT rt = g_GuildManager.CanCreateCorps( pRecvMsg->m_GuildGuid, pRecvMsg->m_OperCharGuid, pRecvMsg->m_szTargetCharName, byCorpsNum );
	if( rt != RC::RC_GUILD_SUCCESS )
	{
		MSG_GZ_GUILD_CREATE_CORPS_NAK nakMSG;
		nakMSG.m_dwKey = pRecvMsg->m_dwKey;
		nakMSG.m_byErrorCode = rt;
		pServerSession->SendPacket(&nakMSG, sizeof(nakMSG));
		return;
	}

	//������ ����� ���� ������ ������ �´�.
	GuildMember* pMember = g_GuildManager.FindGuildMemberByName( pRecvMsg->m_GuildGuid, pRecvMsg->m_szTargetCharName );
	if( !pMember )	return;

	MSG_DZ_GUILD_CREATE_CORPS_SYN synMSG;
	synMSG.m_dwKey			= pRecvMsg->m_dwKey;
	synMSG.m_dwSessionIndex = pServerSession->GetSessionIndex();
	synMSG.m_GuildGuid		= pRecvMsg->m_GuildGuid;
	synMSG.m_TargetCharGuid	= pMember->GetCharGuid();
	synMSG.m_byCorpsNum		= byCorpsNum;
	if( !GuildServer::GetMainFrame()->ConnectSessions.SendToDBProxy( &synMSG, sizeof(synMSG) ) )
	{
		MSG_GZ_GUILD_CREATE_CORPS_NAK nakMSG;
		nakMSG.m_dwKey = pRecvMsg->m_dwKey;
		nakMSG.m_byErrorCode = RC::RC_GUILD_CREATE_CORPS_FAILED;
		pServerSession->SendPacket(&nakMSG, sizeof(nakMSG));
		SUNLOG( eCRITICAL_LOG, "DZ_GUILD_CREATE_CORPS_SYN ERROR - Can't send Packet![GuildGuid:%u]\n", pRecvMsg->m_GuildGuid );
		return;
	}
}

Handler_FromGameServer_IMPL(GZ_GUILD_DESTROY_CORPS_SYN)
{
	MSG_GZ_GUILD_DESTROY_CORPS_SYN* pRecvMsg = (MSG_GZ_GUILD_DESTROY_CORPS_SYN	*)pMsg;

	RC::eGUILD_RESULT rt = g_GuildManager.CanDestoryCorps( pRecvMsg->m_GuildGuid, pRecvMsg->m_OperCharGuid, pRecvMsg->m_byCorpsNum );
	if( rt != RC::RC_GUILD_SUCCESS )
	{
		MSG_GZ_GUILD_DESTROY_CORPS_NAK nakMSG;
		nakMSG.m_dwKey = pRecvMsg->m_dwKey;
		nakMSG.m_byErrorCode = rt;
		pServerSession->SendPacket(&nakMSG, sizeof(nakMSG));
		SUNLOG( eFULL_LOG, "[GZ_GUILD_DESTROY_CORPS_SYN] ERROR, GuildGuid = %d, OperGuid = %u, CorpsNum = %d", pRecvMsg->m_GuildGuid,  pRecvMsg->m_OperCharGuid, pRecvMsg->m_byCorpsNum );
		return;
	}

	MSG_DZ_GUILD_DESTROY_CORPS_SYN synMsg;
	synMsg.m_dwKey			= pRecvMsg->m_dwKey;
	synMsg.m_dwSessionIndex = pServerSession->GetSessionIndex();
	synMsg.m_GuildGuid		= pRecvMsg->m_GuildGuid;
	synMsg.m_byCorpsNum		= pRecvMsg->m_byCorpsNum;
	synMsg.m_OperCharGuid   = pRecvMsg->m_OperCharGuid;

	if( !GuildServer::GetMainFrame()->ConnectSessions.SendToDBProxy( &synMsg, sizeof(synMsg) ) )
	{
		MSG_GZ_GUILD_DESTROY_CORPS_NAK nakMSG;
		nakMSG.m_dwKey = pRecvMsg->m_dwKey;
		nakMSG.m_byErrorCode = RC::RC_GUILD_FAILED_BY_NOT_CONNECT_SERVER;
		pServerSession->SendPacket(&nakMSG, sizeof(nakMSG));
		SUNLOG( eCRITICAL_LOG, "[MSG_GZ_GUILD_DESTROY_CORPS_SYN] ERROR - Can't send Packet![GuildGuid:%u]\n", pRecvMsg->m_GuildGuid );
		return;
	}
}

Handler_FromGameServer_IMPL(GZ_GUILD_EMPTY_COMMANDER_INFO_SYN)
{
	MSG_GZ_GUILD_EMPTY_COMMANDER_INFO_SYN* pRecvMsg = (MSG_GZ_GUILD_EMPTY_COMMANDER_INFO_SYN*)pMsg;

	//����ִ� ���������� ���ڴٴ°��� �ᱹ ��å������ �ϰڴٴ� ����. ���� ��å���� ������ ����.
	BOOL bRight = g_GuildManager.CheckRight( pRecvMsg->m_GuildGuid, pRecvMsg->m_OperCharGuid, eGUILD_RIGHTS_CHANGE_DUTY );
	if( !bRight )
	{
		MSG_GZ_GUILD_EMPTY_COMMANDER_INFO_NAK nakMSG;
		nakMSG.m_dwKey = pRecvMsg->m_dwKey;
		nakMSG.m_byErrorCode = RC::RC_GUILD_HAVE_NOT_RIGHT;
		pServerSession->SendPacket(&nakMSG, sizeof(nakMSG));
		return;
	}

	//������ ������ ���� ��� �ִ� ���� ������ ������ �´�.
	MSG_GZ_GUILD_EMPTY_COMMANDER_INFO_ACK	AckMsg;
	AckMsg.m_dwKey = pRecvMsg->m_dwKey;
	AckMsg.m_GuildGuid = pRecvMsg->m_GuildGuid;
	AckMsg.m_byEmptyCommanderCount = g_GuildManager.GetEmptyCommanderInfo( pRecvMsg->m_GuildGuid, AckMsg.m_byCorpsNum );
	pServerSession->SendPacket( &AckMsg, AckMsg.GetSize() );
}

Handler_FromGameServer_IMPL(GZ_GUILD_CHANGE_COMMANDER_SYN)
{
	MSG_GZ_GUILD_CHANGE_COMMANDER_SYN* pRecvMsg = (MSG_GZ_GUILD_CHANGE_COMMANDER_SYN*)pMsg;

	RC::eGUILD_RESULT rt = g_GuildManager.CanChangeCommander( pRecvMsg->m_GuildGuid, pRecvMsg->m_OperCharGuid, pRecvMsg->m_szTargetCharName, pRecvMsg->m_byCorpsNum );
	if( rt != RC::RC_GUILD_SUCCESS )
	{
		MSG_GZ_GUILD_CHANGE_COMMANDER_NAK nakMSG;
		nakMSG.m_dwKey = pRecvMsg->m_dwKey;
		nakMSG.m_byErrorCode = rt;
		pServerSession->SendPacket(&nakMSG, sizeof(nakMSG));
		return;
	}

	GuildMember* pMember = g_GuildManager.FindGuildMemberByName( pRecvMsg->m_GuildGuid, pRecvMsg->m_szTargetCharName );
	if( !pMember )		//������ �˻������Ƿ� ������ ����.
		return;

	MSG_DZ_GUILD_CHANGE_COMMANDER_SYN synMSG;
	synMSG.m_dwKey			= pRecvMsg->m_dwKey;
	synMSG.m_dwSessionIndex = pServerSession->GetSessionIndex();
	synMSG.m_GuildGuid		= pRecvMsg->m_GuildGuid;
	synMSG.m_TargetCharGuid	= pMember->GetCharGuid();
	synMSG.m_byCorpsNum		= pRecvMsg->m_byCorpsNum;
	if( !GuildServer::GetMainFrame()->ConnectSessions.SendToDBProxy( &synMSG, sizeof(synMSG) ) )
	{
		MSG_GZ_GUILD_CHANGE_COMMANDER_NAK nakMSG;
		nakMSG.m_dwKey = pRecvMsg->m_dwKey;
		nakMSG.m_byErrorCode = RC::RC_GUILD_FAILED_BY_NOT_CONNECT_SERVER;
		pServerSession->SendPacket(&nakMSG, sizeof(nakMSG));
		SUNLOG( eCRITICAL_LOG, "[Handler_FromGameServer_IMPL(GZ_GUILD_CHANGE_COMMANDER_SYN)] - Can't send Packet![GuildGuid:%u]\n", pRecvMsg->m_GuildGuid );
		return;
	}
}

Handler_FromGameServer_IMPL(GZ_GUILD_CHANGE_CORPS_SYN)
{
	MSG_GZ_GUILD_CHANGE_CORPS_SYN* pRecvMsg = (MSG_GZ_GUILD_CHANGE_CORPS_SYN*)pMsg;

	RC::eGUILD_RESULT rt = g_GuildManager.CanChangeCorps( pRecvMsg->m_GuildGuid, pRecvMsg->m_OperCharGuid, pRecvMsg->m_szTargetCharName, pRecvMsg->m_byCorpsNum );
	if( rt != RC::RC_GUILD_SUCCESS )
	{
		MSG_GZ_GUILD_CHANGE_CORPS_NAK nakMSG;
		nakMSG.m_dwKey = pRecvMsg->m_dwKey;
		nakMSG.m_byErrorCode = rt;
		pServerSession->SendPacket(&nakMSG, sizeof(nakMSG));
		return;
	}

	GuildMember* pTargetMember = g_GuildManager.FindGuildMemberByName( pRecvMsg->m_GuildGuid, pRecvMsg->m_szTargetCharName );
	if( !pTargetMember )		return;

	MSG_DZ_GUILD_CHANGE_CORPS_SYN synMSG;
	synMSG.m_dwKey				= pRecvMsg->m_dwKey;
	synMSG.m_dwSessionIndex 	= pServerSession->GetSessionIndex();
	synMSG.m_GuildGuid			= pRecvMsg->m_GuildGuid;
	synMSG.m_bySourceCorpsNum	= pTargetMember->GetCorpsNumber();
	synMSG.m_byTargetCorpsNum	= pRecvMsg->m_byCorpsNum;
	synMSG.m_CharGuid			= pTargetMember->GetCharGuid();
	if( !GuildServer::GetMainFrame()->ConnectSessions.SendToDBProxy( &synMSG, sizeof(synMSG) ) )
	{
		MSG_GZ_GUILD_CHANGE_COMMANDER_NAK nakMSG;
		nakMSG.m_dwKey = pRecvMsg->m_dwKey;
		nakMSG.m_byErrorCode = RC::RC_GUILD_FAILED_BY_NOT_CONNECT_SERVER;
		pServerSession->SendPacket(&nakMSG, sizeof(nakMSG));
		SUNLOG( eCRITICAL_LOG, "[Handler_FromGameServer_IMPL(GZ_GUILD_CHANGE_COMMANDER_SYN)] - Can't send Packet![GuildGuid:%u]\n", pRecvMsg->m_GuildGuid );
		return;
	}
}

Handler_FromGameServer_IMPL(GZ_GUILD_CREATE_REAR_CAMP_SYN)
{
	MSG_GZ_GUILD_CREATE_REAR_CAMP_SYN* pRecvMsg = (MSG_GZ_GUILD_CREATE_REAR_CAMP_SYN*)pMsg;

	BYTE byCorpsNum = 0;
	RC::eGUILD_RESULT rt = g_GuildManager.CanCreateRearCamp( pRecvMsg->m_GuildGuid, pRecvMsg->m_OperCharGuid, pRecvMsg->m_szTargetCharName, byCorpsNum );
	if( rt != RC::RC_GUILD_SUCCESS )
	{
		MSG_GZ_GUILD_CREATE_REAR_CAMP_NAK nakMSG;
		nakMSG.m_dwKey = pRecvMsg->m_dwKey;
		nakMSG.m_byErrorCode = rt;
		pServerSession->SendPacket(&nakMSG, sizeof(nakMSG));
		return;
	}

	GuildMember* pTargetMember = g_GuildManager.FindGuildMemberByName( pRecvMsg->m_GuildGuid, pRecvMsg->m_szTargetCharName );
	if( !pTargetMember )		return;

	MSG_DZ_GUILD_CREATE_REAR_CAMP_SYN synMSG;
	synMSG.m_dwKey			= pRecvMsg->m_dwKey;
	synMSG.m_dwSessionIndex = pServerSession->GetSessionIndex();
	synMSG.m_GuildGuid		= pRecvMsg->m_GuildGuid;
	synMSG.m_TargetCharGuid = pTargetMember->GetCharGuid();
	synMSG.m_byCorpsNum		= byCorpsNum;
	if( !GuildServer::GetMainFrame()->ConnectSessions.SendToDBProxy( &synMSG, sizeof(synMSG) ) )
	{
		MSG_GZ_GUILD_CREATE_REAR_CAMP_NAK nakMSG;
		nakMSG.m_dwKey = pRecvMsg->m_dwKey;
		nakMSG.m_byErrorCode = RC::RC_GUILD_FAILED_BY_NOT_CONNECT_SERVER;
		pServerSession->SendPacket(&nakMSG, sizeof(nakMSG));
		SUNLOG( eCRITICAL_LOG, "[Handler_FromGameServer_IMPL(GZ_GUILD_CREATE_REAR_CAMP_SYN)] - Can't send Packet![GuildGuid:%u]\n", pRecvMsg->m_GuildGuid );
		return;
	}
}

Handler_FromGameServer_IMPL(GZ_GUILD_DESTROY_REAR_CAMP_SYN)
{
	MSG_GZ_GUILD_DESTROY_REAR_CAMP_SYN* pRecvMsg = (MSG_GZ_GUILD_DESTROY_REAR_CAMP_SYN	*)pMsg;

	RC::eGUILD_RESULT rt = g_GuildManager.CanDestroyRearCamp( pRecvMsg->m_GuildGuid, pRecvMsg->m_OperCharGuid, pRecvMsg->m_byCorpsNum );
	if( rt != RC::RC_GUILD_SUCCESS )
	{
		MSG_GZ_GUILD_DESTROY_REAR_CAMP_NAK nakMSG;
		nakMSG.m_dwKey = pRecvMsg->m_dwKey;
		nakMSG.m_byErrorCode = rt;
		pServerSession->SendPacket(&nakMSG, sizeof(nakMSG));
		SUNLOG( eFULL_LOG, "[Handler_FromGameServer_IMPL(GZ_GUILD_DESTROY_REAR_CAMP_SYN)] ERROR, GuildGuid = %d, OperGuid = %u, CorpsNum = %d", pRecvMsg->m_GuildGuid,  pRecvMsg->m_OperCharGuid, pRecvMsg->m_byCorpsNum );
		return;
	}

	MSG_DZ_GUILD_DESTROY_REAR_CAMP_SYN synMsg;
	synMsg.m_dwKey			= pRecvMsg->m_dwKey;
	synMsg.m_dwSessionIndex = pServerSession->GetSessionIndex();
	synMsg.m_GuildGuid		= pRecvMsg->m_GuildGuid;
	synMsg.m_byCorpsNum		= pRecvMsg->m_byCorpsNum;
	synMsg.m_OperCharGuid   = pRecvMsg->m_OperCharGuid;

	if( !GuildServer::GetMainFrame()->ConnectSessions.SendToDBProxy( &synMsg, sizeof(synMsg) ) )
	{
		MSG_GZ_GUILD_DESTROY_REAR_CAMP_NAK nakMSG;
		nakMSG.m_dwKey = pRecvMsg->m_dwKey;
		nakMSG.m_byErrorCode = RC::RC_GUILD_FAILED_BY_NOT_CONNECT_SERVER;
		pServerSession->SendPacket(&nakMSG, sizeof(nakMSG));
		SUNLOG( eCRITICAL_LOG, "[Handler_FromGameServer_IMPL(GZ_GUILD_DESTROY_REAR_CAMP_SYN)] ERROR - Can't send Packet![GuildGuid:%u]\n", pRecvMsg->m_GuildGuid );
		return;
	}
}

//��� �ʴ�.
Handler_FromGameServer_IMPL(GZ_GUILD_INVITE_SYN)
{
	MSG_GZ_GUILD_INVITE_SYN* pRecvMsg = (MSG_GZ_GUILD_INVITE_SYN*)pMsg;

	//Invite�� Join�̳� ������ �Ȱ���.
	RC::eGUILD_RESULT rt = g_GuildManager.CanInviteGuild( pRecvMsg->m_GuildGuid, pRecvMsg->m_OperCharGuid, pRecvMsg->m_TargetCharGuid, pRecvMsg->m_Level );
	if( rt != RC::RC_GUILD_SUCCESS )
	{
		MSG_GZ_GUILD_INVITE_NAK nakMSG;
		nakMSG.m_dwKey = pRecvMsg->m_dwKey;
		nakMSG.m_byErrorCode = rt;
		pServerSession->SendPacket(&nakMSG, sizeof(nakMSG));
		SUNLOG( eFULL_LOG, "[Handler_FromGameServer_IMPL(GZ_GUILD_INVITE_SYN)] ERROR, GuildGuid = %d, OperGuid = %u, TargetGuid = %u, rt = %d", 
			pRecvMsg->m_GuildGuid,  pRecvMsg->m_OperCharGuid, pRecvMsg->m_TargetCharGuid, rt );
		return;
	}

	MSG_GZ_GUILD_INVITE_ACK	ackMsg;
	ackMsg.m_dwKey = pRecvMsg->m_dwKey;
	ackMsg.m_TargetUserKey	= pRecvMsg->m_TargetUserKey;
	pServerSession->SendPacket(&ackMsg, sizeof(ackMsg));
}

//��� ��� �α���
Handler_FromGameServer_IMPL(GZ_GUILD_LOGIN_CHAR_SYN)
{
	MSG_GZ_GUILD_LOGIN_CHAR_SYN* pRecvMsg = (MSG_GZ_GUILD_LOGIN_CHAR_SYN*)pMsg;

	//Invite�� Join�̳� ������ �Ȱ���.
	RC::eGUILD_RESULT rt = g_GuildManager.CanLogin( pRecvMsg->m_GuildGuid, pRecvMsg->m_CharGuid );

	if( rt == RC::RC_GUILD_NOT_EXIST_GUILD )
	{
		// ���� ��弭���� ��尡 ���� ���� ������.. DB�� ��� ������ ��û�Ѵ�.
		MSG_DZ_GUILD_SELECT_SYN synMsg;
		synMsg.m_dwKey = pRecvMsg->m_dwKey;
        synMsg.m_dwSessionIndex = 0; // (WAVERIX) (090903) (CHANGES) ...-.-;...
		synMsg.m_GuildGuid = pRecvMsg->m_GuildGuid;
		synMsg.m_bySelectType = 1;		//1�� �α��ν� SELECT�� ���Ѵ�.
		synMsg.m_LoginCharGuid	= pRecvMsg->m_CharGuid;
		if( !GuildServer::GetMainFrame()->ConnectSessions.SendToDBProxy( &synMsg, sizeof(synMsg) ) )
		{
			MSG_GZ_GUILD_LOGIN_CHAR_NAK nakMsg;
			nakMsg.m_byErrorCode = RC::RC_GUILD_FAILED_BY_NOT_CONNECT_SERVER;
			pServerSession->SendPacket( &nakMsg, sizeof(nakMsg) );
			return;
		}
        // CHANGES: f110627.2L, fixed the invalid acknowledgement answer
        // that this status is pended.
        return;
	}
	else if( rt == RC::RC_GUILD_SUCCESS )
	{
		GuildServer* pGuildServer = GuildServer::GetMainFrame();

		// ��� ������ �����ϰ�, ����� ��� ����̴�. ���ϰ� üũ ���� �ʰ� �ٷ� �α��� ��Ų��.
		g_GuildManager.Login( pRecvMsg->m_GuildGuid, pRecvMsg->m_CharGuid );

		MSG_GZ_GUILD_LOGIN_CHAR_BRD brdMsg;
		brdMsg.m_dwKey = pRecvMsg->m_dwKey;
		g_GuildManager.SerializeGuildInfo( pRecvMsg->m_GuildGuid, brdMsg.m_GuildInfo, SERIALIZE_LOAD );
		g_GuildManager.SerializeCorpsInfo( pRecvMsg->m_GuildGuid, brdMsg.m_CorpsTotalInfo, SERIALIZE_LOAD );
		g_GuildManager.GetGuildMemberInfo( pRecvMsg->m_GuildGuid, pRecvMsg->m_CharGuid, brdMsg.m_MemberInfo );
		pGuildServer->SendToAllGameServers( &brdMsg, sizeof(brdMsg) );

		MSG_WZ_GUILD_LOGIN_CHAR_CMD	cmdMsg;
		g_GuildManager.GetWorldGuildInfo( pRecvMsg->m_GuildGuid, cmdMsg.m_WorldGuildInfo );
		g_GuildManager.GetGuildBaseMemberInfo( pRecvMsg->m_GuildGuid, pRecvMsg->m_CharGuid, cmdMsg.m_BaseMemberInfo );
		pGuildServer->ConnectSessions.SendToWorld( &cmdMsg, sizeof(cmdMsg) );
		return;
	}
	else if( rt == RC::RC_GUILD_ALREADY_LOGIN_MEMBER )
		return;

	//������ ���̽��� ���� ���̽���.
	MSG_GZ_GUILD_LOGIN_CHAR_NAK nakMsg;
    nakMsg.m_dwKey = pRecvMsg->m_dwKey; // (WAVERIX) (090903) (BUG-FIX) ...-.-+...
	nakMsg.m_byErrorCode = rt;
	pServerSession->SendPacket( &nakMsg, sizeof(nakMsg) );
}

//��� ��� �α׾ƿ�
Handler_FromGameServer_IMPL(GZ_GUILD_LOGOUT_CHAR_SYN)
{
	MSG_GZ_GUILD_LOGOUT_CHAR_SYN* pRecvMsg = (MSG_GZ_GUILD_LOGOUT_CHAR_SYN*)pMsg;
	GuildServer* pGuildServer = GuildServer::GetMainFrame();
	if( !pGuildServer )		return;

	//1. ��弭���� �ش� ������ ������ �α׾ƿ����� �����Ѵ�.
	RC::eGUILD_RESULT rc = g_GuildManager.Logout( pRecvMsg->m_GuildGuid, pRecvMsg->m_CharGuid );

	// ��弭���� ĳ���� �������� �߰� �Ǹ�, ��弭������ ���� ĳ���Ϳ� ���� ��� ������ ����.( ���� ������ �������� �ʴ� �̻�... )
	// ���� ���Ӽ��������� ���GUID�� �־ �α׾ƿ� ��Ŷ�� ������, ��� ���������� �ش� ��尡 ���ٰ� �� ���̴�.
	if( rc == RC::RC_GUILD_NOT_EXIST_GUILD )
		return;

	if( rc != RC::RC_GUILD_SUCCESS )
	{
		SUNLOG( eCRITICAL_LOG, "Handler_FromGameServer_IMPL(GZ_GUILD_LOGOUT_CHAR_SYN) Fail, GuildGuid = %u, CharGuid = %u, rc = %d",
			pRecvMsg->m_GuildGuid, pRecvMsg->m_CharGuid, rc );
		return;
	}

	//2. ���Ӽ����� ������.
	MSG_GZ_GUILD_LOGOUT_CHAR_BRD	brdMsg;
	brdMsg.m_GuildGuid = pRecvMsg->m_GuildGuid;
	brdMsg.m_CharGuid = pRecvMsg->m_CharGuid;
	g_GuildManager.GetCharName( pRecvMsg->m_GuildGuid, pRecvMsg->m_CharGuid, brdMsg.m_szCharName, MAX_CHARNAME_LENGTH );
	pGuildServer->SendToAllGameServers( &brdMsg, sizeof(brdMsg) );

	//3. ���弭���� ������.
	MSG_WZ_GUILD_LOGOUT_CHAR_CMD	cmdMsg;
	cmdMsg.m_GuildGuid = pRecvMsg->m_GuildGuid;
	cmdMsg.m_CharGuid = pRecvMsg->m_CharGuid;
	pGuildServer->ConnectSessions.SendToWorld( &cmdMsg, sizeof(cmdMsg) );
}


//��� ��� ������
Handler_FromGameServer_IMPL(GZ_GUILD_CHAR_LEVELUP_SYN)
{
	MSG_GZ_GUILD_CHAR_LEVELUP_SYN* pRecvMsg = (MSG_GZ_GUILD_CHAR_LEVELUP_SYN*)pMsg;

	RC::eGUILD_RESULT rt = g_GuildManager.LevelUpMember( pRecvMsg->m_GuildGuid, pRecvMsg->m_CharGuid, pRecvMsg->m_Level );

	// ����� �������� ���� ��� Ż���̸�..
	if( rt == RC::RC_GUILD_KICK_GUILD_BY_LEVELUP )
	{
		RC::eGUILD_RESULT rt = g_GuildManager.CanLeaveGuild( pRecvMsg->m_GuildGuid, pRecvMsg->m_CharGuid );
		if( rt != RC::RC_GUILD_SUCCESS )
		{
			MSG_GZ_GUILD_WITHDRAW_NAK msgNAK;
			msgNAK.m_dwKey			= pRecvMsg->m_dwKey;
			msgNAK.m_byErrorCode	= rt;
			pServerSession->SendPacket( &msgNAK, sizeof(msgNAK) );
			return;
		}

		MSG_DZ_GUILD_WITHDRAW_SYN msgSYN;
		msgSYN.m_dwKey			= pRecvMsg->m_dwKey;
		msgSYN.m_dwSessionIndex	= pServerSession->GetSessionIndex();
		msgSYN.m_GuildGuid		= pRecvMsg->m_GuildGuid;
		msgSYN.m_CharGuid		= pRecvMsg->m_CharGuid;
		msgSYN.m_byPenaltyType	= 3; // �������� ���� Ż��.
		if( !GuildServer::GetMainFrame()->ConnectSessions.SendToDBProxy( &msgSYN, sizeof(msgSYN) ) )
		{
			MSG_GZ_GUILD_WITHDRAW_NAK msgNAK;
			msgNAK.m_dwKey			= pRecvMsg->m_dwKey;
			msgNAK.m_byErrorCode	= RC::RC_GUILD_FAILED_BY_NOT_CONNECT_SERVER;
			pServerSession->SendPacket( &msgNAK, sizeof(msgNAK) );
		}
		return;
	}

	if( rt != RC::RC_GUILD_SUCCESS )
	{
		SUNLOG( eCRITICAL_LOG, "[Handler_FromGameServer_IMPL(GZ_GUILD_CHAR_LEVELUP_SYN)] Fail, GuildGuid = %u, CharGuid = %u, Level = %u, rt = %d", 
			 pRecvMsg->m_GuildGuid, pRecvMsg->m_CharGuid, pRecvMsg->m_Level, rt );
		return;
	}

	MSG_GZ_GUILD_CHAR_LEVELUP_BRD brdMsg;
	brdMsg.m_GuildGuid = pRecvMsg->m_GuildGuid;
	g_GuildManager.GetCharName( pRecvMsg->m_GuildGuid, pRecvMsg->m_CharGuid, brdMsg.m_szCharName, MAX_CHARNAME_LENGTH  );
	brdMsg.m_Level = pRecvMsg->m_Level;
	pServerSession->SendPacket( &brdMsg, sizeof(brdMsg) );
}

Handler_FromGameServer_IMPL( GZ_GUILD_TOTALINFO_REQ_SYN )
{
	MSG_GZ_GUILD_TOTALINFO_REQ_SYN* pRecvMsg = (MSG_GZ_GUILD_TOTALINFO_REQ_SYN*)pMsg;

	RC::eGUILD_RESULT rt = g_GuildManager.CanRequestTotalInfo( pRecvMsg->m_GuildGuid, pRecvMsg->m_CharGuid );
	if( rt != RC::RC_GUILD_SUCCESS )
	{
		MSG_GZ_GUILD_TOTALINFO_REQ_NAK msgNAK;
		msgNAK.m_dwKey			= pRecvMsg->m_dwKey;
		msgNAK.m_byErrorCode	= rt;
		pServerSession->SendPacket( &msgNAK, sizeof(msgNAK) );
		return;
	}

	MSG_GZ_GUILD_TOTALINFO_REQ_ACK ackMsg;
	ackMsg.m_dwKey = pRecvMsg->m_dwKey;
	g_GuildManager.SerializeGuildInfo( pRecvMsg->m_GuildGuid, ackMsg.m_GuildInfo, SERIALIZE_LOAD );
	g_GuildManager.SerializeCorpsInfo( pRecvMsg->m_GuildGuid, ackMsg.m_CropsTotalInfo, SERIALIZE_LOAD );
	g_GuildManager.SerializeMemberTotalInfo( pRecvMsg->m_GuildGuid, ackMsg.m_MemberTotalInfo, SERIALIZE_LOAD );
	pServerSession->SendPacket( &ackMsg, ackMsg.GetSize() );
}

//GM��ɾ� �������Ʈ ����
Handler_FromGameServer_IMPL(GZ_GUILD_GM_GUILD_POINT_SYN )
{
	MSG_GZ_GUILD_GM_GUILD_POINT_SYN* pRecvMsg = (MSG_GZ_GUILD_GM_GUILD_POINT_SYN*)pMsg;

	Guild* pGuild = g_GuildManager.FindGuild( pRecvMsg->m_GuildGuid );
	if( !pGuild )
	{
		//���� ��� ������ ������ ������, �̻��� �����̰ų� Agent������ �α��� ���� ���� �����̴�.
		MSG_GZ_GUILD_GM_GUILD_POINT_NAK	nakMsg;
		nakMsg.m_dwKey = pRecvMsg->m_dwKey;
		nakMsg.m_byErrorCode = RC::RC_GUILD_NOT_EXIST_GUILD;
		pServerSession->SendPacket( &nakMsg, sizeof(nakMsg) );
		return;
	}

	GuildMember* pMember = pGuild->FindMember( pRecvMsg->m_CharGuid );
	if( !pMember )
	{
		MSG_GZ_GUILD_GM_GUILD_POINT_NAK	nakMsg;   
		nakMsg.m_dwKey = pRecvMsg->m_dwKey;
		nakMsg.m_byErrorCode = RC::RC_GUILD_NOT_GUILD_MEMBER;
		pServerSession->SendPacket( &nakMsg, sizeof(nakMsg) );
		return;
	}

	MSG_DZ_GUILD_GM_GUILD_POINT_SYN	synMsg;
	synMsg.m_dwKey = pRecvMsg->m_dwKey;
	synMsg.m_dwSessionIndex = pServerSession->GetSessionIndex();
	synMsg.m_GuildGuid = pRecvMsg->m_GuildGuid;
	synMsg.m_UP	= pRecvMsg->m_UP;
	synMsg.m_GP = pRecvMsg->m_GP;
	if( !GuildServer::GetMainFrame()->ConnectSessions.SendToDBProxy( &synMsg, sizeof(synMsg) ) )
	{
		MSG_GZ_GUILD_GM_GUILD_POINT_NAK	nakMsg;
		nakMsg.m_dwKey = pRecvMsg->m_dwKey;
		nakMsg.m_byErrorCode = RC::RC_GUILD_FAILED_BY_NOT_CONNECT_SERVER;
		pServerSession->SendPacket( &nakMsg, sizeof(nakMsg) );
		return;
	}

}

Handler_FromGameServer_IMPL(GZ_GUILD_RELATION_INFO_SYN)
{
	MSG_GZ_GUILD_RELATION_INFO_SYN* pRecvMsg = (MSG_GZ_GUILD_RELATION_INFO_SYN*)pMsg;

	MSG_DZ_GUILD_RELATION_INFO_SYN msg;
	msg.m_dwKey = pMsg->m_dwKey;
    msg.m_dwSessionIndex = pServerSession->GetSessionIndex(); // (WAVERIX) (090903) (BUG-FIX) ...-.-;...
	msg.m_GuildGuid =  pRecvMsg->m_GuildGuid;
	if(!GuildServer::GetMainFrame()->ConnectSessions.SendToDBProxy( &msg, sizeof(msg)))
	{
		//MSG_GZ_GUILD_ADD_INFO_NAK nakMsg;
		//nakMsg.m_dwKey			= pRecvMsg->m_dwKey;
		//strncpy( nakMsg.m_szTargetCharName, pRecvMsg->m_tszCharName, MAX_CHARNAME_LENGTH );
		//nakMsg.m_byErrorCode	= RC::RC_GUILD_FAILED_BY_NOT_CONNECT_SERVER;
		//pServerSession->SendPacket( &nakMsg, sizeof(nakMsg) );
		return;
	}

	//MSG_GZ_GUILD_RELATION_INFO_BRD SendToGameServer;
	//SendToGameServer.m_dwKey = pRecvMsg->m_dwKey;
	//SendToGameServer.m_AddInfo.m_GuildGuid = pRecvMsg->m_GuildGuid;
	//LoadRelationInfo(&SendToGameServer.m_AddInfo);
	//SendToGameServer.m_byRelCnt = SendToGameServer.m_AddInfo.m_byRelationGuildCnt;
	//pServerSession->SendPacket( &SendToGameServer, SendToGameServer.GetSize());
}

Handler_FromGameServer_IMPL(GZ_GUILD_JOIN_SYN)
{
	MSG_GZ_GUILD_JOIN_SYN * pRecvMsg = (MSG_GZ_GUILD_JOIN_SYN *)pMsg;

	eGUILD_DUTY Duty = eGUILD_DUTY_NONE;
	BYTE byCorpsNum = 0;
	RC::eGUILD_RESULT rt = g_GuildManager.CanJoinGuild( pRecvMsg->m_GuildGuid, pRecvMsg->m_OperCharGuid, pRecvMsg->m_TargetCharGuid, pRecvMsg->m_Level, Duty, byCorpsNum );
	if( rt != RC::RC_GUILD_SUCCESS )
	{
		MSG_GZ_GUILD_JOIN_NAK nakMsg;
		nakMsg.m_dwKey			= pRecvMsg->m_dwKey;
        strncpy(nakMsg.m_szTargetCharName, pRecvMsg->m_tszCharName,
                _countof(nakMsg.m_szTargetCharName));
        nakMsg.m_szTargetCharName[_countof(nakMsg.m_szTargetCharName) - 1] = '\0';
		strncpy( nakMsg.m_szTargetCharName, pRecvMsg->m_tszCharName, MAX_CHARNAME_LENGTH );
		nakMsg.m_byErrorCode	= rt;
		pServerSession->SendPacket( &nakMsg, sizeof(nakMsg) );
		return;
	}

	MSG_DZ_GUILD_JOIN_SYN msg;
	msg.m_dwKey				= pRecvMsg->m_dwKey;
	msg.m_dwSessionIndex	= pServerSession->GetSessionIndex();   //���Ӽ����� ������ �����Ѵ�.
	msg.m_GuildGuid			= pRecvMsg->m_GuildGuid;
	msg.m_OperCharGuid		= pRecvMsg->m_OperCharGuid;
	msg.m_TargetCharGuid	= pRecvMsg->m_TargetCharGuid;
    strncpy(msg.m_tszCharName, pRecvMsg->m_tszCharName, _countof(msg.m_tszCharName));
    msg.m_tszCharName[_countof(msg.m_tszCharName) - 1] = '\0';
	msg.m_byClass			= pRecvMsg->m_byClass;
	msg.m_Level				= pRecvMsg->m_Level;
	msg.m_Duty				= Duty;		
	msg.m_byCorpsNum		= byCorpsNum;
	if( !GuildServer::GetMainFrame()->ConnectSessions.SendToDBProxy( &msg, sizeof(msg) ) )
	{
		MSG_GZ_GUILD_JOIN_NAK nakMsg;
		nakMsg.m_dwKey			= pRecvMsg->m_dwKey;
        strncpy(nakMsg.m_szTargetCharName, pRecvMsg->m_tszCharName,
                _countof(nakMsg.m_szTargetCharName));
        nakMsg.m_szTargetCharName[_countof(nakMsg.m_szTargetCharName) - 1] = '\0';
		nakMsg.m_byErrorCode	= RC::RC_GUILD_FAILED_BY_NOT_CONNECT_SERVER;
		pServerSession->SendPacket( &nakMsg, sizeof(nakMsg) );
		return;
	}
}



Handler_FromGameServer_IMPL(GZ_GUILD_RELATION_ALLIENCE_SYN)
{
	PACKET_PTR_CONVERT(MSG_GZ_GUILD_RELATION_ALLIENCE_SYN, pRecvMsg, pMsg);

	CHARGUID RightMemberIndex = 0;
	GUILDGUID response_guild_index = 0;
	RC::eGUILD_RESULT result = GuildSmallContentsEntry::RelationSystem::CanDoRelationChange(
        pRecvMsg->m_GuildGuid, pRecvMsg->m_RequestPlayerIndex);
	if(result == RC::RC_GUILD_SUCCESS)
	{
		response_guild_index = g_GuildManager.GetGuildGuidByGuildName(pRecvMsg->m_tszRelationGuildName);
		if(0 != response_guild_index)
		{
			// (lst1024)(090402) ��� ���� ��� ���¸� ���� ��û �� �� ����.
			Guild* pGuild = g_GuildManager.FindGuild(response_guild_index);
			if( !pGuild )
				result = RC::RC_GUILD_NOT_EXIST_GUILD;
			else
			{
				if(eGUILD_DELCHK_DESTROY_WAIT == pGuild->CheckGuildDeleteState())
					result = RC::RC_GUILD_DELCHK_DESTROY_WAIT;
				else
				{
					// ��û ���� ��忡�� ������ ���� ��� ��� ã��
					RightMemberIndex = g_GuildManager.GetGuildMasterByGuildName(pRecvMsg->m_tszRelationGuildName);
					if(0 == RightMemberIndex) //��� �����Ͱ� ���� ���
					{
						result = RC::RC_GUILD_CURRENT_NONE_MASTER;

						// ���� ������ ���� ����
						RightMemberIndex = g_GuildManager.FindRightMember(response_guild_index, eGUILD_DUTY_SUBMASTER);
						if(0 != RightMemberIndex)
						{
							result = GuildSmallContentsEntry::RelationSystem::CanDoRelationChange(
                                response_guild_index, RightMemberIndex);
							if(result !=RC::RC_GUILD_SUCCESS)
							{
								// ���� ���� ����
								RightMemberIndex = g_GuildManager.FindRightMember(response_guild_index, eGUILD_DUTY_STRATEGIST);
								if(0 != RightMemberIndex)
								{
									result = GuildSmallContentsEntry::RelationSystem::CanDoRelationChange(
                                        response_guild_index, RightMemberIndex);
								}
							}
						}
					}
				}
			}
		}
		else
		{
			result = RC::RC_GUILD_CURRENT_NONE_MASTER;
		}
	}

	//. ������ ���� ����� ������~
	if(RC::RC_GUILD_SUCCESS == result)
	{
		result = GuildSmallContentsEntry::RelationSystem::CanDoRelationShip(
            pRecvMsg->m_GuildGuid, response_guild_index, eGUILD_RELATION_ALLIENCE);
		if(RC::RC_GUILD_SUCCESS == result)
		{
			MSG_GZ_GUILD_RELATION_ALLIENCE_ACK msg;
			msg.m_dwKey = pRecvMsg->m_dwKey;
			msg.m_eRelationType = pRecvMsg->m_eRelationType;
			msg.m_RequestGuildGuid = pRecvMsg->m_GuildGuid;
			msg.m_ResponseGuildGuid = response_guild_index;
			msg.m_ResponsePlayerIndex = RightMemberIndex;
			msg.m_RequestPlayerIndex = pRecvMsg->m_RequestPlayerIndex;
			pServerSession->SendPacket(&msg, sizeof(msg));
			return;
		}
	}


	MSG_GZ_GUILD_RELATION_NAK msg;
	msg.m_dwKey = pRecvMsg->m_dwKey;
	msg.m_byErrorCode = result;
	pServerSession->SendPacket(&msg, sizeof(msg));
	return;
}

Handler_FromGameServer_IMPL(GZ_GUILD_RELATION_SYN)
{
	PACKET_PTR_CONVERT(MSG_GZ_GUILD_RELATION_SYN, pRecvMsg, pMsg);

	//��û���� ���� ����.
	// (lst1024)(090408) ��� ���� üũ�� �ɸ���GUID ���
 	if( !g_GuildManager.CheckRight( pRecvMsg->m_RequestGuildGuid, pRecvMsg->m_RequestCharGuid, eGUILD_RIGHTS_RELATION ) )
 	{
		MSG_GZ_GUILD_RELATION_NAK msg;
		msg.m_dwKey = pRecvMsg->m_dwKey;
		msg.m_byErrorCode = RC::RC_GUILD_HAVE_NOT_RIGHT;;
		pServerSession->SendPacket(&msg, sizeof(msg));
		return;
	}

	Guild*  pResponseGuild = NULL;
	if(pRecvMsg->m_ResponseGuildGuid)
	{
		pResponseGuild = g_GuildManager.FindGuild(pRecvMsg->m_ResponseGuildGuid);
	}
	else
	{
		pResponseGuild = g_GuildManager.FindGuildName(pRecvMsg->m_tszRelationGuildName);
	}

	RC::eGUILD_RESULT result = RC::RC_GUILD_SUCCESS;

	if(pResponseGuild)
	{
		GUILDGUID request_guild_index = pRecvMsg->m_RequestGuildGuid;
		GUILDGUID response_guild_index = pResponseGuild->GetGuildGuid();

		result = GuildSmallContentsEntry::RelationSystem::CanDoRelationShip(
            request_guild_index, response_guild_index, pRecvMsg->m_eRelationType);
		if(RC::RC_GUILD_SUCCESS == result)
		{
			MSG_DZ_GUILD_RELATION_SYN msg;
			msg.m_dwKey = pRecvMsg->m_dwKey; /* AnswerPlayer ���� Ű */
			msg.m_eRelationType = pRecvMsg->m_eRelationType;
			msg.m_RequestGuildGuid	= pRecvMsg->m_RequestGuildGuid;
			msg.m_RequestUserKey = pRecvMsg->m_RequestUserKey; /* RequestPlayer ���� Ű */
			msg.m_ResponseGuildGuid	= pRecvMsg->m_ResponseGuildGuid;
			msg.m_dwSessionIndex = pServerSession->GetSessionIndex();
			util::GUILDNAMECOPY(msg.m_ResponseGuildName, pRecvMsg->m_tszRelationGuildName);
			if(GuildServer::GetMainFrame()->ConnectSessions.SendToDBProxy(&msg, sizeof(msg))) 
				return;
			
			result = RC::RC_GUILD_DISCONNECTED_GUILD_SERVER;
		}
	}

	if(!pResponseGuild && eGUILD_RELATION_ALLIENCE != pRecvMsg->m_eRelationType)
	{
		MSG_DZ_GUILD_RELATION_SYN msg;
		msg.m_dwKey = pRecvMsg->m_dwKey; /* AnswerPlayer ���� Ű */
		msg.m_eRelationType = pRecvMsg->m_eRelationType;
		msg.m_RequestGuildGuid	= pRecvMsg->m_RequestGuildGuid;
		msg.m_RequestUserKey = pRecvMsg->m_RequestUserKey; /* RequestPlayer ���� Ű */
		msg.m_ResponseGuildGuid	= pRecvMsg->m_ResponseGuildGuid;
		msg.m_dwSessionIndex = pServerSession->GetSessionIndex();
		util::GUILDNAMECOPY(msg.m_ResponseGuildName, pRecvMsg->m_tszRelationGuildName);
		if(GuildServer::GetMainFrame()->ConnectSessions.SendToDBProxy(&msg, sizeof(msg))) 
			return;
		
		result = RC::RC_GUILD_DISCONNECTED_GUILD_SERVER;
	}

	MSG_GZ_GUILD_RELATION_NAK msg;
	msg.m_dwKey = pRecvMsg->m_dwKey;
	msg.m_byErrorCode = result;
	pServerSession->SendPacket(&msg, sizeof(msg));
}

Handler_FromGameServer_IMPL(GZ_GUILD_RELATION_BOTH_AGAINST_SYN)
{
	PACKET_PTR_CONVERT(MSG_GZ_GUILD_RELATION_BOTH_AGAINST_SYN, pRecvMsg, pMsg);
	GuildServer*  pGuildServer = GuildServer::GetMainFrame();
	if(!pGuildServer) return;

	GUILDGUID requestGuildGuid = pRecvMsg->m_GuildGuid;
	GUILDGUID responseGuildGuid = pRecvMsg->m_RelGuildGuid;
	CHARGUID requestPlayerGuid = pRecvMsg->m_MasterCharGuid;

	RC::eGUILD_RESULT result = GuildSmallContentsEntry::RelationSystem::CanDoRelationChange(
        requestGuildGuid, requestPlayerGuid);
	if(result != RC::RC_GUILD_SUCCESS)
	{
		MSG_GZ_GUILD_RELATION_BOTH_AGAINST_NAK msg;
		msg.m_dwKey = pRecvMsg->m_dwKey;
		msg.m_byErrorCode = result;
		pServerSession->SendPacket(&msg, sizeof(msg));
		return;
	}

	result = GuildSmallContentsEntry::RelationSystem::CanDoRelationBothHostility(
        requestGuildGuid, requestPlayerGuid);

	MSG_DZ_GUILD_RELATION_BOTH_AGAINST_SYN msg;
	msg.m_dwKey = pRecvMsg->m_dwKey;
	msg.m_GuildGuid = requestGuildGuid;
	msg.m_RelGuildGuid = responseGuildGuid;
	msg.m_MasterCharGuid = requestPlayerGuid;
	msg.m_dwSessionIndex = pServerSession->GetSessionIndex();

	if(!GuildServer::GetMainFrame()->ConnectSessions.SendToDBProxy(&msg, sizeof(msg)))
	{
		MSG_GZ_GUILD_RELATION_BOTH_AGAINST_NAK msg;
		msg.m_dwKey = pRecvMsg->m_dwKey;
		msg.m_byErrorCode = RC::RC_GUILD_DISCONNECTED_GUILD_SERVER;
		pServerSession->SendPacket(&msg, sizeof(msg));
		return;
	}

}

Handler_FromGameServer_IMPL(GZ_GUILD_RELATION_DELETE_SYN)
{
	PACKET_PTR_CONVERT(MSG_GZ_GUILD_RELATION_DELETE_SYN, pRecvMsg, pMsg);
	GuildServer*  pGuildServer = GuildServer::GetMainFrame();
	if(!pGuildServer) return;

	eGUILD_RELATION eType = pRecvMsg->m_eType;
	GUILDGUID requestGuildGuid = pRecvMsg->m_GuildGuid;
	GUILDGUID responseGuildGuid = pRecvMsg->m_RelGuildGuid;
	CHARGUID requestPlayerGuid = pRecvMsg->m_RequestPlayerIndex;

	RC::eGUILD_RESULT result = GuildSmallContentsEntry::RelationSystem::CanDoRelationChange(
        requestGuildGuid, requestPlayerGuid);
	if(result != RC::RC_GUILD_SUCCESS)
	{
		MSG_GZ_GUILD_RELATION_DELETE_NAK msg;
		msg.m_dwKey = pRecvMsg->m_dwKey;
		msg.m_byErrorCode = result;
		pServerSession->SendPacket(&msg, sizeof(msg));
		return;
	}

	result = GuildSmallContentsEntry::RelationSystem::CanDoRelationDelete(
        requestGuildGuid, responseGuildGuid, pRecvMsg->m_eType);
	if(RC::RC_GUILD_SUCCESS == result)
	{
		MSG_DZ_GUILD_RELATION_DELETE_SYN msg;
		msg.m_dwKey = pRecvMsg->m_dwKey;
		msg.m_eType = eType;
		msg.m_GuildGuid = requestGuildGuid;
		msg.m_RelGuildGuid	 = responseGuildGuid;
		msg.m_dwSessionIndex = pServerSession->GetSessionIndex();
		if(pGuildServer->ConnectSessions.SendToDBProxy(&msg, sizeof(msg)))
			return;
		
		result = RC::RC_GUILD_FAILED_BY_NOT_CONNECT_SERVER;
	}
	
	MSG_GZ_GUILD_RELATION_DELETE_NAK msg;
	msg.m_dwKey = pRecvMsg->m_dwKey;
	msg.m_byErrorCode = result;
	pServerSession->SendPacket(&msg, sizeof(msg));
}


Handler_FromGameServer_IMPL(GZ_GUILD_RECOVERY_GUILD_SYN)
{
	MSG_GZ_GUILD_RECOVERY_GUILD_SYN* pRecvMsg = 
		static_cast<MSG_GZ_GUILD_RECOVERY_GUILD_SYN*>(pMsg);

	RC::eGUILD_RESULT result =
        GuildSmallContentsEntry::GuildPenalty::CanRecoveryGuild(pRecvMsg->m_GuildGuid);
	if(RC::RC_GUILD_SUCCESS == result)
	{
		MSG_DZ_GUILD_RECOVERY_GUILD_SYN msg;
		msg.m_dwKey		= pRecvMsg->m_dwKey;
        msg.m_dwSessionIndex = pServerSession->GetSessionIndex(); // (WAVERIX) (090903) (CHANGES) ...-.-;...
		msg.m_CharGuid	= pRecvMsg->m_CharGuid;
		msg.m_GuildGuid	= pRecvMsg->m_GuildGuid;

		if(GuildServer::GetMainFrame()->ConnectSessions.SendToDBProxy(&msg, sizeof(msg)))
			return;

		result = RC::RC_GUILD_RECOVERY_FAILED;
	}

	MSG_GZ_GUILD_RECOVERY_GUILD_NAK msg;
	msg.m_dwKey	= pRecvMsg->m_dwKey;
	msg.m_CharGuid	 = pRecvMsg->m_CharGuid;
	msg.m_byErrorCode = result;
	pServerSession->SendPacket(&msg, sizeof(msg));
}


Handler_FromGameServer_IMPL(GZ_GUILD_GUILDMARK_REGISTER_SYN)
{
	MSG_GZ_GUILD_GUILDMARK_REGISTER_SYN* pRecvMsg = 
		static_cast<MSG_GZ_GUILD_GUILDMARK_REGISTER_SYN*>(pMsg);
	
	GuildSmallContentsEntry::GuildMark::GuildMarkInfo tempInfo;
	tempInfo.guild_Index = pRecvMsg->m_GuildIndex;
	tempInfo.pattern_index = pRecvMsg->m_PatternIndex;
	tempInfo.background_index = pRecvMsg->m_BackGroundIndex;
	tempInfo.request_player_index = pRecvMsg->m_RequestPlayerIndex;
	
	RC::eGUILD_RESULT result = GuildSmallContentsEntry::GuildMark::CanModify(tempInfo);
	if(RC::RC_GUILD_SUCCESS != result) 
	{
		DWORD userKey  = pRecvMsg->m_dwKey;
		GuildSmallContentsEntry::GuildMark::ResultCastToGameServer(pServerSession, result, userKey);
		return;
	}

	MSG_DZ_GUILD_GUILDMARK_REGISTER_SYN msg;
	msg.m_dwKey = pRecvMsg->m_dwKey;
    msg.m_dwSessionIndex = pServerSession->GetSessionIndex(); // (WAVERIX) (090903) (BUG-FIX) ...-.-;...
	msg.m_GuildIndex = tempInfo.guild_Index;
	msg.m_PatternIndex = tempInfo.pattern_index;
	msg.m_BackGroundIndex = tempInfo.background_index;
	msg.m_RequestPlayerIndex = tempInfo.request_player_index;
	msg.m_SlotIdx = pRecvMsg->m_SlotIdx;
	msg.m_SlotPos = pRecvMsg->m_SlotPos;
	
	if(!GuildServer::GetMainFrame()->ConnectSessions.SendToDBProxy(&msg, sizeof(msg)))
		MessageOut(eDEV_LOG, "RegisterCastToDBProxy Error");

}

Handler_FromGameServer_IMPL(GZ_GUILD_GUILDMARK_DELETE_SYN)
{
	MSG_GZ_GUILD_GUILDMARK_DELETE_SYN* pRecvMsg = 
		static_cast<MSG_GZ_GUILD_GUILDMARK_DELETE_SYN*>(pMsg);

	GuildSmallContentsEntry::GuildMark::GuildMarkInfo tempInfo;
	tempInfo.guild_Index = pRecvMsg->m_GuildIndex;
	tempInfo.pattern_index = pRecvMsg->m_PatternIndex;
	tempInfo.background_index = pRecvMsg->m_BackGroundIndex;
	tempInfo.request_player_index = pRecvMsg->m_RequestPlayerIndex;
	
	RC::eGUILD_RESULT result = GuildSmallContentsEntry::GuildMark::CanModify(tempInfo);
	if(RC::RC_GUILD_SUCCESS != result)
	{
		GuildSmallContentsEntry::GuildMark::ResultCastToGameServer(
            pServerSession, result, pRecvMsg->m_dwKey);
		return;
	}

	MSG_DZ_GUILD_GUILDMARK_DELETE_SYN msg;
	msg.m_dwKey = pRecvMsg->m_dwKey;
    msg.m_dwSessionIndex = pServerSession->GetSessionIndex(); // (WAVERIX) (090903) (BUG-FIX) ...-.-;...
	msg.m_GuildIndex = tempInfo.guild_Index;
	msg.m_PatternIndex = tempInfo.pattern_index;
	msg.m_BackGroundIndex = tempInfo.background_index;
	msg.m_RequestPlayerIndex = tempInfo.request_player_index;

	if(!GuildServer::GetMainFrame()->ConnectSessions.SendToDBProxy(&msg, sizeof(msg)))
		MessageOut(eDEV_LOG, "RegisterCastToDBProxy Error");
}

Handler_FromGameServer_IMPL(GZ_GUILD_WAREHOUSE_OPEN_SYN)
{
    PACKET_PTR_CONVERT(MSG_GZ_GUILD_WAREHOUSE_OPEN_SYN,pRecvMsg, pMsg);
    GuildServer* pGuildServer = GuildServer::GetMainFrame();
    if(!pGuildServer || !g_GuildManager.CheckRight(pRecvMsg->m_GuildGuid,
                                                   pRecvMsg->m_OperCharGuid,
                                                   eGUILD_RIGHTS_WAREHOUSE_SAVE))
    {
        MSG_GZ_GUILD_WAREHOUSE_OPEN_NAK msg;
        msg.m_dwKey = pRecvMsg->m_dwKey;
        msg.m_bErrorCode = RC::RC_GUILD_WAREHOUSE_UNABLE_CURR_GRADE;
        pServerSession->SendPacket(&msg, sizeof(msg));
        return;
    }

    MSG_DZ_GUILD_WAREHOUSE_OPEN_SYN msg;
    msg.m_dwKey = pRecvMsg->m_dwKey;
    msg.m_dwSessionIndex = pServerSession->GetSessionIndex(); // (WAVERIX) (090903) (CHANGES) ...-.-;...
    msg.m_GuildGuid = pRecvMsg->m_GuildGuid;
    msg.m_OperCharGuid = pRecvMsg->m_OperCharGuid;
    if(!pGuildServer->ConnectSessions.SendToDBProxy(&msg, sizeof(msg)))
        SUNLOG(eDEV_LOG, "RegisterCastToDBProxy Error(%d)", pRecvMsg->m_GuildGuid);
}

Handler_FromGameServer_IMPL(GZ_GUILD_WAREHOUSE_CLOSE_SYN)
{
	MSG_GZ_GUILD_WAREHOUSE_CLOSE_SYN* pRecvMsg = 
		static_cast<MSG_GZ_GUILD_WAREHOUSE_CLOSE_SYN*>(pMsg);
    // (WAVERIX) (090903) (NOTE) �ƹ����� ���� �׳� ���⿡�� ������ ���� �� ����.
    // ������ �������� �ڵ�� �������� �ʴ´�.
}


Handler_FromGameServer_IMPL(GZ_GUILD_NAME_CHANGE_SYN)
{
	MSG_GZ_GUILD_NAME_CHANGE_SYN* pRecvMsg = (MSG_GZ_GUILD_NAME_CHANGE_SYN*)pMsg;
	GuildServer* pGuildServer = GuildServer::GetMainFrame();

	RC::eGUILD_RESULT result = g_GuildManager.CanChangeGuildName( pRecvMsg->m_GuildGuid, pRecvMsg->m_OperCharGuid );
	if(RC::RC_GUILD_SUCCESS != result)
	{
		MSG_GZ_GUILD_NAME_CHANGE_NAK msg;
		msg.m_dwKey = pRecvMsg->m_dwKey;
		msg.m_TargetPos = pRecvMsg->m_TargetPos;
		msg.m_byErrorCode = result;
		pServerSession->SendPacket(&msg, sizeof(msg));
		return;
	}

	//.//

	MSG_DZ_GUILD_NAME_CHANGE_SYN msg;
	msg.m_dwKey			 = pRecvMsg->m_dwKey;
	msg.m_TargetPos		 = pRecvMsg->m_TargetPos;
	msg.m_GuildGuid		 = pRecvMsg->m_GuildGuid;
	msg.m_OperCharGuid	 = pRecvMsg->m_OperCharGuid;
	msg.m_dwSessionIndex = pServerSession->GetSessionIndex();
	memcpy( msg.m_tszGuildName, pRecvMsg->m_tszGuildName, MAX_GUILDNAME_LENGTH*sizeof(TCHAR) );
	msg.m_tszGuildName[MAX_GUILDNAME_LENGTH] = '\0';

	if(!pGuildServer->ConnectSessions.SendToDBProxy(&msg, sizeof(msg)))
	{
		MSG_GZ_GUILD_NAME_CHANGE_NAK msg;
		msg.m_dwKey = pRecvMsg->m_dwKey;
		msg.m_TargetPos = pRecvMsg->m_TargetPos;
		msg.m_byErrorCode = RC::RC_GUILD_DISCONNECTED_DBP_SERVER;
		pServerSession->SendPacket(&msg, sizeof(msg));

		SUNLOG(eDEV_LOG, "RegisterCastToDBProxy Error(%d)", pRecvMsg->m_GuildGuid);
	}
}

//���� ������ �⺻ ���� ���� ���� �۾�
Handler_FromGameServer_IMPL(GZ_DOMINATION_INFO_SYN)
{
    const MSG_GZ_DOMINATION_INFO_SYN* recv_msg = 
        static_cast<MSG_GZ_DOMINATION_INFO_SYN*>(pMsg);

    //�⺻ ���� �ε尡 ������ �����̸� ���� ������ ������ �ش�
    if (DominationManager::Instance()-> GetLoadDominationInfo() == true)
    {
        DominationManager::Instance()->SendDominationInfo(pServerSession, recv_msg->m_MapCode);
    }
}

Handler_FromGameServer_IMPL(GZ_DOMINATION_AUCTION_INFO_SYN)
{
    const MSG_GZ_DOMINATION_AUCTION_INFO_SYN* pRecvMsg = 
        static_cast<MSG_GZ_DOMINATION_AUCTION_INFO_SYN*>(pMsg);

    //��� ���� �ε尡 ������ �����̸� ���� ������ ������ �ش�
    if (DominationManager::Instance()->GetLoadDominationAuctionInfo() == true)
    {
        DominationManager::Instance()->SendActionList(pServerSession, pRecvMsg->m_MapCode);
    }
}

//DB�� �⺻ ���� Update��û�� �Ѵ�.
Handler_FromGameServer_IMPL(GZ_DOMINATION_UPDATE_INFO_SYN)
{
    const MSG_GZ_DOMINATION_UPDATE_INFO_SYN* pRecvMsg = 
        static_cast<MSG_GZ_DOMINATION_UPDATE_INFO_SYN*>(pMsg);

    MSG_DZ_DOMINATION_UPDATE_INFO_SYN sendMsg;
    sendMsg.m_eSetType = pRecvMsg->m_eSetType;
    sendMsg.m_DominationInfo = pRecvMsg->m_DominationInfo;


    switch(pRecvMsg->m_eSetType)
    {
    case DOMINATION_GM_OFFENCE_GUILD_SET:
    case DOMINATION_GM_DEFENCE_GUILD_SET:
        {
            DOMINATION_BASIC_GUILD_INFO* guild_info = NULL;
            if (pRecvMsg->m_eSetType == DOMINATION_GM_OFFENCE_GUILD_SET)
                guild_info = &sendMsg.m_DominationInfo.m_disGuildInfo;
            else
                guild_info = &sendMsg.m_DominationInfo.m_inGuildInfo;

            Guild* pGuild = g_GuildManager.FindGuildName(guild_info->m_tszGuildName);
            if (!pGuild)
                return;

            guild_info->m_GuildGuid = pGuild->GetGuildGuid();
		    pGuild->AcquireGuildMark(guild_info->m_BackGroundIndex, guild_info->m_PatternIndex);

            GuildMember* master = pGuild->FindMaster();
            guild_info->m_GuildMasterGuid = master->GetCharGuid();
            _tcsncpy(guild_info->m_tszGuildMaster, master->GetCharName(), MAX_CHARNAME_LENGTH);
            guild_info->m_tszGuildMaster[MAX_CHARNAME_LENGTH] = '\0';

            sendMsg.m_eSetType = DOMINATION_INIT;
        }
        break;
    }

    GuildServer* pGuildServer = GuildServer::GetMainFrame();
    if( pGuildServer == NULL )
    {
        SUNLOG( eCRITICAL_LOG, "["__FUNCTION__"] - Not Fount pGuildServer\n" );
        return;
    }
    pGuildServer->ConnectSessions.SendToDBProxy(&sendMsg, sizeof(sendMsg));
}

Handler_FromGameServer_IMPL(GZ_DOMINATION_ACCEP_HEIM_SYN)
{
    const MSG_GZ_DOMINATION_ACCEP_HEIM_SYN* recv_msg = 
        static_cast<MSG_GZ_DOMINATION_ACCEP_HEIM_SYN*>(pMsg);

    GuildServer* guild_server = GuildServer::GetMainFrame();
    if (guild_server == NULL)
    {
        SUNLOG( eCRITICAL_LOG, "["__FUNCTION__"]Not Fount guild_server" );
        return;
    }

    MSG_DZ_DOMINATION_ACCEP_HEIM_SYN send_msg;

#ifdef _CH_20100420_CHANGE_DOMINATION_APPLY_WAY
    send_msg.m_TokenPos = recv_msg->m_TokenPos;
    send_msg.number_of_betting_item = recv_msg->number_of_betting_item;
#endif//_CH_20100420_CHANGE_DOMINATION_APPLY_WAY
    send_msg.m_dwKey = recv_msg->m_dwKey;
    send_msg.m_DominationActionInfo = recv_msg->m_DominationActionInfo;

    guild_server->ConnectSessions.SendToDBProxy(&send_msg, sizeof(send_msg));
}

Handler_FromGameServer_IMPL(GZ_DOMINATION_INIT_ACTIONINFO_SYN)
{
    const MSG_GZ_DOMINATION_INIT_ACTIONINFO_SYN* pRecvMsg = 
        static_cast<MSG_GZ_DOMINATION_INIT_ACTIONINFO_SYN*>(pMsg);

    GuildServer* pGuildServer = GuildServer::GetMainFrame();
    if( pGuildServer == NULL )
    {
        SUNLOG( eCRITICAL_LOG, "["__FUNCTION__"]Not Fount pGuildServer\n" );
        return;
    }

    MSG_DZ_DOMINATION_INIT_AUCTIONINFO_SYN sendMsg;
    sendMsg.m_MapCode = pRecvMsg->m_MapCode;
    pGuildServer->ConnectSessions.SendToDBProxy(&sendMsg, sizeof(sendMsg));
}

//������ ������ ��� �������� Guid����Ʈ�� �ۼ��Ͽ� WorldServer�� ������.
Handler_FromGameServer_IMPL(GZ_DOMINATION_RETURNHIME_SYN)
{
    const MSG_GZ_DOMINATION_RETURNHIME_SYN* pRecvMsg = 
        static_cast<MSG_GZ_DOMINATION_RETURNHIME_SYN*>(pMsg);

    GuildServer* pGuildServer = GuildServer::GetMainFrame();
    if( pGuildServer == NULL )
    {
        SUNLOG( eCRITICAL_LOG, "["__FUNCTION__"] - Not Fount pGuildServer\n" );
        return;
    }

    DOMINATION_RETURNMONEY_INFO ReturnInfo[MAX_DOMINATION_AUCTION];
    ZeroMemory( ReturnInfo, sizeof(ReturnInfo) );

    const DOMINATION_INFO& dominationInfo = pRecvMsg->m_DominationInfo;

    INT countData = 0;

    //��ſ� ��÷ ���� ���� ��� ����Ʈ �ۼ�
    DominationManager::Instance()->SetListForReturnHeim( dominationInfo.m_FieldCode, 
        ReturnInfo, MAX_DOMINATION_AUCTION, countData );

#ifndef _CH_20100420_CHANGE_DOMINATION_APPLY_WAY
    //�������� ���� ���� ��� �������̸��� �ݾ��� �ִ´�.(���� ������ ��� ���� ����)
    {
        ReturnInfo[countData].m_TenderCost = dominationInfo.m_BestCost;
        ReturnInfo[countData].m_CostType = SYSMEMO_DOMINATION_REWARD_MONEY;
        memcpy( ReturnInfo[countData].m_tszMaster, 
            dominationInfo.m_inGuildInfo.m_tszGuildMaster, sizeof(TCHAR)*MAX_CHARNAME_LENGTH );
        ReturnInfo[countData].m_tszMaster[MAX_CHARNAME_LENGTH] = '\0';

        ++countData;
    }
#endif//_CH_20100420_CHANGE_DOMINATION_APPLY_WAY
    
    //WorldServer�� ���� ������ �������� ��û
    MSG_WZ_GUILD_DOMINATION_RETURNHIME_CMD sendMsg;
    sendMsg.m_MapCode = dominationInfo.m_FieldCode;
    memcpy( sendMsg.m_tszGuild, dominationInfo.m_disGuildInfo.m_tszGuildName, sizeof(TCHAR)*MAX_GUILDNAME_LENGTH );
    sendMsg.m_tszGuild[MAX_GUILDNAME_LENGTH] = '\0';
    memcpy( sendMsg.m_ReturnInfo, ReturnInfo, sizeof(sendMsg.m_ReturnInfo) );
    sendMsg.m_DataCount = countData;

    pGuildServer->ConnectSessions.SendToWorld(&sendMsg, sendMsg.GetSize());

    //GameServer�� �Ϸ� ������ �Ѵ�.
    MSG_GZ_DOMINATION_RETURNHIME_ACK ackMsg;
    ackMsg.m_MapCode = dominationInfo.m_FieldCode;

    pServerSession->SendPacket(&ackMsg, sizeof(ackMsg));
}

Handler_FromGameServer_IMPL(GZ_DOMINATION_BATTING_HEIM_SYN)
{
    const MSG_GZ_DOMINATION_BATTING_HEIM_SYN* pRecvMsg = 
        static_cast<MSG_GZ_DOMINATION_BATTING_HEIM_SYN*>(pMsg);

    MSG_GZ_DOMINATION_BATTING_HEIM_ACK sendMsg;

    Guild* pGuild = g_GuildManager.FindGuild( pRecvMsg->m_guildGuid );
    if( pGuild == NULL )
    {
        sendMsg.m_ResultCode = RC::DOMINATION_RESULT_NOT_GUILDMEMBER;
    }
    else if( pGuild->CheckGuildDeleteState() == eGUILD_DELCHK_DESTROY_WAIT )
    {
        sendMsg.m_ResultCode = RC::DOMINATION_RESULT_GUILDSTAT_DESTROYWAIT;
    }
    else
    {
        //���� ���� ��� ��û�� �ص� �Ǵ� ����̴�.(�ػ� ��û�ǿ� �־)
        sendMsg.m_ResultCode = RC::DOMINATION_RESULT_SUCCESS;
    }
    sendMsg.m_dwKey = pRecvMsg->m_dwKey;
#ifdef _CH_20100420_CHANGE_DOMINATION_APPLY_WAY
    sendMsg.m_mapCode = pRecvMsg->m_mapCode;
#endif

    pServerSession->SendPacket( &sendMsg, sizeof(sendMsg) );
}

#ifdef _NA_20100604_BETTER_DOMINATION_MANAGER
Handler_FromGameServer_IMPL(GZ_DOMINATION_UPDATE_GUILDINFO_SYN)
{
    const MSG_GZ_DOMINATION_UPDATE_GUILDINFO_SYN* recv_msg = 
        static_cast<MSG_GZ_DOMINATION_UPDATE_GUILDINFO_SYN*>(pMsg);

    GuildServer* guild_server = GuildServer::GetMainFrame();
    if (guild_server == NULL)
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"]Not Fount pGuildServer\n");
        return;
    }

    MSG_DZ_DOMINATION_UPDATE_GUILDINFO_SYN send_msg;
    {
        send_msg.guild_guid_ = recv_msg->guild_guid_;
        send_msg.domination_join_count_ = recv_msg->domination_join_count_;
        send_msg.domination_defense_count_ = recv_msg->domination_defense_count_;
        send_msg.domination_hunting_count_ = recv_msg->domination_hunting_count_;
    }
    guild_server->ConnectSessions.SendToDBProxy(&send_msg, sizeof(send_msg));

}
#endif//_NA_20100604_BETTER_DOMINATION_MANAGER

//_NA_20100913_GUILD_WAREHOUSE_GM_COMMAND
// GM ��ɿ� ���� ��� â�� ���� �ݱ�
Handler_FromGameServer_IMPL(GZ_GUILD_GM_WAREHOUSE_FORCE_CLOSE_CMD)
{
    const MSG_GZ_GUILD_GM_WAREHOUSE_FORCE_CLOSE_CMD* msg = 
        static_cast<MSG_GZ_GUILD_GM_WAREHOUSE_FORCE_CLOSE_CMD*>(pMsg);

    Guild* guild = g_GuildManager.FindGuildName(msg->guild_name);
    if (guild == NULL)
    {
        SUNLOG(eFULL_LOG, "[GZ_GUILD_GM_WAREHOUSE_FORCE_CLOSE_CMD] Guild name '%s' not found!", msg->guild_name);
        return;
    }
    
    // DBProxy�� ���� ������� �����Ѵ�.
    // �� �� ������ ������ �����ϱ⸦ ����ϹǷ� Ÿ�̹� ����� ���� ��� ���� �ٷ� ��������� �Ѹ���.
    
    // DBProxy�� ���� �ݱ� ����� �����ϰ�
    MSG_DZ_GUILD_GM_WAREHOUSE_FORCE_CLOSE_CMD cmd;
    cmd.gm_player_key = msg->gm_player_key;
    cmd.guild_guid = guild->GetGuildGuid();
    GuildServer::GetMainFrame()->ConnectSessions.SendToDBProxy(&cmd, sizeof(cmd));

    // ���� �����鿡 �����Ѵ�.
    MSG_GZ_GUILD_GM_WAREHOUSE_FORCE_CLOSE_BRD brd;
    brd.gm_player_key = msg->gm_player_key;
    brd.guild_guid = guild->GetGuildGuid();
    GuildServer::GetMainFrame()->SendToAllGameServers(&brd, sizeof(brd));
}

//gamelulu_dev_not : ������ ���� ���� �⺻ ������ Game <-> Guild <-> DBP �̴�.
//������ ��κ��� Game->DBP->Guild->Game �� �����ν� �ذ��� �����ϴ�.
#ifdef _NA_20110216_INCREASE_DOMINATION_DEFFENSEWALL_POINT
Handler_FromGameServer_IMPL(GZ_DOMINATION_ADD_DEFFENSEWALL_POINT_SYN)
{
    const MSG_GZ_DOMINATION_ADD_DEFFENSEWALL_POINT_SYN* recv_msg = 
        static_cast<MSG_GZ_DOMINATION_ADD_DEFFENSEWALL_POINT_SYN*>(pMsg);

    MSG_DZ_DOMINATION_ADD_DEFFENSEWALL_POINT_SYN send_msg;
    send_msg.item_code_ = recv_msg->item_code_;
    send_msg.m_dwKey = recv_msg->m_dwKey;
    send_msg.domination_info_ = recv_msg->domination_info_;

    GuildServer* guild_server = GuildServer::GetMainFrame();
    if (guild_server == NULL)
    {
        SUNLOG(eCRITICAL_LOG,"["__FUNCTION__"]Not connected to guild server");
        return;
    }
    guild_server->ConnectSessions.SendToDBProxy(&send_msg, sizeof(send_msg));
}
#endif//_NA_20110216_INCREASE_DOMINATION_DEFFENSEWALL_POINT

#ifdef _NA_20110406_CHANGE_RULE_FOR_DOMINATION_JOIN
Handler_FromGameServer_IMPL(GZ_DOMINATION_MEMBER_JOIN_SYN)
{
    const MSG_GZ_DOMINATION_MEMBER_JOIN_SYN* recv_msg = 
        static_cast<MSG_GZ_DOMINATION_MEMBER_JOIN_SYN*>(pMsg);

    MSG_DZ_DOMINATION_MEMBER_JOIN_SYN send_msg;
    send_msg.map_code_ = recv_msg->map_code_;
    send_msg.member_info_ = recv_msg->member_info_;

    GuildServer* guild_server = GuildServer::GetMainFrame();
    if (guild_server == NULL)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__":Not connected to guild server");
        return;
    }
    guild_server->ConnectSessions.SendToDBProxy(&send_msg, sizeof(send_msg));    
}

Handler_FromGameServer_IMPL(GZ_DOMINATION_MEMBER_LEAVE_SYN)
{
    const MSG_GZ_DOMINATION_MEMBER_LEAVE_SYN* recv_msg = 
        static_cast<MSG_GZ_DOMINATION_MEMBER_LEAVE_SYN*>(pMsg);

    MSG_DZ_DOMINATION_MEMBER_LEAVE_SYN send_msg;
    send_msg.map_code_ = recv_msg->map_code_;
    send_msg.member_guid_ = recv_msg->member_guid_;
    send_msg.requester_guid = recv_msg->requester_guid;

    GuildServer* guild_server = GuildServer::GetMainFrame();
    if (guild_server == NULL)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__":Not connected to guild server");
        return;
    }
    guild_server->ConnectSessions.SendToDBProxy(&send_msg, sizeof(send_msg));    
}

Handler_FromGameServer_IMPL(GZ_DOMINATION_MEMBERLIST_REQ)
{
    const MSG_GZ_DOMINATION_MEMBERLIST_REQ* recv_msg = 
        static_cast<MSG_GZ_DOMINATION_MEMBERLIST_REQ*>(pMsg);

    if (DominationManager::Instance()->GetLoadDominationMemberInfo() == true)
    {
        DominationManager::Instance()->SendMemberList(pServerSession, recv_msg->map_code_);
    }
}

Handler_FromGameServer_IMPL(GZ_DOMINATION_MEMBERLIST_INIT_CMD)
{
    const MSG_GZ_DOMINATION_MEMBERLIST_INIT_CMD* recv_msg = 
        static_cast<MSG_GZ_DOMINATION_MEMBERLIST_INIT_CMD*>(pMsg);

    MSG_DZ_DOMINATION_MEMBERLIST_INIT_SYN send_msg;

    GuildServer* guild_server = GuildServer::GetMainFrame();
    if (guild_server == NULL)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__":Not connected to guild server");
        return;
    }
    guild_server->ConnectSessions.SendToDBProxy(&send_msg, sizeof(send_msg));    
}
#endif//_NA_20110406_CHANGE_RULE_FOR_DOMINATION_JOIN




