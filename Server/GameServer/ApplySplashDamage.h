#pragma once
#ifndef GAMESERVER_APPLY_SPLASH_DAMAGE_H
#define GAMESERVER_APPLY_SPLASH_DAMAGE_H

namespace nsSkill {
;

// CHANGES: f110217.5L, changes periodic damage formula
struct BaseAttackPowerApplyOption
{
    bool use_base_attack_apply;
    int base_attack_application_ratio; // BASE_ABILITYINFO::m_iOption2 <- PeriodicDamage
};

extern void ApplySplashDamage(GameField* game_field, Character* attacker,
                              SLOTCODE skill_code, eATTACK_TYPE attack_type, 
                              DAMAGETYPE damage, int max_number_of_target_select,
                              DWORD sector_index, WzVector& vDestPos,
                              float fRadius, eSD_APPLY SDApply,
                              const BaseAttackPowerApplyOption* apply_option);

}; //end of namespace

#endif //GAMESERVER_APPLY_SPLASH_DAMAGE_H