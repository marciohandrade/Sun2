#pragma once

#include "./PacketHandler.h"

#define Handler_FromGuildServer_DECL( p )	static VOID On##p( ChildServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
#define Handler_FromGuildServer_IMPL( p )	VOID Handler_FromGuildServer::On##p( ChildServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )

class ChildServerSession;

class Handler_FromGuildServer
{
public:
	Handler_FromGuildServer(void) {}
	~Handler_FromGuildServer(void) {}
    // NOTE: f110518.2L
    static const nsPacket::PacketHandlerInfo GetPacketHandlerNodes();

	Handler_FromGuildServer_DECL( DZ_GUILD_SELECT_SYN );
	Handler_FromGuildServer_DECL( DZ_GUILD_SELECT_DBR );
	/*Handler_FromGuildServer_DECL( DZ_GUILD_SELECT_MEMBER_SYN );*/
	Handler_FromGuildServer_DECL( DZ_GUILD_SELECT_MEMBER_DBR );

	Handler_FromGuildServer_DECL( DZ_GUILD_CREATE_SYN );
	Handler_FromGuildServer_DECL( DZ_GUILD_CREATE_DBR );
	Handler_FromGuildServer_DECL( DZ_GUILD_DESTROY_SYN );
	Handler_FromGuildServer_DECL( DZ_GUILD_DESTROY_DBR );

	Handler_FromGuildServer_DECL( DZ_GUILD_JOIN_SYN );
	Handler_FromGuildServer_DECL( DZ_GUILD_JOIN_DBR );
	Handler_FromGuildServer_DECL( DZ_GUILD_WITHDRAW_SYN );
	Handler_FromGuildServer_DECL( DZ_GUILD_WITHDRAW_DBR );

	Handler_FromGuildServer_DECL( DZ_GUILD_KICKPLAYER_SYN );
	Handler_FromGuildServer_DECL( DZ_GUILD_KICKPLAYER_DBR );

	Handler_FromGuildServer_DECL( DZ_GUILD_CHANGE_NOTICE_SYN );
	Handler_FromGuildServer_DECL( DZ_GUILD_CHANGE_NOTICE_DBR );

	Handler_FromGuildServer_DECL( DZ_GUILD_DONATE_UP_SYN);
	Handler_FromGuildServer_DECL( DZ_GUILD_DONATE_UP_DBR);
	Handler_FromGuildServer_DECL( DZ_GUILD_CONVERT_UP2GP_SYN);
	Handler_FromGuildServer_DECL( DZ_GUILD_CONVERT_UP2GP_DBR);

#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
	Handler_FromGuildServer_DECL( DZ_GUILD_CHUNTING_PVP_RESULT_SYN );
	Handler_FromGuildServer_DECL( DZ_GUILD_CHUNTING_PVP_RESULT_DBR );
#endif
	Handler_FromGuildServer_DECL(DZ_GUILD_RELATION_SYN);
	Handler_FromGuildServer_DECL(DZ_GUILD_RELATION_DBR);	
	Handler_FromGuildServer_DECL(DZ_GUILD_RELATION_DELETE_SYN);
	Handler_FromGuildServer_DECL(DZ_GUILD_RELATION_DELETE_DBR);
	Handler_FromGuildServer_DECL(DZ_GUILD_RELATION_BOTH_AGAINST_SYN);
	Handler_FromGuildServer_DECL(DZ_GUILD_RELATION_BOTH_AGAINST_DBR);

    Handler_FromGuildServer_DECL(DZ_GUILD_RELATION_INFO_SYN);
	Handler_FromGuildServer_DECL(DZ_GUILD_SELECT_RELATION_DBR);
	Handler_FromGuildServer_DECL(DZ_GUILD_CHANGE_DUTY_SYN);
	Handler_FromGuildServer_DECL(DZ_GUILD_CHANGE_DUTY_DBR);
	Handler_FromGuildServer_DECL(DZ_GUILD_RIGHT_DELEGATION_SYN);
	Handler_FromGuildServer_DECL(DZ_GUILD_RIGHT_DELEGATION_DBR);
	Handler_FromGuildServer_DECL(DZ_GUILD_CHANGE_MASTER_SYN);
	Handler_FromGuildServer_DECL(DZ_GUILD_CHANGE_MASTER_DBR);

	Handler_FromGuildServer_DECL(DZ_GUILD_CREATE_CORPS_SYN);
	Handler_FromGuildServer_DECL(DZ_GUILD_CREATE_CORPS_DBR);
	Handler_FromGuildServer_DECL(DZ_GUILD_DESTROY_CORPS_SYN);
	Handler_FromGuildServer_DECL(DZ_GUILD_DESTROY_CORPS_DBR);
	Handler_FromGuildServer_DECL(DZ_GUILD_CHANGE_COMMANDER_SYN);
	Handler_FromGuildServer_DECL(DZ_GUILD_CHANGE_COMMANDER_DBR);
	Handler_FromGuildServer_DECL(DZ_GUILD_CHANGE_CORPS_SYN);
	Handler_FromGuildServer_DECL(DZ_GUILD_CHANGE_CORPS_DBR);
	Handler_FromGuildServer_DECL(DZ_GUILD_CREATE_REAR_CAMP_SYN);
	Handler_FromGuildServer_DECL(DZ_GUILD_CREATE_REAR_CAMP_DBR);
	Handler_FromGuildServer_DECL(DZ_GUILD_DESTROY_REAR_CAMP_SYN);
	Handler_FromGuildServer_DECL(DZ_GUILD_DESTROY_REAR_CAMP_DBR);
	Handler_FromGuildServer_DECL(DZ_GUILD_SELECT_CORPS_DBR );
	Handler_FromGuildServer_DECL(DZ_GUILD_GM_GUILD_POINT_SYN );
	Handler_FromGuildServer_DECL(DZ_GUILD_GM_GUILD_POINT_DBR );

