#pragma once
#ifdef _NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL
#include "uiBase.h"
#include "FTextRender.h"

class uiGuildPoint : public uiBase
{
    enum UIControlPos
    {
        kControl_Text_S005 = 0, // 소지 길드 코인
        kControl_Text_B000,     // 길드 코인 기부 안내

        kControl_Edit_E000,     // 기부 코인 입력 에디트

        kControl_Button_B001,   // 기부 버튼
        kControl_Button_B002,   // 취소
        kControl_Button_B999,   // 닫기

        kControl_Size,
    };

    enum
    {
        kMaxEditLength = 2,
    };

public:
    static const DWORD kDialog_ID;

public:
    uiGuildPoint(InterfaceManager* ui_manager);
    virtual ~uiGuildPoint();

    virtual void Init(CDrawBase* draw_base);
    virtual void Release();
    virtual void MessageProc(SI_MESSAGE* message);
    virtual void OnShowWindow(BOOL is_show);
    virtual void OnUpdateSolarDialog();
    virtual void OnRenderSolarDialog();

private:
    bool OnLButtonClick(SI_MESSAGE* message);
    bool OnEditEnterDown(SI_MESSAGE* message);
    void OnClickDonateButton();

    void UpdateCoinInfo();
    void InitEditControl();

    void SEND_CG_POINTWALLET_GUILDCOIN_DONATION_SYN(DWORD donation_code);

    FTextRender& donate_notice() { return donate_notice_; }
    DWORD donation_coin() const { return donation_coin_; }
    void set_donation_coin(DWORD value) { donation_coin_ = value; }
    DWORD currenct_coin() const { return currenct_coin_; }
    void set_currenct_coin(DWORD value) { currenct_coin_ = value; }

private:
    FTextRender donate_notice_;
    DWORD donation_coin_;
    DWORD currenct_coin_;
    
};

#endif //_NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL