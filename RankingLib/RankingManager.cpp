#include "stdafx.h"
#ifdef _NA_008012_20150130_RANKING_SYSTEM
#include "./RankingManager.h"

#include <time.h>
#include <functional_custom.h>

#include <ArchiveOfStream.h>

//==================================================================================================
// check rules

// NOTE: normally convert to uint8_t, check RankingManager::GetStatus
BOOST_STATIC_ASSERT(eRANKING_STATUS_COUNTS < UCHAR_MAX);

struct RankingManager::SettleInfo
{   
    // 정산 시작 날짜
    tm m_OldSettleDate;
    /*int m_nYear;
    int m_nMonth;
    int m_nDay;
    int m_nHour;
    int m_nMinute;
    int m_nSecond;*/

    int m_SuspendTime;
    // 재 계산 주기(날 수)
    int m_PeriodicDay;
    // 정산 날짜
    tm m_SettleDate;
    time_t m_SettleTime;
    // 일시 중지 시작시간
    tm m_SuspendStartDate;
    time_t m_SuspendStartTime;
    // 일시 중지 끝시간
    tm m_SuspendEndDate;
    time_t m_SuspendEndTime;

    BOOL m_bSettle;
};

//==================================================================================================

RankingManager::RankingManager()
    : settle_data_impl_(NULL)
    , mission_ranking_reward_array_(NULL)
{
    const DWORD kNumberOfPoolNodes = 100;
    m_MissionRankingPool.Initialize(kNumberOfPoolNodes);
    m_MissionRankingRecordPool.Initialize(kNumberOfPoolNodes);
    m_RankingMemberPool.Initialize(kNumberOfPoolNodes);

    MissionRanking::SetPool(&m_MissionRankingPool);
    MissionRankingRecord::SetPool(&m_MissionRankingRecordPool);
    RankingMember::SetPool(&m_RankingMemberPool);
    // CHANGES: f110518.7L, fixed not updated object problem
    // because the uninitialized timer is always disabled (like can't expired)
    update_timer_.SetTimer(kUpdateInterval);
}

RankingManager::~RankingManager()
{
    UnLoad();
}

void RankingManager::Initialize()
{
    CONSOLELOG(eCRITICAL_LOG, _T("RankingManager Initialize()"));
    SetStatus(eRANKING_STATUS_INIT);
}

void RankingManager::Release()
{
    SAFE_DELETE(settle_data_impl_);

    ClearRanking();
}

//==================================================================================================

MissionRankingRecord* RankingManager::FindTmpMissionRankingRecord(RANKINGKEY key) const
{
    TMP_MISSION_RECORD_HASH::const_iterator it = temp_ranking_table_.find(key);
    if (it == temp_ranking_table_.end()) {
        return NULL;
    };

    MissionRankingRecord* record = it->second;
    return record;
}
#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
MissionRankingRecord* RankingManager::FindMissionRecord(
    eRANKING_TYPE type, RANKINGNO mission_no, RANKINGKEY key, RANKINGTYPE ranking_type /*= 0*/) const
{
    if (type >= eRANKING_MAX && type != eRANKING_MYRANKING) {
        return NULL;
    };

    const MissionRanking* ranking;
    if (ranking_type >= eRANKING_TYPE_BATTLEGROUND_WIN)
    {
        //전장랭킹은 랭킹타입을 이용한 추가검색이 필요하다.
        RANKINGID ranking_id = FindRankingID(mission_no, ranking_type);
        if (ranking_id == 0)
            return NULL;

        BATTLEGROUND_RANKING::const_iterator it = battleground_ranking_tables_[type].find(ranking_id);
        if (it == battleground_ranking_tables_[type].end()) {
            return NULL;
        };
        ranking = it->second;
    }
    else
    {
        // 전장랭킹이 아님
        MISSION_RANKING::const_iterator it = ranking_tables_[type].find(mission_no);
        if (it == ranking_tables_[type].end()) {
            return NULL;
        };
        ranking = it->second;;
    }
    return ranking->FindRanking(key);
}

MissionRanking* RankingManager::FindMissionRanking(
    eRANKING_TYPE type, RANKINGNO mission_no, RANKINGTYPE ranking_type /*= 0*/) const
{
    if (type >= eRANKING_MAX && type != eRANKING_MYRANKING) {
        return NULL;
    };

    MissionRanking* ranking;
    if (ranking_type >= eRANKING_TYPE_BATTLEGROUND_WIN)
    {
        //전장랭킹은 랭킹타입을 이용한 추가검색이 필요하다.
        RANKINGID ranking_id = FindRankingID(mission_no, ranking_type);
        if (ranking_id == 0)
            return NULL;
        // '내 순위 찾기'일 경우 처리
        if (type == eRANKING_MYRANKING)
        {
            BATTLEGROUND_RANKING::const_iterator it = my_battleground_table_.find(ranking_id);
            if (it == my_battleground_table_.end()) {
                return NULL;
            };

            ranking = it->second;
        }
        else
        {
            BATTLEGROUND_RANKING::const_iterator it = battleground_ranking_tables_[type].find(ranking_id);
            if (it == battleground_ranking_tables_[type].end()) {
                return NULL;
            };

            ranking = it->second;
        }
    }
    else
    {
        // 전장랭킹이 아님
        // '내 순위 찾기'일 경우 처리
        if (type == eRANKING_MYRANKING)
        {
            MISSION_RANKING::const_iterator it = my_ranking_table_.find(mission_no);
            if (it == my_ranking_table_.end()) {
                return NULL;
            };

            ranking = it->second;
        }
        else
        {
            MISSION_RANKING::const_iterator it = ranking_tables_[type].find(mission_no);
            if (it == ranking_tables_[type].end()) {
                return NULL;
            };

            ranking = it->second;
        }

    }
    return ranking;
}
#else
MissionRankingRecord* RankingManager::FindMissionRecord(
    eRANKING_TYPE type, RANKINGNO mission_no, RANKINGKEY key) const
{
    if (type >= eRANKING_MAX && type != eRANKING_MYRANKING) {
        return NULL;
    };

    MISSION_RANKING::const_iterator it = ranking_tables_[type].find(mission_no);
    if (it == ranking_tables_[type].end()) {
        return NULL;
    };

    const MissionRanking* ranking = it->second;
    return ranking->FindRanking(key);
}

MissionRanking* RankingManager::FindMissionRanking(
    eRANKING_TYPE type, RANKINGNO mission_no) const
{
#if defined (_NA_006802_20130704_REORGANIZATION_RANK_SYSTEM)
    if (type == eRANKING_MYRANKING)
    {
        MISSION_RANKING::const_iterator it = my_ranking_table_.find(mission_no);
        if (it == my_ranking_table_.end()) {
            return NULL;
        };

        MissionRanking* ranking = it->second;
        return ranking;
    }
#endif // _NA_006802_20130704_REORGANIZATION_RANK_SYSTEM
    if (type >= eRANKING_MAX) {
        return NULL;
    };
    MISSION_RANKING::const_iterator it = ranking_tables_[type].find(mission_no);
    if (it == ranking_tables_[type].end()) {
        return NULL;
    };

    MissionRanking* ranking = it->second;
    return ranking;
}
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR

#ifdef _NA_006801_20130705_RENEWAL_RANKINGSYSTEM
RankingManager::LEVEL_AREA* RankingManager::FindLVArea(RANKINGNO no, RANKINGTYPE ranking_type /*= 0*/) const
{
    if (ranking_type > 0)
    {
        //랭킹타입을 적용시켜 검색
        RANKINGID ranking_id = FindRankingID(no, ranking_type);
        MISSION_LEVEL_AREA_HASH::const_iterator it = mission_level_area_table_.find(ranking_id);
        if (it == mission_level_area_table_.end()) {
            return NULL;
        };
        LEVEL_AREA* level_area = it->second;
        return level_area;
    }
    else
    {
        //랭킹타입을 미적용시켜 검색
        MISSION_LEVEL_AREA_HASH::const_iterator it = mission_level_area_table_.begin();
        for( it; it != mission_level_area_table_.end(); ++it)
        {
            LEVEL_AREA* level_area = it->second;
            if (level_area == NULL) {
                continue;
            }

            if (level_area->m_MissionNo == no)
                return level_area;
        }
    }
    
    return NULL;
}

RankingManager::REWARD* RankingManager::FindReward(
    RANKINGNO mission_no, BYTE class_type, WORD ranking, BYTE reward_type) const
{
    DWORD key = Make_Mission_RewardKey(mission_no, class_type, ranking, reward_type);
    MISSION_REWARD_HASH::const_iterator it = mission_reward_table_.find(key);
    if (it == mission_reward_table_.end()) {
        return NULL;
    };
    REWARD* reward = it->second;
    return reward;
}

const RANKINGID RankingManager::FindRankingID( RANKINGNO mission_no, RANKINGTYPE ranking_type ) const
{
    MISSION_LEVEL_AREA_HASH::const_iterator it = mission_level_area_table_.begin();

    for( it; it != mission_level_area_table_.end(); ++it)
    {
        LEVEL_AREA* level_area = it->second;
        if (level_area == NULL) {
            continue;
        }

        if (level_area->m_MissionNo == mission_no && level_area->m_RankingType == ranking_type)
            return level_area->m_Id;
    }


    return 0;
}

RC::eRANKING_RESULT RankingManager::CanReward(
    RANKINGNO mission_no, RANKINGKEY key, const TCHAR* char_name,
    RankingMember** const result_ranking_member, RANKINGTYPE ranking_type /*= 0*/) const
{
    if (this->ranking_status_ != eRANKING_STATUS_NORMAL) {
        return RC::RC_RANKING_SETTLING_INFO;
    };

    if (ranking_type >= eRANKING_TYPE_BATTLEGROUND_WIN)
    {
#ifdef _NA_006802_20130819_REORGANIZATION_RANK_SYSTEM_MINOR
        RANKINGID ranking_id = FindRankingID(mission_no, ranking_type);
        //전장랭킹
        BATTLEGROUND_RANKING::const_iterator it = battleground_ranking_tables_[eRANKING_LASTWEEK].find(ranking_id);
        if (it == battleground_ranking_tables_[eRANKING_LASTWEEK].end()) {
            return RC::RC_RANKING_INVALID_STATE;
        };
        MissionRanking* const mission_ranking = it->second;
        MissionRankingRecord* ranking_record = mission_ranking->FindRanking(key);
        if (ranking_record == NULL) {
            return RC::RC_RANKING_INVALID_STATE;
        };
        //점수가 0점이하인 유저에겐 보상을 주지 말자
        if (ranking_record->GetClearTime() <= 0) {
            return RC::RC_RANKING_INVALID_STATE;
        }
        RankingMember* ranking_member = ranking_record->FindMember(char_name);
        if (ranking_member == NULL) {
            return RC::RC_RANKING_INVALID_STATE;
        };
        if (ranking_member->GetReward()) {
            return RC::RC_RANKING_ALREADY_REWARDED;
        };
        if (result_ranking_member) {
            *result_ranking_member = ranking_member;
        };
        return RC::RC_RANKING_SUCCESS;
#endif //_NA_006802_20130819_REORGANIZATION_RANK_SYSTEM_MINOR
    }
    else
    {
        //미션,포인트랭킹
        MISSION_RANKING::const_iterator it = ranking_tables_[eRANKING_LASTWEEK].find(mission_no);
        if (it == ranking_tables_[eRANKING_LASTWEEK].end()) {
            return RC::RC_RANKING_INVALID_STATE;
        };
        MissionRanking* const mission_ranking = it->second;
        MissionRankingRecord* ranking_record = mission_ranking->FindRanking(key);
        if (ranking_record == NULL) {
            return RC::RC_RANKING_INVALID_STATE;
        };
        //점수가 0점이하인 유저에겐 보상을 주지 말자
        if (ranking_record->GetClearTime() <= 0) {
            return RC::RC_RANKING_INVALID_STATE;
        }
        RankingMember* ranking_member = ranking_record->FindMember(char_name);
        if (ranking_member == NULL) {
            return RC::RC_RANKING_INVALID_STATE;
        };
        if (ranking_member->GetReward()) {
            return RC::RC_RANKING_ALREADY_REWARDED;
        };
        if (result_ranking_member) {
            *result_ranking_member = ranking_member;
        };
        return RC::RC_RANKING_SUCCESS;
    }
    return RC::RC_RANKING_INVALID_STATE;
}

RC::eRANKING_RESULT
RankingManager::DoReward(RANKINGNO mission_no, RANKINGKEY key, const TCHAR* char_name, 
                         RANKINGTYPE ranking_type /*= 0*/)
{
    RankingMember* ranking_member = NULL;
    RC::eRANKING_RESULT check_rewardable_state =
        CanReward(mission_no, key, char_name, &ranking_member, ranking_type);
    if (check_rewardable_state != RC::RC_RANKING_SUCCESS) {
        return check_rewardable_state;
    };
    if (ranking_member->DoReward() == false) {
        return RC::RC_RANKING_DOING_TRANSACTION;
    };
    // 디비로 실시간 업데이트 쿼리
    return RC::RC_RANKING_SUCCESS;
}

void RankingManager::OnReward(BOOL success, RANKINGNO mission_no, RANKINGKEY key, const TCHAR* charname, 
                              RANKINGTYPE ranking_type /*= 0*/)
{
    RankingMember* ranking_member = NULL;
    RC::eRANKING_RESULT check_rewardable_state =
        CanReward(mission_no, key, charname, &ranking_member, ranking_type);
    if (FlowControl::FCAssert(check_rewardable_state == RC::RC_RANKING_SUCCESS) == false) {
        SUNLOG(eCRITICAL_LOG, "OnReward Failed! : no=%u, key=%d, charname=%s, rt=%d",
            mission_no, key, charname, check_rewardable_state);
    };

    if (ranking_member)
    {
        // pRankingMember == NULL 타이밍시 발생가능 존재???
        if (success) {
            ranking_member->SetReward(true);
        }
        ranking_member->EndTransaction();
    }
    // REWARD_ACK를 보내준다!
}
#else
RankingManager::LEVEL_AREA* RankingManager::FindLVArea(RANKINGNO no) const
{
    MISSION_LEVEL_AREA_HASH::const_iterator it = mission_level_area_table_.find(no);
    if (it == mission_level_area_table_.end()) {
        return NULL;
    };
    LEVEL_AREA* level_area = it->second;
    return level_area;
}

RankingManager::REWARD* RankingManager::FindReward(
    RANKINGNO mission_no, BYTE class_type, WORD ranking) const
{
    DWORD key = Make_Mission_RewardKey(mission_no, class_type, ranking);
    MISSION_REWARD_HASH::const_iterator it = mission_reward_table_.find(key);
    if (it == mission_reward_table_.end()) {
        return NULL;
    };
    REWARD* reward = it->second;
    return reward;
}

RC::eRANKING_RESULT RankingManager::CanReward(
    RANKINGNO mission_no, RANKINGKEY key, const TCHAR* char_name,
    RankingMember** const result_ranking_member) const
{
    if (this->ranking_status_ != eRANKING_STATUS_NORMAL) {
        return RC::RC_RANKING_SETTLING_INFO;
    };
    MISSION_RANKING::const_iterator it = ranking_tables_[eRANKING_LASTWEEK].find(mission_no);
    if (it == ranking_tables_[eRANKING_LASTWEEK].end()) {
        return RC::RC_RANKING_INVALID_STATE;
    };
    MissionRanking* const mission_ranking = it->second;
    MissionRankingRecord* ranking_record = mission_ranking->FindRanking(key);
    if (ranking_record == NULL) {
        return RC::RC_RANKING_INVALID_STATE;
    };
    RankingMember* ranking_member = ranking_record->FindMember(char_name);
    if (ranking_member == NULL) {
        return RC::RC_RANKING_INVALID_STATE;
    };
    if (ranking_member->GetReward()) {
        return RC::RC_RANKING_ALREADY_REWARDED;
    };
    if (result_ranking_member) {
        *result_ranking_member = ranking_member;
    };
    return RC::RC_RANKING_SUCCESS;
}

RC::eRANKING_RESULT
RankingManager::DoReward(RANKINGNO mission_no, RANKINGKEY key, const TCHAR* char_name)
{
    RankingMember* ranking_member = NULL;
    RC::eRANKING_RESULT check_rewardable_state =
        CanReward(mission_no, key, char_name, &ranking_member);
    if (check_rewardable_state != RC::RC_RANKING_SUCCESS) {
        return check_rewardable_state;
    };
    if (ranking_member->DoReward() == false) {
        return RC::RC_RANKING_DOING_TRANSACTION;
    };
    // 디비로 실시간 업데이트 쿼리
    return RC::RC_RANKING_SUCCESS;
}

void RankingManager::OnReward(BOOL success,
                              RANKINGNO mission_no, RANKINGKEY key, const TCHAR* charname)
{
    RankingMember* ranking_member = NULL;
    RC::eRANKING_RESULT check_rewardable_state =
        CanReward(mission_no, key, charname, &ranking_member);
    if (FlowControl::FCAssert(check_rewardable_state == RC::RC_RANKING_SUCCESS) == false) {
        SUNLOG(eCRITICAL_LOG, "OnReward Failed! : no=%u, key=%d, charname=%s, rt=%d",
            mission_no, key, charname, check_rewardable_state);
    };

    if (ranking_member)
    {
        // pRankingMember == NULL 타이밍시 발생가능 존재???
        if (success) {
            ranking_member->SetReward(true);
        }
        ranking_member->EndTransaction();
    }
    // REWARD_ACK를 보내준다!
}
#endif //_NA_006801_20130705_RENEWAL_RANKINGSYSTEM
//==================================================================================================

DWORD RankingManager::GetSettledDate()
{
    assert(settle_data_impl_);
    if (!settle_data_impl_) return 0;

    return (DWORD)((settle_data_impl_->m_SettleDate.tm_year+1900)*10000)
        +((settle_data_impl_->m_SettleDate.tm_mon+1)*100)
        +settle_data_impl_->m_SettleDate.tm_mday;
}

DWORD RankingManager::GetOldSettledDate()   
{ 
    assert(settle_data_impl_);
    return (DWORD)(GetOldSettledDate_Year()*10000)+(GetOldSettledDate_Month()*100)+GetOldSettledDate_Day();
}

int RankingManager::GetPeriod()     
{ 
    assert(settle_data_impl_);
    return settle_data_impl_->m_PeriodicDay; 
}

int RankingManager::GetSuspendTime()
{
    assert(settle_data_impl_);
    return settle_data_impl_->m_SuspendTime; 
}

int RankingManager::GetOldSettledDate_Year()
{
    assert(settle_data_impl_);
    return settle_data_impl_->m_OldSettleDate.tm_year+1900; 
}

int RankingManager::GetOldSettledDate_Month()
{
    assert(settle_data_impl_);
    return settle_data_impl_->m_OldSettleDate.tm_mon+1; 
}

int RankingManager::GetOldSettledDate_Day()
{
    assert(settle_data_impl_);
    return settle_data_impl_->m_OldSettleDate.tm_mday; 
}

//==================================================================================================

