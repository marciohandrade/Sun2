#pragma once

#ifdef _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM

#include "uiBase.h"
#include "ItemSlotContainer.h"
#include "FTextParser.h"
#include "FTextSentence.h"
#include "FTextParam.h"
#include "FTextRender.h"
#include "uiSystemMan/uiSystemMessage.h"

class uiEquipmentWakeupMan;
class ItemUnitRenderer;
class uiEquipmentAwakeningDialog : public uiBase
{
	enum UIControlPos
	{
		kControl_Button_B000, // 각성 슬롯
		kControl_Button_B002, // 재료 슬롯
		kControl_Button_B003, // 첨가제 슬롯

		kControl_Button_B999, // x 버튼

		kControl_Text_T004,   // 각성 아이템 텍스트1
		kControl_Text_S001,   // 각성 아이템 텍스트2

		kControl_Text_S004,   // 재료 아이템 텍스트1
		kControl_Text_S007,   // 재료 아이템 텍스트2

		kControl_Text_S017,   // 첨가제 아이템 텍스트1
		kControl_Text_S016,   // 첨가제 아이템 텍스트2

		kControl_Picture_P001, 
		kControl_Picture_P008, 
		kControl_Picture_P009,
		kControl_Picture_P010, 
		kControl_Picture_P017, 
		kControl_Picture_P020, 

		kControl_Button_B005,

		kControl_Button_B006,

		kControl_Button_B001,

		kControl_Size,
	};

	enum EQUIPMENT_AWAKENING_SLOT_POS
	{
		EQUIPMENT_EQUIP_SLOT = 0,
		EQUIPMENT_MATERIAL_SLOT,
		EQUIPMENT_ADDITIVE_SLOT,
		EQUIPMENT_AWAKENING_SLOT_MAX,
	};

	enum ePACKET_STATUS
	{
		PACKET_STATUS_NONE = 0,
		PACKET_STATUS_AWAKENING = 1,
	};

public:
	enum AWAKENING_MODE
	{
		AWAKENING_MODE_EQUIPMENT = 0,    // 장비 등록 모드
		AWAKENING_MODE_MATERIAL,         // 재료 등록 모드
		AWAKENING_MODE_ADDITIVE,         // 첨가제 등록 모드
		AWAKENING_MODE_COMPLETE,
	};
public:
	uiEquipmentAwakeningDialog(InterfaceManager* ui_manager);
	virtual ~uiEquipmentAwakeningDialog(void);

public:
	virtual void Init(CDrawBase* draw_base);
	virtual void Release();
	virtual void OnShowWindow(BOOL is_show);
	virtual void MessageProc(SI_MESSAGE* message);
	virtual void NetworkProc(MSG_BASE* recv_packet);
	virtual void OnUpdateSolarDialog();
	virtual void OnRenderSolarDialog();

	SCItemSlotContainer* GetEquipmentWakeupContainer() { return slot_container_[0].GetSlotContainer(); }

private:
	bool         OnLButtonClick(SI_MESSAGE* message);
	bool         OnRButtonClick(SI_MESSAGE* message);
	bool         OnMouseOver(SI_MESSAGE* message);
	void         UpdateTooltip();

private:
	void         InitEquipmentAwakeningSlotContainer();
	void         InitFtextRender();
	bool         PermitEquipmentCheck(const SCItemSlot* item); // 장비 슬롯 조건 체크
	bool         PermitMaterialCheck(const SCItemSlot* item);  // 재료 슬롯 조건 체크
	bool         PermitAdditiveCheck(const SCItemSlot* item); // 첨가제 슬롯 조건 체크
	void         EquipmentInfoSetting();
	void         MaterialInfoSetting();
	void         AdditiveInfoSetting();
	void         UpdateFtextRenderRectSetting();
	void         EquipmentAwakeningSlotClearSetting();
	void         MaterialSlotClearSetting();
	void         AdditiveSlotClearSetting();
	void         EquipmentAwakeningItemLock(const bool& is_lock);
	void         EquipmentAwakeningMaterialItemLock(const bool& is_lock);
	void         EquipmentAwakeningAdditiveItemLock(const bool& is_lock);
	void         Awakening();
	void         OpenTutorialGuid();

public:
	void         OnEquipmentAwakeningItem_InventoryRightClick(const SLOTIDX from_container_index,
		const POSTYPE from_container_position);
	void         OnEquipmentAwakeningItem_Click(const POSTYPE& equipment_wakeup_slot_index, const SLOTIDX from_container_index,
		const POSTYPE from_container_position);
	void         ResultCodeProc(const RC::eITEM_RESULT& result_code);
	void         ResultCodeSoundProc(const RC::eITEM_RESULT& result_code);
	void         RemoveBitCheck();

	static void Callback_SendPacketItemAwakeningSyn(uiSystemMessage::ButtonIndex button_index);
	void         SendPacketItemAwakeningSyn(bool is_send_packet);
	void         ResetAwakeningDialog();
	AWAKENING_MODE GetAwakeningUIMode();

public:
	POSTYPE      get_equip_item_pos_() const { return equip_item_pos_; }
	POSTYPE      get_material_item_pos_() const { return material_item_pos_; }
	POSTYPE      get_additive_item_pos_() const { return additive_item_pos_; }

protected:
	POSTYPE      getCtrlIDToPos( WzID wzId );
	CControlWZ*  getControl( POSTYPE pos );

private:
	static WzID wz_id_[kControl_Size];

private:
	uiEquipmentWakeupMan* ui_equipment_awakening_manager_;

private:
	ItemSlotContainer     slot_container_[EQUIPMENT_AWAKENING_SLOT_MAX];
	ItemUnitRenderer*     item_unit_renderer_;
	POSTYPE               equip_item_pos_;
	POSTYPE               material_item_pos_;
	POSTYPE               additive_item_pos_;
	FTextParser           equipment_awakening_ftext_parser_;
	FTextSentence         equipment_awakening_ftext_sentence_;
	FTextParam            equipment_awakening_ftext_param_;
	FTextRender           equipment_awakening_first_render_;
	FTextRender           equipment_awakening_second_render_;
	FTextRender           material_first_render_;
	FTextRender           material_second_render_;
	FTextRender           additive_first_render_;
	FTextRender           additive_second_render_;
};

inline
POSTYPE uiEquipmentAwakeningDialog::getCtrlIDToPos( WzID wzId )
{
	for ( POSTYPE i = 0; i < kControl_Size; ++i )
	{
		if ( wz_id_[i] == wzId )
		{
			return i;
		}
	}
	return kControl_Size;
}
#endif // _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM
