#pragma once

#include "uibase.h"
#include "SCItemSlotContainer.h"
#include "DummySlotContainer.h"

class uiSocketComposite : public uiBase, public DummySlotContainer
{
    static const int kMax_Animate_Count = 240;
    static const int kMax_End_Count = 300;
    int kMax_End_Tick;

	enum DIALOG_POSITION
	{
        DIALOG_SOCKET_COMPOSITE_RESOURCE1,
        DIALOG_SOCKET_COMPOSITE_RESOURCE2,
        DIALOG_SOCKET_COMPOSITE_RESOURCE3,
        DIALOG_SOCKET_COMPOSITE_ADD_RESOURCE,
        DIALOG_SOCKET_COMPOSITE_RESULT_ITEM,
        DIALOG_SOCKET_COMPOSITE_LIST,
        DIALOG_SOCKET_COMPOSITE_EXIT,
        DIALOG_SOCKET_COMPOSITE_TITTLE,
        DIALOG_SOCKET_COMPOSITE_START,
        DIALOG_SOCKET_COMPOSITE_CANCLE,
        DIALOG_SOCKET_COMPOSITE_SUCCESS_AREA_BACKGROUND,
        DIALOG_SOCKET_COMPOSITE_MOVE_STICK,
        DIALOG_SOCKET_COMPOSITE_EFFECT_1,
        DIALOG_SOCKET_COMPOSITE_EFFECT_2,
        DIALOG_SOCKET_COMPOSITE_EFFECT_3,
        DIALOG_SOCKET_COMPOSITE_EFFECT_4,
		DIALOG_SOCKET_COMPOSITE_CONTROLS_MAX,
	};

    enum SOCKET_COMPOSITE_BUTTONS
    {
        RESOURCE1_BUTTON = 0,
        RESOURCE2_BUTTON,
        RESOURCE3_BUTTON,
        ADD_RESOURCE_BUTTON,
        RESULT_ITEM_BUTTON,
        START_BUTTON,
        CANCLE_BUTTON,
        SOCKET_COMPOSITE_BUTTONS_MAX,
    };

    enum SOCKET_COMPOSITE_PICTURES
    {
        SUCCESS_AREA_BACKGROUND = 0,
        MOVE_STICK,
        EFFECT_1,
        EFFECT_2,
        EFFECT_3,
        EFFECT_4,
        SOCKET_COMPOSITE_PICTURES_MAX,
    };

public:
    enum SOCKET_COMPOSITE_SLOT
    {
        SOCKET_COMPOSITE_SLOT_RESOURCE1 = 0,
        SOCKET_COMPOSITE_SLOT_RESOURCE2,
        SOCKET_COMPOSITE_SLOT_RESOURCE3,
        SOCKET_COMPOSITE_SLOT_RESOURCE_MAX,
        SOCKET_COMPOSITE_SLOT_ADD_RESOURCE = SOCKET_COMPOSITE_SLOT_RESOURCE_MAX,
        SOCKET_COMPOSITE_SLOT_RESULT_ITEM,
        SOCKET_COMPOSITE_SLOT_RESULT_MAX,
    };

    enum SOCKET_COMPOSITE_INFORMATION
    {
        SOCKET_COMPOSITE_INFO_STRING1,
        SOCKET_COMPOSITE_INFO_STRING2,
        SOCKET_COMPOSITE_INFO_STRING3,
        SOCKET_COMPOSITE_INFO_STRING4,
        INFORMATION_MAX,
    };
    
public:
	uiSocketComposite(InterfaceManager* ui_manager);
	~uiSocketComposite();
	virtual void		Init(CDrawBase* draw_base);
	virtual void		Release();
	virtual void		OnRenderSolarDialog();
	virtual void		Process(DWORD tick_count);	
	virtual void		OnShowWindow(BOOL open_value);
	virtual void		MessageProc(SI_MESSAGE* message);
	virtual void		NetworkProc(MSG_BASE* message);
    void MouseOver_MsgProc(SI_MESSAGE* message);
    SCSlotContainer* GetSlotContainer();
    RC::eSLOT_INSERT_RESULT InsertSlot(POSTYPE at_pos, SCSlot& slot);
    BOOL DeleteSlot(POSTYPE at_pos, SCSlot* slotOut);
    void DeleteAllSlot();
    void DeleteResultSlot();
    void DeleteResourceSlot();
    void OnSocketComposite_InventoryRightClick(const SLOTIDX from_container_index, 
        const POSTYPE from_container_position);
    void SetMessage(int index, TCHAR* message);
    void ResetAnimation();

protected:
	CControlWZ*		get_control(POSTYPE AtPos);
	POSTYPE			get_control_id_to_pos(WzID wzID);
    void SetGague(float ratio);
    void AnimateEffect();
    float RollGoalPosition(bool is_success);

private:
    ItemUnitRenderer* item_unit_renderer_;
    static WzID		wz_control_id_[DIALOG_SOCKET_COMPOSITE_CONTROLS_MAX];
    CCtrlListWZ* socket_composite_list_;
    CCtrlButtonWZ* socket_composite_button_[SOCKET_COMPOSITE_BUTTONS_MAX];
    CCtrlPictureWZ* socket_composite_picture_[SOCKET_COMPOSITE_PICTURES_MAX];
    POSTYPE item_inventory_position_[SOCKET_COMPOSITE_SLOT_RESOURCE_MAX];
    POSTYPE additional_item_position_;
    int animate_frame_count_;
    int success_sound_count_;
    int fail_sound_count_;
    //int animate_total_frame_;

    DWORD animate_now_tick_;
    DWORD animate_start_tick_;
    DWORD animate_end_tick_;
    DWORD animate_tick_gap_; // 
    int fail_number_;
    bool socket_composite_lock_;
    INVENTORY_TOTAL_INFO save_inventory_item_info;
    int result_string_code_;
    bool socket_composite_success_;
    TCHAR message_[INFORMATION_MAX][INTERFACE_STRING_LENGTH];
    float gague_left_; // 성공영역의 왼쪽 좌표
    float gague_right_;// 성공영역의 오른쪽 좌표
    float move_gap_x_; // 현재 막대의 총 이동 거리
    float goal_position_; // 목표하는 좌표

    //==========================================================
    void InitAnimation();
    void BaseAnimation();
    void AddtionalAnimation();
    void MoveEffect(RECT dialog_rect, RECT control_size, float position_x);
    void MoveStick(RECT dialog_rect, RECT control_size, float position_x);
    //void CompleteAnimation();
    //void CompleteMessage(int string_code);
    void ControlsStateInit();
};

inline CControlWZ* uiSocketComposite::get_control(POSTYPE AtPos)
{
	assert(AtPos < DIALOG_SOCKET_COMPOSITE_CONTROLS_MAX);
	return GetControlWZ(wz_control_id_[AtPos]);
}

inline POSTYPE uiSocketComposite::get_control_id_to_pos(WzID wzID)
{
	for ( int i = 0; i < DIALOG_SOCKET_COMPOSITE_CONTROLS_MAX ; ++i)
	{
		if ( wz_control_id_[i] == wzID)
			return i;
	}
	return DIALOG_SOCKET_COMPOSITE_CONTROLS_MAX;
}
