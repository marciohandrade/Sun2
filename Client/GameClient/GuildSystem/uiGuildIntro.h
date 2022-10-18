#pragma once
#ifdef _NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL
#include "uiBase.h"
#include "VScrollDialog.h"
#include "FTextRender.h"

class uiGuildIntro : public uiBase, public VScrollDialog
{
    enum
    {
        kTab_Notice = 0,
        kTab_Invite,
        kTab_Max,
    };
    enum UIControlPos
    {
        kControl_CheckBT_C000 = 0,
        kControl_CheckBT_C001,
        kControl_CheckBT_C003,
        kControl_CheckBT_C004,
#ifdef _NA_008334_20150608_SONNENSCHEIN
        kControl_CheckBT_C008,
#endif //_NA_008334_20150608_SONNENSCHEIN
        kControl_CheckBT_C002,

        kControl_CheckBT_C005,
        kControl_CheckBT_C006,

        kControl_Button_B005,
        kControl_Button_B006,
        kControl_Button_B009,
        kControl_Button_B000,
        kControl_Text_S000,
        kControl_Text_S001,
        kControl_List_L000,
        kControl_VScroll_V000,
        kControl_Edit_E000,

        kControl_Picture_P007, // 길드경험치 게이지
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
        kControl_Button_B001,  // 가입요청 목록
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION

        kControl_Size,
    };

public:
    static const DWORD kDialog_ID;

public:
    uiGuildIntro(InterfaceManager* ui_manager);
    virtual ~uiGuildIntro();

    virtual void Init(CDrawBase* draw_base);
    virtual void Release();

    virtual void MessageProc(SI_MESSAGE* message);
    virtual void NetworkProc(MSG_BASE* recv_packet);

    virtual void OnShowWindow(BOOL is_show);
    virtual void OnUpdateSolarDialog();

    void SetEditMode(bool is_mode);
    void SetDeleteMessage(bool force_delete = false);
protected:
    CCtrlVScrollWZ* vscroll_control();
    bool GetScrollArea(RECT& area);
    bool OnVScrollThumb(SI_MESSAGE* msg);
    bool OnVScrollUp(SI_MESSAGE* msg);
    bool OnVScrollDown(SI_MESSAGE* msg);
private:
    bool OnLButtonClick(SI_MESSAGE* message);
    bool OnRButtonClick(SI_MESSAGE* message);
    bool OnMouseOver(SI_MESSAGE* message);
    //추가 함수
    void ShowButton(DWORD control_id, BOOL is_show);
    void ShowGuildInOutButton();
    void ShowWriteButton();
    void RefreshTab();
    void SetGuildNotice(TCHAR* string);
    void RefreshGuildNotice();
    bool WriteNotice(TCHAR* message);
    void UpdateGuildCoin();
    void UpdateGuildEXPGauge(float gauge_rate);
    void ChangeWriteButton(bool is_mode);
    void ChangeStateTabButton(int first_tab_num);
    void SelectStateTabButton(DWORD control_pos);
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
public:
    void GetGuildIntro();
private:
#else
    void GetGuildIntro();
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION

    float gauge_control_width() const { return gauge_control_width_; }
    void set_control_gauge_width(float value) { gauge_control_width_ = value; }
    float gauge_texture_width() const { return gauge_texture_width_; }
    void set_gauge_texture_width(float value) { gauge_texture_width_ = value; }

    bool is_edit_mode_;
    TCHAR now_notice_string_[MAX_GUILDNOTICE_LENGTH+1];
    WCHAR now_notice_wstring_[MAX_GUILDNOTICE_LENGTH+1];
    std::vector<util::_tstring>	guild_notice_string_;

    TCHAR now_invite_string_[MAX_GUILDNOTICE_LENGTH+1];
    WCHAR now_invite_wstring_[MAX_GUILDNOTICE_LENGTH+1];
    std::vector<util::_tstring>	guild_invite_string_;

    FTextRender	ftext_render_;
    int first_tab_num_;
    int is_select_tab_;
    float gauge_control_width_;
    float gauge_texture_width_;
};

#endif //_NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL
