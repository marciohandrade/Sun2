#pragma once
#ifdef _NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL
#include "uiBase.h"

class uiGuildMarkRegister : public uiBase
{
    enum UIControlPos
    {   
        kControl_Button_B000 = 0,   // ���յ� ��帶ũ
        kControl_Button_B005,       // Ȯ�� ��ư
        kControl_Button_B006,       // ��� ��ư
        kControl_Button_B999,       // �ݱ� ��ư

        kControl_Size,
    };

public:
    static const DWORD kDialog_ID;

public:
    uiGuildMarkRegister(InterfaceManager* ui_manager);
    virtual ~uiGuildMarkRegister();

    virtual void Init(CDrawBase* draw_base);
    virtual void Release();
    virtual void MessageProc(SI_MESSAGE* message);
    virtual void OnShowWindow(BOOL is_show);
    virtual void OnRenderSolarDialog();

    void SetSelectedGuildMark(DWORD backgound_index, DWORD pattern_index);

private:
    bool OnLButtonClick(SI_MESSAGE* message);
    void OnClickRegisterButton();

    DWORD selected_background() const { return selected_background_; }
    void set_selected_background(DWORD value) { selected_background_ = value; }
    DWORD selected_pattern() const { return selected_pattern_; }
    void set_selected_pattern(DWORD value) { selected_pattern_ = value; }

private:
    DWORD selected_background_;
    DWORD selected_pattern_;
};

#endif //_NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL
