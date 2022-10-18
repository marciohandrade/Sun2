#ifndef __GAMESERVER_STATUS_MANAGER_H
#define __GAMESERVER_STATUS_MANAGER_H
#pragma once

//==================================================================================================
// @history
//  - |~09/10/11|
//  - |09/10/11|waverix|make DEL_LIST to cheap routine
//  - |10/02/16|waverix|CHANGES=code refactoring about status manager|
//                     |CHANGES=delete macro '__NA_S00015_20080828_NEW_HOPE_SHADOW (902)'\
//                                           '__NA_S00028_SHADOW_ADD_WORK (902)'|
//                     |UPDATES=changes status manager which is had recursive graph call,\
//                      that can remove only in remove routine to simple transaction| 
//  - |10/02/18|waverix|CHANGES=code refactoring about status manager|STEP=2nd|
//                     |CHANGES=fix up invalid operation removed flag by test value|
//  - TODO : unify delete_reserved and BaseStatus::StopStatus
//  - |10/08/22|waverix|CHANGES=change a name of AllocAbility to AllocAbilityStatus

#include "BaseStatus.h"
#include "Status_Operator.h"
#include "AnimationDelayControl.h"  //__NA001003_080527_PLAYER_ANIMATION_TIME_CONTROL__

class Character;
class Ability;
class AbilityStatus;
class StatusFactory;
class SkillInfoParser;
struct STATE_INFO;

typedef std::pair<BOOL, BaseStatus*> ALLOC_STATUS_PAIR;

//==================================================================================================
//  <StatusManager>
//
class StatusManager
{
    typedef DWORD64 RESTRICT_STATUS_BITFIELD;
    struct InterState;
    struct Transaction;
    friend struct Transaction;

    struct STATUS_NODE
    {
        BOOLEAN delete_reserved;
        //BOOLEAN reserved sections[3];
        BaseStatus* status;
    };
    typedef STLX_MAP<DWORD, STATUS_NODE> STATUS_TABLE;
public:
//--------------------------------------------------------------------------------------------------
    StatusManager(Character* owner);
    ~StatusManager();

    void  Release();

    void  Update(DWORD delta_tick);
#ifdef _NA_006728_20130520_SKILL_BALANCE_MODIFY
    void  UpdateReflect(Character* attacker, DamageArgs* damage_arg); 
#else
    void  UpdateReflect(Character* attacker, DAMAGETYPE damage);
#endif // _NA_006728_20130520_SKILL_BALANCE_MODIFY
#ifdef _NA_007667_20141001_WITCHBLADE_SKILL
    void UpdateStatusByAttack(Character* target);
#endif //_NA_007667_20141001_WITCHBLADE_SKILL

    ALLOC_STATUS_PAIR AllocStatus(eCHAR_STATE_TYPE state_id,
                                  int expire_time = BASE_EXPIRE_TIME_INFINITY,
                                  int period = 0,
                                  int regen_hp = 0,
                                  int regen_mp = 0);
    AbilityStatus* AllocAbilityStatus(Character* attacker, Ability* ability);
    // (NOTE) change paradigm from 'delete node' action to 'delete reserved' tagging
    BOOLEAN Remove(eCHAR_STATE_TYPE state_id);

    void  CureAll(eSTATE_TYPE state_type);

    // 기타 사용 함수들
    bool CanAttack(); // 공격할 수 있는 상태인가
    bool CanBeAttacked();

    bool CanMove(const eCHAR_STATE_TYPE exception_type = eCHAR_STATE_INVALID);
    // CHANGES: f110315.2L, check whether request action is acceptable like a riding or spread wings.
    bool CanSpecialAction(const eCHAR_STATE_TYPE exception_type = eCHAR_STATE_INVALID) const;

    BaseStatus* FindStatus(eCHAR_STATE_TYPE state_id) const;
    // added by __NA_S00015_20080828_NEW_HOPE_SHADOW
    // |RETURNS=real stopped status|
    BYTE  StopStatusByStateType(eSTATE_TYPE state_type, BYTE number_of_stops);
    BOOLEAN IsDBSaveStatus(eCHAR_STATE_TYPE state_id);
    
    //!~ _NA_000000_20130812_NEW_CHARACTER_HELLROID
    bool IsAuroraStatus(eCHAR_STATE_TYPE state_id);
    BaseStatus* FindAuroraStatus();
    //~! _NA_000000_20130812_NEW_CHARACTER_HELLROID
#ifdef _NA_007667_20141001_WITCHBLADE_SKILL
    BaseStatus* FindToggleFPStatus();
#endif //_NA_007667_20141001_WITCHBLADE_SKILL

    // Static 상태 관련
    eCHAR_CONDITION GetCondition() const { return current_condition_; }
    void  SetCondition(eCHAR_CONDITION condition);
    // 상태를 바꾸고 주변에 알린다.
    RC::eCONDITION_RESULT ChangeCondition(eCHAR_CONDITION condition);

    // SSQ Field에서 공격/피격 불가능 상태를 강제 설정하기 위해 추가됨.
    // 별도의 실제 상태 객체를 가지는 게 아니다.
    void  ChangeInterState(const eCHAR_STATE_TYPE state, const BOOLEAN is_add);
    void  RemoveNonCashStatus(); // special interface, 1차 레벨에서만 사용할 것. --> cleared
    RC::eSKILL_RESULT CanUseSkill();
    BOOLEAN CanEnchant();
    void  AllocFightingStatus(); //__NA_001290_20090525_SHIELD_SYSTEM

    void  ActEmotion(); // Emotion 동작을 시작한다.
    void  ChangeHP();
    void  Move();

    void  SerialzieStatusInfo(STATE_TOTAL_INFO& OUT state_total_info_out, BOOL is_in_village = false);
    void  SerialzieStatusDetailInfo(STATE_DETAIL_TOTAL_INFO& INOUT state_total_info, eSERIALIZE se);
    //__NA_001290_20090525_SHIELD_SYSTEM_NOTIFY_NOTABILITY_STATE
    BYTE  SerialzieEtcStatusInfo(ETC_STATE_TOTAL_INFO& OUT etc_state_total_info_out);
    //__NA_S00028_SHADOW_ADD_WORK
    void  UpdateExpiredTime(eCHAR_STATE_TYPE state_type, DWORD expired_time);
    void  RemoveEventStatus(BYTE event_code);

    // 현재 데미지 면역상태인지 체크
#ifdef _NA_008486_20150914_TOTAL_BALANCE
    bool isImmunityDamageState(Character* attacker) const;
#else
    bool isImmunityDamageState() const;
#endif //_NA_008486_20150914_TOTAL_BALANCE

#ifdef _NA_007667_20141001_WITCHBLADE_SKILL
    // 각인상태를 건다. 동시에 각인시 제거되어야 할 상태들도 제거
    ALLOC_STATUS_PAIR AllocStampStatus(const DWORD collecting_time);
    void  Attack(Character* target, DAMAGETYPE damage); // Owner가 공격을 개시할 때 처리
#else
    void  Attack(DAMAGETYPE damage); // Owner가 공격을 개시할 때 처리
#endif //_NA_007667_20141001_WITCHBLADE_SKILL
    void  Attacked();   // Owner가 공격을 받았을 때 처리

    // Owner가 데미지를 받았을 때 처리
#ifdef _NA_006728_20130520_SKILL_BALANCE_MODIFY
    void  Damaged(Character* attacker, DamageArgs* damage_arg);
#else
    void  Damaged(Character* attacker, eATTACK_TYPE attack_type, DAMAGETYPE& damage);
#endif // _NA_006728_20130520_SKILL_BALANCE_MODIFY
    void  DamagedAbsorb(const DAMAGETYPE damage); //_NA_6260_20100507_BUG_FIX_SPDAMAGED_ABSORB

#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
    bool IsDragonTransforming() {return Status.IsDragonTransforming(); }
    BOOL RemoveDragonTransformStatus();
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM

#ifdef _NA_008132_20150316_MONDSCHEIN_TEXTLOG
    std::string PrintLogStatusList();
#endif //_NA_008132_20150316_MONDSCHEIN_TEXTLOG
private:
    // (CHANGES) 삭제 대기 상태의 목록들을 일괄 삭제한다.
    // assert(transaction_depth == 0)인 상태에서 호출 필요.
    void  ReleaseDeleteReservedList();
    BOOLEAN Remove(const STATUS_TABLE::iterator& delete_it);
    // desired must be success, shared logic to support 'AllocStatus' and 'AllocAbility'
    void  ReplaceToNewStatus(BOOLEAN need_replace,
                             eCHAR_STATE_TYPE state_id, BaseStatus* new_status,
                             STATUS_TABLE::iterator* if_need_replace_then_support_pos);
    void  OnDead();

    // (NOTE) ordered fields based on access used frequency
public:
    // (Property)
    StatusField  Status; //  Property <- 각종 정책 제어
private:
    // (Fields)
    Character* const owner_;
    eCHAR_CONDITION current_condition_; //  유일하게 가질 수 있는 현재 상태
    STATUS_TABLE status_table_;
    //  - 특정 상태에서 해당 액션이 허용되는지 여부 검사용
    RESTRICT_STATUS_BITFIELD check_status_field_;
    int transaction_depth_; // prevent recursive loop to deleted iterators
public:
    AnimationDelayControl AnimationDelay;
private:
    static StatusFactory* status_allocator_static_;
    __DISABLE_COPY(StatusManager);
};


//==================================================================================================
//
struct StatusManager::Transaction
{
    Transaction(StatusManager* status_manager)
        : status_manager_(status_manager)
    {
        ++status_manager_->transaction_depth_;
    };
    ~Transaction()
    {
        assert(status_manager_->transaction_depth_ > 0);
        if (status_manager_->transaction_depth_ > 0) {
            --status_manager_->transaction_depth_;
        }
    }
    StatusManager* const status_manager_;
    __DISABLE_COPY(Transaction);
};

struct StatusManager::InterState
{
    enum eCHECK_STATE;
    static void ClearField(RESTRICT_STATUS_BITFIELD& field);
    static bool IsCanAttack(const RESTRICT_STATUS_BITFIELD& field);
    static bool IsCanBeAttacked(const RESTRICT_STATUS_BITFIELD& field);
    static bool IsCanMove(const RESTRICT_STATUS_BITFIELD& field);
    static bool IsCanSpecialAction(const RESTRICT_STATUS_BITFIELD& field);
    static RC::eSKILL_RESULT IsCanUseSkill(const RESTRICT_STATUS_BITFIELD& field);
    // (CHANGES) (100218) (WAVERIX) fix up invalid operation removed flag by test value
    // specification : if status_field is not null then default processing
    //                 else then simulate based on current state
    static void AddRestrictStatus(const eCHAR_STATE_TYPE state_id,
                                  RESTRICT_STATUS_BITFIELD& INOUT field,
                                  StatusField* status_field);
    static void DelRestrictStatus(const eCHAR_STATE_TYPE state_id,
                                  RESTRICT_STATUS_BITFIELD& INOUT field,
                                  StatusField* status_field);
};

//==================================================================================================

inline BaseStatus*
StatusManager::FindStatus(eCHAR_STATE_TYPE state_id) const
{
    // (NOTE) ignore 'delete_reserved'
    STATUS_TABLE::const_iterator it = status_table_.find(state_id);
    if (it != status_table_.end())
    {
        const STATUS_NODE& node = it->second;
        return node.status;
    }
    return NULL;
}

inline BOOLEAN
StatusManager::Remove(eCHAR_STATE_TYPE state_id)
{
    STATUS_TABLE::iterator it = status_table_.find(state_id);
    if (it != status_table_.end()) {
        return Remove(it);
    }
    return false;
}

inline bool
StatusManager::CanAttack() // 공격할 수 있는 상태인가
{
    const bool is_can_attack = !(Status.FIELD & Status.ENABLE_GM_TRANSPARENT) &&
                                 InterState::IsCanAttack(check_status_field_);
    return is_can_attack;
}

inline bool
StatusManager::CanBeAttacked()
{
    // 투명 상태에서도 공격은 받을수 있다. 타켓은 잡히지 않는다. - (1002xx~)
    const bool is_can_be_attacked = !(Status.FIELD&Status.ENABLE_GM_TRANSPARENT) &&
                                      InterState::IsCanBeAttacked(check_status_field_);
    return is_can_be_attacked;
}

inline bool
StatusManager::CanMove(const eCHAR_STATE_TYPE excepted_type)
{
    RESTRICT_STATUS_BITFIELD filtered = check_status_field_;
    if (eCHAR_STATE_INVALID != excepted_type) {
        // (CHANGES) (100218) (WAVERIX) fix up invalid operation removed flag by test value
        InterState::DelRestrictStatus(excepted_type, filtered, NULL);
    }
    bool is_can_move = InterState::IsCanMove(filtered);
    return is_can_move;
}

inline RC::eSKILL_RESULT
StatusManager::CanUseSkill() {
    return InterState::IsCanUseSkill(check_status_field_);
}

// CHANGES: f110315.2L, check whether request action is acceptable like a riding or spread wings.
inline bool StatusManager::CanSpecialAction(const eCHAR_STATE_TYPE excepted_type) const
{
    //return InterState::IsCanSpecialAction(check_status_field_);
    RESTRICT_STATUS_BITFIELD filtered = check_status_field_;
    if (eCHAR_STATE_INVALID != excepted_type) {
        // (CHANGES) (100218) (WAVERIX) fix up invalid operation removed flag by test value
        InterState::DelRestrictStatus(excepted_type, filtered, NULL);
    }
    bool can_special_action = InterState::IsCanSpecialAction(filtered);
    return can_special_action;
}

inline void
StatusManager::ChangeInterState(const eCHAR_STATE_TYPE state, const BOOLEAN is_add)
{
    if(is_add) {
        InterState::AddRestrictStatus(state, check_status_field_, &Status);
    }
    else {
        InterState::DelRestrictStatus(state, check_status_field_, &Status);
    }
}

#endif //__GAMESERVER_STATUS_MANAGER_H


