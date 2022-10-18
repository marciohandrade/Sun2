////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   ItemScoreParser.cpp
/// @author hukim
/// @email  silped@gmail.com
/// @brief  [2011/07/04 by hukim] This is created.\n
///
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include ".\ItemScoreParser.h" // header
#include <SCItemSlot.h>
#include <SCItemSlotContainer.h>
#include <PlayerAttributes.h>
#include <ItemAttrCalculatorEP2.h>
#include <boost/foreach.hpp>
#ifdef _NA_008124_20150313_AWAKENING_SYSTEM
#include "AwakeningInfoParser.h"
#endif // _NA_008124_20150313_AWAKENING_SYSTEM

#ifdef _NA_002935_20110704_ITEM_SCORE

////////////////////////////////////////////////////////////////////////////////////////////////////
// REGION : WeightManager
////////////////////////////////////////////////////////////////////////////////////////////////////

namespace LocalUtil {
// 포인터를 저장하는 맵을 초기화한다.
template < typename MAP_T >
inline void ClearPointerMap(MAP_T& map)
{
    for (MAP_T::iterator it = map.begin(); it != map.end(); ++it)
    {
        typename MAP_T::mapped_type pointer = it->second;
        delete pointer;
    }
    map.clear();
}

// 포인터 맵에 포인터를 삽입한다.
template < typename MAP_T >
inline void InsertPointerMap(
    MAP_T& map, const typename MAP_T::key_type key, typename MAP_T::mapped_type pointer)
{
    if (pointer == NULL)
    {
        return;
    }
    MAP_T::iterator it = map.find(key);
    if (it != map.end())
    {
        typename MAP_T::mapped_type old_pointer = it->second;
        delete old_pointer;
    }
    map[key] = pointer;
}

// 포인터 맵에서 포인터를 찾는다.
template < typename MAP_T >
inline typename MAP_T::mapped_type FindPointerMap(MAP_T& map, const typename MAP_T::key_type key)
{
    const MAP_T::const_iterator it = map.find(key);
    if (it == map.end())
    {
        return NULL;
    }
    return it->second;
}
} // LocalUtil

ItemScoreParser::WeightManager::WeightManager()
{
}

ItemScoreParser::WeightManager::~WeightManager()
{
    Clear();
}

void ItemScoreParser::WeightManager::Clear()
{
    LocalUtil::ClearPointerMap(item_level_weight_map_);
    LocalUtil::ClearPointerMap(divine_item_level_weight_map_);
    LocalUtil::ClearPointerMap(enchant_weight_map_);
    LocalUtil::ClearPointerMap(extra_stone_weight_map_);
    zard_option_point_weight_map_.clear();
    LocalUtil::ClearPointerMap(option_weight_map_);
}

void ItemScoreParser::WeightManager::InsertItemLevelWeight(
    const ItemLevelWeight* weight, const bool is_divine)
{
    ItemLevelWeightMap& item_level_weight_map = 
        is_divine ? divine_item_level_weight_map_ : item_level_weight_map_;
    LocalUtil::InsertPointerMap(item_level_weight_map, weight->item_level, weight);
}

ItemScoreParser::Weight ItemScoreParser::WeightManager::FindItemLevelWeight( 
    const LEVELTYPE item_level, 
    const ItemLevelWeight::EquipPosition equip_position, 
    const bool is_divine) const
{
    const ItemLevelWeightMap& item_level_weight_map = 
        is_divine ? divine_item_level_weight_map_ : item_level_weight_map_;
    const ItemLevelWeight* const weight = 
        LocalUtil::FindPointerMap(item_level_weight_map, item_level);
    if (weight == NULL)
    {
        return 0;
    }
    typedef ItemLevelWeight::EquipPositionMap EquipPositionMap;
    const EquipPositionMap& equip_position_map = weight->equip_position_map;
    const EquipPositionMap::const_iterator eq_pos_it = equip_position_map.find(equip_position);
    if (eq_pos_it == equip_position_map.end())
    {
        return 0;
    }
    return eq_pos_it->second;
}

