#ifndef PROGRAMCOMMON_BATTLEGROUNDINFOPARSER_H_
#define PROGRAMCOMMON_BATTLEGROUNDINFOPARSER_H_


#if _MSC_VER > 1000
#pragma once
#endif

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND

#include <SolarHashTable.h>
#include <Singleton.h>
#include <SolarFileReader.h>
#include <ScriptCode.IParser.h>


struct BattleGroundInfo
{
    enum { MAX_ITEM_LIMIT_COUNT = 10 };

    // ���� ��, ���� ���� �뵵/���ǿ� ���� ���� �ʿ���
    // ! �� Ÿ��
    BYTE index;
    // ! �� �ڵ�
    DWORD map_code;
    // ! ���� ����
    DWORD limit_level;
    // ! ������� ����
    DWORD equipment_rating;
    // ! �ִ� �ο�
    BYTE team_max;
    // ! ��Ƽ��û�����ο�
    BYTE max_party;
    // ! �ִ��Ī���ð�(��)
    DWORD max_match_time;
    // ! ��ߴ��ð�(��)
    DWORD start_time;
    // ! ������ð�(��)
    DWORD entry_time;
    // ! �غ���������ڵ�
    DWORD prepare_condition_code;
    // ! �غ�ð�(��)
    DWORD prepare_time;
    // ! ����Ʈ����Ʈ����ID
    DWORD gate_trigger_id;
    // ! ��ǥ����
    DWORD goal_score;
    // ! ���νð�(��)
    DWORD sealing_time;
    // ! ���ɴ��ð�(��)
    DWORD conquer_wait_time;
    // ! �ڿ�����
    DWORD gather_point;
    // ! �ڿ������ֱ�(��)
    DWORD gather_period;
#ifdef _NA_006599_20130401_BATTLEGROUND_PKPOINT
    // ! �� óġ �� �߰� ȹ�� ������
    BYTE pk_point;
#endif
    // ! ��������ð�(��)
    DWORD dead_time;
    // ! ��Ȱ�ֱ�(��)
    DWORD resurrection_period;
    // ! ��Ȱ�������ڵ�
    DWORD resurrection_condition_code;
    // ! ��ö���ð�(��)
    DWORD iron_mode;
    // ! ���¡ ����Ʈ
    DWORD abusing_point;
    // ! �¸��������ڵ�
    DWORD win_item_code;
    // ! �⺻���ڵ�
    DWORD base_item_code;
    // ! �г�Ƽ�����ڵ�
    DWORD penalty_item_code;
    // ! ������Ÿ������01~10
    DWORD limit_item[MAX_ITEM_LIMIT_COUNT];
    // ! ��ȿó���ð�(��)
    DWORD draw_time;
    // ! �������г�Ƽ����
    DWORD re_enter_condiion;
    // ! �������г�Ƽ�ð�(��)
    DWORD re_enter_condition_period;
    // ! ��Ż�г�Ƽ����
    DWORD exit_condition;
    // ! ��Ż�г�Ƽ�ð�(��)
    DWORD exit_period;


    //------------------------------------------------------------------------------ 
    BattleGroundInfo()
    {
        index = 0;
        map_code = 0;
        limit_level = 0;
        equipment_rating = 0;
        team_max = 0;
        max_party = 0;
        max_match_time = 0;
        start_time = 0;
        entry_time = 0;
        prepare_condition_code = 0;
        prepare_time = 0;
        gate_trigger_id = 0;
        goal_score = 0;
        sealing_time = 0;
        conquer_wait_time = 0;
        gather_point = 0;
        gather_period = 0;
#ifdef _NA_006599_20130401_BATTLEGROUND_PKPOINT
        pk_point = 0;
#endif
        dead_time = 0;
        resurrection_period = 0;
        resurrection_condition_code = 0;
        iron_mode = 0;
        abusing_point = 0;
        win_item_code = 0;
        base_item_code = 0;
        penalty_item_code = 0;
        ZeroMemory(limit_item, (sizeof(limit_item[0])*MAX_ITEM_LIMIT_COUNT));
        draw_time = 0;
        re_enter_condiion = 0;
        re_enter_condition_period = 0;
        exit_condition = 0;
        exit_period = 0;
    }
};

class BattleGroundInfoParser : public util::Singleton<BattleGroundInfoParser>, public IParser
{
public:
    typedef STLX_HASH_MAP<DWORD, BattleGroundInfo*> BattleGroundInfo_Hash;

    BattleGroundInfoParser(void);
    ~BattleGroundInfoParser(void);

    void Init();
    void Init(char* pszPackFileName);

    virtual void Release();
    virtual BOOL LoadScript(eSCRIPT_CODE ScriptCode, BOOL bReload);

    BattleGroundInfo_Hash& battlegroundinfo_hash() { return battlegroundinfo_hash_; }
    const BattleGroundInfo* FindData(const MAPCODE map_code) const;
private:
    BOOL _Load(BOOL bReload);
    void Unload();

    BattleGroundInfo_Hash battlegroundinfo_hash_;
};


#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND

#endif //PROGRAMCOMMON_CHAOSZONETREEPARSER_H_
