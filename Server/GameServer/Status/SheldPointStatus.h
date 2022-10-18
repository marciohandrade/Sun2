#pragma once

#include "EtcStatus.h"

class SheldPointStatus : public EtcStatus
{
public:
	SheldPointStatus() {}
	~SheldPointStatus() {}

	virtual VOID		Start();
	virtual VOID		End();
    virtual BOOL		CanRemove();
    //__NA_001290_20090525_SHIELD_SYSTEM_NOTIFY_NOTABILITY_STATE
    virtual BOOL        IsNotifyStatus()  { return TRUE; }
};

