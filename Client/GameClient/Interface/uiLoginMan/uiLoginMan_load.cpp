//------------------------------------------------------------------------------
//  uiLoginMan_load.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "uiLoginMan.h"
#include "uiLoginBack/uiLoginBack.h"
#include "uiLoginBack/uiLoginBackTop.h"
#include "uiLoginAccount/uiLoginAccount.h"
#include "uiLoginServerList/uiLoginServerList.h"
#include "uiLoginChannelList/uiLoginChannelList.h"
#include "uiLoginServerChannelList/uiLoginServerChannelList.h"
#include "uiLogin15Year/uiLogin15Year.h"
#if defined(_JP000000_ONETIMEPASSWORD_AUTHFLOW_) || defined(__CN_20100909_SECURITY_TOKEN__)
#include "uiLoginOneTimePassword/uiLoginOneTimePassword.h"
#endif //_JP000000_ONETIMEPASSWORD_AUTHFLOW_ __CN_20100909_SECURITY_TOKEN__

const DWORD IM_LOGIN_MANAGER::LOGIN_BACK		    = StrToWzID("0010");
const DWORD IM_LOGIN_MANAGER::LOGIN_ACCOUNT		    = StrToWzID("0011");
const DWORD IM_LOGIN_MANAGER::LOGIN_SERVERLIST	    = StrToWzID("0020");
const DWORD IM_LOGIN_MANAGER::LOGIN_CHANNELLIST	    = StrToWzID("0022");
const DWORD IM_LOGIN_MANAGER::LOGIN_15YEAR		    = StrToWzID("0013");
#if defined(_JP000000_ONETIMEPASSWORD_AUTHFLOW_) || defined(__CN_20100909_SECURITY_TOKEN__)
const DWORD IM_LOGIN_MANAGER::LOGIN_ONETIMEPASSWORD	= StrToWzID("0019");
#endif //_JP000000_ONETIMEPASSWORD_AUTHFLOW_ __CN_20100909_SECURITY_TOKEN__

//------------------------------------------------------------------------------
/**
*/
BOOL uiLoginMan::_LoadUI( void )
{
	m_pUILoginBack			= CreateUIDialog<uiLoginBack>( IM_LOGIN_MANAGER::LOGIN_BACK, c_strDlgFname_LoginBack, this );
    SUN_ASSERT(m_pUILoginBack);
#ifdef _COMMON_CHANNELING_LOGIC_DEFINE
#else
	m_pUILoginAccount		= CreateUIDialog<uiLoginAccount>( IM_LOGIN_MANAGER::LOGIN_ACCOUNT, c_strDlgFname_LoginAccount, this );
    SUN_ASSERT(m_pUILoginAccount);
#endif //_COMMON_CHANNELING_LOGIC_DEFINE

    m_pUILoginBackTop = CreateUIDialog<uiLoginBackTop>(uiLoginBackTop::kDialog_ID, "Data\\Interface\\2_3_Login_Window.iwz", this );
    SUN_ASSERT(m_pUILoginBackTop);
    m_pUILoginServerList	= CreateUIDialog<uiLoginServerChannelList>( IM_LOGIN_MANAGER::LOGIN_SERVERLIST, c_strDlgFname_LoginServerList, this );
    SUN_ASSERT(m_pUILoginServerList);
	m_pUILogin15Year		= CreateUIDialog<uiLogin15Year>( IM_LOGIN_MANAGER::LOGIN_15YEAR, c_strDlgFname_Login15year, this );
    SUN_ASSERT(m_pUILogin15Year);
#if defined(_JP000000_ONETIMEPASSWORD_AUTHFLOW_) || defined(__CN_20100909_SECURITY_TOKEN__)
    m_pUILoginOneTimePassword = CreateUIDialog<uiLoginOneTimePassword>( IM_LOGIN_MANAGER::LOGIN_ONETIMEPASSWORD, c_strDlgFname_LoginOneTimePassword, this );
    SUN_ASSERT(m_pUILoginOneTimePassword);
#endif //_JP000000_ONETIMEPASSWORD_AUTHFLOW_ __CN_20100909_SECURITY_TOKEN__

	return TRUE;
}

//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------