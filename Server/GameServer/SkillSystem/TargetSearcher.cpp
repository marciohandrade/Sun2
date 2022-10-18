#include "stdafx.h"
#include "./TargetSearcher.h"
// shared include list
//
// local include list
#include "./SearchLinkNode.h"
#include "StatusManager.h"
#include "MovingPlanner.h"
#include "SkillManager.h"

#include "Skill.h"
#include "Ability.h"

//==================================================================================================
namespace ns_object {
//==================================================================================================
;

//==================================================================================================
}; //end of namespace
//==================================================================================================

//==================================================================================================

bool ns_object::SearchOption::IsSatisfiedObjectFindingConditions(
        const nsGrid::ViewBoxCubic* apply_box,
        Character* const actor,
        Character* const target) const
{
    if (arguments.except_object_if_exist == target) {
        return false;
    }
    // NOTE: curling
    const bool himself = (actor == target);
    if ((iff_filter.value & iff_filter.eIFF_CheckSkip) == 0) {
        if (himself && (iff_filter.value & iff_filter.eIFF_AttackerOnly) == 0) {
            return false;
        };
    };
    const WzVector* const target_pos = target->GetPosPtr();
    //
    if (himself == false) {
        if (apply_box && apply_box->IsValidCoordinateXYZ(target_pos) == false) {
            return false;
        };
    };
    if (const StatusField* const status_field = &(target->GetStatusManager()->Status))
    {
        if (this->filter & this->eFilter_SkipObserver) {
            // nothing to do
        }
        else {
            if ((status_field->FIELD & status_field->ENABLE_OBSERVER_MODE) != 0) {
                return false;
            };
        };
        //
        if (this->filter & this->eFilter_SkipTransparent) {
            // nothing to do
        }
        else {
            if ((status_field->FIELD & status_field->ENABLE_CHARACTER_TRANSPARENT) != 0) {
                //return false; //섀도우 은신상태에서 기본공격(스타일)가능..
            };
        }
        //
        if (this->filter & this->eFilter_SkipCantBeAttacked) {
            // nothing to do
        }
        else {
            if (target->CanBeAttacked() == false) {
                return false;
            };
        }
        //
        if (this->filter & this->eFilter_CheckNonPreemptive) {
            if ((status_field->FIELD & status_field->ENABLE_NON_PREEMPTIVE_ATTACK) != 0) {
                return false;
            };
        }
        else {
            // nothing to do
        }
        //
        if (this->filter & this->eFilter_SkipDead) {
            // nothing to do
        }
        else {
            if ((target->link_node_.select.object_node.object_type & CHARACTER_OBJECT) !=
                CHARACTER_OBJECT)
            {
                return false;
            }
            const SearchNode::CharacterNode& char_node = target->link_node_.select.character_node;
            if (char_node.current_hp == NULL || *char_node.current_hp == 0) {
                return false;
            }
        }
        //
        if (this->filter & this->eFilter_SkipTileCheck) {
            // nothing to do
        }
        else {
            WORD skip_check_tile_attr_for_target = 0;
            if (target->IsEqualObjectKind(MAPNPC_OBJECT))  {
                skip_check_tile_attr_for_target = PTA_NO_WALK;
            }

            if (nsAI::PlanEventObject::AcceptableTarget(target, PTA_SAFETY_ZONE,
                    skip_check_tile_attr_for_target) == false)
            {
                return false;
            }
        }
        //
    };

    // special exception
    // ...
#if SUN_CODE_BACKUP
    // TODO: change 'IsCoincidentWithTargetType'
    // 타겟 타입 체크
    if (actor->IsCoincidentWithTargetType(target, arguments.search_target_type) == false) {
        return false;
    }
#endif
    if (himself == false && ((filter & this->eFilter_SkipRangeCheck) == 0))
    {
        // 스킬 사거리 체크
        if (arguments.attack_ranges != 0.0f)
        {
            if (actor->CheckSkillRange(target, *target_pos, arguments.attack_ranges) == false) {
                return false;
            }
        }

        // 스킬 Area 체크
        if (actor->CheckArea(*target_pos, const_cast<WzVector&>(*arguments.application_center_pos),
               arguments.forms_of_application, arguments.ranges_of_application) == false)
        {
            return false;
        }
    };
    return true;
}

// NOTE: f110309.1L, this method is generally used by using embedded IFF_Filter rule
// this method check whether a target is friend or enemy.
bool ns_object::SearchOption::IsSatisfiedObjectIFFRelations(
        Character* const actor, Character* const target,
        const RelationHint* cached_relation, RelationHint* result_relation) const
{
    if (iff_filter.validated == false)
    {
        if (result_relation) {
            result_relation->checked = false;
            result_relation->relation = USER_RELATION_NEUTRALIST;
        };
        return false;
    };
    //
    const eUSER_RELATION_TYPE relation
        = (cached_relation && cached_relation->checked)
        ?   cached_relation->relation
        :   actor->IsFriend(target);
    if (result_relation) {
        result_relation->checked = true;
        result_relation->relation = relation;
    };

    const IFF_Filter::value_type iff_value = iff_filter.value;
    if (iff_value & iff_filter.eIFF_CheckSkip) {
        return true;
    };
    if (iff_value & (iff_filter.eIFF_Friend | iff_filter.eIFF_Enemy))
    {
        if (iff_value & iff_filter.eIFF_Friend) {
            if (relation != USER_RELATION_FRIEND) {
                return false;
            }
        }
        else if (iff_value & iff_filter.eIFF_Enemy) {
            if (relation != USER_RELATION_ENEMY) {
                return false;
            }
        };
        //
    };
    return true;
}

// NOTE: f110309.1L, apply IFF_Filter rule. this method is used by Skill abilities
bool ns_object::SearchOption::IsSatisfiedObjectAbilityConditions(
        Character* const actor, Character* const target,
        const BASE_ABILITYINFO* base_ability_info) const
{
    const IFF_Filter& iff_filter = base_ability_info->iff_filter;
    if (iff_filter.validated == false) {
        return false;
    };
    //
    const IFF_Filter::value_type iff_value = iff_filter.value;
    if (iff_value & iff_filter.eIFF_CheckSkip) {
        return true;
    };

    const eOBJECT_TYPE target_object_type = target->link_node_.select.object_node.object_type;
    if (iff_value & iff_filter.eIFF_Player) {
        if ((target_object_type & PLAYER_OBJECT) != PLAYER_OBJECT) {
            return false;
        }
    };
    if (iff_value & iff_filter.eIFF_Corpse)
    {
        if ((target_object_type & CHARACTER_OBJECT) != CHARACTER_OBJECT)
        {
            assert(!"this routine can't accept non-character");
            return false;
        }
        const SearchNode::CharacterNode& char_node = target->link_node_.select.character_node;
        if (char_node.current_hp == NULL || *char_node.current_hp != 0) {
            return false;
        }
        if ((iff_value & iff_filter.eIFF_Enemy) == 0 &&
            (actor->CanResurrection(target) == false))
        {
            return false;
        }
    };

    return true;
};

//==================================================================================================

