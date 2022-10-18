#pragma once
#ifdef _NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL
#include "uiBase.h"
#include "TabCheckButtonControlManager.h"

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
class uiToolTipMan;
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
class uiGuildInfo : public uiBase
{
    enum UIControlPos
    {
        kControl_TextImage_TI02 = 0,// 길드연합
        kControl_TextImage_I006,    // 길드명
        kControl_TextImage_I007,    // 길드마스터
        kControl_TextImage_I008,    // 길드랭킹
        kControl_TextImage_I009,    // 길드레벨
        kControl_TextImage_I010,    // 길드멤버수

        kControl_CheckBT_C000,      // 길드가입요청차단체크버튼
        kControl_Text_S004,         // 길드가입요청차단
        kControl_Text_S005,         // 길드코인

        kControl_Button_B999,       // 닫기
        kControl_Button_BT01,       // 코인기부 및 가입요청

        kControl_Picture_PI00,      // 길드마크        

        kControl_Size,
    };

public:
    enum TabDialog
    {
        kGuildIntro = 0,
        kGuildMember, 
        kGuildInstitution, 
        kGuildManagement, 
#ifdef _NA_008334_20150608_SONNENSCHEIN
        kGuildPvE, 
#endif //_NA_008334_20150608_SONNENSCHEIN
        kGuildLog, 

        kTab_Max,
    };

    static const DWORD kDialog_ID;

public:
    uiGuildInfo(InterfaceManager* ui_manager);
    virtual ~uiGuildInfo();

    virtual void Init(CDrawBase* draw_base);
    virtual void Release();
    virtual void MessageProc(SI_MESSAGE* message);
    virtual void OnShowWindow(BOOL is_show);
    virtual void OnUpdateSolarDialog();
    virtual void OnRenderSolarDialog();

    void InitTabController();
    void MoveSubDialog();

    bool show_flag() const { return show_flag_; }
    void set_show_flag(bool value) { show_flag_ = value; }

    TabCheckButtonControllManager& tab_button_controller() { return tab_button_controller_; }

private:
    bool OnLButtonClick(SI_MESSAGE* message);
    void OnClickDonateGuildCoin();

    void UpdateGuildInfo();
    void UpdateButtonState();

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    void RenderTooltip();
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION

private:
    bool show_flag_;    

    TabCheckButtonControllManager tab_button_controller_;
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    uiToolTipMan* tooltip_manager_ptr_;
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
};

#endif //_NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL