#pragma once
#ifndef PROGRAMCOMMON_ATTRIBUTE_H
#define PROGRAMCOMMON_ATTRIBUTE_H

#include <assert.h>
#include "misc.h"

#pragma pack(push)
#pragma pack()

class BoundPolicies;
class AttrUpdatePolicy;
class Attributes;
class PlayerAttributes;
class NPCAttributes;
class AttrValue;
class ItemAttrCalculator;
class SkillAttrCalculator;

//==================================================================================================
// ���� �ٲٸ� �ȵȴ�.
enum eATTR_KIND
{
    eATTR_KIND_CALC,        // ������
    eATTR_KIND_BASE,        // �⺻��
    eATTR_KIND_ITEM,        // ���������� ��ȭ�� ��
    eATTR_KIND_SKILL,       // ��ų�� ��ȭ�� ��
    eATTR_KIND_ITEM_RATIO,  // �����ۿ� ���� ������
    eATTR_KIND_SKILL_RATIO, // ��ų�� ���� ������
    eATTR_KIND_RATIO,       // ��ü�� ���� ������
    eATTR_KIND_MAX,
};

enum eBOUNDTYPE
{
    eBOUNDTYPE_MP_100,          // MinusPlus100
    eBOUNDTYPE_MP_1000,
    eBOUNDTYPE_MP_10000,
    eBOUNDTYPE_MP_INFINITE,

    eBOUNDTYPE_ZP_100,          // ZeroPlus100
    eBOUNDTYPE_ZP_1000,
    eBOUNDTYPE_ZP_10000,
    eBOUNDTYPE_ZP_INFINITE,

    eBOUNDTYPE_MAX,
};

//==================================================================================================
//  MinMaxBoundPolicy : min~max bound check

struct MinMaxBoundPolicy
{
    virtual int touch(int check_value) const = 0;

#if !defined(_SERVER)
    virtual const int GetMaxValue() const = 0;
#endif //!defined(_SERVER)
};

template<eBOUNDTYPE BoundType, int Min, int Max>
struct MinMaxBoundPolicyT : public MinMaxBoundPolicy
{
    static const eBOUNDTYPE kBoundType = BoundType;
    static const int kMinValue = Min;
    static const int kMaxValue = Max;
    BOOST_STATIC_ASSERT(kMinValue <= kMaxValue);
    virtual int touch(int check_value) const {
        return BOUND_CHECK(kMinValue, check_value, kMaxValue);
    }

#if !defined(_SERVER)
    virtual const int GetMaxValue() const {
        return kMaxValue;
    }
#endif //!defined(_SERVER)
};

class BoundPolicies
{
public:
    BoundPolicies();
    ~BoundPolicies() {}

    int touch(eBOUNDTYPE bound_type, int check_value) const;

#if !defined(_SERVER)
    const int GetMaxValue(eBOUNDTYPE bound_type) const {
        assert(0 <= bound_type && bound_type < eBOUNDTYPE_MAX);
        return (policy_list_[bound_type])->GetMaxValue();
    }
#endif //!defined(_SERVER)
    
private:
    const MinMaxBoundPolicy* policy_list_[eBOUNDTYPE_MAX];
    //
    MinMaxBoundPolicyT<eBOUNDTYPE_MP_100,       -100,       100>         policy_mp_100_;
    MinMaxBoundPolicyT<eBOUNDTYPE_MP_1000,      -1000,      1000>        policy_mp_1000_;
    MinMaxBoundPolicyT<eBOUNDTYPE_MP_10000,     -10000,     10000>       policy_mp_10000_;
    MinMaxBoundPolicyT<eBOUNDTYPE_MP_INFINITE,  -2000000000,2000000000>  policy_mp_infinite_;

    MinMaxBoundPolicyT<eBOUNDTYPE_ZP_100,       0,          100>         policy_zp_100_;
    MinMaxBoundPolicyT<eBOUNDTYPE_ZP_1000,      0,          1000>        policy_zp_1000_;
    MinMaxBoundPolicyT<eBOUNDTYPE_ZP_10000,     0,          10000>       policy_zp_10000_;
    MinMaxBoundPolicyT<eBOUNDTYPE_ZP_INFINITE,  0,          2000000000>  policy_zp_infinite_;
};

//extern BoundPolicies s_BoundPolicies;

//==================================================================================================
// INTValue : int ������ ������ ���� ���꿡 ���� BoundCheckPolicy�� �����Ѵ�.
//
// @BoundCheck
//  void _touch(); // modification after change value
//  (f100716.4L) (NOTE) in OnUpdate(Ex), INTValues of attribute items should be call 'touch'
class INTValue
{
public:
    INTValue() : value_(0) {}//, bound_type_(eBOUNDTYPE_MP_INFINITE) {}
    INTValue(int value) : value_(value) {}
    //void SetBoundType(BYTE bound_type) { bound_type_ = bound_type; }

    operator int() const { return value_; }
    operator short() const { return static_cast<short>(value_); }
    operator ulong() const { return value_; }
    operator float() const { return static_cast<float>(value_); }
    //---------------------------------------------------------------------------------------------
    // fast debug step, busy access point
    void operator+=(int value) { value_ += value; }
    void operator-=(int value) { value_ -= value; }
    void operator= (int value) { value_ =  value; }
    void operator*=(int value) { value_ *= static_cast<int>(value); }
    void operator/=(int value) { value_ /= value; }
    //---------------------------------------------------------------------------------------------
    // fast debug step
    void operator+=(ulong value) { value_ += static_cast<int>(value); }
    void operator-=(ulong value) { value_ -= static_cast<int>(value); }
    void operator= (ulong value) { value_ =  static_cast<int>(value); }
    //void operator *= (ulong value) { value_ *= static_cast<int>(value); }
    //void operator /= (ulong value) { value_ /= static_cast<int>(value); }
    //---------------------------------------------------------------------------------------------
    void operator+=(const INTValue& value) { value_ += value.value_; }
    void operator-=(const INTValue& value) { value_ -= value.value_; }
    void operator= (const INTValue& value) { value_ =  value.value_; }
    //void operator *= (const INTValue& value) { value_ *= static_cast<int>(value); }
    //void operator /= (const INTValue& value) { value_ /= static_cast<int>(value); }
    //---------------------------------------------------------------------------------------------
    template<class T>
    void operator+=(const T& value) {
        BOOST_STATIC_ASSERT(boost::is_integral<T>::value || boost::is_float<T>::value);
        value_ += static_cast<int>(value);
    }
    template<class T>
    void operator-=(const T& value) {
        BOOST_STATIC_ASSERT(boost::is_integral<T>::value || boost::is_float<T>::value);
        value_ -= static_cast<int>(value);
    }
    template<class T>
    void operator= (const T& value) {
        BOOST_STATIC_ASSERT(boost::is_integral<T>::value || boost::is_float<T>::value);
        value_ =  static_cast<int>(value);
    }
    /* unused
    template<class T>
    void operator *= (const T& value) {
        BOOST_STATIC_ASSERT(boost::is_integral<T>::value || boost::is_float<T>::value);
        value_ *= static_cast<int>(value);
    }
    template<class T>
    void operator /= (const T& value) {
        BOOST_STATIC_ASSERT(boost::is_integral<T>::value || boost::is_float<T>::value);
        value_ /= static_cast<int>(value);
    }
    */
    int touch(eBOUNDTYPE bound_type);
protected:
    //
    int value_;
    //BYTE bound_type_;
    friend class AttrUpdatePolicy;
    friend class Attributes;
    friend class PlayerAttributes;
    friend class NPCAttributes;
    friend class AttrValue;
    friend class ItemAttrCalculator;
    friend class SkillAttrCalculator;
};

//==================================================================================================
//  AttrSumPolicy : �� ������ ���ؼ� CALC�� �����Ѵ�. 
//
// eUPDATETYPE_NOTHING,    AttrValue::Update()���� ������� �ʴ´�. ���� ����ϴ� ������ �־����.
// eUPDATETYPE_NOT_RATIO,  AttrValue::Update()���� ��ġ���� ������ ����Ѵ�.
// eUPDATETYPE_SUM_RATIO,  AttrValue::Update()���� ��ġ�� ������ ����� �� �������� ���� �ش�.
enum eUPDATETYPE
{
    eUPDATETYPE_NOTHING,
    eUPDATETYPE_NOT_RATIO,
    eUPDATETYPE_SUM_RATIO,
    eUPDATETYPE_MAX,
};

//==================================================================================================
// AttrValue : INTValue�� ������ eATTR_KIND�� ���� �� ���� �Ѱ��ش�.
//

class AttrValue
{
public:
    AttrValue();
    ~AttrValue() {}

    void Init();
    void SetType(eBOUNDTYPE bound_type, eUPDATETYPE update_type);
    INTValue& operator[](eATTR_KIND attr_kind);
    int operator[](eATTR_KIND attr_kind) const;
    void Update();

private:
    INTValue calc_value_;    // ������ calc_value_ = (base_value_+item_value_+skill_value_)*m_iRatio/100
    INTValue base_value_;    // �⺻��
    INTValue item_value_;    // ���������� ��ȭ�� ��
    INTValue skill_value_;   // ��ų�� ��ȭ�� ��

    // �ε��Ҽ��� ������ ����Ȯ�� ������ 100�� ���ؼ� ����Ѵ�!
    INTValue item_ratio_;
    INTValue skill_ratio_;
    static INTValue calc_ratio_static_;

    uint8_t bound_type_;
    uint8_t update_type_;

    friend class Attributes;
    friend class PlayerAttributes;
    friend class NPCAttributes;
    friend class ItemAttrCalculator;
    friend class SkillAttrCalculator;
};

inline AttrValue::AttrValue()
{
    ZeroMemory(this, sizeof(*this));
    bound_type_  = eBOUNDTYPE_MP_INFINITE;
    update_type_ = eUPDATETYPE_SUM_RATIO;
}

inline void AttrValue::Init()
{
    // changes to prevent a client compile problems occurred by a client debug macro 'new'
    ZeroMemory(this, sizeof(*this));
    bound_type_  = eBOUNDTYPE_MP_INFINITE;
    update_type_ = eUPDATETYPE_SUM_RATIO;
}

inline void AttrValue::SetType(eBOUNDTYPE bound_type, eUPDATETYPE update_type) 
{ 
    bound_type_  = static_cast<uint8_t>(bound_type);
    update_type_ = static_cast<uint8_t>(update_type);
}

//==================================================================================================
// GetRatio##attr() <- item_ratio + skill_ratio
#define __DECLARE_ATTR_VALUE(attr) \
  public: \
    int Get##attr() const      { return m_##attr.calc_value_.value_; } \
    int GetBase##attr() const  { return m_##attr.base_value_.value_; } \
    int GetItem##attr() const  { return m_##attr.item_value_.value_; } \
    int GetRatio##attr() const { return m_##attr[eATTR_KIND_RATIO]; } \
  protected: \
    AttrValue m_##attr;

#ifdef _NA001605_EP2_ATTRIBUTES_RENEWAL_
    #define __DECLARE_ATTR_VALUE_EP1(attr) \
      public: \
        int Get##attr() const      { return 0; } \
        int GetBase##attr() const  { return 0; } \
        int GetItem##attr() const  { return 0; } \
        int GetRatio##attr() const { return 0; }
#else
    #define __DECLARE_ATTR_VALUE_EP1(attr) __DECLARE_ATTR_VALUE(##attr)
#endif

//
//==================================================================================================
// Attributes : ĳ������ �Ӽ������� ������.
//
class Attributes
{
public:
    struct Operation {
        enum eOper {
            eOper_Set, eOper_Add, eOper_Sub, /*eOper_Mul, eOper_Div,*/ eOper_Counts
        };
        uint16_t attr_type;
    };
    // (f100809.3L) add an interface to support containing a specific information per record
    struct OperationRecord {
        uint8_t operate; // ref Operation::eOper
        uint8_t attr_kind;
        uint16_t attr_type;
        int attr_value;
    };
    // f101224.1L, added by _NA001605_EP2_ATTRIBUTES_RENEWAL_
    struct Physicals {
        enum ePhysical { Melee, Range, Counts };
        static const ulong kAcceptTypesField = (1 << eATTACK_TYPE_MELEE)
                                             | (1 << eATTACK_TYPE_RANGE);
        static const eATTACK_TYPE kPhysicalsMappedArray[Counts]; // items = { eATTACK_TYPE_MELEE, ... };
        static const int kToPhysicalIndex[eATTACK_TYPE_MAX];
        //
        int values[Counts];
    };
    struct Elements {
        enum eElement { Water, Fire, Wind, Earth, Darkness, Counts };
        static const ulong kAcceptTypesField = (1 << eATTACK_TYPE_WATER)
                                             | (1 << eATTACK_TYPE_FIRE)
                                             | (1 << eATTACK_TYPE_WIND)
                                             | (1 << eATTACK_TYPE_EARTH)
                                             | (1 << eATTACK_TYPE_DARKNESS);
        static const eATTACK_TYPE kElementsMappedArray[Counts]; // items = { eATTACK_TYPE_WATER, ... };
        static const int kToElementIndex[eATTACK_TYPE_MAX];
        //
        int values[Counts];
    };
    //
    Attributes();
    virtual ~Attributes();

    AttrValue& operator[](eATTR_TYPE attr_type) {
        assert(attr_type < eATTR_MAX);
        return *attr_values_[attr_type].attr_value;
    }
    const AttrValue& operator[](eATTR_TYPE attr_type) const {
        assert(attr_type < eATTR_MAX);
        return *attr_values_[attr_type].attr_value;
    }
    bool IsNULL(eATTR_TYPE attr_type) const {
        return (attr_values_[attr_type].attr_value == NULL);
    }
    // NOTE: f110217.4L, add interface function type properties not using operator
    int GetAttrValue(eATTR_TYPE attr_type) const;
    int GetAttrValue(eATTR_TYPE attr_type, eATTR_KIND attr_kind) const;
    void SetAttrValue(eATTR_TYPE attr_type, int value);
    void SetAttrValue(eATTR_TYPE attr_type, eATTR_KIND attr_kind, int value);
    //
    virtual void Clear() = 0;
    virtual void Update() = 0;
    virtual void UpdateEx() = 0;
    // NOTE: f100720.3L, special purpose interfaces
    //  eATTR_KIND_SKILL = clear fields { eATTR_KIND_SKILL, eATTR_KIND_SKILL_RATIO }
    //  eATTR_KIND_ITEM = clear fields { eATTR_KIND_ITEM, eATTR_KIND_ITEM_RATIO }
    void ClearRelatedAttrKind(eATTR_KIND attr_kind);
    // external multi-operate command support interface
    bool GroupOperate(const Operation::eOper operate_cmd, const eATTR_KIND attr_kind,
                      const Operation* attr_array, const int* value_array,
                      const int number_of_data);
    // NOTE: f100809.3L, external multi-operate command support interface to support records update
    bool GroupBatchOperate(const OperationRecord* records, const int number_of_records);

#if !defined(_SERVER)
    int GetAttrMaxBoundValue(eATTR_TYPE attr_type) const; // calc_value ������ MAX ���� ����
#endif //!defined(_SERVER)

protected:
    // is_reuse_data : to support cloning
    void RegisterAll(bool is_reuse_data = false);
    void RegisterAttr(eATTR_TYPE attr_type, AttrValue* attr_value,
                      eBOUNDTYPE bound_type, eUPDATETYPE update_type = eUPDATETYPE_SUM_RATIO);

protected:
    struct AttrNode {
        AttrValue* attr_value;
#if defined(_DEBUG) || defined(_SERVER)
        eATTR_TYPE attr_type;
        const char* attr_type_str;
        const char* attr_field;
        const char* attr_desc;
#endif
    };
    // (WARNING) don't declare virtual object in the attributes field section
    AttrNode attr_values_[eATTR_MAX];    // �� attr���� �迭�� ������ �� �ְ� �Ѵ�.

    __DECLARE_ATTR_VALUE(STR);        // ��
    __DECLARE_ATTR_VALUE(DEX);        // ��ø 
    __DECLARE_ATTR_VALUE(VIT);        // ü�� 
    __DECLARE_ATTR_VALUE(SPR);        // ���ŷ�
    __DECLARE_ATTR_VALUE(INT);        // ���� 

    // ���õ�
    __DECLARE_ATTR_VALUE_EP1(Experty1);
    __DECLARE_ATTR_VALUE_EP1(Experty2);

