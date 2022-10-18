#include "SunClientPrecompiledHeader.h"

#ifdef _NA_007029_20140120_DOMINATION_RESURRECTION_REFORM

#include "uiDominationResurrection.h"
#include "DominationInfoParser.h"
#include "map.h"
#include "Hero.h"
#include "uiDominationTowerMan/uiDominationTowerMan.h"
#include "CastLeaveFieldDialog.h"
#include "GlobalFunc.h"

uiDominationResurrection::uiDominationResurrection(InterfaceManager *pUIMan) : uiBase(pUIMan)
{
    RegisterControlPosition(StrToWzID("TI00"), kControl_Button_TI00);
    RegisterControlPosition(StrToWzID("TI01"), kControl_Button_TI01);
    RegisterControlPosition(StrToWzID("L000"), kControl_List_L000);
}

uiDominationResurrection::~uiDominationResurrection()
{
    this->Release();
}

void uiDominationResurrection::Init(CDrawBase* pDrawBase)
{
    CControlWZ* button = GetControlWZ_byPosition(kControl_Button_TI00);
    if (button)
    {
        button->SetTextID(3476); // 3476 부활하기
    }
    button = GetControlWZ_byPosition(kControl_Button_TI01);
    if (button)
    {
        button->SetTextID(3301); // 3301 필드로 이동
    }

}

void uiDominationResurrection::Release()
{
}

void uiDominationResurrection::OnShowWindow( BOOL val )
{
    if (val)
    {
        // esc 로 시스템창 안열리게.
        g_KeyQueueManager.SetDefaultFun(NULL);

        CastLeaveFieldDialog* castleave_dialog = GET_CAST_DIALOG(CastLeaveFieldDialog, StrToWzID("0281"));    
        if (castleave_dialog)
        {
            castleave_dialog->ShowWindow(FALSE);
        }
    }
}

void uiDominationResurrection::OnUpdateSolarDialog()
{
    if (g_pHero && g_pHero->IsDead() == FALSE)
    {
        g_KeyQueueManager.SetDefaultFun(GlobalFunc::DoSystemDialog);
        ShowInterface(FALSE);
    }
}

void uiDominationResurrection::OnRenderSolarDialog()
{
    CControlWZ* list_control = GetControlWZ_byPosition(kControl_List_L000);
    if (list_control == NULL)
    {
        return;
    }

    RECT rect;
    RECT rect2 = list_control->GetSizeRect();

#ifdef _INTERNATIONAL_UI
    const int iLineGap=0;
#else
    const int iLineGap=8;	
#endif//_INTERNATIONAL_UI
    SIZE FontSize;
    
    this->GetDrawBase()->x_pManagerTextOut->StartToUseFont("st10");
    this->GetDrawBase()->x_pManagerTextOut->GetTextExtentPoint(_T("1"), 1, &FontSize);

    SetRect(&rect,
        rect2.left,
        rect2.top + (FontSize.cy*0) + 30,
        rect2.right,
        rect2.top + (FontSize.cy*0) + FontSize.cy+iLineGap + 30);

    TCHAR temp_string[INTERFACE_STRING_LENGTH] = {0,};
    g_InterfaceManager.GetInterfaceString(891, temp_string);

    this->GetDrawBase()->x_pManagerTextOut->DrawText(
        temp_string,
        &rect,
        WzColor_RGBA(240,201,121,255 ),
        WzColor_RGBA(0,0,0,0),
        TP_HCENTER);

    this->GetDrawBase()->x_pManagerTextOut->EndUseFont();
}
//------------------------------------------------------------------------------
void uiDominationResurrection::MessageProc( SI_MESSAGE * pMessage )
{
    if (IsLock())
        return;

    if (GetMan() == NULL)
        return;

    switch (pMessage->eResultMsg)
    {
    case RT_MSG_LBUTTONCLICK:
        OnLButtonClick(pMessage);
        break;
    }
}

bool uiDominationResurrection::OnLButtonClick(SI_MESSAGE* message)
{
    switch (GetControlPosition_byID(message->dwCtrlID))
    {
    case kControl_Button_TI00:
        {
            // 부활하기
            const sDOMINATION_INFO* domination_info =
                DominationInfoParser::Instance()->GetDominationInfo(g_pMap->GetCurrentFieldID());
            if (g_pHero && g_pHero->IsDead() && domination_info)
            {
                keyMsg msg;
                ZeroMemory(&msg,sizeof(msg));
                msg.dwType		= eDoType_Delay_Spawn_DominationWar;
                msg.DoSomething	= GlobalFunc::DeadConfirmInDomination;
                msg.lParam		= InterfaceManager::DIALOG_DELAY_CASTING;
                msg.wParam		= eDoType_Delay_Spawn_DominationWar;
                g_KeyQueueManager.PushBack(msg);

                StartLeaveFieldCastTimer( eDoType_Delay_Spawn_DominationWar, domination_info->m_RespawnDelay);

                // 3326	%d초 후에 시작지점으로 리스폰 됩니다.
                TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
                TCHAR szResultInfo[INTERFACE_STRING_LENGTH] = {0,};
                g_InterfaceManager.GetInterfaceString(3326, szMessage, INTERFACE_STRING_LENGTH);
                Sprintf( szResultInfo, szMessage, domination_info->m_RespawnDelay/1000);
                GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szResultInfo);
            }
            else
            {
                g_KeyQueueManager.SetDefaultFun(GlobalFunc::DoSystemDialog);
            }

            ShowInterface(FALSE);
        }
        break;
    case kControl_Button_TI01:
        {
            // 지상으로 내려가기
            keyMsg msg;
            ZeroMemory(&msg,sizeof(msg));
            msg.dwType		= eDoType_Delay_Leave_Domination;
            msg.DoSomething	= GlobalFunc::DeadConfirmInDomination;
            msg.lParam		= InterfaceManager::DIALOG_DELAY_CASTING;
            msg.wParam		= eDoType_Delay_Leave_Domination;
            g_KeyQueueManager.PushBack(msg);

            // 딜레이 캐스트 창을 띄우고 타이머를 세팅한다.
            StartLeaveFieldCastTimer( eDoType_Delay_Leave_Domination, DELAY_LEAVE_FIELD_TICK );

            ShowInterface(FALSE);
        }
        break;
    }



    return true;
}

#endif //_NA_007029_20140120_DOMINATION_RESURRECTION_REFORM