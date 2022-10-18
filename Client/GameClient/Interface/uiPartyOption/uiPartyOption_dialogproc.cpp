//------------------------------------------------------------------------------
//  uiPartyOption_dialogproc.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "uiPartyOption.h"
#include "TargetDialog.h"
#include "uiPartyLeavePopup/uiPartyLeavePopup.h"
#include "uiPartyMan/uiPartyMan.h"
#include "Hero.h"
#include "globalfunc.h"
#include "InterfaceManager.h"
//------------------------------------------------------------------------------
WzID uiPartyOption::m_wzId[DIALOG_MAX] = 
{ 
    StrToWzID("TI00"),    // DLGPOS_LEAVE,
    StrToWzID("TI01"),    // DLGPOS_FOR CEDEXPLUSION,
    StrToWzID("TI02"),    // DLGPOS_DESTROY,
    StrToWzID("TI03"),    // DLGPOS_MIGRATION,
};

//------------------------------------------------------------------------------
/**
    called to show request
*/
void
uiPartyOption::OnShowWindow( BOOL val )
{
    //this->m_bShow = val;    // for uiBase::IsVisable()

    if (val)
    {
        float x =.0f, y =.0f;

        MoveWindowWZ( x, y );

        this->OffLock();
        this->OffPending();

		keyMsg msg;
		ZeroMemory(&msg,sizeof(msg));
		msg.dwType=eDoType_dialog_party_option;
		msg.wParam=InterfaceManager::DIALOG_PARTY_OPTION;
		msg.DoSomething=GlobalFunc::DoShowWindow;
		g_KeyQueueManager.PushBack(msg);
    }
    else
    {
		g_KeyQueueManager.DeleteMsg(eDoType_dialog_party_option);
    }
}

//------------------------------------------------------------------------------
/**
*/
void
uiPartyOption::MessageProc( SI_MESSAGE * pMessage )
{
    if (this->IsLock())
        return;

    switch( pMessage->eResultMsg ) 
    {
    case RT_MSG_LBUTTONCLICK:
        {
            switch( getCtrlIDToPos( pMessage->dwCtrlID ) )
            {
            case DLGPOS_LEAVE:
                {
					uiBase * pDlg = GET_DIALOG( IM_PARTY_MANAGER::PARTY_LEAVE_POPUP );

					if( pDlg )
						pDlg->ShowInterface( TRUE );

                    ShowInterface(FALSE);
                }
                break;

            case DLGPOS_FORCEDEXPLUSION:
                {
					if(this->GetManager())
					{
						this->GetManager()->ForcedExpulsion();
					}
				}
                break;

            case DLGPOS_MIGRATION:
                {
					if(this->GetManager())
					{
						this->GetManager()->ChangeMaster();
					}
                }
                break;
            }
        }
        break;

	default:
		{
			SolarDialog::MessageProc( pMessage );
		}
		break;
    }
}

//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
