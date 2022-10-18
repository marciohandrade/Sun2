#include "SunClientPrecompiledHeader.h"
#include "AskReturnPartyDialog.h"
#include "uiAssistMan/uiAssistMan.h"
#include "GameConst_Color.h"
#include "uiPartyMan/uiPartyMan.h"
#include "Application.h"

//------------------------------------------------------------------------------ 
AskReturnPartyDialog::AskReturnPartyDialog(InterfaceManager* ui_man) : AskDialog(ui_man)
{

}

//------------------------------------------------------------------------------
AskReturnPartyDialog::~AskReturnPartyDialog()
{
}

//------------------------------------------------------------------------------
void AskReturnPartyDialog::OnUpdateSolarDialog()
{
    if (this->GetStatus() == ASK_STATUS_OK)
    {
        this->DoIt();
        SetStatus(ASK_STATUS_NORMAL);
    }
    else if (this->GetStatus() == ASK_STATUS_CANCEL)
    {
        this->Cancel();
        SetStatus(ASK_STATUS_NORMAL);
    }
}

//------------------------------------------------------------------------------
void AskReturnPartyDialog::MessageProc(SI_MESSAGE * pMessage)
{
    switch(pMessage->eResultMsg) 
    {
    case RT_MSG_LBUTTONCLICK:
        {
            switch(getCtrlIDToPos(pMessage->dwCtrlID))
            {
            case ASK_OK:
                {
                    this->m_status = ASK_STATUS_OK;
                }
                break;

            case ASK_CANCEL:
                {
                    this->m_status = ASK_STATUS_CANCEL;
                }
                break;
            }
        }
        break;
    default:
        {
            SolarDialog::MessageProc(pMessage);
        }
        break;
    }
}

//------------------------------------------------------------------------------
void AskReturnPartyDialog::OnShowWindow(BOOL val)
{
    if (val)
    {
        MoveCenter();
        SetTexts();
        g_InterfaceManager.LockDlgs(IM_ASSIST_MANAGER::RETURN_PARTY_DIALOG); // 화면 락
    }
    else
    {
        g_InterfaceManager.UnLockDlgs();
    }
}

//------------------------------------------------------------------------------
void AskReturnPartyDialog::OnRenderSolarDialog()
{
    if (GetDialogWZ()->IsState(WZ_SHOW) == true)
    {
        POINT pt = g_pApplication->Get2DModeMapping();
        RECT rc;

        SetRect(&rc,
            0,
            0,
            pt.x,
            pt.y);

        g_pSunRenderer->ColorFill(WzColor_RGBA(0,0,0,128), 0, 0, (float)rc.right,(float)rc.bottom);
        
        g_InterfaceManager.AttachFirstDialog(IM_ASSIST_MANAGER::RETURN_PARTY_DIALOG);
        GetDialogWZ()->OnDraw(true);
    }
}

//------------------------------------------------------------------------------
void AskReturnPartyDialog::SetTexts()
{
    // 5238 비정상 종료가 되기 이전에 파티로 복귀 하시겠습니까?
    TCHAR message[INTERFACE_STRING_LENGTH] ={0,};
    g_InterfaceManager.GetInterfaceString(5238, message);
    InsertMessage(message, c_InterfaceBKColor, c_InterfaceTextColor);
}

//------------------------------------------------------------------------------
void AskReturnPartyDialog::DoIt()
{
    uiPartyMan* party_manager = GET_DIALOG_MANAGER(uiPartyMan, UIMAN_PARTY);
    if(party_manager)
    {
        party_manager->NET_SEND_CG_PARTY_RETURN_C2G_MEMBER_QUERY_ACK(true);
    }
    ShowInterface(FALSE);
}

//------------------------------------------------------------------------------
void AskReturnPartyDialog::Cancel()
{
    uiPartyMan* party_manager = GET_DIALOG_MANAGER(uiPartyMan, UIMAN_PARTY);
    if(party_manager)
    {
        party_manager->NET_SEND_CG_PARTY_RETURN_C2G_MEMBER_QUERY_ACK(false);
    }
    ShowInterface(FALSE);
}
