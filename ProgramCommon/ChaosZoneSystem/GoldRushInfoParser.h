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
    BYTE index;             // �ε���
    MAPCODE map_code;       // ���ڵ�

    DWORD play_time;        // �÷��̽ð� (��)
    DWORD extra_time;       // �߰��ð� (��)

    WORD start_point;       // ���۽�, ���޵Ǵ� �������Ʈ
    WORD base_kill_point;   // �� óġ ��, �⺻���� �����ϴ� �������Ʈ
    BYTE steal_point_ratio;   // �� óġ ��, ���Ѵ� �������Ʈ ���� (%)
    BYTE lose_point_ratio;    // ��� ��, ���ѱ�� �������Ʈ ���� (%)

    MONSTERCODE last_boss_code; // ��巯�� �������� �����ڵ�

    // ���� ��޺� óġ�� ȹ���ϴ� �������Ʈ (���� ���������� �Ǿ����� ���� ��� ���)
    WORD monster_grade_point[eMONSTERGRADEPOINT_SIZE]; // �Ϲ�/����/����/ĸƾ/����
    // ��� ��޺� ���� ����ġ
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
    typedef STLX_HASH_MAP<BYTE, GoldRushInfo*> GoldRushInfo_Hash;   // [�ε���/��巯������]

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