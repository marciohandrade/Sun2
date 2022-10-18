#include "SunClientPrecompiledHeader.h"
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#include "uiChaosZoneArena.h"
#include "ChaosSystem/ChaosSystemMan.h"
#include "uiToolTipMan/uiToolTipMan.h"
#include "Mouse.h"


const DWORD uiChaosZoneArena::kDialog_ID = StrToWzID("0642");

uiChaosZoneArena::uiChaosZoneArena(InterfaceManager* ui_manager)
{
}
//------------------------------------------------------------------------------
uiChaosZoneArena::~uiChaosZoneArena()
{
}
//------------------------------------------------------------------------------
void uiChaosZoneArena::Init(CDrawBase* draw_base)
{
}
//------------------------------------------------------------------------------
void uiChaosZoneArena::Release()
{
}
//------------------------------------------------------------------------------
void uiChaosZoneArena::NetworkProc(struct MSG_BASE *message)
{
}
//------------------------------------------------------------------------------
void uiChaosZoneArena::MessageProc(SI_MESSAGE* message)
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
void uiChaosZoneArena::OnRenderSolarDialog()
{
    RenderTooltip();
}

//------------------------------------------------------------------------------
bool uiChaosZoneArena::OnLButtonClick(SI_MESSAGE* message)
{
    bool result = true;
    /*switch (GetControlPosition_byID(message->dwCtrlID))
    {
    case kControl_Button_B999:
        {
        }
        break;
    }*/
    return result;
}

//-----------------------------------------------------------------------------
void uiChaosZoneArena::RenderTooltip()
{
    //uiToolTipMan* tooltip_manager = GET_DIALOG_MANAGER(uiToolTipMan, UIMAN_TOOLTIP);
    //if (tooltip_manager == NULL)
    //{
    //    return;
    //}

    //int mouse_x = Mouse::Instance()->GetMouseX();
    //int mouse_y = Mouse::Instance()->GetMouseY();

    //int tap_controls[] = {kControl_Button_B000,0 /*kControl_Button_B001*/,0 /*kControl_Button_B002*/,
    //    kControl_Button_B003, kControl_Button_B004, kControl_Button_B007};
    //CControlWZ* control;
    //for (int index = 0; index < sizeof(tap_controls)/sizeof(tap_controls[0]); ++index)
    //{
    //if (tap_controls[index] == 0)
    //{
    //    continue;
    //}
    //    control = GetControlWZ_byPosition(tap_controls[index]);
    //    if (control)
    //    {
    //        RECT position = control->GetSizeRect();
    //        if (position.left < mouse_x && position.right > mouse_x &&
    //            position.top < mouse_y && position.bottom > mouse_y)
    //        {
    //            // 2571 ~ 2576 / 이스라필의 대지~ 카오스 히스토리
    //            tooltip_manager->RegisterTooltipWithStringCode(2571 + index);
    //        }
    //    }
    //}
}
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
