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
// 합성한다.
static RC::eITEM_RESULT Compose(
    const MSG_CG_ITEM_COMPOSE_OR_DECOMPOSE_SYN& request, 
    Player& INOUT requester, 
    ItemCompositionOrDecompositionReward& OUT composition_reward)
{
    // 카오 상태에서 사용 가능여부는 일관성을 위해 기존 합성 규칙을 따라간다.
    if (requester.GetChaoState().CanUseFuction(CHAO_USE_COMPOSE) == false) 
    {
        SUNLOG(eCRITICAL_LOG, 
               _T("|%s|카오 상태에서는 사용 가능하지 않습니다.|캐릭터이름 = %s|"), 
               __FUNCTION__, 
               requester.GetCharName());
        return RC::RC_ITEM_UNABLE_FUNCTION_FOR_CHAOSTATE;
    }

    // 합성 정보를 찾는다.
    const CubeSystemParser* const cube_system_parser = CubeSystemParser::Instance();
    const CompositionInfo* const composition = 
        cube_system_parser->FindComposition(request.reserved_code_);
    if (composition == NULL)
    {
        SUNLOG(eCRITICAL_LOG, 
               _T("|%s|합성 정보를 찾을 수 없습니다.|합성코드 = %d, 캐릭터이름 = %s|"), 
               __FUNCTION__, 
               request.reserved_code_, 
               requester.GetCharName());
        return RC::RC_ITEM_INVALID_VALUE;
    }

    ItemManager* const item_manager = requester.GetItemManager();
    SCItemSlotContainer* const inventory = item_manager->GetItemSlotContainer(SI_INVENTORY);

    // 요청한 재료들이 합성법에 일치하는지 검사한다.
    for (MSG_CG_ITEM_COMPOSE_OR_DECOMPOSE_SYN::MaterialListSize request_material_index = 0; 
         request_material_index < request.material_list_size_; 
         ++request_material_index)
    {
        // 요청한 재료를 얻는다.
        const MSG_CG_ITEM_COMPOSE_OR_DECOMPOSE_SYN::MaterialInfo& request_material = 
            (request.material_list_)[request_material_index];
        
        // 요청한 재료와 일치하는 합성정보의 재료를 얻는다.
        const CompositionInfo::MaterialInfo& composition_material = 
            composition->material_list_[request_material.matched_index_];

        // 요청한 재료와 일치하는 합성 재료를 찾았는데 사용안함으로 설정되어 있다면
        if (composition_material.is_used_ == CompositionInfo::kUnused)
        {
            // 잘못된 값이다.
            SUNLOG(eCRITICAL_LOG, 
                   _T("|%s|요청한 재료는 합성 정보에서 사용 가능하지 않습니다.|요청재료인덱스 = %d, 일치재료인덱스 = %d, 캐릭터이름 = %s|"), 
                   __FUNCTION__, 
                   request_material_index, 
                   request_material.matched_index_, 
                   requester.GetCharName());
            return RC::RC_ITEM_INVALID_VALUE;
        }
    
        // 요청 재료 개수가 합성정보의 재료 개수 보다 적다면 같은 종류의 재료들끼리 모아서 합성이 가능한지 검사한다.
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

                // 종류가 같은 재료끼리 개수를 모아 합성 재료 개수를 만족시키면
                same_material_number += next_request_material.selected_number_;
                if (same_material_number >= composition_material.number_)
                {
                    // 검사 성공하고 합친 재료만큼 인덱스를 밀어준다.
                    check_combine_composition = true;
                    request_material_index = same_material_index;
                    break;
                }
            }
            
            if (!check_combine_composition)
            {
                SUNLOG(eCRITICAL_LOG, 
                       _T("|%s|요청한 재료 개수가 합성에서 요구하는 개수보다 부족합니다.|합성코드 = %d, 재료칼럼번호 = %d, 필요개수 = %d, 현재개수 = %d, 캐릭터이름 = %s|"), 
                       __FUNCTION__, 
                       composition->code_, 
                       composition_material.column_number_, 
                       composition_material.number_, 
                       same_material_number, 
                       requester.GetCharName());
                return RC::RC_ITEM_INVALID_VALUE;
            }
        }
        
        // 재료에 해당하는 인벤토리 아이템 상태를 검사한다.
        SCItemSlot& inventory_item = 
            static_cast<SCItemSlot&>(inventory->GetSlot(request_material.inventory_position_));
        if (inventory_item.IsLocked() || 
            inventory_item.IsBlocked() || 
            inventory_item.IsUserBlocked() || 
            !inventory_item.IsContainValidItem())
        {
            SUNLOG(eCRITICAL_LOG, 
                   _T("|%s|재료에 해당하는 인벤토리 아이템이 합성 가능한 상태가 아닙니다.|인벤토리위치 = %d, 캐릭터이름 = %s|"), 
                   __FUNCTION__, 
                   request_material.inventory_position_, 
                   requester.GetCharName());
            return RC::RC_ITEM_INVALID_VALUE;
        }

        // 요청한 재료의 개수를 검사한다.
        if (request_material.selected_number_ > inventory_item.GetNum())
        {
            SUNLOG(eCRITICAL_LOG, 
                   _T("|%s|요청한 재료 개수가 실제 인벤토리의 아이템 개수보다 많습니다.|인벤토리위치 = %d, 요청개수 = %d, 실제개수 = %d, 캐릭터이름 = %s|"), 
                   __FUNCTION__, 
                   request_material.inventory_position_, 
                   request_material.selected_number_, 
                   inventory_item.GetNum(), 
                   requester.GetCharName());
            return RC::RC_ITEM_INVALID_VALUE;
        }

        // 합성정보의 재료코드를 조건으로 사용할 때 재료 코드가 일치하는지 검사한다.
        if (composition_material.code_ != CompositionInfo::kUnused)
        {
            if (composition_material.code_ != inventory_item.GetItemCode())
            {
                SUNLOG(eCRITICAL_LOG, 
                       _T("|%s|합성재료코드가 일치하지 않습니다.|합성재료코드 = %d, 인벤토리재료코드 = %d, 캐릭터이름 = %s|"), 
                       __FUNCTION__, 
                       composition_material.code_, 
                       inventory_item.GetItemCode(), 
                       requester.GetCharName());
                return RC::RC_ITEM_INVALID_VALUE;
            }
        }

        // 합성정보의 재료종류코드를 조건으로 사용할 때 재료종류코드가 일치하는지 검사한다.
        if (composition_material.type_code_ != CompositionInfo::kUnused)
        {
            if (composition_material.type_code_ != inventory_item.GetItemInfo()->m_wType)
            {
                SUNLOG(eCRITICAL_LOG, 
                       _T("|%s|합성재료종류코드가 일치하지 않습니다.|합성재료종류코드 = %d, 인벤토리재료종류코드 = %d, 캐릭터이름 = %s|"), 
                       __FUNCTION__, 
                       composition_material.type_code_, 
                       inventory_item.GetType(), 
                       requester.GetCharName());
                return RC::RC_ITEM_INVALID_VALUE;
            }
        }
        
        // 합성법의 재료레벨을 조건으로 사용할 때 재료레벨이 일치하는지 검사한다.
        if (composition_material.level_ != CompositionInfo::kUnused)
        {
            if (composition_material.level_ != inventory_item.GetItemInfo()->m_LV)
            {
                SUNLOG(eCRITICAL_LOG, 
                       _T("|%s|합성재료레벨이 일치하지 않습니다.|합성재료레벨 = %d, 인벤토리재료레벨 = %d, 캐릭터이름 = %s|"), 
                       __FUNCTION__, 
                       composition_material.level_, 
                       inventory_item.GetItemInfo()->m_LV, 
                       requester.GetCharName());
                return RC::RC_ITEM_INVALID_VALUE;
            }
        }

        // 요청한 재료 아이템 타입이 일치하는지 검사한다.
        const KindOfItemType::value_type inventory_ig_type = 
            nsSlot::ItemTypeChanger::GetKindOfItemType(inventory_item);
        if (composition_material.ig_type_ != inventory_ig_type)
        {
            SUNLOG(eCRITICAL_LOG, 
                   _T("|%s|합성재료아이템타입이 일치하지 않습니다.|합성재료아이템타입 = %d, 인벤토리재료아이템타입 = %d, 캐릭터이름 = %s|"), 
                   __FUNCTION__, 
                   composition_material.ig_type_, 
                   inventory_ig_type, 
                   requester.GetCharName());
            return RC::RC_ITEM_INVALID_VALUE;
        }
    } // 요청한 재료들이 합성법에 일치하는지 검사한다.

    // 인벤토리에 최소한 보상 종류만큼은 여유 공간이 있는지 검사한다. 
    const CompositionInfo::RewardList& reward_list = composition->reward_list_;
    const CompositionInfo::RewardList::size_type reward_list_size = reward_list.size();
    if (inventory->GetEmptySlotNum() < reward_list_size)
    {
        SUNLOG(eCRITICAL_LOG, 
               _T("|%s|인벤토리에 보상을 받을 여유 공간이 없습니다.|캐릭터이름 = %s|"), 
               __FUNCTION__, 
               requester.GetCharName());
        return RC::RC_ITEM_HAVENOTSPACE;
    }

    // 보상 종류가 보상코드일 경우 보상을 설정한다.
    typedef ItemCompositionOrDecompositionReward CubeCompositionReward;
    const bool is_reward_code_type = 
        (reward_list_size == 1) && 
        (reward_list[0].type_ == CompositionInfo::RewardInfo::kRewardCode);
    if (is_reward_code_type)
    {
        if (!composition_reward.Init(&requester, CubeCompositionReward::kRewardCodeReward))
        {
            SUNLOG(eCRITICAL_LOG, 
                   _T("|%s|큐브 합성 보상목록 보상을 초기화 하는데 실패 했습니다.|캐릭터이름 = %s|"), 
                   __FUNCTION__, 
                   requester.GetCharName());
            return RC::RC_ITEM_INVALID_VALUE;
        }

        const CubeCompositionReward::RewardCode reward_code = reward_list[0].code_;
        if (!composition_reward.AddReward(reward_code))
        {
            SUNLOG(eCRITICAL_LOG, 
                   _T("|%s|보상코드를 보상에 추가하는데 실패 했습니다.|보상코드 = %d, 캐릭터이름 = %s|"), 
                   __FUNCTION__, 
                   reward_code, 
                   requester.GetCharName());
            return RC::RC_ITEM_INVALID_VALUE;
        }

        return RC::RC_ITEM_SUCCESS_TO_COMPOSE_CUBE;
    }
    
    // 보상 종류가 아이템코드일 경우 보상을 설정한다.
    {
        FOREACH_CONTAINER(
            const CompositionInfo::RewardInfo& reward, 
            reward_list, 
            CompositionInfo::RewardList)
        {
            // 보상 확률을 사용한다면
            if (reward.ratio_ > 0)
            {
                // 보상 가능 여부를 검사한다.
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
                       _T("|%s|큐브 합성 보상 아이템을 보상에 추가하는데 실패 했습니다.|보상아이템코드 = %d, 보상아이템개수 = %d, 캐릭터이름 = %s|"), 
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

    // 분해 보상 목록에 대해
    FOREACH_CONTAINER(
        const DecompositionInfo::RewardInfo& reward, 
        reward_list, 
        DecompositionInfo::RewardList)
    {
        // 보상확률이 100%보다 작다면 보상 가능 여부를 검사한다.
        if (reward.ratio_ < DecompositionInfo::RewardInfo::kMaxRewardRatioSize)
        {
            const int fail_ratio = random(1, DecompositionInfo::RewardInfo::kMaxRewardRatioSize);
            if (fail_ratio > reward.ratio_)
            {
                continue;
            }
        }

        // 보상할 총 개수를 구한다.
        DURATYPE reward_item_number = 1;
        if (reward.ratio_ > DecompositionInfo::RewardInfo::kMaxRewardRatioSize)
        {
            // 먼저 무조건 보상할 개수를 얻는다.
            reward_item_number = reward.ratio_ / DecompositionInfo::RewardInfo::kMaxRewardRatioSize;

            // 소수점 이하로 보상할 확률을 계산한다.
            const DecompositionInfo::RewardInfo::RewardRatio temp_ratio = 
                reward.ratio_ % DecompositionInfo::RewardInfo::kMaxRewardRatioSize;
            const int fail_ratio = random(1, DecompositionInfo::RewardInfo::kMaxRewardRatioSize);
            if (fail_ratio <= temp_ratio)
            {
                ++reward_item_number;
            }
        }

        // 보상 종류에 따라 처리한다.
        switch (reward.type_)
        {
        case DecompositionInfo::RewardInfo::kZard: // 보상 종류가 쟈드일때
            {
                // 쟈드 등급을 검사한다.
                if (reward.code_ < DecompositionInfo::ZardInfo::kGradeLv1 || 
                    reward.code_ > DecompositionInfo::ZardInfo::kGradeLv4)
                {
                    SUNLOG(eCRITICAL_LOG, 
                           _T("|%s|알 수 없는 쟈드 등급입니다.|쟈드등급 = %d|캐릭터이름 = %s|"), 
                           __FUNCTION__, 
                           reward.code_, 
                           requester.GetCharName());
                    return RC::RC_ITEM_INVALID_VALUE;
                }

                // 보상 범위를 검사한다.
                if (reward.min_code_ > reward.max_code_)
                {
                    SUNLOG(eCRITICAL_LOG, 
                           _T("|%s|쟈드 보상일때 최소보상코드는 최대보상코드 보다 클 수 없습니다.|최소보상코드 = %d, 최대보상코드 = %d, 캐릭터이름 = %s|"), 
                           __FUNCTION__, 
                           reward.min_code_, 
                           reward.max_code_, 
                           requester.GetCharName());
                    return RC::RC_ITEM_INVALID_VALUE;
                }

                // 쟈드는 개별적으로 보상을 설정한다.
                for (DURATYPE zard_count = 0; zard_count < reward_item_number; ++zard_count)
                {
                    // 보상할 쟈드 아이템을 찾는다.
                    const DecompositionInfo::RewardInfo::RewardCode zard_code = reward.SelectZard();
                    const BASE_ITEMINFO* const zard = item_info_parser->GetItemInfo(zard_code);
                    if (zard == NULL)
                    {
                        SUNLOG(eCRITICAL_LOG, 
                               _T("|%s|보상받을 쟈드 아이템을 찾을 수 없습니다.|아이템 코드 = %u, 캐릭터이름 = %s|"), 
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
                               _T("|%s|쟈드 아이템을 큐브 분해 보상에 추가하는데 실패 했습니다.|쟈드아이템코드 = %d, 캐릭터이름 = %s|"), 
                               __FUNCTION__, 
                               reward_item.item_code, 
                               requester.GetCharName());
                        return RC::RC_ITEM_INVALID_VALUE;
                    }
                }
            }
            break;
        case DecompositionInfo::RewardInfo::kItemCode: // 보상종류가 아이템일때
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
                           _T("|%s|큐브 분해 보상 아이템을 보상에 추가하는데 실패 했습니다.|보상아이템코드 = %d, 보상아이템개수 = %d, 캐릭터이름 = %s|"), 
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
                   _T("|%s|알 수 없는 보상종류입니다.|보상종류 = %d|캐릭터이름 = %s|"), 
                   __FUNCTION__, 
                   reward.type_, 
                   requester.GetCharName());
            return RC::RC_ITEM_INVALID_VALUE;
        }
    } // 분해 보상 목록에 대해
    return RC::RC_ITEM_SUCCESS_TO_DECOMPOSE_CUBE;
}

// 분해한다.
static RC::eITEM_RESULT Decompose(
    const MSG_CG_ITEM_COMPOSE_OR_DECOMPOSE_SYN& request, 
    Player& requester, 
    ItemCompositionOrDecompositionReward& OUT decomposition_reward)
{
    // 카오 상태에서 사용 가능여부는 일관성을 위해 기존 결정화 규칙을 따라간다.
    if (requester.GetChaoState().CanUseFuction(CHAO_USE_CRYSTALIZE) == false) 
    {
        SUNLOG(eCRITICAL_LOG, 
               _T("|%s|카오 상태에서는 사용 가능하지 않습니다.|캐릭터이름 = %s|"), 
               __FUNCTION__, 
               requester.GetCharName());
        return RC::RC_ITEM_UNABLE_FUNCTION_FOR_CHAOSTATE;
    }
    
    // 인벤토리 위치 목록을 얻는다.
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

    // 요청한 재료 목록으로부터 분해할 재료 목록을 얻는다.
    DecompositionInfo::MaterialList decomposition_material_list;
    const CubeSystemParser* const cube_system_parser = CubeSystemParser::Instance();
    if (!cube_system_parser->GetDecompositionMaterialList(*inventory, inventory_position_list, decomposition_material_list))
    {
        SUNLOG(eCRITICAL_LOG, 
               _T("|%s|요청으로부터 분해할 재료 목록을 얻을 수 없습니다.|캐릭터이름 = %s|"), 
               __FUNCTION__, 
               requester.GetCharName());
        return RC::RC_ITEM_INVALID_VALUE;
    }
    
    // 클라이언트에서 보내온 분해 포인트값이 서버에서 계산한값과 같은지 검사한다.
    const DecompositionInfo::DecompositionPoint decomposition_point = 
        cube_system_parser->GetDecompositionPoint(decomposition_material_list);
    if (decomposition_point != static_cast<DecompositionInfo::DecompositionPoint>(request.reserved_code_))
    {
        SUNLOG(eCRITICAL_LOG, 
               _T("|%s|분해 포인트 값이 일치하지 않습니다.|클라요청값 = %d, 서버계산값 = %d|캐릭터이름 = %s|"), 
               __FUNCTION__, 
               request.reserved_code_, 
               decomposition_point, 
               requester.GetCharName());
        return RC::RC_ITEM_INVALID_VALUE;
    }

    // 보상을 얻는다.
    DecompositionInfo::RewardList reward_list;
    if (!cube_system_parser->GetDecompositionRewardList(decomposition_point, reward_list))
    {
        SUNLOG(eCRITICAL_LOG, 
               _T("|%s|분해 포인트에 해당하는 보상 목록을 얻는데 실패했습니다.|분해포인트 = %d|캐릭터이름 = %s|"), 
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
    // 큐브 아이템을 사용한다.
    ItemManager* const item_manager = requester.GetItemManager();
    const RC::eITEM_RESULT cube_item_using_result = item_manager->UseItemByType(
        eITEMTYPE_CUBE, SI_INVENTORY, request.using_item_position_, true, __FUNCTION__);
    if (cube_item_using_result != RC::RC_ITEM_SUCCESS)
    {
        return cube_item_using_result;
    }

    // 큐브 아이템을 정상적으로 사용했으므로 큐브안의 아이템들을 합성 또는 분해한다.
    if (request.is_composition_)
    {
        return util::internal::Compose(request, requester, reward);
    }
    return util::internal::Decompose(request, requester, reward);
}
#endif // _NA_002253_20100811_CUBE_SYSTEM