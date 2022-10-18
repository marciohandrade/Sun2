#ifndef PROGRAMCOMMON_CHAOSZONEINFOPARSER_H_
#define PROGRAMCOMMON_CHAOSZONEINFOPARSER_H_


#if _MSC_VER > 1000
#pragma once
#endif

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
// 카오스존 트리는 배틀존미션 트리과 같은 형태로 만들어짐

#include <SolarHashTable.h>
#include <Singleton.h>
#include <SolarFileReader.h>
#include <SCItemSlot.h>
#include <ScriptCode.IParser.h>

struct ChaosZoneInfo
{
    // ! 맵코드/미션코드
    DWORD map_code;
    // ! 최소 권장 레벨
    DWORD min_level;
    // ! 최대 권장 레벨
    DWORD max_level;
    // ! 요구 아이템
    DWORD require_item[MAX_REQUIRE_ITEM];
    // ! 미션명 스트링
    TCHAR mission_name[MAX_ITEMNAME_LENGTH];
    // ! 룸 타입
    BYTE kind_of_room;

    BYTE item_obtain_;  //전체획득여부
    //------------------------------------------------------------------------------ 
    ChaosZoneInfo()
    {
        map_code = 0;
        min_level = 0;
        max_level = 0;
        ZeroMemory(require_item, sizeof(require_item));
        ZeroMemory(mission_name, sizeof(mission_name));
        kind_of_room = PartyOfMax;
        item_obtain_ = kObtainType_Disable;
    }
};

class ChaosZoneInfoParser : public util::Singleton<ChaosZoneInfoParser>, public IParser
{
public:
    typedef STLX_HASH_MAP<DWORD, ChaosZoneInfo*> ChaosZoneInfo_Hash;

    ChaosZoneInfoParser(void);
    ~ChaosZoneInfoParser(void);

    void Init();
    void Init(char* pszPackFileName);

    virtual void Release();
    virtual BOOL LoadScript(eSCRIPT_CODE ScriptCode, BOOL bReload);

    ChaosZoneInfo_Hash& chaoszone_info_hash() { return chaoszone_info_hash_; }

    const ChaosZoneInfo* FindData(MAPCODE map_code)const;
    const ChaosZoneInfo* FindData(MAPCODE map_code, 
        ChaosZoneType zone_type)const;

    eZONETYPE GetZoneType(MAPCODE map_code)const;
    ChaosZoneType GetType(MAPCODE map_code)const;

    bool IsMissionItem(MAPCODE map_code,
        ChaosZoneType zone_type,
        SLOTCODE require_item)const;

    bool IsMissionOfRequireItem(MAPCODE map_code,
        ChaosZoneType zone_type)const;

    ObtainType GetObtainType(MAPCODE map_code, ChaosZoneType zone_type) const;

#ifdef _NA_008334_20150608_SONNENSCHEIN
    MAPCODE GetSonnenScheinMapCode();

#endif //_NA_008334_20150608_SONNENSCHEIN

#ifdef _NA_008543_20151027_ONE_DAYS_GUILD_MISSION
	MAPCODE GetOneDaysGuildmissionMapCode();
#endif //(_NA_008543_20151027_ONE_DAYS_GUILD_MISSION)

private:
    BOOL _Load(BOOL bReload);
    void Unload();

    ChaosZoneInfo_Hash chaoszone_info_hash_;
};


#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND

#endif //PROGRAMCOMMON_CHAOSZONEINFOPARSER_H_
