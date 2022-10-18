#pragma once
#ifdef _DH_BATTLEZONE2_
#include "uiBase.h"

#ifdef _YMS_TREECONTROL
#include "./TreeMenuGroup.h"
#endif//_YMS_TREECONTROL
#include "VScrollDialog.h"

#include "uiBattlezone2Man/uiBattlezone2Man.h"

#define TREE_MAX_ROOM 100

class uiBattlezone2Man;
class uiBattlezone2Reservation : public uiBase, public VScrollDialog
{
    enum UIControlPos
    {
        kControl_Button_B004 = 0,
        kControl_Button_B001,
        kControl_Button_B002,
        kControl_Button_B003,
        kControl_Button_B000,
        kControl_Button_B005,
        kControl_List_L000,
        kControl_VScroll_V001,
        kControl_Size,
    };

public:
    enum UIMode
    {
        kCreateMode = 0,
        kModificationMode,
        kReservationMode,        
        kModeMax,
    };

    enum EditControlType
    {
        kTitleEditControl = 0,
        kPasswordEditControl,
        kEditControlTypeMax,
    };

    enum
    {
        eCREATE_BATTLE_MISSION_TYPE = 0,
        eCREATE_BATTLE_PVP_TYPE,
    };

    uiBattlezone2Reservation(InterfaceManager* ui_manager);
    virtual ~uiBattlezone2Reservation(void);

    virtual void Init(CDrawBase* draw_base);

#ifdef _NA_003649_20111115_BATTLE_ZONE_HARDMODE
    void MakeBattlezoneTree(BYTE depth_3, BYTE depth_2, BYTE depth_1, BYTE depth_0, int depth, DWORD string_code, TCHAR* string, const DWORD* font_color);
#else
    void MakeBattlezoneTree(BYTE depth_3, BYTE depth_2, BYTE depth_1, BYTE depth_0, int depth, DWORD string_code, TCHAR* string);
#endif//_NA_003649_20111115_BATTLE_ZONE_HARDMODE
    virtual void Release();
    virtual void OnUpdateSolarDialog();
    virtual void OnRenderSolarDialog();
    virtual void MessageProc(SI_MESSAGE* message);
    virtual void MouseOver_MsgProc(SI_MESSAGE* message);
    virtual void OnShowWindow(BOOL is_show);
    virtual void Process(DWORD tick_count);
    virtual void NetworkProc(MSG_BASE* message_ptr);

    void OnMouseWheelScroll(BYTE mouse_wheel_state);
    bool OnUpScroll(SI_MESSAGE* message);
    bool OnDownScroll(SI_MESSAGE* message);
    bool OnThumbScroll(SI_MESSAGE* message);
    void SetMode(BYTE mode);
    BYTE battlezone_tree_mode(){ return battlezone_tree_mode_; }

#ifdef _YMS_TREECONTROL
    TreeMenuGroup& tree_menu() { return tree_menu_; }
#endif//_YMS_TREECONTROL
    uiBattlezone2Man* GetManager();
    void Clear();

    void set_save_party_key(WORD key){ save_party_key_ = key; }
    WORD save_party_key(){ return save_party_key_; }
    bool ReservationStandByRoom();
    bool is_stop() {  return is_stop_; }
    bool is_reservation() { return is_reservation_; }
protected:
    virtual CCtrlVScrollWZ* vscroll_control();
    virtual bool OnVScrollDown(SI_MESSAGE* msg);
    virtual bool OnVScrollUp(SI_MESSAGE* msg);
    virtual bool OnVScrollThumb(SI_MESSAGE* msg);
    virtual bool GetScrollArea(RECT& area);
private:
    void RegisterControl();
    bool OnLButtonClick(SI_MESSAGE* message);
    bool OnRButtonClick(SI_MESSAGE* message);
    bool OnMouseOver(SI_MESSAGE* message);
    bool CreateRoom();
    bool ReservationRoom();
    bool ShowEditControl(BYTE type, bool is_show);
    bool SelectMenu(TreeMenuGroup::TreeObject* tree_object);
    bool SelectCreate(TreeMenuGroup::TreeObject* tree_object);
    void SelectRoomClear();
    void SetReservationCheck();
    void SetCreationCheck();
    void AllCheckOff();
    void SetDefaultRoomName(int select = -1);
    bool ResetReservationRoom();
    bool ClearReservationRoom(bool value);
    void SetButton(BYTE position, bool is_enable);
    void ShowButton(BYTE position, bool is_show);
    bool CheckingSelect();
    void RestoreReservation();

    bool is_mouse_over_;
    BYTE battlezone_tree_mode_;
#ifdef _YMS_TREECONTROL
    TreeMenuGroup tree_menu_;
#endif//_YMS_TREECONTROL
    uiBattlezone2Man* ui_battlezone2_manager_;
    BYTE selected_count_;
    BYTE old_selected_count_;

    BATTLEZONE2_TREE checked_rooms_[TREE_MAX_ROOM];
    BATTLEZONE2_TREE selected_rooms_[TREE_MAX_ROOM];
    BATTLEZONE2_TREE created_room_;
    bool is_stop_;
    bool is_reservation_;
    WORD save_party_key_;

    /*CCtrlEditWZ* edit_controls_[kEditControlTypeMax];
    CCtrlStaticWZ* static_edit_controls_[kEditControlTypeMax];*/
};
#endif//_DH_BATTLEZONE2_