#include "stdafx.h"
#include "./Handler_FromGameServer.Ranking.h"
#ifdef _NA002635_GP_DP_PROTOCOL_UINIFICATION_IN_DBP
//==================================================================================================

#include <ArchiveOfStream.h>
#include <SolarDateTime.h>

#include <PacketStruct_DG.h>
#include <PacketStruct_DP.h>
#include <PacketStruct_GP.h>

#include <RankingManager.h>

#include "QueryObjects/PortalServerQuery.h"
#include "Sessions/ChildServerSession.h"
#include "Services/PortalRankingManager.h"

//==================================================================================================
// CHANGES: f110518.2L
const nsPacket::PacketHandlerInfo Handler_GP::GetPacketHandlerNodes()
{
#define HANDLER_NODE_INFO(c, p, phase) { MAKEWORD(c, p), false, phase, Handler_GP::On##p }
#define HANDLER_NODE_INFO_DBR(c, p, phase) { MAKEWORD(c, p), true, phase, Handler_GP::On##p }
#define HANDLER_NODE_INFO_EOR() { 0, false, static_cast<ePHASE_SHIFT>(0), NULL }

    static const nsPacket::PacketHandlerNode handlers_static_[] =
    {
#ifdef _NA_008012_20150130_RANKING_SYSTEM
        //
        HANDLER_NODE_INFO(GP_RANKING, GP_RANKING_INFO_REQ_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO(GP_RANKING, GP_RANKING_MISSION_CLEAR_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO(GP_RANKING, GP_RANKING_MISSION_REWARD_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO(GP_RANKING, GP_RANKING_SETTLE_CMD, PHASE_UNLIMITED),
        HANDLER_NODE_INFO(GP_RANKING, GP_RANKING_REGISTER_MISSION_POINT_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO(GP_RANKING, GP_MISSION_RANKING_SETTLE_DATE_SYN, PHASE_UNLIMITED),
        //
#endif // _NA_008012_20150130_RANKING_SYSTEM

        HANDLER_NODE_INFO_EOR() // end of records
    };
    BOOST_STATIC_ASSERT(_countof(handlers_static_) >= 1);
    nsPacket::PacketHandlerInfo return_value = { handlers_static_, _countof(handlers_static_) - 1 };
    //
    return return_value;

#undef HANDLER_NODE_INFO
#undef HANDLER_NODE_INFO_DBR
#undef HANDLER_NODE_INFO_EOR
};

//==================================================================================================
// CHANGES: f110518.2L
const nsPacket::PacketHandlerInfo Handler_P2D::GetPacketHandlerNodes()
{
#define HANDLER_NODE_INFO(c, p, phase) { MAKEWORD(c, p), false, phase, Handler_P2D::On##p }
#define HANDLER_NODE_INFO_DBR(c, p, phase) { MAKEWORD(c, p), true, phase, Handler_P2D::On##p }
#define HANDLER_NODE_INFO_EOR() { 0, false, static_cast<ePHASE_SHIFT>(0), NULL }

    static const nsPacket::PacketHandlerNode handlers_static_[] =
    {
        //
#ifdef _NA_008012_20150130_RANKING_SYSTEM
        HANDLER_NODE_INFO_DBR(DP_RANKING, DP_RANKING_INFO_REQ_DBR, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DP_RANKING, DP_RANKING_MISSION_CLEAR_DBR, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DP_RANKING, DP_RANKING_MISSION_REWARD_DBR, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DP_RANKING, DP_RANKING_REGISTER_MISSION_POINT_DBR, PHASE_UNLIMITED),
#endif // _NA_008012_20150130_RANKING_SYSTEM
        //
        HANDLER_NODE_INFO_EOR() // end of records
    };
    BOOST_STATIC_ASSERT(_countof(handlers_static_) >= 1);
    nsPacket::PacketHandlerInfo return_value = { handlers_static_, _countof(handlers_static_) - 1 };
    //
    return return_value;

#undef HANDLER_NODE_INFO
#undef HANDLER_NODE_INFO_DBR
#undef HANDLER_NODE_INFO_EOR
};

//==================================================================================================
//==================================================================================================
//==================================================================================================

#ifdef _NA_008012_20150130_RANKING_SYSTEM
Handler_P2D_IMPL(DP_RANKING_INFO_REQ_SYN)
{
    WAVERIX_WRAPPER((WAVERIX_LC_CALL1("waverix_log_trace_ranking",
                    "|"__FUNCTION__"|{}=")));
    __TOUCHED((pServerSession, wSize));
	MSG_DP_RANKING_INFO_REQ_SYN * pRecvMsg = (MSG_DP_RANKING_INFO_REQ_SYN *)pMsg;

	static char buff[128];
#ifdef _NA_006801_20130705_RENEWAL_RANKINGSYSTEM
    std::string strQuery = "{?=call S_Ranking_Select( ?, ?,";
#else
	std::string strQuery = "{?=call S_Ranking_Select( ?, ?, ?,";
#endif //_NA_006801_20130705_RENEWAL_RANKINGSYSTEM
		strQuery = strQuery + _ultoa( pRecvMsg->m_SettledStartDate, buff, 10 )+",";
		strQuery = strQuery + _ultoa( pRecvMsg->m_Period, buff, 10 )+") }";

	SUNLOG( eDEV_LOG, "[Handler_FromPortalServer_IMPL( DP_RANKING_INFO_REQ_SYN )] %s", strQuery.c_str() );

	Query_Ranking_Select * pQuery = Query_Ranking_Select::ALLOC();
	pQuery->clear();
	//pQuery->SetSessionIndex( pRecvMsg->m_dwSessionIndex );
	pQuery->SetQuery( strQuery.c_str() );
	pQuery->SetIndex( MAKEDWORD( (WORD)DP_RANKING, (WORD)DP_RANKING_INFO_REQ_DBR ) );
	pQuery->SetVoidObject( pServerSession );
	pServerSession->DBQuery( pQuery );
}

Handler_P2D_IMPL(DP_RANKING_INFO_REQ_DBR)
{
    WAVERIX_WRAPPER((WAVERIX_LC_CALL1("waverix_log_trace_ranking",
                    "|"__FUNCTION__"|{}=")));
    __TOUCHED((pServerSession, wSize));
	MSG_DBPROXY_RESULT * pResult	= (MSG_DBPROXY_RESULT *)pMsg;
	Query_Ranking_Select * pQResult	= (Query_Ranking_Select *)pResult->m_pData;

	int total = 0;

    for( int type = 0 ; type < Query_Ranking_Select::MAX_ROWS ; ++type )
	{
		total += pQResult->pParam[0].m_Rows[type];
	}
	if( total < 0 || total > Query_Ranking_Select::RESULT_ROW_NUM )
	{
		SUNLOG( eCRITICAL_LOG, _T("개수를 넘었습니다.(%d>%u)"), total, Query_Ranking_Select::RESULT_ROW_NUM );
		Query_Ranking_Select::FREE(pQResult); pQResult = NULL;
		return ;
	}
    //RankingManager g_RankingHelper;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	g_RankingMgr.ClearRanking();
	SHORT PreRows = 0;	

	ASSERT( Query_Ranking_Select::MAX_ROWS == eRANKING_MAX );
	for( int type = 0 ; type < eRANKING_MAX ; ++type )
	{
		SHORT MaxRows = pQResult->pParam[0].m_Rows[type];
		
		Query_Ranking_Select::sQUERY * pData = &pQResult->pResult[PreRows];
		for( SHORT row = 0 ; row < MaxRows ; ++row )
		{
#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
            MissionRanking * pRanking = g_RankingMgr.CreateRanking( (eRANKING_TYPE)type, pData[row].m_MissionNo, 
                                                                    pData[row].ranking_type );
            if (pRanking == NULL)
                continue;

#else
            MissionRanking * pRanking = g_RankingMgr.CreateRanking( (eRANKING_TYPE)type, pData[row].m_MissionNo );
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
			ASSERT(pRanking);
			MissionRankingRecord * pRecord = pRanking->AddRanking_Info(	pData[row].m_Key,
																		pData[row].m_ClearTime,
																		pData[row].m_RegisterTime );
			ASSERT( pRecord );
			BYTE MaxMemberCount = 0;
			for( ; MaxMemberCount < MAX_PARTYMEMBER_NUM ; ++MaxMemberCount )
			{
				if( 0 == pData[row].m_CharInfo[MaxMemberCount].m_UserGuid ) break;
			}
			Query_Ranking_Select::sQUERY::_MEMBER * pMemberInfo = pData[row].m_CharInfo;
			for( int count = 0 ; count < MaxMemberCount ; ++count )
			{
				pRecord->AddMember_Info(
					pMemberInfo[count].m_Reward,
					pMemberInfo[count].m_Class,
					pMemberInfo[count].m_LV,
					pMemberInfo[count].m_CharName,
					pMemberInfo[count].m_GuildName );
				SUNLOG( eDEV_LOG, "[Handler_FromPortalServer_IMPL( DP_RANKING_INFO_REQ_DBR )] Type = %d, MissionNo = %d, Name = %s, MemTotal = %d", type, pData[row].m_MissionNo, pMemberInfo[count].m_CharName, MaxMemberCount );
			}
		}
		PreRows += MaxRows;
	}
	g_RankingMgr.SortAll();
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	g_RankingMgr.Display();
#if SUN_CODE_BACKUP
	static const int MAX_PAGE = 5;
	static MSG_DP_RANKING_INFO_REQ_ACK msg;
	//msg.m_dwSessionIndex = pQResult->GetSessionIndex();
	for( int type = 0 ; type < eRANKING_MAX ; ++type )
	{
		for( int MissionNo = 0 ; MissionNo < MAX_MAPCODE_NUM ; ++MissionNo )
		{
			MissionRanking * pRanking = g_RankingHelper.FindMissionRanking( (eRANKING_TYPE)type, MissionNo );
			if( !pRanking ) continue;

			for( int page = 0 ; page < MAX_PAGE ; ++page )
			{
				msg.m_Type		= type;
				msg.m_MissionNo = MissionNo;
				msg.m_Page		= page;
				msg.m_Size		= msg.MAX_BUFFER_SIZE;
				ArchiveOfStream lArchive(msg.m_pBUFFER, msg.m_Size);
				pRanking->Serialize( msg.m_Page, lArchive, RANKING_SERIALIZE_LOAD );
				msg.m_Size = lArchive.GetSize();
                Handler_D2P::OnDP_RANKING_INFO_REQ_ACK(pServerSession, &msg, msg.GetSize());
			}
		}
	}
#endif
	MSG_DP_RANKING_INFO_REQ_END_NTF ntf;
    Handler_D2P::OnDP_RANKING_INFO_REQ_END_NTF(pServerSession, &ntf, sizeof(ntf));

	Query_Ranking_Select::FREE(pQResult); pQResult = NULL;
}

Handler_P2D_IMPL(DP_RANKING_MISSION_CLEAR_SYN)
{
    WAVERIX_WRAPPER((WAVERIX_LC_CALL1("waverix_log_trace_ranking",
                    "|"__FUNCTION__"|{}=")));
    __TOUCHED((pServerSession, wSize));
	MSG_DP_RANKING_MISSION_CLEAR_SYN * pRecvMsg = (MSG_DP_RANKING_MISSION_CLEAR_SYN *)pMsg;
	
	_MISSION_RANKING_TOTAL_INFO & rTotalInfo = pRecvMsg->m_totalInfo;
	SHORT TotalLV = 0;
	for( int i = 0 ; i < rTotalInfo.m_Count ; ++i )
	{
		TotalLV += rTotalInfo.m_Member[i].m_LV;
	}

	static char buff[128];
	std::string query_string  = "{?=call S_Ranking_Insert(?," ;
	query_string = query_string+_itoa(rTotalInfo.m_Info.m_MissionNo, buff, 10 )+",";
#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
    //S_Ranking_Insert는 미션랭킹같은 중복형 랭킹에만 적용되고, 그것은 1번타입의 랭킹뿐이다. 1번을 넣어주자.
    query_string = query_string+_itoa((BYTE)eRANKING_TYPE_TIME_ATTACK, buff, 10 )+",";
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
	query_string = query_string+_itoa(rTotalInfo.m_Info.m_ClearTime, buff, 10 )+",";
	query_string = query_string+_itoa(TotalLV, buff, 10 )+",";
	query_string = query_string+_itoa(rTotalInfo.m_Count, buff, 10 )+",";
	query_string = query_string+_i64toa(rTotalInfo.m_Info.m_RegisterTime, buff, 10 )+",'";
	util::GetSQLDateTime_YYYYMMDDHHMMSSNNN( rTotalInfo.m_Info.m_RegisterTime, buff );
	query_string = query_string+buff+"'";

	for( int i = 0 ; i < rTotalInfo.m_Count ; ++i )
	{
		query_string += ",";

		query_string = query_string+_itoa(rTotalInfo.m_Member[i].m_UserGuid, buff, 10 )+",'";
		query_string = query_string+rTotalInfo.m_Member[i].m_tszCharName+"',";
		query_string = query_string+_itoa(rTotalInfo.m_Member[i].m_LV, buff, 10 )+",";
		query_string = query_string+_itoa(rTotalInfo.m_Member[i].m_Class, buff, 10 )+",'";
		query_string = query_string+rTotalInfo.m_Member[i].m_tszGuildName+"'";
	}
	query_string += ")}";

	//DISPMSG( query_string.c_str() );
	if( query_string.size() >= Query::MAX_QUERY_LENGTH ) 
	{
		MSG_DP_RANKING_MISSION_CLEAR_NAK msg;
		msg.m_MissionNo		= rTotalInfo.m_Info.m_MissionNo;
		msg.m_byErrorCode	= RC::RC_RANKING_DBPROXY_SP_LENGTH_OVERFLOW;
        Handler_D2P::OnDP_RANKING_MISSION_CLEAR_NAK(pServerSession, &msg, sizeof(msg));
		return;
	}

	Query_Ranking_Insert * pQuery = Query_Ranking_Insert::ALLOC();
	pQuery->SetUserKey( pRecvMsg->m_dwKey );
	pQuery->SetSessionIndex( pRecvMsg->m_dwSessionIndex );
	pQuery->SetOldRankingKey( pRecvMsg->m_Key );
	pQuery->SetMissionNo( rTotalInfo.m_Info.m_MissionNo );
	pQuery->SetQuery(query_string.c_str());
	pQuery->SetIndex( MAKEDWORD( (WORD)DP_RANKING, (WORD)DP_RANKING_MISSION_CLEAR_DBR ) );
	pQuery->SetVoidObject( pServerSession );
	pServerSession->DBQuery( pQuery );
}

Handler_P2D_IMPL(DP_RANKING_MISSION_CLEAR_DBR)
{
    WAVERIX_WRAPPER((WAVERIX_LC_CALL1("waverix_log_trace_ranking",
                    "|"__FUNCTION__"|{}=")));
    __TOUCHED((pServerSession, wSize));
	MSG_DBPROXY_RESULT * pResult	= (MSG_DBPROXY_RESULT *)pMsg;
	Query_Ranking_Insert * pQResult	= (Query_Ranking_Insert *)pResult->m_pData;

	if( !pQResult->ResultSuccess() )
	{
		MSG_DP_RANKING_MISSION_CLEAR_NAK msg;
		msg.m_dwKey			= pQResult->GetUserKey();
		msg.m_dwSessionIndex = pQResult->GetSessionIndex();
		msg.m_Key			= pQResult->GetOldRankingKey();
		msg.m_MissionNo		= pQResult->GetMissionNo();
		msg.m_byErrorCode	= RC::RC_RANKING_DBPROXY_SP_TRANS_ERROR;
        Handler_D2P::OnDP_RANKING_MISSION_CLEAR_NAK(pServerSession, &msg, sizeof(msg));

		SUNLOG( eCRITICAL_LOG, "[DP_RANKING_MISSION_CLEAR_DBR][오류]:%s", pQResult->GetQuery() );
		Query_Ranking_Insert::FREE(pQResult); pQResult = NULL;

		return;
	}

	MSG_DP_RANKING_MISSION_CLEAR_ACK msg;
	msg.m_dwKey				= pQResult->GetUserKey();
	msg.m_dwSessionIndex = pQResult->GetSessionIndex();
	msg.m_Key				= pQResult->GetOldRankingKey();
	msg.m_NewKey			= pQResult->pParam[0].m_Key;
	msg.m_MissionNo			= pQResult->GetMissionNo();
    Handler_D2P::OnDP_RANKING_MISSION_CLEAR_ACK(pServerSession, &msg, sizeof(msg));

	Query_Ranking_Insert::FREE(pQResult); pQResult = NULL;
}

Handler_P2D_IMPL(DP_RANKING_MISSION_REWARD_SYN)
{
    WAVERIX_WRAPPER((WAVERIX_LC_CALL1("waverix_log_trace_ranking",
                    "|"__FUNCTION__"|{}=")));
    __TOUCHED((pServerSession, wSize));
	MSG_DP_RANKING_MISSION_REWARD_SYN * pRecvMsg = (MSG_DP_RANKING_MISSION_REWARD_SYN *)pMsg;

	char buff[128];
	std::string query_string = "{?=call S_Ranking_Reward_Update(";
	query_string = query_string+_itoa(pRecvMsg->m_Key, buff, 10 )+",'";
	query_string = query_string+pRecvMsg->m_tszCharName+"')}";
	

	Query_Ranking_Reward_Update * pQuery = Query_Ranking_Reward_Update::ALLOC();
	
	pQuery->SetSessionIndex( pRecvMsg->m_dwSessionIndex );
	pQuery->SetKey(pRecvMsg->m_Key);
	pQuery->SetUserKey(pRecvMsg->m_dwKey);
	pQuery->SetCharName(pRecvMsg->m_tszCharName);
	pQuery->SetMissionNo(pRecvMsg->m_MissionNo);
#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
    pQuery->SetRankingType(pRecvMsg->ranking_type);
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR

	pQuery->SetQuery(query_string.c_str());
	pQuery->SetIndex( MAKEDWORD( (WORD)DP_RANKING, (WORD)DP_RANKING_MISSION_REWARD_DBR ) );
	pQuery->SetVoidObject( pServerSession );
	pServerSession->DBQuery( pQuery );
}

Handler_P2D_IMPL(DP_RANKING_MISSION_REWARD_DBR)
{
    WAVERIX_WRAPPER((WAVERIX_LC_CALL1("waverix_log_trace_ranking",
                    "|"__FUNCTION__"|{}=")));
    __TOUCHED((pServerSession, wSize));
	MSG_DBPROXY_RESULT * pResult	= (MSG_DBPROXY_RESULT *)pMsg;
	Query_Ranking_Reward_Update * pQResult	= (Query_Ranking_Reward_Update *)pResult->m_pData;

	if( !pQResult->ResultSuccess() )
	{
		MSG_DP_RANKING_MISSION_REWARD_NAK msg;
		msg.m_dwKey = pQResult->GetUserKey();
		msg.m_dwSessionIndex = pQResult->GetSessionIndex();
		msg.m_byErrorCode	= 2;
		msg.m_Key			= pQResult->GetKey();
        _tcsncpy(msg.m_tszCharName, pQResult->GetCharName(), _countof(msg.m_tszCharName));
        msg.m_tszCharName[_countof(msg.m_tszCharName) - 1] = _T('\0');
		msg.m_MissionNo		= pQResult->GetMissionNo();
#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
        msg.ranking_type    = pQResult->GetRankingType();
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
        Handler_D2P::OnDP_RANKING_MISSION_REWARD_NAK(pServerSession, &msg, sizeof(msg));

		SUNLOG( eCRITICAL_LOG, "[DP_RANKING_MISSION_REWARD_DBR][오류]:%s", pQResult->GetQuery() );
		Query_Ranking_Reward_Update::FREE(pQResult); pQResult = NULL;

		return;
	}

	MSG_DP_RANKING_MISSION_REWARD_ACK msg;
	msg.m_dwKey = pQResult->GetUserKey();
	msg.m_dwSessionIndex = pQResult->GetSessionIndex();
	msg.m_Key			= pQResult->GetKey();
    _tcsncpy(msg.m_tszCharName, pQResult->GetCharName(), _countof(msg.m_tszCharName));
    msg.m_tszCharName[_countof(msg.m_tszCharName) - 1] = _T('\0');
	msg.m_MissionNo		= pQResult->GetMissionNo();
#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
    msg.ranking_type    = pQResult->GetRankingType();
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
    Handler_D2P::OnDP_RANKING_MISSION_REWARD_ACK(pServerSession, &msg, sizeof(msg));

	Query_Ranking_Reward_Update::FREE(pQResult); pQResult = NULL;
}

Handler_P2D_IMPL(DP_RANKING_REGISTER_MISSION_POINT_SYN)
{
    WAVERIX_WRAPPER((WAVERIX_LC_CALL1("waverix_log_trace_ranking",
                    "|"__FUNCTION__"|{}=")));
    __TOUCHED((pServerSession, wSize));
	MSG_DP_RANKING_REGISTER_MISSION_POINT_SYN* pRecvMsg = (MSG_DP_RANKING_REGISTER_MISSION_POINT_SYN*)pMsg;	

	static char buff[128];
	_MISSION_RANKING_TOTAL_INFO & rTotalInfo = pRecvMsg->TotalInfo;	
	std::string query_string  = "{?=call S_Ranking_Update_MissionType2(" ;
	query_string = query_string+_itoa(pRecvMsg->DBKey, buff, 10 )+",";
	query_string = query_string+_itoa(rTotalInfo.m_Info.m_MissionNo, buff, 10 )+",";
#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
    query_string = query_string+_itoa(rTotalInfo.m_Info.ranking_type, buff, 10 )+",";
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
	query_string = query_string+_itoa(rTotalInfo.m_Info.m_ClearTime, buff, 10 )+",";
	query_string = query_string+_itoa(rTotalInfo.m_Member[0].m_LV, buff, 10 )+",";
	query_string = query_string+_itoa(rTotalInfo.m_Count, buff, 10 )+",";
	query_string = query_string+_i64toa(rTotalInfo.m_Info.m_RegisterTime, buff, 10 )+",'";
	util::GetSQLDateTime_YYYYMMDDHHMMSSNNN( rTotalInfo.m_Info.m_RegisterTime, buff );
	query_string = query_string+buff+"',";
	
	query_string = query_string + _ultoa( pRecvMsg->Period, buff, 10 )+",";
	query_string = query_string + _ultoa( pRecvMsg->SettledStartDate, buff, 10 )+",";
	
	query_string = query_string+_itoa(rTotalInfo.m_Member[0].m_UserGuid, buff, 10 )+",'";
	query_string = query_string+rTotalInfo.m_Member[0].m_tszCharName+"',";
	query_string = query_string+_itoa(rTotalInfo.m_Member[0].m_LV, buff, 10 )+",";
	query_string = query_string+_itoa(rTotalInfo.m_Member[0].m_Class, buff, 10 )+",'";
	query_string = query_string+rTotalInfo.m_Member[0].m_tszGuildName+"'";

	query_string += ")}";

	SUNLOG( eDEV_LOG, "[Handler_FromPortalServer_IMPL( DP_RANKING_REGISTER_MISSION_POINT_SYN )] %s = ", query_string.c_str() );
	
	
	if( query_string.size() >= Query::MAX_QUERY_LENGTH ) 
	{
		SUNLOG(eFULL_LOG, "DP_RANKING_REGISTER_MISSION_POINT_SYN - DB LENGTH OVERFLOW" );

		MSG_DP_RANKING_REGISTER_MISSION_POINT_NAK msg;
		msg.m_OldKey		= pRecvMsg->TempKey;
		msg.m_MissionNo		= rTotalInfo.m_Info.m_MissionNo;
		msg.m_byErrorCode	= RC::RC_RANKING_DBPROXY_SP_LENGTH_OVERFLOW;
        Handler_D2P::OnDP_RANKING_REGISTER_MISSION_POINT_NAK(pServerSession, &msg, sizeof(msg));

		return;
	}
	
	Query_Ranking_Register_Update * pQuery = Query_Ranking_Register_Update::ALLOC();
	pQuery->SetUserKey( pRecvMsg->m_dwKey );
	pQuery->SetSessionIndex( pRecvMsg->m_dwSessionIndex );
	pQuery->SetTempKey( pRecvMsg->TempKey );
	pQuery->SetDBKey( pRecvMsg->DBKey);
	pQuery->SetMissionNo( rTotalInfo.m_Info.m_MissionNo );
#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
    pQuery->SetRanking_Type(rTotalInfo.m_Info.ranking_type);
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
	pQuery->SetQuery(query_string.c_str());
	pQuery->SetIndex( MAKEDWORD( (WORD)DP_RANKING, (WORD)DP_RANKING_REGISTER_MISSION_POINT_DBR ) );
	pQuery->SetVoidObject( pServerSession );
	pServerSession->DBQuery( pQuery );
}

Handler_P2D_IMPL(DP_RANKING_REGISTER_MISSION_POINT_DBR)
{
    WAVERIX_WRAPPER((WAVERIX_LC_CALL1("waverix_log_trace_ranking",
                    "|"__FUNCTION__"|{}=")));
    __TOUCHED((pServerSession, wSize));
	MSG_DBPROXY_RESULT * pResult	= (MSG_DBPROXY_RESULT *)pMsg;
	Query_Ranking_Register_Update * pQResult	= (Query_Ranking_Register_Update *)pResult->m_pData;

	if( !pQResult->ResultSuccess() )
	{
		MSG_DP_RANKING_REGISTER_MISSION_POINT_NAK msg;
		msg.m_dwKey			= pQResult->GetUserKey();
		msg.m_dwSessionIndex = pQResult->GetSessionIndex();
		msg.m_OldKey		= pQResult->GetTempKey();
		msg.m_MissionNo		= pQResult->GetMissionNo();	
#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
        msg.ranking_type    = pQResult->GetRanking_Type();
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
		msg.m_byErrorCode	= RC::RC_RANKING_NOT_REGIST_INVALID_PERIOD;		
        Handler_D2P::OnDP_RANKING_REGISTER_MISSION_POINT_NAK(pServerSession, &msg, sizeof(msg));

		SUNLOG( eCRITICAL_LOG, "[DP_RANKING_REGISTER_MISSION_POINT_DBR][오류]:%s", pQResult->GetQuery() );
		Query_Ranking_Register_Update::FREE(pQResult); pQResult = NULL;

		return;
	}
    
	MSG_DP_RANKING_REGISTER_MISSION_POINT_ACK msg;
	msg.TempKey		= pQResult->GetTempKey();
	msg.DBKey		= pQResult->GetDBKey();
	msg.MissionNo	= pQResult->GetMissionNo();
#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
    msg.ranking_type = pQResult->GetRanking_Type();
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
    Handler_D2P::OnDP_RANKING_REGISTER_MISSION_POINT_ACK(pServerSession, &msg, sizeof(msg));

	Query_Ranking_Register_Update::FREE(pQResult); pQResult = NULL;
}

//==================================================================================================
//==================================================================================================
//==================================================================================================
//==================================================================================================
//==================================================================================================
//==================================================================================================

Handler_D2P_IMPL(DP_RANKING_INFO_REQ_ACK)
{
    __TOUCHED((pServerSession, wSize));
    WAVERIX_WRAPPER((WAVERIX_LC_CALL1("waverix_log_trace_ranking",
                    "|"__FUNCTION__"|{}=")));
	MSG_DP_RANKING_INFO_REQ_ACK * recv_msg = (MSG_DP_RANKING_INFO_REQ_ACK*)pMsg;
	
	DISPMSG( "Ranking Stream Received(Type:%u,No:%u,Page:%u,Size:%u)\n", 
		recv_msg->m_Type, recv_msg->m_MissionNo, recv_msg->m_Page, recv_msg->m_Size );
	g_RankingMgr.Serialize( (eRANKING_TYPE)recv_msg->m_Type,
        recv_msg->m_MissionNo, recv_msg->m_Page, recv_msg->m_pBUFFER, recv_msg->m_Size, RANKING_SERIALIZE_SAVE );	
}

Handler_D2P_IMPL(DP_RANKING_INFO_REQ_END_NTF)
{
    __TOUCHED((pServerSession, wSize));
    WAVERIX_WRAPPER((WAVERIX_LC_CALL1("waverix_log_trace_ranking",
                    "|"__FUNCTION__"|{}=")));
	MSG_DP_RANKING_INFO_REQ_END_NTF * recv_msg = (MSG_DP_RANKING_INFO_REQ_END_NTF*)pMsg;

	DISPMSG( "Ranking Stream Received Successfully....Status is Normal and ServerSide Listen\n" );
	g_RankingMgr.SetStatus( eRANKING_STATUS_NORMAL );
    g_RankingMgr.ChangeToRankingLoaded();
}

Handler_D2P_IMPL(DP_RANKING_INFO_REQ_NAK)
{
    __TOUCHED((pServerSession, wSize));
    WAVERIX_WRAPPER((WAVERIX_LC_CALL1("waverix_log_trace_ranking",
                    "|"__FUNCTION__"|{}=")));
	MSG_DP_RANKING_INFO_REQ_NAK * recv_msg = (MSG_DP_RANKING_INFO_REQ_NAK*)pMsg;

	SUNLOG( eCRITICAL_LOG, _T("DP_RANKING_INFO_REQ_NAK Error[%u] can't receive Ranking Info from DBProxy"), recv_msg->m_byErrorCode );
}


Handler_D2P_IMPL(DP_RANKING_MISSION_CLEAR_ACK)
{
    __TOUCHED((pServerSession, wSize));
    WAVERIX_WRAPPER((WAVERIX_LC_CALL1("waverix_log_trace_ranking",
                    "|"__FUNCTION__"|{}=")));
	MSG_DP_RANKING_MISSION_CLEAR_ACK * recv_msg = (MSG_DP_RANKING_MISSION_CLEAR_ACK *)pMsg;
	
	// 등수에 들어가면 모든 서버에 SUCCESS BRD
	// 등수에 들어가지 못하면 그 서버에만 FAILED BRD

	ServerSession * pGameServer = g_DBProxyServer.FindSession(recv_msg->m_dwSessionIndex);
	ASSERT( pGameServer && pGameServer->GetServerType() == GAME_SERVER );

	WORD Ranking = (WORD)g_RankingMgr.OnRegisterMissionRecordOnTodayweek( TRUE, recv_msg->m_MissionNo, recv_msg->m_Key, recv_msg->m_NewKey );
	if( 0 == Ranking )
	{
		if( pGameServer )
		{
			MSG_GP_RANKING_MISSION_FAILED_NAK msg;
			msg.m_dwKey			= recv_msg->m_dwKey;
			msg.m_Key			= recv_msg->m_Key;
			msg.m_MissionNo		= recv_msg->m_MissionNo;
			msg.m_dwErrorCode	= RC::RC_RANKING_DONT_INCLUDE_RANKING;
			pGameServer->SendPacket( &msg, sizeof(msg) );
		}
	}
	else
	{
		if( pGameServer )
		{
			MSG_GP_RANKING_MISSION_SUCCESS_ACK amsg;
			amsg.m_dwKey		= recv_msg->m_dwKey;
			amsg.m_Key			= recv_msg->m_Key;
			amsg.m_NewKey		= recv_msg->m_NewKey;
			amsg.m_MissionNo	= recv_msg->m_MissionNo;
			amsg.m_Ranking		= Ranking;
			pGameServer->SendPacket( &amsg, sizeof(amsg) );
		}
		MissionRankingRecord * pRecord = g_RankingMgr.FindMissionRecord( eRANKING_TODAYWEEK, recv_msg->m_MissionNo, recv_msg->m_NewKey );
		ASSERT(pRecord);
		if( pRecord )
		{
			MSG_GP_RANKING_MISSION_SUCCESS_BRD msg;
			msg.m_NewKey = recv_msg->m_NewKey;
			msg.m_RankingInfo.m_Info.m_MissionNo = recv_msg->m_MissionNo;
#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
            msg.m_RankingInfo.m_Info.ranking_type = eRANKING_TYPE_TIME_ATTACK;
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
			pRecord->Serialize( msg.m_RankingInfo, SERIALIZE_LOAD );
			g_DBProxyServer.SendToSessionType( GAME_SERVER, &msg, sizeof(msg), recv_msg->m_dwSessionIndex );
		}
	}
}

Handler_D2P_IMPL(DP_RANKING_MISSION_CLEAR_NAK)
{
    __TOUCHED((pServerSession, wSize));
    WAVERIX_WRAPPER((WAVERIX_LC_CALL1("waverix_log_trace_ranking",
                    "|"__FUNCTION__"|{}=")));
	MSG_DP_RANKING_MISSION_CLEAR_NAK * recv_msg = (MSG_DP_RANKING_MISSION_CLEAR_NAK *)pMsg;

	g_RankingMgr.OnRegisterMissionRecordOnTodayweek( FALSE, 0, recv_msg->m_Key, 0 );

	// 게임서버로 포워딩
	ServerSession * pGameServer = g_DBProxyServer.FindSession(recv_msg->m_dwSessionIndex);
	ASSERT( pGameServer && pGameServer->GetServerType() == GAME_SERVER );
	if( pGameServer )
	{
		MSG_GP_RANKING_MISSION_FAILED_NAK msg;
		msg.m_dwKey			= recv_msg->m_dwKey;
		msg.m_Key			= recv_msg->m_Key;
		msg.m_MissionNo		= recv_msg->m_MissionNo;
		msg.m_dwErrorCode	= recv_msg->m_byErrorCode;
		pGameServer->SendPacket( &msg, sizeof(msg) );
	}
}

Handler_D2P_IMPL(DP_RANKING_MISSION_REWARD_ACK)
{
    __TOUCHED((pServerSession, wSize));
    WAVERIX_WRAPPER((WAVERIX_LC_CALL1("waverix_log_trace_ranking",
                    "|"__FUNCTION__"|{}=")));
	MSG_DP_RANKING_MISSION_REWARD_ACK * recv_msg = (MSG_DP_RANKING_MISSION_REWARD_ACK *)pMsg;

#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
    g_RankingMgr.OnReward( TRUE, recv_msg->m_MissionNo, recv_msg->m_Key, recv_msg->m_tszCharName, 
        recv_msg->ranking_type);
#else
    g_RankingMgr.OnReward( TRUE, recv_msg->m_MissionNo, recv_msg->m_Key, recv_msg->m_tszCharName);
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR

	// 게임서버로 포워딩
	ServerSession * pGameServer = g_DBProxyServer.FindSession(recv_msg->m_dwSessionIndex);
	ASSERT( pGameServer && pGameServer->GetServerType() == GAME_SERVER );
	if( pGameServer )
	{
		MSG_GP_RANKING_MISSION_REWARD_ACK msg;
		msg.m_dwKey		= recv_msg->m_dwKey;
		msg.m_Key		= recv_msg->m_Key;
		msg.m_MissionNo	= recv_msg->m_MissionNo;
#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
        msg.ranking_type = recv_msg->ranking_type;
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
		_tcsncpy(msg.m_tszCharName, recv_msg->m_tszCharName, MAX_CHARNAME_LENGTH);
		msg.m_tszCharName[MAX_CHARNAME_LENGTH] = '\0';
		pGameServer->SendPacket( &msg, msg.GetSize() );

		MSG_GP_RANKING_MISSION_REWARD_BRD bmsg;
		bmsg.m_dwKey		= recv_msg->m_dwKey;
		bmsg.m_Key		= recv_msg->m_Key;
		bmsg.m_MissionNo	= recv_msg->m_MissionNo;
#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
        bmsg.ranking_type = recv_msg->ranking_type;
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
		_tcsncpy(bmsg.m_tszCharName, recv_msg->m_tszCharName, MAX_CHARNAME_LENGTH);
		bmsg.m_tszCharName[MAX_CHARNAME_LENGTH] = '\0';
		g_DBProxyServer.SendToSessionType( GAME_SERVER, &bmsg, bmsg.GetSize(), recv_msg->m_dwSessionIndex );
	}

}

Handler_D2P_IMPL(DP_RANKING_MISSION_REWARD_NAK)
{
    __TOUCHED((pServerSession, wSize));
    WAVERIX_WRAPPER((WAVERIX_LC_CALL1("waverix_log_trace_ranking",
                    "|"__FUNCTION__"|{}=")));
	MSG_DP_RANKING_MISSION_REWARD_NAK * recv_msg = (MSG_DP_RANKING_MISSION_REWARD_NAK *)pMsg;

	SUNLOG( eCRITICAL_LOG, 
		_T("DP_RANKING_MISSION_REWARD_NAK Error[%u] can't execute Ranking Reward Action from DBProxy"), 
		recv_msg->m_byErrorCode );

#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
    g_RankingMgr.OnReward( FALSE, recv_msg->m_MissionNo, recv_msg->m_Key, recv_msg->m_tszCharName, 
                           recv_msg->ranking_type );
#else
    g_RankingMgr.OnReward( FALSE, recv_msg->m_MissionNo, recv_msg->m_Key, recv_msg->m_tszCharName );
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR

	// 게임서버로 포워딩
	ServerSession * pGameServer = g_DBProxyServer.FindSession(recv_msg->m_dwSessionIndex);
	ASSERT( pGameServer && pGameServer->GetServerType() == GAME_SERVER );
	if( pGameServer )
	{
		MSG_GP_RANKING_MISSION_REWARD_NAK nmsg;
		nmsg.m_byErrorCode	= recv_msg->m_byErrorCode;
		nmsg.m_Key			= recv_msg->m_Key;
		nmsg.m_MissionNo	= recv_msg->m_MissionNo;
#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
        nmsg.ranking_type   = recv_msg->ranking_type;
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
		_tcsncpy(nmsg.m_tszCharName, recv_msg->m_tszCharName, MAX_CHARNAME_LENGTH);
		nmsg.m_tszCharName[MAX_CHARNAME_LENGTH] = '\0';
		pGameServer->SendPacket( &nmsg, nmsg.GetSize() );
	}
}

Handler_D2P_IMPL(DP_RANKING_REGISTER_MISSION_POINT_ACK)
{
    __TOUCHED((pServerSession, wSize));
    WAVERIX_WRAPPER((WAVERIX_LC_CALL1("waverix_log_trace_ranking",
                    "|"__FUNCTION__"|{}=")));
	MSG_DP_RANKING_REGISTER_MISSION_POINT_ACK * recv_msg = (MSG_DP_RANKING_REGISTER_MISSION_POINT_ACK *)pMsg;	

#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
    WORD Ranking = (WORD)g_RankingMgr.OnRegisterMissionRecordOnTodayweek( TRUE, recv_msg->MissionNo, 
                                      recv_msg->TempKey, recv_msg->DBKey, recv_msg->ranking_type );
#else
    WORD Ranking = (WORD)g_RankingMgr.OnRegisterMissionRecordOnTodayweek( TRUE, recv_msg->MissionNo, recv_msg->TempKey, recv_msg->DBKey );
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
	if( 0 == Ranking )
	{
		//순위권에 못들면 랭킹등록 안함.
		//SUNLOG(eCRITICAL_LOG, "[DP_RANKING_REGISTER_MISSION_POINT_ACK] - OnRegisterMissionRecordOnTodayweek Fail.");
	}
	else
	{
#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
        MissionRankingRecord * pRecord = g_RankingMgr.FindMissionRecord( eRANKING_TODAYWEEK, 
                                                      recv_msg->MissionNo, recv_msg->DBKey, recv_msg->ranking_type );
#else
        MissionRankingRecord * pRecord = g_RankingMgr.FindMissionRecord( eRANKING_TODAYWEEK, recv_msg->MissionNo, recv_msg->DBKey );
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
		ASSERT(pRecord);
		if( pRecord )
		{
			MSG_GP_RANKING_MISSION_SUCCESS_BRD msg;
			msg.m_NewKey = recv_msg->DBKey;
			msg.m_RankingInfo.m_Info.m_MissionNo = recv_msg->MissionNo;
#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
            msg.m_RankingInfo.m_Info.ranking_type = recv_msg->ranking_type;
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
			pRecord->Serialize( msg.m_RankingInfo, SERIALIZE_LOAD );
			g_DBProxyServer.SendToSessionType( GAME_SERVER, &msg, sizeof(msg) );
		}
	}
}

Handler_D2P_IMPL(DP_RANKING_REGISTER_MISSION_POINT_NAK)
{
    __TOUCHED((pServerSession, wSize));
    WAVERIX_WRAPPER((WAVERIX_LC_CALL1("waverix_log_trace_ranking",
                    "|"__FUNCTION__"|{}=")));
	MSG_DP_RANKING_REGISTER_MISSION_POINT_NAK * recv_msg = (MSG_DP_RANKING_REGISTER_MISSION_POINT_NAK *)pMsg;

#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
	g_RankingMgr.OnRegisterMissionRecordOnTodayweek( FALSE, 0, recv_msg->m_OldKey, 0, recv_msg->ranking_type );
#else
	g_RankingMgr.OnRegisterMissionRecordOnTodayweek( FALSE, 0, recv_msg->m_OldKey, 0 );
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR

	// 게임서버로 포워딩 할필요 없음
	// 시퀀스 다이어그램 참고
}


//==================================================================================================
//==================================================================================================
//==================================================================================================
//==================================================================================================
//==================================================================================================
//==================================================================================================

Handler_GP_IMPL(GP_RANKING_INFO_REQ_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    WAVERIX_WRAPPER((WAVERIX_LC_CALL1("waverix_log_trace_ranking",
                    "|"__FUNCTION__"|{}=")));
	MSG_GP_RANKING_INFO_REQ_SYN * pRecvMsg = (MSG_GP_RANKING_INFO_REQ_SYN *)pMsg;
	
	// 이정보를 받을 시점에 이미 Ranking 정보가 Establish되어 있어야 한다.
	// Establish후 Listen

	static const int MAX_PAGE = 5;
	static MSG_GP_RANKING_INFO_REQ_ACK msg;
#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
    for( int type = 0 ; type < eRANKING_MAX ; ++type )
    {
        for( int MissionNo = 0 ; MissionNo < MAX_MAPCODE_NUM ; ++MissionNo )
        {
            for( RANKINGTYPE ranking_type = 1; ranking_type < eRANKING_TYPE_MAX; ++ranking_type)
            {
                MissionRanking * pRanking = g_RankingMgr.FindMissionRanking( (eRANKING_TYPE)type, MissionNo, ranking_type );
                if( !pRanking ) continue;

                for( int page = 0 ; page < MAX_PAGE ; ++page )
                {
                    msg.m_Type		= type;
                    msg.m_MissionNo = MissionNo;
                    msg.ranking_type = ranking_type;
                    msg.m_Page		= page;
                    msg.m_Size		= msg.MAX_BUFFER_SIZE;
                    ArchiveOfStream lArchive(msg.m_pBUFFER, msg.m_Size);
                    pRanking->Serialize( msg.m_Page, lArchive, RANKING_SERIALIZE_LOAD );
                    msg.m_Size = lArchive.GetSize();
                    pServerSession->SendPacket( &msg, msg.GetSize() );
                }
            }
        }
    }
#else
    for( int type = 0 ; type < eRANKING_MAX ; ++type )
    {
        for( int MissionNo = 0 ; MissionNo < MAX_MAPCODE_NUM ; ++MissionNo )
        {
            MissionRanking * pRanking = g_RankingMgr.FindMissionRanking( (eRANKING_TYPE)type, MissionNo );
            if( !pRanking ) continue;

            for( int page = 0 ; page < MAX_PAGE ; ++page )
            {
                msg.m_Type		= type;
                msg.m_MissionNo = MissionNo;
                msg.m_Page		= page;
                msg.m_Size		= msg.MAX_BUFFER_SIZE;
                ArchiveOfStream lArchive(msg.m_pBUFFER, msg.m_Size);
                pRanking->Serialize( msg.m_Page, lArchive, RANKING_SERIALIZE_LOAD );
                msg.m_Size = lArchive.GetSize();
                pServerSession->SendPacket( &msg, msg.GetSize() );
            }
        }
    }
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR

	MSG_GP_RANKING_INFO_REQ_END_NTF ntf;
	ntf.m_SettledYear	= g_RankingMgr.GetOldSettledDate_Year();
	ntf.m_SettledMonth	= g_RankingMgr.GetOldSettledDate_Month();
	ntf.m_SettledDay	= g_RankingMgr.GetOldSettledDate_Day();
	ntf.m_PeriodDay		= g_RankingMgr.GetPeriod();
	ntf.m_SuspendTime	= g_RankingMgr.GetSuspendTime();
	pServerSession->SendPacket( &ntf, sizeof(ntf) );

	//static MSG_GP_RANKING_INFO_REQ_ACK msg;
	//msg.m_Size = MSG_GP_RANKING_INFO_REQ_ACK::MAX_BUFFER_SIZE;
	//g_RankingMgr.Serialize( msg.m_pBUFFER, msg.m_Size,  RANKING_SERIALIZE_LOAD );
	//pServerSession->SendPacket( &msg, msg.GetSize() );
}

Handler_GP_IMPL(GP_RANKING_MISSION_CLEAR_SYN)
{
    __TOUCHED((pServerSession, wSize));
    WAVERIX_WRAPPER((WAVERIX_LC_CALL1("waverix_log_trace_ranking",
                    "|"__FUNCTION__"|{}=")));
	MSG_GP_RANKING_MISSION_CLEAR_SYN * pRecvMsg = (MSG_GP_RANKING_MISSION_CLEAR_SYN *)pMsg;

	RC::eRANKING_RESULT rt = g_RankingMgr.CanRegisterMissionRecord( pRecvMsg->m_totalInfo );
	if( rt == RC::RC_RANKING_SUCCESS )
	{
		MSG_DP_RANKING_MISSION_CLEAR_SYN msg;
		msg.m_dwKey				= pRecvMsg->m_dwKey;
		msg.m_Key				= pRecvMsg->m_Key;
		msg.m_dwSessionIndex	= pServerSession->GetSessionIndex();
		memcpy( &msg.m_totalInfo, &pRecvMsg->m_totalInfo, pRecvMsg->m_totalInfo.GetSize() );
        Handler_P2D::OnDP_RANKING_MISSION_CLEAR_SYN(pServerSession, &msg, msg.GetSize());
		g_RankingMgr.RegisterMissionRecordInTemp( pRecvMsg->m_totalInfo, pRecvMsg->m_Key );
		return;
	}
	
	MSG_GP_RANKING_MISSION_FAILED_NAK nmsg;
	nmsg.m_dwKey		= pRecvMsg->m_dwKey;
	nmsg.m_Key			= pRecvMsg->m_Key;
	nmsg.m_MissionNo		= pRecvMsg->m_totalInfo.m_Info.m_MissionNo;
	nmsg.m_dwErrorCode	= rt;
	pServerSession->SendPacket( &nmsg, sizeof(nmsg) );
}


Handler_GP_IMPL(GP_RANKING_MISSION_REWARD_SYN)
{
    __TOUCHED((pServerSession, wSize));
    WAVERIX_WRAPPER((WAVERIX_LC_CALL1("waverix_log_trace_ranking",
                    "|"__FUNCTION__"|{}=")));
	MSG_GP_RANKING_MISSION_REWARD_SYN * pRecvMsg = (MSG_GP_RANKING_MISSION_REWARD_SYN *)pMsg;
	
	// 보상 여부 체크
	// DoReward
	// DB로 보상 쿼리
	RC::eRANKING_RESULT rt = RC::RC_RANKING_NETWORK_ERROR_FOR_DBPROXY;
#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
    	rt = g_RankingMgr.DoReward( pRecvMsg->m_MissionNo, pRecvMsg->m_Key, pRecvMsg->m_tszCharName, 
                                    pRecvMsg->ranking_type );
#else
    	rt = g_RankingMgr.DoReward( pRecvMsg->m_MissionNo, pRecvMsg->m_Key, pRecvMsg->m_tszCharName );
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
	if( rt == RC::RC_RANKING_SUCCESS )
	{
		MSG_DP_RANKING_MISSION_REWARD_SYN msg;
		msg.m_dwSessionIndex = pServerSession->GetSessionIndex();
		msg.m_dwKey			= pRecvMsg->m_dwKey;
		msg.m_Key			= pRecvMsg->m_Key;						//< 보상받기를 원하는 지난주랭킹의 키(일련번호)
		msg.m_MissionNo 	= pRecvMsg->m_MissionNo;
#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
        msg.ranking_type    = pRecvMsg->ranking_type;
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
		_tcsncpy(msg.m_tszCharName, pRecvMsg->m_tszCharName, MAX_CHARNAME_LENGTH);
		msg.m_tszCharName[MAX_CHARNAME_LENGTH] = '\0';
        Handler_P2D::OnDP_RANKING_MISSION_REWARD_SYN(pServerSession, &msg, msg.GetSize());

		return;
		//------------------------------------------------------------------------------------------
		//------------------------------------------------------------------------------------------
	}
#if SUN_CODE_BACKUP
	MSG_GP_RANKING_MISSION_REWARD_NAK nmsg;
	nmsg.m_dwKey		= pRecvMsg->m_dwKey;
	nmsg.m_byErrorCode	= rt;
	nmsg.m_Key			= pRecvMsg->m_Key;						//< 보상받기를 원하는 지난주랭킹의 키(일련번호)
	nmsg.m_MissionNo	= pRecvMsg->m_MissionNo;
	_tcsncpy(nmsg.m_tszCharName, pRecvMsg->m_tszCharName, MAX_CHARNAME_LENGTH);
	nmsg.m_tszCharName[MAX_CHARNAME_LENGTH] = '\0';
	pServerSession->SendPacket( &nmsg, nmsg.GetSize() );
#endif
}

Handler_GP_IMPL(GP_RANKING_SETTLE_CMD)
{
    __TOUCHED((pServerSession, wSize));
    WAVERIX_WRAPPER((WAVERIX_LC_CALL1("waverix_log_trace_ranking",
                    "|"__FUNCTION__"|{}=")));
	// 현재시간을 정산시간으로 맞추고
	g_RankingMgr.DoTestCurSettle();
	// 자동 정산됨
	//g_RankingMgr.DoSettle();

	// 중지기간을 원상태로 돌려놓는다?
	//g_RankingMgr.InitComputation( tmdate->tm_year+1900, tmdate->tm_mon+1, tmdate->tm_mday, 
	//	tmdate->tm_hour, tmdate->tm_min, tmdate->tm_sec, 1, 15 );
}

Handler_GP_IMPL(GP_RANKING_REGISTER_MISSION_POINT_SYN)
{
    __TOUCHED((pServerSession, wSize));
    WAVERIX_WRAPPER((WAVERIX_LC_CALL1("waverix_log_trace_ranking",
                    "|"__FUNCTION__"|{}=")));
	MSG_GP_RANKING_REGISTER_MISSION_POINT_SYN * pRecvMsg = (MSG_GP_RANKING_REGISTER_MISSION_POINT_SYN *)pMsg;

	//정산중이면 등록 안함
	if( g_RankingMgr.GetStatus() != eRANKING_STATUS_NORMAL )
		return;

	MSG_DP_RANKING_REGISTER_MISSION_POINT_SYN SynMsg;
	SynMsg.m_dwSessionIndex			= pServerSession->GetSessionIndex();
	SynMsg.TotalInfo.m_Count		= 1;
	SynMsg.TotalInfo.m_Info			= pRecvMsg->RecordInfo; 
	SynMsg.TotalInfo.m_Member[0]	= pRecvMsg->MemberInfo;
	SynMsg.SettledStartDate			= g_RankingMgr.GetOldSettledDate();
	SynMsg.Period					= g_RankingMgr.GetPeriod();
	SynMsg.DBKey					= pRecvMsg->DBKey;
	SynMsg.TempKey					= g_RankingMgr.RegisterMissionRecordInTemp( SynMsg.TotalInfo );

    Handler_P2D::OnDP_RANKING_REGISTER_MISSION_POINT_SYN(pServerSession, &SynMsg, SynMsg.GetSize());
}

Handler_GP_IMPL(GP_MISSION_RANKING_SETTLE_DATE_SYN)
{
    __TOUCHED((pServerSession, wSize));
    WAVERIX_WRAPPER((WAVERIX_LC_CALL1("waverix_log_trace_ranking",
                    "|"__FUNCTION__"|{}=")));
	MSG_GP_MISSION_RANKING_SETTLE_DATE_SYN * pRecvMsg = (MSG_GP_MISSION_RANKING_SETTLE_DATE_SYN *)pMsg;

	MSG_GP_MISSION_RANKING_SETTLE_DATE_ACK Ack;
	Ack.RecordInfo			= pRecvMsg->RecordInfo;
	Ack.MemberInfo			= pRecvMsg->MemberInfo;
	Ack.SettledStartDate	= g_RankingMgr.GetOldSettledDate();
	Ack.Period				= g_RankingMgr.GetPeriod();

	pServerSession->SendPacket( &Ack, sizeof(Ack) );
}
#endif // _NA_008012_20150130_RANKING_SYSTEM

#endif //_NA002635_GP_DP_PROTOCOL_UINIFICATION_IN_DBP
