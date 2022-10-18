
#pragma once

#include <PacketStruct_WZ.h>
#include "PacketHandler.h"

// <PROTOTYPEs>
//class GameServerSession;

//-------------------------------------------------------------------------------------------------
#define Handler_Guild_GameToGuild_DECL( p )	static VOID On##p( ChildServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize )
#define Handler_Guild_GameToGuild_IMPL( p )	VOID Handler_Guild_GameToGuild::On##p( ChildServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize )
//-------------------------------------------------------------------------------------------------

class Handler_Guild_GameToGuild
{
public:
    // NOTE: f110518.2L
    static const nsPacket::PacketHandlerInfo GetPacketHandlerNodes();
private:

	//Handler_Guild_GameToGuild_DECL(SERVERCOMMON_HEARTBEAT){}

	Handler_Guild_GameToGuild_DECL(GZ_GUILD_CREATE_SYN);
	Handler_Guild_GameToGuild_DECL(GZ_GUILD_DESTROY_SYN);
	Handler_Guild_GameToGuild_DECL(GZ_GUILD_JOIN_SYN);
	Handler_Guild_GameToGuild_DECL(GZ_GUILD_WITHDRAW_SYN);
	Handler_Guild_GameToGuild_DECL(GZ_GUILD_KICKPLAYER_SYN);
	Handler_Guild_GameToGuild_DECL(GZ_GUILD_DONATE_UP_SYN);
	Handler_Guild_GameToGuild_DECL(GZ_GUILD_CONVERT_UP2GP_SYN);

#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
	Handler_Guild_GameToGuild_DECL(GZ_GUILD_CHUNTING_PVP_RESULT_CMD);
#endif

	Handler_Guild_GameToGuild_DECL(GZ_GUILD_CHANGE_DUTY_SYN);					//��� ��å ����	
	Handler_Guild_GameToGuild_DECL(GZ_GUILD_RIGHT_DELEGATION_LIST_SYN);		//��� ���� ���� ��� ��û	
	Handler_Guild_GameToGuild_DECL(GZ_GUILD_RIGHT_DELEGATION_SYN);				//��� ���� ����
	Handler_Guild_GameToGuild_DECL(GZ_GUILD_CHANGE_MASTER_SYN);				//����� ����

	Handler_Guild_GameToGuild_DECL(GZ_GUILD_CREATE_CORPS_SYN);					//��� ���� ����
	Handler_Guild_GameToGuild_DECL(GZ_GUILD_DESTROY_CORPS_SYN);				//��� ���� �ػ�
	Handler_Guild_GameToGuild_DECL(GZ_GUILD_EMPTY_COMMANDER_INFO_SYN);			//��� �� ������ ���� ��û.
	Handler_Guild_GameToGuild_DECL(GZ_GUILD_CHANGE_COMMANDER_SYN);				//��� ������ ����.
	Handler_Guild_GameToGuild_DECL(GZ_GUILD_CHANGE_CORPS_SYN);					//��� ���� ����.
	Handler_Guild_GameToGuild_DECL(GZ_GUILD_CREATE_REAR_CAMP_SYN);				//��� ����ķ�� ����.
	Handler_Guild_GameToGuild_DECL(GZ_GUILD_DESTROY_REAR_CAMP_SYN);			//��� ����ķ�� �ػ�.
	Handler_Guild_GameToGuild_DECL(GZ_GUILD_INVITE_SYN);						//��� �ʴ�.
	Handler_Guild_GameToGuild_DECL(GZ_GUILD_LOGIN_CHAR_SYN);					//��� ��� �α���.
	Handler_Guild_GameToGuild_DECL(GZ_GUILD_LOGOUT_CHAR_SYN);					//��� ��� �α׾ƿ�.
	Handler_Guild_GameToGuild_DECL(GZ_GUILD_CHAR_LEVELUP_SYN);					//���� ������.
	Handler_Guild_GameToGuild_DECL(GZ_GUILD_TOTALINFO_REQ_SYN );
	Handler_Guild_GameToGuild_DECL(GZ_GUILD_GM_GUILD_POINT_SYN );				//GM��ɾ� �������Ʈ ����
	Handler_Guild_GameToGuild_DECL(GZ_GUILD_CHANGE_CHAOSTATE_SYN );			
	Handler_Guild_GameToGuild_DECL(GZ_GUILD_ENTER_SERVER_SYN );				//������ ���Ӽ������� �̵��� ��.
	Handler_Guild_GameToGuild_DECL(GZ_GUILD_PRE_LOGIN_CMD );					//���� �α��� ���� �غ�.

	Handler_Guild_GameToGuild_DECL(GZ_GUILD_RELATION_SYN);				//��� ����
	Handler_Guild_GameToGuild_DECL(GZ_GUILD_RELATION_DELETE_SYN);
	Handler_Guild_GameToGuild_DECL(GZ_GUILD_RELATION_BOTH_AGAINST_SYN);
	Handler_Guild_GameToGuild_DECL(GZ_GUILD_BOTH_RELATION_SYN);

    Handler_Guild_GameToGuild_DECL(GZ_GUILD_RELATION_INFO_SYN);
	Handler_Guild_GameToGuild_DECL(GZ_GUILD_RECOVERY_GUILD_SYN);
	Handler_Guild_GameToGuild_DECL(GZ_GUILD_GUILDMARK_REGISTER_SYN);
	Handler_Guild_GameToGuild_DECL(GZ_GUILD_GUILDMARK_DELETE_SYN);

    Handler_Guild_GameToGuild_DECL(GZ_GUILD_WAREHOUSE_OPEN_SYN);
	Handler_Guild_GameToGuild_DECL(GZ_GUILD_WAREHOUSE_CLOSE_SYN);

	Handler_Guild_GameToGuild_DECL(GZ_GUILD_NAME_CHANGE_SYN);
    //{ __KR_001385_20090907_DOMINATION_PACKET
    Handler_Guild_GameToGuild_DECL(GZ_DOMINATION_INFO_SYN);
    Handler_Guild_GameToGuild_DECL(GZ_DOMINATION_AUCTION_INFO_SYN);
    Handler_Guild_GameToGuild_DECL(GZ_DOMINATION_UPDATE_INFO_SYN);
    Handler_Guild_GameToGuild_DECL(GZ_DOMINATION_ACCEP_HEIM_SYN);
    Handler_Guild_GameToGuild_DECL(GZ_DOMINATION_INIT_ACTIONINFO_SYN);
    Handler_Guild_GameToGuild_DECL(GZ_DOMINATION_RETURNHIME_SYN);
    Handler_Guild_GameToGuild_DECL(GZ_DOMINATION_BATTING_HEIM_SYN);
    //}
    //_NA_20100604_BETTER_DOMINATION_MANAGER
    Handler_Guild_GameToGuild_DECL(GZ_DOMINATION_UPDATE_GUILDINFO_SYN);

    //_NA_20100913_GUILD_WAREHOUSE_GM_COMMAND
    Handler_Guild_GameToGuild_DECL(GZ_GUILD_GM_WAREHOUSE_FORCE_CLOSE_CMD);
    //_NA_20110216_INCREASE_DOMINATION_DEFFENSEWALL_POINT
    Handler_Guild_GameToGuild_DECL(GZ_DOMINATION_ADD_DEFFENSEWALL_POINT_SYN);

    //_NA_20110406_CHANGE_RULE_FOR_DOMINATION_JOIN
    Handler_Guild_GameToGuild_DECL(GZ_DOMINATION_MEMBER_JOIN_SYN);
    Handler_Guild_GameToGuild_DECL(GZ_DOMINATION_MEMBER_LEAVE_SYN);
    Handler_Guild_GameToGuild_DECL(GZ_DOMINATION_MEMBERLIST_REQ);
    Handler_Guild_GameToGuild_DECL(GZ_DOMINATION_MEMBERLIST_INIT_CMD);

#ifdef _NA004034_20120102_GUILD_POINT_SYSTEM
    Handler_Guild_GameToGuild_DECL(GZ_GUILDPOINTSYSTEM);
    Handler_Guild_GameToGuild_DECL(GZ_GUILDPOINT_CHANGE_CMD);
#endif //_NA004034_20120102_GUILD_POINT_SYSTEM

#ifdef _NA_003923_20120130_GUILD_RENEWAL
    Handler_Guild_GameToGuild_DECL(GZ_GUILD_IN_GAME_GUILD_LOG_LIST_REQ);
#endif
#ifdef _NA_004034_20120102_GUILD_INTRODUCTION
    Handler_Guild_GameToGuild_DECL(GZ_GUILD_INTRODUCTION_SAVE_SYN);
#endif
#ifdef _NA_000000_20130429_DOMINATION_PENALTY
    Handler_Guild_GameToGuild_DECL(GZ_DOMINATION_PENALTY_CMD);
#endif _NA_000000_20130429_DOMINATION_PENALTY

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    Handler_Guild_GameToGuild_DECL(GZ_GUILD_JOIN_REQUEST_SYN);
    Handler_Guild_GameToGuild_DECL(GZ_GUILD_JOIN_REQUEST_REJECT_SYN);
    Handler_Guild_GameToGuild_DECL(GZ_GUILD_RELOAD_SYN);

    Handler_Guild_GameToGuild_DECL(GZ_GUILD_FACILITY_UPGRADE_SYN);
    Handler_Guild_GameToGuild_DECL(GZ_GUILD_FACILITY_SELECT_SYN);
#endif //_NA_007330_20140620_GUILD_SYSTEM_EXTENSION

#ifdef _NA_008334_20150608_SONNENSCHEIN
    Handler_Guild_GameToGuild_DECL(GZ_GUILD_SONNENSCHEIN_UPDATE_SYN);
#endif //_NA_008334_20150608_SONNENSCHEIN
};
