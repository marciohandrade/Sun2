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
        // �������� �������� �ʴ� ������ ������ ��ü�� �ٸ� ��ġ�� �����ϱ� �����̴�.
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
    PHYSICAL_INDEX_INVALID, //eATTACK_TYPE_ALL_OPTION = 0, // ��� �ɼ�
    0, //eATTACK_TYPE_MELEE, // ����
	1, //eATTACK_TYPE_RANGE, // ���Ÿ�
	PHYSICAL_INDEX_INVALID, //eATTACK_TYPE_WATER, // ��
	PHYSICAL_INDEX_INVALID, //eATTACK_TYPE_FIRE, // ��
	PHYSICAL_INDEX_INVALID, //eATTACK_TYPE_WIND, // �ٶ�
	PHYSICAL_INDEX_INVALID, //eATTACK_TYPE_EARTH, // ����
	PHYSICAL_INDEX_INVALID, //eATTACK_TYPE_DARKNESS, // ����
	PHYSICAL_INDEX_INVALID, //eATTACK_TYPE_DIVINE, // �ż�
	PHYSICAL_INDEX_INVALID, //eATTACK_TYPE_PHYSICAL_OPTION, // ���� �ɼ�
	PHYSICAL_INDEX_INVALID, //eATTACK_TYPE_MAGIC_OPTION, // ���� �ɼ�
	PHYSICAL_INDEX_INVALID, //eATTACK_TYPE_ALL_MAGIC, // ��� �����Ӽ�
	PHYSICAL_INDEX_INVALID, //eATTACK_TYPE_MAGIC, // ����
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
    ELEMENT_INDEX_INVALID, //eATTACK_TYPE_ALL_OPTION = 0, // ��� �ɼ�
    ELEMENT_INDEX_INVALID, //eATTACK_TYPE_MELEE,					// ����
	ELEMENT_INDEX_INVALID, //eATTACK_TYPE_RANGE,					// ���Ÿ�
	0, //eATTACK_TYPE_WATER,					// ��
	1, //eATTACK_TYPE_FIRE,					// ��
	2, //eATTACK_TYPE_WIND,					// �ٶ�
	3, //eATTACK_TYPE_EARTH,					// ����
	4, //eATTACK_TYPE_DARKNESS,				// ����
	ELEMENT_INDEX_INVALID, //eATTACK_TYPE_DIVINE,				// �ż�
	ELEMENT_INDEX_INVALID, //eATTACK_TYPE_PHYSICAL_OPTION,		// ���� �ɼ�
	ELEMENT_INDEX_INVALID, //eATTACK_TYPE_MAGIC_OPTION,			// ���� �ɼ�
	ELEMENT_INDEX_INVALID, //eATTACK_TYPE_ALL_MAGIC,				// ��� �����Ӽ�
	ELEMENT_INDEX_INVALID, //eATTACK_TYPE_MAGIC,					// ����
#if !defined(_NA001605_EP2_ATTRIBUTES_RENEWAL_)
	ELEMENT_INDEX_INVALID, //eATTACK_TYPE_INVALID,
#endif
#ifdef _NA_004965_20120613_NEW_CHARACTER_MYSTIC
    ELEMENT_INDEX_INVALID, //eATTACK_TYPE_SKILL,					// ��ų
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
    //              "��ȿ���� �ʴ� �Ӽ�");
    //
    ;{
        REGISTER_ATTR(eATTR_STR, &m_STR,
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_SUM_RATIO,
                      "23:�� �߰�");
        REGISTER_ATTR(eATTR_DEX, &m_DEX,
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_SUM_RATIO,
                      "24:��ø �߰�");
        REGISTER_ATTR(eATTR_VIT, &m_VIT,
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_SUM_RATIO,
                      "25:ü�� �߰�");
        REGISTER_ATTR(eATTR_INT, &m_INT,
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_SUM_RATIO,
                      "26:���� �߰�");
        REGISTER_ATTR(eATTR_SPR, &m_SPR,
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_SUM_RATIO,
                      "27:���� �߰�");
    };
    ;{
        REGISTER_ATTR_EP1(eATTR_EXPERTY1, &m_Experty1,
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_SUM_RATIO,
                      "����1");
        REGISTER_ATTR_EP1(eATTR_EXPERTY2, &m_Experty2,
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_SUM_RATIO,
                      "����2");
    };
    ;{
        // HP, MP
        REGISTER_ATTR(eATTR_MAX_HP, &m_MaxHP,
                      eBOUNDTYPE_ZP_INFINITE, eUPDATETYPE_SUM_RATIO,
                      "28:�ִ� HP �߰�");
        REGISTER_ATTR(eATTR_MAX_MP, &m_MaxMP,
                      eBOUNDTYPE_ZP_INFINITE, eUPDATETYPE_SUM_RATIO,
                      "29:�ִ� MP/SP �߰�");
        REGISTER_ATTR(eATTR_CUR_HP, &m_CurHP,
                      eBOUNDTYPE_ZP_INFINITE, eUPDATETYPE_SUM_RATIO,
                      "���� HP");
        REGISTER_ATTR(eATTR_CUR_MP, &m_CurMP,
                      eBOUNDTYPE_ZP_INFINITE, eUPDATETYPE_SUM_RATIO,
                      "���� MP");
        REGISTER_ATTR(eATTR_RECOVERY_HP, &m_RecoverHP,
                      eBOUNDTYPE_MP_10000, eUPDATETYPE_SUM_RATIO,
                      "39:HP ȸ����");
        REGISTER_ATTR(eATTR_RECOVERY_MP, &m_RecoverMP,
                      eBOUNDTYPE_MP_10000, eUPDATETYPE_SUM_RATIO,
                      "40:MP ȸ����");
    };
    //-----------------------------------------------------------------
    // ���ݷ� ���� �Ķ����
    //-----------------------------------------------------------------
    ;{
        // �⺻ ����, ���� ���ݷ�
        REGISTER_ATTR(eATTR_BASE_MELEE_MIN_ATTACK_POWER, &m_BaseMeleeMinAttPower,
                      eBOUNDTYPE_ZP_INFINITE, eUPDATETYPE_SUM_RATIO,
                      "�⺻ ���� ���ݷ� �ּ�");
        REGISTER_ATTR(eATTR_BASE_MELEE_MAX_ATTACK_POWER, &m_BaseMeleeMaxAttPower,
                      eBOUNDTYPE_ZP_INFINITE, eUPDATETYPE_SUM_RATIO,
                      "�⺻ ���� ���ݷ� �ִ�");
        REGISTER_ATTR(eATTR_BASE_RANGE_MIN_ATTACK_POWER, &m_BaseRangeMinAttPower,
                      eBOUNDTYPE_ZP_INFINITE, eUPDATETYPE_SUM_RATIO,
                      "�⺻ ���� ���ݷ� �ּ�");
        REGISTER_ATTR(eATTR_BASE_RANGE_MAX_ATTACK_POWER, &m_BaseRangeMaxAttPower,
                      eBOUNDTYPE_ZP_INFINITE, eUPDATETYPE_SUM_RATIO,
                      "�⺻ ���� ���ݷ� �ִ�");
        REGISTER_ATTR_EP1(eATTR_BASE_MAGICAL_MIN_ATTACK_POWER, &m_BaseMagicMinAttPower,
                      eBOUNDTYPE_ZP_INFINITE, eUPDATETYPE_SUM_RATIO,
                      "�⺻ ���� ���ݷ� �ּ�");
        REGISTER_ATTR_EP1(eATTR_BASE_MAGICAL_MAX_ATTACK_POWER, &m_BaseMagicMaxAttPower,
                      eBOUNDTYPE_ZP_INFINITE, eUPDATETYPE_SUM_RATIO,
                      "�⺻ ���� ���ݷ� �ִ�");
    };
    ;{
        // �ɼ� ����, ���� ���ݷ�
        REGISTER_ATTR(eATTR_OPTION_PHYSICAL_ATTACK_POWER, &m_OptionPhysicalAttPower,
                      eBOUNDTYPE_MP_10000, eUPDATETYPE_NOT_RATIO,
                      "47:�ɼ� ���� ���ݷ�");
        REGISTER_ATTR_EP1(eATTR_OPTION_MAGICAL_ATTACK_POWER, &m_OptionMagicAttPower,
                      eBOUNDTYPE_MP_10000, eUPDATETYPE_NOT_RATIO,
                      "48:�ɼ� ���� ���ݷ�");
        REGISTER_ATTR_EP1(eATTR_OPTION_ALL_ATTACK_POWER, &m_OptionAllAttPower,
                      eBOUNDTYPE_MP_10000, eUPDATETYPE_NOT_RATIO,
                      "21:�ɼ� ��ü ���ݷ�");
    };
    ;{
        // �����Ӽ� ���ݷ�
        REGISTER_ATTR(eATTR_MAGICAL_WATER_ATTACK_POWER,
                      &m_MagicalAttPower[eATTACK_TYPE_WATER],
                      eBOUNDTYPE_ZP_INFINITE, eUPDATETYPE_SUM_RATIO,
                      "4:�����Ӽ� ���ݷ�(��) �߰�");
        REGISTER_ATTR(eATTR_MAGICAL_FIRE_ATTACK_POWER,
                      &m_MagicalAttPower[eATTACK_TYPE_FIRE],
                      eBOUNDTYPE_ZP_INFINITE, eUPDATETYPE_SUM_RATIO,
                      "3:�����Ӽ� ���ݷ�(��) �߰�");
        REGISTER_ATTR(eATTR_MAGICAL_WIND_ATTACK_POWER,
                      &m_MagicalAttPower[eATTACK_TYPE_WIND],
                      eBOUNDTYPE_ZP_INFINITE, eUPDATETYPE_SUM_RATIO,
                      "6:�����Ӽ� ���ݷ�(�ٶ�) �߰�");
        REGISTER_ATTR(eATTR_MAGICAL_EARTH_ATTACK_POWER,
                      &m_MagicalAttPower[eATTACK_TYPE_EARTH],
                      eBOUNDTYPE_ZP_INFINITE, eUPDATETYPE_SUM_RATIO,
                      "5:�����Ӽ� ���ݷ�(����) �߰�");
        REGISTER_ATTR(eATTR_MAGICAL_DARKNESS_ATTACK_POWER,
                      &m_MagicalAttPower[eATTACK_TYPE_DARKNESS],
                      eBOUNDTYPE_ZP_INFINITE, eUPDATETYPE_SUM_RATIO,
                      "7 �����Ӽ����ݷ�(����) �߰�");
        REGISTER_ATTR(eATTR_MAGICAL_DIVINE_ATTACK_POWER,
                      &m_MagicalAttPower[eATTACK_TYPE_DIVINE],
                      eBOUNDTYPE_ZP_INFINITE, eUPDATETYPE_SUM_RATIO,
                      "");

        REGISTER_ATTR(eATTR_MAGICAL_ALL_ATTACK_POWER, &m_MagicalAllAttPower,
                      eBOUNDTYPE_ZP_INFINITE, eUPDATETYPE_NOT_RATIO,
                      "35:��� ���� ���ݷ� �߰�");
    };
    ;{
        // ��ų ���ݷ�, ��ų �ۼ�Ʈ �߰� ������
        REGISTER_ATTR(eATTR_ADD_SKILL_ATTACK_POWER, &m_SkillAttackPower,
                      eBOUNDTYPE_ZP_10000, eUPDATETYPE_SUM_RATIO,
                      "30:��ų ���ݷ�");
        REGISTER_ATTR(eATTR_ADD_SKILL_DAMAGE_RATIO, &m_SkillPercentDamage,
                      eBOUNDTYPE_ZP_INFINITE, eUPDATETYPE_SUM_RATIO,
                      "31:��ų ������ �߰���: ��ų �ۼ�Ʈ �߰� ������");
        // eBOUNDTYPE_ZP_1000->INFINITE ����[CHANGUP_INTENSIFY_SKILLDAMAGE_STATUS]
    };
    //-----------------------------------------------------------------
    // ���� ���� �Ķ����
    //-----------------------------------------------------------------
    ;{
        // �⺻ ����, ���� ����
        REGISTER_ATTR(eATTR_BASE_MELEE_DEFENSE_POWER, &m_BaseMeleeDefPower,
                      eBOUNDTYPE_ZP_INFINITE, eUPDATETYPE_SUM_RATIO,
                      "�⺻ ���� ����");
        REGISTER_ATTR(eATTR_BASE_RANGE_DEFENSE_POWER, &m_BaseRangeDefPower,
                      eBOUNDTYPE_ZP_INFINITE, eUPDATETYPE_SUM_RATIO,
                      "�⺻ ���� ���� ����");
        REGISTER_ATTR_EP1(eATTR_BASE_MAGICAL_DEFENSE_POWER, &m_BaseMagicDefPower,
                      eBOUNDTYPE_ZP_INFINITE, eUPDATETYPE_SUM_RATIO,
                      "�⺻ ���� ����");
    };
    ;{
        // �ɼ� ����, ���� ����
        REGISTER_ATTR(eATTR_OPTION_PHYSICAL_DEFENSE_POWER, &m_OptionPhysicalDefPower,
                      eBOUNDTYPE_MP_10000, eUPDATETYPE_NOT_RATIO,
                      "49:���������߰�");
        REGISTER_ATTR_EP1(eATTR_OPTION_MAGICAL_DEFENSE_POWER, &m_OptionMagicDefPower,
                      eBOUNDTYPE_MP_10000, eUPDATETYPE_NOT_RATIO,
                      "50:���������߰�");
        REGISTER_ATTR_EP1(eATTR_OPTION_ALL_DEFENSE_POWER, &m_OptionAllDefPower,
                      eBOUNDTYPE_MP_10000, eUPDATETYPE_NOT_RATIO,
                      "22:��ü�����߰� (����+����)");
    };
    ;{
        // �����Ӽ� ����(�ּ�)
        REGISTER_ATTR_EP1(eATTR_MAGICAL_WATER_DEFENSE_POWER,
                      &m_MagicalDefPower[eATTACK_TYPE_WATER],
                      eBOUNDTYPE_ZP_INFINITE, eUPDATETYPE_SUM_RATIO,
                      "17:�����Ӽ�����(��)�߰�");
        REGISTER_ATTR_EP1(eATTR_MAGICAL_FIRE_DEFENSE_POWER,
                      &m_MagicalDefPower[eATTACK_TYPE_FIRE],
                      eBOUNDTYPE_ZP_INFINITE, eUPDATETYPE_SUM_RATIO,
                      "16:�����Ӽ�����(��)�߰�");
        REGISTER_ATTR_EP1(eATTR_MAGICAL_WIND_DEFENSE_POWER,
                      &m_MagicalDefPower[eATTACK_TYPE_WIND],
                      eBOUNDTYPE_ZP_INFINITE, eUPDATETYPE_SUM_RATIO,
                      "19:�����Ӽ�����(�ٶ�)�߰�");
        REGISTER_ATTR_EP1(eATTR_MAGICAL_EARTH_DEFENSE_POWER,
                      &m_MagicalDefPower[eATTACK_TYPE_EARTH],
                      eBOUNDTYPE_ZP_INFINITE, eUPDATETYPE_SUM_RATIO,
                      "18:�����Ӽ�����(����)�߰�");
        REGISTER_ATTR_EP1(eATTR_MAGICAL_DARKNESS_DEFENSE_POWER,
                      &m_MagicalDefPower[eATTACK_TYPE_DARKNESS],
                      eBOUNDTYPE_ZP_INFINITE, eUPDATETYPE_SUM_RATIO,
                      "20:�����Ӽ�����(����)�߰�");
        REGISTER_ATTR_EP1(eATTR_MAGICAL_DIVINE_DEFENSE_POWER,
                      &m_MagicalDefPower[eATTACK_TYPE_DIVINE],
                      eBOUNDTYPE_ZP_INFINITE, eUPDATETYPE_SUM_RATIO,
                      "");

        REGISTER_ATTR_EP1(eATTR_MAGICAL_ALL_DEFENSE_POWER,
                      &m_MagicalAllDefPower,
                      eBOUNDTYPE_ZP_INFINITE, eUPDATETYPE_NOT_RATIO,
                      "36:��� �Ӽ� ���� �߰�");
    };
    ;{
        // ���� Ÿ�Ժ� �߰� ����
        REGISTER_ATTR_EP1(eATTR_ADD_ALL_DEFENSE_POWER,
                      &m_BonusDefense[eATTACK_TYPE_ALL_OPTION],
                      eBOUNDTYPE_MP_10000, eUPDATETYPE_SUM_RATIO,
                      "��� ���� Ÿ�Ժ� �߰� ����");
        REGISTER_ATTR_EP1(eATTR_ADD_MELEE_DEFENSE_POWER,
                      &m_BonusDefense[eATTACK_TYPE_MELEE],
                      eBOUNDTYPE_MP_10000, eUPDATETYPE_SUM_RATIO,
                      "���� ���� ���� �߰�");
        REGISTER_ATTR_EP1(eATTR_ADD_RANGE_DEFENSE_POWER,
                      &m_BonusDefense[eATTACK_TYPE_RANGE],
                      eBOUNDTYPE_MP_10000, eUPDATETYPE_SUM_RATIO,
                      "���� ���� ���� �߰�");
        REGISTER_ATTR_EP1(eATTR_ADD_WATER_DEFENSE_POWER,
                      &m_BonusDefense[eATTACK_TYPE_WATER],
                      eBOUNDTYPE_MP_10000, eUPDATETYPE_SUM_RATIO,
                      "(��) ���� ���� �߰�");
        REGISTER_ATTR_EP1(eATTR_ADD_FIRE_DEFENSE_POWER,
                      &m_BonusDefense[eATTACK_TYPE_FIRE],
                      eBOUNDTYPE_MP_10000, eUPDATETYPE_SUM_RATIO,
                      "(��) ���� ���� �߰�");
        REGISTER_ATTR_EP1(eATTR_ADD_WIND_DEFENSE_POWER,
                      &m_BonusDefense[eATTACK_TYPE_WIND],
                      eBOUNDTYPE_MP_10000, eUPDATETYPE_SUM_RATIO,
                      "(�ٶ�) ���� ���� �߰�");
        REGISTER_ATTR_EP1(eATTR_ADD_EARTH_DEFENSE_POWER,
                      &m_BonusDefense[eATTACK_TYPE_EARTH],
                      eBOUNDTYPE_MP_10000, eUPDATETYPE_SUM_RATIO,
                      "(����) ���� ���� �߰�");
        REGISTER_ATTR_EP1(eATTR_ADD_DARKNESS_DEFENSE_POWER,
                      &m_BonusDefense[eATTACK_TYPE_DARKNESS],
                      eBOUNDTYPE_MP_10000, eUPDATETYPE_SUM_RATIO,
                      "(����) ���� ���� �߰�");
        REGISTER_ATTR_EP1(eATTR_ADD_DIVINE_DEFENSE_POWER,
                      &m_BonusDefense[eATTACK_TYPE_DIVINE],
                      eBOUNDTYPE_MP_10000, eUPDATETYPE_SUM_RATIO,
                      "(��) ���� ���� �߰�");
        REGISTER_ATTR_EP1(eATTR_ADD_PHYSICAL_DEFENSE_POWER,
                      &m_BonusDefense[eATTACK_TYPE_PHYSICAL_OPTION],
                      eBOUNDTYPE_MP_10000, eUPDATETYPE_SUM_RATIO,
                      "(����) ���� ���� �߰�");
        REGISTER_ATTR_EP1(eATTR_ADD_MAGICAL_DEFENSE_POWER,
                      &m_BonusDefense[eATTACK_TYPE_MAGIC_OPTION],
                      eBOUNDTYPE_MP_10000, eUPDATETYPE_SUM_RATIO,
                      "(����) ���� ���� �߰�");
        REGISTER_ATTR_EP1(eATTR_ADD_MAGICAL_ALL_DEFENSE_POWER,
                      &m_BonusDefense[eATTACK_TYPE_ALL_MAGIC],
                      eBOUNDTYPE_MP_10000, eUPDATETYPE_SUM_RATIO,
                      "(��� ����) ���� ���� �߰�");
    };
#ifdef _NA001605_EP2_ATTRIBUTES_RENEWAL_
    ;{
        // ���� ����� ���� ������
        REGISTER_ATTR(eATTR_DEL_PHYSICAL_TARGET_DEFENSE_RATIO,
            &m_ReducePhysicalTargetDefenseRatio,
            eBOUNDTYPE_MP_1000, eUPDATETYPE_SUM_RATIO,
            "8:���� ���ݿ� ���� ����� ���� ������");
        REGISTER_ATTR(eATTR_DEL_WATER_TARGET_DEFENSE_RATIO,
            &m_ReduceElementTargetDefenseRatio[Elements::kToElementIndex[eATTACK_TYPE_WATER]],
            eBOUNDTYPE_MP_1000, eUPDATETYPE_SUM_RATIO,
            "10:����(��) ���ݿ� ���� ����� ���� ������");
        REGISTER_ATTR(eATTR_DEL_FIRE_TARGET_DEFENSE_RATIO,
            &m_ReduceElementTargetDefenseRatio[Elements::kToElementIndex[eATTACK_TYPE_FIRE]],
            eBOUNDTYPE_MP_1000, eUPDATETYPE_SUM_RATIO,
            "9:����(��) ���ݿ� ���� ����� ���� ������");
        REGISTER_ATTR(eATTR_DEL_WIND_TARGET_DEFENSE_RATIO,
            &m_ReduceElementTargetDefenseRatio[Elements::kToElementIndex[eATTACK_TYPE_WIND]],
            eBOUNDTYPE_MP_1000, eUPDATETYPE_SUM_RATIO,
            "12:����(�ٶ�) ���ݿ� ���� ����� ���� ������");
        REGISTER_ATTR(eATTR_DEL_EARTH_TARGET_DEFENSE_RATIO,
            &m_ReduceElementTargetDefenseRatio[Elements::kToElementIndex[eATTACK_TYPE_EARTH]],
            eBOUNDTYPE_MP_1000, eUPDATETYPE_SUM_RATIO,
            "11:����(����) ���ݿ� ���� ����� ���� ������");
        REGISTER_ATTR(eATTR_DEL_DARKNESS_TARGET_DEFENSE_RATIO,
            &m_ReduceElementTargetDefenseRatio[Elements::kToElementIndex[eATTACK_TYPE_DARKNESS]],
            eBOUNDTYPE_MP_1000, eUPDATETYPE_SUM_RATIO,
            "13:����(����) ���ݿ� ���� ����� ���� ������");
        REGISTER_ATTR(eATTR_DEL_MAGICAL_ALL_TARGET_DEFENSE_RATIO,
            &m_ReduceElementTargetDefenseRatioAll,
            eBOUNDTYPE_MP_1000, eUPDATETYPE_SUM_RATIO,
            "����(��� ����) ���ݿ� ���� ����� ���� ������");
        REGISTER_ATTR_UNUSED(eATTR_DEL_ALL_TARGET_DEFENSE_RATIO);
        REGISTER_ATTR_UNUSED(eATTR_DEL_MELEE_TARGET_DEFENSE_RATIO);
        REGISTER_ATTR_UNUSED(eATTR_DEL_RANGE_TARGET_DEFENSE_RATIO);
        REGISTER_ATTR_UNUSED(eATTR_DEL_DIVINE_TARGET_DEFENSE_RATIO);
        REGISTER_ATTR_UNUSED(eATTR_DEL_MAGICAL_TARGET_DEFENSE_RATIO);
    };
