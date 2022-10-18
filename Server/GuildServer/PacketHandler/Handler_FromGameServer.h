
#pragma once

#include <PacketStruct_WZ.h>
#include "PacketHandler.h"

// <PROTOTYPEs>
class GameServerSession;

//-------------------------------------------------------------------------------------------------
#define Handler_FromGameServer_DECL( p )	static VOID On##p( GameServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize )
#define Handler_FromGameServer_IMPL( p )	VOID Handler_FromGameServer::On##p( GameServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize )
//-------------------------------------------------------------------------------------------------

class Handler_FromGameServer
{
public:
	static VOID		RegisterHandler_ALL( PACKET_HANDLER_DELEGATOR pInstance );
private:
	static sPACKET_HANDLER_INFO*	GetHandlerInfo();
private:

	Handler_FromGameServer_DECL(SERVERCOMMON_HEARTBEAT){}

	Handler_FromGameServer_DECL(GZ_GUILD_CREATE_SYN);
	Handler_FromGameServer_DECL(GZ_GUILD_DESTROY_SYN);
	Handler_FromGameServer_DECL(GZ_GUILD_JOIN_SYN);
	Handler_FromGameServer_DECL(GZ_GUILD_WITHDRAW_SYN);
	Handler_FromGameServer_DECL(GZ_GUILD_KICKPLAYER_SYN);
	Handler_FromGameServer_DECL(GZ_GUILD_DONATE_UP_SYN);
	Handler_FromGameServer_DECL(GZ_GUILD_CONVERT_UP2GP_SYN);
	Handler_FromGameServer_DECL(GZ_GUILD_RANKUP_SYN);

#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
	Handler_FromGameServer_DECL(GZ_GUILD_CHUNTING_PVP_RESULT_CMD);
#endif

	Handler_FromGameServer_DECL(GZ_GUILD_CHANGE_DUTY_SYN);					//길드 직책 설정	
	Handler_FromGameServer_DECL(GZ_GUILD_RIGHT_DELEGATION_LIST_SYN);		//길드 위임 권한 목록 요청	
	Handler_FromGameServer_DECL(GZ_GUILD_RIGHT_DELEGATION_SYN);				//길드 권한 위임
	Handler_FromGameServer_DECL(GZ_GUILD_CHANGE_MASTER_SYN);				//길드장 변경

	Handler_FromGameServer_DECL(GZ_GUILD_CREATE_CORPS_SYN);					//길드 군단 생성
	Handler_FromGameServer_DECL(GZ_GUILD_DESTROY_CORPS_SYN);				//길드 군단 해산
	Handler_FromGameServer_DECL(GZ_GUILD_EMPTY_COMMANDER_INFO_SYN);			//길드 빈 군단장 정보 요청.
	Handler_FromGameServer_DECL(GZ_GUILD_CHANGE_COMMANDER_SYN);				//길드 군단장 변경.
	Handler_FromGameServer_DECL(GZ_GUILD_CHANGE_CORPS_SYN);					//길드 군단 변경.
	Handler_FromGameServer_DECL(GZ_GUILD_CREATE_REAR_CAMP_SYN);				//길드 육성캠프 생성.
	Handler_FromGameServer_DECL(GZ_GUILD_DESTROY_REAR_CAMP_SYN);			//길드 육성캠프 해산.
	Handler_FromGameServer_DECL(GZ_GUILD_INVITE_SYN);						//길드 초대.
	Handler_FromGameServer_DECL(GZ_GUILD_LOGIN_CHAR_SYN);					//길드 멤버 로그인.
	Handler_FromGameServer_DECL(GZ_GUILD_LOGOUT_CHAR_SYN);					//길드 멤버 로그아웃.
	Handler_FromGameServer_DECL(GZ_GUILD_CHAR_LEVELUP_SYN);					//길드원 레벨업.
	Handler_FromGameServer_DECL(GZ_GUILD_TOTALINFO_REQ_SYN );
	Handler_FromGameServer_DECL(GZ_GUILD_GM_GUILD_POINT_SYN );				//GM명령어 길드포인트 세팅
	Handler_FromGameServer_DECL(GZ_GUILD_CHANGE_CHAOSTATE_SYN );			
	Handler_FromGameServer_DECL(GZ_GUILD_ENTER_SERVER_SYN );				//길드원이 게임서버간을 이동할 때.
	Handler_FromGameServer_DECL(GZ_GUILD_PRE_LOGIN_CMD );					//길드원 로그인 사전 준비.

	Handler_FromGameServer_DECL(GZ_GUILD_RELATION_SYN);				//길드 관계
	Handler_FromGameServer_DECL(GZ_GUILD_RELATION_DELETE_SYN);
	Handler_FromGameServer_DECL(GZ_GUILD_RELATION_BOTH_AGAINST_SYN);
	Handler_FromGameServer_DECL(GZ_GUILD_RELATION_ALLIENCE_SYN);

    Handler_FromGameServer_DECL(GZ_GUILD_RELATION_INFO_SYN);
	Handler_FromGameServer_DECL(GZ_GUILD_RECOVERY_GUILD_SYN);
	Handler_FromGameServer_DECL(GZ_GUILD_GUILDMARK_REGISTER_SYN);
	Handler_FromGameServer_DECL(GZ_GUILD_GUILDMARK_DELETE_SYN);

    Handler_FromGameServer_DECL(GZ_GUILD_WAREHOUSE_OPEN_SYN);
	Handler_FromGameServer_DECL(GZ_GUILD_WAREHOUSE_CLOSE_SYN);

	Handler_FromGameServer_DECL(GZ_GUILD_NAME_CHANGE_SYN);
    //{ __KR_001385_20090907_DOMINATION_PACKET
    Handler_FromGameServer_DECL(GZ_DOMINATION_INFO_SYN);
    Handler_FromGameServer_DECL(GZ_DOMINATION_AUCTION_INFO_SYN);
    Handler_FromGameServer_DECL(GZ_DOMINATION_UPDATE_INFO_SYN);
    Handler_FromGameServer_DECL(GZ_DOMINATION_ACCEP_HEIM_SYN);
    Handler_FromGameServer_DECL(GZ_DOMINATION_INIT_ACTIONINFO_SYN);
    Handler_FromGameServer_DECL(GZ_DOMINATION_RETURNHIME_SYN);
    Handler_FromGameServer_DECL(GZ_DOMINATION_BATTING_HEIM_SYN);
    //}
#ifdef _NA_20100604_BETTER_DOMINATION_MANAGER
    Handler_FromGameServer_DECL(GZ_DOMINATION_UPDATE_GUILDINFO_SYN);
#endif
    //_NA_20100913_GUILD_WAREHOUSE_GM_COMMAND
    Handler_FromGameServer_DECL(GZ_GUILD_GM_WAREHOUSE_FORCE_CLOSE_CMD);
#ifdef _NA_20110216_INCREASE_DOMINATION_DEFFENSEWALL_POINT
    Handler_FromGameServer_DECL(GZ_DOMINATION_ADD_DEFFENSEWALL_POINT_SYN);
#endif

#ifdef _NA_20110406_CHANGE_RULE_FOR_DOMINATION_JOIN
    Handler_FromGameServer_DECL(GZ_DOMINATION_MEMBER_JOIN_SYN);
    Handler_FromGameServer_DECL(GZ_DOMINATION_MEMBER_LEAVE_SYN);
    Handler_FromGameServer_DECL(GZ_DOMINATION_MEMBERLIST_REQ);
    Handler_FromGameServer_DECL(GZ_DOMINATION_MEMBERLIST_INIT_CMD);
#endif
};