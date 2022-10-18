#include "stdafx.h"

#include <float.h>
#include <PublicMath.h>

#include "DragonTransStatus.h"
#include "GameServerEx.h"
#include "GameZone.h"
#include "ItemInfoParser.h"
#include "ItemManager.h"
#include "MovementChecker.h"
#include "Player.h"
#include "ResultCode.h"
#include "SCSlotContainer.h"
#include "SCSkillSlotContainer.h"
#include "ServerOptionParserEx.h"
#include "Skill.h"
#include "SkillInfoParser.h"
#include "SkillSlotManager.h"
#include "StatusManager.h"
#include "SummonedNPCs.h"
#include "SummonManager.h"

#ifdef _NA_007667_20141001_WITCHBLADE_SKILL
#include "MovingPlanner.h"
#endif //_NA_007667_20141001_WITCHBLADE_SKILL

RC::eSKILL_RESULT Player::CanUseSkilltoTarget(SkillScriptInfo* pBaseSkillInfo, DWORD TargetKey)
{
    GameField* pField = GetField();
    if(pField == NULL) return RC::RC_SKILL_INVALID_STATE;

    Player* pCharacter = static_cast<Player*>(pField->FindObject(PLAYER_OBJECT, TargetKey));
    if(pCharacter)
    {
        if((pBaseSkillInfo->m_byTarget == SKILL_TARGET_ENEMY_PLAYER) && (IsFriend(pCharacter) == USER_RELATION_FRIEND))
            return RC::RC_SKILL_STATE_WHERE_CANNOT_ATTACK_ENEMY;

        if(pCharacter->IsDead() && (pBaseSkillInfo->m_SkillClassCode != eSKILL_RESSURECTION) &&
#ifdef _NA_000000_20130812_NEW_CHARACTER_HELLROID
			(pBaseSkillInfo->m_SkillClassCode != eSKILL_HELLROID_1021) &&	// 헬로이드 리바이브 스킬
#endif //_NA_000000_20130812_NEW_CHARACTER_HELLROID
            (pBaseSkillInfo->m_SkillClassCode != eSKILL_REVIVAL) && (pBaseSkillInfo->m_SkillClassCode != eSKILL_REVERSE))
            return RC::RC_SKILL_TARGET_ISDEAD;
#ifdef _NA_006623_20130409_STEEL_MODE_WHEN_ENERMY_CAN_USE_SKILL
        if (pCharacter->IsStatus(eCHAR_STATE_STEEL)) {
            return RC::RC_SKILL_INVALID_STATE;
        }
#endif // _NA_006623_20130409_STEEL_MODE_WHEN_ENERMY_CAN_USE_SKILL
    }

    BASE_ABILITYINFO* pBaseAbilityInfo = NULL;
#ifdef _NA_003649_20111115_BATTLE_ZONE_HARDMODE
#if !defined(_NA_007123_20140324_SHADOW_SKILL_RENEWAL)
    pBaseAbilityInfo = pBaseSkillInfo->GetAbilityInfo(eABILITY_SUMMON_DEAD_MONSTER);
    if(pBaseAbilityInfo) {
        // 트리거 설정된 몬스터는 소환 불가능 - arycoat 2012.01.20
        NPC* target_monster = static_cast<NPC*>(pField->FindObject(MONSTER_OBJECT, TargetKey));
        if(target_monster) {
            const MONSTERCODE monster_code = target_monster->GetBaseInfo()->m_MonsterCode;
            const sNPC_POLICY::ePOLICY policy = \
                AppliedNpcDependencyChain::Instance()->GetNpcDependencyInfo( monster_code );

            if (sNPC_POLICY::NPC_POLICY_NPC_ONDEAD_TRIGGER & policy ) {
                return RC::RC_SKILL_CANNOT_SUMMON;
            }
        }
    }
#endif //_NA_007123_20140324_SHADOW_SKILL_RENEWAL

    pBaseAbilityInfo = pBaseSkillInfo->GetAbilityInfo(eABILITY_CUR_HP_INCREASE);
    if (pBaseAbilityInfo) {
        if (pBaseAbilityInfo->m_byRangeType == SKILL_ABILITY_FRIEND ||
            pBaseAbilityInfo->m_byRangeType == SKILL_ABILITY_MYAREA_FRIEND ) 
        {
            NPC* target_monster = static_cast<NPC*>(pField->FindObject(MONSTER_OBJECT, TargetKey));
            if (target_monster && target_monster->IsEqualObjectKind(SUMMON_OBJECT)) {
                return RC::RC_SKILL_CANNOT_HEAL;
            }
        }
    }
#endif //_NA_003649_20111115_BATTLE_ZONE_HARDMODE

    return RC::RC_SKILL_SUCCESS;
}

#ifdef _NA_007667_20141001_WITCHBLADE_SKILL
RC::eSKILL_RESULT Player::CanUseSkill( const SkillInfo* skill_info, BOOL bCoolTimerReset /*= TRUE*/ )
{
    SLOTCODE skill_code = skill_info->skill_code_;
    DWORD target_key = skill_info->main_target_key_;
    WzVector target_pos = skill_info->main_target_pos_;

    SkillScriptInfo* pBaseSkillInfo = (SkillScriptInfo *)SkillInfoParser::Instance()->GetSkillInfo(skill_code);
    if(!pBaseSkillInfo)
    {
        return RC::RC_SKILL_BASEINFO_NOTEXIST;
    }

    RC::eSKILL_RESULT rcResult = Character::CanUseSkill(skill_code, bCoolTimerReset);
    if(rcResult != RC::RC_SKILL_SUCCESS)  
        return rcResult;

    if (GetCharType() == eCHAR_WITCHBLADE)
    {
        //FP검사-----------------------------------------------------
        BASE_ABILITYINFO* fp_ability_info = pBaseSkillInfo->GetAbilityInfo(eABILITY_FP_CHANGE);
        if (fp_ability_info == NULL) {
            fp_ability_info = pBaseSkillInfo->GetAbilityInfo(eABILITY_FP_TOGGLE);
        }

        if (fp_ability_info != NULL) 
        {
            PLAYERFP need_fp = (PLAYERFP)fp_ability_info->m_iParam[1];
            PLAYERFP result_fp = min(0, GetFP() + need_fp);

            if (result_fp < 0) {
                return RC::RC_SKILL_FP_RUN_SHORT;
            }
        }
        //고정형 소환수-----------------------------------------------------
        GameField* field = GetField();
        fp_ability_info = NULL;
        fp_ability_info = pBaseSkillInfo->GetAbilityInfo(eABILITY_SUMMON_IMMOVABLE);
        if (fp_ability_info != NULL && field != NULL)
        {
            nsAI::MovingPlanner* moving_planner = field->GetMovingPlanner();
            WORD attributes;
            INT iTile = moving_planner->GetAllowTileToStand(target_pos, NULL, -1.0f, 100.0f, &attributes,
                PTA_NO_WALK | PTA_PLAYER_FALLDOWN_TILE);
            if (iTile  < 0) {
                return RC::RC_SKILL_FAILED;
            }
        }
    }

    SCSkillSlotContainer* pContainer = m_pSkillSlotManager->GetSkillSlotContainer();

    SCSkillSlot* pCurSkillSlot = pContainer->GetSkillSlotByCode(skill_code);

    // 널 체크한다.
    if(FALSE == CheckNullSkillSlot(pCurSkillSlot,skill_code))  
        return RC::RC_SKILL_DOES_NOT_HAVE;           

    // 요구레벨을 체크한다.
    if(FALSE == CheckRequireLv(pBaseSkillInfo))               
        return RC::RC_SKILL_REQUIRE_LEVEL_LIMIT;     

    // 스킬을 사용할 수 있는 캐릭터인지 체크한다.
    if(!CheckClassDefine(skill_code, TRUE))
    {
        SUNLOG(eFULL_LOG, "[Player::CanUseSkill] SkillCode[%d] The Type Of Character is Invalid!", skill_code);
        return RC::RC_SKILL_CHAR_CLASS_LIMIT;
    }

    // 스킬, 어빌리티 별로 조건 체크
    rcResult = CheckSkillCondition(skill_code);
    if(rcResult != RC::RC_SKILL_SUCCESS)  return rcResult;

    // 무기 제한
    if (CheckWeaponDefine(pBaseSkillInfo) == false)
    {
        return RC::RC_SKILL_WEAPON_LIMIT;
    }

    rcResult = CanUseSkilltoTarget(pBaseSkillInfo, target_key);
    if(rcResult != RC::RC_SKILL_SUCCESS) {
        return rcResult;
    }

    // 쿨타임 처리
    if(bCoolTimerReset && FALSE == CoolTimerReset(skill_code)) {
        return RC::RC_SKILL_COOLTIME_ERROR;
    }

    return RC::RC_SKILL_SUCCESS;
}
#else
RC::eSKILL_RESULT Player::CanUseSkill(SLOTCODE SkillCode, DWORD TargetKey, BOOL bCoolTimerReset)
{
    SkillScriptInfo* pBaseSkillInfo = (SkillScriptInfo *)SkillInfoParser::Instance()->GetSkillInfo(SkillCode);
    if(!pBaseSkillInfo)
    {
        return RC::RC_SKILL_BASEINFO_NOTEXIST;
    }

    RC::eSKILL_RESULT rcResult = Character::CanUseSkill(SkillCode, bCoolTimerReset);
    if(rcResult != RC::RC_SKILL_SUCCESS)  
        return rcResult;

    SCSkillSlotContainer* pContainer = m_pSkillSlotManager->GetSkillSlotContainer();

    SCSkillSlot* pCurSkillSlot = pContainer->GetSkillSlotByCode(SkillCode);

    // 널 체크한다.
    if(FALSE == CheckNullSkillSlot(pCurSkillSlot,SkillCode))  
        return RC::RC_SKILL_DOES_NOT_HAVE;           

    // 요구레벨을 체크한다.
    if(FALSE == CheckRequireLv(pBaseSkillInfo))               
        return RC::RC_SKILL_REQUIRE_LEVEL_LIMIT;     

#if !defined(_NA_0_20100901_SKILLTREE_SYSTEM)
    // 요구 숙련도 체크
    if(FALSE == CheckRequireSkillStat(pBaseSkillInfo))        
        return RC::RC_SKILL_REQUIRE_SKILLSTAT_LIMIT; 
#endif

    // 스킬을 사용할 수 있는 캐릭터인지 체크한다.
    if(!CheckClassDefine(SkillCode, TRUE))
    {
        SUNLOG(eFULL_LOG, "[Player::CanUseSkill] SkillCode[%d] The Type Of Character is Invalid!", SkillCode);
        return RC::RC_SKILL_CHAR_CLASS_LIMIT;
    }

    // 스킬, 어빌리티 별로 조건 체크
    rcResult = CheckSkillCondition(SkillCode);
    if(rcResult != RC::RC_SKILL_SUCCESS)  return rcResult;

    // 무기 제한
    if (CheckWeaponDefine(pBaseSkillInfo) == false)
    {
        return RC::RC_SKILL_WEAPON_LIMIT;
    }

    rcResult = CanUseSkilltoTarget(pBaseSkillInfo, TargetKey);
    if(rcResult != RC::RC_SKILL_SUCCESS) {
        return rcResult;
    }

    // 쿨타임 처리
    if(bCoolTimerReset && FALSE == CoolTimerReset(SkillCode)) {
        return RC::RC_SKILL_COOLTIME_ERROR;
    }

    return RC::RC_SKILL_SUCCESS;
}
#endif //_NA_007667_20141001_WITCHBLADE_SKILL

RC::eSKILL_RESULT Player::CanUseSummonSkill(SLOTCODE SkillCode, DWORD TargetKey, BOOL bCoolTimerReset)
{
    SkillScriptInfo* pBaseSkillInfo = (SkillScriptInfo *)SkillInfoParser::Instance()->GetSkillInfo(SkillCode);
    if(!pBaseSkillInfo)
        return RC::RC_SKILL_BASEINFO_NOTEXIST;

    SCSkillSlotContainer* pContainer = m_pSkillSlotManager->GetSkillSlotContainer();

    SCSkillSlot* pCurSkillSlot = pContainer->GetSkillSlotByCode(SkillCode);

    // 널 체크한다.
    if(FALSE == CheckNullSkillSlot(pCurSkillSlot,SkillCode))      
        return RC::RC_SKILL_DOES_NOT_HAVE;      

    // 요구레벨을 체크한다.
    if(FALSE == CheckRequireLv(pBaseSkillInfo))                   
        return RC::RC_SKILL_REQUIRE_LEVEL_LIMIT;     

#if !defined(_NA_0_20100901_SKILLTREE_SYSTEM)
    // 요구 숙련도 체크
    if(FALSE == CheckRequireSkillStat(pBaseSkillInfo))            
        return RC::RC_SKILL_REQUIRE_SKILLSTAT_LIMIT; 
#endif

    RC::eSKILL_RESULT rcResult = CanUseSkilltoTarget(pBaseSkillInfo, TargetKey);
    if(rcResult != RC::RC_SKILL_SUCCESS) {
        return rcResult;
    }

    // 쿨타임 처리
    if(bCoolTimerReset && FALSE == CoolTimerReset(SkillCode)) {
        return RC::RC_SKILL_COOLTIME_ERROR;
    }

    return RC::RC_SKILL_SUCCESS;
}

RC::eSKILL_RESULT Player::CanUseSkillItem(SLOTCODE SkillCode, DWORD TargetKey, BOOL bCoolTimerReset)
{
    SkillScriptInfo* pBaseSkillInfo = (SkillScriptInfo *)SkillInfoParser::Instance()->GetSkillInfo(SkillCode);
    if(!pBaseSkillInfo)
        return RC::RC_SKILL_BASEINFO_NOTEXIST;

    RC::eSKILL_RESULT rcResult = Character::CanUseSkillItem(SkillCode);
    if(rcResult != RC::RC_SKILL_SUCCESS)  
        return rcResult;

    // 요구레벨을 체크한다.
    if(FALSE == CheckRequireLv(pBaseSkillInfo))               
        return RC::RC_SKILL_REQUIRE_LEVEL_LIMIT;     

    return CanUseSkilltoTarget(pBaseSkillInfo, TargetKey);
}

BOOL Player::CheckNullSkillSlot(SCSkillSlot* pCurSkillSlot, SLOTCODE SkillCode)
{
    if(!pCurSkillSlot)
    {
        SUNLOG(eMIDDLE_LOG, "[Player::CanUseSkill] SkillCode[%d] The Player doesn't have the skill!", SkillCode);
        return FALSE;
    }

    return TRUE;
}

// 요구레벨을 체크한다.
BOOL Player::CheckRequireLv(SkillScriptInfo* pBaseSkillInfo)
{
    if(GetLevel() < pBaseSkillInfo->m_wRequireLv)
    {
        SUNLOG(eFULL_LOG, "Player : [CanUseSkill] SkillCode[%d] The Level of Player doesn't reach RequireLevel!", pBaseSkillInfo->m_SkillCode);
        return FALSE;
    }

    return TRUE;
}

BOOL Player::CheckRequireSkillStat(SkillScriptInfo* pBaseSkillInfo)
{
    // 요구 숙련도1 체크
    if(pBaseSkillInfo->m_wRequireSkillStat[0] && GetExperty1() < pBaseSkillInfo->m_wRequireSkillStat[0])
    {
        SUNLOG(eFULL_LOG, "Player : [CanUseSkill] SkillCode[%d] The SkillStat1 of Player doesn't reach RequireSkillStat1!", pBaseSkillInfo->m_SkillCode);
        return FALSE;
    }

    // 요구 숙련도2 체크
    if(pBaseSkillInfo->m_wRequireSkillStat[1] && GetExperty2() < pBaseSkillInfo->m_wRequireSkillStat[1])
    {
        SUNLOG(eFULL_LOG, "Player : [CanUseSkill] SkillCode[%d] The SkillStat2 of Player doesn't reach RequireSkillStat2!", pBaseSkillInfo->m_SkillCode);
        return FALSE;
    }

    return TRUE;
}

bool Player::CheckWeaponDefine(const SkillScriptInfo* const skill_script_info) const
{
    if (skill_script_info->m_iWeaponDefine[0] == -1)
    {
        return true;
    }
        
    const DWORD weapon_kind = GetWeaponKind();

#ifdef _NA_004530_20120315_1202_SKILL_RENEWAL
#else
    // 무기가 없으면 스킬사용 불가
    if (weapon_kind == eWEAPONTYPE_PUNCH)
    {
        SUNLOG(
            eCRITICAL_LOG, 
            _T("|%s|%d|무기 제한 타입이 올바르지 않습니다.|SkillCode = %d, WeaponType = %d|"), 
            __FUNCTION__, 
            __LINE__, 
            skill_script_info->m_SkillCode, 
            weapon_kind
        );
        return false;
    }
#endif

    // 무기 제한조건 4개를 체크한다.
    for (int i = 0; i < 4; ++i)
    {
        if (weapon_kind == skill_script_info->m_iWeaponDefine[i])
        {
            return true;
        }
    }
    
    SUNLOG(
        eCRITICAL_LOG, 
        _T("|%s|%d|무기 제한 타입이 올바르지 않습니다.|SkillCode = %d, WeaponType = %d|"), 
        __FUNCTION__, 
        __LINE__, 
        skill_script_info->m_SkillCode, 
        weapon_kind
    );
    return false;
}

// UNDONE | Player::CheckSkillCondition <- each loop check... by update passive skill
RC::eSKILL_RESULT Player::CheckSkillCondition(const SLOTCODE skill_code) const
{
    const SkillScriptInfo* const skill_info = SkillInfoParser::Instance()->GetSkillInfo(skill_code);
    if (skill_info == NULL)
    {
        RC::RC_SKILL_BASEINFO_NOTEXIST;
    }
    
#if !defined(_NA001605_EP2_ATTRIBUTES_RENEWAL_)
    // HP 소진
    {
        const BASE_ABILITYINFO* const ability_info = skill_info->GetAbilityInfo(eABILITY_EXHAUST_HP);
        if (ability_info != NULL)
        {
            const int value_type = ability_info->m_iParam[0];
            const int ratio = ability_info->m_iParam[1];

            int calc_hp = 0;
            if (value_type == VALUE_TYPE_PERCENT_PER_MAX)
            {
                calc_hp = GetMaxHP() * ratio / 1000;
            }
            else if(value_type == VALUE_TYPE_PERCENT_PER_CUR)
            {
                calc_hp = GetHP() * ratio / 1000;
            }
                
            if (calc_hp <= 0)
            {
                return RC::RC_SKILL_HPMP_RUN_SHORT;
            }                
        }
    }
    // HP 저하시 공격력, 방어력 증감
    {
        const BASE_ABILITYINFO* ability_info = 
            skill_info->GetAbilityInfo(eABILITY_LOWHP_ATTACKPOWER_CHANGE);
        if (ability_info == NULL)
        {
            ability_info = skill_info->GetAbilityInfo(eABILITY_LOWHP_DEFENSE_CHANGE);
        }
        if (ability_info != NULL)
        {
            const float req_hp_ratio = ability_info->m_iOption2 / 1000.f;
            float hp_ratio = 0;
            if (GetMaxHP())
            {
                hp_ratio = (float)GetHP() / GetMaxHP();
            }
            if (hp_ratio > req_hp_ratio)
            {
                return RC::RC_SKILL_HPMP_RUN_SHORT;
            }
        }
    }    
#endif    
    // 투기 개수 증가
#ifdef _NA_008480_20150909_MODIFY_FIGHTING_ENERGY
#else
    {
        const BASE_ABILITYINFO* const ability_info = 
            skill_info->GetAbilityInfo(eABILITY_FIGHTING_ENERGY_NUM_INCREASE);
        if (ability_info != NULL)
        {
            if (GetFightingEnergy() >= MAX_FIGHTING_ENERGY_NUM)
            {
                return RC::RC_SKILL_FIGHTING_ENERGY_FULL;
            }
        }
    }
#endif //_NA_008480_20150909_MODIFY_FIGHTING_ENERGY
    // SP 당 추가 공격력
    {
        const BASE_ABILITYINFO* const ability_info = 
            skill_info->GetAbilityInfo(eABILITY_BONUS_DAMAGE_PER_SP);
        if (ability_info != NULL)
        {
            const DWORD reqire_mp = GetMaxMP() * ability_info->m_iOption2 / 1000;
            if (GetMP() < reqire_mp)
            {
                return RC::RC_SKILL_HPMP_RUN_SHORT;
            }
        }
    }
    // 아드레날린
    {
        const BASE_ABILITYINFO* const ability_info = skill_info->GetAbilityInfo(eABILITY_ADRENALINE);
        if (ability_info != NULL)
        {
            const SummonedNPCs* const summoned_npcs = 
                SummonManager::Instance()->FindSummonNPCs(GetObjectKey());
            if (summoned_npcs == NULL || (summoned_npcs->GetNumberOfNPCs() <= 0))
            {
                return RC::RC_SKILL_SUMMONED_NOTEXIST;
            }
        }
    }
#ifdef _NA_007123_20140324_SHADOW_SKILL_RENEWAL
#else
    // 하이드
    {
        const BASE_ABILITYINFO* const ability_info = skill_info->GetAbilityInfo(eABILITY_HIDE);
        if (ability_info != NULL)
        {
            const BaseStatus* const battle_state = GetStatusManager()->FindStatus(eCHAR_STATE_BATTLE);
            if (battle_state != NULL)
            {
                return RC::RC_SKILL_CAN_NOT_STATE_HIDE;     
            }
#ifdef _NA_005028_20120627_MODIFY_IMPOSSIBLE_CAST_HIDE_SKILL_IN_SSQ
            const GameZone* const game_zone = GetGameZonePtr();
            if (game_zone != NULL)
            {
                const sPOLICY_ZONE& policy = game_zone->Policyment;
                if (!!(policy.FIELD & policy.ENABLE_SSQ_CONTROL)) 
                {
                    return RC::RC_SKILL_FAILED;
                }
            }
#endif  // _NA_005028_20120627_MODIFY_IMPOSSIBLE_CAST_HIDE_SKILL_IN_SSQ
        }
    }
#endif //_NA_007123_2014_0324_SHADOW_SKILL_RENEWAL
    // 오오라
    ;{
        const BASE_ABILITYINFO* const ability_info = skill_info->GetAbilityInfo(eABILITY_AURORA);
        if (ability_info != NULL)
        {
            const BaseStatus* const aurora_state = GetStatusManager()->FindAuroraStatus();
            if (aurora_state != NULL && aurora_state->IsAbilityStatus())
            {
                AbilityStatus* ability_state = (AbilityStatus*)aurora_state;
                SLOTCODE skill_code = ability_state->GetSkillCode();

                if(false == CheckCoolTimer(skill_code)) {
                    return RC::RC_SKILL_COOLTIME_ERROR;
                }
            }
        }
    };

#ifdef _NA_007667_20141001_WITCHBLADE_SKILL
    ;{
        // 스킬사용 조건 검사용 어빌리티 확인
        const BASE_ABILITYINFO* condition_ability = skill_info->GetAbilityInfo(eABILITY_SKILL_CONDITION);
        if (condition_ability != NULL)
        {
            int skill_condition = condition_ability->m_iOption2;
            int value_type = condition_ability->m_iParam[0];
            int condition_value = condition_ability->m_iParam[1];
            RC::eSKILL_RESULT result = RC::RC_SKILL_SUCCESS;
            
            switch (skill_condition)
            {
            case eSKILL_CONDITION_BELOW_HP:
                {
                    if (value_type == VALUE_TYPE_VALUE)
                    {
                        if (GetHP() > condition_value) {
                            result = RC::RC_SKILL_FAILED;
                        }
                    }
                    else if (value_type == VALUE_TYPE_PERCENT_PER_MAX) 
                    {
                        if ((GetHPRatio() * 1000.0f) > condition_value) {
                            result = RC::RC_SKILL_FAILED;
                        }
                    }
                } break;
            }

            return result;
        }
    };
#endif //_NA_007667_20141001_WITCHBLADE_SKILL
    
    return RC::RC_SKILL_SUCCESS;
}

// CHANGES, f100824.7L
bool Player::CanApplyPassiveSkill(SLOTCODE SkillCode)
{
    SkillScriptInfo* pBaseSkillInfo = (SkillScriptInfo *)SkillInfoParser::Instance()->GetSkillInfo(SkillCode);

    // 패시브 스킬인지 체크한다.
    if(pBaseSkillInfo->m_bySkillType != SKILL_TYPE_PASSIVE)   return FALSE;

    // 스킬을 사용할 수 있는 캐릭터인지 체크한다.
    if(!CheckClassDefine(SkillCode, FALSE))
    {
//      SUNLOG(eFULL_LOG,  "Player : [CanApplyPassiveSkill] The Type Of Character is Invalid! ");
        return FALSE;
    }

    // 무기 제한
    if(pBaseSkillInfo->m_iWeaponDefine[0] != -1)
    {
        DWORD dwCurWeaponKind = GetWeaponKind();

#if !defined(_NA_004965_20120613_NEW_CHARACTER_MYSTIC)
        // 무기가 없으면 스킬사용 불가
        if(dwCurWeaponKind == eWEAPONTYPE_PUNCH)
        {
//          SUNLOG(eFULL_LOG,  "Player : [CanApplyPassiveSkill] The Weapon Type is Invalid! ");
            return FALSE;
        }
#endif //_NA_004965_20120613_NEW_CHARACTER_MYSTIC

        // 무기 제한조건 4개를 체크한다.
        BOOL bSuccess = FALSE;
        for(int i = 0; i < 4; ++i)
        {
            if(dwCurWeaponKind == pBaseSkillInfo->m_iWeaponDefine[i])
            {
                bSuccess = TRUE;
                break;
            }
        }
        if(!bSuccess)
        {
//          SUNLOG(eFULL_LOG,  "Player : [CanApplyPassiveSkill] The Weapon Type is Invalid! ");
            return FALSE;
        }
    }

    // 요구레벨을 체크한다.
    if(GetLevel() < pBaseSkillInfo->m_wRequireLv)
    {
//      SUNLOG(eFULL_LOG,  "Player : [CanUseSkill] The Level of Player doesn't reach RequireLevel! ");
        return FALSE;
    }

    // 요구 숙련도1 체크
    if(pBaseSkillInfo->m_wRequireSkillStat[0] && GetExperty1() < pBaseSkillInfo->m_wRequireSkillStat[0])
    {
//      SUNLOG(eFULL_LOG,  "Player : [CanUseSkill] The SkillStat1 of Player doesn't reach RequireSkillStat1! ");
        return FALSE;
    }

    // 요구 숙련도2 체크
    if(pBaseSkillInfo->m_wRequireSkillStat[1] && GetExperty2() < pBaseSkillInfo->m_wRequireSkillStat[1])
    {
//      SUNLOG(eFULL_LOG,  "Player : [CanUseSkill] The SkillStat2 of Player doesn't reach RequireSkillStat2! ");
        return FALSE;
    }

    // 스킬, 어빌리티 별로 조건 체크
    RC::eSKILL_RESULT rcResult = CheckSkillCondition(SkillCode);
    if(rcResult != RC::RC_SKILL_SUCCESS)  return FALSE;

    return TRUE;
}

//-------------------------------------------------------------------------------------------------
// 스킬, 스타일 습득
//-------------------------------------------------------------------------------------------------
SLOTCODE Player::GetDefaultStyle(const CODETYPE weapon_code) const
{
    if (weapon_code == 0)
    {
        return GetPunchStyle();
    }

    const BASE_ITEMINFO* const weapon_info = ItemInfoParser::Instance()->GetItemInfo(weapon_code);
    if (weapon_info == NULL)
    {
        return GetPunchStyle();
    }
    
    switch(weapon_info->m_wType)
    {   
    case eITEMTYPE_TWOHANDSWORD:        
        return eSTYLE_TWOHANDSWORD_NORMAL;
    case eITEMTYPE_TWOHANDAXE:          
        return eSTYLE_TWOHANDAXE_NORMAL;
    case eITEMTYPE_ONEHANDSWORD:        
        return eSTYLE_ONEHANDSWORD_NORMAL; 
    case eITEMTYPE_SPEAR:               
        return eSTYLE_SPEAR_NORMAL;
    case eITEMTYPE_DAGGER:              
        return eSTYLE_SHADOW_DAGGER_NORMAL;
    case eITEMTYPE_WHIP:                
        return eSTYLE_SHADOW_WHIP_NORMAL;
    case eITEMTYPE_STAFF:               
        return eSTYLE_STAFF_NORMAL;
    case eITEMTYPE_ORB:                 
        return eSTYLE_ORB_NORMAL; 
    case eITEMTYPE_ONEHANDCROSSBOW:     
        return eSTYLE_ONEHANDCROSSBOW_NORMAL;
    case eITEMTYPE_ETHERWEAPON:         
        return eSTYLE_ETHER_NORMAL; 
    case eITEMTYPE_GUNTLET: // _NA_004965_20120613_NEW_CHARACTER_MYSTIC
        return eSTYLE_MYSTIC_GUNTLET_NORMAL;
    case eITEMTYPE_POLEARM: // _NA_004965_20120613_NEW_CHARACTER_MYSTIC
        return eSTYLE_MYSTIC_POLEARM_NORMAL;
    case eITEMTYPE_HELLROID_1: //_NA_000000_20130812_NEW_CHARACTER_HELLROID
        return eSTYLE_HELLROID_WEAPON1;
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    case eITEMTYPE_ARCBLADE:
        return eSTYLE_WITCHBLADE_ARCBLADE;
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    default:                            
        return GetPunchStyle(); 
    }
}

SLOTCODE Player::GetPunchStyle() const
{
    switch(GetCharType())
    {
    case eCHAR_BERSERKER:       
        return eSTYLE_BERSERKER_PUNCH;
    case eCHAR_DRAGON:          
        return eSTYLE_DRAGON_PUNCH;
    case eCHAR_SHADOW:          
        return eSTYLE_SHADOW_PUNCH;
    case eCHAR_VALKYRIE:        
        return eSTYLE_VALKYRIE_PUNCH;
    case eCHAR_ELEMENTALIST:    
        return eSTYLE_MAGICIAN_PUNCH;
    case eCHAR_MYSTIC: // _NA_004965_20120613_NEW_CHARACTER_MYSTIC
        return eSTYLE_MYSTIC_PUNCH;
    case eCHAR_HELLROID: //_NA_000000_20130812_NEW_CHARACTER_HELLROID
        return eSTYLE_HELLROID_PUNCH;
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    case eCHAR_WITCHBLADE:
        return eSTYLE_WITCHBLADE_PUNCH;
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

    default:                    
        ASSERT(!"Player : [GetPunchStyle] Invalid CharType!");    
        return eSTYLE_BERSERKER_PUNCH;
    }
}

