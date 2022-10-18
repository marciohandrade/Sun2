#pragma once

struct MSG_BASE;
class GuildServerSession;
class GameDBProxySession; // NOTE: f110525.3L

/************************************************************************/
/* 월드서버에 길드의 역활이란 2가지 이다.                               */
/* 1 : 채팅 ( 길드, 군단, 육성캠프 )									*/
/* 2 : 메세지 or 공지 전달												*/
/************************************************************************/


//---------------------------------------------------------------------------------------------------------------------//
#ifdef _NA002635_GZ_DZ_PROTOCOL_UINIFICATION
  #define Handler_WZ_DECL(p) static void On##p(GameDBProxySession* pServerSession, MSG_BASE* pMsg, WORD wSize)
  #define Handler_WZ_IMPL(p) void Handler_WZ::On##p(GameDBProxySession* pServerSession, MSG_BASE* pMsg, WORD wSize)
#else
#define Handler_WZ_DECL( p )		static VOID On##p( GuildServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize )
#define Handler_WZ_IMPL( p )		VOID	Handler_WZ::On##p( GuildServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize )
#endif
//---------------------------------------------------------------------------------------------------------------------//

class Handler_WZ
{
public:

	Handler_WZ_DECL( SERVERCOMMON_HEARTBEAT );

	Handler_WZ_DECL( WZ_GUILD_DESTROY_GUILD_CMD );
	Handler_WZ_DECL( WZ_GUILD_JOIN_GUILD_CMD );
	Handler_WZ_DECL( WZ_GUILD_WITHDRAW_GUILD_CMD );
	Handler_WZ_DECL( WZ_GUILD_NOTICE_CMD );

#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
	Handler_WZ_DECL( WZ_GUILD_CHUNTING_PVP_RESULT_CMD );
#endif

	Handler_WZ_DECL( WZ_GUILD_CHANGE_DUTY_CMD );
	Handler_WZ_DECL( WZ_GUILD_CHANGE_MASTER_CMD );

    Handler_WZ_DECL( WZ_GUILD_CHANGE_COMMANDER_CMD );
	Handler_WZ_DECL( WZ_GUILD_CHANGE_CORPS_CMD );
	Handler_WZ_DECL( WZ_GUILD_CREATE_REAR_CAMP_CMD );
	Handler_WZ_DECL( WZ_GUILD_DESTROY_REAR_CAMP_CMD );
	Handler_WZ_DECL( WZ_GUILD_CREATE_CMD );
	Handler_WZ_DECL( WZ_GUILD_RESTORE_ACK );
	Handler_WZ_DECL( WZ_GUILD_LOGIN_CHAR_CMD );
	Handler_WZ_DECL( WZ_GUILD_LOGOUT_CHAR_CMD );
	Handler_WZ_DECL( WZ_GUILD_KICK_PLAYER_CMD );
	Handler_WZ_DECL( WZ_GUILD_MEMBER_INFO_REQ_ACK );

	Handler_WZ_DECL( WZ_GUILD_RECOVERY_GUILD_CMD );
	Handler_WZ_DECL( WZ_GUILD_DESTROY_WAIT_CMD );
	Handler_WZ_DECL( WZ_GUILD_GUILDMARK_REGISTER_CMD );
	Handler_WZ_DECL( WZ_GUILD_GUILDMARK_DELETE_CMD );
	Handler_WZ_DECL( WZ_GUILD_CHANGE_NOTICE_CMD );
	Handler_WZ_DECL( WZ_GUILD_CHANGE_NOTICE_NAK );

	Handler_WZ_DECL( WZ_GUILD_NAME_CHANGE_CMD );
    Handler_WZ_DECL( WZ_GUILD_RENAME_CHAR_CMD );

    Handler_WZ_DECL( WZ_GUILD_DOMINATION_RETURNHIME_CMD );
    //_NA_0_20100527_ADD_CHAR_RELATION_MEMO
    Handler_WZ_DECL( WZ_GUILD_CHECK_CONDITION_ACCESS_DETAIL_INFO_ACK );
    Handler_WZ_DECL( WZ_GUILD_CHECK_CONDITION_ACCESS_DETAIL_INFO_NAK );
};

