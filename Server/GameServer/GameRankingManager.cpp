#include "stdafx.h"
#ifdef _NA_008012_20150130_RANKING_SYSTEM
#include "./GameRankingManager.h"

#include <PacketStruct_GP.h>

#include "GameServerEx.h"
#include "RewardManager.h"
#include "PlayerManager.h"
#include "Player.h"
#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
#include "SystemMemo.h"
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
//==================================================================================================

GameRankingManager g_GameRankingMgr;

//==================================================================================================

GameRankingManager::GameRankingManager()
    : ranking_mode_(eRankingMode_None)
{
}

GameRankingManager::~GameRankingManager()
{
}

void GameRankingManager::Initialize()
{
    WAVERIX_WRAPPER((WAVERIX_LC_CALL2("waverix_log_trace_ranking",
                    "|"__FUNCTION__"|{ranking_mode}=", ranking_mode_)));
    if (bool not_instanced = !(ranking_mode_ & this->eRankingMode_Instanced))
    {
        RankingManager::Initialize();
        ranking_mode_ = eRankingMode(ranking_mode_ | this->eRankingMode_Instanced);
    };
#if !defined(_NA002635_GP_DP_PROTOCOL_UINIFICATION)
    RequestRankingInformation();
#endif

#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
    system_memo_ = new SystemMemo();
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
}

// CHANGES: f110511.1L, shared logic, _NA002635_GP_DP_PROTOCOL_UINIFICATION
void GameRankingManager::OnScriptsLoaded()
{
    WAVERIX_WRAPPER((WAVERIX_LC_CALL2("waverix_log_trace_ranking",
                    "|"__FUNCTION__"|{ranking_mode}=", ranking_mode_)));
    ranking_mode_ = eRankingMode(ranking_mode_ | this->eRankingMode_ScriptLoaded);
}

bool GameRankingManager::RequestRankingInformation()
{
    bool desired_no_transaction = ((ranking_mode_ & eRankingMode_RankingLoading) == 0);
    if (FlowControl::FCAssert(desired_no_transaction) == false) {
        return false;
    };
    WAVERIX_WRAPPER((WAVERIX_LC_CALL2("waverix_log_trace_ranking",
                    "|"__FUNCTION__"|{ranking_mode}=", ranking_mode_)));
    MSG_GP_RANKING_INFO_REQ_SYN msg;
    g_pGameServer->SendToServer(RANKING_SERVER, &msg, sizeof(msg));

    ranking_mode_ = eRankingMode(ranking_mode_ & ~this->eRankingMode_RankingLoaded);
    ranking_mode_ = eRankingMode(ranking_mode_ | this->eRankingMode_RankingLoading);
    return true;
}

// CHANGES: f110511.1L, shared logic, _NA002635_GP_DP_PROTOCOL_UINIFICATION
void GameRankingManager::ChangeToRankingLoaded()
{
    bool desired_transaction = ((ranking_mode_ & eRankingMode_RankingLoading) != 0);
    ASSERT(desired_transaction);
    //
    ranking_mode_ = eRankingMode(ranking_mode_ & ~this->eRankingMode_RankingLoading);
    ranking_mode_ = eRankingMode(ranking_mode_ | this->eRankingMode_RankingLoaded);
    WAVERIX_WRAPPER((WAVERIX_LC_CALL2("waverix_log_trace_ranking",
                    "|"__FUNCTION__"|{ranking_mode}=", ranking_mode_)));
    //
    ASSERT(enabled_service());
    g_GameRankingMgr.SetStatus(eRANKING_STATUS_NORMAL);
}

void GameRankingManager::OnReceivedStatusControl(eRANKING_STATUS ranking_status)
{
    WAVERIX_WRAPPER((WAVERIX_LC_CALL3("waverix_log_trace_ranking",
                    "|"__FUNCTION__"|{ranking_mode, ranking_status}=",
                    ranking_mode_, ranking_status)));

    SetStatus(ranking_status);
    if ((ranking_status == eRANKING_STATUS_INIT) ||
        (ranking_status == eRANKING_STATUS_SUSPENDING))
    {
        ranking_mode_ = eRankingMode(ranking_mode_ & ~this->eRankingMode_RankingLoaded);
        this->ClearRanking();
    }
    else if (ranking_status == eRANKING_STATUS_NORMAL)
    {
        ranking_mode_ = eRankingMode(ranking_mode_ | this->eRankingMode_RankingLoaded);
    }
    else
    {
        ASSERT(!"unexpected ranking status");
    };
}

