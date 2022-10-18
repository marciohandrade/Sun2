#pragma once
#ifndef GAMESERVER_CHARACTER_FORMULA_H
#define GAMESERVER_CHARACTER_FORMULA_H

#include "CommonCharacterFormula.h"


////////////////////////////////////////////////////////////////////////////////////////////////////
// REGION : Ability
////////////////////////////////////////////////////////////////////////////////////////////////////

namespace ns_formula {
namespace ability {

// 치유량 증가값을 계산한다.
int CalcIncreseHeal(
    const IncreseHealAbilityType::Value heal_type, 
    const int heal_value, 
    const Character& skill_user
);

// 저주 증가로 인한 추가값을 계산한다.
int CalcIncreseCurse(
    const eSKILL skill_class_code, 
    const int skill_value, 
    const Character& skill_user
);

// 콤보 사용으로 인한 데미지 추가값을 계산한다.
int CalcComboDamage(
    const Character& skill_user, 
    const Skill& skill, 
    const int skill_value
);

// 콤보 사용으로 인한 추가 상태 지속시간을 계산한다. 
int CalcComboApplyTime(
    const Character& skill_user, 
    const Skill& skill, 
    const int state_id
);

// 스킬 데미지 증가 상태에 의한 데미지 추가값을 계산한다.
int CalcIncreseSkillDamage(
    const Character& skill_user, 
    const Skill& skill, 
    const int skill_value
);

// 투기 사용으로 인한 데미지 추가값을 계산한다.
int CalcFightingEnergy(
    Character& skill_user, 
    const Skill& skill, 
    const int skill_value
);

} // ability
} // ns_formula


////////////////////////////////////////////////////////////////////////////////////////////////////
// REGION : EP1
////////////////////////////////////////////////////////////////////////////////////////////////////

namespace ns_formula { namespace ep1 {
;

extern int CalcDoubleDamage(Character* attacker, int baseDamage);
//
extern EXPTYPE GetKillExpOfParty(BYTE byRoomBonusType, DWORD partymemberNum,
                                 LEVELTYPE partyTotalLV, LEVELTYPE attackerLV, LEVELTYPE targetLV,
                                 float fNPCGradeExpRatio,
                                 DWORD attackDamage,
                                 DWORD targetMaxHP,
                                 eZONETYPE ZoneType,
                                 Player* player);

// HP,MP 회복량(플레이어)
extern int CalcHPRecover(eCHAR_TYPE char_type, DWORD maxHP, BYTE byCharState, LEVELTYPE Level);
extern int CalcSDRecover(eCHAR_TYPE char_type, eCHAR_CONDITION byCharState, BYTE byCharAction,
                         LEVELTYPE level);


extern int CalcMPRecover(eCHAR_TYPE char_type, DWORD spirit, BYTE byCharState);
extern DWORD CalcMPRecoverByAttack(eCHAR_TYPE char_type, DWORD spirit,
                                   DWORD skillRecoverPercent,
                                   DWORD skillRequireMP); // 공격할때 회복량
extern DWORD CalcMPRecoverByAttacked(eCHAR_TYPE char_type,
                                     DWORD spirit,
                                     DAMAGETYPE damage); // 타격 받았을떄 회복량

// HP, MP 회복량(몬스터)
extern int CalcNPCHPRecover(DWORD maxHP, DWORD recoverRatio = 1);
extern int CalcNPCMPRecover(DWORD maxMP, DWORD recoverRatio = 1);
extern int CalcNPCSDRecover(DWORD maxSD);

// 일반 공격(물리)
extern DAMAGETYPE CalcNormalDamage(Character* attacker, Character* target,
                                   eATTACK_TYPE eBaseAttackType,
                                   eATTACK_TYPE eMagicalAttackType, 
                                   WORD criticalRatioBonus = 0,
                                   BYTE* pSkillEffect = NULL,
                                   float fDefenseIgnore = 0);

// 스킬 공격
extern DAMAGETYPE CalcSkillDamage(Character* attacker, Character* target,
                                  eATTACK_TYPE eAttackType,
                                  int iSkillAttackPower, 
                                  float fSkillPercentDamage,
                                  WORD criticalRatioBonus = 0,
                                  BYTE* pSkillEffect = NULL,
                                  BYTE bySkillStatType = 0,
                                  BOOL bCritical = true);

// 상태적용 성공여부
extern bool IsStatusHit(
    Character* const attacker, 
    Character* const target, 
    const int success_ratio, 
    const int state_id = eCHAR_STATE_INVALID, 
    const Skill* const skill = NULL
);

// 내구도 감소비율
extern float GetDuraDecRatio(eZONE_STATE zone_state, eCOMPETITION_MODE_TYPE eCompetitionMode);
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
extern float GetDuraDecRatio(eZONE_STATE zone_state, ChaosZoneType chaos_zone_type);
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND

}}; //end of namespace 'ns_formula::ep1'


////////////////////////////////////////////////////////////////////////////////////////////////////
// REGION : EP2
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _NA001605_EP2_ATTRIBUTES_RENEWAL_

