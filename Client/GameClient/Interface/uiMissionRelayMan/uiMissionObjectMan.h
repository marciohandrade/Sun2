#pragma once

#ifdef _DH_DISPLAY_FOR_MISSION_OBJECT
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
	~uiMissionObjectMan(void);

public:
	virtual void            Init(CDrawBase* draw_base);
	virtual void            Release();
	virtual void            LoadUI(UINT ui_id);
	void                    Clear();
private:
	uiMissionObject* ui_mission_object_;
};
#endif//_DH_DISPLAY_FOR_MISSION_OBJECT