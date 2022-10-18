#include "stdafx.h"
#include "SkillInfoParser.h"
#include "NPC.h"
#include "GameField.h"
#include "Character.h"
#include "Player.h"
#include "NormalAttack.h"
#include "AITypes.h"
#include "CharacterFormula.h"
#include <PacketStruct_CG.h>
#include <Battle.h>
//#include <Global.h>
#include "StatusManager.h"
#include "AIParamParser.h"
#include "EtcStatus.h"
#include "EtherBulletInfoParser.h"

void NormalAttack::SetExecuteInterval()
{
	interval_ = 0;
}

void NormalAttack::Init(Character* owner,
                        const SkillInfo* skill_info,
                        const ROOT_SKILLINFO* root_skill_info)
{
    Skill::SetupDefault(owner, skill_info, root_skill_info);
	// init data fields for a NormalAttack
	m_byAttackSequence = skill_info->attack_sequence_;
	m_pMainTarget = NULL;
    m_wDamage = 0;
    m_wMainTargetDamage = 0;
}

bool NormalAttack::FindTargets(
    ns_object::SearchOption::SingleSearch::result_type* const result)
{
    __UNUSED(result);
    assert(!"can't entered at this point");
    return false;
    //return Skill::FindTargetsDefault(result);
}


BOOL NormalAttack::StartExecute()
{
	game_field_ = owner_->GetField();
	if( !game_field_ ) 
	{
		MSG_CG_BATTLE_PLAYER_ATTACK_NAK NakMsg;
		NakMsg.m_byErrorCode = RC::RC_BATTLE_FIELD_IS_NULL;
		owner_->SendPacket( &NakMsg, sizeof(NakMsg) );
		SUNLOG( eCRITICAL_LOG, "[NormalAttack::StartExecute] GameField is NULL!" );
		return FALSE;
	}
    BattleResult skill_result = { 0, };
	// MainTarget�� ��ȿ���� üũ�Ѵ�.(���� ��Ÿ� üũ)
	if( !CheckMainTarget(&skill_result) )
	{
		// Nak ��Ŷ�� ������.
		MSG_CG_BATTLE_PLAYER_ATTACK_NAK msg;
		msg.m_byErrorCode = skill_result.result_code;
		owner_->SendPacket( &msg, sizeof(msg) );
		return FALSE;
	}

	// �������� üũ�Ѵ�.
	if( (owner_->IsFriend(m_pMainTarget) == USER_RELATION_ENEMY) && m_pMainTarget->CanBeAttacked() )
	{
		// PK �����̶�� ����ó���� �Ѵ�.

		owner_->ForcedAttack( m_pMainTarget );

		// �������� ����Ѵ�.
		if( CalcDamage( m_pMainTarget ) )
		{
			// ���� �����Ƽ�� ó���ϴ� �߿� �ڱ��ڽ��� �׾ ��ų�� �ʱ�ȭ�� ���!( ���� �߻��ϸ� �ȵ����� ����ó��! )
			if( !owner_ )
			{
				SUNLOG( eCRITICAL_LOG, "[NormalAttack::StartExecute] owner_ is NULL!" );
				return FALSE;
			}
            
			// ������ �������� ���
            SendAIMessage( m_pMainTarget, m_wDamage ); 
		}
		owner_->GetStatusManager()->UpdateExpiredTime( eCHAR_STATE_BATTLE, STATE_BATTLE_TIME );
	}

	// �����ϸ� MP, HP�� ȸ���Ǵ� ĳ���Ͱ� �ִ�.

    //_NA_0_20100211_SD_EFFECT_MODIFY
    owner_->OnAttack( m_pMainTarget, SKILLCODE_NORMAL_ATTACK, m_wDamage );

	// ��� ��Ŷ�� ������.
	Broadcast_Normal_Result();

	return TRUE;
}


BOOL NormalAttack::CheckMainTarget(BattleResult* const detailed_result)
{
    BattleResult battle_result = { 0, };
    BattleResult* result_handle = detailed_result ? detailed_result : &battle_result;
    result_handle->result_code = RC::RC_BATTLE_SUCCESS;
    //
    if (game_field_ == NULL)
    {
        SUNLOG(eCRITICAL_LOG, _T("[NormalAttack::CheckMainTarget] GameField is NULL!"));
        result_handle->result_code = RC::RC_BATTLE_INVALID_MAINTARGET;
        return false;
    }

    // ����Ÿ�� ��ǥ�� �����Ѵ�.
    m_pMainTarget = game_field_->FindCharacter(skill_info_data_.main_target_key_);
    if (m_pMainTarget == NULL) {
        result_handle->result_code = RC::RC_BATTLE_INVALID_MAINTARGET;
        return false;
    }
    m_pMainTarget->GetPos(&skill_info_data_.main_target_pos_);

    // ����Ÿ�ٿ� ���� ��Ÿ��� üũ�Ѵ�.
    //_NA_007079_BUG_WEAPON_INACTIVATE_STATE
    // CheckAttackRange�� �÷��̾� ������ ��Ÿ��� ��������, checkSkillRange�� ������ ���� ���ݻ�Ÿ��� �Ѵ�.
    // ��Ÿ�� ��ų�� ����� ������� ó���ϴµ�, ��Ÿ�� ��Ÿ�������� ��Ÿ��� �����ϰ� ���⿡�� ���� ��Ÿ���
    // �������� ���� �ϰ������� ����. ��Ÿ�� ��Ÿ�������� ��Ÿ��� ����ϵ��� �Ѵ�.
    //BOOL bSkillRangeCheck = owner_->CheckSkillRange(m_pMainTarget, skill_info_data_.main_target_pos_, 
    //                                                base_style_info_->m_fAttRange);

    BOOL bSkillRangeCheck = owner_->CheckAttackRange(m_pMainTarget, skill_info_data_.main_target_pos_);

    if (bSkillRangeCheck == false) {
        result_handle->result_code = RC::RC_BATTLE_OUT_OF_RANGE;
        return false;
    }

    return true;
}

BOOL NormalAttack::CalcDamage( Character *pTarget )
{
    // f100908.1L
#ifdef _NA_006728_20130520_SKILL_BALANCE_MODIFY
    DamageArgs damage_args(owner_, owner_->GetWeaponBaseAttType(), GetSkillCode());
#else
    DamageArgs damage_args(owner_, owner_->GetWeaponBaseAttType());
#endif // _NA_006728_20130520_SKILL_BALANCE_MODIFY

#ifdef _NA001605_EP2_ATTRIBUTES_RENEWAL_
    damage_args.damage_ = ep2::CalcNormalDamage(owner_, pTarget,
        damage_args.attack_type_, true, base_style_info_->m_wCriticalBonus,
        &(skill_info_data_.skill_effect_));
#else
    damage_args.damage_ = ep1::CalcNormalDamage(owner_, pTarget,
        owner_->GetWeaponBaseAttType(), owner_->GetWeaponMagicalAttType(),
        base_style_info_->m_wCriticalBonus, &(skill_info_data_.skill_effect_));
#endif
    //  [2/16/2010 lst1024]  �ڵ� ����
    const int   iAddDamage     = base_style_info_->m_iAddDamage[m_byAttackSequence];
    const float fDamagePercent = base_style_info_->m_fDamagePercent[m_byAttackSequence];

    damage_args.damage_ = static_cast<DAMAGETYPE>(damage_args.damage_ * (1 + fDamagePercent)) + iAddDamage;

    ;{
        damage_args.option_.is_critical_ = !!(skill_info_data_.skill_effect_ & SKILL_EFFECT_CRITICAL);
        //damage_args.option_.is_mirror_;
        //damage_args.limit_hp_;
        //damage_args.attack_regist_kind_;
        damage_args.sd_apply_ = eSD_APPLY_DO;
        //damage_args.damage_;
        //damage_args.damage_sd_;
    };
    ;{
        SkillExecutionTransaction transaction; // CHANGES: f110303.3L
        pTarget->Damaged(&damage_args);
    };
    m_wDamage = damage_args.damage_;

	if( is_applied_ether_bullet_option_ )
	{
		if( applied_ether_bullet_info_ )
		{
			skill_info_data_.skill_effect_ |= applied_ether_bullet_info_->m_dwEffect;
		}
	}	

    return damage_args.damage_ != 0;
}

void NormalAttack::Broadcast_Normal_Result()
{
    ;{
        MSGSUB_BATTLE_PLAYER_ATTACK_BRD msg_brd;
        msg_brd.Clear();
        msg_brd.dwAttackerKey = owner_->GetObjectKey();
        msg_brd.byAttackType = skill_info_data_.attack_sequence_;
    #ifdef _NA003109_STYLE_PRESENTATION_IMPROVEMENT
        msg_brd.style_presentation = skill_info_data_.style_presentation_;
    #endif
        msg_brd.StyleCode = skill_info_data_.skill_code_;
        msg_brd.dwClientSerial = skill_info_data_.client_serial_;
        msg_brd.dwTargetKey = skill_info_data_.main_target_key_;
        msg_brd.wvCurPos = skill_info_data_.cur_pos_;
        msg_brd.wDamage = m_wDamage;
        msg_brd.dwTargetHP = m_pMainTarget->GetHP();
        msg_brd.byEffect = skill_info_data_.skill_effect_;
        msg_brd.byEtherComboCount = ether_combo_count_;
#ifdef _NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO
        msg_brd.target_sd = m_pMainTarget->GetSD();
#endif // _NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO
        owner_->SendPacketAroundForSync(&msg_brd);
    };
}











