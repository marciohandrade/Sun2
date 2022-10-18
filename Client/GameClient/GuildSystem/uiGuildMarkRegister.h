#pragma once
#ifdef _NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL
#include "uiBase.h"

class uiGuildMarkRegister : public uiBase
{
    enum UIControlPos
    {   
        kControl_Button_B000 = 0,   // 조합된 길드마크
        kControl_Button_B005,       // 확인 버튼
        kControl_Button_B006,       // 취소 버튼
        kControl_Button_B999,       // 닫기 버튼

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
