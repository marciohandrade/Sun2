#include "stdafx.h"
#include "InstantSkill.h"

//==================================================================================================

#include <PacketStruct_CG.h>
#include <SkillInfoParser.h>
#include <Battle.h>

#include "AITypes.h"
#include "CharacterFormula.h"

#include "GameField.h"

#include "Character.h"
#include "Player.h"
#include "NPC.h"

//==================================================================================================
// @history
//  - ~110308
//  - 110308, waverix, code arrangement

void InstantSkill::SetExecuteInterval()
{
    interval_ = 0;
    //
    const eSKILL skill_class_code = GetSkillClassCode();
    switch (skill_class_code)
    {
    case eSKILL_DOUBLE_ATTACK:  interval_ = 2000; break;
    case eSKILL_ILLUSION_DANCE: interval_ = 1500; break;
    case eSKILL_BLOOD_RAIN:     interval_ = 2000; break;
    case eSKILL_AIRBLOW:        interval_ = 4000; break;
    case eSKILL_BATTLERHONE:    interval_ = 4000; break;
    };
}

bool InstantSkill::FindTargets(
    ns_object::SearchOption::SingleSearch::result_type* const result)
{
    //_NA000000_110210_ENHANCED_TARGET_SEARCH_ALGORITHM
    __UNUSED(result);
    assert(!"can't entered at this point");
    return false;
}

BOOL InstantSkill::StartExecute()
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
        return false;
    }
    //----------------------------------------------------------------------------------------------
    Skill::InitResultMsg();
    //----------------------------------------------------------------------------------------------
    // test whether the main target is valid like a skill attack ranges checking.
    if (CheckMainTarget(NULL) == false)
    {
        // 스킬이 무효화되지 않고 Miss 처리되므로 MP를 감소시킨다.
        DecreaseHPMP();
        // Miss 처리를 하기 위해서 결과 패킷은 보낸다.
        Broadcast_Instant_Result();
        return false;
    }

    // MP를 감소시킨다.
    DecreaseHPMP();

    //_NA000000_110210_ENHANCED_TARGET_SEARCH_ALGORITHM
    Skill::ExecuteSkill();
    //
    Broadcast_Instant_Result();

    // NOTE: the instant skill has not Skill::EndExecute() calling... why?

    return true;
}














