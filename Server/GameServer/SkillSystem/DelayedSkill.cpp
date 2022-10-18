#include "stdafx.h"
#include "DelayedSkill.h"

//==================================================================================================

#include <PacketStruct_CG.h>
#include <SkillInfoParser.h>
#include <Battle.h>

#include "AITypes.h"
#include "CharacterFormula.h"

#include "GameField.h"
#include "GameDominationField.h"

#include "Character.h"
#include "Player.h"
#include "NPC.h"

//==================================================================================================
// @history
//  - ~110308
//  - 110308, waverix, code arrangement

void DelayedSkill::SetExecuteInterval()
{
    // ĳ���� Ÿ�� ���� ����
    int iCastingTime = base_skill_info_->m_wSkillCasting;
    if (base_skill_info_->m_wSkillCasting)
    {
        int iBonusCastingTime = owner_->GetAttr().GetBonusCastingTime();
        float fBonusCastingTimeRatio = owner_->GetAttr().GetRatioBonusCastingTime() / 100.f;
        fBonusCastingTimeRatio = max(-1.0f, fBonusCastingTimeRatio);

        iCastingTime = iCastingTime + iBonusCastingTime;
        iCastingTime = max(0, iCastingTime);
        iCastingTime = (int)(iCastingTime * (1 + fBonusCastingTimeRatio));
    }

    interval_ = iCastingTime + base_skill_info_->m_wFlyingLifeTime + skill_info_data_.skill_delay_;
}

bool DelayedSkill::FindTargets(
    ns_object::SearchOption::SingleSearch::result_type* const result)
{
    //_NA000000_110210_ENHANCED_TARGET_SEARCH_ALGORITHM
    __UNUSED(result);
    assert(!"can't entered at this point");
    return false;
}

BOOL DelayedSkill::StartExecute()
{
    game_field_ = owner_->GetField();
    if (game_field_ == NULL)
    {
        MSG_CG_SKILL_ACTION_NAK msg_nak;
        msg_nak.m_byErrorCode = RC::RC_SKILL_FIELD_IS_NULL;
        owner_->SendPacket(&msg_nak, sizeof(msg_nak));
        SUNLOG(eCRITICAL_LOG, _T("|["__FUNCTION__"]|Msg=GameField is NULL!"));
        return false;
    }

    // test whether the main target is valid like a skill attack ranges checking.
    if (CheckMainTarget(NULL) == false)
    {
        is_missed_ = true;
    }

    ;{
        MSGSUB_SKILL_ACTION_DELAY_START_BRD msg_brd;
        msg_brd.Clear();
        if (SkillActionDelayStart* result = &msg_brd.result)
        {
            result->m_SkillCode = skill_info_data_.skill_code_;
            result->m_dwClientSerial = skill_info_data_.client_serial_;
            result->m_dwAttackerKey = owner_->GetObjectKey();
            result->m_dwPrimaryTargetKey = skill_info_data_.main_target_key_;
            result->m_wvMainTargetPos = skill_info_data_.main_target_pos_;
            result->m_wvCurPos  = skill_info_data_.cur_pos_;
            result->m_wvDestPos = skill_info_data_.dest_pos_;
        };
        owner_->SendPacketAroundForSync(&msg_brd);
    };
    return true;
}

void DelayedSkill::EndExecute()
{
    // TODO: WARNING: if the result of method processing is fails,
    // may be skip Skill::EndExecute() calling

    game_field_ = owner_->GetField();
    if (game_field_ == NULL)
    {
        MSG_CG_SKILL_ACTION_NAK msg_nak;
        msg_nak.m_byErrorCode = RC::RC_SKILL_FIELD_IS_NULL;
        owner_->SendPacket(&msg_nak, sizeof(msg_nak));
        SUNLOG(eCRITICAL_LOG, _T("|["__FUNCTION__"]|Msg=GameField is NULL!"));
        return;
    }

    //_NA001385_20090924_DOMINATION_FINDPATH_ATTACK
    // test whether the main target is valid like a skill attack ranges checking.
    if (CheckMainTarget(NULL) == false) {
        is_missed_ = true;
    }
    //----------------------------------------------------------------------------------------------
    Skill::InitResultMsg();
    //----------------------------------------------------------------------------------------------
    if (is_missed_) 
    {
        // ��ų�� ��ȿȭ���� �ʰ� Miss ó���ǹǷ� MP�� ���ҽ�Ų��.
        DecreaseHPMP();
        // Miss ó���� �ϱ� ���ؼ� ��� ��Ŷ�� ������.
        Broadcast_Delayed_Result();
        //
        Skill::EndExecute();
        return;
    }

    // MP�� ���ҽ�Ų��.
    DecreaseHPMP();

    //_NA000000_110210_ENHANCED_TARGET_SEARCH_ALGORITHM
    Skill::ExecuteSkill();
    //
    Broadcast_Delayed_Result();

    Skill::EndExecute();
    //
    return;
}

// NOTE: cancel casting.
// TODO: waverix, this skill must be have casting delay if the result of method is success.
void DelayedSkill::CancelExcute()
{
    // ��ų ��ҿ� ���� �⺻�� ����
    SetSkillCancel(false); 

    Skill::EndExecute();

    // reset cool timer
    util::Timer* cool_timer = NULL;

    switch (skill_info_data_.skill_factor_)
    {
    case SKILL_FACTOR_NOMAL:
        cool_timer = owner_->GetSkillCoolTimer(base_skill_info_->m_SkillCode);
        break;
    case SKILL_FACTOR_DOMINATION:
        if (GameZone* game_zone = owner_->GetGameZonePtr())
        {
            const eZONETYPE zone_type = game_zone->GetZoneType();
            if (zone_type == eZONETYPE_DOMINATION_FIELD)
            {
                GameDominationField* domination_field = static_cast<GameDominationField*>(game_zone);
                cool_timer = domination_field->GetCoolTimer(owner_);
            }
        }
        break;
    }

    if (cool_timer == NULL)
    {
        MSG_CG_SKILL_CASTING_CANCEL_NAK msg_nak;
        owner_->SendPacket(&msg_nak, sizeof(msg_nak));
        return;
    }
    else
    {
        cool_timer->InitCoolTime();
        ;{
            MSGSUB_SKILL_CASTING_CANCEL_BRD msg_brd;
            msg_brd.Clear();
            msg_brd.m_dwObjectKey = owner_->GetObjectKey();
            msg_brd.m_SkillCode = skill_info_data_.skill_code_;

            owner_->SendPacketAroundForSync(&msg_brd);
        };
    }
}



