    Handler_FromGuildServer_DECL(DZ_GUILD_GUILDMARK_REGISTER_SYN);
	Handler_FromGuildServer_DECL(DZ_GUILD_GUILDMARK_REGISTER_DBR);
	Handler_FromGuildServer_DECL(DZ_GUILD_GUILDMARK_DELETE_SYN);
	Handler_FromGuildServer_DECL(DZ_GUILD_GUILDMARK_DELETE_DBR);

    Handler_FromGuildServer_DECL(DZ_GUILD_RECOVERY_GUILD_SYN);
	Handler_FromGuildServer_DECL(DZ_GUILD_RECOVERY_GUILD_DBR);
	Handler_FromGuildServer_DECL(DZ_GUILD_DESTROY_COMPLETE_SYN);
	Handler_FromGuildServer_DECL(DZ_GUILD_DESTROY_COMPLETE_DBR);

	Handler_FromGuildServer_DECL(DZ_GUILD_WAREHOUSE_OPEN_SYN);
	Handler_FromGuildServer_DECL(DZ_GUILD_WAREHOUSE_OPEN_DBR);
    Handler_FromGuildServer_DECL(DZ_GUILD_WAREHOUSE_ITEMSLOTINFO_SELECT_INTER_DBR);
	Handler_FromGuildServer_DECL(DZ_GUILD_NAME_CHANGE_SYN);
	Handler_FromGuildServer_DECL(DZ_GUILD_NAME_CHANGE_DBR);

    //{ _KR_001385_20091013_DOMINATION_BASICINFO_SETTING
    Handler_FromGuildServer_DECL(DZ_DOMINATION_INFO_SYN);
    Handler_FromGuildServer_DECL(DZ_DOMINATION_INFO_DBR);

    Handler_FromGuildServer_DECL(DZ_DOMINATION_AUCTION_INFO_SYN);
    Handler_FromGuildServer_DECL(DZ_DOMINATION_AUCTION_INFO_DBR);
       
    Handler_FromGuildServer_DECL(DZ_DOMINATION_UPDATE_INFO_SYN);
    Handler_FromGuildServer_DECL(DZ_DOMINATION_UPDATE_INFO_DBR);

    Handler_FromGuildServer_DECL(DZ_DOMINATION_ACCEP_HEIM_SYN);
    Handler_FromGuildServer_DECL(DZ_DOMINATION_ACCEP_HEIM_DBR);

    Handler_FromGuildServer_DECL(DZ_DOMINATION_INIT_AUCTIONINFO_SYN);
    Handler_FromGuildServer_DECL(DZ_DOMINATION_INIT_AUCTIONINFO_DBR);

    Handler_FromGuildServer_DECL(DZ_DOMINATION_UPDATE_RETURNHEIM_DBR);
    Handler_FromGuildServer_DECL(DZ_DOMINATION_DELETE_RETURNHEIM_DBR);
    //}
    //_NA_20100604_BETTER_DOMINATION_MANAGER
    Handler_FromGuildServer_DECL(DZ_DOMINATION_UPDATE_GUILDINFO_SYN);
    Handler_FromGuildServer_DECL(DZ_DOMINATION_UPDATE_GUILDINFO_DBR);

    //_NA_20100913_GUILD_WAREHOUSE_GM_COMMAND
    Handler_FromGuildServer_DECL(DZ_GUILD_GM_WAREHOUSE_FORCE_CLOSE_CMD);
    //_NA_20110216_INCREASE_DOMINATION_DEFFENSEWALL_POINT
    Handler_FromGuildServer_DECL(DZ_DOMINATION_ADD_DEFFENSEWALL_POINT_SYN);
    Handler_FromGuildServer_DECL(DZ_DOMINATION_ADD_DEFFENSEWALL_POINT_DBR);

    //_NA_20110406_CHANGE_RULE_FOR_DOMINATION_JOIN
    Handler_FromGuildServer_DECL(DZ_DOMINATION_MEMBER_JOIN_SYN);
    Handler_FromGuildServer_DECL(DZ_DOMINATION_MEMBER_JOIN_DBR);
    Handler_FromGuildServer_DECL(DZ_DOMINATION_MEMBER_LEAVE_SYN);
    Handler_FromGuildServer_DECL(DZ_DOMINATION_MEMBER_LEAVE_DBR);
    Handler_FromGuildServer_DECL(DZ_DOMINATION_MEMBERLIST_REQ);
    Handler_FromGuildServer_DECL(DZ_DOMINATION_MEMBERLIST_DBR);
    Handler_FromGuildServer_DECL(DZ_DOMINATION_MEMBERLIST_INIT_SYN);
    Handler_FromGuildServer_DECL(DZ_DOMINATION_MEMBERLIST_INIT_DBR);
};
















