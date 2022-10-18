#pragma once

#ifndef GAMESERVER_ABILITY_H_
#define GAMESERVER_ABILITY_H_

enum eABILITY_TYPE
{
    eABILITY_TYPE_ACTIVE,
    eABILITY_TYPE_PASSIVE,
    eABILITY_TYPE_EFFECT,
    eABILITY_TYPE_MANUAL, 
    eABILITY_TYPE_ACTIVE_AND_EFFECT,
    eABILITY_TYPE_MAX
};

class Character;
class Skill;
struct SkillInfo;
struct BONUS_ABILITY_EFFECT;

class Ability
{
public:
    Ability()
    {
        Clear();
    }
    virtual ~Ability()
    {
    }

public:
    virtual void Init(Skill* skill, const BASE_ABILITYINFO* ability_info);
    virtual void InitDetailed(
        Character* attacker,
        SLOTCODE skill_code, 
        SkillInfo* skill_info,
        BYTE skill_stat_type, 
        const BASE_ABILITYINFO* ability_info
    );
    virtual void Release(Character* /*target*/) 
    {
    }
    virtual eABILITY_TYPE GetAbilityType() const
    {
        return eABILITY_TYPE_ACTIVE;
    }
    virtual bool IsValidStateID() const;
    virtual bool CanExecute(
        Character* attacker,
        Character* target, 
        DWORD main_target_key, 
        WzVector main_target_pos
    );
    virtual bool Execute(Character* target, BYTE* msg, WORD& msg_size);
    virtual bool ExecuteEffect(BYTE* msg, WORD& msg_size)
    {
        __UNUSED((msg, msg_size));
        return false;
    }
    virtual void SetBonusEffect(BONUS_ABILITY_EFFECT* bonus_ability_effect)
    {
        __UNUSED(bonus_ability_effect);
    }
    
public:
    void Clear()
    {
        base_ability_info_ = NULL;
        attacker_ = NULL;
        skill_ = NULL;
        skill_code_ = 0;
        event_code_ = 0;
    }
    SLOTCODE GetSkillCode() const
    {
        return skill_code_;
    }
    const BASE_ABILITYINFO* GetBaseAbilityInfo() const
    {
        return base_ability_info_;
    }
    eABILITY GetAbilityID() const
    {
        return (eABILITY)base_ability_info_->m_wAbilityID;
    }
    eATTR_TYPE GetAttrID() const
    {
        return base_ability_info_->GetAttrType();
    }
    eCHAR_STATE_TYPE GetStateID() const
    {
        return (eCHAR_STATE_TYPE)base_ability_info_->m_wStateID;
    }
    BYTE GetIndex() const
    {
        return base_ability_info_->m_byIndex;
    }
    Skill* GetSKill() const
    {
        return skill_;
    }
    void SetEventCode(const BYTE event_code)
    {
        event_code_ = event_code;
    }
    BYTE GetEventCode() const
    {
        return event_code_;
    }

protected:
    bool CheckAbilityRange(
        Character* const attacker, 
        Character* const target,
        const DWORD main_target_key, 
        const BYTE ability_range_type
    ) const;
    Character* GetAttacker() const
    {
        if (attacker_ != NULL && attacker_->IsDeleted())
        {
            return NULL;
        }
        return attacker_;
    }

private:
    SLOTCODE skill_code_;
    BYTE event_code_;
    const BASE_ABILITYINFO* base_ability_info_;
    Character* attacker_;
    Skill* skill_;
};

#endif // GAMESERVER_ABILITY_H_