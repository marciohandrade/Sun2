#include "SunClientPrecompiledHeader.h"

#ifdef _YMS_AUTOBOT_GAME

#include "uiAutobotResult.h"
#include "uiAutobotMan.h"

#include "Autobot/Condition/AutobotConditionManager.h"

#include "Map.h"
#include "GlobalFunc.h"
#include "AutomaticInfoParser.h"
#include "AutomaticComboInfoParser.h"

//------------------------------------------------------------------------------ 
const DWORD uiAutobotResult::kDialog_ID = StrToWzID("0954");;

//------------------------------------------------------------------------------ 
uiAutobotResult::uiAutobotResult(InterfaceManager* ui_man) : uiBase(ui_man)
{
    RegisterControlPosition(StrToWzID("B001"), kControl_Button_B001);
    RegisterControlPosition(StrToWzID("B002"), kControl_Button_B002);
    RegisterControlPosition(StrToWzID("T001"), kControl_Text_T001);
    RegisterControlPosition(StrToWzID("S000"), kControl_Text_S000);
    RegisterControlPosition(StrToWzID("S001"), kControl_Text_S001);
    RegisterControlPosition(StrToWzID("S002"), kControl_Text_S002);
    RegisterControlPosition(StrToWzID("S003"), kControl_Text_S003);
    RegisterControlPosition(StrToWzID("P000"), kControl_Picture_P000);
    RegisterControlPosition(StrToWzID("P001"), kControl_Picture_P001);
}

//------------------------------------------------------------------------------ 
uiAutobotResult::~uiAutobotResult( void )
{
    Release();
}
//------------------------------------------------------------------------------ 
void uiAutobotResult::Init(CDrawBase* drawbase)
{

}
//------------------------------------------------------------------------------ 
void uiAutobotResult::Release(void)
{

}
//------------------------------------------------------------------------------ 
void uiAutobotResult::OnShowWindow(BOOL val)
{
    if (val == TRUE)
    {
        MoveCenter();
        //g_InterfaceManager.LockDlgs(kDialog_ID);
    }
    else
    {
        //if (IsLock())
        //{
        //    g_InterfaceManager.UnLockDlgs();
        //}
    }
}
//------------------------------------------------------------------------------ 
void uiAutobotResult::NetworkProc(MSG_BASE* msgbase)
{

}
//------------------------------------------------------------------------------
void uiAutobotResult::MessageProc(SI_MESSAGE* message)
{
    if (IsLock())
    {
        return;
    }

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
bool uiAutobotResult::OnLButtonClick(SI_MESSAGE* message)
{
    bool result = true;
    UIControlPos control_pos = static_cast<UIControlPos>(GetControlPosition_byID(message->dwCtrlID));
    switch (control_pos)
    {
    case kControl_Button_B001:
    case kControl_Button_B002:
        {
            ShowInterface(FALSE);
        }
    default:
        {
        }
        break;
    }
    return result;
}
//------------------------------------------------------------------------------ 
void uiAutobotResult::SetResult(LONGLONG money, LONGLONG exp)
{
    GetControlWZ(GetControlID_byPosition(kControl_Text_S003))->SetMoneyWithSymbol(money);
    GetControlWZ(GetControlID_byPosition(kControl_Text_S002))->SetMoneyWithSymbol(exp);
}
#endif //_YMS_AUTOBOT_GAME