void ItemScoreParser::WeightManager::InsertEnchantWeight(const EnchantWeight* weight)
{
    LocalUtil::InsertPointerMap(enchant_weight_map_, weight->enchant_grade, weight);
}

ItemScoreParser::Weight ItemScoreParser::WeightManager::FindEnchantWeight( 
    const EnchantWeight::EnchantGrade enchant_grade, const EnchantWeight::ItemType item_type) const
{
    const EnchantWeight* const weight = 
        LocalUtil::FindPointerMap(enchant_weight_map_, enchant_grade);
    if (weight == NULL)
    {
        return 0;
    }
    if (item_type < 0 || item_type >= _countof(weight->item_type_list))
    {
        return 0;
    }
    return (weight->item_type_list)[item_type];
}

void ItemScoreParser::WeightManager::InsertExtraStoneWeight(const ExtraStoneWeight* weight)
{
    LocalUtil::InsertPointerMap(extra_stone_weight_map_, weight->extra_stone_size, weight);
}

ItemScoreParser::Weight ItemScoreParser::WeightManager::FindExtraStoneWeight( 
    const ExtraStoneWeight::ExtraStoneSize extra_stone_size, 
    const ExtraStoneWeight::ItemType item_type) const
{
    const ExtraStoneWeight* const weight = 
        LocalUtil::FindPointerMap(extra_stone_weight_map_, extra_stone_size);
    if (weight == NULL)
    {
        return 0;
    }
    if (item_type < 0 || item_type >= _countof(weight->item_type_list))
    {
        return 0;
    }
    return (weight->item_type_list)[item_type];
}

void ItemScoreParser::WeightManager::InsertZardOptionPointWeight( 
    const ZardOptionPoint zard_option_point, const Weight weight)
{
    zard_option_point_weight_map_[zard_option_point] = weight;
}

ItemScoreParser::Weight ItemScoreParser::WeightManager::FindZardOptionPointWeight( 
    const ZardOptionPoint zard_option_point) const
{
    if (zard_option_point <= 0)
    {
        return 0;
    }
    ZardOptionPointWeightMap::const_reverse_iterator it = zard_option_point_weight_map_.rbegin();
    for (; it != zard_option_point_weight_map_.rend(); ++it)
    {
        const ZardOptionPoint option_point = it->first;
        if (option_point <= zard_option_point)
        {
            return it->second;
        }
    }
    return 0;
}

void ItemScoreParser::WeightManager::InsertOptionWeight(const OptionWeight* weight)
{
    LocalUtil::InsertPointerMap(option_weight_map_, weight->attr_option_code, weight);
}

