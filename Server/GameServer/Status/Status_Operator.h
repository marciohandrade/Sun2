#ifndef __GAMESERVER_STATUS_OPERATOR_H
#define __GAMESERVER_STATUS_OPERATOR_H
#pragma once

//==================================================================================================

class SkillScriptInfo;
struct BASE_ABILITYINFO;

struct MatchCheckCodeFilters;

//==================================================================================================
//  <StatusField>
//  - ���ݰ��ɿ���/�̵����ɿ���/��ų���ɿ��� ����� ��� �� �ΰ����� �߰� ���� ���Ǵ� ���µ���
//  ����� �д�.
//  -   �Ǵ� Ư�� ���� ������ ��쿡�� ����� �� ���� ���̴�.
//      ù ��� ��ʴ� 'Ÿ���� ���' �����������,
//      ���¡ ������� ���� ������ ������ �Ŷ� �����ȴ�.
class StatusField
{
public:
    typedef DWORD POLICY_T;
    enum eFLAG {
        DISABLE_ALL                   = 0,
        ENABLE_GM_UNDEAD              = 1 <<  3, // GM �÷��̾� '���� ��' ����
        ENABLE_GM_TRANSPARENT         = 1 <<  4, // GM �÷��̾� '���� ��' ����
        ENABLE_DRAGON_TRANSFORMING    = 1 <<  5, // �巡�ﳪ��Ʈ ���� �ʵ�, �巡�� �������� ����
        //__NA_S00015_20080828_NEW_HOPE_SHADOW
        ENABLE_CHARACTER_TRANSPARENT  = 1 <<  6, // ĳ���� '����(����)' ����
        //__NA_01240_GM_CMD_ADD_SERVER_TIME
        ENABLE_GM_MAX_DAMAGE          = 1 <<  7,
        ENABLE_GM_MAX_EXP             = 1 <<  8,
        ENABLE_GM_DROP_LIST_ALL       = 1 <<  9,
        //__NA001390_090915_RIDING_SYSTEM__
        ENABLE_SUMMONED_AND_RIDING    = 1 << 10, // Rider Summoned + Riding Status
        ENABLE_NON_PREEMPTIVE_ATTACK  = 1 << 11, // ���� �񼱰� ����, Ÿ�Ͽ� ������ �ʴ´�. // _NA001385_20090924_DOMINATION_ETC
        ENABLE_SPREAD_WINGS           = 1 << 12, // CHANGES: f110315.2L, whether a player spread winds.
        //  (Special Control) - Ÿ���� ��������� Ư�� �߰���, ���� �ٸ� �뵵�� ����� ���� �ְ�����...
        ENABLE_OBSERVER_MODE          = 1 << 25, // ���� ���...__NA001187_081015_SSQ_OBSERVER_MODE__
        ENABLE_ONLY_SKILL_TARGET_ME   = 1 << 26, // eSKILL_TARGET_TYPE ����, �ڽŸ� ��� ���� ����, ��å�� ���� ó�� ����
        ENABLE_FILTER_SKILL_GROUP     = 1 << 28, // ��ų ��� ���� �ɸ� ����, Skill Group Array �˻� �ʿ�
        ENABLE_FILTER_ITEM_TYPE       = 1 << 29, // ������ Ÿ�� ��� ���� �ɸ� ����, Item Type Array �˻� �ʿ�
    };


public:
    const POLICY_T& FIELD;

private:
    POLICY_T field_; // FIELD
    const MatchCheckCodeFilters* filter_;

public:
    StatusField() : FIELD(field_), field_(DISABLE_ALL), filter_(NULL) {}
    void Clear();
    void SetField(const POLICY_T flags);
    POLICY_T GetField() const;

    void AddFlag(const POLICY_T flag);
    void RemoveFlag(const POLICY_T flag);

    //  ���� ���� ����ϴ� ����̹Ƿ�...
    bool IsDragonTransforming() const;
    bool IsGM_TransOn() const;
    bool IsPlayer_TransOn() const;
    bool IsRidingRider() const;
    bool IsObserverModeOn() const;
    bool IsSpreadWings() const;
    bool IsPassConstraint(Character* character, const SkillScriptInfo* skill_script_info) const;
    bool IsPassConstraint(Character* character, const BASE_ABILITYINFO* ability_info) const;
    bool IsPassConstraint(Character* character, const BASE_ITEMINFO* item_info) const;
    //  Special Event
    void On_DragonTransformation(bool turn_on);
    void On_GMTransOn(bool turn_on);
    void On_GMUndeadOn(bool turn_on);
    void On_CharTransOn(bool turn_on);
    void On_GMMaxDamageOn(bool turn_on);
    void On_GMMaxExpOn(bool turn_on);
    void On_GMDropListAllOn(bool turn_on);
    void On_FilterControl(bool is_restrict_skill_target_only_me,
                          bool is_restrict_use_skill_group_and_use_item_type,
                          const MatchCheckCodeFilters* check_filter);

    //  <comment>
    //  -   skill_script_info�� null�̸�, �Ϲ�/��Ÿ�� �������� �ν�
private:
    bool _IsPassConstraint(Character* character, const SkillScriptInfo* skill_script_info) const;
    bool _IsPassConstraint(Character* character, const BASE_ABILITYINFO* ability_info) const;
    bool _IsPassConstraint(Character* character, const BASE_ITEMINFO* item_info) const;
    __DISABLE_COPY(StatusField);
};

//==================================================================================================

inline void StatusField::Clear() {
    field_ = DISABLE_ALL;
}

inline void StatusField::SetField(const POLICY_T flags) {
    field_ = (POLICY_T)flags;
}

inline StatusField::POLICY_T
StatusField::GetField() const {
    return field_;
}

inline void StatusField::AddFlag(const POLICY_T flag) {
    field_ = field_ | (flag);
}

inline void StatusField::RemoveFlag(const POLICY_T flag) {
    field_ = field_ & (~flag);
}

//  ���� ���� ����ϴ� ����̹Ƿ�...
inline bool StatusField::IsDragonTransforming() const {
    return !!(field_ & ENABLE_DRAGON_TRANSFORMING);
}

inline bool StatusField::IsGM_TransOn() const {
    return !!(field_ & ENABLE_GM_TRANSPARENT);
}

inline bool StatusField::IsPlayer_TransOn() const {
    return !!(field_ & ENABLE_CHARACTER_TRANSPARENT);
}

inline bool StatusField::IsRidingRider() const {
    return !!(field_ & ENABLE_SUMMONED_AND_RIDING);
}

inline bool StatusField::IsObserverModeOn() const {
    return !!(field_ & ENABLE_OBSERVER_MODE);
}

// CHANGES: f110315.2L, declared by _NA001955_110210_WING_COSTUME
inline bool StatusField::IsSpreadWings() const {
    return !!(field_ & ENABLE_SPREAD_WINGS);
};

//==================================================================================================
#pragma warning(push)
#pragma warning(disable : 4201)
struct MatchCheckCodeFilterArray
{
    MatchCheckCodeFilterArray()
        : count_(0)
        , it_(&count_)
        , end_(&count_)
    {}

    MatchCheckCodeFilterArray(WORD count, const WORD* it)
        : count_(count)
        , it_(it)
        , end_(it + count)
    {}
    const WORD  count_;
    const WORD* const it_;
    const WORD* const end_;

    WORD IsExist(const WORD code) const;
    //
    __DISABLE_COPY(MatchCheckCodeFilterArray);
};


struct MatchCheckCodeFilters
{
    MatchCheckCodeFilters() {}
    //
    MatchCheckCodeFilterArray   SkillGroupFilter;
    MatchCheckCodeFilterArray   ItemTypeFilter;
    //
    __DISABLE_COPY(MatchCheckCodeFilters);
};
#pragma warning(pop)


inline WORD
MatchCheckCodeFilterArray::IsExist(const WORD code) const
{
    for (const WORD* it = it_; it != end_; ++it)
    {
        if (code == *it) {
            return *it;
        }
    }
    return 0;
}

//==================================================================================================

