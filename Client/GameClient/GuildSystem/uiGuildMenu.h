#pragma once
#ifdef _NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL
#include "uiBase.h"

class uiGuildMenu : public uiBase
{
    enum UIControlPos
    {
        kControl_Button_B001 = 0, // 길드창설/해산/해산해제
        kControl_Button_B028,     // 길드랭킹
        kControl_Button_B011,     // 길드마크
        kControl_Button_B012,     // 길드창고
        kControl_Button_B004,     // 닫기
        kControl_Text_S002,       // 안내메시지

        kControl_Size,
    };

public:
    static const DWORD kDialog_ID;

public:
    uiGuildMenu(InterfaceManager* ui_manager);
    virtual ~uiGuildMenu();

    virtual void Init(CDrawBase* draw_base);
    virtual void Release();
    virtual void MessageProc(SI_MESSAGE* message);
    virtual void OnShowWindow(BOOL is_show);
    virtual void OnUpdateSolarDialog();

private:
    bool OnLButtonClick(SI_MESSAGE* message);
    void OnGuildCreateNDestroy();

    void UpdateGuildMenu();

};

#endif //_NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL
