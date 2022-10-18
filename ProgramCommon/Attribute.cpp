#include "stdafx.h"
#include "Attribute.h"

#include <Const.h>
#include "Struct.h"
#include "ItemInfoParser.h"


//==================================================================================================
static BoundPolicies s_BoundPolicies;

BoundPolicies::BoundPolicies()
{
    policy_list_[eBOUNDTYPE_MP_100] = &policy_mp_100_;
    policy_list_[eBOUNDTYPE_MP_1000] = &policy_mp_1000_;
    policy_list_[eBOUNDTYPE_MP_10000] = &policy_mp_10000_;
    policy_list_[eBOUNDTYPE_MP_INFINITE] = &policy_mp_infinite_;
    policy_list_[eBOUNDTYPE_ZP_100] = &policy_zp_100_;
    policy_list_[eBOUNDTYPE_ZP_1000] = &policy_zp_1000_;
    policy_list_[eBOUNDTYPE_ZP_10000] = &policy_zp_10000_;
    policy_list_[eBOUNDTYPE_ZP_INFINITE] = &policy_zp_infinite_;
}

inline int BoundPolicies::touch(eBOUNDTYPE bound_type, int check_value) const
{
    assert(0 <= bound_type && bound_type < _countof(policy_list_));
    return policy_list_[bound_type]->touch(check_value);
}

//==================================================================================================

int INTValue::touch(eBOUNDTYPE bound_type)
{
    value_ = s_BoundPolicies.touch(bound_type, value_);
    return value_;
}

//==================================================================================================
INTValue AttrValue::calc_ratio_static_;

INTValue& AttrValue::operator[](eATTR_KIND eAttr)
{
    assert(eAttr < eATTR_KIND_MAX);
    switch (eAttr)
    {
    case eATTR_KIND_BASE:
        return base_value_;
    case eATTR_KIND_ITEM:
        return item_value_;
    case eATTR_KIND_SKILL:
        return skill_value_;
    case eATTR_KIND_ITEM_RATIO:
        return item_ratio_;
    case eATTR_KIND_SKILL_RATIO:
        return skill_ratio_;
    case eATTR_KIND_RATIO :         
        {
            INTValue calc_ratio;
            calc_ratio.value_ += item_ratio_.value_;
            calc_ratio.value_ += skill_ratio_.value_;
            calc_ratio.touch(eBOUNDTYPE_MP_10000);
            calc_ratio_static_ = calc_ratio;
            return calc_ratio_static_;
        };
    }
    return calc_value_;
}

int AttrValue::operator[](eATTR_KIND eAttr) const
{
    assert(eAttr < eATTR_KIND_MAX);
    switch (eAttr)
    {
    case eATTR_KIND_BASE:
        return base_value_;
    case eATTR_KIND_ITEM:
        return item_value_;
    case eATTR_KIND_SKILL:
        return skill_value_;
    case eATTR_KIND_ITEM_RATIO:
        return item_ratio_;
    case eATTR_KIND_SKILL_RATIO:
        return skill_ratio_;
    case eATTR_KIND_RATIO:
        {
            INTValue calc_ratio;
            calc_ratio.value_ += item_ratio_.value_;
            calc_ratio.value_ += skill_ratio_.value_;
            calc_ratio.touch(eBOUNDTYPE_MP_10000);
            return calc_ratio.value_;
        };
    }       
    return calc_value_.value_;
}

void AttrValue::Update()
{
    //s_AttrUpdatePolicies[update_type_].Update(calc_value_,
    //    base_value_, item_value_, skill_value_, item_ratio_, skill_ratio_);
    if (item_value_.value_ != 0) {
        item_value_.touch(eBOUNDTYPE_MP_INFINITE);
    }
    if (skill_value_.value_ != 0) {
        skill_value_.touch(eBOUNDTYPE_MP_INFINITE);
    }
    if (item_ratio_.value_ != 0) {
        item_ratio_.touch(eBOUNDTYPE_MP_10000);
    }
    if (skill_ratio_.value_ != 0) {
        skill_ratio_.touch(eBOUNDTYPE_MP_10000);
    }
    if (base_value_.value_ != 0) {
        base_value_.touch(static_cast<eBOUNDTYPE>(bound_type_));
    }

    if (update_type_ == eUPDATETYPE_NOTHING)
    {
        if (calc_value_.value_ != 0) {
            calc_value_.touch(static_cast<eBOUNDTYPE>(bound_type_));
        }
        return;
    }

    if (update_type_ == eUPDATETYPE_NOT_RATIO)
    {
        // 비율값을 적용하지 않는 이유는 비율값 자체를 다른 수치에 적용하기 위함이다.
        calc_value_.value_ = base_value_.value_;
        calc_value_.value_ += item_value_.value_;
        calc_value_.value_ += skill_value_.value_;
        if (calc_value_.value_ != 0) {
            calc_value_.touch(static_cast<eBOUNDTYPE>(bound_type_));
        }
    }
    else if (update_type_ == eUPDATETYPE_SUM_RATIO)
    {
        INTValue calc_ratio;
        calc_ratio.value_ += item_ratio_.value_;
        calc_ratio.value_ += skill_ratio_.value_;
        if (calc_ratio.value_ != 0) {
            calc_ratio.touch(eBOUNDTYPE_MP_10000);
        }
        //
        calc_value_.value_ = base_value_.value_;
        calc_value_.value_ += item_value_.value_;
        calc_value_.value_ += skill_value_.value_;
        if (calc_value_.value_ != 0 && calc_ratio.value_ != 0)
        {
            calc_value_.value_ *= (100L + calc_ratio.value_);
            calc_value_.value_ /= 100L;
        }
        if (calc_value_.value_ != 0) {
            calc_value_.touch(static_cast<eBOUNDTYPE>(bound_type_));
        }
    }
}

//==================================================================================================
AttrValue Attributes::attr_value_static_;

const eATTACK_TYPE Attributes::Physicals::kPhysicalsMappedArray[Counts] =
{
    eATTACK_TYPE_MELEE,     // 1
    eATTACK_TYPE_RANGE,     // 2
};

#define PHYSICAL_INDEX_INVALID (_countof(Attributes::Physicals::kPhysicalsMappedArray))
const int Attributes::Physicals::kToPhysicalIndex[eATTACK_TYPE_MAX] =
{
    PHYSICAL_INDEX_INVALID, //eATTACK_TYPE_ALL_OPTION = 0, // 모든 옵션
    0, //eATTACK_TYPE_MELEE, // 근접
	1, //eATTACK_TYPE_RANGE, // 원거리
	PHYSICAL_INDEX_INVALID, //eATTACK_TYPE_WATER, // 물
	PHYSICAL_INDEX_INVALID, //eATTACK_TYPE_FIRE, // 불
	PHYSICAL_INDEX_INVALID, //eATTACK_TYPE_WIND, // 바람
	PHYSICAL_INDEX_INVALID, //eATTACK_TYPE_EARTH, // 대지
	PHYSICAL_INDEX_INVALID, //eATTACK_TYPE_DARKNESS, // 암흑
	PHYSICAL_INDEX_INVALID, //eATTACK_TYPE_DIVINE, // 신성
	PHYSICAL_INDEX_INVALID, //eATTACK_TYPE_PHYSICAL_OPTION, // 물리 옵션
	PHYSICAL_INDEX_INVALID, //eATTACK_TYPE_MAGIC_OPTION, // 마법 옵션
	PHYSICAL_INDEX_INVALID, //eATTACK_TYPE_ALL_MAGIC, // 모든 마법속성
	PHYSICAL_INDEX_INVALID, //eATTACK_TYPE_MAGIC, // 마법
#if !defined(_NA001605_EP2_ATTRIBUTES_RENEWAL_)
	PHYSICAL_INDEX_INVALID, //eATTACK_TYPE_INVALID,
#endif
	//eATTACK_TYPE_MAX,
};

const eATTACK_TYPE Attributes::Elements::kElementsMappedArray[Counts] =
{
    eATTACK_TYPE_WATER,     // 3
    eATTACK_TYPE_FIRE,      // 4
    eATTACK_TYPE_WIND,      // 5
    eATTACK_TYPE_EARTH,     // 6
    eATTACK_TYPE_DARKNESS   // 7
};

#define ELEMENT_INDEX_INVALID (_countof(Attributes::Elements::kElementsMappedArray))
const int Attributes::Elements::kToElementIndex[eATTACK_TYPE_MAX] =
{
    ELEMENT_INDEX_INVALID, //eATTACK_TYPE_ALL_OPTION = 0, // 모든 옵션
    ELEMENT_INDEX_INVALID, //eATTACK_TYPE_MELEE,					// 근접
	ELEMENT_INDEX_INVALID, //eATTACK_TYPE_RANGE,					// 원거리
	0, //eATTACK_TYPE_WATER,					// 물
	1, //eATTACK_TYPE_FIRE,					// 불
	2, //eATTACK_TYPE_WIND,					// 바람
	3, //eATTACK_TYPE_EARTH,					// 대지
	4, //eATTACK_TYPE_DARKNESS,				// 암흑
	ELEMENT_INDEX_INVALID, //eATTACK_TYPE_DIVINE,				// 신성
	ELEMENT_INDEX_INVALID, //eATTACK_TYPE_PHYSICAL_OPTION,		// 물리 옵션
	ELEMENT_INDEX_INVALID, //eATTACK_TYPE_MAGIC_OPTION,			// 마법 옵션
	ELEMENT_INDEX_INVALID, //eATTACK_TYPE_ALL_MAGIC,				// 모든 마법속성
	ELEMENT_INDEX_INVALID, //eATTACK_TYPE_MAGIC,					// 마법
#if !defined(_NA001605_EP2_ATTRIBUTES_RENEWAL_)
	ELEMENT_INDEX_INVALID, //eATTACK_TYPE_INVALID,
#endif
#ifdef _NA_004965_20120613_NEW_CHARACTER_MYSTIC
    ELEMENT_INDEX_INVALID, //eATTACK_TYPE_SKILL,					// 스킬
#endif //_NA_004965_20120613_NEW_CHARACTER_MYSTIC
	//eATTACK_TYPE_MAX,
};
#ifdef _NA_004965_20120613_NEW_CHARACTER_MYSTIC
    BOOST_STATIC_ASSERT((_countof(Attributes::Physicals::kToPhysicalIndex) == 14));
    BOOST_STATIC_ASSERT((_countof(Attributes::Elements::kToElementIndex) == 14));
