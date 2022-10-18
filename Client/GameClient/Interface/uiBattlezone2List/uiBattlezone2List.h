#pragma once
#ifdef _DH_BATTLEZONE2_
#include "uiBase.h"
#include "DropdownControl.h"
#include "VScrollDialog.h"

#define ANIMATION_FRAME 12
#define ANIMATION_FRAME_WIDTH 40.0f
#define ANIMATION_FRAME_HEIGHT 40.0f

class uiBattlezone2Man;
class uiBattlezone2List : public uiBase, public VScrollDialog
{
    enum UIControlPos
    {
        kControl_Button_B004 = 0,
        kControl_Button_B000,
        kControl_Button_B005,
        kControl_Button_B006,
        kControl_Button_B007,
        kControl_Button_B008,
        kControl_Button_B009,
        kControl_Button_B010,
        //kControl_Text_ST20,
        kControl_Button_B016,
        kControl_Text_S001,
        kControl_Text_S002,
        kControl_Text_S000,
        kControl_Text_S003,
        kControl_Text_S005,
        kControl_Text_S006,
        kControl_Text_S007,
        kControl_Edit_E000,
        kControl_List_L000,
        kControl_List_L001,
        kControl_Picture_P003,
        kControl_Picture_P002,
        kControl_Picture_P000,
        kControl_Picture_P001,
        kControl_Picture_P004,
        kControl_Picture_P005,
        kControl_Picture_P006,
        kControl_Picture_P007,
        kControl_Picture_P009,
        kControl_Picture_P010,
        kControl_VScroll_V001,
        kControl_VScroll_V000,

        kControl_Text_S008,
        kControl_Button_B001,
        kControl_Button_B002,
        kControl_List_L002,

        kControl_Button_B012,
        kControl_Button_B003,
        kControl_Button_B011,
        kControl_Button_B013,
        kControl_Button_B014,
        kControl_Button_B015,
        
        kControl_Size,
    };
    
    enum
    {
        BATTLEZONE2_LIST_NUM,
        BATTLEZONE2_LIST_TYPE,
        BATTLEZONE2_LIST_STATE,
        BATTLEZONE2_LIST_TITLE,
        BATTLEZONE2_LIST_MEMBER,
        BATTLEZONE2_LIST_MAPNAME,
        BATTLEZONE2_LIST_MAX,
    };

    enum
    {
        BATTLEZONE2_FIND_BY_MAP_NAME = 0,
        BATTLEZONE2_FIND_BY_ID,
        BATTLEZONE2_FIND_BY_NUMBER,
        BATTLEZONE2_FIND_MAX,
    };

    typedef struct _BATTLEZONE_MAP_INFO
    {
        MAPCODE map_code;
        TCHAR map_name[INTERFACE_STRING_LENGTH];
        BYTE type;
    }BATTLEZONE_MAP_INFO;

public:
    uiBattlezone2List(InterfaceManager* ui_manager);
    virtual ~uiBattlezone2List(void);

    virtual void Init(CDrawBase* draw_base);
    virtual void Release();
    virtual void OnUpdateSolarDialog();
    virtual void OnRenderSolarDialog();
    virtual void MessageProc(SI_MESSAGE* message);
    virtual void MouseOver_MsgProc(SI_MESSAGE* message);
    virtual void OnShowWindow(BOOL is_show);
    virtual void Process(DWORD tick_count);
    virtual void NetworkProc(MSG_BASE* message_ptr);
    uiBattlezone2Man* GetManager();
    void ShowBattlezoneTree(BYTE mode);
    bool EnterRoom(WORD party_key, const TCHAR* password, PartyRoomInfo* party_info);

    DropdownControl& dropdown_class_filter() { return dropdown_class_filter_; }
    void Clear();
    void SetIsSSQOpen(bool val);
    bool order_by(){ return order_by_; }

    WORD follow_party_key(){ return follow_party_key_; }
protected:
    virtual CCtrlVScrollWZ* vscroll_control();
    virtual bool GetScrollArea(RECT& area);
    virtual bool OnVScrollDown(SI_MESSAGE* msg);
    virtual bool OnVScrollUp(SI_MESSAGE* msg);
    virtual bool OnVScrollThumb(SI_MESSAGE* msg);

    virtual CCtrlVScrollWZ* find_vscroll_control();
    virtual bool GetFindScrollArea(RECT& area);
    virtual bool OnFindVScrollDown(SI_MESSAGE* msg);
    virtual bool OnFindVScrollUp(SI_MESSAGE* msg);
    virtual bool OnFindVScrollThumb(SI_MESSAGE* msg);
    bool FindVScrollUp(SI_MESSAGE* msg);
    bool FindVScrollDown(SI_MESSAGE* msg);

private:
    void RegisterControl();
    bool OnDoubleButtonClick(SI_MESSAGE* message);
    bool OnLButtonClick(SI_MESSAGE* message);
    bool OnRButtonClick(SI_MESSAGE* message);
    bool OnMouseOver(SI_MESSAGE* message);
    void ToggleViewRoom(bool is_show);
    bool ShowEditControl(bool is_show);
    void RequestBattlezoneList(bool is_fast = false);
    void OnSearch();
    void SetBattlezoneList();
    void ShowBattlezoneLobby();
    void RenderIcon();
    void SearchID(TCHAR* search_id);
    void SearchNo(WORD number);
    void SearchMap(TCHAR* search_map, bool is_button = false);
    void SearchResult(DWORD result_code, PartyRoomInfo* party_room_info);
    void RecvBattlezoneList(MSG_BASE* message_ptr);
    void RecvBattlezoneWatingList(MSG_BASE* message_ptr);
    void RecvBattlezoneMapNameList(MSG_BASE* message_ptr);
    void SetPage(int current_page, int max_page);
    void SortList(int sort_by, bool order_by);
    void InitTextures();
    void PlayAnimation();
    void ShowAnimation(bool is_value);
    void ShowFindWords(bool is_value);
    DWORD LoadMapNameCode();
    void InsertFindList(const char* item_name);
    void FillFindListData(TCHAR* map_name);
    void SendMapFind(BATTLEZONE_MAP_INFO* select_map_name_info);
    void ShowWaitingButton(bool is_show);

    DropdownControl dropdown_class_filter_;

    std::vector<PartyRoomInfo> battlezone_list_;

    PartyRoomInfo* last_party_info_;
    uiBattlezone2Man* ui_battlezone2_manager_;
    CCtrlDxIMEEditBox* edit_control_;
    CCtrlStaticWZ* static_edit_control_;
    CCtrlStaticWZ* static_page_control_;
    CCtrlListWZ* list_control_;
    CCtrlListWZ* find_list_ptr_;
    WORD follow_party_key_;
    int last_sort_by_function_;
    CTimerSimple list_refresh_timer_;
    CTimerSimple animation_refresh_timer_;
    bool is_searched_;
    int search_count_;
    int last_scroll_position_;
    //DropdownControl find_type_;
    bool is_ssq_open_;
    bool order_by_;

    int current_page_; //현재페이지
    int max_page_; //최대페이지
    bool is_first_;//처음 여는 것인지

    HANDLE effect_texture_;
    FRECT texture_rectangles_[ANIMATION_FRAME];
    bool animatino_show_;

    std::vector<BATTLEZONE_MAP_INFO> map_infomations_;
    std::vector<BATTLEZONE_MAP_INFO> searched_results_;
};
#endif//_DEV_VER //_DH_GM_ITEM_WINDOW