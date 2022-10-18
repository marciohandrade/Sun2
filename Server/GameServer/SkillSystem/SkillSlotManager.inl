#pragma once
#ifndef SKILL_SYSTEM_VERSION_ROUTING_INL
    #error "can't direct include this file"
#endif

#include "SkillManager.h"


//==================================================================================================

class SlotManager;
class SCSkillSlotContainer;
class SCQuickSlotContainer;
class SCQuickStyleSlotContainer;
class Player;

//==================================================================================================
// NOTE, f100826.2L, a 'StyleQuickSlot' redirecting interface to support The Episode II perk system.
class IQuickStyleManager
{
public:
    virtual bool UpdateStyleSlots() = 0;
    virtual RC::eSTYLE_RESULT LinkStyle(SLOTCODE style_code, POSTYPE pos) = 0;
    virtual void SerializeStyleTotalInfo(STYLE_TOTAL_INFO& style_total_info,
                                         eSERIALIZE serialize_type) = 0;
};

//==================================================================================================
// @history
//  ~100824
//  100824, waverix, code arrangement and change an interface access level

class SkillSlotManager : public IQuickStyleManager
{
public:
    typedef STLX_MAP<SLOTCODE, Skill*> SKILL_HASH;
    //
    SkillSlotManager();
    virtual ~SkillSlotManager();
    // IQuickStyleManager interfaces {
    virtual bool UpdateStyleSlots();
    virtual RC::eSTYLE_RESULT LinkStyle(SLOTCODE style_code, POSTYPE pos);
    virtual void SerializeStyleTotalInfo(STYLE_TOTAL_INFO& style_total_info,
                                         eSERIALIZE serialize_type);
    // } end of IQuickStyleManager declaration
    void Init(SlotManager* const slot_manager);
    void Release();

    SCSkillSlotContainer* GetSkillSlotContainer() const;
    SCQuickSlotContainer* GetQuickSlotContainer() const;

    bool IsEmpty(POSTYPE pos);

    // 무기 아이템 교환에 따른 스킬 적용 변경
    void AddWeapon(CODETYPE new_weapon_code);
    void DelWeapon();
    // CHANGES f100825.2L, change a routine to a total serializing skill stream converter
    void SerializeSkillStreamTotal(SKILL_TOTAL_INFO* const total_skill_stream,
                                   eSERIALIZE serialize_type);
    //// 스킬 슬롯 갱신
    //void SerializeSkillStream(POSTYPE pos, SKILLSTREAM* IN pStream, eSERIALIZE eType);
    void InitSkill(); // call on status initialization
    void LevelDownSkill(SLOTCODE cur_skill_code, SLOTCODE lower_skill_code);
    void UnLearnSkill(SLOTCODE cur_skill_code);
    void LevelDownSkillUntilSatisfaction(SLOTCODE cur_skill_code);

    // 스킬 습득 관련
    //_NA_0_20110131_SKILLTREE_SYSTEM_RENEWAL
    RC::eSKILL_RESULT LevelUpSkill(
        SKILL_TOTAL_INFO& OUT total_stream, 
        const SLOTCODE* skill_code_array, 
        const uint skill_code_count
    );

    //=============================================================================================
    // NOTE: r100825.3L, When we encounter the Episode II that we don't use anymore
    // about styles and passive skills.
    // but, default styles will keep on using like a punch and default types each a weapon type.
    // NOTE: r100917.3L, the transformation skill, the special skill of a dragon knight,
    // has changed style also.
    RC::eSKILL_RESULT CanSelectStyle(SLOTCODE new_style_code);
    void SetSelectStyle(SLOTCODE new_style_code);
    // {{{
    // r100825.3L, (SKILL_SYSTEM_VERSION == 1)
    void ApplyStyleBuff(SLOTCODE style_code);
    void ReleaseStyleBuff(SLOTCODE style_code);
    // 스타일 습득 관련
    RC::eSKILL_RESULT CanLevelUpStyle(SLOTCODE cur_style_code, SLOTCODE new_style_code);
    void LevelUpStyle(SLOTCODE cur_style_code, SLOTCODE new_style_code, SKILLSLOT& OUT new_skill_slot);
    RC::eSKILL_RESULT CanLearnStyle(BOOL is_first_learn, SLOTCODE new_style_code);
    void LearnStyle(SLOTCODE new_style_code, SKILLSLOT& OUT new_skill_slot);
    void UnLearnStyle(SLOTCODE style_code);
    //
    void InitPassiveSkillStyle();
    void UpdatePassiveSkills();  // 현재 조건에 따라 패시브 스킬들을 적용 및 해제한다.
    Skill* FindPassiveSkill(SLOTCODE skill_code) const;
    const Skill* FindPassiveSkillfromSkillClass(const eSKILL skill_class) const;
    const SLOTCODE FindSkill(const eSKILL skill_class) const;
    // }}}
    //=============================================================================================

private:
    bool ValidState();
    // {{{
    // NOTE: f100824.7L, change an access level to private
    // NOTE: r100825.3L, (SKILL_SYSTEM_VERSION == 1)
    bool CanUnLearnSkill(SLOTCODE cur_skill_code);
    bool CanLevelDownSkill(SLOTCODE cur_skill_code);
    //
    void AddPassiveSkill(Skill* const skill);
    bool AddAttrForPassiveSkill(const SLOTCODE skill_code);   // 패시브 스킬의 기능을 적용한다.
    bool RemovePassiveSkill(const SLOTCODE skill_code);
    // }}}

#ifdef _NA_0_20100901_SKILLTREE_SYSTEM
    uint get_used_skill_point() const;
#endif

