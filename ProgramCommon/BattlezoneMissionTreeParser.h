#ifndef SUN_GAMESERVER_BATTLEZONEMISSIONTREEPARSER_H_
#define SUN_GAMESERVER_BATTLEZONEMISSIONTREEPARSER_H_


#if _MSC_VER > 1000
#pragma once
#endif

//_NA_20110630_BATTLE_ZONE_RENEWAL

#include <SolarHashTable.h>
#include <Singleton.h>
#include <SolarFileReader.h>
#include <SCItemSlot.h>
#include <ScriptCode.IParser.h>

typedef union _INDEX
{
    BYTE menu_depth[4];
    DWORD menu_index;
}BATTLEZONE_TREE_INDEX;

struct BattlezoneMissionTreeInfo 
{
    enum Font_type
    {
        kFontType_Enable = 0,
        kFontType_Disable,
        kFontType_Over,
        kFontType_Select,
        kFontType_Max,
    };

    union
    {
        BYTE menu_depth[4];
        DWORD menu_index;
    };
    // ! ��׷� ��Ʈ��
    DWORD title_string_code;
    // ! �ұ׷� ��Ʈ��
    DWORD subtitle_string_code;
    // ! ���� ��Ʈ��
    DWORD tooltip_string_code;
    // ! �̼� ��Ʈ��
    DWORD mission_string_code;
    // ! ���ڵ�/�̼��ڵ�
    DWORD map_code;
    // ! �ּ� ���� ����
    DWORD min_level;
    // ! �ִ� ���� ����
    DWORD max_level;
    // ! �䱸 ������
    DWORD require_item[MAX_REQUIRE_ITEM];
    // ! �̼Ǹ� ��Ʈ��
    TCHAR mission_name[MAX_ITEMNAME_LENGTH];
    // ! �� Ÿ��
    BYTE kind_of_room;

    DWORD font_color[kFontType_Max];

#ifdef _NA_004204_20120131_BATTLE_ZONE_PARTY_OBTAIN
    BYTE item_obtain_;  //��üȹ�濩��
#endif //_NA_004204_20120131_BATTLE_ZONE_PARTY_OBTAIN
    //------------------------------------------------------------------------------ 
    BattlezoneMissionTreeInfo()
    {
        menu_index = 0;
        title_string_code = 0;
        subtitle_string_code = 0;
        tooltip_string_code = 0;
        map_code = 0;
        min_level = 0;
        max_level = 0;
        ZeroMemory(require_item, sizeof(require_item));
        ZeroMemory(mission_name, sizeof(mission_name));
        kind_of_room = PartyOfMax;
#ifdef _NA_003649_20111115_BATTLE_ZONE_HARDMODE
        ZeroMemory(font_color, sizeof(font_color));
#endif//_NA_003649_20111115_BATTLE_ZONE_HARDMODE
#ifdef _NA_004204_20120131_BATTLE_ZONE_PARTY_OBTAIN
        item_obtain_ = kObtainType_Disable;
#endif //_NA_004204_20120131_BATTLE_ZONE_PARTY_OBTAIN
    }
};

class BattlezoneMissionTreeParser : public util::Singleton<BattlezoneMissionTreeParser>, public IParser
{
public:
    typedef STLX_HASH_MAP<DWORD, BattlezoneMissionTreeInfo*> BattlezoneTree_Hash;

    BattlezoneMissionTreeParser(void);
    ~BattlezoneMissionTreeParser(void);

    void Init();
    void Init(char* pszPackFileName);

    virtual void Release();
    virtual BOOL LoadScript(eSCRIPT_CODE ScriptCode, BOOL bReload);

    const BattlezoneMissionTreeInfo* GetTreeMenu(BYTE lindex, BYTE sindex, BYTE order);
    BYTE* max_menu_depth() { return max_menu_depth_; }

    BattlezoneTree_Hash& battlezone_tree_hash() { return battlezone_tree_hash_; }

    const BattlezoneMissionTreeInfo* FindData(MAPCODE map_code)const;
    const BattlezoneMissionTreeInfo* FindData(MAPCODE map_code, 
                                              PartyZoneType party_zone_type)const;

    eZONETYPE GetZoneType(MAPCODE map_code)const;
    PartyZoneType GetType(MAPCODE map_code)const;

    bool IsMissionItem(MAPCODE map_code, 
                       PartyZoneType party_zone_type, 
                       SLOTCODE require_item)const;

    bool IsMissionOfRequireItem(MAPCODE map_code, 
                                PartyZoneType party_zone_type)const;

#ifdef _NA_004204_20120131_BATTLE_ZONE_PARTY_OBTAIN
    ObtainType GetObtainType(MAPCODE map_code, PartyZoneType party_zone_type) const;
#endif //_NA_004204_20120131_BATTLE_ZONE_PARTY_OBTAIN

private:
    BOOL _Load(BOOL bReload);
    void Unload();
    
    BattlezoneTree_Hash battlezone_tree_hash_;
    BYTE max_menu_depth_[4];
};



#endif //SUN_GAMESERVER_BATTLEZONEMISSIONTREEPARSER_H_
