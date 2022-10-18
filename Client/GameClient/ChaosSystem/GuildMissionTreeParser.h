#pragma once

#ifdef _NA_008334_20150608_SONNENSCHEIN

#include "Singleton.h"
#include <ScriptCode.IParser.h>
#include <SolarHashTable.h>
#include <SolarFileReader.h>

struct GuildMissionInfo
{
    BYTE large_group; // 대그룹
    BYTE sub_group; // 소그룹
    DWORD large_group_string;   // 대그룹 스트링
    DWORD sub_group_string;   // 소그룹 스트링
    DWORD mission_string_code;  // 미션 스트링
    MAPCODE map_code; //맵 코드
    DWORD font_color[4];        // 색상 (활성화,비활성화,마우스오버,선택)
};

class GuildMissionTreeParser : public util::Singleton<GuildMissionTreeParser>, public IParser
{
public:
    typedef STLX_HASH_MAP<MAPCODE, GuildMissionInfo*> GuildMissionInfoHash;
    GuildMissionTreeParser() {}
    ~GuildMissionTreeParser() {}

    VOID Init() {}
    VOID Init(char* file_name);

    virtual VOID Release();
    virtual BOOL LoadScript(eSCRIPT_CODE script_code, BOOL is_reload);

    const GuildMissionInfo* FindData(const MAPCODE& map_code) const;
    const GuildMissionInfo* FindData(const BYTE LGroup, const BYTE SGroup) const; // 클라용. 그룹번호로 찾기

    GuildMissionInfoHash& GetGuildMissionInfoHash() {return info_hash_;}
private:
    GuildMissionInfoHash info_hash_;

    BOOL _Load(BOOL bReload);
    VOID _Unload();
};

#endif //_NA_008334_20150608_SONNENSCHEIN