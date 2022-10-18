#pragma once

#include <SolarHashTable.h>
#include <Singleton.h>
#include <SolarFileReader.h>


struct BattleGroundFlagInfo
{
    BattleGroundFlagInfo();
    ~BattleGroundFlagInfo();

    DWORD collection_id;    // 채집 id
    DWORD strong_point_key; // 거점의 오브젝트키
    DWORD strong_point_name; // 거점 이름(string_code)
    std::vector<DWORD> vector_flag_key; // 깃발의 오브젝트키
};

typedef STLX_MULTIMAP<MAPCODE, BattleGroundFlagInfo*> BATTLE_GROUND_FLAG_INFO_MAP;
typedef BATTLE_GROUND_FLAG_INFO_MAP::iterator BATTLE_GROUND_FLAG_INFO_MAP_ITER;

class BattleGroundFlagInfoParser : public util::Singleton<BattleGroundFlagInfoParser>, public IParser
{
public:
    BattleGroundFlagInfoParser();
    ~BattleGroundFlagInfoParser();
    virtual void Release() {}
    virtual	BOOL LoadScript(eSCRIPT_CODE ScriptCode, BOOL bReload);

    BattleGroundFlagInfo* GetBattleGroundFlagInfo(const MAPCODE map_code, const DWORD collection_id);
    DWORD GetStrongPointName(const MAPCODE map_code, const DWORD object_id);

    const BATTLE_GROUND_FLAG_INFO_MAP& GetBattleGroundFlagInfoHashMap() const { return data_table_; }

private:
    void Unload();
    BATTLE_GROUND_FLAG_INFO_MAP data_table_;
};
