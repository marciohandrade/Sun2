#include "stdafx.h"
#include "SCItemStreamSerializer.h"
#include "SCItemSlot.h"
#include <ItemInfoParser.h>

#include <BitStream.hpp>

//==================================================================================================
// _NA002217_100728_EP2_PERK_SYSTEM_
const PerkDataList::eGroupCodeActiveSlot
PerkDataList::kGroupCodeActiveSlots[kNumberOfActiveSlots] =
{
    eActiveGroup01,
    eActiveGroup02,
    eActiveGroup03,
    eActiveGroup04,
    eActiveGroup10, // cash slot linker
};

// index
//   default = [00, 19] (20 slots)
//   cash    = [20, 23] ( 4 slots)
//   active  = [30, 40] ( 5 slots)
// |[30][31][32][33]|   |[39]|
// |-------------------------|
// |[00][01][02][03][04]|[20]|
// |[05][06][07][08][09]|[21]|
// |[10][11][12][13]    |[22]|
// |-------------------------|

const POSTYPE PerkDataList::kDefaultSlotPositions[kNumberOfDefaultSlots] =
{
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14
};

const POSTYPE PerkDataList::kCashSlotPositions[kNumberOfCashSlots] =
{
    20, 21, 22
};

const POSTYPE PerkDataList::kActiveSlotPositions[kNumberOfActiveSlots] =
{
    30, 31, 32, 33, 39
};

//==================================================================================================

//==================================================================================================

namespace nsSlot {
;

struct BindNode
{
    void* ptr_;
    BYTE type_size_;  //byte
    BYTE bit_range_;
    WORD bit_offset_; //byte*bits offset
};
//
//--------------------------------------------------------------------------------------------------

struct Binder
{
    typedef STLX_VECTOR<BindNode> BindList;
    // CHANGES: f101105.3L, modify upperbound lock value
    // because this stream contained the parse_type field already.
    enum { kMaxAlignBits = sizeof(OPTIONPART) * 8 };

    Binder(/*BYTE number_of_bytes_for_stream*/)
        : bind_lock_(true)
        , bit_sum_(0)
        //, upperbound_max_bits_(number_of_bytes_for_stream * 8 /*bits*/)
    {
        binding_params_.reserve(10);
    };

    ~Binder()
    {
    }

    void StartBinding()
    {
        if (bind_lock_ == 0) {
            __debugbreak();
        }
        bind_lock_ = 0;
        bit_sum_ = 0;
        if (binding_params_.empty() == false) {
            __debugbreak();
        }
    }

    void EndBinding()
    {
        bind_lock_ = true;
        //if (bit_sum_ != upperbound_max_bits_)
        if (bit_sum_ > kMaxAlignBits) {
            __debugbreak();
        }
        index_ = 0;
    }

    template <typename VALUE_T >
    bool BindVar(VALUE_T& val, const size_t used_bits)
    {
        if (bind_lock_ != 0) 
        {
            __debugbreak();
            return false;
        }
        const size_t size = sizeof(val);
        if (size > UCHAR_MAX) 
        {
            __debugbreak();
            return false;
        }
        if (used_bits > UCHAR_MAX) 
        {
            __debugbreak();
            return false;
        }
        BindNode node = { 0, };
        node.ptr_ = &val;
        node.type_size_ = BYTE(size);
        node.bit_range_ = BYTE(used_bits);
        node.bit_offset_ = WORD(bit_sum_);
        bit_sum_ += node.bit_range_;
        if (bit_sum_ > USHRT_MAX || bit_sum_ > kMaxAlignBits) 
        {
            __debugbreak();
            return false;
        }
        binding_params_.push_back(node);
        return true;
    }

    bool Encode(OPTIONPART* OUT to/*, eSerializeControl ctrl*/) const
    {
#if SUN_CODE_BACKUP
        DWORD* dest = (DWORD*)to;

        FOREACH_CONTAINER(const BindList::value_type& bind, binding_params_, BindList)
        {
            const BindNode& node = bind;
            //if ((node.control_ != eSCtrl_Unlimit) && (ctrl & node.control_) == 0)
            //    continue;

            const DWORD* base = (const DWORD*)node.ptr_;
            int range = node.bit_range_;
            int offset = node.bit_offset_;

            for (int index = 0; index < range; ++index, ++offset) {
                BOOLEAN set = !!(base[index >> 5] & (1UL << (index & 31)));
                if (set) {
                    dest[offset >> 5] |= (1UL << (offset & 31));
                }
            }
        }
#endif
        BitStream bit_stream(to, sizeof(*to), true);
        BindList::const_iterator it = binding_params_.begin();
        const BindList::const_iterator end = binding_params_.end();
        for (; it != end; ++it)
        {
            const BindNode& node = *it;
            bit_stream.WriteBits(node.ptr_, node.bit_range_, node.type_size_);
        };
        return true;
    }

    bool Decode(const OPTIONPART* IN from/*, eSerializeControl ctrl*/) const
    {
#if SUN_CODE_BACKUP
        const DWORD* base = (const DWORD*)from;

        FOREACH_CONTAINER(const BindList::value_type& bind, binding_params_, BindList)
        {
            const BindNode& node = bind;
            //if ((node.control_ != eSCtrl_Unlimit) && (ctrl & node.control_) == 0)
            //    continue;

            DWORD* dest = (DWORD*)node.ptr_;
            int range = node.bit_range_;
            int offset = node.bit_offset_;

            for (int index = 0; index < range; ++index, ++offset) {
                BOOLEAN set = !!(base[offset >> 5] & (1UL << (offset & 31)));
                if (set) {
                    dest[index >> 5] |= (1UL << (index & 31));
                }
            }
        }
#endif
        BitStream bit_stream(const_cast<OPTIONPART*>(from), sizeof(*from), false);
        BindList::const_iterator it = binding_params_.begin();
        const BindList::const_iterator end = binding_params_.end();
        for (; it != end; ++it)
        {
            const BindNode& node = *it;
            bit_stream.ReadBits(node.ptr_, node.bit_range_, node.type_size_);
        };
        return true;
    }

    //----------------------------------------------------------------------------------------------
    BOOLEAN bind_lock_;
    DWORD bit_sum_;
    DWORD index_;
    //const DWORD upperbound_max_bits_;
    BindList binding_params_;
};

//--------------------------------------------------------------------------------------------------

static struct ItemSerializeBinderInfo
{
    const SCItemSlot* first_;
    const Binder* binders_[ItemParseType::kUpperbound];
} binder_info_static_;

static SerializeItemOption serialize_block_static_;

#ifdef _NA000000_110218_RANK_REPORTER_IN_DBUNIT
// NOTE: f110221.1L, result of one item convert if it has version upgrade
static eRANK convert_report_about_rank_static_;
#endif

//==================================================================================================
//  자주 사용되는 아이템 분류 데이터
//  (NOTE) 아이템 관련 룰들을 최대한 끌어 모았다.
//  이곳 저곳에 흩어져 있는 것은 아무래도 찾기도 힘들고 문제가 생길 여지가 너무 많다.
//  true:  pass constraints
//  false: violation item constraints
bool
ItemRuleInfo::GetRuleInfo(ItemRuleInfo* OUT result, const BASE_ITEMINFO* base_item_info,
                          bool is_limited_set)
{
    ZeroMemory(result, sizeof(*result));
    if (base_item_info->m_Code == 0) {
        return false;
    }
    const ItemRuleInfo& item_rule_info = base_item_info->item_rule_info;
    bool is_valid = item_rule_info.is_valid_item != false;
    if (is_valid == false) {
        util::AssertCompileTimeVariableTypeEqual(*result, item_rule_info);
        return false;
    }
    //
    *result = item_rule_info;
    if (is_limited_set)
    {
        // 무기/장비만 허용
        if (item_rule_info.is_weapon || item_rule_info.is_armor) {
            result->is_limited = true;
            result->is_use_shared = true;
        }
    }
    return is_valid;
}

//  true:  pass constraints
//  false: violation item constraints
bool ItemRuleInfo::GetRuleInfo(
    ItemRuleInfo* OUT result, 
    const SCItemSlot& ref_item_slot,
    bool is_limited_set, 
    const ItemParseType::ValueType* const parent_parse_type)
{
    ZeroMemory(result, sizeof(*result));
    if (ref_item_slot.GetItemCode() == 0) 
    {
        return false;
    }
    const ItemRuleInfo& item_rule_info = ref_item_slot.GetItemRuleInfo();
    bool is_valid = item_rule_info.is_valid_item != false;
    if (is_valid == false) 
    {
        util::AssertCompileTimeVariableTypeEqual(*result, item_rule_info);
        return false;
    }
    //
    *result = item_rule_info;
    if (is_limited_set)
    {
        // 무기/장비만 허용
        if (item_rule_info.is_weapon || item_rule_info.is_armor) {
            result->is_limited = true;
            result->is_use_shared = true;
        }
    };
    // CHANGES: f110327.1L, changes charge_sub_type access field
    // to support various making item with timer
    //               if the basic rules has no timer facility
    if (parent_parse_type && result->charge_sub_type == eCHARGE_SUB_GENERAL)
    {
        const ItemParseType::ValueType parent_parse_type_value = *parent_parse_type;
        result->parse_type = parent_parse_type_value;
        // accetable = { sDefault -> sTime, sEquip -> sEquipTime, sEquipEtc -> sEquipEtcTime }
        // modifying field = { value, change_sub_type, func_attr };
        if (parent_parse_type_value == ItemParseType::kDefaultTime) 
        {
            // CHANGES: f110603.4L, changes the timer type whether the normal item enables to equip.
            result->charge_sub_type = (ref_item_slot.CanEquip()) ?
                eCHARGE_SUB_FIXED_AMOUNT_EX : eCHARGE_SUB_FIXED_AMOUNT;
            result->func_attr.value |= result->func_attr.eEnable_Timer;
        }
        else if (parent_parse_type_value == ItemParseType::kEquipTime)
        {
            result->charge_sub_type = eCHARGE_SUB_FIXED_AMOUNT_EX;
            result->func_attr.value |= result->func_attr.eEnable_Timer;
        }
        else if (parent_parse_type_value == ItemParseType::kEquipEtcsTime)
        {
            result->charge_sub_type = eCHARGE_SUB_FIXED_AMOUNT_EX;
            result->func_attr.value |= result->func_attr.eEnable_Timer;
        }
    }
    return is_valid;
}
//
//  true:  pass constraints
//  false: violation item constraints
// (CHANGES) (f100523.2L) prefetch the rule for a item information
bool ItemRuleInfo::PrefetchRuleInfo(ItemRuleInfo* OUT result, const BASE_ITEMINFO* item_info)
{
    ZeroMemory(result, sizeof(*result));
    if (item_info->m_Code == 0) {
        return false;
    }
    int counter_for_valid = 0;
    bool can_overlapped = true;
    bool enable_possession = false;
    bool use_shared_block = false;

    // EqPos 비교
    if (item_info->IsArmor()) {
        result->is_armor = true;
        can_overlapped = false;
        enable_possession = true;
        ++counter_for_valid;
    }
    else if (item_info->IsWeapon()) {
        result->is_weapon = true;
        can_overlapped = false;
        enable_possession = true;
        ++counter_for_valid;
    }
    // wType 비교 <- (CHANGES) add 'else' keyword, no effect
    else if (item_info->IsAccessory()) {
        result->is_accessory = true;
        can_overlapped = false;
        enable_possession = true;
        ++counter_for_valid;
    }
#ifdef _NA002823_WINGS_EQUIPMENTS_EXTENTION
    // CHANGES: f110624.3L
    else if (item_info->m_wType == eITEMTYPE_WINGS) {
        result->is_accessory_special = true;
        result->is_accessory_wings = true;
        can_overlapped = false;
        enable_possession = true;
        ++counter_for_valid;
    }
#endif
    else if (item_info->IsSpecialAccessory()) {
        result->is_accessory_special = true;
        can_overlapped = false;
        enable_possession = true;
        ++counter_for_valid;
    }
    else if (item_info->m_wType == eITEMTYPE_LOTTO) {
        result->is_lottery = true;
        can_overlapped = false;
        use_shared_block = true;
        ++counter_for_valid;
    }
    else if (item_info->m_wType == eITEMTYPE_PET) {
        result->is_pet = true;
        can_overlapped = false;
        use_shared_block = true;
        ++counter_for_valid;
    }
    else if (item_info->m_wType == eITEMTYPE_WAYPOINT) {
        result->is_waypoint = true;
        can_overlapped = false;
        use_shared_block = true;
        ++counter_for_valid;
    }
    else if (item_info->m_wType == eITEMTYPE_RIDER) {
        result->is_rider = true;
        can_overlapped = false;
        use_shared_block = true;
        ++counter_for_valid;
    }
    else if (item_info->m_wType == eITEMTYPE_RIDER_PARTS)
    {
        result->is_rider_parts = true;
        can_overlapped = false;
        use_shared_block = true;
        ++counter_for_valid;
    }
    //_NA_0_20100727_SOCKET_SYSTEM_CONVERT
    else if (item_info->m_wType == eITEMTYPE_SOCKET_ZARD)
    {
        result->is_extend_data = true;
        can_overlapped = false;
        use_shared_block = true;
        ++counter_for_valid;
    }
    //_NA_004035_20120227_COSTUME_ITEM
    else if (item_info->m_wType == eITEMTYPE_COSTUME)
    {
        result->is_costume = true;
        can_overlapped = false;
        use_shared_block = true;
        ++counter_for_valid;
    }
    //_NA_008610_20160104_ADD_DECORATECOSTUME
    else if (item_info->m_wType == eITEMTYPE_COSTUME_DECORATE)
    {
        result->is_deco_costume = true;
        can_overlapped = false;
        use_shared_block = true;
        ++counter_for_valid;
    }
    //_NA_006372_20130108_BADGE_ITEM
    else if (item_info->m_wType == eITEMTYPE_RENDER_EFFECT_ITEM)
    {
        result->is_toggle_item = true;
        can_overlapped = false;
        use_shared_block = true;
        ++counter_for_valid;
    }

    bool is_valid = FlowControl::FCAssert(counter_for_valid <= 1) != false;
    bool with_timer = false;
    if (is_valid) {
        result->charge_sub_type = eCHARGE_SUB_TYPE(item_info->m_ChargeSubType);
        // check constraints
        switch (result->charge_sub_type)
        {
        case eCHARGE_SUB_COORDINATE_EX:
            if (result->is_waypoint == false) {
                ASSERT(!"Invalid WayPoint Item");
                is_valid = false;
            }
            break;
        case eCHARGE_SUB_COORDINATE:
            if (counter_for_valid != 0) {
                ASSERT(!"Invalid Coordinate Item");
                is_valid = false;
            }
            else {
                result->is_coord = true;
                can_overlapped = false;
                use_shared_block = true;
            }
            break;
        case eCHARGE_SUB_FATE:
            if (result->is_armor == false) {
                ASSERT(!"Invalid Fate Item: must be armor");
                is_valid = false;
            }
            else {
                result->is_fate = true;
                can_overlapped = false;
                use_shared_block = true;
            }
            break;
        case eCHARGE_SUB_RIDER_WITH_PART:
        case eCHARGE_SUB_RIDER_WITH_PART_NOTIMER:
            if (result->is_rider == false && result->is_rider_parts == false) {
                ASSERT(!"Invalid Rider | Rider Parts Item");
                is_valid = false;
            }
            break;
        default:
            if (counter_for_valid == 0) {
                result->is_normal = true;
            }
            break;
        }

        // set timer
        switch (result->charge_sub_type)
        {
        case eCHARGE_SUB_FIXED_AMOUNT:
        case eCHARGE_SUB_FIXED_AMOUNT_EX:
        case eCHARGE_SUB_FIXED_QUANTITY:
        case eCHARGE_SUB_FIXED_AMOUNT_WAIT:
        case eCHARGE_SUB_FATE:
        case eCHARGE_SUB_COORDINATE_EX:
        case eCHARGE_SUB_RIDER_WITH_PART:
        case eCHARGE_SUB_RIDER_WITH_PART_NOTIMER: //for part control
        case eCHARGE_SUB_FIXED_AMOUNT_EX2:
            // by item info parser constraints
            //ASSERT(item_info->m_byDupNum == 1 && "ChargeItem with timer that can't overlapped");
            with_timer = true;
            can_overlapped = false;
            break;
        }
    }
/*
    if (is_valid && is_limited_set) {
        // 무기/장비만 허용
        if (result->is_weapon || result->is_armor) {
            result->is_limited = true;
            use_shared_block = true;
        }
    }
*/
    // - 실패시 그냥 초기화는... 음... need more thinking!
    if (is_valid == false) {
        ZeroMemory(result, sizeof(*result));
        return is_valid;
    }
    // TODO: 'is_enable_rankup' deletion reserved
    // rank up
    bool is_enable_rankup = false;
    if (result->is_weapon || result->is_armor || result->is_accessory)
    {
        if (item_info->IsChargeItem() == false &&
            // (CHANGES) (091123) (WAVERIX) 기존 엘리트(with rankuped item) 설정 유지 목적
            // (WARNING) 이제 로직|스크립트에서 룰을 제어해야 한다.
            /*item_info->IsElite() == false &&*/
            item_info->IsUnique() == false)
        {
            is_enable_rankup = true;
        }
    }

    // - set function attribute
    if (result->is_normal) {
        result->func_attr.value = ItemFuncAttr::kDefaultItem;
    } else if (result->is_limited) {
        result->func_attr.value = ItemFuncAttr::kLimitedItem;
    } else if (result->is_fate) {
        result->func_attr.value = ItemFuncAttr::kFateItem;
    } else if (result->is_weapon || result->is_armor) {
        result->func_attr.value = ItemFuncAttr::kEquipItem;
    } else if (result->is_accessory_wings) { // NOTE: f110624.3L
        result->func_attr.value = ItemFuncAttr::kEquipEtcsItem;
    } else if (result->is_accessory || result->is_accessory_special) {
        result->func_attr.value = ItemFuncAttr::kEquipEtcsItem;
    } else if (result->is_lottery) {
        result->func_attr.value = ItemFuncAttr::kLotteryItem;
    } else if (result->is_pet) {
        result->func_attr.value = ItemFuncAttr::kPetItem;
    } else if (result->is_waypoint) {
        result->func_attr.value = ItemFuncAttr::kWaypointItem;
    } else if (result->is_coord) {
        result->func_attr.value = ItemFuncAttr::kCordItem;
    //__NA1390_1391_090915_RIDING_SYSTEM_ITEM_PART__
    } else if (result->is_rider) {
        result->func_attr.value = ItemFuncAttr::kRiderItem;
    } else if (result->is_rider_parts) {
        result->func_attr.value = ItemFuncAttr::kRiderPartsItem;
    //_NA_0_20100727_SOCKET_SYSTEM_CONVERT
    } else if (result->is_extend_data) {
        result->func_attr.value = ItemFuncAttr::kExtendDataItem;
    };


    if (is_enable_rankup == false) {
        result->func_attr.value = (result->func_attr.value & ~result->func_attr.eEnable_Rank);
    }
    //
    if (with_timer) {
        result->func_attr.value |= result->func_attr.eEnable_Timer;
    };
    if (enable_possession != 0 && item_info->m_bPossessionItem != 0) {
        result->func_attr.value |= result->func_attr.eEnable_Possession;
    };
    //
    const bool is_unique = (item_info->IsUnique() != false);__TOUCHED(is_unique);
    //{ _NA000109_100427_ITEM_CUSTOMIZING_CONTENTS_
    // CHANGES: f101122.1L, changes the rule that the unique item can't etherealize.
    if ((with_timer == false) &&
        (result->is_weapon || result->is_armor))
    {
        result->func_attr.value |= result->func_attr.eEnable_Etherealize;
    }
    else if (with_timer == false && result->is_pet) {
        // CHANGES, f100903.2L, the pet item containing the expiration date can't etherealize.
        result->func_attr.value |= result->func_attr.eEnable_Etherealize;
    }
  #ifdef _NA002823_WINGS_EQUIPMENTS_EXTENTION
    // CHANGES: f110624.3L, the wings item enable the equipments extention like an armor
    else if ((result->is_accessory_wings)) {
        result->func_attr.value |= (result->func_attr.eEnable_Enchant |
                                    result->func_attr.eEnable_Divine);
    }
  #endif
    // added rule to support _NA001202_101104_ITEM_CUSTOMIZING_CONTENTS_ETHEREALIZABLE_LUCKYITEM
    else if (result->is_fate) {
        // CHANGES: f111104.2L, adopt a planner's request.
        // the lucky items can etherealize to the etheria not the skin
        result->func_attr.value |= result->func_attr.eEnable_Etherealize;
    }
    // added by _NA000960_101011_ITEM_CUSTOMIZING_CONTENTS_ADD_RULE_
    // NOTE: f101011.4L, adopt adding blocking rule to support request of the China.
    // the default rule enable etherealizing about pet, weapon and armor having no timer.
    // this script data control field modified enable state or not state after the custom rule.
    if (item_info->etherealization_mode_.value == item_info->etherealization_mode_.CantEtherealize)
    {
        result->func_attr.value &= ~result->func_attr.eEnable_Etherealize;
    };
    //}
    // skip false case
    if (can_overlapped) {
        can_overlapped = item_info->m_byDupNum > 1;
        result->is_can_overlap = can_overlapped;
    };
    if (use_shared_block) {
        result->is_use_shared = true;
    }
    // the contents of a _NA000109_100427_ITEM_CUSTOMIZING_CONTENTS_
    // whether can call about 'set' facilities related to options
    // however, this block is always true in this point that is applied after the OPTIONPART parsing
    if (result->is_etheria == false && result->is_shell == false) {
        result->is_editable = true;
    }
    result->is_valid_item = true;
    return is_valid;
}


}; //end of namespace

//==================================================================================================

nsSlot::SerializeItemOption*
SCItemSlot::_GetBindingBlock()
{
    return &nsSlot::serialize_block_static_;
}

#ifdef _NA000000_110218_RANK_REPORTER_IN_DBUNIT
// NOTE: f110221.1L, result of one item convert if it has version upgrade
void SCItemSlot::ClearConvertResult() {
    nsSlot::convert_report_about_rank_static_ = RANK_E;
}

bool SCItemSlot::GetConvertResult(eRANK* rank) {
    eRANK rank_value = nsSlot::convert_report_about_rank_static_;
    if (rank_value != RANK_E) {
        *rank = nsSlot::convert_report_about_rank_static_;
        return true;
    }
    return false;
}
    #define NOTIFY_RANKINFO_BEFORE_EP2_CONVERT(__rank__) \
        { nsSlot::convert_report_about_rank_static_ = static_cast<eRANK>(__rank__); }
#else
    #define NOTIFY_RANKINFO_BEFORE_EP2_CONVERT(__rank__)
#endif

int SCItemSlot::_SerializeUsedBits(const ItemParseType::ValueType parse_type)
{
    using namespace nsSlot;

    BOOST_STATIC_ASSERT(sizeof(ItemParseType::ValueType) == 1);
    enum { kDefaultSize = sizeof(ItemParseType::ValueType) * 8 };

    switch (parse_type)
    {
    case ItemParseType::kEmpty:
        return kDefaultSize;
    case ItemParseType::kCanNotParseOld:
        return sizeof(OPTIONPART) * 8;
    case ItemParseType::kCanNotParse:
        return kDefaultSize;
    }
    if (parse_type < ItemParseType::kUpperbound) 
    {
        const Binder* binder = binder_info_static_.binders_[parse_type];
        if (binder == NULL) 
        {
            return kDefaultSize;
        }
        return binder->bit_sum_;
    }
    return kDefaultSize;
}

bool SCItemSlot::_SerializeToItemStream(OPTIONPART* OUT serialized_stream, int query_version)
{
    using namespace nsSlot;

    ::ZeroMemory(serialized_stream, sizeof(*serialized_stream));
    
    switch (serialize_block_static_.Default.ParseType)
    {
    case ItemParseType::kEmpty:
        BOOST_STATIC_ASSERT(ItemParseType::kEmpty == 0);
        return true;
    case ItemParseType::kCanNotParseOld:
        serialized_stream->ParseType = BYTE(serialize_block_static_.BackupStream.ParseType);
        ::CopyMemory(
            serialized_stream->Variable, 
            &serialize_block_static_.BackupStream.stream,
            sizeof(serialized_stream->Variable)
        );
        BOOST_STATIC_ASSERT(sizeof(serialized_stream->ParseType) == sizeof(BYTE));
        BOOST_STATIC_ASSERT(sizeof(serialized_stream->Variable) == OptionPartCurVer::kBufferSize);
        return true;
    case ItemParseType::kCanNotParse:
        //....
        ASSERT(!"Not implementation currently");
        return false; // <좀 더 생각하고 처리할 것.
    }

    const ulong temp_parse_type = serialize_block_static_.Default.ParseType;
    if (temp_parse_type < ItemParseType::kUpperbound) 
    {
        const ItemParseType::ValueType parse_type = 
            static_cast<ItemParseType::ValueType>(temp_parse_type);
        
        // (f100803.3L) change a binder allocator to support multiple versions and types
        const Binder* binder = NULL;
        if (SerializeItemOption::GetBinder(parse_type, query_version, &binder, NULL) == false) 
        {
            // (NOTE) change to empty option
            return true;
        }
        serialized_stream->ParseType = parse_type;
        binder->Encode(serialized_stream);

#if WAVERIX_CODE_BACKUP
        SerializeItemOption testblock = serialize_block_static_;
        ZeroMemory(&serialize_block_static_, sizeof(serialize_block_static_));
        binder->Decode(serialized_stream);

        ASSERT(memcmp(&serialize_block_static_, &testblock, sizeof(serialize_block_static_)) == 0);
#endif
        return true;
    }

    assert(0);
    return false;
}

bool SCItemSlot::_SerializeFromItemStream(SLOTCODE item_code, const OPTIONPART* serialized_stream)
{
    __UNUSED(item_code);
    // (NOTE) serializing 정책이 관입층을 서로 바라보는 형태로 설정되어 있으므로 item_slot에 직접적
    // 인 연산이 이곳에서는 일어나지 않는다.
    using namespace nsSlot;

    ::ZeroMemory(&serialize_block_static_, sizeof(serialize_block_static_));
    
    switch (serialized_stream->ParseType)
    {
    case ItemParseType::kEmpty:
        BOOST_STATIC_ASSERT(ItemParseType::kEmpty == 0);
        return true;
    case ItemParseType::kCanNotParseOld:
        serialize_block_static_.BackupStream.ParseType = serialized_stream->ParseType;
        ::CopyMemory(
            serialize_block_static_.BackupStream.stream, 
            serialized_stream->Variable,
            sizeof(serialize_block_static_.BackupStream.stream)
        );
        BOOST_STATIC_ASSERT(sizeof(serialized_stream->ParseType) == sizeof(BYTE));
        BOOST_STATIC_ASSERT(sizeof(serialized_stream->Variable) == OptionPartCurVer::kBufferSize);
        return true;
    case ItemParseType::kCanNotParse:
        //....
        ASSERT(!"Not implementation currently");
        return false; // <좀 더 생각하고 처리할 것.
    }

    if (serialized_stream->ParseType < ItemParseType::kUpperbound) 
    {
        const Binder* binder = binder_info_static_.binders_[serialized_stream->ParseType];
        serialize_block_static_.Default.ParseType = serialized_stream->ParseType;
        if (binder == NULL) 
        {
            return true;
        }
        binder->Decode(serialized_stream);
        return true;
    }

    assert(0);
    return false;
}

bool SCItemSlot::_InitializeBinding(const SCItemSlot* first)
{
    using namespace nsSlot;
    
    ::ZeroMemory(&binder_info_static_, sizeof(binder_info_static_));
    binder_info_static_.first_ = first;
    ::ZeroMemory(&serialize_block_static_, sizeof(serialize_block_static_));
    
    const ItemParseType::Value parse_types[] =
    {
        ItemParseType::kDefault, 
        ItemParseType::kDefaultTime, 
    #ifdef _NA_0_20100727_SOCKET_SYSTEM_CONVERT
        ItemParseType::kExtendData, 
    #endif
        ItemParseType::kEquip, 
        ItemParseType::kEquipTime, 
        ItemParseType::kEquipEtcs, 
        ItemParseType::kEquipEtcsTime, 
        ItemParseType::kLimited, 
        ItemParseType::kFate, 
        ItemParseType::kCoord, 
        ItemParseType::kWayPoint, 
        ItemParseType::kPet, 
        ItemParseType::kRider, 
        ItemParseType::kRiderParts, 
        ItemParseType::kLottery, 
    };

    for (int i = 0; i < _countof(parse_types); ++i)
    {
        const ItemParseType::ValueType parse_type = 
            static_cast<ItemParseType::ValueType>(parse_types[i]);
        SerializeItemOption::GetBinder(
            parse_type, 
            ITEM_STREAM_SIZE_LAST_VERSION,
            &(binder_info_static_.binders_[parse_type]), 
            NULL
        );
    }
    
    return true;
}

//==================================================================================================
namespace nsSlot {
;

#if defined(_NA_0_20100727_SOCKET_SYSTEM_CONVERT) && \
    defined(_NA_0_20100727_SOCKET_SYSTEM_CONVERT_IN_JAPAN)

struct Ep2_RankupItemRewardInJapan
{
    static const uint8_t kSocketMatchTable[RANK_MAX];
    //
    static bool Check3SocketItem(const BASE_ITEMINFO* item_info)
    {
        switch (item_info->m_LV)
        {
        case ItemCustomLevel::kElite1_0stLV:
        case ItemCustomLevel::kElite1_5stLV:
        case ItemCustomLevel::kSuperJapanLV:
        //case ItemCustomLevel::kCurruptTempleLV: <- same 'kElite1_0stLV'
            return true;
        };
        return false;
    }
    static bool Check4SocketItem(const BASE_ITEMINFO* item_info)
    {
        switch (item_info->m_LV)
        {
        case ItemCustomLevel::kElite2_0stLV:
        case ItemCustomLevel::kUnique1_0stLV:
        case ItemCustomLevel::kUnique1_5stLV:
        case ItemCustomLevel::kUnique2_0stLV:
        case ItemCustomLevel::kUnique2_5stLV:
            return true;
        };
        return false;
    }
    //
    template<class SubStorage>
    static void Convert(SubStorage& storage, const BASE_ITEMINFO* item_info);
    //
    template<>
    static void Convert<SerializeItemOption::sEquip>(
        SerializeItemOption::sEquip& base, const BASE_ITEMINFO* item_info)
    {
        base.SocketIdentifyType = eSOCKET_IDENTIFY_NOT_PROCESS;
        base.NumberOfSockets = 0;
        ZeroMemory(base.SocketOption, sizeof(base.SocketOption));
        //
        bool enable_process = (base.History != eEtheriaHistory::Shell);
        if (enable_process == false) {
            return;
        };

        if (bool exist_rank = (base.Rank > RANK_E))
        {
            if (base.Rank >= _countof(kSocketMatchTable)) {
                base.Rank = RANK_PS;
            };
            base.SocketIdentifyType = eSOCKET_IDENTIFIED_ITEM;
            base.NumberOfSockets = kSocketMatchTable[base.Rank];
        }
        else if (bool make_to_3sockets = Check3SocketItem(item_info))
        {
            base.SocketIdentifyType = eSOCKET_IDENTIFIED_ITEM;
            base.NumberOfSockets = 3;
        }
        else if (bool make_to_4sockets = Check4SocketItem(item_info))
        {
            base.SocketIdentifyType = eSOCKET_IDENTIFIED_ITEM;
            base.NumberOfSockets = 4;
        }
        else
        {
            base.SocketIdentifyType = eSOCKET_UNIDENTIFIED_ITEM;
        }
    }
    template<>
    static void Convert<SerializeItemOption::sEquipTime>(
        SerializeItemOption::sEquipTime& base, const BASE_ITEMINFO* item_info)
    {
        base.SocketIdentifyType = eSOCKET_IDENTIFY_NOT_PROCESS;
        base.NumberOfSockets = 0;
        ZeroMemory(base.SocketOption, sizeof(base.SocketOption));
        //
        if (bool exist_rank = (base.Rank > RANK_E))
        {
            if (base.Rank >= _countof(kSocketMatchTable)) {
                base.Rank = RANK_PS;
            };
            base.SocketIdentifyType = eSOCKET_IDENTIFIED_ITEM;
            base.NumberOfSockets = kSocketMatchTable[base.Rank];
        }
        else if (bool make_to_3sockets = Check3SocketItem(item_info))
        {
            base.SocketIdentifyType = eSOCKET_IDENTIFIED_ITEM;
            base.NumberOfSockets = 3;
        }
        else if (bool make_to_4sockets = Check4SocketItem(item_info))
        {
            base.SocketIdentifyType = eSOCKET_IDENTIFIED_ITEM;
            base.NumberOfSockets = 4;
        }
        else
        {
            base.SocketIdentifyType = eSOCKET_UNIDENTIFIED_ITEM;
        }
    }
    template<>
    static void Convert<SerializeItemOption::sLimited>(
        SerializeItemOption::sLimited& base, const BASE_ITEMINFO* item_info)
    {
        base.SocketIdentifyType = eSOCKET_IDENTIFY_NOT_PROCESS;
        base.NumberOfSockets = 0;
        ZeroMemory(base.SocketOption, sizeof(base.SocketOption));
        //
        bool enable_process = (base.History != eEtheriaHistory::Shell);
        if (enable_process == false) {
            return;
        };

        {
            base.SocketIdentifyType = eSOCKET_UNIDENTIFIED_ITEM;
        }
    }
    template<>
    static void Convert<SerializeItemOption::sFate>(
        SerializeItemOption::sFate& base, const BASE_ITEMINFO* item_info)
    {
        base.SocketIdentifyType = eSOCKET_IDENTIFY_NOT_PROCESS;
        base.NumberOfSockets = 0;
        ZeroMemory(base.SocketOption, sizeof(base.SocketOption));
        //
        bool enable_process = (base.History != eEtheriaHistory::Shell);
        if (enable_process == false) {
            return;
        };

        {
            base.SocketIdentifyType = eSOCKET_UNIDENTIFIED_ITEM;
        }
    }
};

const uint8_t Ep2_RankupItemRewardInJapan::kSocketMatchTable[RANK_MAX] =
{
    0, //RANK_E = 0,
    2, //RANK_D,
    2, //RANK_C,
    3, //RANK_B,
    4, //RANK_MA,
    4, //RANK_A,
    4, //RANK_PA,
    5, //RANK_MS,
    5, //RANK_S,
    5, //RANK_PS,
    //RANK_MAX,
};
#endif

//-----------------------------------------------
#define BINDVAR(name)   (&(name)), sizeof(name)
//-----------------------------------------------

// (CHANGES) (f100802.4L) implement function block
// to acquire multiple list based on type and version
template<ItemParseType::Value>
static bool _GetBinder(
    int version, const Binder** result_binder, const IBinderPerVersion** result_modifier);

// 일반 템들
template<>
static bool _GetBinder<ItemParseType::kDefault>(
    int version, const Binder** result_binder, const IBinderPerVersion** result_modifier)
{
    static struct BinderPerVersion : public IBinderPerVersion 
    {
        BinderPerVersion()
        {
            SerializeItemOption::sDefault& base = serialize_block_static_.Default;
            {
                Binder& binder = binder_v7_;
                
                binder.StartBinding();
                
                binder.BindVar(base.ParseType, 8);
                binder.BindVar(base.Block, SCItemLockInfo::BitSize::kBlocked);
                binder.BindVar(base.UserBlock, SCItemLockInfo::BitSize::kUserBlocked);
                binder.BindVar(base.Tag, SCItemLockInfo::BitSize::kItemTag);
                
                binder.EndBinding();
            };
        };

        virtual bool ModifySerializeBlock(const Binder* binder, const BASE_ITEMINFO* item_info) const
        {
            __UNUSED((binder, item_info));
            return true;
        }

        Binder binder_v7_;
    } binder_per_version_static_;
    
    if (result_modifier) 
    {
        *result_modifier = &binder_per_version_static_;
    };
    
    if (false) 
    {
        //
    }
    else if (version >= 7) 
    {
        *result_binder = &binder_per_version_static_.binder_v7_;
        return true;
    }
    assert(!"Unsupported version");
    return false;
}

// 일반 템들(시간 제약 있는 것들)
template<>
static bool _GetBinder<ItemParseType::kDefaultTime>(
    int version, const Binder** result_binder, const IBinderPerVersion** result_modifier)
{
    static struct BinderPerVersion : public IBinderPerVersion 
    {
        BinderPerVersion()
        {
            SerializeItemOption::sTime& base = serialize_block_static_.Time;
            {
                Binder& binder = binder_v7_;
                binder.StartBinding();

                binder.BindVar(base.ParseType, 8); 
                binder.BindVar(base.Block, SCItemLockInfo::BitSize::kBlocked);
                binder.BindVar(base.UserBlock, SCItemLockInfo::BitSize::kUserBlocked);
                binder.BindVar(base.Tag, SCItemLockInfo::BitSize::kItemTag);

                binder.BindVar(base.IsExpired, SCTimeInfo::BitSize::kIsExpired);
                binder.BindVar(base.DateTime, SCTimeInfo::BitSize::kDateTime);
                binder.BindVar(base.IsProgressTime, SCTimeInfo::BitSize::kIsProgress);
                binder.BindVar(base.IsExpiredWaitTime, SCTimeInfo::BitSize::kIsExpiredWait);
                
                binder.EndBinding();
            };
        };
        
        virtual bool ModifySerializeBlock(const Binder* binder, const BASE_ITEMINFO* item_info) const
        {
            __UNUSED((binder, item_info));
            return true;
        }
        
        Binder binder_v7_;
    } binder_per_version_static_;
    
    if (result_modifier) 
    {
        *result_modifier = &binder_per_version_static_;
    };
    
    if (false) 
    {
        //
    }
    else if (version >= 7) 
    {
        *result_binder = &binder_per_version_static_.binder_v7_;
        return true;
    }
    assert(!"Unsupported version");
    return false;
}

#ifdef _NA_0_20100727_SOCKET_SYSTEM_CONVERT
// 일반 템중 정보가 필요한 것
template<>
static bool _GetBinder<ItemParseType::kExtendData>(
        int version, const Binder** result_binder, const IBinderPerVersion** result_modifier)
{
    static struct BinderPerVersion : public IBinderPerVersion 
    {
        BinderPerVersion()
        {
            SerializeItemOption::sExtendData& base = serialize_block_static_.ExtendData;
            {
                Binder& binder = binder_v8_;
                binder.StartBinding();
                
                binder.BindVar(base.ParseType, 8); 
                binder.BindVar(base.Block, SCItemLockInfo::BitSize::kBlocked);
                binder.BindVar(base.UserBlock, SCItemLockInfo::BitSize::kUserBlocked);
                binder.BindVar(base.Tag, SCItemLockInfo::BitSize::kItemTag);
                binder.BindVar(base.data, 16);
                
                binder.EndBinding();
            };
        };
        
        virtual bool ModifySerializeBlock(const Binder* binder, const BASE_ITEMINFO* item_info) const
        {
            __UNUSED((binder, item_info));
            return true;
        }
        
        Binder binder_v8_;
    } binder_per_version_static_;
    
    if (result_modifier) 
    {
        *result_modifier = &binder_per_version_static_;
    };
    
    if (false) 
    {
    }
    else if (version >= 8) 
    {
        *result_binder = &binder_per_version_static_.binder_v8_;
        return true;
    }
    assert(!"Unsupported version");
    return false;
}
#endif

// 장비 무기
template<>
static bool _GetBinder<ItemParseType::kEquip>(
    int version, const Binder** result_binder, const IBinderPerVersion** result_modifier)
{
    static struct BinderPerVersion : public IBinderPerVersion 
    {
        BinderPerVersion()
        {
            SerializeItemOption::sEquip& base = serialize_block_static_.Equip;
        #ifdef _NA002237_100802_EP2_SOCKET_SYSTEM_DB_
            {
                Binder& binder = binder_v8_;
                binder.StartBinding();
                
                binder.BindVar(base.ParseType, 8); 
                binder.BindVar(base.Block, SCItemLockInfo::BitSize::kBlocked);
                binder.BindVar(base.UserBlock, SCItemLockInfo::BitSize::kUserBlocked);
                binder.BindVar(base.Tag, SCItemLockInfo::BitSize::kItemTag);
                
                binder.BindVar(base.History, 2);
                binder.BindVar(base.IsIdentifyPossession, 1);
                binder.BindVar(base.IsDestroyed, 1); //__NA_0_POSSESSION_ITEM_DROP
                binder.BindVar(base.IsDivine, 1);
                binder.BindVar(base.NOption, 3);
                binder.BindVar(base.EnchantGrade, 4);
                // __NA_000968_ETHER_EMISSION_DEVICE
                binder.BindVar(base.IsEquipEtherDevice, 1);
                binder.BindVar(base.EtherBulletIndex, 4);
                // _NA000109_100427_ITEM_CUSTOMIZING_CONTENTS_
                binder.BindVar(base.ImageCode, 16);
                // _NA_0_20100727_SOCKET_SYSTEM_CONVERT
                binder.BindVar(base.SocketIdentifyType, 2);
                binder.BindVar(base.NumberOfSockets, 3);
                for (int i = 0; i < SOCKET_MAX_V8; ++i) 
                {
                    binder.BindVar(base.SocketOption[i], 10);
                }
            #ifdef _NA_003740_20111122_RANDOM_ITEM
                for (int i = 0; i < _countof(base.RandomOptions); ++i) 
                {
                    binder.BindVar(
                        base.RandomOptions[i], SerializeItemOption::RandomSetting::kOptionBitSize);
                }
            #endif // _NA_003740_20111122_RANDOM_ITEM

            #ifdef _NA_008124_20150313_AWAKENING_SYSTEM
                binder.BindVar(base.AwakeningGrade, 4);
            #endif// _NA_008124_20150313_AWAKENING_SYSTEM

                binder.EndBinding();
            };
        #endif
            {
                Binder& binder = binder_v7_;
                binder.StartBinding();
                
                binder.BindVar(base.ParseType, 8); 
                binder.BindVar(base.Block, SCItemLockInfo::BitSize::kBlocked);
                binder.BindVar(base.UserBlock, SCItemLockInfo::BitSize::kUserBlocked);
                binder.BindVar(base.Tag, SCItemLockInfo::BitSize::kItemTag);
                binder.BindVar(base.IsIdentifyPossession, 1);
                binder.BindVar(base.IsDestroyed, 1); //__NA_0_POSSESSION_ITEM_DROP
                binder.BindVar(base.NOption, 3);
                binder.BindVar(base.Rank, 4);
                BOOST_STATIC_ASSERT(RANK_D == 1 && RANK_MAX == 10);
                for (int i = RANK_D; i < RANK_MAX; ++i) 
                {
                    binder.BindVar(base.RankOption[i], 7);
                }
                binder.BindVar(base.EnchantGrade, 4);
                binder.BindVar(base.IsDivine, 1);
                binder.BindVar(base.NumberOfSockets, 2);
                for (int i = 0; i < SOCKET_MAX_V7; ++i) 
                {
                    binder.BindVar(base.SocketOption[i], 8);
                }
                // __NA_000968_ETHER_EMISSION_DEVICE
                binder.BindVar(base.IsEquipEtherDevice, 1);
                binder.BindVar(base.EtherBulletIndex, 4);
                // _NA000109_100427_ITEM_CUSTOMIZING_CONTENTS_
                binder.BindVar(base.History, 2);
                binder.BindVar(base.ImageCode, 16);
                
                binder.EndBinding();
            };
        };
        
        virtual bool ModifySerializeBlock(const Binder* binder, const BASE_ITEMINFO* item_info) const
        {
            __TOUCHED((binder, item_info));
            SerializeItemOption::sEquip& base = serialize_block_static_.Equip;
            base;
            // pre-version ~ version 8, data control
            if (&binder_v8_ == binder)
            {   // NOTE, a socket information is reset by The Episode II Socket System.
                // a rankup field is erased because of not using anymore, but it is controlled a binder.
                NOTIFY_RANKINFO_BEFORE_EP2_CONVERT(base.Rank);
            #if defined(_NA_0_20100727_SOCKET_SYSTEM_CONVERT) && \
                defined(_NA_0_20100727_SOCKET_SYSTEM_CONVERT_IN_JAPAN)
                Ep2_RankupItemRewardInJapan::Convert(base, item_info);
            #elif defined(_NA_0_20100727_SOCKET_SYSTEM_CONVERT)
                if ((base.History != eEtheriaHistory::Shell) && 
                    (base.NumberOfSockets || base.Rank > RANK_E))
                {
                    base.SocketIdentifyType = eSOCKET_UNIDENTIFIED_ITEM;
                }
                else
                {
                    base.SocketIdentifyType = eSOCKET_IDENTIFY_NOT_PROCESS;
                }
                base.NumberOfSockets = 0;
                ZeroMemory(base.SocketOption, sizeof(base.SocketOption));
            #endif
            }
            return true;
        }
        
        Binder binder_v8_;
        Binder binder_v7_;
    } binder_per_version_static_;
    
    if (result_modifier) 
    {
        *result_modifier = &binder_per_version_static_;
    };
    
    if (false) 
    {
    }
#ifdef _NA002237_100802_EP2_SOCKET_SYSTEM_DB_
    else if (version >= 8) 
    {
        *result_binder = &binder_per_version_static_.binder_v8_;
        return true;
    }
#endif
    else if (version >= 7) 
    {
        *result_binder = &binder_per_version_static_.binder_v7_;
        return true;
    }
    assert(!"Unsupported version");
    return false;
}

// 장비 (무기/방어구)
template<>
static bool _GetBinder<ItemParseType::kEquipTime>(\
    int version, const Binder** result_binder, const IBinderPerVersion** result_modifier)
{
    static struct BinderPerVersion : public IBinderPerVersion {
        BinderPerVersion()
        {
            SerializeItemOption::sEquipTime& base = serialize_block_static_.EquipTime;
        #ifdef _NA002237_100802_EP2_SOCKET_SYSTEM_DB_
            ;{
                Binder& binder = binder_v8_;
                //--------------------------------------------------------
                // version 8 ~ specification (remove rank info, add socket info x2)
                binder.StartBinding();
                // Shared Option Part
                binder.BindVar(base.ParseType, 8); 
                binder.BindVar(base.Block, SCItemLockInfo::BitSize::kBlocked);
                binder.BindVar(base.UserBlock, SCItemLockInfo::BitSize::kUserBlocked);
                binder.BindVar(base.Tag, SCItemLockInfo::BitSize::kItemTag);
                //
                //// _NA000109_100427_ITEM_CUSTOMIZING_CONTENTS_
                //binder.BindVar(base.History, 2); //2
                binder.BindVar(base.IsIdentifyPossession, 1);
                binder.BindVar(base.IsDestroyed, 1); //__NA_0_POSSESSION_ITEM_DROP
                binder.BindVar(base.IsDivine, 1);

                binder.BindVar(base.NOption, 3);
                binder.BindVar(base.EnchantGrade, 4);
                //__NA_000968_ETHER_EMISSION_DEVICE
                binder.BindVar(base.IsEquipEtherDevice, 1);
                binder.BindVar(base.EtherBulletIndex, 4);
                // time attributes
                binder.BindVar(base.IsExpired, SCTimeInfo::BitSize::kIsExpired);
                binder.BindVar(base.DateTime, SCTimeInfo::BitSize::kDateTime);
                binder.BindVar(base.IsProgressTime, SCTimeInfo::BitSize::kIsProgress);
                binder.BindVar(base.IsExpiredWaitTime, SCTimeInfo::BitSize::kIsExpiredWait);
                // _NA_0_20100727_SOCKET_SYSTEM_CONVERT
                binder.BindVar(base.SocketIdentifyType, 2); //2
                binder.BindVar(base.NumberOfSockets, 3); // v7(2)
                for (int i = 0; i < SOCKET_MAX_V8; ++i) 
                {
                    binder.BindVar(base.SocketOption[i], 10);
                }
                //
            #ifdef _NA_003740_20111122_RANDOM_ITEM
                for (int i = 0; i < _countof(base.RandomOptions); ++i) 
                {
                    binder.BindVar(
                        base.RandomOptions[i], SerializeItemOption::RandomSetting::kOptionBitSize);
                }
            #endif // _NA_003740_20111122_RANDOM_ITEM
                //

            #ifdef _NA_008124_20150313_AWAKENING_SYSTEM
                binder.BindVar(base.AwakeningGrade, 4);
            #endif// _NA_008124_20150313_AWAKENING_SYSTEM

                binder.EndBinding();
                //--------------------------------------------------------
            };
        #endif
            ;{
                Binder& binder = binder_v7_;
                //--------------------------------------------------------
                // version 7 ~ specification
                binder.StartBinding();
                // Shared Option Part
                binder.BindVar(base.ParseType, 8); 
                binder.BindVar(base.Block, SCItemLockInfo::BitSize::kBlocked);
                binder.BindVar(base.UserBlock, SCItemLockInfo::BitSize::kUserBlocked);
                binder.BindVar(base.Tag, SCItemLockInfo::BitSize::kItemTag);

                binder.BindVar(base.IsIdentifyPossession, 1);
                binder.BindVar(base.IsDestroyed, 1); //__NA_0_POSSESSION_ITEM_DROP

                binder.BindVar(base.NOption, 3);

                binder.BindVar(base.EnchantGrade, 4);
                binder.BindVar(base.IsDivine, 1);

                binder.BindVar(base.NumberOfSockets, 2);
                for (int i = 0; i < SOCKET_MAX_V7; ++i) 
                {
                    binder.BindVar(base.SocketOption[i], 8);
                }
                binder.BindVar(base.Rank, 4);
                BOOST_STATIC_ASSERT(RANK_D == 1 && RANK_MAX == 10);
                for (int i = RANK_D; i < RANK_MAX; ++i) 
                {
                    binder.BindVar(base.RankOption[i], 7);
                }

                //__NA_000968_ETHER_EMISSION_DEVICE
                binder.BindVar(base.IsEquipEtherDevice, 1);
                binder.BindVar(base.EtherBulletIndex, 4);

                binder.BindVar(base.IsExpired, SCTimeInfo::BitSize::kIsExpired);
                binder.BindVar(base.DateTime, SCTimeInfo::BitSize::kDateTime);
                binder.BindVar(base.IsProgressTime, SCTimeInfo::BitSize::kIsProgress);
                binder.BindVar(base.IsExpiredWaitTime, SCTimeInfo::BitSize::kIsExpiredWait);
                binder.EndBinding();
                //--------------------------------------------------------
            };
        };
        //
        virtual bool ModifySerializeBlock(const Binder* binder, const BASE_ITEMINFO* item_info) const
        {
            __TOUCHED((binder, item_info));
            SerializeItemOption::sEquipTime& base = serialize_block_static_.EquipTime;
            // pre-version ~ version 8, data control
            if (&binder_v8_ == binder)
            {   // NOTE, a socket information is reset by The Episode II Socket System.
                // a rankup field is erased because of not using anymore, but it is controlled a binder.
                NOTIFY_RANKINFO_BEFORE_EP2_CONVERT(base.Rank);
            #if defined(_NA_0_20100727_SOCKET_SYSTEM_CONVERT) && \
                defined(_NA_0_20100727_SOCKET_SYSTEM_CONVERT_IN_JAPAN)
                Ep2_RankupItemRewardInJapan::Convert(base, item_info);
            #elif defined(_NA_0_20100727_SOCKET_SYSTEM_CONVERT)
                if (base.NumberOfSockets || base.Rank > RANK_E)
                {
                    base.SocketIdentifyType = eSOCKET_UNIDENTIFIED_ITEM;
                }
                else
                {
                    base.SocketIdentifyType = eSOCKET_IDENTIFY_NOT_PROCESS;
                }
                base.NumberOfSockets = 0;
                ZeroMemory(base.SocketOption, sizeof(base.SocketOption));
            #endif
            }
            return true;
        }
        //
        Binder binder_v8_;
        Binder binder_v7_;
    } binder_per_version_static_;
    //
    if (result_modifier) {
        *result_modifier = &binder_per_version_static_;
    };
    //
    if (false) {
        ;
    }
    //ex: else if (version >= 13) { ...
#ifdef _NA002237_100802_EP2_SOCKET_SYSTEM_DB_
    else if (version >= 8) {
        *result_binder = &binder_per_version_static_.binder_v8_;
        return true;
    }
#endif
    else if (version >= 7)
    {
        *result_binder = &binder_per_version_static_.binder_v7_;
        return true;
    }
    assert(!"Unsupported version");
    return false;
}

// 장비 (악세사리/특수악세사리)
template<>
static bool _GetBinder<ItemParseType::kEquipEtcs>(\
    int version, const Binder** result_binder, const IBinderPerVersion** result_modifier)
{
    static struct BinderPerVersion : public IBinderPerVersion {
        BinderPerVersion()
        {
            SerializeItemOption::sEquipEtc& base = serialize_block_static_.EquipEtc;
        #ifdef _NA002237_100802_EP2_SOCKET_SYSTEM_DB_
            ;{
                Binder& binder = binder_v8_;
                //--------------------------------------------------------
                // version 8 ~ specification (remove rank info, add socket info x2)
                binder.StartBinding();
                // Shared Option Part
                binder.BindVar(base.ParseType, 8); 
                binder.BindVar(base.Block, SCItemLockInfo::BitSize::kBlocked);
                binder.BindVar(base.UserBlock, SCItemLockInfo::BitSize::kUserBlocked);
                binder.BindVar(base.Tag, SCItemLockInfo::BitSize::kItemTag);
                //
                binder.BindVar(base.IsIdentifyPossession, 1);
                binder.BindVar(base.IsDestroyed, 1); //__NA_0_POSSESSION_ITEM_DROP
                //
                binder.BindVar(base.NOption, 3);
            #ifdef _NA002823_WINGS_EQUIPMENTS_EXTENTION
                binder.BindVar(base.EnchantGrade, 4);
                binder.BindVar(base.IsDivine, 1);
            #endif
                //
            #ifdef _NA_003740_20111122_RANDOM_ITEM
                for (int i = 0; i < _countof(base.RandomOptions); ++i) 
                {
                    binder.BindVar(
                        base.RandomOptions[i], SerializeItemOption::RandomSetting::kOptionBitSize);
                }
            #endif // _NA_003740_20111122_RANDOM_ITEM
                //
                binder.EndBinding();
                //--------------------------------------------------------
            };
        #endif
            ;{
                Binder& binder = binder_v7_;
                //--------------------------------------------------------
                // version 7 ~ specification
                binder.StartBinding();
                // Shared Option Part
                binder.BindVar(base.ParseType, 8); 
                binder.BindVar(base.Block, SCItemLockInfo::BitSize::kBlocked);
                binder.BindVar(base.UserBlock, SCItemLockInfo::BitSize::kUserBlocked);
                binder.BindVar(base.Tag, SCItemLockInfo::BitSize::kItemTag);

                binder.BindVar(base.IsIdentifyPossession, 1);
                binder.BindVar(base.IsDestroyed, 1); //__NA_0_POSSESSION_ITEM_DROP

                binder.BindVar(base.NOption, 3);
                binder.BindVar(base.Rank, 4);
                BOOST_STATIC_ASSERT(RANK_D == 1 && RANK_MAX == 10);
                for (int i = RANK_D; i < RANK_MAX; ++i) {
                    binder.BindVar(base.RankOption[i], 7);
                }
                //binder.BindVar(base.EnchantGrade, 4);
                //binder.BindVar(base.IsDivine, 1);
                //  < 최종 확인 divine없다. 사용 안 하는 필드로 남겨둔다.
                binder.EndBinding();
                //--------------------------------------------------------
            };
        };
        //
        virtual bool ModifySerializeBlock(const Binder* binder, const BASE_ITEMINFO* item_info) const
        {
            __TOUCHED((binder, item_info));
            SerializeItemOption::sEquipEtc& base = serialize_block_static_.EquipEtc;
            base;
            // pre-version ~ version 8, data control
            if (&binder_v8_ == binder)
            {   // NOTE, a socket information is reset by The Episode II Socket System.
                // a rankup field is erased because of not using anymore, but it is controlled a binder.
                NOTIFY_RANKINFO_BEFORE_EP2_CONVERT(base.Rank);
            };
            return true;
        }
        //
        Binder binder_v8_;
        Binder binder_v7_;
    } binder_per_version_static_;
    //
    if (result_modifier) {
        *result_modifier = &binder_per_version_static_;
    };
    //
    if (false) {
        ;
    }
    //ex: else if (version >= 13) { ...
#ifdef _NA002237_100802_EP2_SOCKET_SYSTEM_DB_
    else if (version >= 8) {
        *result_binder = &binder_per_version_static_.binder_v8_;
        return true;
    }
#endif
    else if (version >= 7)
    {
        *result_binder = &binder_per_version_static_.binder_v7_;
        return true;
    }
    assert(!"Unsupported version");
    return false;
}

// 장비 (악세사리/특수악세사리) + 시간 설정
template<>
static bool _GetBinder<ItemParseType::kEquipEtcsTime>(\
    int version, const Binder** result_binder, const IBinderPerVersion** result_modifier)
{
    static struct BinderPerVersion : public IBinderPerVersion {
        BinderPerVersion()
        {
            SerializeItemOption::sEquipEtcTime& base = serialize_block_static_.EquipEtcTime;
        #ifdef _NA002237_100802_EP2_SOCKET_SYSTEM_DB_
            ;{
                Binder& binder = binder_v8_;
                //--------------------------------------------------------
                // version 8 ~ specification (remove rank info, add socket info x2)
                binder.StartBinding();
                // Shared Option Part
                binder.BindVar(base.ParseType, 8); 
                binder.BindVar(base.Block, SCItemLockInfo::BitSize::kBlocked);
                binder.BindVar(base.UserBlock, SCItemLockInfo::BitSize::kUserBlocked);
                binder.BindVar(base.Tag, SCItemLockInfo::BitSize::kItemTag);
                //
                binder.BindVar(base.IsIdentifyPossession, 1);
                binder.BindVar(base.IsDestroyed, 1); //__NA_0_POSSESSION_ITEM_DROP
                //
                binder.BindVar(base.NOption, 3);
                //
                binder.BindVar(base.IsExpired, SCTimeInfo::BitSize::kIsExpired);
                binder.BindVar(base.DateTime, SCTimeInfo::BitSize::kDateTime);
                binder.BindVar(base.IsProgressTime, SCTimeInfo::BitSize::kIsProgress);
                binder.BindVar(base.IsExpiredWaitTime, SCTimeInfo::BitSize::kIsExpiredWait);
            #ifdef _NA002823_WINGS_EQUIPMENTS_EXTENTION
                binder.BindVar(base.EnchantGrade, 4);
                binder.BindVar(base.IsDivine, 1);
            #endif
                //
            #ifdef _NA_003740_20111122_RANDOM_ITEM
                for (int i = 0; i < _countof(base.RandomOptions); ++i) 
                {
                    binder.BindVar(
                        base.RandomOptions[i], SerializeItemOption::RandomSetting::kOptionBitSize);
                }
            #endif // _NA_003740_20111122_RANDOM_ITEM
                //
                binder.EndBinding();
                //--------------------------------------------------------
            };
        #endif
            ;{
                Binder& binder = binder_v7_;
                //--------------------------------------------------------
                // version 7 ~ specification
                binder.StartBinding();
                // Shared Option Part
                binder.BindVar(base.ParseType, 8); 
                binder.BindVar(base.Block, SCItemLockInfo::BitSize::kBlocked);
                binder.BindVar(base.UserBlock, SCItemLockInfo::BitSize::kUserBlocked);
                binder.BindVar(base.Tag, SCItemLockInfo::BitSize::kItemTag);

                binder.BindVar(base.IsIdentifyPossession, 1);
                binder.BindVar(base.IsDestroyed, 1); //__NA_0_POSSESSION_ITEM_DROP

                binder.BindVar(base.NOption, 3);
                binder.BindVar(base.Rank, 4);
                BOOST_STATIC_ASSERT(RANK_D == 1 && RANK_MAX == 10);
                for (int i = RANK_D; i < RANK_MAX; ++i) 
                {
                    binder.BindVar(base.RankOption[i], 7);
                }
                //binder.BindVar(base.EnchantGrade, 4);
                //binder.BindVar(base.IsDivine, 1);  //< 최종 확인 divine없다.

                binder.BindVar(base.IsExpired, SCTimeInfo::BitSize::kIsExpired);
                binder.BindVar(base.DateTime, SCTimeInfo::BitSize::kDateTime);
                binder.BindVar(base.IsProgressTime, SCTimeInfo::BitSize::kIsProgress);
                binder.BindVar(base.IsExpiredWaitTime, SCTimeInfo::BitSize::kIsExpiredWait);
                binder.EndBinding();
                //--------------------------------------------------------
            };
        };
        //
        virtual bool ModifySerializeBlock(const Binder* binder, const BASE_ITEMINFO* item_info) const
        {
            __TOUCHED((binder, item_info));
            SerializeItemOption::sEquipEtcTime& base = serialize_block_static_.EquipEtcTime;
            base;
            // pre-version ~ version 8, data control
            if (&binder_v8_ == binder)
            {   // NOTE, a socket information is reset by The Episode II Socket System.
                // a rankup field is erased because of not using anymore, but it is controlled a binder.
                NOTIFY_RANKINFO_BEFORE_EP2_CONVERT(base.Rank);
            };
            return true;
        }
        //
        Binder binder_v8_;
        Binder binder_v7_;
    } binder_per_version_static_;
    //
    if (result_modifier) {
        *result_modifier = &binder_per_version_static_;
    };
    //
    if (false) {
        ;
    }
    //ex: else if (version >= 13) { ...
#ifdef _NA002237_100802_EP2_SOCKET_SYSTEM_DB_
    else if (version >= 8) {
        *result_binder = &binder_per_version_static_.binder_v8_;
        return true;
    }
#endif
    else if (version >= 7)
    {
        *result_binder = &binder_per_version_static_.binder_v7_;
        return true;
    }
    assert(!"Unsupported version");
    return false;
}

// 방어구만...
template<>
static bool _GetBinder<ItemParseType::kLimited>(\
    int version, const Binder** result_binder, const IBinderPerVersion** result_modifier)
{
    static struct BinderPerVersion : public IBinderPerVersion {
        BinderPerVersion()
        {
            SerializeItemOption::sLimited& base = serialize_block_static_.Limited;
        #ifdef _NA002237_100802_EP2_SOCKET_SYSTEM_DB_
            ;{
                Binder& binder = binder_v8_;
                //--------------------------------------------------------
                // version 8 ~ specification (add socket info x2)
                binder.StartBinding();
                // Shared Option Part
                binder.BindVar(base.ParseType, 8);
                binder.BindVar(base.Block, SCItemLockInfo::BitSize::kBlocked);
                binder.BindVar(base.UserBlock, SCItemLockInfo::BitSize::kUserBlocked);
                binder.BindVar(base.Tag, SCItemLockInfo::BitSize::kItemTag);
                //
                // _NA000109_100427_ITEM_CUSTOMIZING_CONTENTS_
                binder.BindVar(base.History, 2); //2
                //
                binder.BindVar(base.IsIdentifyPossession, 1);
                binder.BindVar(base.IsDestroyed, 1); //__NA_0_POSSESSION_ITEM_DROP
                binder.BindVar(base.IsDivine, 1);

                binder.BindVar(base.NOption, 3);
                binder.BindVar(base.EnchantGrade, 4);
                // limited attributes
                binder.BindVar(base.Index, 16);
                binder.BindVar(base.DurationForRestore, 9);
                binder.BindVar(base.Duration, 9);
                binder.BindVar(base.NumberOfRepairs, 4);
                binder.BindVar(base.DefenceRatio, 9);
                binder.BindVar(base.LimitValueRatio, 9);
                // _NA000109_100427_ITEM_CUSTOMIZING_CONTENTS_
                binder.BindVar(base.ImageCode, 16); //16
                //
                // _NA_0_20100727_SOCKET_SYSTEM_CONVERT
                binder.BindVar(base.SocketIdentifyType, 2); //2
                binder.BindVar(base.NumberOfSockets, 3); // v7(2)
                for (int i = 0; i < SOCKET_MAX_V8; ++i) 
                {
                    binder.BindVar(base.SocketOption[i], 10);
                }
                //
            #ifdef _NA_003740_20111122_RANDOM_ITEM
                for (int i = 0; i < _countof(base.RandomOptions); ++i) 
                {
                    binder.BindVar(
                        base.RandomOptions[i], SerializeItemOption::RandomSetting::kOptionBitSize);
                }
            #endif // _NA_003740_20111122_RANDOM_ITEM
                //
                binder.EndBinding();
                //--------------------------------------------------------
            };
        #endif
            ;{
                Binder& binder = binder_v7_;
                //--------------------------------------------------------
                // version 7 ~ specification
                binder.StartBinding();
                // Shared Option Part
                binder.BindVar(base.ParseType, 8);
                binder.BindVar(base.Block, SCItemLockInfo::BitSize::kBlocked);
                binder.BindVar(base.UserBlock, SCItemLockInfo::BitSize::kUserBlocked);
                binder.BindVar(base.Tag, SCItemLockInfo::BitSize::kItemTag);

                binder.BindVar(base.IsIdentifyPossession, 1);
                binder.BindVar(base.IsDestroyed, 1); //__NA_0_POSSESSION_ITEM_DROP

                binder.BindVar(base.NOption, 3);
                binder.BindVar(base.EnchantGrade, 4);
                binder.BindVar(base.IsDivine, 1); //원래 2다.

                binder.BindVar(base.NumberOfSockets, 2);
                BOOST_STATIC_ASSERT(SOCKET_1 == 0 && SOCKET_MAX_V7 == 3);
                for (int i = 0; i < SOCKET_MAX_V7; ++i) 
                {
                    binder.BindVar(base.SocketOption[i], 8);
                }
                binder.BindVar(base.Index, 16);
                binder.BindVar(base.DurationForRestore, 9);
                binder.BindVar(base.Duration, 9);
                binder.BindVar(base.NumberOfRepairs, 4);
                binder.BindVar(base.DefenceRatio, 9);
                binder.BindVar(base.LimitValueRatio, 9);
                // (CHANGES) (UNUSED SECTION) (WAVERIX) - 기획 확인
                /*
                BOOST_STATIC_ASSERT(LIMITED_1 == 0 && LIMITED_MAX == 3);
                for (int i = 0; i < LIMITED_MAX; ++i) {
                    binder.BindVar(base.LimitedOption[i].Option), 7);
                    binder.BindVar(base.LimitedOption[i].Value), 6);
                }
                */
                //_NA000109_100427_ITEM_CUSTOMIZING_CONTENTS_
                binder.BindVar(base.History, 2); //2
                binder.BindVar(base.ImageCode, 16); //16
                //
                binder.EndBinding();
                //--------------------------------------------------------
            };
        };
        //
        virtual bool ModifySerializeBlock(const Binder* binder, const BASE_ITEMINFO* item_info) const
        {
            __TOUCHED((binder, item_info));
            SerializeItemOption::sLimited& base = serialize_block_static_.Limited;
            base;
            // pre-version ~ version 8, data control
            if (&binder_v8_ == binder)
            {   // NOTE, a socket information is reset by The Episode II Socket System.
                // a rankup field is erased because of not using anymore, but it is controlled a binder.
            #if defined(_NA_0_20100727_SOCKET_SYSTEM_CONVERT) && \
                defined(_NA_0_20100727_SOCKET_SYSTEM_CONVERT_IN_JAPAN)
                Ep2_RankupItemRewardInJapan::Convert(base, item_info);
            #elif defined(_NA_0_20100727_SOCKET_SYSTEM_CONVERT)
                if ((base.History != eEtheriaHistory::Shell) && base.NumberOfSockets)
                {
                    base.SocketIdentifyType = eSOCKET_UNIDENTIFIED_ITEM;
                }
                else
                {
                    base.SocketIdentifyType = eSOCKET_IDENTIFY_NOT_PROCESS;
                }
                base.NumberOfSockets = 0;
                ZeroMemory(base.SocketOption, sizeof(base.SocketOption));
            #endif
            }
            return true;
        }
        //
        Binder binder_v8_;
        Binder binder_v7_;
    } binder_per_version_static_;
    //
    if (result_modifier) {
        *result_modifier = &binder_per_version_static_;
    };
    //
    if (false) {
        ;
    }
    //ex: else if (version >= 13) { ...
#ifdef _NA002237_100802_EP2_SOCKET_SYSTEM_DB_
    else if (version >= 8) {
        *result_binder = &binder_per_version_static_.binder_v8_;
        return true;
    }
#endif
    else if (version >= 7)
    {
        *result_binder = &binder_per_version_static_.binder_v7_;
        return true;
    }
    assert(!"Unsupported version");
    return false;
}

// LuckyItem
template<>
static bool _GetBinder<ItemParseType::kFate>(\
    int version, const Binder** result_binder, const IBinderPerVersion** result_modifier)
{
    static struct BinderPerVersion : public IBinderPerVersion {
        BinderPerVersion()
        {
            SerializeItemOption::sFate& base = serialize_block_static_.Fate;
        #ifdef _NA002237_100802_EP2_SOCKET_SYSTEM_DB_
            ;{
                Binder& binder = binder_v8_;
                //--------------------------------------------------------
                // version 8 ~ specification (add socket info x2)
                binder.StartBinding();
                // Shared Option Part
                binder.BindVar(base.ParseType, 8); 
                binder.BindVar(base.Block, SCItemLockInfo::BitSize::kBlocked);
                binder.BindVar(base.UserBlock, SCItemLockInfo::BitSize::kUserBlocked);
                binder.BindVar(base.Tag, SCItemLockInfo::BitSize::kItemTag);
                //
                //_NA001202_101104_ITEM_CUSTOMIZING_CONTENTS_ETHEREALIZABLE_LUCKYITEM
                binder.BindVar(base.History, 2); //2
                //
                binder.BindVar(base.IsIdentifyPossession, 1);
                binder.BindVar(base.IsDestroyed, 1); //__NA_0_POSSESSION_ITEM_DROP
                //
                binder.BindVar(base.NOption, 3);
                binder.BindVar(base.EnchantGrade, 4);
                // f101105.3L, unused. __NA_000968_ETHER_EMISSION_DEVICE
                //binder.BindVar(base.IsEquipEtherDevice), 1);
                //binder.BindVar(base.EtherBulletIndex), 4);
                // lucky attributes
                binder.BindVar(base.Index, 16);
                binder.BindVar(base.AtkDefRatio, 9);
                binder.BindVar(base.Duration, 9);
                binder.BindVar(base.IsIdentify, 1);
                // time attributes
                binder.BindVar(base.IsExpired, SCTimeInfo::BitSize::kIsExpired);
                binder.BindVar(base.IsExpiredWaitTime, SCTimeInfo::BitSize::kIsExpiredWait);
                binder.BindVar(base.DateTime, SCTimeInfo::BitSize::kDateTime);
                binder.BindVar(base.Period, 14);
                //_NA001202_101104_ITEM_CUSTOMIZING_CONTENTS_ETHEREALIZABLE_LUCKYITEM
                binder.BindVar(base.ImageCode, 16); //16
                //
                // _NA_0_20100727_SOCKET_SYSTEM_CONVERT
                binder.BindVar(base.SocketIdentifyType, 2); //2
                binder.BindVar(base.NumberOfSockets, 3); // v7(2)
                for (int i = 0; i < SOCKET_MAX_V8; ++i) 
                {
                    binder.BindVar(base.SocketOption[i], 10);
                }
                //
            #ifdef _NA_003740_20111122_RANDOM_ITEM
                for (int i = 0; i < _countof(base.RandomOptions); ++i) 
                {
                    binder.BindVar(
                        base.RandomOptions[i], SerializeItemOption::RandomSetting::kOptionBitSize);
                }
            #endif // _NA_003740_20111122_RANDOM_ITEM
                //
                binder.EndBinding();
                //--------------------------------------------------------
            };
        #endif
            ;{
                Binder& binder = binder_v7_;
                //--------------------------------------------------------
                // version 7 ~ specification
                binder.StartBinding();
                // Shared Option Part
                binder.BindVar(base.ParseType, 8); 
                binder.BindVar(base.Block, SCItemLockInfo::BitSize::kBlocked);
                binder.BindVar(base.UserBlock, SCItemLockInfo::BitSize::kUserBlocked);
                binder.BindVar(base.Tag, SCItemLockInfo::BitSize::kItemTag);

                binder.BindVar(base.IsIdentifyPossession, 1);
                binder.BindVar(base.IsDestroyed, 1); //__NA_0_POSSESSION_ITEM_DROP

                binder.BindVar(base.NOption, 3);
                binder.BindVar(base.EnchantGrade, 4);

                binder.BindVar(base.NumberOfSockets, 2);
                for (int i = 0; i < SOCKET_MAX_V7; ++i) 
                {
                    binder.BindVar(base.SocketOption[i], 8);
                }
                binder.BindVar(base.Index, 16);
                binder.BindVar(base.AtkDefRatio, 9);
                binder.BindVar(base.Duration, 9);
                binder.BindVar(base.IsIdentify, 1);

                //__NA_000968_ETHER_EMISSION_DEVICE
                binder.BindVar(base.IsEquipEtherDevice, 1);
                binder.BindVar(base.EtherBulletIndex, 4);

                binder.BindVar(base.IsExpired, SCTimeInfo::BitSize::kIsExpired);
                binder.BindVar(base.IsExpiredWaitTime, SCTimeInfo::BitSize::kIsExpiredWait);
                binder.BindVar(base.DateTime, SCTimeInfo::BitSize::kDateTime);
                binder.BindVar(base.Period, 14);
                //_NA001202_101104_ITEM_CUSTOMIZING_CONTENTS_ETHEREALIZABLE_LUCKYITEM
                binder.BindVar(base.History, 2); //2
                binder.BindVar(base.ImageCode, 16); //16
                //
                binder.EndBinding();
                //--------------------------------------------------------
            };
        };
        //
        virtual bool ModifySerializeBlock(const Binder* binder, const BASE_ITEMINFO* item_info) const
        {
            __TOUCHED((binder, item_info));
            SerializeItemOption::sFate& base = serialize_block_static_.Fate;
            base;
            // pre-version ~ version 8, data control
            if (&binder_v8_ == binder)
            {   // NOTE, a socket information is reset by The Episode II Socket System.
                // a rankup field is erased because of not using anymore, but it is controlled a binder.
            #if defined(_NA_0_20100727_SOCKET_SYSTEM_CONVERT) && \
                defined(_NA_0_20100727_SOCKET_SYSTEM_CONVERT_IN_JAPAN)
                Ep2_RankupItemRewardInJapan::Convert(base, item_info);
            #elif defined(_NA_0_20100727_SOCKET_SYSTEM_CONVERT)
                if ((base.History != eEtheriaHistory::Shell) && base.NumberOfSockets)
                {
                    base.SocketIdentifyType = eSOCKET_UNIDENTIFIED_ITEM;
                }
                else
                {
                    base.SocketIdentifyType = eSOCKET_IDENTIFY_NOT_PROCESS;
                }
                base.NumberOfSockets = 0;
                ZeroMemory(base.SocketOption, sizeof(base.SocketOption));
            #endif
            }
            return true;
        }
        //
        Binder binder_v8_;
        Binder binder_v7_;
    } binder_per_version_static_;
    //
    if (result_modifier) {
        *result_modifier = &binder_per_version_static_;
    };
    //
    if (false) {
        ;
    }
    //ex: else if (version >= 13) { ...
#ifdef _NA002237_100802_EP2_SOCKET_SYSTEM_DB_
    else if (version >= 8) {
        *result_binder = &binder_per_version_static_.binder_v8_;
        return true;
    }
#endif
    else if (version >= 7)
    {
        *result_binder = &binder_per_version_static_.binder_v7_;
        return true;
    }
    assert(!"Unsupported version");
    return false;
}

// 좌표 정보 담는 아이템 옵션
template<>
static bool _GetBinder<ItemParseType::kCoord>(\
    int version, const Binder** result_binder, const IBinderPerVersion** result_modifier)
{
    static struct BinderPerVersion : public IBinderPerVersion {
        BinderPerVersion()
        {
            SerializeItemOption::sCoord& base = serialize_block_static_.Coord;
            ;{
                Binder& binder = binder_v7_;
                //--------------------------------------------------------
                // version 7 ~ specification
                binder.StartBinding();
                // Shared Option Part
                binder.BindVar(base.ParseType, 8);
                binder.BindVar(base.Block, SCItemLockInfo::BitSize::kBlocked);
                binder.BindVar(base.UserBlock, SCItemLockInfo::BitSize::kUserBlocked);
                binder.BindVar(base.Tag, SCItemLockInfo::BitSize::kItemTag);

                binder.BindVar(base.MapCode, 16);
                binder.BindVar(base.X, 16);
                binder.BindVar(base.Y, 16);
                binder.BindVar(base.Z, 16);
                binder.EndBinding();
                //--------------------------------------------------------
            };
        };
        //
        virtual bool ModifySerializeBlock(const Binder* binder, const BASE_ITEMINFO* item_info) const
        {
            __TOUCHED((binder, item_info));
            return true;
        }
        //
        Binder binder_v7_;
    } binder_per_version_static_;
    //
    if (result_modifier) {
        *result_modifier = &binder_per_version_static_;
    };
    //
    if (false) {
        ;
    }
    //ex: else if (version >= 13) { ...
    //ex:_NA002237_100802_EP2_SOCKET_SYSTEM_DB_
    //else if (version >= 8) { ...
    else if (version >= 7)
    {
        *result_binder = &binder_per_version_static_.binder_v7_;
        return true;
    }
    assert(!"Unsupported version");
    return false;
}

//
template<>
static bool _GetBinder<ItemParseType::kWayPoint>(\
    int version, const Binder** result_binder, const IBinderPerVersion** result_modifier)
{
    static struct BinderPerVersion : public IBinderPerVersion {
        BinderPerVersion()
        {
            SerializeItemOption::sWayPoint& base = serialize_block_static_.WayPoint;
            ;{
                Binder& binder = binder_v7_;
                //--------------------------------------------------------
                // version 7 ~ specification
                binder.StartBinding();
                // Shared Option Part
                binder.BindVar(base.ParseType, 8);
                binder.BindVar(base.Block, SCItemLockInfo::BitSize::kBlocked);
                binder.BindVar(base.UserBlock, SCItemLockInfo::BitSize::kUserBlocked);
                binder.BindVar(base.Tag, SCItemLockInfo::BitSize::kItemTag);

                binder.BindVar(base.IsExpired, SCTimeInfo::BitSize::kIsExpired);
                binder.BindVar(base.IsProgress, 1);
                binder.BindVar(base.DateTime, SCTimeInfo::BitSize::kDateTime);

                binder.BindVar(base.WayPointDeleteEnabled, 1);
                binder.BindVar(base.WayPointDeleteDateTime, 45);
                binder.EndBinding();
                //--------------------------------------------------------
            };
        };
        //
        virtual bool ModifySerializeBlock(const Binder* binder, const BASE_ITEMINFO* item_info) const
        {
            __TOUCHED((binder, item_info));
            return true;
        }
        //
        Binder binder_v7_;
    } binder_per_version_static_;
    //
    if (result_modifier) {
        *result_modifier = &binder_per_version_static_;
    };
    //
    if (false) {
        ;
    }
    //ex: else if (version >= 13) { ...
    //ex:_NA002237_100802_EP2_SOCKET_SYSTEM_DB_
    //else if (version >= 8) { ...
    else if (version >= 7)
    {
        *result_binder = &binder_per_version_static_.binder_v7_;
        return true;
    }
    assert(!"Unsupported version");
    return false;
}

//
template<>
static bool _GetBinder<ItemParseType::kPet>(\
    int version, const Binder** result_binder, const IBinderPerVersion** result_modifier)
{
    static struct BinderPerVersion : public IBinderPerVersion {
        BinderPerVersion()
        {
            SerializeItemOption::sPet& base = serialize_block_static_.Pet;
            ;{
                Binder& binder = binder_v7_;
                //--------------------------------------------------------
                // version 7 ~ specification
                binder.StartBinding();
                // Shared Option Part
                binder.BindVar(base.ParseType, 8);
                binder.BindVar(base.Block, SCItemLockInfo::BitSize::kBlocked);
                binder.BindVar(base.UserBlock, SCItemLockInfo::BitSize::kUserBlocked);
                binder.BindVar(base.Tag, SCItemLockInfo::BitSize::kItemTag);

                binder.BindVar(base.Fullness, 8);
                binder.BindVar(base.Intimacy, 8);

                //__NA_1131_PET_NAME_SETTING
                binder.BindVar(base.IsSetPetName, 1); //원래 Enchant :4를 사용했었다.
                binder.BindVar(base.Name, 8 * MAX_PET_NAME_LENGTH);

                binder.BindVar(base.IsExpired, SCTimeInfo::BitSize::kIsExpired); //1
                binder.BindVar(base.DateTime, SCTimeInfo::BitSize::kDateTime); //45
                //_NA000109_100427_ITEM_CUSTOMIZING_CONTENTS_
                binder.BindVar(base.History, 2); //2
                binder.BindVar(base.ImageCode, 16); //16
                //
                binder.EndBinding();
                //--------------------------------------------------------
            };
        };
        //
        virtual bool ModifySerializeBlock(const Binder* binder, const BASE_ITEMINFO* item_info) const
        {
            __TOUCHED((binder, item_info));
            return true;
        }
        //
        Binder binder_v7_;
    } binder_per_version_static_;
    //
    if (result_modifier) {
        *result_modifier = &binder_per_version_static_;
    };
    //
    if (false) {
        ;
    }
    //ex: else if (version >= 13) { ...
    //ex:_NA002237_100802_EP2_SOCKET_SYSTEM_DB_
    //else if (version >= 8) { ...
    else if (version >= 7)
    {
        *result_binder = &binder_per_version_static_.binder_v7_;
        return true;
    }
    assert(!"Unsupported version");
    return false;
}

// 탈 것 아이템
template<>
static bool _GetBinder<ItemParseType::kRider>(\
    int version, const Binder** result_binder, const IBinderPerVersion** result_modifier)
{
    static struct BinderPerVersion : public IBinderPerVersion {
        BinderPerVersion()
        {
            SerializeItemOption::sRider& base = serialize_block_static_.Rider;
            ;{
                Binder& binder = binder_v7_;
                //--------------------------------------------------------
                // version 7 ~ specification
                binder.StartBinding();
                //__NA1390_1391_090915_RIDING_SYSTEM_ITEM_PART__
                // Shared Option Part
                binder.BindVar(base.ParseType, 8); 
                binder.BindVar(base.Block, SCItemLockInfo::BitSize::kBlocked);
                binder.BindVar(base.UserBlock, SCItemLockInfo::BitSize::kUserBlocked);
                binder.BindVar(base.Tag, SCItemLockInfo::BitSize::kItemTag);

                //binder.BindVar(base.IsIdentifyPossession, 1);
                //binder.BindVar(base.IsDestroyed, 1); //__NA_0_POSSESSION_ITEM_DROP

                binder.BindVar(base.IsUnlimitedPeriod, 1);
                binder.BindVar(base.IsExpiredWaitTime, SCTimeInfo::BitSize::kIsExpiredWait);
                binder.BindVar(base.DateTime, SCTimeInfo::BitSize::kDateTime);

                BOOST_STATIC_ASSERT(eRIDER_PARTS_POS::Counts == 3);
                for (int i = 0; i < eRIDER_PARTS_POS::Counts; ++i) {
                    SerializeItemOption::sRider::sPart& parts = base.Parts[i];
                    binder.BindVar(parts.ItemCode, 16);
                    // (CHANGES) (f100803.2L) change used bits range from 4 bits to 3 bits [0,...5]
                    // to support the Global version need 'Tag' field about a rider parts in a rider
                    binder.BindVar(parts.EnchantGrade, 3); // 이펙트 툴 설정 등급으로 변환 처리 필요
                    binder.BindVar(parts.Tag, 1); // the Global version p-coin history tagging
                    binder.BindVar(parts.IsUnlimitedPeriod, 1);
                    binder.BindVar(parts.IsProgress, 1);
                    binder.BindVar(parts.IsExpiredWaitTime, 1);
                    binder.BindVar(parts.DateOffset, 17); //base on Rider Time
                }
                binder.EndBinding();
                //--------------------------------------------------------
            };
        };
        //
        virtual bool ModifySerializeBlock(const Binder* binder, const BASE_ITEMINFO* item_info) const
        {
            __TOUCHED((binder, item_info));
            return true;
        }
        //
        Binder binder_v7_;
    } binder_per_version_static_;
    //
    if (result_modifier) {
        *result_modifier = &binder_per_version_static_;
    };
    //
    if (false) {
        ;
    }
    //ex: else if (version >= 13) { ...
    //ex:_NA002237_100802_EP2_SOCKET_SYSTEM_DB_
    //else if (version >= 8) { ...
    else if (version >= 7)
    {
        *result_binder = &binder_per_version_static_.binder_v7_;
        return true;
    }
    assert(!"Unsupported version");
    return false;
}

// 탈 것 장착 아이템
template<>
static bool _GetBinder<ItemParseType::kRiderParts>(\
    int version, const Binder** result_binder, const IBinderPerVersion** result_modifier)
{
    static struct BinderPerVersion : public IBinderPerVersion {
        BinderPerVersion()
        {
            SerializeItemOption::sRiderParts& base = serialize_block_static_.RiderParts;
            ;{
                Binder& binder = binder_v7_;
                //--------------------------------------------------------
                // version 7 ~ specification
                binder.StartBinding();
                //__NA1390_1391_090915_RIDING_SYSTEM_ITEM_PART__
                // Shared Option Part
                binder.BindVar(base.ParseType, 8); 
                binder.BindVar(base.Block, SCItemLockInfo::BitSize::kBlocked);
                binder.BindVar(base.UserBlock, SCItemLockInfo::BitSize::kUserBlocked);
                binder.BindVar(base.Tag, SCItemLockInfo::BitSize::kItemTag);
                // (CHANGES) (f100803.2L) an independent rider parts item has Tag
                // based on shared option block. warn 4bits usage of a EnchantGrade field
                binder.BindVar(base.EnchantGrade, 4);
                binder.BindVar(base.IsUnlimitedPeriod, 1);
                binder.BindVar(base.IsProgress, 1);
                binder.BindVar(base.IsExpiredWaitTime, SCTimeInfo::BitSize::kIsExpiredWait);
                binder.BindVar(base.DateTime, SCTimeInfo::BitSize::kDateTime);
                //
                binder.EndBinding();
                //--------------------------------------------------------
            };
        };
        //
        virtual bool ModifySerializeBlock(const Binder* binder, const BASE_ITEMINFO* item_info) const
        {
            __TOUCHED((binder, item_info));
            return true;
        }
        //
        Binder binder_v7_;
    } binder_per_version_static_;
    //
    if (result_modifier) {
        *result_modifier = &binder_per_version_static_;
    };
    //
    if (false) {
        ;
    }
    //ex: else if (version >= 13) { ...
    //ex:_NA002237_100802_EP2_SOCKET_SYSTEM_DB_
    //else if (version >= 8) { ...
    else if (version >= 7)
    {
        *result_binder = &binder_per_version_static_.binder_v7_;
        return true;
    }
    assert(!"Unsupported version");
    return false;
}

//
template<>
static bool _GetBinder<ItemParseType::kLottery>(\
    int version, const Binder** result_binder, const IBinderPerVersion** result_modifier)
{
    static struct BinderPerVersion : public IBinderPerVersion {
        BinderPerVersion()
        {
            SerializeItemOption::sLottery& base = serialize_block_static_.Lottery;
            ;{
                Binder& binder = binder_v7_;
                //--------------------------------------------------------
                // version 7 ~ specification
                binder.StartBinding();
                // Shared Option Part
                binder.BindVar(base.ParseType, 8);
                binder.BindVar(base.Block, SCItemLockInfo::BitSize::kBlocked);
                binder.BindVar(base.UserBlock, SCItemLockInfo::BitSize::kUserBlocked);
                binder.BindVar(base.Tag, SCItemLockInfo::BitSize::kItemTag);

                binder.BindVar(base.Index, 16);
                binder.BindVar(base.Identity, 8);
                binder.BindVar(base.Count, 8);

                for (int i = 0; i < MAX_PANDORA_SLOT_NUM; ++i) {
                    binder.BindVar(base.Slots[i].Random, 1);
                    binder.BindVar(base.Slots[i].Exist, 1);
                    binder.BindVar(base.Slots[i].Position, 4);
                }
                binder.EndBinding();
                //--------------------------------------------------------
            };
        };
        //
        virtual bool ModifySerializeBlock(const Binder* binder, const BASE_ITEMINFO* item_info) const
        {
            __TOUCHED((binder, item_info));
            return true;
        }
        //
        Binder binder_v7_;
    } binder_per_version_static_;
    //
    if (result_modifier) {
        *result_modifier = &binder_per_version_static_;
    };
    //
    if (false) {
        ;
    }
    //ex: else if (version >= 13) { ...
    //ex:_NA002237_100802_EP2_SOCKET_SYSTEM_DB_
    //else if (version >= 8) { ...
    else if (version >= 7)
    {
        *result_binder = &binder_per_version_static_.binder_v7_;
        return true;
    }
    assert(!"Unsupported version");
    return false;
}

//--------------------------------------------------------------------------------------------------

// (CHANGES) (f100802.4L) add an interface to acquire multiple list based on type and version
bool SerializeItemOption::GetBinder(
    const ItemParseType::ValueType parse_type,
    const int version,
    const Binder** result_binder,
    const IBinderPerVersion** result_modifier)
{
    static const Binder empty_binder;
    *result_binder = NULL;
    
    switch (parse_type)
    {
    case ItemParseType::kDefault:
        return _GetBinder<ItemParseType::kDefault>(version, result_binder, result_modifier);
    case ItemParseType::kDefaultTime:
        return _GetBinder<ItemParseType::kDefaultTime>(version, result_binder, result_modifier);
#ifdef _NA_0_20100727_SOCKET_SYSTEM_CONVERT
    case ItemParseType::kExtendData:
        return _GetBinder<ItemParseType::kExtendData>(version, result_binder, result_modifier);
#endif
    case ItemParseType::kEquip:
        return _GetBinder<ItemParseType::kEquip>(version, result_binder, result_modifier);
    case ItemParseType::kEquipTime:
        return _GetBinder<ItemParseType::kEquipTime>(version, result_binder, result_modifier);
    case ItemParseType::kEquipEtcs:
        return _GetBinder<ItemParseType::kEquipEtcs>(version, result_binder, result_modifier);
    case ItemParseType::kEquipEtcsTime:
        return _GetBinder<ItemParseType::kEquipEtcsTime>(version, result_binder, result_modifier);
    case ItemParseType::kLimited:
        return _GetBinder<ItemParseType::kLimited>(version, result_binder, result_modifier);
    case ItemParseType::kFate:
        return _GetBinder<ItemParseType::kFate>(version, result_binder, result_modifier);
    case ItemParseType::kCoord:
        return _GetBinder<ItemParseType::kCoord>(version, result_binder, result_modifier);
    case ItemParseType::kWayPoint:
        return _GetBinder<ItemParseType::kWayPoint>(version, result_binder, result_modifier);
    case ItemParseType::kPet:
        return _GetBinder<ItemParseType::kPet>(version, result_binder, result_modifier);
    case ItemParseType::kRider:
        return _GetBinder<ItemParseType::kRider>(version, result_binder, result_modifier);
    case ItemParseType::kRiderParts:
        return _GetBinder<ItemParseType::kRiderParts>(version, result_binder, result_modifier);
    case ItemParseType::kLottery:
        return _GetBinder<ItemParseType::kLottery>(version, result_binder, result_modifier);
    };

    *result_binder = &empty_binder;
    return false;
}
}; //end of namespace

//#ifdef _NA002237_100802_EP2_SOCKET_SYSTEM_DB_
// (f100803.4L) detach a convert version with binder from an old version converter.
bool SCItemSlot::_ConvertStreamWithBinder(int start_version, ITEMSTREAMEX* start_stream,
                                          int* result_latest_version,
                                          ITEMSTREAMEX* result_latest_stream)
{
    using nsSlot::SerializeItemOption;
    const int kLatestVersion = ITEM_STREAM_SIZE_LAST_VERSION;
    BOOST_STATIC_ASSERT((boost::is_same<ITEMPART_VER3, ITEMPART>::value));
    //                                 v3,...v7,v8 are same all
    // (NOTE) returns a latest version number and cleared memory stream
    // without regard to this function result is fail
    ZeroMemory(result_latest_stream, sizeof(*result_latest_stream));
    //
    ITEMPART* from_part = &start_stream->Part;
    // (f100803.5L) skip a process related to an empty item code stream
    const SLOTCODE item_code = from_part->wCode;
    const BASE_ITEMINFO* item_info = (item_code == 0)
                                   ?    NULL
                                   :    ItemInfoParser::Instance()->GetItemInfo(item_code);
    if (item_info == NULL) {
        // (WARNING) clear stream
        *result_latest_version = kLatestVersion;
        return true;
    }
    //
    ITEMPART* to_part = &result_latest_stream->Part;
    *to_part = *from_part;
    to_part->byReserved2 = 0;
    to_part->byReserved1 = 0;
    //
    OPTIONPART working_space = start_stream->OptionPart;
    //
    if (working_space.ParseType >= ItemParseType::kUpperbound)
    {
        *result_latest_version = kLatestVersion;
        return false;
    }
    const ItemParseType::ValueType parse_type = working_space.ParseType;
    //
    nsSlot::SerializeItemOption* const item_option = _GetBindingBlock();
    const nsSlot::Binder* from_binder = NULL;
    const nsSlot::Binder* to_binder = NULL;
    const nsSlot::IBinderPerVersion* to_pre_modifier = NULL;
    for (int pre_version = start_version; pre_version < kLatestVersion; )
    {
        from_binder = to_binder = NULL;
        if (!SerializeItemOption::GetBinder(parse_type, pre_version, &from_binder, NULL) ||
            //                              parse type             next
            !SerializeItemOption::GetBinder(parse_type, pre_version + 1, &to_binder, &to_pre_modifier))
        {
            // (WARNING) make to empty item because of this option has known parsing type
            *result_latest_version = kLatestVersion;
            return false;
        }
        //
        if (from_binder == to_binder) {
            ++pre_version;
            continue;
        };
        // encoded info. read 'working_space' -> decoded data write 'item_option'
        ZeroMemory(item_option, sizeof(*item_option));
        from_binder->Decode(&working_space);
        // modify an item_option data block before an encoding process
        to_pre_modifier->ModifySerializeBlock(to_binder, item_info);
        // decoded data read 'item_option' -> encoded info. write 'working_space'
        ZeroMemory(&working_space, sizeof(working_space));
        to_binder->Encode(&working_space);
        //
        ++pre_version;
    } //end 'for'
    //
    result_latest_stream->OptionPart = working_space;
    *result_latest_version = kLatestVersion;
    //
    return true;
}

//#endif