#else
    ;{
        // ���� ����� ���� ������
        REGISTER_ATTR_EP1(eATTR_DEL_ALL_TARGET_DEFENSE_RATIO,
                      &m_ReduceDefenseRate[eATTACK_TYPE_ALL_OPTION],
                      eBOUNDTYPE_MP_1000, eUPDATETYPE_SUM_RATIO,
                      "(���) ���ݿ� ���� ����� ���� ������");
        REGISTER_ATTR(eATTR_DEL_MELEE_TARGET_DEFENSE_RATIO,
                      &m_ReduceDefenseRate[eATTACK_TYPE_MELEE],
                      eBOUNDTYPE_MP_1000, eUPDATETYPE_SUM_RATIO,
                      "(����) ���ݿ� ���� ����� ���� ������");
        REGISTER_ATTR(eATTR_DEL_RANGE_TARGET_DEFENSE_RATIO,
                      &m_ReduceDefenseRate[eATTACK_TYPE_RANGE],
                      eBOUNDTYPE_MP_1000, eUPDATETYPE_SUM_RATIO,
                      "(����) ���ݿ� ���� ����� ���� ������");
        REGISTER_ATTR(eATTR_DEL_WATER_TARGET_DEFENSE_RATIO,
                      &m_ReduceDefenseRate[eATTACK_TYPE_WATER],
                      eBOUNDTYPE_MP_1000, eUPDATETYPE_SUM_RATIO,
                      "10:(��) ���ݿ� ���� ����� ���� ������");
        REGISTER_ATTR(eATTR_DEL_FIRE_TARGET_DEFENSE_RATIO,
                      &m_ReduceDefenseRate[eATTACK_TYPE_FIRE],
                      eBOUNDTYPE_MP_1000, eUPDATETYPE_SUM_RATIO,
                      "9:(��) ���ݿ� ���� ����� ���� ������");
        REGISTER_ATTR(eATTR_DEL_WIND_TARGET_DEFENSE_RATIO,
                      &m_ReduceDefenseRate[eATTACK_TYPE_WIND],
                      eBOUNDTYPE_MP_1000, eUPDATETYPE_SUM_RATIO,
                      "12:(�ٶ�) ���ݿ� ���� ����� ���� ������");
        REGISTER_ATTR(eATTR_DEL_EARTH_TARGET_DEFENSE_RATIO,
                      &m_ReduceDefenseRate[eATTACK_TYPE_EARTH],
                      eBOUNDTYPE_MP_1000, eUPDATETYPE_SUM_RATIO,
                      "11:(����) ���ݿ� ���� ����� ���� ������");
        REGISTER_ATTR(eATTR_DEL_DARKNESS_TARGET_DEFENSE_RATIO,
                      &m_ReduceDefenseRate[eATTACK_TYPE_DARKNESS],
                      eBOUNDTYPE_MP_1000, eUPDATETYPE_SUM_RATIO,
                      "13:(����) ���ݿ� ���� ����� ���� ������");
        REGISTER_ATTR_EP1(eATTR_DEL_DIVINE_TARGET_DEFENSE_RATIO,
                      &m_ReduceDefenseRate[eATTACK_TYPE_DIVINE],
                      eBOUNDTYPE_MP_1000, eUPDATETYPE_SUM_RATIO,
                      "(Divine) ���ݿ� ���� ����� ���� ������");
        REGISTER_ATTR(eATTR_DEL_PHYSICAL_TARGET_DEFENSE_RATIO,
                      &m_ReduceDefenseRate[eATTACK_TYPE_PHYSICAL_OPTION],
                      eBOUNDTYPE_MP_1000, eUPDATETYPE_SUM_RATIO,
                      "8:(����) ���ݿ� ���� ����� ���� ������");
        REGISTER_ATTR_EP1(eATTR_DEL_MAGICAL_TARGET_DEFENSE_RATIO,
                      &m_ReduceDefenseRate[eATTACK_TYPE_MAGIC_OPTION],
                      eBOUNDTYPE_MP_1000, eUPDATETYPE_SUM_RATIO,
                      "(����) ���ݿ� ���� ����� ���� ������");
        REGISTER_ATTR(eATTR_DEL_MAGICAL_ALL_TARGET_DEFENSE_RATIO,
                      &m_ReduceDefenseRate[eATTACK_TYPE_ALL_MAGIC],
                      eBOUNDTYPE_MP_1000, eUPDATETYPE_SUM_RATIO,
                      "(��� ����) ���ݿ� ���� ����� ���� ������");
    };
#endif
    //-----------------------------------------------------------------
    // ������ ���� �Ķ����
    //-----------------------------------------------------------------
    ;{
        // �Ƹ� Ÿ�Ժ� �߰� ������
        REGISTER_ATTR_EP1(eATTR_ADD_ARMOR_HARD_DAMAGE, &m_BonusDamage[eARMOR_HARD],
                      eBOUNDTYPE_MP_10000, eUPDATETYPE_SUM_RATIO,
                      "(Hard) �Ƹӿ� ���� �߰� ������");
        REGISTER_ATTR_EP1(eATTR_ADD_ARMOR_MEDIUM_DAMAGE, &m_BonusDamage[eARMOR_MEDIUM],
                      eBOUNDTYPE_MP_10000, eUPDATETYPE_SUM_RATIO,
                      "(Medium) �Ƹӿ� ���� �߰� ������");
        REGISTER_ATTR_EP1(eATTR_ADD_ARMOR_SOFT_DAMAGE, &m_BonusDamage[eARMOR_SOFT],
                      eBOUNDTYPE_MP_10000, eUPDATETYPE_SUM_RATIO,
                      "(Soft) �Ƹӿ� ���� �߰� ������");
        REGISTER_ATTR_EP1(eATTR_ADD_ARMOR_SIEGE_DAMAGE, &m_BonusDamage[eARMOR_SIEGE],
                      eBOUNDTYPE_MP_10000, eUPDATETYPE_SUM_RATIO,
                      "(Siege) �Ƹӿ� ���� �߰� ������");
        REGISTER_ATTR_EP1(eATTR_ADD_ARMOR_UNARMOR_DAMAGE, &m_BonusDamage[eARMOR_UNARMOR],
                      eBOUNDTYPE_MP_10000, eUPDATETYPE_SUM_RATIO,
                      "(Unarmor) �Ƹӿ� ���� �߰� ������");
    };
    ;{
        // �Ƹ� Ÿ�Ժ� �߰� ������(�ۼ�Ʈ)
        REGISTER_ATTR_EP1(eATTR_ADD_RATIO_ARMOR_HARD_DAMAGE, &m_BonusPercentDamage[eARMOR_HARD],
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_SUM_RATIO,
                      "(Hard) �Ƹӿ� ���� �߰� ������ (������)");
        REGISTER_ATTR_EP1(eATTR_ADD_RATIO_ARMOR_MEDIUM_DAMAGE, &m_BonusPercentDamage[eARMOR_MEDIUM],
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_SUM_RATIO,
                      "(Medium) �Ƹӿ� ���� �߰� ������ (������)");
        REGISTER_ATTR_EP1(eATTR_ADD_RATIO_ARMOR_SOFT_DAMAGE, &m_BonusPercentDamage[eARMOR_SOFT],
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_SUM_RATIO,
                      "(Soft) �Ƹӿ� ���� �߰� ������ (������)");
        REGISTER_ATTR_EP1(eATTR_ADD_RATIO_ARMOR_SIEGE_DAMAGE, &m_BonusPercentDamage[eARMOR_SIEGE],
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_SUM_RATIO,
                      "(Siege) �Ƹӿ� ���� �߰� ������ (������)");
        REGISTER_ATTR_EP1(eATTR_ADD_RATIO_ARMOR_UNARMOR_DAMAGE, &m_BonusPercentDamage[eARMOR_UNARMOR],
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_SUM_RATIO,
                      "(Unarmor) �Ƹӿ� ���� �߰� ������ (������)");
    };
#ifdef _NA001605_EP2_ATTRIBUTES_RENEWAL_
    ;{
        // ������ ����
        REGISTER_ATTR(eATTR_DEL_PHYSICAL_DAMAGE,
                      &m_ReducePhysicalDamageRatio,
                      eBOUNDTYPE_MP_1000, eUPDATETYPE_SUM_RATIO,
                      "���� ���ݿ� ���� ������ ������");
        REGISTER_ATTR(eATTR_DEL_WATER_DAMAGE,
                      &m_ReduceElementDamageRatio[Elements::kToElementIndex[eATTACK_TYPE_WATER]],
                      eBOUNDTYPE_MP_1000, eUPDATETYPE_SUM_RATIO,
                      "����(��) ���ݿ� ���� ������ ������");
        REGISTER_ATTR(eATTR_DEL_FIRE_DAMAGE,
                      &m_ReduceElementDamageRatio[Elements::kToElementIndex[eATTACK_TYPE_FIRE]],
                      eBOUNDTYPE_MP_1000, eUPDATETYPE_SUM_RATIO,
                      "����(��) ���ݿ� ���� ������ ������");
        REGISTER_ATTR(eATTR_DEL_WIND_DAMAGE,
                      &m_ReduceElementDamageRatio[Elements::kToElementIndex[eATTACK_TYPE_WIND]],
                      eBOUNDTYPE_MP_1000, eUPDATETYPE_SUM_RATIO,
                      "����(�ٶ�) ���ݿ� ���� ������ ������");
        REGISTER_ATTR(eATTR_DEL_EARTH_DAMAGE,
                      &m_ReduceElementDamageRatio[Elements::kToElementIndex[eATTACK_TYPE_EARTH]],
                      eBOUNDTYPE_MP_1000, eUPDATETYPE_SUM_RATIO,
                      "����(����) ���ݿ� ���� ������ ������");
        REGISTER_ATTR(eATTR_DEL_DARKNESS_DAMAGE,
                      &m_ReduceElementDamageRatio[Elements::kToElementIndex[eATTACK_TYPE_DARKNESS]],
                      eBOUNDTYPE_MP_1000, eUPDATETYPE_SUM_RATIO,
                      "����(����) ���ݿ� ���� ������ ������");
        REGISTER_ATTR(eATTR_DEL_MAGICAL_ALL_DAMAGE,
                      &m_ReduceElementDamageRatioAll,
                      eBOUNDTYPE_MP_1000, eUPDATETYPE_SUM_RATIO,
                      "����(���) ���ݿ� ���� ������ ������");
        REGISTER_ATTR_UNUSED(eATTR_DEL_ALL_DAMAGE);
        REGISTER_ATTR_UNUSED(eATTR_DEL_MELEE_DAMAGE);
        REGISTER_ATTR_UNUSED(eATTR_DEL_RANGE_DAMAGE);
        REGISTER_ATTR_UNUSED(eATTR_DEL_DIVINE_DAMAGE);
        REGISTER_ATTR_UNUSED(eATTR_DEL_PHYSICAL_DAMAGE);
        REGISTER_ATTR_UNUSED(eATTR_DEL_MAGICAL_DAMAGE);
    };
#else
    ;{
        // ������ ����
        REGISTER_ATTR_EP1(eATTR_DEL_ALL_DAMAGE, &m_ReduceDamage[eATTACK_TYPE_ALL_OPTION],
                      eBOUNDTYPE_MP_10000, eUPDATETYPE_SUM_RATIO,
                      "(���) ���ݿ� ���� ������ ����");
        REGISTER_ATTR_EP1(eATTR_DEL_MELEE_DAMAGE, &m_ReduceDamage[eATTACK_TYPE_MELEE],
                      eBOUNDTYPE_MP_10000, eUPDATETYPE_SUM_RATIO,
                      "(����) ���ݿ� ���� ������ ����");
        REGISTER_ATTR_EP1(eATTR_DEL_RANGE_DAMAGE, &m_ReduceDamage[eATTACK_TYPE_RANGE],
                      eBOUNDTYPE_MP_10000, eUPDATETYPE_SUM_RATIO,
                      "(����) ���ݿ� ���� ������ ����");
        REGISTER_ATTR_EP1(eATTR_DEL_WATER_DAMAGE, &m_ReduceDamage[eATTACK_TYPE_WATER],
                      eBOUNDTYPE_MP_10000, eUPDATETYPE_SUM_RATIO,
                      "(��) ���ݿ� ���� ������ ����");
        REGISTER_ATTR_EP1(eATTR_DEL_FIRE_DAMAGE, &m_ReduceDamage[eATTACK_TYPE_FIRE],
                      eBOUNDTYPE_MP_10000, eUPDATETYPE_SUM_RATIO,
                      "(��) ���ݿ� ���� ������ ����");
        REGISTER_ATTR_EP1(eATTR_DEL_WIND_DAMAGE, &m_ReduceDamage[eATTACK_TYPE_WIND],
                      eBOUNDTYPE_MP_10000, eUPDATETYPE_SUM_RATIO,
                      "(�ٶ�) ���ݿ� ���� ������ ����");
        REGISTER_ATTR_EP1(eATTR_DEL_EARTH_DAMAGE, &m_ReduceDamage[eATTACK_TYPE_EARTH],
                      eBOUNDTYPE_MP_10000, eUPDATETYPE_SUM_RATIO,
                      "(����) ���ݿ� ���� ������ ����");
        REGISTER_ATTR_EP1(eATTR_DEL_DARKNESS_DAMAGE, &m_ReduceDamage[eATTACK_TYPE_DARKNESS],
                      eBOUNDTYPE_MP_10000, eUPDATETYPE_SUM_RATIO,
                      "(����) ���ݿ� ���� ������ ����");
        REGISTER_ATTR_EP1(eATTR_DEL_DIVINE_DAMAGE, &m_ReduceDamage[eATTACK_TYPE_DIVINE],
                      eBOUNDTYPE_MP_10000, eUPDATETYPE_SUM_RATIO,
                      "(Divine) ���ݿ� ���� ������ ����");
        REGISTER_ATTR_EP1(eATTR_DEL_PHYSICAL_DAMAGE, &m_ReduceDamage[eATTACK_TYPE_PHYSICAL_OPTION],
                      eBOUNDTYPE_MP_10000, eUPDATETYPE_SUM_RATIO,
                      "(����) ���ݿ� ���� ������ ����");
        REGISTER_ATTR_EP1(eATTR_DEL_MAGICAL_DAMAGE, &m_ReduceDamage[eATTACK_TYPE_MAGIC_OPTION],
                      eBOUNDTYPE_MP_10000, eUPDATETYPE_SUM_RATIO,
                      "(����) ���ݿ� ���� ������ ����");
        REGISTER_ATTR_EP1(eATTR_DEL_MAGICAL_ALL_DAMAGE, &m_ReduceDamage[eATTACK_TYPE_ALL_MAGIC],
                      eBOUNDTYPE_MP_10000, eUPDATETYPE_SUM_RATIO,
                      "(��� ����) ���ݿ� ���� ������ ����");
    };
#endif
    ;{
        // ���� ������, ȸ����
    #ifdef _NA001605_EP2_ATTRIBUTES_RENEWAL_
        uint8_t update_type_select = eUPDATETYPE_NOTHING;
    #else
        uint8_t update_type_select = eUPDATETYPE_SUM_RATIO;
    #endif
        REGISTER_ATTR(eATTR_PHYSICAL_ATTACK_SUCCESS_RATIO, &m_PhysicalAttackRate,
                      eBOUNDTYPE_ZP_10000, update_type_select,
                      "2:��ü ���� ���� ������ (����)");
        REGISTER_ATTR(eATTR_PHYSICAL_ATTACK_BLOCK_RATIO, &m_PhysicalAvoidRate,
                      eBOUNDTYPE_ZP_10000, update_type_select,
                      "15:[R][ 15] ���� ����� (�ɼ�) �߰��� (����)");
    #ifdef _NA001605_EP2_ATTRIBUTES_RENEWAL_
        REGISTER_ATTR(eATTR_PHYSICAL_ATTACK_BLOCK_BASE_ARMOR_RATIO, &m_ArmorPhysicalAvoidRate,
                      eBOUNDTYPE_ZP_10000, eUPDATETYPE_NOTHING,
                      "EP2:added, ���� ����� ������ ���� (�� �ջ� / 5)");
    #endif
    };
    ;{
        // �ӵ� 
        REGISTER_ATTR(eATTR_MOVE_SPEED, &m_MoveSpeedRatio,
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_NOTHING,
                      "14:�̵� �ӵ� �߰�");
        REGISTER_ATTR(eATTR_ATTACK_SPEED, &m_AttSpeedRatio,
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_NOTHING,
                      "1:��ü ���� �ӵ� �߰�");
    };
    ;{
        // ���ʽ� ��Ÿ�
        REGISTER_ATTR(eATTR_ALL_ATTACK_RANGE, &m_AllRangeBonus,
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_SUM_RATIO,
                      "��� ��Ÿ�");
        REGISTER_ATTR(eATTR_NORMAL_ATTACK_RANGE, &m_NormalRangeBonus,
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_SUM_RATIO,
                      "�Ϲ� ��Ÿ�");
        REGISTER_ATTR(eATTR_SKILL_ATTACK_RANGE, &m_SkillRangeBonus,
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_SUM_RATIO,
                      "��ų ��Ÿ�");
    };
    ;{
        // �þ�
        REGISTER_ATTR(eATTR_SIGHT_RANGE, &m_SightRange,
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_SUM_RATIO,
                      "�þ�");
    };
    ;{
        // ũ��Ƽ�� 
        REGISTER_ATTR(eATTR_CRITICAL_RATIO_CHANGE, &m_CriticalRatioChange,
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_SUM_RATIO,
                      "ũ��Ƽ�� Ȯ�� ����");
        REGISTER_ATTR_EP1(eATTR_ADD_MAGICAL_CRITICAL_RATIO, &m_MagicCriticalRatio,
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_SUM_RATIO,
                      "���� ũ��Ƽ�� Ȯ�� �߰�");
        REGISTER_ATTR(eATTR_ADD_ALL_CRITICAL_RATIO, &m_CriticalRatioBonus,
                      eBOUNDTYPE_MP_1000, eUPDATETYPE_NOT_RATIO,
                      "41:��ü ũ��Ƽ�� Ȯ�� �߰�");
        REGISTER_ATTR(eATTR_CRITICAL_DAMAGE_CHANGE, &m_CriticalDamageBonus,
                      eBOUNDTYPE_MP_INFINITE, eUPDATETYPE_SUM_RATIO,
                      "42:ũ��Ƽ�� ������ �߰�");
    };
    ;{
        // ��Ÿ
        REGISTER_ATTR(eATTR_ADD_ATTACK_INC_RATIO, &m_AttackIncRate,
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_SUM_RATIO,
                      "�߰� ���� ���ɼ� ���� (����)");
        REGISTER_ATTR(eATTR_ADD_DEFENSE_INC_RATIO, &m_DefenseIncRate,
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_SUM_RATIO,
                      "�߰� ���ݿ� ���� ��� ���ɼ� ���� (����)");
        REGISTER_ATTR(eATTR_INCREASE_SKILL_LEVEL, &m_SkillLevelAll,
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_SUM_RATIO,
                      "��ų ���� 1���");
        // (NOTE) Special attribute using option flag
        //REGISTER_ATTR(eATTR_INCREASE_STAT_POINT, NULL,
        //              eBOUNDTYPE_ZP_1000, eUPDATETYPE_SUM_RATIO,
        //              "32 ��� ���� �߰�");    //< ���� ������ �ʴ´�. ���¸� ������.
    };
    ;{
        // taiyo
        REGISTER_ATTR(eATTR_DECREASE_LIMIT_STAT, &m_DecreaseLimitStat,
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_SUM_RATIO,
                      "33:���� ���� ��ġ ����");
        REGISTER_ATTR(eATTR_MP_SPEND_INCREASE, &m_MPSpendIncrease,
                      eBOUNDTYPE_MP_10000, eUPDATETYPE_NOT_RATIO,
                      "34:MP �Һ� ����");
    };
    ;{
        // ���ȭ ������ �ɼ� �߰�
        REGISTER_ATTR(eATTR_ABSORB_HP, &m_AbsorbHP,
                      eBOUNDTYPE_ZP_10000, eUPDATETYPE_SUM_RATIO,
                      "37:HP���");
        REGISTER_ATTR(eATTR_ABSORB_MP, &m_AbsorbMP,
                      eBOUNDTYPE_ZP_10000, eUPDATETYPE_SUM_RATIO,
                      "38:MP/SP ���");
        REGISTER_ATTR(eATTR_REFLECT_DAMAGE_RATIO, &m_ReflectDamageRatio,
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_SUM_RATIO,
                      "43:������ �ݻ�");
        REGISTER_ATTR(eATTR_BONUS_MONEY_RATIO, &m_BonusMoneyRatio,
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_SUM_RATIO,
                      "44:���� ����");
        REGISTER_ATTR(eATTR_BONUS_EXP_RATIO, &m_BonusExpRatio,
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_SUM_RATIO,
                      "45:����ġ ����");
        REGISTER_ATTR(eATTR_AREA_ATTACK_RATIO, &m_AreaAttackRatio,
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_SUM_RATIO,
                      "46:���߰��� Ȯ��");
        REGISTER_ATTR(eATTR_BONUS_CASTING_TIME, &m_BonusCastingTime,
                      eBOUNDTYPE_MP_1000, eUPDATETYPE_NOT_RATIO,
                      "65:��ų ĳ���� �ð� ����");
        REGISTER_ATTR(eATTR_BONUS_SKILL_COOL_TIME, &m_BonusSkillCoolTime,
                      eBOUNDTYPE_MP_1000, eUPDATETYPE_NOT_RATIO,
                      "EP2:75:[R] ��ų ��Ÿ�� ����");
        REGISTER_ATTR(eATTR_DECREASE_DAMAGE, &m_DecDamage,
                      eBOUNDTYPE_MP_10000, eUPDATETYPE_NOT_RATIO,
                      "51:������ ����");

        REGISTER_ATTR(eATTR_RESURRECTION_RATIO, &m_ResurrectionRatio,
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_SUM_RATIO,
                      "52:����� ������ ��Ȱ �� Ȯ��");
        REGISTER_ATTR(eATTR_DOUBLE_DAMAGE_RATIO, &m_DoubleDamageRatio,
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_SUM_RATIO,
                      "53:�������� �ι谡 �� Ȯ��");
        REGISTER_ATTR(eATTR_LUCKMON_INC_DAMAGE, &m_LuckMonIncDamage,
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_SUM_RATIO,
                      "54:��Ű ���� ���ݽ� �߰� ������");
        REGISTER_ATTR(eATTR_COPOSITE_INC_RATIO, &m_CompositeIncRatio,
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_SUM_RATIO,
                      "55:���� ������ ����");
        REGISTER_ATTR(eATTR_BYPASS_DEFENCE_RATIO, &m_BypassDeffenceRatio,
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_SUM_RATIO,
                      "56:���� ���� Ȯ��");
        // 57 �ּ� ������ ����(��ġ)/���� Ȯ�� [m_iItemRatio�� ����Ȯ���� �̿��Ѵ�.]
        REGISTER_ATTR(eATTR_INCREASE_MIN_DAMAGE, &m_IncreaseMinDamage,
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_NOT_RATIO,
                      "57:�ּ� ������ ����"); 
        // 58 �ִ� ������ ����(��ġ)/���� Ȯ�� [m_iItemRatio�� ����Ȯ���� �̿��Ѵ�.]
        REGISTER_ATTR(eATTR_INCREASE_MAX_DAMAGE, &m_IncreaseMaxDamage,
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_NOT_RATIO,
                      "58:�ִ� ������ ����");
        REGISTER_ATTR(eATTR_DECREASE_ITEMDURA_RATIO, &m_DecreaseItemDuraRatio,
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_SUM_RATIO,
                      "59:������ ������ ���ҹ��� Ȯ��");
        // ���� ���� Ȯ�� / ��ų ���� �ð�
        REGISTER_ATTR(eATTR_RESISTANCE_BADSTATUS_RATIO, &m_ResistBadStatusRatio,
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_SUM_RATIO,
                      "60:��ų ȿ��(�����+Ư�� ����) ���� Ȯ��");
        REGISTER_ATTR(eATTR_INCREASE_SKILLDURATION, &m_IncreaseSkillDuration,
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_SUM_RATIO,
                      "61:��ų ȿ�� �ð� ���� (����)");
        REGISTER_ATTR(eATTR_DECREASE_SKILL_SKILLDURATION, &m_DecreaseSkillDuration,
                      eBOUNDTYPE_MP_1000, eUPDATETYPE_SUM_RATIO,
                      "62:��ų ȿ�� �ð� ���� (�����)"); // CHANGES: f110603.2L, f110531.6L

        // eATTR_TYPE ������ ���ſ� ���� ����������
        REGISTER_ATTR(eATTR_OPTION_ETHER_DAMAGE_RATIO, &m_EhterDamageRatio,
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_SUM_RATIO,
                      "63:���׸����� ������ ��ȭ");
        REGISTER_ATTR(eATTR_OPTION_ETHER_PvE_DAMAGE_RATIO, &m_EhterPvEDamageRatio,
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_SUM_RATIO,
                      "76:���׸����� ������ ��ȭ (PvE)"); // NOTE: f110601.4L
        //__NA_001196_20081110_RECOVERY_ADD_OPTION
        REGISTER_ATTR(eATTR_INCREASE_RESERVE_HP, &m_IncreaseReserveHPRatio,
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_SUM_RATIO,
                      "64:���� HP ����");

        //#ifdef _NA_001231_20081216_ADD_OPTION_KIND
        REGISTER_ATTR(eATTR_RESIST_HOLDING, &m_ResistHolding,
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_SUM_RATIO,
                      "66:Ȧ�� ����");
        REGISTER_ATTR(eATTR_RESIST_SLEEP,   &m_ResistSleep,
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_SUM_RATIO,
                      "67:���� ����");
        REGISTER_ATTR(eATTR_RESIST_POISON,  &m_ResistPoison,
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_SUM_RATIO,
                      "68:�ߵ� ����");
        REGISTER_ATTR(eATTR_RESIST_KNOCKBACK, &m_ResistKnockBack,
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_SUM_RATIO,
                      "69:�˹� ����");
        REGISTER_ATTR(eATTR_RESIST_DOWN,    &m_ResistDown,
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_SUM_RATIO,
                      "70:�ٿ� ����");
        REGISTER_ATTR(eATTR_RESIST_STUN,    &m_ResistStun,
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_SUM_RATIO,
                      "71:���� ����");
        REGISTER_ATTR(eATTR_DECREASE_PVPDAMAGE, &m_DecreasePVPDamage,
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_SUM_RATIO,
                      "72:PVP������ ����");
        //#endif
        // __NA_S00015_20080828_NEW_HOPE_SHADOW
        REGISTER_ATTR(eATTR_ADD_DAMAGE, &m_AddDamage,
                      eBOUNDTYPE_ZP_10000, eUPDATETYPE_SUM_RATIO,
                      "73:�߰�������");
        //__NA_1241_AUTO_PICKUP_PET_OPTION
        REGISTER_ATTR(eATTR_AUTO_ITEM_PICK_UP, &m_AutoPickup,
                      eBOUNDTYPE_ZP_100, eUPDATETYPE_SUM_RATIO,
                      "74:Item �ڵ� �ݱ�");
        // _NA_003966_20111227_ADD_ENCHANT
        REGISTER_ATTR(eATTR_INCREASE_ENCHANT_RATIO, &m_IncreaseEnchantRatio,
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_SUM_RATIO,
                      "77:��þƮ ������ ����");
    };
    ;{
        //__NA_001290_20090525_SHIELD_SYSTEM
        REGISTER_ATTR(eATTR_MAX_SD, &m_MaxSD,
                      eBOUNDTYPE_ZP_INFINITE, eUPDATETYPE_SUM_RATIO,
                      "93:�ִ� SD�߰�");
        REGISTER_ATTR(eATTR_RECOVERY_SD, &m_RecoverSD,
                      eBOUNDTYPE_MP_10000, eUPDATETYPE_SUM_RATIO,
                      "94:SD ȸ����");
    };
