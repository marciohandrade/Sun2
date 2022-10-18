#include "SunClientPrecompiledHeader.h"

#pragma once
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#include "uiBase.h"

class uiChaosZoneRankBattle : public uiBase
{
    enum UIControlPos
    {
        kControl_Size,
    };

public:
    static const DWORD kDialog_ID;

public:
    uiChaosZoneRankBattle(InterfaceManager* ui_manager);
    virtual ~uiChaosZoneRankBattle();

    virtual void Init(CDrawBase* draw_base);
    virtual void Release();

    virtual void MessageProc(SI_MESSAGE* message);
    virtual void NetworkProc(MSG_BASE* recv_packet);
private:
    bool OnLButtonClick(SI_MESSAGE* message);
    bool OnRButtonClick(SI_MESSAGE* message);
    bool OnMouseOver(SI_MESSAGE* message);
};

#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
