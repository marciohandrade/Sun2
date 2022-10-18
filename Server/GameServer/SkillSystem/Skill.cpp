#include "stdafx.h"
#include "Skill.h"

#include <Const.h>
#include <Struct.h>
#include <PublicMath.h>
#include <PacketStruct_CG.h>
#include <SkillInfoParser.h>

#include "AITypes.h"

#include "FieldInfo.h"
#include "GameField.h"
#include "Character.h"
#include "Player.h"
#include "NPC.h"

#include "Ability.h"
#include "AbilityFactory.h"
#include "StatusManager.h"
#include "ChaoState.h"
#include "MovingPlanner.h"

#ifdef _NA_008588_20151202_MODIFY_MOVEING_SKILL_LOGIC
#include "SkillSystem/CSSyncTuner.h"
#endif //_NA_008588_20151202_MODIFY_MOVEING_SKILL_LOGIC

#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
#include "CharacterFormula.h"
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

//==================================================================================================
//  (History)
//  - 09/07/09, waverix, 로직 정리

//==================================================================================================
//
RESULTMSG Skill::result_msg_array_[MAX_TARGET_NUM];
RESULTMSG Skill::result_effect_msg_array_[5];
//
void Skill::InitResultMsg()
{
    number_of_hit_targets_ = 0;
    number_of_effects_ = 0;
}


//==================================================================================================
//
//
Skill::~Skill()
{
    Release();
}

void
Skill::Release()
{
    if (is_applied_ether_bullet_option_ && owner_)
    {
        ((Player*)owner_)->m_EtherManager.EndEtherEmissionDevice(this);
        ether_combo_count_ = 0;
    }
    RemoveAbilities();
}


// represent a kind of 'Init'
void Skill::SetupDefault(Character* owner,
                         const SkillInfo* skill_info, const ROOT_SKILLINFO* root_skill_info)
{
    ZeroMemory(field_block_start_, static_cast<size_t>(field_block_end_ - field_block_start_));
    // step#1 : first operation block
    ;{
        owner_ = owner;
        if (owner) {
            game_field_ = owner->GetField();
        }
        if (root_skill_info != NULL)
        {
            if (root_skill_info->IsSkill()) {
                base_skill_info_ = static_cast<const SkillScriptInfo*>(root_skill_info);
            }
            else {
                base_style_info_ = static_cast<const BASE_STYLEINFO*>(root_skill_info);
            }
        }
        skill_info_data_ = *skill_info;
    };
    //
    //
    bonus_ability_effect_.Init(); // ... real operation is unless, clear memrory ...

    // step#2 : timer control using polymorphism interface
    ;{
        // ExecuteInterval 셋팅
        SetExecuteInterval(); // virtual
        ConfigureAnimationDelay();

        // ExecuteTick 셋팅
        execute_tick_ = GetTickCount() + GetExecuteInterval();
    };
    // pseudo : if is_passive_skill is false, that means an Active skill or a Normal(Style) skill
    const bool is_passive_skill = base_skill_info_ ?
        (base_skill_info_->m_bySkillType == SKILL_TYPE_PASSIVE) : false;
    //
    // step#3 : safety zone control, moved from Init(...)
    // (CHANGES) (REF:#950) (WAVERIX) owner_가 없어지더라도 발동 시점의 위치는 한번 설정 후
    // 변하지 않는다. 시전 시점의 위치이므로, 따라서 구해둔다.
    // Character::GetTileAtt로 구하지 않는다. GetTileAtt는 Progress완료 이후 정확한 값이 설정된다.
    is_invoked_in_safety_area_ = false;
    if (is_passive_skill == false && game_field_)
    {
        const int invoked_tile = owner->PlanEvent->GetTile();
        if ((invoked_tile < 0) == false)
        {
            const WORD invoked_tile_attr = game_field_->GetMovingPlanner()->GetAttribute(invoked_tile);
            if (invoked_tile_attr & PTA_SAFETY_ZONE) {
                is_invoked_in_safety_area_ = true;
            }
        }
    }

    // step#4 : ether device control block
    ether_combo_count_ = tagEtherBulletInfo::eETHER_DEVICE_COMBO_NORMAL;
    if (owner_ && owner_->IsEqualObjectKind(PLAYER_OBJECT))
    {
        CEtherEmissionDevice& rDevice = static_cast<Player*>(owner_)->m_EtherManager;
        ether_combo_count_ = rDevice.StartEtherEmissionDevice(this, skill_info_data_.main_target_key_);
    }
}

void
Skill::Init(Character* owner, const SkillInfo* skill_info, const ROOT_SKILLINFO* root_skill_info)
{
    SetupDefault(owner, skill_info, root_skill_info);
    //
    // skills = { instant, delayed }
    AddAbilities(); // register abilities
    SetAttribution();
}

void Skill::ConfigureAnimationDelay(const DWORD /*empty_arg*/)
{
    Character* const owner = GetOwner();
    if (owner == NULL || owner->GetField() == NULL)
    {
        return;
    }
    if (base_skill_info_ == NULL)
    {
        return;
    }    
    const WORD delay_tick = base_skill_info_->m_CSSyncDelay;
    if (delay_tick == 0)
    {
        return;
    }   
    if (owner->IsEqualObjectKind(PLAYER_OBJECT))
    {
        const Player* const player = static_cast<Player*>(owner);
        StatusManager* const status_manager = player->GetStatusManager();
        (status_manager->AnimationDelay).SetAnimationDelay(this, delay_tick);
    }
}


void
Skill::EndExecute()
{
    if (owner_->IsEqualObjectKind(PLAYER_OBJECT))
    {
        Player* pPlayer = (Player*)owner_;

        pPlayer->SetForceAttack(false);
        //SUNLOG(eDEV_LOG, "[Skill::EndExecute()] Name = %s, SkillCode = %d, Force Attack is false",
        //       pPlayer->GetCharName(), GetSkillCode());
    }
}

void
Skill::AddAbilities()
{
    // CHANGES, f100821.1L, change an ability hash table to a fixed size array.
    if (abilities_.number_of_abilities_ != 0)
    {
        SUNLOG(eCRITICAL_LOG, _T("|["__FUNCTION__"]|Msg=MUST be zero|number_of_abilities=%d|"),
               abilities_.number_of_abilities_);
        RemoveAbilities();
    };
    // CHANGES, f100831.4L
    const uint8_t event_code = this->skill_info_data_.event_code_;
    AbilityFactory* const ability_factory = AbilityFactory::Instance();
    //
    AbilityContainer::Node* it = abilities_.nodes_;
    base_skill_info_->SetFirst();
    while (const BASE_ABILITYINFO* ability_info = base_skill_info_->GetNext())
    {
        int index = static_cast<int>(it - abilities_.nodes_);
        assert(index == ability_info->m_byIndex && index < _countof(abilities_.nodes_));
        if (index >= _countof(abilities_.nodes_)) {
            break;
        }
        const eABILITY ability_id = static_cast<eABILITY>(ability_info->m_wAbilityID);
        // Ability 생성
        Ability* ability = ability_factory->AllocAbility(static_cast<WORD>(ability_id), GetSkillType());
        if (!ability) {
            SUNLOG(eCRITICAL_LOG, _T("["__FUNCTION__"] #1 %d, %d, %d", 
                GetSkillCode(), index, ability_id));
        };
        assert(ability);
        ability->Init(this, ability_info);
        ability->SetEventCode(event_code);
        //
        it->ability_id_ = ability_id;
        it->ability_ = ability;
        abilities_.number_of_abilities_ = (index + 1); // used by number
        ++it;
    }
}

void
Skill::RemoveAbilities()
{
    AbilityFactory* const ability_factory = AbilityFactory::Instance();
    AbilityContainer::Node* it = abilities_.nodes_,
                          * const end = &abilities_.nodes_[_countof(abilities_.nodes_)];
    for (int index = 0; index < abilities_.number_of_abilities_ && it != end; ++index, ++it)
    {
        Ability* ability = it->ability_;
        ability_factory->FreeAbility(ability);
    }
    ZeroMemory(&abilities_, sizeof(abilities_));
}

void
Skill::DecreaseHPMP()
{
    // use only a kind of Skill
    if (base_skill_info_ == 0)
    {
        SUNLOG(eCRITICAL_LOG, "[Skill::DecreaseMP] base_skill_info_ is NULL!");
        return;
    }

    // 플레이어의 HP, MP를 감소시킨다.
    // formula
    // MP spend = value * ratio + spend_change_value
    int mp_spend = base_skill_info_->m_wMPSpend;
    ;   mp_spend = static_cast<int>(mp_spend * (1 + owner_->GetMPSpendIncRatio()));
    ;   mp_spend += owner_->GetMPSpendIncValue();
    ;   mp_spend = max(0, mp_spend);

    owner_->DecreaseHP(base_skill_info_->m_wHPSpend);
    owner_->DecreaseMP(mp_spend);
}

void
Skill::SetAttribution()
{
    switch(GetSkillClassCode())
    {
    case eSKILL_DOUBLE_ATTACK:
    case eSKILL_AIRBLOW:
    case eSKILL_BATTLERHONE:
        skill_attribution_ |= SKILL_ATTRIBUTION_TARGETFLYING;
        break;
    case eSKILL_ILLUSION_DANCE:
        skill_attribution_ |= SKILL_ATTRIBUTION_TARGETSTOP;
        break;
    }
}

BOOL
Skill::CheckMainTarget(BattleResult* const detailed_result)
{
    BattleResult battle_result = { 0, };
    BattleResult* result_handle = detailed_result ? detailed_result : &battle_result;
    result_handle->result_code = RC::RC_BATTLE_SUCCESS;
    //
    if ((game_field_ && base_skill_info_) == 0)
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] GameField [%u] BaseSkillInfo [%u]",
               !!game_field_, !!base_skill_info_);
        result_handle->result_code = RC::RC_BATTLE_INVALID_MAINTARGET;
        return false;
    }

    const eSKILL_TARGET_TYPE skill_target_type = eSKILL_TARGET_TYPE(base_skill_info_->m_byTarget);

    // 소환스킬은 메인타겟이 없다.
    if (skill_target_type == SKILL_TARGET_SUMMON)
        return true;

    // 메인타겟 좌표를 설정한다.
    float fSkillRange = base_skill_info_->m_wSkillRange / 10.0f;

    Character* pMainTarget = NULL;
    if (skill_target_type == SKILL_TARGET_ME)
    {
        skill_info_data_.main_target_key_ = owner_->GetObjectKey();
        skill_info_data_.main_target_pos_ = skill_info_data_.cur_pos_;
    }
    else if (skill_target_type == SKILL_TARGET_AREA)
    {
        //_NA001385_20090924_DOMINATION_FINDPATH_ATTACK
        if (false == owner_->CheckAttackFindPath(skill_info_data_.main_target_pos_)) {
            result_handle->result_code = RC::RC_BATTLE_PLAYER_STATE_WHERE_CANNOT_ATTACK_ENEMY;
            return false;
        }
    }
    else if (base_skill_info_->m_byAttRangeform == SRF_PIERCE)
    {   // 관통스킬인 경우 m_wvMainTargetPos을 그대로 쓴다.
        // 관통스킬은 연출상 1m 정도 더 나가도록 한다.
        fSkillRange += 1.f;
    }
    else
    {
        pMainTarget = game_field_->FindCharacter(skill_info_data_.main_target_key_);
        if (pMainTarget)
            skill_info_data_.main_target_pos_ = *pMainTarget->GetPosPtr();
    }

    // 메인타겟에 대해 스킬 사거리를 체크한다.
    if (pMainTarget)
    {
        BOOL is_skill_range_ok =
            owner_->CheckSkillRange(pMainTarget, skill_info_data_.main_target_pos_, fSkillRange);

        if (is_skill_range_ok == 0) {
            result_handle->result_code = RC::RC_BATTLE_OUT_OF_RANGE;
            return false;
        }
    }

    return true;
}

BOOL
Skill::IsMaxTargetNum(Character* pTarget)
{
    if (pTarget == owner_)
        is_target_owner_ = true;

    if (!base_skill_info_->m_byMaxTargetNo)
        return false;

    int iCheckCount = number_of_hit_targets_;
    if (is_target_owner_)
        --iCheckCount;

    if (iCheckCount >= base_skill_info_->m_byMaxTargetNo)
        return true;

    return false;
}

//_NA000000_110210_ENHANCED_TARGET_SEARCH_ALGORITHM
// NOTE: f110309.1L, this method is implemented for (Instant/Delayed) skill processing.
bool Skill::ExecuteSkill()
{
    using namespace ns_object;

    if ((game_field_ && base_skill_info_) == false)
    {
        SUNLOG(eCRITICAL_LOG, _T("["__FUNCTION__"] GameField [%u] BaseSkillInfo [%u]"),
               !!game_field_, !!base_skill_info_);
        return false;
    }
    //------------------------------------------------------------------------------------------
    InitResultMsg();

    assert(number_of_hit_targets_ == 0 && number_of_effects_ == 0);
    //
    struct AutoSearchOptionDestroyer {
        ~AutoSearchOptionDestroyer() { SearchOption::SkillBase::DestroyInstance(); };
    } auto_search_option_destroyer;
    //
    SearchOption::SkillBase& search_option = *SearchOption::SkillBase::CreateInstance(this);
    //
    const SECTOR_IDX coordinates = search_option.actor->GetSectorIndex();
    const nsGrid::SectorGridVR* sector_grid_vr =
        game_field_->GetSectorGridVR(coordinates, search_option.arguments.ranges_of_application);
    if (sector_grid_vr == NULL) {
        return false;
    };

    const ns_object::SearchOption::Arguments& arguments = search_option.arguments;
    nsGrid::SectorGrid3x3 OUT grid;
    ;{
        bool success = sector_grid_vr->GetAroundSectors(
            arguments.application_center_pos, &grid, NULL);
        if (success == false) {
            return false;
        }
    };
    //----------------------------------------------------------------------------------------------
    ViewBoxCubic damage_box;
    // CHANGES: f110621.3L, changed the damage box ranges according to the attack range formation
    if (search_option.GetApplicationRangeBox(&damage_box,
            (game_field_->IsWarCastle() ? 30.0f : 5.0f)) == false)
    {
        return false;
    };
    //
    SearchOption::SkillBase::result_type search_result = { 0, };
    //----------------------------------------------------------------------------------------------
    // 단일 타겟에게 사용하는 스킬인 경우 타겟을 검색하지 않고 메인타겟으로 셋팅한다.
    if (base_skill_info_->m_byAttRangeform == SRF_FOWARD_ONE)
    {
        Character* main_target = game_field_->FindCharacter(skill_info_data_.main_target_key_);
        if (main_target)
        {
            search_option.working.main_target = main_target;
            // actor
            if (main_target != owner_)
            {
                ;{
                    SkillExecutionTransaction transaction; // CHANGES: f110303.3L
                    // execute abilities
                    if (Skill::ExecuteAbilities(&search_option, damage_box,
                            owner_, &result_msg_array_[number_of_hit_targets_]))
                    {
                        ++number_of_hit_targets_;
                    };
                };
            };
            // main_target
            ;{
                SkillExecutionTransaction transaction; // CHANGES: f110303.3L
                // execute abilities
                if (Skill::ExecuteAbilities(&search_option, damage_box,
                        main_target, &result_msg_array_[number_of_hit_targets_]))
                {
                    search_option.working.main_target = main_target;

                    // process about PK attack
                    owner_->ForcedAttack(main_target);
                    // send AI message if this attack is hit.
                    Skill::SendAIMessage(main_target);

                    ++number_of_hit_targets_;
                }
            };
        }
    }
    // 소환스킬은 타겟을 구하지 않는다.
    else if (base_skill_info_->m_byTarget == SKILL_TARGET_SUMMON)
    {
    }
    // 타겟이 없이 좌표만 있는 스킬인 경우 타겟을 구하지 않는다.
    else if (base_skill_info_->m_byAttRangeform == SRF_AREA_POSITION)
    {
    }
    // 랜덤영역선택 스킬이라면 타겟을 구하지 않는다.
    else if (base_skill_info_->GetAbilityInfo(eABILITY_RANDOM_AREA_ATTACK))
    {
    }
    else
    {
        search_result.multi_select = true;
    };
    //----------------------------------------------------------------------------------------------
    if (search_result.multi_select == false)
    {
        // jump to next
    }
    else
    {
        //------------------------------------------------------------------------------------------
        //const bool skip_target = (except_object_key != 0);
        GRID3x3_CENTER_PRIOR_SWAP(grid); //  Center를 먼저 처리하면서 주변 섹터 처리
        FOREACH_ARRAY(const SECTOR_IDX sector_idx, grid.Grid, SectorGrid3x3::GridArray)
        {
            if (number_of_hit_targets_ >= MAX_TARGET_NUM) {
                break;
            };
            if (sector_idx == INVALID_SECTOR_INDEX) {
                continue;
            };

            Sector* const sector = game_field_->GetSector(sector_idx);
            ns_object::SearchNodeConstIterator it = sector->GetCharacterRoot();
            const ns_object::SearchNodeConstIterator end = sector->GetCharacterEnd();

            while (const ns_object::SearchNode::LinkNode* link_node = it.ClimbToNext(end))
            {
                if (number_of_hit_targets_ >= MAX_TARGET_NUM) {
                    break;
                };
                const ns_object::SearchNode::CharacterNode& char_node =
                    link_node->select.character_node;
                Character* const target = char_node.character;
                ;{
                    SkillExecutionTransaction transaction; // CHANGES: f110303.3L
                    // execute abilities
                    if (!Skill::ExecuteAbilities(&search_option, damage_box,
                            target, &result_msg_array_[number_of_hit_targets_]))
                    {
                        continue;
                    }
                };
                if (skill_info_data_.main_target_key_ == char_node.object_key) {
                    search_option.working.main_target = target;
                };
                // process about PK attack
                owner_->ForcedAttack(target);
                // send AI message if this attack is hit.
                Skill::SendAIMessage(target);

                ++number_of_hit_targets_;

                if (IsMaxTargetNum(target)) {
                    break;
                };
            } //end 'for' characters
        }; //end 'for' sectors
    };
    //----------------------------------------------------------------------------------------------
    // apply field effect abilities
    number_of_effects_ = ExecuteEffectAbilities(&result_effect_msg_array_[0]);
    //----------------------------------------------------------------------------------------------
    // process internal events. this process may be to occur recovery process when anyone is damaged
    if (search_option.working.main_target &&
        skill_info_data_.skill_factor_ == SKILL_FACTOR_NOMAL)
    {
        owner_->OnAttack(search_option.working.main_target, skill_info_data_.skill_code_, 0);
    }
    if (FlowControl::FCAssert(number_of_hit_targets_ <= MAX_TARGET_NUM) == false) {
        // WARNING: heap buffer overrun
        number_of_hit_targets_ = MAX_TARGET_NUM;
    }

    // 연계 스킬 활성화 어빌리티가 실행되지 않았으면 활성화된 연계 스킬 상태를 제거한다.
    const SkillScriptInfo* const skill_info = GetSkillBaseInfo();
    if (skill_info != NULL)
    {
        const bool is_active_combo = (skill_info->GetAbilityInfo(kAbilityActiveComboSkill) != NULL);
        if (is_active_combo == false)
        {
            const Character* const owner = GetOwner();
            if (owner != NULL)
            {
                StatusManager* const status_manager = owner->GetStatusManager();
                status_manager->Remove(kCharStateActiveComboSkill);
//#define DEBUG_COMBO_SKILL_
#ifdef DEBUG_COMBO_SKILL_
                SUNLOG(
                    eCRITICAL_LOG, 
                    _T("|%s|Removed active combo state.|SkillCode = %d|"), 
                    __FUNCTION__, 
                    base_skill_info_->m_SkillCode
                );
#endif
            }
        }
    }

#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    if ( owner_->IsEqualObjectKind(PLAYER_OBJECT) == true )
    {
        Player* player = reinterpret_cast<Player*>(owner_);
        if (player != NULL && player->GetCharType() == eCHAR_WITCHBLADE)
        {
            for (int count=0; count < number_of_hit_targets_; ++count)
            {
                SKILL_RESULT_BASE* base_msg = reinterpret_cast<SKILL_RESULT_BASE*>(result_msg_array_[count].m_byMsg);
                //SKILL_RESULT_DAMAGE* result_msg = reinterpret_cast<SKILL_RESULT_DAMAGE*>(base_msg+1);
                if (base_msg->isDamaged == true)
                {
                    player->OnRecoverFP(CalcFPRecoverByAttack(player->GetCharType()));
                    break;
                }
            }
        }
    }
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

#ifdef _NA_008588_20151202_MODIFY_MOVEING_SKILL_LOGIC
    _ProcessSkillMove();
#endif //_NA_008588_20151202_MODIFY_MOVEING_SKILL_LOGIC
    return true;
}

#ifdef _NA_0_20100819_AGGRO_RENEWAL
// NOTE: f110621.4L, this method is only specialied to acquire the aggressive dispersion counter.
// generally, it is not supposed to use this logic because such usages are rare.
BYTE Skill::FindTargetCount(eSKILL_TARGET_TYPE skill_target_type)
{
    //_NA000000_110210_ENHANCED_TARGET_SEARCH_ALGORITHM
    __UNUSED(skill_target_type);

    GameField* const game_field = owner_->GetField();
    if (game_field == NULL) {
        return 0;
    };
    ns_object::SearchResult::SingleSearch search_result;
    ns_object::SearchOption::SingleSearch search_option(
        MAX_TARGET_NUM,//int max_number_of_selected_characters,
        owner_, //Character* input_actor,
        NULL, //Character* input_main_target_if_exist_it,
        owner_->GetPosPtr(), //const WzVector* input_specific_application_center_pos_if_main_target_is_not_used_to_pos,
        //
        0.0f, //float attack_ranges,
        base_skill_info_->m_wSkillArea / 10.0f, //float ranges_of_application,
        SRF_FOWARD_360); //eSKILL_AREA_TYPE forms_of_application
    bool success = game_field->FindTargetsUsingSingleSearch(search_option, &search_result);
    if (success == false) {
        return 0;
    };
    // ranges = [0, MAX_TARGET_NUM]
    return static_cast<uint8_t>(search_result.number_of_selected_chars);
    //----------------------------------------------------------------------------------------------
};
#endif //_NA_0_20100819_AGGRO_RENEWAL

//==================================================================================================

//_NA000000_110210_ENHANCED_TARGET_SEARCH_ALGORITHM
bool Skill::ExecuteAbilities(
        ns_object::SearchOption::SkillBase* search_option,
        const nsGrid::ViewBoxCubic& damage_box,
        Character* target, RESULTMSG* const raw_result_msg)
{
    ZeroMemory(raw_result_msg, sizeof(*raw_result_msg));
    if (target == NULL) {
        return false;
    };
    //----------------------------------------------------------------------------------------------

    SKILL_RESULT_BASE* const result_msg =
        reinterpret_cast<SKILL_RESULT_BASE*>(raw_result_msg->m_byMsg);
    ;{
        raw_result_msg->m_wSize = result_msg->GetSize();
        result_msg->m_dwTargetKey = target->GetObjectKey();
    };
    skill_info_data_.skill_effect_ = 0;

    WORD msg_size = 0;
    BYTE* msg_it = reinterpret_cast<BYTE*>(result_msg + 1);
    ns_object::RelationHint relation = { 0, };
    //
    const AbilityContainer::Node* ability_it = abilities_.nodes_,
        * const ability_end = &abilities_.nodes_[_countof(abilities_.nodes_)];

    for (int index = 0;
         index < abilities_.number_of_abilities_ && ability_it != ability_end;
         ++index, ++ability_it)
    {
        if (search_option->working.need_process_abilities[index] == false) {
            continue;
        }
        // NOTE: register the IFF filter of a currently processing ability to check internally
        search_option->iff_filter = search_option->working.ability_infos[index]->iff_filter;
        //------------------------------------------------------------------------------------------
        const bool himself = (search_option->actor == target);
        if ((search_option->iff_filter.value & search_option->iff_filter.eIFF_CheckSkip) == 0)
        {
            bool skip_ranges_check = himself &&
                (search_option->iff_filter.value & search_option->iff_filter.eIFF_AttackerOnly);
            ;;;; if (skip_ranges_check == false && himself)
            {
                const WzVector* const target_pos = target->GetPosPtr();
                if (damage_box.IsValidCoordinateXYZ(target_pos) == false) {
                    continue;
                };
            }
            else if (skip_ranges_check == false)
            {
                const WzVector* const target_pos = target->GetPosPtr();
                if (damage_box.IsValidCoordinateXYZ(target_pos) == false) {
                    continue;
                };
                const ns_object::SearchOption::Arguments& arguments = \
                    search_option->arguments;
                if (arguments.attack_ranges != 0.0f) {
                    if (search_option->actor->CheckSkillRange(target, *target_pos,
                            arguments.attack_ranges) == false) {
                        continue;
                    }
                };
                if (search_option->actor->CheckArea(*target_pos,
                        const_cast<WzVector&>(*arguments.application_center_pos),
                       arguments.forms_of_application, arguments.ranges_of_application) == false)
                {
                   return false;
                }
            };
        };
        if (search_option->IsSatisfiedObjectAbilityConditions(search_option->actor, target,
                search_option->working.ability_infos[index]) == false)
        {
            continue;
        };
        if (search_option->IsSatisfiedObjectIFFRelations(search_option->actor, target,
                &relation, &relation) == false)
        {
            continue;
        };
        //------------------------------------------------------------------------------------------
        Ability* const ability = ability_it->ability_;
    #if SUN_CODE_BACKUP // NOTE: already tested
        const eABILITY_TYPE ability_type = ability->GetAbilityType();
        // changes a concept. designed by __NA_S00015_20080828_NEW_HOPE_SHADOW
        if (ability_type == eABILITY_TYPE_EFFECT || ability_type == eABILITY_TYPE_MANUAL) {
            continue;
        };
    #endif
        // check whether enable execution ability
        if (!ability->CanExecute(owner_, target, skill_info_data_.main_target_key_,
                skill_info_data_.main_target_pos_))
        {
            continue;
        }

        ability->SetBonusEffect(&bonus_ability_effect_);
        // CHANGES: f110218.1L, prevent probability of garbage data occurrence
        msg_size = 0;
        if (ability->Execute(target, msg_it, msg_size))
        {
            if (msg_size > 512)
            {
                SUNLOG(eCRITICAL_LOG,
                    _T("|["__FUNCTION__"]|Warnings=big message size(%d)|"), msg_size);
                continue;
            }
            // CHANGES: f110218.1L, changes order, old comment "(WAVERIX) (090122) (BUG-FIX)"
            if (msg_size)
            {
                raw_result_msg->m_wSize += msg_size;
                ++result_msg->m_byAbilityNum;

#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
                // 해당 어빌리티가 Damage어빌리티고, 이 어빌리티로 상대에게 피해를 주었으면
                if (ability->GetAbilityID() == eABILITY_DAMAGE)
                {
                    SKILL_RESULT_DAMAGE* damage_msg = reinterpret_cast<SKILL_RESULT_DAMAGE*>(msg_it);
                    if (damage_msg->m_wDamage != 0 ) 
                    {
                        result_msg->isDamaged = true;

#ifdef _NA_007667_20141001_WITCHBLADE_SKILL
                        StatusManager* status_manager = owner_->GetStatusManager();
                        if (status_manager != NULL) {
                            status_manager->UpdateStatusByAttack(target);
                        }
#endif //_NA_007667_20141001_WITCHBLADE_SKILL
                    }
                }
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
                msg_it += msg_size;
            };
        }
    } //end 'for' abilities

    // 타겟 한명한테만 적용하는 거라면 다음타겟전에 초기화한다.
    if (bonus_ability_effect_.m_dwObjectKey) {
        bonus_ability_effect_.Init();
    }

    result_msg->m_bySkillEffect = skill_info_data_.skill_effect_;

    return (result_msg->m_byAbilityNum != 0);
}

BYTE Skill::ExecuteEffectAbilities(RESULTMSG* const result_msg)
{
    ZeroMemory(result_msg, sizeof(*result_msg));
    WORD msg_size = 0;  
    BYTE excute_count = 0;

    const AbilityContainer::Node* it = abilities_.nodes_;
    const AbilityContainer::Node* const end = &abilities_.nodes_[_countof(abilities_.nodes_)];

    for (int i = 0; i < abilities_.number_of_abilities_ && it != end; ++i, ++it)
    {
        Ability* const ability = it->ability_;
        eABILITY_TYPE ability_type = ability->GetAbilityType();
        if (ability_type != eABILITY_TYPE_EFFECT && ability_type != eABILITY_TYPE_ACTIVE_AND_EFFECT)
        {
            continue;
        }
        if (ability->ExecuteEffect(result_msg[excute_count].m_byMsg, msg_size))
        {
            result_msg[excute_count].m_wSize += msg_size;
            ++excute_count;
        }
    }
    return excute_count;
}

void
Skill::Broadcast_Instant_Result()
{
    if (game_field_ == 0)
    {
        SUNLOG(eCRITICAL_LOG, "[Skill::Broadcast_Instant_Result] GameField is NULL!");
        return;
    }

    uint8_t variable_buffer[12 * 1024];
    MSGSUB_SKILL_ACTION_INSTANT_RESULT_BRD& msg_brd = \
        *reinterpret_cast<MSGSUB_SKILL_ACTION_INSTANT_RESULT_BRD*>(variable_buffer);
    msg_brd.Clear();
    ;{
        msg_brd.result.m_SkillCode             = skill_info_data_.skill_code_;
        msg_brd.result.m_dwClientSerial        = skill_info_data_.client_serial_;
        msg_brd.result.m_dwAttackerKey         = owner_->GetObjectKey();
        msg_brd.result.m_dwPrimaryTargetKey    = skill_info_data_.main_target_key_;
        msg_brd.result.m_wvMainTargetPos       = skill_info_data_.main_target_pos_;
        msg_brd.result.m_wvCurPos              = skill_info_data_.cur_pos_;
        msg_brd.result.m_wvDestPos             = skill_info_data_.dest_pos_;
        msg_brd.result.m_dwAttackerHP          = owner_->GetHP();
        msg_brd.result.m_dwAttackerMP          = owner_->GetMP();
        //msg_brd.result.m_byNumberOfTargets     = number_of_hit_targets_;
        //msg_brd.result.m_byNumberOfFieldEffect = number_of_effects_;
    };
    // DONE: need check 'number_of_hit_targets_' and 'number_of_effects_' boundaries
    uint8_t* offset_it = variable_buffer + sizeof(msg_brd);
    int additional_size = 0;
    if (RESULTMSG* const end = &result_msg_array_[number_of_hit_targets_])
    {
        RESULTMSG* it = result_msg_array_;
        for ( ; it != end; ++it)
        {
            const int size = it->m_wSize;
            if (offset_it + size < &variable_buffer[_countof(variable_buffer)]) {
                CopyMemory(offset_it, it->m_byMsg, size);
                offset_it += size;
                additional_size += size;
            }
            else {
                SUNLOG(eCRITICAL_LOG,
                       _T("|["__FUNCTION__"]|additional target information size is too big"));
                number_of_hit_targets_ = static_cast<uint8_t>(it - result_msg_array_);
                break;
            }
        };
    };
    //
    if (RESULTMSG* const end = &result_effect_msg_array_[number_of_effects_])
    {
        RESULTMSG* it = result_effect_msg_array_;
        for ( ; it != end; ++it)
        {
            const int size = it->m_wSize;
            if (offset_it + size < &variable_buffer[_countof(variable_buffer)]) {
                CopyMemory(offset_it, it->m_byMsg, size);
                offset_it += size;
                additional_size += size;
            }
            else {
                SUNLOG(eCRITICAL_LOG,
                       _T("|["__FUNCTION__"]|additional effect information size is too big"));
                number_of_effects_ = static_cast<uint8_t>(it - result_effect_msg_array_);
                break;
            }
        };
    };
    //-------------------------------------------------------------
    ;{
        // lazy alignment
        msg_brd.result.m_byNumberOfTargets     = static_cast<uint8_t>(number_of_hit_targets_);
        msg_brd.result.m_byNumberOfFieldEffect = static_cast<uint8_t>(number_of_effects_);
        //
        msg_brd.sizeof_additions = static_cast<uint16_t>(additional_size);
    };
    //
    owner_->SendPacketAroundForSync(&msg_brd);
    //
}

void
Skill::Broadcast_Delayed_Result()
{
    if (game_field_ == 0)
    {
        SUNLOG(eCRITICAL_LOG, "[Skill::Broadcast_Delayed_Result] GameField is NULL!");
        return;
    }

    uint8_t variable_buffer[12 * 1024];
    MSGSUB_SKILL_ACTION_DELAY_RESULT_BRD& msg_brd = \
        *reinterpret_cast<MSGSUB_SKILL_ACTION_DELAY_RESULT_BRD*>(variable_buffer);
    msg_brd.Clear();
    ;{
        msg_brd.result.m_SkillCode = skill_info_data_.skill_code_;
        msg_brd.result.m_dwClientSerial = skill_info_data_.client_serial_;
        msg_brd.result.m_dwAttackerKey = owner_->GetObjectKey();
        msg_brd.result.m_dwPrimaryTargetKey = skill_info_data_.main_target_key_;
        msg_brd.result.m_wvMainTargetPos = skill_info_data_.main_target_pos_;
        msg_brd.result.m_dwAttackerHP = owner_->GetHP();
        msg_brd.result.m_dwAttackerMP = owner_->GetMP();
        //msg_brd.m_byNumberOfTargets = number_of_hit_targets_;
        //msg_brd.m_byNumberOfFieldEffect = number_of_effects_;
    };
    // DONE: need check 'number_of_hit_targets_' and 'number_of_effects_' boundaries
    uint8_t* offset_it = variable_buffer + sizeof(msg_brd);
    int additional_size = 0;
    if (RESULTMSG* const end = &result_msg_array_[number_of_hit_targets_])
    {
        RESULTMSG* it = result_msg_array_;
        for ( ; it != end; ++it)
        {
            const int size = it->m_wSize;
            if (offset_it + size < &variable_buffer[_countof(variable_buffer)]) {
                CopyMemory(offset_it, it->m_byMsg, size);
                offset_it += size;
                additional_size += size;
            }
            else {
                SUNLOG(eCRITICAL_LOG,
                       _T("|["__FUNCTION__"]|additional target information size is too big"));
                number_of_hit_targets_ = static_cast<uint8_t>(it - result_msg_array_);
                break;
            }
        };
    };
    //
    if (RESULTMSG* const end = &result_effect_msg_array_[number_of_effects_])
    {
        RESULTMSG* it = result_effect_msg_array_;
        for ( ; it != end; ++it)
        {
            const int size = it->m_wSize;
            if (offset_it + size < &variable_buffer[_countof(variable_buffer)]) {
                CopyMemory(offset_it, it->m_byMsg, size);
                offset_it += size;
                additional_size += size;
            }
            else {
                SUNLOG(eCRITICAL_LOG,
                       _T("|["__FUNCTION__"]|additional effect information size is too big"));
                number_of_effects_ = static_cast<uint8_t>(it - result_effect_msg_array_);
                break;
            }
        };
    };
    //-------------------------------------------------------------
    ;{
        // lazy alignment
        msg_brd.result.m_byNumberOfTargets     = static_cast<uint8_t>(number_of_hit_targets_);
        msg_brd.result.m_byNumberOfFieldEffect = static_cast<uint8_t>(number_of_effects_);
        //
        msg_brd.sizeof_additions = static_cast<uint16_t>(additional_size);
    };
    //
    owner_->SendPacketAroundForSync(&msg_brd);
}


void
Skill::SendAIMessage(Character* pTarget, DAMAGETYPE wDamage)
{
    // 공격당한 NPC에게 AIMessage를 날린다.
    AI_MSG_ATTACKED AIAttackedMsg;
    AIAttackedMsg.dwAttackerKey = owner_->GetObjectKey();
    AIAttackedMsg.wDamage = wDamage;
    pTarget->SendAIMessage(&AIAttackedMsg, sizeof(AIAttackedMsg));

    // NPC가 공중으로 뜨는 스킬일 경우 Flying Message를 날린다.
    if (skill_attribution_ & SKILL_ATTRIBUTION_TARGETFLYING)
    {
        AI_MSG_FLYING flyMsg;
        flyMsg.dwFlyingTick = GetExecuteInterval();
        pTarget->SendAIMessage(&flyMsg, sizeof(flyMsg));
    }

    // 타겟을 멈춰놓는 스킬일 경우
    if (skill_attribution_ & SKILL_ATTRIBUTION_TARGETSTOP)
    {
        AI_MSG_KNOCKDOWN downMsg;
        downMsg.dwKnockDownTick = GetExecuteInterval();
        pTarget->SendAIMessage(&downMsg, sizeof(downMsg));
    }
}

#ifdef _NA_008588_20151202_MODIFY_MOVEING_SKILL_LOGIC
VOID Skill::_ProcessSkillMove()
{
    // 스킬사용 후 이동처리 (돌진형 스킬들)
    const BASE_ABILITYINFO* dmg_ability_info = base_skill_info_->GetAbilityInfo(eABILITY_DAMAGE);
    Character* main_target = game_field_->FindCharacter(skill_info_data_.main_target_key_);

    // 1번어빌리티가 없으면 그냥 넘어간다
    if (dmg_ability_info != NULL && main_target != NULL) 
    {
        //파라미터3의 값이 1이면 이동처리를 한다
        bool is_moving_damage = (dmg_ability_info->m_iParam[2] == 1);
        if (is_moving_damage)
        {
            const WzVector& attacker_pos = *owner_->GetPosPtr();
            const WzVector& target_pos = *(main_target)->GetPosPtr();

            WzVector distance = target_pos;
            distance -= attacker_pos;

            WzVector direction; // nomalized vector
            VectorNormalize(&direction, &distance);

            WzVector offset = direction;
            offset *= 2.0f;
            WzVector destination;

            if (base_skill_info_ && base_skill_info_->m_byAttRangeform == SRF_PIERCE)
            {
                const ns_object::SearchOption::SkillBase& search_option = \
                    *ns_object::SearchOption::SkillBase::CreateInstance(this);
                destination = *search_option.arguments.application_center_pos;
            }
            else
            {
                destination = attacker_pos;
                destination += (distance - offset);
            }

            const WORD kFilteredAttr = (PTA_NO_WALK);

            GameField* const game_field = owner_->GetField();
            if (game_field == NULL) {
                SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] Cannot Find Attacker's GameField Using Skill");
                return;
            }
            nsAI::MovingPlanner* moving_planner = game_field->GetMovingPlanner();

            if (moving_planner->PathPlan_FastMoved(owner_, destination, kFilteredAttr, &destination) != false)
            {
                if (owner_->IsEqualObjectKind(PLAYER_OBJECT)) 
                {
                    static_cast<Player*>(owner_)->GetSyncTuner()->OnFastMoved(destination);
                };
            };
            // changes attacker position
            owner_->SetPos(&destination);
        };
    }
}
#endif //_NA_008588_20151202_MODIFY_MOVEING_SKILL_LOGIC
