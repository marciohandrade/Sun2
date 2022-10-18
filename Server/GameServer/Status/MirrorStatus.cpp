#include "stdafx.h"
#include "Ability.h"
#include "MirrorStatus.h"
#include "AbilityFactory.h"
#include "StatusManager.h"
#include "MovingPlanner.h"
#include "StateInfoParser.h"
#include "SkillManager.h"

VOID MirrorStatus::Init( Character *pOwner, Character *pAttacker, Ability *pAbility )
{
	AbilityStatus::Init( pOwner, pAttacker, pAbility );

	const BASE_ABILITYINFO *pBaseAbilityInfo = pAbility->GetBaseAbilityInfo();
	m_fAbsorbRatio = pBaseAbilityInfo->m_iOption1 / 1000.0f;
	m_iOption2 = pBaseAbilityInfo->m_iOption2;

#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    m_LinkSkillCoolTime = 0;
    if (GetAbilityID() == eABILITY_REFLECT_LINK_SKILL)
    {
        const ROOT_SKILLINFO* root_skill_info = SkillInfoParser::Instance()->GetInfo(m_iOption2);
        if (root_skill_info->IsStyle() == false) 
        {
            const SkillScriptInfo* skill_info = static_cast<const SkillScriptInfo*>(root_skill_info);
            m_LinkSkillCoolTime = skill_info->m_dwCoolTime;
        }
    }
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
}