    // HP, MP
    __DECLARE_ATTR_VALUE(MaxHP);
    __DECLARE_ATTR_VALUE(MaxMP);
    __DECLARE_ATTR_VALUE(CurHP);      // ���� ü��(����������, ���� ���Ǵ� �Ķ�����̱� ������ Update�� ���� ���ϸ� ���̰��� Player���� ���� ������)
    __DECLARE_ATTR_VALUE(CurMP);      // ���� mana(����������)
    __DECLARE_ATTR_VALUE(RecoverHP);  // HP ȸ����
    __DECLARE_ATTR_VALUE(RecoverMP);  // MP ȸ����

    // ���� ������, ȸ����
    __DECLARE_ATTR_VALUE(PhysicalAttackRate);
    __DECLARE_ATTR_VALUE(PhysicalAvoidRate);
#ifdef _NA001605_EP2_ATTRIBUTES_RENEWAL_
    // CHANGES: f110209.6L, changes formula by planner request
    __DECLARE_ATTR_VALUE(ArmorPhysicalAvoidRate);
#endif
    // �ӵ�
    __DECLARE_ATTR_VALUE(MoveSpeedRatio); // 100�� ���� %��ġ
    __DECLARE_ATTR_VALUE(AttSpeedRatio);  // 100�� ���� %��ġ

    // ���ʽ� ��Ÿ�(�����Ƽ���� ��ġ�� 10�踦 �ؼ� ����ϹǷ� float�� �ƴ϶� int�� ����Ѵ�.)
    __DECLARE_ATTR_VALUE(AllRangeBonus);
    __DECLARE_ATTR_VALUE(NormalRangeBonus);
    __DECLARE_ATTR_VALUE(SkillRangeBonus);

    // ũ��Ƽ��(ũ��Ƽ�� ������ ���ʽ��� ��ġ�� ������ �����ϴ� ����� �ٸ� �Ӽ�ġ�ʹ� �ٸ��Ƿ� AttrSumPolicy�� ����Ѵ�.)
    __DECLARE_ATTR_VALUE(CriticalRatioChange);  // ���� ũ��Ƽ�� Ȯ��(��ġ)
    __DECLARE_ATTR_VALUE_EP1(MagicCriticalRatio);     // ���� ũ��Ƽ�� Ȯ��(��ġ)
    __DECLARE_ATTR_VALUE(CriticalRatioBonus);     // ũ��Ƽ�� Ȯ�� ���ʽ�
    __DECLARE_ATTR_VALUE(CriticalDamageBonus);    // ũ��Ƽ�� ������ ���ʽ�+�ۼ�Ʈ ������

    // �⺻ ����, ���� ���ݷ�
    __DECLARE_ATTR_VALUE(BaseMeleeMinAttPower);
    __DECLARE_ATTR_VALUE(BaseMeleeMaxAttPower);
    __DECLARE_ATTR_VALUE(BaseRangeMinAttPower);
    __DECLARE_ATTR_VALUE(BaseRangeMaxAttPower);
    __DECLARE_ATTR_VALUE_EP1(BaseMagicMinAttPower);
    __DECLARE_ATTR_VALUE_EP1(BaseMagicMaxAttPower);

    // �ɼ� ����, ���� ���ݷ�(�������� ������ ���� ���� ����� �ϹǷ� AttrSumRatioPolicy�� �̿����� �ʰ� AttrSumPolicy�� ����Ѵ�.)
    __DECLARE_ATTR_VALUE(OptionPhysicalAttPower);
    __DECLARE_ATTR_VALUE_EP1(OptionMagicAttPower);
    __DECLARE_ATTR_VALUE_EP1(OptionAllAttPower);

    // ��� ���� �Ӽ� ���ݷ�
    __DECLARE_ATTR_VALUE(MagicalAllAttPower);

    // �⺻ ����, ���� ����
    __DECLARE_ATTR_VALUE(BaseMeleeDefPower);
    __DECLARE_ATTR_VALUE(BaseRangeDefPower);
    __DECLARE_ATTR_VALUE_EP1(BaseMagicDefPower);

    // �ɼ� ����, ���� ����
    __DECLARE_ATTR_VALUE(OptionPhysicalDefPower);
    __DECLARE_ATTR_VALUE_EP1(OptionMagicDefPower);
    __DECLARE_ATTR_VALUE_EP1(OptionAllDefPower);

    // ��� ���� �Ӽ� ����
    __DECLARE_ATTR_VALUE_EP1(MagicalAllDefPower);

    // �þ�
    __DECLARE_ATTR_VALUE(SightRange);

    // ��ų ���ݷ�, ��ų �ۼ�Ʈ �߰� ������
    __DECLARE_ATTR_VALUE(SkillAttackPower);
    __DECLARE_ATTR_VALUE(SkillPercentDamage);

    // �ο�TAIYO �߰��Ѱ�(������ �ɼ�)
    __DECLARE_ATTR_VALUE(AttackIncRate);
    __DECLARE_ATTR_VALUE(DefenseIncRate);
    __DECLARE_ATTR_VALUE(SkillLevelAll);

    __DECLARE_ATTR_VALUE(DecreaseLimitStat);
    __DECLARE_ATTR_VALUE(MPSpendIncrease);

    // ���ȭ ������ �ɼ� �߰�
    __DECLARE_ATTR_VALUE(AbsorbHP);               // HP ���
    __DECLARE_ATTR_VALUE(AbsorbMP);               // MP ���
    __DECLARE_ATTR_VALUE(ReflectDamageRatio);     // ������ �ݻ�
    __DECLARE_ATTR_VALUE(BonusMoneyRatio);        // ���� ����
    __DECLARE_ATTR_VALUE(BonusExpRatio);          // ����ġ ����
    __DECLARE_ATTR_VALUE(AreaAttackRatio);        // ���߰��� Ȯ��
    __DECLARE_ATTR_VALUE(BonusCastingTime);       // ĳ���� Ÿ�� ����
    __DECLARE_ATTR_VALUE(BonusSkillCoolTime);     // ��ų ��Ÿ�� ����(65)
    __DECLARE_ATTR_VALUE(DecDamage);              // ������ ����

    __DECLARE_ATTR_VALUE(ResurrectionRatio);      //52 ����� ������ ��Ȱ �� Ȯ��
    __DECLARE_ATTR_VALUE(DoubleDamageRatio);      //53 �������� �ι谡 ����� Ȯ��
    __DECLARE_ATTR_VALUE(LuckMonIncDamage);       //54 ��Ű ���� �߰� ������
    __DECLARE_ATTR_VALUE(CompositeIncRatio);      //55 ���� ������ ����
    __DECLARE_ATTR_VALUE(BypassDeffenceRatio);    //56 ���� ���� Ȯ��
    __DECLARE_ATTR_VALUE(IncreaseMinDamage);      //57 �ּ� ������ ����
    __DECLARE_ATTR_VALUE(IncreaseMaxDamage);      //58 �ִ� ������ ����
    __DECLARE_ATTR_VALUE(DecreaseItemDuraRatio);  //59 ������ ������ ���ҹ��� Ȯ��
    __DECLARE_ATTR_VALUE(ResistBadStatusRatio);   //60 ��ų ȿ�� ���� Ȯ��
    __DECLARE_ATTR_VALUE(IncreaseSkillDuration);  //61 ��ų ���� �ð� ���� (Buff)
    __DECLARE_ATTR_VALUE(DecreaseSkillDuration);  //62 ��ų ���� �ð� ���� (DeBuff)

    // eATTR_TYPE ������ ���ſ� ���� ����������
    __DECLARE_ATTR_VALUE(EhterDamageRatio);       //63 ���׸����� ������ ��ȭ
    __DECLARE_ATTR_VALUE(EhterPvEDamageRatio);    //76 ���׸����� ������ ��ȭ, f110601.4L
    __DECLARE_ATTR_VALUE(IncreaseReserveHPRatio); //64 ����HP ����.

    //{_NA_001231_20081216_ADD_OPTION_KIND
    __DECLARE_ATTR_VALUE(ResistHolding);          //66 Ȧ�� ����.
    __DECLARE_ATTR_VALUE(ResistSleep);            //67 ���� ����.
    __DECLARE_ATTR_VALUE(ResistPoison);           //68 �ߵ� ����.
    __DECLARE_ATTR_VALUE(ResistKnockBack);        //69 �˹� ����.
    __DECLARE_ATTR_VALUE(ResistDown);             //70 �ٿ� ����.
    __DECLARE_ATTR_VALUE(ResistStun);             //71 ���� ����.
    __DECLARE_ATTR_VALUE(DecreasePVPDamage);      //72 PVP������ ����.
    //};
    __DECLARE_ATTR_VALUE(AddDamage);              //74 �߰������� 0902, __NA_S00015_20080828_NEW_HOPE_SHADOW
    __DECLARE_ATTR_VALUE(AutoPickup);             //74 ������ �ڵ�ȹ��, __NA_1241_AUTO_PICKUP_PET_OPTION
    //{__NA_001290_20090525_SHIELD_SYSTEM
    __DECLARE_ATTR_VALUE(MaxSD);                  // 85 �ִ� �ǵ� ����Ʈ
    __DECLARE_ATTR_VALUE(RecoverSD);              // 86 SD ȸ����
    //};
    __DECLARE_ATTR_VALUE(IncreaseEnchantRatio); // 77 ��þƮ ������ ����
#ifdef _NA_006731_20130521_ENCHANT_ADD_OPTION
    __DECLARE_ATTR_VALUE(EnemyCriticalRatioChange); // 96 �ǰ� �� ����� ũ��Ƽ�� Ȯ�� ����
#endif// _NA_006731_20130521_ENCHANT_ADD_OPTION
    __DECLARE_ATTR_VALUE(PCBang);                   // PCBang
#ifdef _NA_006680_20130426_ITEM_OPTION_ADD_AND_MODIFY
    __DECLARE_ATTR_VALUE(AttackDamageAbsorbSDRatio); // 98 ���� ������ SD ��ȯ
    __DECLARE_ATTR_VALUE(AttackDamageAbsorbHPRatio); // 99 ���� ������ HP ��ȯ
#endif // _NA_006680_20130426_ITEM_OPTION_ADD_AND_MODIFY
#ifdef _NA_006937_20131030_ABILITY_AND_STATE_CHANGE_CRITICAL
    __DECLARE_ATTR_VALUE(EnemyCriticalRatioIncrease);
    __DECLARE_ATTR_VALUE(EnemyCriticalDamageChange);

    __DECLARE_ATTR_VALUE(CriticalRatioDecrease);
    __DECLARE_ATTR_VALUE(CriticalDamageDecrease);
#endif //_NA_006937_20131030_ABILITY_AND_STATE_CHANGE_CRITICAL
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    __DECLARE_ATTR_VALUE(CraftCostRatio);                       // 101 ���� ��� ����
    __DECLARE_ATTR_VALUE(CraftPreventExtinctionMaterialRatio);  // 102 ���� ���н� ��� �Ҹ� ���� Ȯ�� ����
    __DECLARE_ATTR_VALUE(EnchantCostRatio);                     // 103 ��æƮ ��� ����
    __DECLARE_ATTR_VALUE(EnchantPreventDestroyNDowngradeItemRatio); // 104 ��æƮ ���н� ������ �Ҹ�, �ٿ� ���� Ȯ�� ����
    __DECLARE_ATTR_VALUE(RecoverPotionCooltimeRatio);           // 105 ȸ�� ���� ��Ÿ�� ����
    __DECLARE_ATTR_VALUE(RecoverPotionRecoveryRatio);           // 106 ȸ�� ���� ȸ���� ����
    __DECLARE_ATTR_VALUE(QuestRewardExpRatio);                  // 107 ����Ʈ ���� ����ġ ����
    __DECLARE_ATTR_VALUE(MaxDamageRatio);                       // 108 �ִ� ������ �߻�Ȯ�� ����
    __DECLARE_ATTR_VALUE(DominationMapobjectDamageRatio);       // 109 ���� ������Ʈ ������ ����
    __DECLARE_ATTR_VALUE(ShopRepairHeimRatio);                  // 110 ���� ���� ���� ����
    __DECLARE_ATTR_VALUE(ShopBuyHeimRatio);                     // 111 ���� ���� ���� ����
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    __DECLARE_ATTR_VALUE(MaxFP);                                // 112 ��ġ���̵� �ִ�FP
    __DECLARE_ATTR_VALUE(RecoverFP);                            // 113 FPȸ���� (���ݽ�)
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    __DECLARE_ATTR_VALUE(IncreaseDamageRatio);                  // 114 ������ ����
#ifdef _NA_008124_20150313_AWAKENING_SYSTEM
    __DECLARE_ATTR_VALUE(AwakeningProbability);                 // 115 ���� Ȯ�� ����
#endif // _NA_008124_20150313_AWAKENING_SYSTEM
#ifdef _NA_008486_20150914_TOTAL_BALANCE
    __DECLARE_ATTR_VALUE(DebuffDuration)                        // 116 �ڽ��� �������� ȿ�� ���ӽð� ��ȭ (�и�������)
#endif //_NA_008486_20150914_TOTAL_BALANCE
#ifdef _NA_008540_20151027_ADD_ITEMOPTION_ELITE4
    __DECLARE_ATTR_VALUE(DecreaseDamageNPC);                    // 117 NPC ������ ����
    __DECLARE_ATTR_VALUE(DecreaseDamageBerserker);              // 118 ����Ŀ�� �ִ� ������ ����
    __DECLARE_ATTR_VALUE(DecreaseDamageDragonKnight);           // 119 �巡�ﳪ��Ʈ�� �ִ� ������ ����
    __DECLARE_ATTR_VALUE(DecreaseDamageValkyrie);               // 120 ��Ű���� �ִ� ������ ����
    __DECLARE_ATTR_VALUE(DecreaseDamageElementalist);           // 121 ������Ż����Ʈ�� �ִ� ������ ����
    __DECLARE_ATTR_VALUE(DecreaseDamageShadow);                 // 122 �����찡 �ִ� ������ ����
    __DECLARE_ATTR_VALUE(DecreaseDamageMystic);                 // 123 �̽�ƽ�� �ִ� ������ ����
    __DECLARE_ATTR_VALUE(DecreaseDamageHellroid);               // 124 ����̵尡 �ִ� ������ ����
    __DECLARE_ATTR_VALUE(DecreaseDamageWitchBlade);             // 125 ��ġ���̵尡 �ִ� ������ ����
#endif //_NA_008540_20151027_ADD_ITEMOPTION_ELITE4
public:
template <typename temptype>
    temptype GetValueAppliedAttr(temptype current_value, eATTR_TYPE attr_type);
#endif //_NA_007330_20140620_GUILD_SYSTEM_EXTENSION
public:
    int GetMagicalAttackPower(eATTACK_TYPE attack_type, eATTR_KIND attr_kind = eATTR_KIND_CALC) const;
#if !defined(_NA001605_EP2_ATTRIBUTES_RENEWAL_)
    int GetMagicalDefense(eATTACK_TYPE attack_type, eATTR_KIND attr_kind = eATTR_KIND_CALC) const;
    int GetBonusDefense(eATTACK_TYPE attack_type) const;
    int GetReduceDefenseRate(eATTACK_TYPE attack_type) const;
#endif
    int GetBonusDamage(eARMOR_TYPE armor_type) const;
    int GetBonusPercentDamage(eARMOR_TYPE armor_type) const;
#ifdef _NA001605_EP2_ATTRIBUTES_RENEWAL_
    int GetReducePhysicalTargetDefenseRatio() const;
    int GetReduceElementTargetDefenseRatio(eATTACK_TYPE attack_type) const;
    int GetReducePhysicalDamageRatio() const;
    int GetReduceElementDamageRatio(eATTACK_TYPE attack_type) const;
    int GetReduceElementDamageRatio(eATTACK_TYPE attack_type, eATTR_KIND attr_kind) const;
#else
    int GetReduceDamage(eATTACK_TYPE attack_type) const;
#endif
    ////#ifdef __NA_001244_20090417_ATTACK_RESIST
    //int GetResistAttack(eCHAR_TYPE char_type, eATTACK_RESIST attack_kind) const;
    //int GetRatioResistAttack(eCHAR_TYPE char_type, eATTACK_RESIST attack_kind) const;
    //#endif
    // (NOTE) �ϱ� ��ũ�δ� �����Ǹ� ������ ����
    int GetRiderSpeedRatio() const      { return m_RiderSpeedRatio.calc_value_.value_; }
    int GetBaseRiderSpeedRatio() const  { return m_RiderSpeedRatio.base_value_.value_; }
    AttrValue& GetAttrValueRiderSpeedRatio() { return m_RiderSpeedRatio; }; //�̰� ��ʰ� ���� ���ϴ�.
    // added by _NA001605_EP2_ATTRIBUTES_RENEWAL_
    void GetDefaultMagicalAttributes(
        Elements* const attack,
        Elements* const rate_of_defense_power_reduction_of_target,
        Elements* const damage_reduction_against_attack) const;
protected :
    AttrValue m_MagicalAttPower[eATTACK_TYPE_MAX];        // �Ӽ��� ���ݷ�

#ifdef _NA001605_EP2_ATTRIBUTES_RENEWAL_
    AttrValue m_ReducePhysicalDamageRatio;
    AttrValue m_ReduceElementDamageRatioAll;
    AttrValue m_ReduceElementDamageRatio[Elements::Counts];
    AttrValue m_ReducePhysicalTargetDefenseRatio;
    AttrValue m_ReduceElementTargetDefenseRatioAll;
    AttrValue m_ReduceElementTargetDefenseRatio[Elements::Counts];
#else //if !defined(_NA001605_EP2_ATTRIBUTES_RENEWAL_)
    AttrValue m_ReduceDefenseRate[eATTACK_TYPE_MAX];      // ���� ����� ���� ������
    AttrValue m_BonusDefense[eATTACK_TYPE_MAX];           // ������ ���� Ÿ�Կ� ���� �߰� ����
    AttrValue m_MagicalDefPower[eATTACK_TYPE_MAX];        // �Ӽ��� ����
    AttrValue m_BonusDamage[eARMOR_TYPE_MAX];             // �Ƹ� Ÿ�Ժ� �߰� ������
    AttrValue m_BonusPercentDamage[eARMOR_TYPE_MAX];      // �Ƹ� Ÿ�Ժ� �߰� ������ �ۼ�Ʈ
    AttrValue m_ReduceDamage[eATTACK_TYPE_MAX];           // �Ӽ��� ����������
#endif

