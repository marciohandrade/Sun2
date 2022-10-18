//------------------------------------------------------------------------------
//  uiLoginAcount_dialogproc.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "uiLoginAccount.h"
#include "uiLoginMan/uiLoginMan.h"
#include "interfacemanager.h"
#include "globalfunc.h"
#include "uiSystemMan/uiSystemMan.h"

#include "LoginScene.h"

//------------------------------------------------------------------------------
WzID uiLoginAccount::m_wzId[DIALOG_MAX] = 
{ 
    StrToWzID("TI00"),  // DLGPOS_BTN_CONNECT=0,
    StrToWzID("E000"),  // DLGPOS_EDT_ID,
    StrToWzID("E001"),  // DLGPOS_EDT_PW,
    StrToWzID("L000"),  // DLGPOS_LST_VERSION
};

//------------------------------------------------------------------------------
/**
*/
void
uiLoginAccount::OnShowWindow(BOOL val)
{
    if (val)
    {
#if defined(_APPLY_RUSSIA_INGAMBA_CHANNELING)
    #if defined(_SUNGAME_DUMP_VER) || defined(_SUNGAME_VER)
        GlobalFunc::LogOutGame();
    #endif //defined(_SUNGAME_DUMP_VER) || defined(_SUNGAME_VER)
#endif //defined(_APPLY_RUSSIA_INGAMBA_CHANNELING)
        if (!this->CheckControls())
            return;

		//MoveWindowWZ( 0, 0 );

		GlobalFunc::SetFocus( (SolarDialog *)this, m_pEdtControls[0] );

#ifdef _JP000000_ONETIMEPASSWORD_AUTHFLOW_
        this->ConnectBtnOn();
#else
        if (this->GetManager()->IsAuthFinished())
        {
            this->GetManager()->autoexecLoginQuery();
        }
        else
        {
            this->ConnectBtnOn();
        }
#endif //_JAPAN, _JP000000_ONETIMEPASSWORD_AUTHFLOW_        

        m_fTrans = 0.0f;
        m_dwTransAcc = 0;
#ifdef _DEBUG
        m_fTrans = 1.0f;
#endif //_DEBUG
		m_dwTransEnd = uiLogineAsigned::is_show_count_Account_ > 1 ? (SEC_PER_TICK) : 3500/*frame per tick*/ ;
		hide_check_mouse_ = false;

		uiLogineAsigned::is_show_count_Account_++;
    }
    else
    {
    }
}

//------------------------------------------------------------------------------
/**
*/
void
uiLoginAccount::MessageProc( SI_MESSAGE * pMessage )
{
    if (this->IsLock())
        return;

	
    switch( pMessage->eResultMsg ) 
    {
    case RT_MSG_EDIT_TABKEYDOWN:
        {
            switch( getCtrlIDToPos( pMessage->dwCtrlID ) )
            {
            case DLGPOS_EDT_ID:
                {
					GlobalFunc::SetFocus( (SolarDialog *)this, m_pEdtControls[1] );
                }
                break;
            case DLGPOS_EDT_PW:
                {
					GlobalFunc::SetFocus( (SolarDialog *)this, m_pEdtControls[0] );
                }
                break;
            }
        }
        break;

    case RT_MSG_EDIT_ENTERKEYDOWN:
        {
            switch( getCtrlIDToPos( pMessage->dwCtrlID ) )
            {
            case DLGPOS_EDT_ID:
            case DLGPOS_EDT_PW:
                {
					connectLogin();
                }
                break;
            }
        }
        break;

    case RT_MSG_LBUTTONCLICK:
        {
            switch( getCtrlIDToPos( pMessage->dwCtrlID ) )
            {
            case DLGPOS_BTN_CONNECT:
                {
                    connectLogin();
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
/**
*/
void
uiLoginAccount::Process(DWORD dwTick)
{
	

    if (!this->IsVisible())
		return;

    m_dwTransAcc += dwTick;

    if (m_dwTransAcc >= m_dwTransEnd)
    {
        float fPercent =
            ((float)(m_dwTransAcc - m_dwTransEnd) / (float)m_dwTransEnd);

        float fTrans = 1.0f - fPercent;
        if( fTrans < 0.0f )
            fTrans = 0.0f;

        m_fTrans = 1.0f - (fTrans * 1.0f);
    }

	if(hide_check_mouse_)
	{

		float SetAlpah = 1.0f - m_fTrans;
		SetAlpah = SetAlpah <= 0.0f ? 0.0f : SetAlpah;

		GetDialogWZ()->SetTransparency(SetAlpah);

		if(SetAlpah == 0.0f)
		{
			this->ShowInterface(FALSE);
		}

		return;
	}


    GetDialogWZ()->SetTransparency(m_fTrans);
}

//------------------------------------------------------------------------------
/** 중복구문이라서 함수로 분리(ENTER,접속버튼눌렀을때 인증서버에 접속하는구문)
*/
void
uiLoginAccount::connectLogin()
{
	TCHAR   lpszTxtID[INTERFACE_STRING_LENGTH];
	TCHAR   lpszTxtPW[INTERFACE_STRING_LENGTH];

#ifdef __CH1305_090303_ACCOUNT_ID_LENGTH_EXTENTION__
	GlobalFunc::GetText(this->m_pEdtControls[0], lpszTxtID, ID_MAX_LEN+1);
	GlobalFunc::GetText(this->m_pEdtControls[1], lpszTxtPW, PASSWD_MAX_LEN+1);
#else
	GlobalFunc::GetText(this->m_pEdtControls[0], lpszTxtID, ID_MAX_LEN);
	GlobalFunc::GetText(this->m_pEdtControls[1], lpszTxtPW, PASSWD_MAX_LEN);
#endif//__CH1305_090303_ACCOUNT_ID_LENGTH_EXTENTION__
	if (lstrlen(lpszTxtID)==0)
	{

	}
	else if (lstrlen(lpszTxtPW)==0)
	{

	}
	else
	{
		if (this->GetManager()->execLoginQuery(lpszTxtID, lpszTxtPW))
		{

#ifdef _NHN_USA_CHANNELING
			GlobalFunc::SendHanReportPacket(310);
#endif//_NHN_USA_CHANNELING(북미 채널링:지표)
	
			if (GENERALPARAM->IsNet())
				this->ConnectBtnOff();
			else
				ShowInterface(FALSE);

            g_LoginScene.SetLoginProgressState(1);
		}
	}
}


//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
