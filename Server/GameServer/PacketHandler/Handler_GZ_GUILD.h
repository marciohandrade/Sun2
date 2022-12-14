#pragma once
#ifndef GAMESERVER_GZ_GUILD_H
#define GAMESERVER_GZ_GUILD_H

#include "./PacketHandler.h"

#define Handler_GZ_GUILD_DECL(p) \
    static void On##p(ServerSession* pServerSession, MSG_BASE* pRecvMsg, WORD wSize)
#define Handler_GZ_GUILD_IMPL(p) \
    void Handler_GZ_GUILD::On##p(ServerSession* pServerSession, MSG_BASE* pRecvMsg, WORD wSize)




class Handler_GZ_GUILD
{
public:
    Handler_GZ_GUILD(){}
    ~Handler_GZ_GUILD(){}

    // changes, f100819.1L
    static const nsPacket::PacketHandlerInfo GetPacketHandlerNodes();

    Handler_GZ_GUILD_DECL(DG_CONNECTION_GROUND_CMD);
    Handler_GZ_GUILD_DECL(GZ_GUILD_INFO_BRD);
    Handler_GZ_GUILD_DECL(GZ_GUILD_DESTROY_BRD);
    Handler_GZ_GUILD_DECL(GZ_GUILD_WITHDRAW_BRD);
    Handler_GZ_GUILD_DECL(GZ_GUILD_JOIN_NAK);
    Handler_GZ_GUILD_DECL(GZ_GUILD_SELECT_ACK);
    Handler_GZ_GUILD_DECL(GZ_GUILD_CREATE_ACK);
    Handler_GZ_GUILD_DECL(GZ_GUILD_JOIN_BRD);

    Handler_GZ_GUILD_DECL(GZ_GUILD_SELECT_NAK);
    Handler_GZ_GUILD_DECL(GZ_GUILD_CREATE_NAK);
    Handler_GZ_GUILD_DECL(GZ_GUILD_DESTROY_NAK);
    Handler_GZ_GUILD_DECL(GZ_GUILD_WITHDRAW_NAK);
    Handler_GZ_GUILD_DECL(GZ_GUILD_KICKPLAYER_NAK);

    Handler_GZ_GUILD_DECL(GZ_GUILD_DONATE_UP_BRD);
    Handler_GZ_GUILD_DECL(GZ_GUILD_DONATE_UP_NAK);
    Handler_GZ_GUILD_DECL(GZ_GUILD_CONVERT_UP2GP_BRD);
    Handler_GZ_GUILD_DECL(GZ_GUILD_CONVERT_UP2GP_NAK);
    Handler_GZ_GUILD_DECL(GZ_GUILD_RANKUP_BRD);

#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
    Handler_GZ_GUILD_DECL(GZ_GUILD_CHUNTING_PVP_RESULT_BRD);
#endif

    Handler_GZ_GUILD_DECL(GZ_GUILD_CHANGE_DUTY_BRD);
    Handler_GZ_GUILD_DECL(GZ_GUILD_CHANGE_DUTY_NAK);
    Handler_GZ_GUILD_DECL(GZ_GUILD_RIGHT_DELEGATION_LIST_ACK);
    Handler_GZ_GUILD_DECL(GZ_GUILD_RIGHT_DELEGATION_LIST_NAK);
    Handler_GZ_GUILD_DECL(GZ_GUILD_RIGHT_DELEGATION_BRD);
    Handler_GZ_GUILD_DECL(GZ_GUILD_RIGHT_DELEGATION_NAK);
    Handler_GZ_GUILD_DECL(GZ_GUILD_CHANGE_MASTER_BRD);
    Handler_GZ_GUILD_DECL(GZ_GUILD_CHANGE_MASTER_NAK);

    Handler_GZ_GUILD_DECL(GZ_GUILD_EMPTY_COMMANDER_INFO_ACK);
    Handler_GZ_GUILD_DECL(GZ_GUILD_EMPTY_COMMANDER_INFO_NAK);
    Handler_GZ_GUILD_DECL(GZ_GUILD_CREATE_CORPS_BRD);
    Handler_GZ_GUILD_DECL(GZ_GUILD_CREATE_CORPS_NAK);
    Handler_GZ_GUILD_DECL(GZ_GUILD_DESTROY_CORPS_BRD);
    Handler_GZ_GUILD_DECL(GZ_GUILD_DESTROY_CORPS_NAK);
    Handler_GZ_GUILD_DECL(GZ_GUILD_CHANGE_COMMANDER_BRD);       // ???? ?????? ????
    Handler_GZ_GUILD_DECL(GZ_GUILD_CHANGE_COMMANDER_NAK);
    Handler_GZ_GUILD_DECL(GZ_GUILD_CHANGE_CORPS_BRD);           // ???? ???? ????
    Handler_GZ_GUILD_DECL(GZ_GUILD_CHANGE_CORPS_NAK);
    Handler_GZ_GUILD_DECL(GZ_GUILD_CREATE_REAR_CAMP_BRD);       // ???? ???????? ????
    Handler_GZ_GUILD_DECL(GZ_GUILD_CREATE_REAR_CAMP_NAK);
    Handler_GZ_GUILD_DECL(GZ_GUILD_DESTROY_REAR_CAMP_BRD);      // ???? ???????? ????
    Handler_GZ_GUILD_DECL(GZ_GUILD_DESTROY_REAR_CAMP_NAK);
    Handler_GZ_GUILD_DECL(GZ_GUILD_INVITE_ACK);                 // ???? ???? 
    Handler_GZ_GUILD_DECL(GZ_GUILD_INVITE_NAK);
    Handler_GZ_GUILD_DECL(GZ_GUILD_LOGIN_CHAR_BRD);
    Handler_GZ_GUILD_DECL(GZ_GUILD_LOGIN_CHAR_NAK);
    Handler_GZ_GUILD_DECL(GZ_GUILD_LOGOUT_CHAR_BRD);
    Handler_GZ_GUILD_DECL(GZ_GUILD_CHAR_LEVELUP_BRD);
    Handler_GZ_GUILD_DECL(GZ_GUILD_KICKPLAYER_BRD);
    Handler_GZ_GUILD_DECL(GZ_GUILD_TOTALINFO_REQ_ACK);
    Handler_GZ_GUILD_DECL(GZ_GUILD_TOTALINFO_REQ_NAK);
    Handler_GZ_GUILD_DECL(GZ_GUILD_GM_GUILD_POINT_BRD);
    Handler_GZ_GUILD_DECL(GZ_GUILD_GM_GUILD_POINT_NAK);
    Handler_GZ_GUILD_DECL(GZ_GUILD_CHANGE_CHAOSTATE_BRD);
    Handler_GZ_GUILD_DECL(GZ_GUILD_ENTER_SERVER_ACK);

    Handler_GZ_GUILD_DECL(GZ_GUILD_RELATION_NAK);
    Handler_GZ_GUILD_DECL(GZ_GUILD_RELATION_BRD);    
    Handler_GZ_GUILD_DECL(GZ_GUILD_RELATION_DELETE_BRD);
    Handler_GZ_GUILD_DECL(GZ_GUILD_RELATION_DELETE_NAK);
    Handler_GZ_GUILD_DECL(GZ_GUILD_RELATION_BOTH_AGAINST_BRD);
    Handler_GZ_GUILD_DECL(GZ_GUILD_RELATION_BOTH_AGAINST_NAK);
    Handler_GZ_GUILD_DECL(GZ_GUILD_BOTH_RELATION_ACK);

    Handler_GZ_GUILD_DECL(GZ_GUILD_RELATION_INFO_BRD);
    Handler_GZ_GUILD_DECL(GZ_GUILD_DESTROY_WAIT_BRD);
    Handler_GZ_GUILD_DECL(GZ_GUILD_RECOVERY_GUILD_BRD);
    Handler_GZ_GUILD_DECL(GZ_GUILD_RECOVERY_GUILD_NAK);
    Handler_GZ_GUILD_DECL(GZ_GUILD_GUILDMARK_REGISTER_BRD);
    Handler_GZ_GUILD_DECL(GZ_GUILD_GUILDMARK_REGISTER_NAK);
    Handler_GZ_GUILD_DECL(GZ_GUILD_GUILDMARK_DELETE_BRD);
    Handler_GZ_GUILD_DECL(GZ_GUILD_GUILDMARK_DELETE_NAK);

    Handler_GZ_GUILD_DECL(GZ_GUILD_WAREHOUSE_OPEN_BRD);
    Handler_GZ_GUILD_DECL(GZ_GUILD_WAREHOUSE_OPEN_NAK);

    Handler_GZ_GUILD_DECL(GZ_GUILD_NAME_CHANGE_ACK);
    Handler_GZ_GUILD_DECL(GZ_GUILD_NAME_CHANGE_BRD);
    Handler_GZ_GUILD_DECL(GZ_GUILD_NAME_CHANGE_NAK);
    //{ _KR_001385_20091013_DOMINATION_BASICINFO_SETTING
    Handler_GZ_GUILD_DECL(GZ_DOMINATION_AUCTION_INFO_ACK);
    Handler_GZ_GUILD_DECL(GZ_DOMINATION_INFO_ACK);
    Handler_GZ_GUILD_DECL(GZ_DOMINATION_UPDATE_INFO_ACK);
    Handler_GZ_GUILD_DECL(GZ_DOMINATION_ACCEP_HEIM_ACK);
    Handler_GZ_GUILD_DECL(GZ_DOMINATION_INIT_ACTIONINFO_ACK);
    Handler_GZ_GUILD_DECL(GZ_DOMINATION_BATTING_HEIM_ACK);
    Handler_GZ_GUILD_DECL(GZ_DOMINATION_RETURNHIME_ACK);
    //}
    //_NA_20100604_BETTER_DOMINATION_MANAGER
    Handler_GZ_GUILD_DECL(GZ_DOMINATION_UPDATE_GUILDINFO_ACK);
    //_NA_20100913_GUILD_WAREHOUSE_GM_COMMAND
    Handler_GZ_GUILD_DECL(GZ_GUILD_GM_WAREHOUSE_FORCE_CLOSE_BRD);
    //_NA_20110216_INCREASE_DOMINATION_DEFFENSEWALL_POINT
    Handler_GZ_GUILD_DECL(GZ_DOMINATION_ADD_DEFFENSEWALL_POINT_ACK);

    //_NA_20110406_CHANGE_RULE_FOR_DOMINATION_JOIN
    Handler_GZ_GUILD_DECL(GZ_DOMINATION_MEMBER_JOIN_ACK);
    Handler_GZ_GUILD_DECL(GZ_DOMINATION_MEMBER_LEAVE_ACK);
    Handler_GZ_GUILD_DECL(GZ_DOMINATION_MEMBERLIST_ANS);
#ifdef _NA004034_20120102_GUILD_POINT_SYSTEM
    Handler_GZ_GUILD_DECL(GZ_GUILDPOINTSYSTEM);
#endif
#ifdef _NA_004034_20120102_GUILD_INTRODUCTION
    Handler_GZ_GUILD_DECL(GZ_GUILD_INTRODUCTION_SAVE_ACK);
#endif
#ifdef _NA_003923_20120130_GUILD_RENEWAL
    Handler_GZ_GUILD_DECL(GZ_GUILD_IN_GAME_GUILD_LOG_LIST_ANS);
#endif
    Handler_GZ_GUILD_DECL(GZ_GUILD_RENAME_CHAR_CMD);

#ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION
    Handler_GZ_GUILD_DECL(GZ_DOMINATION_CONTINENT_LORD_INFO_ACK);
#endif //_NA_007128_20140326_MONDSCHIEN_DOMINATION

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    Handler_GZ_GUILD_DECL(GZ_GUILD_JOIN_REQUEST_ACK);
    Handler_GZ_GUILD_DECL(GZ_GUILD_JOIN_REQUEST_REJECT_ACK);
    Handler_GZ_GUILD_DECL(GZ_GUILD_RELOAD_ACK);

    Handler_GZ_GUILD_DECL(GZ_GUILD_FACILITY_UPGRADE_ACK);
    Handler_GZ_GUILD_DECL(GZ_GUILD_FACILITY_SELECT_ACK);
#endif //_NA_007330_20140620_GUILD_SYSTEM_EXTENSION
#ifdef _NA_008334_20150608_SONNENSCHEIN
    Handler_GZ_GUILD_DECL(GZ_GUILD_SONNENSCHEIN_INITIALIZE_CMD);
    Handler_GZ_GUILD_DECL(GZ_GUILD_SONNENSCHEIN_UPDATE_ACK);
#endif //_NA_008334_20150608_SONNENSCHEIN
};

#endif //GAMESERVER_GZ_GUILD_H