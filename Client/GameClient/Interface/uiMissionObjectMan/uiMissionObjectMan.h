#pragma once

#include "uiBaseMan.h"

struct IM_MISSION_OBJECT_MANAGER
{
    static const DWORD MISSION_OBJECT;
};

class uiMissionObject;
class uiMissionObjectMan: public uiBaseMan
{
public:
	uiMissionObjectMan(InterfaceManager* ui_manager);

public:
	virtual void            OnInitialize();

private:
	uiMissionObject* ui_mission_object_;
};
