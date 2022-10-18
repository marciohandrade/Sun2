#ifndef SUN_GAMESERVER_BATTLERECORDER_H_
#define SUN_GAMESERVER_BATTLERECORDER_H_

#if _MSC_VER > 1000
#pragma once
#endif

//_NA_0_20091118_GAMEROOM_BATTLE_RESULT

/* 클래스 다이어그램 : 2010.03.18 by 남기봉

                                BattlePlayer <*>---> BattleRecorder
                                             <m>---> Player
                                    A
                                    |
                                  <*:n>
         GameZone <*>-----> GameBattleResult <--- CHuntingBattleResult
            A                                 |
            |                                 |- DominationBattleResult
            |                                 |
            |                                 |- InstanceDungeonBattleResult
            |                                 |
            |                                 |- MissionBattleResult
            |                                 |
            |                                 |- PVPBattleResult
           < > 
CG_ZONE_PLAYER_BATTLE_RESULT_SYN

*/


/* class BattleRecorder : 2010.03.18 by 남기봉

- 기록된 전투 상황에 대한 값들을 가진다.

- 플레이어들의 전투 상황(공격/죽음/힐 등)에 대한 기록.

*/


class BattleRecorder
{
public:
    struct Record
    {
        LEVELTYPE   level;                  // 플레이어 레벨
        uint        total_damage_to_enemy;  // 적에게 입힌 데미지 총량
        uint        total_damage_by_enemy;  // 적에게 당한 데미지 총량
        uint        total_heal_to_party;    // 자신및 파티에게 사용한 총 힐량
        uint        max_damage_to_enemy;    // 적에게 입힌 최대 데미지
        uint        monster_kill_count;     // 몬스터 죽인 횟수
        uint        player_kill_count;      // 플레이어 죽인 횟수
        uint        dead_count;             // 죽은 횟수
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
        BYTE strongpoint_attack;        // 전장 거점 공격
        BYTE strongpoint_defense;       // 전장 거점 방어
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#ifdef _NA_006633_20130415_ADD_BATTLEGROUND_ASSIST
        uint        assist_count;        // 전장 어시스트 횟수
#endif //_NA_006633_20130415_ADD_BATTLEGROUND_ASSIST
#ifdef _NA_007065_20140217_CHAOSZONE_GOLDRUSH
        WORD        gold_point;         //소지한 골드포인트(골드러시)
        WORD cumulative_required_gold_point;
#endif //_NA_007065_20140217_CHAOSZONE_GOLDRUSH
    };

    BattleRecorder();
    ~BattleRecorder();

    void ClearRecord();
#ifdef _NA_007065_20140217_CHAOSZONE_GOLDRUSH
    bool DoRecord(eBATTLE_RECORD_TYPE record_type, int impact_value);
#else
    bool DoRecord(eBATTLE_RECORD_TYPE record_type, uint impact_value);
#endif //_NA_007065_20140217_CHAOSZONE_GOLDRUSH
    const BattleRecorder::Record& get_record() const;

private:
    BattleRecorder::Record  record_;
};

//---------------------------------------------------------------------------------
inline const BattleRecorder::Record& BattleRecorder::get_record() const
{
    return record_;
}

#endif //SUN_GAMESERVER_BATTLERECORDER_H_