VOID MirrorStatus::DamageMirror( Character *pAttacker, DAMAGETYPE wDamage )
{
    Character* const owner = GetOwner();
    if (owner == NULL)
    {
        return;
    }

    //                  (f100414.2L) add field condition checker
    if (pAttacker == NULL || pAttacker->GetField() == NULL) {
        return;
    }
    // (CHANGES) (f100414.2L) add skip filter,
    // to prevent mirror damage if the attacker in a safety area that attacker is dead or \
    // is teleported to a safety area.
    // (CHANGES) (f100514.3L) add the field check condition to prevent ANY actions that
    // a player do a delayed spiking whether the player the player request a LEAVE.
    if (pAttacker->IsDead() || pAttacker->CanBeAttacked() == false) {
        return;
    }
    const nsAI::TileEvent* attacker_tile_event = 0;
    if (pAttacker->PlanEvent->GetTileEvent(&attacker_tile_event) == false ||
        (attacker_tile_event->Attr & PTA_SAFETY_ZONE))
    {
        return;
    }
    //
    MSGSUB_SKILL_REFLECT_DAMAGE_BRD BrdMsg;
    BrdMsg.Clear();
    // 데미지를 반사한다.
    int iMirrorDamage = (int)(wDamage * m_fAbsorbRatio);

    // f100908.1L
#ifdef _NA_006728_20130520_SKILL_BALANCE_MODIFY
    DamageArgs damage_args(owner, eATTACK_TYPE_MELEE, GetSkillCode());
#else
    DamageArgs damage_args(owner, eATTACK_TYPE_MELEE);
#endif // _NA_006728_20130520_SKILL_BALANCE_MODIFY
    ;{
        //damage_args.option_.is_critical_;
        damage_args.option_.is_mirror_ = true;
        //damage_args.limit_hp_;
        damage_args.attack_regist_kind_ = eATTACK_RESIST_SKILL;
        BASE_STATEINFO* pStateInfo = StateInfoParser::Instance()->GetStateInfo(GetStateID());
        if (pStateInfo) {
            damage_args.sd_apply_ = (eSD_APPLY)pStateInfo->m_bySDApply;
        }
        damage_args.damage_ = iMirrorDamage;
        //damage_args.damage_sd_;
    };
    // (CHANGES) fix up damaged owner that m_pOwner changes to pAttacker,
    // attacker have reflected his attack damage

    pAttacker->Damaged(&damage_args);

	BrdMsg.m_dwAttackerKey = owner->GetObjectKey();
	BrdMsg.m_dwTargetKey = pAttacker->GetObjectKey();
    //_NA_0_20100211_SD_EFFECT_MODIFY
    BrdMsg.m_wDamage = damage_args.damage_;
	BrdMsg.m_dwTargetHP = pAttacker->GetHP();
    // (f100708.2L) add a missing data which is related to 'ReserveHP'
    BrdMsg.m_dwTargetResrveHP = pAttacker->GetReserveHP();
	BrdMsg.m_SkillCode = 0;
	BrdMsg.m_wAbilityCode = 0;

    // (CHANGES) (f100513.1L) remove a send logic
	// 피격 데미지 반사일 경우 상태이상은 걸리지 않는다.
    const eABILITY ability_id = GetAbilityID();
	if( ability_id == eABILITY_REFLECT_DAMAGE )	
	{
        // (CHANGES) (f100513.1L) (WAVERIX) move a send logic to at this point
        // to prevent other abilities have been send twice.
        owner->SendPacketAroundForSync(&BrdMsg);
		return;
	}

#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    if (ability_id == eABILITY_REFLECT_LINK_SKILL && m_iOption2 != 0)
    {
        owner->SendPacketAroundForSync(&BrdMsg);

        // 링크된 스킬을 등록한다
        _RegistLinkSkill(pAttacker);
        return;
    }
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

	// SkillInfo 초기화
	memset( &m_SkillInfoData, 0, sizeof(SkillInfo) );

	// BaseAbilityInfo 셋팅
	m_BaseAbilityInfo.m_byRangeType = 1;		m_BaseAbilityInfo.m_wSuccessRate = 1000;	
	m_BaseAbilityInfo.m_iOption1 = 0;			m_BaseAbilityInfo.m_iOption2 = 0;		
	m_BaseAbilityInfo.m_iParam[0] = m_iOption2;	
    m_BaseAbilityInfo.m_iParam[1] = GetAbilityValueType();
	m_BaseAbilityInfo.m_iParam[2] = GetAbilityValue();	
    m_BaseAbilityInfo.m_iParam[3] = 0;

	switch( ability_id )
	{
	case eABILITY_REFLECT_STUN:		// 피격 스턴
		m_BaseAbilityInfo.m_wAbilityID = eABILITY_ABNORMAL_STATUS;
		m_BaseAbilityInfo.m_wStateID = eCHAR_STATE_STUN;
		break;

	case eABILITY_REFLECT_FEAR:		// 피격 피어
		m_BaseAbilityInfo.m_wAbilityID = eABILITY_FEAR;
		m_BaseAbilityInfo.m_wStateID = eCHAR_STATE_FEAR;
		break;

	case eABILITY_REFLECT_FROZEN:	// 피격 프로즌
		m_BaseAbilityInfo.m_wAbilityID = eABILITY_ABNORMAL_STATUS;
		m_BaseAbilityInfo.m_wStateID = eCHAR_STATE_FROZEN;
		break;

	case eABILITY_REFLECT_SLOW:		// 피격 족쇄
		m_BaseAbilityInfo.m_wAbilityID = eABILITY_MOVE_SPEED_INCREASE;
		m_BaseAbilityInfo.m_wStateID = eCHAR_STATE_FETTER;
		break;

	case eABILITY_REFLECT_SLOWDOWN:	// 피격 둔화
		m_BaseAbilityInfo.m_wAbilityID = eABILITY_PHYSICAL_SPEED_INCREASE;
		m_BaseAbilityInfo.m_wStateID = eCHAR_STATE_SLOWDOWN;
		break;
	default:
		SUNLOG( eCRITICAL_LOG, "[MirrorStatus::DamageMirror]ABILITYID IS WRONG!![%d]\n", ability_id);
		return;
	}

#ifdef _NA_008766_20160407_PROTECTIONSTATUS_MODIFICATION_ANOMALIES
	if(pAttacker->GetStatusManager()->FindStatus(eCHAR_STATE_PROTECTION) || pAttacker->GetStatusManager()->FindStatus(eCHAR_STATE_PROTECTION1))
	{
		return;
	}
#endif //(_NA_008766_20160407_PROTECTIONSTATUS_MODIFICATION_ANOMALIES)

	// 피격어빌리티 생성
    const SLOTCODE skill_code = GetSkillCode();
	Ability	*pNewAbility = AbilityFactory::Instance()->AllocAbility( m_BaseAbilityInfo.m_wAbilityID, SKILL_TYPE_ACTIVE );
	pNewAbility->InitDetailed( owner, skill_code, &m_SkillInfoData, 0, &m_BaseAbilityInfo );

	// 이상상태 적용
	AbilityStatus *pAbilityStatus = pAttacker->GetStatusManager()->AllocAbilityStatus( owner, pNewAbility );

	if( pAbilityStatus )
	{
		BrdMsg.m_SkillCode = skill_code;
		BrdMsg.m_wAbilityCode = ability_id;
	}
    owner->SendPacketAroundForSync(&BrdMsg);

    // delete
    {
        AbilityFactory::Instance()->FreeAbility(pNewAbility);
        pNewAbility = NULL;
    }
}

#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
BOOL MirrorStatus::Update( DWORD curTime )
{
    BOOL updated = AbilityStatus::Update( curTime );
    Character* owner = GetOwner();
    if (owner == NULL || updated == FALSE || GetAbilityID() != eABILITY_REFLECT_LINK_SKILL)
    {
        return updated;
    }

    LinkSkillMap::iterator begin_iterator = m_LinkSkillMap.begin(), end_iterator = m_LinkSkillMap.end();
    while(begin_iterator != end_iterator)
    {
        LinkSkill& link_skill = begin_iterator->second;

        // 실행된 스킬이라면
        if (link_skill.execute_time != 0)
        {
            if (curTime - link_skill.execute_time >= m_LinkSkillCoolTime) // 쿨타임이 지나면 삭제하고
            {
                begin_iterator = m_LinkSkillMap.erase(begin_iterator);
            }
            else // 아닐경우 스킵
            {
                ++begin_iterator;
            }
            continue;
        }

        Character* const p_target = link_skill.target;
        if (p_target == NULL || p_target->IsDead() == true || p_target->IsDeleted() == true)
        {
            begin_iterator = m_LinkSkillMap.erase(begin_iterator);
            continue;
        }

        link_skill.execute_time = curTime;

        // 스킬 실행
        const WzVector* cur_pos = p_target->GetPosPtr();
        SkillInfo skill_info_data(owner, m_iOption2, 0);
        skill_info_data.SetArgsObjectInfo(p_target->GetObjectKey(), owner->GetPosPtr(), 
            owner->GetPosPtr(), p_target->GetPosPtr());
        skill_info_data.SetArgsSkillExt(skill_info_data.kDefaultSequence,
            skill_info_data.kDefaultSkillEffect,
            skill_info_data.kDefaultSkillDelay,
            SKILL_FACTOR_NOMAL,
            skill_info_data.kDefaultPropensity,
            skill_info_data.kDefaultEvent);
        owner->GetSkillManager()->RegisterSkill(SKILL_TYPE_ACTIVE, &skill_info_data);


        ++begin_iterator;
    }

	return updated;
}

void MirrorStatus::_RegistLinkSkill( Character *pAttacker )
{
    LinkSkillMap::iterator find_iter = m_LinkSkillMap.find(pAttacker),
        end_iter = m_LinkSkillMap.end();
    
    if (find_iter == end_iter)
    {
        LinkSkill link_skill;
        link_skill.target = pAttacker;
        link_skill.execute_time = 0;
        m_LinkSkillMap.insert(LinkSkillMap::value_type(pAttacker, link_skill));
    }
}

#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE




