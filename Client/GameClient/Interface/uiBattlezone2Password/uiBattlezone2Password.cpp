#include "SunClientPrecompiledHeader.h"
#ifdef _DH_BATTLEZONE2_
#include "uiBattlezone2Password/uiBattlezone2Password.h"
#include "Mouse.h"
#include "uiBattlezone2Man/uiBattlezone2Man.h"
#include "uiBattlezone2List/uiBattlezone2List.h"
#include "interfacemanager.h"
#include "globalfunc.h"
#include "Hero.h"
#include "uiSystemMan/uiSystemMan.h"
#include "uiPartyMan/uiPartyMan.h"
//------------------------------------------------------------------------------
void uiBattlezone2Password::RegisterControl()
{
    RegisterControlPosition(StrToWzID("E000"), DLGPOS_EDT_PASSWORD); 
    RegisterControlPosition(StrToWzID("TI00"), DLGPOS_BTN_OK); 
    RegisterControlPosition(StrToWzID("TI01"), DLGPOS_BTN_CANCEL); 
}

uiBattlezone2Password::uiBattlezone2Password(InterfaceManager *pUIMan) :
uiBase(pUIMan)
{
}

//------------------------------------------------------------------------------
/**
*/
uiBattlezone2Password::~uiBattlezone2Password()
{
    this->Release();
}

//------------------------------------------------------------------------------
/**
*/
void uiBattlezone2Password::Init(CDrawBase* pDrawBase)
{
    uiBase::Init(pDrawBase);
    RegisterControl();
    set_party_info_ = NULL;
}

uiBattlezone2Man* uiBattlezone2Password::GetManager()
{
    if (ui_battlezone2_manager_ == NULL)
    {
        ui_battlezone2_manager_ = static_cast<uiBattlezone2Man*>(this->GetUIMan()->GetUserInterfaceManager(UIMAN_BATTLE_ZONE_MANAGER));
        assert (ui_battlezone2_manager_);
    }
    return ui_battlezone2_manager_;
}
//------------------------------------------------------------------------------
/**
*/
void uiBattlezone2Password::Release()
{
    set_party_info_ = NULL;
}
//------------------------------------------------------------------------------
/**
*/
void
uiBattlezone2Password::OnShowWindow( BOOL val )
{
    /*if (val)
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
    }*/
}

//------------------------------------------------------------------------------
/**
*/
void
uiBattlezone2Password::MessageProc( SI_MESSAGE * pMessage )
{
	TCHAR szMessage[INTERFACE_STRING_LENGTH];
    TCHAR temp_string[INTERFACE_STRING_LENGTH];

    switch (pMessage->eResultMsg)
    {
    case RT_MSG_EDIT_ENTERKEYDOWN:
        {
            switch (GetControlPosition_byID(pMessage->dwCtrlID))
            {
            case DLGPOS_EDT_PASSWORD:
                {
                    TCHAR		pszRoomPW[MAX_ROOMPASSWORD_LENGTH+1];
                    CCtrlDxIMEEditBox * pE;

                    //
                    // 배틀존 비밀번호
                    //

                    pE = (CCtrlDxIMEEditBox *)GetControlWZ_byPosition(DLGPOS_EDT_PASSWORD);
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

            DWORD pos = GetControlPosition_byID(pMessage->dwCtrlID);

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

                        pE = (CCtrlDxIMEEditBox *)GetControlWZ_byPosition(DLGPOS_EDT_PASSWORD);
                        pE->GetTextWZ( pszRoomPW, MAX_ROOMPASSWORD_LENGTH+1);
                        int len = _tcslen( pszRoomPW );
                        if( len == 0 )
                        {
                            TCHAR buffer_string[INTERFACE_STRING_LENGTH] = {0,};
							//	비밀번호를 입력 하십시오.
                            if (g_pHero->IsParty())
                            {
                                g_InterfaceManager.GetInterfaceString(eST_INPUT_PASSWORD, temp_string, INTERFACE_STRING_LENGTH);
                                // 방 입장시 현재 파티가 탈퇴 됩니다. 계속 하시겠습니까? 2475
                                g_InterfaceManager.GetInterfaceString(2475, buffer_string, INTERFACE_STRING_LENGTH);
                                Sprintf(szMessage, temp_string, buffer_string);
                            }
                            else
                            {
							    g_InterfaceManager.GetInterfaceString(eST_INPUT_PASSWORD, szMessage, INTERFACE_STRING_LENGTH);
                            }
                            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
							GlobalFunc::SetFocus( (SolarDialog *)this, pE );
                        }
                        else
                        {
							this->m_strPassword = pszRoomPW;

                            uiBattlezone2List* battlezone_list_dialog = 
                                GET_CAST_DIALOG(uiBattlezone2List, IM_BATTLEZONE2_MANAGER::BATTLEZONE2_LIST);

                            if (battlezone_list_dialog)
                            {
							    battlezone_list_dialog->EnterRoom(save_party_key_, m_strPassword.c_str(), set_party_info_);
                            }
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
uiBattlezone2Password::OnUpdateSolarDialog()
{
    //if (GetManager()->IsWait())
	{
		if(!this->HaveFocus())
			ShowInterface(FALSE);
	}
}

void uiBattlezone2Password::Process(DWORD tick_count)
{
}

void uiBattlezone2Password::MouseOver_MsgProc(SI_MESSAGE* message)
{

}
#endif//_DH_BATTLEZONE2_
//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
