#include "SunClientPrecompiledHeader.h"
#pragma once
#ifdef _NA_007086_20140318_MONSTERBOOK

#include "uiBase.h"
#include "VScrollDialog.h"
#include "TreeMenuGroup.h"
#include "uiMonsterBookMan/uiMonsterBookMan.h"
//
// ���͵��� ui�� ������ �Ǿ��ְ� �ϳ��� �پ��ִ� ����̶� 3�� â�� �ٹ������ϴµ�,
// ui�� 70%�� ���� �����̿��� BaseŬ�������� ����� ������ ó���ϰ� ��ӹް���
//
//      ��������������������
//  ��1 ��Base  ����Viewâ��    31_1_mob_book.iwz
//      ��      ����      ��
//      ��������������������
//      ��������������������
//  ��2 ��Base  ����DropL ��    31_2_mob_book.iwz
//      ��      ����istâ ��
//      ��������������������
//      ��������������������
//  ��3 ��Base  ����Extra ��    31_3_mobbook.iwz (������)
//      ��      ���� ctâ ��
//      ��������������������
//
//
struct WindowState
{
    // �� �ѱ� �� ���� â�� ������ �Ѱܼ� ��������
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
    // 3�� â�� ����� ��Ʈ��
    // 3�� ui�� ����� ��Ʈ�� ID���� �����ؾ� �� (����� ����)
    enum UIControlPos
    {
        kControl_List_L001,    // ����Ʈ - �����޴�
        kControl_List_L005,    // ����Ʈ - ���͸���Ʈ
        kControl_VScroll_V002, // ��ũ��
        kControl_Edit_E000,    // �˻� ����Ʈ 
        kControl_Static_S019,  // �˻� �ؽ�Ʈ 
        kControl_Button_B086,  // �ݱ�
        kControl_Button_B002,  // �˻�

        kControl_Check_C000,   // ��1
        //kControl_Check_C001,
        kControl_Check_C002,   // ��2

        kControl_Text_S000,    // Ȯ�εȸ��� %s/%s

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
    void set_tap_control(BYTE index);   // �� ��Ʈ�� ����
    virtual void SetMonsterInfo(DWORD monster_code) = 0;    // �ǿ� �´� �������� ����
    void SelectFirstMonster();     // Ʈ���� �� �� ���͸� ����

    static HANDLE   handle_texture_;
    static HANDLE   handle_texture2_;
    static HANDLE   handle_active_texture_;
    static BOOL     character_first_show_;  // ĳ������ �� ó�� ����
    static DWORD    select_monster_code_;   // ���õ� ����(static)
    WindowState     current_window_state_;  // â ����

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

    TreeMenuGroup tree_menu_;           // ���� �����޴�
    TreeMenuGroup tree_monster_list_;   // �߾� ���͸���Ʈ
    BOOL is_search_mode_;
    static BOOL tree_loaded_;
    static DWORD    tooltip_montercode_;
#ifdef _SCJ_TOGGLE_EDITBOX_CONTROL
    ToggleEditBox* toggle_edit_box_;
#endif //_SCJ_TOGGLE_EDITBOX_CONTROL
};

#endif //_NA_007086_20140318_MONSTERBOOK
