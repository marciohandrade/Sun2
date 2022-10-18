////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   PartyFindingManager.cpp
/// @author hukim
/// @date   2010/04/19
/// @brief  [2010/04/19 by hukim] 작업 내역 최초 생성.\n
///
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "PartyFindingManager.h" // 헤더

#include <boost/foreach.hpp>
#include "User.h"
#include "UserManager.h"

#ifdef _NA_000068_20100419_PARTY_FINDING_SYSTEM


////////////////////////////////////////////////////////////////////////////////////////////////////
// REGION : 생성자 & 소멸자
////////////////////////////////////////////////////////////////////////////////////////////////////

PartyFindingManager::PartyFindingManager() : 
    lastest_party_finding_key_(PartyFindingInfo::NULL_PARTY_FINDING)
{
    enum { kDefaultPoolSize = 100 };
    enum { kExtendedPoolSize = kDefaultPoolSize / 2 };
    party_finding_pool_.Initialize(kDefaultPoolSize, kExtendedPoolSize);
    party_pool_.Initialize(kDefaultPoolSize, kExtendedPoolSize);
}

PartyFindingManager::~PartyFindingManager()
{
}

// 구현 전용 지역 함수
namespace {

// 파티 모집 정보의 유효성을 검사한다.
inline PartyFindingResultValueType scoped_is_valid_party_finding(const PartyFindingInfo& party_finding)
{
    typedef RC::PartyFindingResultUtil ResultUtil;

    // 파티 모집 목적을 검사한다.
    if (!(party_finding.party_goal_ > PartyFindingGoalConst::kMinBound && 
        party_finding.party_goal_ < PartyFindingGoalConst::kCount))
    {
        return ResultUtil::EnumToValue(RC::PARTY_FINDING_FAIL_NOT_VALID_INPUT);
    }

    // 파티 모집 인원수를 검사한다.
    if (party_finding.num_of_max_joinable_member_ < PartyFindingNumOfMemberConst::kMin ||
        party_finding.num_of_max_joinable_member_ > PartyFindingNumOfMemberConst::kMax)
    {
        return ResultUtil::EnumToValue(RC::PARTY_FINDING_FAIL_NOT_VALID_INPUT);
    }

    return ResultUtil::EnumToValue(RC::PARTY_FINDING_SUCCESS);
}

} // namespace

////////////////////////////////////////////////////////////////////////////////////////////////////
// REGION : 공개 인터페이스
////////////////////////////////////////////////////////////////////////////////////////////////////

PartyFindingResultValueType PartyFindingManager::Register(PartyFindingInfo& INOUT temp_party_finding)
{
    typedef RC::PartyFindingResultUtil ResultUtil;

    // 파티 모집 정보가 유효한지 검사한다.
    if (scoped_is_valid_party_finding(temp_party_finding) != RC::PARTY_FINDING_SUCCESS)
    {
        return ResultUtil::EnumToValue(RC::PARTY_FINDING_FAIL_NOT_VALID_INPUT);
    }
    
    // 파티 모집 요청자를 검사한다.
    User* const party_master = 
        UserManager::Instance()->FindUser(temp_party_finding.party_master_name_);
    if (party_master == NULL)
    {
        return ResultUtil::EnumToValue(RC::PARTY_FINDING_FAIL_CANT_FIND_MASTER);
    }

    // 파티 모집 요청자가 이미 파티에 속해 있는지 검사한다.
    if (party_master->GetParty() != NULL)
    {
        return ResultUtil::EnumToValue(RC::PARTY_FINDING_FAIL_MASTER_IS_MEMBER);
    }

    // 파티 모집 요청자가 이미 파티 모집 중인지 검사한다.
    BOOST_FOREACH(const PartyFindingInfoMap::value_type& party_finding_pair, party_finding_map_)
    {
        const PartyFindingInfo* const party_finding = party_finding_pair.second;
        if (party_finding != NULL)
        {
            if (_tcsicmp(party_finding->party_master_name_, (party_master->GetCharName()).c_str()) == 0)
            {
                return ResultUtil::EnumToValue(RC::PARTY_FINDING_FAIL_MASTER_IS_MEMBER);
            }
        }
    }

    // 키를 생성한다.
    const PartyFindingKeyType new_party_finding_key = MakePartyFindingKey();
    if (FindPartyFinding(new_party_finding_key) != NULL)
    {
        return ResultUtil::EnumToValue(RC::PARTY_FINDING_FAIL_SAME_KEY_IS_EXIST);
    }

    // 파티 모집 정보를 생성한다.
    PartyFindingInfo* const new_party_finding = AllocatePartyFinding();
    if (new_party_finding == NULL)
    {
        return ResultUtil::EnumToValue(RC::PARTY_FINDING_FAIL_CANT_ALLOC_POOL);
    }

    temp_party_finding.party_finding_key_ = new_party_finding_key;
    *new_party_finding = temp_party_finding;

    // 맵에 삽입한다.
    party_finding_map_.insert(
        PartyFindingInfoMap::value_type(new_party_finding->party_finding_key_, new_party_finding));
    
    return ResultUtil::EnumToValue(RC::PARTY_FINDING_SUCCESS);
}

PartyFindingResultValueType PartyFindingManager::Update(const PartyFindingInfo& temp_party_finding)
{
    typedef RC::PartyFindingResultUtil ResultUtil;

    // 파티 모집 정보가 유효한지 검사한다.
    if (scoped_is_valid_party_finding(temp_party_finding) != RC::PARTY_FINDING_SUCCESS)
    {
        return ResultUtil::EnumToValue(RC::PARTY_FINDING_FAIL_NOT_VALID_INPUT);
    }

    // 갱신하려는 파티 모집 정보가 존재하는지 검사한다.
    PartyFindingInfo* const update_wanted_party_finding = 
        FindPartyFindingByMutable(temp_party_finding.party_finding_key_);
    if (update_wanted_party_finding == NULL)
    {
        return ResultUtil::EnumToValue(RC::PARTY_FINDING_FAIL_CANT_FIND_PARTY_FINDING);
    }

    // 파티장이 맞는지 검사한다.
    if (_tcsicmp(update_wanted_party_finding->party_master_name_, 
                 temp_party_finding.party_master_name_) != 0)
    {
        return ResultUtil::EnumToValue(RC::PARTY_FINDING_FAIL_IS_NOT_MASTER);
    }

    // 연결된 파티 정보가 존재한다면
    if (update_wanted_party_finding->linked_party_key_ != PartyFindingInfo::NULL_PARTY)
    {
        const PartyInfo* const linked_party = 
            FindParty(update_wanted_party_finding->linked_party_key_);
        if (linked_party == NULL)
        {
            return ResultUtil::EnumToValue(RC::PARTY_FINDING_FAIL_CANT_FIND_PARTY);
        }

        // 갱신하려는 모집 인원 수가 현재 인원 수 보다 작은지 검사한다.
        if (temp_party_finding.num_of_max_joinable_member_ < linked_party->num_of_joined_member_)
        {
            return ResultUtil::EnumToValue(RC::PARTY_FINDING_FAIL_MAX_NUM_OF_MEMBER_IS_TOO_SMALL);
        }

        // 모집 인원수를 갱신하여 모집 인원이 찼다면
        if (temp_party_finding.num_of_max_joinable_member_ <= linked_party->num_of_joined_member_)
        {
            // 파티 모집 정보를 삭제한다.
            User* const party_master = UserManager::Instance()->FindUser(temp_party_finding.party_master_name_);
            if (Cancel(party_master, update_wanted_party_finding->party_finding_key_) != RC::PARTY_FINDING_SUCCESS)
            {
                return ResultUtil::EnumToValue(RC::PARTY_FINDING_FAIL_CANT_CANCEL);
            }
        }
    }
    
    *update_wanted_party_finding = temp_party_finding;

    return ResultUtil::EnumToValue(RC::PARTY_FINDING_SUCCESS);
}

PartyFindingResultValueType PartyFindingManager::GetList( 
    PartyFindingInfo* INOUT party_finding_list, 
    PartyFindingListConst::ValueType& OUT party_finding_list_size)
{
    typedef RC::PartyFindingResultUtil ResultUtil;

    // 가장 최근에 생성된 항목이 목록의 앞에 표시되어야 하기 때문에 역순환한다.
    PartyFindingListConst::ValueType list_index = 0;
    PartyFindingInfoMap::reverse_iterator rpos = party_finding_map_.rbegin();
    const PartyFindingInfoMap::reverse_iterator& rpos_end = party_finding_map_.rend();

    while (rpos != rpos_end)
    {
        // 파티 모집을 검사한다.
        const PartyFindingInfo* const party_finding = rpos->second;
        if (party_finding == NULL)
        {
            party_finding_map_.erase(--rpos.base());
            continue;
        }

        // 파티 모집 조건을 검사한다.
        if (scoped_is_valid_party_finding(*party_finding) != RC::PARTY_FINDING_SUCCESS)
        {
            party_finding_map_.erase(--rpos.base());
            continue;
        }

        // 파티장을 검사한다.
        const User* const party_master = 
            UserManager::Instance()->FindUser(party_finding->party_master_name_);
        if (party_master == NULL)
        {
            party_finding_map_.erase(--rpos.base());
            continue;
        }

        // 응답 목록에 복사한다.
        party_finding_list[list_index] = *(rpos->second);

        // 응답 목록 개수 제한을 검사한다.
        if (++list_index == PartyFindingListConst::kMaxLength)
        {
            break;
        }

        ++rpos;   
    }
    party_finding_list_size = list_index;
    
    return ResultUtil::EnumToValue(RC::PARTY_FINDING_SUCCESS);
}

PartyFindingResultValueType PartyFindingManager::Cancel(User* const party_master, 
                                                        const PartyFindingKeyType party_finding_key)
{
    typedef RC::PartyFindingResultUtil ResultUtil;

    // 파티장을 검사한다.
    if (party_master == NULL)
    {
        return ResultUtil::EnumToValue(RC::PARTY_FINDING_FAIL_CANT_FIND_MASTER);
    }

    // 삭제하려는 파티 모집 정보가 존재하는지 검사한다.
    const PartyFindingInfoMap::iterator& pos = party_finding_map_.find(party_finding_key);
    if (pos == party_finding_map_.end())
    {
        return ResultUtil::EnumToValue(RC::PARTY_FINDING_FAIL_CANT_FIND_PARTY_FINDING);
    }

    // 삭제하려는 파티모집의 파티장이 맞는지 검사한다.
    const PartyFindingInfo* const party_finding = pos->second;
    if (party_finding != NULL)
    {
        if (_tcsicmp(party_finding->party_master_name_, (party_master->GetCharName()).c_str()) != 0)
        {
            return ResultUtil::EnumToValue(RC::PARTY_FINDING_FAIL_IS_NOT_MASTER);
        }
    }

    // 삭제한다.
    party_finding_map_.erase(pos);

    return ResultUtil::EnumToValue(RC::PARTY_FINDING_SUCCESS);
}

PartyFindingResultValueType PartyFindingManager::CreateParty(const PartyInfo& temp_party)
{
    typedef RC::PartyFindingResultUtil ResultUtil;

    // 파티 정보를 생성한다.
    PartyInfo* const new_party = AllocateParty();
    if (new_party == NULL)
    {
        return ResultUtil::EnumToValue(RC::PARTY_FINDING_FAIL_CANT_ALLOC_POOL);
    }

    *new_party = temp_party;
    
    // 맵에 삽입한다.
    party_map_.insert(PartyInfoMap::value_type(new_party->party_key_, new_party));

    return ResultUtil::EnumToValue(RC::PARTY_FINDING_SUCCESS);
}

PartyFindingResultValueType PartyFindingManager::UpdateParty(PartyInfo& INOUT temp_party)
{
    typedef RC::PartyFindingResultUtil ResultUtil;

    // 갱신할 파티 정보를 찾는다.
    PartyInfo* const update_wanted_party = FindPartyByMutable(temp_party.party_key_);
    if (update_wanted_party == NULL)
    {
        return ResultUtil::EnumToValue(RC::PARTY_FINDING_FAIL_CANT_FIND_PARTY);
    }
    
    // 파티 인원수가 변경되었으면
    if (update_wanted_party->num_of_joined_member_ != temp_party.num_of_joined_member_)
    {
        // 연결된 파티 모집 정보가 존재하고
        if (update_wanted_party->linked_party_finding_key_ != PartyFindingInfo::NULL_PARTY_FINDING)
        {
            const PartyFindingInfo* const linked_party_finding = 
                FindPartyFinding(update_wanted_party->linked_party_finding_key_);
            if (linked_party_finding != NULL)
            {
                // 입력 정보의 연결된 파티 모집 정보를 갱신하고
                temp_party.linked_party_finding_key_ = update_wanted_party->linked_party_finding_key_;

                // 모집 인원수를 채웠으면
                if (temp_party.num_of_joined_member_ >= linked_party_finding->num_of_max_joinable_member_)
                {
                    // 파티 모집을 취소한다.
                    User* const update_wanted_party_master = 
                        UserManager::Instance()->FindUserByCharGuid(update_wanted_party->party_master_id_);
                    if (Cancel(update_wanted_party_master, update_wanted_party->linked_party_finding_key_) != 
                        RC::PARTY_FINDING_SUCCESS)
                    {
                        return ResultUtil::EnumToValue(RC::PARTY_FINDING_FAIL_CANT_CANCEL);
                    }
                }
            }
        }
    }

    // 파티장이 변경되었으면
    if (update_wanted_party->party_master_id_ != temp_party.party_master_id_)
    {
        // 새 파티장을 검사한다.
        User* const new_party_master = 
            UserManager::Instance()->FindUserByCharGuid(temp_party.party_master_id_);
        if (new_party_master == NULL)
        {
            return ResultUtil::EnumToValue(RC::PARTY_FINDING_FAIL_CANT_FIND_MASTER);
        }

        // 연결된 파티 모집 정보가 존재하면
        if (update_wanted_party->linked_party_finding_key_ != PartyFindingInfo::NULL_PARTY_FINDING && 
            FindPartyFinding(update_wanted_party->linked_party_finding_key_) != NULL)
        {
            // 입력 정보의 연결된 파티 모집 정보를 갱신하고
            temp_party.linked_party_finding_key_ = update_wanted_party->linked_party_finding_key_;

            // 파티 모집을 취소한다.
            User* const update_wanted_party_master = 
                UserManager::Instance()->FindUserByCharGuid(update_wanted_party->party_master_id_);
            if (Cancel(update_wanted_party_master, update_wanted_party->linked_party_finding_key_) != 
                RC::PARTY_FINDING_SUCCESS)
            {
                return ResultUtil::EnumToValue(RC::PARTY_FINDING_FAIL_CANT_CANCEL);
            }
        }
    }

    // 검증에 성공하면 파티 정보를 갱신한다.
    *update_wanted_party = temp_party;
    
    return ResultUtil::EnumToValue(RC::PARTY_FINDING_SUCCESS);
}

PartyFindingResultValueType PartyFindingManager::DestroyParty(const PartyInfo& temp_party)
{
    typedef RC::PartyFindingResultUtil ResultUtil;

    // 삭제할 파티 정보를 찾는다.
    const PartyInfoMap::iterator& party_pos = party_map_.find(temp_party.party_key_);
    if (party_pos == party_map_.end())
    {
        return ResultUtil::EnumToValue(RC::PARTY_FINDING_FAIL_CANT_FIND_PARTY);
    }

    const PartyInfo* const destroy_wanted_party = party_pos->second;

    // 연결된 파티 모집 정보가 존재하면
    const PartyFindingInfoMap::iterator& party_finding_pos = 
        party_finding_map_.find(destroy_wanted_party->linked_party_finding_key_);
    if (party_finding_pos != party_finding_map_.end())
    {
        // 파티 모집 정보를 삭제한다.
        party_finding_map_.erase(party_finding_pos);
    }
    
    // 파티 정보를 삭제한다.
    party_map_.erase(party_pos);

    return ResultUtil::EnumToValue(RC::PARTY_FINDING_SUCCESS);
}

#endif // _NA_000068_20100419_PARTY_FINDING_SYSTEM
