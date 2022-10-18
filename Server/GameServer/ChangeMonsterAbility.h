#ifndef _GAMESERVER_CHANGE_MONSTER_ABILITY_H
#define _GAMESERVER_CHANGE_MONSTER_ABILITY_H
#pragma once

class GameZone;
class Player;
class NPC;

class ChangeMonsterAbility
{
public:
    ChangeMonsterAbility() {}
    ~ChangeMonsterAbility() {}

    void UpdateMonster(GameZone* pZone, NPC* pNPC);
    void UpdateAllMonster(GameZone* pZone);
    void NotifyChangeAbility(GameZone* pZone, Player* pPlayer);

private:
    // (WAVERIX) (090624) (CHANGES) add-in bFirst by __NA000000_090622_MOB_ABILITY_CONTROL_CHANGE__
    BYTE GetPlayerNum(GameZone* pZone, BOOLEAN bRefresh = false);
};


extern ChangeMonsterAbility g_ChangeMonsterAbility;

#endif //_GAMESERVER_CHANGE_MONSTER_ABILITY_H
