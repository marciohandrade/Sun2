#pragma once

#ifdef _NA_006088_20120910_CHAOS_ZONE_FREE_PVP
#include <World/ChaosZoneSystem/ChaosZone.h>
class FreePVP : public ChaosZone
{
public:
    FreePVP(){}
    virtual ~FreePVP(){}

    void Init(GameInstanceDungeon* const instance_dungeon);
    void Release();
    void Update();
    bool EnterPlayer(Player* const player);
    void LeavePlayer(Player* const player);
    void EnterTeam(const ChaosZoneTeam team_key, const BYTE player_count, DWORD* user_key_list);
    void DeadCharacter(Character* const dead_character, Character* const kill_character);
    eUSER_RELATION_TYPE IsFriendPVP(Player* attacker,Player* target);

    void Resurrection(Player* player);

private:
    RC::eSTATUS_RESULT _OnMoveToSafezone(Player* const player);

private:
    PLAYER_HASH player_hash_; // 진행중인 플레이어 리스트
};
#endif // _NA_006088_20120910_CHAOS_ZONE_FREE_PVP
