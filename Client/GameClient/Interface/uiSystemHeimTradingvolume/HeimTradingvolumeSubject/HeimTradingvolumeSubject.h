#pragma once
#ifdef _NA_008678_20160203_ADD_HEIM_TRADINGVOLUME_NOTIFICATION_OPTION

class HeimTradingvolumeSubject
{
public:
	HeimTradingvolumeSubject(void);
	virtual ~HeimTradingvolumeSubject(void);

public:
	virtual void Execute() = 0;
	virtual void Cancel() {}
	virtual void Release() {}

public:
	virtual MONEY GetTradingvolume() = 0;
};
#endif // _NA_008678_20160203_ADD_HEIM_TRADINGVOLUME_NOTIFICATION_OPTION
