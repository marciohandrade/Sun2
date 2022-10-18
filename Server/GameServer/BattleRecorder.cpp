#include "StdAfx.h"
#include ".\BattleRecorder.h"

// _NA_0_20091118_GAMEROOM_BATTLE_RESULT

BattleRecorder::BattleRecorder()
{
    ClearRecord();
}

BattleRecorder::~BattleRecorder()
{
}

void BattleRecorder::ClearRecord()
{
    record_.total_damage_to_enemy = 0;
    record_.total_damage_by_enemy = 0;
    record_.total_heal_to_party = 0;
    record_.max_damage_to_enemy = 0;
    record_.monster_kill_count = 0;
    record_.player_kill_count = 0;
    record_.dead_count = 0;
    record_.level = 0;
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    record_.strongpoint_attack = 0;
    record_.strongpoint_defense = 0;
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#ifdef _NA_006633_20130415_ADD_BATTLEGROUND_ASSIST
    record_.assist_count = 0;
#endif //_NA_006633_20130415_ADD_BATTLEGROUND_ASSIST
#ifdef _NA_007065_20140217_CHAOSZONE_GOLDRUSH
    record_.gold_point = 0;
    record_.cumulative_required_gold_point = 0;
#endif //_NA_007065_20140217_CHAOSZONE_GOLDRUSH
}

#ifdef _NA_007065_20140217_CHAOSZONE_GOLDRUSH
bool BattleRecorder::DoRecord(eBATTLE_RECORD_TYPE record_type, int impact_value)
#else
bool BattleRecorder::DoRecord(eBATTLE_RECORD_TYPE record_type, uint impact_value)
#endif //_NA_007065_20140217_CHAOSZONE_GOLDRUSH
{
    bool dirty = false;
    switch (record_type)
    {
    case BATTLE_RECORD_ATTACK:
        {
            if (impact_value > 0)
            {
                if (record_.max_damage_to_enemy < impact_value)
                    record_.max_damage_to_enemy = impact_value;
                record_.total_damage_to_enemy += impact_value;
                dirty = true;
            }
        }
        break;
    case BATTLE_RECORD_DAMAGE:
        {
            if (impact_value > 0)
            {
                record_.total_damage_by_enemy += impact_value;
                dirty = true;
            }
        }
        break;
    case BATTLE_RECORD_MONSTER_KILL:
        {
            ++(record_.monster_kill_count);
            dirty = true;
        }
        break;
    case BATTLE_RECORD_PLAYER_KILL:
        {
            ++(record_.player_kill_count);
            dirty = true;
        }
        break;
    case BATTLE_RECORD_DEAD:
        {
            ++(record_.dead_count);
            dirty = true;
        }
        break;
    case BATTLE_RECORD_HEAL:
        {
            if (impact_value > 0)
            {
                record_.total_heal_to_party += impact_value;
                dirty = true;
            }
        }
        break;
    case BATTLE_RECORD_LEVEL:
        {
            if (impact_value > 0 && ((record_.level == 0) || (record_.level != impact_value)))
            {
                record_.level = impact_value;
                dirty = true;
            }
        }
        break;
    case BATTLE_RECORD_STRONGPOINT_ATTACK:
        {
            ++(record_.strongpoint_attack);
            dirty = true;
        }
        break;
    case BATTLE_RECORD_STRONGPOINT_DEFENSE:
        {
            ++(record_.strongpoint_defense);
            dirty = true;
        }
        break;
#ifdef _NA_006633_20130415_ADD_BATTLEGROUND_ASSIST
    case BATTLE_RECORD_ASSIST:
        {
            ++(record_.assist_count);
            dirty = true;
        }
        break;
#endif //_NA_006633_20130415_ADD_BATTLEGROUND_ASSIST
#ifdef _NA_007065_20140217_CHAOSZONE_GOLDRUSH
    case BATTLE_RECORD_GOLDPOINT:
        {
            if (impact_value <= 0 && abs(impact_value) > record_.gold_point) {
                // °ñµå °¨¼Ò·®ÀÌ ¼ÒÁö°ñµåº¸´Ù Å¬ ¶§
                record_.gold_point = 0;
            }
            else 
            {
                record_.gold_point += impact_value;
                if (impact_value > 0) {
                    //´©Àû °ñµåÈ¹µæ·® °è»ê
                    record_.cumulative_required_gold_point += impact_value;
                }
            }
            dirty = true;
        }
        break;
#endif //_NA_007065_20140217_CHAOSZONE_GOLDRUSH
    default:
        break;
    }

    return dirty;
}
