#ifndef     _UIITEMCUBE_H
#define     _UIITEMCUBE_H

#include "uiBase.h"
#include "SCItemSlotContainer.h"
#include "DummySlotContainer.h"

class uiItemCompositeMan;
class uiItemCube : public uiBase, public DummySlotContainer
{
	static const int kTextures_Frame = 4;
	static const int kMax_Animate_Count = 120;
	static const int kMax_Animate_Tick = 4000;

	enum CompositeType
	{
		kType_Composite = 0,
		kType_Decomposite,
		kType_Composite_Max,
	};    

	enum UIControlPos
	{
		kControl_Button_B020 = 0,
		kControl_Button_B001,
		kControl_Button_B002,
		kControl_Button_B003,
		kControl_Button_B004,
		kControl_Button_B005,
		kControl_Button_B006,
		kControl_Button_B007,
		kControl_Button_B008,
		kControl_Button_BT01,
		kControl_Button_B009,
		kControl_Button_B000,
		kControl_Text_T999,

		kControl_Button_B010,   // ▶버튼
		kControl_Button_B011,   // ◀버튼
		kControl_Button_B012,   // 리스트 영역1 선택 버튼
		kControl_Button_B013,   // 리스트 영역2 선택 버튼
		kControl_Button_B014,   // 리스트 영역3 선택 버튼
		kControl_Button_B015,   // 리스트 영역4 선택 버튼
		kControl_Button_B016,   // 리스트 영역5 선택 버튼

		kControl_Button_B017,   // 리스트 아이템 슬롯1
		kControl_Button_B018,   // 리스트 아이템 슬롯2
		kControl_Button_B019,   // 리스트 아이템 슬롯3
		kControl_Button_B021,   // 리스트 아이템 슬롯4
		kControl_Button_B022,   // 리스트 아이템 슬롯5
		kControl_Button_B023,   // 리스트 아이템 슬롯0 (선택후)

		kControl_Button_B024,   // 잠김 버튼 1       
		kControl_Button_B026,   // 잠김 버튼 2
		kControl_Button_B027,   // 잠김 버튼 3
		kControl_Button_B028,   // 잠김 버튼 4
		kControl_Button_B029,   // 잠김 버튼 5

		kControl_Button_B025,   // 열림 버튼 1
		kControl_Button_B030,   // 열림 버튼 2
		kControl_Button_B031,   // 열림 버튼 3
		kControl_Button_B032,   // 열림 버튼 4
		kControl_Button_B033,   // 열림 버튼 5

		kControl_List_L000,     // 리스트 1
		kControl_List_L001,     // 리스트 2
		kControl_List_L003,     // 리스트 3
		kControl_List_L002,     // 리스트 4        
		kControl_List_L004,     // 리스트 5
		kControl_List_L005,     // 선택된 아이템 이름 표시 리스트
		kControl_List_L006,     // 리스트 확장 설명

		kControl_Picture_P000,  // 전체 배경
		kControl_Picture_P025,  // 작은 버튼 배경
		kControl_Picture_P003,  // 선택된 아이템 배경
		kControl_Picture_P004,  // 아이템 슬롯
		kControl_Picture_P002,  // 확장 설명 배경
		kControl_Picture_P500,	// 컨트롤 확장 배경

		kControl_VScroll_V000,  // 리스트 스크롤바
		kControl_Static_S000,  // 리스트 제목
		
		//새로 추가된 리스트
		kControl_Picture_P444,
		//kControl_Picture_P004,
		kControl_Picture_P005,
		kControl_Picture_P007,
		kControl_Picture_P008,
		kControl_Picture_P020,
		kControl_Picture_P999,

        // 2013.04.03 / 홍성현 / P006(큐브 다이얼로그) 추가
        kControl_Picture_P006,  

		kControl_Size,        
	};

	enum UIControlPosName
	{
		kControl_Button_Slot0 = kControl_Button_B020,   // 슬롯0
		kControl_Button_Slot1 = kControl_Button_B001,   // 슬롯1
		kControl_Button_Slot2 = kControl_Button_B002,   // 슬롯2
		kControl_Button_Slot3 = kControl_Button_B003,   // 슬롯3
		kControl_Button_Slot4 = kControl_Button_B004,   // 슬롯4
		kControl_Button_Slot5 = kControl_Button_B005,   // 슬롯5
		kControl_Button_Slot6 = kControl_Button_B006,   // 슬롯6
		kControl_Button_Slot7 = kControl_Button_B007,   // 슬롯7
		kControl_Button_Slot8 = kControl_Button_B008,   // 슬롯8
		kControl_Button_Composite = kControl_Button_BT01,   // 합성
		kControl_Button_DeComposite = kControl_Button_B009, // 분해
		kControl_Button_Exit = kControl_Button_B000,    // 종료 버튼
		kControl_Text_Title = kControl_Text_T999,

		kControl_Button_ListClose = kControl_Button_B010,   // ▶버튼
		kControl_Button_Open = kControl_Button_B011,   // ◀버튼
		kControl_Button_List1 = kControl_Button_B012,   // 리스트 영역1 선택 버튼
		kControl_Button_List2 = kControl_Button_B013,   // 리스트 영역2 선택 버튼
		kControl_Button_List3 = kControl_Button_B014,   // 리스트 영역3 선택 버튼
		kControl_Button_List4 = kControl_Button_B015,   // 리스트 영역4 선택 버튼
		kControl_Button_List5 = kControl_Button_B016,   // 리스트 영역5 선택 버튼

		kControl_Button_ListSlot1 = kControl_Button_B017,   // 리스트 아이템 슬롯1
		kControl_Button_ListSlot2 = kControl_Button_B018,   // 리스트 아이템 슬롯2
		kControl_Button_ListSlot3 = kControl_Button_B019,   // 리스트 아이템 슬롯3
		kControl_Button_ListSlot4 = kControl_Button_B021,   // 리스트 아이템 슬롯4
		kControl_Button_ListSlot5 = kControl_Button_B022,   // 리스트 아이템 슬롯5
		kControl_Button_ListSlot0 = kControl_Button_B023,   // 리스트 아이템 슬롯0 (선택후)

		kControl_Button_Lock1 = kControl_Button_B024,   // 잠김 버튼 1       
		kControl_Button_Lock2 = kControl_Button_B026,   // 잠김 버튼 2
		kControl_Button_Lock3 = kControl_Button_B027,   // 잠김 버튼 3
		kControl_Button_Lock4 = kControl_Button_B028,   // 잠김 버튼 4
		kControl_Button_Lock5 = kControl_Button_B029,   // 잠김 버튼 5

		kControl_Button_UnLock1 = kControl_Button_B025,   // 열림 버튼 1
		kControl_Button_UnLock2 = kControl_Button_B030,   // 열림 버튼 2
		kControl_Button_UnLock3 = kControl_Button_B031,   // 열림 버튼 3
		kControl_Button_UnLock4 = kControl_Button_B032,   // 열림 버튼 4
		kControl_Button_UnLock5 = kControl_Button_B033,   // 열림 버튼 5

		kControl_List1 = kControl_List_L000,     // 리스트 1
		kControl_List2 = kControl_List_L001,     // 리스트 2
		kControl_List3 = kControl_List_L003,     // 리스트 3
		kControl_List4 = kControl_List_L002,     // 리스트 4        
		kControl_List5 = kControl_List_L004,     // 리스트 5
		kControl_List0 = kControl_List_L005,     // 선택된 아이템 이름 표시 리스트
		kControl_List_Description = kControl_List_L006,     // 리스트 확장 설명


		KControl_Picture_Back =kControl_Picture_P500,
		kControl_Picture_Top = kControl_Picture_P025,
		kControl_List_Background = kControl_Picture_P000,  // 전체 배경
		kControl_Small_Background = kControl_Picture_P002,  // 작은 버튼 배경
		kControl_Select_Background = kControl_Picture_P003,  // 선택된 아이템 배경
		kControl_Slot_Background = kControl_Picture_P004,  // 아이템 슬롯
		kControl_Description_Background = kControl_Picture_P002,  // 확장 설명 배경
		kControl_List_Scroll = kControl_VScroll_V000,  // 리스트 스크롤바
		kControl_List_Title = kControl_Static_S000,  // 리스트 제목

        // 2013.04.03 / 홍성현 / P006(큐브 다이얼로그) 추가
        kControl_Picture_Cube_Back = kControl_Picture_P006,  // 큐브 다이얼로그 전체 배경		

		kControl_Max_Size = kControl_Size,
	};
public:
	uiItemCube(InterfaceManager* ui_man);
	virtual ~uiItemCube();

	virtual void Init(CDrawBase* draw_base);
	virtual void Release();

	uiItemCompositeMan* GetManager();    
	virtual void MessageProc(SI_MESSAGE* message_ptr);
	void MouseOver_MsgProc(SI_MESSAGE* message);
	void NetworkProc(MSG_BASE* message);

	SCSlotContainer* GetSlotContainer();
	RC::eSLOT_INSERT_RESULT uiItemCube::InsertSlot(POSTYPE at_pos, SCSlot& slot);
	BOOL DeleteSlot(POSTYPE at_pos, SCSlot* slotOut);
	void DeleteAllSlot();

	void OnItemCube_InventoryRightClick(const SLOTIDX from_container_index, 
		const POSTYPE from_container_position);

	void SetCubeItemPosition(POSTYPE real_position){ cube_position_ = real_position; }
	POSTYPE cube_position(){ return cube_position_; }
    bool Move_Click_Check(){ return click_move_enable_; } 
protected:
	virtual void OnShowWindow(BOOL value);
	virtual void OnRenderSolarDialog();
	virtual void OnUpdateSolarDialog();

private:
	bool OnDoubleButtonClick(SI_MESSAGE* message);
	bool OnLButtonClick(SI_MESSAGE* message);
	bool OnRButtonClick(SI_MESSAGE* message);
	bool OnScroll(SI_MESSAGE* message);
	bool OnMouseOver(SI_MESSAGE* message);
	void ResetAnimation();
	void InitTextures();
	void InitContainer();
	void AnimateEffect(int type);
	bool ShowItemVisible();
	bool ExecuteCube(bool is_composition);

	void ShowCubeLists(bool is_value);
	void ShowBackgrounds(bool is_value);
	void ShowSelectionControls(bool is_value);
	void ShowListControls(bool is_value);
	void ShowListOpenButton(bool is_value);

	void SetListLock(int index, bool is_lock);
	void SetListItem(int index, int focus);
	void SetListItems();
	void SetListLocking(int index, int focus);
	void SetItemIcon(int index, int focus);
	void OpenDetailList(int index);
	void SetDetailListItem(int focus);

	void UpdateToolTip();
	void SendPacketGetList();
	void SendPacketListLock();
	WzColor GetAlphaValue();
	/*  bool OnCubeItem(const SLOTIDX from_container_index, const POSTYPE from_container_position,
	const POSTYPE to_container_position);*/
private:
	ItemUnitRenderer* item_unit_renderer_;
	FRECT texture_rectangles_[kTextures_Frame];
	HANDLE effect_texture_[kType_Composite_Max];
	int animate_frame_count_;
	int animate_effect_type_;
	DWORD animate_now_tick_;
	DWORD animate_start_tick_;
	DWORD animate_end_tick_;
	DWORD tooltip_string_code_;
	bool cube_lock_;
    bool click_move_enable_;
	POSTYPE item_inventory_position[kControl_Button_B008+1];
	INVENTORY_TOTAL_INFO save_inventory_item_info;
	POSTYPE cube_position_;
	ItemCompositionOrDecompositionHistory history_list_[MSG_CG_ITEM_GET_COMPOSITION_OR_DECOMPOSITION_HISTORIES_ACK::kMaxHistoryListSize];
	int list_current_focus_;
	int list_size_;
	MSG_CG_ITEM_MODIFY_COMPOSITION_OR_DECOMPOSITION_HISTORIES_CMD::ModifyCondition 
		modify_condition_list_[MSG_CG_ITEM_MODIFY_COMPOSITION_OR_DECOMPOSITION_HISTORIES_CMD::kMaxConditionListSize];
	DWORD default_list_background_height_;
	bool is_show_cube_history_;
};

#endif//_UIITEMCUBE_H