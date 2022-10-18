#ifndef SUN_GAMESERVER_BATTLERECORDER_H_
#define SUN_GAMESERVER_BATTLERECORDER_H_

#if _MSC_VER > 1000
#pragma once
#endif

//_NA_0_20091118_GAMEROOM_BATTLE_RESULT

/* Ŭ���� ���̾�׷� : 2010.03.18 by �����

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


/* class BattleRecorder : 2010.03.18 by �����

- ��ϵ� ���� ��Ȳ�� ���� ������ ������.

- �÷��̾���� ���� ��Ȳ(����/����/�� ��)�� ���� ���.

*/


class BattleRecorder
{
public:
    struct Record
    {
        LEVELTYPE   level;                  // �÷��̾� ����
        uint        total_damage_to_enemy;  // ������ ���� ������ �ѷ�
        uint        total_damage_by_enemy;  // ������ ���� ������ �ѷ�
        uint        total_heal_to_party;    // �ڽŹ� ��Ƽ���� ����� �� ����
        uint        max_damage_to_enemy;    // ������ ���� �ִ� ������
        uint        monster_kill_count;     // ���� ���� Ƚ��
        uint        player_kill_count;      // �÷��̾� ���� Ƚ��
        uint        dead_count;             // ���� Ƚ��
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
        BYTE strongpoint_attack;        // ���� ���� ����
        BYTE strongpoint_defense;       // ���� ���� ���
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#ifdef _NA_006633_20130415_ADD_BATTLEGROUND_ASSIST
        uint        assist_count;        // ���� ��ý�Ʈ Ƚ��
#endif //_NA_006633_20130415_ADD_BATTLEGROUND_ASSIST
#ifdef _NA_007065_20140217_CHAOSZONE_GOLDRUSH
        WORD        gold_point;         //������ �������Ʈ(��巯��)
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
