#pragma once


#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
// 카오스존 트리는 배틀존미션 트리과 같은 형태로 만들어짐

#include <SolarHashTable.h>
#include <Singleton.h>
#include <SolarFileReader.h>
#include <SCItemSlot.h>
#include <ScriptCode.IParser.h>

typedef union _CHAOSZONE_TREE_INDEX
{
    BYTE menu_depth[4];
    DWORD menu_index;
}CHAOSZONE_TREE_INDEX;

struct ChaosZoneTreeInfo 
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
    // ! 대그룹 스트링
    DWORD title_string_code;
    // ! 소그룹 스트링
    DWORD subtitle_string_code;
    // ! 툴팁 스트링
    DWORD tooltip_string_code;
    // ! 미션 스트링
    DWORD mission_string_code;
    // ! 맵코드/미션코드
    DWORD map_code;
    // ! 미션명 스트링
    TCHAR mission_name[MAX_ITEMNAME_LENGTH];

#ifdef _NA_007029_20140114_CHAOS_ZONE_ENTER_BLIND
    // ! 입장 활성화 여부
    BYTE enter_active;
#endif //_NA_007029_20140114_CHAOS_ZONE_ENTER_BLIND

    DWORD font_color[kFontType_Max];

    //------------------------------------------------------------------------------ 
    ChaosZoneTreeInfo()
    {
        menu_index = 0;
        title_string_code = 0;
        subtitle_string_code = 0;
        tooltip_string_code = 0;
        mission_string_code = 0;
        map_code = 0;
        ZeroMemory(mission_name, sizeof(mission_name));
        ZeroMemory(font_color, sizeof(font_color));
#ifdef _NA_007029_20140114_CHAOS_ZONE_ENTER_BLIND
        enter_active = 0;
#endif //_NA_007029_20140114_CHAOS_ZONE_ENTER_BLIND
    }
};

class ChaosZoneTreeParser : public util::Singleton<ChaosZoneTreeParser>, public IParser
{
public:
    typedef STLX_HASH_MAP<DWORD, ChaosZoneTreeInfo*> ChaosZoneTree_Hash;

    ChaosZoneTreeParser(void);
    ~ChaosZoneTreeParser(void);

    void Init();
    void Init(char* pszPackFileName);

    virtual void Release();
    virtual BOOL LoadScript(eSCRIPT_CODE ScriptCode, BOOL bReload);

    const ChaosZoneTreeInfo* GetTreeMenu(BYTE lindex, BYTE sindex, BYTE order);
    BYTE* max_menu_depth() { return max_menu_depth_; }

    ChaosZoneTree_Hash& chaoszone_tree_hash() { return chaoszone_tree_hash_; }

private:
    BOOL _Load(BOOL bReload);
    void Unload();

    ChaosZoneTree_Hash chaoszone_tree_hash_;
    BYTE max_menu_depth_[4];
};


#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND

