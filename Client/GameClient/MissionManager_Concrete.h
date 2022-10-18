#pragma once
#include <missionmanager.h>

class MissionManager_Concrete : public MissionManager
{
public:
	MissionManager_Concrete(void);
	virtual ~MissionManager_Concrete(void);
};

extern MissionManager_Concrete * g_pMission_Concrete;
