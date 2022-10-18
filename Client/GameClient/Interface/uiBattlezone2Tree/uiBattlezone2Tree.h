#pragma once
#ifdef _DH_BATTLEZONE2_
#include "uiBase.h"

#ifdef _YMS_TREECONTROL
#include "./TreeMenuGroup.h"
#endif//_YMS_TREECONTROL
#include "VScrollDialog.h"

#include "uiBattlezone2Man/uiBattlezone2Man.h"
#include "BattlezoneMissionTreeParser.h"

class uiBattlezone2Man;
class uiBattlezone2Tree : public uiBase, public VScrollDialog
{
    enum UIControlPos
    {
        kControl_Button_B004 = 0,
        kControl_Button_B000,
        kControl_Button_B001,
        kControl_Button_B002,
        kControl_Button_B003,
        kControl_Text_TI00,
        kControl_Text_S000,
        kControl_Text_S001,
        kControl_Text_S002,
        kControl_Text_T001,
        kControl_Edit_E001,
        kControl_Edit_E002,
        kControl_List_L000,
        kControl_Picture_P003,
        kControl_Picture_P002,
        kControl_Picture_P000,
        kControl_Picture_P001,
        kControl_Picture_P004,
        kControl_Picture_P005,
        kControl_Picture_P007,
        kControl_Picture_P006,
        kControl_Picture_P008,
        kControl_Scroll_V001,
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

    uiBattlezone2Tree(InterfaceManager* ui_manager);
    virtual ~uiBattlezone2Tree(void);

    virtual void Init(CDrawBase* draw_base);

#ifdef _NA_003649_20111115_BATTLE_ZONE_HARDMODE
    void MakeBattlezoneTree(BYTE depth_3, BYTE depth_2, BYTE depth_1, BYTE depth_0, int depth, DWORD string_code, TCHAR* string, const DWORD* font_colors);
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
    bool CreateRoom();
    void set_save_party_key(WORD key){ save_party_key_ = key; }
    WORD save_party_key(){ return save_party_key_; }
    void set_last_create_room(DWORD val) { set_last_create_room_ = val; }
    DWORD GetRequireItem(const BattlezoneMissionTreeInfo* find_info);
    bool is_creating(){ return is_creating_; }
    void set_is_creating(bool is_creating){ is_creating_ = is_creating; }
    
#ifdef _NA_007192_20140428_CLIENT_LUA_MACRO_SYSTEM
    void set_created_room_menu_index(DWORD index) { created_room_.menu_index = index; }
    void SetDefaultRoomName(int select = -1);
    void SetRoomPassword(TCHAR* password);
#endif //_NA_007192_20140428_CLIENT_LUA_MACRO_SYSTEM

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
    bool OnDoubleLButtonClick(SI_MESSAGE* message);
    
    bool ReservationRoom();
    bool ShowEditControl(BYTE type, bool is_show);
    bool SelectMenu(TreeMenuGroup::TreeObject* tree_object);
    bool SelectCreate(TreeMenuGroup::TreeObject* tree_object);
    void SelectRoomClear();
    void SetReservationCheck();
    void SetCreationCheck();
    void AllCheckOff();
#ifdef _NA_007192_20140428_CLIENT_LUA_MACRO_SYSTEM
#else
    void SetDefaultRoomName(int select = -1);
#endif //_NA_007192_20140428_CLIENT_LUA_MACRO_SYSTEM

    void SetDefaultSpecialType();
    bool ModificationRoom();
    void SetLastData();

#ifdef _NA_004204_20120131_BATTLE_ZONE_PARTY_OBTAIN
    static void Callback_TreeRender(CCtrlListWZ* list_control,
        CCtrlListWZ::CListSellData* sell_data,
        int sell_index,
        RECT& render_area);
#endif //_NA_004204_20120131_BATTLE_ZONE_PARTY_OBTAIN

    bool is_creating_;//생성중인가?
    bool is_mouse_over_;
    DWORD set_last_create_room_;
    BATTLEZONE2_TREE selected_create_room_;//롤백위한것
    BYTE battlezone_tree_mode_;
#ifdef _YMS_TREECONTROL
    TreeMenuGroup tree_menu_;
#endif//_YMS_TREECONTROL
    uiBattlezone2Man* ui_battlezone2_manager_;
    BYTE selected_count_;
    //BATTLEZONE2_TREE selected_rooms_[TREE_MAX_ROOM];
    BATTLEZONE2_TREE created_room_;
    DWORD last_create_room_;
    WORD save_party_key_;

    CCtrlDxEditBox* edit_controls_[kEditControlTypeMax];
    CCtrlStaticWZ* static_edit_controls_[kEditControlTypeMax];
};
#endif //_DH_BATTLEZONE2_