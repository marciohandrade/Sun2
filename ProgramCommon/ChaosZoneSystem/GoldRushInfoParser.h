#pragma once

#ifdef _NA_007065_20140217_CHAOSZONE_GOLDRUSH
#include <Singleton.h>
#include <SolarHashTable.h>
#include <SolarFileReader.h>
#include <ScriptCode.IParser.h>

struct GoldRushInfo
{
    enum eGOLDRUSHINFO
    {
        eMONSTERGRADEPOINT_SIZE = 5,
        eGOLDLEVEL_SIZE = 6
    };
    BYTE index;             // 인덱스
    MAPCODE map_code;       // 맵코드

    DWORD play_time;        // 플레이시간 (초)
    DWORD extra_time;       // 추가시간 (초)

    WORD start_point;       // 시작시, 지급되는 골드포인트
    WORD base_kill_point;   // 적 처치 시, 기본으로 지급하는 골드포인트
    BYTE steal_point_ratio;   // 적 처치 시, 빼앗는 골드포인트 비율 (%)
    BYTE lose_point_ratio;    // 사망 시, 빼앗기는 골드포인트 비율 (%)

    MONSTERCODE last_boss_code; // 골드러시 최종보스 몬스터코드

    // 몬스터 등급별 처치시 획득하는 골드포인트 (몬스터 개별설정이 되어있지 않을 경우 사용)
    WORD monster_grade_point[eMONSTERGRADEPOINT_SIZE]; // 일반/정예/리더/캡틴/보스
    // 골드 등급별 상한 기준치
    WORD gold_grade[eGOLDLEVEL_SIZE];

    GoldRushInfo() :
        index(0), map_code(0),
        play_time(0), extra_time(0),
        start_point(0), base_kill_point(0), steal_point_ratio(0), lose_point_ratio(0),
        last_boss_code(0)
        {}
};

class GoldRushInfoParser : public util::Singleton<GoldRushInfoParser>, public IParser
{
public:
    typedef STLX_HASH_MAP<BYTE, GoldRushInfo*> GoldRushInfo_Hash;   // [인덱스/골드러시정보]

    GoldRushInfoParser() {}
    virtual ~GoldRushInfoParser() {}

    void Init() {}
    void Init(char* pszPackFileName);

    virtual void Release();
    virtual BOOL LoadScript(eSCRIPT_CODE scriptCode, BOOL bReload);

    GoldRushInfo_Hash& goldrushinfo_hash() { return goldrushinfo_hash_; }
    const GoldRushInfo* FindData(const MAPCODE map_code) const;
private:
    BOOL _Load(BOOL is_reload);
    void Unload();

    GoldRushInfo_Hash goldrushinfo_hash_;
};
#endif //_NA_007065_20140217_CHAOSZONE_GOLDRUSH