#pragma once
#ifndef SKILL_SYSTEM_VERSION_ROUTING_INL
    #error "can't direct include this file"
#endif

#include "Skill.h"

//==================================================================================================

class DelayedSkill : public Skill
{
public:
    DelayedSkill() {}
    virtual ~DelayedSkill() {}
    //
    //virtual void Init(Character* owner,
    //                  const SkillInfo* skill_info,
    //                  const ROOT_SKILLINFO* root_skill_info);
    //virtual void Release();
    virtual eSKILL_TYPE GetSkillType() const;
    virtual BOOL StartExecute();
    virtual void EndExecute();
    virtual void CancelExcute();
    //virtual void SendAIMessage(Character* target, DAMAGETYPE damage = 0);  // �ʿ��� ��� NPC���� AIMessage�� ������.
    //virtual BOOL CheckMainTarget(BattleResult* const detailed_result); // MainTarget�� ��ȿ���� üũ�Ѵ�.
    virtual bool FindTargets(ns_object::SearchOption::SingleSearch::result_type* const result);
    //virtual BYTE FindTargets(Character** target_array); // Ÿ�ٸ���Ʈ�� ���ϴ� �κ�
    virtual void SetExecuteInterval();
    //virtual WORD GetExecuteInterval();
};

//==================================================================================================

inline eSKILL_TYPE DelayedSkill::GetSkillType() const {
    return SKILL_TYPE_ACTIVE_DELAYED;
}
