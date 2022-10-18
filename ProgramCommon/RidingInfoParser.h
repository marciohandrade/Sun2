#ifndef _PROGRAMCOMMON_RIDINGINFO_PARSER_H
#define _PROGRAMCOMMON_RIDINGINFO_PARSER_H
#pragma once

#include <Singleton.h>
#include <ScriptCode.IParser.h>

//==================================================================================================
struct sRiderInfo
{
    enum eSpawnTypes { // control 수치 - 기획 수치 0(제약 없음) 1(마을|필드)
        eSpawn_Disabled = 0,
        eSpawn_Village  = 1 << 0,
        eSpawn_Field    = 1 << 1,
        eSpawn_Battle   = 1 << 2,
    };
    SLOTCODE index_; // "Index" - ItemScript::SkillCode 와 연동 -> SLOTCODE
    int set_speed_; // "SetSpeed"
    BYTE spawn_area_; // "SpawnArea", ref: eSpawnTypes
    bool flying_; //Flying
    WORD spawn_cancel_count_; // "SpawnCancelCount"
    int cancel_count_timer_; // "CountTimer" based on second
    int spawn_timer_;  // "SpawnTime"
    //
    // client graphic control fields
    MONSTERCODE monster_code_; // "MonsterCode"
    char anicode_[2]; // "AniMaping"
    DWORD base_skin[3];// "SetParts1","SetParts2","SetParts3"
    float size_control_;// "SizeControl"
    DWORD set_jump_; // "SetJump"
    DWORD set_hangtime_; // "SetHangTime"
    DWORD spawn_effect_code_; // "SpawnEffectCode"
    //
    bool IsAcceptField(eZONETYPE zone_type) const;
};


//==================================================================================================
class RidingInfoParser : public util::Singleton<RidingInfoParser>, public IParser
{
public:
    RidingInfoParser();
    virtual ~RidingInfoParser();
    // IParser interfaces
    virtual void Release();
    virtual BOOL LoadScript(eSCRIPT_CODE script_code, BOOL reloaded);
    // Local interfaces
    void Init(const char* pack_file_name); // null이면 pack file없는 거당
    const sRiderInfo* GetInfo(DWORD index) const;

private:
    typedef STLX_HASH_MAP<DWORD, sRiderInfo> RiderTable;
    //
    bool _Load(bool reloaded);
    // data field...
    RiderTable rider_table_;
};

//==================================================================================================

inline const sRiderInfo*
RidingInfoParser::GetInfo(DWORD index) const {
    RiderTable::const_iterator it = rider_table_.find(index);
    return it != rider_table_.end() ? &it->second
                                    : 0;
}


inline bool
sRiderInfo::IsAcceptField(eZONETYPE zone_type) const
{
    switch(zone_type)
    {
    case eZONETYPE_VILLAGE:
    case eZONETYPE_TUTORIAL_FIELD: //마을 처럼 처리하도록 한다.
        return !!(spawn_area_ & this->eSpawn_Village);
    case eZONETYPE_FIELD:
        return !!(spawn_area_ & this->eSpawn_Field);
    case eZONETYPE_MISSION:
    case eZONETYPE_CHUNTING:
    case eZONETYPE_INSTANCE:
    case eZONETYPE_PVP:
        return !!(spawn_area_ & this->eSpawn_Battle);
    case eZONETYPE_DOMINATION_FIELD:
#ifdef _NA_000000_20141222_DOMINATION_RIDER_ACTIVATE
        return !!(spawn_area_ & this->eSpawn_Battle); // 점령전에서 탈 것 허용
#else
        return false; // (CHANGES) (091208) (WOPS:5607) 점령전 맵에서는 탈 것 소환 불가
#endif //_NA_000000_20141222_DOMINATION_RIDER_ACTIVATE
#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
    case eZONETYPE_SPA_FIELD:   // 온천에서 탈것불가
        return false;
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM
    }
    return false;
}

#endif //_PROGRAMCOMMON_RIDINGINFO_PARSER_H
