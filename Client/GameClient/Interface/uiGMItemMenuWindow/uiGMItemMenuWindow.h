#pragma once
#ifdef _DEV_VER
#include "uiBase.h"

class uiGMWindowMan;
class uiGMItemMenuWindow : public uiBase
{
public:
    enum
    {
        kMenu_Create_Setting,    // 생성 (설정값으로)
        kMenu_Create_Normal,     // 일반 생성
        kMenu_Create_Ethria ,    // 에테리아 생성
        kMenu_Create_Skin,       // 스킨 생성
        kMenu_Create_Socket,     // 소켓 생성
        kMenu_Create_NoSocket,   // 소켓 없이 생성
        kMenu_Create_Limited,    // 리미티드 생성
        kMenu_Create_Lucky,      // 럭키 생성
        kMenu_Create_Visible_All,// 연속 생성
        kMenu_Add_Bookmark,      // 즐겨찾기 추가
        kMenu_Delete_Bookmark,   // 즐겨찾기 삭제

        kMenu_Lucky_1_Hour,   // 럭키 아이템 기간 1시간
        kMenu_Lucky_30_Day,   // 럭키 아이템 기간 30일
        kMenu_Lucky_60_Day,   // 럭키 아이템 기간 60일
        kMenu_Lucky_90_Day,   // 럭키 아이템 기간 90일
        kMenu_Lucky_Finite,   // 럭키 아이템 기간 만료
        kMenu_Lucky_Infinite,   // 럭키 아이템 기간 무제한

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