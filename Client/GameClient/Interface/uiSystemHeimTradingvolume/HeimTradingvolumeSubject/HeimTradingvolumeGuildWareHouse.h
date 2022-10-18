#pragma once
#ifdef _NA_008678_20160203_ADD_HEIM_TRADINGVOLUME_NOTIFICATION_OPTION
#include "HeimTradingvolumeSubject.h"

class uiGuildWareHouse;
class HeimTradingvolumeGuildWareHouse : public HeimTradingvolumeSubject
{
public:
	HeimTradingvolumeGuildWareHouse(uiGuildWareHouse* ui_guild_warehouse, 
		                            const MONEY&      money);
	~HeimTradingvolumeGuildWareHouse(void);

public:
	virtual void Execute();
	virtual void Cancel();

public:
	virtual MONEY GetTradingvolume();

private:
	uiGuildWareHouse* ui_guild_warehouse_;
	MONEY             money_;
};
#endif // _NA_008678_20160203_ADD_HEIM_TRADINGVOLUME_NOTIFICATION_OPTION