void RankingManager::ClearRanking()
{
    if (FlowControl::FCAssert(temp_ranking_table_.empty()) == false)
    {
        FOREACH_CONTAINER(const TMP_MISSION_RECORD_HASH::value_type& node, temp_ranking_table_,
                          TMP_MISSION_RECORD_HASH)
        {
            MissionRankingRecord* record = node.second;
            FREE_RANKING_RECORD(record);
        }
        temp_ranking_table_.clear();
    };

    for (int r_type = 0; r_type < eRANKING_MAX; ++r_type)
    {
        MISSION_RANKING& ranking_table = ranking_tables_[r_type];
        FOREACH_CONTAINER(const MISSION_RANKING::value_type& node, ranking_table,
                          MISSION_RANKING)
        {
            MissionRanking* ranking = node.second;
            FREE_RANKING(ranking);
        }
        ranking_table.clear();
    }
#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
    for (int r_type = 0; r_type < eRANKING_MAX; ++r_type)
    {
        BATTLEGROUND_RANKING& ranking_table = battleground_ranking_tables_[r_type];
        FOREACH_CONTAINER(const BATTLEGROUND_RANKING::value_type& node, ranking_table,
            BATTLEGROUND_RANKING)
        {
            MissionRanking* ranking = node.second;
            FREE_RANKING(ranking);
        }
        ranking_table.clear();
    }

    BATTLEGROUND_RANKING& my_battleground_table = my_battleground_table_;
    FOREACH_CONTAINER(const BATTLEGROUND_RANKING::value_type& node, my_battleground_table,
        BATTLEGROUND_RANKING)
    {
        MissionRanking* ranking = node.second;
        FREE_RANKING(ranking);
    }
    my_battleground_table.clear();
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR

#if defined(_NA_006802_20130704_REORGANIZATION_RANK_SYSTEM)
    MISSION_RANKING& my_ranking_table = my_ranking_table_;
    FOREACH_CONTAINER(const MISSION_RANKING::value_type& node, my_ranking_table,
        MISSION_RANKING)
    {
        MissionRanking* ranking = node.second;
        FREE_RANKING(ranking);
    }
    my_ranking_table.clear();
#endif // _NA_006802_20130704_REORGANIZATION_RANK_SYSTEM
}

#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
MissionRanking* RankingManager::CreateRanking(eRANKING_TYPE type, RANKINGNO mission_no, 
                                              RANKINGTYPE ranking_type /*= 0*/)
{
    MissionRanking* mission_ranking = FindMissionRanking(type, mission_no, ranking_type);
    if (mission_ranking == NULL) 
    {
        //랭킹타입 입력
        if (ranking_type >= eRANKING_TYPE_BATTLEGROUND_WIN)
        {
            //전장랭킹일 경우
            RANKINGID ranking_id = FindRankingID(mission_no, ranking_type);
            if (ranking_id == 0)
                return NULL;

            mission_ranking = MissionRanking::ALLOC();
            mission_ranking->SetMissionNo(mission_no);
            mission_ranking->SetRankingType(static_cast<eRANKING_SYSTEM_TYPE>(ranking_type));

            BATTLEGROUND_RANKING::_Pairib result;
            //테이블에 입력
            if (type == eRANKING_MYRANKING)
            {
                result = my_battleground_table_.insert(
                    BATTLEGROUND_RANKING::value_type(ranking_id, mission_ranking));
            }
            else
            {
                result = battleground_ranking_tables_[type].insert(
                    BATTLEGROUND_RANKING::value_type(ranking_id, mission_ranking));
            }
            ASSERT(result.second);
        }
        else
        {
                        //전장랭킹이 아닐경우
            mission_ranking = MissionRanking::ALLOC();
            mission_ranking->SetMissionNo(mission_no);
            mission_ranking->SetRankingType(GetRankingType(mission_no));

            MISSION_RANKING::_Pairib result;
            //테이블에 입력
            if (type == eRANKING_MYRANKING)
            {
                result = my_ranking_table_.insert(
                    MISSION_RANKING::value_type(mission_no, mission_ranking));
            }
            else
            {
                result = ranking_tables_[type].insert(
                    MISSION_RANKING::value_type(mission_no, mission_ranking));
            }
            ASSERT(result.second);
        }
    }

    return mission_ranking;
}

void RankingManager::Serialize(eRANKING_TYPE type, RANKINGNO mission_no, BYTE page, 
                               BYTE* INOUT stream, WORD& INOUT size, eRANKING_SERIALIZE se, 
                               RANKINGTYPE ranking_type /*= 0*/)
{
    if (se == RANKING_SERIALIZE_SAVE)
    {
        if (size == 0) {
            return;
        }; // NOTE: thinks remains...
        ArchiveOfStream archive(stream, size);
        //
        MissionRanking* const mission_ranking = CreateRanking(type, mission_no, ranking_type);
        mission_ranking->Serialize(page, archive, se);
        mission_ranking->Sort();
    }
    else if (se == RANKING_SERIALIZE_LOAD)
    {
        ArchiveOfStream archive(stream, size);
        if (FlowControl::FCAssert(type < eRANKING_MAX))
        {
            if (ranking_type >= eRANKING_TYPE_BATTLEGROUND_WIN)
            {
                //전장랭킹일 경우
                RANKINGID ranking_id = FindRankingID(mission_no, ranking_type);
                BATTLEGROUND_RANKING::const_iterator it = battleground_ranking_tables_[type].find(ranking_id);
                if (it != battleground_ranking_tables_[type].end())
                {
                    MissionRanking* mission_ranking = it->second;
                    mission_ranking->Serialize(page, archive, se);
                }
            }
            else
            {
                //전장랭킹이 아닐 경우
                MISSION_RANKING::const_iterator it = ranking_tables_[type].find(mission_no);
                if (it != ranking_tables_[type].end())
                {
                    MissionRanking* mission_ranking = it->second;
                    mission_ranking->Serialize(page, archive, se);
                }
            }
        }
        int archive_size = archive.GetSize();
        ASSERT(archive_size <= USHRT_MAX);
        size = static_cast<WORD>(archive_size);
    }
}

// 랭킹맴버를 제외한 기본 랭킹 정보만 직렬화
// 클라이언트를 위한 랭킹 직렬화
void RankingManager::SerializeForClient(eRANKING_TYPE type, RANKINGNO mission_no, BYTE page, 
                                        BYTE* INOUT stream, WORD& INOUT size, eRANKING_SERIALIZE se, 
                                        RANKINGTYPE ranking_type /*= 0*/)
{
    if (se == RANKING_SERIALIZE_CLIENT_SAVE)
    {
        if (size == 0) {
            return;
        };
        ArchiveOfStream archive(stream, size);
        MissionRanking* const mission_ranking = CreateRanking(type, mission_no, ranking_type);
        mission_ranking->SerializeForClient(page, archive, se);
        mission_ranking->Sort();
    }
    else if (se == RANKING_SERIALIZE_CLIENT_LOAD)
    {
        ArchiveOfStream archive(stream, size);
        if (FlowControl::FCAssert(type < eRANKING_MAX))
        {
            if (ranking_type >= eRANKING_TYPE_BATTLEGROUND_WIN)
            {
                //전장랭킹일경우
                RANKINGID ranking_id = FindRankingID(mission_no, ranking_type);
                BATTLEGROUND_RANKING::const_iterator it = battleground_ranking_tables_[type].find(ranking_id);
                if (it != battleground_ranking_tables_[type].end())
                {
                    MissionRanking* mission_ranking = it->second;
                    mission_ranking->SerializeForClient(page, archive, se);
                }
            }
            else
            {
                //전장랭킹이 아닐경우
                MISSION_RANKING::const_iterator it = ranking_tables_[type].find(mission_no);
                if (it != ranking_tables_[type].end())
                {
                    MissionRanking* mission_ranking = it->second;
                    mission_ranking->SerializeForClient(page, archive, se);
                }
            }
        }
        int archive_size = archive.GetSize();
        ASSERT(archive_size <= USHRT_MAX);
        size = static_cast<WORD>(archive_size);
    }
}
#else //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
MissionRanking* RankingManager::CreateRanking(eRANKING_TYPE type, RANKINGNO mission_no)
{
    MissionRanking* mission_ranking = FindMissionRanking(type, mission_no);
    if (mission_ranking == NULL) 
    {
        mission_ranking = MissionRanking::ALLOC();
        mission_ranking->SetMissionNo(mission_no);
        mission_ranking->SetRankingType(GetRankingType(mission_no));

#if defined(_NA_006802_20130704_REORGANIZATION_RANK_SYSTEM)
        if (type == eRANKING_MYRANKING)
        {
            MISSION_RANKING::_Pairib result = my_ranking_table_.insert(
                MISSION_RANKING::value_type(mission_no, mission_ranking));
            ASSERT(result.second);
        }
        else
        {
            MISSION_RANKING::_Pairib result = ranking_tables_[type].insert(
                MISSION_RANKING::value_type(mission_no, mission_ranking));
            ASSERT(result.second);
        }
#else
        MISSION_RANKING::_Pairib result = ranking_tables_[type].insert(
            MISSION_RANKING::value_type(mission_no, mission_ranking));
        ASSERT(result.second);
#endif // _NA_006802_20130704_REORGANIZATION_RANK_SYSTEM        
    }

    return mission_ranking;
}

void RankingManager::Serialize(eRANKING_TYPE type, RANKINGNO mission_no, BYTE page, 
                               BYTE* INOUT stream, WORD& INOUT size, eRANKING_SERIALIZE se)
{
    if (se == RANKING_SERIALIZE_SAVE)
    {
        if (size == 0) {
            return;
        }; // NOTE: thinks remains...
        ArchiveOfStream archive(stream, size);
        //
        MissionRanking* const mission_ranking = CreateRanking(type, mission_no);
        mission_ranking->Serialize(page, archive, se);
        mission_ranking->Sort();
    }
    else if (se == RANKING_SERIALIZE_LOAD)
    {
        ArchiveOfStream archive(stream, size);
        if (FlowControl::FCAssert(type < eRANKING_MAX))
        {
            //archive << (BYTE)1;  //< 하나의 미션 랭킹이기 때문에 1
            MISSION_RANKING::const_iterator it = ranking_tables_[type].find(mission_no);
            if (it != ranking_tables_[type].end())
            {
                MissionRanking* mission_ranking = it->second;
                mission_ranking->Serialize(page, archive, se);
            }
        }
        int archive_size = archive.GetSize();
        ASSERT(archive_size <= USHRT_MAX);
        size = static_cast<WORD>(archive_size);
    }
}

// 랭킹맴버를 제외한 기본 랭킹 정보만 직렬화
// 클라이언트를 위한 랭킹 직렬화
void RankingManager::SerializeForClient(eRANKING_TYPE type, RANKINGNO mission_no, BYTE page, 
                                        BYTE* INOUT stream, WORD& INOUT size, eRANKING_SERIALIZE se)
{
    if (se == RANKING_SERIALIZE_CLIENT_SAVE)
    {
        if (size == 0) {
            return;
        };
        ArchiveOfStream archive(stream, size);
        MissionRanking* const mission_ranking = CreateRanking(type, mission_no);
        mission_ranking->SerializeForClient(page, archive, se);
        mission_ranking->Sort();
    }
    else if (se == RANKING_SERIALIZE_CLIENT_LOAD)
    {
        ArchiveOfStream archive(stream, size);
        if (FlowControl::FCAssert(type < eRANKING_MAX))
        {
            //archive << (BYTE)1;  //< 하나의 미션 랭킹이기 때문에 1
            MISSION_RANKING::const_iterator it = ranking_tables_[type].find(mission_no);
            if (it != ranking_tables_[type].end())
            {
                MissionRanking* mission_ranking = it->second;
                mission_ranking->SerializeForClient(page, archive, se);
            }
        }
        int archive_size = archive.GetSize();
        ASSERT(archive_size <= USHRT_MAX);
        size = static_cast<WORD>(archive_size);
    }
}
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR

void RankingManager::SortAll()
{
    for (int r_type = 0 ; r_type < eRANKING_MAX  ; ++r_type)
    {
        MISSION_RANKING& ranking_table = ranking_tables_[r_type];
        FOREACH_CONTAINER(const MISSION_RANKING::value_type& node, ranking_table,
                          MISSION_RANKING)
        {
            MissionRanking* ranking = node.second;
            ranking->Sort();
        };
#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
        BATTLEGROUND_RANKING& battleground_table = battleground_ranking_tables_[r_type];
        FOREACH_CONTAINER(const BATTLEGROUND_RANKING::value_type& node, battleground_table, 
                          BATTLEGROUND_RANKING)
        {
            MissionRanking* battleground_ranking = node.second;
            battleground_ranking->Sort();
        }
#endif  //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
    }
}


namespace util { namespace internal {
;

void __GenerateCode_MissionRanking_Method_Instanced()
{
    MISSION_RANK_MEMBER_INFO member_info[1];
    ((RankingManager*)(0))->CanRegisterMissionRecord(0, _countof(member_info), member_info);
};

}}; //end of namespace
/*
RC::eRANKING_RESULT RankingManager::CanRegisterMissionRecord(
    RANKINGNO mission_no, BYTE number_of_members,
    BYTE Class1, WORD LV1,
    BYTE Class2, WORD LV2,
    BYTE Class3, WORD LV3,
    BYTE Class4, WORD LV4,
    BYTE Class5, WORD LV5,
    BYTE Class6, WORD LV6,
    BYTE Class7, WORD LV7,
    BYTE Class8, WORD LV8,
    BYTE Class9, WORD LV9,
    BYTE Class10, WORD LV10) const
{
    MISSION_RANK_MEMBER_INFO member_info_list[MAX_PARTYMEMBER_NUM];
    BOOST_STATIC_ASSERT(_countof(member_info_list) == 10);

    member_info_list[0].m_Class   = Class1;
    member_info_list[0].m_LV      = LV1;
    member_info_list[1].m_Class   = Class2;
    member_info_list[1].m_LV      = LV2;
    member_info_list[2].m_Class   = Class3;
    member_info_list[2].m_LV      = LV3;
    member_info_list[3].m_Class   = Class4;
    member_info_list[3].m_LV      = LV4;
    member_info_list[4].m_Class   = Class5;
    member_info_list[4].m_LV      = LV5;
    member_info_list[5].m_Class   = Class6;
    member_info_list[5].m_LV      = LV6;
    member_info_list[6].m_Class   = Class7;
    member_info_list[6].m_LV      = LV7;
    member_info_list[7].m_Class   = Class8;
    member_info_list[7].m_LV      = LV8;
    member_info_list[8].m_Class   = Class9;
    member_info_list[8].m_LV      = LV9;
    member_info_list[9].m_Class   = Class10;
    member_info_list[9].m_LV      = LV10;
    return CanRegisterMissionRecord(mission_no, number_of_members, member_info_list);
}
*/
RC::eRANKING_RESULT RankingManager::CanRegisterMissionRecord(
    const _MISSION_RANKING_TOTAL_INFO& ranking_total_info) const
{
    return CanRegisterMissionRecord(ranking_total_info.m_Info.m_MissionNo,
        ranking_total_info.m_Count, ranking_total_info.m_Member);
}

template<class MEMBER_INFO_TYPE>
RC::eRANKING_RESULT RankingManager::CanRegisterMissionRecord(RANKINGNO mission_no,
    BYTE number_of_members, const MEMBER_INFO_TYPE* const member_info_list) const
{
    if (ranking_status_ != eRANKING_STATUS_NORMAL) {
        return RC::RC_RANKING_SETTLING_INFO;
    };
    // 1. 5인이상
    // 2. 모든 캐릭터가 랭킹 구간 내에 존재
    // 3. 한클래스가 1/2를 넘지않는다.
    // 임시 주석
#ifdef _NA_006801_20130705_RENEWAL_RANKINGSYSTEM
    //위 3가지의 랭킹등록 조건을 제거
#else
    if (number_of_members < 5) {
        return RC::RC_RANKING_INSUFFICEINT_PARTY_MEMBER;
    };

    MISSION_LEVEL_AREA_HASH::const_iterator it = mission_level_area_table_.find(mission_no);
    if (it == mission_level_area_table_.end()) {
        return RC::RC_RANKING_INVALID_PARAM;
    };
    LEVEL_AREA* level_area = it->second;
    BYTE counter_per_classes[eCHAR_TYPE_MAX] = { 0, };
    const MEMBER_INFO_TYPE* member_it = member_info_list,
        * const member_end = member_info_list + number_of_members;
    for (; member_it != member_end; ++member_it)
    {
        ++counter_per_classes[member_it->m_Class];
        if (member_it->m_LV < level_area->m_MinLV ||
            member_it->m_LV > level_area->m_MaxLV)
        {
            return RC::RC_RANKING_DONT_INCLUDE_MISSION_LEVEL_AREA;
        }
    };
    for (BYTE* class_it = &counter_per_classes[eCHAR_BERSERKER];
        class_it < &counter_per_classes[eCHAR_TYPE_MAX];
        ++class_it)
    {
        if ((*class_it) * 2 > number_of_members) {
            return RC::RC_RANKING_A_CLASS_OBTAIN_MAJORITY;
        };
    }
#endif //_NA_006801_20130705_RENEWAL_RANKINGSYSTEM

    return RC::RC_RANKING_SUCCESS;
}

//
DWORD RankingManager::RegisterMissionRecordInTemp(
    _MISSION_RANKING_TOTAL_INFO& ranking_total_info, DWORD registering_key)
{
    static DWORD temp_key_generator_s = 0;
    MissionRankingRecord* const record = MissionRankingRecord::ALLOC();
    record->Serialize(ranking_total_info, SERIALIZE_STORE);

    const DWORD registered_key = (registering_key == 0)
                               ?    (++temp_key_generator_s) : registering_key;
    record->SetKey(registered_key);
    temp_ranking_table_.insert(TMP_MISSION_RECORD_HASH::value_type(registered_key, record));

    // Caller에서 실시간 DB Insert 쿼리 SYN
    return registered_key;
}

void RankingManager::RegisterMissionRecordOnTodayweek( RANKINGKEY key, 
                                                      _MISSION_RANKING_TOTAL_INFO& ranking_total_info)
{
    RANKINGNO mission_no = ranking_total_info.m_Info.m_MissionNo;
#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
    RANKINGTYPE ranking_type = ranking_total_info.m_Info.ranking_type;
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR

    // 현재주 랭킹 업데이트
    MissionRankingRecord* const record_this_week = MissionRankingRecord::ALLOC();
    record_this_week->Serialize(ranking_total_info, SERIALIZE_STORE);
    record_this_week->SetKey(key);

#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
    _RegisterRecord(eRANKING_TODAYWEEK, MAX_TODAYWEEK_CUTLINE_NUM, mission_no, record_this_week, ranking_type);
#else
    _RegisterRecord(eRANKING_TODAYWEEK, MAX_TODAYWEEK_CUTLINE_NUM, mission_no, record_this_week);
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR

#ifdef _NA_006801_20130705_RENEWAL_RANKINGSYSTEM
#else
    // 역대 랭킹 업데이트
    MissionRankingRecord* const record_in_history = MissionRankingRecord::ALLOC();
    record_in_history->Serialize(ranking_total_info, SERIALIZE_STORE);
    record_in_history->SetKey(key);

    _RegisterRecord(eRANKING_FAME, MAX_FAME_CUTLINE_NUM, mission_no, record_in_history);
#endif //_NA_006801_20130705_RENEWAL_RANKINGSYSTEM
}

