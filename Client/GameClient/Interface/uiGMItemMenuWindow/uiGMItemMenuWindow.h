#pragma once
#ifdef _DEV_VER
#include "uiBase.h"

class uiGMWindowMan;
class uiGMItemMenuWindow : public uiBase
{
public:
    enum
    {
        kMenu_Create_Setting,    // ���� (����������)
        kMenu_Create_Normal,     // �Ϲ� ����
        kMenu_Create_Ethria ,    // ���׸��� ����
        kMenu_Create_Skin,       // ��Ų ����
        kMenu_Create_Socket,     // ���� ����
        kMenu_Create_NoSocket,   // ���� ���� ����
        kMenu_Create_Limited,    // ����Ƽ�� ����
        kMenu_Create_Lucky,      // ��Ű ����
        kMenu_Create_Visible_All,// ���� ����
        kMenu_Add_Bookmark,      // ���ã�� �߰�
        kMenu_Delete_Bookmark,   // ���ã�� ����

        kMenu_Lucky_1_Hour,   // ��Ű ������ �Ⱓ 1�ð�
        kMenu_Lucky_30_Day,   // ��Ű ������ �Ⱓ 30��
        kMenu_Lucky_60_Day,   // ��Ű ������ �Ⱓ 60��
        kMenu_Lucky_90_Day,   // ��Ű ������ �Ⱓ 90��
        kMenu_Lucky_Finite,   // ��Ű ������ �Ⱓ ����
        kMenu_Lucky_Infinite,   // ��Ű ������ �Ⱓ ������

        kItem_Menu_MAX
    };

    enum UIControlPos
    {
        kControl_Button_L001,
        kControl_Size
    };

    uiGMItemMenuWindow(InterfaceManager* ui_manager);
    virtual ~uiGMItemMenuWindow(void);

    virtual void Init(CDrawBase* drawbase);
    virtual void Release();

    virtual void MessageProc(SI_MESSAGE* message);
    virtual void OnShowWindow(BOOL val);

    void UpdateMenus(BASE_ITEMINFO* item_info, bool is_bookmark_state);
    void UpdateMenuList(int mouse_x, int mouse_y);
    void UpdateLuckyMenus();

private:
    void RegisterControl();
    void OnLButtonClick(SI_MESSAGE* message);
    bool ProcessMenu(BYTE menu_position);

private:
    uiGMWindowMan* ui_gm_window_manager_;
    CCtrlListWZ* list_control_;
    ITEM_PARAMETAS item_option_parametas_;

    std::vector<BYTE> menu_list;
    CODETYPE selected_item_code_; 
    //BYTE popup_menus[kItem_Menu_MAX];
};
#endif//_DEV_VER