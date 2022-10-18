#pragma once

#ifndef SERVER_GUILD_FACILITY_INFO_PARSER_H
#define SERVER_GUILD_FACILITY_INFO_PARSER_H
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION

#include <Singleton.h>
#include <ScriptCode.IParser.h>
#include "Struct.h"

struct FACILITY_OPTION_PASSIVE
{
    DWORD       option_type;            // g_ITEM_OPTION
    BYTE        option_value_type;      // 수치 타입    // 1:수치, 2:비율
    int         option_value;           // 적용 수치
};

struct FACILITY_OPTION_ACTIVE
{
    CODETYPE    active_item_code;       // 활성화 아이템 코드
    DWORD       active_item_count;      // 활성화 아이템 필요 갯수
    SLOTCODE    active_skill_code;      // 스킬 코드
};

struct FACILITY_INFO 
{
    /** 시설 **/
    std::string   facility_name;          // 시설명
    DWORD         facility_index;         // 인덱스
    FCODE         facility_code;          // 시설코드
    FLEVEL        facility_level;         // 시설레벨
    BYTE          upgrade_type;           // 업그레이드 타입
    DWORD         upgrade_guild_point;    // 업그레이드 필요 길드포인트
    CODETYPE      upgrade_item_code;      // 업그레이드 필요 아이템 코드
    DWORD         upgrade_item_num;       // 업그레이드 필요 아이템 갯수

    /** 옵션 **/
    FACILITY_OPTION_PASSIVE passive_option;
    FACILITY_OPTION_ACTIVE active_option;
};

class GuildFacilityInfoParser : public util::Singleton<GuildFacilityInfoParser>, public IParser
{
public:
    typedef std::pair<FCODE, FLEVEL> FACILITYTAG;
    typedef std::map<FACILITYTAG, FACILITY_INFO> FACILITY_MAP;
public:
    GuildFacilityInfoParser(void){};
    ~GuildFacilityInfoParser(void){};

public:
    /** Parser **/
    virtual void Release(){};
    virtual	BOOL LoadScript(eSCRIPT_CODE script_code, BOOL is_reload);

    /** 시설 정보 **/
    const FACILITY_INFO* GetGuildFacilityInfo(FCODE  facility_code, FLEVEL facility_level);
    const FACILITY_OPTION_PASSIVE* GetGuildFacilityPassive(FCODE  facility_code, FLEVEL facility_level);
    const FACILITY_OPTION_ACTIVE*  GetGuildFacilityActive(FCODE  facility_code, FLEVEL facility_level);

    const FACILITY_MAP& GetGuildFacilityInfoMap() { return facility_map; }
private:
    const bool IsValidFacilityInfo(FACILITY_INFO* info);

    FACILITY_MAP facility_map;
};

#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
#endif // SERVER_GUILD_FACILITY_INFO_PARSER_H