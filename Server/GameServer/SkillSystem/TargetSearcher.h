#pragma once
#ifndef GAMESERVER_TARGET_SEARCHER_H
#define GAMESERVER_TARGET_SEARCHER_H

//==================================================================================================

class Skill;

namespace ns_object {
;

struct RelationHint
{
    bool checked;
    eUSER_RELATION_TYPE relation;
};

//-------------------------------------
// A : Actor
// T : main Target
// X : apply target or range (targets)
// R : Range (A<->T)
// r : radius in of range
// * : boundary
//-------------------------------------
// ##1:
//  *  *  *  *  *
//  *  *  *  *  *
//  *  *  T  *  * 
//  *  *  *  *  *
//  *  *  *  *  *
//-------------------------------------
// ##2:
//  *  *  *  *  *
//  *  X  X  X  *
//  *  X  T  X  *
//  *  X  X  X  *
//  *  *  *<-r->*
//-------------------------------------
//  *  *  *  *  *
//  *  X  X  X  *
//  *  X  X  X  *
//  *  X  X  X  *
//  *  *  *<-r->*
//-------------------------------------

struct SearchOption
{
    // ##1: default range select based on center position like a NpcAI
    enum eOptions
    {
        eOption_None,
        eOption_LookAround,
        eOption_NpcAI,
        eOption_SingleSearch,
        eOption_SkillBase,
        eOption_SelectBestFit,
    };
    enum eFilter
    {
        eFilter_None = 0,
        //
        eFilter_SkipCantBeAttacked = 1 << 0, //
        eFilter_SkipObserver       = 1 << 1, //
        eFilter_SkipTransparent    = 1 << 2, //
        eFilter_SkipDead           = 1 << 4, //
        eFilter_SkipTileCheck      = 1 << 5, // SkillBase
        eFilter_SkipRangeCheck     = 1 << 7, // SkillBase
        //
        eFilter_CheckNonPreemptive = 1 << 10, // NpcAI
        //
    };
    enum eContinueResult {
        eStop,
        eContinue,
    };

    struct LookAround;
    struct NpcAI;
    struct SelectBestFit;
    struct SingleSearch;
    struct SkillBase;

    SearchOption(const eOptions& search_option, const ulong& search_filter);
    // NOTE: f110521.3L, changed the damage box ranges according to the attack range formation
    bool GetApplicationRangeBox(\
        nsGrid::ViewBoxCubic* const damage_box, const float z_axis_tolerant_value) const;
    // NOTE: this method find accetable objects fit in conditions.
    bool IsSatisfiedObjectFindingConditions(
        const nsGrid::ViewBoxCubic* apply_box,
        Character* const actor, Character* const target) const;
    // NOTE: f110309.1L, this method is generally used by using embedded IFF_Filter rule
    // this method check whether a target is friend or enemy.
    bool IsSatisfiedObjectIFFRelations(
        Character* const actor, Character* const target,
        const RelationHint* cached_relation, RelationHint* result_relation) const;
    // NOTE: f110309.1L, apply IFF_Filter rule. this method is used by Skill abilities
    bool IsSatisfiedObjectAbilityConditions(
        Character* const actor, Character* const target,
        const BASE_ABILITYINFO* base_ability_info) const;
    //
    // Fields...
    const eOptions option;
    mutable ulong filter;
    mutable IFF_Filter iff_filter;
    mutable int max_number_of_selected_chars;
    mutable bool validated;
    struct Arguments {
        //
        eSKILL_TARGET_TYPE search_target_type;
        const WzVector* start_pos;
        const WzVector* application_center_pos;
        float attack_ranges;
        float ranges_of_application;
        eSKILL_AREA_TYPE forms_of_application;
        //
        Character* except_object_if_exist;
        //
    } arguments;
    //
    __DISABLE_COPY(SearchOption);
};

inline SearchOption::SearchOption(
    const eOptions& search_option, const ulong& search_filter)
    : option(search_option)
    , filter(search_filter)
    , max_number_of_selected_chars(0)
    , validated(false)
{
    ZeroMemory(&arguments, sizeof(arguments));
}

struct SearchResult
{
    struct LookAround;
    struct NpcAI;
    struct SelectBestFit;
    struct SingleSearch;
    struct SkillBase;
};

struct SearchOption::LookAround : public SearchOption
{
    typedef SearchResult::LookAround result_type;
    static const ulong kFilterValue;
    //
    explicit LookAround(eSKILL_TARGET_TYPE target_type)
        : SearchOption(eOption_LookAround, kFilterValue) { __TOUCHED(target_type); }
    //
    __DISABLE_COPY(LookAround);
};

struct SearchOption::NpcAI : public SearchOption
{
    typedef SearchResult::NpcAI result_type;
    static const ulong kFilterValue;
    //
    NpcAI(NPC* npc);

