#pragma once

#ifdef _NA_008334_20150608_SONNENSCHEIN

#include "Singleton.h"
#include <ScriptCode.IParser.h>
#include <SolarHashTable.h>
#include <SolarFileReader.h>

struct GuildMissionInfo
{
    BYTE large_group; // ��׷�
    BYTE sub_group; // �ұ׷�
    DWORD large_group_string;   // ��׷� ��Ʈ��
    DWORD sub_group_string;   // �ұ׷� ��Ʈ��
    DWORD mission_string_code;  // �̼� ��Ʈ��
    MAPCODE map_code; //�� �ڵ�
    DWORD font_color[4];        // ���� (Ȱ��ȭ,��Ȱ��ȭ,���콺����,����)
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
    const GuildMissionInfo* FindData(const BYTE LGroup, const BYTE SGroup) const; // Ŭ���. �׷��ȣ�� ã��

    GuildMissionInfoHash& GetGuildMissionInfoHash() {return info_hash_;}
private:
    GuildMissionInfoHash info_hash_;

    BOOL _Load(BOOL bReload);
    VOID _Unload();
};

#endif //_NA_008334_20150608_SONNENSCHEIN