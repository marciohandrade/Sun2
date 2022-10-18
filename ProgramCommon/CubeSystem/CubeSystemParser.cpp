////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   CubeSystemParser.cpp
/// @author hukim
/// @brief  [2010/08/24 by hukim] This is created.\n
///
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "./CubeSystemParser.h" // header
#include "PacketStruct_CG.h"
#include "SCItemSlot.h"
#include "SCItemSlotContainer.h"
#include "ItemTypeList.h"
#include "ItemInfoParser.h"


#ifdef _NA_002253_20100811_CUBE_SYSTEM
////////////////////////////////////////////////////////////////////////////////////////////////////
// REGION : Constructor & Destructor
////////////////////////////////////////////////////////////////////////////////////////////////////

CubeSystemParser::CubeSystemParser()
{
}

CubeSystemParser::~CubeSystemParser()
{
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// REGION : Implement
////////////////////////////////////////////////////////////////////////////////////////////////////

namespace util {
namespace internal {
// 입력 재료 목록을 정렬한다.
static bool PredicateInputMaterial(
    const CompositionInfo::MaterialInfo& prev_material, 
    const CompositionInfo::MaterialInfo& next_material)
{
    // 코드 순으로 정렬한다.
    if (prev_material.code_ != next_material.code_)
    {
        return prev_material.code_ < next_material.code_;
    }

    // 타입 순으로 정렬한다.
    if (prev_material.type_code_ != next_material.type_code_)
    {
        return prev_material.type_code_ < next_material.type_code_;
    }

    // 레벨 순으로 정렬한다.
    if (prev_material.level_ != next_material.level_)
    {
        return prev_material.level_ < next_material.level_;
    }

    // 개수 순으로 정렬한다.
    return prev_material.number_ < next_material.number_;
}

// 유효하지 않은 인벤토리 위치인가?
static bool IsValidInventoryPosition(const SCItemSlotContainer& inventory, const POSTYPE inventory_position)
{
    return (inventory_position >= 0 && inventory_position < inventory.GetMaxSlotNum());
}

// 유효하지 않은 인벤토리 아이템인가?
static bool IsValidInventoryItemState(const SCItemSlot& inventory_item)
{
#ifdef _SERVER
    return (!inventory_item.IsLocked() && 
        !inventory_item.IsBlocked() && 
        !inventory_item.IsUserBlocked() && 
        inventory_item.IsContainValidItem());
#else
    return (!inventory_item.IsBlocked() && 
        !inventory_item.IsUserBlocked() && 
        inventory_item.IsContainValidItem());
#endif // _SERVER
}

// 입력 재료 목록을 얻는다.
static bool GetInputMaterialList(
    SCItemSlotContainer& inventory, 
    const MaterialInventoryPositionList& inventory_position_list, 
    CompositionInfo::MaterialList& OUT input_material_list)
{
    CompositionInfo::MaterialList::size_type material_index = 0;

    FOREACH_CONTAINER(
        const POSTYPE inventory_position, inventory_position_list, MaterialInventoryPositionList)
    {
        if (!IsValidInventoryPosition(inventory, inventory_position))
        {
            continue;
        }

        SCItemSlot& inventory_item = static_cast<SCItemSlot&>(inventory.GetSlot(inventory_position));
        if (!IsValidInventoryItemState(inventory_item))
        {
            continue;
        }

        CompositionInfo::MaterialInfo& input_material = input_material_list[material_index];
        input_material.is_used_ = false;
        input_material.code_ = inventory_item.GetItemCode();
        input_material.type_code_ = inventory_item.GetItemInfo()->m_wType;
        input_material.level_ = inventory_item.GetItemInfo()->m_LV;
        input_material.number_ = inventory_item.GetNum();
        input_material.ig_type_ = nsSlot::ItemTypeChanger::GetKindOfItemType(inventory_item);
        input_material.inventory_position_ = inventory_position;
        
        ++material_index;
    }

    if (material_index == 0)
    {
        return false;
    }

    input_material_list.set_size(material_index);

    // 입력한 재료를 코드/타입/레벨/개수 순으로 정렬한다.
    std::stable_sort(input_material_list.begin(), input_material_list.end(), PredicateInputMaterial);

    return true;
}

// 일치하는 합성을 정렬한다.
static bool PredicateMatchedComposition(
    const MatchedCompositionInfo& prev_composition, 
    const MatchedCompositionInfo& next_composition)
{
    return (prev_composition.composition_->weight_) > (next_composition.composition_->weight_);
}

// 일치하는 합성을 얻는다.
static bool GetMatchedComposition(
    const CompositionInfo::MaterialList& input_material_list, 
    const CompositionInfo& composition, 
    MatchedCompositionInfo& OUT matched_composition)
{
    // 합성 재료 개수가 입력 재료 개수보다 많다면
    if ((composition.material_list_).size() > input_material_list.size())
    {
        // 일치할 수 없다.
        return false;
    }

    // 일치하는 재료 목록 크기를 초기화하고 합성 재료 목록 크기로 설정한다.
    MatchedCompositionInfo::MatchedMaterialList& matched_material_list = matched_composition.matched_material_list_;
    FOREACH_CONTAINER(
        MatchedCompositionInfo::MatchedMaterialInfo& matched_material, 
        matched_material_list, 
        MatchedCompositionInfo::MatchedMaterialList)
    {
        matched_material.Clear();
    }
    matched_material_list.set_size((composition.material_list_).size());

    // 입력 재료에 대해
    CompositionInfo::MaterialList::const_iterator input_material_pos = input_material_list.begin();
    while (input_material_pos != input_material_list.end())
    {
        const CompositionInfo::MaterialInfo& input_material = *input_material_pos;
        MatchedCompositionInfo::MatchedMaterialList::iterator matched_material_pos = 
            matched_material_list.begin();
        
        // 합성 재료에 대해
        FOREACH_CONTAINER(
            const CompositionInfo::MaterialInfo& composition_material, 
            composition.material_list_, 
            CompositionInfo::MaterialList)
        {
            MatchedCompositionInfo::MatchedMaterialInfo& matched_material = *matched_material_pos;
            ++matched_material_pos;

            // 이미 일치하는 재료를 찾았다면
            if (matched_material.is_find_)
            {
                // 다음 재료를 검사한다.
                continue;
            }

            // 재료 코드가 일치하는지 검사한다.
            if (composition_material.code_ != CompositionInfo::kUnused)
            {
                if (composition_material.code_ != input_material.code_)
                {
                    continue;
                }
            }

            // 재료 종류가 일치하는지 검사한다.
            if (composition_material.type_code_ != CompositionInfo::kUnused)
            {
                if (composition_material.type_code_ != input_material.type_code_)
                {
                    continue;
                }
            }

            // 재료 레벨이 일치하는지 검사한다.
            if (composition_material.level_ != CompositionInfo::kUnused)
            {
                if (composition_material.level_ != input_material.level_)
                {
                    continue;
                }
            }

            // 재료 아이템 타입이 일치하는지 검사한다.
            if (composition_material.ig_type_ != input_material.ig_type_)
            {
                continue;
            }

            matched_material.input_material_start_pos_ = &input_material;
            matched_material.is_mixed_input_material_ = false;

            // 다른 조건은 만족하는데 입력 재료 개수가 부족하다면
            if (composition_material.number_ > input_material.number_)
            {
                // 코드가 같은 입력 재료들의 개수를 합쳐서 합성 재료 개수를 만족시킬 수 있는지 검사한다.
                int same_material_number = input_material.number_;                        
                CompositionInfo::MaterialList::const_iterator temp_input_material_pos = &input_material;
                while (++temp_input_material_pos != input_material_list.end())
                {
                    // 다음 입력 재료가 현재 입력 재료와 같은 코드인지 검사한다.
                    const CompositionInfo::MaterialInfo& next_input_material = *temp_input_material_pos;
                    if (next_input_material.code_ != input_material.code_)
                    {
                        break;
                    }

                    // 다음 입력 재료가 같은 코드의 재료라면 개수를 더해서 다시 검사한다.
                    same_material_number += next_input_material.number_;
                    if (composition_material.number_ > same_material_number)
                    {
                        continue;
                    }

                    // 여기까지 왔다면 같은 코드의 입력 재료를 모아서 합성 재료 하나를 만족 시킨다.
                    matched_material.input_material_end_pos_ = temp_input_material_pos + 1;
                    matched_material.is_mixed_input_material_ = true;
                    break;
                }

                // 같은 코드의 재료를 합쳐도 개수가 부족하면
                if (!matched_material.is_mixed_input_material_)
                {
                    // 다음 재료를 검사한다.
                    continue;
                }

                // 같은 코드의 입력 재료를 합쳐서 일치하는 재료 찾기에 성공했다면 
                // 입력 재료의 반복자를 합친 범위 만큼 증가시켜 준다.
                input_material_pos = temp_input_material_pos;
            }

            // 일치하는 재료를 발견했음을 설정한다.
            matched_material.composition_material_ = &composition_material;
            matched_material.is_find_ = true;            
        } // 합성 재료에 대해

        ++input_material_pos;
    } // 입력 재료에 대해
    
    // 모든 일치하는 재료를 찾았는지 검사한다.
    FOREACH_CONTAINER(
        const MatchedCompositionInfo::MatchedMaterialInfo& matched_material, 
        matched_material_list, 
        MatchedCompositionInfo::MatchedMaterialList)
    {
        if (!matched_material.is_find_)
        {
            return false;
        }
    }
    
    matched_composition.composition_ = &composition;
    return true;
}

// 일치하는 합성 정보로부터 요청을 설정한다.
static bool SetCompositionRequest(
    const MatchedCompositionList& matched_composition_list, 
    MSG_CG_ITEM_COMPOSE_OR_DECOMPOSE_SYN& OUT request)
{
    if (matched_composition_list.size() == 0)
    {
        return false;
    }

    // 일치한 합성 중 첫번째를 선택한다.
    const MatchedCompositionInfo& matched_composition = matched_composition_list[0];
    const MatchedCompositionInfo::MatchedMaterialList& matched_material_list = 
        matched_composition.matched_material_list_;

    // 합성 코드를 설정한다.
    request.reserved_code_ = (matched_composition.composition_)->code_;

    // 일치한 합성을 기준으로 요청을 만든다.
    MSG_CG_ITEM_COMPOSE_OR_DECOMPOSE_SYN::MaterialListSize request_material_index = 0;
    FOREACH_CONTAINER(
        const MatchedCompositionInfo::MatchedMaterialInfo& matched_material, 
        matched_material_list, 
        MatchedCompositionInfo::MatchedMaterialList)
    {
        // 같은 코드의 입력 재료를 합쳐서 합성 재료를 만족 시켰다면
        if (matched_material.is_mixed_input_material_)
        {
            int same_material_number = 0;

            // 합친 입력 재료에 대해
            CompositionInfo::MaterialList::const_iterator input_material_pos = 
                matched_material.input_material_start_pos_;
            for (; input_material_pos != matched_material.input_material_end_pos_; ++input_material_pos)
            {
                const CompositionInfo::MaterialInfo& input_material = *input_material_pos;
                
                MSG_CG_ITEM_COMPOSE_OR_DECOMPOSE_SYN::MaterialInfo& request_material = 
                    (request.material_list_)[request_material_index];
                {
                    request_material.inventory_position_ = input_material.inventory_position_;
                    request_material.matched_index_ = 
                        (matched_material.composition_material_)->column_number_ - 1;

                    // 마지막 입력 재료라면
                    if (input_material_pos == (matched_material.input_material_end_pos_ - 1))
                    {
                        // 합성 재료에 필요한 개수만큼만 요청 개수로 설정한다.
                        request_material.selected_number_ = 
                            static_cast<BYTE>((matched_material.composition_material_)->number_ - same_material_number);
                    }
                    else
                    {
                        request_material.selected_number_ = input_material.number_;
                        same_material_number += request_material.selected_number_;
                    }
                }
                ++request_material_index;
            }
            continue;
        }

        // 입력 재료와 합성 재료가 1:1로 일치한다.
        MSG_CG_ITEM_COMPOSE_OR_DECOMPOSE_SYN::MaterialInfo& request_material = 
            (request.material_list_)[request_material_index];
        {
            request_material.inventory_position_ = 
                (matched_material.input_material_start_pos_)->inventory_position_;
            request_material.selected_number_ = (matched_material.composition_material_)->number_;
            request_material.matched_index_ = 
                (matched_material.composition_material_)->column_number_ - 1;
        }
        ++request_material_index;
    }

    request.material_list_size_ = request_material_index;
    return true;
}

// 보상의 아이템코드가 같은지 검사한다.
static bool IsSameItemCode(
    const CompositionInfo::RewardInfo& reward, 
    CompositionInfo::RewardInfo::RewardCode reward_code)
{
    if (reward.type_ == CompositionInfo::RewardInfo::kItemCode)
    {
        return reward.code_ == reward_code;
    }
    return false;
}

// 합성 요청을 얻는다.
static bool GetCompositionRequest( 
    const CompositionMap& composition_map, 
    SCItemSlotContainer& inventory, 
    const MaterialInventoryPositionList& inventory_position_list, 
    MSG_CG_ITEM_COMPOSE_OR_DECOMPOSE_SYN& OUT request)
{
    // 인벤토리 아이템으로부터 합성정보를 탐색할 재료 목록을 생성한다.
    CompositionInfo::MaterialList input_material_list;
    if (!GetInputMaterialList(inventory, inventory_position_list, input_material_list))
    {
        return false;
    }

    // 입력 재료에 일치하는 합성 목록을 얻는다.
    MatchedCompositionList matched_composition_list;
    {
        // 합성 목록의 합성에 대해
        MatchedCompositionList::size_type matched_composition_index = 0;
        FOREACH_CONTAINER(
            const CompositionMap::value_type& composition_value, 
            composition_map, 
            CompositionMap)
        {
            // 일치하는 합성 목록이 다 찼다면
            if (matched_composition_index >= _countof(matched_composition_list.elems) || 
                matched_composition_list.full())
            {
                // 찾기를 중지한다.
                break;
            }

            // 일치하는 합성을 찾는다.
            const CompositionInfo& composition = *(composition_value.second);
            MatchedCompositionInfo& matched_composition = 
                matched_composition_list[matched_composition_index];
            if (!GetMatchedComposition(input_material_list, composition, matched_composition))
            {
                continue;
            }

            ++matched_composition_index;
            matched_composition_list.set_size(matched_composition_index);
        }

        // 일치하는 합성을 찾았는지 검사한다.
        if (matched_composition_index == 0)
        {
            return false;
        }
    }

    // 합성 목록을 조건이 더 구체적인 순으로 정렬한다.
    std::stable_sort(
        matched_composition_list.begin(), 
        matched_composition_list.end(), 
        PredicateMatchedComposition);

    return SetCompositionRequest(matched_composition_list, request);
}
} // internal
} // util


////////////////////////////////////////////////////////////////////////////////////////////////////
// REGION : Public
////////////////////////////////////////////////////////////////////////////////////////////////////

void CubeSystemParser::Release()
{
    ReleaseComposition();
}

BOOL CubeSystemParser::LoadScript(eSCRIPT_CODE script_code, BOOL is_reload)
{
    if (script_code == SCRIPT_CUBE_COMPOSE)
    {
        return (LoadComposition(!!is_reload) ? TRUE : FALSE);
    }
    else if (script_code == SCRIPT_CUBE_DECOMPOSE)
    {
        return (LoadDecomposition(!!is_reload) ? TRUE : FALSE);
    }
    return FALSE;
}

CompositionInfo* CubeSystemParser::FindComposition(const CompositionInfo::CompositionCode code) const
{
    const CompositionMap::const_iterator& pos = composition_map_.find(code);
    if (pos != composition_map_.end())
    {
        return pos->second;
    }
    return NULL;
}

// 분해 재료 목록을 얻는다.
bool CubeSystemParser::GetDecompositionMaterialList(
    SCItemSlotContainer& inventory, 
    const MaterialInventoryPositionList& inventory_position_list, 
    DecompositionInfo::MaterialList& OUT material_list) const
{
    DecompositionInfo::MaterialList::size_type material_index = 0;
    
    FOREACH_CONTAINER(
        const POSTYPE inventory_position, 
        inventory_position_list, 
        MaterialInventoryPositionList)
    {
        using namespace util::internal;

        // 인벤토리 위치를 검사한다.
        if (IsValidInventoryPosition(inventory, inventory_position) == false)
        {
            continue;
        }

        // 인벤토리 아이템 상태를 검사한다.
        SCItemSlot& inventory_item = static_cast<SCItemSlot&>(inventory.GetSlot(inventory_position));
        if (IsValidInventoryItemState(inventory_item) == false)
        {
            continue;
        }
        
        // 무기나 방어구인지 검사한다.
        const BASE_ITEMINFO* const inventory_item_info = inventory_item.GetItemInfo();
        if ((inventory_item_info->IsWeapon() == false) && (inventory_item_info->IsArmor() == false))
        {
            continue;
        }

        // 캐시 아이템은 분해 목록에서 제외 시킨다.
        if (inventory_item_info->IsChargeItem() == true)
        {
            continue;
        }

        DecompositionInfo::MaterialInfo& material = material_list[material_index];
        material.enchant_ = inventory_item.GetEnchant();
        material.socket_number_ = inventory_item.GetSocketNum();
        material.item_level_ = inventory_item.GetItemInfo()->m_LV;
        material.number_ = inventory_item.GetNum();
        material.inventory_position_ = inventory_position;
        
        // 소켓 개수를 검사한다.
        if (material.socket_number_ > SOCKET_MAX)
        {
            continue;
        }

        BOOST_STATIC_ASSERT(DecompositionInfo::ZardInfo::kMaxSize >= 4);
        (material.zard_list_[0]).grade_ = DecompositionInfo::ZardInfo::kGradeLv1;
        (material.zard_list_[1]).grade_ = DecompositionInfo::ZardInfo::kGradeLv2;
        (material.zard_list_[2]).grade_ = DecompositionInfo::ZardInfo::kGradeLv3;
        (material.zard_list_[3]).grade_ = DecompositionInfo::ZardInfo::kGradeLv4;
        material.zard_list_.set_size(DecompositionInfo::ZardInfo::kMaxSize);
        
        for (DecompositionInfo::MaterialInfo::SocketNumber socket_index = 0; 
            socket_index < material.socket_number_; 
            ++socket_index)
        {            
#ifdef _NA_000251_20100727_SOCKET_SYSTEM
            const sSOCKET_OPTION* socket_option = 
                inventory_item.GetSocketOption(static_cast<eSOCKET>(socket_index));
            if (socket_option != NULL)
            {
                const eSOCKET_LEVEL zard_grade = socket_option->zard_grade;
                switch (zard_grade)
                {
                case kZardGrade1:
                    (material.zard_list_[0]).number_++;
                    break;
                case kZardGrade2:
                    (material.zard_list_[1]).number_++;
                    break;
                case kZardGrade3:
                    (material.zard_list_[2]).number_++;
                    break;
                case kZardGrade4:
                    (material.zard_list_[3]).number_++;
                    break;
                }
            }
#else // _NA_000251_20100727_SOCKET_SYSTEM
            const eSOCKET_LEVEL socket_level = 
                inventory_item.GetSocketAttrLevel(static_cast<eSOCKET>(socket_index));
            switch (socket_level)
            {
            case SOCKETLV_LOW:
                (material.zard_list_[0]).number_++;
                break;
            case SOCKETLV_HIGH:
                (material.zard_list_[1]).number_++;
                break;
            case SOCKETLV_MAX:
                (material.zard_list_[2]).number_++;
                break;
            }
#endif // _NA_000251_20100727_SOCKET_SYSTEM            
        }

        ++material_index;
    }

    if (material_index == 0)
    {
        return false;
    }

    material_list.set_size(material_index);

    return true;
}

DecompositionInfo::DecompositionPoint CubeSystemParser::GetDecompositionPoint( 
    const DecompositionInfo::MaterialList& material_list) const
{
    const BYTE kEnchantPerPoint = 4; // 인첸트 1당 포인트
    const BYTE kSocketNumberPerPoint = 2; // 소켓 1개당 포인트
    const BYTE kItemLevelPerPoint = 2; // 아이템 레벨 1당 포인트

    const BYTE kGradeLv1ZardPerPoint = 1; // 하급 쟈드 1당 포인트
    const BYTE kGradeLv2ZardPerPoint = 3; // 중급 쟈드 1당 포인트
    const BYTE kGradeLv3ZardPerPoint = 4; // 상급 쟈드 1당 포인트
    const BYTE kGradeLv4ZardPerPoint = 10; // 최상급 쟈드 1당 포인트

    DecompositionInfo::DecompositionPoint decomposition_point = 0;

    // 재료 목록의 재료에 대해
    FOREACH_CONTAINER(
        const DecompositionInfo::MaterialInfo& material, 
        material_list, 
        DecompositionInfo::MaterialList)
    {
        // 인첸트 포인트를 계산한다.
        const DecompositionInfo::DecompositionPoint enchant_point = material.enchant_ * kEnchantPerPoint;

        // 소켓 포인트를 계산한다.
        const DecompositionInfo::DecompositionPoint socket_point = material.socket_number_ * kSocketNumberPerPoint;

        // 아이템 레벨 포인트를 계산한다.
        const DecompositionInfo::DecompositionPoint item_level_point = material.item_level_ * kItemLevelPerPoint;

        // 쟈드 포인트를 계산한다.
        DecompositionInfo::DecompositionPoint zard_point = 0;
        FOREACH_CONTAINER(
            const DecompositionInfo::ZardInfo& zard, 
            material.zard_list_, 
            DecompositionInfo::ZardList)
        {
            if (zard.number_ < 1)
            {
                continue;
            }

            switch (zard.grade_)
            {
            case DecompositionInfo::ZardInfo::kGradeLv1:
                zard_point += (zard.number_ * kGradeLv1ZardPerPoint);
                break;
            case DecompositionInfo::ZardInfo::kGradeLv2:
                zard_point += (zard.number_ * kGradeLv2ZardPerPoint);
                break;
            case DecompositionInfo::ZardInfo::kGradeLv3:
                zard_point += (zard.number_ * kGradeLv3ZardPerPoint);
                break;
            case DecompositionInfo::ZardInfo::kGradeLv4:
                zard_point += (zard.number_ * kGradeLv4ZardPerPoint);
                break;
            }
        }

        // 분해 포인트를 계산한다.
        decomposition_point += ((enchant_point + socket_point + zard_point + 1) * item_level_point);
    }

    return decomposition_point;
}

bool CubeSystemParser::GetDecompositionRewardList(
    const DecompositionInfo::DecompositionPoint decomposition_point, 
    DecompositionInfo::RewardList& OUT reward_list) const
{
    // 입력값을 검사한다.
    if (decomposition_point == 0 || !reward_list.empty())
    {
        return false;
    }

    FOREACH_CONTAINER(
        const DecompositionInfo::RewardInfo& reward, 
        decomposition_reward_list_, 
        DecompositionInfo::RewardList)
    {
        // 반환할 보상 정보를 삽인한 후
        reward_list.push_back(reward);

        // 보상 확률을 계산한다.
        DecompositionInfo::RewardInfo& new_reward = reward_list.back();
        new_reward.ratio_ = reward.ratio_ * decomposition_point;
    }

    return true;
}

bool CubeSystemParser::GetRequest(
    SCItemSlotContainer& inventory, 
    const bool is_composition, 
    const MaterialInventoryPositionList& inventory_position_list, 
    const POSTYPE using_item_position, 
    MSG_CG_ITEM_COMPOSE_OR_DECOMPOSE_SYN& OUT request) const
{
    // 인벤토리에 여유 공간이 없으면 시도할 필요가 없다.
    if (inventory.GetEmptySlotNum() == 0)
    {
        return false;
    }

    request.request_type_ = MSG_CG_ITEM_COMPOSE_OR_DECOMPOSE_SYN::kCubeRequest;
    request.is_composition_ = is_composition;

    // 큐브 시스템은 아이템을 사용해서 실행하기 때문에 큐브 아이템을 검사한다.
    {
        if (!util::internal::IsValidInventoryPosition(inventory, using_item_position))
        {
            return false;
        }
        
        SCItemSlot& using_item = static_cast<SCItemSlot&>(inventory.GetSlot(using_item_position));
        if (!util::internal::IsValidInventoryItemState(using_item))
        {
            return false;
        }

        if ((using_item.GetItemInfo())->m_wType != eITEMTYPE_CUBE)
        {
            return false;
        }

        request.using_item_position_ = using_item_position;
    }
    
    // 합성 요청이라면
    if (request.is_composition_)
    {
        return util::internal::GetCompositionRequest(
            composition_map_, inventory, inventory_position_list, request);        
    }
    
    // 분해할 재료 목록을 얻는다.
    DecompositionInfo::MaterialList material_list;
    if (!GetDecompositionMaterialList(inventory, inventory_position_list, material_list))
    {
        return false;
    }

    // 요청할 재료를 설정한다.
    MSG_CG_ITEM_COMPOSE_OR_DECOMPOSE_SYN::MaterialListSize material_index = 0;
    FOREACH_CONTAINER(
        const DecompositionInfo::MaterialInfo& material, 
        material_list, 
        DecompositionInfo::MaterialList)
    {
        MSG_CG_ITEM_COMPOSE_OR_DECOMPOSE_SYN::MaterialInfo& request_material = 
            request.material_list_[material_index];
        request_material.inventory_position_ = material.inventory_position_;
        request_material.selected_number_ = material.number_;
        request_material.matched_index_ = 0;
        ++material_index;
    }
    request.material_list_size_ = material_index;

    // 분해 포인트를 얻는다.
    const DecompositionInfo::DecompositionPoint decomposition_point = 
        GetDecompositionPoint(material_list);
    if (decomposition_point == 0)
    {
        return false;
    }
    request.reserved_code_ = decomposition_point;

    return true;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// REGION : Private
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CubeSystemParser::LoadComposition(const bool is_reload)
{
    // 스크립트를 다시 읽는다면
    if (is_reload)
    {
        // 이전 합성 정보를 제거한다.
        ReleaseComposition();
    }

    bool is_valid_script = true;

    const int row_size = GetRowSize();
    for (int row = 0; row < row_size; row++)
    {
        // 합성 코드를 읽어와서 합성 정보를 찾는다.
        const CompositionInfo::CompositionCode composition_code = GetDataDWORD(_T("Code"), row);
        CompositionInfo* composition = FindComposition(composition_code);

        // 합성 정보를 찾을 수 없으면
        if (composition == NULL)
        {
            // 새로운 합성 정보를 생성한다.
            composition = new CompositionInfo(composition_code);
            composition_map_[composition_code] = composition;
        }

        // 칼럼 개수를 검사한다.
        const CompositionInfo::ColumnNumber column_number = GetDataBYTE(_T("ColumnNumber"), row);
        if (column_number < 1 || 
            column_number > max(CompositionInfo::MaterialInfo::kMaxSize, CompositionInfo::RewardInfo::kMaxSize))
        {
            SUNLOG(eCRITICAL_LOG, 
                   _T("|%s|ColumnNumber(칼럼번호)는 1~%d 사이의 숫자로 입력해야 합니다.|현재줄 = %d, ColumnNumber = %d|"), 
                   __FUNCTION__, 
                   max(CompositionInfo::MaterialInfo::kMaxSize, CompositionInfo::RewardInfo::kMaxSize), 
                   row, 
                   column_number);
            is_valid_script = false;
            continue;
        }
        const CompositionInfo::ColumnNumber material_or_reward_index = column_number-1;

        // 재료 칼럼을 사용한다면
        const int use_material = GetDataInt(_T("UseMaterial"), row);
        if (use_material == CompositionInfo::kUsed)
        {
            CompositionInfo::MaterialList& material_list = composition->material_list_;
            CompositionInfo::MaterialInfo& material = material_list[material_or_reward_index];

            // 재료가 이미 설정되었다면
            if (material.is_used_)
            {
                // 중복된 재료 칼럼이다.
                SUNLOG(eCRITICAL_LOG, 
                       _T("|%s|같은 합성정보안에서 재료의 ColumnNumber(칼럼번호)는 중복 될 수 없습니다.|현재줄 = %d, ColumnNumber = %d|"), 
                       __FUNCTION__, 
                       row, 
                       column_number);
                is_valid_script = false;
                continue;
            }

            // 재료 개수를 검사한다.
            const int material_number = GetDataInt(_T("MaterialNumber"), row);
            if (material_number < 1 || material_number > UCHAR_MAX)
            {
                SUNLOG(eCRITICAL_LOG, 
                       _T("|%s|MaterialNumber(재료개수)가 올바르지 않습니다.|현재줄 = %d, 재료개수 = %d|"), 
                       __FUNCTION__, 
                       row, 
                       material_number);
                is_valid_script = false;
                continue;
            }

            // 재료 정보를 읽어온다.
            material.is_used_ = true;
            material.column_number_ = column_number;
            material.code_ = GetDataDWORD(_T("MaterialCode"), row);
            material.type_code_ = GetDataDWORD(_T("MaterialTypeCode"), row);
            material.level_ = GetDataWORD(_T("MaterialLevel"), row);
            material.number_ = static_cast<CompositionInfo::MaterialInfo::MaterialNumber>(material_number);
            material.ig_type_ = GetDataBYTE(_T("MaterialIGType"), row);

            // 읽어온 재료의 조건들 중 최소한 하나는 사용해야 한다.
            if (material.code_ == CompositionInfo::kUnused && 
                material.type_code_ == CompositionInfo::kUnused && 
                material.level_ == CompositionInfo::kUnused)
            {
                SUNLOG(eCRITICAL_LOG, 
                       _T("|%s|MaterialCode(재료코드),MaterialTypeCode(재료종류코드),MaterialLevel(재료레벨) 중에 최소한 하나는 입력해야 합니다.|현재줄 = %d|"), 
                       __FUNCTION__, 
                       row);
                is_valid_script = false;
                continue;
            }

            // 재료 아이템 타입 범위를 검사한다.
            if (material.ig_type_ < KindOfItemType::Default || 
                material.ig_type_ >= KindOfItemType::Counts)
            {
                SUNLOG(eCRITICAL_LOG, 
                       _T("|%s|MaterialIGType(재료아이템타입) 값의 범위가 옳바르지 않습니다.|현재줄 = %d, MaterialIGType = %d|"), 
                       __FUNCTION__, 
                       row, 
                       material.ig_type_);
                is_valid_script = false;
                continue;
            }

            // 가중치를 계산한다. (코드 > 타입 코드 > 레벨 > 개수)
            {
                enum
                {
                    kMaterialNumberWeight = 1, 
                    kMaterialLevelWeight, 
                    kMaterialTypeWeight, 
                    kMaterialCodeWeight
                };

                if (material.code_ != CompositionInfo::kUnused)
                {
                    composition->weight_ += kMaterialCodeWeight;
                }

                if (material.type_code_ != CompositionInfo::kUnused)
                {
                    composition->weight_ += kMaterialTypeWeight;
                }

                if (material.level_ != CompositionInfo::kUnused)
                {
                    composition->weight_ += kMaterialLevelWeight;
                }

                composition->weight_ += (material.number_ * kMaterialNumberWeight);
            }
            
            // 칼럼 번호가 목록 크기보다 크다면 새로 삽입된 것이므로 
            if (static_cast<CompositionInfo::MaterialList::size_type>(column_number) > material_list.size())
            {
                // 목록의 크기를 새로 설정한다.
                material_list.set_size(column_number);
            }
        }

        // 보상 칼럼을 사용한다면
        const int use_reward = GetDataInt(_T("UseReward"), row);
        if (use_reward == CompositionInfo::kUsed)
        {
            CompositionInfo::RewardList& reward_list = composition->reward_list_;
            CompositionInfo::RewardInfo& reward = reward_list[material_or_reward_index];

            // 보상이 이미 설정되었다면
            if (reward.is_used_)
            {
                // 중복된 보상 칼럼이다.
                SUNLOG(eCRITICAL_LOG, 
                       _T("|%s|같은 합성정보안에서 보상의 ColumnNumber(칼럼번호)는 중복 될 수 없습니다.|현재줄 = %d, ColumnNumber = %d|"), 
                       __FUNCTION__, 
                       row, 
                       column_number);
                is_valid_script = false;
                continue;
            }

            // 보상 개수를 검사한다.
            const int reward_number = GetDataInt(_T("RewardNumber"), row);
            if (reward_number > UCHAR_MAX)
            {
                SUNLOG(eCRITICAL_LOG, 
                       _T("|%s|RewardNumber(보상개수)가 올바르지 않습니다.|현재줄 = %d, 보상개수 = %d|"), 
                       __FUNCTION__, 
                       row, 
                       reward_number);
                is_valid_script = false;
                continue;
            }

            // 보상 정보를 읽어온다.
            reward.is_used_ = true;
            reward.column_number_ = column_number;
            reward.type_ = GetDataInt(_T("RewardType"), row);
            reward.code_ = GetDataDWORD(_T("RewardCode"), row);
            reward.ig_type_ = GetDataBYTE(_T("RewardIGType"), row);
            reward.ratio_ = GetDataWORD(_T("RewardRatio"), row);
            reward.number_ = static_cast<CompositionInfo::RewardInfo::RewardNumber>(reward_number);

            // 보상종류를 검사한다.
            if (reward.type_ != CompositionInfo::RewardInfo::kItemCode && 
                reward.type_ != CompositionInfo::RewardInfo::kRewardCode)
            {
                SUNLOG(eCRITICAL_LOG, 
                       _T("|%s|RewardType(보상종류)은 아이템코드(%d)이거나 보상코드(%d)이어야 합니다.|현재줄 = %d, RewardType = %d|"), 
                       __FUNCTION__, 
                       CompositionInfo::RewardInfo::kItemCode, 
                       CompositionInfo::RewardInfo::kRewardCode, 
                       row, 
                       reward.type_);
                is_valid_script = false;
                continue;
            }

            // 보상코드는 보상 종류에 상관없이 반드시 입력 되어야 한다.
            if (reward.code_ == CompositionInfo::kUnused)
            {
                SUNLOG(eCRITICAL_LOG, 
                       _T("|%s|RewardCode(보상코드)는 RewardType(보상종류)에 해당하는 값을 반드시 입력해야 합니다.|현재줄 = %d|"), 
                       __FUNCTION__, 
                       row);
                is_valid_script = false;
                continue;
            }

            // 보상 아이템 타입 범위를 검사한다.
            if (reward.ig_type_ < KindOfItemType::Default || 
                reward.ig_type_ >= KindOfItemType::Counts)
            {
                SUNLOG(eCRITICAL_LOG, 
                       _T("|%s|RewardIGType(보상아이템타입) 값의 범위가 옳바르지 않습니다.|현재줄 = %d, RewardIGType = %d|"), 
                       __FUNCTION__, 
                       row, 
                       reward.ig_type_);
                is_valid_script = false;
                continue;
            }

            // 보상종류가 아이템코드일때
            if (reward.type_ == CompositionInfo::RewardInfo::kItemCode)
            {
                // 보상 확률은 0보다 작을 수 없다.
                if (reward.ratio_ < 0 || reward.ratio_ > CompositionInfo::RewardInfo::kMaxRewardRatioSize)
                {
                    SUNLOG(eCRITICAL_LOG, 
                           _T("|%s|RewardType(보상종류)가 아이템코드(%d)일때 RewardRatio(보상확률)은 0~%d 사이의 값이어야 합니다.|현재줄 = %d|"), 
                           __FUNCTION__, 
                           CompositionInfo::RewardInfo::kItemCode, 
                           CompositionInfo::RewardInfo::kMaxRewardRatioSize, 
                           row);
                    is_valid_script = false;
                    continue;
                }

                // 보상 개수는 0보다 커야 한다.
                if (reward.number_ == 0)
                {
                    SUNLOG(eCRITICAL_LOG, 
                           _T("|%s|RewardType(보상종류)가 아이템코드(%d)일때 RewardNumber(보상개수)는 0보다 커야 합니다.|현재줄 = %d|"), 
                           __FUNCTION__, 
                           CompositionInfo::RewardInfo::kItemCode, 
                           row);
                    is_valid_script = false;
                    continue;
                }
            }

            // 보상종류가 보상코드일때
            if (reward.type_ == CompositionInfo::RewardInfo::kRewardCode)
            {
                // 보상확률은 0이 아니라면
                if (reward.ratio_ != 0.0)
                {
                    // 보정 가능하므로 보정한다.
                    SUNLOG(eCRITICAL_LOG, 
                           _T("|%s|RewardType(보상종류)가 보상코드(%d)일때 RewardRatio(보상확률)는 0이어야 합니다.|현재줄 = %d|"), 
                           __FUNCTION__, 
                           CompositionInfo::RewardInfo::kRewardCode, 
                           row);
                    reward.ratio_ = 0;
                }

                // 보상개수가 0이 아니라면
                if (reward.number_ != 0)
                {
                    // 보정 가능하므로 보정한다.
                    SUNLOG(eCRITICAL_LOG, 
                           _T("|%s|RewardType(보상종류)가 보상코드(%d)일때 RewardNumber(보상개수)는 0이어야 합니다.|현재줄 = %d|"), 
                           __FUNCTION__, 
                           CompositionInfo::RewardInfo::kRewardCode, 
                           row);
                    reward.number_ = 0;
                }
            }

            // 칼럼 번호가 목록 크기보다 크다면 새로 삽입된 것이므로 
            if (static_cast<CompositionInfo::MaterialList::size_type>(column_number) > reward_list.size())
            {
                // 목록의 크기를 새로 설정한다.
                reward_list.set_size(column_number);
            }
        }
    } // for (int row = 0; row < row_size; row++)

    // 스크립트를 미리 검증하자.
    // 보상 종류가 보상코드라면 복수의 보상 의미를 내포하고 있으므로 보상 코드일때는 오직 하나만 허용한다.
    // 하지만 보상 종류가 아이템코드라면 복수의 아이템코드를 허용한다.
    // 보상코드와 아이템코드의 조합은 허용하지 않는다.

    // 각 합성에 대해
    FOREACH_CONTAINER(
        const CompositionMap::value_type& composition_value, composition_map_, CompositionMap)
    {
        const CompositionInfo* const composition = composition_value.second;

        // 합성 정보의 재료 개수가 최소 1개는 필요하다.
        if ((composition->material_list_).size() == 0)
        {
            SUNLOG(eCRITICAL_LOG, 
                   _T("|%s|합성 정보안에 재료가 최소한 하나 이상 필요합니다.|CompositionCode = %d|"), 
                   __FUNCTION__, 
                   composition->code_);
            is_valid_script = false;
            continue;
        }
        
        // 읽어들인 재료 목록을 검사한다.
        FOREACH_CONTAINER(
            const CompositionInfo::MaterialInfo& material, 
            composition->material_list_, 
            CompositionInfo::MaterialList)
        {
            // 재료가 사용 가능한지 검사한다.
            if (!material.is_used_)
            {
                SUNLOG(eCRITICAL_LOG, 
                       _T("|%s|재료가 사용안함으로 설정되어 있습니다.|CompositionCode = %d|ColumnNumber = %d|"), 
                       __FUNCTION__, 
                       composition->code_, 
                       material.column_number_);
                is_valid_script = false;
                break;
            }
        }

        // 읽어들인 보상 목록을 검사한다.
        bool is_find_reward_code = false;
        FOREACH_CONTAINER(
            const CompositionInfo::RewardInfo& reward, 
            composition->reward_list_, 
            CompositionInfo::RewardList)
        {
            // 보상이 사용 가능한지 검사한다.
            if (!reward.is_used_)
            {
                SUNLOG(eCRITICAL_LOG, 
                       _T("|%s|보상이 사용안함으로 설정되어 있습니다.|CompositionCode = %d|ColumnNumber = %d|"), 
                       __FUNCTION__, 
                       composition->code_, 
                       reward.column_number_);
                is_valid_script = false;
                break;
            }

            // 이미 보상코드가 하나 발견되었는데 
            if (is_find_reward_code)
            {
                // 다른 종류의 보상이 또 발견되었다.
                SUNLOG(eCRITICAL_LOG, 
                       _T("|%s|RewardType(보상종류)가 보상코드로 설정되어 있을 때는 오직 하나의 보상만 입력 가능합니다.|CompositionCode = %d|"), 
                       __FUNCTION__, 
                       composition->code_);
                is_valid_script = false;
                break;
            }

            if (reward.type_ == CompositionInfo::RewardInfo::kRewardCode)
            {
                is_find_reward_code = true;
            }

            // 보상의 아이템 코드가 보상목록에서 유일한지 검사한다.
            const CompositionInfo::RewardList::difference_type same_item_code_count = \
                std::count_if(composition->reward_list_.begin(),
                              composition->reward_list_.end(), 
                              boost::bind(&util::internal::IsSameItemCode, _1, reward.code_));
            if (same_item_code_count > 1)
            {
                SUNLOG(eCRITICAL_LOG, 
                       _T("|%s|보상 목록에서 아이템 코드는 유일해야 합니다.|중복개수 = %d|"), 
                       __FUNCTION__, 
                       same_item_code_count);
                is_valid_script = false;
                break;
            }
        }

        // 보상 코드가 발견되었는데 보상이 하나가 아니다.
        if (is_find_reward_code && (composition->reward_list_).size() != 1)
        {
            SUNLOG(eCRITICAL_LOG, 
                   _T("|%s|RewardType(보상종류)가 보상코드로 설정되어 있을 때는 오직 하나의 보상만 입력 가능합니다.|CompositionCode = %d|"), 
                   __FUNCTION__, 
                   composition->code_);
            is_valid_script = false;
        }
    } // 각 합성에 대해

    FASSERT(is_valid_script && _T("스크립트 데이터에 오류가 있습니다."));
    return is_valid_script;
}

bool CubeSystemParser::LoadDecomposition(const bool is_reload)
{
    if (is_reload)
    {
        decomposition_reward_list_.clear();
    }

    bool is_valid_script = true;

    const int row_size = GetRowSize();
    for (int row = 0; row < row_size; row++)
    {
        // 보상 정보를 읽어온다.
        DecompositionInfo::RewardInfo reward;
        {
            reward.type_ = GetDataInt(_T("DecRewardType"), row);
            reward.code_ = GetDataDWORD(_T("DecRewardCode"), row);
            reward.min_code_ = GetDataDWORD(_T("DecMinCode"), row);
            reward.max_code_ = GetDataDWORD(_T("DecMaxCode"), row);
            reward.ratio_ = GetDataWORD(_T("DecRewardRatio"), row);
        }

        // 보상종류를 검사한다.
        if (reward.type_ != DecompositionInfo::RewardInfo::kItemCode && 
            reward.type_ != DecompositionInfo::RewardInfo::kZard)
        {
            SUNLOG(eCRITICAL_LOG, 
                   _T("|%s|DecRewardType(보상종류)은 아이템코드(%d)이거나 쟈드(%d) 이어야 합니다.|현재줄 = %d, DecRewardType = %d|"), 
                   __FUNCTION__, 
                   DecompositionInfo::RewardInfo::kItemCode, 
                   DecompositionInfo::RewardInfo::kZard, 
                   row, 
                   reward.type_);
            is_valid_script = false;
            continue;
        }

        // 보상코드를 검사한다.
        if (reward.code_ == DecompositionInfo::kUnused)
        {
            SUNLOG(eCRITICAL_LOG, 
                   _T("|%s|DecRewardCode(보상코드)에는 반드시 DecRewardType(보상종류)에 해당하는 코드를 입력해야 합니다.|현재줄 = %d|"), 
                   __FUNCTION__, 
                   row);
            is_valid_script = false;
            continue;
        }

        // 보상확률을 검사한다.
        if (reward.ratio_ <= 0 || reward.ratio_ > DecompositionInfo::RewardInfo::kMaxRewardRatioSize)
        {
            SUNLOG(eCRITICAL_LOG, 
                   _T("|%s|DecRewardRatio(보상확률)는 1~%d 사이의 값이어야 합니다.|현재줄 = %d|"), 
                   __FUNCTION__, 
                   DecompositionInfo::RewardInfo::kMaxRewardRatioSize, 
                   row);
            is_valid_script = false;
            continue;
        }
        
        // 보상종류가 쟈드일때
        if (reward.type_ == DecompositionInfo::RewardInfo::kZard)
        {
            // 보상코드에 쟈드등급이 들어갔는지 검사한다.
            if (reward.code_ != DecompositionInfo::ZardInfo::kGradeLv1 && 
                reward.code_ != DecompositionInfo::ZardInfo::kGradeLv2 && 
                reward.code_ != DecompositionInfo::ZardInfo::kGradeLv3 && 
                reward.code_ != DecompositionInfo::ZardInfo::kGradeLv4)
            {
                SUNLOG(eCRITICAL_LOG, 
                       _T("|%s|옳바른 쟈드 등급을 입력해야 합니다.|현재줄 = %d, DecRewardCode = %d|"), 
                       __FUNCTION__, 
                       row, 
                       reward.code_);
                is_valid_script = false;
                continue;
            }

            // 쟈드인 아이템의 최소 코드 범위가 입력되었는지 검사한다.
            if (reward.min_code_ == DecompositionInfo::kUnused)
            {
                SUNLOG(eCRITICAL_LOG, 
                       _T("|%s|DecMinCode 에는 쟈드 등급이 %d인 아이템들의 최소 아이템코드 범위를 입력해야 합니다.|현재줄 = %d|"), 
                       __FUNCTION__, 
                       reward.code_, 
                       row);
                is_valid_script = false;
                continue;
            }

            // 쟈드인 아이템의 최대 코드 범위가 입력되었는지 검사한다. 
            if (reward.max_code_ == DecompositionInfo::kUnused)
            {
                SUNLOG(eCRITICAL_LOG, 
                       _T("|%s|DecMaxCode 에는 쟈드 등급이 %d인 아이템들의 최대 아이템코드 범위를 입력해야 합니다.|현재줄 = %d|"), 
                       __FUNCTION__, 
                       reward.code_, 
                       row);
                is_valid_script = false;
                continue;
            }

            // 최소 보상코드는 최대 보상 코드보다 클 수 없다.
            if (reward.min_code_ > reward.max_code_)
            {
                SUNLOG(eCRITICAL_LOG, 
                       _T("|%s|DecMinCode는 DecMaxCode 보다 클 수 없습니다.|DecMinCode = %d, DecMaxCode = %d, 현재줄 = %d|"), 
                       __FUNCTION__, 
                       reward.min_code_, 
                       reward.max_code_, 
                       row);
                is_valid_script = false;
                continue;
            }

            // 쟈드 보상 범위를 검사해서 유효한 범위만 얻는다.
            const ItemInfoParser* const item_info_parser = ItemInfoParser::Instance();
            for (DecompositionInfo::RewardInfo::RewardCode zard_code = reward.min_code_; 
                zard_code <= reward.max_code_; 
                ++zard_code)
            {
                const BASE_ITEMINFO* const zard = item_info_parser->GetItemInfo(zard_code);
                if (zard == NULL)
                {
                    continue;
                }
                if (zard->m_wType != eITEMTYPE_SOCKET_ZARD)
                {
                    continue;
                }
                #ifdef _NA_006251_CUBESYSTEM_Decomposition_REWARD_REMOVE_CASH_ZARD
                if (zard->m_ChargeType == 1) // 캐시아이템일경우 ChargeType을 1로 설정
                {
                    // 캐시쟈드 아이템은 보상리스트에 포함시키지 않는다.
                    continue;
                }
                #endif // _NA_006251_CUBESYSTEM_Decomposition_REWARD_REMOVE_CASH_ZARD
                (reward.zard_list).push_back(zard_code);
            }
            if ((reward.zard_list).empty())
            {
                SUNLOG(eCRITICAL_LOG, 
                       _T("|%s|쟈드 보상 코드 범위가 유효하지 않습니다.|DecMinCode = %d, DecMaxCode = %d, 현재줄 = %d|"), 
                       __FUNCTION__, 
                       reward.min_code_, 
                       reward.max_code_, 
                       row);
                is_valid_script = false;
                continue;
            }
        }
        
        // 보상 목록에 삽입한다.
        decomposition_reward_list_.push_back(reward);
    }

    FASSERT(is_valid_script && _T("스크립트 데이터에 오류가 있습니다."));
    return is_valid_script;
}

void CubeSystemParser::ReleaseComposition()
{
    FOREACH_CONTAINER(
        const CompositionMap::value_type& composition_value, 
        composition_map_, 
        CompositionMap)
    {
        CompositionInfo* composition = composition_value.second;
        SAFE_DELETE(composition);
    }
    composition_map_.clear();
}
#endif // _NA_002253_20100811_CUBE_SYSTEM