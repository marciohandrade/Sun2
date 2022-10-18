#pragma once
#ifdef _NA_008678_20160203_ADD_HEIM_TRADINGVOLUME_NOTIFICATION_OPTION
#include "HeimTradingvolumeSubject.h"

class HeimTradingvolumeVendorBuy : public HeimTradingvolumeSubject
{
public:
	HeimTradingvolumeVendorBuy(const POSTYPE& vendor_pos, const MONEY& money);
	~HeimTradingvolumeVendorBuy(void);

public:
	virtual void Execute();

public:
	virtual MONEY GetTradingvolume();

private:
	POSTYPE vendor_pos_;
	MONEY   money_;
};
#endif // _NA_008678_20160203_ADD_HEIM_TRADINGVOLUME_NOTIFICATION_OPTION
