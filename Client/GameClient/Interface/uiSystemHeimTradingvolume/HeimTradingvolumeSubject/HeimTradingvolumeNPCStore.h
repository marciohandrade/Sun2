#pragma once
#ifdef _NA_008678_20160203_ADD_HEIM_TRADINGVOLUME_NOTIFICATION_OPTION
#include "HeimTradingvolumeSubject.h"

class HeimTradingvolumeNPCStore : public HeimTradingvolumeSubject
{
public:
	HeimTradingvolumeNPCStore(const NUMTYPE& divide_num, 
		                      const SLOTIDX& from_slot_idx, 
							  const POSTYPE& from_postype, 
							  const eSlotType& slot_type, 
							  const SLOTCODE& slot_code, 
							  const MONEY& price);
	virtual ~HeimTradingvolumeNPCStore(void);

public:
	virtual void Execute();

public:
	virtual MONEY GetTradingvolume();

private:
	NUMTYPE   divide_num_;
	SLOTIDX   from_slot_idx_;
	POSTYPE   from_postype_;
	eSlotType slot_type_;
	SLOTCODE  slot_code_;
	MONEY     price_;
};
#endif // _NA_008678_20160203_ADD_HEIM_TRADINGVOLUME_NOTIFICATION_OPTION
