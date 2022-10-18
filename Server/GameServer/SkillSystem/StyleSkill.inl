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
    //virtual void CancelExcute(); // Interval 중에 스킬 시전 취소되는 부분.
    //virtual void SendAIMessage(Character* target, DAMAGETYPE damage = 0);  // 필요한 경우 NPC에게 AIMessage를 날린다.
    virtual BOOL CheckMainTarget(BattleResult* const detailed_result); // MainTarget이 유효한지 체크한다.
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
        BYTE       common_effect;            // Target 공통으로 적용되는 이펙트
        //_NA_0_20100211_SD_EFFECT_MODIFY
        DAMAGETYPE damage;                   // 각 타겟의 데미지
        DAMAGETYPE main_target_damage;       // MainTarget의 데미지

        WzVector   player_pos;               // 플레이어 위치
        WzVector   target_pos;               // 타겟 위치
        WzVector   thrust_pos;               // 녹백 이후의 타겟위치
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

