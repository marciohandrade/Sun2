////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   PartyFindingManager.cpp
/// @author hukim
/// @date   2010/04/19
/// @brief  [2010/04/19 by hukim] �۾� ���� ���� ����.\n
///
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "PartyFindingManager.h" // ���

#include <boost/foreach.hpp>
#include "User.h"
#include "UserManager.h"

#ifdef _NA_000068_20100419_PARTY_FINDING_SYSTEM


////////////////////////////////////////////////////////////////////////////////////////////////////
// REGION : ������ & �Ҹ���
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

// ���� ���� ���� �Լ�
namespace {

// ��Ƽ ���� ������ ��ȿ���� �˻��Ѵ�.
inline PartyFindingResultValueType scoped_is_valid_party_finding(const PartyFindingInfo& party_finding)
{
    typedef RC::PartyFindingResultUtil ResultUtil;

    // ��Ƽ ���� ������ �˻��Ѵ�.
    if (!(party_finding.party_goal_ > PartyFindingGoalConst::kMinBound && 
        party_finding.party_goal_ < PartyFindingGoalConst::kCount))
    {
        return ResultUtil::EnumToValue(RC::PARTY_FINDING_FAIL_NOT_VALID_INPUT);
    }

    // ��Ƽ ���� �ο����� �˻��Ѵ�.
    if (party_finding.num_of_max_joinable_member_ < PartyFindingNumOfMemberConst::kMin ||
        party_finding.num_of_max_joinable_member_ > PartyFindingNumOfMemberConst::kMax)
    {
        return ResultUtil::EnumToValue(RC::PARTY_FINDING_FAIL_NOT_VALID_INPUT);
    }

    return ResultUtil::EnumToValue(RC::PARTY_FINDING_SUCCESS);
}

} // namespace

////////////////////////////////////////////////////////////////////////////////////////////////////
// REGION : ���� �������̽�
////////////////////////////////////////////////////////////////////////////////////////////////////

PartyFindingResultValueType PartyFindingManager::Register(PartyFindingInfo& INOUT temp_party_finding)
{
    typedef RC::PartyFindingResultUtil ResultUtil;

    // ��Ƽ ���� ������ ��ȿ���� �˻��Ѵ�.
    if (scoped_is_valid_party_finding(temp_party_finding) != RC::PARTY_FINDING_SUCCESS)
    {
        return ResultUtil::EnumToValue(RC::PARTY_FINDING_FAIL_NOT_VALID_INPUT);
    }
    
    // ��Ƽ ���� ��û�ڸ� �˻��Ѵ�.
    User* const party_master = 
        UserManager::Instance()->FindUser(temp_party_finding.party_master_name_);
    if (party_master == NULL)
    {
        return ResultUtil::EnumToValue(RC::PARTY_FINDING_FAIL_CANT_FIND_MASTER);
    }

    // ��Ƽ ���� ��û�ڰ� �̹� ��Ƽ�� ���� �ִ��� �˻��Ѵ�.
    if (party_master->GetParty() != NULL)
    {
        return ResultUtil::EnumToValue(RC::PARTY_FINDING_FAIL_MASTER_IS_MEMBER);
    }

    // ��Ƽ ���� ��û�ڰ� �̹� ��Ƽ ���� ������ �˻��Ѵ�.
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

    // Ű�� �����Ѵ�.
    const PartyFindingKeyType new_party_finding_key = MakePartyFindingKey();
    if (FindPartyFinding(new_party_finding_key) != NULL)
    {
        return ResultUtil::EnumToValue(RC::PARTY_FINDING_FAIL_SAME_KEY_IS_EXIST);
    }

    // ��Ƽ ���� ������ �����Ѵ�.
    PartyFindingInfo* const new_party_finding = AllocatePartyFinding();
    if (new_party_finding == NULL)
    {
        return ResultUtil::EnumToValue(RC::PARTY_FINDING_FAIL_CANT_ALLOC_POOL);
    }

    temp_party_finding.party_finding_key_ = new_party_finding_key;
    *new_party_finding = temp_party_finding;

    // �ʿ� �����Ѵ�.
    party_finding_map_.insert(
        PartyFindingInfoMap::value_type(new_party_finding->party_finding_key_, new_party_finding));
    
    return ResultUtil::EnumToValue(RC::PARTY_FINDING_SUCCESS);
}

PartyFindingResultValueType PartyFindingManager::Update(const PartyFindingInfo& temp_party_finding)
{
    typedef RC::PartyFindingResultUtil ResultUtil;

    // ��Ƽ ���� ������ ��ȿ���� �˻��Ѵ�.
    if (scoped_is_valid_party_finding(temp_party_finding) != RC::PARTY_FINDING_SUCCESS)
    {
        return ResultUtil::EnumToValue(RC::PARTY_FINDING_FAIL_NOT_VALID_INPUT);
    }

    // �����Ϸ��� ��Ƽ ���� ������ �����ϴ��� �˻��Ѵ�.
    PartyFindingInfo* const update_wanted_party_finding = 
        FindPartyFindingByMutable(temp_party_finding.party_finding_key_);
    if (update_wanted_party_finding == NULL)
    {
        return ResultUtil::EnumToValue(RC::PARTY_FINDING_FAIL_CANT_FIND_PARTY_FINDING);
    }

    // ��Ƽ���� �´��� �˻��Ѵ�.
    if (_tcsicmp(update_wanted_party_finding->party_master_name_, 
                 temp_party_finding.party_master_name_) != 0)
    {
        return ResultUtil::EnumToValue(RC::PARTY_FINDING_FAIL_IS_NOT_MASTER);
    }

    // ����� ��Ƽ ������ �����Ѵٸ�
    if (update_wanted_party_finding->linked_party_key_ != PartyFindingInfo::NULL_PARTY)
    {
        const PartyInfo* const linked_party = 
            FindParty(update_wanted_party_finding->linked_party_key_);
        if (linked_party == NULL)
        {
            return ResultUtil::EnumToValue(RC::PARTY_FINDING_FAIL_CANT_FIND_PARTY);
        }

        // �����Ϸ��� ���� �ο� ���� ���� �ο� �� ���� ������ �˻��Ѵ�.
        if (temp_party_finding.num_of_max_joinable_member_ < linked_party->num_of_joined_member_)
        {
            return ResultUtil::EnumToValue(RC::PARTY_FINDING_FAIL_MAX_NUM_OF_MEMBER_IS_TOO_SMALL);
        }

        // ���� �ο����� �����Ͽ� ���� �ο��� á�ٸ�
        if (temp_party_finding.num_of_max_joinable_member_ <= linked_party->num_of_joined_member_)
        {
            // ��Ƽ ���� ������ �����Ѵ�.
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

    // ���� �ֱٿ� ������ �׸��� ����� �տ� ǥ�õǾ�� �ϱ� ������ ����ȯ�Ѵ�.
    PartyFindingListConst::ValueType list_index = 0;
    PartyFindingInfoMap::reverse_iterator rpos = party_finding_map_.rbegin();
    const PartyFindingInfoMap::reverse_iterator& rpos_end = party_finding_map_.rend();

    while (rpos != rpos_end)
    {
        // ��Ƽ ������ �˻��Ѵ�.
        const PartyFindingInfo* const party_finding = rpos->second;
        if (party_finding == NULL)
        {
            party_finding_map_.erase(--rpos.base());
            continue;
        }

        // ��Ƽ ���� ������ �˻��Ѵ�.
        if (scoped_is_valid_party_finding(*party_finding) != RC::PARTY_FINDING_SUCCESS)
        {
            party_finding_map_.erase(--rpos.base());
            continue;
        }

        // ��Ƽ���� �˻��Ѵ�.
        const User* const party_master = 
            UserManager::Instance()->FindUser(party_finding->party_master_name_);
        if (party_master == NULL)
        {
            party_finding_map_.erase(--rpos.base());
            continue;
        }

        // ���� ��Ͽ� �����Ѵ�.
        party_finding_list[list_index] = *(rpos->second);

        // ���� ��� ���� ������ �˻��Ѵ�.
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

    // ��Ƽ���� �˻��Ѵ�.
    if (party_master == NULL)
    {
        return ResultUtil::EnumToValue(RC::PARTY_FINDING_FAIL_CANT_FIND_MASTER);
    }

    // �����Ϸ��� ��Ƽ ���� ������ �����ϴ��� �˻��Ѵ�.
    const PartyFindingInfoMap::iterator& pos = party_finding_map_.find(party_finding_key);
    if (pos == party_finding_map_.end())
    {
        return ResultUtil::EnumToValue(RC::PARTY_FINDING_FAIL_CANT_FIND_PARTY_FINDING);
    }

    // �����Ϸ��� ��Ƽ������ ��Ƽ���� �´��� �˻��Ѵ�.
    const PartyFindingInfo* const party_finding = pos->second;
    if (party_finding != NULL)
    {
        if (_tcsicmp(party_finding->party_master_name_, (party_master->GetCharName()).c_str()) != 0)
        {
            return ResultUtil::EnumToValue(RC::PARTY_FINDING_FAIL_IS_NOT_MASTER);
        }
    }

    // �����Ѵ�.
    party_finding_map_.erase(pos);

    return ResultUtil::EnumToValue(RC::PARTY_FINDING_SUCCESS);
}

PartyFindingResultValueType PartyFindingManager::CreateParty(const PartyInfo& temp_party)
{
    typedef RC::PartyFindingResultUtil ResultUtil;

    // ��Ƽ ������ �����Ѵ�.
    PartyInfo* const new_party = AllocateParty();
    if (new_party == NULL)
    {
        return ResultUtil::EnumToValue(RC::PARTY_FINDING_FAIL_CANT_ALLOC_POOL);
    }

    *new_party = temp_party;
    
    // �ʿ� �����Ѵ�.
    party_map_.insert(PartyInfoMap::value_type(new_party->party_key_, new_party));

    return ResultUtil::EnumToValue(RC::PARTY_FINDING_SUCCESS);
}

PartyFindingResultValueType PartyFindingManager::UpdateParty(PartyInfo& INOUT temp_party)
{
    typedef RC::PartyFindingResultUtil ResultUtil;

    // ������ ��Ƽ ������ ã�´�.
    PartyInfo* const update_wanted_party = FindPartyByMutable(temp_party.party_key_);
    if (update_wanted_party == NULL)
    {
        return ResultUtil::EnumToValue(RC::PARTY_FINDING_FAIL_CANT_FIND_PARTY);
    }
    
    // ��Ƽ �ο����� ����Ǿ�����
    if (update_wanted_party->num_of_joined_member_ != temp_party.num_of_joined_member_)
    {
        // ����� ��Ƽ ���� ������ �����ϰ�
        if (update_wanted_party->linked_party_finding_key_ != PartyFindingInfo::NULL_PARTY_FINDING)
        {
            const PartyFindingInfo* const linked_party_finding = 
                FindPartyFinding(update_wanted_party->linked_party_finding_key_);
            if (linked_party_finding != NULL)
            {
                // �Է� ������ ����� ��Ƽ ���� ������ �����ϰ�
                temp_party.linked_party_finding_key_ = update_wanted_party->linked_party_finding_key_;

                // ���� �ο����� ä������
                if (temp_party.num_of_joined_member_ >= linked_party_finding->num_of_max_joinable_member_)
                {
                    // ��Ƽ ������ ����Ѵ�.
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

    // ��Ƽ���� ����Ǿ�����
    if (update_wanted_party->party_master_id_ != temp_party.party_master_id_)
    {
        // �� ��Ƽ���� �˻��Ѵ�.
        User* const new_party_master = 
            UserManager::Instance()->FindUserByCharGuid(temp_party.party_master_id_);
        if (new_party_master == NULL)
        {
            return ResultUtil::EnumToValue(RC::PARTY_FINDING_FAIL_CANT_FIND_MASTER);
        }

        // ����� ��Ƽ ���� ������ �����ϸ�
        if (update_wanted_party->linked_party_finding_key_ != PartyFindingInfo::NULL_PARTY_FINDING && 
            FindPartyFinding(update_wanted_party->linked_party_finding_key_) != NULL)
        {
            // �Է� ������ ����� ��Ƽ ���� ������ �����ϰ�
            temp_party.linked_party_finding_key_ = update_wanted_party->linked_party_finding_key_;

            // ��Ƽ ������ ����Ѵ�.
            User* const update_wanted_party_master = 
                UserManager::Instance()->FindUserByCharGuid(update_wanted_party->party_master_id_);
            if (Cancel(update_wanted_party_master, update_wanted_party->linked_party_finding_key_) != 
                RC::PARTY_FINDING_SUCCESS)
            {
                return ResultUtil::EnumToValue(RC::PARTY_FINDING_FAIL_CANT_CANCEL);
            }
        }
    }

    // ������ �����ϸ� ��Ƽ ������ �����Ѵ�.
    *update_wanted_party = temp_party;
    
    return ResultUtil::EnumToValue(RC::PARTY_FINDING_SUCCESS);
}

PartyFindingResultValueType PartyFindingManager::DestroyParty(const PartyInfo& temp_party)
{
    typedef RC::PartyFindingResultUtil ResultUtil;

    // ������ ��Ƽ ������ ã�´�.
    const PartyInfoMap::iterator& party_pos = party_map_.find(temp_party.party_key_);
    if (party_pos == party_map_.end())
    {
        return ResultUtil::EnumToValue(RC::PARTY_FINDING_FAIL_CANT_FIND_PARTY);
    }

    const PartyInfo* const destroy_wanted_party = party_pos->second;

    // ����� ��Ƽ ���� ������ �����ϸ�
    const PartyFindingInfoMap::iterator& party_finding_pos = 
        party_finding_map_.find(destroy_wanted_party->linked_party_finding_key_);
    if (party_finding_pos != party_finding_map_.end())
    {
        // ��Ƽ ���� ������ �����Ѵ�.
        party_finding_map_.erase(party_finding_pos);
    }
    
    // ��Ƽ ������ �����Ѵ�.
    party_map_.erase(party_pos);

    return ResultUtil::EnumToValue(RC::PARTY_FINDING_SUCCESS);
}

#endif // _NA_000068_20100419_PARTY_FINDING_SYSTEM
