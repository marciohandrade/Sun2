#pragma once

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
class GameInstanceDungeon;

class ChaosZone
{
public:
    ChaosZone() : instance_dugeon_(NULL)
    {
    }
    virtual ~ChaosZone() {}

    virtual void Init(GameInstanceDungeon* const instance_dungeon) = 0;
    virtual void Release() = 0;
    virtual void Update() = 0;
    virtual bool EnterPlayer(Player* const player) = 0;
    virtual void LeavePlayer(Player* const player) = 0;
    virtual void EnterTeam(const ChaosZoneTeam team_key, const BYTE player_count, 
                                                               DWORD* user_key_list) = 0;
    virtual void DeadCharacter(Character* const dead_character, Character* const kill_character) = 0;
    virtual eUSER_RELATION_TYPE IsFriendPVP(Player* attacker,Player* target) = 0;

    inline GameInstanceDungeon* GetInstanceDungeon() const { return instance_dugeon_; }

#ifdef _NA_008334_20150608_SONNENSCHEIN
    virtual RC::eSTATUS_RESULT ResurrectionAtSafezone( Player *player ) {__UNUSED(player); return RC::RC_STATUS_UNKNOWN;}
#endif //_NA_008334_20150608_SONNENSCHEIN
protected:
    GameInstanceDungeon* instance_dugeon_;
};
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND