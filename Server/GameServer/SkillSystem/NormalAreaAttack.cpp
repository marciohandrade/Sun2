#include "stdafx.h"
#include "SkillInfoParser.h"
#include "NPC.h"
#include "GameField.h"
#include "Character.h"
#include "Player.h"
#include "NormalAreaAttack.h"
#include "AITypes.h"
#include "CharacterFormula.h"
#include <PacketStruct_CG.h>
#include <Battle.h>
#include "StatusManager.h"
#include "AIParamParser.h"
#include "EtcStatus.h"

//==================================================================================================

const int _MAX_ATTR_AREA_ATTACK_TARGET_NUM = 3;
const float _MAX_ATTR_AREA_ATTACK_RANGE = 3.f;

//==================================================================================================

/*
// removed because of this method use only a m_wMainTargetDamage clear operation.
// CHANGES, f100821.1L, fix up an invalid inherited interface specification occurred by 'event code'
void NormalAreaAttack::Init(Character* owner,
                            const SkillInfo* skill_info,
                            const ROOT_SKILLINFO* root_skill_info,
                            BYTE event_code)
{
	m_wMainTargetDamage = 0;

	NormalAttack::Init( pOwnerChar, pSkillInfo, pRootSkillInfo, byEventCode);
}
*/

BOOL NormalAreaAttack::StartExecute()
{
	game_field_ = owner_->GetField();
	if( !game_field_ ) 
	{
		MSG_CG_BATTLE_PLAYER_ATTACK_NAK NakMsg;
		NakMsg.m_byErrorCode = RC::RC_BATTLE_FIELD_IS_NULL;
		owner_->SendPacket( &NakMsg, sizeof(NakMsg) );
		SUNLOG( eCRITICAL_LOG, "[NormalAreaAttack::StartExecute] GameField is NULL!" );
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

    MSGSUB_BATTLE_PLAYER_AREA_ATTACK_BRD msg_brd;
    msg_brd.Clear();
    ;{
        msg_brd.m_dwClientSerial = skill_info_data_.client_serial_;
        msg_brd.m_dwAttackerKey = owner_->GetObjectKey();
        msg_brd.m_dwPrimaryTargetKey = skill_info_data_.main_target_key_;
        msg_brd.m_wvCurPos = skill_info_data_.cur_pos_;
        msg_brd.m_StyleCode = skill_info_data_.skill_code_;
        msg_brd.m_byAttackType = skill_info_data_.attack_sequence_;
    };

    // Ÿ�ٸ���Ʈ�� ���Ѵ�.
    ns_object::SearchOption::SingleSearch::result_type search_result;
    if (FindTargets(&search_result) == false) {
        return false;
    }
    const int number_of_targets = search_result.number_of_selected_chars;
    // Ÿ�ٺ��� �������� ����Ʈ�� �����Ѵ�.
    for (int i = 0; i < number_of_targets; ++i)
    {
        Character* const target = search_result.targets[i];
        // NOTE: change filters
        if (target == NULL ||
            target->IsDead() ||
            target->CanBeAttacked() == false ||
            owner_->IsFriend(target) != USER_RELATION_ENEMY)
        {
            continue;
        }
        // PK �����̶�� ����ó���� �Ѵ�.
        owner_->ForcedAttack(target);
        // �������� ����Ѵ�.
        skill_info_data_.skill_effect_ = 0;
        // it's possible that is occur side-effect like a owner_ object integrity
        if (CalcDamage(target)) 
        {
            // ���� �����Ƽ�� ó���ϴ� �߿� �ڱ��ڽ��� �׾ ��ų�� �ʱ�ȭ�� ���!
            // (���� �߻��ϸ� �ȵ����� ����ó��!)
            if (owner_ == NULL)
            {
                SUNLOG(eCRITICAL_LOG, "[NormalAreaAttack::StartExecute] owner_ is NULL!");
                return false;
            }
            // ������ �������� ���
            target->Attacked();

            SendAIMessage(target, m_wDamage);
        }

        if (AREA_ATTACK_RESULT_INFO* const attack_result =
                &msg_brd.AttackInfo[number_of_hit_targets_])
        {
            attack_result->m_dwTargetKey = target->GetObjectKey();
            attack_result->m_wDamage = m_wDamage;
            attack_result->m_dwTargetHP = target->GetHP();
            attack_result->m_byEffect = skill_info_data_.skill_effect_;
            attack_result->m_byComboCount = ether_combo_count_;
#ifdef _NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO
            attack_result->target_sd = target->GetSD();
#endif // _NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO
        }
        ++number_of_hit_targets_;
        // MaxTarget �� ����(�Ӽ��� ���� ���� ������ 3��������)
        if (number_of_hit_targets_ >= _MAX_ATTR_AREA_ATTACK_TARGET_NUM) {
            break;
        }
    }; //end 'for'
    //
    // �����ϸ� MP, HP�� ȸ���Ǵ� ĳ���Ͱ� �ִ�.
    owner_->OnAttack(m_pMainTarget, skill_info_data_.skill_code_, m_wMainTargetDamage);
    //
    msg_brd.m_byNumberOfTargets = number_of_hit_targets_;
    owner_->SendPacketAroundForSync(&msg_brd);

    return true;
}

bool NormalAreaAttack::FindTargets(
    ns_object::SearchOption::SingleSearch::result_type* const search_result)
{
    //_NA000000_110210_ENHANCED_TARGET_SEARCH_ALGORITHM
    assert(game_field_ != NULL);
    // result->targets sequence
    //  [MainTarget] | [OtherEnemy] | ...
    // CHANGES: f110528.3L, fixed invalid result linking.
    //ns_object::SearchResult::SingleSearch search_result;
    search_result->number_of_selected_chars = 0;
    ;{
        ns_object::SearchOption::SingleSearch search_option(
            _MAX_ATTR_AREA_ATTACK_TARGET_NUM,//int max_number_of_selected_characters,
            owner_, //Character* input_actor,
            m_pMainTarget, //Character* input_main_target_if_exist_it,
            NULL, //const WzVector* input_specific_application_center_pos_if_main_target_is_not_used_to_pos,
            //
            owner_->GetAttackRange(), //float attack_ranges,
            _MAX_ATTR_AREA_ATTACK_RANGE, //float ranges_of_application,
            SRF_FOWARD_360); //eSKILL_AREA_TYPE forms_of_application
        bool success = game_field_->FindTargetsUsingSingleSearch(search_option, search_result);
        if (success == false) {
            return false;
        };
    };
    return true;    
}

BOOL NormalAreaAttack::CalcDamage( Character *pTarget )
{
	if( NormalAttack::CalcDamage( pTarget ) )
	{
		// MainTarget�� ���
		if( skill_info_data_.main_target_key_ == pTarget->GetObjectKey() )
		{
            //_NA_0_20100211_SD_EFFECT_MODIFY
            m_wMainTargetDamage = m_wDamage;
		}

		return TRUE;
	}

	return FALSE;
}











