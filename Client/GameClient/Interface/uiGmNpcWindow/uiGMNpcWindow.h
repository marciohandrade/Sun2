#pragma once
#ifdef _DEV_VER
#include "uiBase.h"
#include "FTextRender.h"
//#include "Achievement.h"
#include "NPCInfoParser.h"
#include <SCItemSlotContainer.h>
#include "VScrollDialog.h"
#include "uiGMWindowMan/uiGMWindowMan.h"

#define SEARCH_FRAME_UPDATE_TIME 100
#define NUMBER_OF_SEARCHING_PER_A_FRAME 200
#define MAX_VISIBLE_FINDLIST_NUM	6

struct NpcListInfo
{
    MONSTERCODE		m_MonsterCode;
    char			m_debug_name[MAX_MONSTERNAME_LENGTH];//디버그 네임
    LEVELTYPE		m_Level;
    LEVELTYPE		m_DisplayLevel;
    DWORD			m_dwMaxHP;
    DWORD			m_dwMaxMP;
    DWORD			m_dwMaxSD;
    CODETYPE		m_NCode;						// 네임코드
    BYTE			m_byGrade;						// 보스/일반
    WORD			m_wClass;						// 몬스터 계열
    NpcElementWeakness element_weakness;
#ifdef _DH_NPC_GROUP_NAME
    DWORD group_name; //그룹네임코드
#endif//_DH_NPC_GROUP_NAME
};

class uiGMWindowMan;
class uiGMNpcWindow : public uiBase, public VScrollDialog, public SCItemSlotContainer
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
        kControl_Button_B001,
        kControl_Button_B002,
        kControl_Button_B012,
        kControl_Button_B003,
        kControl_Button_B011,
        kControl_Button_B013,
        kControl_Button_B014,
        kControl_Button_B015,
        kControl_Button_B016,
        kControl_Text_S000,
        kControl_Text_S002,
        kControl_Text_S008,
        kControl_Edit_E000,
        kControl_List_L000,
        kControl_List_L001,
        kControl_List_L002,
        kControl_VScroll_V001,
        kControl_VScroll_V000,
        kControl_Picture_P003,
        kControl_Picture_P002,
        kControl_Picture_P000,
        kControl_Picture_P001,
        kControl_Picture_P004,
        kControl_Picture_P005,
        kControl_Picture_P012,
        kControl_Picture_P011,
        kControl_Picture_P006,
        kControl_Size,
    };

    enum
    {
        kDisplay_Item_Info_Num = 20,
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
        kFind_From_Name, //이름 검색
        kFind_From_Description, //설명(디버그용이름) 검색
        kFind_From_ItemType, //아이템 타입 검색
        kFind_From_WasteType, //소모 타입 검색(WasteType)
        kFind_Type_MAX,
    };

    enum
    {
        kNpc_ID,
        //땅불바람물 색상아이콘을 이용해보자
        kNpc_Type,//정예 럭키... <-> 몬스터, 엔피시, 플레이어
        kNpc_State,//hp/mp/sd <-> damage
        kNpc_Name,//이름 <-> 디버그네임
        kNpc_lv,//표시레벨 <-> 실제레벨
        kNpc_Location, //거주지역<-> 공격타입
        kNpc_Max,
    };

    enum
    {
        kGradeLevel4 = 4,
    };
public:
    uiGMNpcWindow(InterfaceManager* ui_manager);
    virtual ~uiGMNpcWindow(void);

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
    void CreateNpc(BASE_NPCINFO* npc_info, ITEM_PARAMETAS* item_parameta, BYTE create_type = ITEM_TYPE_NORMAL, BYTE lucky_expiry = 0);
    void CreateItemFromList();
    void RefreshList(BYTE state, DWORD search_count, DWORD find_size);
    BYTE item_window_state(){ return item_window_state_; }
    void RefreshItemInfo();
    DWORD last_summon_npc_code(){ return last_summon_npc_code_; }

protected:
    virtual CCtrlVScrollWZ* vscroll_control();
    virtual bool GetScrollArea(RECT& area);
    virtual bool OnVScrollDown(SI_MESSAGE* msg);
    virtual bool OnVScrollUp(SI_MESSAGE* msg);
    virtual bool OnVScrollThumb(SI_MESSAGE* msg);

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
    void SetNpcDataString(int index_display, BASE_NPCINFO* npc_info);
    void SetRectItemControl(int index_display, bool enable_reward);
    void RenderItemInfoText(DWORD index_display);
    static void CallBackRenderItemInfo(CControlWZ* control_ptr, void* parameter);
    void ShowControl(POSTYPE control_pos, bool is_show);
    void ToggleShowControl(POSTYPE control_pos);
    void SetSearchProgressData(DWORD search_count, DWORD find_count);
    void UpdateToolTip();
    //void SetItemSlot(int index_display, BASE_NPCINFO* item_info);
    //bool AddItem(POSTYPE at_pos, SLOTCODE item_code);

    void ChangeFindIconImage(CControlWZ* control, bool is_searching);
    void RegisterESCKeyClose(bool is_value);
    static void ESCKeyProcess(DWORD wParam,DWORD lParam);

    bool GetFilterEquip(BASE_ITEMINFO* item_infomation, BYTE sub_filter);
    bool GetFilterWaste(BASE_ITEMINFO* item_infomation, BYTE sub_filter);
    bool GetFilterGrade(BASE_ITEMINFO* item_infomation, BYTE sub_filter);
    bool GetFilterEtc(BASE_ITEMINFO *item_infomation, BYTE sub_filter);
    bool GetFilterResult(DWORD item_code);
    void LoadCreateHistory();
    std::vector<DWORD>* GetFocusList();
    DWORD GetFocusData(DWORD index);
    void SetItemWindowState(BYTE state);
    void RefreshPreFindList(TCHAR* find_string);
    void SetCurrentScrollPosition(int current_scroll_pos);
    void SetNpcDataList(int index_display, NpcListInfo* npc_list_info);
    void ListInit();

private:
    uiGMWindowMan* ui_gm_window_manager_;
    CCtrlDxEditBox* edit_control_;
    CCtrlStaticWZ* static_edit_control_;
    CCtrlButtonWZ* static_state_control_;
    CCtrlListWZ* find_list_ptr_;
    CCtrlButtonWZ* button_control_;
    CCtrlButtonWZ* history_button_;
    CCtrlButtonWZ* monster_kill_button_;
    CCtrlButtonWZ* reset_button_;
    CCtrlButtonWZ* button_search_type_text_control_;
    std::vector<FTextRender> item_info_text_render_;
    FTextRender info_detail_text_;

    NPC_NAME_INFO::iterator npc_name_info_iterator_;
    NPC_NAME_INFO::iterator npc_name_info_iterator_end_;

    std::vector<NpcListInfo> npc_list_;

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
    DWORD last_summon_npc_code_;

    int history_scroll_pos_;
    int search_scroll_pos_;
    int bookmark_scroll_pos_;

    std::vector<DWORD>* searched_results_;
    std::vector<DWORD>* created_histroys_;
    std::vector<DWORD>* bookmark_lists_;
};

inline std::vector<DWORD>* uiGMNpcWindow::GetFocusList()
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

inline DWORD uiGMNpcWindow::GetFocusData(DWORD index)
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

inline void uiGMNpcWindow::ShowControl(POSTYPE control_pos, bool is_show)
{
    CControlWZ* control = GetControlWZ(GetControlID_byPosition(control_pos));
    if (control != NULL)
    {
        control->ShowWindowWZ((is_show == true) ? WZ_SHOW : WZ_HIDE);
    }
}

inline void uiGMNpcWindow::ToggleShowControl(POSTYPE control_pos)
{
    CControlWZ* control = GetControlWZ(GetControlID_byPosition(control_pos));
    if (control != NULL)
    {
        ENUM_STATEWZ control_state = control->GetState();
        control->ShowWindowWZ((control_state == WZ_HIDE) ? WZ_SHOW : WZ_HIDE);
    }
}
#endif//_DEV_VER 