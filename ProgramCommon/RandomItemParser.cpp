////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   RandomItemParser.cpp
/// @author hukim
/// @email  silped@gmail.com
/// @brief  [2011/11/22 by hukim] This is created.\n
///
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include ".\RandomItemParser.h" // header
#include "SCSlotHeader.h"
#include "SCItemSlot.h"
#include "SCItemSlotContainer.h"
#include "ItemInfoParser.h"
#include "PacketStruct_CG.h"


////////////////////////////////////////////////////////////////////////////////////////////////////
// REGION : Constructor & Destructor
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _SERVER
#ifdef _NA_000000_20140103_MALLOC_CHANGE_TO_TALLOCATE_CREATEITEM
RandomItemParser::RandomItemParser()
{
}
#else
RandomItemParser::RandomItemParser() : item_info_pool_(kItemInfoPoolSize)
{
}
#endif //_NA_000000_20140103_MALLOC_CHANGE_TO_TALLOCATE_CREATEITEM
#else
RandomItemParser::RandomItemParser()
{
}
#endif // _SERVER

RandomItemParser::~RandomItemParser()
{
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// REGION : Public
////////////////////////////////////////////////////////////////////////////////////////////////////

BOOL RandomItemParser::LoadScript(eSCRIPT_CODE script_code, BOOL is_reload)
{
    created_item_infos_.clear();

    const bool reload = (is_reload == TRUE);
    switch (script_code)
    {
    case SCRIPT_RANDOM_ITEM_VALUE:
        if (!FlowControl::FCAssert(value_manager_.LoadScript(*this, reload)))
        {
            return FALSE;
        }
        break;
    case SCRIPT_RANDOM_ITEM_OPTION_TYPE:
        if (!FlowControl::FCAssert(option_manager_.LoadScript(*this, reload)))
        {
            return FALSE;
        }
        break;
    case SCRIPT_RANDOM_ITEM_INFO:
        if (!FlowControl::FCAssert(item_manager_.LoadScript(*this, reload)))
        {
            return FALSE;
        }
        break;
    case SCRIPT_RANDOM_ITEM_KEY:
        if (!FlowControl::FCAssert(key_manager_.LoadScript(*this, reload)))
        {
            return FALSE;
        }
        break;
    default:
        return FALSE;
    }
    return TRUE;
}

bool RandomItemParser::EnableRandomize(const BASE_ITEMINFO& item_info) const
{
    return (item_manager_.GetItem(item_info.m_Code) != NULL);
}

RandomItemParser::OptionParam RandomItemParser::RandomizeOption(
    const RandomValueTypeGrade::Value value_type_grade, const BASE_ITEMINFO& item_info) const
{
    OptionParam param;

    const RandomItem* const random_item = item_manager_.GetItem(item_info.m_Code);
    if (random_item == NULL)
    {
        return param;
    }

    STLX_HASH_SET<RandomOptionPart::Kind> sel_option_kinds;

    const RandomItem::Options& options = random_item->options;
    for (RandomItem::Options::const_iterator it = options.begin(); 
        it != options.end(); ++it)
    {
        const RandomOption* const option = *it;
        if (option == NULL)
        {
            return param;
        }
        
        RatioSelector<RandomOptionPart::Order, RandomOptionPart::Ratio> option_part_selector;
        const RandomOption::Parts& option_parts = option->parts;
        for (RandomOption::Parts::const_iterator it2 = option_parts.begin(); 
            it2 != option_parts.end(); ++it2)
        {
            const RandomOptionPart& option_part = *it2;
            if (!option_part_selector.Add(option_part.order, option_part.ratio))
            {
                return param;
            }
        }
        
        const RandomOptionPart* option_part(NULL);
        while (option_part_selector.GetCount() > 0)
        {
            const RandomOptionPart::Ratio ratio = rand() % option_part_selector.GetTotalRatio();
            RandomOptionPart::Order part_order = 0;
            if (!option_part_selector.Select(ratio, part_order))
            {
                return param;
            }
            option_part = option->GetPart(part_order);
            if (option_part == NULL)
            {
                return param;
            }
            if (option_part->is_empty)
            {
                break;
            }
            if (sel_option_kinds.find(option_part->kind) == sel_option_kinds.end())
            {
                sel_option_kinds.insert(option_part->kind);
                break;
            }
            if (!option_part_selector.Remove(part_order))
            {
                return param;
            }
            option_part = NULL;
        }
        if (option_part == NULL)
        {
            return param;
        }

        const RandomOption::Index option_index = option->index;
        const RandomOptionPart::Order option_order = option_part->order;
        RandomValue::Index value_index = 0;
        RandomValuePart::Order value_order = 0;
        if (option_part->is_empty == false)
        {
            const RandomValue* const random_value = option_part->GetValue(value_type_grade);
            if (random_value == NULL)
            {
                return param;
            }
            value_index = random_value->index;
            const RandomValuePart* const value_part = random_value->SelectPart();
            if (value_part == NULL)
            {
                return param;
            }
            value_order = value_part->order;
        }

        const RandomKey* const random_key = 
            key_manager_.GetKey(option_index, option_order, value_index, value_order);
        if (random_key == NULL)
        {
            return param;
        }
        if (param.size >= _countof(param.keys))
        {
            return param;
        }
        param.keys[param.size] = random_key->key;
        ++param.size;
    }
    
    param.is_valid = true;
    return param;
}

namespace local
{
    void ClearOptionPart(BASE_ITEMINFO& OUT item_info)
    {
        ::ZeroMemory(item_info.m_byOptionExEffect, sizeof(item_info.m_byOptionExEffect));
        ::ZeroMemory(item_info.m_wOptionIndex, sizeof(item_info.m_wOptionIndex));
        ::ZeroMemory(item_info.m_byOptionType, sizeof(item_info.m_byOptionType));
        ::ZeroMemory(item_info.m_iOptionValue, sizeof(item_info.m_iOptionValue));
    }
}

bool RandomItemParser::CreateOption(OptionParam& INOUT param, BASE_ITEMINFO& OUT item_info) const
{
    if (param.is_valid == false || param.size == 0 || param.size > _countof(param.keys))
    {
        return false;
    }

    local::ClearOptionPart(item_info);

    uint8_t not_empty_option_size = 0;
    for (uint8_t i = 0; i < param.size; ++i)
    {
        const OptionParam::Key param_key = param.keys[i];
        const RandomKey* const random_key = key_manager_.GetKey(param_key);
        if (random_key == NULL)
        {
            local::ClearOptionPart(item_info);
            return false;
        }
        const RandomOptionPart* const option_part = random_key->option_part;
        if (option_part == NULL)
        {
            local::ClearOptionPart(item_info);
            return false;
        }
        if (option_part->is_empty)
        {
            continue;
        }
        const RandomValuePart* const value_part = random_key->value_part;
        if (value_part == NULL)
        {
            local::ClearOptionPart(item_info);
            return false;
        }
        if (ns_formula::CheckOptionValidity(
            option_part->kind, option_part->value_type, value_part->my_value) == false)
        {
            local::ClearOptionPart(item_info);
            return false;
        }
        item_info.m_byOptionExEffect[not_empty_option_size] = option_part->active_type;
        item_info.m_wOptionIndex[not_empty_option_size] = option_part->kind;
        item_info.m_byOptionType[not_empty_option_size] = option_part->value_type;
        item_info.m_iOptionValue[not_empty_option_size] = value_part->my_value;
        param.grades[not_empty_option_size] = value_part->grade;
        ++not_empty_option_size;
    }

    return true;
}

namespace local
{
    bool IsValidItemState(const SCItemSlot& item)
    {
#ifdef _SERVER
        return (!item.IsLocked() && 
            !item.IsBlocked() && 
            !item.IsUserBlocked() && 
            item.IsContainValidItem());
#else
        return (!item.IsBlocked() && !item.IsUserBlocked() && item.IsContainValidItem());
#endif
    }
}

bool RandomItemParser::EnableItemRandomize( 
    const SCItemSlotContainer& inventory, 
    const POSTYPE request_item_pos, 
    const POSTYPE target_item_pos, 
    SCItemSlot*& OUT _request_item, 
    SCItemSlot*& OUT _target_item) const
{
    if (inventory.GetSlotType() != SI_INVENTORY)
    {
        return false;
    }
    if (inventory.ValidStateByConst() == false)
    {
        return false;
    }
    if (inventory.ValidPos(request_item_pos) == false)
    {
        return false;
    }
    if (inventory.ValidPos(target_item_pos) == false)
    {
        return false;
    }
    SCItemSlot* const request_item = 
        static_cast<SCItemSlot*>(inventory.GetValidSlotPtr(NULL, request_item_pos));
    if (request_item == NULL)
    {
        return false;
    }
    if (local::IsValidItemState(*request_item) == false)
    {
        return false;
    }
    const BASE_ITEMINFO* const request_item_info = request_item->GetItemInfo(); 
    if (request_item_info->m_wType != eITEMTYPE_RANDOMIZE_ITEM)
    {
        return false;
    }
    if (request_item_info->m_wSubType < RandomValueTypeGrade::kMinGrade ||
        request_item_info->m_wSubType > RandomValueTypeGrade::kMaxGrade)
    {
        return false;
    }
    SCItemSlot* const target_item = 
        static_cast<SCItemSlot*>(inventory.GetValidSlotPtr(NULL, target_item_pos));
    if (target_item == NULL)
    {
        return false;
    }
    if (local::IsValidItemState(*target_item) == false)
    {
        return false;
    }
    if (target_item->EnableRandomize() == false)
    {
        return false;
    }
    _request_item = request_item;
    _target_item = target_item;
    return true;
}

bool RandomItemParser::GetItemRandomizeRequest( 
    const SCItemSlotContainer& inventory, 
    const POSTYPE request_item_pos, 
    const POSTYPE target_item_pos, 
    MSG_CG_ITEM_RANDOMIZE_SYN& OUT request) const
{
    SCItemSlot* request_item(NULL);
    SCItemSlot* target_item(NULL);
    if (EnableItemRandomize(inventory, request_item_pos, target_item_pos, request_item, target_item) == false)
    {
        return false;
    }
    request.Init();
    request.request_item_pos = request_item_pos;
    request.target_item_pos = target_item_pos;
    return true;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// REGION : Private
////////////////////////////////////////////////////////////////////////////////////////////////////

bool RandomItemParser::RandomValueManager::LoadScript( 
    const RandomItemParser& parser, const bool is_reload)
{
    if (is_reload)
    {
        random_values_.clear();
    }

    RandomValue::Index prev_index = 0;

    const int row_size = parser.GetRowSize();
    for (int row = 0; row < row_size; ++row)
    {
        const RandomValue::Index index = parser.GetDataWORD(_T("Index"), row);
        if (index < RandomValue::kMinIndex || index > RandomValue::kMaxIndex)
        {
            SUNLOG(
                eCRITICAL_LOG, 
                _T("|%s|%d|Index is not valid.|Row = %d, Index = %d|"), 
                __FUNCTION__, 
                __LINE__, 
                row, 
                index
            );
            assert(false);
            return false;
        }
        const RandomValuePart::Order order = parser.GetDataBYTE(_T("Order"), row);
        if (order < RandomValuePart::kMinOrder || order > RandomValuePart::kMaxOrder)
        {
            SUNLOG(
                eCRITICAL_LOG, 
                _T("|%s|%d|Order is not valid.|Row = %d, Order = %d|"), 
                __FUNCTION__, 
                __LINE__, 
                row, 
                order
            );
            assert(false);
            return false;
        }
        const RandomValuePart::Value my_value = parser.GetDataInt(_T("Value"), row);
        if (my_value < RandomValuePart::kMinValue || my_value > RandomValuePart::kMaxValue)
        {
            SUNLOG(
                eCRITICAL_LOG, 
                _T("|%s|%d|Value is not valid.|Row = %d, Value = %d|"), 
                __FUNCTION__, 
                __LINE__, 
                row, 
                my_value
            );
            assert(false);
            return false;
        }
        const RandomValuePart::Ratio ratio = parser.GetDataWORD(_T("Ratio"), row);
        if (ratio < RandomValuePart::kMinRatio || ratio > RandomValuePart::kMaxRatio)
        {
            SUNLOG(
                eCRITICAL_LOG, 
                _T("|%s|%d|Ratio is not valid.|Row = %d, Ratio = %d|"), 
                __FUNCTION__, 
                __LINE__, 
                row, 
                ratio
            );
            assert(false);
            return false;
        }
        const uint8_t grade = parser.GetDataBYTE(_T("Grade"), row);
        if (grade < RandomValueGrade::kMinGrade || grade > RandomValueGrade::kMaxGrade)
        {
            SUNLOG(
                eCRITICAL_LOG, 
                _T("|%s|%d|Grade is not valid.|Row = %d, Grade = %d|"), 
                __FUNCTION__, 
                __LINE__, 
                row, 
                grade
            );
            assert(false);
            return false;
        }
        const bool is_new_index = (prev_index != index);
        if (is_new_index)
        {
            RandomValues::const_iterator prev_values_it = random_values_.find(prev_index);
            if (prev_values_it != random_values_.end())
            {
                const RandomValue& prev_random_value = prev_values_it->second;
                const size_t part_size = prev_random_value.parts.size();
                if (part_size < RandomValuePart::kMinOrder || part_size > RandomValuePart::kMaxOrder)
                {
                    SUNLOG(
                        eCRITICAL_LOG, 
                        _T("|%s|%d|Value part size is not valid.|Index = %d, Size = %d|"), 
                        __FUNCTION__, 
                        __LINE__, 
                        prev_index, 
                        part_size
                    );
                    assert(false);
                    return false;
                }
                const RandomValuePart::Ratio ratio_sum = 
                    prev_random_value.part_selector.GetTotalRatio();
                if (ratio_sum != RandomValuePart::kMaxRatio)
                {
                    SUNLOG(
                        eCRITICAL_LOG, 
                        _T("|%s|%d|Ratio sum is not valid.|Index = %d, Sum = %d|"), 
                        __FUNCTION__, 
                        __LINE__, 
                        prev_index, 
                        ratio_sum
                    );
                    assert(false);
                    return false;
                }
            }
            if (random_values_.find(index) != random_values_.end())
            {
                SUNLOG(
                    eCRITICAL_LOG, 
                    _T("|%s|%d|Index is already exist.|Row = %d, Index = %d|"), 
                    __FUNCTION__, 
                    __LINE__, 
                    row, 
                    index
                );
                assert(false);
                return false;
            }
            RandomValue random_value;
            random_value.index = index;
            random_values_[index] = random_value;
            prev_index = index;
        }
        RandomValues::iterator values_it = random_values_.find(index);
        if (values_it == random_values_.end())
        {
            SUNLOG(
                eCRITICAL_LOG, 
                _T("|%s|%d|Index is not find.|Row = %d, Index = %d|"), 
                __FUNCTION__, 
                __LINE__, 
                row, 
                index
            );
            assert(false);
            return false;
        }
        RandomValue& random_value = values_it->second; 
        RandomValue::Parts& parts = random_value.parts;
        if (order != (parts.size()+1))
        {
            SUNLOG(
                eCRITICAL_LOG, 
                _T("|%s|%d|Order is not valid.|Row = %d, Order = %d|"), 
                __FUNCTION__, 
                __LINE__, 
                row, 
                order
            );
            assert(false);
            return false;
        }
        RandomValuePart part;
        part.order = order;
        part.my_value = my_value;
        part.ratio = ratio;
        part.grade = static_cast<RandomValueGrade::Value>(grade);
        parts.push_back(part);
        if (!random_value.part_selector.Add(order, ratio))
        {
            SUNLOG(
                eCRITICAL_LOG, 
                _T("|%s|%d|Fail to add ratio.|Row = %d, Index = %d, Order = %d|"), 
                __FUNCTION__, 
                __LINE__, 
                row, 
                index, 
                order
            );
            assert(false);
            return false;
        }
    }

    return true;
}

bool RandomItemParser::RandomOptionManager::LoadScript( 
    const RandomItemParser& parser, const bool is_reload)
{
    if (is_reload)
    {
        random_options_.clear();
    }

    RandomValue::Index prev_index = 0;

    const int row_size = parser.GetRowSize();
    for (int row = 0; row < row_size; ++row)
    {
        const RandomOption::Index index = parser.GetDataWORD(_T("Index"), row);
        if (index < RandomOption::kMinIndex || index > RandomOption::kMaxIndex)
        {
            SUNLOG(
                eCRITICAL_LOG, 
                _T("|%s|%d|Index is not valid.|Row = %d, Index = %d|"), 
                __FUNCTION__, 
                __LINE__, 
                row, 
                index
            );
            assert(false);
            return false;
        }
        const RandomOptionPart::Order order = parser.GetDataBYTE(_T("Order"), row);
        if (order < RandomOptionPart::kMinOrder || order > RandomOptionPart::kMaxOrder)
        {
            SUNLOG(
                eCRITICAL_LOG, 
                _T("|%s|%d|Order is not valid.|Row = %d, Order = %d|"), 
                __FUNCTION__, 
                __LINE__, 
                row, 
                order
            );
            assert(false);
            return false;
        }
        const RandomOptionPart::ActiveType active_type = parser.GetDataBYTE(_T("ExerciseEffect"), row);
        if (active_type < eITEM_EFFECT_NONE || active_type >= eITEM_EFFECT_UPPERBOUND)
        {
            SUNLOG(
                eCRITICAL_LOG, 
                _T("|%s|%d|ExerciseEffect is not valid.|Row = %d, ExerciseEffect = %d|"), 
                __FUNCTION__, 
                __LINE__, 
                row, 
                active_type
            );
            assert(false);
            return false;
        }
        const RandomOptionPart::Kind kind = parser.GetDataWORD(_T("OptionKind"), row);
        const RandomOptionPart::ValueType value_type = parser.GetDataBYTE(_T("OptionValueType"), row);
        const bool is_empty_option = (active_type == eITEM_EFFECT_NONE);
        if (is_empty_option)
        {
            if (kind != 0)
            {
                SUNLOG(
                    eCRITICAL_LOG, 
                    _T("|%s|%d|OptionKind is not 0.|Row = %d, OptionKind = %d|"), 
                    __FUNCTION__, 
                    __LINE__, 
                    row, 
                    kind
                );
                assert(false);
                return false;
            }
            if (value_type != 0)
            {
                SUNLOG(
                    eCRITICAL_LOG, 
                    _T("|%s|%d|OptionValueType is not 0.|Row = %d, OptionValueType = %d|"), 
                    __FUNCTION__, 
                    __LINE__, 
                    row, 
                    value_type
                );
                assert(false);
                return false;
            }
        }
        else
        {
            const eATTR_TYPE attr_kind = g_ITEM_OPTION[kind];
            if (kind >= _countof(g_ITEM_OPTION) || attr_kind == eATTR_TYPE_INVALID)
            {
                SUNLOG(
                    eCRITICAL_LOG, 
                    _T("|%s|%d|OptionKind is not valid.|Row = %d, OptionKind = %d|"), 
                    __FUNCTION__, 
                    __LINE__, 
                    row, 
                    kind
                );
                assert(false);
                return false;
            }
            if (value_type != VALUE_TYPE_VALUE && 
                value_type != VALUE_TYPE_PERCENT_PER_CUR &&
                value_type != VALUE_TYPE_PERCENT_PER_MAX &&
                value_type != VALUE_TYPE_RATIO_VALUE)
            {
                SUNLOG(
                    eCRITICAL_LOG, 
                    _T("|%s|%d|OptionValueType is not valid..|Row = %d, OptionValueType = %d|"), 
                    __FUNCTION__, 
                    __LINE__, 
                    row, 
                    value_type
                );
                assert(false);
                return false;
            }
        }
        const RandomValue* random_values[RandomOptionPart::kValueGradeSize];
        ::ZeroMemory(random_values, sizeof(random_values));
        RandomValue::Index value_indexes[_countof(random_values)];
        ::ZeroMemory(value_indexes, sizeof(value_indexes));
        value_indexes[RandomOptionPart::kValueGrade0] = parser.GetDataWORD(_T("RandomValueIndex1"), row);
        value_indexes[RandomOptionPart::kValueGrade1] = parser.GetDataWORD(_T("RandomValueIndex2"), row);
        for (int i = 0; i < _countof(value_indexes); ++i)
        {
            const RandomValue::Index value_index = value_indexes[i];
            if (is_empty_option)
            {
                if (value_index != 0)
                {
                    SUNLOG(
                        eCRITICAL_LOG, 
                        _T("|%s|%d|RandomValueIndex is not valid.|Row = %d, RandomValueIndex = %d|"), 
                        __FUNCTION__, 
                        __LINE__, 
                        row, 
                        value_index
                    );
                    assert(false);
                    return false;
                }
                continue;
            }
            const RandomValue* const random_value = (parser.value_manager_).GetValue(value_index);
            if (random_value == NULL)
            {
                SUNLOG(
                    eCRITICAL_LOG, 
                    _T("|%s|%d|RandomValueIndex is not valid.|Row = %d, RandomValueIndex = %d|"), 
                    __FUNCTION__, 
                    __LINE__, 
                    row, 
                    value_index
                );
                assert(false);
                return false;
            }
            const RandomValue::Parts& random_value_parts = random_value->parts; 
            RandomValue::Parts::const_iterator value_parts_it = random_value_parts.begin();
            for (; value_parts_it != random_value_parts.end(); ++value_parts_it)
            {
                const RandomValuePart& part = *value_parts_it;
                if (ns_formula::CheckOptionValidity(kind, value_type, part.my_value) == false)
                {
                    SUNLOG(
                        eCRITICAL_LOG, 
                        _T("|%s|%d|RandomValue is not valid.|Row = %d, RandomValueIndex = %d, RandomValueOrder = %d, Value = %d|"), 
                        __FUNCTION__, 
                        __LINE__, 
                        row, 
                        value_index, 
                        part.order, 
                        part.my_value
                    );
                    assert(false);
                    return false;
                }
            }
            random_values[i] = random_value;
        }
        const RandomOptionPart::Ratio ratio = parser.GetDataWORD(_T("Ratio"), row);
        if (ratio < RandomOptionPart::kMinRatio || ratio > RandomOptionPart::kMaxRatio)
        {
            SUNLOG(
                eCRITICAL_LOG, 
                _T("|%s|%d|Ratio is not valid.|Row = %d, Ratio = %d|"), 
                __FUNCTION__, 
                __LINE__, 
                row, 
                ratio
            );
            assert(false);
            return false;
        }
        const bool is_new_index = (prev_index != index);
        if (is_new_index)
        {
            RandomOptions::const_iterator prev_options_it = random_options_.find(prev_index);
            if (prev_options_it != random_options_.end())
            {
                const RandomOption& prev_random_option = prev_options_it->second;
                const RandomOptionPart::Ratio ratio_sum = 
                    prev_random_option.part_selector.GetTotalRatio();
                if (ratio_sum != RandomOptionPart::kMaxRatio)
                {
                    SUNLOG(
                        eCRITICAL_LOG, 
                        _T("|%s|%d|Ratio sum is not valid.|Index = %d, Sum = %d|"), 
                        __FUNCTION__, 
                        __LINE__, 
                        prev_index, 
                        ratio_sum
                    );
                    assert(false);
                    return false;
                }
            }
            if (random_options_.find(index) != random_options_.end())
            {
                SUNLOG(
                    eCRITICAL_LOG, 
                    _T("|%s|%d|Index is already exist.|Row = %d, Index = %d|"), 
                    __FUNCTION__, 
                    __LINE__, 
                    row, 
                    index
                );
                assert(false);
                return false;
            }
            RandomOption random_option;
            random_option.index = index;
            random_options_[index] = random_option;
            prev_index = index;
        }
        RandomOptions::iterator options_it = random_options_.find(index);
        if (options_it == random_options_.end())
        {
            SUNLOG(
                eCRITICAL_LOG, 
                _T("|%s|%d|Index is not find.|Row = %d, Index = %d|"), 
                __FUNCTION__, 
                __LINE__, 
                row, 
                index
            );
            assert(false);
            return false;
        }
        RandomOption& random_option = options_it->second;
        RandomOption::Parts& parts = random_option.parts;
        if (order != (parts.size()+1))
        {
            SUNLOG(
                eCRITICAL_LOG, 
                _T("|%s|%d|Order is not valid.|Row = %d, Order = %d|"), 
                __FUNCTION__, 
                __LINE__, 
                row, 
                order
            );
            assert(false);
            return false;
        }
        if (is_empty_option != false)
        {
            for (RandomOption::Parts::const_iterator it = parts.begin(); it != parts.end(); ++it)
            {
                const RandomOptionPart& part = *it;
                if (part.kind == kind)
                {
                    SUNLOG(
                        eCRITICAL_LOG, 
                        _T("|%s|%d|OptionKind is already exist.|Row = %d, OptionKind = %d|"), 
                        __FUNCTION__, 
                        __LINE__, 
                        row, 
                        kind
                    );
                    assert(false);
                    return false;
                }
            }
        }
        RandomOptionPart part;
        part.order = order;
        part.active_type = active_type;
        part.kind = kind;
        part.value_type = value_type;
        ::CopyMemory(part.random_values, random_values, sizeof(random_values));
        part.ratio = ratio;
        part.is_empty = is_empty_option;
        parts.push_back(part);
        if (!random_option.part_selector.Add(order, ratio))
        {
            SUNLOG(
                eCRITICAL_LOG, 
                _T("|%s|%d|Fail to add ratio.|Row = %d, Index = %d, Order = %d|"), 
                __FUNCTION__, 
                __LINE__, 
                row, 
                index, 
                order
            );
            assert(false);
            return false;
        }
    }

    return true;
}

bool RandomItemParser::RandomItemManager::LoadScript( 
    const RandomItemParser& parser, const bool is_reload)
{
    if (is_reload)
    {
        random_items_.clear();
    }

    const TCHAR* option_index_name[RandomItem::kMaxOptionSize] = { 
        _T("RandomOptionTypeIndex1"), 
        _T("RandomOptionTypeIndex2"), 
        _T("RandomOptionTypeIndex3"), 
        _T("RandomOptionTypeIndex4"), 
        _T("RandomOptionTypeIndex5")
    };

    const int row_size = parser.GetRowSize();
    for (int row = 0; row < row_size; ++row)
    {
        const RandomItem::ItemCode item_code = parser.GetDataDWORD(_T("ItemCode"), row);
        if ((ItemInfoParser::Instance())->GetItemInfo(item_code) == NULL)
        {
            SUNLOG(
                eCRITICAL_LOG, 
                _T("|%s|%d|ItemCode is not valid.|Row = %d, ItemCode = %d|"), 
                __FUNCTION__, 
                __LINE__, 
                row, 
                item_code
            );
            assert(false);
            return false;
        }
        
        RandomItem random_item;
        random_item.item_code = item_code;

        for (int i = 0; i < _countof(option_index_name); ++i)
        {
            const RandomOption::Index option_index = parser.GetDataWORD(option_index_name[i], row);
            if (option_index == 0)
            {
                break;
            }
            const RandomOption* const option = (parser.option_manager_).GetOption(option_index);
            if (option == NULL)
            {
                SUNLOG(
                    eCRITICAL_LOG, 
                    _T("|%s|%d|RandomOptionTypeIndex%d is not valid.|Row = %d, RandomOptionTypeIndex = %d|"), 
                    __FUNCTION__, 
                    __LINE__, 
                    i, 
                    row, 
                    option_index
                );
                assert(false);
                return false;
            }
            (random_item.options).push_back(option);
        }
        if ((random_item.options).empty())
        {
            SUNLOG(
                eCRITICAL_LOG, 
                _T("|%s|%d|OptionTypeIndex is empty.|Row = %d, ItemCode = %d|"), 
                __FUNCTION__, 
                __LINE__, 
                row, 
                item_code
            );
            assert(false);
            return false;
        }
        if (random_items_.find(item_code) != random_items_.end())
        {
            SUNLOG(
                eCRITICAL_LOG, 
                _T("|%s|%d|ItemCode is already exist.|Row = %d, ItemCode = %d|"), 
                __FUNCTION__, 
                __LINE__, 
                row, 
                item_code
            );
            assert(false);
            return false;
        }
        random_items_[item_code] = random_item;
    }

    return true;
}

bool RandomItemParser::RandomKeyManager::LoadScript( 
    RandomItemParser& parser, const bool is_reload)
{
    if (is_reload)
    {
        key_datas_.clear();
        random_keys_.clear();
    }

    const int row_size = parser.GetRowSize();
    for (int row = 0; row < row_size; ++row)
    {
        const RandomKey::Key key = parser.GetDataWORD(_T("Key"), row);
        if (key < RandomKey::kMinKey || key > RandomKey::kMaxKey || 
            random_keys_.find(key) != random_keys_.end())
        {
            SUNLOG(
                eCRITICAL_LOG, 
                _T("|%s|%d|Key is not valid.|Row = %d, Key = %d|"), 
                __FUNCTION__, 
                __LINE__, 
                row, 
                key
            );
            assert(false);
            return false;
        }
        const RandomOption::Index option_index = parser.GetDataWORD(_T("OptionTypeIndex"), row);
        const RandomOption* const option = (parser.option_manager_).GetOption(option_index);
        if (option == NULL)
        {
            SUNLOG(
                eCRITICAL_LOG, 
                _T("|%s|%d|OptionTypeIndex is not valid.|Row = %d, OptionTypeIndex = %d|"), 
                __FUNCTION__, 
                __LINE__, 
                row, 
                option_index
            );
            assert(false);
            return false;
        }
        const RandomOptionPart::Order option_order = parser.GetDataBYTE(_T("OptionTypeOrder"), row);
        const RandomOptionPart* const option_part = option->GetPart(option_order);
        if (option_part == NULL)
        {
            SUNLOG(
                eCRITICAL_LOG, 
                _T("|%s|%d|OptionTypeOrder is not valid.|Row = %d, OptionTypeOrder = %d|"), 
                __FUNCTION__, 
                __LINE__, 
                row, 
                option_order
            );
            assert(false);
            return false;
        }
        const RandomValue::Index value_index = parser.GetDataWORD(_T("ValueIndex"), row);
        const RandomValuePart::Order value_order = parser.GetDataBYTE(_T("ValueOrder"), row);
        const RandomValuePart* value_part(NULL);
        if (option_part->is_empty == false)
        {
            const RandomValue* random_value = option_part->GetValue(RandomValueTypeGrade::kGrade1);
            if (random_value == NULL)
            {
                random_value = option_part->GetValue(RandomValueTypeGrade::kGrade2);
                if (random_value == NULL)
                {
                    SUNLOG(
                        eCRITICAL_LOG, 
                        _T("|%s|%d|ValueIndex is not valid.|Row = %d, ValueIndex = %d|"), 
                        __LINE__, 
                        __FUNCTION__, 
                        row, 
                        value_index
                    );
                }
                assert(false);
                return false;
            }
            value_part = random_value->GetPart(value_order);
            if (value_part == NULL)
            {
                SUNLOG(
                    eCRITICAL_LOG, 
                    _T("|%s|%d|ValueOrder is not valid.|Row = %d, ValueOrder = %d|"), 
                    __FUNCTION__, 
                    __LINE__, 
                    row, 
                    value_order
                );
                assert(false);
                return false;
            }
        }
        const KeyData key_data = CreateKeyData(option_index, option_order, value_index, value_order);
        if (key_datas_.find(key_data) != key_datas_.end())
        {
            SUNLOG(
                eCRITICAL_LOG, 
                _T("|%s|%d|KeyData is already exist.|Row = %d, KeyData = %I64|"), 
                __FUNCTION__, 
                __LINE__, 
                row, 
                key_data
            );
            assert(false);
            return false;
        }

        RandomKey random_key;
        random_key.key = key;
        random_key.option_part = option_part;
        random_key.value_part = value_part;
        random_keys_[key] = random_key;

        RandomKeys::const_iterator it = random_keys_.find(key);
        if (it == random_keys_.end())
        {
            SUNLOG(
                eCRITICAL_LOG, 
                _T("|%s|%d|Fail to insert random key.|Row = %d, Key = %d|"), 
                __FUNCTION__, 
                __LINE__, 
                row, 
                key
            );
            assert(false);
            return false;
        }
        const RandomKey* const inserted_random_key = &(it->second);
        key_datas_[key_data] = inserted_random_key;
    }

    if (InsertKeysToOption(parser) == false)
    {
        return false;
    }

    return true;
}

bool RandomItemParser::RandomKeyManager::InsertKeysToOption(RandomItemParser& parser) const
{
    RandomItemManager::RandomItems& random_items = parser.item_manager_.random_items_;
    for (RandomItemManager::RandomItems::iterator it = random_items.begin(); 
        it != random_items.end(); ++it)
    {
        RandomItem& random_item = it->second;
        
        // item code
        const RandomItem::ItemCode item_code = random_item.item_code;

        const RandomItem::Options& random_options = random_item.options;
        for (RandomItem::Options::const_iterator it2 = random_options.begin(); 
            it2 != random_options.end(); ++it2)
        {
            RandomOption* const random_option = const_cast<RandomOption*>(*it2);
            if (random_option == NULL)
            {
                SUNLOG(
                    eCRITICAL_LOG, 
                    _T("|%s|%d|RandomOption is null.|ItemCode = %d|"), 
                    __FUNCTION__, 
                    __LINE__, 
                    random_item.item_code
                );
                return false;
            }

            // option index
            const RandomOption::Index option_index = random_option->index;

            const RandomOption::Parts& option_parts = random_option->parts;
            for (RandomOption::Parts::const_iterator it3 = option_parts.begin(); 
                it3 != option_parts.end(); ++it3)
            {
                const RandomOptionPart& option_part = *it3;
                
                // option order
                const RandomOptionPart::Order option_order = option_part.order;
                
                if (option_part.is_empty)
                {
                    const int value_grade = 0; // value grade
                    const RandomValue::Index value_index = 0; // value index
                    const RandomValuePart::Order value_order = 0; // value order

                    const RandomKey* const random_key = 
                        GetKey(option_index, option_order, value_index, value_order);
                    if (random_key == NULL)
                    {
                        SUNLOG(
                            eCRITICAL_LOG, 
                            _T("|%s|%d|RandomKey is not registered.|ItemCode = %d, OptionIndex = %d, OptionOrder = %d, ValueGrade = %d, ValueIndex = %d, ValueOrder = %d|"), 
                            __FUNCTION__, 
                            __LINE__, 
                            item_code, 
                            option_index, 
                            option_order, 
                            value_grade, 
                            value_index, 
                            value_order
                        );
                        continue;
                        //return false;
                    }
                    random_option->keys[random_key->key] = random_key;
                    continue;
                }
                for (int i = 0; i < _countof(option_part.random_values); ++i)
                {
                    const RandomValue* const random_value = option_part.random_values[i];
                    if (random_value == NULL)
                    {
                        SUNLOG(
                            eCRITICAL_LOG, 
                            _T("|%s|%d|RandomValue is null.|OptionIndex = %d, OptionOrder = %d|"), 
                            __FUNCTION__, 
                            __LINE__, 
                            option_index, 
                            option_order
                        );
                        return false;
                    }
                    
                    // value grade
                    const int value_grade = i;

                    // value index
                    const RandomValue::Index value_index = random_value->index;

                    const RandomValue::Parts& value_parts = random_value->parts;
                    for (RandomValue::Parts::const_iterator it4 = value_parts.begin();
                        it4 != value_parts.end(); ++it4)
                    {
                        const RandomValuePart& value_part = *it4;

                        // value order
                        const RandomValuePart::Order value_order = value_part.order;

                        const RandomKey* const random_key = 
                            GetKey(option_index, option_order, value_index, value_order);
                        if (random_key == NULL)
                        {
                            SUNLOG(
                                eCRITICAL_LOG, 
                                _T("|%s|%d|RandomKey is not registered.|ItemCode = %d, OptionIndex = %d, OptionOrder = %d, ValueGrade = %d, ValueIndex = %d, ValueOrder = %d|"), 
                                __FUNCTION__, 
                                __LINE__, 
                                item_code, 
                                option_index, 
                                option_order, 
                                value_grade, 
                                value_index, 
                                value_order
                            );
                            continue;
                            //return false;
                        }
                        random_option->keys[random_key->key] = random_key;
                    }
                }
            }
        }
    }
    return true;
}