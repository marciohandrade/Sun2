#pragma once

#include "EtcStatus.h"

class BattleStatus : public EtcStatus
{
public:
	BattleStatus(void);
	~BattleStatus(void);
#ifdef _NA_006633_20130415_ADD_BATTLEGROUND_ASSIST
    void End();
#endif //_NA_006633_20130415_ADD_BATTLEGROUND_ASSIST
};

