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
// 순서 바꾸면 안된다.
enum eATTR_KIND
{
    eATTR_KIND_CALC,        // 최종값
    eATTR_KIND_BASE,        // 기본값
    eATTR_KIND_ITEM,        // 아이템으로 변화된 값
    eATTR_KIND_SKILL,       // 스킬로 변화된 값
    eATTR_KIND_ITEM_RATIO,  // 아이템에 대한 비율값
    eATTR_KIND_SKILL_RATIO, // 스킬에 대한 비율값
    eATTR_KIND_RATIO,       // 전체에 대한 비율값
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
// INTValue : int 변수를 가지며 각종 연산에 대한 BoundCheckPolicy를 적용한다.
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
//  AttrSumPolicy : 각 값들을 더해서 CALC에 설정한다. 
//
// eUPDATETYPE_NOTHING,    AttrValue::Update()에서 계산하지 않는다. 따로 계산하는 공식이 있어야함.
// eUPDATETYPE_NOT_RATIO,  AttrValue::Update()에서 수치값만 총합을 계산한다.
// eUPDATETYPE_SUM_RATIO,  AttrValue::Update()에서 수치값 총합을 계산한 후 비율값을 곱해 준다.
enum eUPDATETYPE
{
    eUPDATETYPE_NOTHING,
    eUPDATETYPE_NOT_RATIO,
    eUPDATETYPE_SUM_RATIO,
    eUPDATETYPE_MAX,
};

//==================================================================================================
// AttrValue : INTValue를 가지고 eATTR_KIND에 따라서 그 값을 넘겨준다.
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
    INTValue calc_value_;    // 최종값 calc_value_ = (base_value_+item_value_+skill_value_)*m_iRatio/100
    INTValue base_value_;    // 기본값
    INTValue item_value_;    // 아이템으로 변화된 값
    INTValue skill_value_;   // 스킬로 변화된 값

    // 부동소수점 연산의 부정확성 때문에 100을 곱해서 사용한다!
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
// Attributes : 캐릭터의 속성값들을 가진다.
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
    int GetAttrMaxBoundValue(eATTR_TYPE attr_type) const; // calc_value 범위의 MAX 값을 리턴
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
    AttrNode attr_values_[eATTR_MAX];    // 각 attr들을 배열로 접근할 수 있게 한다.

    __DECLARE_ATTR_VALUE(STR);        // 힘
    __DECLARE_ATTR_VALUE(DEX);        // 민첩 
    __DECLARE_ATTR_VALUE(VIT);        // 체력 
    __DECLARE_ATTR_VALUE(SPR);        // 정신력
    __DECLARE_ATTR_VALUE(INT);        // 지력 

    // 숙련도
    __DECLARE_ATTR_VALUE_EP1(Experty1);
    __DECLARE_ATTR_VALUE_EP1(Experty2);

    // HP, MP
    __DECLARE_ATTR_VALUE(MaxHP);
    __DECLARE_ATTR_VALUE(MaxMP);
    __DECLARE_ATTR_VALUE(CurHP);      // 현재 체력(사용되지않음, 자주 사용되는 파라미터이기 때문에 Update에 대한 부하를 줄이고자 Player에서 직접 관리함)
    __DECLARE_ATTR_VALUE(CurMP);      // 현재 mana(사용되지않음)
    __DECLARE_ATTR_VALUE(RecoverHP);  // HP 회복량
    __DECLARE_ATTR_VALUE(RecoverMP);  // MP 회복량

    // 공격 성공률, 회피율
    __DECLARE_ATTR_VALUE(PhysicalAttackRate);
    __DECLARE_ATTR_VALUE(PhysicalAvoidRate);
#ifdef _NA001605_EP2_ATTRIBUTES_RENEWAL_
    // CHANGES: f110209.6L, changes formula by planner request
    __DECLARE_ATTR_VALUE(ArmorPhysicalAvoidRate);
#endif
    // 속도
    __DECLARE_ATTR_VALUE(MoveSpeedRatio); // 100을 곱한 %수치
    __DECLARE_ATTR_VALUE(AttSpeedRatio);  // 100을 곱한 %수치