#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
void GameRankingManager::ExecuteReward(BOOL bSuccess,
                                       PLAYERKEY PlayerKey, RANKINGNO no, RANKINGKEY key,
                                       const TCHAR * charname, RANKINGTYPE ranking_type , BYTE ErrorCode /*= 0*/)
#else
void GameRankingManager::ExecuteReward(BOOL bSuccess,
                                       PLAYERKEY PlayerKey, RANKINGNO no, RANKINGKEY key,
                                       const TCHAR * charname, BYTE ErrorCode)
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
{
    WAVERIX_WRAPPER((WAVERIX_LC_CALL2("waverix_log_trace_ranking",
                    "|"__FUNCTION__"|{ranking_mode}=", ranking_mode_)));

#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
    RankingManager::OnReward(bSuccess, no, key, charname, ranking_type);
#else
    RankingManager::OnReward(bSuccess, no, key, charname);
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR

    if (bSuccess)
    {
        Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey(PlayerKey);
        if (!pPlayer) return ;

#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
        MissionRanking * pRanking = FindMissionRanking(eRANKING_LASTWEEK, no, ranking_type);
#else
        MissionRanking * pRanking = FindMissionRanking(eRANKING_LASTWEEK, no);
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
        ASSERT(pRanking);
        DWORD Ranking = pRanking->GetRanking(key);

        // 랭킹 보상 로그 남겨야 함!
#ifdef _NA_006801_20130705_RENEWAL_RANKINGSYSTEM
        REWARD * pReward = FindReward(no, pPlayer->GetCharType(), (WORD)Ranking, pRanking->GetRankingType());
#else
        REWARD * pReward = FindReward(no, pPlayer->GetCharType(), (WORD)Ranking);
#endif //_NA_006801_20130705_RENEWAL_RANKINGSYSTEM

        if (pReward)
        {
            MSG_CG_MISSION_RANKING_REWARD_ACK msg;

            RC::eREWARD_RESULT rt = g_RewardManager.RewardItem(RewardManager::eREWARD_MISSIONRANKING, pReward->m_RewardCode, 0, pPlayer, msg.m_ItemInfo);
            ASSERT(rt == RC::RC_REWARD_SUCCESS);
            if (rt == RC::RC_REWARD_SUCCESS)
            {
#ifdef _NA_006801_20130705_RENEWAL_RANKINGSYSTEM
                RC::eREWARD_RESULT reward_result_hime = 
                    g_RewardManager.RewardHime(pReward->m_RewardCode, pPlayer,msg.rewarded_money, 100);
                msg.hime_bonus_ratio = 0;
                ASSERT(reward_result_hime == RC::RC_REWARD_SUCCESS);
#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
                msg.ranking_id = FindLVArea(no, ranking_type)->m_Id;
#else
                msg.ranking_id = FindLVArea(no)->m_Id;
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
                msg.reward_ranking = Ranking;
#endif //_NA_006801_20130705_RENEWAL_RANKINGSYSTEM
                pPlayer->SendPacket(&msg, msg.GetSize());

                return;
                // RETURN ------------------------------------------------------------------------------------------------------
            }           
            else
            {
                SUNLOG(eCRITICAL_LOG, _T("Cannot Ranking Reward1 %u,%u,%s"), no, key, charname);
                ErrorCode = RC::RC_RANKING_FAILED;
                // 플레이어를 찾아서 보내줌
                MSG_CG_MISSION_RANKING_REWARD_NAK msg;
                msg.m_dwErrorCode = ErrorCode;
                pPlayer->SendPacket(&msg, sizeof(msg));
            }
        }
        else
        {
            SUNLOG(eCRITICAL_LOG, _T("Cannot Ranking Reward2 %u,%u,%s"), no, key, charname);
            ErrorCode = RC::RC_RANKING_INVALID_REWARD_INDEX;
            MSG_CG_MISSION_RANKING_REWARD_NAK msg;
            msg.m_dwErrorCode = ErrorCode;
            pPlayer->SendPacket(&msg, sizeof(msg));
        }

    }
    else
    {
        SUNLOG(eCRITICAL_LOG, 
            _T("GP_RANKING_MISSION_REWARD_NAK Error[%u] can't execute Ranking Reward Action from Portal"), 
            ErrorCode);

        Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey(PlayerKey);
        if (!pPlayer) return ;

        // 플레이어를 찾아서 보내줌
        MSG_CG_MISSION_RANKING_REWARD_NAK msg;
        msg.m_dwErrorCode = ErrorCode;
        pPlayer->SendPacket(&msg, sizeof(msg));

    }


}

#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
void GameRankingManager::Release()
{
    SAFE_DELETE(system_memo_);
    RankingManager::Release();
}

void GameRankingManager::SendSystemMemoOfWorldRanking()
{
    SystemMemoInfoOfWorldRanking request_memo_info[MAX_SYSTEM_MEMO_LIST];
    DWORD count = 0;

    const MISSION_RANKING* const_ranking_tables = GetRankingTables();
    MISSION_RANKING::const_iterator table_itr = ranking_tables_[eRANKING_LASTWEEK].begin();
    BATTLEGROUND_RANKING::const_iterator bg_table_itr = battleground_ranking_tables_[eRANKING_LASTWEEK].begin();

    for (table_itr; table_itr != ranking_tables_[eRANKING_LASTWEEK].end(); ++table_itr)
    {
        MissionRanking* mission_ranking = table_itr->second;
        RankingManager::LEVEL_AREA* level_area = FindLVArea(mission_ranking->GetMissionNo(), mission_ranking->GetRankingType());
        //mission_ranking의 level_area를 찾지 못하면 넘어가고 다음 mission_ranking을 탐색
        if (level_area == NULL)
            continue;

        MissionRanking::RANKING_INFO_LIST& ranking_record_list =  mission_ranking->GetMemberHash();
        
        MissionRanking::RANKING_INFO_LIST::const_iterator record_itr = ranking_record_list.begin();
        for (record_itr; record_itr != ranking_record_list.end(); ++record_itr)
        {
            MissionRankingRecord* ranking_record = *record_itr;
            
            //점수가 0점 이하이면 보상을 받을 수 없다. 쪽지도 보내지말자
            if ( ranking_record->GetClearTime() <= 0) {
                continue;
            }
            const MissionRankingRecord::MEMBER_HASH& member_hash = ranking_record->GetMemberHash();
            
            MissionRankingRecord::MEMBER_HASH::const_iterator member_itr = member_hash.begin();
            for (member_itr; member_itr != member_hash.end(); ++member_itr)
            {
                RankingMember* ranking_member = member_itr->second;
                SystemMemoInfoOfWorldRanking& temp_memo_info = request_memo_info[count];
                temp_memo_info.ranking_id = level_area->m_Id;
                temp_memo_info.reward_ranking = mission_ranking->GetRanking(ranking_record->GetKey());

                strncpy(temp_memo_info.recv_user_name_, ranking_member->GetCharName(), 
                    _countof(temp_memo_info.recv_user_name_));
                temp_memo_info.recv_user_name_[_countof(temp_memo_info.recv_user_name_) - 1] = '\0';
                temp_memo_info.system_memo_type_ = SYSMEMO_WORLD_RANKING_GRADE;

                ++count;
            }

        }
        
    }

    for (bg_table_itr; bg_table_itr != battleground_ranking_tables_[eRANKING_LASTWEEK].end(); ++bg_table_itr)
    {
        MissionRanking* mission_ranking = bg_table_itr->second;
        MissionRanking::RANKING_INFO_LIST& ranking_record_list =  mission_ranking->GetMemberHash();

        MissionRanking::RANKING_INFO_LIST::const_iterator record_itr = ranking_record_list.begin();
        for (record_itr; record_itr != ranking_record_list.end(); ++record_itr)
        {
            MissionRankingRecord* ranking_record = *record_itr;

            //점수가 0점 이하이면 보상을 받을 수 없다. 쪽지도 보내지말자
            if ( ranking_record->GetClearTime() <= 0) {
                continue;
            }
            const MissionRankingRecord::MEMBER_HASH& member_hash = ranking_record->GetMemberHash();

            MissionRankingRecord::MEMBER_HASH::const_iterator member_itr = member_hash.begin();
            for (member_itr; member_itr != member_hash.end(); ++member_itr)
            {
                RankingMember* ranking_member = member_itr->second;
                SystemMemoInfoOfWorldRanking& temp_memo_info = request_memo_info[count];
                temp_memo_info.ranking_id = FindLVArea(mission_ranking->GetMissionNo(), mission_ranking->GetRankingType())->m_Id;
                temp_memo_info.reward_ranking = mission_ranking->GetRanking(ranking_record->GetKey());

                strncpy(temp_memo_info.recv_user_name_, ranking_member->GetCharName(), 
                    _countof(temp_memo_info.recv_user_name_));
                temp_memo_info.recv_user_name_[_countof(temp_memo_info.recv_user_name_) - 1] = '\0';
                temp_memo_info.system_memo_type_ = SYSMEMO_WORLD_RANKING_GRADE;

                ++count;
            }

        }

    }

    if (count > 0)
    {
        system_memo_->SendSystemMemo(request_memo_info, count, SYSMEMO_WORLD_RANKING_GRADE);
    }
}
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
#endif // _NA_008012_20150130_RANKING_SYSTEM