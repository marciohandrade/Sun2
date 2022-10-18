#pragma once
#ifndef SKILL_SYSTEM_VERSION_ROUTING_INL
    #error "can't direct include this file"
#endif

#include "Skill.h"

//==================================================================================================

class NormalAttack : public Skill
{
public:
    NormalAttack() {}
    virtual ~NormalAttack() {}
    //
    virtual void Init(Character* owner,
                      const SkillInfo* skill_info,
                      const ROOT_SKILLINFO* root_skill_info);
    //virtual void Release();
    virtual eSKILL_TYPE GetSkillType() const;
    virtual BOOL StartExecute();
    //virtual void EndExecute();      // Interval 후에 실행되는 부분
    //virtual void CancelExcute(); // Interval 중에 스킬 시전 취소되는 부분.
    //virtual void SendAIMessage(Character* target, DAMAGETYPE damage = 0);  // 필요한 경우 NPC에게 AIMessage를 날린다.
    virtual BOOL CheckMainTarget(BattleResult* const detailed_result);  // MainTarget이 유효한지 체크한다.
    virtual bool FindTargets(ns_object::SearchOption::SingleSearch::result_type* const result);
    //virtual BYTE FindTargets(Character** target_array); // 타겟리스트를 구하는 부분
    virtual void SetExecuteInterval();
    //virtual WORD GetExecuteInterval();
protected:
    // added by NormalAttack
    virtual BOOL CalcDamage(Character* target);

private:
    // added by NormalAttack
    virtual void SetAttribution() {}
    //
    void Broadcast_Normal_Result();

protected:
    BYTE m_byAttackSequence; // 공격타입(1타, 2타)
    Character* m_pMainTarget; // 메인타겟

    //_NA_0_20100211_SD_EFFECT_MODIFY
    DAMAGETYPE m_wDamage; // 타겟의 데미지
    // MainTarget의 데미지, used by NormalAreaAttack
    DAMAGETYPE m_wMainTargetDamage;
};

//==================================================================================================

inline eSKILL_TYPE NormalAttack::GetSkillType() const {
    return SKILL_TYPE_NORMAL;
}

