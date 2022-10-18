#include "StdAfx.h"

#ifdef _NA_1419_20100121_ACCUMULATE_POINT

#include ".\AccumulatePointParser.h"


AccumulatePointParser::AccumulatePointParser(void)
{
}

AccumulatePointParser::~AccumulatePointParser(void)
{
}

VOID AccumulatePointParser::Init()
{
}

VOID AccumulatePointParser::Init( char * pszPackFileName )
{
	SetPackFileName( pszPackFileName );
}

VOID AccumulatePointParser::Release()
{
}

BOOL AccumulatePointParser::_Load( BOOL bReload )
{
    __UNUSED(bReload);
    TCHAR field_name[64] = {0,};
    int row_count = min(AccumulatePointParser::Max_Day, GetRowSize());

    for (int row = 0 ; row < row_count ; ++row)
    {
        RewardGroup& reward_group = reward_group_array_[row];

        reward_group.day_flag = GetDataBYTE("DayFlag", row);
        reward_group.require_point = GetDataBYTE("RequestCoin", row);

        for (int col = 0 ; col < RewardGroup::Max_Reward ; ++col)
        {
            _sntprintf(field_name, _countof(field_name), "RewardGroup%d", col+1);
            reward_group.reward_array[col].reward_code = GetDataDWORD(field_name, row);

            _sntprintf(field_name, _countof(field_name), "RewardGroupRate%d", col+1);
            reward_group.reward_array[col].reward_ratio = GetDataBYTE(field_name, row);
        }

#ifdef _NA_0_20100528_ACCUMULATE_POINT_EVENT
        reward_group.reward_event.start_time = GetDataDWORD("EventStartTime", row);
        reward_group.reward_event.end_time = GetDataDWORD("EventEndTime", row);
        reward_group.reward_event.reward_code = GetDataDWORD("EventReward", row);
#endif
        reward_group.generate_time = GetDataDWORD("MileageTimeSet", row);
        reward_group.generate_point = GetDataBYTE("MileageCoinSet", row);

#ifdef _NA_0_20101011_ACCUMULATE_POINT_LIMIT_OPTION
        reward_group.limit_option.receptible_min_level = GetDataBYTE("ReceptibleMinLevel", row);
        reward_group.limit_option.receptible_max_level = GetDataBYTE("ReceptibleMaxLevel", row);
        reward_group.limit_option.limit_max_point = GetDataDWORD("LimitCoin", row);
#endif
    }

    return TRUE;
}

BOOL AccumulatePointParser::LoadScript( eSCRIPT_CODE scriptCode, BOOL bReload )
{
	switch (scriptCode)
	{
	case SCRIPT_ACCUMULATE_POINT:
		return _Load( bReload );
	}

    return FALSE;	
}

#endif //_NA_1419_20100121_ACCUMULATE_POINT
