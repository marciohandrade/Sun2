#include "StdAfx.h"
#include ".\handler_fromportalserver.h"

#if !defined(_NA002635_GP_DP_PROTOCOL_UINIFICATION)

#include <PacketStruct_DP.h>
#include "PortalServerQuery.h"
#include "ChildServerSession.h"
#include <ArchiveOfStream.h>
#include <SolarDateTime.h>
#include <RankingManager.h>

//==================================================================================================
// CHANGES: f110518.2L
const nsPacket::PacketHandlerInfo Handler_FromPortalServer::GetPacketHandlerNodes()
{
#define HANDLER_NODE_INFO(c, p, phase) { MAKEWORD(c, p), false, phase, Handler_FromPortalServer::On##p }
#define HANDLER_NODE_INFO_DBR(c, p, phase) { MAKEWORD(c, p), true, phase, Handler_FromPortalServer::On##p }
#define HANDLER_NODE_INFO_EOR() { 0, false, static_cast<ePHASE_SHIFT>(0), NULL }

    static const nsPacket::PacketHandlerNode handlers_static_[] =
    {
        //
        HANDLER_NODE_INFO(DP_RANKING, DP_RANKING_INFO_REQ_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DP_RANKING, DP_RANKING_INFO_REQ_DBR, PHASE_UNLIMITED),
        HANDLER_NODE_INFO(DP_RANKING, DP_RANKING_MISSION_CLEAR_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DP_RANKING, DP_RANKING_MISSION_CLEAR_DBR, PHASE_UNLIMITED),
        HANDLER_NODE_INFO(DP_RANKING, DP_RANKING_MISSION_REWARD_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DP_RANKING, DP_RANKING_MISSION_REWARD_DBR, PHASE_UNLIMITED),

        HANDLER_NODE_INFO(DP_RANKING, DP_RANKING_REGISTER_MISSION_POINT_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DP_RANKING, DP_RANKING_REGISTER_MISSION_POINT_DBR, PHASE_UNLIMITED),
        //
        HANDLER_NODE_INFO_EOR() // end of records
    };
    BOOST_STATIC_ASSERT(_countof(handlers_static_) >= 1);
    nsPacket::PacketHandlerInfo return_value = { handlers_static_, _countof(handlers_static_) - 1 };
    //
    return return_value;
};

//==================================================================================================


Handler_FromPortalServer_IMPL( DP_RANKING_INFO_REQ_SYN )
{
    __TOUCHED((pServerSession, wSize));
	MSG_DP_RANKING_INFO_REQ_SYN * pRecvMsg = (MSG_DP_RANKING_INFO_REQ_SYN *)pMsg;

	static char buff[128];
	std::string strQuery = "{?=call S_Ranking_Select( ?, ?, ?,";
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

RankingManager g_RankingHelper;
Handler_FromPortalServer_IMPL( DP_RANKING_INFO_REQ_DBR )
{
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

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	g_RankingHelper.ClearRanking();
	SHORT PreRows = 0;	
	ASSERT( Query_Ranking_Select::MAX_ROWS == eRANKING_MAX );
	for( int type = 0 ; type < eRANKING_MAX ; ++type )
	{
		SHORT MaxRows = pQResult->pParam[0].m_Rows[type];
		
		Query_Ranking_Select::sQUERY * pData = &pQResult->pResult[PreRows];
		for( SHORT row = 0 ; row < MaxRows ; ++row )
		{
			MissionRanking * pRanking = g_RankingHelper.CreateRanking( (eRANKING_TYPE)type, pData[row].m_MissionNo );
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
	g_RankingHelper.SortAll();
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	g_RankingHelper.Display();

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
				pServerSession->SendPacket( &msg, msg.GetSize() );
			}
		}
	}
	
	MSG_DP_RANKING_INFO_REQ_END_NTF ntf;
	pServerSession->SendPacket( &ntf, sizeof(ntf) );

	Query_Ranking_Select::FREE(pQResult); pQResult = NULL;
}

Handler_FromPortalServer_IMPL( DP_RANKING_MISSION_CLEAR_SYN )
{
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
		pServerSession->SendPacket( &msg, sizeof(msg) );
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

Handler_FromPortalServer_IMPL( DP_RANKING_MISSION_CLEAR_DBR )
{
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
		pServerSession->SendPacket( &msg, sizeof(msg) );

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
	pServerSession->SendPacket( &msg, sizeof(msg) );

	Query_Ranking_Insert::FREE(pQResult); pQResult = NULL;
}

Handler_FromPortalServer_IMPL( DP_RANKING_MISSION_REWARD_SYN )
{
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

	pQuery->SetQuery(query_string.c_str());
	pQuery->SetIndex( MAKEDWORD( (WORD)DP_RANKING, (WORD)DP_RANKING_MISSION_REWARD_DBR ) );
	pQuery->SetVoidObject( pServerSession );
	pServerSession->DBQuery( pQuery );
}

Handler_FromPortalServer_IMPL( DP_RANKING_MISSION_REWARD_DBR )
{
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
		pServerSession->SendPacket( &msg, sizeof(msg) );

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
	pServerSession->SendPacket( &msg, sizeof(msg) );

	Query_Ranking_Reward_Update::FREE(pQResult); pQResult = NULL;
}

Handler_FromPortalServer_IMPL( DP_RANKING_REGISTER_MISSION_POINT_SYN )
{
    __TOUCHED((pServerSession, wSize));
	MSG_DP_RANKING_REGISTER_MISSION_POINT_SYN* pRecvMsg = (MSG_DP_RANKING_REGISTER_MISSION_POINT_SYN*)pMsg;	

	static char buff[128];
	_MISSION_RANKING_TOTAL_INFO & rTotalInfo = pRecvMsg->TotalInfo;	
	std::string query_string  = "{?=call S_Ranking_Update_MissionType2(" ;
	query_string = query_string+_itoa(pRecvMsg->DBKey, buff, 10 )+",";
	query_string = query_string+_itoa(rTotalInfo.m_Info.m_MissionNo, buff, 10 )+",";
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
		pServerSession->SendPacket( &msg, sizeof(msg) );		

		return;
	}
	
	Query_Ranking_Register_Update * pQuery = Query_Ranking_Register_Update::ALLOC();
	pQuery->SetUserKey( pRecvMsg->m_dwKey );
	pQuery->SetSessionIndex( pRecvMsg->m_dwSessionIndex );
	pQuery->SetTempKey( pRecvMsg->TempKey );
	pQuery->SetDBKey( pRecvMsg->DBKey);
	pQuery->SetMissionNo( rTotalInfo.m_Info.m_MissionNo );
	pQuery->SetQuery(query_string.c_str());
	pQuery->SetIndex( MAKEDWORD( (WORD)DP_RANKING, (WORD)DP_RANKING_REGISTER_MISSION_POINT_DBR ) );
	pQuery->SetVoidObject( pServerSession );
	pServerSession->DBQuery( pQuery );
}

Handler_FromPortalServer_IMPL( DP_RANKING_REGISTER_MISSION_POINT_DBR )
{
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
		msg.m_byErrorCode	= RC::RC_RANKING_NOT_REGIST_INVALID_PERIOD;		
		pServerSession->SendPacket( &msg, sizeof(msg) );

		SUNLOG( eCRITICAL_LOG, "[DP_RANKING_REGISTER_MISSION_POINT_DBR][오류]:%s", pQResult->GetQuery() );
		Query_Ranking_Register_Update::FREE(pQResult); pQResult = NULL;

		return;
	}
    
	MSG_DP_RANKING_REGISTER_MISSION_POINT_ACK msg;
	msg.TempKey		= pQResult->GetTempKey();
	msg.DBKey		= pQResult->GetDBKey();
	msg.MissionNo	= pQResult->GetMissionNo();

	pServerSession->SendPacket( &msg, sizeof(msg) );

	Query_Ranking_Register_Update::FREE(pQResult); pQResult = NULL;
}

#endif //if !defined(_NA002635_GP_DP_PROTOCOL_UINIFICATION)