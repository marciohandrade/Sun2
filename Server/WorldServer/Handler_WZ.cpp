#include "StdAfx.h"
#include ".\handler_WZ.h"
#include "WorldServer.h"
#include "GuildServerSession.h"
#include "ServerSessionManager.h"
#include <PacketStruct_CW.h>
#include <PacketStruct_WZ.h>
#include "WorldGuildManager.h"
#include "WorldGuild.h"
#include "WorldGuildMember.h"
#include "UserManager.h"
#include "User.h"
#include <PacketStruct_AW.h>
#include <PacketStruct_DW.h>    //_NA_0_20100527_ADD_CHAR_RELATION_MEMO

Handler_WZ_IMPL( SERVERCOMMON_HEARTBEAT )
{
}

Handler_WZ_IMPL( WZ_GUILD_DESTROY_GUILD_CMD )
{
	MSG_WZ_GUILD_DESTROY_GUILD_CMD* pRecvMsg = (MSG_WZ_GUILD_DESTROY_GUILD_CMD*)pMsg;

	WorldGuildManager::Instance()->DestroyGuild( pRecvMsg->m_GuildGuid );
}

Handler_WZ_IMPL( WZ_GUILD_JOIN_GUILD_CMD )
{
	MSG_WZ_GUILD_JOIN_GUILD_CMD* pRecvMsg = (MSG_WZ_GUILD_JOIN_GUILD_CMD*)pMsg;

	// 길드 정보 있는지 검사하여 없으면 복구 요청
	WorldGuild* pGuild = WorldGuildManager::Instance()->FindGuild( pRecvMsg->m_GuildGuid );
	if( !pGuild )
	{
		SUNLOG( eCRITICAL_LOG, "GUILD_JOIN Error - No Guild[GuildGuid:%u]", pRecvMsg->m_GuildGuid );
		return;
	}

	//1. User 길드 세팅.
	User* pUser = UserManager::Instance()->FindUserByCharGuid( pRecvMsg->m_Info.m_CharGuid );
	if( !pUser )
	{
		SUNLOG( eCRITICAL_LOG, "[Handler_WZ_IMPL( WZ_GUILD_JOIN_GUILD_CMD )] User Not Found, CharGuid  =%u", pRecvMsg->m_Info.m_CharGuid );
		return;
	}

	pUser->SetGuildGuid( pRecvMsg->m_GuildGuid );

	WorldGuildManager::Instance()->JoinGuild( pRecvMsg->m_GuildGuid, pRecvMsg->m_Info );

	// 새로 가입했으면 길드 공지를 날려준다.
	MSG_CW_GUILD_NOTICE_CMD	noticeMsg;
	noticeMsg.m_wNoticeLen = strlen(pGuild->GetGuildNotice());
	//memset(noticeMsg.m_ptszNotice, 0, sizeof(TCHAR)*(MAX_GUILDNOTICE_LENGTH+1));
	strncpy( noticeMsg.m_ptszNotice, pGuild->GetGuildNotice(), MAX_GUILDNOTICE_LENGTH );
	//noticeMsg.m_ptszNotice[MAX_GUILDNOTICE_LENGTH+1] = '\0';

	//해당 길드원한테만 보내도록 하자
	pGuild->SendAllMember( (BYTE*)&noticeMsg, noticeMsg.GetSize() ); 
}

Handler_WZ_IMPL( WZ_GUILD_WITHDRAW_GUILD_CMD )
{
	MSG_WZ_GUILD_WITHDRAW_GUILD_CMD* pRecvMsg = (MSG_WZ_GUILD_WITHDRAW_GUILD_CMD*)pMsg;

	// 이미 게임서버에서 탈퇴한것을 알려줬다.
	// 따라서 길드 멤버 구조만 유지한다.
	WorldGuildManager::Instance()->WithdrawGuild( pRecvMsg->m_GuildGuid, pRecvMsg->m_CharGuid );

	User* pUser = UserManager::Instance()->FindUserByCharGuid( pRecvMsg->m_CharGuid );
	if( pUser )
		pUser->SetGuildGuid( 0 );
}


// 길드 공지
Handler_WZ_IMPL( WZ_GUILD_NOTICE_CMD )
{
	MSG_WZ_GUILD_NOTICE_CMD* pRecvMsg = (MSG_WZ_GUILD_NOTICE_CMD*)pMsg;
}

#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP

	Handler_WZ_IMPL( WZ_GUILD_CHUNTING_PVP_RESULT_CMD )
	{
		MSG_WZ_GUILD_CHUNTING_PVP_RESULT_CMD *pRecvMsg = (MSG_WZ_GUILD_CHUNTING_PVP_RESULT_CMD*)pMsg;

		WorldGuildManager::Instance()->SetChuntingPVPInfo( pRecvMsg->m_GuildGuid, pRecvMsg->m_PVPInfo );
	}

#endif //__NA_000000_20070528_TAIWAN_GUILD_PVP

//길드 직책
Handler_WZ_IMPL( WZ_GUILD_CHANGE_DUTY_CMD )
{
	MSG_WZ_GUILD_CHANGE_DUTY_CMD *pRecvMsg = (MSG_WZ_GUILD_CHANGE_DUTY_CMD *)pMsg;

	if( !WorldGuildManager::Instance()->ChangeMemberDuty( pRecvMsg->m_GuildGuid, pRecvMsg->m_TargetCharGuid, pRecvMsg->m_eGuildDuty ) )
	{
		SUNLOG( eCRITICAL_LOG, "[Handler_WZ::OnWZ_GUILD_CHANGE_DUTY_CMD] DB에서는 성공인데, 월드서버에서 실패다.. 문제다. GUILDGUID= %u, CHARGUID = %u, DUTY =%d",
			pRecvMsg->m_GuildGuid, pRecvMsg->m_TargetCharGuid, pRecvMsg->m_eGuildDuty );
	}
}

Handler_WZ_IMPL( WZ_GUILD_CHANGE_MASTER_CMD )
{
	MSG_WZ_GUILD_CHANGE_MASTER_CMD* pRecvMsg = (MSG_WZ_GUILD_CHANGE_MASTER_CMD*)pMsg;

	//길드장이 변경되었으므로, 직책이 바뀐거하고 똑같이 처리한다.

	//1. 이전길드장을 정예병으로 바꾼다.
	WorldGuildManager::Instance()->ChangeMemberDuty( pRecvMsg->m_GuildGuid, pRecvMsg->m_OldMasterGuid, eGUILD_DUTY_REGULAR_SOLDIER );
	//2. 새로운 길드장을 길마로 바꾼다.
	WorldGuildManager::Instance()->ChangeMemberDuty( pRecvMsg->m_GuildGuid, pRecvMsg->m_NewMasterGuid, eGUILD_DUTY_MASTER );
}

Handler_WZ_IMPL( WZ_GUILD_CHANGE_COMMANDER_CMD )
{
	MSG_WZ_GUILD_CHANGE_COMMANDER_CMD* pRecvMsg = (MSG_WZ_GUILD_CHANGE_COMMANDER_CMD*)pMsg;

	//군단장이 변경되었으면, 직책이 변경된거다.
	WorldGuildManager::Instance()->ChangeCommander( pRecvMsg->m_GuildGuid, pRecvMsg->m_TargetCharGuid, pRecvMsg->m_byCorpsNum );
}


Handler_WZ_IMPL( WZ_GUILD_CHANGE_CORPS_CMD )
{
	MSG_WZ_GUILD_CHANGE_CORPS_CMD* pRecvMsg = (MSG_WZ_GUILD_CHANGE_CORPS_CMD*)pMsg;

	//군단변경.
	WorldGuildManager::Instance()->ChangeCorps( pRecvMsg->m_GuildGuid, pRecvMsg->m_TargetCharGuid, pRecvMsg->m_byCorpsNum );
}


Handler_WZ_IMPL( WZ_GUILD_CREATE_REAR_CAMP_CMD )
{
	MSG_WZ_GUILD_CREATE_REAR_CAMP_CMD* pRecvMsg = (MSG_WZ_GUILD_CREATE_REAR_CAMP_CMD*)pMsg;

	WorldGuildManager::Instance()->CreateRearCamp( pRecvMsg->m_GuildGuid, pRecvMsg->m_byCorpsNum );
}


Handler_WZ_IMPL( WZ_GUILD_DESTROY_REAR_CAMP_CMD )
{
	MSG_WZ_GUILD_DESTROY_REAR_CAMP_CMD* pRecvMsg = (MSG_WZ_GUILD_DESTROY_REAR_CAMP_CMD*)pMsg;

	WorldGuildManager::Instance()->DestroyRearCamp( pRecvMsg->m_GuildGuid, pRecvMsg->m_byCorpsNum );

}

Handler_WZ_IMPL( WZ_GUILD_CREATE_CMD )
{
	MSG_WZ_GUILD_CREATE_CMD* pRecvMsg = (MSG_WZ_GUILD_CREATE_CMD*)pMsg;

	//길드생성.
	WorldGuildManager::Instance()->CreateGuild( pRecvMsg->m_WorldGuildInfo, pRecvMsg->m_BaseMemberInfo );

	// 이부분을 함수로 빼고 싶지만, UserManager와 WorldGuildManager를 둘다 참조한다. 
	// 될 수 있으면 전역으로 쓰이는 것은 서로 연관시키지 말자. 

	// 길마가 접속해 있으면 길마에 길드정보 세팅하자.
	User*	pUser = UserManager::Instance()->FindUserByCharGuid( pRecvMsg->m_BaseMemberInfo.m_CharGuid );
	if( !pUser )
	{
		SUNLOG( eMIDDLE_LOG, "WZ_GUILD_INFO Error - No Online Member[CharGuid:%u]!!", pRecvMsg->m_BaseMemberInfo.m_CharGuid );
		return;
	}

	pUser->SetGuildGuid( pRecvMsg->m_WorldGuildInfo.m_GuildGuid );	// 세~팅~!!!
}

Handler_WZ_IMPL( WZ_GUILD_RESTORE_ACK )
{
	MSG_WZ_GUILD_RESTORE_ACK* pRecvMsg = (MSG_WZ_GUILD_RESTORE_ACK*)pMsg;

	//내부적으로 월드서버에 세팅한다.
	WorldGuildManager::Instance()->GuildRestore( pRecvMsg->m_WorldGuildInfo, pRecvMsg->m_BaseMemberInfo );
}

Handler_WZ_IMPL( WZ_GUILD_LOGIN_CHAR_CMD )
{
	MSG_WZ_GUILD_LOGIN_CHAR_CMD* pRecvMsg = (MSG_WZ_GUILD_LOGIN_CHAR_CMD*)pMsg;

	//내부적으로 월드서버에 세팅한다.
	WorldGuildManager::Instance()->LoginGuild( pRecvMsg->m_WorldGuildInfo, pRecvMsg->m_BaseMemberInfo );

	// 길드 공지 보내줘야 한다.
	MSG_CW_GUILD_CHANGE_NOTICE_BRD msg;
	msg.m_wNoticeLen = _tcslen( pRecvMsg->m_WorldGuildInfo.m_szGuildNotice );
	if( msg.m_wNoticeLen >= MAX_GUILDNOTICE_LENGTH )
	{
		SUNLOG( eCRITICAL_LOG, "[Handler_WZ::OnWZ_GUILD_LOGIN_CHAR_CMD] 길드공지가 최고 길이를 넘어섰다. Notice Len = %d", msg.m_wNoticeLen );
		return;
	}
	strncpy( msg.m_ptszNotice, pRecvMsg->m_WorldGuildInfo.m_szGuildNotice, MAX_GUILDNOTICE_LENGTH);
	msg.m_ptszNotice[MAX_GUILDNOTICE_LENGTH] = '\0';
	User* pUser = UserManager::Instance()->FindUserByCharGuid( pRecvMsg->m_BaseMemberInfo.m_CharGuid );
	if( pUser )
	{
		pUser->SendPacket( (BYTE*)&msg, msg.GetSize());
	}
}

Handler_WZ_IMPL( WZ_GUILD_LOGOUT_CHAR_CMD )
{
	MSG_WZ_GUILD_LOGOUT_CHAR_CMD* pRecvMsg = (MSG_WZ_GUILD_LOGOUT_CHAR_CMD*)pMsg;

	// 내부적으로 월드서버에 세팅한다.
	WorldGuildManager::Instance()->LogoutGuild( pRecvMsg->m_GuildGuid, pRecvMsg->m_CharGuid );
}

Handler_WZ_IMPL( WZ_GUILD_KICK_PLAYER_CMD )
{
	MSG_WZ_GUILD_KICK_PLAYER_CMD* pRecvMsg = (MSG_WZ_GUILD_KICK_PLAYER_CMD*)pMsg;
	// 강퇴시킨다.
	WorldGuildManager::Instance()->KickMember( pRecvMsg->m_GuildGuid, pRecvMsg->m_KickCharGuid );

	User* pUser = UserManager::Instance()->FindUserByCharGuid( pRecvMsg->m_KickCharGuid );
	if( pUser )
		pUser->SetGuildGuid( 0 );
}

Handler_WZ_IMPL( WZ_GUILD_MEMBER_INFO_REQ_ACK )
{
	MSG_WZ_GUILD_MEMBER_INFO_REQ_ACK* pRecvMsg = (MSG_WZ_GUILD_MEMBER_INFO_REQ_ACK*)pMsg;

	WorldGuildManager::Instance()->SerializeBaseMemberInfo( pRecvMsg->m_GuildGuid, pRecvMsg->m_BaseMemberInfo, SERIALIZE_STORE );
}

Handler_WZ_IMPL( WZ_GUILD_RECOVERY_GUILD_CMD )
{
	MSG_WZ_GUILD_RECOVERY_GUILD_CMD *pRecvMsg = 
		static_cast<MSG_WZ_GUILD_RECOVERY_GUILD_CMD*>(pMsg);
	
	WorldGuild *pGuild = WorldGuildManager::Instance()->FindGuild(pRecvMsg->m_GuildGuid);
	if(pGuild)
	{	
		MSG_CW_GUILD_RECOVERY_GUILD_BRD msg;
		msg.m_GuildIndex = pRecvMsg->m_GuildGuid;
		pGuild->SendAllMember((BYTE*)&msg, sizeof(msg));
	}
}

Handler_WZ_IMPL( WZ_GUILD_DESTROY_WAIT_CMD )
{
	MSG_WZ_GUILD_DESTROY_WAIT_CMD*pRecvMsg = 
		static_cast<MSG_WZ_GUILD_DESTROY_WAIT_CMD*>(pMsg);

	WorldGuild *pGuild = WorldGuildManager::Instance()->FindGuild(pRecvMsg->m_GuildGuid);
	if(pGuild)
	{
		//MSG_CW_GUILD_DESTROY_WAIT_CMD msg;
		MSG_CW_GUILD_RECOVERY_GUILD_BRD msg;
		msg.m_GuildIndex = pRecvMsg->m_GuildGuid;
		pGuild->SendAllMember((BYTE*)&msg, sizeof(msg));
	}
}

Handler_WZ_IMPL( WZ_GUILD_GUILDMARK_REGISTER_CMD )
{
	MSG_WZ_GUILD_GUILDMARK_REGISTER_CMD *pRecvMsg = 
		static_cast<MSG_WZ_GUILD_GUILDMARK_REGISTER_CMD*>(pMsg);

	WorldGuild *pGuild = WorldGuildManager::Instance()->FindGuild(pRecvMsg->m_GuildGuid);

	if(pGuild)
	{	
		MSG_CW_GUILD_GUILDMARK_REGISTER_BRD msg;
		msg.m_GuildIndex= pRecvMsg->m_GuildGuid;
		msg.m_PatternIndex = pRecvMsg->m_PatternIndex;
		msg.m_BackGroundIndex = pRecvMsg->m_BackGroundIndex;
		pGuild->SendAllMember((BYTE*)&msg, sizeof(msg));
	}
}

Handler_WZ_IMPL( WZ_GUILD_GUILDMARK_DELETE_CMD )
{
	MSG_WZ_GUILD_GUILDMARK_DELETE_CMD *pRecvMsg = 
		static_cast<MSG_WZ_GUILD_GUILDMARK_DELETE_CMD*>(pMsg);

	WorldGuild *pGuild = WorldGuildManager::Instance()->FindGuild(pRecvMsg->m_GuildGuid);

	if(pGuild)
	{	
		MSG_CW_GUILD_GUILDMARK_DELETE_BRD msg;
		msg.m_GuildIndex= pRecvMsg->m_GuildGuid;
		msg.m_PatternIndex = pRecvMsg->m_PatternIndex;
		msg.m_BackGroundIndex = pRecvMsg->m_BackGroundIndex;
		pGuild->SendAllMember((BYTE*)&msg, sizeof(msg));
	}
}


Handler_WZ_IMPL( WZ_GUILD_CHANGE_NOTICE_CMD )
{
	PACKET_PTR_CONVERT(MSG_WZ_GUILD_CHANGE_NOTICE_CMD, pRecvMsg, pMsg);
	WorldGuild* pGuild = WorldGuildManager::Instance()->FindGuild(pRecvMsg->m_GuildGuid);
	if(pGuild)
	{
		pGuild->SetGuildNotice(pRecvMsg->m_szGuildNotice);

		MSG_CW_GUILD_CHANGE_NOTICE_BRD msg;
		msg.m_wNoticeLen= pRecvMsg->m_wNoticeLen;
		strncpy(msg.m_ptszNotice, pRecvMsg->m_szGuildNotice, MAX_GUILDNOTICE_LENGTH);
		msg.m_ptszNotice[MAX_GUILDNOTICE_LENGTH] = '\0';

		pGuild->SendAllMember((BYTE*)&msg, sizeof(msg));
	}
}

Handler_WZ_IMPL( WZ_GUILD_CHANGE_NOTICE_NAK )
{
	PACKET_PTR_CONVERT(MSG_WZ_GUILD_CHANGE_NOTICE_NAK, pRecvMsg, pMsg);
	User* pUser = UserManager::Instance()->FindUserByCharGuid(pRecvMsg->m_OperCharGuid);
	if(pUser)
	{
		MSG_CW_GUILD_CHANGE_NOTICE_NAK msg;
		msg.m_byErrorCode = pRecvMsg->m_byErrorCode;
		pUser->SendPacket((BYTE*)&msg, sizeof(msg));
	}
}

Handler_WZ_IMPL( WZ_GUILD_NAME_CHANGE_CMD )
{
	MSG_WZ_GUILD_NAME_CHANGE_CMD *pRecvMsg = 
		static_cast<MSG_WZ_GUILD_NAME_CHANGE_CMD*>(pMsg);

	WorldGuild *pGuild = WorldGuildManager::Instance()->FindGuild(pRecvMsg->m_GuildGuid);

	if(pGuild)
		pGuild->ChangeGuildName(pRecvMsg->m_tszGuildName);
}

Handler_WZ_IMPL(WZ_GUILD_RENAME_CHAR_CMD)
{
    const MSG_WZ_GUILD_RENAME_CHAR_CMD* recv_msg = \
        static_cast<MSG_WZ_GUILD_RENAME_CHAR_CMD*>(pMsg);

    WorldGuild *pGuild = WorldGuildManager::Instance()->FindGuild(recv_msg->guildguid);
    if(!pGuild)
        return;
    
    WorldGuildMember* guild_member = pGuild->FindMember(recv_msg->charguid);
    if(!guild_member)
        return;

    guild_member->SetCharName(recv_msg->character_name_);
}

Handler_WZ_IMPL( WZ_GUILD_DOMINATION_RETURNHIME_CMD )
{
   const MSG_WZ_GUILD_DOMINATION_RETURNHIME_CMD* recv_msg = 
       static_cast<MSG_WZ_GUILD_DOMINATION_RETURNHIME_CMD*>(pMsg);

   WorldGuildManager::Instance()->SendDominationHeimReturnMemo(recv_msg);
}

//_NA_0_20100527_ADD_CHAR_RELATION_MEMO
Handler_WZ_IMPL( WZ_GUILD_CHECK_CONDITION_ACCESS_DETAIL_INFO_ACK )
{
    const MSG_WZ_GUILD_CHECK_CONDITION_ACCESS_DETAIL_INFO_ACK *pRecvMsg = 
        static_cast<MSG_WZ_GUILD_CHECK_CONDITION_ACCESS_DETAIL_INFO_ACK*>(pMsg);

    User* pUser = UserManager::Instance()->FindUserByCharGuid(pRecvMsg->m_dwKey);
    if(!pUser)
        return;

    MSG_DW_FRIEND_DETAIL_INFO_SYN msg;
    msg.m_dwKey = pRecvMsg->m_dwKey;
    msg.m_dwToCharGuid = pRecvMsg->m_dwToCharGuid;

    ServerSession* pGameDBProxy = ServerSessionManager::Instance()->GetGameDBProxy();
    if (pGameDBProxy == NULL)
    {        
        MSG_CW_FRIEND_STATUS_NAK nak_msg;
        nak_msg.m_byErrorCode = RC::RC_FRIEND_FAIL;
        pUser->SendPacket((BYTE*)&nak_msg, sizeof(nak_msg));
        return;
    }
    pGameDBProxy->Send((BYTE*)&msg, sizeof(msg));
}

Handler_WZ_IMPL( WZ_GUILD_CHECK_CONDITION_ACCESS_DETAIL_INFO_NAK )
{
    const MSG_WZ_GUILD_CHECK_CONDITION_ACCESS_DETAIL_INFO_NAK *pRecvMsg = 
        static_cast<MSG_WZ_GUILD_CHECK_CONDITION_ACCESS_DETAIL_INFO_NAK*>(pMsg);

    User* pUser = UserManager::Instance()->FindUserByCharGuid(pRecvMsg->m_dwKey);
    if(!pUser)
        return;

    MSG_CW_FRIEND_STATUS_NAK nak_msg;
    nak_msg.m_byErrorCode = pRecvMsg->m_byErrorCode;
    pUser->SendPacket((BYTE*)&nak_msg, sizeof(nak_msg));
}
