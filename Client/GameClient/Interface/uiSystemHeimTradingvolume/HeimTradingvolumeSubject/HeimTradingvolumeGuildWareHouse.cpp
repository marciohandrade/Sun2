#include "SunClientPrecompiledHeader.h"
#ifdef _NA_008678_20160203_ADD_HEIM_TRADINGVOLUME_NOTIFICATION_OPTION
#include "HeimTradingvolumeGuildWareHouse.h"
#include "GuildSystem/uiGuildWareHouse.h"

HeimTradingvolumeGuildWareHouse::HeimTradingvolumeGuildWareHouse( uiGuildWareHouse* ui_guild_warehouse, const MONEY& money ) : 
ui_guild_warehouse_(ui_guild_warehouse), 
money_(money)
{

}

HeimTradingvolumeGuildWareHouse::~HeimTradingvolumeGuildWareHouse(void)
{
}

void HeimTradingvolumeGuildWareHouse::Execute()
{
	ui_guild_warehouse_->SendWareHouseMoney(MSG_CG_GUILD_WAREHOUSE_MONEY_SYN::PUTMONEY);
}

void HeimTradingvolumeGuildWareHouse::Cancel()
{
	ui_guild_warehouse_->HeimTradingvolumeCancel();
}

MONEY HeimTradingvolumeGuildWareHouse::GetTradingvolume()
{
	return money_;
}

#endif // _NA_008678_20160203_ADD_HEIM_TRADINGVOLUME_NOTIFICATION_OPTION
