#pragma once
#include "uiBase.h"
#include "Timer.h"

class uiHelpMan;

class uiHelpMissionTimer : public uiBase
{
    enum 
    {
        INVALID_TIMER_TYPE = 0xFF,
        INVALID_TIME = 0xFFFFFFFF,
    };

    enum eDIALOG_POS
    {
        DLGPOS_TIMER_TEXT,

        DIALOG_MAX,
    };

public:
    uiHelpMissionTimer(InterfaceManager *pUIMan);
    virtual ~uiHelpMissionTimer();

    virtual void Init(CDrawBase* pDrawBase);
    virtual void Release();

    bool IsActiveTimer()    {   return IsValidProgressType(progress_type_); }
    void SetMissionTimer(DWORD require_tIme, BYTE progress_type);

protected:
    virtual void OnShowWindow(BOOL val);
    virtual void OnUpdateSolarDialog();    

    bool IsValidProgressType(BYTE progress_type);
    void SetTimerText(DWORD current_tIme);

private:
    static WzID controls_id_[DIALOG_MAX];

    CCtrlStaticWZ* mission_timer_text_;
    uiHelpMan* ui_manager_;
    Timer mission_timer_;
    DWORD progress_tIme_;
    BYTE progress_type_;
};