ItemScoreParser::Weight ItemScoreParser::WeightManager::FindOptionWeight( 
    const eATTR_TYPE attr_option_code, const OptionWeight::ValueType value_type) const
{
    const OptionWeight* const weight = 
        LocalUtil::FindPointerMap(option_weight_map_, attr_option_code);
    if (weight == NULL)
    {
        return 0;
    }
    if (value_type < 0 || value_type >= _countof(weight->value_type_list))
    {
        return 0;
    }
    if (weight->is_reverse_benefit)
    {
        return (-1 * (weight->value_type_list)[value_type]);
    }
    return (weight->value_type_list)[value_type];
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// REGION : Constructor & Destructor
////////////////////////////////////////////////////////////////////////////////////////////////////

ItemScoreParser::ItemScoreParser()
{
}

ItemScoreParser::~ItemScoreParser()
{
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// REGION : Public
////////////////////////////////////////////////////////////////////////////////////////////////////

BOOL ItemScoreParser::LoadScript(eSCRIPT_CODE script_code, BOOL is_reload)
{
    if (script_code != SCRIPT_ITEM_SCORE_COMMON)
    {
        FASSERT(false && _T("스크립트 데이터에 오류가 있습니다."));
        return FALSE;
    }
    return (LoadWeight(is_reload == TRUE) ? TRUE : FALSE);
}

ItemScoreParser::Weight ItemScoreParser::CalculateSingleScore(
    const SCItemSlot& item_slot, const bool is_pc_room) const
{
    const SLOTCODE item_code = item_slot.GetItemCode();
    if (item_code == 0)
    {
        return 0;
    }

    const BASE_ITEMINFO* const item_info = item_slot.GetItemInfo();
    if ((item_info->IsWeapon() == false) &&
        (item_info->IsArmor() == false) &&
        (item_info->IsAccessory() == false) &&
        (item_info->IsSpecialAccessory() == false) &&
        (item_slot.IsWingItem() == false))
    {
        return 0;
    }

    // 옵션 점수
    Weight option_weight = CalculateOptionWeight(
        item_info->m_wOptionIndex, item_info->m_byOptionType, item_info->m_iOptionValue);
    if (is_pc_room)
    {
        option_weight += CalculateOptionWeight(
            item_info->m_wPCOptionIndex, item_info->m_byPCOptionType, item_info->m_iPCOptionValue);        
    }

    // 악세사리 점수를 계산한다.
    if (item_info->IsAccessory())
    {
        return option_weight;
    }

    // 인챈트 점수
    const EnchantWeight::ItemType enchant_item_type = 
        item_info->IsWeapon() ? EnchantWeight::kWeapon : EnchantWeight::kArmor;
    const Weight enchant_score = 
        weight_manager_.FindEnchantWeight(item_slot.GetEnchant(), enchant_item_type);

#ifdef _NA_008124_20150313_AWAKENING_SYSTEM
    // 각성 점수
    const AwakeningWeight::ItemType awakening_item_type = 
        item_info->IsWeapon() ? AwakeningWeight::kWeapon : AwakeningWeight::kArmor;
    Weight awakening_score = 0;
    if (item_slot.GetAwakening() > 0)
    {
        awakening_score = weight_manager_.GetAwakeningWeight(awakening_item_type)
            * AwakeningInfoParser::Instance()->GetOptionValue(item_slot.GetCode(), item_slot.GetAwakening());
    }
#endif // _NA_008124_20150313_AWAKENING_SYSTEM
    
    // 무기나 방어구 점수를 계산한다.
    if (item_info->IsWeapon() == true || item_info->IsArmor() == true)
    {
        // 기본 점수
        const Weight base_score = weight_manager_.FindItemLevelWeight(
            item_info->m_LV, item_info->m_wEqPos, (item_slot.IsDivine() == TRUE));

        // 엑스트라 스톤 점수
        const ExtraStoneWeight::ExtraStoneSize extra_stone_size = item_slot.GetNOptionValueIndex();
        const ExtraStoneWeight::ItemType extra_stone_item_type = 
            item_info->IsWeapon() ? ExtraStoneWeight::kWeapon : ExtraStoneWeight::kArmor;
        const Weight extra_stone_score = 
            weight_manager_.FindExtraStoneWeight(extra_stone_size, extra_stone_item_type);

#ifdef _NA_008124_20150313_AWAKENING_SYSTEM
        // 기본 점수 + 인챈트 점수 + 엑스트라 스톤 점수 + 옵션 점수 + 각성 점수
        return (base_score + enchant_score + extra_stone_score + option_weight + awakening_score);
#else
        // 기본 점수 + 인챈트 점수 + 엑스트라 스톤 점수 + 옵션 점수
        return (base_score + enchant_score + extra_stone_score + option_weight);
#endif // _NA_008124_20150313_AWAKENING_SYSTEM
    }

    // 특수악세서리 점수를 계산한다.
    if (item_info->IsSpecialAccessory())
    {
        const WORD item_phy_def = item_slot.IsDivine() ? item_info->m_wDivPhyDef : item_info->m_wPhyDef;
        if (item_phy_def == 0)
        {
            // 스크립트에 방어력이 없는 특수악세서리 아이템은 옵션점수만 적용 
            return option_weight;
        }
        else
        {
            const Weight defense_option_weight = 
                weight_manager_.FindOptionWeight(eATTR_OPTION_PHYSICAL_DEFENSE_POWER, OptionWeight::kFix);
            const Weight defense_score = defense_option_weight * (Weight)item_phy_def;

            // 방어력 점수 + 인챈트 점수 + 옵션 점수
            return (defense_score + enchant_score + option_weight);
        }
    }

#ifdef _DEV_VER
	FASSERT(false && _T("지원하지 않는 아이템 타입 입니다."));
#endif

    return 0;
}

void ItemScoreParser::CalculateTotalEquipScore(
    const PlayerAttributes& player_attrs, Weight& socket_score, Weight& set_option_score) const
{
    socket_score = 0;
    set_option_score = 0;

    const SocketOptionAttributes& socket_attrs = player_attrs.GetSocketOptionAttributes();
    const SetOptionAttributes& set_attrs = player_attrs.GetSetOptionAttributes();

    for (int i = eATTR_TYPE_INVALID+1; i < eATTR_MAX; ++i)
    {
        const eATTR_TYPE attr_type = static_cast<eATTR_TYPE>(i);

        // 소켓 평가 점수를 계산한다.
        const SocketOptionAttributes::AttrValue& socket_attr_value = 
            socket_attrs.GetAttrValue(attr_type);
        if (socket_attr_value.point != 0)
        {
            socket_score += weight_manager_.FindZardOptionPointWeight(socket_attr_value.point);
        }
        
        // 세트 옵션 평가 점수를 계산한다.
        const SetOptionAttributes::AttrValue& set_attr_value = set_attrs.GetAttrValue(attr_type);
        if (set_attr_value.fix_value != 0)
        {
            // 고정형 값을 계산한다.
            const Weight option_weight = 
                weight_manager_.FindOptionWeight(attr_type, OptionWeight::kFix);
            if (option_weight != 0)
            {
                set_option_score += (option_weight * set_attr_value.fix_value);
            }
        }
        if (set_attr_value.ratio_value != 0)
        {
            // 비율형 값을 계산한다.
            const Weight option_weight = 
                weight_manager_.FindOptionWeight(attr_type, OptionWeight::kRatio);
            if (option_weight != 0)
            {
                set_option_score += (option_weight * set_attr_value.ratio_value);
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// REGION : Private
////////////////////////////////////////////////////////////////////////////////////////////////////

bool ItemScoreParser::LoadWeight(const bool is_reload)
{
    if (is_reload)
    {
        weight_manager_.Clear();
    }

    const int row_size = GetRowSize();
    for (int row = 0; row < row_size; ++row)
    {
        const int large_group = GetDataInt(_T("LGroup"), row);
        enum LargeGroup
        {
            kItemLevel = 1, 
            kDivineItemLevel = 2,
            kEnchant = 3, 
            kExtraStone = 4, 
            kZardOptionPoint = 5, 
            kOption = 6,
#ifdef _NA_008124_20150313_AWAKENING_SYSTEM
            kAwakening = 7,
#endif // _NA_008124_20150313_AWAKENING_SYSTEM
        };
        switch (large_group)
        {
        case kItemLevel:
        case kDivineItemLevel:
            {
                enum { kItemLevelMaxWeight = 9999 };

                const LEVELTYPE item_level = GetDataWORD(_T("SGroup"), row);
                if (item_level < ItemCustomLevel::kMinLV || item_level > ItemCustomLevel::kMaxLV)
                {
                    SUNLOG(eCRITICAL_LOG, 
                           _T("|%s|아이템 레벨이 올바르지 않습니다.|줄번호 = %d, 레벨 = %d|"), 
                           __FUNCTION__, 
                           row, 
                           item_level);
                    assert(false);
                    continue;
                }
                const Weight weapon_weight = GetDataFloat(_T("Value1"), row);
                if (weapon_weight < 0 || weapon_weight > kItemLevelMaxWeight)
                {
                    SUNLOG(eCRITICAL_LOG, 
                           _T("|%s|아이템 레벨 무기 가중치가 올바르지 않습니다.|줄번호 = %d, 가중치 = %f|"), 
                           __FUNCTION__, 
                           row, 
                           weapon_weight);
                    assert(false);
                    continue;
                }
                const Weight armor_weight = GetDataFloat(_T("Value2"), row);
                if (armor_weight < 0 || armor_weight > kItemLevelMaxWeight)
                {
                    SUNLOG(eCRITICAL_LOG, 
                           _T("|%s|아이템 레벨 아머 가중치가 올바르지 않습니다.|줄번호 = %d, 가중치 = %f|"), 
                           __FUNCTION__, 
                           row, 
                           armor_weight);
                    assert(false);
                    continue;
                }
                const Weight protector_weight = GetDataFloat(_T("Value3"), row);
                if (protector_weight < 0 || protector_weight > kItemLevelMaxWeight)
                {
                    SUNLOG(eCRITICAL_LOG, 
                           _T("|%s|아이템 레벨 프로텍터 가중치가 올바르지 않습니다.|줄번호 = %d, 가중치 = %f|"), 
                           __FUNCTION__, 
                           row, 
                           protector_weight);
                    assert(false);
                    continue;
                }
                const Weight helmet_weight = GetDataFloat(_T("Value4"), row);
                if (helmet_weight < 0 || helmet_weight > kItemLevelMaxWeight)
                {
                    SUNLOG(eCRITICAL_LOG, 
                           _T("|%s|아이템 레벨 헬멧 가중치가 올바르지 않습니다.|줄번호 = %d, 가중치 = %f|"), 
                           __FUNCTION__, 
                           row, 
                           helmet_weight);
                    assert(false);
                    continue;
                }
                const Weight pants_weight = GetDataFloat(_T("Value5"), row);
                if (pants_weight < 0 || pants_weight > kItemLevelMaxWeight)
                {
                    SUNLOG(eCRITICAL_LOG, 
                           _T("|%s|아이템 레벨 바지 가중치가 올바르지 않습니다.|줄번호 = %d, 가중치 = %f|"), 
                           __FUNCTION__, 
                           row, 
                           pants_weight);
                    assert(false);
                    continue;
                }
                const Weight boots_weight = GetDataFloat(_T("Value6"), row);
                if (boots_weight < 0 || boots_weight > kItemLevelMaxWeight)
                {
                    SUNLOG(eCRITICAL_LOG, 
                           _T("|%s|아이템 레벨 부츠 가중치가 올바르지 않습니다.|줄번호 = %d, 가중치 = %f|"), 
                           __FUNCTION__, 
                           row, 
                           boots_weight);
                    assert(false);
                    continue;
                }
                const Weight glove_weight = GetDataFloat(_T("Value7"), row);
                if (glove_weight < 0 || glove_weight > kItemLevelMaxWeight)
                {
                    SUNLOG(eCRITICAL_LOG, 
                           _T("|%s|아이템 레벨 글러브 가중치가 올바르지 않습니다.|줄번호 = %d, 가중치 = %f|"), 
                           __FUNCTION__, 
                           row, 
                           glove_weight);
                    assert(false);
                    continue;
                }
                const Weight belt_weight = GetDataFloat(_T("Value8"), row);
                if (belt_weight < 0 || belt_weight > kItemLevelMaxWeight)
                {
                    SUNLOG(eCRITICAL_LOG, 
                           _T("|%s|아이템 레벨 벨트 가중치가 올바르지 않습니다.|줄번호 = %d, 가중치 = %f|"), 
                           __FUNCTION__, 
                           row, 
                           belt_weight);
                    assert(false);
                    continue;
                }
                const Weight shirts_weight = GetDataFloat(_T("Value9"), row);
                if (shirts_weight < 0 || shirts_weight > kItemLevelMaxWeight)
                {
                    SUNLOG(eCRITICAL_LOG, 
                           _T("|%s|아이템 레벨 셔츠 가중치가 올바르지 않습니다.|줄번호 = %d, 가중치 = %f|"), 
                           __FUNCTION__, 
                           row, 
                           shirts_weight);
                    assert(false);
                    continue;
                }
                
                ItemLevelWeight* weight(new ItemLevelWeight(item_level));
                ItemLevelWeight::EquipPositionMap& equip_position_map = weight->equip_position_map;
                equip_position_map[eEQUIPCONTAINER_WEAPON] = weapon_weight;
                equip_position_map[eEQUIPCONTAINER_ARMOR] = armor_weight;
                equip_position_map[eEQUIPCONTAINER_PROTECTOR] = protector_weight;
                equip_position_map[eEQUIPCONTAINER_HELMET] = helmet_weight;
                equip_position_map[eEQUIPCONTAINER_PANTS] = pants_weight;
                equip_position_map[eEQUIPCONTAINER_BOOTS] = boots_weight;
                equip_position_map[eEQUIPCONTAINER_GLOVE] = glove_weight;
                equip_position_map[eEQUIPCONTAINER_BELT] = belt_weight;
                equip_position_map[eEQUIPCONTAINER_SHIRTS] = shirts_weight;
                
                weight_manager_.InsertItemLevelWeight(weight, (large_group == kDivineItemLevel));
            }
            break;
        case kEnchant:
            {
#ifdef _NA_003966_20111227_ADD_ENCHANT
                enum { kMaxEnchantGrade = 15 };
#else
                enum { kMaxEnchantGrade = 12 };
#endif                
                enum { kEnchantMaxWeight = 9999 };

                const EnchantWeight::EnchantGrade enchant_grade = GetDataBYTE(_T("SGroup"), row);
                if (enchant_grade > kMaxEnchantGrade)
                {
                    SUNLOG(eCRITICAL_LOG, 
                           _T("|%s|인첸트 등급이 올바르지 않습니다.|줄번호 = %d, 등급 = %d|"), 
                           __FUNCTION__, 
                           row, 
                           enchant_grade);
                    assert(false);
                    continue;
                }
                const Weight weapon_weight = GetDataFloat(_T("Value1"), row);
                if (weapon_weight < 0 || weapon_weight > kEnchantMaxWeight)
                {
                    SUNLOG(eCRITICAL_LOG, 
                           _T("|%s|인첸트 무기 가중치가 올바르지 않습니다.|줄번호 = %d, 가중치 = %f|"), 
                           __FUNCTION__, 
                           row, 
                           weapon_weight);
                    assert(false);
                    continue;
                }
                const Weight armor_weight = GetDataFloat(_T("Value2"), row);
                if (armor_weight < 0 || armor_weight > kEnchantMaxWeight)
                {
                    SUNLOG(eCRITICAL_LOG, 
                           _T("|%s|인첸트 방어구 가중치가 올바르지 않습니다.|줄번호 = %d, 가중치 = %f|"), 
                           __FUNCTION__, 
                           row, 
                           armor_weight);
                    assert(false);
                    continue;
                }

                Weight item_type_list[EnchantWeight::kMaxSize];
                ::ZeroMemory(item_type_list, sizeof(item_type_list));
                item_type_list[EnchantWeight::kWeapon] = weapon_weight;
                item_type_list[EnchantWeight::kArmor] = armor_weight;

                const EnchantWeight* weight(new EnchantWeight(enchant_grade, item_type_list));
                weight_manager_.InsertEnchantWeight(weight);
            }
            break;
        case kExtraStone:
            {
                enum { kExtraStoneMaxWeight = 9999 };
                const ExtraStoneWeight::ExtraStoneSize extra_stone_size = 
                    GetDataInt(_T("SGroup"), row);
                if (extra_stone_size < 0 || extra_stone_size > sEXTRASTONE_OPT_INFO::MAX_VALUE_NUM)
                {
                    SUNLOG(eCRITICAL_LOG, 
                           _T("|%s|엑스트라 스톤 사용 개수가 올바르지 않습니다.|줄번호 = %d, 개수 = %d|"), 
                           __FUNCTION__, 
                           row, 
                           extra_stone_size);
                    assert(false);
                    continue;
                }
                const Weight weapon_weight = GetDataFloat(_T("Value1"), row);
                if (weapon_weight < 0 || weapon_weight > kExtraStoneMaxWeight)
                {
                    SUNLOG(eCRITICAL_LOG, 
                           _T("|%s|엑스트라 스톤 무기 가중치가 올바르지 않습니다.|줄번호 = %d, 가중치 = %f|"), 
                           __FUNCTION__, 
                           row, 
                           weapon_weight);
                    assert(false);
                    continue;
                }
                const Weight armor_weight = GetDataFloat(_T("Value2"), row);
                if (armor_weight < 0 || armor_weight > kExtraStoneMaxWeight)
                {
                    SUNLOG(eCRITICAL_LOG, 
                           _T("|%s|엑스트라 스톤 방어구 가중치가 올바르지 않습니다.|줄번호 = %d, 가중치 = %f|"), 
                           __FUNCTION__, 
                           row, 
                           armor_weight);
                    assert(false);
                    continue;
                }
                
                Weight item_type_list[ExtraStoneWeight::kMaxSize];
                ::ZeroMemory(item_type_list, sizeof(item_type_list));
                item_type_list[ExtraStoneWeight::kWeapon] = weapon_weight;
                item_type_list[ExtraStoneWeight::kArmor] = armor_weight;

                const ExtraStoneWeight* weight(new ExtraStoneWeight(extra_stone_size, item_type_list));
                weight_manager_.InsertExtraStoneWeight(weight);
            }
            break;
        case kZardOptionPoint:
            {
                enum { kZardOptionPointMaxWeight = 9999 };
                const ZardOptionPoint zard_option_point = GetDataInt(_T("SGroup"), row);
                const Weight weight = GetDataFloat(_T("Value1"), row);
                if (weight < 0 || weight > kZardOptionPointMaxWeight)
                {
                    SUNLOG(eCRITICAL_LOG, 
                           _T("|%s|쟈드 옵션 포인트 가중치가 올바르지 않습니다.|줄번호 = %d, 가중치 = %f|"), 
                           __FUNCTION__, 
                           row, 
                           weight);
                    assert(false);
                    continue;
                }
                weight_manager_.InsertZardOptionPointWeight(zard_option_point, weight);
            }
            break;
        case kOption:
            {
                enum { kOptionMaxWeight = 9999 };
                const WORD item_option_code = GetDataWORD(_T("SGroup"), row);
                if (item_option_code == 0 || item_option_code >= _countof(g_ITEM_OPTION))
                {
                    SUNLOG(eCRITICAL_LOG, 
                           _T("|%s|알 수 없는 옵션 코드 입니다.|줄번호 = %d, 옵션 코드 = %u|"), 
                           __FUNCTION__, 
                           row, 
                           item_option_code);
                    assert(false);
                    continue;
                }
                const Weight fix_weight = GetDataFloat(_T("Value1"), row);
                if (fix_weight < 0 || fix_weight > kOptionMaxWeight)
                {
                    SUNLOG(eCRITICAL_LOG, 
                           _T("|%s|옵션 고정값 가중치가 올바르지 않습니다.|줄번호 = %d, 가중치 = %f|"), 
                           __FUNCTION__, 
                           row, 
                           fix_weight);
                    assert(false);
                    continue;
                }
                const Weight ratio_weight = GetDataFloat(_T("Value2"), row);
                if (ratio_weight < 0 || ratio_weight > kOptionMaxWeight)
                {
                    SUNLOG(eCRITICAL_LOG, 
                           _T("|%s|옵션 비율값 가중치가 올바르지 않습니다.|줄번호 = %d, 가중치 = %f|"), 
                           __FUNCTION__, 
                           row, 
                           ratio_weight);
                    assert(false);
                    continue;
                }
                const int readed_reverse_benefit = GetDataInt(_T("Value3"), row);
                if (readed_reverse_benefit != 0 && readed_reverse_benefit != 1)
                {
                    SUNLOG(eCRITICAL_LOG, 
                           _T("|%s|옵션 역순 이익 여부가 올바르지 않습니다.|줄번호 = %d, 역순 이익 여부 = %d|"), 
                           __FUNCTION__, 
                           row, 
                           readed_reverse_benefit);
                    assert(false);
                    continue;
                }
                const bool is_reverse_benefit = (readed_reverse_benefit == 1);

                const eATTR_TYPE attr_option_code = g_ITEM_OPTION[item_option_code];
                Weight value_type_list[OptionWeight::kMaxSize];
                ::ZeroMemory(value_type_list, sizeof(value_type_list));
                value_type_list[OptionWeight::kFix] = fix_weight;
                value_type_list[OptionWeight::kRatio] = ratio_weight;

                const OptionWeight* weight(
                    new OptionWeight(attr_option_code, is_reverse_benefit, value_type_list));
                weight_manager_.InsertOptionWeight(weight);
            }
            break;
#ifdef _NA_008124_20150313_AWAKENING_SYSTEM
        case kAwakening:
            {
                const int unused = GetDataInt(_T("SGroup"), row);
                Weight weapon_weight = GetDataFloat(_T("Value1"), row);
                if (!weight_manager_.SetAwakeningWeight(weapon_weight, AwakeningWeight::kWeapon))
                {
                    SUNLOG(eCRITICAL_LOG, 
                        _T("|%s|각성 가중치가 올바르지 않습니다.|줄번호 = %d, 가중치 = %f|"), 
                        __FUNCTION__, 
                        row, 
                        weapon_weight);
                    assert(false);
                    continue;
                }

                Weight armor_weight = GetDataFloat(_T("Value2"), row);
                if (!weight_manager_.SetAwakeningWeight(armor_weight, AwakeningWeight::kArmor))
                {
                    SUNLOG(eCRITICAL_LOG, 
                        _T("|%s|각성 가중치가 올바르지 않습니다.|줄번호 = %d, 가중치 = %f|"), 
                        __FUNCTION__, 
                        row, 
                        armor_weight);
                    assert(false);
                    continue;
                }
            }
            break;
#endif // _NA_008124_20150313_AWAKENING_SYSTEM
        default:
            SUNLOG(eCRITICAL_LOG, 
                   _T("|%s|알 수 없는 대분류 입니다.|줄번호 = %d, 대분류 = %d|"), 
                   __FUNCTION__, 
                   row, 
                   large_group);
            assert(false);
            continue;
        }
    }
    return true;
}

ItemScoreParser::Weight ItemScoreParser::CalculateOptionWeight(
    const BASE_ITEMINFO::OptionIndexes& option_indexes, 
    const BASE_ITEMINFO::OptionTypes& option_types, 
    const BASE_ITEMINFO::OptionValues& option_values) const
{
    Weight weight = 0;
    for (int i = 0; i < _countof(option_indexes); ++i)
    {
        const WORD item_option_code = option_indexes[i];
        if (item_option_code == 0 || item_option_code >= _countof(g_ITEM_OPTION))
        {
            continue;
        }
        if (option_values[i] == 0)
        {
            continue;
        }
        OptionWeight::ValueType option_value_type = OptionWeight::kMaxSize;
        if (option_types[i] == VALUE_TYPE_VALUE ||
            option_types[i] == VALUE_TYPE_RATIO_VALUE)
        {
            option_value_type = OptionWeight::kFix;
        }
        else if (option_types[i] == VALUE_TYPE_PERCENT_PER_CUR)
        {
            option_value_type = OptionWeight::kRatio;
        }
        else
        {
            continue;
        }
        const eATTR_TYPE attr_option_code = g_ITEM_OPTION[item_option_code];
        const Weight option_weight = 
            weight_manager_.FindOptionWeight(attr_option_code, option_value_type);
        if (option_weight != 0)
        {
            // 옵션 점수 = 옵션 가중치 * 옵션값
            weight += (option_weight * option_values[i]);
        }
    }
    return weight;
}

#endif //_NA_002935_20110704_ITEM_SCORE