#include "stdafx.h"
#include "Ability.h"
#include "StatusManager.h"
#include "StatusFactory.h"
#include "AbilityStatus.h"
#include "SkillInfoParser.h"
#include "CharacterFormula.h"
#include "StateInfoParser.h"
#include "LowHPStatus.h"
#include "BonusDamageStatus.h"
#include "Player.h"
#include "Status_Operator.h"
#include "StateInfoParser.h"
#include <ServerOptionParserEx.h>

//==================================================================================================
//  (NOTE) <eCHECK_STATE>  ������ �� ���µ� ��� �߰� ����
//  -   ���� Define���Ž� ���� �ǿ� �ش� ���� �ű�� ������ �ذ�� ���̴�.
//
//  Ÿ���� ����� ���� ����
//#define LIST_SSQ_CTRL()             MAKE_ENUM_CA(eCHAR_STATE_SSQ_CTRL_BLOCK_ATTACK  )
//#define LIST_OBSERVER_MODE()        MAKE_ENUM_CA(eCHAR_STATE_WAR_CTRL_OBSERVER_MODE)

#ifdef _NA001955_110210_WING_COSTUME
    #define SELECT_SPREAD_WINDS_TAG()   MAKE_ENUM_CA(eCHAR_STATE_SPREAD_WINGS)
#else
    #define SELECT_SPREAD_WINDS_TAG()
#endif
#ifdef _NA000000_SPEED_HACK_PREVENTION_SUPPORT
    #define SELECT_SPEED_HACK_PREVENTION_TAG() MAKE_ENUM_CA(eCHAR_STATE_SPEED_HACK_PREVENTION)
#else
    #define SELECT_SPEED_HACK_PREVENTION_TAG()
#endif
//--------------------------------------------------------------------------------------------------
//  (NOTE) <eCHECK_STATE>  ������ �� ���µ� �籸��
//  -   Ư�� ���¿��� �̵�, ���� ���� ���� �Ұ��� �����ϱ� ���� �ʵ��̹Ƿ�,
//  -   ���� �߰��� ���⿡ �߰��Ѵ�. �ٸ� �κ��� ������ �ʿ� ����.
//
#define LIST_CANT_ATTACK_STATES()                           \
    MAKE_ENUM_CA(eCHAR_STATE_DOWN                       )   \
    MAKE_ENUM_CA(eCHAR_STATE_DELAY                      )   \
    MAKE_ENUM_CA(eCHAR_STATE_STUN                       )   \
    MAKE_ENUM_CA(eCHAR_STATE_STONE                      )   \
    MAKE_ENUM_CA(eCHAR_STATE_SLEEP                      )   \
    MAKE_ENUM_CA(eCHAR_STATE_FROZEN                     )   \
    MAKE_ENUM_CA(eCHAR_STATE_BLUR                       )   \
    MAKE_ENUM_CA(eCHAR_STATE_BLUR_TRIGGER               )   \
    MAKE_ENUM_CA(eCHAR_STATE_STEEL                      )   \
    MAKE_ENUM_CA(eCHAR_STATE_THRUST                     )   \
    MAKE_ENUM_CA(eCHAR_STATE_ETC_RETURNING              )   \
    MAKE_ENUM_CA(eCHAR_STATE_SUMMON                     )   \
    MAKE_ENUM_CA(eCHAR_STATE_FEAR                       )   \
    MAKE_ENUM_CA(eCHAR_STATE_MP_FEAR2                   )   \
    MAKE_ENUM_CA(eCHAR_STATE_ATTACK_IMPOSSIBLE          )   \
    MAKE_ENUM_CA(eCHAR_STATE_WAR_CTRL_OBSERVER_MODE     )   \
    MAKE_ENUM_CA(eCHAR_STATE_RIDING_RIDER)                  \
    SELECT_SPREAD_WINDS_TAG()                               \
    MAKE_ENUM_CA(eCHAR_STATE_ZONE_TRANSACTION)              \
    MAKE_ENUM_CA(eCHAR_STATE_FATAL_POINT)                   \
    MAKE_ENUM_CA(eCHAR_STATE_POLYMORPH                  )   \
    MAKE_ENUM_CA(eCHAR_STATE_UPPERDOWN                  )   \
    MAKE_ENUM_CA(eCHAR_STATE_CHARMED                    )


#define LIST_CANT_BEATTACKED_STATES()                       \
	MAKE_ENUM_CA(eCHAR_STATE_STONE                      )   \
	MAKE_ENUM_CA(eCHAR_STATE_STEEL                      )   \
	MAKE_ENUM_CA(eCHAR_STATE_ETC_DISABLE_VISION         )   \
	MAKE_ENUM_CA(eCHAR_STATE_ETC_DISABLE_VISION_TRIGGER )   \
	MAKE_ENUM_CA(eCHAR_STATE_SSQ_CTRL_BLOCK_ATTACK      )   \
	MAKE_ENUM_CA(eCHAR_STATE_WAR_CTRL_OBSERVER_MODE     )   \
	MAKE_ENUM_CA(eCHAR_STATE_ETC_RETURNING)                 \
	MAKE_ENUM_CA(eCHAR_STATE_ZONE_TRANSACTION)                  /* (CHANGES) (f100514.3L) */


#define LIST_CANT_MOVE_STATES()                             \
    MAKE_ENUM_CA(eCHAR_STATE_DOWN                       )   \
    MAKE_ENUM_CA(eCHAR_STATE_DELAY                      )   \
    MAKE_ENUM_CA(eCHAR_STATE_STUN                       )   \
    MAKE_ENUM_CA(eCHAR_STATE_STONE                      )   \
    MAKE_ENUM_CA(eCHAR_STATE_SLEEP                      )   \
    MAKE_ENUM_CA(eCHAR_STATE_FROZEN                     )   \
    MAKE_ENUM_CA(eCHAR_STATE_HOLDING                    )   \
    MAKE_ENUM_CA(eCHAR_STATE_THRUST                     )   \
    MAKE_ENUM_CA(eCHAR_STATE_ETC_FORCED_WARP            )   \
    MAKE_ENUM_CA(eCHAR_STATE_SUMMON                     )   \
    MAKE_ENUM_CA(eCHAR_STATE_FEAR                       )   \
    MAKE_ENUM_CA(eCHAR_STATE_MP_FEAR2                   )   \
    MAKE_ENUM_CA(eCHAR_STATE_WAR_CTRL_OBSERVER_MODE)        \
    MAKE_ENUM_CA(eCHAR_STATE_ETC_TRIGGER_HOLDING)           \
    MAKE_ENUM_CA(eCHAR_STATE_ZONE_TRANSACTION)              \
    MAKE_ENUM_CA(eCHAR_STATE_FATAL_POINT)                   \
    SELECT_SPEED_HACK_PREVENTION_TAG()                      \
    MAKE_ENUM_CA(eCHAR_STATE_UPPERDOWN                  )   \
    MAKE_ENUM_CA(eCHAR_STATE_CHARMED                    )


