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
    //virtual void SendAIMessage(Character* target, DAMAGETYPE damage = 0);  // 필요한 경우 NPC에게 AIMessage를 날린다.
    //virtual BOOL CheckMainTarget(BattleResult* const detailed_result); // MainTarget이 유효한지 체크한다.
    virtual bool FindTargets(ns_object::SearchOption::SingleSearch::result_type* const result);
    //virtual BYTE FindTargets(Character** target_array); // 타겟리스트를 구하는 부분
    virtual void SetExecuteInterval();
    //virtual WORD GetExecuteInterval();
};

//==================================================================================================

inline eSKILL_TYPE DelayedSkill::GetSkillType() const {
    return SKILL_TYPE_ACTIVE_DELAYED;
}
