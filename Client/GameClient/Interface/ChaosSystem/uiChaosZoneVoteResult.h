#include "SunClientPrecompiledHeader.h"
#pragma once
#ifdef _NA_006800_20130703_BATTLE_GROUND_VOTE_SYSTEM

#include "uiBase.h"

class uiChaosZoneVoteResult : public uiBase
{
    enum eDIALOG_POS
    {
        kControl_Title_Text = 0,    // 제목
        kControl_Message_Text,      // 내용

        kControl_Size,
    };

public:
    uiChaosZoneVoteResult(InterfaceManager *ui_man);
    virtual ~uiChaosZoneVoteResult();

    virtual void Init(CDrawBase* draw_base);
    virtual void Release();
    virtual VOID Process(DWORD tick);
    virtual bool ShowInterface(BOOL val, BOOL showoff = FALSE);

    virtual void NetworkProc(MSG_BASE* /*pMsg*/) {};  // Pure
    virtual void MessageProc(SI_MESSAGE* pMessage);   // Pure


    inline
    void set_character_name(TCHAR* character_name)
    {
        StrnCopy(character_name_, character_name, MAX_CHARNAME_LENGTH);
    }
    TCHAR* character_name() { return character_name_; }

    BYTE vote_yes_count() { return vote_yes_count_; }
    void set_vote_yes_count(BYTE count) { vote_yes_count_ = count; }

    BYTE vote_no_count() { return vote_no_count_; }
    void set_vote_no_count(BYTE count) { vote_no_count_ = count; }

    void set_vote_remain_time(DWORD remain_time);

protected:
    virtual void OnShowWindow(BOOL val);
    virtual void OnRenderSolarDialog();
    virtual void OnUpdateSolarDialog();

private:

    DWORD current_time_; // 현재 시간
    DWORD vote_end_time_; // 투표 종료 시간

    BYTE vote_yes_count_; // 찬성 투표 수
    BYTE vote_no_count_;  // 반대 투표 수

    TCHAR character_name_[MAX_CHARNAME_LENGTH + 1];
};

inline
void uiChaosZoneVoteResult::set_vote_remain_time(DWORD remain_time)
{
    current_time_ = clock_function::GetTickCount();
    vote_end_time_ = remain_time + clock_function::GetTickCount();
}
//------------------------------------------------------------------------------

#endif //_NA_006800_20130703_BATTLE_GROUND_VOTE_SYSTEM