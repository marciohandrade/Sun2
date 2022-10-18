#include "SunClientPrecompiledHeader.h"

#include "uiTutorialSkip.h"
#include "InterfaceManager.h"
#include "uiMenuMan.h"
#include "uiSystemMan\uiSystemMan.h"


////------------------------------------------------------------------------------ 
const DWORD uiTutorialSkip::kDialog_ID = StrToWzID("0243");;

//------------------------------------------------------------------------------ 
//static function
void uiTutorialSkip::Callback_TutorialSkip(bool is_yes)
{
    if ( is_yes ) 
    {
        uiMenuMan* man = static_cast<uiMenuMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_MENU));
        if (man != NULL)
        {
            man->Send_CG_ZONE_EX_TUTORIAL_FIELD_LEAVE_SYN();
        }
    }
}

//------------------------------------------------------------------------------ 
uiTutorialSkip::uiTutorialSkip(InterfaceManager* ui_man) : uiBase(ui_man)
{
    RegisterControlPosition(StrToWzID("B999"), kControl_Button_B999);
}
//------------------------------------------------------------------------------ 
uiTutorialSkip::~uiTutorialSkip( void )
{
    Release();
}
//------------------------------------------------------------------------------ 
uiMenuMan* uiTutorialSkip::GetManager()
{
    return static_cast<uiMenuMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_MENU));
}
//------------------------------------------------------------------------------
void uiTutorialSkip::MessageProc(SI_MESSAGE* message)
{
    if (IsLock())
    {
        return;
    }

    if (GetManager() == NULL)
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
bool uiTutorialSkip::OnLButtonClick(SI_MESSAGE* message)
{
    bool result = true;
    switch (GetControlPosition_byID(message->dwCtrlID))
    {
    case kControl_Button_B999:
        {
            uiSystemMan* system_man = static_cast<uiSystemMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM));
            if (system_man)
            {
                //70552	튜토리얼을 종료하고 벨트헨 마을로 이동 하시겠습니까?\n\n [주의] 벨트헨 마을로 이동 시 에소드 마을로 돌아 올 수 없습니다.
                system_man->Sys_Confirm(g_InterfaceManager.GetInterfaceString(70552), uiTutorialSkip::Callback_TutorialSkip);
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
