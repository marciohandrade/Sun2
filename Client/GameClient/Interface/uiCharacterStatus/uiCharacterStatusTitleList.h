#pragma once

#ifdef _NA_003027_20111013_HONOR_SYSTEM

#include "uiBase.h"
#include "TreeMenuGroup.h"

class uiCharacterStatusTitleList : public uiBase
{
    enum UIControlPos
    {
        kControl_Button_B004 = 0,   // 닫기 버튼
        kControl_Text_S000,         // 칭호 텍스트
        kControl_List_L000,         // 칭호 리스트
        kControl_VScroll_V000,      // 스크롤

        kControl_Size,
    };
public:
    static const DWORD kDialog_ID;

public:
    uiCharacterStatusTitleList(InterfaceManager* interface_manager);
    ~uiCharacterStatusTitleList();

    virtual void Init(CDrawBase* draw_base);
    virtual void Release();
    virtual void OnShowWindow(BOOL is_show);
    virtual void MessageProc(SI_MESSAGE* message);

    void OnChangeTitle(WORD title_code);

    WORD confirm_title_code() const { return confirm_title_code_; }
    DWORD reset_confirm_dialog_key() { return confirm_dialog_key_ = 0; }

    // 칭호 리스트 렌더링 함수
    static void Callback_TitleListMenuRender(CCtrlListWZ* list_control, 
        CCtrlListWZ::CListSellData* sell_data, 
        int sell_index, 
        RECT& render_area);

    static void CallBack_ConfirmUseTitle(bool is_yes);
 
private:
    virtual void OnUpdateSolarDialog();

    void InitTitleListInfo();

    void OnDClickToUseTitle(WORD title_code);
    bool OnLButtonClick(SI_MESSAGE* message);
    void OnClose();
    


    TreeMenuGroup& title_list_menu() { return title_list_menu_; }

private:
    TreeMenuGroup title_list_menu_;

    WORD prev_title_code_;
    WORD confirm_title_code_;
    DWORD confirm_dialog_key_;
};

#endif //_NA_003027_20111013_HONOR_SYSTEM