    //#ifdef __NA_001244_20090417_ATTACK_RESIST
    //AttrValue m_ResistAttack[eCHAR_TYPE_MAX][eATTACK_RESIST_MAX];         // Ŭ������ ���� ������ ���� 75~84 (�÷��̾ ���� ���ݽ� �� PvP��)
    // (NOTE) (WAVERIX) Attributs�� �ִ� ������ ������ ���� ���� ���� �ڵ� ����...
    // Attributs�� ������ ����... �ؾ��� ���̵��� ���� �� ������...
    AttrValue m_RiderSpeedRatio;
    //
    // last field
    int8_t last_field_end_[1]; // range = { attr_values_, last_field_end_ }
    static AttrValue attr_value_static_;
    //
//--------------------------------------------------------------------------------------------------
#if defined(WAVERIX_TEST_BLOCK_IN_CLASS)
    WAVERIX_TEST_BLOCK_IN_CLASS(Attributes);
#endif
};

//==================================================================================================

// NOTE: f110217.4L, add interface function type properties not using operator
inline int Attributes::GetAttrValue(eATTR_TYPE attr_type) const
{
    if (attr_type < eATTR_MAX)
    {
        const AttrValue* attr_value = attr_values_[attr_type].attr_value;
        if (attr_value && attr_value != &attr_value_static_) {
            return attr_value->calc_value_.value_;
        }
        assert(attr_value && attr_value != &attr_value_static_);
        return 0;
    }
    assert(attr_type < eATTR_MAX);
    return 0;
}

inline void Attributes::SetAttrValue(eATTR_TYPE attr_type, int value)
{
    if (attr_type < eATTR_MAX)
    {
        AttrValue* attr_value = attr_values_[attr_type].attr_value;
        if (attr_value && attr_value != &attr_value_static_) {
            attr_value->calc_value_.value_ = value;
            return;
        }
        assert(attr_value && attr_value != &attr_value_static_);
        return;
    }
    assert(attr_type < eATTR_MAX);
}

inline int Attributes::GetMagicalAttackPower(eATTACK_TYPE attack_type,
                                             eATTR_KIND attr_kind) const
{
    ASSERT(attack_type < eATTACK_TYPE_MAX);
    const AttrValue& attr_value = m_MagicalAttPower[attack_type];
    if (attr_kind == eATTR_KIND_CALC) {
        return attr_value.calc_value_.value_;
    }
    return attr_value[attr_kind];
}

#if !defined(_NA001605_EP2_ATTRIBUTES_RENEWAL_)
inline int Attributes::GetMagicalDefense(eATTACK_TYPE attack_type, eATTR_KIND attr_kind) const
{
    ASSERT(attack_type < eATTACK_TYPE_MAX);
    const AttrValue& attr_value = m_MagicalDefPower[attack_type];
    if (attr_kind == eATTR_KIND_CALC) {
        return attr_value.calc_value_.value_;
    }
    return attr_value[attr_kind];
}

inline int Attributes::GetBonusDefense(eATTACK_TYPE attack_type) const
{
    ASSERT(attack_type < eATTACK_TYPE_MAX);
    return m_BonusDefense[attack_type].calc_value_.value_;
}

inline int Attributes::GetReduceDefenseRate(eATTACK_TYPE attack_type) const
{
    ASSERT(attack_type < eATTACK_TYPE_MAX);
    return m_ReduceDefenseRate[attack_type].calc_value_.value_;
}
#endif


inline int Attributes::GetBonusDamage(eARMOR_TYPE armor_type) const
{
#ifdef _NA001605_EP2_ATTRIBUTES_RENEWAL_
    __UNUSED(armor_type);
    return 0;
#else
    ASSERT(armor_type < eARMOR_TYPE_MAX);
    return m_BonusDamage[armor_type].calc_value_.value_;
#endif
}

inline int Attributes::GetBonusPercentDamage(eARMOR_TYPE armor_type) const
{
#ifdef _NA001605_EP2_ATTRIBUTES_RENEWAL_
    __UNUSED(armor_type);
    return 0;
#else
    ASSERT(armor_type < eARMOR_TYPE_MAX);
    return m_BonusPercentDamage[armor_type].calc_value_.value_;
#endif
}

#ifdef _NA001605_EP2_ATTRIBUTES_RENEWAL_

inline int Attributes::GetReducePhysicalTargetDefenseRatio() const
{
    return m_ReducePhysicalTargetDefenseRatio.calc_value_.value_;
}

inline int Attributes::GetReduceElementTargetDefenseRatio(eATTACK_TYPE attack_type) const
{
    if (attack_type == eATTR_DEL_MAGICAL_ALL_DAMAGE) {
        return m_ReduceElementTargetDefenseRatioAll.calc_value_.value_;
    }
    const ulong bit_field = (1 << attack_type);
    if ((bit_field & Elements::kAcceptTypesField) == 0) {
        return 0;
    };
    const int index = Elements::kToElementIndex[attack_type];
    assert(_countof(m_ReduceElementTargetDefenseRatio) > index);
    return m_ReduceElementTargetDefenseRatio[index].calc_value_.value_;
}

inline int Attributes::GetReducePhysicalDamageRatio() const
{
    return m_ReducePhysicalDamageRatio.calc_value_.value_;
}

inline int Attributes::GetReduceElementDamageRatio(eATTACK_TYPE attack_type) const
{
    if (attack_type == eATTR_DEL_MAGICAL_ALL_DAMAGE) {
        return m_ReduceElementDamageRatioAll.calc_value_.value_;
    }
    const ulong bit_field = (1 << attack_type);
    if ((bit_field & Elements::kAcceptTypesField) == 0) {
        return 0;
    };
    const int index = Elements::kToElementIndex[attack_type];
    assert(_countof(m_ReduceElementDamageRatio) > index);
    return m_ReduceElementDamageRatio[index].calc_value_.value_;
};

inline int Attributes::GetReduceElementDamageRatio(eATTACK_TYPE attack_type,
                                                   eATTR_KIND attr_kind) const
{
    if (attack_type == eATTR_DEL_MAGICAL_ALL_DAMAGE) {
        return m_ReduceElementDamageRatioAll[attr_kind];
    }
    const ulong bit_field = (1 << attack_type);
    if ((bit_field & Elements::kAcceptTypesField) == 0) {
        return 0;
    };
    const int index = Elements::kToElementIndex[attack_type];
    assert(_countof(m_ReduceElementDamageRatio) > index);
    return m_ReduceElementDamageRatio[index][attr_kind];
};
#else
inline int Attributes::GetReduceDamage(eATTACK_TYPE attack_type) const
{
    ASSERT(attack_type < eATTACK_TYPE_MAX);
    return m_ReduceDamage[attack_type].calc_value_.value_;
}
#endif
////#ifdef __NA_001244_20090417_ATTACK_RESIST
//inline int Attributes::GetResistAttack(eCHAR_TYPE char_type, eATTACK_RESIST attack_kind) const
//{ 
//    ASSERT(char_type < eCHAR_TYPE_MAX && attack_kind < eATTACK_RESIST_MAX);   
//    return m_ResistAttack[char_type][attack_kind].calc_value_.value_;
//}
//
//inline int Attributes::GetRatioResistAttack(eCHAR_TYPE char_type, eATTACK_RESIST attack_kind) const
//{ 
//    ASSERT(char_type < eCHAR_TYPE_MAX && attack_kind < eATTACK_RESIST_MAX);   
//    return m_ResistAttack[char_type][attack_kind][eATTR_KIND_RATIO];
//}
////#endif


#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
template <typename temptype>
temptype Attributes::GetValueAppliedAttr( temptype current_value, eATTR_TYPE attr_type )
{
    double change_value = GetAttrValue(attr_type);

    AttrValue* attr_value = NULL;
    if (attr_type < eATTR_MAX && attr_type > eATTR_TYPE_INVALID)
    {
        attr_value = attr_values_[attr_type].attr_value;
    }
    if (attr_value == NULL)
    {
        return current_value;
    }
    
    double ratio = 100.0f;
    switch(attr_value->bound_type_)
    {
    case eBOUNDTYPE_ZP_100:
    case eBOUNDTYPE_MP_100:
        break;
    case eBOUNDTYPE_ZP_1000:
    case eBOUNDTYPE_MP_1000:
        ratio = 1000.0f;
        break;
    case eBOUNDTYPE_ZP_10000:
    case eBOUNDTYPE_MP_10000:
        ratio = 10000.0f;
        break;
    case eBOUNDTYPE_MP_INFINITE:
    case eBOUNDTYPE_ZP_INFINITE:
        ratio = 1.0f;
        break;
    default:
        return current_value;
        break;
    }
    temptype final_value = current_value;
    if (change_value != 0.0f)
    {
        if (attr_value->update_type_ == eUPDATETYPE_SUM_RATIO)
        {
            final_value = final_value * (1.0f + change_value / ratio);
        }
        if (attr_value->update_type_ == eUPDATETYPE_NOT_RATIO)
        {
            final_value = final_value + (change_value / ratio);
        }
    }
    return final_value;
}
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION

#pragma pack(pop)

#endif //PROGRAMCOMMON_ATTRIBUTE_H