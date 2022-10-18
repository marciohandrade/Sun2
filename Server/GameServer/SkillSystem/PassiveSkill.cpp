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

// �нú��� ��� ���� SkillManager���� ����Ʈ�� �����ǰ� �ִ�.
// �ٸ� ��ų��� ������ ������� ó���ǵ��� �������� �ٲ���� ���̴�. ���� �Ʒ� �ڵ�� ���ȵǰ� �ִ� ������. �Ф�
BOOL PassiveSkill::StartExecute()
{
    // �̹� ����Ǿ� �ִ� ��ų���� üũ
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
    // �̹� ����Ǿ� �ִ� ��ų���� üũ
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
    // Ability�� �����Ų��.
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
    // �̹� ����Ǿ� �ִ� ��ų���� üũ
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
