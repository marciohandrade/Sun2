#include "StdAfx.h"

//_NA_20110630_BATTLE_ZONE_RENEWAL
#include ".\BattlezoneMissionTreeParser.h"

//------------------------------------------------------------------------------ 
BattlezoneMissionTreeParser::BattlezoneMissionTreeParser(void)
{
    max_menu_depth_[0] = 0;
    max_menu_depth_[1] = 0;
    max_menu_depth_[2] = 0;
    max_menu_depth_[3] = 0;
}
//------------------------------------------------------------------------------ 
BattlezoneMissionTreeParser::~BattlezoneMissionTreeParser(void)
{
}
VOID BattlezoneMissionTreeParser::Init()
{
}

VOID BattlezoneMissionTreeParser::Init(char * pszPackFileName)
{
    SetPackFileName(pszPackFileName);
}
//------------------------------------------------------------------------------ 
void BattlezoneMissionTreeParser::Release()
{
    Unload();
}

void BattlezoneMissionTreeParser::Unload()
{
    BattlezoneTree_Hash::const_iterator it, end( battlezone_tree_hash_.end() );
    for(it = battlezone_tree_hash_.begin() ; it != end ; ++it)
    {
        BattlezoneMissionTreeInfo* info = (it->second);
        SAFE_DELETE(info);
    }

    battlezone_tree_hash_.clear();	
}

BOOL BattlezoneMissionTreeParser::_Load(BOOL bReload)
{

    if (bReload == TRUE)
    {
        battlezone_tree_hash().clear();
    }

    int nRowSize = GetRowSize();
    for (int nRow = 0; nRow < nRowSize; ++nRow)
    {
        BattlezoneMissionTreeInfo* info = new BattlezoneMissionTreeInfo();

        info->menu_depth[0] = GetDataBYTE("LGroup", nRow);
        info->menu_depth[1] = GetDataBYTE("SGroup", nRow);
        info->menu_depth[2] = GetDataBYTE("Order", nRow);
        info->menu_depth[3] = 0;
        info->kind_of_room = GetDataBYTE("Kind", nRow);
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
        info->min_level = GetDataDWORD("ReqMinLev", nRow);
        info->max_level = GetDataDWORD("ReqMaxLev", nRow);
        info->require_item[0] = GetDataDWORD("ReqItem1", nRow);
        info->require_item[1] = GetDataDWORD("ReqItem2", nRow);
        info->require_item[2] = GetDataDWORD("ReqItem3", nRow);
        info->require_item[3] = GetDataDWORD("ReqItem4", nRow);
        info->require_item[4] = GetDataDWORD("ReqItem5", nRow);
        info->require_item[5] = GetDataDWORD("ReqItem6", nRow);
        info->require_item[6] = GetDataDWORD("ReqItem7", nRow);
        info->require_item[7] = GetDataDWORD("ReqItem8", nRow);
        info->require_item[8] = GetDataDWORD("ReqItem9", nRow);
        info->require_item[9] = GetDataDWORD("ReqItem10", nRow);
#ifdef _NA_003649_20111115_BATTLE_ZONE_HARDMODE
        sscanf(GetDataString("EnableColor", nRow), "%x", &(info->font_color[BattlezoneMissionTreeInfo::kFontType_Enable]));
        sscanf(GetDataString("DisableColor", nRow), "%x", &(info->font_color[BattlezoneMissionTreeInfo::kFontType_Disable]));
        sscanf(GetDataString("OverColor", nRow), "%x", &(info->font_color[BattlezoneMissionTreeInfo::kFontType_Over]));
        sscanf(GetDataString("SelectColor", nRow), "%x", &(info->font_color[BattlezoneMissionTreeInfo::kFontType_Select]));
#endif//_NA_003649_20111115_BATTLE_ZONE_HARDMODE
#ifdef _NA_004204_20120131_BATTLE_ZONE_PARTY_OBTAIN
        info->item_obtain_ = GetDataBYTE("ItemObtain", nRow);
#endif //_NA_004204_20120131_BATTLE_ZONE_PARTY_OBTAIN

        BattlezoneTree_Hash::_Pairib result = battlezone_tree_hash().insert(
            BattlezoneTree_Hash::value_type(static_cast<DWORD>(info->menu_index), info));
        ASSERT(result.second == true);       
    }
    return TRUE;
}
//------------------------------------------------------------------------------ 
BOOL BattlezoneMissionTreeParser::LoadScript(eSCRIPT_CODE scriptCode, BOOL bReload)
{
    switch (scriptCode)
    {
    case SCRIPT_BATTLEZONE_TREE:
        return _Load(bReload);
    }

    return FALSE;
}
//------------------------------------------------------------------------------ 
const BattlezoneMissionTreeInfo* BattlezoneMissionTreeParser::GetTreeMenu(BYTE lindex, BYTE sindex, BYTE order)
{
    BATTLEZONE_TREE_INDEX index;
    index.menu_depth[0] = lindex;
    index.menu_depth[1] = sindex;
    index.menu_depth[2] = order;
    index.menu_depth[3] = 0;

    if (index.menu_index == 0) 
    {
        return NULL;
    }

    BattlezoneTree_Hash::const_iterator found = battlezone_tree_hash().find(index.menu_index);
    if (found == battlezone_tree_hash().end()) 
    {
        return NULL;
    }
    const BattlezoneMissionTreeInfo* info = found->second;
    return info;
}

const BattlezoneMissionTreeInfo* 
BattlezoneMissionTreeParser::FindData(MAPCODE map_code)const
{
    BattlezoneTree_Hash::const_iterator begin_iterator = battlezone_tree_hash_.begin(),
                                        end_iterator = battlezone_tree_hash_.end();

    for (; begin_iterator != end_iterator; ++begin_iterator)
    {
        const BattlezoneMissionTreeInfo* find_info = begin_iterator->second;
        if (find_info->map_code == map_code)
        {
            return find_info;
        }
    }
    return NULL;
}

const BattlezoneMissionTreeInfo* 
BattlezoneMissionTreeParser::FindData(MAPCODE map_code, PartyZoneType party_zone_type)const
{
    BattlezoneTree_Hash::const_iterator begin_iterator = battlezone_tree_hash_.begin(),
                                        end_iterator = battlezone_tree_hash_.end();

    BYTE compair_zont_type = static_cast<BYTE>(party_zone_type);

    for (; begin_iterator != end_iterator; ++begin_iterator)
    {
        const BattlezoneMissionTreeInfo* find_info = begin_iterator->second;
        if (find_info->map_code == map_code && compair_zont_type == find_info->kind_of_room)
        {
            return find_info;
        }
    }
    return NULL;
}

eZONETYPE
BattlezoneMissionTreeParser::GetZoneType(MAPCODE map_code)const
{
    const BattlezoneMissionTreeInfo* info = FindData(map_code);
    if (info == NULL)
    {
        return eZONETYPE_MAX;
    }

    PartyZoneType party_room_type = static_cast<PartyZoneType>(info->kind_of_room);
    switch(party_room_type)
    {
    case PartyOfNone:
        return eZONETYPE_FIELD;
    case PartyOfMission:
#ifdef _NA_003649_20111115_BATTLE_ZONE_HARDMODE
    case PartyOfHardcore:
#endif
#ifdef _NA_007156_20140703_TOP_OF_TRIAL_LIGHT
    case PartyOfTopOfTrialLight_cash:
    case PartyOfTopOfTrialLight_normal:
#endif //_NA_007156_20140703_TOP_OF_TRIAL_LIGHT
#ifdef _NA_007734_20141027_ADD_ROOMKIND_SPECIALMISSION
    case PartyOfSpecialMission:
#endif //_NA_007734_20141027_ADD_ROOMKIND_SPECIALMISSION
        return eZONETYPE_MISSION;

    case PartyOfTopOfTrialByCash:
    case PartyOfTopOfTrialByNormal:
    case PartyOfEvent:
        return eZONETYPE_INSTANCE;

    case PartyOfPvPByPersonal:
    case PartyOfPvPByTeam:
        return eZONETYPE_PVP;
    }

    return eZONETYPE_MAX;
}

PartyZoneType
BattlezoneMissionTreeParser::GetType(MAPCODE map_code)const
{
    const BattlezoneMissionTreeInfo* info = FindData(map_code);
    if (info == NULL)
    {
        return PartyOfMax;
    }

    return static_cast<PartyZoneType>(info->kind_of_room);
}

bool 
BattlezoneMissionTreeParser::IsMissionItem(MAPCODE map_code, 
                                           PartyZoneType party_zone_type, 
                                           SLOTCODE require_item)const
{
    const BattlezoneMissionTreeInfo* info = FindData(map_code, party_zone_type);
    if (info == NULL) 
    {
        SUNLOG(eCRITICAL_LOG, 
            __FUNCTION__" : Not found script data : map code : %d, party type : %d",
            map_code, party_zone_type);
        return false;
    }

    for (int index = 0; index < MAX_REQUIRE_ITEM; ++index)
    {
        if (require_item == info->require_item[index])
        {
            return true;
        }
    }

    return false;
}

bool 
BattlezoneMissionTreeParser::IsMissionOfRequireItem(MAPCODE map_code, 
                                                    PartyZoneType party_zone_type)const
{
    const BattlezoneMissionTreeInfo* info = FindData(map_code, party_zone_type);
    if (info == NULL) 
    {
        SUNLOG(eCRITICAL_LOG, 
            __FUNCTION__" : Not found script data : map code : %d, party type : %d",
            map_code, party_zone_type);
        return false;
    }

    for (int index = 0; index < MAX_REQUIRE_ITEM; ++index)
    {
        if (info->require_item[index] != 0)
        {
            return true;
        }
    }

    return false;
}

#ifdef _NA_004204_20120131_BATTLE_ZONE_PARTY_OBTAIN
ObtainType 
BattlezoneMissionTreeParser::GetObtainType(MAPCODE map_code, PartyZoneType party_zone_type) const
{
    const BattlezoneMissionTreeInfo* info = FindData(map_code, party_zone_type);
    if (info == NULL)
    {
        return kObtainType_Disable;
    }

    return static_cast<ObtainType>(info->item_obtain_);
}
#endif //_NA_004204_20120131_BATTLE_ZONE_PARTY_OBTAIN

