//------------------------------------------------------------------------------
//  uiBattleZonePassword_dialogproc.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "uiBattleZonePassword.h"
#include "uiBattleZoneList/uiBattleZoneList.h"
#include "Mouse.h"

#include "uiBattleZoneMan/uiBattleZoneMan.h"
#include "interfacemanager.h"
#include "globalfunc.h"
//------------------------------------------------------------------------------
WzID uiBattleZonePassword::m_wzId[DIALOG_MAX] = 
{ 
    StrToWzID("E000"),  // DLGPOS_EDT_PASSWORD=0,
    StrToWzID("TI00"),  // DLGPOS_BTN_OK,
    StrToWzID("TI01"),  // DLGPOS_BTN_CANCEL,
};

//------------------------------------------------------------------------------
/**
*/
void
uiBattleZonePassword::OnShowWindow( BOOL val )
{
    if (val)
    {
        this->m_strPassword.clear();

        //MoveWindowWZ( 0.0f, 0.0f );
		
        CCtrlDxIMEEditBox * pE;

        pE = (CCtrlDxIMEEditBox *)getControl( DLGPOS_EDT_PASSWORD );
        if (pE)
        {
			GlobalFunc::SetFocus( (SolarDialog *)this, pE );

            pE->SetFontID(StrToWzID("mn12"));
            pE->SetColorCaret(RGBA(255,100,100,255));
            pE->SetColorText(RGBA(255,255,255,255));
            pE->SetLimitText(MAX_ROOMPASSWORD_LENGTH-1);
            pE->ClearText();
        }

        this->OffLock();
        this->OffPending();

		GetManager()->SetWait(true);
    }
    else
    {
		GetManager()->SetWait(false);
    }
}

//------------------------------------------------------------------------------
/**
*/
void
uiBattleZonePassword::MessageProc( SI_MESSAGE * pMessage )
{
	TCHAR	szMessage[INTERFACE_STRING_LENGTH];

    switch (pMessage->eResultMsg)
    {
    case RT_MSG_EDIT_ENTERKEYDOWN:
        {
            switch( getCtrlIDToPos( pMessage->dwCtrlID ) )
            {
            case DLGPOS_EDT_PASSWORD:
                {
                    TCHAR		pszRoomPW[MAX_ROOMPASSWORD_LENGTH+1];

                    CCtrlDxIMEEditBox * pE;

                    //
                    // 배틀존 비밀번호
                    //

                    pE = (CCtrlDxIMEEditBox *)getControl( DLGPOS_EDT_PASSWORD );
                    pE->GetTextWZ( pszRoomPW, MAX_ROOMPASSWORD_LENGTH+1);
                    int len = _tcslen( pszRoomPW );
                    if( len == 0 )
                    {
						//	비밀번호를 입력 하십시오.
						g_InterfaceManager.GetInterfaceString( eST_INPUT_PASSWORD, szMessage, INTERFACE_STRING_LENGTH );
                        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
						GlobalFunc::SetFocus( (SolarDialog *)this, pE );
                    }
                    else
                    {
						this->m_strPassword = pszRoomPW;

                        this->ShowInterface(FALSE);
                    }
                }
                break;
            }
        }
        break;

    case RT_MSG_LBUTTONCLICK:
        {
            bool clickedInDlg = false;

            POSTYPE pos = getCtrlIDToPos( pMessage->dwCtrlID );

            if (pos >= DLGPOS_EDT_PASSWORD && pos <= DLGPOS_BTN_CANCEL)
            {
                clickedInDlg = true;

                switch (pos)
                {
                case DLGPOS_BTN_OK:
                    {
                        TCHAR			pszRoomPW[MAX_ROOMPASSWORD_LENGTH+1];

                        CCtrlDxIMEEditBox * pE;

                        //
                        // 배틀존 비밀번호
                        //

                        pE = (CCtrlDxIMEEditBox *)getControl( DLGPOS_EDT_PASSWORD );
                        pE->GetTextWZ( pszRoomPW, MAX_ROOMPASSWORD_LENGTH+1);
                        int len = _tcslen( pszRoomPW );
                        if( len == 0 )
                        {
							//	비밀번호를 입력 하십시오.
							g_InterfaceManager.GetInterfaceString( eST_INPUT_PASSWORD, szMessage, INTERFACE_STRING_LENGTH );
                            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
							GlobalFunc::SetFocus( (SolarDialog *)this, pE );
                        }
                        else
                        {
							this->m_strPassword = pszRoomPW;

							GetManager()->RequestJoinPrivateZone(this->m_strPassword);

                            this->ShowInterface(FALSE);
                        }
                    }
                    break;

                case DLGPOS_BTN_CANCEL:
                    {
						this->ShowInterface(FALSE);
                    }
                    break;
                }
            }
            else
            {
                int iMouseX = Mouse::Instance()->GetMouseX();
                int iMouseY = Mouse::Instance()->GetMouseY();

                RECT rc;

                this->GetDialogWZ()->GetDlgSize(&rc);;

                if (rc.left  <= iMouseX
                    && rc.right >= iMouseX)
                {
                    if (rc.top    <= iMouseY
                        && rc.bottom >= iMouseY)
                    {
                        clickedInDlg = true;
                    }
                }
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
/**
*/
void
uiBattleZonePassword::OnUpdateSolarDialog()
{
    if(GetManager()->IsWait())
	{
		if(!this->HaveFocus())
			ShowInterface(FALSE);
	}
}

//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
