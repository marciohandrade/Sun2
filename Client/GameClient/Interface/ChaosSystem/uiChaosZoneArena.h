#include "SunClientPrecompiledHeader.h"

#pragma once
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#include "uiBase.h"

class uiChaosZoneArena : public uiBase
{
    enum UIControlPos
    {
        kControl_Size,
    };

public:
    static const DWORD kDialog_ID;

public:
    uiChaosZoneArena(InterfaceManager* ui_manager);
    virtual ~uiChaosZoneArena();

    virtual void Init(CDrawBase* draw_base);
    virtual void Release();

    virtual void MessageProc(SI_MESSAGE* message);
    virtual void NetworkProc(MSG_BASE* recv_packet);
    virtual void OnRenderSolarDialog();
private:
    bool OnLButtonClick(SI_MESSAGE* message);
    bool OnRButtonClick(SI_MESSAGE* message);
    bool OnMouseOver(SI_MESSAGE* message);
    void RenderTooltip();
};

#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
