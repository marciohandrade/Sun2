#include "SunClientPrecompiledHeader.h"
#pragma once
#ifdef _NA_007086_20140318_MONSTERBOOK

#include "uiBase.h"
#include "VScrollDialog.h"
#include "TreeMenuGroup.h"
#include "uiMonsterBookMan/uiMonsterBookMan.h"
//
// 몬스터도감 ui는 탭으로 되어있고 하나로 붙어있는 방식이라서 3개 창이 근무교대하는데,
// ui의 70%는 같은 내용이여서 Base클래스에서 공통된 내용을 처리하고 상속받게함
//
//      ┌───┐┌───┐
//  탭1 │Base  ││View창│    31_1_mob_book.iwz
//      │      ││      │
//      └───┘└───┘
//      ┌───┐┌───┐
//  탭2 │Base  ││DropL │    31_2_mob_book.iwz
//      │      ││ist창 │
//      └───┘└───┘
//      ┌───┐┌───┐
//  탭3 │Base  ││Extra │    31_3_mobbook.iwz (없어짐)
//      │      ││ ct창 │
//      └───┘└───┘
//
//
struct WindowState
{
    // 탭 넘길 때 현재 창의 정보를 넘겨서 세팅해줌
    int window_pos_x_;
    int window_pos_y_;
    DWORD menu_select_;
    TreeMenuGroup* list_tree_;
    int list_scroll_pos_;
    DWORD select_monster_;
    TCHAR search_string_[MAX_PATH];

    WindowState()
    {
        window_pos_x_ = -1;
        window_pos_y_ = -1;
        menu_select_ = 0;
        list_tree_ = NULL;
        list_scroll_pos_ = 0;
        select_monster_ = 0;
        ZeroMemory(search_string_, MAX_PATH);
    }
};

class ToggleEditBox;
class uiMonsterBookBase : public uiBase, public VScrollDialog
{
public:
    // 3개 창의 공통된 컨트롤
    // 3개 ui에 공통된 컨트롤 ID값이 동일해야 함 (변경시 주의)
    enum UIControlPos
    {
        kControl_List_L001,    // 리스트 - 지역메뉴
        kControl_List_L005,    // 리스트 - 몬스터리스트
        kControl_VScroll_V002, // 스크롤
        kControl_Edit_E000,    // 검색 에디트 
        kControl_Static_S019,  // 검색 텍스트 
        kControl_Button_B086,  // 닫기
        kControl_Button_B002,  // 검색

        kControl_Check_C000,   // 탭1
        //kControl_Check_C001,
        kControl_Check_C002,   // 탭2

        kControl_Text_S000,    // 확인된몬스터 %s/%s

        kControl_Size,
    };

public:
    uiMonsterBookBase();
    virtual ~uiMonsterBookBase();

    virtual void Init(CDrawBase* draw_base);
    virtual void Release();
    virtual void OnShowWindow(BOOL is_show);
    virtual void MessageProc(SI_MESSAGE* message);
    virtual void NetworkProc(MSG_BASE* recv_packet);
    virtual void OnRenderSolarDialog();
    virtual void OnUpdateSolarDialog();

    void SetWindowState(WindowState& window_state);
    WindowState& GetWindowState() { return current_window_state_; }
    void RefreshMonsterCount();
    void set_character_first_show(BOOL is_bool) { character_first_show_ = is_bool; }
    void SetMonsterList(uiMonsterBookMan::MonsterBook_Group* monster_group);
    void set_is_search_mode(BOOL is_bool) { is_search_mode_ = is_bool; }
    BOOL is_search_mode() { return is_search_mode_; 
    }
protected:
    void set_tap_control(BYTE index);   // 탭 컨트롤 세팅
    virtual void SetMonsterInfo(DWORD monster_code) = 0;    // 탭에 맞는 몬스터정보 세팅
    void SelectFirstMonster();     // 트리의 맨 앞 몬스터를 선택

    static HANDLE   handle_texture_;
    static HANDLE   handle_texture2_;
    static HANDLE   handle_active_texture_;
    static BOOL     character_first_show_;  // 캐릭접속 후 처음 열때
    static DWORD    select_monster_code_;   // 선택된 몬스터(static)
    WindowState     current_window_state_;  // 창 정보

private:
    bool OnLButtonClick(SI_MESSAGE* message);
    void RenderTooltip();
    CCtrlVScrollWZ* vscroll_control();
    bool GetScrollArea(RECT& area);
    bool OnVScrollThumb(SI_MESSAGE* msg);
    bool OnVScrollUp(SI_MESSAGE* msg);
    bool OnVScrollDown(SI_MESSAGE* msg);

    TreeMenuGroup& tree_menu() { return tree_menu_; }
    TreeMenuGroup& tree_monster_list() { return tree_monster_list_; }
    void InitMonsterBookTree();
    bool ClickTreeMenu(TreeMenuGroup::TreeObject* tree_object);
    bool ClickMonsterList(TreeMenuGroup::TreeObject* tree_object);
    void OnSearch();

    static void Callback_MonsterListRender(CCtrlListWZ* list_control,
        CCtrlListWZ::CListSellData* sell_data,
        int sell_index,
        RECT& render_area);

    TreeMenuGroup tree_menu_;           // 좌측 지역메뉴
    TreeMenuGroup tree_monster_list_;   // 중앙 몬스터리스트
    BOOL is_search_mode_;
    static BOOL tree_loaded_;
    static DWORD    tooltip_montercode_;
#ifdef _SCJ_TOGGLE_EDITBOX_CONTROL
    ToggleEditBox* toggle_edit_box_;
#endif //_SCJ_TOGGLE_EDITBOX_CONTROL
};

#endif //_NA_007086_20140318_MONSTERBOOK