inline bool
StatusField::IsPassConstraint(Character* character,
                              const SkillScriptInfo* skill_script_info) const
{
    if (character == NULL) {
        return false;
    }
    const POLICY_T checking_field = ENABLE_ONLY_SKILL_TARGET_ME
                                  | ENABLE_FILTER_SKILL_GROUP
                                  | ENABLE_FILTER_ITEM_TYPE;
    if (skill_script_info == NULL) {
        return !(field_ & ENABLE_ONLY_SKILL_TARGET_ME);
    }
    return (field_ & checking_field) ?  _IsPassConstraint(character, skill_script_info)
                                     :  true;
}

inline bool
StatusField::IsPassConstraint(Character* character,
                              const BASE_ABILITYINFO* ability_info) const
{
    if ((character && ability_info) == false) {
        return false;
    }
    const POLICY_T checking_field = ENABLE_ONLY_SKILL_TARGET_ME;

    return (field_ & checking_field) ?  _IsPassConstraint(character, ability_info)
                                     :  true;
}

inline bool
StatusField::IsPassConstraint(Character* character,
                              const BASE_ITEMINFO* item_info) const
{
    if ((character && item_info) == false) {
        return false;
    }
    const POLICY_T checking_field = ENABLE_ONLY_SKILL_TARGET_ME
                                  | ENABLE_FILTER_SKILL_GROUP
                                  | ENABLE_FILTER_ITEM_TYPE;

    return (field_ & checking_field) ?  _IsPassConstraint(character, item_info)
                                     :  true;
}


inline void
StatusField::On_DragonTransformation(bool turn_on)
{
    field_ = turn_on ?  (field_ | ( ENABLE_DRAGON_TRANSFORMING))
                     :  (field_ & (~ENABLE_DRAGON_TRANSFORMING));
}

inline void
StatusField::On_GMTransOn(bool turn_on)
{
    field_ = turn_on ?  (field_ | ( ENABLE_GM_TRANSPARENT))
                     :  (field_ & (~ENABLE_GM_TRANSPARENT));
}

inline void
StatusField::On_GMUndeadOn(bool turn_on)
{
    field_ = turn_on ?  (field_ | ( ENABLE_GM_UNDEAD))
                     :  (field_ & (~ENABLE_GM_UNDEAD));
}

inline  void
StatusField::On_CharTransOn(bool turn_on)
{
    field_ = turn_on ?  (field_ | ( ENABLE_CHARACTER_TRANSPARENT))
                     :  (field_ & (~ENABLE_CHARACTER_TRANSPARENT));
}

inline void
StatusField::On_GMMaxDamageOn(bool turn_on)
{
    field_ = turn_on ?  (field_ | ( ENABLE_GM_MAX_DAMAGE))
                     :  (field_ & (~ENABLE_GM_MAX_DAMAGE));
}

inline void
StatusField::On_GMMaxExpOn(bool turn_on)
{
    field_ = turn_on ?  (field_ | ( ENABLE_GM_MAX_EXP))
                     :  (field_ & (~ENABLE_GM_MAX_EXP));
}

inline void
StatusField::On_GMDropListAllOn(bool turn_on)
{
    field_ = turn_on ?  (field_ | ( ENABLE_GM_DROP_LIST_ALL))
                     :  (field_ & (~ENABLE_GM_DROP_LIST_ALL));
}

inline void
StatusField::On_FilterControl(bool is_restrict_skill_target_only_me,
                              bool is_restrict_use_skill_group_and_use_item_type,
                              const MatchCheckCodeFilters* check_filter)
{
    field_ = is_restrict_skill_target_only_me
           ?    (field_ | ( ENABLE_ONLY_SKILL_TARGET_ME))
           :    (field_ & (~ENABLE_ONLY_SKILL_TARGET_ME));

    field_ = is_restrict_use_skill_group_and_use_item_type
           ?    (field_ | (  ENABLE_FILTER_SKILL_GROUP | ENABLE_FILTER_ITEM_TYPE))
           :    (field_ & (~(ENABLE_FILTER_SKILL_GROUP | ENABLE_FILTER_ITEM_TYPE)));

    filter_ = check_filter;
}

#endif //__GAMESERVER_STATUS_OPERATOR_H

