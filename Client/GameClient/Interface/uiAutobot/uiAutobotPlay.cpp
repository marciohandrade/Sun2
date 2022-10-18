#include "SunClientPrecompiledHeader.h"

#ifdef _YMS_AUTOBOT_GAME

#include "uiAutobotPlay.h"
#include "uiAutobotMan.h"
#include "uiAutobotConditionBattle.h"
#include "uiAutobotConditionSupport.h"

#include "Autobot/Autobot.h"
#include "Autobot/Condition/AutobotConditionManager.h"
#include "ItemManager.h"
#include "GlobalData.h"
#include "Hero.h"
#include "HeroEquipmentContainer.h"

//------------------------------------------------------------------------------ 
const DWORD uiAutobotPlay::kDialog_ID = StrToWzID("9530");;

//------------------------------------------------------------------------------ 
uiAutobotPlay::uiAutobotPlay(InterfaceManager* ui_man) : uiBase(ui_man)
{
    RegisterControlPosition(StrToWzID("B999"), kControl_Button_B999);
    RegisterControlPosition(StrToWzID("B002"), kControl_Button_B002);
    RegisterControlPosition(StrToWzID("B003"), kControl_Button_B003);
    RegisterControlPosition(StrToWzID("B004"), kControl_Button_B004);
}

//------------------------------------------------------------------------------ 
uiAutobotPlay::~uiAutobotPlay( void )
{
    Release();
}
//------------------------------------------------------------------------------ 
void uiAutobotPlay::Init(CDrawBase* drawbase)
{
}
//------------------------------------------------------------------------------ 
void uiAutobotPlay::Release(void)
{
}
//------------------------------------------------------------------------------ 
void uiAutobotPlay::OnShowWindow(BOOL val)
{
    if (val == TRUE)
    {
        SLOTCODE item_code = 0;
        if (g_pHero->GetHeroEquipContainer()->IsEquipItem(eITEMTYPE_AUTO_HUNTING, &item_code) == true)
        {
            AutobotConditionManager::Instance()->CheckValid(item_code);
        }
    }
}
//------------------------------------------------------------------------------ 
void uiAutobotPlay::Process(DWORD tick)
{
    GetControlWZ(GetControlID_byPosition(kControl_Button_B999))->ShowWindowWZ(WZ_SHOW);
    GetControlWZ(GetControlID_byPosition(kControl_Button_B002))->ShowWindowWZ(WZ_SHOW);
    GetControlWZ(GetControlID_byPosition(kControl_Button_B003))->EnableWindowWZ(true);
    GetControlWZ(GetControlID_byPosition(kControl_Button_B004))->EnableWindowWZ(true);

    if (Autobot::Instance()->IsStarted())
    {
        GetControlWZ(GetControlID_byPosition(kControl_Button_B999))->ShowWindowWZ(WZ_HIDE);
        GetControlWZ(GetControlID_byPosition(kControl_Button_B003))->EnableWindowWZ(false);
        GetControlWZ(GetControlID_byPosition(kControl_Button_B004))->EnableWindowWZ(false);
    }
    else
    {
        GetControlWZ(GetControlID_byPosition(kControl_Button_B002))->ShowWindowWZ(WZ_HIDE);
    }
}
//------------------------------------------------------------------------------ 
void uiAutobotPlay::OnRenderSolarDialog()
{
    if (Autobot::Instance()->IsStarted())
    {
        g_InterfaceManager.RenderActiveAutobot(static_cast<int>(GetDialogWZ()->GetDlgX()) + 2, 
                                            static_cast<int>(GetDialogWZ()->GetDlgY()) + 1);
    }
}
//------------------------------------------------------------------------------ 
void uiAutobotPlay::NetworkProc(MSG_BASE* msgbase)
{

}
//------------------------------------------------------------------------------
void uiAutobotPlay::MessageProc(SI_MESSAGE* message)
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
bool uiAutobotPlay::OnLButtonClick(SI_MESSAGE* message)
{
    bool result = true;
    switch (GetControlPosition_byID(message->dwCtrlID))
    {
    case kControl_Button_B999:
        {
            Autobot::Instance()->Start();
        }
        break;
    case kControl_Button_B002:
        {
            Autobot::Instance()->Stop(false);
        }
        break;
    case kControl_Button_B003:
        {
            static_cast<uiAutobotMan*>(GetMan())->ShowTab();
        }
        break;
    case kControl_Button_B004:
        {
            //! 아이템 내리기
            Autobot::Instance()->Stop(false);

            POSTYPE empty_slot_pos;
            POSTYPE item_pos;
            if (ItemManager::Instance()->GetContainer(SI_INVENTORY)->GetEmptyPos(OUT empty_slot_pos) && 
                ItemManager::Instance()->FindFirstItem(eITEMTYPE_AUTO_HUNTING, OUT item_pos, SI_EQUIPMENT))
            {
                ItemManager::Instance()->SendItemMoveMsg(SI_EQUIPMENT, SI_INVENTORY, item_pos, empty_slot_pos);
            }
        }
        break;
    default:
        {
        }
        break;
    }
    return result;
}

#endif //_YMS_AUTOBOT_GAME