#pragma once

#include <SolarHashTable.h>
#include <Singleton.h>
#include <SolarFileReader.h>


struct BattleGroundFlagInfo
{
    BattleGroundFlagInfo();
    ~BattleGroundFlagInfo();

    DWORD collection_id;    // ä�� id
    DWORD strong_point_key; // ������ ������ƮŰ
    DWORD strong_point_name; // ���� �̸�(string_code)
    std::vector<DWORD> vector_flag_key; // ����� ������ƮŰ
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
