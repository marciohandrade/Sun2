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
// �Է� ��� ����� �����Ѵ�.
static bool PredicateInputMaterial(
    const CompositionInfo::MaterialInfo& prev_material, 
    const CompositionInfo::MaterialInfo& next_material)
{
    // �ڵ� ������ �����Ѵ�.
    if (prev_material.code_ != next_material.code_)
    {
        return prev_material.code_ < next_material.code_;
    }

    // Ÿ�� ������ �����Ѵ�.
    if (prev_material.type_code_ != next_material.type_code_)
    {
        return prev_material.type_code_ < next_material.type_code_;
    }

    // ���� ������ �����Ѵ�.
    if (prev_material.level_ != next_material.level_)
    {
        return prev_material.level_ < next_material.level_;
    }

    // ���� ������ �����Ѵ�.
    return prev_material.number_ < next_material.number_;
}

// ��ȿ���� ���� �κ��丮 ��ġ�ΰ�?
static bool IsValidInventoryPosition(const SCItemSlotContainer& inventory, const POSTYPE inventory_position)
{
    return (inventory_position >= 0 && inventory_position < inventory.GetMaxSlotNum());
}

// ��ȿ���� ���� �κ��丮 �������ΰ�?
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

// �Է� ��� ����� ��´�.
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

    // �Է��� ��Ḧ �ڵ�/Ÿ��/����/���� ������ �����Ѵ�.
    std::stable_sort(input_material_list.begin(), input_material_list.end(), PredicateInputMaterial);

    return true;
}

// ��ġ�ϴ� �ռ��� �����Ѵ�.
static bool PredicateMatchedComposition(
    const MatchedCompositionInfo& prev_composition, 
    const MatchedCompositionInfo& next_composition)
{
    return (prev_composition.composition_->weight_) > (next_composition.composition_->weight_);
}

// ��ġ�ϴ� �ռ��� ��´�.
static bool GetMatchedComposition(
    const CompositionInfo::MaterialList& input_material_list, 
    const CompositionInfo& composition, 
    MatchedCompositionInfo& OUT matched_composition)
{
    // �ռ� ��� ������ �Է� ��� �������� ���ٸ�
    if ((composition.material_list_).size() > input_material_list.size())
    {
        // ��ġ�� �� ����.
        return false;
    }

    // ��ġ�ϴ� ��� ��� ũ�⸦ �ʱ�ȭ�ϰ� �ռ� ��� ��� ũ��� �����Ѵ�.
    MatchedCompositionInfo::MatchedMaterialList& matched_material_list = matched_composition.matched_material_list_;
    FOREACH_CONTAINER(
        MatchedCompositionInfo::MatchedMaterialInfo& matched_material, 
        matched_material_list, 
        MatchedCompositionInfo::MatchedMaterialList)
    {
        matched_material.Clear();
    }
    matched_material_list.set_size((composition.material_list_).size());

    // �Է� ��ῡ ����
    CompositionInfo::MaterialList::const_iterator input_material_pos = input_material_list.begin();
    while (input_material_pos != input_material_list.end())
    {
        const CompositionInfo::MaterialInfo& input_material = *input_material_pos;
        MatchedCompositionInfo::MatchedMaterialList::iterator matched_material_pos = 
            matched_material_list.begin();
        
        // �ռ� ��ῡ ����
        FOREACH_CONTAINER(
            const CompositionInfo::MaterialInfo& composition_material, 
            composition.material_list_, 
            CompositionInfo::MaterialList)
        {
            MatchedCompositionInfo::MatchedMaterialInfo& matched_material = *matched_material_pos;
            ++matched_material_pos;

            // �̹� ��ġ�ϴ� ��Ḧ ã�Ҵٸ�
            if (matched_material.is_find_)
            {
                // ���� ��Ḧ �˻��Ѵ�.
                continue;
            }

            // ��� �ڵ尡 ��ġ�ϴ��� �˻��Ѵ�.
            if (composition_material.code_ != CompositionInfo::kUnused)
            {
                if (composition_material.code_ != input_material.code_)
                {
                    continue;
                }
            }

            // ��� ������ ��ġ�ϴ��� �˻��Ѵ�.
            if (composition_material.type_code_ != CompositionInfo::kUnused)
            {
                if (composition_material.type_code_ != input_material.type_code_)
                {
                    continue;
                }
            }

            // ��� ������ ��ġ�ϴ��� �˻��Ѵ�.
            if (composition_material.level_ != CompositionInfo::kUnused)
            {
                if (composition_material.level_ != input_material.level_)
                {
                    continue;
                }
            }

            // ��� ������ Ÿ���� ��ġ�ϴ��� �˻��Ѵ�.
            if (composition_material.ig_type_ != input_material.ig_type_)
            {
                continue;
            }

            matched_material.input_material_start_pos_ = &input_material;
            matched_material.is_mixed_input_material_ = false;

            // �ٸ� ������ �����ϴµ� �Է� ��� ������ �����ϴٸ�
            if (composition_material.number_ > input_material.number_)
            {
                // �ڵ尡 ���� �Է� ������ ������ ���ļ� �ռ� ��� ������ ������ų �� �ִ��� �˻��Ѵ�.
                int same_material_number = input_material.number_;                        
                CompositionInfo::MaterialList::const_iterator temp_input_material_pos = &input_material;
                while (++temp_input_material_pos != input_material_list.end())
                {
                    // ���� �Է� ��ᰡ ���� �Է� ���� ���� �ڵ����� �˻��Ѵ�.
                    const CompositionInfo::MaterialInfo& next_input_material = *temp_input_material_pos;
                    if (next_input_material.code_ != input_material.code_)
                    {
                        break;
                    }

                    // ���� �Է� ��ᰡ ���� �ڵ��� ����� ������ ���ؼ� �ٽ� �˻��Ѵ�.
                    same_material_number += next_input_material.number_;
                    if (composition_material.number_ > same_material_number)
                    {
                        continue;
                    }

                    // ������� �Դٸ� ���� �ڵ��� �Է� ��Ḧ ��Ƽ� �ռ� ��� �ϳ��� ���� ��Ų��.
                    matched_material.input_material_end_pos_ = temp_input_material_pos + 1;
                    matched_material.is_mixed_input_material_ = true;
                    break;
                }

                // ���� �ڵ��� ��Ḧ ���ĵ� ������ �����ϸ�
                if (!matched_material.is_mixed_input_material_)
                {
                    // ���� ��Ḧ �˻��Ѵ�.
                    continue;
                }

                // ���� �ڵ��� �Է� ��Ḧ ���ļ� ��ġ�ϴ� ��� ã�⿡ �����ߴٸ� 
                // �Է� ����� �ݺ��ڸ� ��ģ ���� ��ŭ �������� �ش�.
                input_material_pos = temp_input_material_pos;
            }

            // ��ġ�ϴ� ��Ḧ �߰������� �����Ѵ�.
            matched_material.composition_material_ = &composition_material;
            matched_material.is_find_ = true;            
        } // �ռ� ��ῡ ����

        ++input_material_pos;
    } // �Է� ��ῡ ����
    
    // ��� ��ġ�ϴ� ��Ḧ ã�Ҵ��� �˻��Ѵ�.
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

// ��ġ�ϴ� �ռ� �����κ��� ��û�� �����Ѵ�.
static bool SetCompositionRequest(
    const MatchedCompositionList& matched_composition_list, 
    MSG_CG_ITEM_COMPOSE_OR_DECOMPOSE_SYN& OUT request)
{
    if (matched_composition_list.size() == 0)
    {
        return false;
    }

    // ��ġ�� �ռ� �� ù��°�� �����Ѵ�.
    const MatchedCompositionInfo& matched_composition = matched_composition_list[0];
    const MatchedCompositionInfo::MatchedMaterialList& matched_material_list = 
        matched_composition.matched_material_list_;

    // �ռ� �ڵ带 �����Ѵ�.
    request.reserved_code_ = (matched_composition.composition_)->code_;

    // ��ġ�� �ռ��� �������� ��û�� �����.
    MSG_CG_ITEM_COMPOSE_OR_DECOMPOSE_SYN::MaterialListSize request_material_index = 0;
    FOREACH_CONTAINER(
        const MatchedCompositionInfo::MatchedMaterialInfo& matched_material, 
        matched_material_list, 
        MatchedCompositionInfo::MatchedMaterialList)
    {
        // ���� �ڵ��� �Է� ��Ḧ ���ļ� �ռ� ��Ḧ ���� ���״ٸ�
        if (matched_material.is_mixed_input_material_)
        {
            int same_material_number = 0;

            // ��ģ �Է� ��ῡ ����
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

                    // ������ �Է� �����
                    if (input_material_pos == (matched_material.input_material_end_pos_ - 1))
                    {
                        // �ռ� ��ῡ �ʿ��� ������ŭ�� ��û ������ �����Ѵ�.
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

        // �Է� ���� �ռ� ��ᰡ 1:1�� ��ġ�Ѵ�.
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

// ������ �������ڵ尡 ������ �˻��Ѵ�.
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

// �ռ� ��û�� ��´�.
static bool GetCompositionRequest( 
    const CompositionMap& composition_map, 
    SCItemSlotContainer& inventory, 
    const MaterialInventoryPositionList& inventory_position_list, 
    MSG_CG_ITEM_COMPOSE_OR_DECOMPOSE_SYN& OUT request)
{
    // �κ��丮 ���������κ��� �ռ������� Ž���� ��� ����� �����Ѵ�.
    CompositionInfo::MaterialList input_material_list;
    if (!GetInputMaterialList(inventory, inventory_position_list, input_material_list))
    {
        return false;
    }

    // �Է� ��ῡ ��ġ�ϴ� �ռ� ����� ��´�.
    MatchedCompositionList matched_composition_list;
    {
        // �ռ� ����� �ռ��� ����
        MatchedCompositionList::size_type matched_composition_index = 0;
        FOREACH_CONTAINER(
            const CompositionMap::value_type& composition_value, 
            composition_map, 
            CompositionMap)
        {
            // ��ġ�ϴ� �ռ� ����� �� á�ٸ�
            if (matched_composition_index >= _countof(matched_composition_list.elems) || 
                matched_composition_list.full())
            {
                // ã�⸦ �����Ѵ�.
                break;
            }

            // ��ġ�ϴ� �ռ��� ã�´�.
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

        // ��ġ�ϴ� �ռ��� ã�Ҵ��� �˻��Ѵ�.
        if (matched_composition_index == 0)
        {
            return false;
        }
    }

    // �ռ� ����� ������ �� ��ü���� ������ �����Ѵ�.
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

// ���� ��� ����� ��´�.
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

        // �κ��丮 ��ġ�� �˻��Ѵ�.
        if (IsValidInventoryPosition(inventory, inventory_position) == false)
        {
            continue;
        }

        // �κ��丮 ������ ���¸� �˻��Ѵ�.
        SCItemSlot& inventory_item = static_cast<SCItemSlot&>(inventory.GetSlot(inventory_position));
        if (IsValidInventoryItemState(inventory_item) == false)
        {
            continue;
        }
        
        // ���⳪ ������ �˻��Ѵ�.
        const BASE_ITEMINFO* const inventory_item_info = inventory_item.GetItemInfo();
        if ((inventory_item_info->IsWeapon() == false) && (inventory_item_info->IsArmor() == false))
        {
            continue;
        }

        // ĳ�� �������� ���� ��Ͽ��� ���� ��Ų��.
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
        
        // ���� ������ �˻��Ѵ�.
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
    const BYTE kEnchantPerPoint = 4; // ��þƮ 1�� ����Ʈ
    const BYTE kSocketNumberPerPoint = 2; // ���� 1���� ����Ʈ
    const BYTE kItemLevelPerPoint = 2; // ������ ���� 1�� ����Ʈ

    const BYTE kGradeLv1ZardPerPoint = 1; // �ϱ� ��� 1�� ����Ʈ
    const BYTE kGradeLv2ZardPerPoint = 3; // �߱� ��� 1�� ����Ʈ
    const BYTE kGradeLv3ZardPerPoint = 4; // ��� ��� 1�� ����Ʈ
    const BYTE kGradeLv4ZardPerPoint = 10; // �ֻ�� ��� 1�� ����Ʈ

    DecompositionInfo::DecompositionPoint decomposition_point = 0;

    // ��� ����� ��ῡ ����
    FOREACH_CONTAINER(
        const DecompositionInfo::MaterialInfo& material, 
        material_list, 
        DecompositionInfo::MaterialList)
    {
        // ��þƮ ����Ʈ�� ����Ѵ�.
        const DecompositionInfo::DecompositionPoint enchant_point = material.enchant_ * kEnchantPerPoint;

        // ���� ����Ʈ�� ����Ѵ�.
        const DecompositionInfo::DecompositionPoint socket_point = material.socket_number_ * kSocketNumberPerPoint;

        // ������ ���� ����Ʈ�� ����Ѵ�.
        const DecompositionInfo::DecompositionPoint item_level_point = material.item_level_ * kItemLevelPerPoint;

        // ��� ����Ʈ�� ����Ѵ�.
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

        // ���� ����Ʈ�� ����Ѵ�.
        decomposition_point += ((enchant_point + socket_point + zard_point + 1) * item_level_point);
    }

    return decomposition_point;
}

bool CubeSystemParser::GetDecompositionRewardList(
    const DecompositionInfo::DecompositionPoint decomposition_point, 
    DecompositionInfo::RewardList& OUT reward_list) const
{
    // �Է°��� �˻��Ѵ�.
    if (decomposition_point == 0 || !reward_list.empty())
    {
        return false;
    }

    FOREACH_CONTAINER(
        const DecompositionInfo::RewardInfo& reward, 
        decomposition_reward_list_, 
        DecompositionInfo::RewardList)
    {
        // ��ȯ�� ���� ������ ������ ��
        reward_list.push_back(reward);

        // ���� Ȯ���� ����Ѵ�.
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
    // �κ��丮�� ���� ������ ������ �õ��� �ʿ䰡 ����.
    if (inventory.GetEmptySlotNum() == 0)
    {
        return false;
    }

    request.request_type_ = MSG_CG_ITEM_COMPOSE_OR_DECOMPOSE_SYN::kCubeRequest;
    request.is_composition_ = is_composition;

    // ť�� �ý����� �������� ����ؼ� �����ϱ� ������ ť�� �������� �˻��Ѵ�.
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
    
    // �ռ� ��û�̶��
    if (request.is_composition_)
    {
        return util::internal::GetCompositionRequest(
            composition_map_, inventory, inventory_position_list, request);        
    }
    
    // ������ ��� ����� ��´�.
    DecompositionInfo::MaterialList material_list;
    if (!GetDecompositionMaterialList(inventory, inventory_position_list, material_list))
    {
        return false;
    }

    // ��û�� ��Ḧ �����Ѵ�.
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

    // ���� ����Ʈ�� ��´�.
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
    // ��ũ��Ʈ�� �ٽ� �д´ٸ�
    if (is_reload)
    {
        // ���� �ռ� ������ �����Ѵ�.
        ReleaseComposition();
    }

    bool is_valid_script = true;

    const int row_size = GetRowSize();
    for (int row = 0; row < row_size; row++)
    {
        // �ռ� �ڵ带 �о�ͼ� �ռ� ������ ã�´�.
        const CompositionInfo::CompositionCode composition_code = GetDataDWORD(_T("Code"), row);
        CompositionInfo* composition = FindComposition(composition_code);

        // �ռ� ������ ã�� �� ������
        if (composition == NULL)
        {
            // ���ο� �ռ� ������ �����Ѵ�.
            composition = new CompositionInfo(composition_code);
            composition_map_[composition_code] = composition;
        }

        // Į�� ������ �˻��Ѵ�.
        const CompositionInfo::ColumnNumber column_number = GetDataBYTE(_T("ColumnNumber"), row);
        if (column_number < 1 || 
            column_number > max(CompositionInfo::MaterialInfo::kMaxSize, CompositionInfo::RewardInfo::kMaxSize))
        {
            SUNLOG(eCRITICAL_LOG, 
                   _T("|%s|ColumnNumber(Į����ȣ)�� 1~%d ������ ���ڷ� �Է��ؾ� �մϴ�.|������ = %d, ColumnNumber = %d|"), 
                   __FUNCTION__, 
                   max(CompositionInfo::MaterialInfo::kMaxSize, CompositionInfo::RewardInfo::kMaxSize), 
                   row, 
                   column_number);
            is_valid_script = false;
            continue;
        }
        const CompositionInfo::ColumnNumber material_or_reward_index = column_number-1;

        // ��� Į���� ����Ѵٸ�
        const int use_material = GetDataInt(_T("UseMaterial"), row);
        if (use_material == CompositionInfo::kUsed)
        {
            CompositionInfo::MaterialList& material_list = composition->material_list_;
            CompositionInfo::MaterialInfo& material = material_list[material_or_reward_index];

            // ��ᰡ �̹� �����Ǿ��ٸ�
            if (material.is_used_)
            {
                // �ߺ��� ��� Į���̴�.
                SUNLOG(eCRITICAL_LOG, 
                       _T("|%s|���� �ռ������ȿ��� ����� ColumnNumber(Į����ȣ)�� �ߺ� �� �� �����ϴ�.|������ = %d, ColumnNumber = %d|"), 
                       __FUNCTION__, 
                       row, 
                       column_number);
                is_valid_script = false;
                continue;
            }

            // ��� ������ �˻��Ѵ�.
            const int material_number = GetDataInt(_T("MaterialNumber"), row);
            if (material_number < 1 || material_number > UCHAR_MAX)
            {
                SUNLOG(eCRITICAL_LOG, 
                       _T("|%s|MaterialNumber(��ᰳ��)�� �ùٸ��� �ʽ��ϴ�.|������ = %d, ��ᰳ�� = %d|"), 
                       __FUNCTION__, 
                       row, 
                       material_number);
                is_valid_script = false;
                continue;
            }

            // ��� ������ �о�´�.
            material.is_used_ = true;
            material.column_number_ = column_number;
            material.code_ = GetDataDWORD(_T("MaterialCode"), row);
            material.type_code_ = GetDataDWORD(_T("MaterialTypeCode"), row);
            material.level_ = GetDataWORD(_T("MaterialLevel"), row);
            material.number_ = static_cast<CompositionInfo::MaterialInfo::MaterialNumber>(material_number);
            material.ig_type_ = GetDataBYTE(_T("MaterialIGType"), row);

            // �о�� ����� ���ǵ� �� �ּ��� �ϳ��� ����ؾ� �Ѵ�.
            if (material.code_ == CompositionInfo::kUnused && 
                material.type_code_ == CompositionInfo::kUnused && 
                material.level_ == CompositionInfo::kUnused)
            {
                SUNLOG(eCRITICAL_LOG, 
                       _T("|%s|MaterialCode(����ڵ�),MaterialTypeCode(��������ڵ�),MaterialLevel(��᷹��) �߿� �ּ��� �ϳ��� �Է��ؾ� �մϴ�.|������ = %d|"), 
                       __FUNCTION__, 
                       row);
                is_valid_script = false;
                continue;
            }

            // ��� ������ Ÿ�� ������ �˻��Ѵ�.
            if (material.ig_type_ < KindOfItemType::Default || 
                material.ig_type_ >= KindOfItemType::Counts)
            {
                SUNLOG(eCRITICAL_LOG, 
                       _T("|%s|MaterialIGType(��������Ÿ��) ���� ������ �ǹٸ��� �ʽ��ϴ�.|������ = %d, MaterialIGType = %d|"), 
                       __FUNCTION__, 
                       row, 
                       material.ig_type_);
                is_valid_script = false;
                continue;
            }

            // ����ġ�� ����Ѵ�. (�ڵ� > Ÿ�� �ڵ� > ���� > ����)
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
            
            // Į�� ��ȣ�� ��� ũ�⺸�� ũ�ٸ� ���� ���Ե� ���̹Ƿ� 
            if (static_cast<CompositionInfo::MaterialList::size_type>(column_number) > material_list.size())
            {
                // ����� ũ�⸦ ���� �����Ѵ�.
                material_list.set_size(column_number);
            }
        }

        // ���� Į���� ����Ѵٸ�
        const int use_reward = GetDataInt(_T("UseReward"), row);
        if (use_reward == CompositionInfo::kUsed)
        {
            CompositionInfo::RewardList& reward_list = composition->reward_list_;
            CompositionInfo::RewardInfo& reward = reward_list[material_or_reward_index];

            // ������ �̹� �����Ǿ��ٸ�
            if (reward.is_used_)
            {
                // �ߺ��� ���� Į���̴�.
                SUNLOG(eCRITICAL_LOG, 
                       _T("|%s|���� �ռ������ȿ��� ������ ColumnNumber(Į����ȣ)�� �ߺ� �� �� �����ϴ�.|������ = %d, ColumnNumber = %d|"), 
                       __FUNCTION__, 
                       row, 
                       column_number);
                is_valid_script = false;
                continue;
            }

            // ���� ������ �˻��Ѵ�.
            const int reward_number = GetDataInt(_T("RewardNumber"), row);
            if (reward_number > UCHAR_MAX)
            {
                SUNLOG(eCRITICAL_LOG, 
                       _T("|%s|RewardNumber(���󰳼�)�� �ùٸ��� �ʽ��ϴ�.|������ = %d, ���󰳼� = %d|"), 
                       __FUNCTION__, 
                       row, 
                       reward_number);
                is_valid_script = false;
                continue;
            }

            // ���� ������ �о�´�.
            reward.is_used_ = true;
            reward.column_number_ = column_number;
            reward.type_ = GetDataInt(_T("RewardType"), row);
            reward.code_ = GetDataDWORD(_T("RewardCode"), row);
            reward.ig_type_ = GetDataBYTE(_T("RewardIGType"), row);
            reward.ratio_ = GetDataWORD(_T("RewardRatio"), row);
            reward.number_ = static_cast<CompositionInfo::RewardInfo::RewardNumber>(reward_number);

            // ���������� �˻��Ѵ�.
            if (reward.type_ != CompositionInfo::RewardInfo::kItemCode && 
                reward.type_ != CompositionInfo::RewardInfo::kRewardCode)
            {
                SUNLOG(eCRITICAL_LOG, 
                       _T("|%s|RewardType(��������)�� �������ڵ�(%d)�̰ų� �����ڵ�(%d)�̾�� �մϴ�.|������ = %d, RewardType = %d|"), 
                       __FUNCTION__, 
                       CompositionInfo::RewardInfo::kItemCode, 
                       CompositionInfo::RewardInfo::kRewardCode, 
                       row, 
                       reward.type_);
                is_valid_script = false;
                continue;
            }

            // �����ڵ�� ���� ������ ������� �ݵ�� �Է� �Ǿ�� �Ѵ�.
            if (reward.code_ == CompositionInfo::kUnused)
            {
                SUNLOG(eCRITICAL_LOG, 
                       _T("|%s|RewardCode(�����ڵ�)�� RewardType(��������)�� �ش��ϴ� ���� �ݵ�� �Է��ؾ� �մϴ�.|������ = %d|"), 
                       __FUNCTION__, 
                       row);
                is_valid_script = false;
                continue;
            }

            // ���� ������ Ÿ�� ������ �˻��Ѵ�.
            if (reward.ig_type_ < KindOfItemType::Default || 
                reward.ig_type_ >= KindOfItemType::Counts)
            {
                SUNLOG(eCRITICAL_LOG, 
                       _T("|%s|RewardIGType(���������Ÿ��) ���� ������ �ǹٸ��� �ʽ��ϴ�.|������ = %d, RewardIGType = %d|"), 
                       __FUNCTION__, 
                       row, 
                       reward.ig_type_);
                is_valid_script = false;
                continue;
            }

            // ���������� �������ڵ��϶�
            if (reward.type_ == CompositionInfo::RewardInfo::kItemCode)
            {
                // ���� Ȯ���� 0���� ���� �� ����.
                if (reward.ratio_ < 0 || reward.ratio_ > CompositionInfo::RewardInfo::kMaxRewardRatioSize)
                {
                    SUNLOG(eCRITICAL_LOG, 
                           _T("|%s|RewardType(��������)�� �������ڵ�(%d)�϶� RewardRatio(����Ȯ��)�� 0~%d ������ ���̾�� �մϴ�.|������ = %d|"), 
                           __FUNCTION__, 
                           CompositionInfo::RewardInfo::kItemCode, 
                           CompositionInfo::RewardInfo::kMaxRewardRatioSize, 
                           row);
                    is_valid_script = false;
                    continue;
                }

                // ���� ������ 0���� Ŀ�� �Ѵ�.
                if (reward.number_ == 0)
                {
                    SUNLOG(eCRITICAL_LOG, 
                           _T("|%s|RewardType(��������)�� �������ڵ�(%d)�϶� RewardNumber(���󰳼�)�� 0���� Ŀ�� �մϴ�.|������ = %d|"), 
                           __FUNCTION__, 
                           CompositionInfo::RewardInfo::kItemCode, 
                           row);
                    is_valid_script = false;
                    continue;
                }
            }

            // ���������� �����ڵ��϶�
            if (reward.type_ == CompositionInfo::RewardInfo::kRewardCode)
            {
                // ����Ȯ���� 0�� �ƴ϶��
                if (reward.ratio_ != 0.0)
                {
                    // ���� �����ϹǷ� �����Ѵ�.
                    SUNLOG(eCRITICAL_LOG, 
                           _T("|%s|RewardType(��������)�� �����ڵ�(%d)�϶� RewardRatio(����Ȯ��)�� 0�̾�� �մϴ�.|������ = %d|"), 
                           __FUNCTION__, 
                           CompositionInfo::RewardInfo::kRewardCode, 
                           row);
                    reward.ratio_ = 0;
                }

                // ���󰳼��� 0�� �ƴ϶��
                if (reward.number_ != 0)
                {
                    // ���� �����ϹǷ� �����Ѵ�.
                    SUNLOG(eCRITICAL_LOG, 
                           _T("|%s|RewardType(��������)�� �����ڵ�(%d)�϶� RewardNumber(���󰳼�)�� 0�̾�� �մϴ�.|������ = %d|"), 
                           __FUNCTION__, 
                           CompositionInfo::RewardInfo::kRewardCode, 
                           row);
                    reward.number_ = 0;
                }
            }

            // Į�� ��ȣ�� ��� ũ�⺸�� ũ�ٸ� ���� ���Ե� ���̹Ƿ� 
            if (static_cast<CompositionInfo::MaterialList::size_type>(column_number) > reward_list.size())
            {
                // ����� ũ�⸦ ���� �����Ѵ�.
                reward_list.set_size(column_number);
            }
        }
    } // for (int row = 0; row < row_size; row++)

    // ��ũ��Ʈ�� �̸� ��������.
    // ���� ������ �����ڵ��� ������ ���� �ǹ̸� �����ϰ� �����Ƿ� ���� �ڵ��϶��� ���� �ϳ��� ����Ѵ�.
    // ������ ���� ������ �������ڵ��� ������ �������ڵ带 ����Ѵ�.
    // �����ڵ�� �������ڵ��� ������ ������� �ʴ´�.

    // �� �ռ��� ����
    FOREACH_CONTAINER(
        const CompositionMap::value_type& composition_value, composition_map_, CompositionMap)
    {
        const CompositionInfo* const composition = composition_value.second;

        // �ռ� ������ ��� ������ �ּ� 1���� �ʿ��ϴ�.
        if ((composition->material_list_).size() == 0)
        {
            SUNLOG(eCRITICAL_LOG, 
                   _T("|%s|�ռ� �����ȿ� ��ᰡ �ּ��� �ϳ� �̻� �ʿ��մϴ�.|CompositionCode = %d|"), 
                   __FUNCTION__, 
                   composition->code_);
            is_valid_script = false;
            continue;
        }
        
        // �о���� ��� ����� �˻��Ѵ�.
        FOREACH_CONTAINER(
            const CompositionInfo::MaterialInfo& material, 
            composition->material_list_, 
            CompositionInfo::MaterialList)
        {
            // ��ᰡ ��� �������� �˻��Ѵ�.
            if (!material.is_used_)
            {
                SUNLOG(eCRITICAL_LOG, 
                       _T("|%s|��ᰡ ���������� �����Ǿ� �ֽ��ϴ�.|CompositionCode = %d|ColumnNumber = %d|"), 
                       __FUNCTION__, 
                       composition->code_, 
                       material.column_number_);
                is_valid_script = false;
                break;
            }
        }

        // �о���� ���� ����� �˻��Ѵ�.
        bool is_find_reward_code = false;
        FOREACH_CONTAINER(
            const CompositionInfo::RewardInfo& reward, 
            composition->reward_list_, 
            CompositionInfo::RewardList)
        {
            // ������ ��� �������� �˻��Ѵ�.
            if (!reward.is_used_)
            {
                SUNLOG(eCRITICAL_LOG, 
                       _T("|%s|������ ���������� �����Ǿ� �ֽ��ϴ�.|CompositionCode = %d|ColumnNumber = %d|"), 
                       __FUNCTION__, 
                       composition->code_, 
                       reward.column_number_);
                is_valid_script = false;
                break;
            }

            // �̹� �����ڵ尡 �ϳ� �߰ߵǾ��µ� 
            if (is_find_reward_code)
            {
                // �ٸ� ������ ������ �� �߰ߵǾ���.
                SUNLOG(eCRITICAL_LOG, 
                       _T("|%s|RewardType(��������)�� �����ڵ�� �����Ǿ� ���� ���� ���� �ϳ��� ���� �Է� �����մϴ�.|CompositionCode = %d|"), 
                       __FUNCTION__, 
                       composition->code_);
                is_valid_script = false;
                break;
            }

            if (reward.type_ == CompositionInfo::RewardInfo::kRewardCode)
            {
                is_find_reward_code = true;
            }

            // ������ ������ �ڵ尡 �����Ͽ��� �������� �˻��Ѵ�.
            const CompositionInfo::RewardList::difference_type same_item_code_count = \
                std::count_if(composition->reward_list_.begin(),
                              composition->reward_list_.end(), 
                              boost::bind(&util::internal::IsSameItemCode, _1, reward.code_));
            if (same_item_code_count > 1)
            {
                SUNLOG(eCRITICAL_LOG, 
                       _T("|%s|���� ��Ͽ��� ������ �ڵ�� �����ؾ� �մϴ�.|�ߺ����� = %d|"), 
                       __FUNCTION__, 
                       same_item_code_count);
                is_valid_script = false;
                break;
            }
        }

        // ���� �ڵ尡 �߰ߵǾ��µ� ������ �ϳ��� �ƴϴ�.
        if (is_find_reward_code && (composition->reward_list_).size() != 1)
        {
            SUNLOG(eCRITICAL_LOG, 
                   _T("|%s|RewardType(��������)�� �����ڵ�� �����Ǿ� ���� ���� ���� �ϳ��� ���� �Է� �����մϴ�.|CompositionCode = %d|"), 
                   __FUNCTION__, 
                   composition->code_);
            is_valid_script = false;
        }
    } // �� �ռ��� ����

    FASSERT(is_valid_script && _T("��ũ��Ʈ �����Ϳ� ������ �ֽ��ϴ�."));
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
        // ���� ������ �о�´�.
        DecompositionInfo::RewardInfo reward;
        {
            reward.type_ = GetDataInt(_T("DecRewardType"), row);
            reward.code_ = GetDataDWORD(_T("DecRewardCode"), row);
            reward.min_code_ = GetDataDWORD(_T("DecMinCode"), row);
            reward.max_code_ = GetDataDWORD(_T("DecMaxCode"), row);
            reward.ratio_ = GetDataWORD(_T("DecRewardRatio"), row);
        }

        // ���������� �˻��Ѵ�.
        if (reward.type_ != DecompositionInfo::RewardInfo::kItemCode && 
            reward.type_ != DecompositionInfo::RewardInfo::kZard)
        {
            SUNLOG(eCRITICAL_LOG, 
                   _T("|%s|DecRewardType(��������)�� �������ڵ�(%d)�̰ų� ���(%d) �̾�� �մϴ�.|������ = %d, DecRewardType = %d|"), 
                   __FUNCTION__, 
                   DecompositionInfo::RewardInfo::kItemCode, 
                   DecompositionInfo::RewardInfo::kZard, 
                   row, 
                   reward.type_);
            is_valid_script = false;
            continue;
        }

        // �����ڵ带 �˻��Ѵ�.
        if (reward.code_ == DecompositionInfo::kUnused)
        {
            SUNLOG(eCRITICAL_LOG, 
                   _T("|%s|DecRewardCode(�����ڵ�)���� �ݵ�� DecRewardType(��������)�� �ش��ϴ� �ڵ带 �Է��ؾ� �մϴ�.|������ = %d|"), 
                   __FUNCTION__, 
                   row);
            is_valid_script = false;
            continue;
        }

        // ����Ȯ���� �˻��Ѵ�.
        if (reward.ratio_ <= 0 || reward.ratio_ > DecompositionInfo::RewardInfo::kMaxRewardRatioSize)
        {
            SUNLOG(eCRITICAL_LOG, 
                   _T("|%s|DecRewardRatio(����Ȯ��)�� 1~%d ������ ���̾�� �մϴ�.|������ = %d|"), 
                   __FUNCTION__, 
                   DecompositionInfo::RewardInfo::kMaxRewardRatioSize, 
                   row);
            is_valid_script = false;
            continue;
        }
        
        // ���������� ����϶�
        if (reward.type_ == DecompositionInfo::RewardInfo::kZard)
        {
            // �����ڵ忡 ������� ������ �˻��Ѵ�.
            if (reward.code_ != DecompositionInfo::ZardInfo::kGradeLv1 && 
                reward.code_ != DecompositionInfo::ZardInfo::kGradeLv2 && 
                reward.code_ != DecompositionInfo::ZardInfo::kGradeLv3 && 
                reward.code_ != DecompositionInfo::ZardInfo::kGradeLv4)
            {
                SUNLOG(eCRITICAL_LOG, 
                       _T("|%s|�ǹٸ� ��� ����� �Է��ؾ� �մϴ�.|������ = %d, DecRewardCode = %d|"), 
                       __FUNCTION__, 
                       row, 
                       reward.code_);
                is_valid_script = false;
                continue;
            }

            // ����� �������� �ּ� �ڵ� ������ �ԷµǾ����� �˻��Ѵ�.
            if (reward.min_code_ == DecompositionInfo::kUnused)
            {
                SUNLOG(eCRITICAL_LOG, 
                       _T("|%s|DecMinCode ���� ��� ����� %d�� �����۵��� �ּ� �������ڵ� ������ �Է��ؾ� �մϴ�.|������ = %d|"), 
                       __FUNCTION__, 
                       reward.code_, 
                       row);
                is_valid_script = false;
                continue;
            }

            // ����� �������� �ִ� �ڵ� ������ �ԷµǾ����� �˻��Ѵ�. 
            if (reward.max_code_ == DecompositionInfo::kUnused)
            {
                SUNLOG(eCRITICAL_LOG, 
                       _T("|%s|DecMaxCode ���� ��� ����� %d�� �����۵��� �ִ� �������ڵ� ������ �Է��ؾ� �մϴ�.|������ = %d|"), 
                       __FUNCTION__, 
                       reward.code_, 
                       row);
                is_valid_script = false;
                continue;
            }

            // �ּ� �����ڵ�� �ִ� ���� �ڵ庸�� Ŭ �� ����.
            if (reward.min_code_ > reward.max_code_)
            {
                SUNLOG(eCRITICAL_LOG, 
                       _T("|%s|DecMinCode�� DecMaxCode ���� Ŭ �� �����ϴ�.|DecMinCode = %d, DecMaxCode = %d, ������ = %d|"), 
                       __FUNCTION__, 
                       reward.min_code_, 
                       reward.max_code_, 
                       row);
                is_valid_script = false;
                continue;
            }

            // ��� ���� ������ �˻��ؼ� ��ȿ�� ������ ��´�.
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
                if (zard->m_ChargeType == 1) // ĳ�þ������ϰ�� ChargeType�� 1�� ����
                {
                    // ĳ����� �������� ���󸮽�Ʈ�� ���Խ�Ű�� �ʴ´�.
                    continue;
                }
                #endif // _NA_006251_CUBESYSTEM_Decomposition_REWARD_REMOVE_CASH_ZARD
                (reward.zard_list).push_back(zard_code);
            }
            if ((reward.zard_list).empty())
            {
                SUNLOG(eCRITICAL_LOG, 
                       _T("|%s|��� ���� �ڵ� ������ ��ȿ���� �ʽ��ϴ�.|DecMinCode = %d, DecMaxCode = %d, ������ = %d|"), 
                       __FUNCTION__, 
                       reward.min_code_, 
                       reward.max_code_, 
                       row);
                is_valid_script = false;
                continue;
            }
        }
        
        // ���� ��Ͽ� �����Ѵ�.
        decomposition_reward_list_.push_back(reward);
    }

    FASSERT(is_valid_script && _T("��ũ��Ʈ �����Ϳ� ������ �ֽ��ϴ�."));
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