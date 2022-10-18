#ifndef GAMESERVER_ABILITY_STATUS_H_
#define GAMESERVER_ABILITY_STATUS_H_

#include "BaseStatus.h"


class Ability;

class AbilityStatus : public BaseStatus
{
public:
    AbilityStatus();
    ~AbilityStatus();

public:
    virtual void Init(Character* owner, Character* attacker, Ability* ability);
    virtual void Start();
    virtual void Execute();
    virtual void End();
    virtual BOOL Update(DWORD curTime);
    virtual bool IsAbilityStatus() const
    { 
        return true; 
    }
    virtual void AttackAbsorb(DAMAGETYPE damage) 
    { 
        __UNUSED(damage); 
    }
    virtual void AttackedAbsorb(eATTACK_TYPE attack_type, DAMAGETYPE damage) 
    { 
        __UNUSED(attack_type); __UNUSED(damage); 
    }
    virtual void DamageMirror(Character* attacker, DAMAGETYPE damage) 
    { 
        __UNUSED(attacker); __UNUSED(damage); 
    }

public:
    void Init(
        Character* const owner, 
        const WORD state_id,
        const BASE_ABILITYINFO* base_ability_info, 
        const SLOTCODE skill_code,
        const int application_time, 
        const int period
    );
    SLOTCODE GetSkillCode() const
    {
        return skill_code_;
    }
    eABILITY GetAbilityID() const
    {
        return ability_id_;
    }
    BYTE GetIndex() const
    {
        return ability_index_;
    }
    eATTR_TYPE GetAttrID() const
    {
        return attr_id_;
    }
    float GetTotemRadius() const
    {
        return totem_radius_;
    }

    bool ComparePriority(const Ability* const ability) const;
    
    // (CHANGES) (f100309.1L) changes interface specification regard with relation.
    // if result == 0 then no is_apply
    // if result <  0 then target be applied bad status, calculate decrease skill duration option
    // if result >  0 then target be applied good status, calculate increase skill duration option
    // '버프 효과 시간 +%d초' 옵션 처리
    int GetDurationControlWithRelation(
        const Character* const owner, 
        Character* attacker,
        const eCHAR_STATE_TYPE char_state,
        const eABILITY ability_id,
        const int application_time
    ) const;
    
    bool IsReflectStatus() const
    {
        switch(GetStateID())
        {
        case eCHAR_STATE_REFLECT_DAMAGE:
        case eCHAR_STATE_REFLECT_SLOW:
        case eCHAR_STATE_REFLECT_FROZEN:
        case eCHAR_STATE_REFLECT_SLOWDOWN:
        case eCHAR_STATE_REFLECT_STUN:
        case eCHAR_STATE_REFLECT_FEAR:
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        case eCHAR_STATE_REFLECT_LINK_SKILL:
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
            return true;
        }
        return false;
    }
#ifdef _NA_007667_20141001_WITCHBLADE_SKILL
    bool IsAutoCastStatus() const
    {
        switch(GetStateID())
        {
        case eCHAR_STATE_AUTOCAST_BLEEDING_ATTACK:
            return true;
        }
        return false;
    }

    virtual void UseSkillByAttack(Character* target) { __UNUSED(target); }
#endif //_NA_007667_20141001_WITCHBLADE_SKILL
    
    const BASE_ABILITYINFO* GetAbilityInfo() const
    { 
        return ability_info_; 
    }
    BYTE GetEventCode() const 
    { 
        return ability_event_code_; 
    }
    void SetEventCode(const BYTE event_code) 
    { 
        ability_event_code_ = event_code; 
    }
    void RegisterSkillOption(const SkillOption& skill_option)
    {
        skill_option_.Clear();
        skill_option_.ApplyOption(skill_option);
    }
    const SkillOption& GetSkillOption() const 
    { 
        return skill_option_; 
    }
    
protected:
    void ChangeHP(
        const bool is_increse, const int value_type, const int in_value, const int hp_limit);
    void ChangeMP(const bool is_increase, const int value_type, const int in_value);
    void ChangeSD(const bool is_increase, const int value_type, const int in_value);

    Character* GetAttacker() const
    {
        if (attacker_ != NULL && attacker_->IsDeleted())
        {
            return NULL;
        }
        return attacker_;
    }
    bool IsApply() const
    {
        return is_apply_;
    }
    void SetApply(const bool is_apply)
    {
        is_apply_ = is_apply;
    }
    int GetAbilityValueType() const
    {
        return ability_value_type_;
    }
    void SetAbilityValueType(const int value_type)
    {
        ability_value_type_ = value_type;
    }
    int GetAbilityValue() const
    {
        return ability_value_;
    }
    int GetSumValue() const
    {
        return sum_value_;
    }
    
protected:
    int m_iOption1;
    int m_iOption2;
    
private:
    bool is_apply_;    
    int ability_value_type_;   // 수치타입
    int ability_value_;       // 수치
    int sum_value_;    // 누적 수치

    SLOTCODE skill_code_;
    eABILITY ability_id_;
    BYTE ability_index_;
    eATTR_TYPE attr_id_;
    float totem_radius_; // 토템 사거리
    BYTE ability_event_code_;  // reserved delete
        
    SkillOption skill_option_; // __NA001374_090708_CACHE_ITEM_BUFF_EXTENTION__
    Character* attacker_;
    const BASE_ABILITYINFO* ability_info_;
};

#endif // GAMESERVER_ABILITY_STATUS_H_