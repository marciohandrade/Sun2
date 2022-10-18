#include "stdafx.h"
#ifdef _NA_008012_20150130_RANKING_SYSTEM
#include "./Handler_PortalServer.h"

#include <PacketStruct_GP.h>
#include <PacketStruct_DG.h>

#include "RewardManager.h"

#include "GameRankingManager.h"
#include "PlayerManager.h"
#include "Player.h"
#include "GameServerEx.h"

//--------------------------------------------------------------------------------------------------
// changes, f100819.1L
const nsPacket::PacketHandlerInfo Handler_RankingServer::GetPacketHandlerNodes()
{
#define HANDLER_NODE_INFO(c, p, phase) { MAKEWORD(c, p), phase, Handler_RankingServer::On##p }
#define HANDLER_NODE_INFO_EOR() { 0, static_cast<ePHASE_SHIFT>(0), NULL }

    static const nsPacket::PacketHandlerNode handlers_static_[] =
    {
        HANDLER_NODE_INFO(GP_RANKING, GP_RANKING_SESSION_CONTROL_CMD, PHASE_SERVICE), // f110511.1L
        HANDLER_NODE_INFO(GP_RANKING, GP_RANKING_INFO_REQ_ACK, PHASE_SERVICE),
        HANDLER_NODE_INFO(GP_RANKING, GP_RANKING_INFO_REQ_NAK, PHASE_SERVICE),
        HANDLER_NODE_INFO(GP_RANKING, GP_RANKING_INFO_REQ_END_NTF, PHASE_SERVICE),

        HANDLER_NODE_INFO(GP_RANKING, GP_RANKING_MISSION_SUCCESS_ACK, PHASE_SERVICE),
        HANDLER_NODE_INFO(GP_RANKING, GP_RANKING_MISSION_SUCCESS_BRD, PHASE_SERVICE),
        HANDLER_NODE_INFO(GP_RANKING, GP_RANKING_MISSION_FAILED_NAK, PHASE_SERVICE),

        HANDLER_NODE_INFO(GP_RANKING, GP_RANKING_MISSION_REWARD_ACK, PHASE_SERVICE),
        HANDLER_NODE_INFO(GP_RANKING, GP_RANKING_MISSION_REWARD_BRD, PHASE_SERVICE),
        HANDLER_NODE_INFO(GP_RANKING, GP_RANKING_MISSION_REWARD_NAK, PHASE_SERVICE),
        HANDLER_NODE_INFO(GP_RANKING, GP_RANKING_SETTLED_INFO_CMD, PHASE_SERVICE),
        HANDLER_NODE_INFO(GP_RANKING, GP_RANKING_SETTLED_INFO_END_NTF, PHASE_SERVICE),
        HANDLER_NODE_INFO(GP_RANKING, GP_RANKING_CHAGED_STATUS_NTF, PHASE_SERVICE),

        HANDLER_NODE_INFO(GP_RANKING, GP_MISSION_RANKING_SETTLE_DATE_ACK, PHASE_SERVICE),
        //
        HANDLER_NODE_INFO_EOR() // end of records
    };
    //
    BOOST_STATIC_ASSERT(_countof(handlers_static_) >= 1);
    nsPacket::PacketHandlerInfo return_value = { handlers_static_, _countof(handlers_static_) - 1 };
    //
    return return_value;
};
//--------------------------------------------------------------------------------------------------
#if SUN_CODE_BACKUP
template< class PACKET_TYPE >
class RankingPacketSender : public std::unary_function<RankingMember *, void>
{
	PACKET_TYPE & m_Packet;
public:
	RankingPacketSender(PACKET_TYPE & p):m_Packet(p){}
	~RankingPacketSender(){}

	void operator() ( RankingMember * pMember ) const
	{
		Player * pPlayer = PlayerManager::Instance()->FindPlayerByName( pMember->GetCharName() );
		if( !pPlayer ) return;

		pPlayer->SendPacket( &m_Packet, sizeof(m_Packet) );
	}
};

class RankingLoger : public std::unary_function<RankingMember *, void>
{
	int			m_LogType;
	RANKINGNO	m_MissionNo;
	BYTE		m_Ranking;
	DWORD		m_ClearTime;
	BYTE		m_Error;
public:
	RankingLoger( int LogType, RANKINGNO MissionNo, BYTE	Ranking, DWORD ClearTime, BYTE Error=0 )
		:m_LogType(LogType)
		,m_MissionNo(MissionNo)
		,m_Ranking(Ranking)
		,m_ClearTime(ClearTime)
		,m_Error(Error)
		{}
	~RankingLoger(){}

	void operator() ( RankingMember * pMember ) const
	{
		Player * pPlayer = PlayerManager::Instance()->FindPlayerByName( pMember->GetCharName() );
		if( !pPlayer ) return;

		GAMELOG->LogAction_Ranking( m_LogType, pPlayer, m_MissionNo, m_Ranking, m_ClearTime, m_Error );
	}
};
#endif //SUN_CODE_BACKUP
//==================================================================================================
namespace util {
;

struct RankingLogArgs
{
    int log_type;
    RANKINGNO mission_no;
    BYTE ranking;
    DWORD clear_time;
    BYTE error_code;
};

static void SendPacketToRankingMembersWithLogging(
    const MissionRankingRecord::MEMBER_HASH& member_table,
    MSG_BASE_FORWARD* msg, uint16_t msg_size, const RankingLogArgs* if_exist_do_logging);

}; //end of namespace
//==================================================================================================

static void util::SendPacketToRankingMembersWithLogging(
    const MissionRankingRecord::MEMBER_HASH& member_table,
    MSG_BASE_FORWARD* msg, uint16_t msg_size, const util::RankingLogArgs* if_exist_do_logging)
{
    MSG_AG_CONNECTION_PACKET_BROADCASTER_ONEtoN_SYN& msg_brd =
        MSG_ONEtoN_BROADCASTER_INTERNAL::InstanceOfBase();
    msg_brd.Init(msg, msg_size);
    //
    uint16_t* const user_begin_it  = msg_brd.GetUsersStartPtr();
    PlayerManager* const player_manager = PlayerManager::Instance();

    MissionRankingRecord::MEMBER_HASH::const_iterator it = member_table.begin();
    const MissionRankingRecord::MEMBER_HASH::const_iterator end = member_table.end();
    for (; it != end; ++it)
    {
        RankingMember* member = it->second;
        DWORD name_hash = member->GetCharNameHash();
        if (name_hash == 0) {
            continue;
        };

        Player* player = player_manager->FindPlayerByNameHash(name_hash);
        if (player == NULL) {
            continue;
        };
        DWORD bound_check = msg_brd.AddUser(user_begin_it, player->GetObjectKey());
        if (bound_check == msg_brd.MAX_USER_COUNT) {
            break;
        };

        if (const util::RankingLogArgs* args = if_exist_do_logging)
        {
            GAMELOG->LogAction_Ranking(args->log_type, player,
                args->mission_no, args->ranking, args->clear_time, args->error_code);
        }
    }

    if (msg_brd.number_of_users) {
        g_pGameServer->SendToServer(AGENT_SERVER, &msg_brd, msg_brd.GetSendingSize());
    };
}
//==================================================================================================

Handler_RankingServer_IMPL(GP_RANKING_SESSION_CONTROL_CMD)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
    WAVERIX_WRAPPER((WAVERIX_LC_CALL1("waverix_log_trace_ranking",
                    "|"__FUNCTION__"|{}=")));
#if !defined(_NA002635_GP_DP_PROTOCOL_UINIFICATION)
    __UNUSED((pServerSession, pRecvMsg, wSize));
#endif
#ifdef _NA002635_GP_DP_PROTOCOL_UINIFICATION
    __UNUSED((pServerSession, wSize));
    const MSG_GP_RANKING_SESSION_CONTROL_CMD* pMsg =
        static_cast<MSG_GP_RANKING_SESSION_CONTROL_CMD*>(pRecvMsg);
    if (pMsg->enabled_service) {
        g_GameRankingMgr.RequestRankingInformation();
    };
#endif
}

Handler_RankingServer_IMPL(GP_RANKING_INFO_REQ_ACK)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
    WAVERIX_WRAPPER((WAVERIX_LC_CALL1("waverix_log_trace_ranking",
                    "|"__FUNCTION__"|{}=")));
	MSG_GP_RANKING_INFO_REQ_ACK * pMsg = (MSG_GP_RANKING_INFO_REQ_ACK*)pRecvMsg;

//	SUNLOG( eCRITICAL_LOG, "Ranking Stream Received(Type:%u,No:%u,Page:%u,Size:%u)", 
//		pMsg->m_Type, pMsg->m_MissionNo, pMsg->m_Page, pMsg->m_Size );

#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
    g_GameRankingMgr.Serialize( (eRANKING_TYPE)pMsg->m_Type, pMsg->m_MissionNo, pMsg->m_Page, 
                                pMsg->m_pBUFFER, pMsg->m_Size, RANKING_SERIALIZE_SAVE, pMsg->ranking_type );
#else
    g_GameRankingMgr.Serialize( (eRANKING_TYPE)pMsg->m_Type, pMsg->m_MissionNo, pMsg->m_Page, pMsg->m_pBUFFER, pMsg->m_Size, RANKING_SERIALIZE_SAVE );
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
}

Handler_RankingServer_IMPL(GP_RANKING_INFO_REQ_END_NTF)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
    WAVERIX_WRAPPER((WAVERIX_LC_CALL1("waverix_log_trace_ranking",
                    "|"__FUNCTION__"|{}=")));
	MSG_GP_RANKING_INFO_REQ_END_NTF * pMsg = (MSG_GP_RANKING_INFO_REQ_END_NTF *)pRecvMsg;

#ifdef _NA_1376_20101123_RANKING_SETTLEMENT_TIME_SHIFT
    int minute = pMsg->m_SuspendTime;
#else
    int minute = 0;
#endif
#ifdef _NA_1376_20101123_RANKING_SETTLEMENT_TIME_SHIFT
    //결산시간을 0시에서 4시로 변경한다
    g_GameRankingMgr.InitComputation( pMsg->m_SettledYear, pMsg->m_SettledMonth, pMsg->m_SettledDay, 4, minute, 0, 
                                      pMsg->m_PeriodDay, pMsg->m_SuspendTime );
#else
    g_GameRankingMgr.InitComputation( pMsg->m_SettledYear, pMsg->m_SettledMonth, pMsg->m_SettledDay, 0, minute, 0, 
                                      pMsg->m_PeriodDay, pMsg->m_SuspendTime );
#endif //_NA_1376_20101123_RANKING_SETTLEMENT_TIME_SHIFT
//	SUNLOG( eCRITICAL_LOG, _T("Ranking Stream Received Successfully..Date:%u%u%u, period:%u, suspendtime:%u"), 
//		pMsg->m_SettledYear, pMsg->m_SettledMonth, pMsg->m_SettledDay, pMsg->m_PeriodDay, pMsg->m_SuspendTime );
    g_GameRankingMgr.ChangeToRankingLoaded();
}


Handler_RankingServer_IMPL(GP_RANKING_INFO_REQ_NAK)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
    WAVERIX_WRAPPER((WAVERIX_LC_CALL1("waverix_log_trace_ranking",
                    "|"__FUNCTION__"|{}=")));
	MSG_GP_RANKING_INFO_REQ_NAK * pMsg = (MSG_GP_RANKING_INFO_REQ_NAK*)pRecvMsg;
	
	SUNLOG( eCRITICAL_LOG, _T("GP_RANKING_INFO_REQ_NAK Error[%u] can't receive Ranking Info from Portal"), pMsg->m_byErrorCode );
}

