#include "StdAfx.h"

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#include ".\ChaosZoneInfoParser.h"

//------------------------------------------------------------------------------ 
ChaosZoneInfoParser::ChaosZoneInfoParser(void)
{
}
//------------------------------------------------------------------------------ 
ChaosZoneInfoParser::~ChaosZoneInfoParser(void)
{
}
VOID ChaosZoneInfoParser::Init()
{
}

VOID ChaosZoneInfoParser::Init(char * pszPackFileName)
{
    SetPackFileName(pszPackFileName);
}
//------------------------------------------------------------------------------ 
void ChaosZoneInfoParser::Release()
{
    Unload();
}

void ChaosZoneInfoParser::Unload()
{
    ChaosZoneInfo_Hash::const_iterator it, end( chaoszone_info_hash_.end() );
    for(it = chaoszone_info_hash_.begin() ; it != end ; ++it)
    {
        ChaosZoneInfo* info = (it->second);
        SAFE_DELETE(info);
    }

    chaoszone_info_hash_.clear();
}

BOOL ChaosZoneInfoParser::_Load(BOOL bReload)
{
    if (bReload == TRUE)
    {
        chaoszone_info_hash().clear();
    }

    int nRowSize = GetRowSize();
    for (int nRow = 0; nRow < nRowSize; ++nRow)
    {
        ChaosZoneInfo* info = new ChaosZoneInfo();

        info->map_code = GetDataDWORD("MapCode", nRow);
        info->kind_of_room = GetDataBYTE("Kind", nRow);
        strncpy(info->mission_name, GetDataString("MName", nRow), MAX_ITEMNAME_LENGTH);
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
        
        info->item_obtain_ = GetDataBYTE("ItemObtain", nRow);

        ChaosZoneInfo_Hash::_Pairib result = chaoszone_info_hash().insert(
            ChaosZoneInfo_Hash::value_type(static_cast<DWORD>(info->map_code), info));
        ASSERT(result.second == true);
    }
    return TRUE;
}
//------------------------------------------------------------------------------ 
BOOL ChaosZoneInfoParser::LoadScript(eSCRIPT_CODE scriptCode, BOOL bReload)
{
    switch (scriptCode)
    {
    case SCRIPT_CHAOSZONE_INFO:
        return _Load(bReload);
    }

    return FALSE;
}
//------------------------------------------------------------------------------ 
const ChaosZoneInfo* ChaosZoneInfoParser::FindData(MAPCODE map_code)const
{
    ChaosZoneInfo_Hash::const_iterator found = chaoszone_info_hash_.find(map_code);
    if (found == chaoszone_info_hash_.end()) 
    {
        return NULL;
    }
    const ChaosZoneInfo* info = found->second;
    return info;
}

const ChaosZoneInfo* ChaosZoneInfoParser::FindData(MAPCODE map_code, ChaosZoneType zone_type)const
{
    ChaosZoneInfo_Hash::const_iterator found = chaoszone_info_hash_.find(map_code);
    if (found == chaoszone_info_hash_.end()) 
    {
        return NULL;
    }

    BYTE compair_zont_type = static_cast<BYTE>(zone_type);

    const ChaosZoneInfo* info = found->second;
    if (info->kind_of_room == zone_type)
    {
        return info;
    }

    return NULL;
}

eZONETYPE ChaosZoneInfoParser::GetZoneType(MAPCODE map_code)const
{
    const ChaosZoneInfo* info = FindData(map_code);
    if (info == NULL)
    {
        return eZONETYPE_MAX;
    }

    ChaosZoneType zone_type = static_cast<ChaosZoneType>(info->kind_of_room);
    if (zone_type > kChaosZoneTypeNone &&
        zone_type <eCHAOSZONE_TYPE_MAX)
    {
        return eZONETYPE_INSTANCE;
    }
}

ChaosZoneType ChaosZoneInfoParser::GetType(MAPCODE map_code)const
{
    const ChaosZoneInfo* info = FindData(map_code);
    if (info == NULL)
    {
        return kChaosZoneTypeNone;
    }

    return static_cast<ChaosZoneType>(info->kind_of_room);
}

bool ChaosZoneInfoParser::IsMissionItem(MAPCODE map_code, 
                                           ChaosZoneType zone_type,
                                           SLOTCODE require_item)const
{
    const ChaosZoneInfo* info = FindData(map_code, zone_type);
    if (info == NULL) 
    {
        SUNLOG(eCRITICAL_LOG, 
            __FUNCTION__" : Not found script data : map code : %d, party type : %d",
            map_code, zone_type);
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

bool ChaosZoneInfoParser::IsMissionOfRequireItem(MAPCODE map_code, 
                                                    ChaosZoneType zone_type)const
{
    const ChaosZoneInfo* info = FindData(map_code, zone_type);
    if (info == NULL) 
    {
        SUNLOG(eCRITICAL_LOG, 
            __FUNCTION__" : Not found script data : map code : %d, party type : %d",
            map_code, zone_type);
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

ObtainType ChaosZoneInfoParser::GetObtainType(MAPCODE map_code, ChaosZoneType zone_type) const
{
    const ChaosZoneInfo* info = FindData(map_code, zone_type);
    if (info == NULL)
    {
        return kObtainType_Disable;
    }

    return static_cast<ObtainType>(info->item_obtain_);
}

#ifdef _NA_008334_20150608_SONNENSCHEIN
MAPCODE ChaosZoneInfoParser::GetSonnenScheinMapCode()
{
    ChaosZoneInfo_Hash::const_iterator begin_iterator = chaoszone_info_hash_.begin(),
        end_iterator = chaoszone_info_hash_.end();


    for (; begin_iterator != end_iterator; ++begin_iterator)
    {
        const ChaosZoneInfo* find_info = begin_iterator->second;
        if (find_info->kind_of_room == eCHAOSZONE_TYPE_SONNENSCHEIN)
        {
            return find_info->map_code;
        }
    }
    return 0;
}
#endif //_NA_008334_20150608_SONNENSCHEIN


#ifdef _NA_008543_20151027_ONE_DAYS_GUILD_MISSION
MAPCODE ChaosZoneInfoParser::GetOneDaysGuildmissionMapCode()
{
	ChaosZoneInfo_Hash::const_iterator begin_iterator = chaoszone_info_hash_.begin(),
		end_iterator = chaoszone_info_hash_.end();


	for (; begin_iterator != end_iterator; ++begin_iterator)
	{
		const ChaosZoneInfo* find_info = begin_iterator->second;
		if (find_info->kind_of_room == eCHAOSZONE_TYPE_ONEDAYSGUILDMISSION)
		{
			return find_info->map_code;
		}
	}
	return 0;
}
#endif //(_NA_008543_20151027_ONE_DAYS_GUILD_MISSION)

#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND