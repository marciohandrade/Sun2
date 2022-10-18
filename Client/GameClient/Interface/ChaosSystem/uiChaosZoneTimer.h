#include "SunClientPrecompiledHeader.h"
#pragma once
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND

#include "uiBase.h"

class uiChaosZoneTimer : public uiBase
{
    enum UIControlPos
    {
        kControl_Picture_P000,
        kControl_Picture_P001,
        kControl_Text_S000,
        kControl_Text_S001,
        kControl_Size,
    };

public:
    uiChaosZoneTimer(InterfaceManager *ui_man);
    virtual ~uiChaosZoneTimer();

    virtual void Init(CDrawBase* draw_base);
    virtual void Release();
    virtual VOID Process(DWORD tick);
    virtual bool ShowInterface(BOOL val, BOOL showoff = FALSE);

    virtual void NetworkProc(MSG_BASE* /*pMsg*/) {};  // Pure
    virtual void MessageProc(SI_MESSAGE* pMessage);   // Pure

    void RegisterControl();
    void ShowChaosZoneResult(ChaosZoneResultType result_type);

protected:
    virtual void OnShowWindow(BOOL val);
    virtual void OnRenderSolarDialog();
    virtual void OnUpdateSolarDialog();

private:
    util::Timer show_dialog_timer_;
};




#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND