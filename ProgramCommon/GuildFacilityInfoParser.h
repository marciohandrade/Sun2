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
    BYTE        option_value_type;      // ��ġ Ÿ��    // 1:��ġ, 2:����
    int         option_value;           // ���� ��ġ
};

struct FACILITY_OPTION_ACTIVE
{
    CODETYPE    active_item_code;       // Ȱ��ȭ ������ �ڵ�
    DWORD       active_item_count;      // Ȱ��ȭ ������ �ʿ� ����
    SLOTCODE    active_skill_code;      // ��ų �ڵ�
};

struct FACILITY_INFO 
{
    /** �ü� **/
    std::string   facility_name;          // �ü���
    DWORD         facility_index;         // �ε���
    FCODE         facility_code;          // �ü��ڵ�
    FLEVEL        facility_level;         // �ü�����
    BYTE          upgrade_type;           // ���׷��̵� Ÿ��
    DWORD         upgrade_guild_point;    // ���׷��̵� �ʿ� �������Ʈ
    CODETYPE      upgrade_item_code;      // ���׷��̵� �ʿ� ������ �ڵ�
    DWORD         upgrade_item_num;       // ���׷��̵� �ʿ� ������ ����

    /** �ɼ� **/
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

    /** �ü� ���� **/
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