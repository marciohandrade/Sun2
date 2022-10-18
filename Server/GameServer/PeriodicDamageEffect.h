#pragma once

#include "BaseEffect.h"


class PeriodicDamageEffect : public BaseEffect
{
public:
	PeriodicDamageEffect();
	~PeriodicDamageEffect() {}

	virtual VOID		Execute();

	virtual VOID		SetDamage( eATTACK_TYPE eType, DAMAGETYPE wDamage ) { m_eDamageType = eType; m_wDamage = wDamage; }
    void SetAbilityInfo(const BASE_ABILITYINFO* ability_info);
    BOOL UseSkill(SLOTCODE skill_code);
    bool ExecuteAurora();

private:
	eATTACK_TYPE		m_eDamageType;
	DAMAGETYPE			m_wDamage;
    const BASE_ABILITYINFO* ability_info_;
};


inline PeriodicDamageEffect::PeriodicDamageEffect()
    : m_eDamageType(eATTACK_TYPE_INVALID)
    , m_wDamage(0)
    , ability_info_(NULL)
{}

inline void PeriodicDamageEffect::SetAbilityInfo(const BASE_ABILITYINFO* ability_info)
{
    ability_info_ = ability_info;
}

