#pragma once

#include "EtcStatus.h"


class SteelStatus : public EtcStatus
{
public:
	SteelStatus() {}
	~SteelStatus() {}

	virtual VOID		Start();
	virtual VOID		End();
    //__NA_001290_20090525_SHIELD_SYSTEM_NOTIFY_NOTABILITY_STATE
    virtual BOOL        IsNotifyStatus()  { return TRUE; }
};

