#pragma once

#ifdef __DISABLE_ALWARYS_MACRO_FOR_INTELLISENSE
    //  �Ʒ� ��ϵ��� StatusManager.cpp ��ܿ� ��ġ�ϴ� ��ϵ��̴�.
    #define LIST_CANT_ATTACK_STATES()
    #define LIST_CANT_BEATTACKED_STATES()
    #define LIST_CANT_MOVE_STATES()
    #define LIST_CANT_USE_SKILL_STATES()
    #define LIST_EXCLUSIVE_ACTION_STATES()
#endif

//==================================================================================================
//  <StatusManager::InterState>
//  -   ���� ���� ���� �ʵ� (���� ���� ����)
//--------------------------------------------------------------------------------------------------
#define TAG_CANT_ATTACK(etype)        eIS_CANT_ATTACK___##etype
#define TAG_CANT_BEATTACKED(etype)    eIS_CANT_BEATTACKED___##etype
#define TAG_CANT_MOVE(etype)          eIS_CANT_MOVE___##etype
#define TAG_CANT_USE_SKILL(etype)     eIS_CANT_USE_SKILL___##etype
#define TAG_EXCLUSIVE_ACTION(etype)   eIS_EXCLUSIVE_ACTION___##etype
//--------------------------------------------------------------------------------------------------
//  <eCHECK_STATE>  ������ �� ���µ� �籸��
//  -   Ư�� ���¿��� �̵�, ���� ���� ���� �Ұ��� �����ϱ� ���� �ʵ�.
//  -   �߰��� ����� ��Ͽ��� �߰��ϸ� ���� ��� ���� �ڵ�ȭ
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
//  ��� ���� �ʱ�ȭ
//
void
StatusManager::InterState::ClearField(RESTRICT_STATUS_BITFIELD& field)
{
    field = 0;
}


//--------------------------------------------------------------------------------------------------
//  <IsCanAttack>   ������ �� ���� ��Ȳ���� �����Ѵ�. ���⿡ �������� ������ ���ɻ���
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
//  <IsCanAttack>   ���� ���� �� ���� ��Ȳ���� �����Ѵ�. ���⿡ �������� ������ ���ɻ���
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
//  <IsCanAttack>   �̵� �� ���� ��Ȳ���� �����Ѵ�. ���⿡ �������� ������ ���ɻ���
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
//  <IsCanAttack>   ��ų�� ����� �� ���� ��Ȳ���� �����Ѵ�. ���⿡ �������� ������ ���ɻ���
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
        return RC::RC_SKILL_SEALING_STATE;  // ���� ���� ���� �����ϹǷ� �ӽ÷� �̷��� ó������.
    }
    if (MAKE_ENUM_CA(eCHAR_STATE_CONFUSE)) {
        return RC::RC_SKILL_SEALING_STATE;  // ���� ���� ���� �����ϹǷ� �ӽ÷� �̷��� ó������.
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
//  <AddRestrictStatus> Ư�� ���� ������ �߰��Ѵ�. �� ���´� �׷������� �ִ�.
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
        // �� ���� 2,3,4 ���� ���µ��� �߰��Ǿ�����, �巡�� ���� ���¿� ���� ó���� �̰� �ϳ����̴�.
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
//  <AddRestrictStatus> Ư�� ���� ������ �����Ѵ�. �� ���´� �׷������� �ִ�.
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
        // �� ���� 2,3,4 ���� ���µ��� �߰��Ǿ�����, �巡�� ���� ���¿� ���� ó���� �̰� �ϳ����̴�.
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
            // (WAVERIX) (090219) (CHANGE) Ÿ���� ������� ��� �Ұ� ������ Ÿ�� �߿�
            // Ư�� eWASTETYPE(13)�� ���� ���
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

