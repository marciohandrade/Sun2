#pragma once

#ifdef _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM
#include "uiBaseMan.h"

struct IM_EQUIPMENT_AWAKENING_MANAGER
{
	static const DWORD EQUIPMENT_AWAKENING_DIALOG;
};

class uiEquipmentAwakeningDialog;
class uiEquipmentWakeupMan : public uiBaseMan
{
public:
	uiEquipmentWakeupMan(InterfaceManager* ui_manager);
	~uiEquipmentWakeupMan(void);

public:
	SolarDialog* ShowEquipmentAwakeningDialog();

public:
	virtual void NetworkProc( MSG_BASE* message );

private:
	void         Recv_CG_ITEM_AWAKENING_ACK(MSG_BASE* message);

public:
	virtual void OnInitialize();

private:
	uiEquipmentAwakeningDialog* ui_equipment_awakening_dialog_;
};

#endif // _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM
