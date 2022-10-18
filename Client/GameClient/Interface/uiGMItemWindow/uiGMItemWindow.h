#pragma once
#ifdef _DEV_VER
#include "uiBase.h"
#include "FTextRender.h"
//#include "Achievement.h"
#include "ItemInfoParser.h"
#include <SCItemSlotContainer.h>
#include "uiGMWindowMan/uiGMWindowMan.h"

#define SEARCH_FRAME_UPDATE_TIME 100
#define NUMBER_OF_SEARCHING_PER_A_FRAME 200
#define MAX_VISIBLE_FINDLIST_NUM	6

class uiGMWindowMan;
class uiGMItemWindow : public uiBase, public SCItemSlotContainer
{
    enum UIControlPos
    {
        kControl_Button_B011,       // AC항목아이콘#1
        kControl_Button_B012,
        kControl_Button_B013,
        kControl_Button_B014,
        kControl_Button_B015,
        kControl_Button_B016,
        kControl_Button_B017,
        kControl_Button_B018,
        kControl_Button_B019,
        kControl_Button_B020,       // AC항목아이콘#10

        kControl_Button_B999,   
        kControl_Button_BT00,       // 지역명선택버튼
        kControl_Button_BT01,       // 상태선택버튼
        kControl_Button_B000,       // AC항목버튼#1
        kControl_Button_B002,
        kControl_Button_B003,
        kControl_Button_B004,
        kControl_Button_B005,
        kControl_Button_B006,
        kControl_Button_B007,
        kControl_Button_B008,
        kControl_Button_B009,
        kControl_Button_B010,       // AC항목버튼#10
        kControl_Button_B001,       // AC항목보상버튼#1
        kControl_Button_B021,
        kControl_Button_B022,
        kControl_Button_B023,
        kControl_Button_B024,
        kControl_Button_B025,
#ifdef _NA_000000_20130812_NEW_CHARACTER_HELLROID
        kControl_Button_B032,
#endif // _NA_000000_20130812_NEW_CHARACTER_HELLROID
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        kControl_Button_B033,
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        kControl_Button_B026,
        kControl_Button_B027,
        kControl_Button_B028,
        kControl_Button_B029,       // AC항목보상버튼#10
        kControl_Button_B030,       // 상세정보출력
        kControl_Button_B031,       // 상세정보보상버튼
        kControl_Text_T201,         // *선택지역명
        kControl_Text_ST01,         // 선택상태명
        kControl_Text_ST02,         // 완료카운트
        kControl_Text_S000,         // 달성률
        kControl_Picture_P000,      // 달성률게이지
        kControl_List_LS00,         // *검색결과리스트
        kControl_List_LS01,         // *검색조건리스트
        kControl_VScroll_VS00,      // AC항목스크롤
        kControl_Edit_E000,         // Edit박스컨트롤
        kControl_Text_T999,         // *제목표시줄
        kControl_Text_T001,         // *검색어를 입력하세요
        kControl_Size,
    };

    enum
    {
        kDisplay_Item_Info_Num = 10,
    };

public:
    enum 
    {
        kSelect_All = 0,
        kSelect_Progress,
        kSelect_Complete,
        KSelect_Max,
    };

    enum
    {
        kComplete_Text_Color = 0,
        kInComplete_Text_Color,
        kEnable_Text_Color,
        kDisable_Text_Color,

        KMax_Text_Color,
    };

    enum
    {
        kSlotTotalCount = kControl_Button_B020 - kControl_Button_B011 + 1,
    };

    enum
    {
        kFind_From_Name, //이름 검색
        kFind_From_Description, //설명(디버그용이름) 검색
        kFind_From_ItemType, //아이템 타입 검색
        kFind_From_WasteType, //소모 타입 검색(WasteType)
        kFind_Type_MAX,
    };

    enum
    {
        kGradeLevel4 = 4,
    };
public:
    uiGMItemWindow(InterfaceManager* ui_manager);
    virtual ~uiGMItemWindow(void);

    virtual void Init(CDrawBase* draw_base);
    virtual void Release();
    virtual void OnUpdateSolarDialog();
    virtual void OnRenderSolarDialog();
    virtual void MessageProc(SI_MESSAGE* message);
    virtual void MouseOver_MsgProc(SI_MESSAGE* message);
    virtual void OnShowWindow(BOOL is_show);
    virtual void Process(DWORD tick_count);

    void ItemSearch(TCHAR* search_word);
    void SearchStop();
    bool search_end(){ return search_end_; }
    void set_search_end(bool search_end){ search_end_ = search_end; }
    TCHAR* search_string(){ return search_string_; }
    void set_search_string(TCHAR* search_string);
    void SetFilterName(TCHAR* filter_name);
    void CreateItem(BASE_ITEMINFO* item_info, ITEM_PARAMETAS* item_parameta, BYTE create_type = ITEM_TYPE_NORMAL, BYTE lucky_expiry = 0);
    void CreateItemFromList();
    void RefreshList(BYTE state, DWORD search_count, DWORD find_size);
    BYTE item_window_state(){ return item_window_state_; }
    void RefreshItemInfo();
    DWORD last_make_item_code(){ return last_make_item_code_; }
    void SetClass(BYTE class_type);
    void ShowClassIcon(bool is_show);

private:
    bool OnUpScroll(SI_MESSAGE* message);
    bool OnDownScroll(SI_MESSAGE* message);
    bool OnThumbScroll(SI_MESSAGE* message);
    void OnMouseWheelScroll(BYTE mouse_wheel_state);

    void RegisterControl();
    bool OnLButtonClick(SI_MESSAGE* message);
    bool OnRButtonClick(SI_MESSAGE* message);
    bool OnLButtonDoubleClick(SI_MESSAGE* message);
    bool ShowEditControl(bool is_show);
    void OnSearch();
    void FillFindListData(TCHAR* search_string);
    void HideFindWord();
    void ShowFindWord();
    void InsertFindList(const char* item_name);

    void SetItemData(int index_display, int index_info);
    void RefreshScroll(WORD ac_index_index);
    void ClearItemInfoToDisplay(int index_display);
    void SetItemDataString(int index_display, BASE_ITEMINFO* item_info);
    void SetRectItemControl(int index_display, bool enable_reward);
    void RenderItemInfoText(DWORD index_display);
    static void CallBackRenderItemInfo(CControlWZ* control_ptr, void* parameter);
    void ShowControl(POSTYPE control_pos, bool is_show);
    void ToggleShowControl(POSTYPE control_pos);
    void SetSearchProgressData(DWORD search_count, DWORD find_count);
    void UpdateToolTip();
    void SetItemSlot(int index_display, BASE_ITEMINFO* item_info);
    bool AddItem(POSTYPE at_pos, SLOTCODE item_code);
    RC::eSLOT_INSERT_RESULT InsertSlot(POSTYPE at_pos, SCSlot& slot);
    BOOL DeleteSlot(POSTYPE at_pos, SCSlot* slot_out);
    void DeleteAllSlot();
    void ReFreshRenderSlot();
    void ChangeFindIconImage(CControlWZ* control, bool is_searching);
    void RegisterESCKeyClose(bool is_value);
    static void ESCKeyProcess(DWORD wParam,DWORD lParam);

    bool GetFilterEquip(BASE_ITEMINFO* item_infomation, BYTE sub_filter);
    bool GetFilterWaste(BASE_ITEMINFO* item_infomation, BYTE sub_filter);
    bool GetFilterGrade(BASE_ITEMINFO* item_infomation, BYTE sub_filter);
    bool GetFilterGender(BASE_ITEMINFO* item_infomation, BYTE sub_filter);
    bool GetClassResult(DWORD item_code);
    bool GetFilterEtc(BASE_ITEMINFO *item_infomation, BYTE sub_filter);
    bool GetFilterResult(DWORD item_code);
    void LoadCreateHistory();
    std::vector<DWORD>* GetFocusList();
    DWORD GetFocusData(DWORD index);
    void SetItemWindowState(BYTE state);
    void RefreshPreFindList(TCHAR* find_string);
    void SetCurrentScrollPosition(int current_scroll_pos);
    void ChangeClassIcon(CControlWZ* control, BYTE class_type, bool is_checked);
    void MoveClassIcon(CControlWZ* control, BYTE class_type);

private:
    ItemUnitRenderer* item_unit_renderer_;
    uiGMWindowMan* ui_gm_window_manager_;
    CCtrlDxEditBox* edit_control_;
    CCtrlStaticWZ* static_edit_control_;
    CCtrlStaticWZ* static_state_control_;
    CCtrlListWZ* find_list_ptr_;
    CCtrlButtonWZ* button_control_;
    CCtrlButtonWZ* history_button_;
    CCtrlButtonWZ* pickup_button_;
    CCtrlButtonWZ* reset_button_;
    CCtrlStaticWZ* seacrch_type_text_control_;
    std::vector<FTextRender> item_info_text_render_;
    FTextRender info_detail_text_;

    ITEM_NAME_INFO::iterator item_name_info_iterator_;
    ITEM_NAME_INFO::iterator item_name_info_iterator_end_;

    TCHAR pre_find_word_[INTERFACE_STRING_LENGTH];
    TCHAR search_string_[MAX_CHAT_LEN+1];
    bool search_new_;
    bool search_end_;
    int search_index_;
    int find_scroll_current_value_;
    int find_scroll_value_max_;
    BYTE selected_class_;
    BYTE set_selected_class_;
    DWORD item_code_;
    unsigned int search_frame_per_update_time_;
    unsigned int number_of_searching_per_a_frame_;
    float search_gauge_control_size_;
    float search_gauge_texture_size_;
    BYTE search_type_;
    BYTE item_window_state_;
    unsigned int total_searched_count_;
    DWORD last_make_item_code_;

    int history_scroll_pos_;
    int search_scroll_pos_;
    int bookmark_scroll_pos_;

    std::vector<DWORD>* searched_results_;
    std::vector<DWORD>* created_histroys_;
    std::vector<DWORD>* bookmark_lists_;

#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
    HANDLE handle_texture_;
    St_TextureContainer* texture_information_;
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
};

inline std::vector<DWORD>* uiGMItemWindow::GetFocusList()
{
    if (item_window_state_ == kState_History)
    {
        return created_histroys_;
    }
    else if (item_window_state_ == kState_BookMark)
    {
        return bookmark_lists_;
    }
    else // kState_Search
    {
        return searched_results_;
    }
}

inline DWORD uiGMItemWindow::GetFocusData(DWORD index)
{
    if (item_window_state_ == kState_History)
    {
        size_t size = created_histroys_->size();// 히스토리는 역순
        if (index < size)
        {
            size_t real_index = (size - index - 1);
            if (real_index > size)
            {
                real_index = 0;
            }
            return (*created_histroys_)[real_index];
        }
        else
        {
            return 0;
        }
    }
    else if (item_window_state_ == kState_BookMark)
    {
        size_t size = bookmark_lists_->size();// 북마크도 역순
        if (index < size)
        {
            size_t real_index = (size - index - 1);
            if (real_index > size)
            {
                real_index = 0;
            }            
            return (*bookmark_lists_)[real_index];
        }
        else
        {
            return 0;
        }
    }
    else // kState_Search
    {
        return (*searched_results_)[index];
    }
}

inline void uiGMItemWindow::ShowControl(POSTYPE control_pos, bool is_show)
{
    CControlWZ* control = GetControlWZ(GetControlID_byPosition(control_pos));
    if (control != NULL)
    {
        control->ShowWindowWZ((is_show == true) ? WZ_SHOW : WZ_HIDE);
    }
}

inline void uiGMItemWindow::ToggleShowControl(POSTYPE control_pos)
{
    CControlWZ* control = GetControlWZ(GetControlID_byPosition(control_pos));
    if (control != NULL)
    {
        ENUM_STATEWZ control_state = control->GetState();
        control->ShowWindowWZ((control_state == WZ_HIDE) ? WZ_SHOW : WZ_HIDE);
    }
}
#endif//_DEV_VER 