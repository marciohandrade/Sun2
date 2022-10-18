#pragma once

#include "./PacketHandler.h"

#define Handler_FromAgentServer_DECL( p )	static VOID On##p( ChildServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
#define Handler_FromAgentServer_IMPL( p )	VOID Handler_FromAgentServer::On##p( ChildServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )

class DBUser;
class ChildServerSession;
class Handler_FromAgentServer
{
public:
	Handler_FromAgentServer(void){}
	~Handler_FromAgentServer(void){}
    // NOTE: f110518.2L
    static const nsPacket::PacketHandlerInfo GetPacketHandlerNodes();

    Handler_FromAgentServer_DECL(AD_CONNECTION_GROUND_CMD);
    Handler_FromAgentServer_DECL(AD_CONNECTION_HE_IS_HERE_CMD);
	Handler_FromAgentServer_DECL( AD_CHARINFO_CHARLISTREQ_SYN );
	Handler_FromAgentServer_DECL( AD_CHARINFO_CHARLISTREQ_DBR );
    //{_NA000023_090323_ENHANCED_INVENTORY_SLOT_FUNCTIONALITY_DB_
    Handler_FromAgentServer_DECL(AD_CHARINFO_CHARLISTREQ_INTER_CHAR_DBR);
    Handler_FromAgentServer_DECL(AD_CHARINFO_CHARLISTREQ_INTER_INVEN_DBR);
    Handler_FromAgentServer_DECL(AD_CHARINFO_CHARLISTREQ_INTER_PETLIST_DBR);
    Handler_FromAgentServer_DECL(AD_CHARINFO_CHARLISTREQ_INTER_EQUIP_ITEMSLOTINFO_DBR);
    //}
	Handler_FromAgentServer_DECL( AD_CHARINFO_CREATE_SYN );
	Handler_FromAgentServer_DECL( AD_CHARINFO_CREATE_DBR );

	Handler_FromAgentServer_DECL( AD_CHARINFO_DESTROY_SYN );
	Handler_FromAgentServer_DECL( AD_CHARINFO_DESTROY_DBR );

	Handler_FromAgentServer_DECL( AD_CHARINFO_RECOVER_SYN );
	Handler_FromAgentServer_DECL( AD_CHARINFO_RECOVER_DBR );

	Handler_FromAgentServer_DECL( AD_CONNECTION_DISCONNECT_CMD );

	Handler_FromAgentServer_DECL( AD_CHARINFO_STATE_SELECT_DBR );
	Handler_FromAgentServer_DECL( AD_CHARINFO_STATE_UPDATE_DBR );

#ifdef __NA_001044_ADD_CUSTOM_UI
	Handler_FromAgentServer_DECL( AD_CHARINFO_CHECK_CHARACTERID_SYN );
	Handler_FromAgentServer_DECL( AD_CHARINFO_CHECK_CHARACTERID_DBR );
#endif

	Handler_FromAgentServer_DECL( AD_CHARINFO_ADDIN_NTF );

#ifdef __NA000000_SHOP_USERID_50_PASSING_PROC__
    Handler_FromAgentServer_DECL( SERVERCOMMON_QUERY_CHARINFO_SYN );
    Handler_FromAgentServer_DECL( SERVERCOMMON_QUERY_CHARINFO_DBR );
#endif
#ifdef _PCBANG_POLICY_CHANGE_POST_EXPIRED
    Handler_FromAgentServer_DECL( AD_CHARINFO_PCBANG_BILLING_NTF );
#endif
    //_KR_0_20091021_FIRENDINVITATION_EXTEND //DB->D 캐릭터 생성후 친구추천관계 맺기 = {
    Handler_FromAgentServer_DECL( AD_EVENT_INVITATIONRELATION_INSERT_DBR );	//< 캐릭터 생성후 친구추천관계 맺기
    Handler_FromAgentServer_DECL( AD_EVENT_INVITATEFRIEND_REWARD_LIST_SYN );    
    Handler_FromAgentServer_DECL( AD_EVENT_INVITATEFRIEND_REWARD_LIST_DBR );    
    //}
#ifdef _JP_20110222_INGAME_CHARNAME_CHANGE
    Handler_FromAgentServer_DECL( AD_CHARNAME_CHANGE_SYN );    
    Handler_FromAgentServer_DECL( AD_CHARNAME_CHANGE_DBR );    
#endif

#ifdef _NA_0_20110329_GAMEOPTION_SETTING
    Handler_FromAgentServer_DECL( AD_ETC_GAMEOPTION_SELECT_SYN );
    Handler_FromAgentServer_DECL( AD_ETC_GAMEOPTION_SELECT_ACK );
    Handler_FromAgentServer_DECL( AD_ETC_GAMEOPTION_SELECT_DBR );
    Handler_FromAgentServer_DECL( AD_ETC_GAMEOPTION_UPDATE_CMD );
    Handler_FromAgentServer_DECL( AD_ETC_GAMEOPTION_UPDATE_DBR );
#endif //_NA_0_20110329_GAMEOPTION_SETTING
#ifdef _NA004034_20120102_GUILD_POINT_SYSTEM
    Handler_FromAgentServer_DECL(AD_ETC_GUILD_RANKINGINFO_SYN);
    Handler_FromAgentServer_DECL(AD_ETC_GUILD_RANKINGINFO_DBR);
    Handler_FromAgentServer_DECL(AD_ETC_GUILDINFO_SYN);
#endif //_NA004034_20120102_GUILD_POINT_SYSTEM
};
