#pragma once
#ifdef _NA_008678_20160203_ADD_HEIM_TRADINGVOLUME_NOTIFICATION_OPTION
#include "HeimTradingvolumeSubject.h"

class uiTradeMan;
class HeimTradingvolumePersonalTrade : public HeimTradingvolumeSubject
{
public:
	HeimTradingvolumePersonalTrade(uiTradeMan* ui_trade_man, const MONEY& money);
	~HeimTradingvolumePersonalTrade(void);

public:
	virtual void Execute();

public:
	virtual MONEY GetTradingvolume();

private:
	uiTradeMan* ui_trade_man_;
	MONEY money_;
};
#endif // _NA_008678_20160203_ADD_HEIM_TRADINGVOLUME_NOTIFICATION_OPTION