Handler_RankingServer_IMPL(GP_RANKING_MISSION_SUCCESS_ACK)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
    WAVERIX_WRAPPER((WAVERIX_LC_CALL1("waverix_log_trace_ranking",
                    "|"__FUNCTION__"|{}=")));
	MSG_GP_RANKING_MISSION_SUCCESS_ACK * pMsg = (MSG_GP_RANKING_MISSION_SUCCESS_ACK *)pRecvMsg;

	MissionRankingRecord * pRecord = g_GameRankingMgr.FindTmpMissionRankingRecord( pMsg->m_Key );
    if (pRecord == NULL) {
        ASSERT( pRecord );
        return;
    }


	MSG_CG_MISSION_RANKING_SUCCESS_BRD msg;
	msg.m_Key					= pMsg->m_NewKey;
	msg.m_ClearTime				= pRecord->GetClearTime();
	msg.m_Ranking				= pMsg->m_Ranking;

    util::RankingLogArgs log_args = { 0, };
    log_args.log_type = ACT_MISSION_RANKING_SUCCESS;
    log_args.mission_no = pMsg->m_MissionNo;
    log_args.clear_time = pRecord->GetClearTime();
    // 등수
    log_args.ranking = g_GameRankingMgr.OnRegisterMissionRecordOnTodayweek(
        true, pMsg->m_MissionNo, pMsg->m_Key, pMsg->m_NewKey);
    if (FlowControl::FCAssert(log_args.ranking == pMsg->m_Ranking) == false)
    {
        log_args.log_type = ACT_MISSION_RANKING_FAIL;
        log_args.ranking = 0;
        log_args.error_code = 99;
    };

    // CHANGES: f110517.3L
    util::SendPacketToRankingMembersWithLogging(
        pRecord->GetMemberHash(), &msg, sizeof(msg), &log_args);
}

Handler_RankingServer_IMPL(GP_RANKING_MISSION_SUCCESS_BRD)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
    WAVERIX_WRAPPER((WAVERIX_LC_CALL1("waverix_log_trace_ranking",
                    "|"__FUNCTION__"|{}=")));
	MSG_GP_RANKING_MISSION_SUCCESS_BRD * pMsg = (MSG_GP_RANKING_MISSION_SUCCESS_BRD *)pRecvMsg;

    g_GameRankingMgr.RegisterMissionRecordOnTodayweek( pMsg->m_NewKey, pMsg->m_RankingInfo );
}

Handler_RankingServer_IMPL(GP_RANKING_MISSION_FAILED_NAK)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
    WAVERIX_WRAPPER((WAVERIX_LC_CALL1("waverix_log_trace_ranking",
                    "|"__FUNCTION__"|{}=")));
	MSG_GP_RANKING_MISSION_FAILED_NAK * pMsg = (MSG_GP_RANKING_MISSION_FAILED_NAK *)pRecvMsg;
	SUNLOG( eCRITICAL_LOG, 
		_T("GP_RANKING_MISSION_CLEAR_NAK Error[%u] can't execute Ranking Clear Action from Portal"), 
		pMsg->m_dwErrorCode );

	MissionRankingRecord * pRecord = g_GameRankingMgr.FindTmpMissionRankingRecord( pMsg->m_Key );
    if (pRecord == NULL) {
	    ASSERT( pRecord );
        return;
    }

	MSG_CG_MISSION_RANKING_FAILED_BRD msg;
	msg.m_dwErrorCode			= pMsg->m_dwErrorCode;
	msg.m_ClearTime				= pRecord->GetClearTime();
    //
    util::RankingLogArgs log_args = { 0, };
    log_args.log_type = ACT_MISSION_RANKING_FAIL;
    log_args.mission_no = pMsg->m_MissionNo;
    log_args.clear_time = msg.m_ClearTime;
    log_args.error_code = static_cast<uint8_t>(msg.m_dwErrorCode);
    // CHANGES: f110517.3L
    util::SendPacketToRankingMembersWithLogging(
        pRecord->GetMemberHash(), &msg, sizeof(msg), &log_args);
    //
    g_GameRankingMgr.OnRegisterMissionRecordOnTodayweek(false, 0, pMsg->m_Key, 0);
}
Handler_RankingServer_IMPL(GP_RANKING_MISSION_REWARD_BRD)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
    WAVERIX_WRAPPER((WAVERIX_LC_CALL1("waverix_log_trace_ranking",
                    "|"__FUNCTION__"|{}=")));
	MSG_GP_RANKING_MISSION_REWARD_BRD * pMsg = (MSG_GP_RANKING_MISSION_REWARD_BRD*)pRecvMsg;

#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
    g_GameRankingMgr.DoReward( pMsg->m_MissionNo, pMsg->m_Key, pMsg->m_tszCharName, pMsg->ranking_type );
#else
	g_GameRankingMgr.DoReward( pMsg->m_MissionNo, pMsg->m_Key, pMsg->m_tszCharName );
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
}
Handler_RankingServer_IMPL(GP_RANKING_MISSION_REWARD_ACK)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
    WAVERIX_WRAPPER((WAVERIX_LC_CALL1("waverix_log_trace_ranking",
                    "|"__FUNCTION__"|{}=")));
	MSG_GP_RANKING_MISSION_REWARD_ACK * pMsg = (MSG_GP_RANKING_MISSION_REWARD_ACK*)pRecvMsg;

#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
	g_GameRankingMgr.ExecuteReward( TRUE, pMsg->m_dwKey, pMsg->m_MissionNo, pMsg->m_Key, pMsg->m_tszCharName, 
                                    pMsg->ranking_type );
#else
	g_GameRankingMgr.ExecuteReward( TRUE, pMsg->m_dwKey, pMsg->m_MissionNo, pMsg->m_Key, pMsg->m_tszCharName );
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR

	/*
	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pMsg->m_dwKey );
	if( !pPlayer ) return ;

	MissionRanking * pRanking = g_GameRankingMgr.FindMissionRanking( eRANKING_LASTWEEK, pMsg->m_MissionNo );
	ASSERT(pRanking);
	DWORD Ranking = pRanking->GetRanking( pMsg->m_Key );
	// 랭킹 보상 로그 남겨야 함
	MSG_CG_MISSION_RANKING_REWARD_ACK msg;
	RC::eREWARD_RESULT rt = g_RewardManager.RewardItem( RewardManager::eREWARD_MISSIONRANKING, Ranking, 0, pPlayer, msg.m_ItemInfo );
	ASSERT( rt == RC::RC_REWARD_SUCCESS );
	if( rt == RC::RC_REWARD_SUCCESS )
	{
		pPlayer->SendPacket( &msg, msg.GetSize() );
	}
	else
	{
		SUNLOG( eCRITICAL_LOG, _T("Cannot Ranking Reward %u,%u,%s"), pMsg->m_MissionNo, pMsg->m_Key, pMsg->m_tszCharName );
	}
	*/
}

Handler_RankingServer_IMPL(GP_RANKING_MISSION_REWARD_NAK)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
    WAVERIX_WRAPPER((WAVERIX_LC_CALL1("waverix_log_trace_ranking",
                    "|"__FUNCTION__"|{}=")));
	MSG_GP_RANKING_MISSION_REWARD_NAK * pMsg = (MSG_GP_RANKING_MISSION_REWARD_NAK*)pRecvMsg;

#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
    g_GameRankingMgr.ExecuteReward( FALSE, pMsg->m_dwKey, pMsg->m_MissionNo, pMsg->m_Key, 
                                    pMsg->m_tszCharName, pMsg->ranking_type, pMsg->m_byErrorCode );

#else
    g_GameRankingMgr.ExecuteReward( FALSE, pMsg->m_dwKey, pMsg->m_MissionNo, pMsg->m_Key, pMsg->m_tszCharName, pMsg->m_byErrorCode );

#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
	/*
	SUNLOG( eCRITICAL_LOG, 
		_T("GP_RANKING_MISSION_REWARD_NAK Error[%u] can't execute Ranking Reward Action from Portal"), 
		pMsg->m_byErrorCode );

	g_GameRankingMgr.OnReward( FALSE, pMsg->m_MissionNo, pMsg->m_Key, pMsg->m_tszCharName );

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pMsg->m_dwKey );
	if( !pPlayer ) return ;

	// 플레이어를 찾아서 보내줌
	MSG_CG_MISSION_RANKING_REWARD_NAK msg;
	msg.m_dwErrorCode = pMsg->m_byErrorCode;
	pPlayer->SendPacket( &msg, sizeof(msg) );
	*/
}

Handler_RankingServer_IMPL(GP_RANKING_SETTLED_INFO_CMD)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
    WAVERIX_WRAPPER((WAVERIX_LC_CALL1("waverix_log_trace_ranking",
                    "|"__FUNCTION__"|{}=")));
	MSG_GP_RANKING_SETTLED_INFO_CMD * pMsg = (MSG_GP_RANKING_SETTLED_INFO_CMD*)pRecvMsg;

	SUNLOG( eMIDDLE_LOG, "Settled Info Stream Received(Type:%u,No:%u,Page:%u,Size:%u)", 
		pMsg->m_Type, pMsg->m_MissionNo, pMsg->m_Page, pMsg->m_Size );

#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
    g_GameRankingMgr.Serialize( (eRANKING_TYPE)pMsg->m_Type, pMsg->m_MissionNo, pMsg->m_Page, 
                               pMsg->m_pBUFFER, pMsg->m_Size, RANKING_SERIALIZE_SAVE, pMsg->ranking_type );
#else
    g_GameRankingMgr.Serialize( (eRANKING_TYPE)pMsg->m_Type, pMsg->m_MissionNo, pMsg->m_Page, pMsg->m_pBUFFER, pMsg->m_Size, RANKING_SERIALIZE_SAVE );
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
}

Handler_RankingServer_IMPL(GP_RANKING_SETTLED_INFO_END_NTF)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
    WAVERIX_WRAPPER((WAVERIX_LC_CALL1("waverix_log_trace_ranking",
                    "|"__FUNCTION__"|{}=")));
	MSG_GP_RANKING_SETTLED_INFO_END_NTF * pMsg = (MSG_GP_RANKING_SETTLED_INFO_END_NTF*)pRecvMsg;

	SUNLOG( eCRITICAL_LOG, "--Settled Info is Received Successfully--" );
#ifdef _NA_006801_20130705_RENEWAL_RANKINGSYSTEM
    g_GameRankingMgr.UpdateSettleTime_GameServer();
#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
    //필드서버일 경우에만 동작하도록 한다
    if (g_pGameServer->GetGuid() == FIELD_SERVER)
        g_GameRankingMgr.SendSystemMemoOfWorldRanking();
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
#endif //_NA_006801_20130705_RENEWAL_RANKINGSYSTEM
}

Handler_RankingServer_IMPL(GP_RANKING_CHAGED_STATUS_NTF)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
    WAVERIX_WRAPPER((WAVERIX_LC_CALL1("waverix_log_trace_ranking",
                    "|"__FUNCTION__"|{}=")));
	MSG_GP_RANKING_CHAGED_STATUS_NTF * pMsg = (MSG_GP_RANKING_CHAGED_STATUS_NTF*)pRecvMsg;
    // CHANGES: f110511.1L, shared logic, _NA002635_GP_DP_PROTOCOL_UINIFICATION
    g_GameRankingMgr.OnReceivedStatusControl(eRANKING_STATUS(pMsg->m_ChangedStatus));
	SUNLOG( eCRITICAL_LOG, "--Status(%u) Info is Received--", pMsg->m_ChangedStatus );	
}

//PortalServer에서 정산 시간을 알아온뒤에 포인트를 조회한다.
Handler_RankingServer_IMPL(GP_MISSION_RANKING_SETTLE_DATE_ACK)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
    WAVERIX_WRAPPER((WAVERIX_LC_CALL1("waverix_log_trace_ranking",
                    "|"__FUNCTION__"|{}=")));
	MSG_GP_MISSION_RANKING_SETTLE_DATE_ACK *pMsg = (MSG_GP_MISSION_RANKING_SETTLE_DATE_ACK *)pRecvMsg;	

	MSG_DG_MISSION_RANKING_POINT_REQ_SYN SynMsg;
	SynMsg.RecordInfo			=pMsg->RecordInfo;
	SynMsg.MemberInfo			=pMsg->MemberInfo;
	SynMsg.SettledStartDate		=pMsg->SettledStartDate;
	SynMsg.Period				=pMsg->Period;

	g_pGameServer->SendToServer( GAME_DBPROXY, &SynMsg, sizeof(SynMsg) );
}
#endif // _NA_008012_20150130_RANKING_SYSTEM