BOOL    Player::CheckClassDefine(SLOTCODE SkillCode, BOOL bUseActiveSkill)
{
    SkillScriptInfo* pBaseSkillInfo = (SkillScriptInfo *)SkillInfoParser::Instance()->GetSkillInfo(SkillCode);
    if(!(pBaseSkillInfo && (eSKILL_USER_PLAYER == pBaseSkillInfo->m_bySkillUserType)))
        return FALSE;

    //eCHAR_TYPE eCurType = GetCharType();
    //eCHAR_TYPE eCheckType = (eCHAR_TYPE)pBaseSkillInfo->m_dwClassDefine;
    BASE_PLAYERINFO& rPlayerInfo = *GetCharInfo();
    const eCHAREX_TYPE charExType = (eCHAREX_TYPE)(BYTE)rPlayerInfo.m_ChangeOfClassStep;
    const DWORD classDefine = pBaseSkillInfo->m_dwClassDefine;
    const BOOLEAN isMatched = charExType == classDefine;

    if(isMatched)
        return TRUE;

#ifdef _NA_0_20120104_DRAGON_COMMON_SKILL
    if (GetCharType() == eCHAR_DRAGON)
    {
        if (bUseActiveSkill)
        {
            // 변신중 액티브 스킬 사용시
            StatusManager* const pStatusManager = GetStatusManager();
            if(pStatusManager->Status.IsDragonTransforming())
            {
                DragonTransStatus* pDragonTransStatus = (DragonTransStatus*)GetStatusManager()->FindStatus(eCHAR_STATE_TRANSFORMATION);
                const DWORD transformClass = (DWORD)pDragonTransStatus->GetCharType();
                switch (transformClass)
                {
                case eTRANSFORM_TYPE_DRAGON1:
                    {
                        switch (classDefine)
                        {
                        case eTRANSFORM_TYPE_DRAGON1:
                        case eDRAGON_COMMON_SKILL_1stTRANS:
                            return TRUE;
                        case eCHAREX_DRAGON_1stTRANS_100LEVEL:
                            return (charExType == eCHAREX_DRAGON_KNIGHT_100LEVEL) ? TRUE : FALSE;
                        }
                    }
                    break;
                case eTRANSFORM_TYPE_DRAGON2:
                    {
                        switch (classDefine)
                        {
                        case eTRANSFORM_TYPE_DRAGON2:
                        case eDRAGON_COMMON_SKILL_2ndTRANS:
                            return TRUE;
                        case eCHAREX_DRAGON_2ndTRANS_100LEVEL:
                            return (charExType == eCHAREX_DRAGON_KNIGHT_100LEVEL) ? TRUE : FALSE;
                        }
                    }
                    break;
                }
            }
            else
            {
                switch(classDefine)
                {
                case eCHAR_DRAGON:
                case eDRAGON_COMMON_SKILL_1stTRANS:
                case eDRAGON_COMMON_SKILL_2ndTRANS:
                    return TRUE;    // (전직/체인지업 여부를 떠나서) 이전스킬 포괄해야 한다.
                }
            }
        }
        else
        {
            switch(classDefine)
            {
            case eCHAR_DRAGON:
            case eCHAREX_DRAGON_1stTRANS: 
            case eCHAREX_DRAGON_2ndTRANS:
            case eDRAGON_COMMON_SKILL_1stTRANS:
            case eDRAGON_COMMON_SKILL_2ndTRANS:
                return TRUE;    // (전직/체인지업 여부를 떠나서) 이전스킬 포괄해야 한다.
            case eCHAREX_DRAGON_1stTRANS_100LEVEL: 
            case eCHAREX_DRAGON_2ndTRANS_100LEVEL:
                return (charExType == eCHAREX_DRAGON_KNIGHT_100LEVEL) ? TRUE : FALSE;
            }
        }
        return FALSE;
    }
#endif

    // irregular process for DRAGON & DRAGON TRANSFORMATION
    switch(charExType)
    {
    case eCHAREX_BERSERKER_100LEVEL:
    case eCHAREX_SHADOW_100LEVEL:
    case eCHAREX_VALKYRIE_100LEVEL:
    case eCHAREX_ELEMENTALIST_100LEVEL:
    case eCHAREX_MYSTIC_100LEVEL: //_NA_004965_20120613_NEW_CHARACTER_MYSTIC
    case eCHAREX_HELLROID_100LEVEL: //_NA_000000_20130812_NEW_CHARACTER_HELLROID
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    case eCHAREX_WITCHBLADE_100LEVEL:
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        return classDefine == (charExType-eCHAREX_100LEVEL_CHANGEUP_CONSTANT);

#if !defined(_NA_0_20120104_DRAGON_COMMON_SKILL)
    case eCHAR_DRAGON:
    case eCHAREX_DRAGON_KNIGHT_100LEVEL:
        {
            // 드래곤 변신인 경우 액티브스킬사용은 제한하지만, 패시브스킬 사용이나 스킬 습득은 허용한다.
            if(bUseActiveSkill)
            {
                StatusManager* const pStatusManager = GetStatusManager();
                if(pStatusManager->Status.IsDragonTransforming())
                {
                    DragonTransStatus* pDragonTransStatus = (DragonTransStatus*)GetStatusManager()->FindStatus(eCHAR_STATE_TRANSFORMATION);
                    const DWORD transformClass = (DWORD)pDragonTransStatus->GetCharType();
                    return
                        (classDefine == transformClass)   // 기본 비교시
                        || (classDefine == transformClass+10)   // 100레벨 체인지업시
                        //|| (classDefine == transformClass+20)   // 다음 전직 비교시
                        ;
                }
            }

            switch(classDefine)
            {
            case eCHAR_DRAGON:
            case eCHAREX_DRAGON_1stTRANS: 
            case eCHAREX_DRAGON_2ndTRANS:
                return TRUE;    // (전직/체인지업 여부를 떠나서) 이전스킬 포괄해야 한다.
            case eCHAREX_DRAGON_1stTRANS_100LEVEL: 
            case eCHAREX_DRAGON_2ndTRANS_100LEVEL:
                return eCHAREX_DRAGON_KNIGHT_100LEVEL == charExType;
            }
        }
        break;
#endif //#if !defined(_NA_0_20120104_DRAGON_COMMON_SKILL)
    }
    return FALSE;
}


RC::eSKILL_RESULT Player::UseSkill(SkillInfo* skill_info_data,  BYTE byAttackPropensity)
{
    if (skill_info_data == NULL) {
        return RC::RC_SKILL_FAILED;
    };
    //
    GameField* pField = GetField();
    if (!pField || !pField->IsReady()) {
        return RC::RC_SKILL_FAILED;
    };
    //
    if (skill_info_data->skill_code_ == 0) {
        return RC::RC_SKILL_FAILED;
    };
    //
#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
    if (pField->GetGameZone()->GetZoneType() == eZONETYPE_SPA_FIELD)
    {
        return RC::RC_SKILL_CANNOT_USE_SPA;
    }
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM
    //
    ;{  // (NOTE) to support a shadow control, __NA_S00015_20080828_NEW_HOPE_SHADOW
    // 이부분 따로 함수로 빼자.
    //////////////////////////////////////////////////////////////////////////
    BaseStatus* pStatus = NULL;

    // 현재 사용자가 투명 상태이면 투명 상태를 해제 한다.
    StatusManager* pStatusManager = GetStatusManager();

    pStatus = pStatusManager->FindStatus(eCHAR_STATE_HP_SUCTION);
    if (pStatus)
        pStatus->StopStatus();

    pStatus = pStatusManager->FindStatus(eCHAR_STATE_MP_SUCTION);
    if (pStatus)
        pStatus->StopStatus();

#if !defined(_NA_005159_20120807_DISABLE_REMOVE_RUSH_STATE)
    pStatus = pStatusManager->FindStatus(eCHAR_STATE_RUSH);
    if (pStatus)
        pStatus->StopStatus();
#endif //_NA_005159_20120807_DISABLE_REMOVE_RUSH_STATE
    //////////////////////////////////////////////////////////////////////////
    };

    Character* pTargetChar = pField->FindCharacter(skill_info_data->main_target_key_);

    if (!pTargetChar)
        return RC::RC_SKILL_TARGET_NOTEXIST;

    RC::eSKILL_RESULT rcResult = RC::RC_SKILL_SUCCESS;

    const BOOLEAN bApplyGeneralForcedAtkRule = !(Policyment.FIELD & Policyment.PCC_IN_SSQ_FIELD);
    if (bApplyGeneralForcedAtkRule)
    {
        bool befForceAttack = IsForceAttack();
        SetForceAttack(skill_info_data->attack_propensity_ ==ATTACK_PROPENSITY_FORCE?true:false);

        RC::eSKILL_RESULT rcResult = ChkSkillPvP(pTargetChar);
        if (rcResult != RC::RC_SKILL_SUCCESS)
        {
            SetForceAttack(befForceAttack);
            return rcResult;
        }
    }

    // 좌표가 유효한지 체크
    if (FALSE == finite(skill_info_data->cur_pos_))
    {
        SUNLOG(eCRITICAL_LOG,  "SkillAction :: CurPos is Invalid! x[%f] y[%f] z[%f]", skill_info_data->cur_pos_.x, skill_info_data->cur_pos_.y, skill_info_data->cur_pos_.z);
        return RC::RC_SKILL_POSITION_INVALID;
    }
    if (FALSE == finite(skill_info_data->dest_pos_))
    {
        SUNLOG(eCRITICAL_LOG,  "SkillAction :: DestPos is Invalid! x[%f] y[%f] z[%f]", skill_info_data->dest_pos_.x, skill_info_data->dest_pos_.y, skill_info_data->dest_pos_.z);
        return RC::RC_SKILL_POSITION_INVALID;
    }

    // 클라와 서버간에 위치 보정
    if (SKILL_FACTOR_NOMAL == skill_info_data->skill_factor_)
    {
        ModifyPosition(skill_info_data);
        //ModifyPosition(TRUE, wvCurPos, wvDestPos, skill_info_data->m_wvCurPos, skill_info_data->m_wvDestPos, skill_info_data->m_SkillCode);

#ifdef _NA_007667_20141001_WITCHBLADE_SKILL
        rcResult = CanUseSkill(skill_info_data, skill_info_data->main_target_key_);
#else
        rcResult = CanUseSkill(skill_info_data->skill_code_, skill_info_data->main_target_key_);
#endif //_NA_007667_20141001_WITCHBLADE_SKILL
    }
    else
    {
        rcResult = CanUseSkillItem(skill_info_data->skill_code_, skill_info_data->main_target_key_);
    }

    if (rcResult != RC::RC_SKILL_SUCCESS)
        return rcResult;

    m_pSkillManager->RegisterSkill(SKILL_TYPE_ACTIVE, skill_info_data);
    //__NA_S00028_SHADOW_ADD_WORK
    //현재 스킬의 사용대상이 적이면, 전투상태를 갱신한다.
    assert(skill_info_data->root_skill_info_->IsSkill());
    const SkillScriptInfo* base_skill_info = \
        static_cast<const SkillScriptInfo*>(skill_info_data->root_skill_info_);
    if (base_skill_info)
    {
        if (base_skill_info->m_byTarget == SKILL_TARGET_ENEMY || 
            base_skill_info->m_byTarget == SKILL_TARGET_AREA ||
            base_skill_info->m_byTarget == SKILL_TARGET_REACHABLE_ENEMY ||
            base_skill_info->m_byTarget == SKILL_TARGET_ENEMY_PLAYER)
        {
            GetStatusManager()->UpdateExpiredTime(eCHAR_STATE_BATTLE, STATE_BATTLE_TIME);
        }
    }

    return rcResult;
}


// m_byMOVE_SYNC_FAIL_DISTANCE값을 토대로 간단하게 위치 동기화를 한다.
// 채집, 탐색..
BOOL Player::CorrectPosition( WzVector& wvClientCurPos, WzVector& vResultCurPos )
{
    if( FALSE == Character::finite(wvClientCurPos) )
        return FALSE;

    // 체크할 좌표 위치 보정
    WzVector vServerPos;	
    GetPos( &vServerPos );
    FLOAT fCurPosDiff = Math_Vector2DLength( vServerPos, wvClientCurPos );

    SERVER_COMMON_OPTION& stServerOptionInfo = ServerOptionParserEx::Instance()->GetServerOption();
    if( fCurPosDiff >= stServerOptionInfo.m_byMOVE_SYNC_FAIL_DISTANCE )
    {
        vResultCurPos = vServerPos;
    }
    else
    {
        vResultCurPos = wvClientCurPos;
        SetPos( &wvClientCurPos );
        MovementChecker* pCHECKER = GetMovementChecker();
        MovementChecker::sPATH_INFO* pPathInfo = pCHECKER->GetServerPathExpUpdate();
        pPathInfo->SetupLatestState( pCHECKER, .0f, 0, wvClientCurPos, wvClientCurPos );
    }

    return TRUE;
}

#ifdef _NA_007667_20141001_WITCHBLADE_SKILL
void Player::ProcessRisingForce()
{
    if (GetCharType() != eCHAR_WITCHBLADE) {
        return;
    }
    if (IsDead() == TRUE || IsDeleted() == true) {
        return ;
    }

    SkillSlotManager* skillslot_manager = this->GetSkillSlotManager();
    SLOTCODE skill_code = skillslot_manager->FindSkill(eSKILL_RISINGFORCE);

    if (skill_code != 0)
    {
        //조건검사
        if (CheckCoolTimer(skill_code) != true || CheckSkillCondition(skill_code) != RC::RC_SKILL_SUCCESS) {
            return;
        }
        //스킬예약
        SkillManager* skill_manager = this->GetSkillManager();
        skill_manager->SetAutoCastSkill(skill_code, false);
    }
   
}
#endif //_NA_007667_20141001_WITCHBLADE_SKILL