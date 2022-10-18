#include "stdafx.h"
#include "./Handler_Guild.GameToGuild.h"

#include <PacketStruct_DZ.h>
#include <PacketStruct_GZ.h>
#include <PacketStruct_WZ.h>

#include <GuildRankInfoParser.h>
#include "Services/DominationManager.h"

// GUILD
#include "Services/Guild/GuildManager.h"
#include "Services/Guild/Guild.h"
#include "Services/Guild/GuildMember.h"

#include "Services/Guild/GuildRelation.h"
#include "Services/Guild/GuildSmallContentEntry.h"
//
#include "QueryObjects/DBHandler.h"
#include "PacketHandler/Handler_Guild.GuildToDbp.h"
//
#include <PacketStruct_PointSystem.h>
#include <Services/GuildPointSystem/GuildPointSystem.h>

#ifdef _NA_003923_20120130_GUILD_RENEWAL
#include "GuildInGameLog.h"
#endif

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
#include "GuildFacilities.h"
#include "GuildFacilityInfoParser.h"
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION

//==================================================================================================
// CHANGES: f110518.2L
const nsPacket::PacketHandlerInfo Handler_Guild_GameToGuild::GetPacketHandlerNodes()
{
#define HANDLER_NODE_INFO(c, p, phase) { MAKEWORD(c, p), false, phase, Handler_Guild_GameToGuild::On##p }
#define HANDLER_NODE_INFO_DBR(c, p, phase) { MAKEWORD(c, p), true, phase, Handler_Guild_GameToGuild::On##p }
#define HANDLER_NODE_INFO_EOR() { 0, false, static_cast<ePHASE_SHIFT>(0), NULL }

    static const nsPacket::PacketHandlerNode handlers_static_[] =
    {
        //
        //HANDLER_NODE_INFO(AM_OPERATION, AM_OPERATION_USERCOUNT_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_CREATE_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_DESTROY_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_JOIN_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_WITHDRAW_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_KICKPLAYER_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_DONATE_UP_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_CONVERT_UP2GP_SYN, PHASE_UNLIMITED),

#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_CHUNTING_PVP_RESULT_CMD, PHASE_UNLIMITED),
#endif
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_CHANGE_DUTY_SYN, PHASE_UNLIMITED),				//길드 직책 설정	
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_RIGHT_DELEGATION_LIST_SYN, PHASE_UNLIMITED),		//길드 위임 권한 목록 요청	
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_RIGHT_DELEGATION_SYN, PHASE_UNLIMITED),			//길드 권한 위임
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_CHANGE_MASTER_SYN, PHASE_UNLIMITED),				//길드장변경

        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_CREATE_CORPS_SYN, PHASE_UNLIMITED),					//길드 군단 생성
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_DESTROY_CORPS_SYN, PHASE_UNLIMITED),					//길드 군단 해산
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_EMPTY_COMMANDER_INFO_SYN, PHASE_UNLIMITED),			//길드 빈 군단장 정보 요청.
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_CHANGE_COMMANDER_SYN, PHASE_UNLIMITED),				//길드 군단장 변경.
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_CHANGE_CORPS_SYN, PHASE_UNLIMITED),					//길드 군단 변경.
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_CREATE_REAR_CAMP_SYN, PHASE_UNLIMITED),				//길드 육성캠프 생성.
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_DESTROY_REAR_CAMP_SYN, PHASE_UNLIMITED),				//길드 육성캠프 해산.
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_INVITE_SYN, PHASE_UNLIMITED),							//길드 초대.
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_LOGIN_CHAR_SYN, PHASE_UNLIMITED),						//길드 멤버 로그인.
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_LOGOUT_CHAR_SYN, PHASE_UNLIMITED),					//길드 멤버 로그아웃.
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_CHAR_LEVELUP_SYN, PHASE_UNLIMITED),					//길드원 레벨업.
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_TOTALINFO_REQ_SYN, PHASE_UNLIMITED),					//길드원 정보요청.
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_GM_GUILD_POINT_SYN, PHASE_UNLIMITED),					//길드원 포인트 변경 요청.
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_CHANGE_CHAOSTATE_SYN, PHASE_UNLIMITED),				//길드원 카오상태 변경.
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_ENTER_SERVER_SYN, PHASE_UNLIMITED),					//길드원이 게임서버간을 이동할 때.
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_PRE_LOGIN_CMD, PHASE_UNLIMITED),						//길드원이 로그인 사전 준비.

        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_RELATION_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_RELATION_DELETE_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_RELATION_BOTH_AGAINST_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_BOTH_RELATION_SYN, PHASE_UNLIMITED),

        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_RECOVERY_GUILD_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_GUILDMARK_REGISTER_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_GUILDMARK_DELETE_SYN, PHASE_UNLIMITED),

        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_WAREHOUSE_OPEN_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_WAREHOUSE_CLOSE_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_RELATION_INFO_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_NAME_CHANGE_SYN, PHASE_UNLIMITED),
        //{ __KR_001385_20090907_DOMINATION_PACKET
        HANDLER_NODE_INFO(GZ_GUILD, GZ_DOMINATION_INFO_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO(GZ_GUILD, GZ_DOMINATION_AUCTION_INFO_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO(GZ_GUILD, GZ_DOMINATION_UPDATE_INFO_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO(GZ_GUILD, GZ_DOMINATION_ACCEP_HEIM_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO(GZ_GUILD, GZ_DOMINATION_INIT_ACTIONINFO_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO(GZ_GUILD, GZ_DOMINATION_RETURNHIME_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO(GZ_GUILD, GZ_DOMINATION_BATTING_HEIM_SYN, PHASE_UNLIMITED),
        //}
        //_NA_20100604_BETTER_DOMINATION_MANAGER
        HANDLER_NODE_INFO(GZ_GUILD, GZ_DOMINATION_UPDATE_GUILDINFO_SYN, PHASE_UNLIMITED),

        //_NA_20100913_GUILD_WAREHOUSE_GM_COMMAND
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_GM_WAREHOUSE_FORCE_CLOSE_CMD, PHASE_UNLIMITED),         // GM 명령에 의한 길드 창고 강제 닫기
        //_NA_20110216_INCREASE_DOMINATION_DEFFENSEWALL_POINT
        HANDLER_NODE_INFO(GZ_GUILD, GZ_DOMINATION_ADD_DEFFENSEWALL_POINT_SYN, PHASE_UNLIMITED),

        //_NA_20110406_CHANGE_RULE_FOR_DOMINATION_JOIN
        HANDLER_NODE_INFO(GZ_GUILD, GZ_DOMINATION_MEMBER_JOIN_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO(GZ_GUILD, GZ_DOMINATION_MEMBER_LEAVE_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO(GZ_GUILD, GZ_DOMINATION_MEMBERLIST_REQ, PHASE_UNLIMITED),
        HANDLER_NODE_INFO(GZ_GUILD, GZ_DOMINATION_MEMBERLIST_INIT_CMD, PHASE_UNLIMITED),
#ifdef _NA004034_20120102_GUILD_POINT_SYSTEM
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILDPOINTSYSTEM, PHASE_UNLIMITED),
#endif //_NA004034_20120102_GUILD_POINT_SYSTEM

#ifdef _NA_003923_20120130_GUILD_RENEWAL
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_IN_GAME_GUILD_LOG_LIST_REQ, PHASE_UNLIMITED),
#endif
#ifdef _NA_004034_20120102_GUILD_INTRODUCTION
                HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_INTRODUCTION_SAVE_SYN, PHASE_UNLIMITED),
#endif //_NA_004034_20120102_GUILD_INTRODUCTION
#ifdef _NA_000000_20130429_DOMINATION_PENALTY
        HANDLER_NODE_INFO(GZ_GUILD, GZ_DOMINATION_PENALTY_CMD, PHASE_UNLIMITED),
#endif //_NA_000000_20130429_DOMINATION_PENALTY

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_JOIN_REQUEST_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_JOIN_REQUEST_REJECT_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_RELOAD_SYN, PHASE_UNLIMITED),

        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_FACILITY_UPGRADE_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_FACILITY_SELECT_SYN, PHASE_UNLIMITED),
#endif //_NA_007330_20140620_GUILD_SYSTEM_EXTENSION
#ifdef _NA_008334_20150608_SONNENSCHEIN
        HANDLER_NODE_INFO(GZ_GUILD, GZ_GUILD_SONNENSCHEIN_UPDATE_SYN, PHASE_UNLIMITED),
#endif //_NA_008334_20150608_SONNENSCHEIN
        HANDLER_NODE_INFO_EOR() // end of records
    };
    BOOST_STATIC_ASSERT(_countof(handlers_static_) >= 1);
    nsPacket::PacketHandlerInfo return_value = { handlers_static_, _countof(handlers_static_) - 1 };
    //
    return return_value;
};

//==================================================================================================

BOOST_STATIC_ASSERT(RC::RC_GUILD_RESULT_COUNTS <= UCHAR_MAX);

//==================================================================================================

Handler_Guild_GameToGuild_IMPL(GZ_GUILD_CREATE_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
	MSG_GZ_GUILD_CREATE_SYN * pRecvMsg = (MSG_GZ_GUILD_CREATE_SYN *)pMsg;

	RC::eGUILD_RESULT rt = g_GuildManager.CanCreateGuild(pRecvMsg->m_MasterLV);

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
        Handler_Guild_GuildToDbp::OnDZ_GUILD_CREATE_SYN(
            g_DBHandler.GetSharedSession(), &msg, sizeof(msg));
	}
	else
	{
		MSG_GZ_GUILD_CREATE_NAK msg;
		msg.m_dwKey			= pRecvMsg->m_dwKey;
		msg.m_byErrorCode = rt;
		pServerSession->SendPacket( &msg, sizeof(msg) );
	}


}

Handler_Guild_GameToGuild_IMPL(GZ_GUILD_DESTROY_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
	const MSG_GZ_GUILD_DESTROY_SYN* receive_msg = static_cast<MSG_GZ_GUILD_DESTROY_SYN*>(pMsg);
    
#ifdef _USING_GUILD_DESTROY_LOGGING
    SUNLOG(eCRITICAL_LOG, 
        _T("|길드해산신청|Handler_Guild_GameToGuild_IMPL(GZ_GUILD_DESTROY_SYN)|MSG_GZ_GUILD_DESTROY_SYN 받음|GuildGuid:%d, MasterCharGuid:%d|"), 
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
            _T("|길드해산신청|Handler_Guild_GameToGuild_IMPL(GZ_GUILD_DESTROY_SYN)|MSG_DZ_GUILD_DESTROY_SYN 보냄|Key:%u, SessionIndex:%u, MasterCharGuid:%u, GuildGuid:%d, DestroyWaitTime:%u|"), 
            msg.m_dwKey, 
            msg.m_dwSessionIndex, 
            msg.m_MasterCharGuid, 
            msg.m_GuildGuid, 
            msg.m_DestroyWaitTime);
#endif // _USING_GUILD_DESTROY_LOGGING

        Handler_Guild_GuildToDbp::OnDZ_GUILD_DESTROY_SYN(
            g_DBHandler.GetSharedSession(), &msg, sizeof(msg));
	}
	else
	{
		MSG_GZ_GUILD_DESTROY_NAK nak_msg;
		nak_msg.m_dwKey	= receive_msg->m_dwKey;
		nak_msg.m_byErrorCode = result;

#ifdef _USING_GUILD_DESTROY_LOGGING
        SUNLOG(eCRITICAL_LOG, 
            _T("|길드해산신청|Handler_Guild_GameToGuild_IMPL(GZ_GUILD_DESTROY_SYN)|MSG_GZ_GUILD_DESTROY_NAK 보냄|Key:%u, ErrorCode:%d|"), 
            nak_msg.m_dwKey, 
            nak_msg.m_byErrorCode);
#endif // _USING_GUILD_DESTROY_LOGGING

		pServerSession->SendPacket(&nak_msg, sizeof(nak_msg));
	}
}

//탈퇴.
Handler_Guild_GameToGuild_IMPL(GZ_GUILD_WITHDRAW_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
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
	msgSYN.m_byPenaltyType	= 1; // 자기자신 탈퇴
    Handler_Guild_GuildToDbp::OnDZ_GUILD_WITHDRAW_SYN(
        g_DBHandler.GetSharedSession(), &msgSYN, sizeof(msgSYN));
	//{
	//	MSG_GZ_GUILD_WITHDRAW_NAK msgNAK;
	//	msgNAK.m_dwKey			= pRecvMsg->m_dwKey;
	//	msgNAK.m_byErrorCode	= RC::RC_GUILD_FAILED_BY_NOT_CONNECT_SERVER;
	//	pServerSession->SendPacket( &msgNAK, sizeof(msgNAK) );
	//	return;
	//}
}

// 길드원 강퇴
Handler_Guild_GameToGuild_IMPL(GZ_GUILD_KICKPLAYER_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
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
	if( !pTargetMember ) return;		//왜 그냥 리턴하냐면... 위에서 검증되었기 때문이다. 반드시 존재한다. 

	MSG_DZ_GUILD_KICKPLAYER_SYN msg;
	msg.m_dwKey = pRecvMsg->m_dwKey;
	msg.m_GuildGuid	= pRecvMsg->m_GuildGuid;
    msg.m_OperCharGuid = pOperMember->GetCharGuid();
	msg.m_TargetCharGuid = pTargetMember->GetCharGuid();
	msg.m_byCorpsNum = pOperMember->GetCorpsNumber();
	msg.m_dwSessionIndex = pServerSession->GetSessionIndex();
    Handler_Guild_GuildToDbp::OnDZ_GUILD_KICKPLAYER_SYN(
        g_DBHandler.GetSharedSession(), &msg, sizeof(msg));
	//{
	//	MSG_GZ_GUILD_KICKPLAYER_NAK nakMsg;
	//	nakMsg.m_dwKey			= pRecvMsg->m_dwKey;
	//	_tcsncpy( nakMsg.m_ptszCharName, pRecvMsg->m_ptszCharName, MAX_CHARNAME_LENGTH );
	//	nakMsg.m_ptszCharName[MAX_CHARNAME_LENGTH] = '\0';
	//	nakMsg.m_dwErrorCode	= RC::RC_GUILD_FAILED_BY_NOT_CONNECT_SERVER;
	//	pServerSession->SendPacket( &nakMsg, sizeof(nakMsg) );
	//	return;
	//}
}


////////////////////
// 길드 포인트 관련

// UP 적립
Handler_Guild_GameToGuild_IMPL(GZ_GUILD_DONATE_UP_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
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
	sendMsg.m_dwSessionIndex = pServerSession->GetSessionIndex();	// 게임서버의 세션을 저장한다.
	sendMsg.m_GuildGuid = pRecvMsg->m_GuildGuid;
	sendMsg.m_CharGuid = pRecvMsg->m_CharGuid;
	sendMsg.m_GuildUP = pRecvMsg->m_DonateUP;
    Handler_Guild_GuildToDbp::OnDZ_GUILD_DONATE_UP_SYN(
        g_DBHandler.GetSharedSession(), &sendMsg, sizeof(sendMsg));
	//{
	//	SUNLOG( eCRITICAL_LOG, "DZ_GUILD_DONATE_UP Error - Can't send Packet![GuildGuid:%u]\n", pRecvMsg->m_GuildGuid );
	//	MSG_DZ_GUILD_DONATE_UP_NAK nakMsg;
	//	nakMsg.m_GuildGuid = pRecvMsg->m_GuildGuid;
	//	nakMsg.m_byErrorCode = RC::RC_GUILD_FAILED;
	//	pServerSession->SendPacket( &nakMsg, sizeof(nakMsg) );
	//	return;
	//}
}

// UP->GP 변환
Handler_Guild_GameToGuild_IMPL(GZ_GUILD_CONVERT_UP2GP_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
	MSG_GZ_GUILD_CONVERT_UP2GP_SYN* pRecvMsg = (MSG_GZ_GUILD_CONVERT_UP2GP_SYN*)pMsg;

	RC::eGUILD_RESULT rt = g_GuildManager.CanConvertUP2GP( pRecvMsg->m_GuildGuid, pRecvMsg->m_OperCharGuid, pRecvMsg->m_ConvertGP );
	if( rt != RC::RC_GUILD_SUCCESS )
	{
		// 기본적인 권한 검사는 GameServer에서 했다고 판단하므로 따로 검사하지 않고 바로 DBProxy로 보낸다
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
    Handler_Guild_GuildToDbp::OnDZ_GUILD_CONVERT_UP2GP_SYN(
        g_DBHandler.GetSharedSession(), &sendMsg, sizeof(sendMsg));
	//{
	//	MSG_GZ_GUILD_CONVERT_UP2GP_NAK nakMsg;
	//	nakMsg.m_dwKey = pRecvMsg->m_dwKey;
	//	nakMsg.m_GuildGuid = pRecvMsg->m_GuildGuid;
	//	nakMsg.m_byErrorCode = RC::RC_GUILD_FAILED_BY_NOT_CONNECT_SERVER;
	//	pServerSession->SendPacket( &nakMsg, sizeof(nakMsg) );
	//	return;
	//}
}



// 길드원 카오상태 변경
Handler_Guild_GameToGuild_IMPL(GZ_GUILD_CHANGE_CHAOSTATE_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
	MSG_GZ_GUILD_CHANGE_CHAOSTATE_SYN* pRecvMsg = (MSG_GZ_GUILD_CHANGE_CHAOSTATE_SYN*)pMsg;

	RC::eGUILD_RESULT rc = g_GuildManager.ChangeChaoState( pRecvMsg->m_GuildGuid, pRecvMsg->m_CharGuid, pRecvMsg->m_byChaoState );
	if( rc != RC::RC_GUILD_SUCCESS )
	{
		SUNLOG( eCRITICAL_LOG, "[Handler_Guild_GameToGuild_IMPL(GZ_GUILD_CHANGE_CHAOSTATE_SYN)] Fail, GuildGuid = %u, CharGuid = %d, ChaoState = %d",
								pRecvMsg->m_GuildGuid, pRecvMsg->m_CharGuid, pRecvMsg->m_byChaoState );
		return;
	}

	MSG_GZ_GUILD_CHANGE_CHAOSTATE_BRD brdMsg;
	brdMsg.m_GuildGuid = pRecvMsg->m_GuildGuid;
	g_GuildManager.GetCharName( pRecvMsg->m_GuildGuid, pRecvMsg->m_CharGuid, brdMsg.m_szCharName, MAX_CHARNAME_LENGTH );
	brdMsg.m_byChaoState = pRecvMsg->m_byChaoState;
    g_DBProxyServer.SendToSessionType(GAME_SERVER, &brdMsg, sizeof(brdMsg));
}

Handler_Guild_GameToGuild_IMPL(GZ_GUILD_ENTER_SERVER_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
	MSG_GZ_GUILD_ENTER_SERVER_SYN* pRecvMsg = (MSG_GZ_GUILD_ENTER_SERVER_SYN*)pMsg;

	//Invite나 Join이나 조건은 똑같다.
	RC::eGUILD_RESULT rt = g_GuildManager.CanMoveServer( pRecvMsg->m_GuildGuid, pRecvMsg->m_CharGuid );
	if( rt != RC::RC_GUILD_SUCCESS )
	{
		MSG_GZ_GUILD_ENTER_SERVER_NAK nakMsg;
		nakMsg.m_byErrorCode = rt;
		pServerSession->SendPacket( &nakMsg, sizeof(nakMsg) );
		return;
	}

	// 해당 게임서버에 길드 정보를 보낸다.
	MSG_GZ_GUILD_ENTER_SERVER_ACK ackMsg;
	ackMsg.m_dwKey = pRecvMsg->m_dwKey;
	g_GuildManager.SerializeGameGuildInfo( pRecvMsg->m_GuildGuid, ackMsg.m_GameGuildInfo, SERIALIZE_LOAD );
	g_GuildManager.GetGuildBaseMemberInfo( pRecvMsg->m_GuildGuid, pRecvMsg->m_CharGuid, ackMsg.m_BaseMemberInfo );
	pServerSession->SendPacket( &ackMsg, sizeof(ackMsg) );
}

Handler_Guild_GameToGuild_IMPL(GZ_GUILD_PRE_LOGIN_CMD)
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
	MSG_GZ_GUILD_PRE_LOGIN_CMD* pRecvMsg = (MSG_GZ_GUILD_PRE_LOGIN_CMD*)pMsg;

	Guild* pGuild = g_GuildManager.FindGuild( pRecvMsg->m_GuildGuid );
	if( !pGuild )	return;

	GuildMember* pMember = pGuild->FindMember( pRecvMsg->m_CharGuid );
	if( !pMember )	return;

	if( pMember->IsLogIn() == TRUE )
	{
		SUNLOG( eCRITICAL_LOG, "[Handler_Guild_GameToGuild_IMPL(GZ_GUILD_PRE_LOGIN_CMD)] Already Loin Char And Init GuildGuid = %u, CharGuid = %u",
			pRecvMsg->m_GuildGuid, pRecvMsg->m_CharGuid );

		pMember->SetLogin( FALSE );
	}
}

#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
Handler_Guild_GameToGuild_IMPL(GZ_GUILD_CHUNTING_PVP_RESULT_CMD)
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
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
            Handler_Guild_GuildToDbp::OnDZ_GUILD_CHUNTING_PVP_RESULT_SYN(
                g_DBHandler.GetSharedSession(), &msg, sizeof(msg));
		}

		if( pRecvMsg->m_DisGuildGuid )	msg.m_GuildGuid = pRecvMsg->m_DisGuildGuid;
		else							msg.m_GuildGuid = pRecvMsg->m_LoseGuildGuid;
		
		Guild *pLoseGuild = g_GuildManager.FindGuild(msg.m_GuildGuid);
		if( pLoseGuild )
		{
			msg.m_PVPInfo = *pLoseGuild->GetChuntingPVPInfo();
            Handler_Guild_GuildToDbp::OnDZ_GUILD_CHUNTING_PVP_RESULT_SYN(
                g_DBHandler.GetSharedSession(), &msg, sizeof(msg));
		}
	}
}
#endif

// 길드 직책 변경
Handler_Guild_GameToGuild_IMPL(GZ_GUILD_CHANGE_DUTY_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
	MSG_GZ_GUILD_CHANGE_DUTY_SYN *pRecvMsg = (MSG_GZ_GUILD_CHANGE_DUTY_SYN *)pMsg;

	GuildMember* pTargerMember = g_GuildManager.FindGuildMemberByName( pRecvMsg->m_GuildGuid, pRecvMsg->m_szTargetCharName );
	if( !pTargerMember )
	{
		//클라이언트한테 그런 타겟멤버 없다고 NAK보낸다.
		MSG_GZ_GUILD_CHANGE_DUTY_NAK nakMSG;
		nakMSG.m_dwKey			= pRecvMsg->m_dwKey;
		nakMSG.m_byErrorCode	= RC::RC_GUILD_NOT_GUILD_MEMBER;
		pServerSession->SendPacket(&nakMSG, sizeof(nakMSG));
		return;
	}

	// 1. 여기서 길드의 권한을 체크한다.
	RC::eGUILD_RESULT rt = g_GuildManager.CanChangeDuty( pRecvMsg->m_GuildGuid, pRecvMsg->m_OperCharGuid, 
		pTargerMember->GetCharGuid(), (eGUILD_DUTY)pRecvMsg->m_byDuty );
	
	//모든 것은 DB에서 성공 ACK를 받고 진행한다. 
	if(rt != RC::RC_GUILD_SUCCESS)
	{
		MSG_GZ_GUILD_CHANGE_DUTY_NAK nakMSG;
		nakMSG.m_dwKey			= pRecvMsg->m_dwKey;
		nakMSG.m_byErrorCode	= rt;
		pServerSession->SendPacket(&nakMSG, sizeof(nakMSG));
		SUNLOG( eFULL_LOG, "[Handler_Guild_GameToGuild_IMPL(GZ_GUILD_CHANGE_DUTY_SYN)] Fail, [GUILDGUID:%d][OPERGUID:%d][Target:%s][ChangeDuty:%d] rt = %d",
			pRecvMsg->m_GuildGuid, pRecvMsg->m_OperCharGuid, pRecvMsg->m_szTargetCharName, pRecvMsg->m_byDuty, rt );
		return;
	}

	//DB로 전송
	MSG_DZ_GUILD_CHANGE_DUTY_SYN sndMSG;
	sndMSG.m_dwKey			= pRecvMsg->m_dwKey;
	sndMSG.m_dwSessionIndex = pServerSession->GetSessionIndex();
	sndMSG.m_GuildGuid		= pRecvMsg->m_GuildGuid;
	sndMSG.m_OperGuid		= pRecvMsg->m_OperCharGuid;
	sndMSG.m_TargetGuid		= pTargerMember->GetCharGuid();
	sndMSG.m_eNewDuty		= (eGUILD_DUTY)pRecvMsg->m_byDuty;
	sndMSG.m_eOldDuty		= pTargerMember->GetDuty();

    Handler_Guild_GuildToDbp::OnDZ_GUILD_CHANGE_DUTY_SYN(
        g_DBHandler.GetSharedSession(), &sndMSG, sizeof(sndMSG));
	//{
	//	MSG_GZ_GUILD_CHANGE_DUTY_NAK nakMSG;
	//	nakMSG.m_dwKey			= pRecvMsg->m_dwKey;
	//	nakMSG.m_byErrorCode	= RC::RC_GUILD_FAILED_BY_NOT_CONNECT_SERVER;
	//	pServerSession->SendPacket(&nakMSG, sizeof(nakMSG));
	//	SUNLOG( eCRITICAL_LOG, "[Handler_Guild_GameToGuild_IMPL(GZ_GUILD_CHANGE_DUTY_SYN)] Can't send Packet![GuildGuid:%u]\n", pRecvMsg->m_GuildGuid );			
	//	return;
	//}
}

//길드 위임 권한 목록 요청	
Handler_Guild_GameToGuild_IMPL(GZ_GUILD_RIGHT_DELEGATION_LIST_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
	MSG_GZ_GUILD_RIGHT_DELEGATION_LIST_SYN* pRecvMsg = (MSG_GZ_GUILD_RIGHT_DELEGATION_LIST_SYN*)pMsg;
	MSG_GZ_GUILD_RIGHT_DELEGATION_LIST_NAK		nakMSG;
	nakMSG.m_dwKey = pRecvMsg->m_dwKey;

	//1. 길드가 있는지 체크
	Guild* pGuild = g_GuildManager.FindGuild( pRecvMsg->m_GuildGuid );
	if( !pGuild )
	{
		nakMSG.m_byErrorCode = RC::RC_GUILD_NOT_EXIST_GUILD;
		pServerSession->SendPacket(&nakMSG, sizeof(nakMSG));
		return;
	}

	//2. 리스트 요청자가 리스트를 요청해도 되는 사람인지 체크한다.
	if( !g_GuildManager.CheckRight( pRecvMsg->m_GuildGuid, pRecvMsg->m_OperCharGuid, eGUILD_RIGHTS_VIEW_RIGHT ) )
	{
		nakMSG.m_byErrorCode = RC::RC_GUILD_HAVE_NOT_RIGHT;
		pServerSession->SendPacket(&nakMSG, sizeof(nakMSG));
		return;
	}

	//3. 권한위임대상이 위임대상이 되는지 체크.
	RC::eGUILD_RESULT rc = g_GuildManager.IsDelegateChar( pRecvMsg->m_GuildGuid, pRecvMsg->m_szTargetCharName );	
	if( rc != RC::RC_GUILD_SUCCESS )
	{
		nakMSG.m_byErrorCode = rc;
		pServerSession->SendPacket(&nakMSG, sizeof(nakMSG));
		return;
	}

	//4. 위임할수 있는 권한 리스트를 불러온다. 불러올때는 현재 해당 유저의 위임된 권한까지 같이 가져온다.
	MSG_GZ_GUILD_RIGHT_DELEGATION_LIST_ACK			Ack;
	Ack.m_dwKey = pRecvMsg->m_dwKey;
	Ack.m_GuildGuid = pRecvMsg->m_GuildGuid;
	g_GuildManager.GetDelegateRight( pRecvMsg->m_GuildGuid, pRecvMsg->m_szTargetCharName, Ack.m_DelegateRight );
	strncpy( Ack.m_szTargetCharName, pRecvMsg->m_szTargetCharName, MAX_CHARNAME_LENGTH );
	pServerSession->SendPacket( &Ack, sizeof(Ack) );
	
}

//길드 권한 위임
Handler_Guild_GameToGuild_IMPL(GZ_GUILD_RIGHT_DELEGATION_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
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
		return;					//있을수 없다....

	//5. DB에 보낸다.
	MSG_DZ_GUILD_RIGHT_DELEGATION_SYN	synMsg;
	synMsg.m_dwKey			= pRecvMsg->m_dwKey;
	synMsg.m_dwSessionIndex = pServerSession->GetSessionIndex();
	synMsg.m_GuildGuid		= pRecvMsg->m_GuildGuid;
	synMsg.m_TargetGuid		= pTargetMember->GetCharGuid();
	CopyMemory( synMsg.m_DelegateRight, pRecvMsg->m_DelegateRight, sizeof(synMsg.m_DelegateRight) );
    Handler_Guild_GuildToDbp::OnDZ_GUILD_RIGHT_DELEGATION_SYN(
        g_DBHandler.GetSharedSession(), &synMsg, sizeof(synMsg));
	//{
	//	MSG_GZ_GUILD_RIGHT_DELEGATION_NAK		nakMSG;
	//	nakMSG.m_byErrorCode = RC::RC_GUILD_FAILED_BY_NOT_CONNECT_SERVER;
	//	pServerSession->SendPacket(&nakMSG, sizeof(nakMSG));
	//	return;
	//}
}

//길드장 변경
Handler_Guild_GameToGuild_IMPL(GZ_GUILD_CHANGE_MASTER_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
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

	//5. DB에 보낸다.
	MSG_DZ_GUILD_CHANGE_MASTER_SYN	synMsg;
	synMsg.m_dwKey			= pRecvMsg->m_dwKey;
	synMsg.m_dwSessionIndex = pServerSession->GetSessionIndex();
	synMsg.m_GuildGuid		= pRecvMsg->m_GuildGuid;
	synMsg.m_OperCharGuid	= pRecvMsg->m_OperCharGuid;
	synMsg.m_TargetCharGuid = pTargetMember->GetCharGuid();
    Handler_Guild_GuildToDbp::OnDZ_GUILD_CHANGE_MASTER_SYN(
        g_DBHandler.GetSharedSession(), &synMsg, sizeof(synMsg));
	//{
	//	MSG_GZ_GUILD_CHANGE_MASTER_NAK		nakMSG;
	//	nakMSG.m_byErrorCode = RC::RC_GUILD_FAILED;
	//	pServerSession->SendPacket(&nakMSG, sizeof(nakMSG));
	//	return;
	//}
}

//길드 군단 생성
Handler_Guild_GameToGuild_IMPL(GZ_GUILD_CREATE_CORPS_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
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

	//위에서 멤버에 대한 검증을 끝내고 온다.
	GuildMember* pMember = g_GuildManager.FindGuildMemberByName( pRecvMsg->m_GuildGuid, pRecvMsg->m_szTargetCharName );
	if( !pMember )	return;

	MSG_DZ_GUILD_CREATE_CORPS_SYN synMSG;
	synMSG.m_dwKey			= pRecvMsg->m_dwKey;
	synMSG.m_dwSessionIndex = pServerSession->GetSessionIndex();
	synMSG.m_GuildGuid		= pRecvMsg->m_GuildGuid;
	synMSG.m_TargetCharGuid	= pMember->GetCharGuid();
	synMSG.m_byCorpsNum		= byCorpsNum;
    Handler_Guild_GuildToDbp::OnDZ_GUILD_CREATE_CORPS_SYN(
        g_DBHandler.GetSharedSession(), &synMSG, sizeof(synMSG));
	//{
	//	MSG_GZ_GUILD_CREATE_CORPS_NAK nakMSG;
	//	nakMSG.m_dwKey = pRecvMsg->m_dwKey;
	//	nakMSG.m_byErrorCode = RC::RC_GUILD_CREATE_CORPS_FAILED;
	//	pServerSession->SendPacket(&nakMSG, sizeof(nakMSG));
	//	SUNLOG( eCRITICAL_LOG, "DZ_GUILD_CREATE_CORPS_SYN ERROR - Can't send Packet![GuildGuid:%u]\n", pRecvMsg->m_GuildGuid );
	//	return;
	//}
}

Handler_Guild_GameToGuild_IMPL(GZ_GUILD_DESTROY_CORPS_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
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

    Handler_Guild_GuildToDbp::OnDZ_GUILD_DESTROY_CORPS_SYN(
        g_DBHandler.GetSharedSession(), &synMsg, sizeof(synMsg));
	//{
	//	MSG_GZ_GUILD_DESTROY_CORPS_NAK nakMSG;
	//	nakMSG.m_dwKey = pRecvMsg->m_dwKey;
	//	nakMSG.m_byErrorCode = RC::RC_GUILD_FAILED_BY_NOT_CONNECT_SERVER;
	//	pServerSession->SendPacket(&nakMSG, sizeof(nakMSG));
	//	SUNLOG( eCRITICAL_LOG, "[MSG_GZ_GUILD_DESTROY_CORPS_SYN] ERROR - Can't send Packet![GuildGuid:%u]\n", pRecvMsg->m_GuildGuid );
	//	return;
	//}
}

Handler_Guild_GameToGuild_IMPL(GZ_GUILD_EMPTY_COMMANDER_INFO_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
	MSG_GZ_GUILD_EMPTY_COMMANDER_INFO_SYN* pRecvMsg = (MSG_GZ_GUILD_EMPTY_COMMANDER_INFO_SYN*)pMsg;

	//비어있는 군단정보를 보겠다는것은 결국 직책변경을 하겠다는 얘기다. 따라서 직책변경 권한을 본다.
	BOOL bRight = g_GuildManager.CheckRight( pRecvMsg->m_GuildGuid, pRecvMsg->m_OperCharGuid, eGUILD_RIGHTS_CHANGE_DUTY );
	if( !bRight )
	{
		MSG_GZ_GUILD_EMPTY_COMMANDER_INFO_NAK nakMSG;
		nakMSG.m_dwKey = pRecvMsg->m_dwKey;
		nakMSG.m_byErrorCode = RC::RC_GUILD_HAVE_NOT_RIGHT;
		pServerSession->SendPacket(&nakMSG, sizeof(nakMSG));
		return;
	}

	//권한이 있으면 현재 비어 있는 군단 정보를 가지고 온다.
	MSG_GZ_GUILD_EMPTY_COMMANDER_INFO_ACK	AckMsg;
	AckMsg.m_dwKey = pRecvMsg->m_dwKey;
	AckMsg.m_GuildGuid = pRecvMsg->m_GuildGuid;
	AckMsg.m_byEmptyCommanderCount = g_GuildManager.GetEmptyCommanderInfo( pRecvMsg->m_GuildGuid, AckMsg.m_byCorpsNum );
	pServerSession->SendPacket( &AckMsg, AckMsg.GetSize() );
}

Handler_Guild_GameToGuild_IMPL(GZ_GUILD_CHANGE_COMMANDER_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
	MSG_GZ_GUILD_CHANGE_COMMANDER_SYN* pRecvMsg = (MSG_GZ_GUILD_CHANGE_COMMANDER_SYN*)pMsg;

	RC::eGUILD_RESULT rt = 
        g_GuildManager.CanChangeCommander( 
        pRecvMsg->m_GuildGuid, 
        pRecvMsg->m_OperCharGuid, 
        pRecvMsg->m_szTargetCharName, 
        pRecvMsg->m_byCorpsNum 
        );

	if( rt != RC::RC_GUILD_SUCCESS )
	{
		MSG_GZ_GUILD_CHANGE_COMMANDER_NAK nakMSG;
		nakMSG.m_dwKey = pRecvMsg->m_dwKey;
		nakMSG.m_byErrorCode = rt;
		pServerSession->SendPacket(&nakMSG, sizeof(nakMSG));
		return;
	}

	GuildMember* pMember = 
        g_GuildManager.FindGuildMemberByName( 
        pRecvMsg->m_GuildGuid, 
        pRecvMsg->m_szTargetCharName 
        );

	if( !pMember )		//위에서 검사했으므로 없을수 없다.
		return;

	MSG_DZ_GUILD_CHANGE_COMMANDER_SYN synMSG;
	synMSG.m_dwKey			= pRecvMsg->m_dwKey;
	synMSG.m_dwSessionIndex = pServerSession->GetSessionIndex();
	synMSG.m_GuildGuid		= pRecvMsg->m_GuildGuid;
	synMSG.m_TargetCharGuid	= pMember->GetCharGuid();
	synMSG.m_byCorpsNum		= pRecvMsg->m_byCorpsNum;
    Handler_Guild_GuildToDbp::OnDZ_GUILD_CHANGE_COMMANDER_SYN(
        g_DBHandler.GetSharedSession(), &synMSG, sizeof(synMSG));
	//{
	//	MSG_GZ_GUILD_CHANGE_COMMANDER_NAK nakMSG;
	//	nakMSG.m_dwKey = pRecvMsg->m_dwKey;
	//	nakMSG.m_byErrorCode = RC::RC_GUILD_FAILED_BY_NOT_CONNECT_SERVER;
	//	pServerSession->SendPacket(&nakMSG, sizeof(nakMSG));
	//	SUNLOG( eCRITICAL_LOG, "[Handler_Guild_GameToGuild_IMPL(GZ_GUILD_CHANGE_COMMANDER_SYN)] - Can't send Packet![GuildGuid:%u]\n", pRecvMsg->m_GuildGuid );
	//	return;
	//}
}

Handler_Guild_GameToGuild_IMPL(GZ_GUILD_CHANGE_CORPS_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
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
    Handler_Guild_GuildToDbp::OnDZ_GUILD_CHANGE_CORPS_SYN(
        g_DBHandler.GetSharedSession(), &synMSG, sizeof(synMSG));
	//{
	//	MSG_GZ_GUILD_CHANGE_COMMANDER_NAK nakMSG;
	//	nakMSG.m_dwKey = pRecvMsg->m_dwKey;
	//	nakMSG.m_byErrorCode = RC::RC_GUILD_FAILED_BY_NOT_CONNECT_SERVER;
	//	pServerSession->SendPacket(&nakMSG, sizeof(nakMSG));
	//	SUNLOG( eCRITICAL_LOG, "[Handler_Guild_GameToGuild_IMPL(GZ_GUILD_CHANGE_COMMANDER_SYN)] - Can't send Packet![GuildGuid:%u]\n", pRecvMsg->m_GuildGuid );
	//	return;
	//}
}

Handler_Guild_GameToGuild_IMPL(GZ_GUILD_CREATE_REAR_CAMP_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
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
    Handler_Guild_GuildToDbp::OnDZ_GUILD_CREATE_REAR_CAMP_SYN(
        g_DBHandler.GetSharedSession(), &synMSG, sizeof(synMSG));
	//{
	//	MSG_GZ_GUILD_CREATE_REAR_CAMP_NAK nakMSG;
	//	nakMSG.m_dwKey = pRecvMsg->m_dwKey;
	//	nakMSG.m_byErrorCode = RC::RC_GUILD_FAILED_BY_NOT_CONNECT_SERVER;
	//	pServerSession->SendPacket(&nakMSG, sizeof(nakMSG));
	//	SUNLOG( eCRITICAL_LOG, "[Handler_Guild_GameToGuild_IMPL(GZ_GUILD_CREATE_REAR_CAMP_SYN)] - Can't send Packet![GuildGuid:%u]\n", pRecvMsg->m_GuildGuid );
	//	return;
	//}
}

Handler_Guild_GameToGuild_IMPL(GZ_GUILD_DESTROY_REAR_CAMP_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
	MSG_GZ_GUILD_DESTROY_REAR_CAMP_SYN* pRecvMsg = (MSG_GZ_GUILD_DESTROY_REAR_CAMP_SYN	*)pMsg;

	RC::eGUILD_RESULT rt = g_GuildManager.CanDestroyRearCamp( pRecvMsg->m_GuildGuid, pRecvMsg->m_OperCharGuid, pRecvMsg->m_byCorpsNum );
	if( rt != RC::RC_GUILD_SUCCESS )
	{
		MSG_GZ_GUILD_DESTROY_REAR_CAMP_NAK nakMSG;
		nakMSG.m_dwKey = pRecvMsg->m_dwKey;
		nakMSG.m_byErrorCode = rt;
		pServerSession->SendPacket(&nakMSG, sizeof(nakMSG));
		SUNLOG( eFULL_LOG, "[Handler_Guild_GameToGuild_IMPL(GZ_GUILD_DESTROY_REAR_CAMP_SYN)] ERROR, GuildGuid = %d, OperGuid = %u, CorpsNum = %d", pRecvMsg->m_GuildGuid,  pRecvMsg->m_OperCharGuid, pRecvMsg->m_byCorpsNum );
		return;
	}

	MSG_DZ_GUILD_DESTROY_REAR_CAMP_SYN synMsg;
	synMsg.m_dwKey			= pRecvMsg->m_dwKey;
	synMsg.m_dwSessionIndex = pServerSession->GetSessionIndex();
	synMsg.m_GuildGuid		= pRecvMsg->m_GuildGuid;
	synMsg.m_byCorpsNum		= pRecvMsg->m_byCorpsNum;
	synMsg.m_OperCharGuid   = pRecvMsg->m_OperCharGuid;

    Handler_Guild_GuildToDbp::OnDZ_GUILD_DESTROY_REAR_CAMP_SYN(
        g_DBHandler.GetSharedSession(), &synMsg, sizeof(synMsg));
	//{
	//	MSG_GZ_GUILD_DESTROY_REAR_CAMP_NAK nakMSG;
	//	nakMSG.m_dwKey = pRecvMsg->m_dwKey;
	//	nakMSG.m_byErrorCode = RC::RC_GUILD_FAILED_BY_NOT_CONNECT_SERVER;
	//	pServerSession->SendPacket(&nakMSG, sizeof(nakMSG));
	//	SUNLOG( eCRITICAL_LOG, "[Handler_Guild_GameToGuild_IMPL(GZ_GUILD_DESTROY_REAR_CAMP_SYN)] ERROR - Can't send Packet![GuildGuid:%u]\n", pRecvMsg->m_GuildGuid );
	//	return;
	//}
}

//길드 초대.
Handler_Guild_GameToGuild_IMPL(GZ_GUILD_INVITE_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
	MSG_GZ_GUILD_INVITE_SYN* recv_msg = (MSG_GZ_GUILD_INVITE_SYN*)pMsg;

	RC::eGUILD_RESULT result_code = g_GuildManager.CanInviteGuild(recv_msg->m_GuildGuid, 
                                                                  recv_msg->m_OperCharGuid, 
                                                                  recv_msg->m_TargetCharGuid, 
                                                                  recv_msg->m_Level);
	if (result_code != RC::RC_GUILD_SUCCESS)
	{
		MSG_GZ_GUILD_INVITE_NAK nak_msg;
		nak_msg.m_dwKey = recv_msg->m_dwKey;
		nak_msg.m_byErrorCode = result_code;
		pServerSession->SendPacket(&nak_msg, sizeof(nak_msg));
		SUNLOG(
            eFULL_LOG, 
            __FUNCTION__" : Error : guild guid : %d, request : %u, target : %u, result : %d", 
			recv_msg->m_GuildGuid,  
            recv_msg->m_OperCharGuid, 
            recv_msg->m_TargetCharGuid, 
            result_code
            );
		return;
	}

	MSG_GZ_GUILD_INVITE_ACK	ack_msg;
	ack_msg.m_dwKey = recv_msg->m_dwKey;
	ack_msg.m_TargetUserKey	= recv_msg->m_TargetUserKey;
	pServerSession->SendPacket(&ack_msg, sizeof(ack_msg));
}

//길드 멤버 로그인
Handler_Guild_GameToGuild_IMPL(GZ_GUILD_LOGIN_CHAR_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
	MSG_GZ_GUILD_LOGIN_CHAR_SYN* pRecvMsg = (MSG_GZ_GUILD_LOGIN_CHAR_SYN*)pMsg;

	//Invite나 Join이나 조건은 똑같다.
	RC::eGUILD_RESULT rt = g_GuildManager.CanLogin( pRecvMsg->m_GuildGuid, pRecvMsg->m_CharGuid );

	if( rt == RC::RC_GUILD_NOT_EXIST_GUILD )
	{
		// 현재 길드서버에 길드가 존재 하지 않으면.. DB에 길드 정보를 요청한다.
		MSG_DZ_GUILD_SELECT_SYN synMsg;
		synMsg.m_dwKey = pRecvMsg->m_dwKey;
        synMsg.m_dwSessionIndex = 0; // (WAVERIX) (090903) (CHANGES) ...-.-;...
		synMsg.m_GuildGuid = pRecvMsg->m_GuildGuid;
		synMsg.m_bySelectType = 1;		//1은 로그인시 SELECT를 뜻한다.
		synMsg.m_LoginCharGuid	= pRecvMsg->m_CharGuid;
        Handler_Guild_GuildToDbp::OnDZ_GUILD_SELECT_SYN(
            g_DBHandler.GetSharedSession(), &synMsg, sizeof(synMsg));
		//{
		//	MSG_GZ_GUILD_LOGIN_CHAR_NAK nakMsg;
		//	nakMsg.m_byErrorCode = RC::RC_GUILD_FAILED_BY_NOT_CONNECT_SERVER;
		//	pServerSession->SendPacket( &nakMsg, sizeof(nakMsg) );
		//	return;
		//}
        // CHANGES: f110627.2L, fixed the invalid acknowledgement answer
        // that this status is pended.
        return;
	}
	else if( rt == RC::RC_GUILD_SUCCESS )
	{
		// 길드 정보가 존재하고, 멤버가 길드 멤버이다. 리턴값 체크 하지 않고 바로 로그인 시킨다.
		g_GuildManager.Login( pRecvMsg->m_GuildGuid, pRecvMsg->m_CharGuid );

		MSG_GZ_GUILD_LOGIN_CHAR_BRD brdMsg;
		brdMsg.m_dwKey = pRecvMsg->m_dwKey;
		g_GuildManager.SerializeGuildInfo( pRecvMsg->m_GuildGuid, brdMsg.m_GuildInfo, SERIALIZE_LOAD );
		g_GuildManager.SerializeCorpsInfo( pRecvMsg->m_GuildGuid, brdMsg.m_CorpsTotalInfo, SERIALIZE_LOAD );
		g_GuildManager.GetGuildMemberInfo( pRecvMsg->m_GuildGuid, pRecvMsg->m_CharGuid, brdMsg.m_MemberInfo );
		g_DBProxyServer.SendToSessionType(GAME_SERVER, &brdMsg, sizeof(brdMsg));

		MSG_WZ_GUILD_LOGIN_CHAR_CMD	cmdMsg;
		g_GuildManager.GetWorldGuildInfo( pRecvMsg->m_GuildGuid, cmdMsg.m_WorldGuildInfo );
		g_GuildManager.GetGuildBaseMemberInfo( pRecvMsg->m_GuildGuid, pRecvMsg->m_CharGuid, cmdMsg.m_BaseMemberInfo );
        g_DBProxyServer.SendToWorldServer(&cmdMsg, sizeof(cmdMsg));
		return;
	}
	else if( rt == RC::RC_GUILD_ALREADY_LOGIN_MEMBER )
		return;

	//나머지 케이스는 실패 케이스다.
	MSG_GZ_GUILD_LOGIN_CHAR_NAK nakMsg;
    nakMsg.m_dwKey = pRecvMsg->m_dwKey; // (WAVERIX) (090903) (BUG-FIX) ...-.-+...
	nakMsg.m_byErrorCode = rt;
	pServerSession->SendPacket( &nakMsg, sizeof(nakMsg) );
}

//길드 멤버 로그아웃
Handler_Guild_GameToGuild_IMPL(GZ_GUILD_LOGOUT_CHAR_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
	MSG_GZ_GUILD_LOGOUT_CHAR_SYN* pRecvMsg = (MSG_GZ_GUILD_LOGOUT_CHAR_SYN*)pMsg;

	//1. 길드서버의 해당 유저의 정보를 로그아웃으로 변경한다.
	RC::eGUILD_RESULT rc = g_GuildManager.Logout( pRecvMsg->m_GuildGuid, pRecvMsg->m_CharGuid );

	// 길드서버가 캐릭터 접속이후 뜨게 되면, 길드서버에는 현재 캐릭터에 대한 길드 정보가 없다.( 같은 길드원이 접속하지 않는 이상... )
	// 따라서 게임서버에서는 길드GUID가 있어서 로그아웃 패킷을 보내고, 길드 서버에서는 해당 길드가 없다고 할 것이다.
	if( rc == RC::RC_GUILD_NOT_EXIST_GUILD )
		return;

	if( rc != RC::RC_GUILD_SUCCESS )
	{
		SUNLOG( eCRITICAL_LOG, "Handler_Guild_GameToGuild_IMPL(GZ_GUILD_LOGOUT_CHAR_SYN) Fail, GuildGuid = %u, CharGuid = %u, rc = %d",
			pRecvMsg->m_GuildGuid, pRecvMsg->m_CharGuid, rc );
		return;
	}

	//2. 게임서버로 보낸다.
	MSG_GZ_GUILD_LOGOUT_CHAR_BRD	brdMsg;
	brdMsg.m_GuildGuid = pRecvMsg->m_GuildGuid;
	brdMsg.m_CharGuid = pRecvMsg->m_CharGuid;
	g_GuildManager.GetCharName( pRecvMsg->m_GuildGuid, pRecvMsg->m_CharGuid, brdMsg.m_szCharName, MAX_CHARNAME_LENGTH );
	g_DBProxyServer.SendToSessionType(GAME_SERVER, &brdMsg, sizeof(brdMsg));

	//3. 월드서버로 보낸다.
	MSG_WZ_GUILD_LOGOUT_CHAR_CMD	cmdMsg;
	cmdMsg.m_GuildGuid = pRecvMsg->m_GuildGuid;
	cmdMsg.m_CharGuid = pRecvMsg->m_CharGuid;
	g_DBProxyServer.SendToWorldServer(&cmdMsg, sizeof(cmdMsg));
}


//길드 멤버 레벨업
Handler_Guild_GameToGuild_IMPL(GZ_GUILD_CHAR_LEVELUP_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
	MSG_GZ_GUILD_CHAR_LEVELUP_SYN* pRecvMsg = (MSG_GZ_GUILD_CHAR_LEVELUP_SYN*)pMsg;

	RC::eGUILD_RESULT rt = g_GuildManager.LevelUpMember( pRecvMsg->m_GuildGuid, pRecvMsg->m_CharGuid, pRecvMsg->m_Level );

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    g_GuildManager.UpdateGuildJoinRequestCharacter( pRecvMsg->m_CharGuid, pRecvMsg->m_Level );
#endif //_NA_007330_20140620_GUILD_SYSTEM_EXTENSION

	// 결과가 레벨업에 의한 길드 탈퇴이면..
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
		msgSYN.m_byPenaltyType	= 3; // 레벨업에 의한 탈퇴.
        Handler_Guild_GuildToDbp::OnDZ_GUILD_WITHDRAW_SYN(
            g_DBHandler.GetSharedSession(), &msgSYN, sizeof(msgSYN));
		//{
		//	MSG_GZ_GUILD_WITHDRAW_NAK msgNAK;
		//	msgNAK.m_dwKey			= pRecvMsg->m_dwKey;
		//	msgNAK.m_byErrorCode	= RC::RC_GUILD_FAILED_BY_NOT_CONNECT_SERVER;
		//	pServerSession->SendPacket( &msgNAK, sizeof(msgNAK) );
		//}
		return;
	}

	if( rt != RC::RC_GUILD_SUCCESS )
	{
		SUNLOG( eCRITICAL_LOG, "[Handler_Guild_GameToGuild_IMPL(GZ_GUILD_CHAR_LEVELUP_SYN)] Fail, GuildGuid = %u, CharGuid = %u, Level = %u, rt = %d", 
			 pRecvMsg->m_GuildGuid, pRecvMsg->m_CharGuid, pRecvMsg->m_Level, rt );
		return;
	}

	MSG_GZ_GUILD_CHAR_LEVELUP_BRD brdMsg;
	brdMsg.m_GuildGuid = pRecvMsg->m_GuildGuid;
	g_GuildManager.GetCharName( pRecvMsg->m_GuildGuid, pRecvMsg->m_CharGuid, brdMsg.m_szCharName, MAX_CHARNAME_LENGTH  );
	brdMsg.m_Level = pRecvMsg->m_Level;
	pServerSession->SendPacket( &brdMsg, sizeof(brdMsg) );
}

Handler_Guild_GameToGuild_IMPL( GZ_GUILD_TOTALINFO_REQ_SYN )
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
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
#ifdef _NA004034_20120102_GUILD_POINT_SYSTEM
    g_GuildManager.GetGuildInfoExtra(pRecvMsg->m_GuildGuid, ackMsg.guild_info_extra_);
#endif //_NA004034_20120102_GUILD_POINT_SYSTEM
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    g_GuildManager.GetGuildJoinRequestTotalInfo(pRecvMsg->m_GuildGuid, ackMsg.m_GuildJoinRequestTotalInfo);
#endif //_NA_007330_20140620_GUILD_SYSTEM_EXTENSION
	pServerSession->SendPacket( &ackMsg, ackMsg.GetSize() );
}

//GM명령어 길드포인트 세팅
Handler_Guild_GameToGuild_IMPL(GZ_GUILD_GM_GUILD_POINT_SYN )
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
	MSG_GZ_GUILD_GM_GUILD_POINT_SYN* pRecvMsg = (MSG_GZ_GUILD_GM_GUILD_POINT_SYN*)pMsg;

	Guild* pGuild = g_GuildManager.FindGuild( pRecvMsg->m_GuildGuid );
	if( !pGuild )
	{
		//만일 길드 서버에 정보가 없으면, 이상한 유저이거나 Agent서버로 로그인 하지 않은 유저이다.
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
    Handler_Guild_GuildToDbp::OnDZ_GUILD_GM_GUILD_POINT_SYN(
        g_DBHandler.GetSharedSession(), &synMsg, sizeof(synMsg));
	//{
	//	MSG_GZ_GUILD_GM_GUILD_POINT_NAK	nakMsg;
	//	nakMsg.m_dwKey = pRecvMsg->m_dwKey;
	//	nakMsg.m_byErrorCode = RC::RC_GUILD_FAILED_BY_NOT_CONNECT_SERVER;
	//	pServerSession->SendPacket( &nakMsg, sizeof(nakMsg) );
	//	return;
	//}

}

Handler_Guild_GameToGuild_IMPL(GZ_GUILD_RELATION_INFO_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
	MSG_GZ_GUILD_RELATION_INFO_SYN* pRecvMsg = (MSG_GZ_GUILD_RELATION_INFO_SYN*)pMsg;

	MSG_DZ_GUILD_RELATION_INFO_SYN msg;
	msg.m_dwKey = pMsg->m_dwKey;
    msg.m_dwSessionIndex = pServerSession->GetSessionIndex();
	msg.m_GuildGuid =  pRecvMsg->m_GuildGuid;
    Handler_Guild_GuildToDbp::OnDZ_GUILD_RELATION_INFO_SYN(
        g_DBHandler.GetSharedSession(), &msg, sizeof(msg));
	//{
	//	//MSG_GZ_GUILD_ADD_INFO_NAK nakMsg;
	//	//nakMsg.m_dwKey			= pRecvMsg->m_dwKey;
	//	//strncpy( nakMsg.m_szTargetCharName, pRecvMsg->m_tszCharName, MAX_CHARNAME_LENGTH );
	//	//nakMsg.m_byErrorCode	= RC::RC_GUILD_FAILED_BY_NOT_CONNECT_SERVER;
	//	//pServerSession->SendPacket( &nakMsg, sizeof(nakMsg) );
	//	return;
	//}

	//MSG_GZ_GUILD_RELATION_INFO_BRD SendToGameServer;
	//SendToGameServer.m_dwKey = pRecvMsg->m_dwKey;
	//SendToGameServer.m_AddInfo.m_GuildGuid = pRecvMsg->m_GuildGuid;
	//LoadRelationInfo(&SendToGameServer.m_AddInfo);
	//SendToGameServer.m_byRelCnt = SendToGameServer.m_AddInfo.m_byRelationGuildCnt;
	//pServerSession->SendPacket( &SendToGameServer, SendToGameServer.GetSize());
}

Handler_Guild_GameToGuild_IMPL(GZ_GUILD_JOIN_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
	MSG_GZ_GUILD_JOIN_SYN * recv_msg = (MSG_GZ_GUILD_JOIN_SYN *)pMsg;

	eGUILD_DUTY Duty = eGUILD_DUTY_NONE;
	BYTE corps_number = 0;
	RC::eGUILD_RESULT result_code = g_GuildManager.CanJoinGuild(recv_msg->m_GuildGuid, 
                                                                recv_msg->m_OperCharGuid, 
                                                                recv_msg->m_TargetCharGuid, 
                                                                recv_msg->m_Level, 
                                                                Duty, 
                                                                corps_number);
	if (result_code != RC::RC_GUILD_SUCCESS)
	{
		MSG_GZ_GUILD_JOIN_NAK nak_msg;
		nak_msg.m_dwKey = recv_msg->m_dwKey;
        strncpy(nak_msg.m_szTargetCharName, recv_msg->m_tszCharName,
                _countof(nak_msg.m_szTargetCharName));
        nak_msg.m_szTargetCharName[_countof(nak_msg.m_szTargetCharName) - 1] = '\0';
		strncpy( nak_msg.m_szTargetCharName, recv_msg->m_tszCharName, MAX_CHARNAME_LENGTH );
		nak_msg.m_byErrorCode = result_code;
		pServerSession->SendPacket( &nak_msg, sizeof(nak_msg) );
		return;
	}

	MSG_DZ_GUILD_JOIN_SYN syn_msg;
	syn_msg.m_dwKey = recv_msg->m_dwKey;
	syn_msg.m_dwSessionIndex = pServerSession->GetSessionIndex();
	syn_msg.m_GuildGuid	= recv_msg->m_GuildGuid;
	syn_msg.m_OperCharGuid = recv_msg->m_OperCharGuid;
	syn_msg.m_TargetCharGuid = recv_msg->m_TargetCharGuid;
    strncpy(syn_msg.m_tszCharName, recv_msg->m_tszCharName, _countof(syn_msg.m_tszCharName));
    syn_msg.m_tszCharName[_countof(syn_msg.m_tszCharName) - 1] = '\0';
	syn_msg.m_byClass = recv_msg->m_byClass;
	syn_msg.m_Level	= recv_msg->m_Level;
	syn_msg.m_Duty = Duty;		
	syn_msg.m_byCorpsNum = corps_number;
    Handler_Guild_GuildToDbp::OnDZ_GUILD_JOIN_SYN(g_DBHandler.GetSharedSession(), 
                                                  &syn_msg, 
                                                  sizeof(syn_msg));
}



Handler_Guild_GameToGuild_IMPL(GZ_GUILD_BOTH_RELATION_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
	PACKET_PTR_CONVERT(MSG_GZ_GUILD_BOTH_RELATION_SYN, pRecvMsg, pMsg);

	CHARGUID RightMemberIndex = 0;
	GUILDGUID response_guild_index = 0;
	RC::eGUILD_RESULT result = GuildSmallContentEntry::RelationSystem::CanDoRelationChange(
        pRecvMsg->m_GuildGuid, pRecvMsg->m_RequestPlayerIndex);
	if(result == RC::RC_GUILD_SUCCESS)
	{
		response_guild_index = g_GuildManager.GetGuildGuidByGuildName(pRecvMsg->m_tszRelationGuildName);
		if(0 != response_guild_index)
		{
			// (lst1024)(090402) 길드 삭제 대기 상태면 동맹 신청 할 수 없다.
			Guild* pGuild = g_GuildManager.FindGuild(response_guild_index);
			if( !pGuild )
				result = RC::RC_GUILD_NOT_EXIST_GUILD;
			else
			{
				if (pRecvMsg->m_eRelationType == eGUILD_RELATION_ALLIENCE &&
                    pGuild->CheckGuildDeleteState() == eGUILD_DELCHK_DESTROY_WAIT)
                {
					result = RC::RC_GUILD_DELCHK_DESTROY_WAIT;
                }
				else
				{
					// 요청 받은 길드에서 권한을 가진 길드 멤버 찾기
					RightMemberIndex = g_GuildManager.GetGuildMasterByGuildName(pRecvMsg->m_tszRelationGuildName);
					if(0 == RightMemberIndex) //길드 마스터가 없을 경우
					{
						result = RC::RC_GUILD_CURRENT_NONE_MASTER;

						// 서브 마스터 권한 조사
						RightMemberIndex = g_GuildManager.FindRightMember(response_guild_index, eGUILD_DUTY_SUBMASTER);
						if(0 != RightMemberIndex)
						{
							result = GuildSmallContentEntry::RelationSystem::CanDoRelationChange(
                                response_guild_index, RightMemberIndex);
							if(result !=RC::RC_GUILD_SUCCESS)
							{
								// 군사 권한 조사
								RightMemberIndex = g_GuildManager.FindRightMember(response_guild_index, eGUILD_DUTY_STRATEGIST);
								if(0 != RightMemberIndex)
								{
									result = GuildSmallContentEntry::RelationSystem::CanDoRelationChange(
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

	//. 권한을 가진 멤버가 있으면~
	if(RC::RC_GUILD_SUCCESS == result)
	{
		result = 
            GuildSmallContentEntry::RelationSystem::CanDoRelationShip(
            pRecvMsg->m_GuildGuid, 
            response_guild_index, 
            pRecvMsg->m_eRelationType 
            );

		if(RC::RC_GUILD_SUCCESS == result)
		{
			MSG_GZ_GUILD_BOTH_RELATION_ACK msg;
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

Handler_Guild_GameToGuild_IMPL(GZ_GUILD_RELATION_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
	PACKET_PTR_CONVERT(MSG_GZ_GUILD_RELATION_SYN, pRecvMsg, pMsg);

	//요청자의 권한 조사.
	// (lst1024)(090408) 길드 권한 체크시 케릭터GUID 사용
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

		result = GuildSmallContentEntry::RelationSystem::CanDoRelationShip(
            request_guild_index, response_guild_index, pRecvMsg->m_eRelationType);
		if(RC::RC_GUILD_SUCCESS == result)
		{
			MSG_DZ_GUILD_RELATION_SYN msg;
			msg.m_dwKey = pRecvMsg->m_dwKey; /* AnswerPlayer 유저 키 */
			msg.m_eRelationType = pRecvMsg->m_eRelationType;
			msg.m_RequestGuildGuid	= pRecvMsg->m_RequestGuildGuid;
			msg.m_RequestUserKey = pRecvMsg->m_RequestUserKey; /* RequestPlayer 유저 키 */
			msg.m_ResponseGuildGuid	= pRecvMsg->m_ResponseGuildGuid;
			msg.m_dwSessionIndex = pServerSession->GetSessionIndex();
			util::GUILDNAMECOPY(msg.m_ResponseGuildName, pRecvMsg->m_tszRelationGuildName);
            Handler_Guild_GuildToDbp::OnDZ_GUILD_RELATION_SYN(
                g_DBHandler.GetSharedSession(), &msg, sizeof(msg));
            return;
			
			//result = RC::RC_GUILD_DISCONNECTED_GUILD_SERVER;
		}
	}

	if (!pResponseGuild && 
        pRecvMsg->m_eRelationType != eGUILD_RELATION_ALLIENCE &&
        pRecvMsg->m_eRelationType != eGUILD_RELATION_CANCEL_BOTH_HOSTILITY)
	{
		MSG_DZ_GUILD_RELATION_SYN msg;
		msg.m_dwKey = pRecvMsg->m_dwKey; /* AnswerPlayer 유저 키 */
		msg.m_eRelationType = pRecvMsg->m_eRelationType;
		msg.m_RequestGuildGuid	= pRecvMsg->m_RequestGuildGuid;
		msg.m_RequestUserKey = pRecvMsg->m_RequestUserKey; /* RequestPlayer 유저 키 */
		msg.m_ResponseGuildGuid	= pRecvMsg->m_ResponseGuildGuid;
		msg.m_dwSessionIndex = pServerSession->GetSessionIndex();
		util::GUILDNAMECOPY(msg.m_ResponseGuildName, pRecvMsg->m_tszRelationGuildName);
        Handler_Guild_GuildToDbp::OnDZ_GUILD_RELATION_SYN(
            g_DBHandler.GetSharedSession(), &msg, sizeof(msg));
        return;
		
		//result = RC::RC_GUILD_DISCONNECTED_GUILD_SERVER;
	}

	MSG_GZ_GUILD_RELATION_NAK msg;
	msg.m_dwKey = pRecvMsg->m_dwKey;
	msg.m_byErrorCode = result;
	pServerSession->SendPacket(&msg, sizeof(msg));
}

Handler_Guild_GameToGuild_IMPL(GZ_GUILD_RELATION_BOTH_AGAINST_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
	PACKET_PTR_CONVERT(MSG_GZ_GUILD_RELATION_BOTH_AGAINST_SYN, pRecvMsg, pMsg);

	GUILDGUID requestGuildGuid = pRecvMsg->m_GuildGuid;
	GUILDGUID responseGuildGuid = pRecvMsg->m_RelGuildGuid;
	CHARGUID requestPlayerGuid = pRecvMsg->m_MasterCharGuid;

	RC::eGUILD_RESULT result = GuildSmallContentEntry::RelationSystem::CanDoRelationChange(
        requestGuildGuid, requestPlayerGuid);
	if(result != RC::RC_GUILD_SUCCESS)
	{
		MSG_GZ_GUILD_RELATION_BOTH_AGAINST_NAK msg;
		msg.m_dwKey = pRecvMsg->m_dwKey;
		msg.m_byErrorCode = result;
		pServerSession->SendPacket(&msg, sizeof(msg));
		return;
	}

	result = GuildSmallContentEntry::RelationSystem::CanDoRelationBothHostility(
        requestGuildGuid, requestPlayerGuid);

	MSG_DZ_GUILD_RELATION_BOTH_AGAINST_SYN msg;
	msg.m_dwKey = pRecvMsg->m_dwKey;
	msg.m_GuildGuid = requestGuildGuid;
	msg.m_RelGuildGuid = responseGuildGuid;
	msg.m_MasterCharGuid = requestPlayerGuid;
	msg.m_dwSessionIndex = pServerSession->GetSessionIndex();
    ZeroMemory(msg.m_tszRelGuildName, sizeof(msg.m_tszRelGuildName));

    Handler_Guild_GuildToDbp::OnDZ_GUILD_RELATION_BOTH_AGAINST_SYN(
        g_DBHandler.GetSharedSession(), &msg, sizeof(msg));
	//{
	//	MSG_GZ_GUILD_RELATION_BOTH_AGAINST_NAK msg;
	//	msg.m_dwKey = pRecvMsg->m_dwKey;
	//	msg.m_byErrorCode = RC::RC_GUILD_DISCONNECTED_GUILD_SERVER;
	//	pServerSession->SendPacket(&msg, sizeof(msg));
	//	return;
	//}

}

Handler_Guild_GameToGuild_IMPL(GZ_GUILD_RELATION_DELETE_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
	PACKET_PTR_CONVERT(MSG_GZ_GUILD_RELATION_DELETE_SYN, pRecvMsg, pMsg);

	eGUILD_RELATION eType = pRecvMsg->m_eType;
	GUILDGUID requestGuildGuid = pRecvMsg->m_GuildGuid;
	GUILDGUID responseGuildGuid = pRecvMsg->m_RelGuildGuid;
	CHARGUID requestPlayerGuid = pRecvMsg->m_RequestPlayerIndex;

	RC::eGUILD_RESULT result = GuildSmallContentEntry::RelationSystem::CanDoRelationChange(
        requestGuildGuid, requestPlayerGuid);
	if(result != RC::RC_GUILD_SUCCESS)
	{
		MSG_GZ_GUILD_RELATION_DELETE_NAK msg;
		msg.m_dwKey = pRecvMsg->m_dwKey;
		msg.m_byErrorCode = result;
		pServerSession->SendPacket(&msg, sizeof(msg));
		return;
	}

	result = GuildSmallContentEntry::RelationSystem::CanDoRelationDelete(
        requestGuildGuid, responseGuildGuid, pRecvMsg->m_eType);
	if(RC::RC_GUILD_SUCCESS == result)
	{
		MSG_DZ_GUILD_RELATION_DELETE_SYN msg;
		msg.m_dwKey = pRecvMsg->m_dwKey;
		msg.m_eType = eType;
		msg.m_GuildGuid = requestGuildGuid;
		msg.m_RelGuildGuid	 = responseGuildGuid;
		msg.m_dwSessionIndex = pServerSession->GetSessionIndex();
        Handler_Guild_GuildToDbp::OnDZ_GUILD_RELATION_DELETE_SYN(
            g_DBHandler.GetSharedSession(), &msg, sizeof(msg));
        return;
		
		//result = RC::RC_GUILD_FAILED_BY_NOT_CONNECT_SERVER;
	}
	
	MSG_GZ_GUILD_RELATION_DELETE_NAK msg;
	msg.m_dwKey = pRecvMsg->m_dwKey;
	msg.m_byErrorCode = result;
	pServerSession->SendPacket(&msg, sizeof(msg));
}


Handler_Guild_GameToGuild_IMPL(GZ_GUILD_RECOVERY_GUILD_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
	MSG_GZ_GUILD_RECOVERY_GUILD_SYN* pRecvMsg = 
		static_cast<MSG_GZ_GUILD_RECOVERY_GUILD_SYN*>(pMsg);

	RC::eGUILD_RESULT result =
        GuildSmallContentEntry::GuildPenalty::CanRecoveryGuild(pRecvMsg->m_GuildGuid);
	if(RC::RC_GUILD_SUCCESS == result)
	{
		MSG_DZ_GUILD_RECOVERY_GUILD_SYN msg;
		msg.m_dwKey		= pRecvMsg->m_dwKey;
        msg.m_dwSessionIndex = pServerSession->GetSessionIndex(); // (WAVERIX) (090903) (CHANGES) ...-.-;...
		msg.m_CharGuid	= pRecvMsg->m_CharGuid;
		msg.m_GuildGuid	= pRecvMsg->m_GuildGuid;

        Handler_Guild_GuildToDbp::OnDZ_GUILD_RECOVERY_GUILD_SYN(
            g_DBHandler.GetSharedSession(), &msg, sizeof(msg));
        return;

		//result = RC::RC_GUILD_RECOVERY_FAILED;
	}

	MSG_GZ_GUILD_RECOVERY_GUILD_NAK msg;
	msg.m_dwKey	= pRecvMsg->m_dwKey;
	msg.m_CharGuid	 = pRecvMsg->m_CharGuid;
	msg.m_byErrorCode = result;
	pServerSession->SendPacket(&msg, sizeof(msg));
}


Handler_Guild_GameToGuild_IMPL(GZ_GUILD_GUILDMARK_REGISTER_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
	MSG_GZ_GUILD_GUILDMARK_REGISTER_SYN* pRecvMsg = 
		static_cast<MSG_GZ_GUILD_GUILDMARK_REGISTER_SYN*>(pMsg);
	
	GuildSmallContentEntry::GuildMark::GuildMarkInfo tempInfo;
	tempInfo.guild_Index = pRecvMsg->m_GuildIndex;
	tempInfo.pattern_index = pRecvMsg->m_PatternIndex;
	tempInfo.background_index = pRecvMsg->m_BackGroundIndex;
	tempInfo.request_player_index = pRecvMsg->m_RequestPlayerIndex;
	
	RC::eGUILD_RESULT result = GuildSmallContentEntry::GuildMark::CanModify(tempInfo);
	if(RC::RC_GUILD_SUCCESS != result) 
	{
		DWORD userKey  = pRecvMsg->m_dwKey;
		GuildSmallContentEntry::GuildMark::ResultCastToGameServer(
            static_cast<GameServerSession*>(pServerSession), result, userKey);
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
	
    Handler_Guild_GuildToDbp::OnDZ_GUILD_GUILDMARK_REGISTER_SYN(
        g_DBHandler.GetSharedSession(), &msg, sizeof(msg));
		//MessageOut(eDEV_LOG, "RegisterCastToDBProxy Error");

}

Handler_Guild_GameToGuild_IMPL(GZ_GUILD_GUILDMARK_DELETE_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
	MSG_GZ_GUILD_GUILDMARK_DELETE_SYN* pRecvMsg = 
		static_cast<MSG_GZ_GUILD_GUILDMARK_DELETE_SYN*>(pMsg);

	GuildSmallContentEntry::GuildMark::GuildMarkInfo tempInfo;
	tempInfo.guild_Index = pRecvMsg->m_GuildIndex;
	tempInfo.pattern_index = pRecvMsg->m_PatternIndex;
	tempInfo.background_index = pRecvMsg->m_BackGroundIndex;
	tempInfo.request_player_index = pRecvMsg->m_RequestPlayerIndex;
	
	RC::eGUILD_RESULT result = GuildSmallContentEntry::GuildMark::CanModify(tempInfo);
	if(RC::RC_GUILD_SUCCESS != result)
	{
		GuildSmallContentEntry::GuildMark::ResultCastToGameServer(
            static_cast<GameServerSession*>(pServerSession), result, pRecvMsg->m_dwKey);
		return;
	}

	MSG_DZ_GUILD_GUILDMARK_DELETE_SYN msg;
	msg.m_dwKey = pRecvMsg->m_dwKey;
    msg.m_dwSessionIndex = pServerSession->GetSessionIndex(); // (WAVERIX) (090903) (BUG-FIX) ...-.-;...
	msg.m_GuildIndex = tempInfo.guild_Index;
	msg.m_PatternIndex = tempInfo.pattern_index;
	msg.m_BackGroundIndex = tempInfo.background_index;
	msg.m_RequestPlayerIndex = tempInfo.request_player_index;

    Handler_Guild_GuildToDbp::OnDZ_GUILD_GUILDMARK_DELETE_SYN(
        g_DBHandler.GetSharedSession(), &msg, sizeof(msg));
		//MessageOut(eDEV_LOG, "RegisterCastToDBProxy Error");
}

Handler_Guild_GameToGuild_IMPL(GZ_GUILD_WAREHOUSE_OPEN_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
    PACKET_PTR_CONVERT(MSG_GZ_GUILD_WAREHOUSE_OPEN_SYN,pRecvMsg, pMsg);
    if (!g_GuildManager.CheckRight(pRecvMsg->m_GuildGuid,
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
    Handler_Guild_GuildToDbp::OnDZ_GUILD_WAREHOUSE_OPEN_SYN(
        g_DBHandler.GetSharedSession(), &msg, sizeof(msg));
        //SUNLOG(eDEV_LOG, "RegisterCastToDBProxy Error(%d)", pRecvMsg->m_GuildGuid);
}

Handler_Guild_GameToGuild_IMPL(GZ_GUILD_WAREHOUSE_CLOSE_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
	MSG_GZ_GUILD_WAREHOUSE_CLOSE_SYN* pRecvMsg = 
		static_cast<MSG_GZ_GUILD_WAREHOUSE_CLOSE_SYN*>(pMsg);
    // (WAVERIX) (090903) (NOTE) 아무래도 길드는 그냥 보기에도 문제가 많을 것 같다.
    // 도저히 정상적인 코드라 생각되지 않는다.
}


Handler_Guild_GameToGuild_IMPL(GZ_GUILD_NAME_CHANGE_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
	MSG_GZ_GUILD_NAME_CHANGE_SYN* pRecvMsg = (MSG_GZ_GUILD_NAME_CHANGE_SYN*)pMsg;

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

    Handler_Guild_GuildToDbp::OnDZ_GUILD_NAME_CHANGE_SYN(
        g_DBHandler.GetSharedSession(), &msg, sizeof(msg));
	//{
	//	MSG_GZ_GUILD_NAME_CHANGE_NAK msg;
	//	msg.m_dwKey = pRecvMsg->m_dwKey;
	//	msg.m_TargetPos = pRecvMsg->m_TargetPos;
	//	msg.m_byErrorCode = RC::RC_GUILD_DISCONNECTED_DBP_SERVER;
	//	pServerSession->SendPacket(&msg, sizeof(msg));

	//	SUNLOG(eDEV_LOG, "RegisterCastToDBProxy Error(%d)", pRecvMsg->m_GuildGuid);
	//}
}

#ifdef _NA_003923_20120130_GUILD_RENEWAL 
Handler_Guild_GameToGuild_IMPL(GZ_GUILD_IN_GAME_GUILD_LOG_LIST_REQ)
{
    const MSG_GZ_GUILD_IN_GAME_GUILD_LOG_LIST_REQ* recv_msg = 
        static_cast<MSG_GZ_GUILD_IN_GAME_GUILD_LOG_LIST_REQ*>(pMsg);

    Guild* guild = g_GuildManager.FindGuild(recv_msg->request_guild_guid_);
    if (guild == NULL)
    {
        SUNLOG(
            eCRITICAL_LOG,
            __FUNCTION__" : Not found guild : guild guid = %d",
            recv_msg->request_guild_guid_
            );
        return;
    }

    guild->GetGuildInGameLog()->SendInGameLogToPlayer(recv_msg->request_char_guid_);
}
#endif//_NA_003923_20120130_GUILD_RENEWAL

#ifdef _NA_004034_20120102_GUILD_INTRODUCTION
Handler_Guild_GameToGuild_IMPL(GZ_GUILD_INTRODUCTION_SAVE_SYN)
{
    const MSG_GZ_GUILD_INTRODUCTION_SAVE_SYN* packet = \
        static_cast<MSG_GZ_GUILD_INTRODUCTION_SAVE_SYN*>(pMsg);

    Guild* guild = g_GuildManager.FindGuild(packet->guild_guid);
    if(!guild)
        return;

    GuildMember* master = guild->FindMaster();
    if(!master || (master->GetCharGuid() != packet->char_guid))
    {
        MSG_GZ_GUILD_INTRODUCTION_SAVE_ACK msg;
        msg.m_dwKey = packet->m_dwKey;
        msg.result_ = RC::RC_GUILD_FAILED;
        memset(msg.introduction_strings_, 0, sizeof(msg.introduction_strings_));
        pServerSession->SendPacket(&msg, sizeof(msg));
        return;
    }

    if (guild) {
        guild->SetGuildIntroduction(packet->guild_introductions_.introduction_strings_);
    }

    ;{
        const CHAR* introduction = guild->GetGuildIntroduction();
        
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
        MSG_GZ_GUILD_INTRODUCTION_SAVE_ACK msg;
        msg.guild_guid = packet->guild_guid;
        msg.m_dwKey = packet->m_dwKey;
        msg.result_ = RC::RC_GUILD_SUCCESS;
        memset(msg.introduction_strings_, 0, sizeof(msg.introduction_strings_));
        strncpy(msg.introduction_strings_, introduction, MAX_GUILD_INTRODUCTION_LENGTH);
        g_DBProxyServer.SendToSessionType(GAME_SERVER, &msg, sizeof(msg));
#else  //_NA_007330_20140620_GUILD_SYSTEM_EXTENSION
        MSG_GZ_GUILD_INTRODUCTION_SAVE_ACK msg;
        msg.m_dwKey = packet->m_dwKey;
        msg.result_ = RC::RC_GUILD_SUCCESS;
        memset(msg.introduction_strings_, 0, sizeof(msg.introduction_strings_));
        strncpy(msg.introduction_strings_, introduction, MAX_GUILD_INTRODUCTION_LENGTH);
        pServerSession->SendPacket(&msg, sizeof(msg));
#endif //_NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    };
    
    MSG_DZ_GUILD_INTRODUCTION_SAVE_SYN dz_msg;
    dz_msg.init();
    dz_msg.char_guid = packet->char_guid;
    dz_msg.guild_guid = packet->guild_guid;
    memcpy(&dz_msg.guild_introductions_, &packet->guild_introductions_, \
        sizeof(packet->guild_introductions_));
    g_DBProxyServer.SendToDBPServer(&dz_msg, sizeof(dz_msg));
}
#endif //_NA_004034_20120102_GUILD_INTRODUCTION

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION

Handler_Guild_GameToGuild_IMPL(GZ_GUILD_JOIN_REQUEST_SYN)
{
    const MSG_GZ_GUILD_JOIN_REQUEST_SYN* packet = \
        static_cast<MSG_GZ_GUILD_JOIN_REQUEST_SYN*>(pMsg);

    GUILDGUID guild_guid = packet->request_info.m_guild_guid;
    CHARGUID  char_guid = packet->request_info.m_char_guid;

    Guild* guild = g_GuildManager.FindGuild(guild_guid);
    if(guild == NULL)
    {
        MSG_GZ_GUILD_JOIN_REQUEST_ACK msg;
        msg.request_info.m_char_guid = char_guid;
        msg.request_info.m_guild_guid = guild_guid;
        msg.result_ = RC::RC_GUILD_JOIN_REQUEST_INVALID_GUILD;
        pServerSession->SendPacket(&msg, sizeof(msg));
        return;
    }

    //캐릭터의 해당길드 가입요청 상태를 가져온다
    RC::eGUILD_RESULT result = g_GuildManager.CanGuildJoinRequest(char_guid, guild_guid);
    if (result != RC::RC_GUILD_SUCCESS)
    {
        MSG_GZ_GUILD_JOIN_REQUEST_ACK msg;
        msg.request_info.m_char_guid = char_guid;
        msg.request_info.m_guild_guid = guild_guid;
        msg.result_ = result;
        pServerSession->SendPacket(&msg, sizeof(msg));
        return;
    }

    //길드 가입 요청 로그를 남긴다
    GUILDLOG->LogGuildJoinRequest(guild, char_guid, packet->request_info.m_char_name);

    MSG_DZ_GUILD_JOIN_REQUEST_INSERT_SYN msg;
    msg.request_info = packet->request_info;

    Handler_Guild_GuildToDbp::OnDZ_GUILD_JOIN_REQUEST_INSERT_SYN(
        g_DBHandler.GetSharedSession(), &msg, sizeof(msg));
}

Handler_Guild_GameToGuild_IMPL(GZ_GUILD_JOIN_REQUEST_REJECT_SYN)
{
    const MSG_GZ_GUILD_JOIN_REQUEST_REJECT_SYN* packet = \
        static_cast<MSG_GZ_GUILD_JOIN_REQUEST_REJECT_SYN*>(pMsg);

    GUILDGUID guild_guid = packet->guild_guid;
    CHARGUID  target_guid = packet->target_guid;

    Guild* guild = g_GuildManager.FindGuild(guild_guid);
    if(guild == NULL)
    {
        MSG_GZ_GUILD_JOIN_REQUEST_REJECT_ACK msg;
        msg.Init();
        msg.m_dwKey = packet->char_guid;
        msg.result = RC::RC_GUILD_JOIN_REQUEST_INVALID_GUILD;
        pServerSession->SendPacket(&msg, sizeof(msg));
        return;
    }

    if (packet->reject_type == eGUILD_JOIN_REJECT_TYPE_NORMAL)
    {
        if (g_GuildManager.CheckRight(packet->guild_guid, packet->char_guid, eGUILD_RIGHTS_JOIN ) == false)
        {
            MSG_GZ_GUILD_JOIN_REQUEST_REJECT_ACK msg;
            msg.Init();
            msg.m_dwKey = packet->char_guid;
            util::GUILDNAMECOPY(msg.guild_name, guild->GetGuildName());
            msg.result = RC::RC_GUILD_DONT_HAVE_RIGHTS_STATE;
            pServerSession->SendPacket(&msg, sizeof(msg));
            return;
        }
    }

    //캐릭터의 해당길드 가입요청 상태를 가져온다
    RC::eGUILD_RESULT result = g_GuildManager.CanGuildJoinRequest(target_guid, guild_guid);
    if (result != RC::RC_GUILD_JOIN_REQUEST_ALREADY_EXIST)
    {
        MSG_GZ_GUILD_JOIN_REQUEST_REJECT_ACK msg;
        msg.Init();
        msg.m_dwKey = packet->char_guid;
        util::GUILDNAMECOPY(msg.guild_name, guild->GetGuildName());
        msg.result = RC::RC_GUILD_DONT_HAVE_RIGHTS_STATE;
        pServerSession->SendPacket(&msg, sizeof(msg));
        return;
    }

    MSG_DZ_GUILD_JOIN_REQUEST_DELETE_SYN msg;
    msg.guild_guid = packet->guild_guid;
    msg.char_guid = packet->char_guid;
    msg.target_guid = packet->target_guid;
    msg.reject_type = packet->reject_type;

    Handler_Guild_GuildToDbp::OnDZ_GUILD_JOIN_REQUEST_DELETE_SYN(
        g_DBHandler.GetSharedSession(), &msg, sizeof(msg));
}

Handler_Guild_GameToGuild_IMPL(GZ_GUILD_RELOAD_SYN)
{
    const MSG_GZ_GUILD_RELOAD_SYN* packet = \
        static_cast<MSG_GZ_GUILD_RELOAD_SYN*>(pMsg);

    //길드에 가입된 캐릭터인지 확인
    GUILDGUID guild_guid = g_GuildManager.FindCharGuildGuid(packet->char_guid);
    
    if (guild_guid != 0)
    {
        MSG_GZ_GUILD_RELOAD_ACK msg;
        msg.guild_guid = guild_guid;
        msg.m_dwKey = packet->m_dwKey;
        g_DBProxyServer.SendToSessionType(GAME_SERVER, &msg, sizeof(msg));
    }
}

Handler_Guild_GameToGuild_IMPL(GZ_GUILD_FACILITY_UPGRADE_SYN)
{
    const MSG_GZ_GUILD_FACILITY_UPGRADE_SYN* packet = \
        static_cast<MSG_GZ_GUILD_FACILITY_UPGRADE_SYN*>(pMsg);

    FLEVEL add_level = 1;

    const RC::eGUILD_RESULT result = g_GuildManager.CanUpdateGuildFacility(packet->facility_code, add_level, packet->guild_guid);
    if (result != RC::RC_GUILD_SUCCESS)
    {
        MSG_GZ_GUILD_FACILITY_UPGRADE_ACK msg;
        msg.char_guid = packet->char_guid;
        msg.facility_code = packet->facility_code;
        msg.m_dwKey = packet->m_dwKey;
        msg.result = result;
        g_DBProxyServer.SendToSessionType(GAME_SERVER, &msg, sizeof(msg));
        return;
    }

    // 권한체크 .
    if (!g_GuildManager.CheckRight(packet->guild_guid, packet->char_guid, eGUILD_RIGHTS_GUILD_EQUIPMENT ))
    {
        MSG_GZ_GUILD_FACILITY_UPGRADE_ACK msg;
        msg.facility_code = packet->facility_code;
        msg.char_guid = packet->char_guid;
        msg.m_dwKey = packet->m_dwKey;
        msg.result = RC::RC_GUILD_HAVE_NOT_RIGHT;
        g_DBProxyServer.SendToSessionType(GAME_SERVER, &msg, sizeof(msg));
        return;
    }

    MSG_DZ_GUILD_FACILITY_UPDATE_SYN syn_msg;
    syn_msg.char_guid = packet->char_guid;
    syn_msg.guild_guid = packet->guild_guid;
    syn_msg.facility_code = packet->facility_code;
    syn_msg.add_level = add_level;

    const GUILD_FACILITY_INFO* info = g_GuildManager.GetGuildFacility(packet->facility_code, packet->guild_guid);
    if (info == NULL)
    {
        syn_msg.current_level = 0;
    }
    else
    {
        syn_msg.current_level = info->level;
    }

    Handler_Guild_GuildToDbp::OnDZ_GUILD_FACILITY_UPDATE_SYN(
        g_DBHandler.GetSharedSession(), &syn_msg, sizeof(syn_msg));
}

Handler_Guild_GameToGuild_IMPL(GZ_GUILD_FACILITY_SELECT_SYN)
{
    const MSG_GZ_GUILD_FACILITY_SELECT_SYN* packet = \
        static_cast<MSG_GZ_GUILD_FACILITY_SELECT_SYN*>(pMsg);

    MSG_DZ_GUILD_FACILITY_SELECT_SYN msg;
    msg.guild_guid = packet->guild_guid;
    
    Handler_Guild_GuildToDbp::OnDZ_GUILD_FACILITY_SELECT_SYN(
        g_DBHandler.GetSharedSession(), &msg, sizeof(msg));
}

#endif //_NA_007330_20140620_GUILD_SYSTEM_EXTENSION

//지역 점령전 기본 정보 셋팅 관련 작업
Handler_Guild_GameToGuild_IMPL(GZ_DOMINATION_INFO_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
    const MSG_GZ_DOMINATION_INFO_SYN* recv_msg = 
        static_cast<MSG_GZ_DOMINATION_INFO_SYN*>(pMsg);

    //기본 정보 로드가 성공한 상태이면 게임 서버에 응답해 준다
    if (DominationManager* domination_manager = DominationManager::Instance())
    {
        if (domination_manager->IsCompleteLoading() == false) {
            return;
        };
        domination_manager->SendAreaInfo(
            static_cast<GameServerSession*>(pServerSession), recv_msg->m_MapCode);
        WAVERIX_WRAPPER((WAVERIX_LC_CALL2("waverix_log_trace_domination",
            _T("|["__FUNCTION__"]|{mapcode}="), recv_msg->m_MapCode)));

#ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION
        domination_manager->SendDominationContinentLordInfo(recv_msg->m_MapCode);
#endif //_NA_007128_20140326_MONDSCHIEN_DOMINATION
    };
}

Handler_Guild_GameToGuild_IMPL(GZ_DOMINATION_AUCTION_INFO_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
    const MSG_GZ_DOMINATION_AUCTION_INFO_SYN* pRecvMsg = 
        static_cast<MSG_GZ_DOMINATION_AUCTION_INFO_SYN*>(pMsg);

    //경매 정보 로드가 성공한 상태이면 게임 서버에 응답해 준다
    if (DominationManager* domination_manager = DominationManager::Instance())
    {
        if (domination_manager->IsCompleteLoading() == false) {
            return;
        };
        domination_manager->SendAuctions(
            static_cast<GameServerSession*>(pServerSession), pRecvMsg->m_MapCode);
        WAVERIX_WRAPPER((WAVERIX_LC_CALL2("waverix_log_trace_domination",
            _T("|["__FUNCTION__"]|{mapcode}="), pRecvMsg->m_MapCode)));
    };
}

//DB로 기본 정보 Update요청을 한다.
Handler_Guild_GameToGuild_IMPL(GZ_DOMINATION_UPDATE_INFO_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
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

    Handler_Guild_GuildToDbp::OnDZ_DOMINATION_UPDATE_INFO_SYN(g_DBHandler.GetSharedSession(), 
                                                              &sendMsg, sizeof(sendMsg));
}

Handler_Guild_GameToGuild_IMPL(GZ_DOMINATION_ACCEP_HEIM_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
    const MSG_GZ_DOMINATION_ACCEP_HEIM_SYN* recv_msg = 
        static_cast<MSG_GZ_DOMINATION_ACCEP_HEIM_SYN*>(pMsg);

    MSG_DZ_DOMINATION_ACCEP_HEIM_SYN send_msg;

#ifdef _CH_20100420_CHANGE_DOMINATION_APPLY_WAY
    send_msg.m_TokenPos = recv_msg->m_TokenPos;
    send_msg.number_of_betting_item = recv_msg->number_of_betting_item;
#endif//_CH_20100420_CHANGE_DOMINATION_APPLY_WAY
    send_msg.m_dwKey = recv_msg->m_dwKey;
    send_msg.m_DominationActionInfo = recv_msg->m_DominationActionInfo;

    Handler_Guild_GuildToDbp::OnDZ_DOMINATION_ACCEP_HEIM_SYN(
        g_DBHandler.GetSharedSession(), &send_msg, sizeof(send_msg));
}

Handler_Guild_GameToGuild_IMPL(GZ_DOMINATION_INIT_ACTIONINFO_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
    const MSG_GZ_DOMINATION_INIT_ACTIONINFO_SYN* pRecvMsg = 
        static_cast<MSG_GZ_DOMINATION_INIT_ACTIONINFO_SYN*>(pMsg);

    MSG_DZ_DOMINATION_INIT_AUCTIONINFO_SYN sendMsg;
    sendMsg.m_MapCode = pRecvMsg->m_MapCode;
    Handler_Guild_GuildToDbp::OnDZ_DOMINATION_INIT_AUCTIONINFO_SYN(
        g_DBHandler.GetSharedSession(), &sendMsg, sizeof(sendMsg));
}

//하임을 돌려줄 길드 마스터의 Guid리스트를 작성하여 WorldServer로 보낸다.
Handler_Guild_GameToGuild_IMPL(GZ_DOMINATION_RETURNHIME_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
    const MSG_GZ_DOMINATION_RETURNHIME_SYN* pRecvMsg = 
        static_cast<MSG_GZ_DOMINATION_RETURNHIME_SYN*>(pMsg);

    DOMINATION_RETURNMONEY_INFO ReturnInfo[MAX_DOMINATION_AUCTION];
    ZeroMemory( ReturnInfo, sizeof(ReturnInfo) );

    const DOMINATION_INFO& dominationInfo = pRecvMsg->m_DominationInfo;

    INT countData = 0;

    //경매에 당첨 되지 않은 길드 리스트 작성
    DominationManager::Instance()->SetListForReturnHeim( dominationInfo.m_FieldCode, 
        ReturnInfo, _countof(ReturnInfo), countData );

#ifndef _CH_20100420_CHANGE_DOMINATION_APPLY_WAY
    //마지막에 지역 점령 길드 마스터이름과 금액을 넣는다.(지역 점령전 경매 보상 하임)
    {
        ReturnInfo[countData].m_TenderCost = dominationInfo.m_BestCost;
        ReturnInfo[countData].m_CostType = SYSMEMO_DOMINATION_REWARD_MONEY;
        memcpy( ReturnInfo[countData].m_tszMaster, 
            dominationInfo.m_inGuildInfo.m_tszGuildMaster, sizeof(TCHAR)*MAX_CHARNAME_LENGTH );
        ReturnInfo[countData].m_tszMaster[MAX_CHARNAME_LENGTH] = '\0';

        ++countData;
    }
#endif//_CH_20100420_CHANGE_DOMINATION_APPLY_WAY
    
    //WorldServer에 하임 쪽지를 돌리도록 요청
    MSG_WZ_GUILD_DOMINATION_RETURNHIME_CMD sendMsg;
    sendMsg.m_MapCode = dominationInfo.m_FieldCode;
    memcpy( sendMsg.m_tszGuild, dominationInfo.m_disGuildInfo.m_tszGuildName, sizeof(TCHAR)*MAX_GUILDNAME_LENGTH );
    sendMsg.m_tszGuild[MAX_GUILDNAME_LENGTH] = '\0';
    memcpy( sendMsg.m_ReturnInfo, ReturnInfo, sizeof(sendMsg.m_ReturnInfo) );
    sendMsg.m_DataCount = countData;

    g_DBProxyServer.SendToWorldServer(&sendMsg, sendMsg.GetSize());

    //GameServer에 완료 응답을 한다.
    MSG_GZ_DOMINATION_RETURNHIME_ACK ackMsg;
    ackMsg.m_MapCode = dominationInfo.m_FieldCode;

    pServerSession->SendPacket(&ackMsg, sizeof(ackMsg));
}

Handler_Guild_GameToGuild_IMPL(GZ_DOMINATION_BATTING_HEIM_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
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
        //현재 길드는 경매 신청을 해도 되는 길드이다.(해산 신청건에 있어서)
        sendMsg.m_ResultCode = RC::DOMINATION_RESULT_SUCCESS;
    }
    sendMsg.m_dwKey = pRecvMsg->m_dwKey;
#ifdef _CH_20100420_CHANGE_DOMINATION_APPLY_WAY
    sendMsg.m_mapCode = pRecvMsg->m_mapCode;
#endif

#ifdef _NA_000000_20130429_DOMINATION_PENALTY
    GuildSmallContentEntry::GuildPenalty::PenaltyTime current_Time = 0;
    util::GetDateTime_YYYYMMDDHHMMSS(current_Time);
    if (pGuild->GetPenaltyTime() >= current_Time) {
        sendMsg.m_ResultCode = RC::DOMINATION_RESULT_PENALTY_TIME;
        sendMsg.penalty_time = pGuild->GetPenaltyTime();
    }
#endif //_NA_000000_20130429_DOMINATION_PENALTY

    pServerSession->SendPacket( &sendMsg, sizeof(sendMsg) );
}

//_NA_20100604_BETTER_DOMINATION_MANAGER
Handler_Guild_GameToGuild_IMPL(GZ_DOMINATION_UPDATE_GUILDINFO_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
    const MSG_GZ_DOMINATION_UPDATE_GUILDINFO_SYN* recv_msg = 
        static_cast<MSG_GZ_DOMINATION_UPDATE_GUILDINFO_SYN*>(pMsg);

    MSG_DZ_DOMINATION_UPDATE_GUILDINFO_SYN send_msg;
    {
        send_msg.guild_guid_ = recv_msg->guild_guid_;
        send_msg.domination_join_count_ = recv_msg->domination_join_count_;
        send_msg.domination_defense_count_ = recv_msg->domination_defense_count_;
        send_msg.domination_hunting_count_ = recv_msg->domination_hunting_count_;
    }
    Handler_Guild_GuildToDbp::OnDZ_DOMINATION_UPDATE_GUILDINFO_SYN(
        g_DBHandler.GetSharedSession(), &send_msg, sizeof(send_msg));

}

//_NA_20100913_GUILD_WAREHOUSE_GM_COMMAND
// GM 명령에 의한 길드 창고 강제 닫기
Handler_Guild_GameToGuild_IMPL(GZ_GUILD_GM_WAREHOUSE_FORCE_CLOSE_CMD)
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
    const MSG_GZ_GUILD_GM_WAREHOUSE_FORCE_CLOSE_CMD* msg = 
        static_cast<MSG_GZ_GUILD_GM_WAREHOUSE_FORCE_CLOSE_CMD*>(pMsg);

    Guild* guild = g_GuildManager.FindGuildName(msg->guild_name);
    if (guild == NULL)
    {
        SUNLOG(eFULL_LOG, "[GZ_GUILD_GM_WAREHOUSE_FORCE_CLOSE_CMD] Guild name '%s' not found!", msg->guild_name);
        return;
    }
    
    // DBProxy와 게임 서버들로 전달한다.
    // 둘 중 가능한 곳에서 성공하기를 기대하므로 타이밍 고려나 응답 대기 없이 바로 양방향으로 뿌린다.
    
    // DBProxy로 강제 닫기 명령을 전달하고
    MSG_DZ_GUILD_GM_WAREHOUSE_FORCE_CLOSE_CMD cmd;
    cmd.gm_player_key = msg->gm_player_key;
    cmd.guild_guid = guild->GetGuildGuid();
    Handler_Guild_GuildToDbp::OnDZ_GUILD_GM_WAREHOUSE_FORCE_CLOSE_CMD(
        g_DBHandler.GetSharedSession(), &cmd, sizeof(cmd));

    // 게임 서버들에 공지한다.
    MSG_GZ_GUILD_GM_WAREHOUSE_FORCE_CLOSE_BRD brd;
    brd.gm_player_key = msg->gm_player_key;
    brd.guild_guid = guild->GetGuildGuid();
    g_DBProxyServer.SendToSessionType(GAME_SERVER, &brd, sizeof(brd));
}

//gamelulu_dev_not : 점령전 정보 저장 기본 구조가 Game <-> Guild <-> DBP 이다.
//하지만 대부분은 Game->DBP->Guild->Game 의 구조로써 해결이 가능하다.
//_NA_20110216_INCREASE_DOMINATION_DEFFENSEWALL_POINT
Handler_Guild_GameToGuild_IMPL(GZ_DOMINATION_ADD_DEFFENSEWALL_POINT_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
    const MSG_GZ_DOMINATION_ADD_DEFFENSEWALL_POINT_SYN* recv_msg = 
        static_cast<MSG_GZ_DOMINATION_ADD_DEFFENSEWALL_POINT_SYN*>(pMsg);

    MSG_DZ_DOMINATION_ADD_DEFFENSEWALL_POINT_SYN send_msg;
    send_msg.item_code_ = recv_msg->item_code_;
    send_msg.m_dwKey = recv_msg->m_dwKey;
    send_msg.domination_info_ = recv_msg->domination_info_;

    Handler_Guild_GuildToDbp::OnDZ_DOMINATION_ADD_DEFFENSEWALL_POINT_SYN(
        g_DBHandler.GetSharedSession(), &send_msg, sizeof(send_msg));
}

//_NA_20110406_CHANGE_RULE_FOR_DOMINATION_JOIN
Handler_Guild_GameToGuild_IMPL(GZ_DOMINATION_MEMBER_JOIN_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
    const MSG_GZ_DOMINATION_MEMBER_JOIN_SYN* recv_msg = 
        static_cast<MSG_GZ_DOMINATION_MEMBER_JOIN_SYN*>(pMsg);

    MSG_DZ_DOMINATION_MEMBER_JOIN_SYN send_msg;
    send_msg.map_code_ = recv_msg->map_code_;
    send_msg.member_info_ = recv_msg->member_info_;

    Handler_Guild_GuildToDbp::OnDZ_DOMINATION_MEMBER_JOIN_SYN(
        g_DBHandler.GetSharedSession(), &send_msg, sizeof(send_msg));
}

Handler_Guild_GameToGuild_IMPL(GZ_DOMINATION_MEMBER_LEAVE_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
    const MSG_GZ_DOMINATION_MEMBER_LEAVE_SYN* recv_msg = 
        static_cast<MSG_GZ_DOMINATION_MEMBER_LEAVE_SYN*>(pMsg);

    MSG_DZ_DOMINATION_MEMBER_LEAVE_SYN send_msg;
    send_msg.map_code_ = recv_msg->map_code_;
    send_msg.member_guid_ = recv_msg->member_guid_;
    send_msg.requester_guid = recv_msg->requester_guid;

    Handler_Guild_GuildToDbp::OnDZ_DOMINATION_MEMBER_LEAVE_SYN(
        g_DBHandler.GetSharedSession(), &send_msg, sizeof(send_msg));
}

Handler_Guild_GameToGuild_IMPL(GZ_DOMINATION_MEMBERLIST_REQ)
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
    const MSG_GZ_DOMINATION_MEMBERLIST_REQ* recv_msg = 
        static_cast<MSG_GZ_DOMINATION_MEMBERLIST_REQ*>(pMsg);

    if (DominationManager* domination_manager = DominationManager::Instance())
    {
        if (domination_manager->IsCompleteLoading() == false) {
            return;
        };
        domination_manager->SendMembers(
            static_cast<GameServerSession*>(pServerSession), recv_msg->map_code_);
        WAVERIX_WRAPPER((WAVERIX_LC_CALL2("waverix_log_trace_domination",
            _T("|["__FUNCTION__"]|{mapcode}="), recv_msg->map_code_)));
    };
}

Handler_Guild_GameToGuild_IMPL(GZ_DOMINATION_MEMBERLIST_INIT_CMD)
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
    const MSG_GZ_DOMINATION_MEMBERLIST_INIT_CMD* recv_msg = 
        static_cast<MSG_GZ_DOMINATION_MEMBERLIST_INIT_CMD*>(pMsg);

    MSG_DZ_DOMINATION_MEMBERLIST_INIT_SYN send_msg;

    Handler_Guild_GuildToDbp::OnDZ_DOMINATION_MEMBERLIST_INIT_SYN(
        g_DBHandler.GetSharedSession(), &send_msg, sizeof(send_msg));
}

#ifdef _NA004034_20120102_GUILD_POINT_SYSTEM
Handler_Guild_GameToGuild_IMPL(GZ_GUILDPOINTSYSTEM)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    const MSG_GZ_GUILDPOINTSYSTEM* packet = \
        static_cast<MSG_GZ_GUILDPOINTSYSTEM*>(pMsg);

    switch(packet->packet_type)
    {
        case GZ_GUILDPOINT_CHANGE_CMD: {
            OnGZ_GUILDPOINT_CHANGE_CMD(pServerSession, pMsg, wSize);
        } break;

        default:
            break;
    }
}

Handler_Guild_GameToGuild_IMPL(GZ_GUILDPOINT_CHANGE_CMD)
{
    const MSG_GZ_GUILDPOINT_CHANGE_CMD* packet = \
        static_cast<MSG_GZ_GUILDPOINT_CHANGE_CMD*>(pMsg);

    DWORD guid = packet->guildpoint.guid_;
    DWORD uuid = packet->guildpoint.uuid_;
    DWORD code = packet->guildpoint.code_;
    DWORD point = packet->guildpoint.point_;

    if(code == CURRENCY_INFO_DONATION_GUILDCOIN)
    {
        g_GuildManager.ProcessGuildCoinChange(guid, uuid, code, point);

        //Write guild game log
        Guild* guild = g_GuildManager.FindGuild(guid);
        if (guild == NULL)
        {
            SUNLOG(eCRITICAL_LOG, __FUNCTION__" : Not found guild : guid = %d", guid);
            return;
        }

        GuildMember* guild_member = guild->FindMember(uuid);
        if (guild_member == NULL)
        {
            SUNLOG(eCRITICAL_LOG, __FUNCTION__ \
                " : Not found guild member : guid = %d, member_guid = %d", guid, uuid);
            return;
        }

        guild->GetGuildInGameLog()->WriteGuildLog(
            GuildGivePoint,
            guid,
            guild_member->GetCharName(),
            point
            );
    }
}
#endif //_NA004034_20120102_GUILD_POINT_SYSTEM

#ifdef _NA_000000_20130429_DOMINATION_PENALTY
Handler_Guild_GameToGuild_IMPL(GZ_DOMINATION_PENALTY_CMD)
{
    const MSG_GZ_DOMINATION_PENALTY_CMD* packet = \
        static_cast<MSG_GZ_DOMINATION_PENALTY_CMD*>(pMsg);

    DWORD guild_guid = packet->guild_guid;
    Guild* guild = g_GuildManager.FindGuild(guild_guid);
    if (guild) 
    {
        CTime penalty_time = packet->penalty_time;
        MAPCODE map_code = packet->map_code;
        WORD member_count = packet->member_count;
        guild->SetPenaltyTime(util::GetDateTime_YYYYMMDDHHMMSS(penalty_time.GetTime()));

        MSG_DZ_DOMINATION_PENALTY_CMD msg;
        msg.guild_guid = guild_guid;
        msg.penalty_time = penalty_time;
        g_DBProxyServer.SendToDBPServer(&msg, sizeof(msg));

        guild->GetGuildInGameLog()->WriteGuildLog( \
            GuildPenaltyTime, guild_guid, NULL, map_code, penalty_time.GetTime(), member_count);
    }
}
#endif //_NA_000000_20130429_DOMINATION_PENALTY

#ifdef _NA_008334_20150608_SONNENSCHEIN
Handler_Guild_GameToGuild_IMPL(GZ_GUILD_SONNENSCHEIN_UPDATE_SYN)
{
    MSG_GZ_GUILD_SONNENSCHEIN_UPDATE_SYN* syn_msg = \
        static_cast<MSG_GZ_GUILD_SONNENSCHEIN_UPDATE_SYN*>(pMsg);

    MSG_DZ_GUILD_SONNENSCHEIN_UPDATE_SYN dz_syn_msg;
    dz_syn_msg.m_dwKey = syn_msg->m_dwKey;
    dz_syn_msg.m_dwSessionIndex = pServerSession->GetSessionIndex();

    dz_syn_msg.guild_guid = syn_msg->guild_guid;
    dz_syn_msg.sonnenschein_phase = syn_msg->sonnenschein_phase;

    Handler_Guild_GuildToDbp::OnDZ_GUILD_SONNENSCHEIN_UPDATE_SYN(g_DBHandler.GetSharedSession(),
        &dz_syn_msg, sizeof(dz_syn_msg));
}
#endif //_NA_008334_20150608_SONNENSCHEIN