#include "SunClientPrecompiledHeader.h"

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#include "uiChaosZoneRankBattle.h"
#include "ChaosSystem/ChaosSystemMan.h"

const DWORD uiChaosZoneRankBattle::kDialog_ID = g_IDGenerator.GenerateID();

uiChaosZoneRankBattle::uiChaosZoneRankBattle(InterfaceManager* ui_manager)
{
}
//------------------------------------------------------------------------------
uiChaosZoneRankBattle::~uiChaosZoneRankBattle()
{
}
//------------------------------------------------------------------------------
void uiChaosZoneRankBattle::Init(CDrawBase* draw_base)
{
}
//------------------------------------------------------------------------------
void uiChaosZoneRankBattle::Release()
{
}
//------------------------------------------------------------------------------
void uiChaosZoneRankBattle::NetworkProc(struct MSG_BASE *message)
{
}
//------------------------------------------------------------------------------
void uiChaosZoneRankBattle::MessageProc(SI_MESSAGE* message)
{
    if (IsLock())
        return;

    if (GetMan() == NULL)
        return;

    switch (message->eResultMsg)
    {
    case RT_MSG_LBUTTONCLICK:
        OnLButtonClick(message);
        break;
    default:
        SolarDialog::MessageProc(message);
        break;
    }
}


//------------------------------------------------------------------------------
bool uiChaosZoneRankBattle::OnLButtonClick(SI_MESSAGE* message)
{
    bool result = true;
    /*switch (GetControlPosition_byID(message->dwCtrlID))
    {
    case kControl_Size:
        break;
    }*/
    return result;
}
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
