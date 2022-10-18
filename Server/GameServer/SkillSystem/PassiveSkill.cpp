#include "stdafx.h"
#include "PassiveSkill.h"
#include "SkillInfoParser.h"
#include "SkillAttrCalculator.h"
#include "Player.h"
#include "AbilityFactory.h"
#include "Ability.h"
#include "StatusManager.h"

void PassiveSkill::Release()
{
    ReleaseAbility();
    Skill::Release();
}

/*
// removed because of this method use only a passive apply check field clear operation.
// TODO: passive skill ... use in SkillSlotManager
void PassiveSkill::Init(Character* pOwnerChar,
    const SkillInfo* pSkillInfo,
    const ROOT_SKILLINFO* pRootSkillInfo,
    BYTE byEventCode)
{
    passive_applied_ = false;
    event_code_       = byEventCode;
    Skill::Init(pOwnerChar, pSkillInfo, pRootSkillInfo);
}
*/


bool PassiveSkill::FindTargets(
    ns_object::SearchOption::SingleSearch::result_type* const result)
{
    __UNUSED(result);
    assert(!"can't entered at this point");
    return false;
    //return Skill::FindTargetsDefault(result);
}

// 패시브의 경우 지금 SkillManager에서 리스트가 관리되고 있다.
// 다른 스킬들과 동일한 방식으로 처리되도록 언젠가는 바꿔야할 것이다. 현재 아래 코드는 사용안되고 있는 실정임. ㅠㅜ
BOOL PassiveSkill::StartExecute()
{
    // 이미 적용되어 있는 스킬인지 체크
    if (passive_applied_) {
        return false;
    }

    if (ApplyAbility())
    {
        passive_applied_ = true;
        return true;
    }
    else
    {
        return false;
    }
}

void PassiveSkill::EndExecute()
{
    // 이미 적용되어 있는 스킬인지 체크
    if (passive_applied_ == false) {
        return;
    }
    if (ReleaseAbility()) {
        passive_applied_ = false;
    }
}

BOOL PassiveSkill::ApplyAbility()
{
    if (owner_ == NULL)
    {
        SUNLOG(eCRITICAL_LOG, "[PassiveSkill::ApplyAbility] owner_ is NULL!");
        return false;
    }

    if (owner_->IsEqualObjectKind(PLAYER_OBJECT) == 0)
    {
        SUNLOG(eCRITICAL_LOG, "[PassiveSkill::ApplyAbility] owner_ is not Player");
        return false;
    }

    Player* pPlayer = (Player*)owner_;

    if (!pPlayer->CanApplyPassiveSkill(GetSkillCode())) {
        return false;
    }
    // Ability를 실행시킨다.
    WORD wMsgSize = 0;
    BYTE* pMsg = NULL;

    const AbilityContainer::Node* it = abilities_.nodes_,
                                * end = &abilities_.nodes_[_countof(abilities_.nodes_)];
    for (int index = 0; index < abilities_.number_of_abilities_ && it != end; ++index, ++it)
    {
        Ability* ability = it->ability_;
        ability->Execute(owner_, pMsg, wMsgSize);
    }

    return true;
}

BOOL PassiveSkill::ReleaseAbility()
{
    // 이미 적용되어 있는 스킬인지 체크
    if (passive_applied_ == false) {
        return false;
    }
    if (owner_ == NULL)
    {
        SUNLOG(eCRITICAL_LOG, "[PassiveSkill::ReleaseAbility] owner_ is NULL!");
        return false;
    }

    if (owner_->IsEqualObjectKind(PLAYER_OBJECT) == 0)
    {
        SUNLOG(eCRITICAL_LOG, "[PassiveSkill::ReleaseAbility] owner_ is not Player");
        return false;
    }

    const AbilityContainer::Node* it = abilities_.nodes_,
                                * end = &abilities_.nodes_[_countof(abilities_.nodes_)];
    for (int index = 0; index < abilities_.number_of_abilities_ && it != end; ++index, ++it)
    {
        Ability* ability = it->ability_;
        ability->Release(owner_);
    }

    return true;
}
