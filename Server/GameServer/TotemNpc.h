#pragma once
#ifndef GAMESERVER_TOTEM_NPC_H
#define GAMESERVER_TOTEM_NPC_H

#include "NPC.h"
//#include <SCItemSlot.h>

//==================================================================================================
// @history
//  - 110209, waverix, fit in coding style
//
class TotemNpc : public NPC
{
public:
    TotemNpc();
    virtual ~TotemNpc();

    void InitTotemNpc(WORD npc_code, Player* summoner, SCItemSlot& IN item_slot);

    virtual BOOL Update(DWORD delta_tick);
    virtual void Release();

    virtual void OnEnterObject(const Sector& IN sector);
    virtual void SetSummoner(Player* summoner);
    Player* GetSummoner();

    virtual void GetTotemNPCRenderInfo(TOTEMNPC_RENDER_INFO* result_render_info);
    virtual void ForcedAttack(Character* target);
    virtual eUSER_RELATION_TYPE IsFriend(Character* target);
    virtual BOOL CanBeAttacked() const;

    BOOL UseSkill(SLOTCODE skill_code);

protected:
    virtual void SpecialAction();
    virtual void StatusResist();

// _NA_0_20091118_GAMEROOM_BATTLE_RESULT
    virtual void OnRecover(int recover_hp, int recover_mp, int recover_sd = 0,
                           eRECOVER_TYPE recover_type = eRECOVER_TYPE_NORMAL,
                           Character* attacker = NULL);

    virtual RC::eSKILL_RESULT CanUseSkill(SLOTCODE skill_code,
        BOOL reset_cooltimer = true, BOOL is_item_skill = false);
    virtual BOOL ExecuteThrust(BOOL forced_move, WzVector* moving_vector,
        WzVector& pos_of_after_thrust, float moving_distance, BOOL down_after_thrust);
    virtual void ChangeState(DWORD state_id, LPARAM param1 = 0, LPARAM param2 = 0, LPARAM param3 = 0);
    virtual void OnAttack(Character* target, SLOTCODE skillcode, DAMAGETYPE damage);

protected:
    virtual void _Damaged(DamageArgs* damage_arg);
    virtual void AddBattlePoint(Character* attacker,
                                eBATTLE_POINT_TYPE battle_point_type, int battle_point);
    virtual BOOL OnResurrection(
        float recover_exp, float recover_hp, float recover_mp, Player* healer = NULL);
    virtual void OnDead();
    virtual void SetInitialState(
        DWORD state_id, BYTE move_type = 0, DWORD move_area_id = 0, LPARAM param1 = 0);
    virtual void AddDefaultSkill();

    virtual Player* GetPlayerOwner();
    BOOL IsTotemSkillAreaType();
    BOOL ChkSkillPvP(Character* target);

private:
    const BASE_ITEMINFO* item_info_;
    Player* summoner_; // ¼ÒÈ¯ÀÚ
    bool destruction_expired_;
    util::ITimerBase destroy_timer_;
    util::ITimerBase skill_timers_[BASE_NPCINFO::MAX_SKILL_NUM];
};

//==================================================================================================
// inline implementations

inline void TotemNpc::SetSummoner(Player* summoner)
{
    summoner_ = summoner;
}

inline Player* TotemNpc::GetSummoner()
{
    if (obj_util::IsValid(summoner_, __FUNCTION__, __LINE__) == false) {
        return NULL;
    }

    return summoner_;
}

inline BOOL TotemNpc::CanBeAttacked() const {
    return false;
}

inline void TotemNpc::SpecialAction()
{}

inline void TotemNpc::StatusResist()
{}

// _NA_0_20091118_GAMEROOM_BATTLE_RESULT
inline void TotemNpc::OnRecover(int recover_hp, int recover_mp, int recover_sd,
                                eRECOVER_TYPE recover_type, Character* attacker)
{ __UNUSED((recover_hp, recover_mp, recover_sd, recover_type, attacker)); }

inline RC::eSKILL_RESULT TotemNpc::CanUseSkill(
    SLOTCODE skill_code, BOOL reset_cooltimer, BOOL is_item_skill)
{ __UNUSED((skill_code, reset_cooltimer, is_item_skill)); return RC::RC_SKILL_SUCCESS; }

inline BOOL TotemNpc::ExecuteThrust(
    BOOL forced_move, WzVector* moving_vector,
    WzVector& pos_of_after_thrust, float moving_distance, BOOL down_after_thrust)
{
    __UNUSED((forced_move, moving_vector, &pos_of_after_thrust, moving_distance, down_after_thrust));
    return false;
}

inline void TotemNpc::ChangeState(DWORD state_id, LPARAM param1, LPARAM param2, LPARAM param3)
{ __UNUSED((state_id, param1, param2, param3)); }

inline void TotemNpc::OnAttack(Character* target, SLOTCODE skillcode, DAMAGETYPE damage)
{ __UNUSED((target, skillcode, damage)); }


inline void TotemNpc::_Damaged(DamageArgs* damage_arg)
{
    damage_arg->damage_ = 0;
}

inline void TotemNpc::AddBattlePoint(
    Character* attacker, eBATTLE_POINT_TYPE battle_point_type, int battle_point)
{ __UNUSED((attacker, battle_point_type, battle_point)); }

inline BOOL TotemNpc::OnResurrection(
    float recover_exp, float recover_hp, float recover_mp, Player* healer)
{ __UNUSED((recover_exp, recover_hp, recover_mp, healer)); return false; }

inline void TotemNpc::SetInitialState(
    DWORD state_id, BYTE move_type, DWORD move_area_id, LPARAM param1)
{ __UNUSED((state_id, move_type, move_area_id, param1)); }

inline void TotemNpc::AddDefaultSkill()
{}

#endif //GAMESERVER_TOTEM_NPC_H