#pragma once

#ifdef __DISABLE_ALWARYS_MACRO_FOR_INTELLISENSE
    //  아래 목록들은 StatusManager.cpp 상단에 위치하는 목록들이다.
    #define LIST_CANT_ATTACK_STATES()
    #define LIST_CANT_BEATTACKED_STATES()
    #define LIST_CANT_MOVE_STATES()
    #define LIST_CANT_USE_SKILL_STATES()
    #define LIST_EXCLUSIVE_ACTION_STATES()
#endif

//==================================================================================================
//  <StatusManager::InterState>
//  -   각종 제약 조건 필드 (루프 감소 목적)
//--------------------------------------------------------------------------------------------------
#define TAG_CANT_ATTACK(etype)        eIS_CANT_ATTACK___##etype
#define TAG_CANT_BEATTACKED(etype)    eIS_CANT_BEATTACKED___##etype
#define TAG_CANT_MOVE(etype)          eIS_CANT_MOVE___##etype
#define TAG_CANT_USE_SKILL(etype)     eIS_CANT_USE_SKILL___##etype
#define TAG_EXCLUSIVE_ACTION(etype)   eIS_EXCLUSIVE_ACTION___##etype
//--------------------------------------------------------------------------------------------------
//  <eCHECK_STATE>  제약을 걸 상태들 재구성
//  -   특정 상태에서 이동, 공격 등의 행위 불가를 구성하기 위한 필드.
//  -   추가는 상단의 목록에만 추가하면 이후 모든 것은 자동화
enum StatusManager::InterState::eCHECK_STATE
{
#undef MAKE_ENUM_CA
#define MAKE_ENUM_CA(etype)     TAG_CANT_ATTACK(etype),     // Can't Attack State
    LIST_CANT_ATTACK_STATES()

#undef MAKE_ENUM_CA
#define MAKE_ENUM_CA(etype)     TAG_CANT_BEATTACKED(etype), // Can't beAttacked State
    LIST_CANT_BEATTACKED_STATES()

#undef MAKE_ENUM_CA
#define MAKE_ENUM_CA(etype)     TAG_CANT_MOVE(etype),       // Can't Move State
    LIST_CANT_MOVE_STATES()

#undef MAKE_ENUM_CA
#define MAKE_ENUM_CA(etype)     TAG_CANT_USE_SKILL(etype),  // Can't Use Skill State
    LIST_CANT_USE_SKILL_STATES()

#undef MAKE_ENUM_CA
#define MAKE_ENUM_CA(etype)     TAG_EXCLUSIVE_ACTION(etype), // can't accept multiple states
    LIST_EXCLUSIVE_ACTION_STATES()

    //----------------------------------------------------------------------------------------------
    MAX_CHECK_STATE // Upper-bound
};


//--------------------------------------------------------------------------------------------------
//  모든 상태 초기화
//
void
StatusManager::InterState::ClearField(RESTRICT_STATUS_BITFIELD& field)
{
    field = 0;
}


//--------------------------------------------------------------------------------------------------
//  <IsCanAttack>   공격할 수 없는 상황들을 열거한다. 여기에 만족하지 않으면 가능상태
bool
StatusManager::InterState::IsCanAttack(const RESTRICT_STATUS_BITFIELD& field)
{
    BOOST_STATIC_ASSERT((StatusManager::InterState::MAX_CHECK_STATE <=
                        (sizeof(RESTRICT_STATUS_BITFIELD)*8)));
    // Can't Attack State
#undef MAKE_ENUM_CA
#define MAKE_ENUM_CA(etype)     | ((RESTRICT_STATUS_BITFIELD)1<<TAG_CANT_ATTACK(etype))
    const RESTRICT_STATUS_BITFIELD  kCantAttackStates = 0 LIST_CANT_ATTACK_STATES();

    return !(kCantAttackStates & field);
}


//--------------------------------------------------------------------------------------------------
//  <IsCanAttack>   공격 받을 수 없는 상황들을 열거한다. 여기에 만족하지 않으면 가능상태
bool
StatusManager::InterState::IsCanBeAttacked(const RESTRICT_STATUS_BITFIELD& field)
{
    // Can't beAttacked State
#undef MAKE_ENUM_CA
#define MAKE_ENUM_CA(etype)     | ((RESTRICT_STATUS_BITFIELD)1<<TAG_CANT_BEATTACKED(etype))
    const RESTRICT_STATUS_BITFIELD  kCantBeAttackedStates = 0 LIST_CANT_BEATTACKED_STATES();
    return !(kCantBeAttackedStates & field);
}


//--------------------------------------------------------------------------------------------------
//  <IsCanAttack>   이동 수 없는 상황들을 열거한다. 여기에 만족하지 않으면 가능상태
bool
StatusManager::InterState::IsCanMove(const RESTRICT_STATUS_BITFIELD& field)
{
    // Can't Move State
#undef MAKE_ENUM_CA
#define MAKE_ENUM_CA(etype)     | ((RESTRICT_STATUS_BITFIELD)1<<TAG_CANT_MOVE(etype))
    const RESTRICT_STATUS_BITFIELD  kCantMoveStates = 0 LIST_CANT_MOVE_STATES();
    return !(kCantMoveStates & field);
}


//--------------------------------------------------------------------------------------------------
//  <IsCanAttack>   스킬을 사용할 수 없는 상황들을 열거한다. 여기에 만족하지 않으면 가능상태
RC::eSKILL_RESULT
StatusManager::InterState::IsCanUseSkill(const RESTRICT_STATUS_BITFIELD& field)
{
    // Can't Use Skill State
#undef MAKE_ENUM_CA
#define MAKE_ENUM_CA(etype)     | ((RESTRICT_STATUS_BITFIELD)1<<TAG_CANT_USE_SKILL(etype))
    const RESTRICT_STATUS_BITFIELD  kCantUseSkillStates = 0 LIST_CANT_USE_SKILL_STATES();
    if (!(kCantUseSkillStates & field)) {
        return RC::RC_SKILL_SUCCESS;
    }
    // Can't Use Skill State
#undef MAKE_ENUM_CA
#define MAKE_ENUM_CA(etype)     field & ((RESTRICT_STATUS_BITFIELD)1<<TAG_CANT_USE_SKILL(etype))
    if (MAKE_ENUM_CA(eCHAR_STATE_SEALING)) {
        return RC::RC_SKILL_SEALING_STATE;  // 세부 리턴 값이 존재하므로 임시로 이렇게 처리했음.
    }
    if (MAKE_ENUM_CA(eCHAR_STATE_CONFUSE)) {
        return RC::RC_SKILL_SEALING_STATE;  // 세부 리턴 값이 존재하므로 임시로 이렇게 처리했음.
    }

    return RC::RC_SKILL_FAILED;
}

bool StatusManager::InterState::IsCanSpecialAction(const RESTRICT_STATUS_BITFIELD& field)
{
#undef MAKE_ENUM_CA
#define MAKE_ENUM_CA(etype)     | ((RESTRICT_STATUS_BITFIELD)1<<TAG_EXCLUSIVE_ACTION(etype))

#ifdef _NA001955_110210_WING_COSTUME
    const RESTRICT_STATUS_BITFIELD  kCurrentAction = 0 LIST_EXCLUSIVE_ACTION_STATES();
    if ((kCurrentAction & field) == 0) {
        return true;
    }
    return false;
#else
    return true;
#endif
}

//--------------------------------------------------------------------------------------------------
//  <AddRestrictStatus> 특정 상태 제약을 추가한다. 각 상태는 그룹지어져 있다.
void
StatusManager::InterState::AddRestrictStatus(const eCHAR_STATE_TYPE state_id,
                                             RESTRICT_STATUS_BITFIELD& INOUT field,
                                             StatusField* status_field_ptr)
{
    RESTRICT_STATUS_BITFIELD checked_field = 0;
    switch (state_id)
    {
        // Can't Attack State
#undef MAKE_ENUM_CA
#define MAKE_ENUM_CA(etype) \
    case etype: checked_field |= (RESTRICT_STATUS_BITFIELD)1 << TAG_CANT_ATTACK(etype); break;

        LIST_CANT_ATTACK_STATES();
    };
    switch(state_id)
    {
        // Can't beAttacked State
#undef MAKE_ENUM_CA
#define MAKE_ENUM_CA(etype) \
    case etype: checked_field |= (RESTRICT_STATUS_BITFIELD)1 << TAG_CANT_BEATTACKED(etype); break;

        LIST_CANT_BEATTACKED_STATES();
    };
    switch(state_id)
    {
        // Can't Move State
#undef MAKE_ENUM_CA
#define MAKE_ENUM_CA(etype) \
    case etype: checked_field |= (RESTRICT_STATUS_BITFIELD)1<<TAG_CANT_MOVE(etype); break;

        LIST_CANT_MOVE_STATES();
    };
    switch(state_id)
    {
        // Can't Use Skill State
#undef MAKE_ENUM_CA
#define MAKE_ENUM_CA(etype) \
    case etype: checked_field |= (RESTRICT_STATUS_BITFIELD)1<<TAG_CANT_USE_SKILL(etype); break;

        LIST_CANT_USE_SKILL_STATES();
    };
    switch (state_id)
    {
        // can't special action
#undef MAKE_ENUM_CA
#define MAKE_ENUM_CA(etype) \
    case etype: checked_field |= (RESTRICT_STATUS_BITFIELD)1<<TAG_EXCLUSIVE_ACTION(etype); break;

        LIST_EXCLUSIVE_ACTION_STATES();
    };

    //------------------------
    field |= checked_field;
    //------------------------
    if (status_field_ptr == NULL) { // simulation test
        return;
    }
    // On Event
    StatusField& status_field = *status_field_ptr;
    switch (state_id)
    {
        // 현 시점 2,3,4 등의 상태들이 추가되었지만, 드래곤 변신 상태에 대한 처리는 이것 하나뿐이다.
    case eCHAR_STATE_TRANSFORMATION:
        status_field.On_DragonTransformation(true);
        break;
    case eCHAR_STATE_WAR_CTRL_OBSERVER_MODE:
        status_field.AddFlag(status_field.ENABLE_OBSERVER_MODE);
        break;
    case eCHAR_STATE_RIDING_RIDER:
        status_field.AddFlag(status_field.ENABLE_SUMMONED_AND_RIDING);
        break;
    case eCHAR_STATE_NPC_NON_PREEMPTIVE_ATTACK:
        status_field.AddFlag(status_field.ENABLE_NON_PREEMPTIVE_ATTACK);
        break;
    case eCHAR_STATE_SPREAD_WINGS:
        status_field.AddFlag(status_field.ENABLE_SPREAD_WINGS);
        break;
    } //end 'switch' for specific event
}


//--------------------------------------------------------------------------------------------------
//  <AddRestrictStatus> 특정 상태 제약을 제거한다. 각 상태는 그룹지어져 있다.
void
StatusManager::InterState::DelRestrictStatus(const eCHAR_STATE_TYPE state_id,
                                             RESTRICT_STATUS_BITFIELD& INOUT field,
                                             StatusField* status_field_ptr)
{
    RESTRICT_STATUS_BITFIELD checked_field = 0;
    switch (state_id)
    {
        // Can't Attack State
#undef MAKE_ENUM_CA
#define MAKE_ENUM_CA(etype) \
    case etype: checked_field |= (RESTRICT_STATUS_BITFIELD)1<<TAG_CANT_ATTACK(etype); break;

        LIST_CANT_ATTACK_STATES();
    };
    switch(state_id)
    {
        // Can't beAttacked State
#undef MAKE_ENUM_CA
#define MAKE_ENUM_CA(etype) \
    case etype: checked_field |= (RESTRICT_STATUS_BITFIELD)1<<TAG_CANT_BEATTACKED(etype); break;

        LIST_CANT_BEATTACKED_STATES();
    };
    switch(state_id)
    {
        // Can't Move State
#undef MAKE_ENUM_CA
#define MAKE_ENUM_CA(etype) \
    case etype: checked_field |= (RESTRICT_STATUS_BITFIELD)1<<TAG_CANT_MOVE(etype); break;

        LIST_CANT_MOVE_STATES();
    };
    switch(state_id)
    {
        // Can't Use Skill State
#undef MAKE_ENUM_CA
#define MAKE_ENUM_CA(etype) \
    case etype: checked_field |= (RESTRICT_STATUS_BITFIELD)1<<TAG_CANT_USE_SKILL(etype); break;

        LIST_CANT_USE_SKILL_STATES();
    };
    switch (state_id)
    {
        // can't special action
#undef MAKE_ENUM_CA
#define MAKE_ENUM_CA(etype) \
    case etype: checked_field |= (RESTRICT_STATUS_BITFIELD)1<<TAG_EXCLUSIVE_ACTION(etype); break;

        LIST_EXCLUSIVE_ACTION_STATES();
    };
    //------------------------
    field &= ~checked_field;
    //------------------------
    if (status_field_ptr == NULL) { // simulation test
        return;
    }
    // On Event
    StatusField& status_field = *status_field_ptr;
    switch (state_id)
    {
        // 현 시점 2,3,4 등의 상태들이 추가되었지만, 드래곤 변신 상태에 대한 처리는 이것 하나뿐이다.
    case eCHAR_STATE_TRANSFORMATION:
        status_field.On_DragonTransformation(false);
        break;
    case eCHAR_STATE_WAR_CTRL_OBSERVER_MODE:
        status_field.RemoveFlag(status_field.ENABLE_OBSERVER_MODE);
        break;
    case eCHAR_STATE_RIDING_RIDER:
        status_field.RemoveFlag(status_field.ENABLE_SUMMONED_AND_RIDING);
        break;
    case eCHAR_STATE_NPC_NON_PREEMPTIVE_ATTACK:
        status_field.RemoveFlag(status_field.ENABLE_NON_PREEMPTIVE_ATTACK);
        break;
    case eCHAR_STATE_SPREAD_WINGS:
        status_field.RemoveFlag(status_field.ENABLE_SPREAD_WINGS);
        break;
    } //end 'switch' for specific event
}


bool StatusField::_IsPassConstraint(
    Character* /*character*/, const SkillScriptInfo* skill_script_info) const
{
    if (FIELD & ENABLE_ONLY_SKILL_TARGET_ME)
    {
        const BYTE target_type = skill_script_info->m_byTarget;
        bool accept_status = target_type == SKILL_TARGET_FRIEND            ?   true
                           : target_type == SKILL_TARGET_ME                ?   true
                           : target_type == SKILL_TARGET_REACHABLE_FRIEND  ?   true
                           : target_type == SKILL_TARGET_REACHABLE_ME      ?   true
                           :                                                   false;
        return accept_status;
    }

    if (filter_)
    {
        if (FIELD & ENABLE_FILTER_SKILL_GROUP)
        {
            if (filter_->SkillGroupFilter.IsExist(skill_script_info->m_SkillClassCode)) {
                return false;
            }
        }

        if (FIELD & ENABLE_FILTER_ITEM_TYPE) {
        }
    }

    return true;
}


bool StatusField::_IsPassConstraint(Character* /*character*/, const BASE_ABILITYINFO* ability_info) const
{
    if (FIELD & ENABLE_ONLY_SKILL_TARGET_ME)
    {
        const BYTE range_type = ability_info->m_byRangeType;
        bool accept_status = range_type == SKILL_ABILITY_FRIEND        ?   true
                           : range_type == SKILL_ABILITY_MYAREA_FRIEND ?   true
                           : range_type == SKILL_ABILITY_ME            ?   true
                           :                                               false;
        return accept_status;
    }

    return true;
}


bool
StatusField::_IsPassConstraint(Character* character, const BASE_ITEMINFO* item_info) const
{
    if (filter_)
    {
        if (FIELD & ENABLE_FILTER_SKILL_GROUP)
        {
        }

        if (FIELD & ENABLE_FILTER_ITEM_TYPE)
        {
            if (filter_->ItemTypeFilter.IsExist(item_info->m_wType)) {
                return false;
            }
            // (WAVERIX) (090219) (CHANGE) 타락한 사원에서 사용 불가 아이템 타입 중에
            // 특정 eWASTETYPE(13)만 막는 기능
            if (item_info->m_byWasteType == eITEMWASTE_EXP_POTION)
            {
                sPOLICY_CHARACTER& rPolicyment = character->Policyment;
                if ( rPolicyment.FIELD & rPolicyment.PCC_IN_SSQ_FIELD_PLAYER )
                {
                    return false;
                }
            }
        }
    }
    return true;
}