#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
RANKING RankingManager::_RegisterRecord(eRANKING_TYPE type, BYTE max_ranking,
                                      RANKINGNO mission_no, MissionRankingRecord* ranking_record, RANKINGTYPE ranking_type /*= 0*/)
{
    const RANKINGKEY ranking_record_key = ranking_record->GetKey();

    if (ranking_type >= eRANKING_TYPE_BATTLEGROUND_WIN)
    {
        //전장랭킹 처리
        RANKINGID ranking_id = FindRankingID(mission_no, ranking_type);
        BATTLEGROUND_RANKING::iterator it = battleground_ranking_tables_[type].find(ranking_id);
        if (it == battleground_ranking_tables_[type].end())
        {
            // 랭킹을 만들어서 넣고
            MissionRanking* const mission_ranking = MissionRanking::ALLOC();
            mission_ranking->Release();
            mission_ranking->SetMissionNo(mission_no);
            mission_ranking->SetRankingType( static_cast<eRANKING_SYSTEM_TYPE>(ranking_type) );
            mission_ranking->AddRanking(ranking_record_key, ranking_record);
            battleground_ranking_tables_[type][ranking_id] = mission_ranking;
            return 1;
        }
        else
        {
            MissionRanking* const mission_ranking = (it->second);
            // 기존의 랭킹에 넣음
            mission_ranking->AddRanking(ranking_record_key, ranking_record);
            mission_ranking->Sort();

            // 순위 MAX_TODAYWEEK_CUTLINE_NUM(50)등을 넘어가면
            // 맨마직막 랭킹을 삭제, 내부에서 FREE
            int number_of_records = mission_ranking->GetRecordCount();
            if (number_of_records > max_ranking)
            {
                // 이 assertion은 보장되지 않는다
                //ASSERT(ranking->GetRecordCount() == (MaxRanking+1));
                mission_ranking->RemoveTailOrderRecord(number_of_records - max_ranking);
            }

            return mission_ranking->GetRanking(ranking_record_key);
        }
    }
    else
    {
        //미션, 포인트랭킹처리
        MISSION_RANKING::iterator it = ranking_tables_[type].find(mission_no);
        if (it == ranking_tables_[type].end())
        {
            // 랭킹을 만들어서 넣고
            MissionRanking* const mission_ranking = MissionRanking::ALLOC();
            mission_ranking->Release();
            mission_ranking->SetMissionNo(mission_no);
            mission_ranking->SetRankingType(GetRankingType(mission_no));
            mission_ranking->AddRanking(ranking_record_key, ranking_record);
            ranking_tables_[type][mission_no] = mission_ranking;
            return 1;
        }
        else
        {
            MissionRanking* const mission_ranking = (it->second);
            // 기존의 랭킹에 넣음
            mission_ranking->AddRanking(ranking_record_key, ranking_record);
            mission_ranking->Sort();

            // 순위 MAX_TODAYWEEK_CUTLINE_NUM(50)등을 넘어가면
            // 맨마직막 랭킹을 삭제, 내부에서 FREE
            int number_of_records = mission_ranking->GetRecordCount();
            if (number_of_records > max_ranking)
            {
                // 이 assertion은 보장되지 않는다
                //ASSERT(ranking->GetRecordCount() == (MaxRanking+1));
                mission_ranking->RemoveTailOrderRecord(number_of_records - max_ranking);
            }

            return mission_ranking->GetRanking(ranking_record_key);
        }
    }

}

DWORD RankingManager::OnRegisterMissionRecordOnTodayweek( BOOL success, RANKINGNO mission_no, 
                                                         RANKINGKEY key, RANKINGKEY new_key, RANKINGTYPE ranking_type /*= 0*/)
{
    // 처리가 실패하면?
    // 리스트에서만 삭제, 로그 남김
    TMP_MISSION_RECORD_HASH::iterator it = temp_ranking_table_.find(key);
    if (FlowControl::FCAssert(it != temp_ranking_table_.end()) == false) {
        SUNLOG(eCRITICAL_LOG, "OnInsertMissionRecord Failed2!no(%u)(X)[key:%d,new:%d]",
            mission_no, key, new_key);
        return 0;
    };

    MissionRankingRecord* record = (it->second);
    temp_ranking_table_.erase(it);

    if (success)
    {
        record->SetKey(new_key);

        return _RegisterRecord(eRANKING_TODAYWEEK, MAX_TODAYWEEK_CUTLINE_NUM, mission_no, record, ranking_type);

    }
    else
    {
        MissionRankingRecord::FREE(record);
        SUNLOG(eCRITICAL_LOG, "OnInsertMissionRecord Failed3!no(%u)[key:%d,new:%d]",
            mission_no, key, new_key);
    }
    return 0;
}

#else //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
DWORD RankingManager::_RegisterRecord(eRANKING_TYPE type, BYTE max_ranking,
                                      RANKINGNO mission_no, MissionRankingRecord* ranking_record)
{
    const RANKINGKEY ranking_record_key = ranking_record->GetKey();
    MISSION_RANKING::iterator it = ranking_tables_[type].find(mission_no);
    if (it == ranking_tables_[type].end())
    {
        // 랭킹을 만들어서 넣고
        MissionRanking* const mission_ranking = MissionRanking::ALLOC();
        mission_ranking->Release();
        mission_ranking->SetMissionNo(mission_no);
        mission_ranking->SetRankingType(GetRankingType(mission_no));
        mission_ranking->AddRanking(ranking_record_key, ranking_record);
        ranking_tables_[type][mission_no] = mission_ranking;
        return 1;
    }
    else
    {
        MissionRanking* const mission_ranking = (it->second);
        // 기존의 랭킹에 넣음
        mission_ranking->AddRanking(ranking_record_key, ranking_record);
        mission_ranking->Sort();

        // 순위 MAX_TODAYWEEK_CUTLINE_NUM(50)등을 넘어가면
        // 맨마직막 랭킹을 삭제, 내부에서 FREE
        int number_of_records = mission_ranking->GetRecordCount();
        if (number_of_records > max_ranking)
        {
            // 이 assertion은 보장되지 않는다
            //ASSERT(ranking->GetRecordCount() == (MaxRanking+1));
            mission_ranking->RemoveTailOrderRecord(number_of_records - max_ranking);
        }

        return mission_ranking->GetRanking(ranking_record_key);
    }
}

DWORD RankingManager::OnRegisterMissionRecordOnTodayweek(
    BOOL success, RANKINGNO mission_no, RANKINGKEY key, RANKINGKEY new_key)
{
    // 처리가 실패하면?
    // 리스트에서만 삭제, 로그 남김
    TMP_MISSION_RECORD_HASH::iterator it = temp_ranking_table_.find(key);
    if (FlowControl::FCAssert(it != temp_ranking_table_.end()) == false) {
        SUNLOG(eCRITICAL_LOG, "OnInsertMissionRecord Failed2!no(%u)(X)[key:%d,new:%d]",
            mission_no, key, new_key);
        return 0;
    };

    MissionRankingRecord* record = (it->second);
    temp_ranking_table_.erase(it);

    if (success)
    {
        record->SetKey(new_key);

#ifdef _NA_006801_20130705_RENEWAL_RANKINGSYSTEM
#else
        // 역대 랭킹 업데이트
        MissionRankingRecord* const history_record = MissionRankingRecord::ALLOC();
        history_record->Clone(record);
        _RegisterRecord(eRANKING_FAME, MAX_FAME_CUTLINE_NUM, mission_no, history_record);
#endif //_NA_006801_20130705_RENEWAL_RANKINGSYSTEM
        return _RegisterRecord(eRANKING_TODAYWEEK, MAX_TODAYWEEK_CUTLINE_NUM, mission_no, record);

    }
    else
    {
        MissionRankingRecord::FREE(record);
        SUNLOG(eCRITICAL_LOG, "OnInsertMissionRecord Failed3!no(%u)[key:%d,new:%d]",
            mission_no, key, new_key);
    }
    return 0;
}
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR

void RankingManager::InitComputation(int year, int month, int day, int hour, int minute, int second,
                                     int period, int suspend_time)
{
    SAFE_DELETE(settle_data_impl_);
    settle_data_impl_ = new SettleInfo;
    ZeroMemory(settle_data_impl_, sizeof(*settle_data_impl_));
    // 지난 정산 날짜
    settle_data_impl_->m_PeriodicDay = period;
    settle_data_impl_->m_SuspendTime = suspend_time;

    settle_data_impl_->m_OldSettleDate.tm_year = year - 1900;
    settle_data_impl_->m_OldSettleDate.tm_mon  = month - 1;
    settle_data_impl_->m_OldSettleDate.tm_mday = day;
    settle_data_impl_->m_OldSettleDate.tm_hour = hour;
    settle_data_impl_->m_OldSettleDate.tm_min  = minute;
    settle_data_impl_->m_OldSettleDate.tm_sec  = second;

    settle_data_impl_->m_SettleDate = settle_data_impl_->m_OldSettleDate;

    time_t ltime = 0, lasttime = 0;
    util::TimeSync::time(&ltime);

    int minus = 0, i = 0;
    for (i = 0 ; minus >= 0 ; ++i)
    {
        tm when2;
        when2.tm_year   = year-1900;
        when2.tm_mon    = month-1;
        when2.tm_mday   = day+(i*period);
        when2.tm_hour   = hour;
        when2.tm_min    = minute;
        when2.tm_sec    = second;
        lasttime = mktime(&when2);
        minus = (int)difftime(ltime, lasttime);
    }

    // 날짜 재수정
    for (; i > 1; --i)
    {
        NextCalcSettleTime();
    }
}

void RankingManager::NextCalcSettleTime()
{
    // 지난 정산 날짜 업데이트
    settle_data_impl_->m_OldSettleDate = settle_data_impl_->m_SettleDate;

    settle_data_impl_->m_SettleDate.tm_mday += settle_data_impl_->m_PeriodicDay;

    settle_data_impl_->m_SettleTime = mktime(&settle_data_impl_->m_SettleDate);

    settle_data_impl_->m_SuspendStartDate         = settle_data_impl_->m_SettleDate;
    settle_data_impl_->m_SuspendStartDate.tm_min -= settle_data_impl_->m_SuspendTime;
    settle_data_impl_->m_SuspendEndDate           = settle_data_impl_->m_SettleDate;
    settle_data_impl_->m_SuspendEndDate.tm_min   += settle_data_impl_->m_SuspendTime;

    settle_data_impl_->m_SuspendStartTime = mktime(&settle_data_impl_->m_SuspendStartDate);
    settle_data_impl_->m_SuspendEndTime   = mktime(&settle_data_impl_->m_SuspendEndDate);

    ASSERT(settle_data_impl_->m_SuspendStartTime <= settle_data_impl_->m_SettleTime);
    ASSERT(settle_data_impl_->m_SettleTime <= settle_data_impl_->m_SuspendEndTime);
}

// 테스트용 임시 함수!!!!!
void RankingManager::DoTestCurSettle()
{
    time_t ltime = 0;
    util::TimeSync::time(&ltime);
    tm* tmdate = localtime(&ltime);
    settle_data_impl_->m_SettleDate = *tmdate;
    settle_data_impl_->m_SuspendStartDate = *tmdate;
    settle_data_impl_->m_SuspendEndDate = *tmdate;

    settle_data_impl_->m_SettleTime       = mktime(&settle_data_impl_->m_SettleDate);
    settle_data_impl_->m_SuspendStartTime = mktime(&settle_data_impl_->m_SuspendStartDate);
    settle_data_impl_->m_SuspendEndTime   = mktime(&settle_data_impl_->m_SuspendEndDate);
    // NOTE: f110525.7L, change the timer interval when the ranking is settled by GM commands
    update_timer_.SetTimer(50);
}

void RankingManager::UpdateNormal()
{   
    time_t ltime = 0;
    util::TimeSync::time(&ltime);
    // 중지 기간이 체크
    if (difftime(ltime, settle_data_impl_->m_SuspendStartTime) > 0.0f)
    {
        settle_data_impl_->m_bSettle = false;
        SetStatus(eRANKING_STATUS_SUSPENDING);

        OnChangedStatus();
    }
}

void RankingManager::UpdateSuspending()
{
    time_t ltime = 0;
    util::TimeSync::time(&ltime);

    if ((settle_data_impl_->m_bSettle == false) &&
        (difftime(ltime, settle_data_impl_->m_SettleTime) > 0.0f))
    {
        DoSettle();
        OnSettled(); // 정산하고 GameServer로 Broadcast
    }

    // 중지 종료 기간 체크
    if (difftime(ltime, settle_data_impl_->m_SuspendEndTime) > 0)
    {
        NextCalcSettleTime();

        ASSERT(settle_data_impl_->m_bSettle && "Not terminate Settling.");
        SetStatus(eRANKING_STATUS_NORMAL);

        OnChangedStatus();
        // NOTE: f110525.7L, it is changed the timer interval
        // when the ranking is settled by GM commands. therefore timer interval rollbacks
        // to support default processing.
        update_timer_.SetTimer(kUpdateInterval);
    }
}

void RankingManager::Update()
{
    if (update_timer_.IsExpired() == false) {
        return;
    };

    ;    if (ranking_status_ == eRANKING_STATUS_NORMAL) {
        UpdateNormal();
    }
    else if (ranking_status_ == eRANKING_STATUS_SUSPENDING) {
        UpdateSuspending();
    }
    else {
    };
}

void RankingManager::DoSettle()
{
    // 임시 리스트에 들어 있는 정보 삭제
    if (FlowControl::FCAssert(temp_ranking_table_.empty()) == false)
    {
        FOREACH_CONTAINER(const TMP_MISSION_RECORD_HASH::value_type& node, temp_ranking_table_,
                          TMP_MISSION_RECORD_HASH)
        {
            MissionRankingRecord* record = node.second;
            FREE_RANKING_RECORD(record);
        };
        temp_ranking_table_.clear();
    };

    // NOTE: f110512.3L, custom processing
    // 정산하는 날이 되면
    // 지난주 클리어
    // 현재주 -> 지난주 랭킹 1,2,3으로 이동
    // 현재주 클리어

    // CHANGES: f110512.3L, logic algorithm improvements
    // #1. clear last week table
    // #2. remove filtered nodes on this week table, remains = { 1st, 2nd, 3rd (Max) }
    // #3. swap this week table with the last week table
    // TODO: ... thinks about pointer swapping

    // #1.
    if (MISSION_RANKING* ranking_table = &ranking_tables_[eRANKING_LASTWEEK])
    {
        FOREACH_CONTAINER(const MISSION_RANKING::value_type& node, *ranking_table,
                          MISSION_RANKING)
        {
            MissionRanking* ranking = node.second;
            FREE_RANKING(ranking);
        }
        ranking_table->clear();
    };

    if (MISSION_RANKING* ranking_table = &ranking_tables_[eRANKING_TODAYWEEK])
    {
        // #2.
        FOREACH_CONTAINER(const MISSION_RANKING::value_type& node, *ranking_table,
                          MISSION_RANKING)
        {
            MissionRanking* ranking = node.second;
            int number_of_records = ranking->GetRecordCount();
            if (number_of_records > 3) {
                ranking->RemoveTailOrderRecord(number_of_records - 3);
            };
        };
        // #3.
        ranking_table->swap(ranking_tables_[eRANKING_LASTWEEK]);
    };

#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
    //전장랭킹 결산
    // #1-2
    if (BATTLEGROUND_RANKING* battleground_table = &battleground_ranking_tables_[eRANKING_LASTWEEK])
    {
        FOREACH_CONTAINER(const BATTLEGROUND_RANKING::value_type& node, *battleground_table,
            BATTLEGROUND_RANKING)
        {
            MissionRanking* ranking = node.second;
            FREE_RANKING(ranking);
        }
        battleground_table->clear();
    };

    if (BATTLEGROUND_RANKING* battleground_table = &battleground_ranking_tables_[eRANKING_TODAYWEEK])
    {
        // #2-2
        FOREACH_CONTAINER(const BATTLEGROUND_RANKING::value_type& node, *battleground_table,
            BATTLEGROUND_RANKING)
        {
            MissionRanking* ranking = node.second;
            int number_of_records = ranking->GetRecordCount();
            if (number_of_records > 3) {
                ranking->RemoveTailOrderRecord(number_of_records - 3);
            };
        };
        // #3-2
        battleground_table->swap(battleground_ranking_tables_[eRANKING_LASTWEEK]);
    };
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR

    SUNLOG(eMIDDLE_LOG, "[RankingManager::DoSettle] Completed Settilement");

    settle_data_impl_->m_bSettle = true;
}

eRANKING_SYSTEM_TYPE RankingManager::GetRankingType(MAPCODE map_code) const
{ 
    LEVEL_AREA* level_area = FindLVArea(map_code);
    if (level_area == NULL) {
        return eRANKING_TYPE_NONE;
    }

    return static_cast<eRANKING_SYSTEM_TYPE>(level_area->m_RankingType);
}

WORD RankingManager::GetPointRankingIndex(MAPCODE map_code) const
{
    LEVEL_AREA* level_area = FindLVArea(map_code);
    if (level_area == NULL) {
        return 0;
    }

    return level_area->m_PointRankingIndex;
}

//==================================================================================================
//==================================================================================================
//==================================================================================================
#ifdef _SERVER

void RankingManager::DisplayRanking() const
{
#ifdef _NA_006801_20130705_RENEWAL_RANKINGSYSTEM
#else
    CONSOLELOG(eCRITICAL_LOG, _T("--------------------Fame Ranking---------------------"));
    ;{
        MISSION_RANKING::const_iterator it = ranking_tables_[eRANKING_FAME].begin(),
                                        end = ranking_tables_[eRANKING_FAME].end();
        for (; it != end; ++it)
        {
            MissionRanking* ranking = it->second;
            ranking->Display();
        }
    };
#endif //_NA_006801_20130705_RENEWAL_RANKINGSYSTEM
    CONSOLELOG(eCRITICAL_LOG, _T("-------------------LastWeek Ranking------------------"));
    {
        MISSION_RANKING::const_iterator it = ranking_tables_[eRANKING_LASTWEEK].begin(),
                                        end = ranking_tables_[eRANKING_LASTWEEK].end();
        for (; it != end ; ++it)
        {
            MissionRanking* ranking = it->second;
            ranking->Display();
        }
#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
        BATTLEGROUND_RANKING::const_iterator iter_start = battleground_ranking_tables_[eRANKING_LASTWEEK].begin(),
                                             iter_end = battleground_ranking_tables_[eRANKING_LASTWEEK].end();
        for (; iter_start != iter_end ; ++iter_start)
        {
            MissionRanking* ranking = iter_start->second;
            ranking->Display();
        }
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
    }
    CONSOLELOG(eCRITICAL_LOG, _T("-------------------ThisWeek Ranking------------------"));
    {
        MISSION_RANKING::const_iterator it = ranking_tables_[eRANKING_TODAYWEEK].begin(),
                                        end = ranking_tables_[eRANKING_TODAYWEEK].end();
        for (; it != end ; ++it)
        {
            MissionRanking* ranking = it->second;
            ranking->Display();
        }
#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
        BATTLEGROUND_RANKING::const_iterator iter_start = battleground_ranking_tables_[eRANKING_TODAYWEEK].begin(),
                                             iter_end = battleground_ranking_tables_[eRANKING_TODAYWEEK].end();
        for (; iter_start != iter_end ; ++iter_start)
        {
            MissionRanking* ranking = iter_start->second;
            ranking->Display();
        }
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
    }
    CONSOLELOG(eCRITICAL_LOG, _T("--------------------Temp Ranking---------------------"));
    {
        TMP_MISSION_RECORD_HASH::const_iterator it = temp_ranking_table_.begin(),
                                                end = temp_ranking_table_.end();
        for (; it != end ; ++it)
        {
            MissionRankingRecord* record = it->second;
            record->Display();
        }
    }
}

void RankingManager::Display() const
{
#ifdef _NA_006801_20130705_RENEWAL_RANKINGSYSTEM
#else
    CONSOLELOG(eCRITICAL_LOG, _T("--------------------Fame Ranking---------------------"));
    ;{
        MISSION_RANKING::const_iterator it = ranking_tables_[eRANKING_FAME].begin(),
                                        end = ranking_tables_[eRANKING_FAME].end();
        for (; it != end ; ++it)
        {
            MissionRanking* ranking = it->second;
            CONSOLELOG(eCRITICAL_LOG, _T("MissionNo=%d, Count=%d"),
                       ranking->GetMissionNo(), ranking->GetRecordCount());
        }

    };
#endif //_NA_006801_20130705_RENEWAL_RANKINGSYSTEM
    CONSOLELOG(eCRITICAL_LOG, _T("-------------------LastWeek Ranking------------------"));
    ;{
        MISSION_RANKING::const_iterator it = ranking_tables_[eRANKING_LASTWEEK].begin(),
                                        end = ranking_tables_[eRANKING_LASTWEEK].end();
        for (; it != end ; ++it)
        {
            MissionRanking* ranking = it->second;
            CONSOLELOG(eCRITICAL_LOG, _T("MissionNo=%d, Count=%d"),
                       ranking->GetMissionNo(), ranking->GetRecordCount());
        }
#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
        BATTLEGROUND_RANKING::const_iterator iter_start = battleground_ranking_tables_[eRANKING_LASTWEEK].begin(),
                                             iter_end = battleground_ranking_tables_[eRANKING_LASTWEEK].end();
        for (; iter_start != iter_end ; ++iter_start)
        {
            MissionRanking* ranking = iter_start->second;
            CONSOLELOG(eCRITICAL_LOG, _T("MissionNo=%d, Count=%d"),
                ranking->GetMissionNo(), ranking->GetRecordCount());
        }
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
    };
    CONSOLELOG(eCRITICAL_LOG, _T("-------------------ThisWeek Ranking------------------"));
    ;{
        MISSION_RANKING::const_iterator it = ranking_tables_[eRANKING_TODAYWEEK].begin(),
                                        end = ranking_tables_[eRANKING_TODAYWEEK].end();
        for (; it != end ; ++it)
        {
            MissionRanking* ranking = it->second;
            CONSOLELOG(eCRITICAL_LOG, _T("MissionNo=%d, Count=%d"),
                       ranking->GetMissionNo(), ranking->GetRecordCount());
        }
#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
        BATTLEGROUND_RANKING::const_iterator iter_start = battleground_ranking_tables_[eRANKING_TODAYWEEK].begin(),
                                             iter_end = battleground_ranking_tables_[eRANKING_TODAYWEEK].end();
        for (; iter_start != iter_end ; ++iter_start)
        {
            MissionRanking* ranking = iter_start->second;
            CONSOLELOG(eCRITICAL_LOG, _T("MissionNo=%d, Count=%d"),
                ranking->GetMissionNo(), ranking->GetRecordCount());
        }
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
    };
    CONSOLELOG(eCRITICAL_LOG, _T("--------------------Temp Ranking---------------------"));
    ;{
        CONSOLELOG(eCRITICAL_LOG, _T("Count=%d"), static_cast<int>(temp_ranking_table_.size()));
    };
    CONSOLELOG(eCRITICAL_LOG, _T("Status=%d"), GetStatus());

    if (settle_data_impl_ == NULL) {
        return;
    };

    char buffers_old_settled_date[32];
    ;{
        _tcsncpy(buffers_old_settled_date, asctime(&settle_data_impl_->m_OldSettleDate),
                 _countof(buffers_old_settled_date));
        buffers_old_settled_date[_countof(buffers_old_settled_date) - 1] = _T('\0');
    };
    char buffers_settled_date[32];
    ;{
        _tcsncpy(buffers_settled_date, asctime(&settle_data_impl_->m_SettleDate),
                 _countof(buffers_settled_date));
        buffers_settled_date[_countof(buffers_settled_date) - 1] = _T('\0');
    };
    char buffers_suspend_start_date[32];
    ;{
        _tcsncpy(buffers_suspend_start_date, asctime(&settle_data_impl_->m_SuspendStartDate),
                 _countof(buffers_suspend_start_date));
        buffers_suspend_start_date[_countof(buffers_suspend_start_date) - 1] = _T('\0');
    };
    char buffers_suspend_end_date[32];
    ;{
        _tcsncpy(buffers_suspend_end_date, asctime(&settle_data_impl_->m_SuspendEndDate),
                 _countof(buffers_suspend_end_date));
        buffers_suspend_end_date[_countof(buffers_suspend_end_date) - 1] = _T('\0');
    };

    CONSOLELOG(eCRITICAL_LOG,
               _T("Old Settle Date = %s")
               _T("Settle Date = %s")
               _T("Suspend Start Date = %s")
               _T("Suspend End Date = %s"),
               buffers_old_settled_date,
               buffers_settled_date,
               buffers_suspend_start_date,
               buffers_suspend_end_date);
}
#endif //_SERVER

#ifdef _NA_006801_20130705_RENEWAL_RANKINGSYSTEM
MissionRankingRecord* RankingManager::FindMyBestRanking( const TCHAR* _charname, eRANKING_TYPE _ranking_type, 
                                                         WORD _missionNo, eRANKING_SYSTEM_TYPE _ranking_system_type )
{
    MissionRanking* mission_ranking = NULL;
#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
    if (_ranking_system_type >= eRANKING_TYPE_BATTLEGROUND_WIN)
    {
        RANKINGID id = FindRankingID(_missionNo, static_cast<RANKINGTYPE>(_ranking_system_type));
        //전장랭킹타입
        BATTLEGROUND_RANKING::const_iterator it = battleground_ranking_tables_[_ranking_type].find(id);

        if (it == battleground_ranking_tables_[_ranking_type].end())
            return NULL;

        mission_ranking = it->second;
    }
    else
    {
        //미션,포인트랭킹타입
        MISSION_RANKING::const_iterator it = ranking_tables_[_ranking_type].find(_missionNo);

        if (it == ranking_tables_[_ranking_type].end())
            return NULL;

        mission_ranking = it->second;
    }
#else
    MISSION_RANKING::const_iterator it = ranking_tables_[_ranking_type].find(_missionNo);

    if (it == ranking_tables_[_ranking_type].end())
        return NULL;

    mission_ranking = it->second;
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR

    MissionRankingRecord* record = mission_ranking->FindMyBestRankingRecord(_charname, _ranking_system_type);

    if (record == NULL)
        return NULL;

    return record;
}

RankingManager::LEVEL_AREA* RankingManager::FindLVArea_ID( RANKINGID id ) const
{
    MISSION_LEVEL_AREA_HASH::const_iterator it = mission_level_area_table_.find(id);
    if (it == mission_level_area_table_.end()) {
        return NULL;
    };
    return it->second ? it->second : NULL;
}

void RankingManager::Serialize_MyRanking(const TCHAR* charname, RANKINGID id, BYTE* INOUT stream, 
                                         WORD& INOUT size, WORD& INOUT myranking)
{
    ArchiveOfStream archive(stream, size);
    LEVEL_AREA* level_area = FindLVArea_ID(id);
    if (level_area == NULL)
        return;
    
    MissionRankingRecord* record = 
        FindMyBestRanking(charname, eRANKING_TODAYWEEK, level_area->m_MissionNo, (eRANKING_SYSTEM_TYPE)level_area->m_RankingType);
    if (record != NULL)
    {
        BYTE page = 0;
        // 페이지 처리 필요
        const size_t kNumberInfoPerPage = 10;
        const size_t start_record_index = page * kNumberInfoPerPage;
        const size_t number_of_nodes = 1;
        size_t end_record_index = start_record_index + kNumberInfoPerPage;
        ;      end_record_index = min(end_record_index, number_of_nodes);
        if ((number_of_nodes <= start_record_index) ||
            (end_record_index <= start_record_index))
        {
            archive << uint8_t(0); // 개수: 0
            int archive_size = archive.GetSize();
            ASSERT(archive_size <= USHRT_MAX);
            size = static_cast<WORD>(archive_size);

            return;
        };
        //
        size_t distance = end_record_index - start_record_index;
        archive << uint8_t(distance);

        record->Serialize(archive, RANKING_SERIALIZE_LOAD);

        //클라이언트에게 보내줄 유저의 순위를 가져온다.
        RANKINGKEY ranking_key = record->GetKey();
#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
        if (level_area->m_RankingType >= eRANKING_TYPE_BATTLEGROUND_WIN)
        {
            //전장랭킹타입
            BATTLEGROUND_RANKING::const_iterator itr = battleground_ranking_tables_[eRANKING_TODAYWEEK].find(id);
            if (itr != battleground_ranking_tables_[eRANKING_TODAYWEEK].end())
            {
                MissionRanking* mission_ranking = itr->second;
                myranking = static_cast<WORD>(mission_ranking->GetRanking(ranking_key));
            }
        }
        else
        {
            //미션,포인트랭킹타입
            MISSION_RANKING::const_iterator itr = ranking_tables_[eRANKING_TODAYWEEK].find(level_area->m_MissionNo);
            if (itr != ranking_tables_[eRANKING_TODAYWEEK].end())
            {
                MissionRanking* mission_ranking = itr->second;
                myranking = static_cast<WORD>(mission_ranking->GetRanking(ranking_key));
            }
        }
#else
        MISSION_RANKING::const_iterator itr = ranking_tables_[eRANKING_TODAYWEEK].find(level_area->m_MissionNo);
        if (itr != ranking_tables_[eRANKING_TODAYWEEK].end())
        {
            MissionRanking* mission_ranking = itr->second;
            myranking = static_cast<WORD>(mission_ranking->GetRanking(ranking_key));
        }
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
    }

    int archive_size = archive.GetSize();
    ASSERT(archive_size <= USHRT_MAX);
    size = static_cast<WORD>(archive_size);
}

void RankingManager::SerializeForClient_MyRanking(const TCHAR* charname, RANKINGID id,BYTE* INOUT stream, 
                                                  WORD& INOUT size, WORD& INOUT myranking)
{
    ArchiveOfStream archive(stream, size);
    LEVEL_AREA* level_area = FindLVArea_ID(id);
    if (level_area == NULL)
        return;

    MissionRankingRecord* record = 
        FindMyBestRanking(charname, eRANKING_TODAYWEEK, level_area->m_MissionNo, (eRANKING_SYSTEM_TYPE)level_area->m_RankingType);
    if (record != NULL)
    {
        BYTE page = 0;
        // 페이지 처리 필요
        const size_t kNumberInfoPerPage = 10;
        const size_t start_record_index = page * kNumberInfoPerPage;
        const size_t number_of_nodes = 1;
        size_t end_record_index = start_record_index + kNumberInfoPerPage;
        ;      end_record_index = min(end_record_index, number_of_nodes);
        if ((number_of_nodes <= start_record_index) ||
            (end_record_index <= start_record_index))
        {
            archive << uint8_t(0); // 개수: 0
            int archive_size = archive.GetSize();
            ASSERT(archive_size <= USHRT_MAX);
            size = static_cast<WORD>(archive_size);

            return;
        };
        //
        size_t distance = end_record_index - start_record_index;
        archive << uint8_t(distance);

        record->SerializeForClient(archive, RANKING_SERIALIZE_CLIENT_LOAD);

        //클라이언트에게 보내줄 유저의 순위를 가져온다.
        RANKINGKEY ranking_key = record->GetKey();
#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
        if (level_area->m_RankingType >= eRANKING_TYPE_BATTLEGROUND_WIN)
        {
            //전장랭킹타입
            BATTLEGROUND_RANKING::const_iterator itr = battleground_ranking_tables_[eRANKING_TODAYWEEK].find(id);
            if (itr != battleground_ranking_tables_[eRANKING_TODAYWEEK].end())
            {
                MissionRanking* mission_ranking = itr->second;
                myranking = static_cast<WORD>(mission_ranking->GetRanking(ranking_key));
            }
        }
        else
        {
            //미션,포인트랭킹타입
            MISSION_RANKING::const_iterator itr = ranking_tables_[eRANKING_TODAYWEEK].find(level_area->m_MissionNo);
            if (itr != ranking_tables_[eRANKING_TODAYWEEK].end())
            {
                MissionRanking* mission_ranking = itr->second;
                myranking = static_cast<WORD>(mission_ranking->GetRanking(ranking_key));
            }
        }
#else
        MISSION_RANKING::const_iterator itr = ranking_tables_[eRANKING_TODAYWEEK].find(level_area->m_MissionNo);
        if (itr != ranking_tables_[eRANKING_TODAYWEEK].end())
        {
            MissionRanking* mission_ranking = itr->second;
            myranking = static_cast<WORD>(mission_ranking->GetRanking(ranking_key));
        }
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
    }

    int archive_size = archive.GetSize();
    ASSERT(archive_size <= USHRT_MAX);
    size = static_cast<WORD>(archive_size);
    
}

void RankingManager::GetRewardableMissionList( const TCHAR* _charname, REWARDABLE_MISSION_LIST* rewardable_mission_list )
{
    MISSION_LEVEL_AREA_HASH::iterator ranking_list_itr = mission_level_area_table_.begin();
    STLX_VECTOR<REWARDABLE_MISSION_LIST> temp_mission_list_container;

    for(ranking_list_itr; ranking_list_itr != mission_level_area_table_.end(); ++ranking_list_itr)
    {
        RANKINGID id = ranking_list_itr->first;

        LEVEL_AREA* level_area = FindLVArea_ID(id);
        if (level_area == NULL)
            return;

        

        MISSION_RANKING::const_iterator itr = ranking_tables_[eRANKING_LASTWEEK].find(level_area->m_MissionNo);
        if (itr != ranking_tables_[eRANKING_LASTWEEK].end())
        {
            MissionRanking* mission_ranking = itr->second;
            mission_ranking->GetRewardableMissionList(_charname, id, temp_mission_list_container);
        }

#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
        BATTLEGROUND_RANKING::const_iterator itr_bg = battleground_ranking_tables_[eRANKING_LASTWEEK].find(id);
        if (itr_bg != battleground_ranking_tables_[eRANKING_LASTWEEK].end())
        {
            MissionRanking* mission_ranking = itr_bg->second;
            mission_ranking->GetRewardableMissionList(_charname, id, temp_mission_list_container);
        }
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
    }

    STLX_VECTOR<REWARDABLE_MISSION_LIST>::iterator itr = temp_mission_list_container.begin();
    int arraycount = 0;
    for(itr; itr != temp_mission_list_container.end(); ++itr)
    {
        rewardable_mission_list[arraycount].ranking_key = itr->ranking_key;
        rewardable_mission_list[arraycount].ranking_id = itr->ranking_id;

        ++arraycount;
    }
}

#endif //_NA_006801_20130705_RENEWAL_RANKINGSYSTE

//==================================================================================================
//==================================================================================================
//==================================================================================================

//void RankingManager::Serialize(BYTE* INOUT stream, WORD& INOUT size, eRANKING_SERIALIZE se)
//{
//  if (se == RANKING_SERIALIZE_SAVE)
//  {
//      ClearRanking();
//
//      ArchiveOfStream archive(stream,size);
//
//      for (int r_type = 0 ; r_type < eRANKING_MAX  ; ++r_type)
//      {
//          // 명예의 전당 미션 랭킹 개수
//          BYTE MissionKindCount = 0;
//          archive >> MissionKindCount;
//          for (BYTE m = 0 ; m < MissionKindCount ; ++m)
//          {
//              MissionRanking* mission_ranking = MissionRanking::ALLOC();
//              mission_ranking->Serialize(archive, se);
//              // 정렬
//              mission_ranking->Sort();
//              std::pair<MISSION_RANKING::iterator, bool> ret = ranking_tables_[r_type].insert(
//                  MISSION_RANKING::value_type(mission_ranking->GetMissionNo(), mission_ranking));
//              ASSERT(ret.second);
//          }
//      }
//  }
//  else if (se == RANKING_SERIALIZE_LOAD)
//  {
//      ArchiveOfStream archive(stream,size);
//
//      for (int r_type = 0 ; r_type < eRANKING_MAX  ; ++r_type)
//      {
//          // 명예의 전당 미션 랭킹 개수
//          archive << (BYTE)(ranking_tables_[r_type].size());
//          MISSION_RANKING::iterator it = ranking_tables_[r_type].begin(), end = ranking_tables_[r_type].end();
//          for (; it != end ; ++it)
//          {
//              (it->second)->Serialize(archive, se);
//          }
//      }
//      //stream = m_PacketBuffer;
//      size = archive.GetSize();
//  }
//}
//
//// 랭킹 타입에 따른 랭킹 직렬화
//void RankingManager::Serialize(eRANKING_TYPE type, MISSIONNO no, 
//                             BYTE* INOUT stream, WORD& INOUT size, eRANKING_SERIALIZE se)
//{
//  if (se == RANKING_SERIALIZE_SAVE || se == RANKING_SERIALIZE_CLIENT_SAVE)
//  {
//      if (size == 0) return;
//
//      ArchiveOfStream archive(stream,size);
//
//      MissionRanking* mission_ranking = NULL;
//      MISSION_RANKING::iterator it = ranking_tables_[type].find(no);
//      if (it != ranking_tables_[type].end())
//      {
//          // 기존 정보 삭제
//          mission_ranking = (it->second);
//          mission_ranking->Release();
//      }
//      else
//      {
//          // 없으면 생성
//          mission_ranking = MissionRanking::ALLOC();
//          mission_ranking->SetMissionNo(no);
//          std::pair<MISSION_RANKING::iterator, bool> ret = ranking_tables_[type].insert(
//              MISSION_RANKING::value_type(mission_ranking->GetMissionNo(), mission_ranking));
//          ASSERT(ret.second);
//      }
//      mission_ranking->Serialize(archive, se);
//      mission_ranking->Sort();
//  }
//  else if (se == RANKING_SERIALIZE_LOAD || se == RANKING_SERIALIZE_CLIENT_LOAD)
//  {
//      ArchiveOfStream archive(stream,size);
//      ASSERT(type < eRANKING_MAX);
//      //archive << (BYTE)1;  //< 하나의 미션 랭킹이기 때문에 1
//      MISSION_RANKING::iterator it = ranking_tables_[type].find(no);
//      if (it != ranking_tables_[type].end())
//      {
//          (it->second)->Serialize(archive, se);
//      }
//      size = archive.GetSize();
//  }
//}
#endif // _NA_008012_20150130_RANKING_SYSTEM