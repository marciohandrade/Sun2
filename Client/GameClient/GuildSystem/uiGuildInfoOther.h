#pragma once
#ifdef _NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL
#include "uiBase.h"
#include "TabCheckButtonControlManager.h"

class uiGuildInfoOther : public uiBase
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
    static const DWORD kDialog_ID;

public:
    uiGuildInfoOther(InterfaceManager* ui_manager);
    virtual ~uiGuildInfoOther();

    virtual void Init(CDrawBase* draw_base);
    virtual void Release();
    virtual void MessageProc(SI_MESSAGE* message);
    virtual void OnShowWindow(BOOL is_show);
    virtual void OnUpdateSolarDialog();
    virtual void OnRenderSolarDialog();

    void SetGuildInfo(GUILD_INFO& value);
    void MoveSubDialog();
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    void OnAnswerJoinRequest();
    void OnClickRequestJoin();
    void SetJoinRequestButton(const eGUILD_JOIN_REQUEST& guild_join_request_state);
    void RequestJoinButtonReset();
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION

    void set_guild_rank_index(int value) { guild_rank_index_ = value; }

private:
    bool OnLButtonClick(SI_MESSAGE* message);
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
#else
    void OnClickRequestJoin();
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION

    void UpdateGuildInfo();
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
#else
    void UpdateButtonState();
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION

    int guild_rank_index() const { return guild_rank_index_; }

private:
    bool show_flag_;
    int guild_rank_index_;
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    eGUILD_JOIN_REQUEST guild_join_request_state_;
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
};

#endif //_NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL