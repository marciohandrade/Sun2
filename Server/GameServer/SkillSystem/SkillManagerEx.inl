#pragma once
#ifndef SKILL_SYSTEM_VERSION_ROUTING_INL
    #error "can't direct include this file"
#endif

//==================================================================================================
//  @history
//      - ~100820
//      - 100820, waverix, CHANGES, f100820.3L, code arrangement
//                                  f100821.1L, change a data structure to manage a short skill list
//                                  side-effect: a skill register sequence has a FIFO sequence
//==================================================================================================

struct SkillInfo;
class Skill;
class Character;

//==================================================================================================
//
class SkillManager
{
private:
    struct SkillNode {
        SLOTCODE code_;
        Skill* skill_;
    };
#ifdef _NA_007667_20141001_WITCHBLADE_SKILL
    struct AutoSkillNode {
        SLOTCODE skill_code;
        Character* target;
        bool is_connected_skill;    // 연결에 의해서 사용되는 스킬인지 체크

        AutoSkillNode() : target(NULL), is_connected_skill(false) {}
    };
#endif //_NA_007667_20141001_WITCHBLADE_SKILL
    //
    typedef STLX_LIST<SkillNode> SKILL_LIST;
public:
    static const int kVersion = 1; // f100820.1L
    //
    SkillManager(Character* owner);
    ~SkillManager();
    //
    void Init();
    void Release();
    void ReleaseDead();
    void Update();
    // CHANGES, f100831.4L, move a field of 'event_code' to a SkillInfo
    bool RegisterSkill(eSKILL_TYPE skill_type, const SkillInfo* skill_info);
    int GetActiveSkillNum() const;

    void CancelActiveSkill(SLOTCODE skill_code) const;
    void CancelAllSkill() const;

#ifdef _NA_007667_20141001_WITCHBLADE_SKILL
    bool SetAutoCastSkill(SLOTCODE skill_code, bool is_connected, Character* target = NULL);
#endif //_NA_007667_20141001_WITCHBLADE_SKILL
private:
    void AddActiveSkill(Skill* skill);
    // CHANGES, f100821.1L, stl iterator wrapper interfaces
    SKILL_LIST::iterator RemoveActiveSkill(Skill* skill);
    SKILL_LIST::iterator RemoveActiveSkill(SKILL_LIST::iterator del_iterator);
    SKILL_LIST::iterator FindActiveSkill(SLOTCODE skill_code);
    SKILL_LIST::const_iterator FindActiveSkill(SLOTCODE skill_code) const;
    //
    Character* owner_;
    SKILL_LIST active_skills_;

#ifdef _NA_007667_20141001_WITCHBLADE_SKILL
    void _ExecuteAutoCastSkill();
    void _ClearAutoCastSkills();

    STLX_MAP<SLOTCODE, AutoSkillNode> autocast_skills_; //(스킬코드, 스킬정보)
#endif //_NA_007667_20141001_WITCHBLADE_SKILL
};

//==================================================================================================

inline int SkillManager::GetActiveSkillNum() const
{
    return static_cast<int>(active_skills_.size());
}

inline SkillManager::SKILL_LIST::iterator
SkillManager::FindActiveSkill(SLOTCODE skill_code)
{
    SKILL_LIST::iterator it = active_skills_.begin(),
                         end = active_skills_.end();
    for ( ; it != end; ++it)
    {
        const SkillNode& node = *it;
        if (node.code_ == skill_code) {
            return it;
        }
    }
    return end;
}

inline SkillManager::SKILL_LIST::const_iterator
SkillManager::FindActiveSkill(SLOTCODE skill_code) const
{
    SKILL_LIST::const_iterator it = active_skills_.begin(),
                               end = active_skills_.end();
    for ( ; it != end; ++it)
    {
        const SkillNode& node = *it;
        if (node.code_ == skill_code) {
            return it;
        }
    }
    return end;
}