    eContinueResult AppendToResultIfIsSatisfiedObject(
        NPC* const actor, Character* const target, result_type* result) const;
    //
    NPC* const actor;
    struct Working {
        DWORD exist_record[MAX_TARGET_NUM];
    } working;
    //
    __DISABLE_COPY(NpcAI);
};

struct SearchOption::SelectBestFit : public SearchOption
{
    typedef SearchResult::SelectBestFit result_type;
    static const ulong kFilterValue;
    //
    SelectBestFit() : SearchOption(eOption_SelectBestFit, kFilterValue) {}

    eContinueResult AppendToResultIfIsSatisfiedObject(
        Character* const actor, Character* const target, result_type* result) const;
    //
    __DISABLE_COPY(SelectBestFit);
};

// NOTE: simple enemies search
struct SearchOption::SingleSearch : public SearchOption
{
    typedef SearchResult::SingleSearch result_type;
    static const ulong kFilterValue;
    //
    SingleSearch(
        int max_number_of_selected_characters,
        Character* input_actor,
        Character* input_main_target_if_exist_it,
        const WzVector* input_specific_application_center_pos_if_main_target_is_not_used_to_pos,
        //
        float attack_ranges,
        float ranges_of_application,
        eSKILL_AREA_TYPE forms_of_application);
    //
    bool PrefetchResult(result_type* const result) const;
    eContinueResult AppendToResultIfIsSatisfiedObject(
        Character* const actor, Character* const target, result_type* result) const;
    //
    Character* const actor;
    struct Working {
        Character* main_target;
    } working;
    //
    __DISABLE_COPY(SingleSearch);
};

// this search option accept only skill not style.
// if you want search targets by style, use SingleSearch
struct SearchOption::SkillBase : public SearchOption
{
    typedef SearchResult::SkillBase result_type;
    static const ulong kFilterValue;
    //
    static SearchOption::SkillBase* CreateInstance(Skill* input_skill);
    static void DestroyInstance();
    //
    //bool PrefetchResult(result_type* const result);
    //eContinueResult AppendToResultIfIsSatisfiedObject(
    //    Character* const actor, Character* const target, result_type* result) const;
    //
    Skill* const skill;
    Character* const actor;
    bool instanced;
    struct Working {
        Character* main_target;
        //bool include_actor;
        bool need_process_abilities[MAX_ABILITY_NUM];
        const BASE_ABILITYINFO* ability_infos[MAX_ABILITY_NUM];
        RelationHint hint[MAX_TARGET_NUM];
    } working;

private:
    SearchOption::SkillBase(Skill* input_skill);
    //
    __DISABLE_COPY(SkillBase);
};

//==================================================================================================

struct SearchResult::LookAround
{
};

struct SearchResult::NpcAI
{
    int number_of_selected_chars;
    Character* targets[MAX_TARGET_NUM];
};

struct SearchResult::SelectBestFit
{
};

struct SearchResult::SingleSearch
{
    int number_of_selected_chars;
    Character* targets[MAX_TARGET_NUM];
};

struct SearchResult::SkillBase
{
    bool multi_select;
};

}; //end of namespace

#endif //GAMESERVER_TARGET_SEARCHER_H