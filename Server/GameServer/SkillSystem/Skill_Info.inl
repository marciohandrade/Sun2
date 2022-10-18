#pragma once
#ifndef SKILL_SYSTEM_VERSION_ROUTING_INL
    #error "can't direct include this file"
#endif

//==================================================================================================
//  (Inline fuction implementation)

inline SkillInfo::SkillInfo()
    : character_(NULL)
{
    ZeroMemory(field_start_, static_cast<size_t>(field_end_ + sizeof(field_end_) - field_start_));
#ifdef _DEBUG
    field_start_[0] = 0x0A;
#endif
}

inline SkillInfo::SkillInfo(const SkillInfo& rhs)
    : character_(rhs.character_)
{
    if (this == &rhs) {
        return;
    }
    CopyMemory(field_start_, rhs.field_start_,
               static_cast<size_t>(field_end_ + sizeof(field_end_) - field_start_));
}

inline SkillInfo& SkillInfo::operator=(const SkillInfo& rhs)
{
    if (this == &rhs) {
        return (*this);
    }
    CopyMemory(this, &rhs, sizeof(*this));
    return (*this);
}

inline void SkillOption::Clear()
{
    ZeroMemory(this, sizeof(*this));
}

inline void SkillOption::ApplyOption(const SkillOption& rhs)
{
    option_ = rhs.option_;
}

inline void SkillOption::AddOption(SkillOption::eOption option)
{
    option_ |= option;
}

//--------------------------------------------------------------------------------------------------

inline DamageArgs::DamageArgs(Character* actor, eATTACK_TYPE attack_type)
    : actor_(actor), attack_type_(attack_type)
    , damage_first_(0)
#ifdef _NA_006728_20130520_SKILL_BALANCE_MODIFY
    ,skill_code_(0)
#endif // _NA_006728_20130520_SKILL_BALANCE_MODIFY
{
    const size_t copy_size = sizeof(*this) -
                             (static_cast<size_t>(reinterpret_cast<uint8_t*>(&this->option_) -
                                                  reinterpret_cast<uint8_t*>(this)));
    CopyMemory(&this->option_, &kDefaultDamageArgs.option_, copy_size);
}

#ifdef _NA_006728_20130520_SKILL_BALANCE_MODIFY
inline DamageArgs::DamageArgs(Character* actor, eATTACK_TYPE attack_type, SLOTCODE skill_code)
    : actor_(actor), attack_type_(attack_type)
#ifdef _NA_006728_20130520_SKILL_BALANCE_MODIFY
    , skill_code_(skill_code)
#endif //_NA_006728_20130520_SKILL_BALANCE_MODIFY
    , damage_first_(0)
{
    const size_t copy_size = sizeof(*this) -
        (static_cast<size_t>(reinterpret_cast<uint8_t*>(&this->option_) -
        reinterpret_cast<uint8_t*>(this)));
    CopyMemory(&this->option_, &kDefaultDamageArgs.option_, copy_size);
}
#endif // _NA_006728_20130520_SKILL_BALANCE_MODIFY

inline DamageArgs::DamageArgs(const DamageArgs& rhs)
    : actor_(rhs.actor_), attack_type_(rhs.attack_type_)
    , damage_first_(0)
{
    if (this == &rhs) {
        return;
    }
    CopyMemory(this, &rhs, sizeof(*this));
}

inline DamageArgs& DamageArgs::operator=(const DamageArgs& rhs)
{
    if (this == &rhs) {
        return (*this);
    }
    CopyMemory(this, &rhs, sizeof(*this));
    return (*this);
}

//--------------------------------------------------------------------------------------------------

inline void BONUS_ABILITY_EFFECT::Init()
{
    ZeroMemory(this, sizeof(*this));
}

//==================================================================================================
