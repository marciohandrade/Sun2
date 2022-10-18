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

    // 변수 명, 변수 형은 용도/편의에 따라 수정 필요함
    // ! 맵 타입
    BYTE index;
    // ! 맵 코드
    DWORD map_code;
    // ! 제한 레벨
    DWORD limit_level;
    // ! 장비평점 제한
    DWORD equipment_rating;
    // ! 최대 인원
    BYTE team_max;
    // ! 파티신청제한인원
    BYTE max_party;
    // ! 최대매칭대기시간(초)
    DWORD max_match_time;
    // ! 출발대기시간(초)
    DWORD start_time;
    // ! 입장대기시간(초)
    DWORD entry_time;
    // ! 준비버프상태코드
    DWORD prepare_condition_code;
    // ! 준비시간(초)
    DWORD prepare_time;
    // ! 게이트오픈트리거ID
    DWORD gate_trigger_id;
    // ! 목표점수
    DWORD goal_score;
    // ! 각인시간(초)
    DWORD sealing_time;
    // ! 점령대기시간(초)
    DWORD conquer_wait_time;
    // ! 자원점수
    DWORD gather_point;
    // ! 자원습득주기(초)
    DWORD gather_period;
#ifdef _NA_006599_20130401_BATTLEGROUND_PKPOINT
    // ! 적 처치 시 추가 획득 점수값
    BYTE pk_point;
#endif
    // ! 사망유지시간(초)
    DWORD dead_time;
    // ! 부활주기(초)
    DWORD resurrection_period;
    // ! 부활대기상태코드
    DWORD resurrection_condition_code;
    // ! 강철모드시간(초)
    DWORD iron_mode;
    // ! 어뷰징 포인트
    DWORD abusing_point;
    // ! 승리팀보상코드
    DWORD win_item_code;
    // ! 기본상코드
    DWORD base_item_code;
    // ! 패널티보상코드
    DWORD penalty_item_code;
    // ! 아이템타입제한01~10
    DWORD limit_item[MAX_ITEM_LIMIT_COUNT];
    // ! 무효처리시간(초)
    DWORD draw_time;
    // ! 재입장패널티상태
    DWORD re_enter_condiion;
    // ! 재입장패널티시간(초)
    DWORD re_enter_condition_period;
    // ! 이탈패널티상태
    DWORD exit_condition;
    // ! 이탈패널티시간(초)
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
