#include "SunClientPrecompiledHeader.h"
#pragma once
#ifdef _NA_006800_20130703_BATTLE_GROUND_VOTE_SYSTEM

#include "uiBase.h"

class uiChaosZoneVote : public uiBase
{
    enum eDIALOG_POS
    {
        kControl_Title_Text = 0,    // 제목
        kControl_Message_Text,      // 내용
        kControl_Confirm_Button,    // 예
        kControl_Rejection_Button,  // 아니오

        kControl_Close_Button,      // 닫기
        kControl_VScroll,           // 스크롤
        kControl_VScroll_Picture,   // 스크롤이미지
        kControl_Verify_Button,     // 확인버튼

        kControl_Size,
    };

public:
    uiChaosZoneVote(InterfaceManager *ui_man);
    virtual ~uiChaosZoneVote();

    virtual void Init(CDrawBase* draw_base);
    virtual void Release();
    virtual VOID Process(DWORD tick);
    virtual bool ShowInterface(BOOL val, BOOL showoff = FALSE);

    virtual void NetworkProc(MSG_BASE* /*pMsg*/) {};  // Pure
    virtual void MessageProc(SI_MESSAGE* pMessage);   // Pure
    
    bool OnLButtonClick(SI_MESSAGE* message);

    inline
    void set_character_name(TCHAR* character_name)
    {
        StrnCopy(character_name_, character_name, MAX_CHARNAME_LENGTH);
    }

    TCHAR* character_name() { return character_name_; }

    void set_vote_remain_time(DWORD remain_time);

protected:
    virtual void OnShowWindow(BOOL val);
    virtual void OnRenderSolarDialog();
    virtual void OnUpdateSolarDialog();

private:

    DWORD current_time_; // 현재 시간
    DWORD vote_end_time_; // 투표 종료 시간

    TCHAR character_name_[MAX_CHARNAME_LENGTH + 1];
};


inline
void uiChaosZoneVote::set_vote_remain_time(DWORD remain_time)
{
    current_time_ = clock_function::GetTickCount();
    vote_end_time_ = remain_time + clock_function::GetTickCount();
}
//------------------------------------------------------------------------------

#endif //_NA_006800_20130703_BATTLE_GROUND_VOTE_SYSTEM
