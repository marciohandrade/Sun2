#pragma once

#include <PacketStruct_DZ.h>
#include "PacketHandler.h"

// <PROTOTYPEs>
class DBProxyServerSession;

//-------------------------------------------------------------------------------------------------
#define Handler_FromDBProxyServer_DECL( p )	static VOID On##p( DBProxyServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize )
#define Handler_FromDBProxyServer_IMPL( p )	VOID Handler_FromDBProxyServer::On##p( DBProxyServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize )
//-------------------------------------------------------------------------------------------------

class Handler_FromDBProxyServer
{
public:
	static VOID		RegisterHandler_ALL( PACKET_HANDLER_DELEGATOR pInstance );
private:
	static sPACKET_HANDLER_INFO*	GetHandlerInfo();
private:


	Handler_FromDBProxyServer_DECL(DZ_GUILD_CREATE_ACK);
	Handler_FromDBProxyServer_DECL(DZ_GUILD_DESTROY_ACK);
	Handler_FromDBProxyServer_DECL(DZ_GUILD_JOIN_ACK);
	Handler_FromDBProxyServer_DECL(DZ_GUILD_WITHDRAW_ACK);
	Handler_FromDBProxyServer_DECL(DZ_GUILD_SELECT_ACK);

	Handler_FromDBProxyServer_DECL(DZ_GUILD_CREATE_NAK);
	Handler_FromDBProxyServer_DECL(DZ_GUILD_DESTROY_NAK);
	Handler_FromDBProxyServer_DECL(DZ_GUILD_JOIN_NAK);
	Handler_FromDBProxyServer_DECL(DZ_GUILD_WITHDRAW_NAK);
	Handler_FromDBProxyServer_DECL(DZ_GUILD_SELECT_NAK);
	Handler_FromDBProxyServer_DECL(DZ_GUILD_KICKPLAYER_ACK);
	Handler_FromDBProxyServer_DECL(DZ_GUILD_KICKPLAYER_NAK);
	Handler_FromDBProxyServer_DECL(DZ_GUILD_CHANGE_NOTICE_ACK);
	Handler_FromDBProxyServer_DECL(DZ_GUILD_CHANGE_NOTICE_NAK);

	Handler_FromDBProxyServer_DECL(DZ_GUILD_DONATE_UP_ACK);
	Handler_FromDBProxyServer_DECL(DZ_GUILD_DONATE_UP_NAK);
	Handler_FromDBProxyServer_DECL(DZ_GUILD_CONVERT_UP2GP_ACK);
	Handler_FromDBProxyServer_DECL(DZ_GUILD_CONVERT_UP2GP_NAK);
	Handler_FromDBProxyServer_DECL(DZ_GUILD_RANKUP_ACK);
	Handler_FromDBProxyServer_DECL(DZ_GUILD_RANKUP_NAK);
	
#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
	Handler_FromDBProxyServer_DECL(DZ_GUILD_CHUNTING_PVP_RESULT_ACK);
#endif
	Handler_FromDBProxyServer_DECL(DZ_GUILD_RELATION_INFO_SYN);
	Handler_FromDBProxyServer_DECL(DZ_GUILD_RELATION_INFO_ACK);
	Handler_FromDBProxyServer_DECL(DZ_GUILD_RELATION_INFO_NAK);

    Handler_FromDBProxyServer_DECL(DZ_GUILD_CHANGE_DUTY_ACK);		// ???? ????
	Handler_FromDBProxyServer_DECL(DZ_GUILD_CHANGE_DUTY_NAK);
	Handler_FromDBProxyServer_DECL(DZ_GUILD_RIGHT_DELEGATION_ACK);
	Handler_FromDBProxyServer_DECL(DZ_GUILD_RIGHT_DELEGATION_NAK);
	Handler_FromDBProxyServer_DECL(DZ_GUILD_CHANGE_MASTER_ACK);		// ???? ?????? ????
	Handler_FromDBProxyServer_DECL(DZ_GUILD_CHANGE_MASTER_NAK);

	Handler_FromDBProxyServer_DECL(DZ_GUILD_CREATE_CORPS_ACK);			// ???? ???? ????
	Handler_FromDBProxyServer_DECL(DZ_GUILD_CREATE_CORPS_NAK);
	Handler_FromDBProxyServer_DECL(DZ_GUILD_DESTROY_CORPS_ACK);			// ???? ???? ????
	Handler_FromDBProxyServer_DECL(DZ_GUILD_DESTROY_CORPS_NAK);
	Handler_FromDBProxyServer_DECL(DZ_GUILD_CHANGE_COMMANDER_ACK);		// ???? ?????? ????
	Handler_FromDBProxyServer_DECL(DZ_GUILD_CHANGE_COMMANDER_NAK);
	Handler_FromDBProxyServer_DECL(DZ_GUILD_CHANGE_CORPS_ACK);			// ???? ???? ????
	Handler_FromDBProxyServer_DECL(DZ_GUILD_CHANGE_CORPS_NAK);
	Handler_FromDBProxyServer_DECL(DZ_GUILD_CREATE_REAR_CAMP_ACK);		// ???? ???????? ????
	Handler_FromDBProxyServer_DECL(DZ_GUILD_CREATE_REAR_CAMP_NAK);
	Handler_FromDBProxyServer_DECL(DZ_GUILD_DESTROY_REAR_CAMP_ACK);		// ???? ???????? ????
	Handler_FromDBProxyServer_DECL(DZ_GUILD_DESTROY_REAR_CAMP_NAK);
	Handler_FromDBProxyServer_DECL(DZ_GUILD_LOGOUT_CHAR_CMD);			// ???????? ????????
	Handler_FromDBProxyServer_DECL(DZ_GUILD_GM_GUILD_POINT_ACK);		// GM?????? ??????????
	Handler_FromDBProxyServer_DECL(DZ_GUILD_GM_GUILD_POINT_NAK);		// GM?????? ??????????
	Handler_FromDBProxyServer_DECL(DZ_GUILD_SELECT_CORPS_NAK);			// ???? ???? ???? ????.

	Handler_FromDBProxyServer_DECL(DZ_GUILD_RELATION_ACK);			// ???? ????
	Handler_FromDBProxyServer_DECL(DZ_GUILD_RELATION_NAK);
	Handler_FromDBProxyServer_DECL(DZ_GUILD_RELATION_DELETE_ACK);
	Handler_FromDBProxyServer_DECL(DZ_GUILD_RELATION_DELETE_NAK);
	Handler_FromDBProxyServer_DECL(DZ_GUILD_RELATION_BOTH_AGAINST_ACK);
	Handler_FromDBProxyServer_DECL(DZ_GUILD_RELATION_BOTH_AGAINST_NAK);

	Handler_FromDBProxyServer_DECL(DZ_GUILD_RECOVERY_GUILD_ACK);
	Handler_FromDBProxyServer_DECL(DZ_GUILD_RECOVERY_GUILD_NAK);
	Handler_FromDBProxyServer_DECL(DZ_GUILD_DESTROY_COMPLETE_ACK);
	Handler_FromDBProxyServer_DECL(DZ_GUILD_DESTROY_COMPLETE_NAK);

    Handler_FromDBProxyServer_DECL(DZ_GUILD_GUILDMARK_REGISTER_ACK);
	Handler_FromDBProxyServer_DECL(DZ_GUILD_GUILDMARK_REGISTER_NAK);
	Handler_FromDBProxyServer_DECL(DZ_GUILD_GUILDMARK_DELETE_ACK);
	Handler_FromDBProxyServer_DECL(DZ_GUILD_GUILDMARK_DELETE_NAK);

    Handler_FromDBProxyServer_DECL(DZ_GUILD_WAREHOUSE_OPEN_ACK);
	Handler_FromDBProxyServer_DECL(DZ_GUILD_WAREHOUSE_OPEN_NAK);

	Handler_FromDBProxyServer_DECL(DZ_GUILD_NAME_CHANGE_ACK);
	Handler_FromDBProxyServer_DECL(DZ_GUILD_NAME_CHANGE_NAK);
    //{ _KR_001385_20091013_DOMINATION_BASICINFO_SETTING
    Handler_FromDBProxyServer_DECL(DZ_DOMINATION_INFO_ACK);
    Handler_FromDBProxyServer_DECL(DZ_DOMINATION_AUCTION_INFO_ACK);
    Handler_FromDBProxyServer_DECL(DZ_DOMINATION_UPDATE_INFO_ACK);
    Handler_FromDBProxyServer_DECL(DZ_DOMINATION_ACCEP_HEIM_ACK);
    Handler_FromDBProxyServer_DECL(DZ_DOMINATION_INIT_AUCTIONINFO_ACK);
    //}
#ifdef _NA_20100604_BETTER_DOMINATION_MANAGER
    Handler_FromDBProxyServer_DECL(DZ_DOMINATION_UPDATE_GUILDINFO_ACK);
#endif

#ifdef _NA_20110216_INCREASE_DOMINATION_DEFFENSEWALL_POINT
    Handler_FromDBProxyServer_DECL(DZ_DOMINATION_ADD_DEFFENSEWALL_POINT_ACK);
#endif

#ifdef _NA_20110406_CHANGE_RULE_FOR_DOMINATION_JOIN
    Handler_FromDBProxyServer_DECL(DZ_DOMINATION_MEMBER_JOIN_ACK);
    Handler_FromDBProxyServer_DECL(DZ_DOMINATION_MEMBER_LEAVE_ACK);
    Handler_FromDBProxyServer_DECL(DZ_DOMINATION_MEMBERLIST_ANS);
    Handler_FromDBProxyServer_DECL(DZ_DOMINATION_MEMBERLIST_INIT_ACK);
#endif
};





















