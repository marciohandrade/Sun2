//------------------------------------------------------------------------------
//  uiLoginAcount_dialogproc.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "uiLoginMan/uiLoginMan.h"
#include "uiLoginOneTimePassword.h"
#include "interfacemanager.h"
#include "globalfunc.h"
#include "uiSystemMan/uiSystemMan.h"

//------------------------------------------------------------------------------
WzID uiLoginOneTimePassword::m_wzId[DIALOG_MAX] = 
{ 
    StrToWzID("E000"),
    StrToWzID("TI00"),
};

//------------------------------------------------------------------------------
/**
*/
uiLoginOneTimePassword::uiLoginOneTimePassword(InterfaceManager *pUIMan)
:uiBase(pUIMan)
{
    m_pEdtControl = NULL;
    m_pBtnControl = NULL;
}

//------------------------------------------------------------------------------
/**
*/
uiLoginOneTimePassword::~uiLoginOneTimePassword()
{

}

//------------------------------------------------------------------------------
/**
*/
void uiLoginOneTimePassword::Init(CDrawBase* pDrawBase)
{
    uiBase::Init(pDrawBase);
    
    // Edit��Ʈ�� �����Ͱ�, �Է±�������
    m_pEdtControl = (CCtrlDxIMEEditBox*)GetDialogWZ()->GetControlWZ(m_wzId[DLGPOS_EDT_OTP]);
    SUN_ASSERT(m_pEdtControl);
    if (m_pEdtControl)
    {
        m_pEdtControl->SetLimitText(MAX_OTP_LENGTH);
    }
}

//------------------------------------------------------------------------------
/**
*/
void uiLoginOneTimePassword::Release()
{
}

//------------------------------------------------------------------------------
/**
*/
void uiLoginOneTimePassword::OnShowWindow(BOOL val)
{
    CDialogManager* pDigMan = g_InterfaceManager.GetDialogManager();
    if (val)
    {
        MoveDefaultWindowPos();

        // ���� �ʱ�ȭ
        if (pDigMan)
        {
            pDigMan->EnableImeSystem(false);
        }
        memset(m_szOTPInput, 0, sizeof(m_szOTPInput));
        if (m_pEdtControl)
        {
            m_pEdtControl->ClearText();
        }        
    }
    else
    {
        if (pDigMan)
        {
            pDigMan->EnableImeSystem(true);
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void uiLoginOneTimePassword::MessageProc( SI_MESSAGE * pMessage )
{
    if (this->IsLock())
        return;

    switch( pMessage->eResultMsg ) 
    {
    case RT_MSG_EDIT_ENTERKEYDOWN:
        {
            // EDIT�κ��� ��Ʈ���� ��� SEND �Լ��� ��Ʈ���� �ѱ�� â�ݱ�
            SendOneTimePassword();
        }
        break;
    case RT_MSG_LBUTTONCLICK:
        {
            if (pMessage->dwCtrlID == m_wzId[DLGPOS_BTN_CONNECT])
            {
                SendOneTimePassword();
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
void uiLoginOneTimePassword::OnUpdateSolarDialog()
{
    // �����̿� �����Է� ����
    m_pEdtControl->GetTextWZA(m_szOTPInput, MAX_OTP_LENGTH);    
    size_t Len = strlen(m_szOTPInput);
    for (size_t index = 0; index < Len; ++index)
    {
        if (isdigit(m_szOTPInput[index]))
        {
            //...
        }
        else
        {
            m_szOTPInput[index] = NULL;
            m_pEdtControl->SetTextWZA(m_szOTPInput);
        }
    }
}


void uiLoginOneTimePassword::SendOneTimePassword()
{
    // EDIT�κ��� ��Ʈ���� ��� SEND �Լ��� ��Ʈ���� �ѱ�� â�ݱ�
    m_pEdtControl->GetTextWZA(m_szOTPInput, MAX_OTP_LENGTH);
    size_t Len = strlen(m_szOTPInput);
    if (Len)
    {
        uiLoginMan* UIMan = (uiLoginMan*)GetMan();
        if (UIMan)
        {
#if defined(__CN_20100909_SECURITY_TOKEN__)
            UIMan->SEND_AUTH_SECURITY_TOKEN_SYN(m_szOTPInput);
#elif defined(_JP000000_ONETIMEPASSWORD_AUTHFLOW_)
            UIMan->SEND_AUTH_OTP_PASS_SYN(m_szOTPInput);
#endif //_JP000000_ONETIMEPASSWORD_AUTHFLOW_ __CN_20100909_SECURITY_TOKEN__
        }
        ShowInterface(FALSE);
    }
    else
    {
    }
}
