#pragma once

#ifdef _NA_003027_20111013_HONOR_SYSTEM

#include "uiBase.h"
#include "TreeMenuGroup.h"

class uiCharacterStatusTitleList : public uiBase
{
    enum UIControlPos
    {
        kControl_Button_B004 = 0,   // �ݱ� ��ư
        kControl_Text_S000,         // Īȣ �ؽ�Ʈ
        kControl_List_L000,         // Īȣ ����Ʈ
        kControl_VScroll_V000,      // ��ũ��

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

    // Īȣ ����Ʈ ������ �Լ�
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
