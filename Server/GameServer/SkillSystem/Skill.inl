#pragma once
#ifndef SKILL_SYSTEM_VERSION_ROUTING_INL
    #error "can't direct include this file"
#endif

#include "./Skill_Info.h"
#include <SkillInfoParser.h>

//==================================================================================================
//  (History)
//  - 09.07.09, waverix, data declarations - move to Skill.h -> Skill_Info.h
//
struct RESULTMSG
{
    enum { MAX_BUFFER_SIZE = 1024, };
    int m_wSize;
    uint8_t m_byMsg[MAX_BUFFER_SIZE];
};

//==================================================================================================
//
class Skill
{
public:
    static const int kVersion = 1; // f100820.1L
    // CHANGES: f101117, added a data structure to support detailed error result for client
    struct BattleResult {
        typedef uint8_t result_t;
        result_t result_code; // RC::eBATTLE_RESULT
    };
    //
    Skill();
    // virtual interfaces
    virtual ~Skill();
    // CHANGES, f100831.4L, move a field of 'event_code' to a SkillInfo
    virtual void Init(Character* owner,
                      const SkillInfo* skill_info,
                      const ROOT_SKILLINFO* root_skill_info);

    virtual void Release();
    //
    virtual eSKILL_TYPE GetSkillType() const;
    virtual BOOL StartExecute() { return true; }; // 즉시 실행되는 부분
    virtual void EndExecute(); // Interval 후에 실행되는 부분
    virtual void CancelExcute() {}; // Interval 중에 스킬 시전 취소되는 부분.
    //
    //----------------------------------------------------------------------------
    Character* GetOwner() const { return owner_; }
    SkillInfo* GetSkillInfo() { return &skill_info_data_; }
    const SkillInfo* GetSkillInfo() const { return &skill_info_data_; }
    const SkillScriptInfo* GetSkillBaseInfo() const { return base_skill_info_; }
    SLOTCODE GetSkillCode() const;
    eSKILL GetSkillClassCode() const;

    DWORD GetExecuteTick() const { return execute_tick_; }

    //스킬 캐스팅 취소 관련부분..
    bool IsSkillCancel() const { return requested_skill_cancel_; }
    void SetSkillCancel(bool need_cancel) { requested_skill_cancel_ = need_cancel; }

    bool IsSkillRemove() const { return requested_skill_remove_; }
    void SetSkillRemove(bool need_remove) { requested_skill_remove_ = need_remove; }
    // ether bullet control interfaces
    void SetAppliedEtherBulletOption(BYTE need_apply) { is_applied_ether_bullet_option_ = (need_apply != false); }
    void SetAppliedEtherBulletInfo(tagEtherBulletInfo* pInfo) { applied_ether_bullet_info_ = pInfo; }
    bool GetAppliedEtherBulletOption() const { return is_applied_ether_bullet_option_; }
    tagEtherBulletInfo* GetAppliedEtherBulletInfo() const { return applied_ether_bullet_info_; }
    //
    bool IsInvokedInSafetyArea() const;
    // CHANGES: f110218.1L, added to support added value per accept state
    int GetAddedAttackPowerPerStatus() const;
    void SetAddedAttackPowerPerStatus(int changed_value);
#ifdef _NA_0_20100819_AGGRO_RENEWAL
	virtual BYTE FindTargetCount(eSKILL_TARGET_TYPE Target_Type);
#endif

protected:
    // CHANGES, f100821.1L, change an ability hash table to a fixed size array.
    struct AbilityContainer {
        struct Node {
            eABILITY ability_id_;
            Ability* ability_;
        };
        int number_of_abilities_;
        Node nodes_[MAX_ABILITY_NUM];
    };
    //
    virtual void SendAIMessage(Character* pTarget, DAMAGETYPE wDamage = 0);  // 필요한 경우 NPC에게 AIMessage를 날린다.
    virtual BOOL CheckMainTarget(BattleResult* const detailed_result); // MainTarget이 유효한지 체크한다.
    // _NA000000_110210_ENHANCED_TARGET_SEARCH_ALGORITHM
    virtual bool FindTargets(ns_object::SearchOption::SingleSearch::result_type* const result) = 0;
    //virtual BYTE FindTargets(Character** pTargetArray); // 타겟리스트를 구하는 부분

    virtual void SetExecuteInterval() {}
    virtual WORD GetExecuteInterval() { return interval_; }
    // WARNING, represent a kind of 'Init', MUST BE call first by Init
    void SetupDefault(Character* owner,
                      const SkillInfo* skill_info,
                      const ROOT_SKILLINFO* root_skill_info);
    void InitResultMsg();


    BOOL IsMaxTargetNum(Character *pTarget);
    void SetAttribution();
    BYTE GetAttribution() { return skill_attribution_; } // (CHANGES) (WAVIERX) changes type

    // ether bullet control interfaces
    tagEtherBulletInfo* GetBulletInfo();
    BOOL CanApplyBulletSkill();
    void ApplyBulletOption();
    void RemoveBulletOption();
    //
    void DecreaseHPMP();
    //_NA000000_110210_ENHANCED_TARGET_SEARCH_ALGORITHM
    // NOTE: f110309.1L, this method is implemented for (Instant/Delayed) skill processing.
    bool ExecuteSkill();
    //
    BYTE ExecuteEffectAbilities(RESULTMSG* const raw_result_msg);  // 이펙트 관련된 어빌리티를 적용한다.
    void Broadcast_Instant_Result();
    void Broadcast_Delayed_Result();
    //

private:
    void ConfigureAnimationDelay(const DWORD emptyArg = 0);
    void AddAbilities();
    void RemoveAbilities();
    //_NA000000_110210_ENHANCED_TARGET_SEARCH_ALGORITHM
    bool ExecuteAbilities(
        ns_object::SearchOption::SkillBase* search_option,
        const nsGrid::ViewBoxCubic& damage_box,
        Character* target, RESULTMSG* const raw_result_msg);  // Ability를 적용시킨다.

#ifdef _NA_008588_20151202_MODIFY_MOVEING_SKILL_LOGIC
    //스킬로 인한 이동처리
    VOID _ProcessSkillMove();
#endif //_NA_008588_20151202_MODIFY_MOVEING_SKILL_LOGIC
protected:
    // WARNING, 1st field, a below section can't declare a data structure like a stl container
    uint8_t field_block_start_[1];
    //
    Character* owner_; // 스킬 시전자
    GameField* game_field_;     // 공격자가 속한 필드
    const SkillScriptInfo* base_skill_info_;
    const BASE_STYLEINFO* base_style_info_; // moved from a NormalAttack ...

    struct { // flag data
        DWORD execute_tick_;
        WORD interval_;
        BYTE skill_attribution_; // Invisible, TargetStop, TargetFlying
        bool is_missed_;
        bool is_target_owner_;
        bool requested_skill_cancel_; // 스킬 취소 여부.
        bool requested_skill_remove_;
        bool is_invoked_in_safety_area_; //_NA001415_091123_CHANGE_MOB_AI_IN_SAFETY_AREA_
        bool passive_applied_; // use only a passive skill section
    };
    struct { // query space
        //ABILITY_HASH m_AbilitiesByIndex;
        AbilityContainer abilities_;
        SkillInfo skill_info_data_;
    };

    struct { // ether bullet data
        tagEtherBulletInfo* applied_ether_bullet_info_;
        bool is_applied_ether_bullet_option_;
        BYTE ether_combo_count_;
    };
    //
    struct { // result process working space
        // CHANGES: f110218.1L, added to support added value per accept state
        int added_attack_power_per_status_;
        BONUS_ABILITY_EFFECT bonus_ability_effect_; // 모든 타겟들에 공통으로 적용되는 어빌리티 효과
        BYTE number_of_hit_targets_;  // 스킬 데미지를 입은 타겟수
        BYTE number_of_effects_;     // Effect 어빌리티 개수
        //BYTE number_of_targets_; // 공격 대상의 수량    
        //Character* target_array_[MAX_TARGET_NUM]; // 공격 대상 리스트
    };
    // MUST be set on a last point
    uint8_t field_block_end_[1];
    //
    static RESULTMSG result_msg_array_[MAX_TARGET_NUM]; // 스킬 결과 구조체
    static RESULTMSG result_effect_msg_array_[5]; // Effect 스킬 결과 구조체
    //
    friend struct ns_object::SearchOption::SkillBase;
};

//==================================================================================================

inline Skill::Skill()
{
    ZeroMemory(field_block_start_, static_cast<size_t>(field_block_end_ - field_block_start_));
}

inline eSKILL_TYPE Skill::GetSkillType() const
{ 
    // can't enter this method
    __debugbreak();
    return SKILL_TYPE_UPPERBOUND;
}

inline SLOTCODE Skill::GetSkillCode() const
{
    return base_skill_info_ ? base_skill_info_->m_SkillCode :
           base_style_info_ ? base_style_info_->m_SkillCode :
                              0;
}

inline eSKILL Skill::GetSkillClassCode() const
{
    return base_skill_info_ ? static_cast<eSKILL>(base_skill_info_->m_SkillClassCode) :
           base_style_info_ ? static_cast<eSKILL>(base_style_info_->m_SkillClassCode) :
                              eSKILL_INVALID;
}

inline bool Skill::IsInvokedInSafetyArea() const
{
    return is_invoked_in_safety_area_;
}

// CHANGES: f110218.1L, added to support added value per accept state
inline int Skill::GetAddedAttackPowerPerStatus() const {
    return added_attack_power_per_status_;
};

inline void Skill::SetAddedAttackPowerPerStatus(int changed_value) {
    added_attack_power_per_status_ = changed_value;
}