 //_NA_0_20110131_SKILLTREE_SYSTEM_RENEWAL
    struct LevelUpData
    {
        SLOTCODE new_skill_code;
        SLOTCODE old_skill_code;
        bool is_passive_skill;
        bool is_summon_skill;
        int  require_skill_point;
        uint skill_line;
        int job_type;
        int job_line_index;

        LevelUpData()
        {
            ::ZeroMemory(this, sizeof(*this));
        }
    };

    friend class SkillLevelDown;

    void UnLockFirstSkillLine();

    RC::eSKILL_RESULT skill_levelup_test(
        LevelUpData* OUT levelup_data, 
        SCSkillSlotContainer* INOUT skill_container, 
        const SkillScriptInfo* new_skill_info
    ) const;

    void apply_skill_levelup(const LevelUpData* levelup_data);

    uint serialize_skill_array(
        SKILL_TOTAL_INFO& OUT total_stream,
        SCSkillSlotContainer* skill_container,
        const SLOTCODE* skill_code_array, 
        const uint skill_code_count
    ) const;

    RC::eSKILL_RESULT check_enough_upper_skill_level(
        const SCSkillSlotContainer* skill_container, 
        const SkillScriptInfo* skill_info
    ) const;

private:
    Player* player_;
    SlotManager* slot_manager_;
    SCSkillSlotContainer* skill_container_;
    SCQuickSlotContainer* quick_container_;
    SCQuickStyleSlotContainer* style_container_;
    SLOTCODE pre_style_code_;
    //
    SKILL_HASH passive_skills_;
};

//==================================================================================================
//==================================================================================================
//==================================================================================================

inline SCSkillSlotContainer* SkillSlotManager::GetSkillSlotContainer() const {
    return skill_container_;
}

inline SCQuickSlotContainer* SkillSlotManager::GetQuickSlotContainer() const {
    return quick_container_;
}

inline Skill* SkillSlotManager::FindPassiveSkill(SLOTCODE skill_code) const
{
    SKILL_HASH::const_iterator found = passive_skills_.find(skill_code);
    return found != passive_skills_.end() ? found->second : NULL;
}

//==================================================================================================
//==================================================================================================
//==================================================================================================

#if SKILL_SYSTEM_VERSION >= 2

// IQuickStyleManager interfaces {
//
inline bool SkillSlotManager::UpdateStyleSlots()
{ return false; }

inline RC::eSTYLE_RESULT SkillSlotManager::LinkStyle(SLOTCODE style_code, POSTYPE pos)
{ __UNUSED((style_code, pos)); return RC::RC_STYLE_FAILED; }

inline void SkillSlotManager::SerializeStyleTotalInfo(STYLE_TOTAL_INFO& style_total_info,
                                                      eSERIALIZE serialize_type)
{
    __UNUSED((style_total_info, serialize_type));
    if (serialize_type == SERIALIZE_LOAD) {
        style_total_info.m_Count = 0;
    }
}

inline void SkillSlotManager::ApplyStyleBuff(SLOTCODE style_code)
{ __UNUSED(style_code); }

inline void SkillSlotManager::ReleaseStyleBuff(SLOTCODE style_code)
{ __UNUSED(style_code); }

inline RC::eSKILL_RESULT
SkillSlotManager::CanLevelUpStyle(SLOTCODE cur_style_code, SLOTCODE new_style_code)
{ __UNUSED((cur_style_code, new_style_code)); return RC::RC_SKILL_FAILED; }

inline void SkillSlotManager::LevelUpStyle(SLOTCODE cur_style_code, SLOTCODE new_style_code,
                                           SKILLSLOT& OUT new_skill_slot)
{ __UNUSED((cur_style_code, new_style_code, new_skill_slot)); }

inline RC::eSKILL_RESULT SkillSlotManager::CanLearnStyle(BOOL is_first_learn,
                                                         SLOTCODE new_style_code)
{ __UNUSED((is_first_learn, new_style_code)); return RC::RC_SKILL_FAILED; }

inline void SkillSlotManager::LearnStyle(SLOTCODE new_style_code, SKILLSLOT& OUT new_skill_slot)
{ __UNUSED((new_style_code, new_skill_slot)); }

inline void SkillSlotManager::UnLearnStyle(SLOTCODE style_code)
{ __UNUSED(style_code); }

inline void SkillSlotManager::InitPassiveSkillStyle()
{}

#endif //SKILL_SYSTEM_VERSION >= 2
