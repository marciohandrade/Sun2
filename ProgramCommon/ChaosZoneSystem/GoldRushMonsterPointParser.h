#pragma once

#ifdef _NA_007065_20140217_CHAOSZONE_GOLDRUSH
#include <Singleton.h>
#include <SolarHashTable.h>
#include <SolarFileReader.h>
#include <ScriptCode.IParser.h>

struct GoldRushMonsterPoint
{
    MONSTERCODE monster_code;
    WORD        monster_goldpoint;

    GoldRushMonsterPoint() : monster_code(0), monster_goldpoint(0)
    {}
};

class GoldRushMonsterPointParser : public util::Singleton<GoldRushMonsterPointParser>, public IParser
{
public:
    typedef STLX_HASH_MAP<MONSTERCODE, GoldRushMonsterPoint*> GoldrushMonsterPoint_Hash;   // [몬스터코드/몬스터포인트정보]

    GoldRushMonsterPointParser() {}
    virtual ~GoldRushMonsterPointParser() {}

    void Init() {}
    void Init(char* pszPackFileName);

    virtual void Release();
    virtual BOOL LoadScript(eSCRIPT_CODE scriptCode, BOOL bReload);

    GoldrushMonsterPoint_Hash& monsterpoint_hash() { return monsterpoint_hash_; }
    const GoldRushMonsterPoint* FindData(const MONSTERCODE monster_code) const;
private:
    BOOL _Load(BOOL is_reload);
    void Unload();

    GoldrushMonsterPoint_Hash monsterpoint_hash_;
};
#endif //_NA_007065_20140217_CHAOSZONE_GOLDRUSH