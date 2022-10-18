#pragma once

#ifdef _NA_008121_20150318_EQUIPMENT_EVOLUTION_SYSTEM
#include "uiBaseMan.h"

struct IM_EQUIPMENT_EVOLUTION_MANAGER
{
	static const DWORD EQUIPMENT_EVOLUTION_DIALOG;
};

class uiEquipmentEvolutionDialog;
class uiEquipmentEvolutionMan : public uiBaseMan
{
public:
	uiEquipmentEvolutionMan(InterfaceManager* ui_manager);
	~uiEquipmentEvolutionMan(void);

public:
	SolarDialog* ShowEquipmentEvolutionDialog();

public:
	virtual void NetworkProc( MSG_BASE* message );

private:
	void         Recv_CG_ITEM_EVOLUTION_ACK(MSG_BASE* message);

public:
	virtual void OnInitialize();

private:
	uiEquipmentEvolutionDialog* ui_equipment_evolution_dialog_;
};

#endif // _NA_008121_20150318_EQUIPMENT_EVOLUTION_SYSTEM
