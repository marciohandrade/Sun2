#include "stdafx.h"
#include "NPCAttributes.h"

#include <Struct.h>
#include "SCNPC.h"
#include "CommonCharacterFormula.h"

//==================================================================================================

NPCAttributes::NPCAttributes()
    : owner_(NULL)
{
    // NPC는 eUPDATETYPE_SUM_RATIO 로 재설정해 준다.
    m_PhysicalAttackRate.SetType(eBOUNDTYPE_ZP_10000, eUPDATETYPE_SUM_RATIO);
    m_PhysicalAvoidRate.SetType(eBOUNDTYPE_ZP_10000, eUPDATETYPE_SUM_RATIO);
}

NPCAttributes::~NPCAttributes()
{
}

void NPCAttributes::SetOwner(SCNPC* pOwner)
{
    assert(pOwner != NULL);
    owner_ = pOwner; 
}

void NPCAttributes::SetCharInfo(const BASE_NPCINFO* npc_info,
                                int recover_hp, int recover_mp, int recover_sd)
{
    assert(npc_info != NULL);

    // HP, MP
    m_MaxHP.base_value_.value_ = npc_info->m_dwMaxHP;
    m_MaxMP.base_value_.value_ = npc_info->m_dwMaxMP;
    m_MaxSD.base_value_.value_ = npc_info->m_dwMaxSD;

    // 근거리, 원거리 공격력
    m_BaseMeleeMinAttPower.base_value_.value_ = npc_info->m_dwMinAttackPower;
    m_BaseMeleeMaxAttPower.base_value_.value_ = npc_info->m_dwMaxAttackPower;
    m_BaseRangeMinAttPower.base_value_.value_ = npc_info->m_dwMinAttackPower;
    m_BaseRangeMaxAttPower.base_value_.value_ = npc_info->m_dwMaxAttackPower;
#if !defined(_NA001605_EP2_ATTRIBUTES_RENEWAL_)
    // 마법 공격력
    m_BaseMagicMinAttPower.base_value_.value_ = npc_info->m_dwMinAttackPower;
    m_BaseMagicMaxAttPower.base_value_.value_ = npc_info->m_dwMaxAttackPower;
#endif
    // 근거리, 원거리 방어력
    m_BaseMeleeDefPower.base_value_.value_ = npc_info->m_dwPhyDef;
    m_BaseRangeDefPower.base_value_.value_ = npc_info->m_dwPhyDef;
#if !defined(_NA001605_EP2_ATTRIBUTES_RENEWAL_)
    // 마법 방어력
    m_BaseMagicDefPower.base_value_.value_ = npc_info->m_dwMagicDef;
#endif
#ifdef _NA001605_EP2_ATTRIBUTES_RENEWAL_
    // f101229.1L,
    m_SkillAttackPower.base_value_.value_ = npc_info->skill_power;

    m_MagicalAttPower[eATTACK_TYPE_WATER].base_value_.value_ = npc_info->element_water_attack;
    m_MagicalAttPower[eATTACK_TYPE_FIRE].base_value_.value_ = npc_info->element_fire_attack;
    m_MagicalAttPower[eATTACK_TYPE_WIND].base_value_.value_ = npc_info->element_wind_attack;
    m_MagicalAttPower[eATTACK_TYPE_EARTH].base_value_.value_ = npc_info->element_earth_attack;
    m_MagicalAttPower[eATTACK_TYPE_DARKNESS].base_value_.value_ = npc_info->element_dark_attack;
    //
    if (AttrValue* attr = &m_ReduceElementDamageRatio[Elements::kToElementIndex[eATTACK_TYPE_WATER]]) {
        attr->base_value_.value_ += npc_info->element_water_resist;
    }
    if (AttrValue* attr = &m_ReduceElementDamageRatio[Elements::kToElementIndex[eATTACK_TYPE_FIRE]]) {
        attr->base_value_.value_ += npc_info->element_fire_resist;
    }
    if (AttrValue* attr = &m_ReduceElementDamageRatio[Elements::kToElementIndex[eATTACK_TYPE_WIND]]) {
        attr->base_value_.value_ += npc_info->element_wind_resist;
    }
    if (AttrValue* attr = &m_ReduceElementDamageRatio[Elements::kToElementIndex[eATTACK_TYPE_EARTH]]) {
        attr->base_value_.value_ += npc_info->element_earth_resist;
    }
    if (AttrValue* attr = &m_ReduceElementDamageRatio[Elements::kToElementIndex[eATTACK_TYPE_DARKNESS]]) {
        attr->base_value_.value_ += npc_info->element_dark_resist;
    }
    //m_MagicalDefPower[eATTACK_TYPE_DARKNESS].base_value_.value_ = npc_info->element_dark_resist;
#else
    m_MagicalDefPower[eATTACK_TYPE_WATER].base_value_.value_ += npc_info->element_water_resist;
    m_MagicalDefPower[eATTACK_TYPE_FIRE].base_value_.value_ += npc_info->element_fire_resist;
    m_MagicalDefPower[eATTACK_TYPE_WIND].base_value_.value_ += npc_info->element_wind_resist;
    m_MagicalDefPower[eATTACK_TYPE_EARTH].base_value_.value_ += npc_info->element_earth_resist;
#endif

#ifdef _NA001605_EP2_ATTRIBUTES_RENEWAL_
    // 공격 성공률, 회피율
    // the update type of this attribute is changed to eUPDATETYPE_NOTHING.
    if (AttrValue* attr_value = &m_PhysicalAttackRate) {
        attr_value->base_value_.value_ = npc_info->m_wPhyAttRate;
        attr_value->calc_value_.value_ = attr_value->base_value_.value_;
    };
    if (AttrValue* attr_value = &m_PhysicalAvoidRate) {
        attr_value->base_value_.value_ = npc_info->m_wPhyAttAvoid;
        attr_value->calc_value_.value_ = attr_value->base_value_.value_;
    };
#else
    // 공격 성공률, 회피율
    m_PhysicalAttackRate.base_value_.value_ = npc_info->m_wPhyAttRate;
    m_PhysicalAvoidRate.base_value_.value_ = npc_info->m_wPhyAttAvoid;
#endif

    // 크리티컬 데미지(기획과 상의)
    m_CriticalRatioChange.base_value_.value_ = npc_info->m_byCriticalRatio;
#if !defined(_NA001605_EP2_ATTRIBUTES_RENEWAL_)
    m_MagicCriticalRatio.base_value_.value_ = npc_info->m_byCriticalRatio;
#endif
    // 시야
    m_SightRange.base_value_.value_ = static_cast<int>(npc_info->m_fViewRange * 10);

    // HP, MP 회복률
    m_RecoverHP.base_value_.value_ = recover_hp;
    m_RecoverMP.base_value_.value_ = recover_mp;
    m_RecoverSD.base_value_.value_ = recover_sd;
}

void NPCAttributes::Update()
{
    //----------------------------------------------------------------------------------------------
    // HP, MP, SD
    ;{
        m_MaxHP.Update();
        m_MaxMP.Update();
        m_MaxSD.Update();
        m_RecoverHP.Update();
        m_RecoverMP.Update();
        m_RecoverSD.Update();
    };
    //----------------------------------------------------------------------------------------------
    // Attack power & Defence power
    ;{
        UpdateAttackPower();
        UpdateDefense();
    };
    //----------------------------------------------------------------------------------------------
    ;{
    #if !defined(_NA001605_EP2_ATTRIBUTES_RENEWAL_)
        // 공격 타입별 추가 방어력
        for (int i = 0; i < eATTACK_TYPE_MAX; ++i)
        {
            m_BonusDefense[i].Update();
        }
        // 아머 타입별 추가 데미지(퍼센트)
        for (int i = eARMOR_HARD; i < eARMOR_TYPE_MAX; ++i)
        {
            m_BonusDamage[i].Update();
            m_BonusPercentDamage[i].Update();
        }
        // 데미지 감소
        for (int i = 0; i < eATTACK_TYPE_MAX; ++i)
        {
            m_ReduceDamage[i].Update();
        }
    #endif
    };
    ;{
        // 공격 성공률, 회피율
        m_PhysicalAttackRate.Update();  m_PhysicalAvoidRate.Update();
    };

    //----------------------------------------------------------------------------------------------
    // range attributes
    ;{
        m_MoveSpeedRatio.Update();
#ifdef _NA_008486_20150914_TOTAL_BALANCE
        int calc_value = CalcMoveSpeedRatio(0, m_MoveSpeedRatio.skill_value_);
#else
        int calc_value = CalcMoveSpeedRatio(0, 0, m_MoveSpeedRatio.skill_value_);
#endif //_NA_008486_20150914_TOTAL_BALANCE
        calc_value *= (100 + static_cast<const AttrValue&>(m_MoveSpeedRatio)[eATTR_KIND_RATIO]);
        calc_value /= 100;
        m_MoveSpeedRatio.calc_value_.value_ = calc_value;
        m_MoveSpeedRatio.calc_value_.touch(static_cast<eBOUNDTYPE>(m_MoveSpeedRatio.bound_type_));
    };
    ;{
        m_AttSpeedRatio.Update();
        int calc_value = CalcAttackSpeedRatio((eCHAR_TYPE)0, eWEAPONTYPE_PUNCH, 0, 0, \
                                              m_AttSpeedRatio.skill_value_);
        calc_value *= (100 + static_cast<const AttrValue&>(m_AttSpeedRatio)[eATTR_KIND_RATIO]);
        calc_value /= 100;
        m_AttSpeedRatio.calc_value_.value_ = calc_value;
        m_AttSpeedRatio.calc_value_.touch(static_cast<eBOUNDTYPE>(m_AttSpeedRatio.bound_type_));
    };
    ;{
        // 사거리 보너스
        for (int i = eATTR_KIND_ITEM; i <= eATTR_KIND_RATIO; ++i)
        {
            eATTR_KIND attr_kind = static_cast<eATTR_KIND>(i);
            const int apply_value = static_cast<const AttrValue&>(m_AllRangeBonus)[attr_kind];
            if (apply_value)
            {
                m_NormalRangeBonus[attr_kind] += apply_value;
                m_SkillRangeBonus[attr_kind] += apply_value;
            }
        }
        m_NormalRangeBonus.Update();
        m_SkillRangeBonus.Update();
        m_AllRangeBonus.Init(); // <- 

        // 시야
        m_SightRange.Update();
    };
    ;{
        // 크리티컬 데미지
        m_CriticalRatioBonus.Update();  // 물리 + 마법 크리티컬

        m_CriticalRatioChange.Update();
        m_CriticalRatioChange.calc_value_.value_ += m_CriticalRatioBonus.calc_value_.value_;
    #if !defined(_NA001605_EP2_ATTRIBUTES_RENEWAL_)
        m_MagicCriticalRatio.Update();
        m_MagicCriticalRatio.calc_value_.value_ += m_CriticalRatioBonus.calc_value_.value_;
    #endif
        m_CriticalDamageBonus.Update();
    };
    ;{
        // 스킬 공격력, 스킬 퍼센트 추가 데미지
        m_SkillAttackPower.Update();
        m_SkillPercentDamage.Update();
    };
    ;{
        m_AbsorbHP.Update();
        m_AbsorbMP.Update();
    };
    ;{
        // REMOVE_UNUSED_SECTION_IN_NPC: m_BonusCastingTime.Update();
        // REMOVE_UNUSED_SECTION_IN_NPC: m_BonusSkillCoolTime.Update();
        // REMOVE_UNUSED_SECTION_IN_NPC: m_IncreaseSkillDuration.Update();
        // REMOVE_UNUSED_SECTION_IN_NPC: m_DecreaseSkillDuration.Update();
    };
    ;{
        m_AttackIncRate.Update();
        m_DefenseIncRate.Update();
        m_AreaAttackRatio.Update();
        m_ReflectDamageRatio.Update();
        m_DecDamage.Update();
        m_DoubleDamageRatio.Update();   // 데미지의 두배가 적용될 확률
        m_IncreaseMinDamage.Update();
        m_IncreaseMaxDamage.Update();
    };
    ;{
        // REMOVE_UNUSED_SECTION_IN_NPC: m_EhterDamageRatio.Update();
        m_DecreasePVPDamage.Update();
        // REMOVE_UNUSED_SECTION_IN_NPC: m_LuckMonIncDamage.Update();    // 럭키 몬스터 추가 데미지
        m_BypassDeffenceRatio.Update();
    };
    ;{
        // REMOVE_UNUSED_SECTION_IN_NPC: m_IncreaseReserveHPRatio.Update();
        // REMOVE_UNUSED_SECTION_IN_NPC: m_MPSpendIncrease.Update();
        m_ResistBadStatusRatio.Update();
    };
    ;{
        m_SkillLevelAll.Update();
        // REMOVE_UNUSED_SECTION_IN_NPC: m_DecreaseLimitStat.Update();
        // REMOVE_UNUSED_SECTION_IN_NPC: m_DecreaseItemDuraRatio.Update();
        // REMOVE_UNUSED_SECTION_IN_NPC: m_BonusMoneyRatio.Update();
        // REMOVE_UNUSED_SECTION_IN_NPC: m_BonusExpRatio.Update();
        // REMOVE_UNUSED_SECTION_IN_NPC: m_ResurrectionRatio.Update();   // 사망시 스스로 부활 할 확률
        // REMOVE_UNUSED_SECTION_IN_NPC: m_CompositeIncRatio.Update();    // 조합 성공률 증가    
    };
    ;{  //_NA_001231_20081216_ADD_OPTION_KIND
        m_ResistHolding.Update();
        m_ResistSleep.Update();
        m_ResistPoison.Update();
        m_ResistKnockBack.Update();
        m_ResistDown.Update();
        m_ResistStun.Update();
    };
#ifdef _NA_006680_20130426_ITEM_OPTION_ADD_AND_MODIFY
    ;{
        m_AttackDamageAbsorbSDRatio.Update();
        m_AttackDamageAbsorbHPRatio.Update();
    };
#endif // _NA_006680_20130426_ITEM_OPTION_ADD_AND_MODIFY

//#ifdef __NA_001244_20090417_ATTACK_RESIST
//    for (int i = 0; i < eCHAR_TYPE_MAX; ++i)
//        for (int j = 0; j < eATTACK_RESIST_MAX; ++j)
//            m_ResistAttack[i][j].Update();
//#endif
}