#else
    #ifdef _NA001605_EP2_ATTRIBUTES_RENEWAL_
    BOOST_STATIC_ASSERT((_countof(Attributes::Physicals::kToPhysicalIndex) == 13));
    BOOST_STATIC_ASSERT((_countof(Attributes::Elements::kToElementIndex) == 13));
    #else
    BOOST_STATIC_ASSERT((_countof(Attributes::Physicals::kToPhysicalIndex) == 14));
    BOOST_STATIC_ASSERT((_countof(Attributes::Elements::kToElementIndex) == 14));
    #endif
#endif
//
// Unused ...
void Attributes::RegisterAttr(eATTR_TYPE attr_type, AttrValue* attr_value,
                              eBOUNDTYPE bound_type, eUPDATETYPE update_type)
{
    ASSERT(attr_type < eATTR_MAX);
    attr_values_[attr_type].attr_value = attr_value;

    if (attr_value) {
        attr_value->SetType(bound_type, update_type);
    }
}

Attributes::Attributes()
{
    RegisterAll();
}

Attributes::~Attributes()
{
}

//--------------------------------------------------------------------------------------------------

// NOTE: f110217.4L, add interface function type properties not using operator
int Attributes::GetAttrValue(eATTR_TYPE attr_type, eATTR_KIND attr_kind) const
{
    if (attr_type < eATTR_MAX && attr_kind < eATTR_KIND_MAX)
    {
        const AttrValue* attr_value = attr_values_[attr_type].attr_value;
        if (attr_value && attr_value != &attr_value_static_) {
            return (*attr_value)[attr_kind];
        }
        assert(attr_value && attr_value != &attr_value_static_);
        return 0;
    }
    assert(attr_type < eATTR_MAX && attr_kind < eATTR_KIND_MAX);
    return 0;
}

void Attributes::SetAttrValue(eATTR_TYPE attr_type, eATTR_KIND attr_kind, int value)
{
    if (attr_type < eATTR_MAX && attr_kind < eATTR_KIND_MAX)
    {
        AttrValue* attr_value = attr_values_[attr_type].attr_value;
        if (attr_value && attr_value != &attr_value_static_) {
            (*attr_value)[attr_kind].value_ = value;
            return;
        }
        assert(attr_value && attr_value != &attr_value_static_);
        return;
    }
    assert(attr_type < eATTR_MAX && attr_kind < eATTR_KIND_MAX);
}

//--------------------------------------------------------------------------------------------------

// (f100720.3L) special purpose interfaces
//  eATTR_KIND_SKILL = clear fields { eATTR_KIND_SKILL, eATTR_KIND_SKILL_RATIO }
//  eATTR_KIND_ITEM = clear fields { eATTR_KIND_ITEM, eATTR_KIND_ITEM_RATIO }
void Attributes::ClearRelatedAttrKind(eATTR_KIND attr_kind)
{
    if (false);
    else if (attr_kind == eATTR_KIND_ITEM)
    {
        AttrNode* it = attr_values_,
                * const end = &attr_values_[eATTR_MAX];
        for ( ; it != end; ++it)
        {
            AttrValue* attr_value = it->attr_value;
            if (attr_value == NULL) {
                continue;
            }
            attr_value->item_value_.value_ = 0;
            attr_value->item_ratio_.value_ = 0;
        }
    }
    else if (attr_kind == eATTR_KIND_SKILL)
    {
        AttrNode* it = attr_values_,
                * const end = &attr_values_[eATTR_MAX];
        for ( ; it != end; ++it)
        {
            AttrValue* attr_value = it->attr_value;
            if (attr_value == NULL) {
                continue;
            }
            attr_value->skill_value_.value_ = 0;
            attr_value->skill_ratio_.value_ = 0;
        }
    }
}

bool Attributes::GroupOperate(const Operation::eOper operate_cmd, const eATTR_KIND attr_kind,
                              const Operation* attr_array, const int* value_array,
                              const int number_of_data)
{
    if (number_of_data <= 0 ||
        operate_cmd >= attr_array->eOper_Counts ||
        attr_kind >= eATTR_KIND_MAX)
    {
        return false;
    }
    const Operation* attr_it = attr_array;
    for (int index = 0; index < number_of_data; ++index, ++attr_it)
    {
        if (attr_it->attr_type == eATTR_TYPE_INVALID ||
            attr_it->attr_type >= eATTR_MAX)
        {
            return false;
        }
        const int value = value_array[index];
        if (operate_cmd != attr_it->eOper_Set && value == 0) {
            continue;
        }
        //
        AttrValue* const attr_value = attr_values_[attr_it->attr_type].attr_value;
        if (attr_value == NULL || attr_value == &attr_value_static_) {
            continue;
        }
        //
        INTValue* int_value = NULL;
        switch (attr_kind)
        {
        case eATTR_KIND_BASE:
            int_value = &attr_value->base_value_;
            break;
        case eATTR_KIND_ITEM:
            int_value = &attr_value->item_value_;
            break;
        case eATTR_KIND_SKILL:
            int_value = &attr_value->skill_value_;
            break;
        case eATTR_KIND_ITEM_RATIO:
            int_value = &attr_value->item_ratio_;
            break;
        case eATTR_KIND_SKILL_RATIO:
            int_value = &attr_value->skill_ratio_;
            break;
        };
        if (int_value == NULL) {
            continue;
        };
        //
        
        switch (operate_cmd)
        {
        case attr_it->eOper_Set:
            int_value->value_ =  value;
            break;
        case attr_it->eOper_Add:
            int_value->value_ += value;
            break;
        case attr_it->eOper_Sub:
            int_value->value_ -= value;
            break;
        //case attr_it->eOper_Mul:
        //    int_value->value_ *= value;
        //    break;
        //case attr_it->eOper_Div:
        //    if (value != 0) {
        //        int_value->value_ /= value;
        //    }
            break;
        };
    } //cmd_end 'for'
    return true;
}

// (f100809.3L) external multi-operate command support interface to support records update
bool Attributes::GroupBatchOperate(const OperationRecord* records, const int number_of_records)
{
    if (number_of_records <= 0) {
        return false;
    }
    const OperationRecord* record_it = records;
    for (int index = 0; index < number_of_records; ++index, ++record_it)
    {
        if (record_it->operate >= Operation::eOper_Counts ||
            record_it->attr_kind >= eATTR_KIND_MAX ||
            record_it->attr_type == eATTR_TYPE_INVALID ||
            record_it->attr_type >= eATTR_MAX)
        {
            return false;
        }
        const int value = record_it->attr_value;
        if (record_it->operate != Operation::eOper_Set && value == 0) {
            continue;
        }
        //
        AttrValue* const attr_value = attr_values_[record_it->attr_type].attr_value;
        if (attr_value == NULL || attr_value == &attr_value_static_) {
            continue;
        }
        //
        INTValue* int_value = NULL;
        switch (record_it->attr_kind)
        {
        case eATTR_KIND_BASE:
            int_value = &attr_value->base_value_;
            break;
        case eATTR_KIND_ITEM:
            int_value = &attr_value->item_value_;
            break;
        case eATTR_KIND_SKILL:
            int_value = &attr_value->skill_value_;
            break;
        case eATTR_KIND_ITEM_RATIO:
            int_value = &attr_value->item_ratio_;
            break;
        case eATTR_KIND_SKILL_RATIO:
            int_value = &attr_value->skill_ratio_;
            break;
        };
        if (int_value == NULL) {
            continue;
        };
        //
        
        switch (record_it->operate)
        {
        case Operation::eOper_Set:
            int_value->value_ =  value;
            break;
        case Operation::eOper_Add:
            int_value->value_ += value;
            break;
        case Operation::eOper_Sub:
            int_value->value_ -= value;
            break;
        //case Operation::eOper_Mul:
        //    int_value->value_ *= value;
        //    break;
        //case Operation::eOper_Div:
        //    if (value != 0) {
        //        int_value->value_ /= value;
        //    }
            break;
        };
    } //cmd_end 'for'
    return true;
}

#if !defined(_SERVER)
int Attributes::GetAttrMaxBoundValue(eATTR_TYPE attr_type) const
{
    if (attr_type < eATTR_MAX)
    {
        const AttrValue* attr_value = attr_values_[attr_type].attr_value;
        if (attr_value && attr_value != &attr_value_static_) {
            return s_BoundPolicies.GetMaxValue(static_cast<eBOUNDTYPE>(attr_value->bound_type_));
        }
        assert(attr_value && attr_value != &attr_value_static_);
        return 0;
    }
    assert(attr_type < eATTR_MAX);
    return 0;
}
#endif //!defined(_SERVER)

void Attributes::RegisterAll(bool is_reuse_data)
{
    if (is_reuse_data == false) {
        ZeroMemory(attr_values_,
            static_cast<size_t>(last_field_end_ - reinterpret_cast<int8_t*>(attr_values_)));
    };
    assert(((&m_ArmorPhysicalAvoidRate + 1) == &m_MoveSpeedRatio) &&
           ((&m_MoveSpeedRatio + 1) == &m_AttSpeedRatio));
//==================================================================================================
#if defined(_DEBUG) || defined(_SERVER)
    #define REGISTER_ATTR(AttrType, AttrValuePtr, BoundType, UpdateType, Desc) \
        ;{ \
            BOOST_STATIC_ASSERT(AttrType < eATTR_MAX); \
            AttrNode* node = &attr_values_[AttrType];  \
            node->attr_value = AttrValuePtr; \
            node->attr_type = AttrType;      \
            node->attr_type_str = #AttrType; \
            node->attr_field = #AttrValuePtr;\
            node->attr_desc = Desc; \
            if (AttrValuePtr) { \
                (AttrValuePtr)->bound_type_ = (BoundType); \
                (AttrValuePtr)->update_type_ = (UpdateType); \
            } \
        };
#else
    #define REGISTER_ATTR(AttrType, AttrValuePtr, BoundType, UpdateType, Desc) \
        ;{ \
            BOOST_STATIC_ASSERT(AttrType < eATTR_MAX); \
            AttrNode* node = &attr_values_[AttrType];  \
            node->attr_value = AttrValuePtr; \
            if (AttrValuePtr) { \
                (AttrValuePtr)->bound_type_ = (BoundType); \
                (AttrValuePtr)->update_type_ = (UpdateType); \
            } \
        };
#endif
//==================================================================================================
#define REGISTER_ATTR_UNUSED(AttrType) \
    REGISTER_ATTR(AttrType, &attr_value_static_, eBOUNDTYPE_MAX, eUPDATETYPE_NOT_RATIO, "")

#ifdef _NA001605_EP2_ATTRIBUTES_RENEWAL_
    #define REGISTER_ATTR_EP1(AttrType, AttrValuePtr, BoundType, UpdateType, Desc) \
        REGISTER_ATTR_UNUSED(##AttrType)
#else
    #define REGISTER_ATTR_EP1(AttrType, AttrValuePtr, BoundType, UpdateType, Desc) \
        REGISTER_ATTR(AttrType, AttrValuePtr, BoundType, UpdateType, Desc)
#endif
//==================================================================================================
    // initialize all
    for (AttrNode* it = attr_values_; it != &attr_values_[_countof(attr_values_)]; ++it) {
        it->attr_value = &attr_value_static_;
    };
    //
    //REGISTER_ATTR(eATTR_TYPE_INVALID, NULL,
    //              eBOUNDTYPE_ZP_1000, eUPDATETYPE_SUM_RATIO,
    //              "유효하지 않는 속성");
    //
    ;{
        REGISTER_ATTR(eATTR_STR, &m_STR,
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_SUM_RATIO,
                      "23:힘 추가");
        REGISTER_ATTR(eATTR_DEX, &m_DEX,
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_SUM_RATIO,
                      "24:민첩 추가");
        REGISTER_ATTR(eATTR_VIT, &m_VIT,
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_SUM_RATIO,
                      "25:체력 추가");
        REGISTER_ATTR(eATTR_INT, &m_INT,
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_SUM_RATIO,
                      "26:지력 추가");
        REGISTER_ATTR(eATTR_SPR, &m_SPR,
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_SUM_RATIO,
                      "27:정신 추가");
    };
    ;{
        REGISTER_ATTR_EP1(eATTR_EXPERTY1, &m_Experty1,
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_SUM_RATIO,
                      "숙련1");
        REGISTER_ATTR_EP1(eATTR_EXPERTY2, &m_Experty2,
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_SUM_RATIO,
                      "숙련2");
    };
    ;{
        // HP, MP
        REGISTER_ATTR(eATTR_MAX_HP, &m_MaxHP,
                      eBOUNDTYPE_ZP_INFINITE, eUPDATETYPE_SUM_RATIO,
                      "28:최대 HP 추가");
        REGISTER_ATTR(eATTR_MAX_MP, &m_MaxMP,
                      eBOUNDTYPE_ZP_INFINITE, eUPDATETYPE_SUM_RATIO,
                      "29:최대 MP/SP 추가");
        REGISTER_ATTR(eATTR_CUR_HP, &m_CurHP,
                      eBOUNDTYPE_ZP_INFINITE, eUPDATETYPE_SUM_RATIO,
                      "현재 HP");
        REGISTER_ATTR(eATTR_CUR_MP, &m_CurMP,
                      eBOUNDTYPE_ZP_INFINITE, eUPDATETYPE_SUM_RATIO,
                      "현재 MP");
        REGISTER_ATTR(eATTR_RECOVERY_HP, &m_RecoverHP,
                      eBOUNDTYPE_MP_10000, eUPDATETYPE_SUM_RATIO,
                      "39:HP 회복량");
        REGISTER_ATTR(eATTR_RECOVERY_MP, &m_RecoverMP,
                      eBOUNDTYPE_MP_10000, eUPDATETYPE_SUM_RATIO,
                      "40:MP 회복량");
    };
    //-----------------------------------------------------------------
    // 공격력 관련 파라미터
    //-----------------------------------------------------------------
    ;{
        // 기본 물리, 마법 공격력
        REGISTER_ATTR(eATTR_BASE_MELEE_MIN_ATTACK_POWER, &m_BaseMeleeMinAttPower,
                      eBOUNDTYPE_ZP_INFINITE, eUPDATETYPE_SUM_RATIO,
                      "기본 물리 공격력 최소");
        REGISTER_ATTR(eATTR_BASE_MELEE_MAX_ATTACK_POWER, &m_BaseMeleeMaxAttPower,
                      eBOUNDTYPE_ZP_INFINITE, eUPDATETYPE_SUM_RATIO,
                      "기본 물리 공격력 최대");
        REGISTER_ATTR(eATTR_BASE_RANGE_MIN_ATTACK_POWER, &m_BaseRangeMinAttPower,
                      eBOUNDTYPE_ZP_INFINITE, eUPDATETYPE_SUM_RATIO,
                      "기본 범위 공격력 최소");
        REGISTER_ATTR(eATTR_BASE_RANGE_MAX_ATTACK_POWER, &m_BaseRangeMaxAttPower,
                      eBOUNDTYPE_ZP_INFINITE, eUPDATETYPE_SUM_RATIO,
                      "기본 범위 공격력 최대");
        REGISTER_ATTR_EP1(eATTR_BASE_MAGICAL_MIN_ATTACK_POWER, &m_BaseMagicMinAttPower,
                      eBOUNDTYPE_ZP_INFINITE, eUPDATETYPE_SUM_RATIO,
                      "기본 마법 공격력 최소");
        REGISTER_ATTR_EP1(eATTR_BASE_MAGICAL_MAX_ATTACK_POWER, &m_BaseMagicMaxAttPower,
                      eBOUNDTYPE_ZP_INFINITE, eUPDATETYPE_SUM_RATIO,
                      "기본 마법 공격력 최대");
    };
    ;{
        // 옵션 물리, 마법 공격력
        REGISTER_ATTR(eATTR_OPTION_PHYSICAL_ATTACK_POWER, &m_OptionPhysicalAttPower,
                      eBOUNDTYPE_MP_10000, eUPDATETYPE_NOT_RATIO,
                      "47:옵션 물리 공격력");
        REGISTER_ATTR_EP1(eATTR_OPTION_MAGICAL_ATTACK_POWER, &m_OptionMagicAttPower,
                      eBOUNDTYPE_MP_10000, eUPDATETYPE_NOT_RATIO,
                      "48:옵션 마법 공격력");
        REGISTER_ATTR_EP1(eATTR_OPTION_ALL_ATTACK_POWER, &m_OptionAllAttPower,
                      eBOUNDTYPE_MP_10000, eUPDATETYPE_NOT_RATIO,
                      "21:옵션 전체 공격력");
    };
    ;{
        // 마법속성 공격력
        REGISTER_ATTR(eATTR_MAGICAL_WATER_ATTACK_POWER,
                      &m_MagicalAttPower[eATTACK_TYPE_WATER],
                      eBOUNDTYPE_ZP_INFINITE, eUPDATETYPE_SUM_RATIO,
                      "4:마법속성 공격력(물) 추가");
        REGISTER_ATTR(eATTR_MAGICAL_FIRE_ATTACK_POWER,
                      &m_MagicalAttPower[eATTACK_TYPE_FIRE],
                      eBOUNDTYPE_ZP_INFINITE, eUPDATETYPE_SUM_RATIO,
                      "3:마법속성 공격력(불) 추가");
        REGISTER_ATTR(eATTR_MAGICAL_WIND_ATTACK_POWER,
                      &m_MagicalAttPower[eATTACK_TYPE_WIND],
                      eBOUNDTYPE_ZP_INFINITE, eUPDATETYPE_SUM_RATIO,
                      "6:마법속성 공격력(바람) 추가");
        REGISTER_ATTR(eATTR_MAGICAL_EARTH_ATTACK_POWER,
                      &m_MagicalAttPower[eATTACK_TYPE_EARTH],
                      eBOUNDTYPE_ZP_INFINITE, eUPDATETYPE_SUM_RATIO,
                      "5:마법속성 공격력(대지) 추가");
        REGISTER_ATTR(eATTR_MAGICAL_DARKNESS_ATTACK_POWER,
                      &m_MagicalAttPower[eATTACK_TYPE_DARKNESS],
                      eBOUNDTYPE_ZP_INFINITE, eUPDATETYPE_SUM_RATIO,
                      "7 마법속성공격력(암흑) 추가");
        REGISTER_ATTR(eATTR_MAGICAL_DIVINE_ATTACK_POWER,
                      &m_MagicalAttPower[eATTACK_TYPE_DIVINE],
                      eBOUNDTYPE_ZP_INFINITE, eUPDATETYPE_SUM_RATIO,
                      "");

        REGISTER_ATTR(eATTR_MAGICAL_ALL_ATTACK_POWER, &m_MagicalAllAttPower,
                      eBOUNDTYPE_ZP_INFINITE, eUPDATETYPE_NOT_RATIO,
                      "35:모든 원소 공격력 추가");
    };
    ;{
        // 스킬 공격력, 스킬 퍼센트 추가 데미지
        REGISTER_ATTR(eATTR_ADD_SKILL_ATTACK_POWER, &m_SkillAttackPower,
                      eBOUNDTYPE_ZP_10000, eUPDATETYPE_SUM_RATIO,
                      "30:스킬 공격력");
        REGISTER_ATTR(eATTR_ADD_SKILL_DAMAGE_RATIO, &m_SkillPercentDamage,
                      eBOUNDTYPE_ZP_INFINITE, eUPDATETYPE_SUM_RATIO,
                      "31:스킬 데미지 추가율: 스킬 퍼센트 추가 데미지");
        // eBOUNDTYPE_ZP_1000->INFINITE 변경[CHANGUP_INTENSIFY_SKILLDAMAGE_STATUS]
    };
    //-----------------------------------------------------------------
    // 방어력 관련 파라미터
    //-----------------------------------------------------------------
    ;{
        // 기본 물리, 마법 방어력
        REGISTER_ATTR(eATTR_BASE_MELEE_DEFENSE_POWER, &m_BaseMeleeDefPower,
                      eBOUNDTYPE_ZP_INFINITE, eUPDATETYPE_SUM_RATIO,
                      "기본 물리 방어력");
        REGISTER_ATTR(eATTR_BASE_RANGE_DEFENSE_POWER, &m_BaseRangeDefPower,
                      eBOUNDTYPE_ZP_INFINITE, eUPDATETYPE_SUM_RATIO,
                      "기본 범위 물리 방어력");
        REGISTER_ATTR_EP1(eATTR_BASE_MAGICAL_DEFENSE_POWER, &m_BaseMagicDefPower,
                      eBOUNDTYPE_ZP_INFINITE, eUPDATETYPE_SUM_RATIO,
                      "기본 마법 방어력");
    };
    ;{
        // 옵션 물리, 마법 방어력
        REGISTER_ATTR(eATTR_OPTION_PHYSICAL_DEFENSE_POWER, &m_OptionPhysicalDefPower,
                      eBOUNDTYPE_MP_10000, eUPDATETYPE_NOT_RATIO,
                      "49:물리방어력추가");
        REGISTER_ATTR_EP1(eATTR_OPTION_MAGICAL_DEFENSE_POWER, &m_OptionMagicDefPower,
                      eBOUNDTYPE_MP_10000, eUPDATETYPE_NOT_RATIO,
                      "50:마법방어력추가");
        REGISTER_ATTR_EP1(eATTR_OPTION_ALL_DEFENSE_POWER, &m_OptionAllDefPower,
                      eBOUNDTYPE_MP_10000, eUPDATETYPE_NOT_RATIO,
                      "22:전체방어력추가 (물리+마법)");
    };
    ;{
        // 마법속성 방어력(최소)
        REGISTER_ATTR_EP1(eATTR_MAGICAL_WATER_DEFENSE_POWER,
                      &m_MagicalDefPower[eATTACK_TYPE_WATER],
                      eBOUNDTYPE_ZP_INFINITE, eUPDATETYPE_SUM_RATIO,
                      "17:마법속성저항(물)추가");
        REGISTER_ATTR_EP1(eATTR_MAGICAL_FIRE_DEFENSE_POWER,
                      &m_MagicalDefPower[eATTACK_TYPE_FIRE],
                      eBOUNDTYPE_ZP_INFINITE, eUPDATETYPE_SUM_RATIO,
                      "16:마법속성저항(불)추가");
        REGISTER_ATTR_EP1(eATTR_MAGICAL_WIND_DEFENSE_POWER,
                      &m_MagicalDefPower[eATTACK_TYPE_WIND],
                      eBOUNDTYPE_ZP_INFINITE, eUPDATETYPE_SUM_RATIO,
                      "19:마법속성저항(바람)추가");
        REGISTER_ATTR_EP1(eATTR_MAGICAL_EARTH_DEFENSE_POWER,
                      &m_MagicalDefPower[eATTACK_TYPE_EARTH],
                      eBOUNDTYPE_ZP_INFINITE, eUPDATETYPE_SUM_RATIO,
                      "18:마법속성저항(대지)추가");
        REGISTER_ATTR_EP1(eATTR_MAGICAL_DARKNESS_DEFENSE_POWER,
                      &m_MagicalDefPower[eATTACK_TYPE_DARKNESS],
                      eBOUNDTYPE_ZP_INFINITE, eUPDATETYPE_SUM_RATIO,
                      "20:마법속성저항(암흑)추가");
        REGISTER_ATTR_EP1(eATTR_MAGICAL_DIVINE_DEFENSE_POWER,
                      &m_MagicalDefPower[eATTACK_TYPE_DIVINE],
                      eBOUNDTYPE_ZP_INFINITE, eUPDATETYPE_SUM_RATIO,
                      "");

        REGISTER_ATTR_EP1(eATTR_MAGICAL_ALL_DEFENSE_POWER,
                      &m_MagicalAllDefPower,
                      eBOUNDTYPE_ZP_INFINITE, eUPDATETYPE_NOT_RATIO,
                      "36:모든 속성 방어력 추가");
    };
    ;{
        // 공격 타입별 추가 방어력
        REGISTER_ATTR_EP1(eATTR_ADD_ALL_DEFENSE_POWER,
                      &m_BonusDefense[eATTACK_TYPE_ALL_OPTION],
                      eBOUNDTYPE_MP_10000, eUPDATETYPE_SUM_RATIO,
                      "모든 공격 타입별 추가 방어력");
        REGISTER_ATTR_EP1(eATTR_ADD_MELEE_DEFENSE_POWER,
                      &m_BonusDefense[eATTACK_TYPE_MELEE],
                      eBOUNDTYPE_MP_10000, eUPDATETYPE_SUM_RATIO,
                      "근접 공격 방어력 추가");
        REGISTER_ATTR_EP1(eATTR_ADD_RANGE_DEFENSE_POWER,
                      &m_BonusDefense[eATTACK_TYPE_RANGE],
                      eBOUNDTYPE_MP_10000, eUPDATETYPE_SUM_RATIO,
                      "범위 공격 방어력 추가");
        REGISTER_ATTR_EP1(eATTR_ADD_WATER_DEFENSE_POWER,
                      &m_BonusDefense[eATTACK_TYPE_WATER],
                      eBOUNDTYPE_MP_10000, eUPDATETYPE_SUM_RATIO,
                      "(물) 공격 방어력 추가");
        REGISTER_ATTR_EP1(eATTR_ADD_FIRE_DEFENSE_POWER,
                      &m_BonusDefense[eATTACK_TYPE_FIRE],
                      eBOUNDTYPE_MP_10000, eUPDATETYPE_SUM_RATIO,
                      "(불) 공격 방어력 추가");
        REGISTER_ATTR_EP1(eATTR_ADD_WIND_DEFENSE_POWER,
                      &m_BonusDefense[eATTACK_TYPE_WIND],
                      eBOUNDTYPE_MP_10000, eUPDATETYPE_SUM_RATIO,
                      "(바람) 공격 방어력 추가");
        REGISTER_ATTR_EP1(eATTR_ADD_EARTH_DEFENSE_POWER,
                      &m_BonusDefense[eATTACK_TYPE_EARTH],
                      eBOUNDTYPE_MP_10000, eUPDATETYPE_SUM_RATIO,
                      "(대지) 공격 방어력 추가");
        REGISTER_ATTR_EP1(eATTR_ADD_DARKNESS_DEFENSE_POWER,
                      &m_BonusDefense[eATTACK_TYPE_DARKNESS],
                      eBOUNDTYPE_MP_10000, eUPDATETYPE_SUM_RATIO,
                      "(암흑) 공격 방어력 추가");
        REGISTER_ATTR_EP1(eATTR_ADD_DIVINE_DEFENSE_POWER,
                      &m_BonusDefense[eATTACK_TYPE_DIVINE],
                      eBOUNDTYPE_MP_10000, eUPDATETYPE_SUM_RATIO,
                      "(성) 공격 방어력 추가");
        REGISTER_ATTR_EP1(eATTR_ADD_PHYSICAL_DEFENSE_POWER,
                      &m_BonusDefense[eATTACK_TYPE_PHYSICAL_OPTION],
                      eBOUNDTYPE_MP_10000, eUPDATETYPE_SUM_RATIO,
                      "(물리) 공격 방어력 추가");
        REGISTER_ATTR_EP1(eATTR_ADD_MAGICAL_DEFENSE_POWER,
                      &m_BonusDefense[eATTACK_TYPE_MAGIC_OPTION],
                      eBOUNDTYPE_MP_10000, eUPDATETYPE_SUM_RATIO,
                      "(마법) 공격 방어력 추가");
        REGISTER_ATTR_EP1(eATTR_ADD_MAGICAL_ALL_DEFENSE_POWER,
                      &m_BonusDefense[eATTACK_TYPE_ALL_MAGIC],
                      eBOUNDTYPE_MP_10000, eUPDATETYPE_SUM_RATIO,
                      "(모든 마법) 공격 방어력 추가");
    };
#ifdef _NA001605_EP2_ATTRIBUTES_RENEWAL_
    ;{
        // 공격 대상의 방어력 감소율
        REGISTER_ATTR(eATTR_DEL_PHYSICAL_TARGET_DEFENSE_RATIO,
            &m_ReducePhysicalTargetDefenseRatio,
            eBOUNDTYPE_MP_1000, eUPDATETYPE_SUM_RATIO,
            "8:물리 공격에 대한 대상의 방어력 감소율");
        REGISTER_ATTR(eATTR_DEL_WATER_TARGET_DEFENSE_RATIO,
            &m_ReduceElementTargetDefenseRatio[Elements::kToElementIndex[eATTACK_TYPE_WATER]],
            eBOUNDTYPE_MP_1000, eUPDATETYPE_SUM_RATIO,
            "10:원소(물) 공격에 대한 대상의 방어력 감소율");
        REGISTER_ATTR(eATTR_DEL_FIRE_TARGET_DEFENSE_RATIO,
            &m_ReduceElementTargetDefenseRatio[Elements::kToElementIndex[eATTACK_TYPE_FIRE]],
            eBOUNDTYPE_MP_1000, eUPDATETYPE_SUM_RATIO,
            "9:원소(불) 공격에 대한 대상의 방어력 감소율");
        REGISTER_ATTR(eATTR_DEL_WIND_TARGET_DEFENSE_RATIO,
            &m_ReduceElementTargetDefenseRatio[Elements::kToElementIndex[eATTACK_TYPE_WIND]],
            eBOUNDTYPE_MP_1000, eUPDATETYPE_SUM_RATIO,
            "12:원소(바람) 공격에 대한 대상의 방어력 감소율");
        REGISTER_ATTR(eATTR_DEL_EARTH_TARGET_DEFENSE_RATIO,
            &m_ReduceElementTargetDefenseRatio[Elements::kToElementIndex[eATTACK_TYPE_EARTH]],
            eBOUNDTYPE_MP_1000, eUPDATETYPE_SUM_RATIO,
            "11:원소(대지) 공격에 대한 대상의 방어력 감소율");
        REGISTER_ATTR(eATTR_DEL_DARKNESS_TARGET_DEFENSE_RATIO,
            &m_ReduceElementTargetDefenseRatio[Elements::kToElementIndex[eATTACK_TYPE_DARKNESS]],
            eBOUNDTYPE_MP_1000, eUPDATETYPE_SUM_RATIO,
            "13:원소(암흑) 공격에 대한 대상의 방어력 감소율");
        REGISTER_ATTR(eATTR_DEL_MAGICAL_ALL_TARGET_DEFENSE_RATIO,
            &m_ReduceElementTargetDefenseRatioAll,
            eBOUNDTYPE_MP_1000, eUPDATETYPE_SUM_RATIO,
            "원소(모든 원소) 공격에 대한 대상의 방어력 감소율");
        REGISTER_ATTR_UNUSED(eATTR_DEL_ALL_TARGET_DEFENSE_RATIO);
        REGISTER_ATTR_UNUSED(eATTR_DEL_MELEE_TARGET_DEFENSE_RATIO);
        REGISTER_ATTR_UNUSED(eATTR_DEL_RANGE_TARGET_DEFENSE_RATIO);
        REGISTER_ATTR_UNUSED(eATTR_DEL_DIVINE_TARGET_DEFENSE_RATIO);
        REGISTER_ATTR_UNUSED(eATTR_DEL_MAGICAL_TARGET_DEFENSE_RATIO);
    };
#else
    ;{
        // 공격 대상의 방어력 감소율
        REGISTER_ATTR_EP1(eATTR_DEL_ALL_TARGET_DEFENSE_RATIO,
                      &m_ReduceDefenseRate[eATTACK_TYPE_ALL_OPTION],
                      eBOUNDTYPE_MP_1000, eUPDATETYPE_SUM_RATIO,
                      "(모든) 공격에 대한 대상의 방어력 감소율");
        REGISTER_ATTR(eATTR_DEL_MELEE_TARGET_DEFENSE_RATIO,
                      &m_ReduceDefenseRate[eATTACK_TYPE_MELEE],
                      eBOUNDTYPE_MP_1000, eUPDATETYPE_SUM_RATIO,
                      "(근접) 공격에 대한 대상의 방어력 감소율");
        REGISTER_ATTR(eATTR_DEL_RANGE_TARGET_DEFENSE_RATIO,
                      &m_ReduceDefenseRate[eATTACK_TYPE_RANGE],
                      eBOUNDTYPE_MP_1000, eUPDATETYPE_SUM_RATIO,
                      "(범위) 공격에 대한 대상의 방어력 감소율");
        REGISTER_ATTR(eATTR_DEL_WATER_TARGET_DEFENSE_RATIO,
                      &m_ReduceDefenseRate[eATTACK_TYPE_WATER],
                      eBOUNDTYPE_MP_1000, eUPDATETYPE_SUM_RATIO,
                      "10:(물) 공격에 대한 대상의 방어력 감소율");
        REGISTER_ATTR(eATTR_DEL_FIRE_TARGET_DEFENSE_RATIO,
                      &m_ReduceDefenseRate[eATTACK_TYPE_FIRE],
                      eBOUNDTYPE_MP_1000, eUPDATETYPE_SUM_RATIO,
                      "9:(불) 공격에 대한 대상의 방어력 감소율");
        REGISTER_ATTR(eATTR_DEL_WIND_TARGET_DEFENSE_RATIO,
                      &m_ReduceDefenseRate[eATTACK_TYPE_WIND],
                      eBOUNDTYPE_MP_1000, eUPDATETYPE_SUM_RATIO,
                      "12:(바람) 공격에 대한 대상의 방어력 감소율");
        REGISTER_ATTR(eATTR_DEL_EARTH_TARGET_DEFENSE_RATIO,
                      &m_ReduceDefenseRate[eATTACK_TYPE_EARTH],
                      eBOUNDTYPE_MP_1000, eUPDATETYPE_SUM_RATIO,
                      "11:(대지) 공격에 대한 대상의 방어력 감소율");
        REGISTER_ATTR(eATTR_DEL_DARKNESS_TARGET_DEFENSE_RATIO,
                      &m_ReduceDefenseRate[eATTACK_TYPE_DARKNESS],
                      eBOUNDTYPE_MP_1000, eUPDATETYPE_SUM_RATIO,
                      "13:(암흑) 공격에 대한 대상의 방어력 감소율");
        REGISTER_ATTR_EP1(eATTR_DEL_DIVINE_TARGET_DEFENSE_RATIO,
                      &m_ReduceDefenseRate[eATTACK_TYPE_DIVINE],
                      eBOUNDTYPE_MP_1000, eUPDATETYPE_SUM_RATIO,
                      "(Divine) 공격에 대한 대상의 방어력 감소율");
        REGISTER_ATTR(eATTR_DEL_PHYSICAL_TARGET_DEFENSE_RATIO,
                      &m_ReduceDefenseRate[eATTACK_TYPE_PHYSICAL_OPTION],
                      eBOUNDTYPE_MP_1000, eUPDATETYPE_SUM_RATIO,
                      "8:(물리) 공격에 대한 대상의 방어력 감소율");
        REGISTER_ATTR_EP1(eATTR_DEL_MAGICAL_TARGET_DEFENSE_RATIO,
                      &m_ReduceDefenseRate[eATTACK_TYPE_MAGIC_OPTION],
                      eBOUNDTYPE_MP_1000, eUPDATETYPE_SUM_RATIO,
                      "(마법) 공격에 대한 대상의 방어력 감소율");
        REGISTER_ATTR(eATTR_DEL_MAGICAL_ALL_TARGET_DEFENSE_RATIO,
                      &m_ReduceDefenseRate[eATTACK_TYPE_ALL_MAGIC],
                      eBOUNDTYPE_MP_1000, eUPDATETYPE_SUM_RATIO,
                      "(모든 마법) 공격에 대한 대상의 방어력 감소율");
    };
#endif
    //-----------------------------------------------------------------
    // 데미지 관련 파라미터
    //-----------------------------------------------------------------
    ;{
        // 아머 타입별 추가 데미지
        REGISTER_ATTR_EP1(eATTR_ADD_ARMOR_HARD_DAMAGE, &m_BonusDamage[eARMOR_HARD],
                      eBOUNDTYPE_MP_10000, eUPDATETYPE_SUM_RATIO,
                      "(Hard) 아머에 대한 추가 데미지");
        REGISTER_ATTR_EP1(eATTR_ADD_ARMOR_MEDIUM_DAMAGE, &m_BonusDamage[eARMOR_MEDIUM],
                      eBOUNDTYPE_MP_10000, eUPDATETYPE_SUM_RATIO,
                      "(Medium) 아머에 대한 추가 데미지");
        REGISTER_ATTR_EP1(eATTR_ADD_ARMOR_SOFT_DAMAGE, &m_BonusDamage[eARMOR_SOFT],
                      eBOUNDTYPE_MP_10000, eUPDATETYPE_SUM_RATIO,
                      "(Soft) 아머에 대한 추가 데미지");
        REGISTER_ATTR_EP1(eATTR_ADD_ARMOR_SIEGE_DAMAGE, &m_BonusDamage[eARMOR_SIEGE],
                      eBOUNDTYPE_MP_10000, eUPDATETYPE_SUM_RATIO,
                      "(Siege) 아머에 대한 추가 데미지");
        REGISTER_ATTR_EP1(eATTR_ADD_ARMOR_UNARMOR_DAMAGE, &m_BonusDamage[eARMOR_UNARMOR],
                      eBOUNDTYPE_MP_10000, eUPDATETYPE_SUM_RATIO,
                      "(Unarmor) 아머에 대한 추가 데미지");
    };
    ;{
        // 아머 타입별 추가 데미지(퍼센트)
        REGISTER_ATTR_EP1(eATTR_ADD_RATIO_ARMOR_HARD_DAMAGE, &m_BonusPercentDamage[eARMOR_HARD],
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_SUM_RATIO,
                      "(Hard) 아머에 대한 추가 데미지 (비율값)");
        REGISTER_ATTR_EP1(eATTR_ADD_RATIO_ARMOR_MEDIUM_DAMAGE, &m_BonusPercentDamage[eARMOR_MEDIUM],
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_SUM_RATIO,
                      "(Medium) 아머에 대한 추가 데미지 (비율값)");
        REGISTER_ATTR_EP1(eATTR_ADD_RATIO_ARMOR_SOFT_DAMAGE, &m_BonusPercentDamage[eARMOR_SOFT],
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_SUM_RATIO,
                      "(Soft) 아머에 대한 추가 데미지 (비율값)");
        REGISTER_ATTR_EP1(eATTR_ADD_RATIO_ARMOR_SIEGE_DAMAGE, &m_BonusPercentDamage[eARMOR_SIEGE],
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_SUM_RATIO,
                      "(Siege) 아머에 대한 추가 데미지 (비율값)");
        REGISTER_ATTR_EP1(eATTR_ADD_RATIO_ARMOR_UNARMOR_DAMAGE, &m_BonusPercentDamage[eARMOR_UNARMOR],
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_SUM_RATIO,
                      "(Unarmor) 아머에 대한 추가 데미지 (비율값)");
    };
#ifdef _NA001605_EP2_ATTRIBUTES_RENEWAL_
    ;{
        // 데미지 감소
        REGISTER_ATTR(eATTR_DEL_PHYSICAL_DAMAGE,
                      &m_ReducePhysicalDamageRatio,
                      eBOUNDTYPE_MP_1000, eUPDATETYPE_SUM_RATIO,
                      "물리 공격에 대한 데미지 감소율");
        REGISTER_ATTR(eATTR_DEL_WATER_DAMAGE,
                      &m_ReduceElementDamageRatio[Elements::kToElementIndex[eATTACK_TYPE_WATER]],
                      eBOUNDTYPE_MP_1000, eUPDATETYPE_SUM_RATIO,
                      "원소(물) 공격에 대한 데미지 감소율");
        REGISTER_ATTR(eATTR_DEL_FIRE_DAMAGE,
                      &m_ReduceElementDamageRatio[Elements::kToElementIndex[eATTACK_TYPE_FIRE]],
                      eBOUNDTYPE_MP_1000, eUPDATETYPE_SUM_RATIO,
                      "원소(불) 공격에 대한 데미지 감소율");
        REGISTER_ATTR(eATTR_DEL_WIND_DAMAGE,
                      &m_ReduceElementDamageRatio[Elements::kToElementIndex[eATTACK_TYPE_WIND]],
                      eBOUNDTYPE_MP_1000, eUPDATETYPE_SUM_RATIO,
                      "원소(바람) 공격에 대한 데미지 감소율");
        REGISTER_ATTR(eATTR_DEL_EARTH_DAMAGE,
                      &m_ReduceElementDamageRatio[Elements::kToElementIndex[eATTACK_TYPE_EARTH]],
                      eBOUNDTYPE_MP_1000, eUPDATETYPE_SUM_RATIO,
                      "원소(대지) 공격에 대한 데미지 감소율");
        REGISTER_ATTR(eATTR_DEL_DARKNESS_DAMAGE,
                      &m_ReduceElementDamageRatio[Elements::kToElementIndex[eATTACK_TYPE_DARKNESS]],
                      eBOUNDTYPE_MP_1000, eUPDATETYPE_SUM_RATIO,
                      "원소(암흑) 공격에 대한 데미지 감소율");
        REGISTER_ATTR(eATTR_DEL_MAGICAL_ALL_DAMAGE,
                      &m_ReduceElementDamageRatioAll,
                      eBOUNDTYPE_MP_1000, eUPDATETYPE_SUM_RATIO,
                      "원소(모든) 공격에 대한 데미지 감소율");
        REGISTER_ATTR_UNUSED(eATTR_DEL_ALL_DAMAGE);
        REGISTER_ATTR_UNUSED(eATTR_DEL_MELEE_DAMAGE);
        REGISTER_ATTR_UNUSED(eATTR_DEL_RANGE_DAMAGE);
        REGISTER_ATTR_UNUSED(eATTR_DEL_DIVINE_DAMAGE);
        REGISTER_ATTR_UNUSED(eATTR_DEL_PHYSICAL_DAMAGE);
        REGISTER_ATTR_UNUSED(eATTR_DEL_MAGICAL_DAMAGE);
    };
#else
    ;{
        // 데미지 감소
        REGISTER_ATTR_EP1(eATTR_DEL_ALL_DAMAGE, &m_ReduceDamage[eATTACK_TYPE_ALL_OPTION],
                      eBOUNDTYPE_MP_10000, eUPDATETYPE_SUM_RATIO,
                      "(모든) 공격에 대한 데미지 감소");
        REGISTER_ATTR_EP1(eATTR_DEL_MELEE_DAMAGE, &m_ReduceDamage[eATTACK_TYPE_MELEE],
                      eBOUNDTYPE_MP_10000, eUPDATETYPE_SUM_RATIO,
                      "(근접) 공격에 대한 데미지 감소");
        REGISTER_ATTR_EP1(eATTR_DEL_RANGE_DAMAGE, &m_ReduceDamage[eATTACK_TYPE_RANGE],
                      eBOUNDTYPE_MP_10000, eUPDATETYPE_SUM_RATIO,
                      "(범위) 공격에 대한 데미지 감소");
        REGISTER_ATTR_EP1(eATTR_DEL_WATER_DAMAGE, &m_ReduceDamage[eATTACK_TYPE_WATER],
                      eBOUNDTYPE_MP_10000, eUPDATETYPE_SUM_RATIO,
                      "(물) 공격에 대한 데미지 감소");
        REGISTER_ATTR_EP1(eATTR_DEL_FIRE_DAMAGE, &m_ReduceDamage[eATTACK_TYPE_FIRE],
                      eBOUNDTYPE_MP_10000, eUPDATETYPE_SUM_RATIO,
                      "(불) 공격에 대한 데미지 감소");
        REGISTER_ATTR_EP1(eATTR_DEL_WIND_DAMAGE, &m_ReduceDamage[eATTACK_TYPE_WIND],
                      eBOUNDTYPE_MP_10000, eUPDATETYPE_SUM_RATIO,
                      "(바람) 공격에 대한 데미지 감소");
        REGISTER_ATTR_EP1(eATTR_DEL_EARTH_DAMAGE, &m_ReduceDamage[eATTACK_TYPE_EARTH],
                      eBOUNDTYPE_MP_10000, eUPDATETYPE_SUM_RATIO,
                      "(대지) 공격에 대한 데미지 감소");
        REGISTER_ATTR_EP1(eATTR_DEL_DARKNESS_DAMAGE, &m_ReduceDamage[eATTACK_TYPE_DARKNESS],
                      eBOUNDTYPE_MP_10000, eUPDATETYPE_SUM_RATIO,
                      "(암흑) 공격에 대한 데미지 감소");
        REGISTER_ATTR_EP1(eATTR_DEL_DIVINE_DAMAGE, &m_ReduceDamage[eATTACK_TYPE_DIVINE],
                      eBOUNDTYPE_MP_10000, eUPDATETYPE_SUM_RATIO,
                      "(Divine) 공격에 대한 데미지 감소");
        REGISTER_ATTR_EP1(eATTR_DEL_PHYSICAL_DAMAGE, &m_ReduceDamage[eATTACK_TYPE_PHYSICAL_OPTION],
                      eBOUNDTYPE_MP_10000, eUPDATETYPE_SUM_RATIO,
                      "(물리) 공격에 대한 데미지 감소");
        REGISTER_ATTR_EP1(eATTR_DEL_MAGICAL_DAMAGE, &m_ReduceDamage[eATTACK_TYPE_MAGIC_OPTION],
                      eBOUNDTYPE_MP_10000, eUPDATETYPE_SUM_RATIO,
                      "(마법) 공격에 대한 데미지 감소");
        REGISTER_ATTR_EP1(eATTR_DEL_MAGICAL_ALL_DAMAGE, &m_ReduceDamage[eATTACK_TYPE_ALL_MAGIC],
                      eBOUNDTYPE_MP_10000, eUPDATETYPE_SUM_RATIO,
                      "(모든 마법) 공격에 대한 데미지 감소");
    };
#endif
    ;{
        // 공격 성공률, 회피율
    #ifdef _NA001605_EP2_ATTRIBUTES_RENEWAL_
        uint8_t update_type_select = eUPDATETYPE_NOTHING;
    #else
        uint8_t update_type_select = eUPDATETYPE_SUM_RATIO;
    #endif
        REGISTER_ATTR(eATTR_PHYSICAL_ATTACK_SUCCESS_RATIO, &m_PhysicalAttackRate,
                      eBOUNDTYPE_ZP_10000, update_type_select,
                      "2:전체 물리 공격 성공률 (비율)");
        REGISTER_ATTR(eATTR_PHYSICAL_ATTACK_BLOCK_RATIO, &m_PhysicalAvoidRate,
                      eBOUNDTYPE_ZP_10000, update_type_select,
                      "15:[R][ 15] 공격 방어율 (옵션) 추가율 (비율)");
    #ifdef _NA001605_EP2_ATTRIBUTES_RENEWAL_
        REGISTER_ATTR(eATTR_PHYSICAL_ATTACK_BLOCK_BASE_ARMOR_RATIO, &m_ArmorPhysicalAvoidRate,
                      eBOUNDTYPE_ZP_10000, eUPDATETYPE_NOTHING,
                      "EP2:added, 공격 방어율 아이템 종합 (총 합산 / 5)");
    #endif
    };
    ;{
        // 속도 
        REGISTER_ATTR(eATTR_MOVE_SPEED, &m_MoveSpeedRatio,
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_NOTHING,
                      "14:이동 속도 추가");
        REGISTER_ATTR(eATTR_ATTACK_SPEED, &m_AttSpeedRatio,
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_NOTHING,
                      "1:전체 공격 속도 추가");
    };
    ;{
        // 보너스 사거리
        REGISTER_ATTR(eATTR_ALL_ATTACK_RANGE, &m_AllRangeBonus,
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_SUM_RATIO,
                      "모든 사거리");
        REGISTER_ATTR(eATTR_NORMAL_ATTACK_RANGE, &m_NormalRangeBonus,
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_SUM_RATIO,
                      "일반 사거리");
        REGISTER_ATTR(eATTR_SKILL_ATTACK_RANGE, &m_SkillRangeBonus,
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_SUM_RATIO,
                      "스킬 사거리");
    };
    ;{
        // 시야
        REGISTER_ATTR(eATTR_SIGHT_RANGE, &m_SightRange,
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_SUM_RATIO,
                      "시야");
    };
    ;{
        // 크리티컬 
        REGISTER_ATTR(eATTR_CRITICAL_RATIO_CHANGE, &m_CriticalRatioChange,
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_SUM_RATIO,
                      "크리티컬 확률 증감");
        REGISTER_ATTR_EP1(eATTR_ADD_MAGICAL_CRITICAL_RATIO, &m_MagicCriticalRatio,
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_SUM_RATIO,
                      "마법 크리티컬 확률 추가");
        REGISTER_ATTR(eATTR_ADD_ALL_CRITICAL_RATIO, &m_CriticalRatioBonus,
                      eBOUNDTYPE_MP_1000, eUPDATETYPE_NOT_RATIO,
                      "41:전체 크리티컬 확률 추가");
        REGISTER_ATTR(eATTR_CRITICAL_DAMAGE_CHANGE, &m_CriticalDamageBonus,
                      eBOUNDTYPE_MP_INFINITE, eUPDATETYPE_SUM_RATIO,
                      "42:크리티컬 데미지 추가");
    };
    ;{
        // 기타
        REGISTER_ATTR(eATTR_ADD_ATTACK_INC_RATIO, &m_AttackIncRate,
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_SUM_RATIO,
                      "추가 공격 가능성 증가 (비율)");
        REGISTER_ATTR(eATTR_ADD_DEFENSE_INC_RATIO, &m_DefenseIncRate,
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_SUM_RATIO,
                      "추가 공격에 대한 방어 가능성 증가 (비율)");
        REGISTER_ATTR(eATTR_INCREASE_SKILL_LEVEL, &m_SkillLevelAll,
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_SUM_RATIO,
                      "스킬 레벨 1상승");
        // (NOTE) Special attribute using option flag
        //REGISTER_ATTR(eATTR_INCREASE_STAT_POINT, NULL,
        //              eBOUNDTYPE_ZP_1000, eUPDATETYPE_SUM_RATIO,
        //              "32 모든 스텟 추가");    //< 값을 가지지 않는다. 상태만 가진다.
    };
    ;{
        // taiyo
        REGISTER_ATTR(eATTR_DECREASE_LIMIT_STAT, &m_DecreaseLimitStat,
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_SUM_RATIO,
                      "33:스텟 제한 수치 감소");
        REGISTER_ATTR(eATTR_MP_SPEND_INCREASE, &m_MPSpendIncrease,
                      eBOUNDTYPE_MP_10000, eUPDATETYPE_NOT_RATIO,
                      "34:MP 소비 증감");
    };
    ;{
        // 상용화 아이템 옵션 추가
        REGISTER_ATTR(eATTR_ABSORB_HP, &m_AbsorbHP,
                      eBOUNDTYPE_ZP_10000, eUPDATETYPE_SUM_RATIO,
                      "37:HP흡수");
        REGISTER_ATTR(eATTR_ABSORB_MP, &m_AbsorbMP,
                      eBOUNDTYPE_ZP_10000, eUPDATETYPE_SUM_RATIO,
                      "38:MP/SP 흡수");
        REGISTER_ATTR(eATTR_REFLECT_DAMAGE_RATIO, &m_ReflectDamageRatio,
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_SUM_RATIO,
                      "43:데미지 반사");
        REGISTER_ATTR(eATTR_BONUS_MONEY_RATIO, &m_BonusMoneyRatio,
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_SUM_RATIO,
                      "44:하임 증가");
        REGISTER_ATTR(eATTR_BONUS_EXP_RATIO, &m_BonusExpRatio,
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_SUM_RATIO,
                      "45:경험치 증가");
        REGISTER_ATTR(eATTR_AREA_ATTACK_RATIO, &m_AreaAttackRatio,
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_SUM_RATIO,
                      "46:다중공격 확률");
        REGISTER_ATTR(eATTR_BONUS_CASTING_TIME, &m_BonusCastingTime,
                      eBOUNDTYPE_MP_1000, eUPDATETYPE_NOT_RATIO,
                      "65:스킬 캐스팅 시간 증감");
        REGISTER_ATTR(eATTR_BONUS_SKILL_COOL_TIME, &m_BonusSkillCoolTime,
                      eBOUNDTYPE_MP_1000, eUPDATETYPE_NOT_RATIO,
                      "EP2:75:[R] 스킬 쿨타임 증감");
        REGISTER_ATTR(eATTR_DECREASE_DAMAGE, &m_DecDamage,
                      eBOUNDTYPE_MP_10000, eUPDATETYPE_NOT_RATIO,
                      "51:데미지 감소");

        REGISTER_ATTR(eATTR_RESURRECTION_RATIO, &m_ResurrectionRatio,
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_SUM_RATIO,
                      "52:사망시 스스로 부활 할 확률");
        REGISTER_ATTR(eATTR_DOUBLE_DAMAGE_RATIO, &m_DoubleDamageRatio,
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_SUM_RATIO,
                      "53:데미지의 두배가 될 확률");
        REGISTER_ATTR(eATTR_LUCKMON_INC_DAMAGE, &m_LuckMonIncDamage,
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_SUM_RATIO,
                      "54:럭키 몬스터 공격시 추가 데미지");
        REGISTER_ATTR(eATTR_COPOSITE_INC_RATIO, &m_CompositeIncRatio,
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_SUM_RATIO,
                      "55:조합 성공률 증가");
        REGISTER_ATTR(eATTR_BYPASS_DEFENCE_RATIO, &m_BypassDeffenceRatio,
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_SUM_RATIO,
                      "56:방어력 무시 확률");
        // 57 최소 데미지 증가(수치)/감소 확률 [m_iItemRatio를 감소확률로 이용한다.]
        REGISTER_ATTR(eATTR_INCREASE_MIN_DAMAGE, &m_IncreaseMinDamage,
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_NOT_RATIO,
                      "57:최소 데미지 증가"); 
        // 58 최대 데미지 증가(수치)/감소 확률 [m_iItemRatio를 감소확률로 이용한다.]
        REGISTER_ATTR(eATTR_INCREASE_MAX_DAMAGE, &m_IncreaseMaxDamage,
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_NOT_RATIO,
                      "58:최대 데미지 증가");
        REGISTER_ATTR(eATTR_DECREASE_ITEMDURA_RATIO, &m_DecreaseItemDuraRatio,
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_SUM_RATIO,
                      "59:아이템 내구력 감소무시 확률");
        // 상태 저항 확률 / 스킬 지속 시간
        REGISTER_ATTR(eATTR_RESISTANCE_BADSTATUS_RATIO, &m_ResistBadStatusRatio,
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_SUM_RATIO,
                      "60:스킬 효과(디버프+특수 상태) 무시 확률");
        REGISTER_ATTR(eATTR_INCREASE_SKILLDURATION, &m_IncreaseSkillDuration,
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_SUM_RATIO,
                      "61:스킬 효과 시간 증가 (버프)");
        REGISTER_ATTR(eATTR_DECREASE_SKILL_SKILLDURATION, &m_DecreaseSkillDuration,
                      eBOUNDTYPE_MP_1000, eUPDATETYPE_SUM_RATIO,
                      "62:스킬 효과 시간 감소 (디버프)"); // CHANGES: f110603.2L, f110531.6L

        // eATTR_TYPE 디파인 제거에 따른 디파인제거
        REGISTER_ATTR(eATTR_OPTION_ETHER_DAMAGE_RATIO, &m_EhterDamageRatio,
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_SUM_RATIO,
                      "63:에테르웨폰 데미지 변화");
        REGISTER_ATTR(eATTR_OPTION_ETHER_PvE_DAMAGE_RATIO, &m_EhterPvEDamageRatio,
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_SUM_RATIO,
                      "76:에테르웨폰 데미지 변화 (PvE)"); // NOTE: f110601.4L
        //__NA_001196_20081110_RECOVERY_ADD_OPTION
        REGISTER_ATTR(eATTR_INCREASE_RESERVE_HP, &m_IncreaseReserveHPRatio,
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_SUM_RATIO,
                      "64:적립 HP 증가");

        //#ifdef _NA_001231_20081216_ADD_OPTION_KIND
        REGISTER_ATTR(eATTR_RESIST_HOLDING, &m_ResistHolding,
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_SUM_RATIO,
                      "66:홀딩 내성");
        REGISTER_ATTR(eATTR_RESIST_SLEEP,   &m_ResistSleep,
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_SUM_RATIO,
                      "67:슬립 내성");
        REGISTER_ATTR(eATTR_RESIST_POISON,  &m_ResistPoison,
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_SUM_RATIO,
                      "68:중독 내성");
        REGISTER_ATTR(eATTR_RESIST_KNOCKBACK, &m_ResistKnockBack,
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_SUM_RATIO,
                      "69:넉백 내성");
        REGISTER_ATTR(eATTR_RESIST_DOWN,    &m_ResistDown,
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_SUM_RATIO,
                      "70:다운 내성");
        REGISTER_ATTR(eATTR_RESIST_STUN,    &m_ResistStun,
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_SUM_RATIO,
                      "71:스턴 내성");
        REGISTER_ATTR(eATTR_DECREASE_PVPDAMAGE, &m_DecreasePVPDamage,
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_SUM_RATIO,
                      "72:PVP데미지 감소");
        //#endif
        // __NA_S00015_20080828_NEW_HOPE_SHADOW
        REGISTER_ATTR(eATTR_ADD_DAMAGE, &m_AddDamage,
                      eBOUNDTYPE_ZP_10000, eUPDATETYPE_SUM_RATIO,
                      "73:추가데미지");
        //__NA_1241_AUTO_PICKUP_PET_OPTION
        REGISTER_ATTR(eATTR_AUTO_ITEM_PICK_UP, &m_AutoPickup,
                      eBOUNDTYPE_ZP_100, eUPDATETYPE_SUM_RATIO,
                      "74:Item 자동 줍기");
        // _NA_003966_20111227_ADD_ENCHANT
        REGISTER_ATTR(eATTR_INCREASE_ENCHANT_RATIO, &m_IncreaseEnchantRatio,
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_SUM_RATIO,
                      "77:인첸트 성공률 증가");
    };
    ;{
        //__NA_001290_20090525_SHIELD_SYSTEM
        REGISTER_ATTR(eATTR_MAX_SD, &m_MaxSD,
                      eBOUNDTYPE_ZP_INFINITE, eUPDATETYPE_SUM_RATIO,
                      "93:최대 SD추가");
        REGISTER_ATTR(eATTR_RECOVERY_SD, &m_RecoverSD,
                      eBOUNDTYPE_MP_10000, eUPDATETYPE_SUM_RATIO,
                      "94:SD 회복량");
    };
#ifdef _NA_006731_20130521_ENCHANT_ADD_OPTION
    ;{
        REGISTER_ATTR(eATTR_ENEMY_CRITICAL_RATIO_CHANGE, &m_EnemyCriticalRatioChange,
                      eBOUNDTYPE_MP_1000, eUPDATETYPE_SUM_RATIO,
                      "96:피격 시 상대의 크리티컬 확률 증감");
    };
#endif //  _NA_006731_20130521_ENCHANT_ADD_OPTION
    ;{
        REGISTER_ATTR(eATTR_PREMIUMSERVICE_PCBANG, &m_PCBang,
                      eBOUNDTYPE_ZP_100, eUPDATETYPE_NOT_RATIO,
                      "97:PC방효과");
    };
#ifdef _NA_006680_20130426_ITEM_OPTION_ADD_AND_MODIFY
    ;{
        REGISTER_ATTR(eATTR_ATTACK_DAMAGE_ABSORB_SD_RATIO, &m_AttackDamageAbsorbSDRatio,
                      eBOUNDTYPE_ZP_100, eUPDATETYPE_SUM_RATIO,
                      "98:가해 데미지 SD 전환량");
        REGISTER_ATTR(eATTR_ATTACK_DAMAGE_ABSORB_HP_RATIO, &m_AttackDamageAbsorbHPRatio,
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_SUM_RATIO,
                      "99:가해 데미지 HP 전환량");
    };
#endif // _NA_006680_20130426_ITEM_OPTION_ADD_AND_MODIFY
#ifdef _NA_006937_20131030_ABILITY_AND_STATE_CHANGE_CRITICAL
    ;{
        REGISTER_ATTR(eATTR_ENEMY_CRITICAL_DAMAGE_CHANGE, &m_EnemyCriticalDamageChange,
                      eBOUNDTYPE_MP_INFINITE, eUPDATETYPE_SUM_RATIO,
                      "100:피격 시 상대의 크리티컬 데미지 증감");
    };
#endif //_NA_006937_20131030_ABILITY_AND_STATE_CHANGE_CRITICAL
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    ;{
        REGISTER_ATTR(eATTR_CRAFT_COST_RATIO, &m_CraftCostRatio,
                      eBOUNDTYPE_MP_100, eUPDATETYPE_SUM_RATIO,
                      "101:제작 비용 증감");
        REGISTER_ATTR(eATTR_CRAFT_PREVENT_EXTINCTION_MATERIAL_RATIO, &m_CraftPreventExtinctionMaterialRatio,
                      eBOUNDTYPE_MP_100, eUPDATETYPE_SUM_RATIO,
                      "102:제작 실패시 재료 소멸 방지");
        REGISTER_ATTR(eATTR_ENCHANT_COST_RATIO, &m_EnchantCostRatio,
                      eBOUNDTYPE_MP_100, eUPDATETYPE_SUM_RATIO,
                      "103:인챈트 비용 증감");
        REGISTER_ATTR(eATTR_ENCHANT_PREVENT_DESTROY_N_DOWNGRADE_ITEM_RATIO, &m_EnchantPreventDestroyNDowngradeItemRatio,
                      eBOUNDTYPE_MP_100, eUPDATETYPE_SUM_RATIO,
                      "104:인챈트 실패시 아이템 소멸, 다운 방지");
        REGISTER_ATTR(eATTR_RECOVER_POTION_COOLTIME_RATIO, &m_RecoverPotionCooltimeRatio,
                      eBOUNDTYPE_MP_100, eUPDATETYPE_SUM_RATIO,
                      "105:회복 포션 쿨타임 증감");
        REGISTER_ATTR(eATTR_RECOVER_POTION_RECOVERY_RATIO, &m_RecoverPotionRecoveryRatio,
                      eBOUNDTYPE_MP_100, eUPDATETYPE_SUM_RATIO,
                      "106:회복 포션 회복량 증감");
        REGISTER_ATTR(eATTR_QUEST_REWARD_EXP_RATIO, &m_QuestRewardExpRatio,
                      eBOUNDTYPE_MP_100, eUPDATETYPE_SUM_RATIO,
                      "107:퀘스트 보상 경험치 증감");
        REGISTER_ATTR(eATTR_MAX_DAMAGE_RATIO, &m_MaxDamageRatio,
                      eBOUNDTYPE_MP_100, eUPDATETYPE_SUM_RATIO,
                      "108:최대 데미지 발생확률 증감");
        REGISTER_ATTR(eATTR_DOMINATION_MAPOBJECT_DAMAGE_RATIO, &m_DominationMapobjectDamageRatio,
                      eBOUNDTYPE_MP_100, eUPDATETYPE_SUM_RATIO,
                      "109:공성 오브젝트 데미지 증감");
        REGISTER_ATTR(eATTR_SHOP_REPAIR_HEIM_RATIO, &m_ShopRepairHeimRatio,
                      eBOUNDTYPE_MP_100, eUPDATETYPE_SUM_RATIO,
                      "110:상점 수리 하임 증감");
        REGISTER_ATTR(eATTR_SHOP_BUY_HEIM_RATIO, &m_ShopBuyHeimRatio,
                      eBOUNDTYPE_MP_100, eUPDATETYPE_SUM_RATIO,
                      "111:상점 구매 하임 증감");
    };
#endif //_NA_007330_20140620_GUILD_SYSTEM_EXTENSION
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        REGISTER_ATTR(eATTR_MAX_FP, &m_MaxFP,
                      eBOUNDTYPE_ZP_100, eUPDATETYPE_SUM_RATIO,
                      "112:최대 FP");
        REGISTER_ATTR(eATTR_RECOVERY_FP, &m_RecoverFP,
                      eBOUNDTYPE_ZP_100, eUPDATETYPE_SUM_RATIO,
                      "113:FP 회복량");
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        REGISTER_ATTR(eATTR_INCREASE_DAMAGE_RATIO, &m_IncreaseDamageRatio,
                      eBOUNDTYPE_MP_1000, eUPDATETYPE_SUM_RATIO,
                      "114:데미지증가");
#ifdef _NA_008124_20150313_AWAKENING_SYSTEM
        REGISTER_ATTR(eATTR_AWAKENING_PROBABILITY, &m_AwakeningProbability,
                      eBOUNDTYPE_MP_1000, eUPDATETYPE_SUM_RATIO,
                      "115:각성확률증감");
#endif // _NA_008124_20150313_AWAKENING_SYSTEM
#ifdef _NA_008486_20150914_TOTAL_BALANCE
        REGISTER_ATTR(eATTR_DEBUFF_DURATION, &m_DebuffDuration,
                      eBOUNDTYPE_ZP_10000, eUPDATETYPE_SUM_RATIO,
                      "116:디버프효과 지속시간 강화");
#endif //_NA_008486_20150914_TOTAL_BALANCE

#ifdef _NA_008540_20151027_ADD_ITEMOPTION_ELITE4
    ;{
        REGISTER_ATTR(eATTR_DECREASE_DAMAGE_NPC, &m_DecreaseDamageNPC,
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_NOT_RATIO,
                      "117:NPC가 주는 데미지 감소");
        REGISTER_ATTR(eATTR_DECREASE_DAMAGE_BERSERKER, &m_DecreaseDamageBerserker,
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_NOT_RATIO,
                      "118:버서커가 주는 데미지 감소");
        REGISTER_ATTR(eATTR_DECREASE_DAMAGE_DRAGONKNIGHT, &m_DecreaseDamageDragonKnight,
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_NOT_RATIO,
                      "119:드래곤나이트가 주는 데미지 감소");
        REGISTER_ATTR(eATTR_DECREASE_DAMAGE_VALKYRIE, &m_DecreaseDamageValkyrie,
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_NOT_RATIO,
                      "120:발키리가 주는 데미지 감소");
        REGISTER_ATTR(eATTR_DECREASE_DAMAGE_ELEMENTALIST, &m_DecreaseDamageElementalist,
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_NOT_RATIO,
                      "121:엘리멘탈리스트가 주는 데미지 감소");
        REGISTER_ATTR(eATTR_DECREASE_DAMAGE_SHADOW, &m_DecreaseDamageShadow,
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_NOT_RATIO,
                      "122:섀도우가 주는 데미지 감소");
        REGISTER_ATTR(eATTR_DECREASE_DAMAGE_MYSTIC, &m_DecreaseDamageMystic,
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_NOT_RATIO,
                      "123:미스틱이 주는 데미지 감소");
        REGISTER_ATTR(eATTR_DECREASE_DAMAGE_HELLROID, &m_DecreaseDamageHellroid,
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_NOT_RATIO,
                      "124:헬로이드가 주는 데미지 감소");
        REGISTER_ATTR(eATTR_DECREASE_DAMAGE_WITCHBLADE, &m_DecreaseDamageWitchBlade,
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_NOT_RATIO,
                      "125:위치블레이드가 주는 데미지 감소");
    };
#endif //_NA_008540_20151027_ADD_ITEMOPTION_ELITE4

    //-----__NA001390_090915_RIDING_SYSTEM__------
    //----이것은 attr_values_에 등록되지 않았다---
    m_RiderSpeedRatio.SetType(eBOUNDTYPE_ZP_1000, eUPDATETYPE_NOTHING);
    //--------------------------------------------

#ifdef _DEBUG
    for (int i = eATTR_TYPE_INVALID + 1; i < eATTR_MAX; ++i)
    {
        if (i != eATTR_INCREASE_STAT_POINT) {
            ASSERT(attr_values_[i].attr_value != NULL);
        }
    }
#endif
}

// added by _NA001605_EP2_ATTRIBUTES_RENEWAL_
void Attributes::GetDefaultMagicalAttributes(
    Elements* const attack,
    Elements* const rate_of_defense_power_reduction_of_target,
    Elements* const damage_reduction_against_attack) const
{
    BOOST_STATIC_ASSERT(_countof(attack->values) == _countof(Elements::kElementsMappedArray));
    if (attack) {
        ZeroMemory(attack, sizeof(*attack));
    }
    if (rate_of_defense_power_reduction_of_target) {
        ZeroMemory(rate_of_defense_power_reduction_of_target,
                   sizeof(*rate_of_defense_power_reduction_of_target));
    };
    if (damage_reduction_against_attack) {
        ZeroMemory(damage_reduction_against_attack,
                   sizeof(*damage_reduction_against_attack));
    };
#ifdef _NA001605_EP2_ATTRIBUTES_RENEWAL_
    // 속성 공격력
    if (attack)
    {
        const eATTACK_TYPE* search_it = Elements::kElementsMappedArray;
        for (int* it = attack->values;
             it != &attack->values[_countof(attack->values)];
             ++it, ++search_it)
        {
            // 속성 공격력 (기본)
            *it = m_MagicalAttPower[*search_it].calc_value_.value_;
        }
    };
  #if SUN_CODE_BACKUP
    // 속성 방어력
    if (defense)
    {
        const eATTACK_TYPE* search_it = Elements::kElementsMappedArray;
        for (int* it = defense->values;
             it != &defense->values[_countof(defense->values)];
             ++it, ++search_it)
        {
            // 속성 방어력 (기본)
            *it = m_MagicalDefPower[*search_it].calc_value_.value_;
            // 공격 타입별 추가 방어력
            *it += m_BonusDefense[*search_it].calc_value_.value_;
        }
    };
  #endif
    // 대상의 방어력 감소율
    if (rate_of_defense_power_reduction_of_target)
    {
        const AttrValue* src_attr_it = m_ReduceElementTargetDefenseRatio;
        for (int* it = rate_of_defense_power_reduction_of_target->values;
             it != &rate_of_defense_power_reduction_of_target->values[
                 _countof(rate_of_defense_power_reduction_of_target->values)];
             ++it, ++src_attr_it)
        {
            *it = src_attr_it->calc_value_.value_;
        }
    };
    // 공격에 대한 데미지 감소율
    if (damage_reduction_against_attack)
    {
        const AttrValue* src_attr_it = m_ReduceElementDamageRatio;
        for (int* it = damage_reduction_against_attack->values;
             it != &damage_reduction_against_attack->values[
                 _countof(damage_reduction_against_attack->values)];
             ++it, ++src_attr_it)
        {
            *it = src_attr_it->calc_value_.value_;
        }
    }
#endif
}

