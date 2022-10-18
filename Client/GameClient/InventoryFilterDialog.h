
#ifndef _INVENTORY_FILTER_DIALOG_H
#define _INVENTORY_FILTER_DIALOG_H

#pragma once

#include "uiBase.h"

class uiInventoryMan;
class InventoryFilterDialog : public uiBase
{	
	enum eINVENTORY_FILTER_POSITION
	{
        FILTER_ALL_SLOT1,
        FILTER_EQUIPMENT_SLOT1,
        FILTER_QUEST_SLOT1,
        FILTER_AC_SLOT1,
        FILTER_RESOURCE_SLOT1,
        FILTER_NORMAL_SLOT1,
        FILTER_WASTE_SLOT1,

        FILTER_ALL_SLOT2,
        FILTER_EQUIPMENT_SLOT2,
        FILTER_QUEST_SLOT2,
        FILTER_AC_SLOT2,
        FILTER_RESOURCE_SLOT2,
        FILTER_NORMAL_SLOT2,
        FILTER_WASTE_SLOT2,

        FILTER_ALL_SLOT3,
        FILTER_EQUIPMENT_SLOT3,
        FILTER_QUEST_SLOT3,
        FILTER_AC_SLOT3,
        FILTER_RESOURCE_SLOT3,
        FILTER_NORMAL_SLOT3,
        FILTER_WASTE_SLOT3,

        FILTER_ALL_SLOT4,
        FILTER_EQUIPMENT_SLOT4,
        FILTER_QUEST_SLOT4,
        FILTER_AC_SLOT4,
        FILTER_RESOURCE_SLOT4,
        FILTER_NORMAL_SLOT4,
        FILTER_WASTE_SLOT4,

        FILTER_ALL_SLOT5,
        FILTER_EQUIPMENT_SLOT5,
        FILTER_QUEST_SLOT5,
        FILTER_AC_SLOT5,
        FILTER_RESOURCE_SLOT5,
        FILTER_NORMAL_SLOT5,
        FILTER_WASTE_SLOT5,

        FILTER_ALL_SLOT6,
        FILTER_EQUIPMENT_SLOT6,
        FILTER_QUEST_SLOT6,
        FILTER_AC_SLOT6,
        FILTER_RESOURCE_SLOT6,
        FILTER_NORMAL_SLOT6,
        FILTER_WASTE_SLOT6,

        FILTER_ALL_SLOT7,
        FILTER_EQUIPMENT_SLOT7,
        FILTER_QUEST_SLOT7,
        FILTER_AC_SLOT7,
        FILTER_RESOURCE_SLOT7,
        FILTER_NORMAL_SLOT7,
        FILTER_WASTE_SLOT7,

        FILTER_ALL_SLOT8,
        FILTER_EQUIPMENT_SLOT8,
        FILTER_QUEST_SLOT8,
        FILTER_AC_SLOT8,
        FILTER_RESOURCE_SLOT8,
        FILTER_NORMAL_SLOT8,
        FILTER_WASTE_SLOT8,

        FILTER_ALL_SLOT9,
        FILTER_EQUIPMENT_SLOT9,
        FILTER_QUEST_SLOT9,
        FILTER_AC_SLOT9,
        FILTER_RESOURCE_SLOT9,
        FILTER_NORMAL_SLOT9,
        FILTER_WASTE_SLOT9,

        FILTER_ALL_SLOT10,
        FILTER_EQUIPMENT_SLOT10,
        FILTER_QUEST_SLOT10,
        FILTER_AC_SLOT10,
        FILTER_RESOURCE_SLOT10,
        FILTER_NORMAL_SLOT10,
        FILTER_WASTE_SLOT10,
		FILTER_OK,
        FILTER_EXIT,
        INVENTORY_FILTERS_MAX,
	};

    enum
    {
        SLOT_COUNT_BASE = 5,
        SLOT_COUNT_MAX = 10,
        FILTERS_MAX = FILTER_WASTE_SLOT10+1,
    };

    enum
    {
        CHECK_FILTER_ALL,
        CHECK_FILTER_EQUIPMENT,
        CHECK_FILTER_QUEST,
        CHECK_FILTER_AC,
        CHECK_FILTER_RESOURCE,
        CHECK_FILTER_NORMAL,
        CHECK_FILTER_WASTE,
        CHECK_FILTER_MAX,
    };
public:	
	InventoryFilterDialog(InterfaceManager* manager_ptr);
	virtual ~InventoryFilterDialog(void);
	virtual void Init(CDrawBase* draw_base_ptr);
	virtual void Release();
	virtual void OnUpdateSolarDialog();
	virtual void MessageProc(SI_MESSAGE* message_ptr);
	virtual void OnShowWindow(BOOL val);
    virtual void Process(DWORD tick_count);
    void InitControls();
    uiInventoryMan*	GetManager();
    void SetItemInventoryConfig(nsSlot::InventoryConfig inventory_config_packet);
private:
	CControlWZ* get_control(POSTYPE AtPos);
	POSTYPE	get_control_id_to_position(WzID controls_id);
    void SetAllCheckAboutOneSlot(int slot_number, bool value);
    bool GetAllCheckAboutOneSlot(int slot_number, bool all_filter_check);
    void SendItemInventoryConfigChangeSyn();
    bool CheckItemInventoryConfigChange();
    void DisableExtendFilterSlot();
	void RollbackItemInventoryConfig();
private:
    static WzID controls_id_[INVENTORY_FILTERS_MAX];
    CCtrlButtonCheckWZ* slot_check_buttons_[SLOT_COUNT_MAX][CHECK_FILTER_MAX];
    int charge_count_;
    nsSlot::InventoryConfig inventory_config_;
};
//------------------------------------------------------------------------------
/**
*/
inline POSTYPE InventoryFilterDialog::get_control_id_to_position(WzID controls_id)
{
	for (POSTYPE i = 0; i < INVENTORY_FILTERS_MAX; ++i )
	{
		if (controls_id_[i] == controls_id)
		{
			return i;
		}
	}
	return INVENTORY_FILTERS_MAX;
}

//------------------------------------------------------------------------------
/**
*/
inline CControlWZ* InventoryFilterDialog::get_control(POSTYPE AtPos)
{
    if (AtPos >= INVENTORY_FILTERS_MAX)
    {
        return NULL;
    }
	return GetControlWZ(controls_id_[AtPos]);
}

#endif//_INVENTORY_FILTER_DIALOG_H
