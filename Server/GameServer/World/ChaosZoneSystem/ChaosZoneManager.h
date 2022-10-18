#pragma once

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#include <Singleton.h>
class GameInstanceDungeon;
class ChaosZone;

class ChaosZoneManager
{
public:
    ChaosZoneManager(GameInstanceDungeon* const instace_dungeon) : 
      instace_dungeon_(instace_dungeon),
      chaos_zone_(NULL)
    {
    }
    ~ChaosZoneManager()
    {
    }
    void Init(const ChaosZoneType chaos_zone_type);
    void Release();
    void Update();

    bool EnterPlayer(Player* const player);
    void LeavePlayer(Player* const player);
    void EnterTeam(const ChaosZoneTeam team_key, const BYTE player_count, DWORD* user_key_list);
    eUSER_RELATION_TYPE	IsFriendPVP(Player* attacker,Player* target);

#ifdef _NA_007065_20140217_CHAOSZONE_GOLDRUSH
    void DeadCharacter(Character* const dead_character, Character* const kill_character);
#endif //_NA_007065_20140217_CHAOSZONE_GOLDRUSH

    inline ChaosZoneType GetChaosZoneType() const { return chaos_zone_type_; }
    inline ChaosZone* GetCurrentZone() const { return chaos_zone_; }

#ifdef _NA_008334_20150608_SONNENSCHEIN
    RC::eSTATUS_RESULT ResurrectionAtSafezone(Player* player);
#endif //_NA_008334_20150608_SONNENSCHEIN
private:
    GameInstanceDungeon* instace_dungeon_;
    ChaosZone* chaos_zone_;
    ChaosZoneType chaos_zone_type_;
};
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND