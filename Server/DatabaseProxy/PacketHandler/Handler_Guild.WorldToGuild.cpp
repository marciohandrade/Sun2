#include "stdafx.h"
#include "./Handler_Guild.WorldToGuild.h"

#include <PacketStruct_DZ.h>
#include <PacketStruct_GZ.h>
#include <PacketStruct_WZ.h>

// GUILD
#include "Services/Guild/GuildManager.h"
#include "Services/Guild/Guild.h"
#include "Services/Guild/GuildMember.h"
//
#include "QueryObjects/DBHandler.h"
#include "PacketHandler/Handler_Guild.GuildToDbp.h"
//
//

//==================================================================================================
// CHANGES: f110518.2L
const nsPacket::PacketHandlerInfo Handler_Guild_WorldToGuild::GetPacketHandlerNodes()
{
#define HANDLER_NODE_INFO(c, p, phase) { MAKEWORD(c, p), false, phase, Handler_Guild_WorldToGuild::On##p }
#define HANDLER_NODE_INFO_DBR(c, p, phase) { MAKEWORD(c, p), true, phase, Handler_Guild_WorldToGuild::On##p }
#define HANDLER_NODE_INFO_EOR() { 0, false, static_cast<ePHASE_SHIFT>(0), NULL }

    static const nsPacket::PacketHandlerNode handlers_static_[] =
    {
        //
        HANDLER_NODE_INFO(WZ_GUILD, WZ_GUILD_RESTORE_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO(WZ_GUILD, WZ_GUILD_MEMBER_INFO_REQ_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO(WZ_GUILD, WZ_GUILD_CHANGE_NOTICE_SYN, PHASE_UNLIMITED),
        //_NA_0_20100527_ADD_CHAR_RELATION_MEMO
        HANDLER_NODE_INFO(WZ_GUILD, WZ_GUILD_CHECK_CONDITION_ACCESS_DETAIL_INFO_SYN, PHASE_UNLIMITED),
        //
        HANDLER_NODE_INFO_EOR() // end of records
    };
    BOOST_STATIC_ASSERT(_countof(handlers_static_) >= 1);
    nsPacket::PacketHandlerInfo return_value = { handlers_static_, _countof(handlers_static_) - 1 };
    //
    return return_value;
};

//==================================================================================================

Handler_Guild_WorldToGuild_IMPL( WZ_GUILD_RESTORE_SYN )
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
	MSG_WZ_GUILD_RESTORE_SYN* pRecvMsg = (MSG_WZ_GUILD_RESTORE_SYN*)pMsg;

	//1. 길드가 존재하는지 체크
	Guild* pGuild = g_GuildManager.FindGuild( pRecvMsg->m_GuildGuid );
	if( !pGuild )
	{
		//만일 길드 서버에 정보가 없으면, 이상한 유저이거나 Agent서버로 로그인 하지 않은 유저이다.
		MSG_WZ_GUILD_RESTORE_NAK	nakMsg;
		nakMsg.m_byErrorCode = RC::RC_GUILD_NOT_EXIST_GUILD;
		pServerSession->SendPacket( &nakMsg, sizeof(nakMsg) );
		return;
	}

	//2. 길드는 존재하는데, 해당 멤버가 길드멤버인지 체크
	GuildMember* pMember = pGuild->FindMember( pRecvMsg->m_CharGuid );
    if (pMember == NULL) // CHANGES: f110523.6L, fixed a missing condition check, old problem...
	{
		MSG_WZ_GUILD_RESTORE_NAK	nakMsg;
		nakMsg.m_byErrorCode = RC::RC_GUILD_NOT_GUILD_MEMBER;
		pServerSession->SendPacket( &nakMsg, sizeof(nakMsg) );
		return;
	}

	MSG_WZ_GUILD_RESTORE_ACK akcMsg;
	g_GuildManager.SerializeBaseGuildInfo( pRecvMsg->m_GuildGuid, akcMsg.m_WorldGuildInfo, SERIALIZE_LOAD );
	g_GuildManager.GetGuildBaseMemberInfo( pRecvMsg->m_GuildGuid, pRecvMsg->m_CharGuid, akcMsg.m_BaseMemberInfo );
	pServerSession->SendPacket( &akcMsg, sizeof(akcMsg) );
}

Handler_Guild_WorldToGuild_IMPL( WZ_GUILD_MEMBER_INFO_REQ_SYN )
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
	MSG_WZ_GUILD_MEMBER_INFO_REQ_SYN* pRecvMsg = (MSG_WZ_GUILD_MEMBER_INFO_REQ_SYN*)pMsg;

	//1. 길드가 존재하는지 체크
	Guild* pGuild = g_GuildManager.FindGuild( pRecvMsg->m_GuildGuid );
	if( !pGuild )
	{
		SUNLOG( eCRITICAL_LOG, "[Handler_Guild_WorldToGuild_IMPL( WZ_GUILD_MEMBER_INFO_REQ_SYN )] Guild Not Exist, GuildGuid = %u", pRecvMsg->m_GuildGuid );
		return;
	}

	//2. 길드는 존재하는데, 해당 멤버가 길드멤버가 아니면..
	GuildMember* pMember = pGuild->FindMember( pRecvMsg->m_CharGuid );
	if( !pMember )
	{
		SUNLOG( eCRITICAL_LOG, "[Handler_Guild_WorldToGuild_IMPL( WZ_GUILD_MEMBER_INFO_REQ_SYN )] Not GuildMember, GuildGuid = %u, CharGuid = %u", pRecvMsg->m_GuildGuid, pRecvMsg->m_CharGuid );
		return;
	}

	MSG_WZ_GUILD_MEMBER_INFO_REQ_ACK akcMsg;
	g_GuildManager.GetGuildBaseMemberInfo( pRecvMsg->m_GuildGuid, pRecvMsg->m_CharGuid, akcMsg.m_BaseMemberInfo );
	pServerSession->SendPacket( &akcMsg, sizeof(akcMsg) );
}

Handler_Guild_WorldToGuild_IMPL( WZ_GUILD_CHANGE_NOTICE_SYN )
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
	PACKET_PTR_CONVERT(MSG_WZ_GUILD_CHANGE_NOTICE_SYN, pRecvMsg, pMsg);
		
	if(!g_GuildManager.CheckRight( pRecvMsg->m_GuildGuid, pRecvMsg->m_OperCharGuid, eGUILD_RIGHTS_NOTICE))
	{
		MSG_WZ_GUILD_CHANGE_NOTICE_NAK msg;
		msg.m_OperCharGuid = pRecvMsg->m_OperCharGuid;
		msg.m_byErrorCode = RC::RC_GUILD_NOTICE_CHNAGE_NONE_RIGHT;
		pServerSession->SendPacket(&msg, sizeof(msg));
		return;
	}

	MSG_DZ_GUILD_CHANGE_NOTICE_SYN msg;
    msg.m_dwKey = 0;          // (WAVERIX) (090903) (CHANGES)
    msg.m_dwSessionIndex = 0; // (WAVERIX) (090903) (CHANGES)
	msg.m_eNoticeType = (eGUILD_NOTICE)0;
	msg.m_GuildGuid = pRecvMsg->m_GuildGuid;
	msg.m_OperCharGuid = pRecvMsg->m_OperCharGuid;
	msg.m_byNoticeLen= pRecvMsg->m_wNoticeLen;
	strncpy( msg.m_tszGuildNotice, pRecvMsg->m_szGuildNotice, MAX_GUILDNOTICE_LENGTH );
	msg.m_tszGuildNotice[MAX_GUILDNOTICE_LENGTH] = '\0';
    Handler_Guild_GuildToDbp::OnDZ_GUILD_CHANGE_NOTICE_SYN(
        g_DBHandler.GetSharedSession(), &msg, msg.GetSize());
	
}

// 길드원 인지 체크
//_NA_0_20100527_ADD_CHAR_RELATION_MEMO
Handler_Guild_WorldToGuild_IMPL( WZ_GUILD_CHECK_CONDITION_ACCESS_DETAIL_INFO_SYN )
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
    PACKET_PTR_CONVERT(MSG_WZ_GUILD_CHECK_CONDITION_ACCESS_DETAIL_INFO_SYN, pRecvMsg, pMsg);

    Guild* pGuild = g_GuildManager.FindGuild(pRecvMsg->m_guildGuid);
    if (pGuild)
    {
        GuildMember* pMember = pGuild->FindMember(pRecvMsg->m_dwToCharGuid);
        if (pMember)
        {
            MSG_WZ_GUILD_CHECK_CONDITION_ACCESS_DETAIL_INFO_ACK msg;
            msg.m_dwKey = pRecvMsg->m_dwKey;
            msg.m_dwToCharGuid = pRecvMsg->m_dwToCharGuid;
            pServerSession->SendPacket(&msg, sizeof(msg));
            return;
        }
    }

    MSG_WZ_GUILD_CHECK_CONDITION_ACCESS_DETAIL_INFO_NAK msg;
    msg.m_byErrorCode = RC::RC_FRIEND_NOTGUILDMEMBER;
    pServerSession->SendPacket(&msg, sizeof(msg));
}
