#include "stdafx.h"
#include "./MissionRanking.h"

#include <functional_custom.h>

#include <ArchiveOfStream.h>

//==================================================================================================

__IMPL_CUSTOMPOOL_PTR(MissionRanking);

//==================================================================================================

MissionRanking::MissionRanking()
{
}

MissionRanking::~MissionRanking()
{
}

void MissionRanking::Release()
{
    mission_number_ = 0;
    ranking_type_ = 0;

    FOREACH_CONTAINER(const RANKING_INFO_LIST::value_type& node, ranking_info_list_,
                      RANKING_INFO_LIST)
    {
        MissionRankingRecord* record = node;
        FREE_RANKING_RECORD(record);
    };
    ranking_info_list_.clear();
    ranking_info_table_.clear();
}


void MissionRanking::AddRanking(RANKINGKEY key, MissionRankingRecord* record)
{
    RANKING_INFO_HASH::iterator found_it = ranking_info_table_.find(key);
    if (found_it == ranking_info_table_.end())
    {
        RANKING_INFO_HASH::_Pairib result = \
            ranking_info_table_.insert(RANKING_INFO_HASH::value_type(key, record));
        if (FlowControl::FCAssert(result.second && "falis ranking info insertion")) {
            ranking_info_list_.push_back(record);
        };
    }
    else
    {
        // DONE: f110526.4L, renewaled, this routine is to replace MissionRankingRecord
        MissionRankingRecord* const found_record = found_it->second;
        RANKING_INFO_LIST::iterator info_it = ranking_info_list_.begin(),
                                    info_end = ranking_info_list_.end();
        for (; info_it != info_end; ++info_it)
        {
            MissionRankingRecord* record_in_list = (*info_it);
            if (record_in_list == found_record)
            {
                FREE_RANKING_RECORD(record_in_list);
                found_it->second = record;
                (*info_it) = record;
                break;
            };
        };
        ASSERT((info_it != info_end) && "table node must match list node");
    };
}

RANKING MissionRanking::GetRanking(RANKINGKEY key) const
{
#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
    RANKING_INFO_LIST::const_iterator it = ranking_info_list_.begin(),
        end = ranking_info_list_.end();
    for (; it != end ; ++it)
    {
        MissionRankingRecord* record = (*it);
        if (key == record->GetKey()) {
            return record->GetRanking();
        };
    }
#else
    RANKING count = 0;
    RANKING_INFO_LIST::const_iterator it = ranking_info_list_.begin(),
        end = ranking_info_list_.end();
    for (; it != end ; ++it)
    {
        ++count;
        MissionRankingRecord* record = (*it);
        if (key == record->GetKey()) {
            return count;
        };
    }
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
    
    return 0;
}

//==================================================================================================
namespace util {
;

static bool RankingSortComp(const MissionRankingRecord* compare_record_first,
                            const MissionRankingRecord* compare_record_second);

static bool RankingSortPointComp(const MissionRankingRecord* compare_record_first,
                                 const MissionRankingRecord* compare_record_second);

static bool util::RankingSortComp(const MissionRankingRecord* compare_record_first,
                                  const MissionRankingRecord* compare_record_second)
{
    ;{
        // Ŭ���� �ð��� ���� ��
        DWORD l_time = compare_record_first->GetClearTime();
        DWORD r_time = compare_record_second->GetClearTime();
        if (l_time < r_time) {
            return true;
        }
        else if (l_time > r_time) {
            return false;
        };
    };
    ;{
        // �ɹ� ���� ���� ��
        int l_number = compare_record_first->GetMemberCount();
        int r_number = compare_record_second->GetMemberCount();
        if (l_number < r_number) {
            return true;
        }
        else if (l_number > r_number) {
            return false;
        };
    };
    ;{
        // ���� ������ ���� ��
        DWORD l_level = compare_record_first->GetTotalLV();
        DWORD r_level = compare_record_second->GetTotalLV();
        if (l_level < r_level) {
            return true;
        }
        else if (l_level > r_level) {
            return false;
        };
    };
    ;{
        //_NA_20100415_ADJUST_POINTRANKING_SETTLE_TERMS
        //�̼�Ű�������� �񱳰� �ƴ� ���� ��ŷ ����Ʈ�� ����� �ð����� ���ؾ� �Ѵ�.
        //�̼� Ű���� �۴ٰ� �ؼ� ����Ʈ ��ϳ�¥�� �������� �ƴϴ�.
        int64_t l_time = compare_record_first->GetRegisterTime();
        int64_t r_time = compare_record_second->GetRegisterTime();
        if (l_time < r_time) {
            return true;
        }
        else if (l_time > r_time) {
            return false;
        };
    };

    //��� ������ ���� �� �� �ִ�. ���� ����Ʈ, ���Ϸ���, ���� ��Ƽ���� ���
//    ASSERT(!"��ŷ �� ���� : ��� �� �� ����!!!");
    // NOTE: f110515.2L, change to 'false'.
    // default result is false if all conditions is not satisfied.
    return false;
}

static bool util::RankingSortPointComp(const MissionRankingRecord* compare_record_first,
                                       const MissionRankingRecord* compare_record_second)
{
    ;{
        // ����Ʈ�� ���� ��
        DWORD l_time = compare_record_first->GetClearTime();
        DWORD r_time = compare_record_second->GetClearTime();
        if (l_time > r_time) {
            return true;
        }
        else if (l_time < r_time) {
            return false;
        };
    };
    ;{
        // ���� ������ ���� ��
        DWORD l_level = compare_record_first->GetTotalLV();
        DWORD r_level = compare_record_second->GetTotalLV();
        if (l_level < r_level) {
            return true;
        }
        else if (l_level > r_level) {
            return false;
        };
    };
    ;{
        //_NA_20100415_ADJUST_POINTRANKING_SETTLE_TERMS
        //�̼�Ű�������� �񱳰� �ƴ� ���� ��ŷ ����Ʈ�� ����� �ð����� ���ؾ� �Ѵ�.
        //�̼� Ű���� �۴ٰ� �ؼ� ����Ʈ ��ϳ�¥�� �������� �ƴϴ�.
        int64_t l_time = compare_record_first->GetRegisterTime();
        int64_t r_time = compare_record_second->GetRegisterTime();
        if (l_time < r_time) {
            return true;
        }
        else if (l_time > r_time) {
            return false;
        };
    };
    //��� ������ ���� �� �� �ִ�. ���� ����Ʈ, ���Ϸ���, ���� ��Ƽ���� ���
//    ASSERT(!"��ŷ �� ���� : ��� �� �� ����!!!");
    // NOTE: f110515.2L, change to 'false'.
    // default result is false if all conditions is not satisfied.
    return false;
}

}; //end of namespace
//==================================================================================================

void MissionRanking::Sort()
{
    switch (ranking_type_)
    {
    case eRANKING_TYPE_NONE:
        SUNLOG(eFULL_LOG, _T("[MapCode:%u]Can't be RankingSystem to NONE_TYPE  - shogen\n"),
               GetMissionNo());
        return;
    case eRANKING_TYPE_TIME_ATTACK:
        std::sort(ranking_info_list_.begin(), ranking_info_list_.end(), &util::RankingSortComp);
        break;
    case eRANKING_TYPE_POINT:
        std::sort(ranking_info_list_.begin(), ranking_info_list_.end(), &util::RankingSortPointComp);
        break;
#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
    case eRANKING_TYPE_BATTLEGROUND_WIN:
    case eRANKING_TYPE_BATTLEGROUND_KILL:
    case eRANKING_TYPE_BATTLEGROUND_ASSIST:
        std::sort(ranking_info_list_.begin(), ranking_info_list_.end(), &util::RankingSortPointComp);
        break;
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
    default:
        SUNLOG(eFULL_LOG, _T("[MapCode:%u]Can't be RankingSystem to DEFAULT - shogen\n"),
               GetMissionNo());
        break;
    }

#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
    SetRankingAllRecords();
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
}

void MissionRanking::RemoveTailOrderRecord(int count)
{
    if (FlowControl::FCAssert(count > 0) == false) {
        return;
    };

    size_t remove_numbers = ranking_info_list_.size();
    ASSERT(ranking_info_list_.empty() == false &&
           remove_numbers >= size_t(count));
    for (remove_numbers = min(remove_numbers, size_t(count)); remove_numbers != 0; --remove_numbers)
    {
        MissionRankingRecord* record = ranking_info_list_.back();
        ranking_info_list_.pop_back();
        ranking_info_table_.erase(record->GetKey());

        FREE_RANKING_RECORD(record);
    }
}

MissionRankingRecord*
MissionRanking::AddRanking_Info(RANKINGKEY key, DWORD clear_time, int64_t register_time)
{
    MissionRankingRecord* const record = MissionRankingRecord::ALLOC();
    record->Release();
    record->SetKey(key);
    record->SetClearTime(clear_time);
    record->SetRegisterTime(register_time);
    this->AddRanking(key, record);
    return record;
}

void MissionRanking::Serialize(BYTE page, ArchiveOfStream& ar, eRANKING_SERIALIZE se)
{
    if (se == RANKING_SERIALIZE_SAVE)
    {
        BYTE number_of_ranking_infos = 0;

        //ar >> mission_number_;
        ar >> number_of_ranking_infos;
        for (int countdown = number_of_ranking_infos; countdown != 0; --countdown)
        {
            MissionRankingRecord* record = MissionRankingRecord::ALLOC();
            record->Serialize(ar, se);
            AddRanking(record->GetKey(), record);
        }
    }
    else if (se == RANKING_SERIALIZE_LOAD)
    {
        //ar << mission_number_;

        // ������ ó�� �ʿ�
        const size_t kNumberInfoPerPage = 10;
        const size_t start_record_index = page * kNumberInfoPerPage;
        const size_t number_of_nodes = ranking_info_list_.size();
        size_t end_record_index = start_record_index + kNumberInfoPerPage;
        ;      end_record_index = min(end_record_index, number_of_nodes);
        if ((number_of_nodes <= start_record_index) ||
            (end_record_index <= start_record_index))
        {
            ar << uint8_t(0); // ����: 0
            return;
        };
        //
        size_t distance = end_record_index - start_record_index;
        ar << uint8_t(distance);

        RANKING_INFO_LIST::const_iterator it = ranking_info_list_.begin();
        std::advance(it, start_record_index);
        RANKING_INFO_LIST::const_iterator end = it;
        std::advance(end, distance);
        for (; it != end; ++it)
        {
            MissionRankingRecord* record = *it;
            record->Serialize(ar, se);
        }
    }
}

// ������ ������ ��ŷ ����ȭ
// RANKING_SERIALIZE_CLIENT_SAVE �ÿ� ������ 0~4������ Serialize�� �Ǿ�� �Ѵ�.
void MissionRanking::SerializeForClient(BYTE page, ArchiveOfStream& ar, eRANKING_SERIALIZE se)
{
    if (se == RANKING_SERIALIZE_CLIENT_SAVE)
    {
        BYTE number_of_ranking_infos = 0;
        ar >> number_of_ranking_infos;
        for (int countdown = number_of_ranking_infos; countdown != 0; --countdown)
        {
            MissionRankingRecord* record = MissionRankingRecord::ALLOC();
            record->Release();
            record->SerializeForClient(ar, se);
            AddRanking(record->GetKey(), record);
        };
    }
    else if (se == RANKING_SERIALIZE_CLIENT_LOAD)
    {
        // ������ ó�� �ʿ�
        const size_t kNumberInfoPerPage = 10;
        const size_t start_record_index = page * kNumberInfoPerPage;
        const size_t number_of_nodes = ranking_info_list_.size();
        size_t end_record_index = start_record_index + kNumberInfoPerPage;
        ;      end_record_index = min(end_record_index, number_of_nodes);
        if ((number_of_nodes <= start_record_index) ||
            (end_record_index <= start_record_index))
        {
            ar << uint8_t(0); // ����: 0
            return;
        };
        //
        size_t distance = end_record_index - start_record_index;
        ar << uint8_t(distance);

        RANKING_INFO_LIST::const_iterator it = ranking_info_list_.begin();
        std::advance(it, start_record_index);
        RANKING_INFO_LIST::const_iterator end = it;
        std::advance(end, distance);
        for (; it != end; ++it)
        {
            MissionRankingRecord* record = *it;
            record->SerializeForClient(ar, se);
        };
    };
}

