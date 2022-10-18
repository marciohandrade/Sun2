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
    //virtual void EndExecute();      // Interval �Ŀ� ����Ǵ� �κ�
    //virtual void CancelExcute(); // Interval �߿� ��ų ���� ��ҵǴ� �κ�.
    //virtual void SendAIMessage(Character* target, DAMAGETYPE damage = 0);  // �ʿ��� ��� NPC���� AIMessage�� ������.
    virtual BOOL CheckMainTarget(BattleResult* const detailed_result);  // MainTarget�� ��ȿ���� üũ�Ѵ�.
    virtual bool FindTargets(ns_object::SearchOption::SingleSearch::result_type* const result);
    //virtual BYTE FindTargets(Character** target_array); // Ÿ�ٸ���Ʈ�� ���ϴ� �κ�
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
    BYTE m_byAttackSequence; // ����Ÿ��(1Ÿ, 2Ÿ)
    Character* m_pMainTarget; // ����Ÿ��

    //_NA_0_20100211_SD_EFFECT_MODIFY
    DAMAGETYPE m_wDamage; // Ÿ���� ������
    // MainTarget�� ������, used by NormalAreaAttack
    DAMAGETYPE m_wMainTargetDamage;
};

//==================================================================================================

inline eSKILL_TYPE NormalAttack::GetSkillType() const {
    return SKILL_TYPE_NORMAL;
}