namespace ns_formula { namespace ep2 {
;
// damage formula specification (version:EP2.01)
// tree diagram
//--------------------------------------------------------
//  [Normal]    [Skill]
//     |           |----------------|
//  [Normal]  [Normal Skill]  [Magic Skill]
//--------------------------------------------------------
//  [Normal] - [Player] with default style attack
//           - [Monster] simple attack
//  [Normal Skill] - no elemental option
//  [Magic Skill]  - with elemental option
//

// NOTE: return input damage or zero whether a damage has doubled by roulette.
// @formula
//  damage:int
//  damage += CalcDoubleDamage(attacker, damage);   OK
extern int CalcDoubleDamage(Character* attacker, int damage);

//
// 상태적용 성공여부, _NA_001231_20081216_ADD_OPTION_KIND
extern bool IsStatusHit(
    Character* const attacker,
    Character* const target,
    const int rate_of_success,
    const int state_id = eCHAR_STATE_INVALID, 
    const Skill* const skill = NULL
);

// NOTE: calculate (simulate) physical normal damage
//  rate_of_critical_bonus : increase damage if the result is succeed
extern int CalcNormalDamage(Character* attacker,
                            Character* target,
                            eATTACK_TYPE attack_type,
                            const bool handle_critical_calculation,
                            const uint16_t rate_of_critical_bonus,
                            uint8_t* const result_of_skill_effect_flag);

// NOTE: calculate (simulate) physical | elemental skill damage
//  rate_of_critical_bonus : increase damage if the result is succeed
extern int CalcSkillDamage(Character* attacker,
                           Character* target,
                           eATTACK_TYPE attack_type,
                           const bool handle_critical_calculation,
                           const uint16_t rate_of_critical_bonus,
                           uint8_t* const result_of_skill_effect_flag,
                           //
                           eATTACK_TYPE skill_attack_type,
                           const int skill_attack_power,  // + value
                           const float skill_added_ratio_damage); // * (100% + ratio value)

// NOTE: f110215.3L, calculate (simulate) fixed physical | elemental skill damage
//  rate_of_critical_bonus : increase damage if the result is succeed
extern int CalcFixedSkillDamage(
    Character* attacker, Character* target,
    eATTACK_TYPE attack_type,
    const bool handle_critical_calculation, const uint16_t rate_of_critical_bonus,
    uint8_t* const result_of_skill_effect_flag,
    //
    eATTACK_TYPE skill_attack_type,
    const int skill_attack_power,  // + value
    const float skill_added_ratio_damage); // * (100% + ratio value)

// 내구도 감소비율
extern float GetDuraDecRatio(eZONE_STATE zone_state, eCOMPETITION_MODE_TYPE eCompetitionMode);
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
extern float GetDuraDecRatio(eZONE_STATE zone_state, ChaosZoneType chaos_zone_type);
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND

//
extern EXPTYPE GetKillExpOfParty(BYTE byRoomBonusType, DWORD partymemberNum,
                                 LEVELTYPE partyTotalLV, LEVELTYPE attackerLV, LEVELTYPE targetLV,
                                 float fNPCGradeExpRatio,
                                 DWORD attackDamage,
                                 DWORD targetMaxHP,
                                 eZONETYPE ZoneType,
                                 Player* player);

// HP,MP 회복량(플레이어)
extern int CalcHPRecover(eCHAR_TYPE char_type, DWORD maxHP, BYTE byCharState, LEVELTYPE Level);
extern int CalcSDRecover(eCHAR_TYPE char_type, eCHAR_CONDITION byCharState, BYTE byCharAction,
                         LEVELTYPE level);

extern int CalcMPRecover(eCHAR_TYPE char_type, DWORD spirit, BYTE byCharState);
extern DWORD CalcMPRecoverByAttack(eCHAR_TYPE char_type, DWORD spirit,
                                   DWORD skillRecoverPercent,
                                   DWORD skillRequireMP); // 공격할때 회복량
extern DWORD CalcMPRecoverByAttacked(eCHAR_TYPE char_type,
                                     DWORD spirit,
                                     DAMAGETYPE damage); // 타격 받았을떄 회복량

#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
// FP회복량 (플레이어, 위치블레이드)
extern PLAYERFP CalcFpRecover(eCHAR_TYPE char_type);
extern PLAYERFP CalcFPRecoverByAttack(eCHAR_TYPE char_type);
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
// HP, MP 회복량(몬스터)
extern int CalcNPCHPRecover(DWORD maxHP, DWORD recoverRatio = 1);
extern int CalcNPCMPRecover(DWORD maxMP, DWORD recoverRatio = 1);
extern int CalcNPCSDRecover(DWORD maxSD);

// CHANGES: f110215.3L, f110217.5L, changes periodic damage formula
// (Base + Element damage) * [apply ratio (iOption2)] + [skill attack power]
struct DamagePerCycleCalculator
{
    static int Simulate(
        Character* attacker, Character* target,
        eATTACK_TYPE attack_type,
        eATTACK_TYPE skill_attack_type,
        const int apply_added_base_damage_ratio, // INPUT : permilage (ability_info->m_iOption2)
        const int skill_attack_power,  // + value
        const float skill_added_ratio_damage); // * (100% + ratio value)
};

// GM utility
struct DamageBalanceConstant
{
    double attacker_level_weight;
    double diff_level_weight;
    double damage_deduction_pvp_weight;
    double damage_deduction_pve_weight;
};

extern bool ModifyDamageBalanceConstant(
    const DamageBalanceConstant& modifying_constants);
extern bool GetCurrentDamageBalanceConstant(
    DamageBalanceConstant* const current_constants);

}}; //end of namespace 'ns_formula::ep2'
#endif //_NA001605_EP2_ATTRIBUTES_RENEWAL_


#ifdef _NA001605_EP2_ATTRIBUTES_RENEWAL_
    using namespace ns_formula::ep2;
#else //if !defined(_NA001605_EP2_ATTRIBUTES_RENEWAL_)
    using namespace ns_formula::ep1;
#endif

#endif //GAMESERVER_CHARACTER_FORMULA_H