// NOTE: f110521.3L, changed the damage box ranges according to the attack range formation
bool ns_object::SearchOption::GetApplicationRangeBox(
    nsGrid::ViewBoxCubic* const damage_box, const float z_axis_tolerant_value) const
{
    switch (this->option)
    {
    case this->eOption_LookAround:
    case this->eOption_NpcAI:
    case this->eOption_SingleSearch:
    case this->eOption_SelectBestFit:
        ;{
            WzVector area_radius = {
                arguments.ranges_of_application,
                arguments.ranges_of_application,
                z_axis_tolerant_value
            };
            damage_box->SetInit3D_DownsideZ(arguments.application_center_pos, &area_radius);
        };
        return true;
    case this->eOption_SkillBase:
        ;{
            const ns_object::SearchOption::SkillBase& root = \
                static_cast<const ns_object::SearchOption::SkillBase&>(*this);
            assert(root.skill && root.skill->GetSkillBaseInfo());
            const SkillScriptInfo* base_skill_info = root.skill->GetSkillBaseInfo();
            if (base_skill_info->m_byAttRangeform != SRF_PIERCE)
            {
                WzVector area_radius = {
                    arguments.ranges_of_application,
                    arguments.ranges_of_application,
                    z_axis_tolerant_value
                };
                damage_box->SetInit3D_DownsideZ(
                    arguments.application_center_pos, &area_radius);
            }
            else
            {
                WzVector* const min_vec = &damage_box->Cubic.m_wvMin;
                WzVector* const max_vec = &damage_box->Cubic.m_wvMax;
                *min_vec = *arguments.start_pos;
                *max_vec = *arguments.application_center_pos;
                if (min_vec->x > max_vec->x) {
                    std::swap(min_vec->x, max_vec->x);
                }
                min_vec->x -= arguments.ranges_of_application;
                max_vec->x += arguments.ranges_of_application;
                if (min_vec->y > max_vec->y) {
                    std::swap(min_vec->y, max_vec->y);
                }
                min_vec->y -= arguments.ranges_of_application;
                max_vec->y += arguments.ranges_of_application;
                if (min_vec->z > max_vec->z) {
                    std::swap(min_vec->z, max_vec->z);
                }
                min_vec->z -= z_axis_tolerant_value;
                max_vec->z += 5.0f;
            }
        };
        return true;
    };
    assert(!"can't entered at this point!");
    return false;
}

//==================================================================================================

const ulong ns_object::SearchOption::LookAround::kFilterValue
    = ns_object::SearchOption::eFilter_None;

//==================================================================================================

const ulong ns_object::SearchOption::NpcAI::kFilterValue
    = ns_object::SearchOption::eFilter_CheckNonPreemptive;

ns_object::SearchOption::NpcAI::NpcAI(NPC* npc)
    : SearchOption(eOption_NpcAI, kFilterValue)
    , actor(npc)
{
    iff_filter.validated = true;
    iff_filter.value = iff_filter.eIFF_Enemy;
    max_number_of_selected_chars = 5;
    ZeroMemory(&arguments, sizeof(arguments));
    ZeroMemory(&working, sizeof(working));
    //
    arguments.search_target_type = SKILL_TARGET_ENEMY;
    arguments.start_pos = actor->GetPosPtr();
    arguments.application_center_pos = arguments.start_pos;
    //arguments.attack_ranges = 0.0f;
    // tolerent area 'kAiCommonInfo.m_fRANGE_TOLERANCE'
    arguments.ranges_of_application = actor->GetSightRange();
    arguments.forms_of_application = SRF_FOWARD_360;
    //
    this->validated = true;
}

ns_object::SearchOption::eContinueResult
ns_object::SearchOption::NpcAI::AppendToResultIfIsSatisfiedObject(
    NPC* const actor, Character* const target, result_type* result) const
{
    __UNUSED(actor);
    const int number = result->number_of_selected_chars;
    const bool in_range =
        (number < _countof(result->targets) && number < this->max_number_of_selected_chars);
    if (in_range == false) {
        return this->eStop;
    };
    result->targets[result->number_of_selected_chars++] = target;
    return this->eContinue;
}

//==================================================================================================

const ulong ns_object::SearchOption::SelectBestFit::kFilterValue
    = ns_object::SearchOption::eFilter_None;

ns_object::SearchOption::eContinueResult
ns_object::SearchOption::SelectBestFit::AppendToResultIfIsSatisfiedObject(
    Character* const actor, Character* const target, result_type* result) const
{
    __UNUSED((actor, target, result));
    assert(!"function not implementation");
    return this->eStop;
}

//==================================================================================================
//==================================================================================================

const ulong ns_object::SearchOption::SingleSearch::kFilterValue
    = ns_object::SearchOption::eFilter_None;

ns_object::SearchOption::SingleSearch::SingleSearch(
    int max_number_of_selected_characters,
    Character* input_actor,
    Character* input_main_target_if_exist_it,
    const WzVector* input_specific_application_center_pos_if_main_target_is_not_used_to_pos,
    //
    float attack_ranges,
    float ranges_of_application,
    eSKILL_AREA_TYPE forms_of_application)
    //
    : SearchOption(eOption_SingleSearch, kFilterValue)
    , actor(input_actor)
{
    bool failed = false;
    iff_filter.validated = true;
    iff_filter.value = iff_filter.eIFF_Enemy;
    this->max_number_of_selected_chars = max_number_of_selected_characters;
    ZeroMemory(&arguments, sizeof(arguments));
    ZeroMemory(&working, sizeof(working));
    //
    arguments.search_target_type = SKILL_TARGET_ENEMY;
    arguments.start_pos = actor->GetPosPtr();
    //arguments.application_center_pos - next
    arguments.attack_ranges = attack_ranges;
    arguments.ranges_of_application = ranges_of_application;
    arguments.forms_of_application = forms_of_application;
    //
    if (input_specific_application_center_pos_if_main_target_is_not_used_to_pos) {
        arguments.application_center_pos = \
            input_specific_application_center_pos_if_main_target_is_not_used_to_pos;
    }
    else if (input_main_target_if_exist_it) {
        arguments.application_center_pos = input_main_target_if_exist_it->GetPosPtr();
    }
    else {
        failed = true;
    };

    if (this->max_number_of_selected_chars == 0) {
        failed = true;
    }

    if (failed == false) {
        this->validated = true;
    }
    //
    working.main_target = input_main_target_if_exist_it;
}

//
bool ns_object::SearchOption::SingleSearch::PrefetchResult(result_type* const result) const
{
    result->number_of_selected_chars = 0;
    //
    if (this->validated == false) {
        return false;
    };
    //
    Character* const * character_begin = result->targets;
    Character** character_it = result->targets;
    // sequential processing
    if (working.main_target && this->actor != working.main_target)
    {
        if (this->IsSatisfiedObjectFindingConditions(NULL, this->actor, working.main_target)) {
            *character_it++ = working.main_target;
            const_cast<Arguments&>(arguments).except_object_if_exist = working.main_target;
        }
    }
    //
    result->number_of_selected_chars = static_cast<int>(character_it - character_begin);
    //
    return true;
}

ns_object::SearchOption::eContinueResult
ns_object::SearchOption::SingleSearch::AppendToResultIfIsSatisfiedObject(
    Character* const actor, Character* const target, result_type* result) const
{
    __TOUCHED(actor);
    const int number = result->number_of_selected_chars;
    const bool in_range =
        (number < _countof(result->targets) && number < this->max_number_of_selected_chars);
    if (in_range == false) {
        return this->eStop;
    };
    // assertion: (target != NULL)
    if (working.main_target == target) {
        return this->eContinue;
    }
    result->targets[result->number_of_selected_chars++] = target;
    return this->eContinue;
}

//==================================================================================================

const ulong ns_object::SearchOption::SkillBase::kFilterValue
    = SearchOption::eFilter_SkipRangeCheck
    | SearchOption::eFilter_SkipTileCheck;

static char skill_search_option_block[sizeof(ns_object::SearchOption::SkillBase)];

ns_object::SearchOption::SkillBase*
ns_object::SearchOption::SkillBase::CreateInstance(Skill* input_skill)
{
    SearchOption::SkillBase* instance =
        new (skill_search_option_block) SearchOption::SkillBase(input_skill);
    instance->instanced = true;
    return instance;
}

void ns_object::SearchOption::SkillBase::DestroyInstance()
{
    SearchOption::SkillBase* instance =
        reinterpret_cast<SearchOption::SkillBase*>(skill_search_option_block);
    instance->instanced = false;
}

ns_object::SearchOption::SkillBase::SkillBase(Skill* input_skill)
    : SearchOption(eOption_SkillBase, kFilterValue)
    , skill(input_skill)
    , actor(input_skill->GetOwner())
{
    bool failed = false;
    ZeroMemory(&arguments, sizeof(arguments));

    iff_filter.validated = false;
    iff_filter.value = iff_filter.eIFF_None;
    //
    const SkillInfo* const skill_info_data = skill->GetSkillInfo();
    assert(skill_info_data->root_skill_info_); // this value is tested in SkillManager
    bool is_skill = skill_info_data->root_skill_info_->IsSkill();
    //working.is_skill = skill_info_data->root_skill_info_->IsSkill();
    if (is_skill)
    {
        const BASE_SKILLINFO* base_skill_info =
            static_cast<const BASE_SKILLINFO*>(skill_info_data->root_skill_info_);
        this->max_number_of_selected_chars = min(base_skill_info->m_byMaxTargetNo, MAX_TARGET_NUM);
        arguments.search_target_type = static_cast<eSKILL_TARGET_TYPE>(base_skill_info->m_byTarget);
        arguments.attack_ranges = (base_skill_info->m_wSkillRange / 10.0f);
        arguments.ranges_of_application = (base_skill_info->m_wSkillArea / 10.0f);
        arguments.forms_of_application =
            static_cast<eSKILL_AREA_TYPE>(base_skill_info->m_byAttRangeform);
    }
    else
    {
        failed = true;
    }
  #if SUN_CODE_BACKUP
    else
    {
        const BASE_STYLEINFO* base_style_info =
            static_cast<const BASE_STYLEINFO*>(skill_info_data->root_skill_info_);
        this->max_number_of_selected_chars = min(base_style_info->m_wMaxTargetNo, MAX_TARGET_NUM);
        arguments.search_target_type = SKILL_TARGET_ENEMY;
        arguments.attack_ranges = actor->GetAttackRange();
        arguments.ranges_of_application = (base_style_info->m_fStyleArea);
        arguments.forms_of_application =
            static_cast<eSKILL_AREA_TYPE>(base_style_info->m_byAttRangeform);
    }
  #endif
    //
    arguments.start_pos = actor->GetPosPtr();
    // application_center_pos is not checked. because many rules prevent target_pos is invalid
    arguments.application_center_pos = &skill_info_data->main_target_pos_;
    //
    ZeroMemory(&working, sizeof(working));

    int registered = 0;
    if (failed == false)
    {
        const Skill::AbilityContainer::Node* ability_it = skill->abilities_.nodes_,
            * const ability_end = &skill->abilities_.nodes_[_countof(skill->abilities_.nodes_)];

        for (int index = 0;
             index < skill->abilities_.number_of_abilities_ && ability_it != ability_end;
             ++index, ++ability_it)
        {
            Ability* const ability = ability_it->ability_;

            const eABILITY_TYPE ability_type = ability->GetAbilityType();
            // changes a concept. designed by __NA_S00015_20080828_NEW_HOPE_SHADOW
            if (ability_type == eABILITY_TYPE_EFFECT || ability_type == eABILITY_TYPE_MANUAL) {
                continue;
            };
            working.need_process_abilities[index] = true;
            const BASE_ABILITYINFO* base_ability_info = ability->GetBaseAbilityInfo();
            const IFF_Filter& ability_filter = base_ability_info->iff_filter;
            if (ability_filter.value & ability_filter.eIFF_Corpse) {
                this->filter |= eFilter_SkipDead;
            }
            if (this->max_number_of_selected_chars == 0 &&
                (ability_filter.value & (ability_filter.eIFF_Enemy | ability_filter.eIFF_One)))
            {
                this->max_number_of_selected_chars = 1;
            }
            working.ability_infos[index] = base_ability_info;
            ++registered;
        };
    };
    if (this->max_number_of_selected_chars == 0) {
        failed = true;
    }
    //
    if (failed || registered == 0) {
        this->validated = false;
    }
}

#if SUN_CODE_BACKUP
bool ns_object::SearchOption::SkillBase::PrefetchResult(result_type* const result)
{
    if (this->validated == false) {
        return false;
    };
    //
    const BASE_SKILLINFO* const base_skill_info = skill->base_skill_info_;
    //
    //

    //
    return true;
}

ns_object::SearchOption::eContinueResult
ns_object::SearchOption::SkillBase::AppendToResultIfIsSatisfiedObject(
    Character* const actor, Character* const target, result_type* result) const
{
    assert(!"function not implementation");
    return this->eStop;
}
#endif
//==================================================================================================
//==================================================================================================
//==================================================================================================

