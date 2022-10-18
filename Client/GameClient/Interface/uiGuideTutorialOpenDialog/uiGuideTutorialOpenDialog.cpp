#include "SunClientPrecompiledHeader.h"

#include "uiGuideSystemMan/uiGuideSystemMan.h"
#include "uiGuideTutorialOpenDialog/uiGuideTutorialOpenDialog.h"
#include "uiGuideTutorialDialog/uiGuideTutorialDialog.h"
#include "GlobalFunc.h"

//------------------------------------------------------------------------------
void uiGuideTutorialOpenDialog::RegisterControl()
{
    RegisterControlPosition(StrToWzID("B001"), kControl_Button_B001);//축소된 창(확대버튼)
    RegisterControlPosition(StrToWzID("P000"), kControl_Picture_P000);//테두리 라인
}

uiGuideTutorialOpenDialog::uiGuideTutorialOpenDialog(InterfaceManager* ui_manager) : uiBase(ui_manager)
{
}

uiGuideTutorialOpenDialog::~uiGuideTutorialOpenDialog(void)
{
}

void uiGuideTutorialOpenDialog::Init(CDrawBase* drawbase)
{
    RegisterControl();

    ui_guide_system_manager_ = static_cast<uiGuideSystemMan*>(GetMan());
    assert(ui_guide_system_manager_);

    CControlWZ* control_picture = static_cast<CControlWZ*>(GetControlWZ_byPosition(kControl_Picture_P000));
    if (control_picture)
    {
        control_picture->SetDiscardMessage(true);
    }
}

void uiGuideTutorialOpenDialog::Release()
{
}

void uiGuideTutorialOpenDialog::OnShowWindow(BOOL val)
{  
    if (val)
    {
        keyMsg msg;
        ZeroMemory(&msg, sizeof(msg));
        msg.dwType = eDoType_GuideTutorial_Open_Dialog;
        msg.wParam = IM_GUIDE_SYSTEM_MANAGER::TUTORIAL_OPEN;
        msg.DoSomething = GlobalFunc::DoShowWindowEx;
        g_KeyQueueManager.PushBack(msg);

        g_InterfaceManager.SetUIBlinkGrid(StrToWzID("0437"), StrToWzID("P000"), true);

    }
    else
    {
        g_KeyQueueManager.DeleteMsg(eDoType_GuideTutorial_Open_Dialog);

        g_InterfaceManager.SetUIBlinkGrid(StrToWzID("0437"), StrToWzID("P000"), false);
    }
}
//------------------------------------------------------------------------------
void uiGuideTutorialOpenDialog::MessageProc(SI_MESSAGE* message)
{
    if (IsLock())
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
bool uiGuideTutorialOpenDialog::OnLButtonClick(SI_MESSAGE* message)
{
    switch (GetControlPosition_byID(message->dwCtrlID))
    {
    case kControl_Button_B001://확대
        {
            uiGuideTutorialDialog* tutorial_dialog =
                GET_CAST_DIALOG(uiGuideTutorialDialog, IM_GUIDE_SYSTEM_MANAGER::TUTORIAL);
            if (tutorial_dialog)
            {
                tutorial_dialog->ShowInterface(TRUE);
            }
            ShowInterface(FALSE);
        }
        break;
    default:
        {
        }
    }
    return true;
}
