
#ifndef _EXTEND_INVENTORY_DIALOG_H
#define _EXTEND_INVENTORY_DIALOG_H

#pragma once

#include "uiBase.h"

class uiInventoryMan;
class InventoryDialog;
class ExtendInventoryDialog : public uiBase
{	
	enum eINVEN_POS
	{
		EXTEND_INVEN_SLOT_1_00,
        EXTEND_INVEN_SLOT_1_01,
        EXTEND_INVEN_SLOT_1_02,
        EXTEND_INVEN_SLOT_1_03,
        EXTEND_INVEN_SLOT_1_04,
        EXTEND_INVEN_SLOT_1_05,
        EXTEND_INVEN_SLOT_1_06,
        EXTEND_INVEN_SLOT_1_07,
        EXTEND_INVEN_SLOT_1_08,
        EXTEND_INVEN_SLOT_1_09,
        EXTEND_INVEN_SLOT_1_10,
        EXTEND_INVEN_SLOT_1_11,
        EXTEND_INVEN_SLOT_1_12,
        EXTEND_INVEN_SLOT_1_13,
        EXTEND_INVEN_SLOT_1_14,

        EXTEND_INVEN_SLOT_2_00,
        EXTEND_INVEN_SLOT_2_01,
        EXTEND_INVEN_SLOT_2_02,
        EXTEND_INVEN_SLOT_2_03,
        EXTEND_INVEN_SLOT_2_04,
        EXTEND_INVEN_SLOT_2_05,
        EXTEND_INVEN_SLOT_2_06,
        EXTEND_INVEN_SLOT_2_07,
        EXTEND_INVEN_SLOT_2_08,
        EXTEND_INVEN_SLOT_2_09,
        EXTEND_INVEN_SLOT_2_10,
        EXTEND_INVEN_SLOT_2_11,
        EXTEND_INVEN_SLOT_2_12,
        EXTEND_INVEN_SLOT_2_13,
        EXTEND_INVEN_SLOT_2_14,

        EXTEND_INVEN_SLOT_3_00,
        EXTEND_INVEN_SLOT_3_01,
        EXTEND_INVEN_SLOT_3_02,
        EXTEND_INVEN_SLOT_3_03,
        EXTEND_INVEN_SLOT_3_04,
        EXTEND_INVEN_SLOT_3_05,
        EXTEND_INVEN_SLOT_3_06,
        EXTEND_INVEN_SLOT_3_07,
        EXTEND_INVEN_SLOT_3_08,
        EXTEND_INVEN_SLOT_3_09,
        EXTEND_INVEN_SLOT_3_10,
        EXTEND_INVEN_SLOT_3_11,
        EXTEND_INVEN_SLOT_3_12,
        EXTEND_INVEN_SLOT_3_13,
        EXTEND_INVEN_SLOT_3_14,

        EXTEND_INVEN_SLOT_4_00,
        EXTEND_INVEN_SLOT_4_01,
        EXTEND_INVEN_SLOT_4_02,
        EXTEND_INVEN_SLOT_4_03,
        EXTEND_INVEN_SLOT_4_04,
        EXTEND_INVEN_SLOT_4_05,
        EXTEND_INVEN_SLOT_4_06,
        EXTEND_INVEN_SLOT_4_07,
        EXTEND_INVEN_SLOT_4_08,
        EXTEND_INVEN_SLOT_4_09,
        EXTEND_INVEN_SLOT_4_10,
        EXTEND_INVEN_SLOT_4_11,
        EXTEND_INVEN_SLOT_4_12,
        EXTEND_INVEN_SLOT_4_13,
        EXTEND_INVEN_SLOT_4_14,

        EXTEND_INVEN_SLOT_5_00,
        EXTEND_INVEN_SLOT_5_01,
        EXTEND_INVEN_SLOT_5_02,
        EXTEND_INVEN_SLOT_5_03,
        EXTEND_INVEN_SLOT_5_04,
        EXTEND_INVEN_SLOT_5_05,
        EXTEND_INVEN_SLOT_5_06,
        EXTEND_INVEN_SLOT_5_07,
        EXTEND_INVEN_SLOT_5_08,
        EXTEND_INVEN_SLOT_5_09,
        EXTEND_INVEN_SLOT_5_10,
        EXTEND_INVEN_SLOT_5_11,
        EXTEND_INVEN_SLOT_5_12,
        EXTEND_INVEN_SLOT_5_13,
        EXTEND_INVEN_SLOT_5_14,

        EXTEND_INVEN_SLOT_6_00,
        EXTEND_INVEN_SLOT_6_01,
        EXTEND_INVEN_SLOT_6_02,
        EXTEND_INVEN_SLOT_6_03,
        EXTEND_INVEN_SLOT_6_04,
        EXTEND_INVEN_SLOT_6_05,
        EXTEND_INVEN_SLOT_6_06,
        EXTEND_INVEN_SLOT_6_07,
        EXTEND_INVEN_SLOT_6_08,
        EXTEND_INVEN_SLOT_6_09,
        EXTEND_INVEN_SLOT_6_10,
        EXTEND_INVEN_SLOT_6_11,
        EXTEND_INVEN_SLOT_6_12,
        EXTEND_INVEN_SLOT_6_13,
        EXTEND_INVEN_SLOT_6_14,

        EXTEND_INVEN_SLOT_7_00,
        EXTEND_INVEN_SLOT_7_01,
        EXTEND_INVEN_SLOT_7_02,
        EXTEND_INVEN_SLOT_7_03,
        EXTEND_INVEN_SLOT_7_04,
        EXTEND_INVEN_SLOT_7_05,
        EXTEND_INVEN_SLOT_7_06,
        EXTEND_INVEN_SLOT_7_07,
        EXTEND_INVEN_SLOT_7_08,
        EXTEND_INVEN_SLOT_7_09,
        EXTEND_INVEN_SLOT_7_10,
        EXTEND_INVEN_SLOT_7_11,
        EXTEND_INVEN_SLOT_7_12,
        EXTEND_INVEN_SLOT_7_13,
        EXTEND_INVEN_SLOT_7_14,

        EXTEND_INVEN_SLOT_8_00,
        EXTEND_INVEN_SLOT_8_01,
        EXTEND_INVEN_SLOT_8_02,
        EXTEND_INVEN_SLOT_8_03,
        EXTEND_INVEN_SLOT_8_04,
        EXTEND_INVEN_SLOT_8_05,
        EXTEND_INVEN_SLOT_8_06,
        EXTEND_INVEN_SLOT_8_07,
        EXTEND_INVEN_SLOT_8_08,
        EXTEND_INVEN_SLOT_8_09,
        EXTEND_INVEN_SLOT_8_10,
        EXTEND_INVEN_SLOT_8_11,
        EXTEND_INVEN_SLOT_8_12,
        EXTEND_INVEN_SLOT_8_13,
        EXTEND_INVEN_SLOT_8_14,

        EXTEND_INVEN_SLOT_9_00,
        EXTEND_INVEN_SLOT_9_01,
        EXTEND_INVEN_SLOT_9_02,
        EXTEND_INVEN_SLOT_9_03,
        EXTEND_INVEN_SLOT_9_04,
        EXTEND_INVEN_SLOT_9_05,
        EXTEND_INVEN_SLOT_9_06,
        EXTEND_INVEN_SLOT_9_07,
        EXTEND_INVEN_SLOT_9_08,
        EXTEND_INVEN_SLOT_9_09,
        EXTEND_INVEN_SLOT_9_10,
        EXTEND_INVEN_SLOT_9_11,
        EXTEND_INVEN_SLOT_9_12,
        EXTEND_INVEN_SLOT_9_13,
        EXTEND_INVEN_SLOT_9_14,

        EXTEND_INVEN_SLOT_10_00,
        EXTEND_INVEN_SLOT_10_01,
        EXTEND_INVEN_SLOT_10_02,
        EXTEND_INVEN_SLOT_10_03,
        EXTEND_INVEN_SLOT_10_04,
        EXTEND_INVEN_SLOT_10_05,
        EXTEND_INVEN_SLOT_10_06,
        EXTEND_INVEN_SLOT_10_07,
        EXTEND_INVEN_SLOT_10_08,
        EXTEND_INVEN_SLOT_10_09,
        EXTEND_INVEN_SLOT_10_10,
        EXTEND_INVEN_SLOT_10_11,
        EXTEND_INVEN_SLOT_10_12,
        EXTEND_INVEN_SLOT_10_13,
        EXTEND_INVEN_SLOT_10_14,

        EXTEND_INVEN_TEXT_TITLE,
        EXTEND_INVEN_BUTTON_UNLOCK,
        EXTEND_INVEN_BUTTON_LOCK,
        EXTEND_INVEN_BUTTON_FILTER,
        EXTEND_INVEN_BUTTON_VIEW_OPTION,
        EXTEND_INVEN_BUTTON_WASTE,
        EXTEND_INVEN_BUTTON_WAY_POINT,
        EXTEND_INVEN_BUTTON_EVENT_INVEN_OFF,
        EXTEND_INVEN_BUTTON_EVENT_INVEN_ON,
        EXTEND_INVEN_BUTTON_MONEY,
        EXTEND_INVEN_BUTTON_EXIT,
        EXTEND_INVEN_IMAGE_BLIND00,
        EXTEND_INVEN_IMAGE_BLIND01,
        EXTEND_INVEN_IMAGE_BLIND02,
        EXTEND_INVEN_IMAGE_BLIND03,
        EXTEND_INVEN_IMAGE_BLIND04,
		EXTEND_INVENTORY_CONTROLS_MAX, 
	};

    enum
    {
        EXTEND_INVENTORY_MAX = EXTEND_INVEN_SLOT_10_14+1,
    };

    enum EXTEND_INVENTORY_LOCK_STATE
	{
		EXTEND_INVENTORY_LOCK_STATE_UNLOCK = 0,
		EXTEND_INVENTORY_LOCK_STATE_LOCK,
		EXTEND_INVENTORY_LOCK_STATE_MAX,
	};

    enum
	{
		EXTEND_INVENTORY = 0,
		EXTEND_TEMP_INVENTORY,
	};
public:	
	ExtendInventoryDialog(InterfaceManager* manager_ptr);
	virtual ~ExtendInventoryDialog(void);
	virtual void Init(CDrawBase* draw_base_ptr);
	virtual void Release();
	virtual void OnUpdateSolarDialog();
	virtual void OnRenderSolarDialog();
	virtual void MessageProc(SI_MESSAGE* message_ptr);
	virtual void OnShowWindow(BOOL value);
    virtual void MouseOver_MsgProc(SI_MESSAGE* pMessage);
    virtual void Process(DWORD tick_count);
    //virtual void UpdatePosition();
    uiInventoryMan*	GetManager();
    void InitRenderer();
    void RefreshRenderSlot();
    void InitTitleByLockState();
    void SetEventInvenState(bool value){ event_inven_off_ = value; }
    void ShowExtendInventory();

    CControlWZ* get_control(POSTYPE AtPos);
private:
	POSTYPE	get_control_id_to_position(WzID controls_id);
    void UpdateMoney();
	void InitControls();
    void RefreshExtendInventoryRenderUnit();
    void UpdateToolTip();
    void CheckChargeInventory(int charge_count);
public:
private:
	uiInventoryMan*	inventory_manager_ptr_;
    static WzID controls_id_[EXTEND_INVENTORY_CONTROLS_MAX];
    TCHAR money_string_[INTERFACE_STRING_LENGTH];
    TCHAR result_string_[INTERFACE_STRING_LENGTH];
    CCtrlStaticWZ* money_text_control_;
    CCtrlStaticWZ* title_static_text_;
    ItemUnitRenderer* item_unit_renderer_;
    InventoryDialog* inventory_dialog_;
    CCtrlButtonWZ* lock_buttons_[EXTEND_INVENTORY_LOCK_STATE_MAX];
    CCtrlButtonWZ* event_button_;
    bool event_inven_on_;
    bool event_inven_off_;
    int charge_count_;
    DWORD flash_tick_for_event_inven_;
};
//------------------------------------------------------------------------------
/**
*/
inline POSTYPE ExtendInventoryDialog::get_control_id_to_position(WzID controls_id)
{
	for (POSTYPE i = 0; i < EXTEND_INVENTORY_CONTROLS_MAX; ++i )
	{
		if (controls_id_[i] == controls_id)
		{
			return i;
		}
	}
	return EXTEND_INVENTORY_CONTROLS_MAX;
}

//------------------------------------------------------------------------------
/**
*/
inline CControlWZ* ExtendInventoryDialog::get_control(POSTYPE AtPos)
{
    if (AtPos >= EXTEND_INVENTORY_CONTROLS_MAX)
    {
        return NULL;
    }
	return GetControlWZ(controls_id_[AtPos]);
}

#endif//_EXTEND_INVENTORY_DIALOG_H