#ifdef _SERVER
void MissionRanking::Display() const
{
    CONSOLELOG(eCRITICAL_LOG, _T("Mission No: %d"), GetMissionNo());
    FOREACH_CONTAINER(const RANKING_INFO_LIST::value_type& node, ranking_info_list_,
                      RANKING_INFO_LIST)
    {
        MissionRankingRecord* record = node;
        record->Display();
    };
}
#endif //_SERVER

#ifdef _NA_006801_20130705_RENEWAL_RANKINGSYSTEM
MissionRankingRecord* MissionRanking::FindMyBestRankingRecord( const TCHAR* _charname, eRANKING_SYSTEM_TYPE _ranking_system_type )
{
    //ĳ���Ͱ� ���ԵǾ��ִ� ��Ƽ(���ڵ�)�߿��� ���� ������ ���� ���ڵ带 ã�´�
    //ranking_info_list�� �̹� ������� ���ĵǾ��ֱ� ������ ó�� �˻��Ǵ� ���ڵ带 �����ϸ� �ȴ�.
    RANKING_INFO_LIST::const_iterator it = ranking_info_list_.begin();


    MissionRankingRecord* cur_record = NULL;
    for(it ; it != ranking_info_list_.end(); ++it)
    {
        cur_record = *it;
        if (cur_record->FindMember(_charname) == NULL)
            continue;

        
        return cur_record;
    }
    
    return NULL;
}

void MissionRanking::GetRewardableMissionList( const TCHAR* _charname, RANKINGID id, STLX_VECTOR<REWARDABLE_MISSION_LIST>& mission_list_container )
{
    RANKING_INFO_LIST::const_iterator it = ranking_info_list_.begin();


    MissionRankingRecord* cur_record = NULL;
    for(it ; it != ranking_info_list_.end(); ++it)
    {
        cur_record = *it;
        //������ 0�� �����̸� ������ ���� �� ����.
        if ( cur_record->GetClearTime() <= 0) {
            continue;
        }

        RankingMember* member = cur_record->FindMember(_charname);
        if ( member == NULL)
            continue;
        //�̹� ������ �޾����� ����Ʈ�� ���� �ʴ´�
        if ( member->GetReward())
            continue;
        
        REWARDABLE_MISSION_LIST temp_list;
        temp_list.ranking_id = id;
        temp_list.ranking_key = cur_record->GetKey();

        mission_list_container.push_back(temp_list);
    }
}

#endif //_NA_006801_20130705_RENEWAL_RANKINGSYSTEM

#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
bool MissionRanking::SetRankingAllRecords()
{
    RANKING ranking = 1;
    DWORD prev_cleartime = -1;

    RANKING_INFO_LIST::const_iterator it = ranking_info_list_.begin(),
        end = ranking_info_list_.end();

    RANKINGTYPE ranking_type = GetRankingType();
    short token;
    switch (ranking_type)
    {
    case eRANKING_TYPE_TIME_ATTACK:
        token = -1; break;
    case eRANKING_TYPE_POINT:
    case eRANKING_TYPE_BATTLEGROUND_WIN:
    case eRANKING_TYPE_BATTLEGROUND_KILL:
    case eRANKING_TYPE_BATTLEGROUND_ASSIST:
        token = 1; break;
    };

    for (; it != end ; ++it)
    {
        MissionRankingRecord* record = (*it);
        if (prev_cleartime == -1)
        {
            //ù ��° ���ڵ�
            record->SetRanking(ranking);
            //���� ���ڵ��� ������ ���ϱ� ���ؼ� �����صд�
            prev_cleartime = record->GetClearTime();
        }
        else
        {
            //�� ��° �̻��� ���ڵ�
            // �������ڵ�� ���緹�ڵ��� ������ ���� = ������ ����
            if (prev_cleartime * token > record->GetClearTime() * token )
            {
                // �������ڵ�� ���緹�ڵ��� ������ �ٸ���
                ranking++;
            }
            else if (prev_cleartime * token < record->GetClearTime() * token)
            {
                //���ڵ� ������ �Ǿ����� �ʴ�.
                return false;
            }

            record->SetRanking(ranking);
            prev_cleartime = record->GetClearTime();
        }
    }

    return true;
}
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR