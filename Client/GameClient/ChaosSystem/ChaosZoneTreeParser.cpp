#include "SunClientPrecompiledHeader.h"

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#include "ChaosSystem/ChaosZoneTreeParser.h"

//------------------------------------------------------------------------------ 
ChaosZoneTreeParser::ChaosZoneTreeParser(void)
{
    max_menu_depth_[0] = 0;
    max_menu_depth_[1] = 0;
    max_menu_depth_[2] = 0;
    max_menu_depth_[3] = 0;
}
//------------------------------------------------------------------------------ 
ChaosZoneTreeParser::~ChaosZoneTreeParser(void)
{
}
VOID ChaosZoneTreeParser::Init()
{
}

VOID ChaosZoneTreeParser::Init(char * pszPackFileName)
{
    SetPackFileName(pszPackFileName);
}
//------------------------------------------------------------------------------ 
void ChaosZoneTreeParser::Release()
{
    Unload();
}

void ChaosZoneTreeParser::Unload()
{
    ChaosZoneTree_Hash::const_iterator it, end( chaoszone_tree_hash_.end() );
    for(it = chaoszone_tree_hash_.begin() ; it != end ; ++it)
    {
        ChaosZoneTreeInfo* info = (it->second);
        SAFE_DELETE(info);
    }

    chaoszone_tree_hash_.clear();
}

BOOL ChaosZoneTreeParser::_Load(BOOL bReload)
{
    if (bReload == TRUE)
    {
        chaoszone_tree_hash().clear();
    }

    int nRowSize = GetRowSize();
    for (int nRow = 0; nRow < nRowSize; ++nRow)
    {
        ChaosZoneTreeInfo* info = new ChaosZoneTreeInfo();

        info->menu_depth[0] = GetDataBYTE("LGroup", nRow);
        info->menu_depth[1] = GetDataBYTE("SGroup", nRow);
        info->menu_depth[2] = GetDataBYTE("Order", nRow);
        info->menu_depth[3] = 0;
#ifdef _NA_007029_20140114_CHAOS_ZONE_ENTER_BLIND
        info->enter_active = GetDataBYTE("Blind", nRow);
#endif //_NA_007029_20140114_CHAOS_ZONE_ENTER_BLIND
        max_menu_depth_[0] = max(max_menu_depth_[0], info->menu_depth[0]);
        max_menu_depth_[1] = max(max_menu_depth_[1], info->menu_depth[1]);
        max_menu_depth_[2] = max(max_menu_depth_[2], info->menu_depth[2]);
        //max_menu_depth[0] = max(max_menu_depth[3], info->menu_depth[3]); //항상 0이므로 비교할 필요 없음
        strncpy(info->mission_name, GetDataString("MName", nRow), MAX_ITEMNAME_LENGTH);
        info->title_string_code = GetDataDWORD("LGroupStringCode", nRow);
        info->subtitle_string_code = GetDataDWORD("SGroupStringCode", nRow);
        info->mission_string_code = GetDataDWORD("MNameStringCode", nRow);
        info->tooltip_string_code = GetDataDWORD("ToolTipStringCode", nRow);
        info->map_code = GetDataDWORD("MapCode", nRow);
        
        sscanf(GetDataString("EnableColor", nRow), "%x", &(info->font_color[ChaosZoneTreeInfo::kFontType_Enable]));
        sscanf(GetDataString("DisableColor", nRow), "%x", &(info->font_color[ChaosZoneTreeInfo::kFontType_Disable]));
        sscanf(GetDataString("OverColor", nRow), "%x", &(info->font_color[ChaosZoneTreeInfo::kFontType_Over]));
        sscanf(GetDataString("SelectColor", nRow), "%x", &(info->font_color[ChaosZoneTreeInfo::kFontType_Select]));
        
        ChaosZoneTree_Hash::_Pairib result = chaoszone_tree_hash().insert(
            ChaosZoneTree_Hash::value_type(static_cast<DWORD>(info->menu_index), info));
        ASSERT(result.second == true);
    }
    return TRUE;
}
//------------------------------------------------------------------------------ 
BOOL ChaosZoneTreeParser::LoadScript(eSCRIPT_CODE scriptCode, BOOL bReload)
{
    switch (scriptCode)
    {
    case SCRIPT_CHAOSZONE_TREE:
        return _Load(bReload);
    }

    return FALSE;
}
//------------------------------------------------------------------------------ 
const ChaosZoneTreeInfo* ChaosZoneTreeParser::GetTreeMenu(BYTE lindex, BYTE sindex, BYTE order)
{
    CHAOSZONE_TREE_INDEX index;
    index.menu_depth[0] = lindex;
    index.menu_depth[1] = sindex;
    index.menu_depth[2] = order;
    index.menu_depth[3] = 0;

    if (index.menu_index == 0) 
    {
        return NULL;
    }

    ChaosZoneTree_Hash::const_iterator found = chaoszone_tree_hash().find(index.menu_index);
    if (found == chaoszone_tree_hash().end()) 
    {
        return NULL;
    }
    const ChaosZoneTreeInfo* info = found->second;
    return info;
}

#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND