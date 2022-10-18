#include "SunClientPrecompiledHeader.h"
#ifdef _NA_007086_20140318_MONSTERBOOK_EXTRACT

#include "uiMonsterBookExtract.h"
#include "uiMonsterBookMan/uiMonsterBookMan.h"
#include "GlobalFunc.h"

uiMonsterBookExtract::uiMonsterBookExtract(InterfaceManager* ui_manager)
{
    /*
    RegisterControlPosition(StrToWzID("B999"), kControl_Button_B999);
    RegisterControlPosition(StrToWzID("B005"), kControl_Button_B005);
    RegisterControlPosition(StrToWzID("B000"), kControl_Button_B000);
    RegisterControlPosition(StrToWzID("B001"), kControl_Button_B001);
    RegisterControlPosition(StrToWzID("B002"), kControl_Button_B002);
    RegisterControlPosition(StrToWzID("B003"), kControl_Button_B003);
    RegisterControlPosition(StrToWzID("B004"), kControl_Button_B004);
    RegisterControlPosition(StrToWzID("B007"), kControl_Button_B007);
    RegisterControlPosition(StrToWzID("B006"), kControl_Button_B006);
    RegisterControlPosition(StrToWzID("B008"), kControl_Button_B008);
    RegisterControlPosition(StrToWzID("T999"), kControl_Text_T999);
    RegisterControlPosition(StrToWzID("L000"), kControl_List_L000);
    RegisterControlPosition(StrToWzID("V001"), kControl_VScroll_V001);
    RegisterControlPosition(StrToWzID("S000"), kControl_Text_S000);
    RegisterControlPosition(StrToWzID("S002"), kControl_Text_S002);
    RegisterControlPosition(StrToWzID("S003"), kControl_Text_S003);
    RegisterControlPosition(StrToWzID("E000"), kControl_Edit_E000);
    RegisterControlPosition(StrToWzID("E001"), kControl_Edit_E001);*/

}
//------------------------------------------------------------------------------
uiMonsterBookExtract::~uiMonsterBookExtract()
{
}
//------------------------------------------------------------------------------
void uiMonsterBookExtract::Init(CDrawBase* draw_base)
{
    uiMonsterBookBase::Init(draw_base);
}
//------------------------------------------------------------------------------
void uiMonsterBookExtract::Release()
{
    uiMonsterBookBase::Release();
}
//------------------------------------------------------------------------------
void uiMonsterBookExtract::NetworkProc(struct MSG_BASE *message)
{
    uiMonsterBookBase::NetworkProc(message);
}
//------------------------------------------------------------------------------
void uiMonsterBookExtract::OnRenderSolarDialog()
{
    uiMonsterBookBase::OnRenderSolarDialog();
    //RenderTooltip();
}
//------------------------------------------------------------------------------
void uiMonsterBookExtract::OnUpdateSolarDialog()
{
    uiMonsterBookBase::OnUpdateSolarDialog();
}
//------------------------------------------------------------------------------
void uiMonsterBookExtract::OnShowWindow(BOOL is_show)
{
    uiMonsterBookBase::OnShowWindow(is_show);

    if (is_show)
    {
        set_tap_control(MonsterBookTap_Extract);

        keyMsg msg;
        ZeroMemory(&msg,sizeof(msg));
        msg.dwType = eDoType_MonsterBook_Extract;
        msg.wParam = IM_MONSTERBOOK_MANAGER::MONSTERBOOK_EXTRACT;
        msg.DoSomething = GlobalFunc::DoMonsterBookDialog;
        g_KeyQueueManager.PushBack(msg);
    }
    else
    {
        g_KeyQueueManager.DeleteMsg(eDoType_MonsterBook_Extract);
    }
}
//------------------------------------------------------------------------------
void uiMonsterBookExtract::MessageProc(SI_MESSAGE* message)
{
    uiMonsterBookBase::MessageProc(message);

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
bool uiMonsterBookExtract::OnLButtonClick(SI_MESSAGE* message)
{
    if (GetMan() == NULL)
    {
        return false;
    }

    switch (GetControlPosition_byID(message->dwCtrlID))
    {
    case kControl_Button_B086:
        {
        }
        break;

    default:
        {
        }
        break;
    }

    return TRUE;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void uiMonsterBookExtract::RenderTooltip()
{
    /*
    uiToolTipMan* tooltip_manager = GET_DIALOG_MANAGER(uiToolTipMan, UIMAN_TOOLTIP);
    if (tooltip_manager == NULL)
    {
        return;
    }

    int mouse_x = Mouse::Instance()->GetMouseX();
    int mouse_y = Mouse::Instance()->GetMouseY();

#ifdef _NA_000000_20120914_CHAOS_ZONE_BATTLE_GROUND_ACTIVATION
    // 전장 활성화
    int tap_controls[] = {
        kControl_Button_B000,
            0/ *kControl_Button_B001* /,
            0/ *kControl_Button_B002* /,
            kControl_Button_B003,
            kControl_Button_B004,
            kControl_Button_B007
    };
#else
    // 전장 비활성화
    int tap_controls[] = {
        0/ *kControl_Button_B000* /,
            0 / *kControl_Button_B001* /,
            0 / *kControl_Button_B002* /,
            0/ *kControl_Button_B003* /,
            kControl_Button_B004,
            0/ *kControl_Button_B007* /
    };
#endif //_NA_000000_20120914_CHAOS_ZONE_BATTLE_GROUND_ACTIVATION

    CControlWZ* control;
    for (int index = 0; index < sizeof(tap_controls)/sizeof(tap_controls[0]); ++index)
    {
        if (tap_controls[index] == 0)
        {
            continue;
        }
        control = GetControlWZ_byPosition(tap_controls[index]);
        if (control)
        {
            RECT position = control->GetSizeRect();
            if (position.left < mouse_x && position.right > mouse_x &&
                position.top < mouse_y && position.bottom > mouse_y)
            {
                // 2571 ~ 2576 / 이스라필의 대지~ 카오스 히스토리
                tooltip_manager->RegisterTooltipWithStringCode(2571 + index);
                tooltip_manager->SetTooltipDialogID(GetDialogKey());
            }
        }
    }*/

}
//-----------------------------------------------------------------------------
void uiMonsterBookExtract::SetMonsterInfo(DWORD monster_code)
{
    // 탭에 맞는 몬스터정보 세팅

}

//------------------------------------------------------------------------------ 
//------------------------------------------------------------------------------ 

#endif //_NA_007086_20140318_MONSTERBOOK
