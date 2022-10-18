//------------------------------------------------------------------------------
//  CharSelectDialog.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include ".\AskPartyInvitingDialog.h"
#include "GameFramework.h"
#include "interfacemanager.h"
#include "Party.h"

//------------------------------------------------------------------------------
/**
*/
AskPartyInvitingDialog::AskPartyInvitingDialog(void)
{
}

//------------------------------------------------------------------------------
/**
*/
AskPartyInvitingDialog::~AskPartyInvitingDialog(void)
{
}

//------------------------------------------------------------------------------
/**
*/
void AskPartyInvitingDialog::showWindow(BOOL val)
{
    AskDialog::showWindow(val);

    if (val)
    {
        RECT rc;
        GetDialogWZ()->GetDlgSize(&rc);

        double width = rc.right;
        double height = rc.bottom;

        // TODO:
        // 전체 화면 해상도를 참조해서 다이얼로그를 화면 중앙에 위치시킨다.
        // ex) 1024x768

        double mw = width * 0.5f;
        double mh = height * 0.5f;

        RECT *rcWindow = g_InterfaceManager.GetWindowRC();

        long sw;
        long sh;
        if (rcWindow)
        {
            sw = (rcWindow->right - rcWindow->left);
            sh = (rcWindow->bottom - rcWindow->top);
        }
        else
        {
            sw = 1024; sh = 768;
        }

        long pw = (sw * 0.5f) - mw;
        long ph = ((sh * 0.5f) - mh) - 200;

        //		char szText[512];
        //		sprintf(szText, 
        //			"Dlg Rc : (left : %d, right : %d)\nmw : %f mh : %f sw : %f sh : %f\npw : %f ph : %f", 
        //			rc.left, rc.right, mw, mh, sw, sh, pw, ph);
        //		OutputDebugString(szText);

        GetDialogWZ()->MoveWindowWZ(pw, ph);
    
        bDestroy = true;
        bDestroyTimer = false;
        dwDestroyTick = 0;
        dwDestroyTickOver = 0;
    }
}

//------------------------------------------------------------------------------
/**
*/
void AskPartyInvitingDialog::Process(DWORD dwTick)
{
    if (bDestroyTimer)
    {
        dwDestroyTick += dwTick;

        if (dwDestroyTick > dwDestroyTickOver)
        {
            if (g_InterfaceManager.IsShowDialog(InterfaceManager::DIALOG_ASK))
            {
                g_InterfaceManager.DeleteInterface(StrToWzID("DASK"));
            }
            this->m_status = ASK_STATUS_CANCEL;
            g_Party.actInvitedNo();

            bDestroyTimer = false;
        }
    }

    if (bDestroy)
    {
        dwDestroyTick = dwTick;
        dwDestroyTickOver = dwTick + (5*1000);
        bDestroy = false;
        bDestroyTimer = true;
    }
}

//------------------------------------------------------------------------------
/**
*/
void AskPartyInvitingDialog::MessageProc( SI_MESSAGE * pMessage )
{
    switch( pMessage->eResultMsg ) 
    {
    case RT_MSG_LBUTTONCLICK:
        {
            switch( getCtrlIDToPos( pMessage->dwCtrlID ) )
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
    }
}

//------------------------------------------------------------------------------
/**
*/
void AskPartyInvitingDialog::update()
{
    CCtrlListWZ * pLCtrl = (CCtrlListWZ *)getControl( ASK_LIST );
    pLCtrl->DeleteItemAll();
    pLCtrl->SetBeginDrawIndex(0);


	TCHAR	szText[INTERFACE_STRING_LENGTH], szMessage[INTERFACE_STRING_LENGTH];
	//	[%s] 님이 귀하를 초대합니다.
	g_InterfaceManager.GetInterfaceString( eST_NAMECODE_INVITE_YOU, szText, INTERFACE_STRING_LENGTH );
	_stprintf( szMessage, szText, g_Party.GetInviterName() );
    pLCtrl->SetColorText(WzColor_RGBA( 255, 255, 255, 255 ));
    pLCtrl->SetItemText(pLCtrl->InsertItemBack(),szMessage);

	//	수락하시겠습니까?
	g_InterfaceManager.GetInterfaceString( eST_ACCEPT_INVITE, szMessage, INTERFACE_STRING_LENGTH );
    pLCtrl->SetColorText(WzColor_RGBA( 255, 255, 255, 255 ));
    pLCtrl->SetItemText(pLCtrl->InsertItemBack(),szMessage);

	//	남은 시간.... ( %d )
	g_InterfaceManager.GetInterfaceString( eST_REMAIN_TIME, szText, INTERFACE_STRING_LENGTH );
    _stprintf(szMessage, szText, ((dwDestroyTickOver - dwDestroyTick)/1000)+1);
    pLCtrl->SetColorText(WzColor_RGBA( 255, 255, 255, 255 ));
    pLCtrl->SetItemText(pLCtrl->InsertItemBack(),szMessage);

    pLCtrl->SetUnuseSelBar(TRUE);
    pLCtrl->OnFocusOut();

    if (this->GetStatus() == ASK_STATUS_OK)
    {
        g_Party.actInvitedYes();
    }
    else if (this->GetStatus() == ASK_STATUS_CANCEL)
    {
        g_Party.actInvitedNo();
    }
}


