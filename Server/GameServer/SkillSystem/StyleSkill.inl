#pragma once
#ifndef SKILL_SYSTEM_VERSION_ROUTING_INL
    #error "can't direct include this file"
#endif

// @history
// ~10/01/22|
//  10/01/22|waverix|code arrangement
//                  |remove macro=__NA_1233_2009_NEW_YEARS_DAY_EVENT|

#include "Skill.h"

//==================================================================================================
//
class StyleSkill : public Skill
{
public:
    StyleSkill();
    virtual ~StyleSkill(){}

    virtual void Init(Character* owner,
                      const SkillInfo* skill_info,
                      const ROOT_SKILLINFO* root_skill_info);
    //virtual void Release();
    virtual eSKILL_TYPE GetSkillType() const;
    virtual BOOL StartExecute();
    virtual void EndExecute();
    //virtual void CancelExcute(); // Interval �߿� ��ų ���� ��ҵǴ� �κ�.
    //virtual void SendAIMessage(Character* target, DAMAGETYPE damage = 0);  // �ʿ��� ��� NPC���� AIMessage�� ������.
    virtual BOOL CheckMainTarget(BattleResult* const detailed_result); // MainTarget�� ��ȿ���� üũ�Ѵ�.
    virtual bool FindTargets(ns_object::SearchOption::SingleSearch::result_type* const result);
    //virtual BYTE FindTargets(Character** pTargetArray);
    virtual void SetExecuteInterval();
    //virtual WORD GetExecuteInterval();

private:
    // added by StyleSkill
    virtual void SetAttribution() {}
    //
    // internal interface 'dice by ratio'
    // arg: 100% = 1.0f
    BOOLEAN IsOccuredByDraw(float ratio_limit);

    void CommonExecute(); // changes to private
    void CommonRelease(); // changes to private
    BOOL CalcDamage(Character* pTarget);
    void ApplyEffect(Character* pTarget);
    void Broadcast_Style_Result();

    //------------------------------------------------------------------------------
    struct {
        BYTE       common_effect;            // Target �������� ����Ǵ� ����Ʈ
        //_NA_0_20100211_SD_EFFECT_MODIFY
        DAMAGETYPE damage;                   // �� Ÿ���� ������
        DAMAGETYPE main_target_damage;       // MainTarget�� ������

        WzVector   player_pos;               // �÷��̾� ��ġ
        WzVector   target_pos;               // Ÿ�� ��ġ
        WzVector   thrust_pos;               // ��� ������ Ÿ����ġ
    } style_;
};

//==================================================================================================

inline StyleSkill::StyleSkill()
{
    ZeroMemory(&style_, sizeof(style_));
}

inline eSKILL_TYPE StyleSkill::GetSkillType() const
{
    return SKILL_TYPE_STYLE;
}