#ifdef _NA_006731_20130521_ENCHANT_ADD_OPTION
    ;{
        REGISTER_ATTR(eATTR_ENEMY_CRITICAL_RATIO_CHANGE, &m_EnemyCriticalRatioChange,
                      eBOUNDTYPE_MP_1000, eUPDATETYPE_SUM_RATIO,
                      "96:�ǰ� �� ����� ũ��Ƽ�� Ȯ�� ����");
    };
#endif //  _NA_006731_20130521_ENCHANT_ADD_OPTION
    ;{
        REGISTER_ATTR(eATTR_PREMIUMSERVICE_PCBANG, &m_PCBang,
                      eBOUNDTYPE_ZP_100, eUPDATETYPE_NOT_RATIO,
                      "97:PC��ȿ��");
    };
#ifdef _NA_006680_20130426_ITEM_OPTION_ADD_AND_MODIFY
    ;{
        REGISTER_ATTR(eATTR_ATTACK_DAMAGE_ABSORB_SD_RATIO, &m_AttackDamageAbsorbSDRatio,
                      eBOUNDTYPE_ZP_100, eUPDATETYPE_SUM_RATIO,
                      "98:���� ������ SD ��ȯ��");
        REGISTER_ATTR(eATTR_ATTACK_DAMAGE_ABSORB_HP_RATIO, &m_AttackDamageAbsorbHPRatio,
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_SUM_RATIO,
                      "99:���� ������ HP ��ȯ��");
    };
#endif // _NA_006680_20130426_ITEM_OPTION_ADD_AND_MODIFY
#ifdef _NA_006937_20131030_ABILITY_AND_STATE_CHANGE_CRITICAL
    ;{
        REGISTER_ATTR(eATTR_ENEMY_CRITICAL_DAMAGE_CHANGE, &m_EnemyCriticalDamageChange,
                      eBOUNDTYPE_MP_INFINITE, eUPDATETYPE_SUM_RATIO,
                      "100:�ǰ� �� ����� ũ��Ƽ�� ������ ����");
    };
#endif //_NA_006937_20131030_ABILITY_AND_STATE_CHANGE_CRITICAL
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    ;{
        REGISTER_ATTR(eATTR_CRAFT_COST_RATIO, &m_CraftCostRatio,
                      eBOUNDTYPE_MP_100, eUPDATETYPE_SUM_RATIO,
                      "101:���� ��� ����");
        REGISTER_ATTR(eATTR_CRAFT_PREVENT_EXTINCTION_MATERIAL_RATIO, &m_CraftPreventExtinctionMaterialRatio,
                      eBOUNDTYPE_MP_100, eUPDATETYPE_SUM_RATIO,
                      "102:���� ���н� ��� �Ҹ� ����");
        REGISTER_ATTR(eATTR_ENCHANT_COST_RATIO, &m_EnchantCostRatio,
                      eBOUNDTYPE_MP_100, eUPDATETYPE_SUM_RATIO,
                      "103:��æƮ ��� ����");
        REGISTER_ATTR(eATTR_ENCHANT_PREVENT_DESTROY_N_DOWNGRADE_ITEM_RATIO, &m_EnchantPreventDestroyNDowngradeItemRatio,
                      eBOUNDTYPE_MP_100, eUPDATETYPE_SUM_RATIO,
                      "104:��æƮ ���н� ������ �Ҹ�, �ٿ� ����");
        REGISTER_ATTR(eATTR_RECOVER_POTION_COOLTIME_RATIO, &m_RecoverPotionCooltimeRatio,
                      eBOUNDTYPE_MP_100, eUPDATETYPE_SUM_RATIO,
                      "105:ȸ�� ���� ��Ÿ�� ����");
        REGISTER_ATTR(eATTR_RECOVER_POTION_RECOVERY_RATIO, &m_RecoverPotionRecoveryRatio,
                      eBOUNDTYPE_MP_100, eUPDATETYPE_SUM_RATIO,
                      "106:ȸ�� ���� ȸ���� ����");
        REGISTER_ATTR(eATTR_QUEST_REWARD_EXP_RATIO, &m_QuestRewardExpRatio,
                      eBOUNDTYPE_MP_100, eUPDATETYPE_SUM_RATIO,
                      "107:����Ʈ ���� ����ġ ����");
        REGISTER_ATTR(eATTR_MAX_DAMAGE_RATIO, &m_MaxDamageRatio,
                      eBOUNDTYPE_MP_100, eUPDATETYPE_SUM_RATIO,
                      "108:�ִ� ������ �߻�Ȯ�� ����");
        REGISTER_ATTR(eATTR_DOMINATION_MAPOBJECT_DAMAGE_RATIO, &m_DominationMapobjectDamageRatio,
                      eBOUNDTYPE_MP_100, eUPDATETYPE_SUM_RATIO,
                      "109:���� ������Ʈ ������ ����");
        REGISTER_ATTR(eATTR_SHOP_REPAIR_HEIM_RATIO, &m_ShopRepairHeimRatio,
                      eBOUNDTYPE_MP_100, eUPDATETYPE_SUM_RATIO,
                      "110:���� ���� ���� ����");
        REGISTER_ATTR(eATTR_SHOP_BUY_HEIM_RATIO, &m_ShopBuyHeimRatio,
                      eBOUNDTYPE_MP_100, eUPDATETYPE_SUM_RATIO,
                      "111:���� ���� ���� ����");
    };
#endif //_NA_007330_20140620_GUILD_SYSTEM_EXTENSION
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        REGISTER_ATTR(eATTR_MAX_FP, &m_MaxFP,
                      eBOUNDTYPE_ZP_100, eUPDATETYPE_SUM_RATIO,
                      "112:�ִ� FP");
        REGISTER_ATTR(eATTR_RECOVERY_FP, &m_RecoverFP,
                      eBOUNDTYPE_ZP_100, eUPDATETYPE_SUM_RATIO,
                      "113:FP ȸ����");
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        REGISTER_ATTR(eATTR_INCREASE_DAMAGE_RATIO, &m_IncreaseDamageRatio,
                      eBOUNDTYPE_MP_1000, eUPDATETYPE_SUM_RATIO,
                      "114:����������");
#ifdef _NA_008124_20150313_AWAKENING_SYSTEM
        REGISTER_ATTR(eATTR_AWAKENING_PROBABILITY, &m_AwakeningProbability,
                      eBOUNDTYPE_MP_1000, eUPDATETYPE_SUM_RATIO,
                      "115:����Ȯ������");
#endif // _NA_008124_20150313_AWAKENING_SYSTEM
#ifdef _NA_008486_20150914_TOTAL_BALANCE
        REGISTER_ATTR(eATTR_DEBUFF_DURATION, &m_DebuffDuration,
                      eBOUNDTYPE_ZP_10000, eUPDATETYPE_SUM_RATIO,
                      "116:�����ȿ�� ���ӽð� ��ȭ");
#endif //_NA_008486_20150914_TOTAL_BALANCE

#ifdef _NA_008540_20151027_ADD_ITEMOPTION_ELITE4
    ;{
        REGISTER_ATTR(eATTR_DECREASE_DAMAGE_NPC, &m_DecreaseDamageNPC,
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_NOT_RATIO,
                      "117:NPC�� �ִ� ������ ����");
        REGISTER_ATTR(eATTR_DECREASE_DAMAGE_BERSERKER, &m_DecreaseDamageBerserker,
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_NOT_RATIO,
                      "118:����Ŀ�� �ִ� ������ ����");
        REGISTER_ATTR(eATTR_DECREASE_DAMAGE_DRAGONKNIGHT, &m_DecreaseDamageDragonKnight,
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_NOT_RATIO,
                      "119:�巡�ﳪ��Ʈ�� �ִ� ������ ����");
        REGISTER_ATTR(eATTR_DECREASE_DAMAGE_VALKYRIE, &m_DecreaseDamageValkyrie,
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_NOT_RATIO,
                      "120:��Ű���� �ִ� ������ ����");
        REGISTER_ATTR(eATTR_DECREASE_DAMAGE_ELEMENTALIST, &m_DecreaseDamageElementalist,
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_NOT_RATIO,
                      "121:������Ż����Ʈ�� �ִ� ������ ����");
        REGISTER_ATTR(eATTR_DECREASE_DAMAGE_SHADOW, &m_DecreaseDamageShadow,
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_NOT_RATIO,
                      "122:�����찡 �ִ� ������ ����");
        REGISTER_ATTR(eATTR_DECREASE_DAMAGE_MYSTIC, &m_DecreaseDamageMystic,
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_NOT_RATIO,
                      "123:�̽�ƽ�� �ִ� ������ ����");
        REGISTER_ATTR(eATTR_DECREASE_DAMAGE_HELLROID, &m_DecreaseDamageHellroid,
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_NOT_RATIO,
                      "124:����̵尡 �ִ� ������ ����");
        REGISTER_ATTR(eATTR_DECREASE_DAMAGE_WITCHBLADE, &m_DecreaseDamageWitchBlade,
                      eBOUNDTYPE_ZP_1000, eUPDATETYPE_NOT_RATIO,
                      "125:��ġ���̵尡 �ִ� ������ ����");
    };
#endif //_NA_008540_20151027_ADD_ITEMOPTION_ELITE4

    //-----__NA001390_090915_RIDING_SYSTEM__------
    //----�̰��� attr_values_�� ��ϵ��� �ʾҴ�---
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
    // �Ӽ� ���ݷ�
    if (attack)
    {
        const eATTACK_TYPE* search_it = Elements::kElementsMappedArray;
        for (int* it = attack->values;
             it != &attack->values[_countof(attack->values)];
             ++it, ++search_it)
        {
            // �Ӽ� ���ݷ� (�⺻)
            *it = m_MagicalAttPower[*search_it].calc_value_.value_;
        }
    };
  #if SUN_CODE_BACKUP
    // �Ӽ� ����
    if (defense)
    {
        const eATTACK_TYPE* search_it = Elements::kElementsMappedArray;
        for (int* it = defense->values;
             it != &defense->values[_countof(defense->values)];
             ++it, ++search_it)
        {
            // �Ӽ� ���� (�⺻)
            *it = m_MagicalDefPower[*search_it].calc_value_.value_;
            // ���� Ÿ�Ժ� �߰� ����
            *it += m_BonusDefense[*search_it].calc_value_.value_;
        }
    };
  #endif
    // ����� ���� ������
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
    // ���ݿ� ���� ������ ������
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

