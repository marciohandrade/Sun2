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
    //virtual void EndExecute(); // Interval �Ŀ� ����Ǵ� �κ�
    //virtual void CancelExcute(); // Interval �߿� ��ų ���� ��ҵǴ� �κ�.
    //virtual SLOTCODE GetSkillCode();
    //virtual eSKILL GetSkillClassCode();
    //virtual void SendAIMessage(Character* target, DAMAGETYPE damage = 0);  // �ʿ��� ��� NPC���� AIMessage�� ������.
    //virtual BOOL CheckMainTarget(BattleResult* const detailed_result); // MainTarget�� ��ȿ���� üũ�Ѵ�.
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
    //DAMAGETYPE m_wMainTargetDamage; // MainTarget�� ������, moved to NormalAttack
};

//==================================================================================================

inline eSKILL_TYPE NormalAreaAttack::GetSkillType() const {
    return SKILL_TYPE_NORMAL_AREA;
}