    // 보너스 사거리(어빌러티에서 수치의 10배를 해서 사용하므로 float가 아니라 int로 사용한다.)
    __DECLARE_ATTR_VALUE(AllRangeBonus);
    __DECLARE_ATTR_VALUE(NormalRangeBonus);
    __DECLARE_ATTR_VALUE(SkillRangeBonus);

    // 크리티컬(크리티컬 데미지 보너스의 수치와 비율을 적용하는 방식이 다른 속성치와는 다르므로 AttrSumPolicy을 사용한다.)
    __DECLARE_ATTR_VALUE(CriticalRatioChange);  // 물리 크리티컬 확률(수치)
    __DECLARE_ATTR_VALUE_EP1(MagicCriticalRatio);     // 마법 크리티컬 확률(수치)
    __DECLARE_ATTR_VALUE(CriticalRatioBonus);     // 크리티컬 확률 보너스
    __DECLARE_ATTR_VALUE(CriticalDamageBonus);    // 크리티컬 데미지 보너스+퍼센트 데미지

    // 기본 물리, 마법 공격력
    __DECLARE_ATTR_VALUE(BaseMeleeMinAttPower);
    __DECLARE_ATTR_VALUE(BaseMeleeMaxAttPower);
    __DECLARE_ATTR_VALUE(BaseRangeMinAttPower);
    __DECLARE_ATTR_VALUE(BaseRangeMaxAttPower);
    __DECLARE_ATTR_VALUE_EP1(BaseMagicMinAttPower);
    __DECLARE_ATTR_VALUE_EP1(BaseMagicMaxAttPower);

    // 옵션 물리, 마법 공격력(비율값을 적용할 때는 따로 계산을 하므로 AttrSumRatioPolicy을 이용하지 않고 AttrSumPolicy을 사용한다.)
    __DECLARE_ATTR_VALUE(OptionPhysicalAttPower);
    __DECLARE_ATTR_VALUE_EP1(OptionMagicAttPower);
    __DECLARE_ATTR_VALUE_EP1(OptionAllAttPower);

    // 모든 마법 속성 공격력
    __DECLARE_ATTR_VALUE(MagicalAllAttPower);

    // 기본 물리, 마법 방어력
    __DECLARE_ATTR_VALUE(BaseMeleeDefPower);
    __DECLARE_ATTR_VALUE(BaseRangeDefPower);
    __DECLARE_ATTR_VALUE_EP1(BaseMagicDefPower);

    // 옵션 물리, 마법 방어력
    __DECLARE_ATTR_VALUE(OptionPhysicalDefPower);
    __DECLARE_ATTR_VALUE_EP1(OptionMagicDefPower);
    __DECLARE_ATTR_VALUE_EP1(OptionAllDefPower);

    // 모든 마법 속성 방어력
    __DECLARE_ATTR_VALUE_EP1(MagicalAllDefPower);

    // 시야
    __DECLARE_ATTR_VALUE(SightRange);

    // 스킬 공격력, 스킬 퍼센트 추가 데미지
    __DECLARE_ATTR_VALUE(SkillAttackPower);
    __DECLARE_ATTR_VALUE(SkillPercentDamage);

    // 민욱TAIYO 추가한것(아이템 옵션)
    __DECLARE_ATTR_VALUE(AttackIncRate);
    __DECLARE_ATTR_VALUE(DefenseIncRate);
    __DECLARE_ATTR_VALUE(SkillLevelAll);

    __DECLARE_ATTR_VALUE(DecreaseLimitStat);
    __DECLARE_ATTR_VALUE(MPSpendIncrease);

