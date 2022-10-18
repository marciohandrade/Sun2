#include "stdafx.h"
#include "./Handler_Guild.GuildToDbp.h"

#include <PacketStruct_DZ.h>
#include <TimeConverter.h>

#include "QueryObjects/DBHandler.h"
#include "QueryObjects/GuildServerQuery.h"
#include "Sessions/ChildServerSession.h"

#include "Services/GuildManagerInDBP.h"
#include "PacketHandler/Handler_Guild.DbpToGuild.h"
#include "PacketStruct_PointSystem.h"
#include <Services/GuildPointSystem/GuildPointSystem.h>
#include <Services/Guild/GuildManager.h>
#include <Services/Guild/Guild.h>

#ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION
#include "Services/DominationManager.h"
#endif //_NA_007128_20140326_MONDSCHIEN_DOMINATION

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
#include "PacketStruct_DW.h"
#include "PacketHandler/Handler_FromWorldServer.h"
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION

//==================================================================================================

BOOST_STATIC_ASSERT(RC::RC_GUILD_RESULT_COUNTS <= UCHAR_MAX);

//==================================================================================================
// CHANGES: f110518.2L
const nsPacket::PacketHandlerInfo Handler_Guild_GuildToDbp::GetPacketHandlerNodes()
{
#define HANDLER_NODE_INFO(c, p, phase) { MAKEWORD(c, p), false, phase, Handler_Guild_GuildToDbp::On##p }
#define HANDLER_NODE_INFO_DBR(c, p, phase) { MAKEWORD(c, p), true, phase, Handler_Guild_GuildToDbp::On##p }
#define HANDLER_NODE_INFO_EOR() { 0, false, static_cast<ePHASE_SHIFT>(0), NULL }

    static const nsPacket::PacketHandlerNode handlers_static_[] =
    {
        //
#ifdef _NA004034_20120102_GUILD_POINT_SYSTEM
        HANDLER_NODE_INFO(DZ_GUILD, DZ_GUILD_SELECT_SYN, PHASE_UNLIMITED),
#endif //_NA004034_20120102_GUILD_POINT_SYSTEM

        HANDLER_NODE_INFO_DBR(DZ_GUILD, DZ_GUILD_SELECT_DBR, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DZ_GUILD, DZ_GUILD_SELECT_MEMBER_DBR, PHASE_UNLIMITED),

        //HANDLER_NODE_INFO(DZ_GUILD, DZ_GUILD_CREATE_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DZ_GUILD, DZ_GUILD_CREATE_DBR, PHASE_UNLIMITED),
        //HANDLER_NODE_INFO(DZ_GUILD, DZ_GUILD_DESTROY_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DZ_GUILD, DZ_GUILD_DESTROY_DBR, PHASE_UNLIMITED),

        //HANDLER_NODE_INFO(DZ_GUILD, DZ_GUILD_JOIN_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DZ_GUILD, DZ_GUILD_JOIN_DBR, PHASE_UNLIMITED),
        //HANDLER_NODE_INFO(DZ_GUILD, DZ_GUILD_WITHDRAW_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DZ_GUILD, DZ_GUILD_WITHDRAW_DBR, PHASE_UNLIMITED),

        //HANDLER_NODE_INFO(DZ_GUILD, DZ_GUILD_KICKPLAYER_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DZ_GUILD, DZ_GUILD_KICKPLAYER_DBR, PHASE_UNLIMITED),

        //HANDLER_NODE_INFO(DZ_GUILD, DZ_GUILD_CHANGE_NOTICE_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DZ_GUILD, DZ_GUILD_CHANGE_NOTICE_DBR, PHASE_UNLIMITED),

        //HANDLER_NODE_INFO(DZ_GUILD, DZ_GUILD_DONATE_UP_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DZ_GUILD, DZ_GUILD_DONATE_UP_DBR, PHASE_UNLIMITED),

        //HANDLER_NODE_INFO(DZ_GUILD, DZ_GUILD_CONVERT_UP2GP_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DZ_GUILD, DZ_GUILD_CONVERT_UP2GP_DBR, PHASE_UNLIMITED),

    #ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
        //HANDLER_NODE_INFO(DZ_GUILD, DZ_GUILD_CHUNTING_PVP_RESULT_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DZ_GUILD, DZ_GUILD_CHUNTING_PVP_RESULT_DBR, PHASE_UNLIMITED),
    #endif
        //HANDLER_NODE_INFO(DZ_GUILD, DZ_GUILD_RELATION_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DZ_GUILD, DZ_GUILD_RELATION_DBR, PHASE_UNLIMITED),
        //HANDLER_NODE_INFO(DZ_GUILD, DZ_GUILD_RELATION_DELETE_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DZ_GUILD, DZ_GUILD_RELATION_DELETE_DBR, PHASE_UNLIMITED),
        //HANDLER_NODE_INFO(DZ_GUILD, DZ_GUILD_RELATION_BOTH_AGAINST_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DZ_GUILD, DZ_GUILD_RELATION_BOTH_AGAINST_DBR, PHASE_UNLIMITED),
        //HANDLER_NODE_INFO(DZ_GUILD, DZ_GUILD_RELATION_INFO_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DZ_GUILD, DZ_GUILD_SELECT_RELATION_DBR, PHASE_UNLIMITED),
        //HANDLER_NODE_INFO(DZ_GUILD, DZ_GUILD_CHANGE_DUTY_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DZ_GUILD, DZ_GUILD_CHANGE_DUTY_DBR, PHASE_UNLIMITED),
        //HANDLER_NODE_INFO(DZ_GUILD, DZ_GUILD_RIGHT_DELEGATION_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DZ_GUILD, DZ_GUILD_RIGHT_DELEGATION_DBR, PHASE_UNLIMITED),
        //HANDLER_NODE_INFO(DZ_GUILD, DZ_GUILD_CHANGE_MASTER_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DZ_GUILD, DZ_GUILD_CHANGE_MASTER_DBR, PHASE_UNLIMITED),

        //HANDLER_NODE_INFO(DZ_GUILD, DZ_GUILD_CREATE_CORPS_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DZ_GUILD, DZ_GUILD_CREATE_CORPS_DBR, PHASE_UNLIMITED),
        //HANDLER_NODE_INFO(DZ_GUILD, DZ_GUILD_DESTROY_CORPS_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DZ_GUILD, DZ_GUILD_DESTROY_CORPS_DBR, PHASE_UNLIMITED),
        //HANDLER_NODE_INFO(DZ_GUILD, DZ_GUILD_CHANGE_COMMANDER_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DZ_GUILD, DZ_GUILD_CHANGE_COMMANDER_DBR, PHASE_UNLIMITED),
        //HANDLER_NODE_INFO(DZ_GUILD, DZ_GUILD_CHANGE_CORPS_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DZ_GUILD, DZ_GUILD_CHANGE_CORPS_DBR, PHASE_UNLIMITED),
        //HANDLER_NODE_INFO(DZ_GUILD, DZ_GUILD_CREATE_REAR_CAMP_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DZ_GUILD, DZ_GUILD_CREATE_REAR_CAMP_DBR, PHASE_UNLIMITED),
        //HANDLER_NODE_INFO(DZ_GUILD, DZ_GUILD_DESTROY_REAR_CAMP_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DZ_GUILD, DZ_GUILD_DESTROY_REAR_CAMP_DBR, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DZ_GUILD, DZ_GUILD_SELECT_CORPS_DBR, PHASE_UNLIMITED),
        //HANDLER_NODE_INFO(DZ_GUILD, DZ_GUILD_GM_GUILD_POINT_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DZ_GUILD, DZ_GUILD_GM_GUILD_POINT_DBR, PHASE_UNLIMITED),

        //HANDLER_NODE_INFO(DZ_GUILD, DZ_GUILD_GUILDMARK_REGISTER_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DZ_GUILD, DZ_GUILD_GUILDMARK_REGISTER_DBR, PHASE_UNLIMITED),
        //HANDLER_NODE_INFO(DZ_GUILD, DZ_GUILD_GUILDMARK_DELETE_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DZ_GUILD, DZ_GUILD_GUILDMARK_DELETE_DBR, PHASE_UNLIMITED),

        HANDLER_NODE_INFO(DZ_GUILD, DZ_GUILD_RECOVERY_GUILD_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DZ_GUILD, DZ_GUILD_RECOVERY_GUILD_DBR, PHASE_UNLIMITED),
        //HANDLER_NODE_INFO(DZ_GUILD, DZ_GUILD_DESTROY_COMPLETE_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DZ_GUILD, DZ_GUILD_DESTROY_COMPLETE_DBR, PHASE_UNLIMITED),

        //HANDLER_NODE_INFO(DZ_GUILD, DZ_GUILD_WAREHOUSE_OPEN_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DZ_GUILD, DZ_GUILD_WAREHOUSE_OPEN_DBR, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DZ_GUILD, DZ_GUILD_WAREHOUSE_ITEMSLOTINFO_SELECT_INTER_DBR, PHASE_UNLIMITED),
        //HANDLER_NODE_INFO(DZ_GUILD, DZ_GUILD_NAME_CHANGE_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DZ_GUILD, DZ_GUILD_NAME_CHANGE_DBR, PHASE_UNLIMITED),

        //{ _KR_001385_20091013_DOMINATION_BASICINFO_SETTING 
        //HANDLER_NODE_INFO(DZ_GUILD, DZ_DOMINATION_INFO_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DZ_GUILD, DZ_DOMINATION_INFO_DBR, PHASE_UNLIMITED),

        //HANDLER_NODE_INFO(DZ_GUILD, DZ_DOMINATION_AUCTION_INFO_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DZ_GUILD, DZ_DOMINATION_AUCTION_INFO_DBR, PHASE_UNLIMITED),

        //HANDLER_NODE_INFO(DZ_GUILD, DZ_DOMINATION_UPDATE_INFO_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DZ_GUILD, DZ_DOMINATION_UPDATE_INFO_DBR, PHASE_UNLIMITED),

        //HANDLER_NODE_INFO(DZ_GUILD, DZ_DOMINATION_ACCEP_HEIM_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DZ_GUILD, DZ_DOMINATION_ACCEP_HEIM_DBR, PHASE_UNLIMITED),

        //HANDLER_NODE_INFO(DZ_GUILD, DZ_DOMINATION_INIT_AUCTIONINFO_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DZ_GUILD, DZ_DOMINATION_INIT_AUCTIONINFO_DBR, PHASE_UNLIMITED),

        HANDLER_NODE_INFO_DBR(DZ_GUILD, DZ_DOMINATION_UPDATE_RETURNHEIM_DBR, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DZ_GUILD, DZ_DOMINATION_DELETE_RETURNHEIM_DBR, PHASE_UNLIMITED),
        //}
        //_NA_20100604_BETTER_DOMINATION_MANAGER
        //HANDLER_NODE_INFO(DZ_GUILD, DZ_DOMINATION_UPDATE_GUILDINFO_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DZ_GUILD, DZ_DOMINATION_UPDATE_GUILDINFO_DBR, PHASE_UNLIMITED),

        //_NA_20100913_GUILD_WAREHOUSE_GM_COMMAND
        //HANDLER_NODE_INFO(DZ_GUILD, DZ_GUILD_GM_WAREHOUSE_FORCE_CLOSE_CMD, PHASE_UNLIMITED),
        //_NA_20110216_INCREASE_DOMINATION_DEFFENSEWALL_POINT
        //HANDLER_NODE_INFO(DZ_GUILD, DZ_DOMINATION_ADD_DEFFENSEWALL_POINT_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DZ_GUILD, DZ_DOMINATION_ADD_DEFFENSEWALL_POINT_DBR, PHASE_UNLIMITED),

        //_NA_20110406_CHANGE_RULE_FOR_DOMINATION_JOIN
        //HANDLER_NODE_INFO(DZ_GUILD, DZ_DOMINATION_MEMBER_JOIN_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DZ_GUILD, DZ_DOMINATION_MEMBER_JOIN_DBR, PHASE_UNLIMITED),
        //HANDLER_NODE_INFO(DZ_GUILD, DZ_DOMINATION_MEMBER_LEAVE_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DZ_GUILD, DZ_DOMINATION_MEMBER_LEAVE_DBR, PHASE_UNLIMITED),
        //HANDLER_NODE_INFO(DZ_GUILD, DZ_DOMINATION_MEMBERLIST_REQ, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DZ_GUILD, DZ_DOMINATION_MEMBERLIST_DBR, PHASE_UNLIMITED),
        //HANDLER_NODE_INFO(DZ_GUILD, DZ_DOMINATION_MEMBERLIST_INIT_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DZ_GUILD, DZ_DOMINATION_MEMBERLIST_INIT_DBR, PHASE_UNLIMITED),
    #ifdef _NA004034_20120102_GUILD_POINT_SYSTEM
        HANDLER_NODE_INFO(DZ_GUILD, DZ_GUILDPOINT_SYSTEM, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DZ_GUILD, DZ_GUILDPOINT_SYSTEM_DBR, PHASE_UNLIMITED),
    #endif //_NA004034_20120102_GUILD_POINT_SYSTEM

    #ifdef _NA_004034_20120102_GUILD_INTRODUCTION        
        HANDLER_NODE_INFO(DZ_GUILD, DZ_GUILD_INTRODUCTION_SAVE_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DZ_GUILD, DZ_GUILD_INTRODUCTION_SAVE_DBR, PHASE_UNLIMITED),
        HANDLER_NODE_INFO(DZ_GUILD, DZ_GUILD_INTRODUCTION_LOAD_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DZ_GUILD, DZ_GUILD_INTRODUCTION_LOAD_DBR, PHASE_UNLIMITED),
    #endif //_NA_004034_20120102_GUILD_INTRODUCTION
#ifdef _NA_003923_20120130_GUILD_RENEWAL
        HANDLER_NODE_INFO(DZ_GUILD, DZ_GUILD_WRITE_IN_GAME_LOG_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DZ_GUILD, DZ_GUILD_WRITE_IN_GAME_LOG_DBR, PHASE_UNLIMITED),

        HANDLER_NODE_INFO(DZ_GUILD, DZ_GUILD_IN_GAME_LOG_LIST_REQ, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DZ_GUILD, DZ_GUILD_IN_GAME_LOG_LIST_DBR, PHASE_UNLIMITED),
#endif
#ifdef _NA_000000_20130429_DOMINATION_PENALTY
        HANDLER_NODE_INFO(DZ_GUILD, DZ_DOMINATION_PENALTY_CMD, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DZ_GUILD, DZ_DOMINATION_PENALTY_DBR, PHASE_UNLIMITED),
#endif //_NA_000000_20130429_DOMINATION_PENALTY
#ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION
        HANDLER_NODE_INFO(DZ_GUILD, DZ_CONTINENT_LORD_INFO_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DZ_GUILD, DZ_CONTINENT_LORD_INFO_DBR, PHASE_UNLIMITED),
#endif //_NA_007128_20140326_MONDSCHIEN_DOMINATION
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
        HANDLER_NODE_INFO(DZ_GUILD, DZ_GUILD_JOIN_REQUEST_INSERT_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DZ_GUILD, DZ_GUILD_JOIN_REQUEST_INSERT_DBR, PHASE_UNLIMITED),
        HANDLER_NODE_INFO(DZ_GUILD, DZ_GUILD_JOIN_REQUEST_DELETE_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DZ_GUILD, DZ_GUILD_JOIN_REQUEST_DELETE_DBR, PHASE_UNLIMITED),
        HANDLER_NODE_INFO(DZ_GUILD, DZ_GUILD_JOIN_REQUEST_SELECT_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DZ_GUILD, DZ_GUILD_JOIN_REQUEST_SELECT_DBR, PHASE_UNLIMITED),

        HANDLER_NODE_INFO(DZ_GUILD, DZ_GUILD_FACILITY_UPDATE_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DZ_GUILD, DZ_GUILD_FACILITY_UPDATE_DBR, PHASE_UNLIMITED),
        HANDLER_NODE_INFO(DZ_GUILD, DZ_GUILD_FACILITY_SELECT_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DZ_GUILD, DZ_GUILD_FACILITY_SELECT_DBR, PHASE_UNLIMITED),
        HANDLER_NODE_INFO(DZ_GUILD, DZ_GUILD_FACILITY_INIT_SYN, PHASE_UNLIMITED),
#endif //_NA_007330_20140620_GUILD_SYSTEM_EXTENSION
#ifdef _NA_008334_20150608_SONNENSCHEIN
        HANDLER_NODE_INFO(DZ_GUILD, DZ_GUILD_SONNENSCHEIN_INITIALIZE_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DZ_GUILD, DZ_GUILD_SONNENSCHEIN_INITIALIZE_DBR, PHASE_UNLIMITED),

        HANDLER_NODE_INFO(DZ_GUILD, DZ_GUILD_SONNENSCHEIN_UPDATE_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DZ_GUILD, DZ_GUILD_SONNENSCHEIN_UPDATE_DBR, PHASE_UNLIMITED),
#endif //_NA_008334_20150608_SONNENSCHEIN
        //
        HANDLER_NODE_INFO_EOR() // end of records
    };
    BOOST_STATIC_ASSERT(_countof(handlers_static_) >= 1);
    nsPacket::PacketHandlerInfo return_value = { handlers_static_, _countof(handlers_static_) - 1 };
    //
    return return_value;
};

//==================================================================================================
Handler_Guild_GuildToDbp_IMPL( DZ_GUILD_SELECT_SYN )
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
	MSG_DZ_GUILD_SELECT_SYN * pRecvMsg = (MSG_DZ_GUILD_SELECT_SYN *)pMsg;

	STRING_SAFER_QUERY64 szQueryBuff;

    szQueryBuff.MakeString( "{?=call S_Guild_Select(%d)}" , pRecvMsg->m_GuildGuid );

	SelectGuildQuery * pQuery = SelectGuildQuery::ALLOC();
	pQuery->SetGuildGuid( pRecvMsg->m_GuildGuid );
	pQuery->SetUserKey( pRecvMsg->m_dwKey );
	pQuery->SetSessionIndex( pRecvMsg->m_dwSessionIndex );	//게임서버의 SessionIndex를 저장한다.

	pQuery->SetSelectType( pRecvMsg->m_bySelectType );
	pQuery->SetLoginCharGuid( pRecvMsg->m_LoginCharGuid );

	pQuery->SetQuery(szQueryBuff);
	pQuery->SetIndex( MAKEDWORD( (WORD)DZ_GUILD, (WORD)DZ_GUILD_SELECT_DBR ) );
	pQuery->SetVoidObject( pServerSession );
	pServerSession->DBQuery( pQuery );
}

Handler_Guild_GuildToDbp_IMPL( DZ_GUILD_SELECT_DBR )
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
	MSG_DBPROXY_RESULT * pResult	= (MSG_DBPROXY_RESULT *)pMsg;
	SelectGuildQuery * pQResult		= (SelectGuildQuery *)pResult->m_pData;

	if( !pQResult->ResultSuccess() )
	{
		MSG_DZ_GUILD_SELECT_NAK nmsg;
		nmsg.m_dwKey			= pQResult->GetUserKey();
		nmsg.m_dwSessionIndex	= pQResult->GetSessionIndex();
		nmsg.m_GuildGuid		= pQResult->GetGuildGuid();
		nmsg.m_byErrorCode		= 0;
        Handler_Guild_DbpToGuild::OnDZ_GUILD_SELECT_NAK(
            g_DBHandler.GetSharedSession(), &nmsg, sizeof(nmsg));
		SelectGuildQuery::FREE(pQResult); pQResult = NULL;
		return;
	}

	//------------------------------------------------------------------------------------------------------------------------
	// 군단정보 쿼리
	STRING_SAFER_QUERY64 szQueryBuff;
	szQueryBuff.MakeString( "{?=call S_GuildCorps_Select(%d)}", pQResult->GetGuildGuid() );

	//군단정보를 쿼리한다.
	SelectGuildCorpsQuery* pQuery = SelectGuildCorpsQuery::ALLOC();
	pQuery->SetUserKey( pQResult->GetUserKey() );
	pQuery->SetGuildGuid( pQResult->GetGuildGuid() );
	pQuery->SetSelectGuildQuery( pQResult );					// 길드 정보 보관
	pQuery->SetSessionIndex( pQResult->GetSessionIndex() );		// 

	pQuery->SetQuery(szQueryBuff);
	pQuery->SetIndex( MAKEDWORD( (WORD)DZ_GUILD, (WORD)DZ_GUILD_SELECT_CORPS_DBR ) );
	pQuery->SetVoidObject( pServerSession );
	pServerSession->DBQuery( pQuery );
}

Handler_Guild_GuildToDbp_IMPL(DZ_GUILD_SELECT_CORPS_DBR )
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
	MSG_DBPROXY_RESULT * pResult		= (MSG_DBPROXY_RESULT *)pMsg;
	SelectGuildCorpsQuery * pQResult	= (SelectGuildCorpsQuery *)pResult->m_pData;

	if( !pQResult->ResultSuccess() )
	{
		MSG_DZ_GUILD_SELECT_CORPS_NAK nmsg;
		nmsg.m_dwKey			= pQResult->GetUserKey();		//쿼리날린 유저의 키값이다.
		nmsg.m_dwSessionIndex	= pQResult->GetSessionIndex();	//게임서버의 세션이다.
		nmsg.m_GuildGuid		= pQResult->GetGuildGuid();
		nmsg.m_byErrorCode		= pQResult->ResultSuccess();
        Handler_Guild_DbpToGuild::OnDZ_GUILD_SELECT_CORPS_NAK(
            g_DBHandler.GetSharedSession(), &nmsg, sizeof(nmsg));
		SelectGuildQuery::FREE(pQResult->GetSelectGuildQuery());
		SelectGuildCorpsQuery::FREE(pQResult); pQResult = NULL;
		return;
	}

	//------------------------------------------------------------------------------------------------------------------------
	// 길드 맴버 쿼리!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	STRING_SAFER_QUERY64 szQueryBuff;
	szQueryBuff.MakeString( "{?=call S_Guild_SelectMember(%d)}", pQResult->GetGuildGuid() );

	SelectGuildMemberQuery * pQuery = SelectGuildMemberQuery::ALLOC();
	pQuery->SetUserKey( pQResult->GetUserKey() );
	pQuery->SetGuildGuid( pQResult->GetGuildGuid() );
	pQuery->SetSelectGuildQuery( pQResult->GetSelectGuildQuery() );	// 길드 정보 보관
	pQuery->SetSelectGuildCorpsQuery( pQResult );
	pQuery->SetSessionIndex( pQResult->GetSessionIndex() );

	pQuery->SetQuery(szQueryBuff);
	pQuery->SetIndex( MAKEDWORD( (WORD)DZ_GUILD, (WORD)DZ_GUILD_SELECT_MEMBER_DBR ) );
	pQuery->SetVoidObject( pServerSession );
	pServerSession->DBQuery( pQuery );
}

//200703218_YARU_길드 군단 정보 추가
Handler_Guild_GuildToDbp_IMPL( DZ_GUILD_SELECT_MEMBER_DBR )
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
	MSG_DBPROXY_RESULT * pResult		= (MSG_DBPROXY_RESULT *)pMsg;
	SelectGuildMemberQuery * pQResult	= (SelectGuildMemberQuery *)pResult->m_pData;

	if( !pQResult->ResultSuccess() )
	{
		MSG_DZ_GUILD_SELECT_NAK nmsg;
		nmsg.m_dwKey			= pQResult->GetUserKey();
		nmsg.m_dwSessionIndex	= pQResult->GetSessionIndex();
		nmsg.m_GuildGuid		= pQResult->GetGuildGuid();
		nmsg.m_byErrorCode		= 0;
        Handler_Guild_DbpToGuild::OnDZ_GUILD_SELECT_NAK(
            g_DBHandler.GetSharedSession(), &nmsg, sizeof(nmsg));

		SelectGuildQuery::FREE(pQResult->GetSelectGuildQuery());
		SelectGuildCorpsQuery::FREE(pQResult->GetSelectGuildCorpsQuery());
		SelectGuildMemberQuery::FREE(pQResult);
		pQResult = NULL;
		return;
	}

	ASSERT( pQResult->GetResultRowNum() > 0 || pQResult->GetResultRowNum() <= MAX_GUILD_MEMBER_NUM ); 
	
	SelectGuildQuery * pSelectQResult			= pQResult->GetSelectGuildQuery();

	MSG_DZ_GUILD_SELECT_ACK	msg;
    memset(&msg.m_GuildInfo, 0, sizeof(msg.m_GuildInfo));
    memset(&msg.m_MemberTotalInfo, 0, sizeof(msg.m_MemberTotalInfo));
	msg.m_dwKey	= pQResult->GetUserKey();
	msg.m_dwSessionIndex = pSelectQResult->GetSessionIndex();

	msg.m_bySelectType							= pSelectQResult->GetSelectType();
	msg.m_LoginCharGuid							= pSelectQResult->GetLoginCharGuid();

    //-- struct BASE_GUILD_INFO
	msg.m_GuildInfo.m_GuildGuid					= pQResult->GetGuildGuid();
	strncpy( msg.m_GuildInfo.m_tszGuildName, pSelectQResult->pResult[0].m_tszGuildName, MAX_GUILDNAME_LENGTH );
	msg.m_GuildInfo.m_tszGuildName[MAX_GUILDNAME_LENGTH] = '\0';
	msg.m_GuildInfo.m_GuildGrade				= pSelectQResult->pResult[0].m_GuildGrade;

    //-- struct GAME_GUILD_INFO : public BASE_GUILD_INFO
#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
    msg.m_GuildInfo.m_ChuntingPVPInfo.m_CHuntingDisCount	= pSelectQResult->pResult[0].m_CHuntingDisCount;
    msg.m_GuildInfo.m_ChuntingPVPInfo.m_CHuntingVCount		= pSelectQResult->pResult[0].m_CHuntingVCount;
    msg.m_GuildInfo.m_ChuntingPVPInfo.m_CHuntingLCount		= pSelectQResult->pResult[0].m_CHuntingLCount;
    msg.m_GuildInfo.m_ChuntingPVPInfo.m_CHuntingTotCount	= pSelectQResult->pResult[0].m_CHuntingTotCount;
    msg.m_GuildInfo.m_ChuntingPVPInfo.m_CHuntingPVPPoint	= pSelectQResult->pResult[0].m_CHuntingPVPPoint;
    msg.m_GuildInfo.m_ChuntingPVPInfo.m_ChuntingRanking		= pSelectQResult->pResult[0].m_ChuntingRanking;
#endif
    msg.m_GuildInfo.m_PatternIdx		= pSelectQResult->pResult[0].m_PatternIndex;
    msg.m_GuildInfo.m_BackGroundIdx		= pSelectQResult->pResult[0].m_BackGroundIndex;

    //-- struct GUILD_INFO : public GAME_GUILD_INFO
    msg.m_GuildInfo.m_GuildPoint				= pSelectQResult->pResult[0].m_GuildPoint;
	msg.m_GuildInfo.m_UserPoint					= pSelectQResult->pResult[0].m_GuildUserPoint;
    msg.m_GuildInfo.m_GuildDestroyWaitFlag		= pSelectQResult->pResult[0].m_GuildDestroyWaitFlag;
#if defined(__NA_000598_20070628_GUILD_SYSTEM_PART2_PENALTY) && !defined(__NA_20070715_GUILD_PENALTY_TEMP_REMOVE)
	msg.m_GuildInfo.m_GuildPenaltyTime	= util::ConvertTCharToDWORD64(pSelectQResult->pResult[0].m_tszGuildPenaltyTime);
	msg.m_GuildInfo.m_GuildDestroyTime	= util::ConvertTCharToDWORD64(pSelectQResult->pResult[0].m_tszGuildDestroyTime);
	msg.m_GuildInfo.m_nMasterChangeTime	= util::ConvertTCharToDWORD64(pSelectQResult->pResult[0].m_tszMasterChangeTime);
#elif defined(__NA_000598_20070628_GUILD_SYSTEM_PART2_PENALTY)
	msg.m_GuildInfo.m_GuildPenaltyTime	= 0;
	msg.m_GuildInfo.m_GuildDestroyTime	= 0;
	msg.m_GuildInfo.m_nMasterChangeTime	= 0;
#endif //__NA_000000_20080521_GUILD_PENALTY_TEMP_REMOVE
    msg.m_GuildInfo.m_DominationMapCount        = 0;//--이정보는 게임 서버에서 설정한다.
    msg.m_GuildInfo.m_DominationJoinCount       = pSelectQResult->pResult[0].m_DominationJoingCount;
    msg.m_GuildInfo.m_DominationDefenseCount    = pSelectQResult->pResult[0].m_DominationDefenseCount;
    msg.m_GuildInfo.m_DominationHuntingCount    = pSelectQResult->pResult[0].m_DominationHuntingCount;
#ifdef _NA_008334_20150608_SONNENSCHEIN
    msg.m_GuildInfo.sonnenschein_phase_ = pSelectQResult->pResult[0].sonnenschein_phase_;
#endif //_NA_008334_20150608_SONNENSCHEIN
    
    //  [7/24/2009 lst1024] 길드 공지가 없다. DB체크 필요.
    //-- 길드 공지
    strncpy( msg.m_szGuildNotice, pSelectQResult->pResult[0].m_tszGuildNotice, MAX_GUILDNOTICE_LENGTH );

	//-- 군단정보 세팅
	SelectGuildCorpsQuery* pCorpsQuery = pQResult->GetSelectGuildCorpsQuery();
	BYTE byTotCoprsNum  = (BYTE)pCorpsQuery->GetResultRowNum();
	if( byTotCoprsNum > MAX_GUILD_CORPS_NUM )
	{
		SUNLOG( eCRITICAL_LOG, "[Handler_Guild_GuildToDbp_IMPL( DZ_GUILD_SELECT_MEMBER_DBR )] Corps Overflow = %d", byTotCoprsNum );
		byTotCoprsNum = MAX_GUILD_CORPS_NUM;
	}

	ZeroMemory( &msg.m_CorpsTotalInfo, sizeof( msg.m_CorpsTotalInfo ) );

	for( BYTE i = 0 ; i < byTotCoprsNum ; ++i  )
	{
		BYTE byCorpsNum = pCorpsQuery->pResult[i].m_byCorpsNum;
		msg.m_CorpsTotalInfo.m_CorpsInfo[byCorpsNum].m_bCreate = TRUE;
		msg.m_CorpsTotalInfo.m_CorpsInfo[byCorpsNum].m_byCorpsNum = byCorpsNum;
		msg.m_CorpsTotalInfo.m_CorpsInfo[byCorpsNum].m_bRearCamp = pCorpsQuery->pResult[i].m_bRearCamp;
	}

    //-- 맴버 정보 셋팅
	msg.m_MemberTotalInfo.m_byMemberCount = (BYTE)pQResult->GetResultRowNum();
	if( msg.m_MemberTotalInfo.m_byMemberCount > MAX_GUILD_MEMBER_NUM )
	{
		SUNLOG( eCRITICAL_LOG, "[Handler_Guild_GuildToDbp_IMPL( DZ_GUILD_SELECT_MEMBER_DBR )] Member Overflow = %d", msg.m_MemberTotalInfo.m_byMemberCount );
		msg.m_MemberTotalInfo.m_byMemberCount = MAX_GUILD_MEMBER_NUM;
	}

	for( BYTE i = 0 ; i < msg.m_MemberTotalInfo.m_byMemberCount ; ++i )
	{
		msg.m_MemberTotalInfo.m_MemberInfo[i].m_CharGuid		= pQResult->pResult[i].m_CharGUID;
		strncpy( msg.m_MemberTotalInfo.m_MemberInfo[i].m_tszCharName, pQResult->pResult[i].m_tszCharName, MAX_CHARNAME_LENGTH );
		msg.m_MemberTotalInfo.m_MemberInfo[i].m_tszCharName[MAX_CHARNAME_LENGTH] = '\0';

		msg.m_MemberTotalInfo.m_MemberInfo[i].m_eGuildDuty	= pQResult->pResult[i].m_eDuty;							//길드 직책

		msg.m_MemberTotalInfo.m_MemberInfo[i].m_DelegateRight[0].m_Right = eGUILD_RIGHTS_GUILD_MARK;
		msg.m_MemberTotalInfo.m_MemberInfo[i].m_DelegateRight[0].m_bRightUse = pQResult->pResult[i].m_bGuildMark;		//길드 마크

		msg.m_MemberTotalInfo.m_MemberInfo[i].m_DelegateRight[1].m_Right = eGUILD_RIGHTS_RANKUP;
		msg.m_MemberTotalInfo.m_MemberInfo[i].m_DelegateRight[1].m_bRightUse = pQResult->pResult[i].m_bRankUp;			

		msg.m_MemberTotalInfo.m_MemberInfo[i].m_DelegateRight[2].m_Right = eGUILD_RIGHTS_RELATION;
		msg.m_MemberTotalInfo.m_MemberInfo[i].m_DelegateRight[2].m_bRightUse = pQResult->pResult[i].m_bRelation;		

		msg.m_MemberTotalInfo.m_MemberInfo[i].m_DelegateRight[3].m_Right = eGUILD_RIGHTS_SEIGE_WARFARE;
		msg.m_MemberTotalInfo.m_MemberInfo[i].m_DelegateRight[3].m_bRightUse = pQResult->pResult[i].m_bSiegeWarfare;		

		msg.m_MemberTotalInfo.m_MemberInfo[i].m_DelegateRight[4].m_Right = eGUILD_RIGHTS_UP2GP;
		msg.m_MemberTotalInfo.m_MemberInfo[i].m_DelegateRight[4].m_bRightUse = pQResult->pResult[i].m_bUpToGp;		

		msg.m_MemberTotalInfo.m_MemberInfo[i].m_DelegateRight[5].m_Right = eGUILD_RIGHTS_USE_UPGP;
		msg.m_MemberTotalInfo.m_MemberInfo[i].m_DelegateRight[5].m_bRightUse = pQResult->pResult[i].m_bUsePoint;		

		msg.m_MemberTotalInfo.m_MemberInfo[i].m_byCorpsNum	= pQResult->pResult[i].m_byCorpsNum;						// 길드 군단 번호
		
		//추가..
		msg.m_MemberTotalInfo.m_MemberInfo[i].m_byChaoState = pQResult->pResult[i].m_byChaoState;
		msg.m_MemberTotalInfo.m_MemberInfo[i].m_Level = pQResult->pResult[i].m_LV;
		msg.m_MemberTotalInfo.m_MemberInfo[i].m_byClass = pQResult->pResult[i].m_byClass;
		msg.m_MemberTotalInfo.m_MemberInfo[i].m_bLogin = FALSE;
	}

	//-- 길드 창고 셋팅
	g_GuildManagerInDBP.CreateGuildWarehouse(pQResult->GetGuildGuid());
    Handler_Guild_DbpToGuild::OnDZ_GUILD_SELECT_ACK(
        g_DBHandler.GetSharedSession(), &msg, msg.GetSize());

    GUILDGUID guid = pQResult->GetGuildGuid();

	SelectGuildQuery::FREE(pQResult->GetSelectGuildQuery());
	SelectGuildCorpsQuery::FREE(pQResult->GetSelectGuildCorpsQuery());
	SelectGuildMemberQuery::FREE(pQResult);
	pQResult = NULL;

#ifdef _NA004034_20120102_GUILD_POINT_SYSTEM
    ;{
        g_GuildPointSystem.CheckIn(guid);
    };
#endif //_NA004034_20120102_GUILD_POINT_SYSTEM
#ifdef _NA_004034_20120102_GUILD_INTRODUCTION
    ;{
        MSG_DZ_GUILD_INTRODUCTION_LOAD_SYN msg;
        msg.guild_guid = guid;
        g_DBProxyServer.SendToDBPServer(&msg, sizeof(msg));
    };
#endif //_NA_004034_20120102_GUILD_INTRODUCTION
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    ;{
        MSG_DZ_GUILD_JOIN_REQUEST_SELECT_SYN guild_join_request_msg;
        guild_join_request_msg.guild_guid = guid;
        g_DBProxyServer.SendToDBPServer(&guild_join_request_msg, sizeof(guild_join_request_msg));
    };
#endif //_NA_007330_20140620_GUILD_SYSTEM_EXTENSION

}

Handler_Guild_GuildToDbp_IMPL( DZ_GUILD_CREATE_SYN )
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
	MSG_DZ_GUILD_CREATE_SYN * pRecvMsg = (MSG_DZ_GUILD_CREATE_SYN *)pMsg;

	//	(WAVERIX)(080630)(BUG-FIX)	캐릭터이름 제한수치에 도달한 캐릭터가 길드 창설시 실패하는 문제 수정
	//	- 원인: 패킷에 주어진 버퍼는 NULL고려 없는 16이며, STRING_SAFER_CHARNAME 은 최소한 NULL이 고려된
	//	16+1이상이 입력되었다고 인식하므로, 마지막 null위치에 임의의 값을 NULL유무를 보고 입력 성공 여부를 평가하게 된다.
	//	EX) 's2lI엘리부대Il2s' 16자이며, DZ_GUILD_CREATE_SYN::tszTmpMasterCharName[16]이므로, NULL이 해당 버퍼에 존재하지 못한다.
	//	- STRING_SAFER_CHARNAME은 최소한 CHAR charName[16+1]의 문자열이 입력되었다고 보며, 의미 있는 16byte이후는 null이 존재해야
	//	안전한 문자열이 입력되었다고 판단한다. 위의 경우는 잘못된 값이 추가로 인식되므로 에러를 반환하게 된다.
	TCHAR tszTmpGuildName[MAX_GUILDNAME_LENGTH+1];
	TCHAR tszTmpMasterCharName[MAX_CHARNAME_LENGTH+1];
    _tcsncpy(tszTmpGuildName, pRecvMsg->m_tszGuildName, _countof(tszTmpGuildName));
    tszTmpGuildName[_countof(tszTmpGuildName) - 1] = _T('\0');
    _tcsncpy(tszTmpMasterCharName, pRecvMsg->m_szMasterCharName, _countof(tszTmpMasterCharName));
    tszTmpMasterCharName[_countof(tszTmpMasterCharName) - 1] = _T('\0');

	//	(WAVERIX)(080630)(BUG-FIX) STRING_SAFER_CHARNAME 사용제거 - 입력 버퍼가 null없는 문자열이므로...

	STRING_SAFER_QUERY256 szQueryBuff;
	szQueryBuff.MakeString(
		"{?=call S_Guild_Create('%s',%d,'%s')}", 
		(TCHAR*)tszTmpGuildName, pRecvMsg->m_MasterCharGuid, (TCHAR*)tszTmpMasterCharName );

	CreateGuildQuery* pQuery = CreateGuildQuery::ALLOC();
	pQuery->SetUserKey( pRecvMsg->m_dwKey );
	pQuery->SetSessionIndex( pRecvMsg->m_dwSessionIndex );
	pQuery->SetCharGuid( pRecvMsg->m_MasterCharGuid );
	pQuery->SetGuildName( tszTmpGuildName );		//	(WAVERIX)(080630)(BUG-FIX) 기껏 연산한 값을 사용하지 않는다. 뭐, 코드상 문제는 없지만...
	pQuery->SetCharName( tszTmpMasterCharName );	//	(WAVERIX)(080630)(BUG-FIX) 기껏 연산한 값을 사용하지 않는다. 뭐, 코드상 문제는 없지만...
	pQuery->SetClass( pRecvMsg->m_byClass );
	pQuery->SetLevel( pRecvMsg->m_Level );

	pQuery->SetQuery(szQueryBuff);
	pQuery->SetIndex( MAKEDWORD( (WORD)DZ_GUILD, (WORD)DZ_GUILD_CREATE_DBR ) );
	pQuery->SetVoidObject( pServerSession );
	pServerSession->DBQuery( pQuery );
}

Handler_Guild_GuildToDbp_IMPL( DZ_GUILD_CREATE_DBR )
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
	MSG_DBPROXY_RESULT * pResult	= (MSG_DBPROXY_RESULT *)pMsg;
	CreateGuildQuery * pQResult		= (CreateGuildQuery *)pResult->m_pData;


	BYTE RelationErrorCode = pQResult->ResultCode();
	g_GuildManagerInDBP.DBGuildCreateResultCodeConvert(RelationErrorCode);

	if( pQResult->ResultCode() != CreateGuildQuery::RETURNCODE_SUCCESS )
	{
		MSG_DZ_GUILD_CREATE_NAK nmsg;
		nmsg.m_dwKey			= pQResult->GetUserKey();
		nmsg.m_dwSessionIndex	= pQResult->GetSessionIndex();
		nmsg.m_byErrorCode		= RelationErrorCode;
		nmsg.m_MasterCharGuid	= pQResult->GetCharGuid();
        Handler_Guild_DbpToGuild::OnDZ_GUILD_CREATE_NAK(
            g_DBHandler.GetSharedSession(), &nmsg, sizeof(nmsg));
		CreateGuildQuery::FREE(pQResult); pQResult = NULL;
		return;
	}

	g_GuildManagerInDBP.CreateGuildWarehouse(pQResult->pResult[0].m_GuildGuid);

	MSG_DZ_GUILD_CREATE_ACK msg;
	msg.m_dwKey				= pQResult->GetUserKey();
	msg.m_dwSessionIndex	= pQResult->GetSessionIndex();
	msg.m_GuildGuid			= pQResult->pResult[0].m_GuildGuid;
	msg.m_MasterCharGuid	= pQResult->GetCharGuid();
    strncpy(msg.m_szGuildName, pQResult->GetGuildName(), _countof(msg.m_szGuildName));
    msg.m_szGuildName[_countof(msg.m_szGuildName) - 1] = '\0';
    strncpy(msg.m_szMasterName, pQResult->GetCharName(), _countof(msg.m_szMasterName));
    msg.m_szMasterName[_countof(msg.m_szMasterName) - 1] = '\0';
	msg.m_MasterDuty		= (eGUILD_DUTY)pQResult->pResult[0].m_byDuty;
	msg.m_byCreateCorpsNum	= pQResult->pResult[0].m_byCorpsNum;
	msg.m_byMasterClass		= pQResult->GetClass();
	msg.m_MasterLevel		= pQResult->GetLevel();
	msg.m_nDestroyRequestTime = util::ConvertTCharToDWORD64( pQResult->pResult[0].m_szDestroyRequestTime );
	msg.m_nMasterChangeTime = util::ConvertTCharToDWORD64( pQResult->pResult[0].m_szMasterChangeTime );
	if( msg.m_byCreateCorpsNum != 0 ) SUNLOG( eCRITICAL_LOG, "[Handler_Guild_GuildToDbp_IMPL( DZ_GUILD_CREATE_DBR )] Error!!! GuildGuid = %u, CorpsNum = %d", msg.m_GuildGuid, msg.m_byCreateCorpsNum );

	#if defined(__NA_000598_20070628_GUILD_SYSTEM_PART2_PENALTY) && !defined(__NA_20070715_GUILD_PENALTY_TEMP_REMOVE)
		msg.m_nGuildPenaltyTime = util::ConvertTCharToDWORD64( pQResult->pResult[0].m_szGuildPenaltyTime );
	#elif defined(__NA_000598_20070628_GUILD_SYSTEM_PART2_PENALTY)
		msg.m_nGuildPenaltyTime = 0;
	#endif
    Handler_Guild_DbpToGuild::OnDZ_GUILD_CREATE_ACK(
        g_DBHandler.GetSharedSession(), &msg, sizeof(msg));

	CreateGuildQuery::FREE(pQResult); pQResult = NULL;
}

Handler_Guild_GuildToDbp_IMPL( DZ_GUILD_DESTROY_SYN )
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
	const MSG_DZ_GUILD_DESTROY_SYN* receive_msg = static_cast<MSG_DZ_GUILD_DESTROY_SYN*>(pMsg);
	
#ifdef _USING_GUILD_DESTROY_LOGGING
    SUNLOG(eCRITICAL_LOG, 
        _T("|길드해산신청|Handler_Guild_GuildToDbp_IMPL( DZ_GUILD_DESTROY_SYN ))|MSG_DZ_GUILD_DESTROY_SYN 받음|Key:%u, SessionIndex:%u, MasterCharGuid:%u, GuildGuid:%d, DestroyWaitTime:%u|"), 
           receive_msg->m_dwKey, 
           receive_msg->m_dwSessionIndex, 
           receive_msg->m_MasterCharGuid, 
           receive_msg->m_GuildGuid, 
           receive_msg->m_DestroyWaitTime);
#endif // _USING_GUILD_DESTROY_LOGGING

    STRING_SAFER_QUERY128 query_string;
    query_string.MakeString(_T("{?=call [S_Guild_DestroyWait_Recent](%d, %d, %d)}"), 
                            receive_msg->m_GuildGuid, 
                            receive_msg->m_MasterCharGuid, 
                            receive_msg->m_DestroyWaitTime);

	DestroyGuildQuery* query = DestroyGuildQuery::ALLOC();
	query->SetGuildGuid(receive_msg->m_GuildGuid);
	query->SetUserKey(receive_msg->m_dwKey);
	query->SetSessionIndex(receive_msg->m_dwSessionIndex);
	query->SetMasterGuid(receive_msg->m_MasterCharGuid);
	query->SetQuery(query_string);
	query->SetIndex(MAKEDWORD(static_cast<WORD>(DZ_GUILD), static_cast<WORD>(DZ_GUILD_DESTROY_DBR)));
	query->SetVoidObject(pServerSession);

	pServerSession->DBQuery(query);
}

Handler_Guild_GuildToDbp_IMPL( DZ_GUILD_DESTROY_DBR )
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
	const MSG_DBPROXY_RESULT* receive_msg = static_cast<MSG_DBPROXY_RESULT*>(pMsg);
	DestroyGuildQuery* query_result	= static_cast<DestroyGuildQuery*>(receive_msg->m_pData);

    SUNLOG(eFULL_LOG, 
        _T("DZ_GUILD_DESTROY_DBR(길드 삭제 대기 신청 결과) [%s] [GUILDGUID:%d] [TIME:%I64d] [ERRCODE:%d]"),
 		query_result->ResultCode() ? _T("FAIL") : _T("SUCCESS"), 
        query_result->GetGuildGuid(),
        util::ConvertTCharToDWORD64(query_result->pResult[0].m_tszDestroyTime),
        query_result->ResultCode());

	if (query_result->ResultCode())
	{
		MSG_DZ_GUILD_DESTROY_NAK nak_msg;
		nak_msg.m_dwKey	= query_result->GetUserKey();
		nak_msg.m_GuildGuid = query_result->GetGuildGuid();
		nak_msg.m_byErrorCode = query_result->GetGuildResult(query_result->ResultCode());
		nak_msg.m_dwSessionIndex = query_result->GetSessionIndex();

#ifdef _USING_GUILD_DESTROY_LOGGING
        SUNLOG(eCRITICAL_LOG, 
            _T("|길드해산신청|Handler_Guild_GuildToDbp_IMPL( DZ_GUILD_DESTROY_DBR )|MSG_DZ_GUILD_DESTROY_NAK 보냄|Key:%u, GuildGuid:%d, ErrorCode:%d, SessionIndex:%u|"), 
               nak_msg.m_dwKey, 
               nak_msg.m_GuildGuid, 
               nak_msg.m_byErrorCode, 
               nak_msg.m_dwSessionIndex);
#endif // _USING_GUILD_DESTROY_LOGGING
        Handler_Guild_DbpToGuild::OnDZ_GUILD_DESTROY_NAK(
            g_DBHandler.GetSharedSession(), &nak_msg, sizeof(nak_msg));

        DestroyGuildQuery::FREE(query_result); 
        query_result = NULL;
        return;
	}

	MSG_DZ_GUILD_DESTROY_ACK msg;
	msg.m_dwKey	= query_result->GetUserKey();
	msg.m_dwSessionIndex = query_result->GetSessionIndex();
	msg.m_GuildGuid	= query_result->GetGuildGuid();
#if defined(__NA_000598_20070628_GUILD_SYSTEM_PART2_PENALTY) && !defined(__NA_20070715_GUILD_PENALTY_TEMP_REMOVE)
	msg.m_MasterCharGuid = query_result->GetMasterGuid();
	msg.m_dwDestroyTime	= util::ConvertTCharToDWORD64(query_result->pResult[0].m_tszDestroyTime);
#elif defined(__NA_000598_20070628_GUILD_SYSTEM_PART2_PENALTY)
	msg.m_MasterCharGuid = query_result->GetMasterGuid();
	msg.m_dwDestroyTime	= 0;
#endif

#ifdef _USING_GUILD_DESTROY_LOGGING
    SUNLOG(eCRITICAL_LOG, 
        _T("|길드해산신청|Handler_Guild_GuildToDbp_IMPL( DZ_GUILD_DESTROY_DBR )|MSG_DZ_GUILD_DESTROY_ACK 보냄|Key:%u, SessionIndex:%u, GuildGuid:%d, MasterCharGuid:%d, DestroyTime:%u|"), 
           msg.m_dwKey, 
           msg.m_dwSessionIndex, 
           msg.m_GuildGuid, 
           msg.m_MasterCharGuid, 
           msg.m_dwDestroyTime);
#endif // _USING_GUILD_DESTROY_LOGGING
    Handler_Guild_DbpToGuild::OnDZ_GUILD_DESTROY_ACK(
        g_DBHandler.GetSharedSession(), &msg, sizeof(msg));

	DestroyGuildQuery::FREE(query_result); 
    query_result = NULL;
}

Handler_Guild_GuildToDbp_IMPL( DZ_GUILD_JOIN_SYN )
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
	MSG_DZ_GUILD_JOIN_SYN * pRecvMsg = (MSG_DZ_GUILD_JOIN_SYN *)pMsg;

	//	(WAVERIX)(080702)(BUG-FIX) - char name 처리 상 null없는 버퍼를 사용중이다.
	TCHAR szCharName[MAX_CHARNAME_LENGTH+1];
	memcpy( szCharName, pRecvMsg->m_tszCharName, MAX_CHARNAME_LENGTH*sizeof(TCHAR) );
	szCharName[MAX_CHARNAME_LENGTH] = '\0';

	STRING_SAFER_QUERY256 szQueryBuff;

	szQueryBuff.MakeString(
		"{?=call S_Guild_JoinMember( %d, %d, %d, '%s', %d, %d)}",
		pRecvMsg->m_GuildGuid, pRecvMsg->m_OperCharGuid, pRecvMsg->m_TargetCharGuid, (TCHAR*)szCharName, pRecvMsg->m_byCorpsNum, pRecvMsg->m_Duty  ); 

	JoinGuildMemberQuery * pQuery = JoinGuildMemberQuery::ALLOC();
	pQuery->SetGuildGuid( pRecvMsg->m_GuildGuid );
	pQuery->SetUserKey( pRecvMsg->m_dwKey );

	pQuery->SetCharGuid( pRecvMsg->m_TargetCharGuid );
	pQuery->SetDuty( pRecvMsg->m_Duty );
	pQuery->SetClass( pRecvMsg->m_byClass );
	pQuery->SetLevel( pRecvMsg->m_Level );
	pQuery->SetCorpsNum( pRecvMsg->m_byCorpsNum );

	pQuery->SetCharName( szCharName );
	pQuery->SetSessionIndex( pRecvMsg->m_dwSessionIndex );
	pQuery->SetQuery(szQueryBuff);
	pQuery->SetIndex( MAKEDWORD( (WORD)DZ_GUILD, (WORD)DZ_GUILD_JOIN_DBR ) );
	pQuery->SetVoidObject( pServerSession );
	pServerSession->DBQuery( pQuery );
}

Handler_Guild_GuildToDbp_IMPL( DZ_GUILD_JOIN_DBR )
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
	MSG_DBPROXY_RESULT * pResult	= (MSG_DBPROXY_RESULT *)pMsg;
	JoinGuildMemberQuery * pQResult	= (JoinGuildMemberQuery *)pResult->m_pData;

	if( pQResult->ResultCode() != JoinGuildMemberQuery::RETURNCODE_SUCCESS )
	{
		MSG_DZ_GUILD_JOIN_NAK nmsg;
		nmsg.m_dwKey			= pQResult->GetUserKey();
		nmsg.m_dwSessionIndex	= pQResult->GetSessionIndex();
		nmsg.m_GuildGuid		= pQResult->GetGuildGuid();
		nmsg.m_CharGuid			= pQResult->GetCharGuid();
        strncpy(nmsg.m_tszCharName, pQResult->GetCharName(), _countof(nmsg.m_tszCharName));
        nmsg.m_tszCharName[_countof(nmsg.m_tszCharName) - 1] = '\0';

        if( pQResult->ResultCode() == 1 )
			nmsg.m_byErrorCode = RC::RC_GUILD_NOT_EXIST_GUILD;
		else if( pQResult->ResultCode() == 2 )
			nmsg.m_byErrorCode = RC::RC_GUILD_INVITE_PLAYER_ALREADY_MEMBER;
		else if( pQResult->ResultCode() == 3 )
			nmsg.m_byErrorCode = RC::RC_GUILD_NOT_GUILD_MEMBER;
		else if( pQResult->ResultCode() == 4 )
			nmsg.m_byErrorCode = RC::RC_GUILD_NOT_EXIST_TARGET_STATE;
		else if( pQResult->ResultCode() == 5 )
			nmsg.m_byErrorCode = RC::RC_GUILD_WITHDRAW_PENALTY_TIME;
		else if( pQResult->ResultCode() == 6 )
			nmsg.m_byErrorCode = RC::RC_GUILD_CORPS_PENALTY_TIME;
		else if( pQResult->ResultCode() == 7 )
			nmsg.m_byErrorCode = RC::RC_GUILD_CORPS_FULL_MEMBER;
		else
			nmsg.m_byErrorCode = RC::RC_GUILD_FAILED;

        Handler_Guild_DbpToGuild::OnDZ_GUILD_JOIN_NAK(
            g_DBHandler.GetSharedSession(), &nmsg, sizeof(nmsg) );
		JoinGuildMemberQuery::FREE(pQResult); pQResult = NULL;
		return;
	}

	MSG_DZ_GUILD_JOIN_ACK msg;
	msg.m_dwKey				= pQResult->GetUserKey();
	msg.m_dwSessionIndex	= pQResult->GetSessionIndex();
	msg.m_GuildGuid			= pQResult->GetGuildGuid();
	msg.m_CharGuid			= pQResult->GetCharGuid();
    strncpy(msg.m_tszCharName, pQResult->GetCharName(), _countof(msg.m_tszCharName));
    msg.m_tszCharName[_countof(msg.m_tszCharName) - 1] = '\0';
	msg.m_Duty				= pQResult->GetDuty();
	msg.m_byClass			= pQResult->GetClass();
	msg.m_Level				= pQResult->GetLevel();
	msg.m_byCorpsNum		= pQResult->GetCorpsNum();
    Handler_Guild_DbpToGuild::OnDZ_GUILD_JOIN_ACK(
        g_DBHandler.GetSharedSession(), &msg, sizeof(msg) );

	JoinGuildMemberQuery::FREE(pQResult); pQResult = NULL;
}

Handler_Guild_GuildToDbp_IMPL( DZ_GUILD_WITHDRAW_SYN )
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
	MSG_DZ_GUILD_WITHDRAW_SYN * pRecvMsg = (MSG_DZ_GUILD_WITHDRAW_SYN *)pMsg;

	STRING_SAFER_QUERY128 szQueryBuff;

	szQueryBuff.MakeString("{?=call S_Guild_WithdrawMember(%d, %d, %d)}", pRecvMsg->m_byPenaltyType, pRecvMsg->m_GuildGuid, pRecvMsg->m_CharGuid );

	WithdrawGuildMemberQuery * pQuery = WithdrawGuildMemberQuery::ALLOC();
	pQuery->SetGuildGuid( pRecvMsg->m_GuildGuid );
	pQuery->SetCharGuid( pRecvMsg->m_CharGuid );
	pQuery->SetUserKey( pRecvMsg->m_dwKey );
	pQuery->SetSessionIndex( pRecvMsg->m_dwSessionIndex );
	pQuery->SetPenaltyType( pRecvMsg->m_byPenaltyType );

	pQuery->SetQuery(szQueryBuff);
	pQuery->SetIndex( MAKEDWORD( (WORD)DZ_GUILD, (WORD)DZ_GUILD_WITHDRAW_DBR ) );
	pQuery->SetVoidObject( pServerSession );
	pServerSession->DBQuery( pQuery );
}

Handler_Guild_GuildToDbp_IMPL( DZ_GUILD_WITHDRAW_DBR )
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
	MSG_DBPROXY_RESULT * pResult		= (MSG_DBPROXY_RESULT *)pMsg;
	WithdrawGuildMemberQuery * pQResult	= (WithdrawGuildMemberQuery *)pResult->m_pData;

	if( !pQResult->ResultSuccess() )
	{
		MSG_DZ_GUILD_WITHDRAW_NAK msgNAK;
		msgNAK.m_dwKey			= pQResult->GetUserKey();
		msgNAK.m_dwSessionIndex	= pQResult->GetSessionIndex();
		msgNAK.m_GuildGuid		= pQResult->GetGuildGuid();
		msgNAK.m_CharGuid		= pQResult->GetCharGuid();
		msgNAK.m_byErrorCode	= 1;
        Handler_Guild_DbpToGuild::OnDZ_GUILD_WITHDRAW_NAK(
            g_DBHandler.GetSharedSession(), &msgNAK, sizeof(msgNAK));
		WithdrawGuildMemberQuery::FREE(pQResult); 
		pQResult = NULL;
		return;
	}

	MSG_DZ_GUILD_WITHDRAW_ACK msgACK;
	msgACK.m_dwKey			= pQResult->GetUserKey();
	msgACK.m_dwSessionIndex	= pQResult->GetSessionIndex();
	msgACK.m_GuildGuid		= pQResult->GetGuildGuid();
	msgACK.m_CharGuid		= pQResult->GetCharGuid();

#if defined(__NA_000598_20070628_GUILD_SYSTEM_PART2_PENALTY) && !defined(__NA_20070715_GUILD_PENALTY_TEMP_REMOVE)
	msgACK.m_MemberPenaltyTime = util::ConvertTCharToDWORD64(pQResult->pResult[0].m_tszMemberPenaltyTime);
	msgACK.m_byPenaltyType = pQResult->GetPenaltyType();
#elif defined(__NA_000598_20070628_GUILD_SYSTEM_PART2_PENALTY)
	msgACK.m_MemberPenaltyTime = 0;
	msgACK.m_byPenaltyType = 0;
#endif
    Handler_Guild_DbpToGuild::OnDZ_GUILD_WITHDRAW_ACK(
        g_DBHandler.GetSharedSession(), &msgACK, sizeof(msgACK));

	WithdrawGuildMemberQuery::FREE(pQResult); 
	pQResult = NULL;
}


// 길드원 강퇴~진행은 Withdraw와 동일
Handler_Guild_GuildToDbp_IMPL( DZ_GUILD_KICKPLAYER_SYN )
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
	MSG_DZ_GUILD_KICKPLAYER_SYN * pRecvMsg = (MSG_DZ_GUILD_KICKPLAYER_SYN *)pMsg;

	TCHAR szQueryBuff[MAX_QUERY_LENGTH_128+1];
	//[first argu] 1: withdraw 2: explusion
	_sntprintf( szQueryBuff, MAX_QUERY_LENGTH_128, 
		"{?=call S_Guild_KickMember(%d, %d, %d)}", pRecvMsg->m_GuildGuid, pRecvMsg->m_TargetCharGuid, pRecvMsg->m_byCorpsNum );

	szQueryBuff[MAX_QUERY_LENGTH_128]='\0';

    WithdrawGuildMemberQuery * pQuery = WithdrawGuildMemberQuery::ALLOC();
	pQuery->SetGuildGuid( pRecvMsg->m_GuildGuid );
    pQuery->SetOperCharGuid( pRecvMsg->m_OperCharGuid );
    pQuery->SetCharGuid( pRecvMsg->m_TargetCharGuid );

	pQuery->SetUserKey( pRecvMsg->m_dwKey );
	pQuery->SetSessionIndex( pRecvMsg->m_dwSessionIndex );
	pQuery->SetQuery(szQueryBuff);
	pQuery->SetIndex( MAKEDWORD( (WORD)DZ_GUILD, (WORD)DZ_GUILD_KICKPLAYER_DBR ) );
	pQuery->SetVoidObject( pServerSession );
	pServerSession->DBQuery( pQuery );
}

Handler_Guild_GuildToDbp_IMPL( DZ_GUILD_KICKPLAYER_DBR )
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
	MSG_DBPROXY_RESULT * pResult		= (MSG_DBPROXY_RESULT *)pMsg;
    WithdrawGuildMemberQuery * pQResult	= (WithdrawGuildMemberQuery *)pResult->m_pData;

	if( !pQResult->ResultSuccess() )
	{
		MSG_DZ_GUILD_KICKPLAYER_NAK msgNAK;
		msgNAK.m_dwKey			= pQResult->GetUserKey();
		msgNAK.m_dwSessionIndex	= pQResult->GetSessionIndex();
		msgNAK.m_GuildGuid		= pQResult->GetGuildGuid();
		msgNAK.m_CharGuid		= pQResult->GetCharGuid();
		_tcsncpy( msgNAK.m_ptszCharName, pQResult->GetCharName(), MAX_CHARNAME_LENGTH );
		msgNAK.m_ptszCharName[MAX_CHARNAME_LENGTH] = '\0';
		if( pQResult->pParam[0].m_Result == 1 )		// 길드멤버가 존재하지 않습니다.
			msgNAK.m_dwErrorCode = 1;
		else 
			msgNAK.m_dwErrorCode = 0;
        Handler_Guild_DbpToGuild::OnDZ_GUILD_KICKPLAYER_NAK(
            g_DBHandler.GetSharedSession(), &msgNAK, sizeof(msgNAK));
        WithdrawGuildMemberQuery::FREE(pQResult); pQResult = NULL;
		return;
	}

	MSG_DZ_GUILD_KICKPLAYER_ACK msgACK;
	msgACK.m_dwKey			= pQResult->GetUserKey();
	msgACK.m_dwSessionIndex	= pQResult->GetSessionIndex();
	msgACK.m_GuildGuid		= pQResult->GetGuildGuid();
    msgACK.m_OperCharGuid   = pQResult->GetOperCharGuid();
    msgACK.m_TargetCharGuid = pQResult->GetCharGuid();
#if defined(__NA_000598_20070628_GUILD_SYSTEM_PART2_PENALTY) && !defined(__NA_20070715_GUILD_PENALTY_TEMP_REMOVE)
	msgACK.m_GuildPenaltyTime	= util::ConvertTCharToDWORD64(pQResult->pResult[0].m_tszGuildPenaltyTime);
	msgACK.m_MemberPenaltyTime	= util::ConvertTCharToDWORD64(pQResult->pResult[0].m_tszMemberPenaltyTime);
#elif defined(__NA_000598_20070628_GUILD_SYSTEM_PART2_PENALTY)
	msgACK.m_GuildPenaltyTime	= 0;
	msgACK.m_MemberPenaltyTime	= 0;
#endif
    Handler_Guild_DbpToGuild::OnDZ_GUILD_KICKPLAYER_ACK(
        g_DBHandler.GetSharedSession(), &msgACK, sizeof(msgACK));
    WithdrawGuildMemberQuery::FREE(pQResult); pQResult = NULL;
}

Handler_Guild_GuildToDbp_IMPL( DZ_GUILD_CHANGE_NOTICE_SYN )
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
	MSG_DZ_GUILD_CHANGE_NOTICE_SYN * pRecvMsg = (MSG_DZ_GUILD_CHANGE_NOTICE_SYN*)pMsg;

	if( pRecvMsg->m_byNoticeLen > MAX_GUILDNOTICE_LENGTH )
	{
		MSG_DZ_GUILD_CHANGE_NOTICE_NAK nakMSG;
		nakMSG.m_dwKey			= pRecvMsg->m_dwKey;
		nakMSG.m_dwSessionIndex	= pRecvMsg->m_dwSessionIndex;
		nakMSG.m_GuildGuid		= pRecvMsg->m_GuildGuid;
		nakMSG.m_byErrorCode	= 0;
        Handler_Guild_DbpToGuild::OnDZ_GUILD_CHANGE_NOTICE_NAK(
            g_DBHandler.GetSharedSession(), &nakMSG, sizeof(nakMSG));
		SUNLOG( eCRITICAL_LOG, "GUILD_CHANGE_NOTICE_SYN Error - NoticeLength OverFlow!!![%u]\n", pRecvMsg->m_byNoticeLen );
		return;
	}

	TCHAR szQueryBuff[MAX_QUERY_LENGTH_1024+1];

	ChangeGuildNoticeQuery * pQuery = ChangeGuildNoticeQuery::ALLOC();
	pQuery->SetGuildGuid(pRecvMsg->m_GuildGuid);
	pQuery->SetUserKey(pRecvMsg->m_dwKey);
	pQuery->SetSessionIndex(pRecvMsg->m_dwSessionIndex);
	pQuery->SetNoticeType(pRecvMsg->m_eNoticeType);
	pQuery->SetNotice(pRecvMsg->m_tszGuildNotice);

	_sntprintf( szQueryBuff, MAX_QUERY_LENGTH_1024, "{?=call S_GuildNotice_Insert(%d, %d, %d, '%s')}",
		pQuery->GetGuildGuid(),
		0, 
		pQuery->GetNoticeType(),
		(TCHAR*)pQuery->GetNotice()); 
		
	szQueryBuff[MAX_QUERY_LENGTH_1024]='\0';

	pQuery->SetQuery(szQueryBuff);
	pQuery->SetIndex( MAKEDWORD( (WORD)DZ_GUILD, (WORD)DZ_GUILD_CHANGE_NOTICE_DBR ) );
	pQuery->SetVoidObject( pServerSession );
	pServerSession->DBQuery( pQuery );
}

Handler_Guild_GuildToDbp_IMPL( DZ_GUILD_CHANGE_NOTICE_DBR )
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
	MSG_DBPROXY_RESULT * pResult		= (MSG_DBPROXY_RESULT *)pMsg;
	ChangeGuildNoticeQuery * pQResult	= (ChangeGuildNoticeQuery *)pResult->m_pData;

	if( !pQResult->ResultSuccess() )
	{
		MSG_DZ_GUILD_CHANGE_NOTICE_NAK nmsg;
		nmsg.m_dwKey			= pQResult->GetUserKey();
		nmsg.m_dwSessionIndex	= pQResult->GetSessionIndex();
		nmsg.m_GuildGuid		= pQResult->GetGuildGuid();
		nmsg.m_byErrorCode		= 0;
        Handler_Guild_DbpToGuild::OnDZ_GUILD_CHANGE_NOTICE_NAK(
            g_DBHandler.GetSharedSession(), &nmsg, sizeof(nmsg));
		ChangeGuildNoticeQuery::FREE(pQResult); pQResult = NULL;

		SUNLOG( eFULL_LOG, "Change Notice Failed[%u]\n", nmsg.m_GuildGuid );

		return;
	}

	MSG_DZ_GUILD_CHANGE_NOTICE_ACK msg;
	msg.m_dwKey				= pQResult->GetUserKey();
	msg.m_GuildGuid			= pQResult->GetGuildGuid();
	msg.m_eNoticeType		= pQResult->GetNoticeType();
	msg.m_OperCharGuid  = pQResult->GetOperCharGuid();
	msg.m_dwSessionIndex	= pQResult->GetSessionIndex();

	strncpy(msg.m_tszGuildNotice, pQResult->GetNotice(), _countof(msg.m_tszGuildNotice));
    msg.m_tszGuildNotice[_countof(msg.m_tszGuildNotice) - 1] = '\0';
    Handler_Guild_DbpToGuild::OnDZ_GUILD_CHANGE_NOTICE_ACK(
        g_DBHandler.GetSharedSession(), &msg, sizeof(msg) );
	ChangeGuildNoticeQuery::FREE(pQResult); pQResult = NULL;
}

// 길드에 UP 기부
Handler_Guild_GuildToDbp_IMPL( DZ_GUILD_DONATE_UP_SYN )
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
	MSG_DZ_GUILD_DONATE_UP_SYN* pRecvMsg = (MSG_DZ_GUILD_DONATE_UP_SYN*)pMsg;

	TCHAR szQueryBuff[MAX_QUERY_LENGTH_128+1];
	_sntprintf( szQueryBuff, MAX_QUERY_LENGTH_128, 
		"{?=call S_Guild_DonateUP(%d, %d, %d)}",
		pRecvMsg->m_GuildGuid, pRecvMsg->m_CharGuid, pRecvMsg->m_GuildUP );
	szQueryBuff[MAX_QUERY_LENGTH_128]='\0';

	DonateUPQuery * pQuery = DonateUPQuery::ALLOC();
	pQuery->SetGuildGuid( pRecvMsg->m_GuildGuid );
	pQuery->SetUserKey( pRecvMsg->m_dwKey );
	pQuery->SetSessionIndex( pRecvMsg->m_dwSessionIndex );
	pQuery->SetDonateUP( pRecvMsg->m_GuildUP );

	pQuery->SetCharGuid( pRecvMsg->m_CharGuid );

	pQuery->SetQuery(szQueryBuff);
	pQuery->SetIndex( MAKEDWORD( (WORD)DZ_GUILD, (WORD)DZ_GUILD_DONATE_UP_DBR ) );
	pQuery->SetVoidObject( pServerSession );
	pServerSession->DBQuery( pQuery );
}

Handler_Guild_GuildToDbp_IMPL( DZ_GUILD_DONATE_UP_DBR )
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
	MSG_DBPROXY_RESULT	* pResult		= (MSG_DBPROXY_RESULT *)pMsg;
	DonateUPQuery		* pQResult		= (DonateUPQuery *)pResult->m_pData;

	if( !pQResult->ResultSuccess() )
	{
		MSG_DZ_GUILD_DONATE_UP_NAK nmsg;
		nmsg.m_dwKey			= pQResult->GetUserKey();
		nmsg.m_dwSessionIndex	= pQResult->GetSessionIndex();
		nmsg.m_GuildGuid		= pQResult->GetGuildGuid();
		nmsg.m_byErrorCode		= pQResult->ResultCode();
        Handler_Guild_DbpToGuild::OnDZ_GUILD_DONATE_UP_NAK(
            g_DBHandler.GetSharedSession(), &nmsg, sizeof(nmsg));
		DonateUPQuery::FREE(pQResult); pQResult = NULL;
		SUNLOG( eFULL_LOG, "Donate UP Failed[%u]", nmsg.m_GuildGuid );

		return;
	}

	MSG_DZ_GUILD_DONATE_UP_ACK msg;
	msg.m_dwKey				= pQResult->GetUserKey();
	msg.m_dwSessionIndex	= pQResult->GetSessionIndex();
	msg.m_GuildGuid			= pQResult->GetGuildGuid();
	msg.m_DonateUP			= pQResult->GetDonateUP();
	msg.m_OperCharGuid		= pQResult->GetCharGuid();
	msg.m_TotalUP			= pQResult->pResult->m_TotalUP;
    Handler_Guild_DbpToGuild::OnDZ_GUILD_DONATE_UP_ACK(
        g_DBHandler.GetSharedSession(), &msg, sizeof(msg));

	DonateUPQuery::FREE(pQResult); pQResult = NULL;

//	SUNLOG( eCRITICAL_LOG, "Donate UP Success[%u][Remain:%u/Tot:%u]", msg.m_GuildGuid, msg.m_RemainUP, msg.m_TotalUP );
}

// UP->GP 변환
Handler_Guild_GuildToDbp_IMPL( DZ_GUILD_CONVERT_UP2GP_SYN )
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
	MSG_DZ_GUILD_CONVERT_UP2GP_SYN* pRecvMsg = (MSG_DZ_GUILD_CONVERT_UP2GP_SYN*)pMsg;

	TCHAR szQueryBuff[MAX_QUERY_LENGTH_128+1];
	_sntprintf( szQueryBuff, MAX_QUERY_LENGTH_128, 
		"{?=call S_Guild_ConvertUP2GP(%d, %d)}", pRecvMsg->m_GuildGuid, pRecvMsg->m_ConverGP );
	szQueryBuff[MAX_QUERY_LENGTH_128]='\0';

	ConvertUP2GPQuery * pQuery = ConvertUP2GPQuery::ALLOC();
	pQuery->SetGuildGuid( pRecvMsg->m_GuildGuid );
	pQuery->SetUserKey( pRecvMsg->m_dwKey );
	pQuery->SetSessionIndex( pRecvMsg->m_dwSessionIndex );

	pQuery->SetQuery(szQueryBuff);
	pQuery->SetIndex( MAKEDWORD( (WORD)DZ_GUILD, (WORD)DZ_GUILD_CONVERT_UP2GP_DBR ) );
	pQuery->SetVoidObject( pServerSession );
	pServerSession->DBQuery( pQuery );
}

Handler_Guild_GuildToDbp_IMPL( DZ_GUILD_CONVERT_UP2GP_DBR )
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
	MSG_DBPROXY_RESULT	* pResult		= (MSG_DBPROXY_RESULT *)pMsg;
	ConvertUP2GPQuery		* pQResult		= (ConvertUP2GPQuery *)pResult->m_pData;

	if( !pQResult->ResultSuccess() )
	{
		MSG_DZ_GUILD_CONVERT_UP2GP_NAK nmsg;
		nmsg.m_dwKey			= pQResult->GetUserKey();
		nmsg.m_dwSessionIndex	= pQResult->GetSessionIndex();
		nmsg.m_GuildGuid		= pQResult->GetGuildGuid();
		nmsg.m_byErrorCode		= pQResult->ResultCode();
        Handler_Guild_DbpToGuild::OnDZ_GUILD_CONVERT_UP2GP_NAK(
            g_DBHandler.GetSharedSession(), &nmsg, sizeof(nmsg));
		ConvertUP2GPQuery::FREE(pQResult); pQResult = NULL;
		return;
	}

	MSG_DZ_GUILD_CONVERT_UP2GP_ACK msg;
	msg.m_dwKey				= pQResult->GetUserKey();
	msg.m_dwSessionIndex	= pQResult->GetSessionIndex();
	msg.m_GuildGuid			= pQResult->GetGuildGuid();
	msg.m_RemainUP			= pQResult->pResult->m_RemainUP;
	msg.m_TotalGP			= pQResult->pResult->m_TotalGP;

    Handler_Guild_DbpToGuild::OnDZ_GUILD_CONVERT_UP2GP_ACK(
        g_DBHandler.GetSharedSession(), &msg, sizeof(msg));

	ConvertUP2GPQuery::FREE(pQResult); pQResult = NULL;

	SUNLOG( eCRITICAL_LOG, "Convert UP->GP Success[%u][Remain:%u/Tot:%u]\n", msg.m_GuildGuid, msg.m_RemainUP, msg.m_TotalGP );
}

#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
Handler_Guild_GuildToDbp_IMPL( DZ_GUILD_CHUNTING_PVP_RESULT_SYN )
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
	MSG_DZ_GUILD_CHUNTING_PVP_RESULT_SYN * pRecvMsg = (MSG_DZ_GUILD_CHUNTING_PVP_RESULT_SYN *)pMsg;

	TCHAR szQueryBuff[MAX_QUERY_LENGTH_256+1];
	_sntprintf( szQueryBuff, MAX_QUERY_LENGTH_256, 
		"{?=call S_Guild_PVPUpdate(%d,%d,%d,%d,%d,%d,%d)}",
		pRecvMsg->m_GuildGuid, 
		pRecvMsg->m_PVPInfo.m_CHuntingDisCount,
		pRecvMsg->m_PVPInfo.m_CHuntingVCount,
		pRecvMsg->m_PVPInfo.m_CHuntingLCount,
		pRecvMsg->m_PVPInfo.m_CHuntingTotCount, 
		pRecvMsg->m_PVPInfo.m_CHuntingPVPPoint,
		pRecvMsg->m_PVPInfo.m_ChuntingRanking);
	szQueryBuff[MAX_QUERY_LENGTH_256]='\0';

	UpdateGuildChuntingRecordQuery * pQuery = UpdateGuildChuntingRecordQuery::ALLOC();
	pQuery->SetGuildGuid( pRecvMsg->m_GuildGuid );
	pQuery->SetUserKey( pRecvMsg->m_dwKey );
	pQuery->SetSessionIndex( pRecvMsg->m_dwSessionIndex );
	pQuery->SetPVPInfo( pRecvMsg->m_PVPInfo );

	pQuery->SetQuery(szQueryBuff);
	pQuery->SetIndex( MAKEDWORD( (WORD)DZ_GUILD, (WORD)DZ_GUILD_CHUNTING_PVP_RESULT_DBR ) );
	pQuery->SetVoidObject( pServerSession );
	pServerSession->DBQuery( pQuery );
}

Handler_Guild_GuildToDbp_IMPL( DZ_GUILD_CHUNTING_PVP_RESULT_DBR )
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
	MSG_DBPROXY_RESULT * pResult	= (MSG_DBPROXY_RESULT *)pMsg;
	UpdateGuildChuntingRecordQuery * pQResult	= (UpdateGuildChuntingRecordQuery *)pResult->m_pData;

	if( !pQResult->ResultSuccess() )
	{
		SUNLOG( eCRITICAL_LOG, "[Handler_Guild_GuildToDbp_IMPL( DZ_GUILD_CHUNTING_PVP_RESULT_DBR )] Fail, Query = %s", pQResult->GetQuery() );
		UpdateGuildChuntingRecordQuery::FREE(pQResult); pQResult = NULL;
		return;
	}

	MSG_DZ_GUILD_CHUNTING_PVP_RESULT_ACK msg;
	msg.m_dwKey						= pQResult->GetUserKey();
	msg.m_dwSessionIndex			= pQResult->GetSessionIndex();
	msg.m_GuildGuid					= pQResult->GetGuildGuid();
	msg.m_PVPInfo					= pQResult->GetPVPInfo();

    Handler_Guild_DbpToGuild::OnDZ_GUILD_CHUNTING_PVP_RESULT_ACK(
        g_DBHandler.GetSharedSession(), &msg, sizeof(msg));

	UpdateGuildChuntingRecordQuery::FREE(pQResult); pQResult = NULL;
}
#endif

//길드 마크 등록
Handler_Guild_GuildToDbp_IMPL(DZ_GUILD_GUILDMARK_REGISTER_SYN)
{ 
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
	MSG_DZ_GUILD_GUILDMARK_REGISTER_SYN* pRecvMsg 
		= static_cast<MSG_DZ_GUILD_GUILDMARK_REGISTER_SYN*>(pMsg);

	GuildMarkQuery* pQuery = GuildMarkQuery::ALLOC();	
	
	pQuery->SetGuildGuid(pRecvMsg->m_GuildIndex);
	pQuery->SetUserKey(pRecvMsg->m_dwKey);
	pQuery->SetSessionIndex(pRecvMsg->m_dwSessionIndex);
	
	STRING_SAFER_QUERY64 szQueryBuff;
	szQueryBuff.MakeString( "{?=call S_GuildMark_Register(%d, %d, %d, %d)}" , 
		pRecvMsg->m_GuildIndex, pRecvMsg->m_BackGroundIndex, pRecvMsg->m_PatternIndex, pRecvMsg->m_RequestPlayerIndex );
	
	pQuery->SetGuildGuid(pRecvMsg->m_GuildIndex);
	pQuery->SetMarkPatternIndex(pRecvMsg->m_PatternIndex);
	pQuery->SetMarkBackgroundIndxe(pRecvMsg->m_BackGroundIndex);
	pQuery->SetSlotIndex(pRecvMsg->m_SlotIdx);
	pQuery->SetSlotPos(pRecvMsg->m_SlotPos);
	pQuery->SetQuery(szQueryBuff);
	pQuery->SetIndex( MAKEDWORD((WORD)DZ_GUILD, (WORD)DZ_GUILD_GUILDMARK_REGISTER_DBR));
	pQuery->SetVoidObject(pServerSession);
	pServerSession->DBQuery(pQuery);
}

Handler_Guild_GuildToDbp_IMPL(DZ_GUILD_GUILDMARK_REGISTER_DBR)
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
	MSG_DBPROXY_RESULT *pResult = static_cast<MSG_DBPROXY_RESULT*>(pMsg);

	GuildMarkQuery* pQResult = static_cast<GuildMarkQuery*>(pResult->m_pData);

	if(GuildMarkQuery::RETURNCODE_SUCCESS  !=  pQResult->ResultCode())
	{
		MSG_DZ_GUILD_GUILDMARK_REGISTER_NAK msg;
		msg.m_dwKey = pQResult->GetUserKey();
		msg.m_dwSessionIndex = pQResult->GetSessionIndex();
		msg.m_Result = RC::RC_GUILD_MARK_CREATE_FAILED;
        Handler_Guild_DbpToGuild::OnDZ_GUILD_GUILDMARK_REGISTER_NAK(
            g_DBHandler.GetSharedSession(), &msg, sizeof(msg));
		GuildMarkQuery::FREE(pQResult); pQResult = NULL;
		return;
	}

	MSG_DZ_GUILD_GUILDMARK_REGISTER_ACK msg;
	msg.m_dwKey	= pQResult->GetUserKey();
	msg.m_dwSessionIndex = pQResult->GetSessionIndex();
	msg.m_GuildIndex= pQResult->GetGuildGuid();
	msg.m_SlotIdx = pQResult->GetSlotIndex();
	msg.m_SlotPos = pQResult->GetSlotPos();
	msg.m_PatternIndex = pQResult->GetMarkPatternIndex();
	msg.m_BackGroundIndex = pQResult->GetMarkBackgroundIndxe();
    Handler_Guild_DbpToGuild::OnDZ_GUILD_GUILDMARK_REGISTER_ACK(
        g_DBHandler.GetSharedSession(), &msg, sizeof(msg));
	GuildMarkQuery::FREE(pQResult); pQResult = NULL;
}

//길드 마크 삭제
Handler_Guild_GuildToDbp_IMPL(DZ_GUILD_GUILDMARK_DELETE_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
	MSG_DZ_GUILD_GUILDMARK_DELETE_SYN* pRecvMsg 
		= static_cast<MSG_DZ_GUILD_GUILDMARK_DELETE_SYN*>(pMsg);

	GuildMarkQuery* pQuery = GuildMarkQuery::ALLOC();	

	pQuery->SetUserKey(pRecvMsg->m_dwKey);
	pQuery->SetGuildGuid(pRecvMsg->m_GuildIndex);
	pQuery->SetSessionIndex(pRecvMsg->m_dwSessionIndex);

	STRING_SAFER_QUERY64 szQueryBuff;
	szQueryBuff.MakeString( "{?=call S_GuildMark_Delete(%d, %d, %d, %d)}" , 
		pRecvMsg->m_GuildIndex, pRecvMsg->m_BackGroundIndex, pRecvMsg->m_PatternIndex, pRecvMsg->m_RequestPlayerIndex);

	pQuery->SetGuildGuid(pRecvMsg->m_GuildIndex);
	pQuery->SetMarkPatternIndex(pRecvMsg->m_PatternIndex);
	pQuery->SetMarkBackgroundIndxe(pRecvMsg->m_BackGroundIndex);
	pQuery->SetQuery(szQueryBuff);
	pQuery->SetIndex( MAKEDWORD((WORD)DZ_GUILD, (WORD)DZ_GUILD_GUILDMARK_DELETE_DBR));
	pQuery->SetVoidObject(pServerSession);
	pServerSession->DBQuery(pQuery);
}

Handler_Guild_GuildToDbp_IMPL(DZ_GUILD_GUILDMARK_DELETE_DBR)
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
	MSG_DBPROXY_RESULT *pResult = static_cast<MSG_DBPROXY_RESULT*>(pMsg);
	GuildMarkQuery*pQResult = static_cast<GuildMarkQuery*>(pResult->m_pData);

	if(GuildMarkQuery::RETURNCODE_SUCCESS  !=  pQResult->ResultCode())
	{
		MSG_DZ_GUILD_GUILDMARK_DELETE_NAK msg;
		msg.m_dwKey = pQResult->GetUserKey();
		msg.m_Result = RC::RC_GUILD_MARK_DELETE_FAILED;
		msg.m_dwSessionIndex = pQResult->GetSessionIndex();
        Handler_Guild_DbpToGuild::OnDZ_GUILD_GUILDMARK_DELETE_NAK(
            g_DBHandler.GetSharedSession(), &msg, sizeof(msg));
		GuildMarkQuery::FREE(pQResult); pQResult = NULL;
		return;
	}

	MSG_DZ_GUILD_GUILDMARK_DELETE_ACK msg;
	msg.m_dwKey	= pQResult->GetUserKey();
	msg.m_dwSessionIndex = pQResult->GetSessionIndex();
	msg.m_GuildIndex= pQResult->GetGuildGuid();
	msg.m_PatternIndex = 0;
	msg.m_BackGroundIndex = 0;
    Handler_Guild_DbpToGuild::OnDZ_GUILD_GUILDMARK_DELETE_ACK(
        g_DBHandler.GetSharedSession(), &msg, sizeof(msg));
	GuildMarkQuery::FREE(pQResult); pQResult = NULL;
}

// 길드 직책 설정
Handler_Guild_GuildToDbp_IMPL(DZ_GUILD_CHANGE_DUTY_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
	MSG_DZ_GUILD_CHANGE_DUTY_SYN *pRecvMsg = (MSG_DZ_GUILD_CHANGE_DUTY_SYN *)pMsg;

	TCHAR szQueryBuffer[MAX_QUERY_LENGTH_128+1];

	// 길드ID, 실행CharGuid, 타겟CharGuid, 직책
	// 1:계급, 2:직책, 3:군단
	_sntprintf(szQueryBuffer, MAX_QUERY_LENGTH_128, "{?=call S_Guild_ChangeMemberPosition(%d, %d, %d)}",
		pRecvMsg->m_GuildGuid, pRecvMsg->m_TargetGuid, pRecvMsg->m_eNewDuty );
	szQueryBuffer[MAX_QUERY_LENGTH_128]='\0';

	ChangeGuildDutyQuery *pQuery = ChangeGuildDutyQuery::ALLOC();
	pQuery->SetGuildGuid(pRecvMsg->m_GuildGuid);
	pQuery->SetOperCharGuid(pRecvMsg->m_OperGuid);
	pQuery->SetTargetCharGuid(pRecvMsg->m_TargetGuid);
	pQuery->SetOldDuty(pRecvMsg->m_eOldDuty);
	pQuery->SetNewDuty(pRecvMsg->m_eNewDuty);
	pQuery->SetUserKey(pRecvMsg->m_dwKey);
	pQuery->SetSessionIndex(pRecvMsg->m_dwSessionIndex);

	pQuery->SetQuery(szQueryBuffer);
	pQuery->SetIndex(MAKEDWORD((WORD)DZ_GUILD, (WORD)DZ_GUILD_CHANGE_DUTY_DBR));
	pQuery->SetVoidObject(pServerSession);
	pServerSession->DBQuery(pQuery);
}

Handler_Guild_GuildToDbp_IMPL( DZ_GUILD_CHANGE_DUTY_DBR )
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
	MSG_DBPROXY_RESULT *pResult		= (MSG_DBPROXY_RESULT *)pMsg;
	ChangeGuildDutyQuery *pQResult	= (ChangeGuildDutyQuery *)pResult->m_pData;

	if(!pQResult->ResultSuccess())
	{
		MSG_DZ_GUILD_CHANGE_DUTY_NAK nakMSG;
		nakMSG.m_dwKey			= pQResult->GetUserKey();
		nakMSG.m_dwSessionIndex = pQResult->GetSessionIndex();
		nakMSG.m_GuildGuid		= pQResult->GetGuildGuid();
		nakMSG.m_byErrorCode	= pQResult->ResultCode();
        Handler_Guild_DbpToGuild::OnDZ_GUILD_CHANGE_DUTY_NAK(
            g_DBHandler.GetSharedSession(), &nakMSG, sizeof(nakMSG));
		ChangeGuildDutyQuery::FREE(pQResult);
		pQResult = NULL;
		return;
	}

#ifndef _NA_003923_20120130_GUILD_RENEWAL
    //현재 바뀐 직책이 길드부마스터나, 길드군사에서 바뀌는 것이라면.. 위임된 권한을 초기화 시켜야 한다.
    if( pQResult->GetOldDuty() == eGUILD_DUTY_SUBMASTER || pQResult->GetOldDuty() == eGUILD_DUTY_STRATEGIST )
    {
        if( pQResult->GetNewDuty() != eGUILD_DUTY_SUBMASTER && pQResult->GetNewDuty() != eGUILD_DUTY_STRATEGIST )
        {
            TCHAR szQueryBuffer[MAX_QUERY_LENGTH_128+1];

            _sntprintf(szQueryBuffer, MAX_QUERY_LENGTH_128, "{?=call S_GuildMemberAuth_Update(%d, %d, %d, %d, %d, %d, %d, %d)}",
                pQResult->GetGuildGuid(), pQResult->GetTargetCharGuid(), 0, 0, 0, 0, 0, 0 );
            szQueryBuffer[MAX_QUERY_LENGTH_128]='\0';

            GUILD_RIGHT_USE_INFO DelegateRight[MAX_GUILD_DELEGATE_RIGHT];
            DelegateRight[0].m_Right = eGUILD_RIGHTS_RELATION;
            DelegateRight[0].m_bRightUse = false;
            DelegateRight[1].m_Right = eGUILD_RIGHTS_RANKUP;
            DelegateRight[1].m_bRightUse = false;
            DelegateRight[2].m_Right = eGUILD_RIGHTS_UP2GP;
            DelegateRight[2].m_bRightUse = false;
            DelegateRight[3].m_Right = eGUILD_RIGHTS_USE_UPGP;
            DelegateRight[3].m_bRightUse = false;
            DelegateRight[4].m_Right = eGUILD_RIGHTS_SEIGE_WARFARE;
            DelegateRight[4].m_bRightUse = false;
            DelegateRight[5].m_Right = eGUILD_RIGHTS_GUILD_MARK;
            DelegateRight[5].m_bRightUse = false;

            GuildRightUpdateQuery* pQuery = GuildRightUpdateQuery::ALLOC();
            pQuery->SetUserKey(pQResult->GetUserKey());
            pQuery->SetSessionIndex(pQResult->GetSessionIndex());
            pQuery->SetGuildGuid(pQResult->GetGuildGuid());
            pQuery->SetTargetCharGuid(pQResult->GetTargetCharGuid());
            pQuery->SetDelegateRight( DelegateRight );

            pQuery->SetQuery(szQueryBuffer);
            pQuery->SetIndex(MAKEDWORD((WORD)DZ_GUILD, (WORD)DZ_GUILD_RIGHT_DELEGATION_DBR));
            pQuery->SetVoidObject(pServerSession);
            pServerSession->DBQuery(pQuery);
        }
    }
#endif

	MSG_DZ_GUILD_CHANGE_DUTY_ACK ackMSG;
	ackMSG.m_dwKey				= pQResult->GetUserKey();
	ackMSG.m_dwSessionIndex		= pQResult->GetSessionIndex();
	ackMSG.m_GuildGuid			= pQResult->GetGuildGuid();
	ackMSG.m_OperCharGuid		= pQResult->GetOperCharGuid();
	ackMSG.m_TargetCharGuid		= pQResult->GetTargetCharGuid();
	ackMSG.m_eTargetChangeDuty	= pQResult->GetNewDuty();
    Handler_Guild_DbpToGuild::OnDZ_GUILD_CHANGE_DUTY_ACK(
        g_DBHandler.GetSharedSession(), &ackMSG, sizeof(ackMSG));
	ChangeGuildDutyQuery::FREE(pQResult);
	pQResult = NULL;
}

Handler_Guild_GuildToDbp_IMPL(DZ_GUILD_RIGHT_DELEGATION_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
	MSG_DZ_GUILD_RIGHT_DELEGATION_SYN* pRecvMsg = (MSG_DZ_GUILD_RIGHT_DELEGATION_SYN*)pMsg;

	TCHAR szQueryBuffer[MAX_QUERY_LENGTH_128+1];

	BOOL bRelation = FALSE;
	BOOL bRankup = FALSE;
	BOOL bUpToGp = FALSE;
	BOOL bUsePoint = FALSE;
	BOOL bSiegeWarfare = FALSE;
	BOOL bGuildMark = FALSE;

	for( int i = 0; i < MAX_GUILD_DELEGATE_RIGHT; i++ )
	{
		switch( pRecvMsg->m_DelegateRight[i].m_Right )
		{
		case eGUILD_RIGHTS_RELATION:
			bRelation = pRecvMsg->m_DelegateRight[i].m_bRightUse;
			break;
		case eGUILD_RIGHTS_RANKUP:
			bRankup = pRecvMsg->m_DelegateRight[i].m_bRightUse;
			break;
		case eGUILD_RIGHTS_UP2GP:
			bUpToGp = pRecvMsg->m_DelegateRight[i].m_bRightUse;
			break;
		case eGUILD_RIGHTS_USE_UPGP:
			bUsePoint = pRecvMsg->m_DelegateRight[i].m_bRightUse;
			break;
		case eGUILD_RIGHTS_SEIGE_WARFARE:
			bSiegeWarfare = pRecvMsg->m_DelegateRight[i].m_bRightUse;
			break;
		case eGUILD_RIGHTS_GUILD_MARK:
			bGuildMark = pRecvMsg->m_DelegateRight[i].m_bRightUse;
			break;
		default:
			break;

		}
	}

	_sntprintf(szQueryBuffer, MAX_QUERY_LENGTH_128, "{?=call S_GuildMemberAuth_Update(%d, %d, %d, %d, %d, %d, %d, %d)}",
		pRecvMsg->m_GuildGuid, pRecvMsg->m_TargetGuid, bRelation, bRankup, bUpToGp, bUsePoint, bSiegeWarfare, bGuildMark );
	szQueryBuffer[MAX_QUERY_LENGTH_128]='\0';

	GuildRightUpdateQuery* pQuery = GuildRightUpdateQuery::ALLOC();
	pQuery->SetGuildGuid(pRecvMsg->m_GuildGuid);
	pQuery->SetTargetCharGuid(pRecvMsg->m_TargetGuid);
	pQuery->SetUserKey(pRecvMsg->m_dwKey);
	pQuery->SetSessionIndex(pRecvMsg->m_dwSessionIndex);
	pQuery->SetDelegateRight( pRecvMsg->m_DelegateRight );

	pQuery->SetQuery(szQueryBuffer);
	pQuery->SetIndex(MAKEDWORD((WORD)DZ_GUILD, (WORD)DZ_GUILD_RIGHT_DELEGATION_DBR));
	pQuery->SetVoidObject(pServerSession);
	pServerSession->DBQuery(pQuery);
}

Handler_Guild_GuildToDbp_IMPL( DZ_GUILD_RIGHT_DELEGATION_DBR )
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
	MSG_DBPROXY_RESULT* pResult		= (MSG_DBPROXY_RESULT*)pMsg;
	GuildRightUpdateQuery* pQResult	= (GuildRightUpdateQuery*)pResult->m_pData;

	if(!pQResult->ResultSuccess())
	{
		MSG_DZ_GUILD_RIGHT_DELEGATION_NAK nakMSG;
		nakMSG.m_dwKey			= pQResult->GetUserKey();
		nakMSG.m_dwSessionIndex = pQResult->GetSessionIndex();
		nakMSG.m_GuildGuid		= pQResult->GetGuildGuid();
		nakMSG.m_byErrorCode	= 0;

        Handler_Guild_DbpToGuild::OnDZ_GUILD_RIGHT_DELEGATION_NAK(
            g_DBHandler.GetSharedSession(), &nakMSG, sizeof(nakMSG));
		GuildRightUpdateQuery::FREE(pQResult);
		pQResult = NULL;
		return;
	}

	MSG_DZ_GUILD_RIGHT_DELEGATION_ACK ackMSG;
	ackMSG.m_dwKey				= pQResult->GetUserKey();
	ackMSG.m_dwSessionIndex		= pQResult->GetSessionIndex();
	ackMSG.m_GuildGuid			= pQResult->GetGuildGuid();
	ackMSG.m_TargetGuid			= pQResult->GetTargetCharGuid();
	pQResult->GetDelegateRight( ackMSG.m_DelegateRight, sizeof(ackMSG.m_DelegateRight) );
    Handler_Guild_DbpToGuild::OnDZ_GUILD_RIGHT_DELEGATION_ACK(
        g_DBHandler.GetSharedSession(), &ackMSG, sizeof(ackMSG));
	GuildRightUpdateQuery::FREE(pQResult);
	pQResult = NULL;
}

Handler_Guild_GuildToDbp_IMPL( DZ_GUILD_CHANGE_MASTER_SYN )
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
	MSG_DZ_GUILD_CHANGE_MASTER_SYN* pRecvMsg = (MSG_DZ_GUILD_CHANGE_MASTER_SYN *)pMsg;

	TCHAR szQueryBuffer[MAX_QUERY_LENGTH_128+1];
	_sntprintf(szQueryBuffer, MAX_QUERY_LENGTH_128, "{?=call S_Guild_ChangeMaster(%d, %d, %d)}",
		pRecvMsg->m_GuildGuid, pRecvMsg->m_OperCharGuid, pRecvMsg->m_TargetCharGuid );
	szQueryBuffer[MAX_QUERY_LENGTH_128]='\0';

	ChangeGuildMasterQuery *pQuery = ChangeGuildMasterQuery::ALLOC();
	pQuery->SetGuildGuid(pRecvMsg->m_GuildGuid);
	pQuery->SetOperCharGuid(pRecvMsg->m_OperCharGuid);
	pQuery->SetTargetCharGuid(pRecvMsg->m_TargetCharGuid);

	pQuery->SetUserKey(pRecvMsg->m_dwKey);
	pQuery->SetSessionIndex(pRecvMsg->m_dwSessionIndex);
	pQuery->SetQuery(szQueryBuffer);
	pQuery->SetIndex(MAKEDWORD((WORD)DZ_GUILD, (WORD)DZ_GUILD_CHANGE_MASTER_DBR));
	pQuery->SetVoidObject(pServerSession);
	pServerSession->DBQuery(pQuery);
}

Handler_Guild_GuildToDbp_IMPL( DZ_GUILD_CHANGE_MASTER_DBR )
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
	MSG_DBPROXY_RESULT* pResult		= (MSG_DBPROXY_RESULT*)pMsg;
	ChangeGuildMasterQuery* pQResult	= (ChangeGuildMasterQuery*)pResult->m_pData;

	if(!pQResult->ResultSuccess())
	{
		MSG_DZ_GUILD_CHANGE_MASTER_NAK nakMSG;
		nakMSG.m_dwKey			= pQResult->GetUserKey();
		nakMSG.m_dwSessionIndex = pQResult->GetSessionIndex();
		//1: ?
		//2: 유효한 캐릭터가 아닐때
		//3: 유효한 길드가 아닐때
		//4: 유효한 길드멤버가 아닐때
		//5: 마스터를 변경할 만한 직책이 아닐때.
		//6: 14일 이내에 마스터를 변경 하려고 할때.
		if( pQResult->ResultCode() == 1 )
			nakMSG.m_byErrorCode	= RC::RC_GUILD_FAILED;
		else if(  pQResult->ResultCode() == 2 )
			nakMSG.m_byErrorCode	= RC::RC_GUILD_NOT_GUILD_MEMBER;
		else if(  pQResult->ResultCode() == 3 )
			nakMSG.m_byErrorCode	= RC::RC_GUILD_NOT_EXIST_GUILD;
		else if(  pQResult->ResultCode() == 4 )
			nakMSG.m_byErrorCode	= RC::RC_GUILD_NOT_GUILD_MEMBER;
		else if(  pQResult->ResultCode() == 5 )
			nakMSG.m_byErrorCode	= RC::RC_GUILD_HAVE_NOT_RIGHT;
		else if(  pQResult->ResultCode() == 6 )
			nakMSG.m_byErrorCode	= RC::RC_GUILD_CANNOT_CHANGE_MASTER_IN_DAY_14;
		else
			nakMSG.m_byErrorCode	= RC::RC_GUILD_FAILED;

        Handler_Guild_DbpToGuild::OnDZ_GUILD_CHANGE_MASTER_NAK(
            g_DBHandler.GetSharedSession(), &nakMSG, sizeof(nakMSG));
		ChangeGuildMasterQuery::FREE(pQResult);
		pQResult = NULL;
		return;
	}

	MSG_DZ_GUILD_CHANGE_MASTER_ACK ackMSG;
	ackMSG.m_dwKey				= pQResult->GetUserKey();
	ackMSG.m_dwSessionIndex		= pQResult->GetSessionIndex();
	ackMSG.m_GuildGuid			= pQResult->GetGuildGuid();
	ackMSG.m_OldMasterCharGuid	= pQResult->GetOperCharGuid();
	ackMSG.m_NewMasterCharGuid	= pQResult->GetTargetCharGuid();
	ackMSG.m_nMasterChangeTime  = util::ConvertTCharToDWORD64( pQResult->pResult[0].m_tszMasterChangeTime );
    Handler_Guild_DbpToGuild::OnDZ_GUILD_CHANGE_MASTER_ACK(
        g_DBHandler.GetSharedSession(), &ackMSG, sizeof(ackMSG));
	ChangeGuildMasterQuery::FREE(pQResult);
	pQResult = NULL;
}



Handler_Guild_GuildToDbp_IMPL(DZ_GUILD_RELATION_INFO_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
	MSG_DZ_GUILD_RELATION_INFO_SYN *pRecvMsg = (MSG_DZ_GUILD_RELATION_INFO_SYN*)pMsg;

	STRING_SAFER_QUERY64 szQueryBuff;
	szQueryBuff.MakeString( "{?=call S_GuildRelation_Select(%d)}", pRecvMsg->m_GuildGuid );

	SelectGuildRelationQuery* pQuery = SelectGuildRelationQuery::ALLOC();
	pQuery->SetUserKey(pRecvMsg->m_dwKey);
	pQuery->SetGuildGuid(pRecvMsg->m_GuildGuid);
	pQuery->SetSessionIndex(pRecvMsg->m_dwSessionIndex);

	pQuery->SetQuery(szQueryBuff);
	pQuery->SetIndex(MAKEDWORD( (WORD)DZ_GUILD, (WORD)DZ_GUILD_SELECT_RELATION_DBR ) );
	pQuery->SetVoidObject( pServerSession );
	pServerSession->DBQuery( pQuery );
}

Handler_Guild_GuildToDbp_IMPL(DZ_GUILD_SELECT_RELATION_DBR)
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
	MSG_DBPROXY_RESULT	 *pResult = reinterpret_cast<MSG_DBPROXY_RESULT *>(pMsg);
	SelectGuildRelationQuery *pQResult = reinterpret_cast<SelectGuildRelationQuery *>(pResult->m_pData);

	if(!pQResult->ResultSuccess())
	{
		MSG_DZ_GUILD_RELATION_INFO_NAK msgNAK;
		msgNAK.m_dwKey = pQResult->GetUserKey();
		msgNAK.m_GuildGuid = pQResult->GetGuildGuid();
		msgNAK.m_byErrorCode = pQResult->ResultCode();
		msgNAK.m_dwSessionIndex = pQResult->GetSessionIndex();
        Handler_Guild_DbpToGuild::OnDZ_GUILD_RELATION_INFO_NAK(
            g_DBHandler.GetSharedSession(), &msgNAK, sizeof(msgNAK));
		SelectGuildRelationQuery::FREE(pQResult); pQResult = NULL;
		return;
	}

	MSG_DZ_GUILD_RELATION_INFO_ACK msg;
	msg.m_dwKey = pQResult->GetUserKey();
	msg.m_AddInfo.m_GuildGuid = pQResult->GetGuildGuid();
	msg.m_byRelCnt = (BYTE)pQResult->GetResultRowNum(); //255개 넘으면 어쩔건데..
	msg.m_dwSessionIndex = pQResult->GetSessionIndex();
	memset(msg.m_AddInfo.m_stRelationGuild, NULL, sizeof(GUILD_RELATION) * msg.m_byRelCnt);

	BYTE byAllyCnt = 0;	
	BYTE byHostCnt = 0;
	BYTE byOneSideCnt = 0;
	for(BYTE cnt = 0; cnt < msg.m_byRelCnt; ++cnt)
	{
		msg.m_AddInfo.m_stRelationGuild[cnt].m_GuildGuid = pQResult->pResult[cnt].RelGuildGuid;
		msg.m_AddInfo.m_stRelationGuild[cnt].m_eRelationType = pQResult->pResult[cnt].eRelationType;
		msg.m_AddInfo.m_stRelationGuild[cnt].m_eProClaim = pQResult->pResult[cnt].m_eProcClaim;
		ZeroMemory(msg.m_AddInfo.m_stRelationGuild[cnt].m_GuildName, MAX_GUILDNAME_LENGTH);
		memcpy(msg.m_AddInfo.m_stRelationGuild[cnt].m_GuildName,pQResult->pResult[cnt].tszRelGuildName, MAX_GUILDNAME_LENGTH * sizeof(TCHAR));
		msg.m_AddInfo.m_stRelationGuild[cnt].m_GuildName[MAX_GUILDNAME_LENGTH] = '\0';
#ifdef _NA_003923_20120130_GUILD_RENEWAL
        msg.m_AddInfo.m_stRelationGuild[cnt].m_RelationApplyTime = 
            util::ConvertTMidTimeToDWORD64(pQResult->pResult[cnt].tszRelationApplyTime);
#endif

		switch(pQResult->pResult[cnt].eRelationType)
		{
		case eGUILD_RELATION_ALLIENCE: byAllyCnt++; break;
		case eGUILD_RELATION_ONE_SIDE_HOSTILITY: byOneSideCnt++; break;
		case eGUILD_RELATION_BOTH_HOSTILITY:
		case eGUILD_RELATION_PROCLAIM_HOSTILITY: 
			byHostCnt++; break;
		}
	}

	msg.m_AddInfo.m_byAllienceGuildCnt	= byAllyCnt;
	msg.m_AddInfo.m_byHostilityGuildCnt	= byHostCnt;
	msg.m_AddInfo.m_byOneSideGuildCnt	= byOneSideCnt;
	msg.m_AddInfo.m_byRelationGuildCnt = msg.m_byRelCnt;
    Handler_Guild_DbpToGuild::OnDZ_GUILD_RELATION_INFO_ACK(
        g_DBHandler.GetSharedSession(), &msg, msg.GetSize());
	SelectGuildRelationQuery::FREE(pQResult);	pQResult = NULL;
}

// 길드 관계 DB
Handler_Guild_GuildToDbp_IMPL(DZ_GUILD_RELATION_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
	MSG_DZ_GUILD_RELATION_SYN *pRecvMsg = (MSG_DZ_GUILD_RELATION_SYN*)pMsg;

	TCHAR szQueryBuff[MAX_QUERY_LENGTH_128+1];
	TCHAR tszGuildName[MAX_GUILDNAME_LENGTH+1];

    _tcsncpy(tszGuildName, pRecvMsg->m_ResponseGuildName, _countof(tszGuildName));
    tszGuildName[_countof(tszGuildName) - 1] = _T('\0');

	_sntprintf(
        szQueryBuff, 
        MAX_QUERY_LENGTH_128, 
        "{?=call S_GuildRelation_Insert(%d, %d, %d, '%s')}",
		pRecvMsg->m_eRelationType, 
        pRecvMsg->m_RequestGuildGuid, 
        pRecvMsg->m_ResponseGuildGuid, 
        pRecvMsg->m_ResponseGuildName
        );
	szQueryBuff[MAX_QUERY_LENGTH_128]='\0';

	RelationGuildQuery *pQuery = RelationGuildQuery::ALLOC();

	pQuery->SetUserKey(pRecvMsg->m_dwKey);
	
    pQuery->SetResponseUserKey(pRecvMsg->m_dwKey);
	pQuery->SetRequestUserKey(pRecvMsg->m_dwKey);

	pQuery->SetRelationType(pRecvMsg->m_eRelationType);
	pQuery->SetGuildGuid(pRecvMsg->m_RequestGuildGuid);
	pQuery->SetRelationGuild(pRecvMsg->m_ResponseGuildGuid);
	pQuery->SetRelationGuildName(tszGuildName);
	pQuery->SetSessionIndex(pRecvMsg->m_dwSessionIndex);

	pQuery->SetQuery(szQueryBuff);
	pQuery->SetIndex(MAKEDWORD((WORD)DZ_GUILD, (WORD)DZ_GUILD_RELATION_DBR));
	pQuery->SetVoidObject(pServerSession);
	pServerSession->DBQuery(pQuery);
}

// DB 쿼리
Handler_Guild_GuildToDbp_IMPL(DZ_GUILD_RELATION_DBR)
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
	MSG_DBPROXY_RESULT *pResult = (MSG_DBPROXY_RESULT*)pMsg;
	RelationGuildQuery *pQResult = (RelationGuildQuery*)pResult->m_pData;

	if(!pQResult->ResultSuccess())
	{	
		MSG_DZ_GUILD_RELATION_NAK msgNAK;
		msgNAK.m_dwKey	= pQResult->GetUserKey();
        msgNAK.m_dwSessionIndex = pQResult->GetSessionIndex();
		msgNAK.m_RequestUserKey = pQResult->GetRequestUserKey();
		msgNAK.m_ResponseUserKey = pQResult->GetResponseUserKey();
		msgNAK.m_byErrorCode = pQResult->GetGuildResult(pQResult->ResultCode());
        Handler_Guild_DbpToGuild::OnDZ_GUILD_RELATION_NAK(
            g_DBHandler.GetSharedSession(), &msgNAK, sizeof(msgNAK));
		RelationGuildQuery::FREE(pQResult);
		pQResult = NULL;
		return;
	}

	MSG_DZ_GUILD_RELATION_ACK msgACK;

	msgACK.m_dwKey	= pQResult->GetUserKey();
	msgACK.m_dwSessionIndex = pQResult->GetSessionIndex();
	msgACK.m_eRelationType	= pQResult->GetRelationType();
	msgACK.m_GuildGuid = pQResult->GetGuildGuid();
	msgACK.m_RelationGuildGuid = (pQResult->GetRelationGuild() == 0) ?  
        pQResult->pResult[0].m_RELGuildGuid : pQResult->GetRelationGuild();
	memset(msgACK.m_tszRELGuildName, 0, MAX_GUILDNAME_LENGTH);
    strncpy(msgACK.m_tszRELGuildName, pQResult->pResult[0].tszRelGuildName,
            _countof(msgACK.m_tszRELGuildName));
    msgACK.m_tszRELGuildName[_countof(msgACK.m_tszRELGuildName) - 1] = '\0';
#ifdef _NA_003923_20120130_GUILD_RENEWAL
    msgACK.m_RelationApplyTime = 
        util::ConvertTMidTimeToDWORD64(pQResult->pResult[0].relation_apply_time);
#endif

    Handler_Guild_DbpToGuild::OnDZ_GUILD_RELATION_ACK(
        g_DBHandler.GetSharedSession(), &msgACK, sizeof(msgACK));
	RelationGuildQuery::FREE(pQResult);
	pQResult = NULL;
}

Handler_Guild_GuildToDbp_IMPL(DZ_GUILD_RELATION_DELETE_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
	MSG_DZ_GUILD_RELATION_DELETE_SYN *pRecvMsg = (MSG_DZ_GUILD_RELATION_DELETE_SYN *)pMsg;

	TCHAR szQueryBuff[MAX_QUERY_LENGTH_128+1];	

	_sntprintf(szQueryBuff, MAX_QUERY_LENGTH_128, "{?=call S_GuildRelation_Delete(%d, %d, %d)}",
		pRecvMsg->m_eType, pRecvMsg->m_GuildGuid, pRecvMsg->m_RelGuildGuid);

	RelationGuildQuery *pQuery = RelationGuildQuery::ALLOC();

	pQuery->SetRelationType(pRecvMsg->m_eType);
	pQuery->SetGuildGuid(pRecvMsg->m_GuildGuid);
	pQuery->SetRelationGuild(pRecvMsg->m_RelGuildGuid);
	//pQuery->SetRelationGuildName(pRecvMsg->m_tszRelationGuildName);
	pQuery->SetUserKey(pRecvMsg->m_dwKey);
	pQuery->SetSessionIndex(pRecvMsg->m_dwSessionIndex);

	pQuery->SetQuery(szQueryBuff);
	pQuery->SetIndex(MAKEDWORD((WORD)DZ_GUILD, (WORD)DZ_GUILD_RELATION_DELETE_DBR));
	pQuery->SetVoidObject(pServerSession);
	pServerSession->DBQuery(pQuery);
}

Handler_Guild_GuildToDbp_IMPL(DZ_GUILD_RELATION_DELETE_DBR)
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
	MSG_DBPROXY_RESULT *pResult = (MSG_DBPROXY_RESULT*)pMsg;
	RelationGuildQuery *pQResult = (RelationGuildQuery*)pResult->m_pData;

	if(!pQResult->ResultSuccess())
	{
		MSG_DZ_GUILD_RELATION_DELETE_NAK msgNAK;
		msgNAK.m_dwKey = pQResult->GetUserKey();
		msgNAK.m_dwSessionIndex	= pQResult->GetSessionIndex();
		msgNAK.m_eType = pQResult->GetRelationType();
		msgNAK.m_GuildGuid = pQResult->GetGuildGuid();
		msgNAK.m_RelGuildGuid = pQResult->GetRelationGuild();
		msgNAK.m_byErrorCode = (pQResult->ResultCode() == 3) ? RC::RC_GUILD_RELATION_DELETE_FAILED_BY_TIME : RC::RC_GUILD_RELATION_DELETE_FAILED;
        Handler_Guild_DbpToGuild::OnDZ_GUILD_RELATION_DELETE_NAK(
            g_DBHandler.GetSharedSession(), &msgNAK, sizeof(msgNAK));
		RelationGuildQuery::FREE(pQResult); pQResult = NULL;
		return;
	}

	MSG_DZ_GUILD_RELATION_DELETE_ACK msgACK;
	msgACK.m_dwKey = pQResult->GetUserKey();
	msgACK.m_eType = pQResult->GetRelationType();
	msgACK.m_GuildGuid = pQResult->GetGuildGuid();
	msgACK.m_RelGuildGuid = pQResult->GetRelationGuild();
	msgACK.m_dwSessionIndex	= pQResult->GetSessionIndex();
    Handler_Guild_DbpToGuild::OnDZ_GUILD_RELATION_DELETE_ACK(
        g_DBHandler.GetSharedSession(), &msgACK, sizeof(msgACK));
	RelationGuildQuery::FREE(pQResult); pQResult = NULL;
}

//쌍방 적대 신청
Handler_Guild_GuildToDbp_IMPL(DZ_GUILD_RELATION_BOTH_AGAINST_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
    MSG_DZ_GUILD_RELATION_BOTH_AGAINST_SYN *pRecvMsg = (MSG_DZ_GUILD_RELATION_BOTH_AGAINST_SYN *)pMsg;

	TCHAR szQueryBuff[MAX_QUERY_LENGTH_128+1];
	_sntprintf(
        szQueryBuff,
        MAX_QUERY_LENGTH_128, 
        "{?=call S_GuildRelation_Both_Against(%d, %d, '%s')}",
        pRecvMsg->m_GuildGuid, 
        pRecvMsg->m_RelGuildGuid,
        pRecvMsg->m_tszRelGuildName
        );

	RelationBothAgainstGuildQuery *pQuery = RelationBothAgainstGuildQuery::ALLOC();
	pQuery->SetRelationType(eGUILD_RELATION_BOTH_HOSTILITY);
	pQuery->SetGuildGuid(pRecvMsg->m_GuildGuid);
	pQuery->SetRelationGuild(pRecvMsg->m_RelGuildGuid);
	pQuery->SetRelationGuildName(pRecvMsg->m_tszRelGuildName);
	pQuery->SetUserKey(pRecvMsg->m_dwKey);
	pQuery->SetSessionIndex(pRecvMsg->m_dwSessionIndex);
	pQuery->SetQuery(szQueryBuff);
	pQuery->SetIndex(MAKEDWORD((WORD)DZ_GUILD, (WORD)DZ_GUILD_RELATION_BOTH_AGAINST_DBR));
	pQuery->SetVoidObject(pServerSession);
	pServerSession->DBQuery(pQuery);
}

Handler_Guild_GuildToDbp_IMPL(DZ_GUILD_RELATION_BOTH_AGAINST_DBR)
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
	MSG_DBPROXY_RESULT *pResult = (MSG_DBPROXY_RESULT*)pMsg;
	RelationBothAgainstGuildQuery *pQResult = (RelationBothAgainstGuildQuery*)pResult->m_pData;

	if(!pQResult->ResultSuccess())
	{
		MSG_DZ_GUILD_RELATION_BOTH_AGAINST_NAK msgNAK;
		msgNAK.m_dwKey	= pQResult->GetUserKey();
		msgNAK.m_dwSessionIndex	= pQResult->GetSessionIndex();
		msgNAK.m_eRelType	= pQResult->GetRelationType();
		msgNAK.m_GuildGuid = pQResult->GetGuildGuid();
		msgNAK.m_RelGuildGuid = pQResult->GetRelationGuild();
		msgNAK.m_byErrorCode = pQResult->GetGuildResult(pQResult->ResultCode());
        Handler_Guild_DbpToGuild::OnDZ_GUILD_RELATION_BOTH_AGAINST_NAK(
            g_DBHandler.GetSharedSession(), &msgNAK, sizeof(msgNAK));
		RelationBothAgainstGuildQuery::FREE(pQResult);
		pQResult = NULL;
		return;
	}

	MSG_DZ_GUILD_RELATION_BOTH_AGAINST_ACK msgACK;
	msgACK.m_dwKey			= pQResult->GetUserKey();
	msgACK.m_dwSessionIndex	= pQResult->GetSessionIndex();
	msgACK.m_eRelType		= pQResult->GetRelationType();
	msgACK.m_GuildGuid		= pQResult->GetGuildGuid();
	msgACK.m_RelGuildGuid	= pQResult->GetRelationGuild();
#ifdef _NA_003923_20120130_GUILD_RENEWAL
    msgACK.m_RelationApplyTime = 
        util::ConvertTMidTimeToDWORD64(pQResult->pResult[0].relation_apply_time);
#endif

    Handler_Guild_DbpToGuild::OnDZ_GUILD_RELATION_BOTH_AGAINST_ACK(
        g_DBHandler.GetSharedSession(), &msgACK, sizeof(msgACK));
	RelationBothAgainstGuildQuery::FREE(pQResult); pQResult = NULL;
}


//길드 군단 생성
Handler_Guild_GuildToDbp_IMPL( DZ_GUILD_CREATE_CORPS_SYN )
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
	MSG_DZ_GUILD_CREATE_CORPS_SYN *pRecvMsg = (MSG_DZ_GUILD_CREATE_CORPS_SYN *)pMsg;

	TCHAR szQueryBuffer[MAX_QUERY_LENGTH_128+1];

	_sntprintf(szQueryBuffer, MAX_QUERY_LENGTH_128, "{?=call S_GuildCorps_Create(%d, %d, %d)}",
		pRecvMsg->m_GuildGuid, pRecvMsg->m_byCorpsNum, pRecvMsg->m_TargetCharGuid );

	CorpsCreateQuery *pQuery = CorpsCreateQuery::ALLOC();
	pQuery->SetUserKey(pRecvMsg->m_dwKey);
	pQuery->SetSessionIndex(pRecvMsg->m_dwSessionIndex);
	pQuery->SetGuildGuid(pRecvMsg->m_GuildGuid);	
	pQuery->SetCorpsNum(pRecvMsg->m_byCorpsNum);
	pQuery->SetTargetCharGuid( pRecvMsg->m_TargetCharGuid );

	pQuery->SetQuery(szQueryBuffer);
	pQuery->SetIndex(MAKEDWORD((WORD)DZ_GUILD, (WORD)DZ_GUILD_CREATE_CORPS_DBR));
	pQuery->SetVoidObject(pServerSession);
	pServerSession->DBQuery(pQuery);
}

Handler_Guild_GuildToDbp_IMPL( DZ_GUILD_CREATE_CORPS_DBR )
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
	MSG_DBPROXY_RESULT *pResult = (MSG_DBPROXY_RESULT *)pMsg;
	CorpsCreateQuery *pQResult = (CorpsCreateQuery *)pResult->m_pData;

	if(!pQResult->ResultSuccess())
	{
		MSG_DZ_GUILD_CREATE_CORPS_NAK nakMSG;
		nakMSG.m_dwKey			= pQResult->GetUserKey();
		nakMSG.m_dwSessionIndex	= pQResult->GetSessionIndex();
		nakMSG.m_GuildGuid		= pQResult->GetGuildGuid();
//		1 : 이미 군단이 존재 한다.
//		2 : 군단장 대상이 길드멤버가 아니다.
//		3 : 이미 해당군단에 군단장이 존재한다.
		nakMSG.m_byErrorCode	= 0;
        Handler_Guild_DbpToGuild::OnDZ_GUILD_CREATE_CORPS_NAK(
            g_DBHandler.GetSharedSession(), &nakMSG, sizeof(nakMSG));
		CorpsCreateQuery::FREE(pQResult);
		pQResult = NULL;
		return;
	}

	MSG_DZ_GUILD_CREATE_CORPS_ACK ackMSG;
	ackMSG.m_dwKey				= pQResult->GetUserKey();
	ackMSG.m_dwSessionIndex		= pQResult->GetSessionIndex();
	ackMSG.m_GuildGuid			= pQResult->GetGuildGuid();
	ackMSG.m_byCorpsNum			= pQResult->GetCorpsNum();	
	ackMSG.m_TargetCharGuid		= pQResult->GetTargetCharGuid();
    Handler_Guild_DbpToGuild::OnDZ_GUILD_CREATE_CORPS_ACK(
        g_DBHandler.GetSharedSession(), &ackMSG, sizeof(ackMSG));
	CorpsCreateQuery::FREE(pQResult);
	pQResult = NULL;
}

//군단해산
Handler_Guild_GuildToDbp_IMPL( DZ_GUILD_DESTROY_CORPS_SYN )
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
	MSG_DZ_GUILD_DESTROY_CORPS_SYN* pRecvMsg = (MSG_DZ_GUILD_DESTROY_CORPS_SYN*)pMsg;

	TCHAR szQueryBuffer[MAX_QUERY_LENGTH_128+1];

	_sntprintf(szQueryBuffer, MAX_QUERY_LENGTH_128, "{?=call S_GuildCorps_Destroy(%d, %d)}",
		pRecvMsg->m_GuildGuid, pRecvMsg->m_byCorpsNum);

	CorpsDestroyQuery* pQuery = CorpsDestroyQuery::ALLOC();
	pQuery->SetGuildGuid(pRecvMsg->m_GuildGuid);	
	pQuery->SetCorpsNum(pRecvMsg->m_byCorpsNum);
	pQuery->SetUserKey(pRecvMsg->m_dwKey);
	pQuery->SetOperCharGuid(pRecvMsg->m_OperCharGuid);
	pQuery->SetSessionIndex(pRecvMsg->m_dwSessionIndex);

	pQuery->SetQuery(szQueryBuffer);
	pQuery->SetIndex(MAKEDWORD((WORD)DZ_GUILD, (WORD)DZ_GUILD_DESTROY_CORPS_DBR));
	pQuery->SetVoidObject(pServerSession);
	pServerSession->DBQuery(pQuery);
}

Handler_Guild_GuildToDbp_IMPL( DZ_GUILD_DESTROY_CORPS_DBR )
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
	MSG_DBPROXY_RESULT *pResult = (MSG_DBPROXY_RESULT *)pMsg;
	CorpsDestroyQuery* pQResult = (CorpsDestroyQuery *)pResult->m_pData;

	if(!pQResult->ResultSuccess())
	{
		MSG_DZ_GUILD_CREATE_CORPS_NAK nakMSG;
		nakMSG.m_dwKey			= pQResult->GetUserKey();
		nakMSG.m_dwSessionIndex	= pQResult->GetSessionIndex();
		nakMSG.m_GuildGuid		= pQResult->GetGuildGuid();
		nakMSG.m_byErrorCode	= 0;
		//	1 : 군단이 존재하지 않습니다.
		//	2 : 군단에 길드 멤버가 존재 합니다.
		//	3 : 쿼리실행 에러.
        Handler_Guild_DbpToGuild::OnDZ_GUILD_CREATE_CORPS_NAK(
            g_DBHandler.GetSharedSession(), &nakMSG, sizeof(nakMSG));
		CorpsDestroyQuery::FREE(pQResult);
		pQResult = NULL;
		return;
	}

	MSG_DZ_GUILD_DESTROY_CORPS_ACK ackMSG;
	ackMSG.m_dwKey				= pQResult->GetUserKey();
	ackMSG.m_dwSessionIndex		= pQResult->GetSessionIndex();
	ackMSG.m_GuildGuid			= pQResult->GetGuildGuid();
	ackMSG.m_byCorpsNum			= pQResult->GetCorpsNum();	
	ackMSG.m_OperCharGuid = pQResult->GetOperCharGuid();
    Handler_Guild_DbpToGuild::OnDZ_GUILD_DESTROY_CORPS_ACK(
        g_DBHandler.GetSharedSession(), &ackMSG, sizeof(ackMSG));
	CorpsDestroyQuery::FREE(pQResult);
	pQResult = NULL;
}



//군단장 변경
Handler_Guild_GuildToDbp_IMPL( DZ_GUILD_CHANGE_COMMANDER_SYN )
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
	MSG_DZ_GUILD_CHANGE_COMMANDER_SYN* pRecvMsg = (MSG_DZ_GUILD_CHANGE_COMMANDER_SYN*)pMsg;

	TCHAR szQueryBuffer[MAX_QUERY_LENGTH_128+1];

	_sntprintf(szQueryBuffer, MAX_QUERY_LENGTH_128, "{?=call S_GuildCorps_ChangeCommander(%d, %d, %d)}",
		pRecvMsg->m_GuildGuid, pRecvMsg->m_byCorpsNum, pRecvMsg->m_TargetCharGuid);

	ChangeCommanderQuery *pQuery = ChangeCommanderQuery::ALLOC();
	pQuery->SetGuildGuid(pRecvMsg->m_GuildGuid);	
	pQuery->SetCharGuid(pRecvMsg->m_TargetCharGuid);
	pQuery->SetCorpsNum(pRecvMsg->m_byCorpsNum);
	pQuery->SetUserKey(pRecvMsg->m_dwKey);
	pQuery->SetSessionIndex(pRecvMsg->m_dwSessionIndex);

	pQuery->SetQuery(szQueryBuffer);
	pQuery->SetIndex(MAKEDWORD((WORD)DZ_GUILD, (WORD)DZ_GUILD_CHANGE_COMMANDER_DBR));
	pQuery->SetVoidObject(pServerSession);
	pServerSession->DBQuery(pQuery);
}

Handler_Guild_GuildToDbp_IMPL( DZ_GUILD_CHANGE_COMMANDER_DBR )
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
	MSG_DBPROXY_RESULT *pResult = (MSG_DBPROXY_RESULT *)pMsg;
	ChangeCommanderQuery* pQResult = (ChangeCommanderQuery *)pResult->m_pData;

	if(!pQResult->ResultSuccess())
	{
		MSG_DZ_GUILD_CHANGE_COMMANDER_NAK nakMSG;
		nakMSG.m_dwKey			= pQResult->GetUserKey();
		nakMSG.m_dwSessionIndex	= pQResult->GetSessionIndex();
		nakMSG.m_GuildGuid		= pQResult->GetGuildGuid();
		nakMSG.m_byErrorCode	= pQResult->ResultCode();
		//0 : 성공
		//1 : update 실패
		//2 : 군단이 존재하지 않습니다.
		//3 : 군단장이 존재합니다.
        Handler_Guild_DbpToGuild::OnDZ_GUILD_CHANGE_COMMANDER_NAK(
            g_DBHandler.GetSharedSession(), &nakMSG, sizeof(nakMSG));
		ChangeCommanderQuery::FREE(pQResult);
		pQResult = NULL;
		return;
	}

	MSG_DZ_GUILD_CHANGE_COMMANDER_ACK ackMSG;
	ackMSG.m_dwKey				= pQResult->GetUserKey();
	ackMSG.m_dwSessionIndex		= pQResult->GetSessionIndex();
	ackMSG.m_GuildGuid			= pQResult->GetGuildGuid();
	ackMSG.m_byCorpsNum			= pQResult->GetCorpsNum();	
	ackMSG.m_TargetGuid			= pQResult->GetCharGuid();
    Handler_Guild_DbpToGuild::OnDZ_GUILD_CHANGE_COMMANDER_ACK(
        g_DBHandler.GetSharedSession(), &ackMSG, sizeof(ackMSG));
	ChangeCommanderQuery::FREE(pQResult);
	pQResult = NULL;
}

//군단 변경
Handler_Guild_GuildToDbp_IMPL( DZ_GUILD_CHANGE_CORPS_SYN )
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
	MSG_DZ_GUILD_CHANGE_CORPS_SYN* pRecvMsg = (MSG_DZ_GUILD_CHANGE_CORPS_SYN*)pMsg;

	TCHAR szQueryBuffer[MAX_QUERY_LENGTH_128+1];

	_sntprintf(szQueryBuffer, MAX_QUERY_LENGTH_128, "{?=call S_GuildCorps_MoveMember(%u, %d, %d, %u)}",
		pRecvMsg->m_GuildGuid, pRecvMsg->m_bySourceCorpsNum, pRecvMsg->m_byTargetCorpsNum, pRecvMsg->m_CharGuid );

	ChangeCorpsQuery* pQuery = ChangeCorpsQuery::ALLOC();
	pQuery->SetGuildGuid(pRecvMsg->m_GuildGuid);	
	pQuery->SetCharGuid(pRecvMsg->m_CharGuid);
	pQuery->SetSourceCorpsNum(pRecvMsg->m_bySourceCorpsNum);
	pQuery->SetTargetCorpsNum(pRecvMsg->m_byTargetCorpsNum);
	pQuery->SetUserKey(pRecvMsg->m_dwKey);
	pQuery->SetSessionIndex(pRecvMsg->m_dwSessionIndex);

	pQuery->SetQuery(szQueryBuffer);
	pQuery->SetIndex(MAKEDWORD((WORD)DZ_GUILD, (WORD)DZ_GUILD_CHANGE_CORPS_DBR));
	pQuery->SetVoidObject(pServerSession);
	pServerSession->DBQuery(pQuery);
}

Handler_Guild_GuildToDbp_IMPL( DZ_GUILD_CHANGE_CORPS_DBR )
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
	MSG_DBPROXY_RESULT *pResult = (MSG_DBPROXY_RESULT *)pMsg;
	ChangeCorpsQuery* pQResult = (ChangeCorpsQuery *)pResult->m_pData;

	if(!pQResult->ResultSuccess())
	{
		MSG_DZ_GUILD_CHANGE_CORPS_NAK nakMSG;
		nakMSG.m_dwKey			= pQResult->GetUserKey();
		nakMSG.m_dwSessionIndex	= pQResult->GetSessionIndex();
		nakMSG.m_GuildGuid		= pQResult->GetGuildGuid();
		nakMSG.m_byErrorCode	= (pQResult->ResultCode() == 4) ? RC::RC_GUILD_CANNOT_MOVE_CORPS_BY_CORPS_PENALTY : pQResult->ResultCode();
		//0 : 성공
		//1 : 이미 해당 군단 입니다.
		//2 : UPDATE 실패
        Handler_Guild_DbpToGuild::OnDZ_GUILD_CHANGE_CORPS_NAK(
            g_DBHandler.GetSharedSession(), &nakMSG, sizeof(nakMSG));
		ChangeCorpsQuery::FREE(pQResult);
		pQResult = NULL;
		return;
	}

	MSG_DZ_GUILD_CHANGE_CORPS_ACK ackMSG;
	ackMSG.m_dwKey				= pQResult->GetUserKey();
	ackMSG.m_dwSessionIndex		= pQResult->GetSessionIndex();
	ackMSG.m_GuildGuid			= pQResult->GetGuildGuid();
	ackMSG.m_TargetGuid			= pQResult->GetCharGuid();
	ackMSG.m_byCorpsNum			= pQResult->GetTargetCorpsNum();	
    Handler_Guild_DbpToGuild::OnDZ_GUILD_CHANGE_CORPS_ACK(
        g_DBHandler.GetSharedSession(), &ackMSG, sizeof(ackMSG));
	ChangeCorpsQuery::FREE(pQResult);
	pQResult = NULL;
}

Handler_Guild_GuildToDbp_IMPL( DZ_GUILD_CREATE_REAR_CAMP_SYN )
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
	MSG_DZ_GUILD_CREATE_REAR_CAMP_SYN* pRecvMsg = (MSG_DZ_GUILD_CREATE_REAR_CAMP_SYN*)pMsg;

	TCHAR szQueryBuffer[MAX_QUERY_LENGTH_128+1];

	_sntprintf(szQueryBuffer, MAX_QUERY_LENGTH_128, "{?=call S_GuildCamp_Create(%d, %d, %d)}",
		pRecvMsg->m_GuildGuid, pRecvMsg->m_byCorpsNum, pRecvMsg->m_TargetCharGuid );			

	CreateRearCampQuery* pQuery = CreateRearCampQuery::ALLOC();
	pQuery->SetGuildGuid(pRecvMsg->m_GuildGuid);	
	pQuery->SetCorpsNum(pRecvMsg->m_byCorpsNum);
	pQuery->SetUserKey(pRecvMsg->m_dwKey);
	pQuery->SetTargetCharGuid( pRecvMsg->m_TargetCharGuid );
	pQuery->SetSessionIndex(pRecvMsg->m_dwSessionIndex);

	pQuery->SetQuery(szQueryBuffer);
	pQuery->SetIndex(MAKEDWORD((WORD)DZ_GUILD, (WORD)DZ_GUILD_CREATE_REAR_CAMP_DBR));
	pQuery->SetVoidObject(pServerSession);
	pServerSession->DBQuery(pQuery);
}

Handler_Guild_GuildToDbp_IMPL( DZ_GUILD_CREATE_REAR_CAMP_DBR )
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
	MSG_DBPROXY_RESULT *pResult = (MSG_DBPROXY_RESULT *)pMsg;
	CreateRearCampQuery* pQResult = (CreateRearCampQuery *)pResult->m_pData;

	if(!pQResult->ResultSuccess())
	{
		MSG_DZ_GUILD_CREATE_REAR_CAMP_NAK nakMSG;
		nakMSG.m_dwKey			= pQResult->GetUserKey();
		nakMSG.m_dwSessionIndex	= pQResult->GetSessionIndex();
		nakMSG.m_GuildGuid		= pQResult->GetGuildGuid();
		//1 : 군단이 존재 하지 않습니다.
		//2 : 육성캠프가 이미 존재합니다.
		//3 : 길드멤버가 존재하지 않습니다.
		nakMSG.m_byErrorCode	= 0;

        Handler_Guild_DbpToGuild::OnDZ_GUILD_CREATE_REAR_CAMP_NAK(
            g_DBHandler.GetSharedSession(), &nakMSG, sizeof(nakMSG));
		CreateRearCampQuery::FREE(pQResult);
		pQResult = NULL;
		return;
	}

	MSG_DZ_GUILD_CREATE_REAR_CAMP_ACK ackMSG;
	ackMSG.m_dwKey				= pQResult->GetUserKey();
	ackMSG.m_dwSessionIndex		= pQResult->GetSessionIndex();
	ackMSG.m_GuildGuid			= pQResult->GetGuildGuid();
	ackMSG.m_byCorpsNum			= pQResult->GetCorpsNum();	
	ackMSG.m_TargetCharGuid		= pQResult->GetTargetCharGuid();
    Handler_Guild_DbpToGuild::OnDZ_GUILD_CREATE_REAR_CAMP_ACK(
        g_DBHandler.GetSharedSession(), &ackMSG, sizeof(ackMSG));
	CreateRearCampQuery::FREE(pQResult);
	pQResult = NULL;
}

Handler_Guild_GuildToDbp_IMPL( DZ_GUILD_DESTROY_REAR_CAMP_SYN )
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
	MSG_DZ_GUILD_DESTROY_REAR_CAMP_SYN* pRecvMsg = (MSG_DZ_GUILD_DESTROY_REAR_CAMP_SYN*)pMsg;

	TCHAR szQueryBuffer[MAX_QUERY_LENGTH_128+1];

	_sntprintf(szQueryBuffer, MAX_QUERY_LENGTH_128, "{?=call S_GuildCamp_Destroy(%d, %d)}",
		pRecvMsg->m_GuildGuid, pRecvMsg->m_byCorpsNum );

	DestroyRearCampQuery* pQuery = DestroyRearCampQuery::ALLOC();
	pQuery->SetGuildGuid(pRecvMsg->m_GuildGuid);	
	pQuery->SetCorpsNum(pRecvMsg->m_byCorpsNum);
	pQuery->SetUserKey(pRecvMsg->m_dwKey);
	pQuery->SetOperCharGuid(pRecvMsg->m_OperCharGuid);
	pQuery->SetSessionIndex(pRecvMsg->m_dwSessionIndex);
	pQuery->SetQuery(szQueryBuffer);
	pQuery->SetIndex(MAKEDWORD((WORD)DZ_GUILD, (WORD)DZ_GUILD_DESTROY_REAR_CAMP_DBR));
	pQuery->SetVoidObject(pServerSession);
	pServerSession->DBQuery(pQuery);
}

Handler_Guild_GuildToDbp_IMPL( DZ_GUILD_DESTROY_REAR_CAMP_DBR )
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
	MSG_DBPROXY_RESULT *pResult = (MSG_DBPROXY_RESULT *)pMsg;
	DestroyRearCampQuery* pQResult = (DestroyRearCampQuery *)pResult->m_pData;

	if(!pQResult->ResultSuccess())
	{
		MSG_DZ_GUILD_DESTROY_REAR_CAMP_NAK nakMSG;
		nakMSG.m_dwKey			= pQResult->GetUserKey();
		nakMSG.m_dwSessionIndex	= pQResult->GetSessionIndex();
		nakMSG.m_GuildGuid		= pQResult->GetGuildGuid();
		//	1 : 육성캠프가 존재하지 않습니다.
		//	2 : 육성캠프에 길드멤버가 존재합니다.
		//	3 : 쿼리 실패
		nakMSG.m_byErrorCode	= 0;
        Handler_Guild_DbpToGuild::OnDZ_GUILD_DESTROY_REAR_CAMP_NAK(
            g_DBHandler.GetSharedSession(), &nakMSG, sizeof(nakMSG));
		DestroyRearCampQuery::FREE(pQResult);
		pQResult = NULL;
		return;
	}

	MSG_DZ_GUILD_DESTROY_REAR_CAMP_ACK ackMSG;
	ackMSG.m_dwKey				= pQResult->GetUserKey();
	ackMSG.m_dwSessionIndex		= pQResult->GetSessionIndex();
	ackMSG.m_GuildGuid			= pQResult->GetGuildGuid();
	ackMSG.m_byCorpsNum			= pQResult->GetCorpsNum();	
	ackMSG.m_OperCharGuid = pQResult->GetOperCharGuid();
    Handler_Guild_DbpToGuild::OnDZ_GUILD_DESTROY_REAR_CAMP_ACK(
        g_DBHandler.GetSharedSession(), &ackMSG, sizeof(ackMSG));
	DestroyRearCampQuery::FREE(pQResult);
	pQResult = NULL;
}

Handler_Guild_GuildToDbp_IMPL(DZ_GUILD_GM_GUILD_POINT_SYN )
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
	MSG_DZ_GUILD_GM_GUILD_POINT_SYN* pRecvMsg = (MSG_DZ_GUILD_GM_GUILD_POINT_SYN*)pMsg;

	TCHAR szQueryBuffer[MAX_QUERY_LENGTH_128+1];
	_sntprintf(szQueryBuffer, MAX_QUERY_LENGTH_128, "{?=call S_Guild_Point(%d, %d, %d)}", pRecvMsg->m_GuildGuid, pRecvMsg->m_UP, pRecvMsg->m_GP);

	GuildPointQuery* pQuery = GuildPointQuery::ALLOC();
	pQuery->SetUserKey(pRecvMsg->m_dwKey);
	pQuery->SetSessionIndex(pRecvMsg->m_dwSessionIndex);
	pQuery->SetGuildGuid(pRecvMsg->m_GuildGuid);	
	pQuery->SetUP( pRecvMsg->m_UP );
	pQuery->SetGP( pRecvMsg->m_GP );

	pQuery->SetQuery(szQueryBuffer);
	pQuery->SetIndex(MAKEDWORD((WORD)DZ_GUILD, (WORD)DZ_GUILD_GM_GUILD_POINT_DBR));
	pQuery->SetVoidObject(pServerSession);
	pServerSession->DBQuery(pQuery);
}

Handler_Guild_GuildToDbp_IMPL(DZ_GUILD_GM_GUILD_POINT_DBR )
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
	MSG_DBPROXY_RESULT *pResult = (MSG_DBPROXY_RESULT *)pMsg;
	GuildPointQuery* pQResult = (GuildPointQuery *)pResult->m_pData;

	if(!pQResult->ResultSuccess())
	{
		MSG_DZ_GUILD_GM_GUILD_POINT_NAK nakMSG;
		nakMSG.m_dwKey			= pQResult->GetUserKey();
		nakMSG.m_dwSessionIndex	= pQResult->GetSessionIndex();
		nakMSG.m_GuildGuid		= pQResult->GetGuildGuid();
		nakMSG.m_byErrorCode	= 0;
        Handler_Guild_DbpToGuild::OnDZ_GUILD_GM_GUILD_POINT_NAK(
            g_DBHandler.GetSharedSession(), &nakMSG, sizeof(nakMSG));
		GuildPointQuery::FREE(pQResult);
		pQResult = NULL;
		return;
	}

	MSG_DZ_GUILD_GM_GUILD_POINT_ACK ackMSG;
	ackMSG.m_dwKey				= pQResult->GetUserKey();
	ackMSG.m_dwSessionIndex		= pQResult->GetSessionIndex();
	ackMSG.m_GuildGuid			= pQResult->GetGuildGuid();
	ackMSG.m_UP					= pQResult->GetUP();
	ackMSG.m_GP					= pQResult->GetGP();
    Handler_Guild_DbpToGuild::OnDZ_GUILD_GM_GUILD_POINT_ACK(
        g_DBHandler.GetSharedSession(), &ackMSG, sizeof(ackMSG));
	GuildPointQuery::FREE(pQResult);
	pQResult = NULL;
}

Handler_Guild_GuildToDbp_IMPL(DZ_GUILD_RECOVERY_GUILD_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
	MSG_DZ_GUILD_RECOVERY_GUILD_SYN *pRecvMsg = (MSG_DZ_GUILD_RECOVERY_GUILD_SYN*)pMsg;

	TCHAR szQueryBuffer[MAX_QUERY_LENGTH_128+1];
	_sntprintf(szQueryBuffer, MAX_QUERY_LENGTH_128, "{?=call S_Guild_Recovery(%d, %d)}", pRecvMsg->m_GuildGuid, pRecvMsg->m_CharGuid);

	GuildRecoveryQuery *pQuery = GuildRecoveryQuery::ALLOC();
	pQuery->SetGuildGuid(pRecvMsg->m_GuildGuid);	
	pQuery->SetUserKey(pRecvMsg->m_dwKey);
	pQuery->SetSessionIndex(pRecvMsg->m_dwSessionIndex);
	pQuery->SetMasterGuid(pRecvMsg->m_CharGuid);
	pQuery->SetQuery(szQueryBuffer);
	pQuery->SetIndex(MAKEDWORD((WORD)DZ_GUILD, (WORD)DZ_GUILD_RECOVERY_GUILD_DBR));
	pQuery->SetVoidObject(pServerSession);
	pServerSession->DBQuery(pQuery);
}

Handler_Guild_GuildToDbp_IMPL(DZ_GUILD_RECOVERY_GUILD_DBR)
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
	MSG_DBPROXY_RESULT *pResult = (MSG_DBPROXY_RESULT *)pMsg;
	GuildRecoveryQuery *pQResult = (GuildRecoveryQuery *)pResult->m_pData;

	if(GuildRecoveryQuery::RETURNCODE_SUCCESS != pQResult->ResultCode())	
	{
		MSG_DZ_GUILD_RECOVERY_GUILD_NAK msgNAK;
		msgNAK.m_dwKey	 = pQResult->GetUserKey();
		msgNAK.m_dwSessionIndex	= pQResult->GetSessionIndex();
		msgNAK.m_GuildGuid = pQResult->GetGuildGuid();
		msgNAK.m_CharGuid = pQResult->GetMasterGuid();
		msgNAK.m_byErrorCode = RC::RC_GUILD_RECOVERY_FAILED;
        Handler_Guild_DbpToGuild::OnDZ_GUILD_RECOVERY_GUILD_NAK(
            g_DBHandler.GetSharedSession(), &msgNAK, sizeof(msgNAK));
		GuildRecoveryQuery::FREE(pQResult); pQResult = NULL;
		return;
	}

	MSG_DZ_GUILD_RECOVERY_GUILD_ACK	msgACK;
	msgACK.m_dwKey	= pQResult->GetUserKey();
	msgACK.m_dwSessionIndex = pQResult->GetSessionIndex();
	msgACK.m_GuildGuid = pQResult->GetGuildGuid();
	msgACK.m_CharGuid	 = pQResult->GetMasterGuid();
    Handler_Guild_DbpToGuild::OnDZ_GUILD_RECOVERY_GUILD_ACK(
        g_DBHandler.GetSharedSession(), &msgACK, sizeof(msgACK));
	GuildRecoveryQuery::FREE(pQResult); 	pQResult = NULL;
}


Handler_Guild_GuildToDbp_IMPL(DZ_GUILD_DESTROY_COMPLETE_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
	MSG_DZ_GUILD_DESTROY_COMPLETE_SYN* receive_msg = static_cast<MSG_DZ_GUILD_DESTROY_COMPLETE_SYN*>(pMsg);

#ifdef _USING_GUILD_DESTROY_LOGGING
    SUNLOG(eCRITICAL_LOG, 
        _T("|길드해산완료|Handler_Guild_GuildToDbp_IMPL(DZ_GUILD_DESTROY_COMPLETE_SYN)|MSG_DZ_GUILD_DESTROY_COMPLETE_SYN 받음|GuildGuid:%d, MasterCharGuid:%d, UserKey:%u, SessionIndex:%u|"), 
        receive_msg->m_GuildGuid, 
        receive_msg->m_MasterCharGuid, 
        receive_msg->m_dwKey, 
        receive_msg->m_dwSessionIndex);
#endif // _USING_GUILD_DESTROY_LOGGING

	STRING_SAFER_QUERY64 query_string;
	query_string.MakeString(_T("{?=call S_Guild_Destroy_Complete(%d, %d)}"), 
                            receive_msg->m_GuildGuid, receive_msg->m_MasterCharGuid);

	GuildDestroyCompleteQuery* query = GuildDestroyCompleteQuery::ALLOC();
	query->SetGuildGuid(receive_msg->m_GuildGuid);	
	query->SetUserKey(receive_msg->m_dwKey);
	query->SetSessionIndex(receive_msg->m_dwSessionIndex);
	query->SetMasterGuid(receive_msg->m_MasterCharGuid);
	query->SetQuery(query_string);
	query->SetIndex(MAKEDWORD(static_cast<WORD>(DZ_GUILD), static_cast<WORD>(DZ_GUILD_DESTROY_COMPLETE_DBR)));
	query->SetVoidObject(pServerSession);

	pServerSession->DBQuery(query);
}

Handler_Guild_GuildToDbp_IMPL(DZ_GUILD_DESTROY_COMPLETE_DBR)
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
	MSG_DBPROXY_RESULT* receive_msg = static_cast<MSG_DBPROXY_RESULT*>(pMsg);
	GuildDestroyCompleteQuery* query_result = static_cast<GuildDestroyCompleteQuery*>(receive_msg->m_pData);

	if (query_result->ResultCode() != GuildDestroyCompleteQuery::RETURNCODE_SUCCESS)	
	{
		MSG_DZ_GUILD_DESTROY_COMPLETE_NAK nak_msg;
		nak_msg.m_dwKey = query_result->pResult[0].m_GuildMasterUserKey;
		nak_msg.m_dwSessionIndex = query_result->GetSessionIndex();
		nak_msg.m_GuildGuid = query_result->GetGuildGuid();
		nak_msg.m_MasterCharGuid = query_result->GetMasterGuid();
		nak_msg.m_byErrorCode = 0;

#ifdef _USING_GUILD_DESTROY_LOGGING
        SUNLOG(eCRITICAL_LOG, 
            _T("|길드해산완료|Handler_Guild_GuildToDbp_IMPL(DZ_GUILD_DESTROY_COMPLETE_DBR)|MSG_DZ_GUILD_DESTROY_COMPLETE_NAK 보냄|PlayerKey:%u, SessionIndex:%u, GuildGuid:%d, MasterCharGuid:%d|"), 
               nak_msg.m_dwKey, 
               nak_msg.m_dwSessionIndex, 
               nak_msg.m_GuildGuid, 
               nak_msg.m_MasterCharGuid);
#endif // _USING_GUILD_DESTROY_LOGGING

        Handler_Guild_DbpToGuild::OnDZ_GUILD_DESTROY_COMPLETE_NAK(
            g_DBHandler.GetSharedSession(), &nak_msg, sizeof(nak_msg));

		SUNLOG(eFULL_LOG, 
            _T("GUILD_DESTROY_COMPLETE_DBR(길드 삭제 결과) DBERROR[GUILDGUID:%d] [ERRCODE:%d]"),
			query_result->GetGuildGuid(), query_result->pParam[0].m_Result);

		GuildDestroyCompleteQuery::FREE(query_result); 
        query_result = NULL;
        return;
	}
	
	SUNLOG(eFULL_LOG, 
        _T("GUILD_DESTROY_COMPLETE_DBR(길드 삭제 결과) SUCCESS [GUILDGUID:%d] "), 
        query_result->GetGuildGuid());

	g_GuildManagerInDBP.DestroyGuildWarehouse(query_result->GetGuildGuid());

	MSG_DZ_GUILD_DESTROY_COMPLETE_ACK msg;
	msg.m_dwKey	= query_result->pResult[0].m_GuildMasterUserKey;	
	msg.m_dwSessionIndex = query_result->GetSessionIndex();
	msg.m_GuildGuid = query_result->GetGuildGuid();
	msg.m_MasterCharGuid = query_result->GetMasterGuid();

#ifdef _USING_GUILD_DESTROY_LOGGING
    SUNLOG(eCRITICAL_LOG, 
        _T("|길드해산완료|Handler_Guild_GuildToDbp_IMPL(DZ_GUILD_DESTROY_COMPLETE_DBR)|MSG_DZ_GUILD_DESTROY_COMPLETE_ACK 보냄|PlayerKey:%u, SessionIndex:%u, GuildGuid:%d, MasterCharGuid:%d|"), 
           msg.m_dwKey, 
           msg.m_dwSessionIndex, 
           msg.m_GuildGuid, 
           msg.m_MasterCharGuid);
#endif // _USING_GUILD_DESTROY_LOGGING

    Handler_Guild_DbpToGuild::OnDZ_GUILD_DESTROY_COMPLETE_ACK(
        g_DBHandler.GetSharedSession(), &msg, sizeof(msg));
	
    GuildDestroyCompleteQuery::FREE(query_result);
	query_result = NULL;
}

Handler_Guild_GuildToDbp_IMPL(DZ_GUILD_WAREHOUSE_OPEN_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
	PACKET_PTR_CONVERT(MSG_DZ_GUILD_WAREHOUSE_OPEN_SYN, recv_msg, pMsg);
	DWORD user_guid = recv_msg->m_dwKey;
	GUILDGUID guild_guid = recv_msg->m_GuildGuid;
	CHARGUID char_requester = recv_msg->m_OperCharGuid;

	DBUser* db_user = g_DBUserManager.FindUser(user_guid);
	if(!db_user)
	{
		g_GuildManagerInDBP.OpenWarehouseError(user_guid, guild_guid, pServerSession,
                                            RC::RC_GUILD_WAREHOUSE_GET_CANNOT_DB,
                                            recv_msg->m_dwSessionIndex);
		SUNLOG(eCRITICAL_LOG, _T("|["__FUNCTION__"]|Msg=can't find user(%u)|"), user_guid);
		return;
	}

	GuildWarehouseInfo* guild_warehouse_info =  g_GuildManagerInDBP.FindGuildWarehouse(guild_guid);
	if (NULL == guild_warehouse_info) {
	    // 이 ResultCode는 임시로 사용
		g_GuildManagerInDBP.OpenWarehouseError(user_guid, guild_guid, pServerSession,
                                            RC::RC_GUILD_WAREHOUSE_PLAYER_DATA_DISCORD,
                                            recv_msg->m_dwSessionIndex);
		SUNLOG(eCRITICAL_LOG,
               _T("|["__FUNCTION__"]|")
               _T("Msg=can't find the guild warehouse|guild_guid=%u|"),
               guild_guid);
		return;
	}

    if (guild_warehouse_info->GetOpenLocker(NULL) || guild_warehouse_info->IsTransaction())
    {
        g_GuildManagerInDBP.OpenWarehouseError(user_guid, guild_guid, pServerSession,
                                            RC::RC_GUILD_WAREHOUSE_ALEADY_USGIN_PLAYER,
                                            recv_msg->m_dwSessionIndex);
        return;
    }
    else if (guild_warehouse_info->IsWarehouseLoaded())
    {
        MSG_DZ_GUILD_WAREHOUSE_OPEN_ACK msg_ack;
        msg_ack.m_dwKey = user_guid;
        //msg.m_ItemInfo.m_Count    = 0;
        msg_ack.m_GuildGuid = guild_guid;
        msg_ack.m_OperCharGuid = char_requester;
        msg_ack.m_dwSessionIndex = recv_msg->m_dwSessionIndex; // (WAVERIX) (090903) (CHANGES) 주석 제거
        msg_ack.m_SlotTabNum = guild_warehouse_info->GetSlotTabNum();
        msg_ack.m_WarehouseMoney = guild_warehouse_info->GetWarehouseMoney();
        // (f100527.4L)
        _GUILDWAREHOUSE_TOTAL_INFO* const guild_warehouse_item_block = &msg_ack.m_ItemInfo;
        guild_warehouse_item_block->Clear();
        guild_warehouse_info->LoadItemSlotInfo(SI_GUILDWAREHOUSE, \
            guild_warehouse_item_block->m_Slot, &guild_warehouse_item_block->m_Count);

        guild_warehouse_info->AcquireOpenLock(user_guid, char_requester);
        Handler_Guild_DbpToGuild::OnDZ_GUILD_WAREHOUSE_OPEN_ACK(
            g_DBHandler.GetSharedSession(), &msg_ack, msg_ack.GetSize());
        return;
    }
    else
    {
        bool requested = g_GuildManagerInDBP.RequestLoadWarehouse(*recv_msg, guild_warehouse_info,
                                                               pServerSession);
        if (requested == false)
        {
            // NOTE: not control fail state.
            SUNLOG(eCRITICAL_LOG,
                   _T("|["__FUNCTION__"]|Msg=unexpected status, User(%d), Char(%d), Guild(%d)|"),
                   user_guid, char_requester, guild_guid);
        }
    }
    //
    return;
}

Handler_Guild_GuildToDbp_IMPL(DZ_GUILD_WAREHOUSE_OPEN_DBR)
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
    const MSG_DBPROXY_RESULT* db_result = static_cast<MSG_DBPROXY_RESULT*>(pMsg);
    GuildWarehouseSelectQuery* query_result = \
        reinterpret_cast<GuildWarehouseSelectQuery*>(db_result->m_pData);

    g_GuildManagerInDBP.DBResult_Warehouse_Select(*query_result, pServerSession);
    ;{
        GuildWarehouseSelectQuery::FREE(query_result);
        query_result = NULL;
    };
    return;
}

Handler_Guild_GuildToDbp_IMPL(DZ_GUILD_WAREHOUSE_ITEMSLOTINFO_SELECT_INTER_DBR)
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();

    const MSG_DBPROXY_RESULT* db_result = static_cast<MSG_DBPROXY_RESULT*>(pMsg);
    Query_GuildWarehouse_ItemSlotInfo_Select* query_result = \
        reinterpret_cast<Query_GuildWarehouse_ItemSlotInfo_Select*>(db_result->m_pData);
    //
    struct AutoClosure {
        AutoClosure(Query_GuildWarehouse_ItemSlotInfo_Select* query_result)
            : query_result_(query_result)
        {}
        ~AutoClosure() {
            Query_GuildWarehouse_ItemSlotInfo_Select::FREE(query_result_);
        }
        Query_GuildWarehouse_ItemSlotInfo_Select* query_result_;
    } auto_closure(query_result);
    //
    GuildWarehouseInfo* const guild_warehouse_info = \
        g_GuildManagerInDBP.FindGuildWarehouse(query_result->guild_guid_);
    if (!FlowControl::FCAssert(guild_warehouse_info)) {
        return;
    };
    //
    bool success = g_GuildManagerInDBP.DBResult_WarehouseItemSlotInfo_Select(\
        guild_warehouse_info, *query_result, pServerSession);
    __TOUCHED(success);assert(success);
    //
    return;
}

Handler_Guild_GuildToDbp_IMPL(DZ_GUILD_NAME_CHANGE_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
	MSG_DZ_GUILD_NAME_CHANGE_SYN* pRecvMsg = (MSG_DZ_GUILD_NAME_CHANGE_SYN*)pMsg;
	GUILDGUID GuildGuid		= pRecvMsg->m_GuildGuid;
	CHARGUID OperCharGuid	= pRecvMsg->m_OperCharGuid;

	//마스터를 통해 들어오는 경우 
	//OperCharGuid => INVALID_CHARGUID(0)
	//dwKey		   => WopsKey;
	//TargetPos	   => INVALID_POSTYPE_VALUE(255);

	STRING_SAFER_QUERY128 szQueryBuff; // (WAVERIX) (090909) (BUG-FIX) 문자열 길이가 64를 넘을 수 있다.
	szQueryBuff.MakeString("{?=call S_Guild_ChangeGuildName(%u, %u, '%s')}", OperCharGuid, GuildGuid, pRecvMsg->m_tszGuildName);

	ChangeGuildName* pQuery = ChangeGuildName::ALLOC();

	pQuery->SetTargetPos(pRecvMsg->m_TargetPos);
	pQuery->SetCharGuid(OperCharGuid);
	pQuery->SetGuildGuid(GuildGuid);
	pQuery->SetGuildName(pRecvMsg->m_tszGuildName);
	pQuery->SetSessionIndex(pRecvMsg->m_dwSessionIndex);	
	pQuery->SetIndex(MAKEDWORD((WORD)DZ_GUILD, (WORD)DZ_GUILD_NAME_CHANGE_DBR));
	pQuery->SetVoidObject(pServerSession);
	pQuery->SetQuery(szQueryBuff);
    // NOTE: f110523.5L, if this messeage is received from MasterServer, 'm_dwKey' is the wops key.
	pQuery->SetUserKey(pRecvMsg->m_dwKey);
	pServerSession->DBQuery( pQuery );
}

Handler_Guild_GuildToDbp_IMPL(DZ_GUILD_NAME_CHANGE_DBR)
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
	MSG_DBPROXY_RESULT * pResult	= (MSG_DBPROXY_RESULT *)pMsg;
	ChangeGuildName * pQResult		= (ChangeGuildName *)pResult->m_pData;

	if( !pQResult->ResultSuccess() )
	{
		MSG_DZ_GUILD_NAME_CHANGE_NAK nmsg;
		nmsg.m_dwKey			= pQResult->GetUserKey();
		nmsg.m_dwSessionIndex	= pQResult->GetSessionIndex();
		nmsg.m_TargetPos		= pQResult->GetTargetPos();
		nmsg.m_OperCharGuid		= pQResult->GetCharGuid();
		
		switch(pQResult->GetResult())
		{
		case 1:	nmsg.m_byErrorCode = RC::RC_GUILD_NAME_CHANGE_CANNOT_DB_SYSTEM;			break;
		case 2:	nmsg.m_byErrorCode = RC::RC_GUILD_NAME_CHANGE_CANNOT_DB_SAME_GUILDNAME;	break;
		case 3:	nmsg.m_byErrorCode = RC::RC_GUILD_NAME_CHANGE_CANNOT_DB_BLANK_SPACE;	break;
		default:nmsg.m_byErrorCode = RC::RC_GUILD_NAME_CHANGE_CANNOT_DB;
		}

        Handler_Guild_DbpToGuild::OnDZ_GUILD_NAME_CHANGE_NAK(
            g_DBHandler.GetSharedSession(), &nmsg, sizeof(nmsg));
		ChangeGuildName::FREE(pQResult); pQResult = NULL;
		return;
	}

	MSG_DZ_GUILD_NAME_CHANGE_ACK msg;
    // NOTE: f110523.5L, if this messeage is received from MasterServer, 'm_dwKey' is the wops key.
	msg.m_dwKey				= pQResult->GetUserKey();
	msg.m_dwSessionIndex	= pQResult->GetSessionIndex();
	msg.m_TargetPos			= pQResult->GetTargetPos();
	msg.m_OperCharGuid		= pQResult->GetCharGuid();
	msg.m_GuildGuid			= pQResult->GetGuildGuid();
	_tcsncpy( msg.m_tszGuildName, pQResult->GetGuildName(), MAX_GUILDNAME_LENGTH );
	msg.m_tszGuildName[MAX_GUILDNAME_LENGTH] = '\0';

    Handler_Guild_DbpToGuild::OnDZ_GUILD_NAME_CHANGE_ACK(
        g_DBHandler.GetSharedSession(), &msg, sizeof(msg));

	ChangeGuildName::FREE(pQResult); pQResult = NULL;
}

/************************************************************************/
/*              지역 점령전 DBProxy 작업 부분                           */
/************************************************************************/
//_KR_001385_20091013_DOMINATION_BASICINFO_SETTING

Handler_Guild_GuildToDbp_IMPL(DZ_DOMINATION_INFO_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
    const MSG_DZ_DOMINATION_INFO_SYN* recv_msg = static_cast<MSG_DZ_DOMINATION_INFO_SYN*>(pMsg);
    __TOUCHED(recv_msg);
    STRING_SAFER_QUERY128 query_buff;
    query_buff.MakeString("{?=call S_DominationInfo_Select}");

    Query_Domination_Info_Select* query = Query_Domination_Info_Select::ALLOC();

    query->SetIndex(MAKEDWORD((WORD)DZ_GUILD, (WORD)DZ_DOMINATION_INFO_DBR));
    query->SetQuery(query_buff);
    query->SetVoidObject(pServerSession);
    pServerSession->DBQuery(query);
}

Handler_Guild_GuildToDbp_IMPL(DZ_DOMINATION_INFO_DBR)
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
    const MSG_DBPROXY_RESULT* db_result = (MSG_DBPROXY_RESULT*)pMsg;
    Query_Domination_Info_Select* query_result = 
        static_cast<Query_Domination_Info_Select*>(db_result->m_pData);

    const Query_Domination_Info_Select::sQUERY* result_records = query_result->pResult;

    if (query_result->IsError())
    {
        SUNLOG(eCRITICAL_LOG, "%s : %s", query_result->GetQuery(), query_result->GetErrorMsg());

        MSG_DZ_DOMINATION_INFO_ACK nak_msg;
        nak_msg.m_RsultCode = RC::DOMINATION_RESULT_FAIL;
        nak_msg.m_NumRecords = 0;
        Handler_Guild_DbpToGuild::OnDZ_DOMINATION_INFO_ACK(
            g_DBHandler.GetSharedSession(), &nak_msg, nak_msg.GetSize());

        Query_Domination_Info_Select::FREE(query_result);
        return;
    }

    MSG_DZ_DOMINATION_INFO_ACK send_msg;

    BYTE num_records = static_cast<BYTE>(query_result->GetResultRowNum());
    if (num_records > _countof(send_msg.m_DominationInfo))
    {
        assert (!"Query records num is over");
        SUNLOG(eCRITICAL_LOG, __FUNCTION__":Query records num is over : %d", num_records);
        Query_Domination_Info_Select::FREE(query_result);
        return;
    }
 
    send_msg.m_RsultCode = RC::DOMINATION_RESULT_SUCCESS;
    send_msg.m_NumRecords = num_records;

    for (WORD i = 0; i < num_records; ++i)
    {
        const Query_Domination_Info_Select::sQUERY& src_info = result_records[i];
        //insert data to domination area info
        DOMINATION_INFO* const dest_info = &send_msg.m_DominationInfo[i];
        ZeroMemory(dest_info, sizeof(*dest_info));
        ;{
            dest_info->m_FieldCode = static_cast<MAPCODE>(src_info.map_code);
            dest_info->m_TotalPoint = src_info.defense_wall_totalpoint;
            dest_info->m_DeffenseWallA = src_info.defense_wall_A;
            dest_info->m_DeffenseWallB = src_info.defense_wall_B;
            dest_info->m_DeffenseWallC = src_info.defense_wall_C;
            dest_info->m_BestCost = 0;
        };
        //insert data to defense guild info
        if (DOMINATION_BASIC_GUILD_INFO* const defense_guild = &dest_info->m_inGuildInfo)
        {
            defense_guild->m_GuildGuid = src_info.defense_guild_guid;
            _tcsncpy(defense_guild->m_tszGuildName, src_info.defense_guild_name,
                     _countof(defense_guild->m_tszGuildName));
            defense_guild->m_tszGuildName[_countof(defense_guild->m_tszGuildName) - 1] = '\0';
            defense_guild->m_GuildMasterGuid = src_info.defense_master_guid;
            defense_guild->m_BackGroundIndex = src_info.defense_background_index;
            defense_guild->m_PatternIndex = src_info.defense_pattern_index;
            _tcsncpy(defense_guild->m_tszGuildMaster, src_info.defense_master_name,
                     _countof(defense_guild->m_tszGuildMaster));
            defense_guild->m_tszGuildMaster[_countof(defense_guild->m_tszGuildMaster) - 1] = '\0';
        };        
        //insert data to attack guild info
        if (DOMINATION_BASIC_GUILD_INFO* const attack_guild = &dest_info->m_disGuildInfo)
        {
            attack_guild->m_GuildGuid = src_info.attack_guild_guid;
            _tcsncpy(attack_guild->m_tszGuildName, src_info.defense_master_name,
                     _countof(attack_guild->m_tszGuildMaster));
            attack_guild->m_tszGuildMaster[_countof(attack_guild->m_tszGuildMaster) - 1] = '\0';
        };
        //_NA_0_20100705_DOMINATION_EVENT
        //insert data to reward event 
        if (DOMINATION_REWARD_EVENT_INFO* const reward_event = &dest_info->m_rewardEvent)
        {
            reward_event->m_byEventStats = src_info.event_state;
            reward_event->m_wEventDayOfWeek = src_info.event_day_of_week;
            reward_event->m_dwEventTime = src_info.event_time;
        };
    }; //end 'for record in records'
    Handler_Guild_DbpToGuild::OnDZ_DOMINATION_INFO_ACK(
        g_DBHandler.GetSharedSession(), &send_msg, send_msg.GetSize());

    Query_Domination_Info_Select::FREE(query_result);
    query_result = NULL;

    return;
}


Handler_Guild_GuildToDbp_IMPL(DZ_DOMINATION_AUCTION_INFO_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
    const MSG_DZ_DOMINATION_AUCTION_INFO_SYN* recv_msg = 
        static_cast<MSG_DZ_DOMINATION_AUCTION_INFO_SYN*>(pMsg);

    STRING_SAFER_QUERY128 query_buff;
    query_buff.MakeString("{?=call S_DominationInfo_ActionInfo_Select(%d)}", recv_msg->m_FieldCode);

    Query_Domination_Info_Auction_Select* query = Query_Domination_Info_Auction_Select::ALLOC();
    query->SetMapCode(recv_msg->m_FieldCode);
    query->SetIndex(MAKEDWORD((WORD)DZ_GUILD, (WORD)DZ_DOMINATION_AUCTION_INFO_DBR));
    query->SetQuery(query_buff);
    query->SetVoidObject(pServerSession);
    pServerSession->DBQuery(query);
}

Handler_Guild_GuildToDbp_IMPL(DZ_DOMINATION_AUCTION_INFO_DBR)
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
    const MSG_DBPROXY_RESULT* db_result = static_cast<MSG_DBPROXY_RESULT*>(pMsg);
    Query_Domination_Info_Auction_Select* query_result = 
        static_cast<Query_Domination_Info_Auction_Select*>(db_result->m_pData);

    if (query_result->IsError())
    {
        SUNLOG(eCRITICAL_LOG, "%s : %s", query_result->GetQuery(), query_result->GetErrorMsg());
        Query_Domination_Info_Auction_Select::FREE(query_result);
        return;
    }

    const Query_Domination_Info_Auction_Select::sQUERY* records = query_result->pResult;

    MSG_DZ_DOMINATION_AUCTION_INFO_ACK send_msg;

    BYTE data_count = static_cast<BYTE>(query_result->GetResultRowNum());
    if (data_count > _countof(send_msg.m_DominationActionDBInfo))
    {
        assert(!"Auction data count is over");
        SUNLOG(eCRITICAL_LOG, "Auction data count is over : %d", data_count);
        Query_Domination_Info_Auction_Select::FREE(query_result);
        query_result = NULL;
        return;
    }

    send_msg.m_RsultCode = RC::DOMINATION_RESULT_SUCCESS;
    send_msg.m_MapCode = query_result->GetMapCode();
    send_msg.m_DataCount = data_count;

    for (BYTE i = 0; i < data_count; ++i)
    {
        const Query_Domination_Info_Auction_Select::sQUERY& src_info = records[i];
        DominationAuctionInfo* const dest_info = &send_msg.m_DominationActionDBInfo[i];//auction info
        DOMINATION_ACTION_INFO* const basic_info = &dest_info->m_ActionInfo;//auction basic info

        //input data to auction basic info
        basic_info->m_FieldCode = static_cast<MAPCODE>(src_info.map_code);
        basic_info->m_GuilGuid = src_info.guild_guid;
        basic_info->m_TenderCost = src_info.tender_cost;
        _tcsncpy(basic_info->m_tszGuildName, src_info.guild_name, 
                 _countof(basic_info->m_tszGuildName));
        basic_info->m_tszGuildName[_countof(basic_info->m_tszGuildName) - 1] = '\0';
        _tcsncpy(basic_info->m_tszActionApplyTime, src_info.auction_apply_time, 
                 _countof(basic_info->m_tszActionApplyTime));
        basic_info->m_tszActionApplyTime[_countof(basic_info->m_tszActionApplyTime) - 1] = '\0';

        //input data to auction info
        dest_info->m_BackGroundIndex = src_info.background_index;
        dest_info->m_PatternIndex = src_info.pattern_index;
        dest_info->m_MasterGuid = src_info.master_guid;
        _tcsncpy(dest_info->m_tszGuildMaster, src_info.master_name, 
                 _countof(dest_info->m_tszGuildMaster));
        dest_info->m_tszGuildMaster[_countof(dest_info->m_tszGuildMaster) - 1] = '\0';
    }
    Handler_Guild_DbpToGuild::OnDZ_DOMINATION_AUCTION_INFO_ACK(
        pServerSession, &send_msg, send_msg.GetSize());

    Query_Domination_Info_Auction_Select::FREE(query_result);
    query_result = NULL;

    return;
}

//_KR_001385_20091105_DOMINATION_UPDATEDOMINATIONINFO
//지역 점령전 관련 정보를 Update한다.
//DOMINATION_DISGUILD_SET    :최고경매 금액을 지운다 (보상 금액계산에 이용)
//DOMINATION_SUCCESS_DEFENCE :수성에 성공하였으므로 Guid를 0으로 셋팅한다.
//DOMINATION_SUCCESS_OFFENCE :공성에 성공하였으므로 위치를 바꾼뒤 공격 Guid를 0으로 셋팅
Handler_Guild_GuildToDbp_IMPL(DZ_DOMINATION_UPDATE_INFO_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
    const MSG_DZ_DOMINATION_UPDATE_INFO_SYN* pRecvMsg = 
        static_cast<MSG_DZ_DOMINATION_UPDATE_INFO_SYN*>(pMsg);

    const DOMINATION_INFO& curDominationInfo = pRecvMsg->m_DominationInfo;
    
    GUILDGUID inGuildGuid = curDominationInfo.m_inGuildInfo.m_GuildGuid ;
    GUILDGUID disGuildGuid = curDominationInfo.m_disGuildInfo.m_GuildGuid;

    switch (pRecvMsg->m_eSetType)
    {
    case DOMINATION_DISGUILD_SET:
        g_GuildManagerInDBP.DominationReturnMoneyDelete(pServerSession, curDominationInfo);
        break;

    case DOMINATION_SUCCESS_DEFENCE:
        disGuildGuid = 0;
        break;

    case DOMINATION_SUCCESS_OFFENCE:
        inGuildGuid = pRecvMsg->m_DominationInfo.m_disGuildInfo.m_GuildGuid;
        disGuildGuid = 0;
        break;
#ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION
    case DOMINATION_TURN_OVER:
        //턴오버 시에 각인 성공길드를 수성 길드에 저장한다.
        inGuildGuid = pRecvMsg->m_DominationInfo.m_disGuildInfo.m_GuildGuid;
        disGuildGuid = 0;
        break;
#endif  //_NA_007128_20140326_MONDSCHIEN_DOMINATION
#ifdef _NA_007488_20140811_MONDSCHUEN_INITIALIZE_N_REWARD
    case DOMINATION_INIT:
        // 게임 서버에 의한 초기화 // 몬트샤인 시작시 초기화를 위해 사용된다
        inGuildGuid = pRecvMsg->m_DominationInfo.m_disGuildInfo.m_GuildGuid;
        disGuildGuid = 0;
        break;
#endif // _NA_007488_20140811_MONDSCHUEN_INITIALIZE_N_REWARD
    default:
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"]Update Type Value Incorrect[MAP:%d][%s]<-[%s]",
               curDominationInfo.m_FieldCode,
               curDominationInfo.m_inGuildInfo.m_tszGuildName,
               curDominationInfo.m_disGuildInfo.m_tszGuildName);
    }

    STRING_SAFER_QUERY128 szQueryBuff;
    // changed by _NA_0_20100705_DOMINATION_EVENT
    szQueryBuff.MakeString("{?=call S_DominationInfo_Update(%d,%d,%d,%d,%d,%d,%d,%d,%d,%d)}",
                           pRecvMsg->m_DominationInfo.m_FieldCode,
                           inGuildGuid,
                           disGuildGuid,
                           pRecvMsg->m_DominationInfo.m_DeffenseWallA,
                           pRecvMsg->m_DominationInfo.m_DeffenseWallB,
                           pRecvMsg->m_DominationInfo.m_DeffenseWallC,
                           pRecvMsg->m_DominationInfo.m_TotalPoint,
                           pRecvMsg->m_DominationInfo.m_rewardEvent.m_byEventStats,
                           pRecvMsg->m_DominationInfo.m_rewardEvent.m_wEventDayOfWeek,
                           pRecvMsg->m_DominationInfo.m_rewardEvent.m_dwEventTime);
    Query_Domination_Info_Update* pQuery = Query_Domination_Info_Update::ALLOC();

    pQuery->SetDominationInfoType(pRecvMsg->m_eSetType);
    pQuery->SetDominationInfo(curDominationInfo);

    pQuery->SetIndex(MAKEDWORD((WORD)DZ_GUILD, (WORD)DZ_DOMINATION_UPDATE_INFO_DBR));
    pQuery->SetQuery(szQueryBuff);
    pQuery->SetVoidObject(pServerSession);
    pServerSession->DBQuery(pQuery);
}

Handler_Guild_GuildToDbp_IMPL(DZ_DOMINATION_UPDATE_INFO_DBR)
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
    const MSG_DBPROXY_RESULT* pResult = (MSG_DBPROXY_RESULT*)pMsg;
    Query_Domination_Info_Update* pQResult = 
        static_cast<Query_Domination_Info_Update*>(pResult->m_pData);

    MSG_DZ_DOMINATION_UPDATE_INFO_ACK sendMsg;

    if (pQResult->IsError() || !pQResult->ResultSuccess())
    {
        if (pQResult->IsError()) 
        {
            SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"](%s):%s", 
                pQResult->GetQuery(), pQResult->GetErrorMsg());

        }
        else 
        {
            SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] : ErrorCode[ %d ]", 
                pQResult->pParam[0].m_Result);
        }

        sendMsg.m_RsultCode = RC::DOMINATION_RESULT_FAIL;
        sendMsg.m_DominationInfo = pQResult->GetDominationInfo();

        Handler_Guild_DbpToGuild::OnDZ_DOMINATION_UPDATE_INFO_ACK(
            g_DBHandler.GetSharedSession(), &sendMsg, sizeof(sendMsg));

        Query_Domination_Info_Update::FREE(pQResult);
        pQResult = NULL;
        return;
    }

    sendMsg.m_RsultCode = RC::DOMINATION_RESULT_SUCCESS;
    sendMsg.m_DominationInfo = pQResult->GetDominationInfo();
    sendMsg.m_eSetType = pQResult->GetDominationInfoType();

    Handler_Guild_DbpToGuild::OnDZ_DOMINATION_UPDATE_INFO_ACK(
        g_DBHandler.GetSharedSession(), &sendMsg, sizeof(sendMsg));

    Query_Domination_Info_Update::FREE(pQResult);
    pQResult = NULL;

    return;
}

Handler_Guild_GuildToDbp_IMPL(DZ_DOMINATION_ACCEP_HEIM_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
    const MSG_DZ_DOMINATION_ACCEP_HEIM_SYN* recv_msg = 
        static_cast<MSG_DZ_DOMINATION_ACCEP_HEIM_SYN*>(pMsg);

    STRING_SAFER_QUERY128 query_string;
    query_string.MakeString( "{?=call S_DominationInfo_ActionInfo_Update(%d,%d,%d)}", 
        recv_msg->m_DominationActionInfo.m_ActionInfo.m_FieldCode,
        recv_msg->m_DominationActionInfo.m_ActionInfo.m_GuilGuid,
        recv_msg->m_DominationActionInfo.m_ActionInfo.m_TenderCost );
    Query_Domination_Info_Action_Update* query = Query_Domination_Info_Action_Update::ALLOC();

    query->SetUserKey(recv_msg->m_dwKey);
    query->SetActionInfo(recv_msg->m_DominationActionInfo);
    query->SetIndex(MAKEDWORD((WORD)DZ_GUILD, (WORD)DZ_DOMINATION_ACCEP_HEIM_DBR));
    query->SetQuery(query_string);
    query->SetVoidObject(pServerSession);
#ifdef _CH_20100420_CHANGE_DOMINATION_APPLY_WAY
    query->m_TokenPos = recv_msg->m_TokenPos;
    query->number_of_betting_item = recv_msg->number_of_betting_item;
#endif//_CH_20100420_CHANGE_DOMINATION_APPLY_WAY
    pServerSession->DBQuery(query);
}

Handler_Guild_GuildToDbp_IMPL(DZ_DOMINATION_ACCEP_HEIM_DBR)
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
    const MSG_DBPROXY_RESULT* pResult = (MSG_DBPROXY_RESULT*)pMsg;
    Query_Domination_Info_Action_Update* pQResult = 
        static_cast<Query_Domination_Info_Action_Update*>(pResult->m_pData);

    MSG_DZ_DOMINATION_ACCEP_HEIM_ACK sendMsg;

    if ( pQResult->IsError() || !pQResult->ResultSuccess() )
    {
        if (pQResult->IsError()) 
        {
            SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"](%s):%s", 
                pQResult->GetQuery(), pQResult->GetErrorMsg());

            Query_Domination_Info_Action_Update::FREE(pQResult);
            return;
        }
        else 
        {
            SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"]ErrorCode[ %d ]", pQResult->pParam[0].m_Result);
        }

        sendMsg.m_dwKey = pQResult->GetUserKey();
        sendMsg.m_ResultCode = RC::DOMINATION_RESULT_DBERROR;
        sendMsg.m_DominationActionInfo = pQResult->GetActionInfo();
    #ifdef _CH_20100420_CHANGE_DOMINATION_APPLY_WAY
        sendMsg.m_TokenPos = pQResult->m_TokenPos;
        sendMsg.number_of_betting_item = pQResult->number_of_betting_item;
    #endif

        Handler_Guild_DbpToGuild::OnDZ_DOMINATION_ACCEP_HEIM_ACK(
            g_DBHandler.GetSharedSession(), &sendMsg, sizeof(sendMsg));

        Query_Domination_Info_Action_Update::FREE(pQResult);
        pQResult = NULL;
        return;
    }

    const DominationAuctionInfo& current_info = pQResult->GetActionInfo();
    g_GuildManagerInDBP.DominationReturnMoneyUpdate(pServerSession, current_info,
                                                 SYSMEMO_DOMINATION_RETURN_MONEY);

    //경매 입찰 성공에 대한 응답을 보낸다.
#ifdef _CH_20100420_CHANGE_DOMINATION_APPLY_WAY
    sendMsg.m_TokenPos = pQResult->m_TokenPos;
    sendMsg.number_of_betting_item = pQResult->number_of_betting_item;
#endif//_CH_20100420_CHANGE_DOMINATION_APPLY_WAY
    sendMsg.m_dwKey = pQResult->GetUserKey();
    sendMsg.m_ResultCode = RC::DOMINATION_RESULT_SUCCESS;
    sendMsg.m_DominationActionInfo = current_info;

    Handler_Guild_DbpToGuild::OnDZ_DOMINATION_ACCEP_HEIM_ACK(
        g_DBHandler.GetSharedSession(), &sendMsg, sizeof(sendMsg));

    Query_Domination_Info_Action_Update::FREE(pQResult);
    pQResult = NULL;

    return;
}

Handler_Guild_GuildToDbp_IMPL(DZ_DOMINATION_DELETE_RETURNHEIM_DBR)
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
    const MSG_DBPROXY_RESULT* pResult = (MSG_DBPROXY_RESULT*)pMsg;
    Query_Domination_ReturnMoney_Delete* pQResult = 
        static_cast<Query_Domination_ReturnMoney_Delete*>(pResult->m_pData);

    if (pQResult->IsError() || !pQResult->ResultSuccess())
    {
        if (pQResult->IsError()) 
        {
            SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"](%s):%s", 
                pQResult->GetQuery(), pQResult->GetErrorMsg());
        }
        else 
        {
            SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] : ErrorCode[ %d ]", 
                pQResult->pParam[0].m_Result);
        }

        Query_Domination_ReturnMoney_Delete::FREE(pQResult);
        pQResult = NULL;
        return;
    }

    const DOMINATION_INFO& dominationInfo = pQResult->GetDominationInfo();

    DominationAuctionInfo actionInfo;
    ZeroMemory(&actionInfo, sizeof(actionInfo));

    actionInfo.m_MasterGuid = dominationInfo.m_inGuildInfo.m_GuildMasterGuid;
    actionInfo.m_ActionInfo.m_TenderCost = dominationInfo.m_BestCost;
    actionInfo.m_ActionInfo.m_FieldCode = dominationInfo.m_FieldCode;

#ifndef _CH_20100420_CHANGE_DOMINATION_APPLY_WAY
    //해당 지역 최고 입찰 금액 삭제를 완료 하였으면 점령길드의 마스터에게 줄 보상하임을 Update한다.
    g_GuildManagerInDBP.DominationReturnMoneyUpdate(pServerSession, actionInfo, 
        SYSMEMO_DOMINATION_REWARD_MONEY);
#endif

    Query_Domination_ReturnMoney_Delete::FREE(pQResult);
    pQResult = NULL;

    return;
}

Handler_Guild_GuildToDbp_IMPL(DZ_DOMINATION_UPDATE_RETURNHEIM_DBR)
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
    const MSG_DBPROXY_RESULT* pResult = (MSG_DBPROXY_RESULT*)pMsg;

    Query_Domination_ActionMoney_Update* pQResult = 
        static_cast<Query_Domination_ActionMoney_Update*>(pResult->m_pData);

    if (pQResult->IsError() || !pQResult->ResultSuccess())
    {
        if (pQResult->IsError()) 
        {
            SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"](%s):%s", 
                pQResult->GetQuery(), pQResult->GetErrorMsg());
        }
        else 
        {
            SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] : ErrorCode[%d]", pQResult->pParam[0].m_Result);
        }

        Query_Domination_ActionMoney_Update::FREE(pQResult);
        pQResult = NULL;
        return;
    }

    Query_Domination_ActionMoney_Update::FREE(pQResult);
    pQResult = NULL;

    return;
}

Handler_Guild_GuildToDbp_IMPL(DZ_DOMINATION_INIT_AUCTIONINFO_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
    const MSG_DZ_DOMINATION_INIT_AUCTIONINFO_SYN* recv_msg = 
        static_cast<MSG_DZ_DOMINATION_INIT_AUCTIONINFO_SYN*>(pMsg);

    STRING_SAFER_QUERY128 query_buff;
    query_buff.MakeString( "{?=call S_DominationInfo_ActionInfo_Init}" );                                                                                           
    Query_Domination_Info_Action_Init* query = Query_Domination_Info_Action_Init::ALLOC();

    query->SetMapCode(recv_msg->m_MapCode);
    query->SetIndex(MAKEDWORD((WORD)DZ_GUILD, (WORD)DZ_DOMINATION_INIT_AUCTIONINFO_DBR));
    query->SetQuery(query_buff);
    query->SetVoidObject(pServerSession);
    pServerSession->DBQuery(query);
}

Handler_Guild_GuildToDbp_IMPL(DZ_DOMINATION_INIT_AUCTIONINFO_DBR)
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
    const MSG_DBPROXY_RESULT* db_result = (MSG_DBPROXY_RESULT*)pMsg;
    Query_Domination_Info_Action_Init* query_result = 
        static_cast<Query_Domination_Info_Action_Init*>(db_result->m_pData);

    if (query_result->IsError() || !query_result->ResultSuccess())
    {
        if (query_result->IsError()) 
        {
            SUNLOG(eCRITICAL_LOG, "%s : %s", query_result->GetQuery(), query_result->GetErrorMsg());
        }
        else 
        {
            SUNLOG(eCRITICAL_LOG, "Error code : %d", query_result->pParam[0].m_Result);
        }

        MSG_DZ_DOMINATION_INIT_AUCTIONINFO_ACK nak_msg;
        nak_msg.m_ResultCode = RC::DOMINATION_RESULT_DBERROR;
        nak_msg.m_MapCode = query_result->GetMapCode();
        Handler_Guild_DbpToGuild::OnDZ_DOMINATION_INIT_AUCTIONINFO_ACK(
            g_DBHandler.GetSharedSession(), &nak_msg, sizeof(nak_msg));

        Query_Domination_Info_Action_Init::FREE(query_result);
        query_result = NULL;
    
        return;
    }

    MSG_DZ_DOMINATION_INIT_AUCTIONINFO_ACK ack_msg;
    ack_msg.m_ResultCode = RC::DOMINATION_RESULT_SUCCESS;
    ack_msg.m_MapCode = query_result->GetMapCode();
    Handler_Guild_DbpToGuild::OnDZ_DOMINATION_INIT_AUCTIONINFO_ACK(
        g_DBHandler.GetSharedSession(), &ack_msg, sizeof(ack_msg));
    
    Query_Domination_Info_Action_Init::FREE(query_result);
    query_result = NULL;

    return;
}

//_NA_20100604_BETTER_DOMINATION_MANAGER
Handler_Guild_GuildToDbp_IMPL(DZ_DOMINATION_UPDATE_GUILDINFO_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
    const MSG_DZ_DOMINATION_UPDATE_GUILDINFO_SYN* recv_msg = 
        static_cast<MSG_DZ_DOMINATION_UPDATE_GUILDINFO_SYN*>(pMsg);

    TCHAR query_string[MAX_QUERY_LENGTH_128+1];
    _sntprintf(query_string, MAX_QUERY_LENGTH_128, 
               "{?=call S_Domination_Increase_Count(%d,%d,%d,%d)}", 
               recv_msg->guild_guid_,
               recv_msg->domination_join_count_,
               recv_msg->domination_defense_count_,
               recv_msg->domination_hunting_count_);
    query_string[MAX_QUERY_LENGTH_128]='\0';

    Query_Domination_Increase_Count* query = Query_Domination_Increase_Count::ALLOC();

    query->SetGuildGuid(recv_msg->guild_guid_);
    query->SetQuery(query_string);
    query->SetIndex(MAKEDWORD((WORD)DZ_GUILD, (WORD)DZ_DOMINATION_UPDATE_GUILDINFO_DBR));
    query->SetVoidObject(pServerSession);
    pServerSession->DBQuery(query);
}

Handler_Guild_GuildToDbp_IMPL(DZ_DOMINATION_UPDATE_GUILDINFO_DBR)
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
    const MSG_DBPROXY_RESULT* db_result = (MSG_DBPROXY_RESULT*)pMsg;
    Query_Domination_Increase_Count* query_result = 
        static_cast<Query_Domination_Increase_Count*>(db_result->m_pData);

    if (query_result->ResultSuccess() == query_result->RETURN_CODE_FAILURE ||
        query_result->IsError())
    {
        assert(!"Query_Domination_Increase_Count Fail~!");

        if (query_result->IsError())
        {
            SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"](%s):%s", 
                query_result->GetQuery(), query_result->GetErrorMsg());
        }

        MSG_DZ_DOMINATION_UPDATE_GUILDINFO_ACK nak_msg;
        {
            nak_msg.db_result_ = MSG_DZ_DOMINATION_UPDATE_GUILDINFO_ACK::DB_RESULT_FAIL;
            nak_msg.guild_guid_ = query_result->GetGuildGuid();
            nak_msg.domination_join_count_ = 0;
            nak_msg.domination_defense_count_ = 0;
            nak_msg.domination_hunting_count_ = 0;
        }
        Handler_Guild_DbpToGuild::OnDZ_DOMINATION_UPDATE_GUILDINFO_ACK(
            g_DBHandler.GetSharedSession(), &nak_msg, sizeof(nak_msg));

        Query_Domination_Increase_Count::FREE(query_result);
        return;
    }

    const Query_Domination_Increase_Count::sQUERY* result_record  = query_result->pResult;

    MSG_DZ_DOMINATION_UPDATE_GUILDINFO_ACK ack_msg;
    {
        ack_msg.db_result_ = MSG_DZ_DOMINATION_UPDATE_GUILDINFO_ACK::DB_RESULT_SUCCESS;
        ack_msg.guild_guid_ = result_record->guild_guid_;
        ack_msg.domination_join_count_ = result_record->domination_join_count_;
        ack_msg.domination_defense_count_ = result_record->domination_defense_count_;
        ack_msg.domination_hunting_count_ = result_record->domination_hunting_count_;
    }
    Handler_Guild_DbpToGuild::OnDZ_DOMINATION_UPDATE_GUILDINFO_ACK(
        g_DBHandler.GetSharedSession(), &ack_msg, sizeof(ack_msg));

    Query_Domination_Increase_Count::FREE(query_result);
}

//_NA_20110216_INCREASE_DOMINATION_DEFFENSEWALL_POINT
Handler_Guild_GuildToDbp_IMPL(DZ_DOMINATION_ADD_DEFFENSEWALL_POINT_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
    const MSG_DZ_DOMINATION_ADD_DEFFENSEWALL_POINT_SYN* recv_msg = 
        static_cast<MSG_DZ_DOMINATION_ADD_DEFFENSEWALL_POINT_SYN*>(pMsg);

    const DOMINATION_INFO& info = recv_msg->domination_info_;
    STRING_SAFER_QUERY128 query_string;
    // changed by _NA_0_20100705_DOMINATION_EVENT
    query_string.MakeString("{?=call S_DominationInfo_Update(%d,%d,%d,%d,%d,%d,%d,%d,%d,%d)}",
                           info.m_FieldCode,
                           info.m_inGuildInfo.m_GuildGuid,
                           info.m_disGuildInfo.m_GuildGuid,
                           info.m_DeffenseWallA,
                           info.m_DeffenseWallB,
                           info.m_DeffenseWallC,
                           info.m_TotalPoint,
                           info.m_rewardEvent.m_byEventStats,
                           info.m_rewardEvent.m_wEventDayOfWeek,
                           info.m_rewardEvent.m_dwEventTime);

    Query_Domination_Info_Update* query = Query_Domination_Info_Update::ALLOC();

    query->SetDominationInfo(recv_msg->domination_info_);
    query->SetCharGuid(recv_msg->m_dwKey);
    query->SetItemCode(recv_msg->item_code_);

    query->SetIndex(MAKEDWORD((WORD)DZ_GUILD, (WORD)DZ_DOMINATION_ADD_DEFFENSEWALL_POINT_DBR));
    query->SetQuery(query_string);
    query->SetVoidObject(pServerSession);
    pServerSession->DBQuery(query);
}

Handler_Guild_GuildToDbp_IMPL(DZ_DOMINATION_ADD_DEFFENSEWALL_POINT_DBR)
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
    const MSG_DBPROXY_RESULT* result = (MSG_DBPROXY_RESULT*)pMsg;
    Query_Domination_Info_Update* qurey_result = 
        static_cast<Query_Domination_Info_Update*>(result->m_pData);

    if (qurey_result->IsError() || qurey_result->ResultSuccess() == false)
    {
        if (qurey_result->IsError()) 
        {
            SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"](%s):%s", 
                   qurey_result->GetQuery(), qurey_result->GetErrorMsg());

        }
        else 
        {
            SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] : ErrorCode[ %d ]", 
                   qurey_result->pParam[0].m_Result);
        }

        MSG_DZ_DOMINATION_ADD_DEFFENSEWALL_POINT_ACK nak_msg;
        nak_msg.result_value_ = false;
        nak_msg.item_code_ = qurey_result->GetItemCode();
        nak_msg.domination_info_ = qurey_result->GetDominationInfo();
        Handler_Guild_DbpToGuild::OnDZ_DOMINATION_ADD_DEFFENSEWALL_POINT_ACK(
            g_DBHandler.GetSharedSession(), &nak_msg, sizeof(nak_msg));

        Query_Domination_Info_Update::FREE(qurey_result);
        return;
    }

    MSG_DZ_DOMINATION_ADD_DEFFENSEWALL_POINT_ACK send_msg;
    send_msg.result_value_ = true;
    send_msg.item_code_ = qurey_result->GetItemCode();
    send_msg.domination_info_ = qurey_result->GetDominationInfo();
    send_msg.m_dwKey = qurey_result->GetCharGuid();
    Handler_Guild_DbpToGuild::OnDZ_DOMINATION_ADD_DEFFENSEWALL_POINT_ACK(
        g_DBHandler.GetSharedSession(), &send_msg, sizeof(send_msg));

    Query_Domination_Info_Update::FREE(qurey_result);

    return;
}

//_NA_20110406_CHANGE_RULE_FOR_DOMINATION_JOIN
Handler_Guild_GuildToDbp_IMPL(DZ_DOMINATION_MEMBER_JOIN_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
    const MSG_DZ_DOMINATION_MEMBER_JOIN_SYN* recv_msg = 
        static_cast<MSG_DZ_DOMINATION_MEMBER_JOIN_SYN*>(pMsg);

    const DominationMemberInfo& member_info = recv_msg->member_info_;
    STRING_SAFER_QUERY128 query_buff;
    query_buff.MakeString("{?=call S_Domination_Member_Insert(%d,%d,%d)}", member_info.member_guid_, 
                                                                           recv_msg->map_code_, 
                                                                           member_info.guild_type_);
    Query_Domination_Member_Insert* query = Query_Domination_Member_Insert::ALLOC();
    query->SetMemberInfo(member_info);
    query->SetMapCode(recv_msg->map_code_);
    query->SetIndex(MAKEDWORD((WORD)DZ_GUILD, (WORD)DZ_DOMINATION_MEMBER_JOIN_DBR));
    query->SetQuery(query_buff);
    query->SetVoidObject(pServerSession);
    pServerSession->DBQuery(query);
}

Handler_Guild_GuildToDbp_IMPL(DZ_DOMINATION_MEMBER_JOIN_DBR)
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
    const MSG_DBPROXY_RESULT* db_result = (MSG_DBPROXY_RESULT*)pMsg;

    Query_Domination_Member_Insert* query_result = 
        static_cast<Query_Domination_Member_Insert*>(db_result->m_pData);

    if (query_result->ResultSuccess() == false || query_result->IsError())
    {
        if (query_result->IsError())
        {
            SUNLOG(eCRITICAL_LOG, "%s : %s",query_result->GetQuery(), query_result->GetErrorMsg());
        }
        else
        {
            SUNLOG(eCRITICAL_LOG, __FUNCTION__":Error code : %d", query_result->result_param[0].result_value_);
        }

        MSG_DZ_DOMINATION_MEMBER_JOIN_ACK nak_msg;
        nak_msg.db_result_ = false;
        nak_msg.map_code_ = query_result->GetMapCode();
        nak_msg.member_info_ = query_result->GetMemberInfo();
        Handler_Guild_DbpToGuild::OnDZ_DOMINATION_MEMBER_JOIN_ACK(
            g_DBHandler.GetSharedSession(), &nak_msg, sizeof(nak_msg));

        Query_Domination_Member_Insert::FREE(query_result);
        query_result = NULL;

        return;
    }

    MSG_DZ_DOMINATION_MEMBER_JOIN_ACK ack_msg;
    ack_msg.db_result_ = true;
    ack_msg.map_code_ = query_result->GetMapCode();
    ack_msg.member_info_ = query_result->GetMemberInfo();
    Handler_Guild_DbpToGuild::OnDZ_DOMINATION_MEMBER_JOIN_ACK(
        g_DBHandler.GetSharedSession(), &ack_msg, sizeof(ack_msg));

    Query_Domination_Member_Insert::FREE(query_result);
    query_result = NULL;
}

Handler_Guild_GuildToDbp_IMPL(DZ_DOMINATION_MEMBER_LEAVE_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
    const MSG_DZ_DOMINATION_MEMBER_LEAVE_SYN* recv_msg = 
        static_cast<MSG_DZ_DOMINATION_MEMBER_LEAVE_SYN*>(pMsg);

    STRING_SAFER_QUERY128 query_buff;
    query_buff.MakeString("{?=call S_Domination_Member_Delete(%d)}",recv_msg->member_guid_);
    Query_Domination_Member_Delete* query = Query_Domination_Member_Delete::ALLOC();
    query->SetMemberGuid(recv_msg->member_guid_);
    query->SetRequesterGuid(recv_msg->requester_guid);
    query->SetMapCode(recv_msg->map_code_);
    query->SetIndex(MAKEDWORD((WORD)DZ_GUILD, (WORD)DZ_DOMINATION_MEMBER_LEAVE_DBR));
    query->SetQuery(query_buff);
    query->SetVoidObject(pServerSession);
    pServerSession->DBQuery(query);
}

Handler_Guild_GuildToDbp_IMPL(DZ_DOMINATION_MEMBER_LEAVE_DBR)
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
    const MSG_DBPROXY_RESULT* db_result = (MSG_DBPROXY_RESULT*)pMsg;

    Query_Domination_Member_Delete* query_result = 
        static_cast<Query_Domination_Member_Delete*>(db_result->m_pData);

    if (query_result->ResultSuccess() == false || query_result->IsError())
    {
        if (query_result->IsError())
        {
            SUNLOG(eCRITICAL_LOG, "%s : %s",query_result->GetQuery(), query_result->GetErrorMsg());
        }
        else
        {
            SUNLOG(eCRITICAL_LOG, __FUNCTION__":Error code : %d", query_result->result_param[0].result_value_);
        }

        MSG_DZ_DOMINATION_MEMBER_LEAVE_ACK nak_msg;
        nak_msg.db_result_ = false;
        nak_msg.map_code_ = query_result->GetMapCode();
        nak_msg.member_guid_ = query_result->GetMemberGuid();
        nak_msg.requester_guid = query_result->GetRequesterGuid();
        Handler_Guild_DbpToGuild::OnDZ_DOMINATION_MEMBER_LEAVE_ACK(
            g_DBHandler.GetSharedSession(), &nak_msg, sizeof(nak_msg));

        Query_Domination_Member_Delete::FREE(query_result);
        query_result = NULL;

        return;
    }

    MSG_DZ_DOMINATION_MEMBER_LEAVE_ACK ack_msg;
    ack_msg.db_result_ = true;
    ack_msg.map_code_ = query_result->GetMapCode();
    ack_msg.member_guid_ = query_result->GetMemberGuid();
    ack_msg.requester_guid = query_result->GetRequesterGuid();
    Handler_Guild_DbpToGuild::OnDZ_DOMINATION_MEMBER_LEAVE_ACK(
        g_DBHandler.GetSharedSession(), &ack_msg, sizeof(ack_msg));

    Query_Domination_Member_Delete::FREE(query_result);
    query_result = NULL;
}

Handler_Guild_GuildToDbp_IMPL(DZ_DOMINATION_MEMBERLIST_REQ)
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
    const MSG_DZ_DOMINATION_MEMBERLIST_REQ* recv_msg = 
        static_cast<MSG_DZ_DOMINATION_MEMBERLIST_REQ*>(pMsg);

    STRING_SAFER_QUERY128 query_buff;
    query_buff.MakeString("{?=call S_Domination_MemberList_Select(%d)}", recv_msg->map_code_);

    Query_Domination_MemberList_Select* query = Query_Domination_MemberList_Select::ALLOC();
    query->SetMapCode(recv_msg->map_code_);
    query->SetIndex(MAKEDWORD((WORD)DZ_GUILD, (WORD)DZ_DOMINATION_MEMBERLIST_DBR));
    query->SetQuery(query_buff);
    query->SetVoidObject(pServerSession);
    pServerSession->DBQuery(query);
}

Handler_Guild_GuildToDbp_IMPL(DZ_DOMINATION_MEMBERLIST_DBR)
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
    const MSG_DBPROXY_RESULT* db_result = static_cast<MSG_DBPROXY_RESULT*>(pMsg);
    Query_Domination_MemberList_Select* query_result = 
        static_cast<Query_Domination_MemberList_Select*>(db_result->m_pData);

    if (query_result->ResultSuccess() == false ||query_result->IsError())
    {
        if (query_result->IsError())
        {
            SUNLOG(eCRITICAL_LOG, "%s : %s",query_result->GetQuery(), query_result->GetErrorMsg());
        }
        else
        {
            SUNLOG(eCRITICAL_LOG, __FUNCTION__":Error code : %d", query_result->result_param[0].result_value_);
        }

        Query_Domination_MemberList_Select::FREE(query_result);
        query_result = NULL;
        return;
    }

    const Query_Domination_MemberList_Select::sQUERY* records = query_result->select_record;

    MSG_DZ_DOMINATION_MEMBERLIST_ANS send_msg;

    BYTE data_count = static_cast<BYTE>(query_result->GetResultRowNum());
    if (data_count > _countof(send_msg.member_list_))
    {
        assert(!"Member list count is over");
        SUNLOG(eCRITICAL_LOG, "Member list count is over : %d", data_count);

        Query_Domination_MemberList_Select::FREE(query_result);
        query_result = NULL;
        return;
    }

    send_msg.map_code_ = query_result->GetMapCode();
    send_msg.data_count_ = data_count;

    for (BYTE index = 0; index < data_count; ++index)
    {
        DominationMemberInfo* const dest_info = &send_msg.member_list_[index];
        const DominationMemberInfo& src_info = records[index].member_info;
        *dest_info = src_info;
        dest_info->member_name_[_countof(dest_info->member_name_) - 1] = '\0';
    }
    Handler_Guild_DbpToGuild::OnDZ_DOMINATION_MEMBERLIST_ANS(
        g_DBHandler.GetSharedSession(), &send_msg, send_msg.GetSize() );

    Query_Domination_MemberList_Select::FREE(query_result);
    query_result = NULL;
}

//gamelulu_dev_note : 순차적 이벤트 컨트롤에 의해서 발생되는 초기화 요청이다.
//하지만 DB처리가 실패 하였을 경우에는 재요청을 하는 방법밖에는 없다.
Handler_Guild_GuildToDbp_IMPL(DZ_DOMINATION_MEMBERLIST_INIT_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
    const MSG_DZ_DOMINATION_MEMBERLIST_INIT_SYN* recv_msg = 
        static_cast<MSG_DZ_DOMINATION_MEMBERLIST_INIT_SYN*>(pMsg);
    __TOUCHED(recv_msg);
    STRING_SAFER_QUERY128 query_buff;
    query_buff.MakeString("{?=call S_Domination_MemberList_Init}");
    Query_Domination_MemberList_Init* query = Query_Domination_MemberList_Init::ALLOC();
    query->SetIndex(MAKEDWORD((WORD)DZ_GUILD, (WORD)DZ_DOMINATION_MEMBERLIST_INIT_DBR));
    query->SetQuery(query_buff);
    query->SetVoidObject(pServerSession);
    pServerSession->DBQuery(query);
}


Handler_Guild_GuildToDbp_IMPL(DZ_DOMINATION_MEMBERLIST_INIT_DBR)
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
    const MSG_DBPROXY_RESULT* db_result = (MSG_DBPROXY_RESULT*)pMsg;

    Query_Domination_MemberList_Init* query_result = 
        static_cast<Query_Domination_MemberList_Init*>(db_result->m_pData);

    if (query_result->ResultSuccess() == false || query_result->IsError())
    {
        if (query_result->IsError())
        {
            SUNLOG(eCRITICAL_LOG, "%s : %s",query_result->GetQuery(), query_result->GetErrorMsg());
        }
        else
        {
            SUNLOG(eCRITICAL_LOG, __FUNCTION__":Error code : %d", query_result->result_param[0].result_value_);
        }

        MSG_DZ_DOMINATION_MEMBERLIST_INIT_ACK nak_msg;
        nak_msg.db_result_ = false;
        Handler_Guild_DbpToGuild::OnDZ_DOMINATION_MEMBERLIST_INIT_ACK(
            g_DBHandler.GetSharedSession(), &nak_msg, sizeof(nak_msg));

        Query_Domination_MemberList_Init::FREE(query_result);
        query_result = NULL;

        return;
    }


    MSG_DZ_DOMINATION_MEMBERLIST_INIT_ACK ack_msg;
    ack_msg.db_result_ = true;
    Handler_Guild_DbpToGuild::OnDZ_DOMINATION_MEMBERLIST_INIT_ACK(
        g_DBHandler.GetSharedSession(), &ack_msg, sizeof(ack_msg));

    Query_Domination_MemberList_Init::FREE(query_result);
    query_result = NULL;
}

/************************************************************************/
/*                  지역 점령전 DBProxy 작업 END                        */
/************************************************************************/

//_NA_20100913_GUILD_WAREHOUSE_GM_COMMAND
Handler_Guild_GuildToDbp_IMPL(DZ_GUILD_GM_WAREHOUSE_FORCE_CLOSE_CMD)
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
    const MSG_DZ_GUILD_GM_WAREHOUSE_FORCE_CLOSE_CMD* msg = \
        static_cast<MSG_DZ_GUILD_GM_WAREHOUSE_FORCE_CLOSE_CMD*>(pMsg);

    GUILDGUID guild_guid = msg->guild_guid;

    GuildWarehouseInfo* guild_warehouse_info = g_GuildManagerInDBP.FindGuildWarehouse(guild_guid);
    if (guild_warehouse_info != NULL)
    {
        // ;;;;
        CHARGUID locker_char_guid = 0;
        const USERGUID locker_user_guid = guild_warehouse_info->GetOpenLocker(&locker_char_guid);
        guild_warehouse_info->ReleaseOpenLock();
        //
        SUNLOG(eCRITICAL_LOG,
               _T("|["__FUNCTION__"]|Msg=release a guild warehouse open lock by GM,")
               _T(" the locker is character(%u) of user(%u)|"),
               locker_char_guid, locker_user_guid);
    }
}

#ifdef _NA004034_20120102_GUILD_POINT_SYSTEM
Handler_Guild_GuildToDbp_IMPL(DZ_GUILDPOINT_SYSTEM)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    const MSG_DZ_GUILDPOINT_SYSTEM* packet = \
        static_cast<MSG_DZ_GUILDPOINT_SYSTEM*>(pMsg);
    
    switch(packet->packet_type)
    {
        case ZD_GUILDPOINT_SELECT_SYN:
        {
            const MSG_ZD_GUILDPOINT_SELECT_SYN* packet = \
                static_cast<MSG_ZD_GUILDPOINT_SELECT_SYN*>(pMsg);

            STRING_SAFER_QUERY128 query_string;
            query_string.MakeString(_T("{? = call S_GuildPoint_Select (?, ?)}"));

            Query_GuildPoint_Select* query = Query_GuildPoint_Select::ALLOC();
            query->packet_type = ZD_GUILDPOINT_SELECT_SYN;
            query->SetIndex(MAKEDWORD((WORD)DZ_GUILD, (WORD)DZ_GUILDPOINT_SYSTEM_DBR));
            query->SetQuery(query_string);
            query->SetVoidObject(pServerSession);

            query->parameters.guid_ = packet->guid_;
            query->parameters.code_ = packet->code_;
            
            pServerSession->DBQuery(query);
        } break;

        case ZD_GUILDPOINT_SELECT_ACK:
        {
            const MSG_ZD_GUILDPOINT_SELECT_ACK* packet = \
                static_cast<MSG_ZD_GUILDPOINT_SELECT_ACK*>(pMsg);

            DWORD guid = packet->guid_;
            DWORD code = packet->code_;

            g_GuildPointSystem.ClearGuildPoint(guid, code);
            g_GuildPointSystem.SetGuildPoint(guid, GuildPointSystem::GUILD_TOTAL_UUID, code, 0);

            for(int i = 0; i < packet->count_; i++)
            {
                DWORD uuid  = packet->guild_points[i].uuid_;
                DWORD point = packet->guild_points[i].point_;
                g_GuildPointSystem.SetGuildPoint(guid, uuid, code, point);
            }
        } break;

        case ZD_GUILDPOINT_CHANGE_CMD:
        {
            const MSG_ZD_GUILDPOINT_CHANGE_CMD* packet = \
                static_cast<MSG_ZD_GUILDPOINT_CHANGE_CMD*>(pMsg);

            STRING_SAFER_QUERY128 query_string;
            query_string.MakeString(_T("{? = call S_GuildPoint_Update (%d, %d, %d, %d)}"),
                packet->guildpoint.guid_, packet->guildpoint.uuid_, \
                packet->guildpoint.code_, packet->guildpoint.point_);

            Query_GuildPoint_Change* query = Query_GuildPoint_Change::ALLOC();
            query->packet_type = ZD_GUILDPOINT_CHANGE_CMD;
            query->SetIndex(MAKEDWORD((WORD)DZ_GUILD, (WORD)DZ_GUILDPOINT_SYSTEM_DBR));
            query->SetQuery(query_string);
            query->SetVoidObject(pServerSession);
            
            pServerSession->DBQuery(query);
        } break;

        case DZ_GUILDPOINT_DELETE_CMD:
        {
            const MSG_DZ_GUILDPOINT_DELETE_CMD* packet = \
                static_cast<MSG_DZ_GUILDPOINT_DELETE_CMD*>(pMsg);

            STRING_SAFER_QUERY128 query_string;
            query_string.MakeString(_T("{? = call S_GuildPoint_Delete (%d, %d, %d)}"),
                packet->guildpoint.guid_, packet->guildpoint.uuid_, \
                packet->guildpoint.code_);

            Query_GuildPoint_Delete* query = Query_GuildPoint_Delete::ALLOC();
            query->packet_type = DZ_GUILDPOINT_DELETE_CMD;
            query->SetIndex(MAKEDWORD((WORD)DZ_GUILD, (WORD)DZ_GUILDPOINT_SYSTEM_DBR));
            query->SetQuery(query_string);
            query->SetVoidObject(pServerSession);

            pServerSession->DBQuery(query);
        } break;

        case ZD_GUILDPOINT_GUILD_LEVEL_CHANGE_CMD:
        {
            const MSG_ZD_GUILDPOINT_GUILD_LEVEL_CHANGE_CMD* packet = \
                static_cast<MSG_ZD_GUILDPOINT_GUILD_LEVEL_CHANGE_CMD*>(pMsg);

            GUILDGUID guild_guid = packet->guid_;
            BYTE slot_tab_num = 0;

            GuildWarehouseInfo* guild_warehouse = \
                g_GuildManagerInDBP.FindGuildWarehouse(guild_guid);
            if (guild_warehouse)
            {
                slot_tab_num = guild_warehouse->GetSlotTabNum();
            }

            BYTE pre_level = packet->pre_level_;
            BYTE cur_level = packet->level_;
            
            if(pre_level < cur_level && cur_level == GUILDWAREHOUSE_SLOTEX_GUILD_GRADE)
            {
                // 레벨업해서 3레벨이 됐을 경우(2->3)
                slot_tab_num = guild_warehouse->GetSlotTabNum() + 1;
                guild_warehouse->SetSlotTabNum(slot_tab_num);
            }
            
            if(pre_level > cur_level && pre_level == GUILDWAREHOUSE_SLOTEX_GUILD_GRADE)
            {
                // 레벨다운 (3 -> 2)
                slot_tab_num = guild_warehouse->GetSlotTabNum() - 1;
                guild_warehouse->SetSlotTabNum(slot_tab_num);
            }

            STRING_SAFER_QUERY128 query_string;
            query_string.MakeString(_T("{? = call S_GuildLevel_Change (%d, %d, %d)}"),
                packet->guid_, packet->level_, slot_tab_num);

            Query_GuildLevel_Change* query = Query_GuildLevel_Change::ALLOC();
            query->packet_type = ZD_GUILDPOINT_GUILD_LEVEL_CHANGE_CMD;
            query->SetIndex(MAKEDWORD((WORD)DZ_GUILD, (WORD)DZ_GUILDPOINT_SYSTEM_DBR));
            query->SetQuery(query_string);
            query->SetVoidObject(pServerSession);

            pServerSession->DBQuery(query);
        } break;
    }
}

Handler_Guild_GuildToDbp_IMPL(DZ_GUILDPOINT_SYSTEM_DBR)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    const MSG_DBPROXY_RESULT* db_result = (MSG_DBPROXY_RESULT*)pMsg;
    DBRObject* object = static_cast<DBRObject*>(db_result->m_pData);

    switch(object->packet_type)
    {
        case ZD_GUILDPOINT_SELECT_SYN:
        {
            Query_GuildPoint_Select* query_result = \
                static_cast<Query_GuildPoint_Select*>(db_result->m_pData);

            if (query_result->IsError()) {
                SUNLOG(eCRITICAL_LOG, "%s : %s", 
                    query_result->GetQuery(), query_result->GetErrorMsg());
            }
            else 
            {
                int row_count = query_result->GetResultRowNum();

                MSG_ZD_GUILDPOINT_SELECT_ACK msg;
                msg.guid_ = query_result->parameters.guid_;
                msg.code_ = query_result->parameters.code_;
                msg.count_ = row_count;
                memcpy(msg.guild_points, query_result->result_records_, \
                    sizeof(query_result->result_records_[0])*row_count);

                g_DBProxyServer.SendToGuildServer(&msg, sizeof(msg));
            }

            Query_GuildPoint_Select::FREE(query_result);
        } break;

        case ZD_GUILDPOINT_CHANGE_CMD:
        {
            Query_GuildPoint_Change* query_result = 
                static_cast<Query_GuildPoint_Change*>(db_result->m_pData);

            if (query_result->IsError()) {
                SUNLOG(eCRITICAL_LOG, "%s : %s", 
                    query_result->GetQuery(), query_result->GetErrorMsg());
            }
            else 
            {
                // nothing to return...
            }

            Query_GuildPoint_Change::FREE(query_result);
        } break;

        case DZ_GUILDPOINT_DELETE_CMD:
        {
            Query_GuildPoint_Delete* query_result = 
                static_cast<Query_GuildPoint_Delete*>(db_result->m_pData);

            if (query_result->IsError()) {
                SUNLOG(eCRITICAL_LOG, "%s : %s", 
                    query_result->GetQuery(), query_result->GetErrorMsg());
            }
            else 
            {
                // nothing to return...
            }

            Query_GuildPoint_Delete::FREE(query_result);
        } break;

        case ZD_GUILDPOINT_GUILD_LEVEL_CHANGE_CMD:
        {
            Query_GuildLevel_Change* query_result = \
                static_cast<Query_GuildLevel_Change*>(db_result->m_pData);

            if (query_result->IsError()) {
                SUNLOG(eCRITICAL_LOG, "%s : %s", 
                    query_result->GetQuery(), query_result->GetErrorMsg());
            }
            else 
            {
                // nothing to return...
            }

            Query_GuildLevel_Change::FREE(query_result);
        } break;
    }
}
#endif //_NA004034_20120102_GUILD_POINT_SYSTEM

#ifdef _NA_004034_20120102_GUILD_INTRODUCTION
Handler_Guild_GuildToDbp_IMPL(DZ_GUILD_INTRODUCTION_SAVE_SYN)
{
    const MSG_DZ_GUILD_INTRODUCTION_SAVE_SYN* packet = \
        static_cast<MSG_DZ_GUILD_INTRODUCTION_SAVE_SYN*>(pMsg);

    STRING_SAFER_QUERY1024 query_buff;
    query_buff.MakeString(
        "{? = call S_GuildIntroduction_Update (%d, '%s')}",
        packet->guild_guid,
        packet->guild_introductions_.introduction_strings_
        );

    Query_GuildIntroduction_Update* query = Query_GuildIntroduction_Update::ALLOC();
    query->SetIndex(MAKEDWORD((WORD)DZ_GUILD, (WORD)DZ_GUILD_INTRODUCTION_SAVE_DBR));
    query->SetQuery(query_buff);
    query->SetVoidObject(pServerSession);
    pServerSession->DBQuery(query);
}

Handler_Guild_GuildToDbp_IMPL(DZ_GUILD_INTRODUCTION_SAVE_DBR)
{
    const MSG_DBPROXY_RESULT* db_result = static_cast<MSG_DBPROXY_RESULT*>(pMsg);
    Query_GuildIntroduction_Update* query_result = 
        static_cast<Query_GuildIntroduction_Update*>(db_result->m_pData);

    if (query_result->IsError())
    {
        SUNLOG(
            eCRITICAL_LOG, 
            "%s : %s",
            query_result->GetQuery(), 
            query_result->GetErrorMsg()
            );
    }

    if (query_result->parameters.result_ != 0)
    {
        SUNLOG(
            eCRITICAL_LOG, 
            __FUNCTION__":Error code = %d", 
            query_result->parameters.result_
            );
    }

    Query_GuildIntroduction_Update::FREE(query_result);
    query_result = NULL;
}

Handler_Guild_GuildToDbp_IMPL(DZ_GUILD_INTRODUCTION_LOAD_SYN)
{
    const MSG_DZ_GUILD_INTRODUCTION_LOAD_SYN* packet = \
        static_cast<MSG_DZ_GUILD_INTRODUCTION_LOAD_SYN*>(pMsg);

    STRING_SAFER_QUERY1024 query_buff;
    query_buff.MakeString( \
        "{? = call S_GuildIntroduction_Select (%d)}", \
        packet->guild_guid);

    Query_GuildIntroduction_Select* query = Query_GuildIntroduction_Select::ALLOC();
    query->SetIndex(MAKEDWORD((WORD)DZ_GUILD, (WORD)DZ_GUILD_INTRODUCTION_LOAD_DBR));
    query->SetQuery(query_buff);
    query->SetVoidObject(pServerSession);
    query->guild_guid = packet->guild_guid;
    pServerSession->DBQuery(query);
}

Handler_Guild_GuildToDbp_IMPL(DZ_GUILD_INTRODUCTION_LOAD_DBR)
{
    const MSG_DBPROXY_RESULT* db_result = static_cast<MSG_DBPROXY_RESULT*>(pMsg);
    Query_GuildIntroduction_Select* query_result = 
        static_cast<Query_GuildIntroduction_Select*>(db_result->m_pData);

    if (query_result->IsError())
    {
        SUNLOG(eCRITICAL_LOG, "%s : %s", query_result->GetQuery(), query_result->GetErrorMsg());

        Query_GuildIntroduction_Select::FREE(query_result);
        query_result = NULL;
        return;
    }

    if (query_result->parameters.result_ != 0)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__":Error code = %d", query_result->parameters.result_);

        Query_GuildIntroduction_Select::FREE(query_result);
        query_result = NULL;
        return;
    }

    GUILDGUID guid = query_result->guild_guid;
    g_GuildManager.SetGuildIntroduction(guid, query_result->result_records[0].introduction_);

    Query_GuildIntroduction_Select::FREE(query_result);
    query_result = NULL;
}
#endif //_NA_004034_20120102_GUILD_INTRODUCTION

#ifdef _NA_003923_20120130_GUILD_RENEWAL
Handler_Guild_GuildToDbp_IMPL(DZ_GUILD_WRITE_IN_GAME_LOG_SYN)
{
    const MSG_DZ_GUILD_WRITE_IN_GAME_LOG_SYN* recv_msg = 
        static_cast<MSG_DZ_GUILD_WRITE_IN_GAME_LOG_SYN*>(pMsg);

    STRING_SAFER_QUERY128 query_buff;
    query_buff.MakeString(
        "{?=call S_GuildInGameLog_Insert (%d, %d, %d, '%s', %d, %d, %d)}",
        recv_msg->guild_guid_,
        recv_msg->log_category_,
        recv_msg->log_data_.log_type_,
        recv_msg->log_data_.string_param_,
        recv_msg->log_data_.param1_,
        recv_msg->log_data_.param2_,
        recv_msg->log_data_.param3_);
    Query_GuildInGameLog_Insert* query = Query_GuildInGameLog_Insert::ALLOC();
    query->SetIndex(MAKEDWORD((WORD)DZ_GUILD, (WORD)DZ_GUILD_WRITE_IN_GAME_LOG_DBR));
    query->SetQuery(query_buff);
    query->SetVoidObject(pServerSession);
    query->SetLogData(recv_msg->log_data_);
    query->SetGuildGuid(recv_msg->guild_guid_);
    pServerSession->DBQuery(query);
}

Handler_Guild_GuildToDbp_IMPL(DZ_GUILD_WRITE_IN_GAME_LOG_DBR)
{
    const MSG_DBPROXY_RESULT* db_result = static_cast<MSG_DBPROXY_RESULT*>(pMsg);
    Query_GuildInGameLog_Insert* query_result = 
        static_cast<Query_GuildInGameLog_Insert*>(db_result->m_pData);

    if (query_result->ResultSuccess() == false ||query_result->IsError())
    {
        if (query_result->IsError())
        {
            SUNLOG(
                eCRITICAL_LOG,
                "%s : %s",
                query_result->GetQuery(),
                query_result->GetErrorMsg()
                );
        }
        else
        {
            SUNLOG(
                eCRITICAL_LOG, 
                __FUNCTION__" : Error code = %d", 
                query_result->result_param[0].result_value_
                );
        }

        Query_GuildInGameLog_Insert::FREE(query_result);
        query_result = NULL;
        return;
    }

    const Query_GuildInGameLog_Insert::sQUERY* records = query_result->select_record;

    MSG_DZ_GUILD_WRITE_IN_GAME_LOG_ACK send_msg;
    send_msg.log_data_ = query_result->GetLogData();
    send_msg.guild_guid_ = query_result->GetGuildGuid();
    send_msg.log_data_.log_time_ = util::ConvertTCharToDWORD64(records->log_time_);

    Handler_Guild_DbpToGuild::OnDZ_GUILD_WRITE_IN_GAME_LOG_ACK(
        g_DBHandler.GetSharedSession(), &send_msg, sizeof(send_msg));

    Query_GuildInGameLog_Insert::FREE(query_result);
    query_result = NULL;
}

Handler_Guild_GuildToDbp_IMPL(DZ_GUILD_IN_GAME_LOG_LIST_REQ)
{
    const MSG_DZ_GUILD_IN_GAME_LOG_LIST_REQ* recv_msg = 
        static_cast<MSG_DZ_GUILD_IN_GAME_LOG_LIST_REQ*>(pMsg);

    STRING_SAFER_QUERY128 query_buff;
    query_buff.MakeString(
        "{?=call S_GuildInGameLog_Select (%d)}",
        recv_msg->guild_guid_
        );
    Query_GuildInGameLog_Select* query = Query_GuildInGameLog_Select::ALLOC();
    query->SetIndex(MAKEDWORD((WORD)DZ_GUILD, (WORD)DZ_GUILD_IN_GAME_LOG_LIST_DBR));
    query->SetQuery(query_buff);
    query->SetVoidObject(pServerSession);
    query->SetGuildGuid(recv_msg->guild_guid_);
    pServerSession->DBQuery(query);
}

Handler_Guild_GuildToDbp_IMPL(DZ_GUILD_IN_GAME_LOG_LIST_DBR)
{
    const MSG_DBPROXY_RESULT* db_result = static_cast<MSG_DBPROXY_RESULT*>(pMsg);
    Query_GuildInGameLog_Select* query_result = 
        static_cast<Query_GuildInGameLog_Select*>(db_result->m_pData);

    if (query_result->ResultSuccess() == false ||query_result->IsError())
    {
        if (query_result->IsError())
        {
            SUNLOG(
                eCRITICAL_LOG, 
                "%s : %s",
                query_result->GetQuery(), 
                query_result->GetErrorMsg()
                );
        }
        else
        {
            SUNLOG(
                eCRITICAL_LOG, 
                __FUNCTION__":Error code = %d", 
                query_result->result_param[0].result_value_
                );
        }

        Query_GuildInGameLog_Select::FREE(query_result);
        query_result = NULL;
        return;
    }

    const Query_GuildInGameLog_Select::sQUERY* records = query_result->select_record;

    MSG_DZ_GUILD_IN_GAME_LOG_LIST_ANS send_msg;

    BYTE data_count = static_cast<BYTE>(query_result->GetResultRowNum());
    if (data_count > _countof(send_msg.log_data_))
    {
        SUNLOG(
            eCRITICAL_LOG, 
            __FUNCTION__" : Log list count is over : count = %d", 
            data_count
            );

        Query_GuildInGameLog_Select::FREE(query_result);
        query_result = NULL;
        return;
    }

    send_msg.guild_guid_ = query_result->GetGuildGuid();
    send_msg.log_count_ = data_count;

    for (WORD index = 0; index < data_count; ++index)
    {
        GuildInGameLogData* const dest_info = &send_msg.log_data_[index];

        dest_info->log_type_ = records[index].log_type_;
        dest_info->param1_ = records[index].param1_;
        dest_info->param2_ = records[index].param2_;
        dest_info->param3_ = records[index].param3_;
        dest_info->log_time_ = util::ConvertTCharToDWORD64(records[index].log_time_);
        _tcsncpy(
            dest_info->string_param_,
            records[index].string_param_,
            _countof(dest_info->string_param_)
            );
        dest_info->string_param_[_countof(dest_info->string_param_) - 1] = '\0';
    }

    Handler_Guild_DbpToGuild::OnDZ_GUILD_IN_GAME_LOG_LIST_ANS(
        g_DBHandler.GetSharedSession(), &send_msg, send_msg.GetSize());

    Query_GuildInGameLog_Select::FREE(query_result);
    query_result = NULL;
}
#endif//_NA_003923_20120130_GUILD_RENEWAL

#ifdef _NA_000000_20130429_DOMINATION_PENALTY
Handler_Guild_GuildToDbp_IMPL(DZ_DOMINATION_PENALTY_CMD)
{
    const MSG_DZ_DOMINATION_PENALTY_CMD* packet = \
        static_cast<MSG_DZ_DOMINATION_PENALTY_CMD*>(pMsg);

    char datetime_string[MAX_DATETIME_SIZE+1] = { 0, };
    
    util::GetSQLDateTime_YYYYMMDDHHMMSSNNN( \
        util::GetDateTime_YYYYMMDDHHMMSS(packet->penalty_time.GetTime())*1000, datetime_string);

    STRING_SAFER_QUERY1024 query_buff;
    query_buff.MakeString( \
        "{? = call S_GuildPenalty_Update (%d, '%s')}", \
        packet->guild_guid, datetime_string);

    Query_GuildPenalty_Update* query = Query_GuildPenalty_Update::ALLOC();
    query->SetIndex(MAKEDWORD((WORD)DZ_GUILD, (WORD)DZ_DOMINATION_PENALTY_DBR));
    query->SetQuery(query_buff);
    query->SetVoidObject(pServerSession);
    pServerSession->DBQuery(query);
}

Handler_Guild_GuildToDbp_IMPL(DZ_DOMINATION_PENALTY_DBR)
{
    const MSG_DBPROXY_RESULT* db_result = static_cast<MSG_DBPROXY_RESULT*>(pMsg);
    Query_GuildPenalty_Update* query_result = \
        static_cast<Query_GuildPenalty_Update*>(db_result->m_pData);

    if (query_result->IsError())
    {
        SUNLOG(eCRITICAL_LOG, "%s : %s", query_result->GetQuery(), query_result->GetErrorMsg());

        Query_GuildPenalty_Update::FREE(query_result);
        query_result = NULL;
        return;
    }

    if (query_result->parameters.result_ != 0)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__":Error code = %d", query_result->parameters.result_);

        Query_GuildPenalty_Update::FREE(query_result);
        query_result = NULL;
        return;
    }

    Query_GuildPenalty_Update::FREE(query_result);
    query_result = NULL;
}
#endif //_NA_000000_20130429_DOMINATION_PENALTY

#ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION

Handler_Guild_GuildToDbp_IMPL(DZ_CONTINENT_LORD_INFO_SYN)
{
    __TOUCHED((pServerSession, wSize));
    const MSG_DZ_CONTINENT_LORD_INFO_SYN* msg = \
        static_cast<MSG_DZ_CONTINENT_LORD_INFO_SYN*>(pMsg);

    STRING_SAFER_QUERY128 query_buff;
    query_buff.MakeString("{?=call S_ContinentLordInfo_Select(%d)}", msg->lord_guild_guid);

    Query_Continent_Lord_Info_Select* query = Query_Continent_Lord_Info_Select::ALLOC();
    query->SetLordGuildGuid(msg->lord_guild_guid);
    query->SetContinentNumber(msg->contienet_number);
    query->SetLordGuildName(msg->lord_guild_name);
    query->SetMapCode(msg->map_code);
    query->SetSendGameServer(msg->send_game_server);
    query->SetIndex(MAKEDWORD((WORD)DZ_GUILD, (WORD)DZ_CONTINENT_LORD_INFO_DBR));
    query->SetQuery(query_buff);
    query->SetVoidObject(pServerSession);
    pServerSession->DBQuery(query);
}

Handler_Guild_GuildToDbp_IMPL(DZ_CONTINENT_LORD_INFO_DBR)
{
    __TOUCHED((pServerSession, wSize));
    const MSG_DBPROXY_RESULT* db_result = (MSG_DBPROXY_RESULT*)pMsg;
    Query_Continent_Lord_Info_Select* query_result = 
        static_cast<Query_Continent_Lord_Info_Select*>(db_result->m_pData);

    if (query_result->ResultSuccess() == false || query_result->IsError())
    {
        if (query_result->IsError())
        {
            SUNLOG(eCRITICAL_LOG, "%s : %s",query_result->GetQuery(), query_result->GetErrorMsg());
        }
        else
        {
            SUNLOG(eCRITICAL_LOG, __FUNCTION__":Error code : %d", query_result->result_param[0].result_value_);
        }
        Query_Continent_Lord_Info_Select::FREE(query_result);
        query_result = NULL;
        return;
    }

    const Query_Continent_Lord_Info_Select::sQUERY* records = query_result->select_record;

    BYTE data_count = static_cast<BYTE>(query_result->GetResultRowNum());

    sDOMINATION_CONTINENT_LORD_INFO lord_info;
    for (BYTE index = 0; index < data_count; ++index)
    {
        util::GUILDNAMECOPY(lord_info.allience_guild_infos[index].allience_guild_name,records[index].guild_name_);
    }
    lord_info.allience_guild_count = data_count;
    lord_info.continent_number = query_result->GetContinentNumber();
    lord_info.map_code = query_result->GetMapCode();
    util::GUILDNAMECOPY(lord_info.guild_name, query_result->GetLordGuildName());

    DominationManager::Instance()->SetDominationContinentLordInfo(lord_info);

    if (query_result->GetSendGameServer() == true)
    {
        DominationManager::Instance()->SendDominationContinentLordInfo(lord_info.map_code);
    }

    Query_Continent_Lord_Info_Select::FREE(query_result);
    query_result = NULL;
}

#endif //_NA_007128_20140326_MONDSCHIEN_DOMINATION

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION

Handler_Guild_GuildToDbp_IMPL(DZ_GUILD_JOIN_REQUEST_INSERT_SYN)
{
    const MSG_DZ_GUILD_JOIN_REQUEST_INSERT_SYN* packet = \
        static_cast<MSG_DZ_GUILD_JOIN_REQUEST_INSERT_SYN*>(pMsg);

    STRING_SAFER_QUERY1024 query_buff;
    query_buff.MakeString("{? = call S_GuildJoinRequest_Insert (%d, %d)}", 
        packet->request_info.m_char_guid, 
        packet->request_info.m_guild_guid);

    Query_GuildJoinRequest_Insert* query = Query_GuildJoinRequest_Insert::ALLOC();
    query->SetIndex(MAKEDWORD((WORD)DZ_GUILD, (WORD)DZ_GUILD_JOIN_REQUEST_INSERT_DBR));
    query->SetQuery(query_buff);
    query->SetVoidObject(pServerSession);
    query->SetGuildJoinRequestInfo(packet->request_info);
    pServerSession->DBQuery(query);
}

Handler_Guild_GuildToDbp_IMPL(DZ_GUILD_JOIN_REQUEST_INSERT_DBR)
{
    const MSG_DBPROXY_RESULT* db_result = static_cast<MSG_DBPROXY_RESULT*>(pMsg);
    Query_GuildJoinRequest_Insert* query_result = 
        static_cast<Query_GuildJoinRequest_Insert*>(db_result->m_pData);

    if (query_result->IsError())
    {
        SUNLOG(eCRITICAL_LOG,  "%s : %s", query_result->GetQuery(),  query_result->GetErrorMsg());
        Query_GuildJoinRequest_Insert::FREE(query_result);
        query_result = NULL;
        return;
    }

    if (query_result->parameters.result_ != 0)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__":Error code = %d", query_result->parameters.result_);
        Query_GuildJoinRequest_Insert::FREE(query_result);
        query_result = NULL;
        return;
    }

    GUILD_JOIN_REQUEST_INFO request_info = query_result->GetGuildJoiinRequestInfo();
    int result_count = query_result->GetResultRowNum();
    for(int i = 0; i < result_count; ++i)
    {
        tm request_tm;
        sscanf(query_result->result_records[i].request_time_, "%4d-%2d-%2d %2d:%2d:%2d",
            &request_tm.tm_year, &request_tm.tm_mon, &request_tm.tm_mday, &request_tm.tm_hour, &request_tm.tm_min, &request_tm.tm_sec);
        request_tm.tm_year -= 1900;
        request_tm.tm_mon -= 1;
        request_tm.tm_isdst = 0;
        request_info.m_request_time = mktime(&request_tm);
    }
    request_info.m_remain_time = MAX_GUILD_JOIN_REQUEST_TIME_MAX;
    g_GuildManager.InsertGuildJoinRequest(&request_info);

    MSG_GZ_GUILD_JOIN_REQUEST_ACK msg;
    msg.request_info = request_info;
    msg.result_ = RC::RC_GUILD_SUCCESS;
    g_DBProxyServer.SendToSessionType(GAME_SERVER, &msg, sizeof(msg));

    //WorldServer에 길드 가입 요청 쪽지를 보내도록 요청
    Guild* guild = g_GuildManager.FindGuild(request_info.m_guild_guid);
    if (guild != NULL)
    {
        CorpsMemberMap member_map;
        //쪽지는 길드 마스터, 서브 마스터, 근위 대장에게 보낸다.
        guild->FindMembersByDuty(eGUILD_DUTY_MASTER, member_map);
        guild->FindMembersByDuty(eGUILD_DUTY_SUBMASTER, member_map);
        guild->FindMembersByDuty(eGUILD_DUTY_LEADER, member_map);

        CorpsMemberMap::iterator cur_iter = member_map.begin();
        CorpsMemberMap::iterator end_iter = member_map.end();
        for (; cur_iter != end_iter; ++cur_iter)
        {
            GuildMember* guild_member = cur_iter->second;
            if (guild_member == NULL)
            {
                continue;
            }

            MSG_DW_MEMO_SEND_SYN MemoMsg;
            MemoMsg.m_dwKey     = 0;
            MemoMsg.m_SendGuid  = 0;
            MemoMsg.m_typeSystemMemo = SYSMEMO_GUILD_JOIN_REQUEST;
            MemoMsg.m_recvItemCode = 0;
            MemoMsg.m_amountCost = 0;
            MemoMsg.m_recvGuid = guild_member->GetCharGuid();
            _tcsncpy( MemoMsg.m_ptszRecvName, guild_member->GetCharName(), MAX_CHARNAME_LENGTH );
            MemoMsg.m_ptszRecvName[MAX_CHARNAME_LENGTH] = '\0';
            _sntprintf(MemoMsg.m_ptszMemo, MAX_MEMO_LENGTH, "%d|%s|", SYSMEMO_GUILD_JOIN_REQUEST, request_info.m_char_name);
            MemoMsg.m_ptszMemo[MAX_MEMO_LENGTH] = '\0';

            Handler_FromWorldServer::OnDW_MEMO_SEND_SYN((ChildServerSession*)pServerSession,
                (MSG_BASE*)&MemoMsg, sizeof(MSG_DW_MEMO_SEND_SYN));
        }
    }

    Query_GuildJoinRequest_Insert::FREE(query_result);
    query_result = NULL;
    return;
}


Handler_Guild_GuildToDbp_IMPL(DZ_GUILD_JOIN_REQUEST_DELETE_SYN)
{
    const MSG_DZ_GUILD_JOIN_REQUEST_DELETE_SYN* packet = \
        static_cast<MSG_DZ_GUILD_JOIN_REQUEST_DELETE_SYN*>(pMsg);

    STRING_SAFER_QUERY1024 query_buff;
    query_buff.MakeString("{? = call S_GuildJoinRequest_Delete (%d, %d)}", 
        packet->target_guid, 
        packet->guild_guid);

    Query_GuildJoinRequest_Delete* query = Query_GuildJoinRequest_Delete::ALLOC();
    query->SetIndex(MAKEDWORD((WORD)DZ_GUILD, (WORD)DZ_GUILD_JOIN_REQUEST_DELETE_DBR));
    query->SetQuery(query_buff);
    query->SetVoidObject(pServerSession);
    query->SetGuildGuid(packet->guild_guid);
    query->SetTargetGuid(packet->target_guid);
    query->SetCharGuid(packet->char_guid);
    query->SetRejectType(packet->reject_type);
    pServerSession->DBQuery(query);
}

Handler_Guild_GuildToDbp_IMPL(DZ_GUILD_JOIN_REQUEST_DELETE_DBR)
{
    const MSG_DBPROXY_RESULT* db_result = static_cast<MSG_DBPROXY_RESULT*>(pMsg);
    Query_GuildJoinRequest_Delete* query_result = 
        static_cast<Query_GuildJoinRequest_Delete*>(db_result->m_pData);

    if (query_result->IsError())
    {
        SUNLOG(eCRITICAL_LOG,  "%s : %s", query_result->GetQuery(),  query_result->GetErrorMsg());
        Query_GuildJoinRequest_Delete::FREE(query_result);
        query_result = NULL;
        return;
    }

    if (query_result->parameters.result_ != 0)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__":Error code = %d", query_result->parameters.result_);
        Query_GuildJoinRequest_Delete::FREE(query_result);
        query_result = NULL;
        return;
    }

    GUILDGUID guild_guid = query_result->GetGuildGuid();
    CHARGUID target_guid = query_result->GetTargetGuid();

    RC::eGUILD_RESULT result = g_GuildManager.DeleteGuildJoinRequest(target_guid, guild_guid);

    MSG_GZ_GUILD_JOIN_REQUEST_REJECT_ACK msg;
    msg.Init();
    msg.char_guid = query_result->GetCharGuid();
    msg.target_guid = target_guid;
    msg.guild_guid = guild_guid;
    msg.result = result;
    msg.reject_type = query_result->GetRejectType();
    Guild* guild = g_GuildManager.FindGuild(guild_guid);
    if (guild)
    {
        util::GUILDNAMECOPY(msg.guild_name, guild->GetGuildName());
    }
    g_DBProxyServer.SendToSessionType(GAME_SERVER, &msg, sizeof(msg));

    Query_GuildJoinRequest_Delete::FREE(query_result);
    query_result = NULL;
    return;
}

Handler_Guild_GuildToDbp_IMPL(DZ_GUILD_JOIN_REQUEST_SELECT_SYN)
{
    const MSG_DZ_GUILD_JOIN_REQUEST_SELECT_SYN* packet = \
        static_cast<MSG_DZ_GUILD_JOIN_REQUEST_SELECT_SYN*>(pMsg);

    STRING_SAFER_QUERY1024 query_buff;
    query_buff.MakeString("{? = call S_GuildJoinRequest_Select (%d)}", packet->guild_guid);

    Query_GuildJoinRequest_Select* query = Query_GuildJoinRequest_Select::ALLOC();
    query->SetIndex(MAKEDWORD((WORD)DZ_GUILD, (WORD)DZ_GUILD_JOIN_REQUEST_SELECT_DBR));
    query->SetQuery(query_buff);
    query->SetVoidObject(pServerSession);
    query->SetGuildGuid(packet->guild_guid);
    pServerSession->DBQuery(query);
}

Handler_Guild_GuildToDbp_IMPL(DZ_GUILD_JOIN_REQUEST_SELECT_DBR)
{
    const MSG_DBPROXY_RESULT* db_result = static_cast<MSG_DBPROXY_RESULT*>(pMsg);
    Query_GuildJoinRequest_Select* query_result = 
        static_cast<Query_GuildJoinRequest_Select*>(db_result->m_pData);

    if (query_result->IsError())
    {
        SUNLOG(eCRITICAL_LOG,  "%s : %s", query_result->GetQuery(),  query_result->GetErrorMsg());
        Query_GuildJoinRequest_Select::FREE(query_result);
        query_result = NULL;
        return;
    }

    if (query_result->parameters.result_ != 0)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__":Error code = %d", query_result->parameters.result_);
        Query_GuildJoinRequest_Select::FREE(query_result);
        query_result = NULL;
        return;
    }

    GUILDGUID guild_guid = query_result->GetGuildGuid();

    int result_count = query_result->GetResultRowNum();
    for(int i = 0; i < result_count; ++i)
    {
        if (query_result->result_records[i].char_guid_ == 0)
        {
            break;
        }
        GUILD_JOIN_REQUEST_INFO request_info;
        request_info.m_char_guid  = query_result->result_records[i].char_guid_;
        strncpy(request_info.m_char_name, query_result->result_records[i].char_name_, MAX_CHARNAME_LENGTH);
        request_info.m_char_class = query_result->result_records[i].char_class_;
        request_info.m_char_level = query_result->result_records[i].char_level_;
        request_info.m_guild_guid = query_result->result_records[i].guild_guid_;

        tm request_tm;

        sscanf(query_result->result_records[i].request_time_, "%4d-%2d-%2d %2d:%2d:%2d",
            &request_tm.tm_year, &request_tm.tm_mon, &request_tm.tm_mday, &request_tm.tm_hour, &request_tm.tm_min, &request_tm.tm_sec);
        request_tm.tm_year -= 1900;
        request_tm.tm_mon -= 1;
        request_tm.tm_isdst = 0;
        request_info.m_request_time = mktime(&request_tm);
        request_info.m_remain_time = MAX_GUILD_JOIN_REQUEST_TIME_MAX;
        g_GuildManager.InsertGuildJoinRequest(&request_info);
    }

    Query_GuildJoinRequest_Select::FREE(query_result);
    query_result = NULL;
}

Handler_Guild_GuildToDbp_IMPL(DZ_GUILD_FACILITY_UPDATE_SYN)
{
    const MSG_DZ_GUILD_FACILITY_UPDATE_SYN* packet = \
        static_cast<MSG_DZ_GUILD_FACILITY_UPDATE_SYN*>(pMsg);

    STRING_SAFER_QUERY1024 query_buff;
    query_buff.MakeString("{? = call S_GuildFacility_Update (%d, %d, %d)}", 
        packet->guild_guid, packet->facility_code, packet->add_level + packet->current_level);

    Query_GuildFacility_Update* query = Query_GuildFacility_Update::ALLOC();
    query->SetIndex(MAKEDWORD((WORD)DZ_GUILD, (WORD)DZ_GUILD_FACILITY_UPDATE_DBR));
    query->SetQuery(query_buff);
    query->SetVoidObject(pServerSession);
    query->SetCharGuid(packet->char_guid);
    query->SetCurLevel(packet->current_level);
    query->SetAddLevel(packet->add_level);
    query->SetGuildGuid(packet->guild_guid);
    query->SetFacilityCode(packet->facility_code);
    pServerSession->DBQuery(query);
}

Handler_Guild_GuildToDbp_IMPL(DZ_GUILD_FACILITY_UPDATE_DBR)
{
    const MSG_DBPROXY_RESULT* db_result = static_cast<MSG_DBPROXY_RESULT*>(pMsg);
    Query_GuildFacility_Update* query_result = 
        static_cast<Query_GuildFacility_Update*>(db_result->m_pData);

    if (query_result->IsError())
    {
        SUNLOG(eCRITICAL_LOG,  "%s : %s", query_result->GetQuery(),  query_result->GetErrorMsg());
        Query_GuildFacility_Update::FREE(query_result);
        query_result = NULL;
        return;
    }

    if (query_result->parameters.result_ != 0)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__":Error code = %d", query_result->parameters.result_);
        Query_GuildFacility_Update::FREE(query_result);
        query_result = NULL;
        return;
    }

    GUILDGUID guild_guid = query_result->GetGuildGuid();
    FCODE   facility_code = query_result->GetFacilityCode();
    FLEVEL  add_level = query_result->GetAddLevel();
    FLEVEL  cur_level = query_result->GetCurLevel();

    GUILD_FACILITY_INFO new_info;
    new_info.code = facility_code;
    new_info.level = add_level + cur_level;

    DWORD result = g_GuildManager.UpdateGuildFacility(new_info, add_level, guild_guid, query_result->GetCharGuid());
    if (result != RC::RC_GUILD_SUCCESS)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__" Failed Update Guild Facility (%d, %d)",add_level, guild_guid);
        Query_GuildFacility_Update::FREE(query_result);
        query_result = NULL;
        return;
    }

    MSG_GZ_GUILD_FACILITY_UPGRADE_ACK msg;
    msg.facility_info = new_info;
    msg.facility_code = new_info.code;
    msg.guild_guid = guild_guid;
    msg.char_guid = query_result->GetCharGuid();
    msg.add_level = add_level;
    msg.cur_level = cur_level;
    msg.result = RC::RC_GUILD_SUCCESS;
    g_DBProxyServer.SendToSessionType(GAME_SERVER, &msg, sizeof(msg));

    Query_GuildFacility_Update::FREE(query_result);
    query_result = NULL;
}


Handler_Guild_GuildToDbp_IMPL(DZ_GUILD_FACILITY_SELECT_SYN)
{
    const MSG_DZ_GUILD_FACILITY_SELECT_SYN* packet = \
        static_cast<MSG_DZ_GUILD_FACILITY_SELECT_SYN*>(pMsg);

    STRING_SAFER_QUERY1024 query_buff;
    query_buff.MakeString("{? = call S_GuildFacility_Select (%d)}", packet->guild_guid);

    Query_GuildFacility_Select* query = Query_GuildFacility_Select::ALLOC();
    query->SetIndex(MAKEDWORD((WORD)DZ_GUILD, (WORD)DZ_GUILD_FACILITY_SELECT_DBR));
    query->SetQuery(query_buff);
    query->SetVoidObject(pServerSession);
    query->SetGuildGuid(packet->guild_guid);
    pServerSession->DBQuery(query);
}

Handler_Guild_GuildToDbp_IMPL(DZ_GUILD_FACILITY_SELECT_DBR)
{
    const MSG_DBPROXY_RESULT* db_result = static_cast<MSG_DBPROXY_RESULT*>(pMsg);
    Query_GuildFacility_Select* query_result = 
        static_cast<Query_GuildFacility_Select*>(db_result->m_pData);

    if (query_result->IsError())
    {
        SUNLOG(eCRITICAL_LOG,  "%s : %s", query_result->GetQuery(),  query_result->GetErrorMsg());
        Query_GuildFacility_Select::FREE(query_result);
        query_result = NULL;
        return;
    }

    if (query_result->parameters.result_ != 0)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__":Error code = %d", query_result->parameters.result_);
        Query_GuildFacility_Select::FREE(query_result);
        query_result = NULL;
        return;
    }

    GUILDGUID guild_guid = query_result->GetGuildGuid();

    MSG_GZ_GUILD_FACILITY_SELECT_ACK ack_msg;
    ack_msg.guild_guid = guild_guid;
    int result_count = query_result->GetResultRowNum();
    ack_msg.total_info.m_count = result_count;
    for(int i = 0; i < result_count; ++i)
    {
        GUILD_FACILITY_INFO facility_info;
        facility_info.code = query_result->result_records[i].facility_code;
        facility_info.level = query_result->result_records[i].facility_level;

        g_GuildManager.SetGuildFacility(facility_info, guild_guid);

        ack_msg.total_info.m_GuildFacilityInfo[i] = facility_info;
    }
    g_DBProxyServer.SendToSessionType(GAME_SERVER, &ack_msg, sizeof(ack_msg));

    Query_GuildFacility_Select::FREE(query_result);
    query_result = NULL;
}

Handler_Guild_GuildToDbp_IMPL(DZ_GUILD_FACILITY_INIT_SYN)
{
    const MSG_DZ_GUILD_FACILITY_INIT_SYN* packet = \
        static_cast<MSG_DZ_GUILD_FACILITY_INIT_SYN*>(pMsg);

    if (g_GuildManager.InitializeGuildPointAndFacility(packet->guild_guid) == false)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__": Failed initialize guild facility (%d)", packet->guild_guid);
    }
}
#endif //_NA_007330_20140620_GUILD_SYSTEM_EXTENSION

#ifdef _NA_008334_20150608_SONNENSCHEIN
Handler_Guild_GuildToDbp_IMPL(DZ_GUILD_SONNENSCHEIN_INITIALIZE_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
    MSG_DZ_GUILD_SONNENSCHEIN_INITIALIZE_SYN * pRecvMsg = \
        static_cast<MSG_DZ_GUILD_SONNENSCHEIN_INITIALIZE_SYN *>(pMsg);

    STRING_SAFER_QUERY64 szQueryBuff;

    szQueryBuff.MakeString( "{?=call S_Guild_SonnenSchein_Initialize}" );

    SonnenScheinInitializeQuery * query = SonnenScheinInitializeQuery::ALLOC();

    query->SetUserKey( pRecvMsg->m_dwKey );
    query->SetSessionIndex( pRecvMsg->m_dwSessionIndex );	//게임서버의 SessionIndex를 저장한다.

    query->SetQuery(szQueryBuff);
    query->SetIndex( MAKEDWORD( (WORD)DZ_GUILD, (WORD)DZ_GUILD_SONNENSCHEIN_INITIALIZE_DBR ) );
    query->SetVoidObject( pServerSession );
    pServerSession->DBQuery( query );
}

Handler_Guild_GuildToDbp_IMPL(DZ_GUILD_SONNENSCHEIN_INITIALIZE_DBR)
{
    const MSG_DBPROXY_RESULT* db_result = static_cast<MSG_DBPROXY_RESULT*>(pMsg);
    SonnenScheinInitializeQuery* query_result = 
        static_cast<SonnenScheinInitializeQuery*>(db_result->m_pData);

    //쿼리 실패
    if (query_result->IsError())
    {
        SUNLOG(eCRITICAL_LOG,  "%s : %s", query_result->GetQuery(),  query_result->GetErrorMsg());
        SonnenScheinInitializeQuery::FREE(query_result);
        query_result = NULL;
        return;
    }
    //쿼리 실패
    if (query_result->ResultSuccess() == FALSE)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__":Error code = %d", query_result->parameter[0].result);
        SonnenScheinInitializeQuery::FREE(query_result);
        query_result = NULL;
        return;
    }

    MSG_DZ_GUILD_SONNENSCHEIN_INITIALIZE_ACK ack_msg;
    Handler_Guild_DbpToGuild::OnDZ_GUILD_SONNENSCHEIN_INITIALIZE_ACK( g_DBHandler.GetSharedSession(), 
        &ack_msg, sizeof(ack_msg));

    SonnenScheinInitializeQuery::FREE(query_result);
    query_result = NULL;
}

Handler_Guild_GuildToDbp_IMPL(DZ_GUILD_SONNENSCHEIN_UPDATE_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));WAVERIX_GUILD_DEBUGGING();
    
    MSG_DZ_GUILD_SONNENSCHEIN_UPDATE_SYN* syn_msg = \
        static_cast<MSG_DZ_GUILD_SONNENSCHEIN_UPDATE_SYN*>(pMsg);

    STRING_SAFER_QUERY64 szQueryBuff;

    szQueryBuff.MakeString( "{?=call S_Guild_SonnenSchein_Update(%d, %d)}", 
                                                syn_msg->guild_guid, syn_msg->sonnenschein_phase );

    SonnenScheinUpdateQuery * query = SonnenScheinUpdateQuery::ALLOC();

    query->SetUserKey( syn_msg->m_dwKey );
    query->SetSessionIndex( syn_msg->m_dwSessionIndex );	//게임서버의 SessionIndex를 저장한다.
    query->SetGuildGuid(syn_msg->guild_guid);
    query->SetSonnenScheinPhase(syn_msg->sonnenschein_phase);

    query->SetQuery(szQueryBuff);
    query->SetIndex( MAKEDWORD( (WORD)DZ_GUILD, (WORD)DZ_GUILD_SONNENSCHEIN_UPDATE_DBR ) );
    query->SetVoidObject( pServerSession );
    pServerSession->DBQuery( query );
}

Handler_Guild_GuildToDbp_IMPL(DZ_GUILD_SONNENSCHEIN_UPDATE_DBR)
{
    const MSG_DBPROXY_RESULT* db_result = static_cast<MSG_DBPROXY_RESULT*>(pMsg);
    SonnenScheinUpdateQuery* query_result = 
        static_cast<SonnenScheinUpdateQuery*>(db_result->m_pData);

    //쿼리 실패
    if (query_result->IsError())
    {
        SUNLOG(eCRITICAL_LOG,  "%s : %s", query_result->GetQuery(),  query_result->GetErrorMsg());
        SonnenScheinUpdateQuery::FREE(query_result);
        query_result = NULL;
        return;
    }
    //쿼리 실패
    if (query_result->ResultSuccess() == FALSE)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__":Error code = %d", query_result->parameter[0].result);
        SonnenScheinUpdateQuery::FREE(query_result);
        query_result = NULL;
        return;
    }

    MSG_DZ_GUILD_SONNENSCHEIN_UPDATE_ACK ack_msg;
    ack_msg.guild_guid = query_result->GetGuildGuid();
    ack_msg.sonnenschein_phase = query_result->GetSonnenScheinPhase();

    Handler_Guild_DbpToGuild::OnDZ_GUILD_SONNENSCHEIN_UPDATE_ACK( g_DBHandler.GetSharedSession(), 
                                                                  &ack_msg, sizeof(ack_msg));

    SonnenScheinUpdateQuery::FREE(query_result);
    query_result = NULL;
}
#endif //_NA_008334_20150608_SONNENSCHEIN