#define LIST_CANT_USE_SKILL_STATES()                        \
    MAKE_ENUM_CA(eCHAR_STATE_SEALING                    )   \
    MAKE_ENUM_CA(eCHAR_STATE_CONFUSE                    )   \
    MAKE_ENUM_CA(eCHAR_STATE_WAR_CTRL_OBSERVER_MODE     )   \
    MAKE_ENUM_CA(eCHAR_STATE_ZONE_TRANSACTION)                  /* (CHANGES) (f100514.3L) */

// CHANGES: f110315.2L, added new state list to support exclusive action checking
#define LIST_EXCLUSIVE_ACTION_STATES()                      \
    MAKE_ENUM_CA(eCHAR_STATE_RIDING_RIDER)                  \
    SELECT_SPREAD_WINDS_TAG()                               

//  (NOTE) (WAVERIX) (080914) ���� ������ �Ʒ� ���Ϸ� �Ű��.
//  - ������ Ư�� ���°� �ɷ��� ��쿡 ���� ����� �߰��ϸ�
//  �ش� ���¿� ���� ����/�ǰ�/�̵�/��ų ������ ���� ���� �ڵ������� �����ȴ�.
#include "StatusManager_Addin.hxx"


//==================================================================================================
//
StatusFactory*
StatusManager::status_allocator_static_ = NULL;

StatusManager::StatusManager(Character* owner)
    : owner_(owner)
    , current_condition_(eCHAR_CONDITION_STANDUP)
    , transaction_depth_(0)
{
    InterState::ClearField(check_status_field_);
    status_allocator_static_ = StatusFactory::Instance();
}

StatusManager::~StatusManager()
{
    Release();
}

// (CHANGES) ���� ��� ������ ��ϵ��� �ϰ� �����Ѵ�.
// assert(transaction_depth == 0)�� ���¿��� ȣ�� �ʿ�.
void
StatusManager::ReleaseDeleteReservedList()
{
    if (FlowControl::FCAssert(transaction_depth_ == 0) == false) {
        return;
    }

    STATUS_TABLE::iterator it = status_table_.begin(),
                           end = status_table_.end();
    while (it != end)
    {
        STATUS_NODE& node = it->second;
        if (node.delete_reserved == false)
        {
            ++it;
            continue;
        }
        if (node.status->CanRemove() == false)
        {
            ++it;
            continue;
        }
        //node.status->End(); <- already called 'End()' : must be called!
        const eCHAR_STATE_TYPE state_id = static_cast<eCHAR_STATE_TYPE>(it->first);
        InterState::DelRestrictStatus(state_id, check_status_field_, &Status);
        status_allocator_static_->FreeStatus(node.status);
        //
        it = status_table_.erase(it);
    }
}

void
StatusManager::Release()
{
    Status.Clear();
    InterState::ClearField(check_status_field_);

    assert(transaction_depth_ == 0);
    transaction_depth_ = 0;

    //Transaction transaction(this);
    STATUS_TABLE::iterator it = status_table_.begin(),
                           end = status_table_.end();
    for ( ; it != end; ++it)
    {
        STATUS_NODE& node = it->second;
        status_allocator_static_->FreeStatus(node.status);
    }
    status_table_.clear();
}


BOOLEAN
StatusManager::Remove(const STATUS_TABLE::iterator& delete_it)
{
    if (delete_it != status_table_.end())
    {
        STATUS_NODE& node = delete_it->second;
        if (node.delete_reserved) {
            return true;
        }
        if (node.status->CanRemove())
        {
            node.delete_reserved = true;
            node.status->End();
            return true;
        }
    }
    return false;
}

void
StatusManager::CureAll(eSTATE_TYPE state_type)
{
    StateInfoParser* const state_info_parser = StateInfoParser::Instance();
    { // loop transaction
        Transaction transaction(this);
        STATUS_TABLE::iterator it = status_table_.begin(),
                               end = status_table_.end();
        for ( ; it != end; ++it)
        {
            STATUS_NODE& node = it->second;
            BaseStatus* status = node.status;
            if (node.delete_reserved) {
                continue;
            }
            if (status->IsAbilityStatus() == false) {
                continue;
            }
            const eCHAR_STATE_TYPE state_id = static_cast<eCHAR_STATE_TYPE>(status->GetStateID());
            if (state_id == eCHAR_STATE_CURE) {
                continue; // ���� ġ���� ����
            }
            const BASE_STATEINFO* state_info = state_info_parser->GetStateInfo(state_id);
            if (state_info == NULL) {
                continue;
            }
            const eSTATE_TYPE linked_category = static_cast<eSTATE_TYPE>(state_info->m_byType);
            if (linked_category == state_type)
            {
                node.delete_reserved = true;
                node.status->End();
            }
        }
    };

    if (this->transaction_depth_ == 0) {
        ReleaseDeleteReservedList();
    }
}

void
StatusManager::OnDead()
{
    { // loop transaction
        Transaction transaction(this);
        STATUS_TABLE::iterator it = status_table_.begin(),
                               end = status_table_.end();
        for ( ; it != end; ++it)
        {
            STATUS_NODE& node = it->second;
            BaseStatus* status = node.status;
            if (node.delete_reserved) {
                continue;
            }
            if (status->IsAbilityStatus())
            {
                const SkillOption& skill_option = \
                    static_cast<AbilityStatus*>(status)->GetSkillOption();
                if (skill_option.option_ & skill_option.eOption_HoldupBuffPostDead) {
                    continue;
                }
            }
            // (CHANGES) (WAVERIX) (f100315.1) fix up invalid operation problems have related reversed index check,
            // delete control on dead, reversed packet flow
            if (node.status->CanRemove())
            {
                node.delete_reserved = true;
                node.status->End();
            }
        }
    };
    if (this->transaction_depth_ == 0) {
        ReleaseDeleteReservedList();
    }
}

// (NOTE) |PROBLEM=special interface, 1�� ���������� ����� ��.|STATUS=cleared post 100217|
// (CHANGES) (100217) can use this method on all status
void
StatusManager::RemoveNonCashStatus()
{
    // hard-code cache kind (2)
    const BYTE kCash_icon_kind = 2;
    StateInfoParser* const state_info_parser = StateInfoParser::Instance();

    { // loop transaction
        Transaction transaction(this);
        STATUS_TABLE::iterator it = status_table_.begin(),
                               end = status_table_.end();
        for ( ; it != end; ++it)
        {
            STATUS_NODE& node = it->second;
            BaseStatus* status = node.status;
            if (node.delete_reserved) {
                continue;
            }
            const eCHAR_STATE_TYPE state_id = static_cast<eCHAR_STATE_TYPE>(status->GetStateID());
            // (WAVERIX) (090804) (CHANGES) Ÿ���� ��� ����� ���� ����Ʈ ����. Ư�� ���� ó�� ����
            // ���� ĳ�õ� ���� Ȱ������ ó����ȯ����.
            // (NOTE) �ϱ�� ���� ��ũ�� �Ŵ� ������ ���ϴ� ���� ����Ѵ�.
            // ���� ũ��Ƽ���� ��� ����... enum, protocol, result_code, ���... ���� ������,
            // ó�� ������ (����/������)�� �ξ� ���������.
            if (state_id == eCHAR_STATE_ETC_EXIST_SHELD_POINT) {
                continue;
            }
            // ���嵵���ڴ� �������� �ʴ´�
            if (state_id == eCHAR_STATE_FUGITIVE) {
                continue;
            }
            const BASE_STATEINFO* state_info = state_info_parser->GetStateInfo(state_id);
            if (state_info == NULL) {
                continue;
            }

            const BOOLEAN is_non_cash = kCash_icon_kind != state_info->m_byIconKind;
            if (is_non_cash) {
                node.delete_reserved = true;
                node.status->End();
            }
        }
    };
    if (this->transaction_depth_ == 0) {
        ReleaseDeleteReservedList();
    }
}

void
StatusManager::Update(DWORD curTime)
{
    const BOOL NEED_REMOVE = false;

    // ĳ���Ͱ� �̹� �׾�����
    if (owner_->IsAlive() == false)
    {
        OnDead();
        // �׾�߸� �ߵ��Ǵ� ���µ� �ִ�. �׷� ���´� OnDead���� �������� �ʴ´�.
        // declared by __NA_S00015_20080828_NEW_HOPE_SHADOW
        if (status_table_.empty()) {
            return;
        }
    }

    { // loop transaction
        Transaction transaction(this);
        STATUS_TABLE::iterator it = status_table_.begin(),
                               end = status_table_.end();
        for ( ; it != end; ++it)
        {
            STATUS_NODE& node = it->second;
            if (node.delete_reserved) {
                continue;
            }
            BaseStatus* status = node.status;
            if (NEED_REMOVE == status->Update(curTime))
            {
                node.delete_reserved = true;
                node.status->End();
            }
        }
    };
    if (this->transaction_depth_ == 0) {
        ReleaseDeleteReservedList();
    }
}

/*
BaseStatus*
StatusManager::OnAlloc(eCHAR_STATE_TYPE state_id, BOOLEAN is_ability)
{
    // Status�� ���� �Ҵ��ؼ� �߰��Ѵ�.
    BaseStatus* status = status_allocator_static_->AllocStatus(state_id, is_ability);
    if (status)
    {
        STATUS_TABLE::value_type insert_node;
        { //assign
            insert_node.first = state_id;
            STATUS_NODE& node = insert_node.second;
            node.delete_reserved = false;
            node.status = status;
        };
        alloc_reserved_list_.push_back(insert_node);
        return status;
    }
    SUNLOG(eCRITICAL_LOG, "[StatusManager] Can't Allocate AbilityStatus %u\n", state_id);
    return NULL;
}
*/

// DB�� �����Ű�� �ʴ� ���µ�..
BOOLEAN
StatusManager::IsDBSaveStatus(eCHAR_STATE_TYPE state_id)
{
    switch (state_id)
    {
    case eCHAR_STATE_HP_SUCTION :
    case eCHAR_STATE_MP_SUCTION :
    case eCHAR_STATE_VITAL_SUCTION :
    case eCHAR_STATE_THRUST :
    case eCHAR_STATE_CHANGE_ATTR :
    //case eCHAR_STATE_TRANSFORMATION: //(gamelulu) (20090421) (LOGIC-CHANGE) �ɸ��� ���� Ŭ���� �巹�� ���� ���¸� �������� �ʱ� ���� �����߰�
                                       //(gamelulu) (20090618) (LOGIC-CHANGE) ���� ���¸� �����ϱ� ���� �ּ�ó��
        return false;
    }

    if (IsAuroraStatus(state_id) == true)
        return false;

    return true;
}

//!~ _NA_000000_20130812_NEW_CHARACTER_HELLROID
bool StatusManager::IsAuroraStatus(eCHAR_STATE_TYPE state_id)
{
    switch (state_id)
    {
        //!~ ����ų 8�� - ������� 8��
        case eCHAR_STATE_SLOW_AURORA:
        case eCHAR_STATE_WEAKNESS_AURORA:
        case eCHAR_STATE_MISCHANCE_AURORA:
        case eCHAR_STATE_DECLINE_AURORA:
        case eCHAR_STATE_RECOVERY_AURORA:
        case eCHAR_STATE_BOOST_AURORA:
        case eCHAR_STATE_IGNORE_AURORA:
        case eCHAR_STATE_CONCENTRATION_AURORA:
        //~!
            return true;
    }
    return false;
}
//!~ _NA_000000_20130812_NEW_CHARACTER_HELLROID

// desired must be success, shared logic to support 'AllocStatus' and 'AllocAbility'
void
StatusManager::ReplaceToNewStatus(BOOLEAN need_replace,
                                  eCHAR_STATE_TYPE state_id, BaseStatus* new_status,
                                  STATUS_TABLE::iterator* if_need_replace_then_support_pos)
{
    if (need_replace)
    {
        assert(if_need_replace_then_support_pos != NULL);
        STATUS_NODE& finded_node = (*if_need_replace_then_support_pos)->second;
        if (finded_node.delete_reserved == false) {
            finded_node.status->End();
        }
        status_allocator_static_->FreeStatus(finded_node.status);
        finded_node.delete_reserved = false;
        finded_node.status = new_status;
    }
    else
    { //specific control: insert operation is safe in looping status
        std::pair<DWORD, STATUS_NODE> insert_node;
        { //assign
            insert_node.first = state_id;
            STATUS_NODE& node = insert_node.second;
            node.delete_reserved = false;
            node.status = new_status;
        };
        status_table_.insert(insert_node);
        InterState::AddRestrictStatus(state_id, check_status_field_, &Status);
    }
}

ALLOC_STATUS_PAIR 
StatusManager::AllocStatus(eCHAR_STATE_TYPE state_id,
                           int expire_time, int period,
                           int regen_hp, int regen_mp)
{
    ALLOC_STATUS_PAIR result;
    STATUS_TABLE::iterator it = status_table_.find(state_id);
    BOOLEAN enabled_alloc = false, need_replace = false;
    if (it == status_table_.end())
    {
        enabled_alloc = true;
    }
    else
    {
        STATUS_NODE& finded_node = it->second;
        if (finded_node.delete_reserved || finded_node.status->CanRemove())
        {
            enabled_alloc = true;
            need_replace = true;
        }
    }
    if (enabled_alloc == false)
    {
        result.first = false, result.second = NULL;
        return result;
    }
    //
    BaseStatus* status = status_allocator_static_->AllocStatus(state_id, false);
    if (status == NULL)
    {
        result.first = false, result.second = NULL;
        return result;
    }
    //
    ReplaceToNewStatus(need_replace, state_id, status, need_replace ? &it : NULL);
    status->Init(owner_, static_cast<SLOTCODE>(state_id), expire_time, period);
    status->SetRegenInfo(regen_hp, regen_mp);
    status->Start();

#ifdef _CN_000000_20140224_STATE_STEEL_MOD_BUG_LOG2
    if (expire_time == BASE_EXPIRE_TIME_INFINITY && state_id == eCHAR_STATE_STEEL)
    {
        SUNLOG(eCRITICAL_LOG,
            "["__FUNCTION__"]|Msg=Infinity Steel Status Allocated!!||StateID=%d|",state_id);
    }
#endif //_CN_000000_20140224_STATE_STEEL_MOD_BUG_LOG2
    // set success result
    result.first = true, result.second = status;
    return result;
}

AbilityStatus*
StatusManager::AllocAbilityStatus(Character* attacker, Ability* ability)
{
    if (attacker != NULL && attacker->IsDeleted())
    {
        return NULL;
    }

    // TotemNPC�� ���°� �ɸ� �� ����.
    if (owner_->IsEqualObjectKind(TOTEMNPC_OBJECT)) {
        return NULL;
    }

    //����������, �����̳� ũ����Ż�� ���°� �ɸ��� ����.
    if (owner_->IsEqualObjectKind(MAPNPC_OBJECT)) {
        if(((NPC*)owner_)->GetBaseInfo()->m_byGrade == eNPC_CRYSTAL_WARP ||
            ((NPC*)owner_)->GetBaseInfo()->m_byGrade == eNPC_DOMINATION_MAPOBJECT_NPC)
        {
            return NULL;
        }
    }

    const eCHAR_STATE_TYPE state_id = ability->GetStateID();
    // if exist same state in table then replace whether new state have higher than old it.
    STATUS_TABLE::iterator it = status_table_.find(state_id);
    BOOLEAN enabled_alloc = false, need_replace = false;
    if (it == status_table_.end())
    {
        enabled_alloc = true;
    }
    else
    {
        STATUS_NODE& finded_node = it->second;
        if (finded_node.delete_reserved || finded_node.status->CanRemove())
        {
            enabled_alloc = true;
            need_replace = true;
        }
    }
    if (enabled_alloc == false) {
        return NULL;
    }
    AbilityStatus* ability_status = NULL;
    if (need_replace)
    { // current state : finded_node.status != NULL
        STATUS_NODE& finded_node = it->second;
        ability_status = static_cast<AbilityStatus*>(finded_node.status);
        if (ability_status->IsAbilityStatus() == false) {
            return NULL;
        }
        // (NOTE) if true then skip, because new item have low priority
        if (ability_status->ComparePriority(ability)) {
            return NULL;
        }
    }
    //
    ability_status = static_cast<AbilityStatus*>(\
        status_allocator_static_->AllocStatus(state_id, true));
    if (ability_status == NULL) {
        return NULL;
    }
    if (ability_status->IsAbilityStatus() == false)
    {
        SUNLOG(eCRITICAL_LOG,
               "["__FUNCTION__"]|Msg=registering state is not AbilityStatus|"
               "|Owner=%d|SkillCode=%d|AbilityID=%d|StateID=%d|",
               owner_->GetObjectKey(), ability->GetSkillCode(),
               ability->GetAbilityID(), state_id);
        status_allocator_static_->FreeStatus(ability_status);
        return NULL;
    }
    //
    ReplaceToNewStatus(need_replace, state_id, ability_status, need_replace ? &it : NULL);
    ability_status->Init(owner_, attacker, ability);
    ability_status->Start();
    return ability_status;
}

#ifdef _NA_006728_20130520_SKILL_BALANCE_MODIFY
void
StatusManager::UpdateReflect(Character* attacker, DamageArgs* damage_arg)
{
    if (attacker != NULL && attacker->IsDeleted())
    {
        return;
    }

    { // loop transaction
        Transaction transaction(this);
        STATUS_TABLE::iterator it = status_table_.begin(),
            end = status_table_.end();
        for ( ; it != end; ++it)
        {
            STATUS_NODE& node = it->second;
            BaseStatus* status = node.status;
            // (CHANGES) (f100513.1L) (WAVERIX) prevent an unstable condition
            // which process a remove pending status
            if (node.delete_reserved) {
                continue;
            }
            const eCHAR_STATE_TYPE state_id = static_cast<eCHAR_STATE_TYPE>(status->GetStateID());
            __TOUCHED(state_id); // for debug
            if (status->IsAbilityStatus() == false) {
                continue;
            }
            AbilityStatus* ability_status = static_cast<AbilityStatus*>(status);
            if (ability_status->IsReflectStatus() == false) {
                continue;
            }

            /*switch(damage_arg->attack_type_)
            {
            case eATTACK_TYPE_MELEE: // �ٰŸ� �������� �� ��� ����
                break;
            case eATTACK_TYPE_RANGE: // ���Ÿ� �������� �� ��� ������
                continue;
            default: // skill ���� �� ��� skillAttribute ���� 11236(��ų�ٰŸ�) ���ݸ� ����
                {
                    const int skill_code = damage_arg->skill_code_;
                    const SkillScriptInfo* const base_skill_info = \
                        SkillInfoParser::Instance()->GetSkillInfo(skill_code);
                    if (base_skill_info && base_skill_info->m_wSkillAttribute != 11236) {
                        continue;
                    }
                }break;
            }*/

            ability_status->DamageMirror(attacker, damage_arg->damage_);
        }
    };
    if (this->transaction_depth_ == 0) {
        ReleaseDeleteReservedList();
    }
}
#else

void
StatusManager::UpdateReflect(Character* attacker, DAMAGETYPE damage)
{
    if (attacker != NULL && attacker->IsDeleted())
    {
        return;
    }

    { // loop transaction
        Transaction transaction(this);
        STATUS_TABLE::iterator it = status_table_.begin(),
                               end = status_table_.end();
        for ( ; it != end; ++it)
        {
            STATUS_NODE& node = it->second;
            BaseStatus* status = node.status;
            // (CHANGES) (f100513.1L) (WAVERIX) prevent an unstable condition
            // which process a remove pending status
            if (node.delete_reserved) {
                continue;
            }
            const eCHAR_STATE_TYPE state_id = static_cast<eCHAR_STATE_TYPE>(status->GetStateID());
            __TOUCHED(state_id); // for debug
            if (status->IsAbilityStatus() == false) {
                continue;
            }
            AbilityStatus* ability_status = static_cast<AbilityStatus*>(status);
            if (ability_status->IsReflectStatus() == false) {
                continue;
            }
            ability_status->DamageMirror(attacker, damage);
        }
    };
    if (this->transaction_depth_ == 0) {
        ReleaseDeleteReservedList();
    }
}
#endif // _NA_006728_20130520_SKILL_BALANCE_MODIFY

/*
RC::eSKILL_RESULT
StatusManager::CanUseSkill()
{
    return InterState::IsCanUseSkill(check_status_field_);
}
*/

BOOLEAN StatusManager::CanEnchant()
{
    // �ɱ� ������ �� ��æƮ, ��ũ�� ���� �Ұ�
    if (GetCondition() == eCHAR_CONDITION_SITDOWN) {
        return false;
    }
    return true;
}

#ifdef _NA_007667_20141001_WITCHBLADE_SKILL
void StatusManager::Attack(Character* target, DAMAGETYPE damage)
#else
void StatusManager::Attack(DAMAGETYPE damage)
#endif //_NA_007667_20141001_WITCHBLADE_SKILL
{
    // ���� ������ ���
    AbilityStatus* status = NULL;
    status = static_cast<AbilityStatus*>(FindStatus(eCHAR_STATE_ABSORB));
    if (status != NULL)
    {
        status->AttackAbsorb(damage);
    }

#ifdef _NA_007667_20141001_WITCHBLADE_SKILL
    if (damage > 0) {
        UpdateStatusByAttack(target);
    }
#endif //_NA_007667_20141001_WITCHBLADE_SKILL

    // �ɱ� ���¶�� ���� ���·� �ٲ��.
    if (GetCondition() == eCHAR_CONDITION_SITDOWN) {
        ChangeCondition(eCHAR_CONDITION_STANDUP);
    }
    // �������·� ����
    AllocFightingStatus();
}

void
StatusManager::Attacked()
{
    // Sleep ���¶�� �����.
    if (FindStatus(eCHAR_STATE_SLEEP)) {
        Remove(eCHAR_STATE_SLEEP);
    }
    if (FindStatus(eCHAR_STATE_POLYMORPH)) {
        Remove(eCHAR_STATE_POLYMORPH);
    }
    // �ɱ� ���¶�� ���� ���·� �ٲ��.
    if (GetCondition() == eCHAR_CONDITION_SITDOWN) {
        SetCondition(eCHAR_CONDITION_STANDUP);
    }
    AllocFightingStatus();
}

void
StatusManager::AllocFightingStatus()
{
    static const SERVER_COMMON_OPTION& kServerOption = \
        ServerOptionParserEx::Instance()->GetServerOption();

    int fighting_time = 0; 
    if (owner_->IsEqualObjectKind(PLAYER_OBJECT)) {
        fighting_time = kServerOption.m_wPLAYER_FIGHTING_TIME;
    }
    else if (owner_->IsEqualObjectKind(SSQMONSTER_OBJECT)) {
        fighting_time = kServerOption.m_wNPC_FIGHTING_TIME;
    }
    if (0 != fighting_time) {
        AllocStatus(eCHAR_STATE_ETC_FIGHTING, fighting_time);
    }
}

void
StatusManager::Move()
{
    // �ɱ� ���¶�� ���� ���·� �ٲ��.
    if (GetCondition() == eCHAR_CONDITION_SITDOWN) {
        SetCondition(eCHAR_CONDITION_STANDUP);
    }
}

#ifdef _NA_006728_20130520_SKILL_BALANCE_MODIFY
void  StatusManager::Damaged(Character* attacker, DamageArgs* damage_arg)
{

    if (attacker != NULL && attacker->IsDeleted())
    {
        return;
    }
    if (damage_arg->damage_ != 0 && owner_->IsAlive())
    {
#ifdef _NA_008486_20150914_TOTAL_BALANCE
        // �ǰ� ������ ���     
        if (isImmunityDamageState(attacker) == true)
        {
            damage_arg->damage_ = 0;
        }
#else
        // �ǰ� ������ ���     
        if (isImmunityDamageState() == true)
        {
            damage_arg->damage_ = 0;
        }
#endif //_NA_008486_20150914_TOTAL_BALANCE

        // �ǰݷ� ���� ó��
        UpdateReflect(attacker, damage_arg);
        // ���ݴ� �߰� ������
        BonusDamageStatus* bonus_damage_status = \
            static_cast<BonusDamageStatus*>(FindStatus(eCHAR_STATE_STAT_DAMAGE_ADD));
        if (bonus_damage_status) {
            bonus_damage_status->AddDamage(damage_arg->attack_type_, damage_arg->damage_);
        }
    }
}
#else
void
StatusManager::Damaged(Character* attacker, eATTACK_TYPE attack_type, DAMAGETYPE& damage)
{
    if (attacker != NULL && attacker->IsDeleted())
    {
        return;
    }
    if (damage != 0 && owner_->IsAlive())
    {
        // �ǰ� ������ ���
        AbilityStatus* status = static_cast<AbilityStatus*>(FindStatus(eCHAR_STATE_IMMUNITY_DAMAGE));
        if (status)
        {
            // ������ Damage�� ����ؾ� �ϴµ� ���� �Ӽ��� �ִٸ�
            // Damage�� 0���� ����� ������.
            //pStatus->AttackedAbsorb(eATTACK_TYPE_ALL_OPTION, damage);
            damage = 0;
        }
#ifdef _NA_6260_20100507_BUG_FIX_SPDAMAGED_ABSORB
#else // ����
        else
        {
            status = static_cast<AbilityStatus*>(FindStatus(eCHAR_STATE_ABSORB));
            if (status) {
                status->AttackedAbsorb(eATTACK_TYPE_ALL_OPTION, damage);
            }
        }
#endif
        // �ǰݷ� ���� ó��
        UpdateReflect(attacker, damage);
        // ���ݴ� �߰� ������
        BonusDamageStatus* bonus_damage_status = \
            static_cast<BonusDamageStatus*>(FindStatus(eCHAR_STATE_STAT_DAMAGE_ADD));
        if (bonus_damage_status) {
            bonus_damage_status->AddDamage(attack_type, damage);
        }
    }
}
#endif // _NA_006728_20130520_SKILL_BALANCE_MODIFY

#ifdef _NA_6260_20100507_BUG_FIX_SPDAMAGED_ABSORB
void
StatusManager::DamagedAbsorb(const DAMAGETYPE damage)
{
    if (damage != 0 && owner_->IsAlive())
    {
        AbilityStatus* status = static_cast<AbilityStatus*>(FindStatus(eCHAR_STATE_ABSORB));
        if (status) 
        {
            status->AttackedAbsorb(eATTACK_TYPE_ALL_OPTION, damage);
        }
    }
}
#endif

void
StatusManager::ActEmotion()
{
    // �ɱ� ���¶�� ���� ���·� �ٲ��.
    if (GetCondition() == eCHAR_CONDITION_SITDOWN) {
        SetCondition(eCHAR_CONDITION_STANDUP);
    }
    if (owner_->IsMoving()) {
        owner_->StopMoving();
    }
}

void
StatusManager::ChangeHP()
{
    // HP ���� ���ǿ� ���� ���ݷ�, ���� ��ȭ
    LowHPStatus* status = NULL;
    status = static_cast<LowHPStatus*>(FindStatus(eCHAR_STATE_STAT_LOWHP_ATTACK_DECREASE));
    if (status) {
        status->UpdateLowHP();
    }
    status = static_cast<LowHPStatus*>(FindStatus(eCHAR_STATE_STAT_LOWHP_DEFENSE_DECREASE));
    if (status) {
        status->UpdateLowHP();
    }
    status = static_cast<LowHPStatus*>(FindStatus(eCHAR_STATE_STAT_LOWHP_ATTACK_INCREASE));
    if (status) {
        status->UpdateLowHP();
    }
    status = static_cast<LowHPStatus*>(FindStatus(eCHAR_STATE_STAT_LOWHP_DEFENSE_INCREASE));
    if (status) {
        status->UpdateLowHP();
    }
}

//{__NA_001290_20090525_SHIELD_SYSTEM_NOTIFY_NOTABILITY_STATE
BYTE
StatusManager::SerialzieEtcStatusInfo(ETC_STATE_TOTAL_INFO& OUT etc_state_total_info_out)
{
    typedef ETC_STATE_TOTAL_INFO::SLOT_TYPE  SLOT_TYPE;
    BYTE& count_out = etc_state_total_info_out.m_Count = 0;
    // no insert/delete to status_table
    STATUS_TABLE::iterator it = status_table_.begin(),
                           end = status_table_.end();
    for ( ; it != end; ++it)
    {
        STATUS_NODE& node = it->second;
        BaseStatus* status = node.status;
        if (node.delete_reserved) {
            continue;
        }
        if (status->IsNotifyStatus())
        {
            SLOT_TYPE& slot_ref = etc_state_total_info_out.m_Slot[count_out];
            slot_ref.m_wStateCode  = status->GetStateID();
            slot_ref.m_iLeavedTime = status->GetLeavedTime();
            if (slot_ref.m_iLeavedTime != BASE_EXPIRE_TIME_INFINITY) {
                slot_ref.m_iLeavedTime /= 1000;
            }
            //
            ++count_out;
        }
    }
    return count_out;
}
//}

void 
StatusManager::SerialzieStatusInfo(STATE_TOTAL_INFO& OUT state_total_info_out, BOOL in_village)
{
    typedef STATE_TOTAL_INFO::SLOT_TYPE  SLOT_TYPE;
    const size_t kMaxSlotLimit = state_total_info_out.MAX_SLOT_NUM;
    BYTE& count_ref = state_total_info_out.m_Count = 0;

    SkillInfoParser* const skill_parser = SkillInfoParser::Instance();
    // no insert/delete to status_table
    STATUS_TABLE::iterator it = status_table_.begin(),
                           end = status_table_.end();
    for ( ; it != end; ++it)
    {
        STATUS_NODE& node = it->second;
        BaseStatus* status = node.status;
        if (node.delete_reserved) {
            continue;
        }
        if (status->IsAbilityStatus() == false) {
            continue;
        }
        //
        AbilityStatus* ability_status = static_cast<AbilityStatus*>(status);
        /*const*/ SkillScriptInfo* base_skill_info = \
            skill_parser->GetSkillInfo(ability_status->GetSkillCode());

        // �������� ���Ż��¿� ��������ۿ� ���� ������ ��쿡�� �����ش�.
        if (in_village && status->GetStateID() != eCHAR_STATE_TRANSFORMATION)
        {
            if (base_skill_info == NULL || base_skill_info->m_dwItemCode == 0) {
                continue;
            }
        }
        //
        SLOT_TYPE& slot_ref = state_total_info_out.m_Slot[count_ref];
        slot_ref.m_SkillCode = ability_status->GetSkillCode();
        slot_ref.m_bReflect = false;
        slot_ref.m_byAbilityIndex = ability_status->GetIndex();
        { // set slot_ref.m_iLeavedTime
            int leaved_time = ability_status->GetLeavedTime();
            slot_ref.m_iLeavedTime = leaved_time == BASE_EXPIRE_TIME_INFINITY
                                   ?    BASE_EXPIRE_TIME_INFINITY
                                   :    leaved_time / 1000;
        };
        // �ǰݻ��¿� �ɸ� ��� ����ó�� �Ѵ�.
        if (ability_status->IsReflectStatus())
        {
            slot_ref.m_bReflect = true;
            if (base_skill_info == NULL) {
                continue;
            }
            const eCHAR_STATE_TYPE state_id = \
                static_cast<eCHAR_STATE_TYPE>(ability_status->GetStateID());

            base_skill_info->SetFirst();
            BYTE ability_index = 0;
            while (const BASE_ABILITYINFO* ability_info = base_skill_info->GetNext())
            {
                if (ability_info->m_wStateID == state_id)
                {
                    slot_ref.m_byAbilityIndex = ability_index;
                    break;
                };
                ++ability_index;
            }; //end 'foreach ability in abilities'
        }
        //
        ++count_ref;
        // (CHANGES) (WAVERIX) (f100315.1) fix up invalid operation problems have related reversed index check,
        // delete control on dead, reversed packet flow
        if (kMaxSlotLimit <= count_ref) {
            break;
        }
    };
    // CHANGES: f100315.x, removed fighting entergy contents
}

void
StatusManager::SerialzieStatusDetailInfo(STATE_DETAIL_TOTAL_INFO& INOUT state_total_info_out,
                                         eSERIALIZE se)
{
    if (se == SERIALIZE_LOAD)
    {
        const size_t kMaxSlotLimit = state_total_info_out.MAX_SLOT_NUM;
        state_total_info_out.m_Count = 0;

        // no insert/delete to status_table
        STATUS_TABLE::iterator it = status_table_.begin(),
                               end = status_table_.end();
        for ( ; it != end; ++it)
        {
            STATUS_NODE& node = it->second;
            BaseStatus* status = node.status;
            if (node.delete_reserved) {
                continue;
            }
            if (status->IsAbilityStatus() == false) {
                continue;
            }
            // issued by '__NA_S00015_20080828_NEW_HOPE_SHADOW'
            const eCHAR_STATE_TYPE state_id = static_cast<eCHAR_STATE_TYPE>(status->GetStateID());
            if (IsDBSaveStatus(state_id) == false) {
                continue;
            }
            AbilityStatus* ability_status = static_cast<AbilityStatus*>(status);
            // TotemNPC�� ���� ���´� �������� �ʴ´�.(��Ż�� Ÿ�� ���� �ٷ� ������)
            if (ability_status->GetTotemRadius()) {
                continue;
            }
            STATE_DETAIL_TOTAL_INFO::SLOT_TYPE& slot_ref = \
                state_total_info_out.m_Slot[state_total_info_out.m_Count];
            slot_ref.m_wStateCode = ability_status->GetStateID();
            slot_ref.m_SkillCode = ability_status->GetSkillCode();
            //__NA001374_090708_CACHE_ITEM_BUFF_EXTENTION__
            BOOST_STATIC_ASSERT(MAX_ABILITY_NUM == 5);
            //
            const SkillOption& skill_option = ability_status->GetSkillOption();
            BYTE ability_index = static_cast<BYTE>(\
                ability_status->GetIndex() & ~skill_option.eOption_DBStoreRangeMask);
            ability_index |= static_cast<BYTE>(\
                skill_option.option_ & skill_option.eOption_DBStoreRangeMask);
            slot_ref.m_byAbilityIndex = ability_index;
            //
            slot_ref.m_iRemainTime = ability_status->GetLeavedTime();
            slot_ref.m_wInterval = static_cast<uint16_t>(ability_status->GetPeriodTime());

            ++state_total_info_out.m_Count;
            // (CHANGES) (f100318.1L) fix up invalid operation problem, referenced by (f100315.1)
            if (kMaxSlotLimit <= state_total_info_out.m_Count) {
                break;
            }
        }
    }
    else if (se == SERIALIZE_STORE)
    {
        assert(this->transaction_depth_ == 0); //first serialized status allocation routine
        SkillInfoParser* skill_parser = SkillInfoParser::Instance();
        const BYTE upperbound = state_total_info_out.m_Count; //[0, 256)
        for (BYTE state_index = 0; state_index < upperbound; ++state_index)
        {
            const STATE_DETAIL_TOTAL_INFO::SLOT_TYPE& slot_ref = \
                state_total_info_out.m_Slot[state_index];
            // skip if state id equal empty
            if (slot_ref.m_wStateCode == 0) {
                continue;
            }
            const SkillScriptInfo* skill_script_info = \
                skill_parser->GetSkillInfo(slot_ref.m_SkillCode);
            if (skill_script_info == NULL) {
                continue;
            }
            //__NA001374_090708_CACHE_ITEM_BUFF_EXTENTION__
            SkillOption skill_option = { 0, };
            BYTE ability_index = static_cast<BYTE>(\
                slot_ref.m_byAbilityIndex & ~skill_option.eOption_DBStoreRangeMask);
            skill_option.option_ |= static_cast<BYTE>(\
                slot_ref.m_byAbilityIndex & skill_option.eOption_DBStoreRangeMask);
            //
            const BASE_ABILITYINFO* base_ability_info = \
                skill_script_info->GetAbilityInfoByIndex(ability_index);
            if (base_ability_info == NULL) {
                continue;
            }

            const eCHAR_STATE_TYPE state_id = static_cast<eCHAR_STATE_TYPE>(slot_ref.m_wStateCode);
            // if exist same state in table then replace whether new state have higher than old it.
            STATUS_TABLE::iterator it = status_table_.find(state_id);
            if (it != status_table_.end())
            {
                SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"]|Msg=Already exist status|Status=%d|",
                       state_id);
                continue;
            }

            AbilityStatus* ability_status = static_cast<AbilityStatus*>(\
                status_allocator_static_->AllocStatus(state_id, true));
            if (ability_status == NULL) {
                continue;
            }
            // preparation to initialize
            {
                int application_time = slot_ref.m_iRemainTime;
                ability_status->Init(owner_, static_cast<SLOTCODE>(state_id),
                                     base_ability_info, slot_ref.m_SkillCode,
                                     application_time, slot_ref.m_wInterval);
                // Init������ �����ؾ� �ϴ� ���� ������? �ϴ��� �̴�� �ΰ� �ʿ��ϸ� ��������.
                if (skill_option.option_ != skill_option.eOption_None) {
                    ability_status->RegisterSkillOption(skill_option);
                }
            };
            ability_status->Start();
            // (CHANGES) (f100318.1L) fix up missing insert operation that 'AllocAbility' has \
            // the side-effect which is inserted in status table
            ReplaceToNewStatus(false, state_id, ability_status, NULL);
        } //end 'for'
    }
}


RC::eCONDITION_RESULT
StatusManager::ChangeCondition(eCHAR_CONDITION condition)
{
    if (owner_->IsEqualObjectKind(PLAYER_OBJECT) == false) {
        return RC::RC_CONDITION_FAILED;
    }
    Player* player = static_cast<Player*>(owner_);
    // �����ϴ� �������� üũ
    if (condition >= eCHAR_CONDITION_MAX) {
        return RC::RC_CONDITION_INVALID_CONDITION;
    }
    // �̹� ���� �����̸� �ٲ��� �ʴ´�.
    if (GetCondition() == condition) {
        return RC::RC_CONDITION_ALREADY_SAME_CONDITION;
    }
    // �����߿��� �ɱ�/���� ������ �ٲ� �� ����.
    // (NOTE) Ŭ���̾�Ʈ�� �� �ְ�, ������ �ɾ� �ִ� ���� ���ɼ� ���� Ȯ���� ��.
    // �ɾ��� ����� ȸ������ �� �ִ� ���¿��� ��� ��Ȳ ����
    if (player->IsDoingAction()) {
        return RC::RC_CONDITION_DOING_ACTION;
    }
    // �̵��߿��� �ɱ�/���� ������ �ٲ� �� ����.
    if (player->IsMoving()) {
        return RC::RC_CONDITION_DOING_ACTION;
    }
    // �����߿��� �ɱ�/���� �Ұ�
    if (Status.IsDragonTransforming()) {
        return RC::RC_CONDITION_DRAGON_TRANSFORMATION_LIMIT;
    }
    SetCondition(condition);

    MSG_CG_STATUS_CONDITION_CHANGE_BRD msg_brd;
    msg_brd.m_dwObjectKey = owner_->GetObjectKey();
    msg_brd.m_byConditionType = condition;
    owner_->SendPacketAround(&msg_brd, sizeof(msg_brd));

    return RC::RC_CONDITION_SUCCESS;
}

void
StatusManager::SetCondition(eCHAR_CONDITION condition)
{
    current_condition_ = condition;
    // ���¿� ���� HP, MP ȸ������ �ٽ� ����Ѵ�.
    owner_->UpdateCalcRecover(true, true, false);
}

// eStateType�� ���� Status�� ���� ��Ų��.
// byStopCount : ���Ž�ų ����.(0�̸� ���� ��� ���� ����, 1 �̻��� �ش� ������ŭ ����)
BYTE
StatusManager::StopStatusByStateType(eSTATE_TYPE state_type, BYTE number_of_stops)
{
    // hard-code cash kind (2)
    const BYTE  kCash_icon_kind = 2;
    BYTE number = 0;
    StateInfoParser* const state_parser = StateInfoParser::Instance();
    { // loop transaction
        Transaction transaction(this);
        STATUS_TABLE::iterator it = status_table_.begin(),
                               end = status_table_.end();
        for ( ; it != end; ++it)
        {
            STATUS_NODE& node = it->second;
            BaseStatus* status = node.status;
            // (CHANGES) (f100513.1L) (WAVERIX) prevent an unstable condition
            // which process a remove pending status
            if (node.delete_reserved) {
                continue;
            }
            const eCHAR_STATE_TYPE state_id = static_cast<eCHAR_STATE_TYPE>(status->GetStateID());
            const BASE_STATEINFO* state_info = state_parser->GetStateInfo(state_id);
            if (state_info == NULL) {
                continue;
            }
            // Ÿ���� ���ƾ� �ϰ�, ĳ�� ���°� �ƴϾ�� �Ѵ�.
            if (static_cast<eSTATE_TYPE>(state_info->m_byType) == state_type &&
                state_info->m_byIconKind != kCash_icon_kind)
            {
                status->StopStatus();
                ++number;
                if ((number_of_stops != 0) && (number_of_stops == number)) {
                    // (CHANGES) (f100428.1L) fix up a problem
                    // which is occurred lazy release process after a calling of the 'StopStatus'
                    break;
                }
            }
        }
    };
    if (this->transaction_depth_ == 0) {
        ReleaseDeleteReservedList();
    }

    return number;
}

void
StatusManager::UpdateExpiredTime(eCHAR_STATE_TYPE state_type, DWORD expired_time)
{
    BaseStatus* status = FindStatus(state_type);
    if (status)
    {
        const DWORD timeout = GetTickCount() + expired_time;
        status->SetExpiredTime(timeout);
    }
    else
    {
        // ���� : ���� ������ ���ڰ��� ExpiredTime�� �ƴ� iApplicationTime �� ����.
        AllocStatus(state_type, expired_time);
    }
}

void
StatusManager::RemoveEventStatus(BYTE event_code)
{
    if (event_code == 0) {
        return;
    }
    { // loop transaction
        Transaction transaction(this);
        STATUS_TABLE::iterator it = status_table_.begin(),
                               end = status_table_.end();
        for ( ; it != end; ++it)
        {
            STATUS_NODE& node = it->second;
            BaseStatus* status = node.status;
            // (CHANGES) (f100513.1L) (WAVERIX) prevent an unstable condition
            // which process a remove pending status
            if (node.delete_reserved) {
                continue;
            }
            if (status->IsAbilityStatus() == false) {
                continue;
            }
            //const eCHAR_STATE_TYPE state_id = static_cast<eCHAR_STATE_TYPE>(pStatus->GetStateID());
            AbilityStatus* ability_status = static_cast<AbilityStatus*>(status);
            if (ability_status->GetEventCode() == event_code)
            {
                node.delete_reserved = true;
                node.status->End();
            }
        }
    };
    if (this->transaction_depth_ == 0) {
        ReleaseDeleteReservedList();
    }
}

BaseStatus* StatusManager::FindAuroraStatus()
{
    STATUS_TABLE::iterator it = status_table_.begin();
    STATUS_TABLE::iterator end = status_table_.end();
    for ( ; it != end; ++it)
    {
        STATUS_NODE& node = it->second;
        BaseStatus* status = node.status;
        if (node.delete_reserved) {
            continue;
        }
        if (status->IsAbilityStatus() == false) {
            continue;
        }

        if (IsAuroraStatus((eCHAR_STATE_TYPE)status->GetStateID()) == true)
            return status;
    }

    return NULL;
}

#ifdef _NA_007667_20141001_WITCHBLADE_SKILL
void StatusManager::UpdateStatusByAttack( Character* target )
{
    if (target != NULL && target->IsDeleted()) {
        return;
    }
    // �ڱ��ڽſ��� ����� ������ �н��Ѵ�
    // Ÿ���� ���� ��쿡�� �ߵ���Ų��
    if (target == owner_ || owner_->IsFriend(target) != USER_RELATION_ENEMY) {
        return;
    }

    { 
        STATUS_TABLE::iterator it = status_table_.begin(),
            end = status_table_.end();
        for ( ; it != end; ++it)
        {
            STATUS_NODE& node = it->second;
            BaseStatus* status = node.status;
            // (CHANGES) (f100513.1L) (WAVERIX) prevent an unstable condition
            // which process a remove pending status
            if (node.delete_reserved) {
                continue;
            }
            const eCHAR_STATE_TYPE state_id = static_cast<eCHAR_STATE_TYPE>(status->GetStateID());
            __TOUCHED(state_id); // for debug
            if (status->IsAbilityStatus() == false) {
                continue;
            }
            AbilityStatus* ability_status = static_cast<AbilityStatus*>(status);
            if (ability_status->IsAutoCastStatus() == false) {
                continue;
            }

            ability_status->UseSkillByAttack(target);
        }
    };
}

BaseStatus* StatusManager::FindToggleFPStatus()
{
    STATUS_TABLE::iterator it = status_table_.begin();
    STATUS_TABLE::iterator end = status_table_.end();
    for ( ; it != end; ++it)
    {
        STATUS_NODE& node = it->second;
        BaseStatus* status = node.status;
        if (node.delete_reserved) {
            continue;
        }
        if (status->IsAbilityStatus() == false) {
            continue;
        }
        if (status->IsToggleFPStatus() == true)
            return status;
    }

    return NULL;
}

ALLOC_STATUS_PAIR StatusManager::AllocStampStatus( const DWORD collecting_time )
{
    //���̾� �ٵ�, ��ö ���´� ���ν� �������.
    this->Remove(eCHAR_STATE_STEEL);
    this->Remove(eCHAR_STATE_IMMUNITY_DAMAGE);
    this->Remove(eCHAR_STATE_RISING_FORCE_IMMUNITY_DAMAMGE);

    return this->AllocStatus(eCHAR_STATE_STAMP, collecting_time);
}
#endif //_NA_007667_20141001_WITCHBLADE_SKILL

#ifdef _NA_008486_20150914_TOTAL_BALANCE
bool StatusManager::isImmunityDamageState(Character* attacker) const
{
    if (FindStatus(eCHAR_STATE_IMMUNITY_DAMAGE) != NULL
#ifdef _NA_007667_20141001_WITCHBLADE_SKILL
        || FindStatus(eCHAR_STATE_RISING_FORCE_IMMUNITY_DAMAMGE) != NULL
#endif //_NA_007667_20141001_WITCHBLADE_SKILL
        )
    {
        return true;
    }

    StatusManager* attackers_mgr = attacker->GetStatusManager();
    if (attackers_mgr != NULL)
    {
        // owner�� [�������]�̸鼭,
        // attacker�� [����ü����]�� ���� ��쿣 �鿪
        if (FindStatus(eCHAR_STATE_TRANSPARENT) != NULL &&
            attackers_mgr->FindStatus(eCHAR_STATE_DETECTING_HIDE) == NULL)
        {
            return true;
        }
    }
    

    return false;
}
#else
bool StatusManager::isImmunityDamageState() const
{
    if (FindStatus(eCHAR_STATE_IMMUNITY_DAMAGE) != NULL
        || FindStatus(eCHAR_STATE_TRANSPARENT) != NULL 
#ifdef _NA_007667_20141001_WITCHBLADE_SKILL
        || FindStatus(eCHAR_STATE_RISING_FORCE_IMMUNITY_DAMAMGE) != NULL
#endif //_NA_007667_20141001_WITCHBLADE_SKILL
        )
    {
        return true;
    }

    return false;
}
#endif //_NA_008486_20150914_TOTAL_BALANCE

#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
BOOL StatusManager::RemoveDragonTransformStatus()
{
    this->Remove(eCHAR_STATE_TRANSFORMATION);
    return TRUE;
}
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM

#ifdef _NA_008132_20150316_MONDSCHEIN_TEXTLOG
std::string StatusManager::PrintLogStatusList()
{   
    std::string status_list;
    std::ostringstream ost;

    status_list.clear();
    STATUS_TABLE::iterator it, end(status_table_.end());
    for( it = status_table_.begin() ; it != end ; ++it )
    {
        ost.str("");
        ost << it->first;
        status_list += ost.str();
        status_list += "/";
    }

    return status_list;
}
#endif //_NA_008132_20150316_MONDSCHEIN_TEXTLOG