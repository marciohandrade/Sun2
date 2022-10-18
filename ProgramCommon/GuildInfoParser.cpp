#include "StdAfx.h"
#include "GuildInfoParser.h"
void 
GuildInfoParser::Release()
{

}

BOOL 
GuildInfoParser::LoadScript(eSCRIPT_CODE script_code, BOOL is_reload)
{
    __UNUSED((script_code, is_reload));

    int row_size = GetRowSize();
    for (int script_row = 0; script_row < row_size; ++script_row)
    {
        guild_data_.guild_create_level_ = GetDataBYTE("NeedLevel", script_row);
        guild_data_.guild_create_heim_ = GetDataDWORD64("NeedHeim", script_row);
        guild_data_.max_alliance_ = GetDataBYTE("MaxAlliance", script_row);
        guild_data_.max_hostile_ = GetDataBYTE("MaxHostile", script_row);
        guild_data_.max_apply_hostile_ = GetDataBYTE("ApplyHostile", script_row);
        guild_data_.time_of_hostile_latency_ = GetDataDWORD("HostileLatency", script_row);
        guild_data_.time_of_hostile_ = GetDataDWORD("HostileHour", script_row);
        guild_data_.hostile_item_code_ = GetDataDWORD("HostileItem", script_row);
        guild_data_.time_of_disbanded_latency_ = GetDataDWORD("DisbandedLatency", script_row);
        guild_data_.time_of_expulsion_penalty_ = GetDataDWORD("ExpulsionPenalty", script_row);
        guild_data_.time_of_withdrawal_penalty_ = GetDataDWORD("WithdrawalPenalty", script_row);
        guild_data_.max_guild_coin_donation_ = GetDataDWORD("CoinDonation", script_row);
    }

    return TRUE;
}

