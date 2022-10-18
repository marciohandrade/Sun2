#pragma once
#ifdef _NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL
#include "uiBase.h"

class uiGuildAuthority : public uiBase
{
    enum UIControlPos
    {
        kControl_CheckBT_C000 = 0,  // 권한 체크
        kControl_CheckBT_C004,
        kControl_CheckBT_C005,
        kControl_CheckBT_C006,
        kControl_CheckBT_C007,
        kControl_CheckBT_C008,
        kControl_CheckBT_C009,
        kControl_CheckBT_C010,
        kControl_CheckBT_C011,
        kControl_CheckBT_C012,
        kControl_CheckBT_C013,
        kControl_CheckBT_C014,
        kControl_CheckBT_C015,
        kControl_CheckBT_C016,
        kControl_CheckBT_C017,
        kControl_CheckBT_C018,
        kControl_CheckBT_C019,
        kControl_CheckBT_C020,
        kControl_CheckBT_C021,
        kControl_CheckBT_C022,
        kControl_CheckBT_C023,
        kControl_CheckBT_C024,
        kControl_CheckBT_C025,
        kControl_CheckBT_C026,
        
        kControl_Text_S002,         // 권한이름
        kControl_Text_S003,
        kControl_Text_S004,
        kControl_Text_S005,
        kControl_Text_S006,
        kControl_Text_S007,
        kControl_Text_S008,
        kControl_Text_S009,
        kControl_Text_S010,
        kControl_Text_S011,
        kControl_Text_S012,
        kControl_Text_S013,
        kControl_Text_S014,
        kControl_Text_S015,
        kControl_Text_S016,
        kControl_Text_S017,
        kControl_Text_S018,
        kControl_Text_S019,
        kControl_Text_S020,
        kControl_Text_S021,
        kControl_Text_S022,
        kControl_Text_S023,
        kControl_Text_S024,
        kControl_Text_S025,

        kControl_Text_S001,         // 직책명

        kControl_Button_B001,       // 확인
        kControl_Button_B004,       // 닫기

        kControl_VScroll_V000,      // 스크롤

        kControl_Size,
    };

    enum
    {
        kAuthorityNumberByLine = 2,
        kMaxLineNumber = 12,
    };

public:
    static const DWORD kDialog_ID;

public:
    uiGuildAuthority(InterfaceManager* ui_manager);
    virtual ~uiGuildAuthority();

    virtual void Init(CDrawBase* draw_base);
    virtual void Release();
    virtual void MessageProc(SI_MESSAGE* message);
    virtual void OnShowWindow(BOOL is_show);
    virtual void OnUpdateSolarDialog();

    void set_guild_duty(eGUILD_DUTY value) { guild_duty_ = value; }

private:
    bool OnLButtonClick(SI_MESSAGE* message);
    bool OnVScrollUp(SI_MESSAGE* message);
    bool OnVScrollDown(SI_MESSAGE* message);
    bool OnVScrollThumb(SI_MESSAGE* message);

    void InitAuthorityInfo();
    void UpdateAuthorityInfo();

    eGUILD_DUTY guild_duty() const { return guild_duty_; }

private:
    eGUILD_DUTY guild_duty_;
};

#endif //_NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL