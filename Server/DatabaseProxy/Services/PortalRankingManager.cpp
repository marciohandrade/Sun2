#include "stdafx.h"
#ifdef _NA_008012_20150130_RANKING_SYSTEM
#include "./PortalRankingManager.h"

//==================================================================================================
#if defined(_NA002635_GP_DP_PROTOCOL_UINIFICATION_IN_DBP)
//==================================================================================================

#include <ArchiveOfStream.h>

#include <PacketStruct_DP.h>
#include <PacketStruct_GP.h>

#include "DBProxyServerEx.h"
#include "PacketHandler/Handler_FromGameServer.Ranking.h"
#include "QueryObjects/DBHandler.h"

//==================================================================================================

PortalRankingManager g_RankingMgr;

//==================================================================================================

PortalRankingManager::PortalRankingManager()
    : ranking_mode_(eRankingMode_None)
{
}

PortalRankingManager::~PortalRankingManager()
{
}

// RankingManager interfaces
void PortalRankingManager::Initialize()
{
    WAVERIX_WRAPPER((WAVERIX_LC_CALL2("waverix_log_trace_ranking",
                    "|"__FUNCTION__"|{ranking_mode}=", ranking_mode_)));
    if (bool not_instanced = !(ranking_mode_ & this->eRankingMode_Instanced))
    {
        RankingManager::Initialize();
        ranking_mode_ = eRankingMode(ranking_mode_ | this->eRankingMode_Instanced);
    }
}

void PortalRankingManager::OnScriptsLoaded()
{
    ranking_mode_ = eRankingMode(ranking_mode_ | this->eRankingMode_ScriptLoaded);
    WAVERIX_WRAPPER((WAVERIX_LC_CALL2("waverix_log_trace_ranking",
                    "|"__FUNCTION__"|{ranking_mode}=", ranking_mode_)));
}

bool PortalRankingManager::RequestRankingInformationToDB()
{
    // NOTE: f110511.1L, remains = { not call 'InitComputation' state }
    ;{  // change valid states
        const int kDesiredBaseInfoPrepared = (eRankingMode_Instanced | eRankingMode_ScriptLoaded);
        bool desired_base_info_prepared =
            (ranking_mode_ & kDesiredBaseInfoPrepared) == kDesiredBaseInfoPrepared;
        if (FlowControl::FCAssert(desired_base_info_prepared) == false) {
            return false;
        };
        bool desired_no_transaction = ((ranking_mode_ & eRankingMode_RankingLoading) == 0);
        if (FlowControl::FCAssert(desired_no_transaction) == false) {
            return false;
        };
    };
    int period = GetPeriod();
    SAFE_NUMERIC_TYPECAST(int, period, BYTE);
    WAVERIX_WRAPPER((WAVERIX_LC_CALL3("waverix_log_trace_ranking",
                    "|"__FUNCTION__"|{old_settled_data, period}=",
                    GetOldSettledDate(), period)));

    MSG_DP_RANKING_INFO_REQ_SYN req_msg;
    req_msg.m_SettledStartDate = GetOldSettledDate();
    req_msg.m_Period = static_cast<BYTE>(period);

    Handler_P2D::OnDP_RANKING_INFO_REQ_SYN(g_DBHandler.GetSharedSession(), &req_msg, sizeof(req_msg));
    //
    ranking_mode_ = eRankingMode(ranking_mode_ & ~this->eRankingMode_RankingLoaded);
    ranking_mode_ = eRankingMode(ranking_mode_ | this->eRankingMode_RankingLoading);
    //
    return true;
}

void PortalRankingManager::ChangeToRankingLoaded()
{
    bool desired_transaction = ((ranking_mode_ & eRankingMode_RankingLoading) != 0);
    ASSERT(desired_transaction);
    //
    ranking_mode_ = eRankingMode(ranking_mode_ & ~this->eRankingMode_RankingLoading);
    ranking_mode_ = eRankingMode(ranking_mode_ | this->eRankingMode_RankingLoaded);
    //
    ASSERT(enabled_service());
    //
    WAVERIX_WRAPPER((WAVERIX_LC_CALL2("waverix_log_trace_ranking",
                    "|"__FUNCTION__"|{ranking_mode}=", ranking_mode_)));
    ;{
        MSG_GP_RANKING_SESSION_CONTROL_CMD msg_cmd;
        msg_cmd.enabled_service = g_RankingMgr.enabled_service();
        g_DBProxyServer.SendToSessionType(GAME_SERVER, &msg_cmd, sizeof(msg_cmd));
    };
}

// CHANGES: f110511.1L, added inherited method to support session control
void PortalRankingManager::Update()
{
    if (enabled_service() == false)
    {
        const int kPreparedBaseInfo = (eRankingMode_Instanced | eRankingMode_ScriptLoaded);
        if (bool not_prepared_base_info = ((ranking_mode_ & kPreparedBaseInfo) != kPreparedBaseInfo)) {
            return;
        };
        if (bool has_transaction = ((ranking_mode_ & eRankingMode_RankingLoading) != 0)) {
            return;
        };
        if (bool need_request_data = ((ranking_mode_ & eRankingMode_RankingLoaded) == 0)) {
            RequestRankingInformationToDB();
            return;
        };
        return;
    };

    base_type::Update();
}

// RankingManager interfaces
void PortalRankingManager::OnChangedStatus()
{
    WAVERIX_WRAPPER((WAVERIX_LC_CALL2("waverix_log_trace_ranking",
                    "|"__FUNCTION__"|{ranking_mode}=", ranking_mode_)));
    MSG_GP_RANKING_CHAGED_STATUS_NTF msg;
    msg.m_ChangedStatus = static_cast<uint8_t>(GetStatus());
    g_DBProxyServer.SendToSessionType(GAME_SERVER, &msg, sizeof(msg));

    SUNLOG(eFULL_LOG, "RankingManager Status Change: %d", (int)GetStatus());
}

// RankingManager interfaces
void PortalRankingManager::OnSettled()
{
    WAVERIX_WRAPPER((WAVERIX_LC_CALL2("waverix_log_trace_ranking",
                    "|"__FUNCTION__"|{ranking_mode}=", ranking_mode_)));
    MSG_GP_RANKING_SETTLED_INFO_CMD m_msg;
    BOOST_STATIC_ASSERT(RANKINGNO(-1) > 0 && RANKINGNO(-1) <= MAX_MAPCODE_NUM &&
                        eRANKING_MAX < UCHAR_MAX);
    static const int MAX_PAGE = 5;
    for (int type = 0 ; type < eRANKING_MAX ; ++type)
    {
        for (int MissionNo = 0 ; MissionNo < MAX_MAPCODE_NUM ; ++MissionNo)
        {
#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
            for (int ranking_type = 1; ranking_type < eRANKING_TYPE_MAX; ++ranking_type)
            {
                MissionRanking* pRanking = \
                    FindMissionRanking(eRANKING_TYPE(type), RANKINGNO(MissionNo), RANKINGTYPE(ranking_type));
                if (!pRanking) continue;

                for (int page = 0 ; page < MAX_PAGE ; ++page)
                {
                    m_msg.m_Type        = BYTE(type);
                    m_msg.m_MissionNo   = RANKINGNO(MissionNo);
                    m_msg.ranking_type  = RANKINGTYPE(ranking_type);
                    m_msg.m_Page        = BYTE(page);
                    m_msg.m_Size        = m_msg.MAX_BUFFER_SIZE;
                    ArchiveOfStream lArchive(m_msg.m_pBUFFER, m_msg.m_Size);
                    pRanking->Serialize(m_msg.m_Page, lArchive, RANKING_SERIALIZE_LOAD);
                    m_msg.m_Size = WORD(lArchive.GetSize());
                    g_DBProxyServer.SendToSessionType(GAME_SERVER, &m_msg, m_msg.GetSize());
                }
            }
#else
            MissionRanking* pRanking = \
                FindMissionRanking(eRANKING_TYPE(type), RANKINGNO(MissionNo));
            if (!pRanking) continue;

            for (int page = 0 ; page < MAX_PAGE ; ++page)
            {
                m_msg.m_Type        = BYTE(type);
                m_msg.m_MissionNo   = RANKINGNO(MissionNo);
                m_msg.m_Page        = BYTE(page);
                m_msg.m_Size        = m_msg.MAX_BUFFER_SIZE;
                ArchiveOfStream lArchive(m_msg.m_pBUFFER, m_msg.m_Size);
                pRanking->Serialize(m_msg.m_Page, lArchive, RANKING_SERIALIZE_LOAD);
                m_msg.m_Size = WORD(lArchive.GetSize());
                g_DBProxyServer.SendToSessionType(GAME_SERVER, &m_msg, m_msg.GetSize());
            }
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
        }
    }

    MSG_GP_RANKING_SETTLED_INFO_END_NTF ntf;
    g_DBProxyServer.SendToSessionType(GAME_SERVER, &ntf, sizeof(ntf));
}

#endif //_NA002635_GP_DP_PROTOCOL_UINIFICATION_IN_DBP
#endif // _NA_008012_20150130_RANKING_SYSTEM