    // 상용화 아이템 옵션 추가
    __DECLARE_ATTR_VALUE(AbsorbHP);               // HP 흡수
    __DECLARE_ATTR_VALUE(AbsorbMP);               // MP 흡수
    __DECLARE_ATTR_VALUE(ReflectDamageRatio);     // 데미지 반사
    __DECLARE_ATTR_VALUE(BonusMoneyRatio);        // 하임 증가
    __DECLARE_ATTR_VALUE(BonusExpRatio);          // 경험치 증가
    __DECLARE_ATTR_VALUE(AreaAttackRatio);        // 다중공격 확률
    __DECLARE_ATTR_VALUE(BonusCastingTime);       // 캐스팅 타임 증감
    __DECLARE_ATTR_VALUE(BonusSkillCoolTime);     // 스킬 쿨타임 증감(65)
    __DECLARE_ATTR_VALUE(DecDamage);              // 데미지 감소

    __DECLARE_ATTR_VALUE(ResurrectionRatio);      //52 사망시 스스로 부활 할 확률
    __DECLARE_ATTR_VALUE(DoubleDamageRatio);      //53 데미지의 두배가 적용될 확률
    __DECLARE_ATTR_VALUE(LuckMonIncDamage);       //54 럭키 몬스터 추가 데미지
    __DECLARE_ATTR_VALUE(CompositeIncRatio);      //55 조합 성공률 증가
    __DECLARE_ATTR_VALUE(BypassDeffenceRatio);    //56 방어력 무시 확률
    __DECLARE_ATTR_VALUE(IncreaseMinDamage);      //57 최소 데미지 증가
    __DECLARE_ATTR_VALUE(IncreaseMaxDamage);      //58 최대 데미지 증가
    __DECLARE_ATTR_VALUE(DecreaseItemDuraRatio);  //59 아이템 내구력 감소무시 확률
    __DECLARE_ATTR_VALUE(ResistBadStatusRatio);   //60 스킬 효과 무시 확률
    __DECLARE_ATTR_VALUE(IncreaseSkillDuration);  //61 스킬 지속 시간 증가 (Buff)
    __DECLARE_ATTR_VALUE(DecreaseSkillDuration);  //62 스킬 지속 시간 감소 (DeBuff)

    // eATTR_TYPE 디파인 제거에 따른 디파인제거
    __DECLARE_ATTR_VALUE(EhterDamageRatio);       //63 에테르웨폰 데미지 변화
    __DECLARE_ATTR_VALUE(EhterPvEDamageRatio);    //76 에테르웨폰 데미지 변화, f110601.4L
    __DECLARE_ATTR_VALUE(IncreaseReserveHPRatio); //64 적립HP 증가.

    //{_NA_001231_20081216_ADD_OPTION_KIND
    __DECLARE_ATTR_VALUE(ResistHolding);          //66 홀딩 내성.
    __DECLARE_ATTR_VALUE(ResistSleep);            //67 슬립 내성.
    __DECLARE_ATTR_VALUE(ResistPoison);           //68 중독 내성.
    __DECLARE_ATTR_VALUE(ResistKnockBack);        //69 넉백 내성.
    __DECLARE_ATTR_VALUE(ResistDown);             //70 다운 내성.
    __DECLARE_ATTR_VALUE(ResistStun);             //71 스턴 내성.
    __DECLARE_ATTR_VALUE(DecreasePVPDamage);      //72 PVP데미지 감소.
    //};
    __DECLARE_ATTR_VALUE(AddDamage);              //74 추가데미지 0902, __NA_S00015_20080828_NEW_HOPE_SHADOW
    __DECLARE_ATTR_VALUE(AutoPickup);             //74 아이템 자동획득, __NA_1241_AUTO_PICKUP_PET_OPTION
    //{__NA_001290_20090525_SHIELD_SYSTEM
    __DECLARE_ATTR_VALUE(MaxSD);                  // 85 최대 실드 포인트
    __DECLARE_ATTR_VALUE(RecoverSD);              // 86 SD 회복량
    //};
    __DECLARE_ATTR_VALUE(IncreaseEnchantRatio); // 77 인첸트 성공률 증가
#ifdef _NA_006731_20130521_ENCHANT_ADD_OPTION
    __DECLARE_ATTR_VALUE(EnemyCriticalRatioChange); // 96 피격 시 상대의 크리티컬 확률 감소
#endif// _NA_006731_20130521_ENCHANT_ADD_OPTION
    __DECLARE_ATTR_VALUE(PCBang);                   // PCBang
#ifdef _NA_006680_20130426_ITEM_OPTION_ADD_AND_MODIFY
    __DECLARE_ATTR_VALUE(AttackDamageAbsorbSDRatio); // 98 가해 데미지 SD 전환
    __DECLARE_ATTR_VALUE(AttackDamageAbsorbHPRatio); // 99 가해 데미지 HP 전환
#endif // _NA_006680_20130426_ITEM_OPTION_ADD_AND_MODIFY
#ifdef _NA_006937_20131030_ABILITY_AND_STATE_CHANGE_CRITICAL
    __DECLARE_ATTR_VALUE(EnemyCriticalRatioIncrease);
    __DECLARE_ATTR_VALUE(EnemyCriticalDamageChange);

    __DECLARE_ATTR_VALUE(CriticalRatioDecrease);
    __DECLARE_ATTR_VALUE(CriticalDamageDecrease);
#endif //_NA_006937_20131030_ABILITY_AND_STATE_CHANGE_CRITICAL
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    __DECLARE_ATTR_VALUE(CraftCostRatio);                       // 101 제작 비용 증감
    __DECLARE_ATTR_VALUE(CraftPreventExtinctionMaterialRatio);  // 102 제작 실패시 재료 소멸 방지 확률 증감
    __DECLARE_ATTR_VALUE(EnchantCostRatio);                     // 103 인챈트 비용 증감
    __DECLARE_ATTR_VALUE(EnchantPreventDestroyNDowngradeItemRatio); // 104 인챈트 실패시 아이템 소멸, 다운 방지 확률 증감
    __DECLARE_ATTR_VALUE(RecoverPotionCooltimeRatio);           // 105 회복 포션 쿨타임 증감
    __DECLARE_ATTR_VALUE(RecoverPotionRecoveryRatio);           // 106 회복 포션 회복량 증감
    __DECLARE_ATTR_VALUE(QuestRewardExpRatio);                  // 107 퀘스트 보상 경험치 증감
    __DECLARE_ATTR_VALUE(MaxDamageRatio);                       // 108 최대 데미지 발생확률 증감
    __DECLARE_ATTR_VALUE(DominationMapobjectDamageRatio);       // 109 공성 오브젝트 데미지 증감
    __DECLARE_ATTR_VALUE(ShopRepairHeimRatio);                  // 110 상점 수리 하임 증감
    __DECLARE_ATTR_VALUE(ShopBuyHeimRatio);                     // 111 상점 구매 하임 증감
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    __DECLARE_ATTR_VALUE(MaxFP);                                // 112 위치블레이드 최대FP
    __DECLARE_ATTR_VALUE(RecoverFP);                            // 113 FP회복량 (공격시)
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    __DECLARE_ATTR_VALUE(IncreaseDamageRatio);                  // 114 데미지 증가
#ifdef _NA_008124_20150313_AWAKENING_SYSTEM
    __DECLARE_ATTR_VALUE(AwakeningProbability);                 // 115 각성 확률 증감
#endif // _NA_008124_20150313_AWAKENING_SYSTEM
#ifdef _NA_008486_20150914_TOTAL_BALANCE
    __DECLARE_ATTR_VALUE(DebuffDuration)                        // 116 자신의 디버프기술 효과 지속시간 강화 (밀리세컨드)
#endif //_NA_008486_20150914_TOTAL_BALANCE
#ifdef _NA_008540_20151027_ADD_ITEMOPTION_ELITE4
    __DECLARE_ATTR_VALUE(DecreaseDamageNPC);                    // 117 NPC 데미지 감소
    __DECLARE_ATTR_VALUE(DecreaseDamageBerserker);              // 118 버서커가 주는 데미지 감소
    __DECLARE_ATTR_VALUE(DecreaseDamageDragonKnight);           // 119 드래곤나이트가 주는 데미지 감소
    __DECLARE_ATTR_VALUE(DecreaseDamageValkyrie);               // 120 발키리가 주는 데미지 감소
    __DECLARE_ATTR_VALUE(DecreaseDamageElementalist);           // 121 엘리멘탈리스트가 주는 데미지 감소
    __DECLARE_ATTR_VALUE(DecreaseDamageShadow);                 // 122 섀도우가 주는 데미지 감소
    __DECLARE_ATTR_VALUE(DecreaseDamageMystic);                 // 123 미스틱이 주는 데미지 감소
    __DECLARE_ATTR_VALUE(DecreaseDamageHellroid);               // 124 헬로이드가 주는 데미지 감소
    __DECLARE_ATTR_VALUE(DecreaseDamageWitchBlade);             // 125 위치블레이드가 주는 데미지 감소
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
    // (NOTE) 하기 매크로는 검증되면 제거할 예정
    int GetRiderSpeedRatio() const      { return m_RiderSpeedRatio.calc_value_.value_; }
    int GetBaseRiderSpeedRatio() const  { return m_RiderSpeedRatio.base_value_.value_; }
    AttrValue& GetAttrValueRiderSpeedRatio() { return m_RiderSpeedRatio; }; //이건 사례가 좋지 못하다.
    // added by _NA001605_EP2_ATTRIBUTES_RENEWAL_
    void GetDefaultMagicalAttributes(
        Elements* const attack,
        Elements* const rate_of_defense_power_reduction_of_target,
        Elements* const damage_reduction_against_attack) const;
protected :
    AttrValue m_MagicalAttPower[eATTACK_TYPE_MAX];        // 속성별 공격력

#ifdef _NA001605_EP2_ATTRIBUTES_RENEWAL_
    AttrValue m_ReducePhysicalDamageRatio;
    AttrValue m_ReduceElementDamageRatioAll;
    AttrValue m_ReduceElementDamageRatio[Elements::Counts];
    AttrValue m_ReducePhysicalTargetDefenseRatio;
    AttrValue m_ReduceElementTargetDefenseRatioAll;
    AttrValue m_ReduceElementTargetDefenseRatio[Elements::Counts];
#else //if !defined(_NA001605_EP2_ATTRIBUTES_RENEWAL_)
    AttrValue m_ReduceDefenseRate[eATTACK_TYPE_MAX];      // 공격 대상의 방어력 감소율
    AttrValue m_BonusDefense[eATTACK_TYPE_MAX];           // 상대방의 공격 타입에 대한 추가 방어력
    AttrValue m_MagicalDefPower[eATTACK_TYPE_MAX];        // 속성별 방어력
    AttrValue m_BonusDamage[eARMOR_TYPE_MAX];             // 아머 타입별 추가 데미지
    AttrValue m_BonusPercentDamage[eARMOR_TYPE_MAX];      // 아머 타입별 추가 데미지 퍼센트
    AttrValue m_ReduceDamage[eATTACK_TYPE_MAX];           // 속성별 데미지감소
#endif

    //#ifdef __NA_001244_20090417_ATTACK_RESIST
    //AttrValue m_ResistAttack[eCHAR_TYPE_MAX][eATTACK_RESIST_MAX];         // 클래스별 공격 데미지 증감 75~84 (플레이어가 몬스터 공격시 와 PvP시)
    // (NOTE) (WAVERIX) Attributs에 있는 내용을 추출해 보기 위한 실험 코드 성격...
    // Attributs를 정리해 보자... 극악의 난이도를 보일 듯 하지만...
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