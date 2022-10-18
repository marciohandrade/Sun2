#pragma once
#ifndef SKILL_SYSTEM_VERSION_ROUTING_INL
    #error "can't direct include this file"
#endif

#include "NormalAttack.h"

//==================================================================================================

class NormalAreaAttack : public NormalAttack
{
public:
    NormalAreaAttack() {}
    virtual ~NormalAreaAttack(){}

    // removed because of this method use only a m_wMainTargetDamage clear operation.
    //virtual void Init(Character* owner,
    //                  const SkillInfo* skill_info,
    //                  const ROOT_SKILLINFO* root_skill_info,
    //                  BYTE event_code = 0);
    //virtual void Release();
    virtual eSKILL_TYPE GetSkillType() const;
    virtual BOOL StartExecute();
    //virtual void EndExecute(); // Interval 후에 실행되는 부분
    //virtual void CancelExcute(); // Interval 중에 스킬 시전 취소되는 부분.
    //virtual SLOTCODE GetSkillCode();
    //virtual eSKILL GetSkillClassCode();
    //virtual void SendAIMessage(Character* target, DAMAGETYPE damage = 0);  // 필요한 경우 NPC에게 AIMessage를 날린다.
    //virtual BOOL CheckMainTarget(BattleResult* const detailed_result); // MainTarget이 유효한지 체크한다.
    virtual bool FindTargets(ns_object::SearchOption::SingleSearch::result_type* const result);
    //virtual BYTE FindTargets(Character** target_array);
    //virtual void SetExecuteInterval();
    //virtual WORD GetExecuteInterval();
private:
    // derived from NormalAttack
    virtual BOOL CalcDamage(Character* target);
    // derived from NormalAttack
    virtual void SetAttribution() {}
private:
    //DAMAGETYPE m_wMainTargetDamage; // MainTarget의 데미지, moved to NormalAttack
};

//==================================================================================================

inline eSKILL_TYPE NormalAreaAttack::GetSkillType() const {
    return SKILL_TYPE_NORMAL_AREA;
}

