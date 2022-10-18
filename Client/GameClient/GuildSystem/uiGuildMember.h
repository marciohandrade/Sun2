#pragma once
#ifdef _NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL
#include "uiBase.h"

class uiGuildMember : public uiBase
{
    enum UIControlPos
    {
        kControl_CheckBT_C000 = 0,  // 길드소개탭
        kControl_CheckBT_C001,      // 길드원정보탭
        kControl_CheckBT_C003,      // 길드시설탭
        kControl_CheckBT_C004,      // 길드운영탭
#ifdef _NA_008334_20150608_SONNENSCHEIN
        kControl_CheckBT_C006,      // 길드PvE
#endif //_NA_008334_20150608_SONNENSCHEIN
        kControl_CheckBT_C002,      // 길드로그탭

        kControl_Button_B000,       // 길드원 이름
        kControl_Button_B001,       // 레벨
        kControl_Button_B002,       // 클래스
        kControl_Button_B003,       // 직책
        kControl_Button_B004,       // 공헌도
        kControl_Button_B005,       // 접속여부

        kControl_Button_B006,       // 길드초대 버튼
        kControl_Button_B007,       // 길드제명 버튼
        kControl_Button_B008,       // 직책변경 버튼
        kControl_Button_B010,       // 길드탈퇴 버튼
        kControl_Button_B009,       // 길드장위임 버튼

        kControl_List_L000,         // 길드원정보 출력 리스트
        kControl_VScroll_V000,      // 스크롤

        kControl_CheckBT_C005,      // 오프라인 길드원 숨기기

        kControl_Size,
    };

public:
    static const DWORD kDialog_ID;

public:
    uiGuildMember(InterfaceManager* ui_manager);
    virtual ~uiGuildMember();

    virtual void Init(CDrawBase* draw_base);
    virtual void Release();
    virtual void MessageProc(SI_MESSAGE* message);
    virtual void OnShowWindow(BOOL is_show);
    virtual void OnUpdateSolarDialog();
    virtual void OnRenderSolarDialog();

    GUILD_MEMBER_INFO* GetSelectedMemberInfo();

private:
    bool OnLButtonClick(SI_MESSAGE* message);
    bool OnLButtonDClick(SI_MESSAGE* message);
    bool OnVScrollUp(SI_MESSAGE* message);
    bool OnVScrollDown(SI_MESSAGE* message);
    bool OnVScrollThumb(SI_MESSAGE* message);

    void OnClickInvateGuildMember();
    void OnClickDutyVest();
    void OnClickDelegateMaster();

    void UpdateMemberInfo();
    void UpdateButtonState();
    void UpdateWheelScroll();
    void RefreshTab();

private:

};

#endif //_NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL