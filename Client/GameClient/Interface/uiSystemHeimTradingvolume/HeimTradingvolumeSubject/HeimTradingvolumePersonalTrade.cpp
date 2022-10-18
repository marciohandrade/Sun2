#include "SunClientPrecompiledHeader.h"
#ifdef _NA_008678_20160203_ADD_HEIM_TRADINGVOLUME_NOTIFICATION_OPTION
#include "HeimTradingvolumePersonalTrade.h"
#include "uiTradeMan/uiTradeMan.h"

HeimTradingvolumePersonalTrade::HeimTradingvolumePersonalTrade(uiTradeMan* ui_trade_man, const MONEY& money) : 
ui_trade_man_(ui_trade_man), 
money_(money) 
{
}

HeimTradingvolumePersonalTrade::~HeimTradingvolumePersonalTrade(void)
{
}

void HeimTradingvolumePersonalTrade::Execute()
{
	if (ui_trade_man_->GetTradeInfo1().m_status == TRADE_STATUS_ACCEPT)
	{
		return;
	}

	if (ui_trade_man_->GetTradeInfo1().m_status != TRADE_STATUS_IDLE)
	{
		ui_trade_man_->TEST_Modify_Mode(true, TRADE_MOD_PENDING_PUTMONEY, money_);
		ui_trade_man_->TEST_Modify();
	}
	else
	{
		ui_trade_man_->SEND_PutMoney(money_);
	}
}

MONEY HeimTradingvolumePersonalTrade::GetTradingvolume()
{
	return money_;
}

#endif // _NA_008678_20160203_ADD_HEIM_TRADINGVOLUME_NOTIFICATION_OPTION
