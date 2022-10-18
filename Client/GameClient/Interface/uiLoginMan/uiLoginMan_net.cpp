//------------------------------------------------------------------------------
//  uiLoginMan_net.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "uiLoginMan.h"
#include "uiLoginBack/uiLoginBack.h"
#include "uiLoginAccount/uiLoginAccount.h"
#include "uiLoginServerList/uiLoginServerList.h"
#include "uiLoginChannelList/uiLoginChannelList.h"
#include "uiLoginBack/uiLoginBackTop.h"
#include "uiLoginServerChannelList/uiLoginServerChannelList.h"

#include "LoginScene.h"
#include "LoadScene.h"
#include "interfacemanager.h"
#ifdef _KIKI_UI_WEB_DIALOG
#include "uiWebDialogMan/uiWebDialogMan.h"
#endif
#include "globalfunc.h"
#include "uiSystemMan/uiSystemMan.h"
#ifdef __CN_269_MATRIX_CARD_AUTH
#include "MatrixCardAuth.h"
#endif
#ifdef _JP000000_ONETIMEPASSWORD_AUTHFLOW_
#include "uiLoginOneTimePassword/uiLoginOneTimePassword.h"
#endif //_JP000000_ONETIMEPASSWORD_AUTHFLOW

VOID _fn_loginReconnect2(void)
{
    g_LoginScene.Reconnect();
}

//------------------------------------------------------------------------------
/**
*/
void
uiLoginMan::InitPKBooleans()
{
    this->m_vPKBool.clear();

    for (int i=ePKBOOL_LOGIN_BASE; i<ePKBOOL_LOGIN_MAX; ++i)
    {
        uicPacketControl packetControl;
        this->m_vPKBool.push_back(packetControl);
    }
}

//------------------------------------------------------------------------------
/**
*/
void _VerifyCallBack_TryReconnect(void)
{
    uiLoginMan* loginMan =
        static_cast<uiLoginMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_LOGIN));
    if (loginMan)
    {
        loginMan->resetTryReconnect();
    }
}

//------------------------------------------------------------------------------
/**
*/
void
uiLoginMan::resetTryReconnect()
{
    this->Clear();
    m_bTryReconnect = false;
    m_bAuthFinished = false;
    m_bAuthConnected = false;
    this->m_pUILoginBack->ShowInterface(FALSE);
#ifdef _COMMON_CHANNELING_LOGIC_DEFINE
    // no operation
#else //_COMMON_CHANNELING_LOGIC_DEFINE
	this->m_pUILoginAccount->ShowInterface(FALSE);
#endif //_COMMON_CHANNELING_LOGIC_DEFINE
#ifdef _JP000000_ONETIMEPASSWORD_AUTHFLOW_
    m_pUILoginOneTimePassword->SetOTPUserStamp(uiLoginOneTimePassword::NoneStamp);
    m_pUILoginOneTimePassword->ShowInterface(FALSE);
#endif //_JP000000_ONETIMEPASSWORD_AUTHFLOW
    this->m_pUILoginServerList->ShowInterface(FALSE);

    m_pUILoginBackTop->ShowInterface(TRUE);

    this->m_pUILoginBack->ShowInterface(TRUE);
    //this->m_pUILoginAccount->ShowInterface(TRUE);
    g_LoginScene.Reconnect();
}
//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
