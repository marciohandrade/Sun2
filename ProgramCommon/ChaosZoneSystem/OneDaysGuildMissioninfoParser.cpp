#include "StdAfx.h"

#ifdef _NA_008543_20151027_ONE_DAYS_GUILD_MISSION
#include ".\OneDaysGuildMissionInfoParser.h"

OneDaysGuildMissionInfoParser::OneDaysGuildMissionInfoParser(void)
{

}

OneDaysGuildMissionInfoParser::~OneDaysGuildMissionInfoParser(void)
{

}

VOID OneDaysGuildMissionInfoParser::Init()
{

}

VOID OneDaysGuildMissionInfoParser::Init(char* pszPackFileName)
{
	SetPackFileName(pszPackFileName);
}

void OneDaysGuildMissionInfoParser::Release()
{
	Unload();
}

BOOL OneDaysGuildMissionInfoParser::LoadScript(eSCRIPT_CODE scriptCode, BOOL bReload)
{
	switch (scriptCode)
	{
	case SCRIPT_ONEDAYSBUILDMISSION:
		return _Load(bReload);
	}

	return FALSE;
}

BOOL OneDaysGuildMissionInfoParser::_Load(BOOL bReload)
{
	if (bReload == TRUE)
	{
		OneDaysGuildMissionInfo_Hash().clear();
	}

	int nRowSize = GetRowSize();
	for(int nRow = 0; nRow < nRowSize; ++nRow)
	{
		OneDaysGuildMissionInfo* info = new OneDaysGuildMissionInfo();

		info->index = GetDataBYTE("Index", nRow);
		info->map_code = GetDataDWORD("MapCode", nRow);
		info->play_time = GetDataDWORD("Playtime", nRow);
		info->start_wait_time = GetDataDWORD("StartWaitTime", nRow);
		info->end_wait_time = GetDataDWORD("EndWaitTime", nRow);
		GetDataWZID(nRow, info->start_trigger_id, "StartTriggerID");
		info->base_reward_item_code = GetDataDWORD("BaseItemCode", nRow);

		OneDaysGuildMissionInfo_Hash::_Pairib result = oneoaysguildmissionInfo_hash().insert(
			OneDaysGuildMissionInfo_Hash::value_type(static_cast<DWORD>(info->map_code), info));
		ASSERT(result.second == true);
	}

	return TRUE;
}

void OneDaysGuildMissionInfoParser::Unload()
{
	OneDaysGuildMissionInfo_Hash::const_iterator it, end( OneDaysGuildMissionInfo_Hash_.end() );
	for(it = OneDaysGuildMissionInfo_Hash_.begin() ; it != end ; ++it)
	{
		OneDaysGuildMissionInfo* info = (it->second);
		SAFE_DELETE(info);
	}

	OneDaysGuildMissionInfo_Hash_.clear();
}

const OneDaysGuildMissionInfo* OneDaysGuildMissionInfoParser::FindData(const MAPCODE map_code) const
{
	OneDaysGuildMissionInfo_Hash::const_iterator begin_iterator = OneDaysGuildMissionInfo_Hash_.begin(),
		end_iterator = OneDaysGuildMissionInfo_Hash_.end();

	for (; begin_iterator != end_iterator; ++begin_iterator)
	{
		const OneDaysGuildMissionInfo* find_info = begin_iterator->second;
		if (find_info->map_code == map_code)
		{
			return find_info;
		}
	}
	return NULL;
}

#endif //(_NA_008543_20151027_ONE_DAYS_GUILD_MISSION)