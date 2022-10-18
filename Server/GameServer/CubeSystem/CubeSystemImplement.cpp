////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   CubeSystemImplement.cpp
/// @author hukim
/// @brief  [2010/08/25 by hukim] This is created.\n
///
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "./CubeSystemImplement.h" // header
#include "ChaoState.h"
#include "CubeSystem/CubeSystemParser.h"
#include "ItemManager.h"
#include "RewardManager.h"
#include "SCItemSlotContainer.h"
#include "ItemTypeList.h"
#include "ImplementUtil.h"

#ifdef _NA_002253_20100811_CUBE_SYSTEM
////////////////////////////////////////////////////////////////////////////////////////////////////
// REGION : Constructor & Destructor
////////////////////////////////////////////////////////////////////////////////////////////////////

CubeSystemImplement::CubeSystemImplement()
{
}

CubeSystemImplement::~CubeSystemImplement()
{
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// REGION : Implement
////////////////////////////////////////////////////////////////////////////////////////////////////

namespace util {
namespace internal {
// �ռ��Ѵ�.
static RC::eITEM_RESULT Compose(
    const MSG_CG_ITEM_COMPOSE_OR_DECOMPOSE_SYN& request, 
    Player& INOUT requester, 
    ItemCompositionOrDecompositionReward& OUT composition_reward)
{
    // ī�� ���¿��� ��� ���ɿ��δ� �ϰ����� ���� ���� �ռ� ��Ģ�� ���󰣴�.
    if (requester.GetChaoState().CanUseFuction(CHAO_USE_COMPOSE) == false) 
    {
        SUNLOG(eCRITICAL_LOG, 
               _T("|%s|ī�� ���¿����� ��� �������� �ʽ��ϴ�.|ĳ�����̸� = %s|"), 
               __FUNCTION__, 
               requester.GetCharName());
        return RC::RC_ITEM_UNABLE_FUNCTION_FOR_CHAOSTATE;
    }

    // �ռ� ������ ã�´�.
    const CubeSystemParser* const cube_system_parser = CubeSystemParser::Instance();
    const CompositionInfo* const composition = 
        cube_system_parser->FindComposition(request.reserved_code_);
    if (composition == NULL)
    {
        SUNLOG(eCRITICAL_LOG, 
               _T("|%s|�ռ� ������ ã�� �� �����ϴ�.|�ռ��ڵ� = %d, ĳ�����̸� = %s|"), 
               __FUNCTION__, 
               request.reserved_code_, 
               requester.GetCharName());
        return RC::RC_ITEM_INVALID_VALUE;
    }

    ItemManager* const item_manager = requester.GetItemManager();
    SCItemSlotContainer* const inventory = item_manager->GetItemSlotContainer(SI_INVENTORY);

    // ��û�� ������ �ռ����� ��ġ�ϴ��� �˻��Ѵ�.
    for (MSG_CG_ITEM_COMPOSE_OR_DECOMPOSE_SYN::MaterialListSize request_material_index = 0; 
         request_material_index < request.material_list_size_; 
         ++request_material_index)
    {
        // ��û�� ��Ḧ ��´�.
        const MSG_CG_ITEM_COMPOSE_OR_DECOMPOSE_SYN::MaterialInfo& request_material = 
            (request.material_list_)[request_material_index];
        
        // ��û�� ���� ��ġ�ϴ� �ռ������� ��Ḧ ��´�.
        const CompositionInfo::MaterialInfo& composition_material = 
            composition->material_list_[request_material.matched_index_];

        // ��û�� ���� ��ġ�ϴ� �ռ� ��Ḧ ã�Ҵµ� ���������� �����Ǿ� �ִٸ�
        if (composition_material.is_used_ == CompositionInfo::kUnused)
        {
            // �߸��� ���̴�.
            SUNLOG(eCRITICAL_LOG, 
                   _T("|%s|��û�� ���� �ռ� �������� ��� �������� �ʽ��ϴ�.|��û����ε��� = %d, ��ġ����ε��� = %d, ĳ�����̸� = %s|"), 
                   __FUNCTION__, 
                   request_material_index, 
                   request_material.matched_index_, 
                   requester.GetCharName());
            return RC::RC_ITEM_INVALID_VALUE;
        }
    
        // ��û ��� ������ �ռ������� ��� ���� ���� ���ٸ� ���� ������ ���鳢�� ��Ƽ� �ռ��� �������� �˻��Ѵ�.
        if (request_material.selected_number_ < composition_material.number_)
        {
            bool check_combine_composition = false;
            MSG_CG_ITEM_COMPOSE_OR_DECOMPOSE_SYN::MaterialListSize same_material_index = request_material_index;
            int same_material_number = request_material.selected_number_;
            
            while (++same_material_index < request.material_list_size_)
            {
                const MSG_CG_ITEM_COMPOSE_OR_DECOMPOSE_SYN::MaterialInfo& next_request_material = 
                    (request.material_list_)[same_material_index];
                if (next_request_material.matched_index_ != request_material.matched_index_)
                {
                    break;
                }

                // ������ ���� ��᳢�� ������ ��� �ռ� ��� ������ ������Ű��
                same_material_number += next_request_material.selected_number_;
                if (same_material_number >= composition_material.number_)
                {
                    // �˻� �����ϰ� ��ģ ��Ḹŭ �ε����� �о��ش�.
                    check_combine_composition = true;
                    request_material_index = same_material_index;
                    break;
                }
            }
            
            if (!check_combine_composition)
            {
                SUNLOG(eCRITICAL_LOG, 
                       _T("|%s|��û�� ��� ������ �ռ����� �䱸�ϴ� �������� �����մϴ�.|�ռ��ڵ� = %d, ���Į����ȣ = %d, �ʿ䰳�� = %d, ���簳�� = %d, ĳ�����̸� = %s|"), 
                       __FUNCTION__, 
                       composition->code_, 
                       composition_material.column_number_, 
                       composition_material.number_, 
                       same_material_number, 
                       requester.GetCharName());
                return RC::RC_ITEM_INVALID_VALUE;
            }
        }
        
        // ��ῡ �ش��ϴ� �κ��丮 ������ ���¸� �˻��Ѵ�.
        SCItemSlot& inventory_item = 
            static_cast<SCItemSlot&>(inventory->GetSlot(request_material.inventory_position_));
        if (inventory_item.IsLocked() || 
            inventory_item.IsBlocked() || 
            inventory_item.IsUserBlocked() || 
            !inventory_item.IsContainValidItem())
        {
            SUNLOG(eCRITICAL_LOG, 
                   _T("|%s|��ῡ �ش��ϴ� �κ��丮 �������� �ռ� ������ ���°� �ƴմϴ�.|�κ��丮��ġ = %d, ĳ�����̸� = %s|"), 
                   __FUNCTION__, 
                   request_material.inventory_position_, 
                   requester.GetCharName());
            return RC::RC_ITEM_INVALID_VALUE;
        }

        // ��û�� ����� ������ �˻��Ѵ�.
        if (request_material.selected_number_ > inventory_item.GetNum())
        {
            SUNLOG(eCRITICAL_LOG, 
                   _T("|%s|��û�� ��� ������ ���� �κ��丮�� ������ �������� �����ϴ�.|�κ��丮��ġ = %d, ��û���� = %d, �������� = %d, ĳ�����̸� = %s|"), 
                   __FUNCTION__, 
                   request_material.inventory_position_, 
                   request_material.selected_number_, 
                   inventory_item.GetNum(), 
                   requester.GetCharName());
            return RC::RC_ITEM_INVALID_VALUE;
        }

        // �ռ������� ����ڵ带 �������� ����� �� ��� �ڵ尡 ��ġ�ϴ��� �˻��Ѵ�.
        if (composition_material.code_ != CompositionInfo::kUnused)
        {
            if (composition_material.code_ != inventory_item.GetItemCode())
            {
                SUNLOG(eCRITICAL_LOG, 
                       _T("|%s|�ռ�����ڵ尡 ��ġ���� �ʽ��ϴ�.|�ռ�����ڵ� = %d, �κ��丮����ڵ� = %d, ĳ�����̸� = %s|"), 
                       __FUNCTION__, 
                       composition_material.code_, 
                       inventory_item.GetItemCode(), 
                       requester.GetCharName());
                return RC::RC_ITEM_INVALID_VALUE;
            }
        }

        // �ռ������� ��������ڵ带 �������� ����� �� ��������ڵ尡 ��ġ�ϴ��� �˻��Ѵ�.
        if (composition_material.type_code_ != CompositionInfo::kUnused)
        {
            if (composition_material.type_code_ != inventory_item.GetItemInfo()->m_wType)
            {
                SUNLOG(eCRITICAL_LOG, 
                       _T("|%s|�ռ���������ڵ尡 ��ġ���� �ʽ��ϴ�.|�ռ���������ڵ� = %d, �κ��丮��������ڵ� = %d, ĳ�����̸� = %s|"), 
                       __FUNCTION__, 
                       composition_material.type_code_, 
                       inventory_item.GetType(), 
                       requester.GetCharName());
                return RC::RC_ITEM_INVALID_VALUE;
            }
        }
        
        // �ռ����� ��᷹���� �������� ����� �� ��᷹���� ��ġ�ϴ��� �˻��Ѵ�.
        if (composition_material.level_ != CompositionInfo::kUnused)
        {
            if (composition_material.level_ != inventory_item.GetItemInfo()->m_LV)
            {
                SUNLOG(eCRITICAL_LOG, 
                       _T("|%s|�ռ���᷹���� ��ġ���� �ʽ��ϴ�.|�ռ���᷹�� = %d, �κ��丮��᷹�� = %d, ĳ�����̸� = %s|"), 
                       __FUNCTION__, 
                       composition_material.level_, 
                       inventory_item.GetItemInfo()->m_LV, 
                       requester.GetCharName());
                return RC::RC_ITEM_INVALID_VALUE;
            }
        }

        // ��û�� ��� ������ Ÿ���� ��ġ�ϴ��� �˻��Ѵ�.
        const KindOfItemType::value_type inventory_ig_type = 
            nsSlot::ItemTypeChanger::GetKindOfItemType(inventory_item);
        if (composition_material.ig_type_ != inventory_ig_type)
        {
            SUNLOG(eCRITICAL_LOG, 
                   _T("|%s|�ռ���������Ÿ���� ��ġ���� �ʽ��ϴ�.|�ռ���������Ÿ�� = %d, �κ��丮��������Ÿ�� = %d, ĳ�����̸� = %s|"), 
                   __FUNCTION__, 
                   composition_material.ig_type_, 
                   inventory_ig_type, 
                   requester.GetCharName());
            return RC::RC_ITEM_INVALID_VALUE;
        }
    } // ��û�� ������ �ռ����� ��ġ�ϴ��� �˻��Ѵ�.

    // �κ��丮�� �ּ��� ���� ������ŭ�� ���� ������ �ִ��� �˻��Ѵ�. 
    const CompositionInfo::RewardList& reward_list = composition->reward_list_;
    const CompositionInfo::RewardList::size_type reward_list_size = reward_list.size();
    if (inventory->GetEmptySlotNum() < reward_list_size)
    {
        SUNLOG(eCRITICAL_LOG, 
               _T("|%s|�κ��丮�� ������ ���� ���� ������ �����ϴ�.|ĳ�����̸� = %s|"), 
               __FUNCTION__, 
               requester.GetCharName());
        return RC::RC_ITEM_HAVENOTSPACE;
    }

    // ���� ������ �����ڵ��� ��� ������ �����Ѵ�.
    typedef ItemCompositionOrDecompositionReward CubeCompositionReward;
    const bool is_reward_code_type = 
        (reward_list_size == 1) && 
        (reward_list[0].type_ == CompositionInfo::RewardInfo::kRewardCode);
    if (is_reward_code_type)
    {
        if (!composition_reward.Init(&requester, CubeCompositionReward::kRewardCodeReward))
        {
            SUNLOG(eCRITICAL_LOG, 
                   _T("|%s|ť�� �ռ� ������ ������ �ʱ�ȭ �ϴµ� ���� �߽��ϴ�.|ĳ�����̸� = %s|"), 
                   __FUNCTION__, 
                   requester.GetCharName());
            return RC::RC_ITEM_INVALID_VALUE;
        }

        const CubeCompositionReward::RewardCode reward_code = reward_list[0].code_;
        if (!composition_reward.AddReward(reward_code))
        {
            SUNLOG(eCRITICAL_LOG, 
                   _T("|%s|�����ڵ带 ���� �߰��ϴµ� ���� �߽��ϴ�.|�����ڵ� = %d, ĳ�����̸� = %s|"), 
                   __FUNCTION__, 
                   reward_code, 
                   requester.GetCharName());
            return RC::RC_ITEM_INVALID_VALUE;
        }

        return RC::RC_ITEM_SUCCESS_TO_COMPOSE_CUBE;
    }
    
    // ���� ������ �������ڵ��� ��� ������ �����Ѵ�.
    {
        FOREACH_CONTAINER(
            const CompositionInfo::RewardInfo& reward, 
            reward_list, 
            CompositionInfo::RewardList)
        {
            // ���� Ȯ���� ����Ѵٸ�
            if (reward.ratio_ > 0)
            {
                // ���� ���� ���θ� �˻��Ѵ�.
                const int fail_ratio = random(1, CompositionInfo::RewardInfo::kMaxRewardRatioSize);
                if (fail_ratio > reward.ratio_)
                {
                    continue;
                }
            }

            CubeCompositionReward::ItemReward reward_item;
            {
                reward_item.item_code = static_cast<CubeCompositionReward::RewardItemCode>(reward.code_);
                reward_item.number_of_item = reward.number_;
                reward_item.ig_type = reward.ig_type_;
            }
            if (!composition_reward.AddItemReward(reward_item))
            {
                SUNLOG(eCRITICAL_LOG, 
                       _T("|%s|ť�� �ռ� ���� �������� ���� �߰��ϴµ� ���� �߽��ϴ�.|����������ڵ� = %d, ��������۰��� = %d, ĳ�����̸� = %s|"), 
                       __FUNCTION__, 
                       reward_item.item_code, 
                       reward_item.number_of_item, 
                       requester.GetCharName());
                return RC::RC_ITEM_INVALID_VALUE;
            }
        }
    }
    return RC::RC_ITEM_SUCCESS_TO_COMPOSE_CUBE;
}

static RC::eITEM_RESULT GetDecompositionReward(
    const DecompositionInfo::RewardList& reward_list, 
    Player& requester, 
    ItemCompositionOrDecompositionReward& OUT decomposition_reward)
{
    typedef ItemCompositionOrDecompositionReward CubeDecompositionReward;
    const ItemInfoParser* const item_info_parser = ItemInfoParser::Instance();

    // ���� ���� ��Ͽ� ����
    FOREACH_CONTAINER(
        const DecompositionInfo::RewardInfo& reward, 
        reward_list, 
        DecompositionInfo::RewardList)
    {
        // ����Ȯ���� 100%���� �۴ٸ� ���� ���� ���θ� �˻��Ѵ�.
        if (reward.ratio_ < DecompositionInfo::RewardInfo::kMaxRewardRatioSize)
        {
            const int fail_ratio = random(1, DecompositionInfo::RewardInfo::kMaxRewardRatioSize);
            if (fail_ratio > reward.ratio_)
            {
                continue;
            }
        }

        // ������ �� ������ ���Ѵ�.
        DURATYPE reward_item_number = 1;
        if (reward.ratio_ > DecompositionInfo::RewardInfo::kMaxRewardRatioSize)
        {
            // ���� ������ ������ ������ ��´�.
            reward_item_number = reward.ratio_ / DecompositionInfo::RewardInfo::kMaxRewardRatioSize;

            // �Ҽ��� ���Ϸ� ������ Ȯ���� ����Ѵ�.
            const DecompositionInfo::RewardInfo::RewardRatio temp_ratio = 
                reward.ratio_ % DecompositionInfo::RewardInfo::kMaxRewardRatioSize;
            const int fail_ratio = random(1, DecompositionInfo::RewardInfo::kMaxRewardRatioSize);
            if (fail_ratio <= temp_ratio)
            {
                ++reward_item_number;
            }
        }

        // ���� ������ ���� ó���Ѵ�.
        switch (reward.type_)
        {
        case DecompositionInfo::RewardInfo::kZard: // ���� ������ ����϶�
            {
                // ��� ����� �˻��Ѵ�.
                if (reward.code_ < DecompositionInfo::ZardInfo::kGradeLv1 || 
                    reward.code_ > DecompositionInfo::ZardInfo::kGradeLv4)
                {
                    SUNLOG(eCRITICAL_LOG, 
                           _T("|%s|�� �� ���� ��� ����Դϴ�.|����� = %d|ĳ�����̸� = %s|"), 
                           __FUNCTION__, 
                           reward.code_, 
                           requester.GetCharName());
                    return RC::RC_ITEM_INVALID_VALUE;
                }

                // ���� ������ �˻��Ѵ�.
                if (reward.min_code_ > reward.max_code_)
                {
                    SUNLOG(eCRITICAL_LOG, 
                           _T("|%s|��� �����϶� �ּҺ����ڵ�� �ִ뺸���ڵ� ���� Ŭ �� �����ϴ�.|�ּҺ����ڵ� = %d, �ִ뺸���ڵ� = %d, ĳ�����̸� = %s|"), 
                           __FUNCTION__, 
                           reward.min_code_, 
                           reward.max_code_, 
                           requester.GetCharName());
                    return RC::RC_ITEM_INVALID_VALUE;
                }

                // ���� ���������� ������ �����Ѵ�.
                for (DURATYPE zard_count = 0; zard_count < reward_item_number; ++zard_count)
                {
                    // ������ ��� �������� ã�´�.
                    const DecompositionInfo::RewardInfo::RewardCode zard_code = reward.SelectZard();
                    const BASE_ITEMINFO* const zard = item_info_parser->GetItemInfo(zard_code);
                    if (zard == NULL)
                    {
                        SUNLOG(eCRITICAL_LOG, 
                               _T("|%s|������� ��� �������� ã�� �� �����ϴ�.|������ �ڵ� = %u, ĳ�����̸� = %s|"), 
                               __FUNCTION__, 
                               zard_code, 
                               requester.GetCharName());                        
                        return RC::RC_ITEM_INVALID_VALUE;
                    }                    
                    CubeDecompositionReward::ItemReward reward_item;
                    {
                        reward_item.item_code = 
                            static_cast<CubeDecompositionReward::RewardItemCode>(zard_code);
                        reward_item.number_of_item = 1;
                    }
                    if (decomposition_reward.AddItemReward(reward_item) == false)
                    {
                        SUNLOG(eCRITICAL_LOG, 
                               _T("|%s|��� �������� ť�� ���� ���� �߰��ϴµ� ���� �߽��ϴ�.|���������ڵ� = %d, ĳ�����̸� = %s|"), 
                               __FUNCTION__, 
                               reward_item.item_code, 
                               requester.GetCharName());
                        return RC::RC_ITEM_INVALID_VALUE;
                    }
                }
            }
            break;
        case DecompositionInfo::RewardInfo::kItemCode: // ���������� �������϶�
            {
                CubeDecompositionReward::ItemReward reward_item;
                {
                    reward_item.item_code = 
                        static_cast<CubeDecompositionReward::RewardItemCode>(reward.code_);
                    reward_item.number_of_item = reward_item_number;
                }
                if (!decomposition_reward.AddItemReward(reward_item))
                {
                    SUNLOG(eCRITICAL_LOG, 
                           _T("|%s|ť�� ���� ���� �������� ���� �߰��ϴµ� ���� �߽��ϴ�.|����������ڵ� = %d, ��������۰��� = %d, ĳ�����̸� = %s|"), 
                           __FUNCTION__, 
                           reward_item.item_code, 
                           reward_item.number_of_item, 
                           requester.GetCharName());
                    return RC::RC_ITEM_INVALID_VALUE;
                }
            }
            break;
        default:
            SUNLOG(eCRITICAL_LOG, 
                   _T("|%s|�� �� ���� ���������Դϴ�.|�������� = %d|ĳ�����̸� = %s|"), 
                   __FUNCTION__, 
                   reward.type_, 
                   requester.GetCharName());
            return RC::RC_ITEM_INVALID_VALUE;
        }
    } // ���� ���� ��Ͽ� ����
    return RC::RC_ITEM_SUCCESS_TO_DECOMPOSE_CUBE;
}

// �����Ѵ�.
static RC::eITEM_RESULT Decompose(
    const MSG_CG_ITEM_COMPOSE_OR_DECOMPOSE_SYN& request, 
    Player& requester, 
    ItemCompositionOrDecompositionReward& OUT decomposition_reward)
{
    // ī�� ���¿��� ��� ���ɿ��δ� �ϰ����� ���� ���� ����ȭ ��Ģ�� ���󰣴�.
    if (requester.GetChaoState().CanUseFuction(CHAO_USE_CRYSTALIZE) == false) 
    {
        SUNLOG(eCRITICAL_LOG, 
               _T("|%s|ī�� ���¿����� ��� �������� �ʽ��ϴ�.|ĳ�����̸� = %s|"), 
               __FUNCTION__, 
               requester.GetCharName());
        return RC::RC_ITEM_UNABLE_FUNCTION_FOR_CHAOSTATE;
    }
    
    // �κ��丮 ��ġ ����� ��´�.
    MaterialInventoryPositionList inventory_position_list;
    for (MSG_CG_ITEM_COMPOSE_OR_DECOMPOSE_SYN::MaterialListSize request_material_index = 0; 
        request_material_index < request.material_list_size_; 
        ++request_material_index)
    {
        const MSG_CG_ITEM_COMPOSE_OR_DECOMPOSE_SYN::MaterialInfo& request_material = 
            request.material_list_[request_material_index];
        inventory_position_list.push_back(request_material.inventory_position_);
    }

    ItemManager* const item_manager = requester.GetItemManager();
    SCItemSlotContainer* const inventory = item_manager->GetItemSlotContainer(SI_INVENTORY);

    // ��û�� ��� ������κ��� ������ ��� ����� ��´�.
    DecompositionInfo::MaterialList decomposition_material_list;
    const CubeSystemParser* const cube_system_parser = CubeSystemParser::Instance();
    if (!cube_system_parser->GetDecompositionMaterialList(*inventory, inventory_position_list, decomposition_material_list))
    {
        SUNLOG(eCRITICAL_LOG, 
               _T("|%s|��û���κ��� ������ ��� ����� ���� �� �����ϴ�.|ĳ�����̸� = %s|"), 
               __FUNCTION__, 
               requester.GetCharName());
        return RC::RC_ITEM_INVALID_VALUE;
    }
    
    // Ŭ���̾�Ʈ���� ������ ���� ����Ʈ���� �������� ����Ѱ��� ������ �˻��Ѵ�.
    const DecompositionInfo::DecompositionPoint decomposition_point = 
        cube_system_parser->GetDecompositionPoint(decomposition_material_list);
    if (decomposition_point != static_cast<DecompositionInfo::DecompositionPoint>(request.reserved_code_))
    {
        SUNLOG(eCRITICAL_LOG, 
               _T("|%s|���� ����Ʈ ���� ��ġ���� �ʽ��ϴ�.|Ŭ���û�� = %d, ������갪 = %d|ĳ�����̸� = %s|"), 
               __FUNCTION__, 
               request.reserved_code_, 
               decomposition_point, 
               requester.GetCharName());
        return RC::RC_ITEM_INVALID_VALUE;
    }

    // ������ ��´�.
    DecompositionInfo::RewardList reward_list;
    if (!cube_system_parser->GetDecompositionRewardList(decomposition_point, reward_list))
    {
        SUNLOG(eCRITICAL_LOG, 
               _T("|%s|���� ����Ʈ�� �ش��ϴ� ���� ����� ��µ� �����߽��ϴ�.|��������Ʈ = %d|ĳ�����̸� = %s|"), 
               __FUNCTION__, 
               decomposition_point, 
               requester.GetCharName());
        return RC::RC_ITEM_INVALID_VALUE;
    }
    return GetDecompositionReward(reward_list, requester, decomposition_reward);
}
} // internal
} // util


////////////////////////////////////////////////////////////////////////////////////////////////////
// REGION : Public
////////////////////////////////////////////////////////////////////////////////////////////////////

RC::eITEM_RESULT CubeSystemImplement::CubeComposeOrDecompose( 
    const MSG_CG_ITEM_COMPOSE_OR_DECOMPOSE_SYN& request, 
    Player& INOUT requester, 
    util::internal::ItemCompositionOrDecompositionReward& OUT reward) const
{
    // ť�� �������� ����Ѵ�.
    ItemManager* const item_manager = requester.GetItemManager();
    const RC::eITEM_RESULT cube_item_using_result = item_manager->UseItemByType(
        eITEMTYPE_CUBE, SI_INVENTORY, request.using_item_position_, true, __FUNCTION__);
    if (cube_item_using_result != RC::RC_ITEM_SUCCESS)
    {
        return cube_item_using_result;
    }

    // ť�� �������� ���������� ��������Ƿ� ť����� �����۵��� �ռ� �Ǵ� �����Ѵ�.
    if (request.is_composition_)
    {
        return util::internal::Compose(request, requester, reward);
    }
    return util::internal::Decompose(request, requester, reward);
}
#endif // _NA_002253_20100811_CUBE_SYSTEM