void NPCAttributes::UpdateAttackPower()
{
    //-----------------------------------------------------------------
    // 기본 공격력 계산
    //-----------------------------------------------------------------
    // 스텟에 의한 Base + 아이템에 의한 Base
    m_BaseMeleeMinAttPower.Update();
    m_BaseMeleeMaxAttPower.Update();
    m_BaseRangeMinAttPower.Update();
    m_BaseRangeMaxAttPower.Update();
#if !defined(_NA001605_EP2_ATTRIBUTES_RENEWAL_)
    m_BaseMagicMinAttPower.Update();
    m_BaseMagicMaxAttPower.Update();
#endif
    //-----------------------------------------------------------------
    // 옵션 공격력
    //-----------------------------------------------------------------
    // 옵션 전체(물리, 마법) 공격력을 적용한다.
#ifdef _NA001605_EP2_ATTRIBUTES_RENEWAL_
    m_OptionPhysicalAttPower.Update();
#else
    for (int i = eATTR_KIND_ITEM; i <= eATTR_KIND_RATIO; ++i)
    {
        eATTR_KIND attr_kind = static_cast<eATTR_KIND>(i);
        const int apply_value = \
            static_cast<const AttrValue&>(m_OptionAllAttPower)[attr_kind];
        if (apply_value)
        {
            m_OptionPhysicalAttPower[attr_kind] += apply_value;
        #if !defined(_NA001605_EP2_ATTRIBUTES_RENEWAL_)
            m_OptionMagicAttPower[attr_kind] += apply_value;
        #endif
        };
    }

    // 옵션 공격력의 eATTR_KIND_CALC 값을 계산한다.
    m_OptionPhysicalAttPower.Update();
    m_OptionMagicAttPower.Update();
    // 전체 반영 파라미터인 경우 초기화를 해야 다음번에 다시 계산되지 않는다.
    m_OptionAllAttPower.Init();
#endif
    //
    int iAverageAttPower = (GetBaseMeleeMinAttPower() + GetBaseMeleeMaxAttPower() +
                            GetBaseRangeMinAttPower() + GetBaseRangeMaxAttPower()) / 4;

    m_OptionPhysicalAttPower.calc_value_.value_ += iAverageAttPower * \
        static_cast<const AttrValue&>(m_OptionPhysicalAttPower)[eATTR_KIND_RATIO] / 100;
    m_OptionPhysicalAttPower.calc_value_.touch(\
        static_cast<eBOUNDTYPE>(m_OptionPhysicalAttPower.bound_type_));
    //
#if !defined(_NA001605_EP2_ATTRIBUTES_RENEWAL_)
    int iAverageMagicAttPower = (GetBaseMagicMinAttPower() + GetBaseMagicMaxAttPower()) / 2;
    m_OptionMagicAttPower.calc_value_.value_ += iAverageMagicAttPower * \
        static_cast<const AttrValue&>(m_OptionMagicAttPower)[eATTR_KIND_RATIO] / 100;
    m_OptionMagicAttPower.calc_value_.touch(\
        static_cast<eBOUNDTYPE>(m_OptionMagicAttPower.bound_type_));
#endif

    //-----------------------------------------------------------------
    // 속성 공격력
    //-----------------------------------------------------------------
    // 속성 전체 공격력을 적용한다.
    ;{
        int magical_all_attacks[eATTR_KIND_RATIO - eATTR_KIND_ITEM + 1];
        int* apply_value_it = magical_all_attacks;
        for (int i = eATTR_KIND_ITEM; i <= eATTR_KIND_RATIO; ++i) {
            *apply_value_it++ = \
                static_cast<const AttrValue&>(m_MagicalAllAttPower)[static_cast<eATTR_KIND>(i)];
        }
        for (int j = eATTACK_TYPE_WATER; j <= eATTACK_TYPE_DARKNESS; ++j)
        {
            AttrValue& attr_value = m_MagicalAttPower[j];
            apply_value_it = magical_all_attacks;
            for (int i = eATTR_KIND_ITEM; i <= eATTR_KIND_RATIO; ++i, ++apply_value_it)
            {
                eATTR_KIND attr_kind = static_cast<eATTR_KIND>(i);
                if (int apply_value = *apply_value_it) {
                    attr_value[attr_kind] += apply_value;
                }
            }
        }
    };

    // 속성 공격력의 eATTR_KIND_CALC 값을 계산한다.
    for (int i = 0; i < eATTACK_TYPE_MAX; ++i)
    {
        m_MagicalAttPower[i].Update();
    }

    // 전체 반영 파라미터인 경우 초기화를 해야 다음번에 다시 계산되지 않는다.
    m_MagicalAllAttPower.Init();
}

void NPCAttributes::UpdateDefense()
{
    //-----------------------------------------------------------------
    // 기본 방어력 계산
    //-----------------------------------------------------------------
    // 스텟에 의한 Base + 아이템에 의한 Base
    m_BaseMeleeDefPower.Update();
    m_BaseRangeDefPower.Update();
#if !defined(_NA001605_EP2_ATTRIBUTES_RENEWAL_)
    m_BaseMagicDefPower.Update();
#endif
    ;{
        //-----------------------------------------------------------------
        // 옵션 방어력
        //-----------------------------------------------------------------
        // 옵션 전체(물리, 마법) 방어력을 적용한다.
    #ifdef _NA001605_EP2_ATTRIBUTES_RENEWAL_
        m_OptionPhysicalDefPower.Update();
    #else
        for (int i = eATTR_KIND_ITEM; i <= eATTR_KIND_RATIO; ++i)
        {
            eATTR_KIND attr_kind = static_cast<eATTR_KIND>(i);
            const int apply_value = \
                static_cast<const AttrValue&>(m_OptionAllDefPower)[attr_kind];
            if (apply_value)
            {
                m_OptionPhysicalDefPower[attr_kind] += apply_value;
            #if !defined(_NA001605_EP2_ATTRIBUTES_RENEWAL_)
                m_OptionMagicDefPower[attr_kind] += apply_value;
            #endif
            }
        }
        m_OptionPhysicalDefPower.Update();
        m_OptionMagicDefPower.Update();
        // 전체 반영 파라미터인 경우 초기화를 해야 다음번에 다시 계산되지 않는다.
        m_OptionAllDefPower.Init();
    #endif
        //
        // 옵션 공격력의 eATTR_KIND_CALC 값을 계산한다.
        int iAverageDefPower = (GetBaseMeleeDefPower() + GetBaseRangeDefPower())/2;
        m_OptionPhysicalDefPower.calc_value_.value_ += iAverageDefPower * \
            static_cast<const AttrValue&>(m_OptionPhysicalDefPower)[eATTR_KIND_RATIO] / 100;
        m_OptionPhysicalDefPower.calc_value_.touch(\
            static_cast<eBOUNDTYPE>(m_OptionPhysicalDefPower.bound_type_));
    #if !defined(_NA001605_EP2_ATTRIBUTES_RENEWAL_)
        m_OptionMagicDefPower.calc_value_.value_ += GetBaseMagicDefPower() * \
            static_cast<const AttrValue&>(m_OptionMagicDefPower)[eATTR_KIND_RATIO] / 100;
        m_OptionMagicDefPower.calc_value_.touch(\
            static_cast<eBOUNDTYPE>(m_OptionMagicDefPower.bound_type_));
    #endif
    };
#ifdef _NA001605_EP2_ATTRIBUTES_RENEWAL_
    ;{
        // 공격 대상 방어력 감소율 (EP1: same as m_ReduceDefenseRate)
        m_ReducePhysicalTargetDefenseRatio.Update();
#ifdef _NA_004994_20120614_RATE_OF_DEFENCE_POWER_REDUCTION_OF_TARGET
        m_ReduceElementTargetDefenseRatioAll.Update();
#endif  // _NA_004994_20120614_RATE_OF_DEFENCE_POWER_REDUCTION_OF_TARGET
        const int all_apply_value = m_ReduceElementTargetDefenseRatioAll.calc_value_.value_;
        for (AttrValue* it = m_ReduceElementTargetDefenseRatio;
             it != &m_ReduceElementTargetDefenseRatio[_countof(m_ReduceElementTargetDefenseRatio)];
             ++it)
        {
            it->Update();
            it->calc_value_.value_ += all_apply_value;
            it->calc_value_.touch(static_cast<eBOUNDTYPE>(it->bound_type_));
        }
    };
    ;{
        // 공격에 대한 데미지 감소율
        m_ReducePhysicalDamageRatio.Update();
        m_ReduceElementDamageRatioAll.Update();
        const int all_apply_value = m_ReduceElementDamageRatioAll.calc_value_.value_;
        // 원소 방어력 ranges = [-100%, +XXX% ]
        for (AttrValue* it = m_ReduceElementDamageRatio;
             it != &m_ReduceElementDamageRatio[_countof(m_ReduceElementDamageRatio)];
             ++it)
        {
            it->Update();
            it->calc_value_.value_ += all_apply_value;
            it->calc_value_.touch(static_cast<eBOUNDTYPE>(it->bound_type_));
        }
    };
#else //if !defined(_NA001605_EP2_ATTRIBUTES_RENEWAL_)
    ;{
        //-----------------------------------------------------------------
        // 속성 방어력
        //-----------------------------------------------------------------
        // 속성 전체 방어력을 적용한다.
        int magical_all_defences[eATTR_KIND_RATIO - eATTR_KIND_ITEM + 1];
        int* apply_value_it = magical_all_defences;
        for (int i = eATTR_KIND_ITEM; i <= eATTR_KIND_RATIO; ++i) {
            *apply_value_it++ = \
                static_cast<const AttrValue&>(m_MagicalAllDefPower)[static_cast<eATTR_KIND>(i)];
        }
        for (int j = eATTACK_TYPE_WATER; j <= eATTACK_TYPE_DARKNESS; ++j)
        {
            AttrValue& attr_value = m_MagicalDefPower[j];
            apply_value_it = magical_all_defences;
            for (int i = eATTR_KIND_ITEM; i <= eATTR_KIND_RATIO; ++i, ++apply_value_it)
            {
                eATTR_KIND attr_kind = static_cast<eATTR_KIND>(i);
                if (int apply_value = *apply_value_it) {
                    attr_value[attr_kind] += apply_value;
                }
            }
        }
    };
    // 속성 방어력의 eATTR_KIND_CALC 값을 계산한다.
    for (int i = 0; i < eATTACK_TYPE_MAX; ++i)
    {
        m_MagicalDefPower[i].Update();
    }

    // 전체 반영 파라미터인 경우 초기화를 해야 다음번에 다시 계산되지 않는다.
    m_MagicalAllDefPower.Init();
#endif
}

#ifdef _SERVER
//==================================================================================================
// CHANGES: f110413.1L, added interface to support light recovery quantity calculation for server
// NOTE: HP, MP and SD recovery have busy update processes.
// if server is to avoid busy updating, detach the recovery parts from full update calling.
// the usage of this method is calling on changed recoveries in run-time service
// not to initialize like the 'SetCharInfo'
void NPCAttributes::UpdateChangedRecoveries(
    bool hp_update, int hp_recovery_quantity,
    bool mp_update, int mp_recovery_quantity,
    bool sd_update, int sd_recovery_quantity)
{
    bool need_update = false;
    if (hp_update) {
        m_RecoverHP.base_value_.value_ = hp_recovery_quantity;
        need_update = true;
    };
    if (mp_update) {
        m_RecoverMP.base_value_.value_ = mp_recovery_quantity;
        need_update = true;
    };
    if (sd_update) {
        m_RecoverSD.base_value_.value_ = sd_recovery_quantity;
        need_update = true;
    };

    if (need_update) {
        // HP, MP and SD recovery don't effect other attributes, also don't notify to client
        m_RecoverHP.Update();
        m_RecoverMP.Update();
        m_RecoverSD.Update();
    };
}

#endif //