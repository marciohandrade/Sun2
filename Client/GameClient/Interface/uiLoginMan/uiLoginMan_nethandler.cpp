//------------------------------------------------------------------------------
//  uiLoginMan_nethandler.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "uiLoginMan.h"
#include "uiLoginAccount/uiLoginAccount.h"
#include "interfacemanager.h"
#include "uiSystemMan/uiSystemMan.h"

//------------------------------------------------------------------------------
/**
*/
void
uiLoginMan::HandlerPKBooleans()
{
    int timeOutPacketIdx = TimeOutPKBooleans();

    if (!timeOutPacketIdx)
        return;

    switch (timeOutPacketIdx)
    {
    case ePKBOOL_LOGIN_C2S_ASK_VERIFY:
	case ePKBOOL_LOGIN_C2S_ASK_AUTH:
	case ePKBOOL_LOGIN_C2S_ASK_SVRLIST:
	case ePKBOOL_LOGIN_S2C_SERVER_NAME_LIST:
	case ePKBOOL_LOGIN_C2S_ASK_SVRSELECT:
		HandlerPKB_LOGIN_PROCESS();
		break;

    default:
        assert (!"packet has been timeout!!");
        break;
    }
}

//------------------------------------------------------------------------------
/**
*/
void _VerifyCallBack_DoReconnect(void)
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
    no response to auth.
    so, we bring back to account required.
	2009/02/19	사용되고 있던 핸들러함수들의 내용이 같기에 하나의 함수로 통일합니다.	- 태준
*/
void uiLoginMan::HandlerPKB_LOGIN_PROCESS()
{
	TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
	//5019	인증서버로부터 응답이 없습니다.\n\n다시 시도해 주시기 바랍니다.\n불편을 끼쳐드려 대단히 죄송합니다.
	g_InterfaceManager.GetInterfaceString(eST_NO_RESPONSE_FROM_AUTH_SERVER , szMessage , INTERFACE_STRING_LENGTH );
	
	uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
	if(pSystemMan)
		pSystemMan->Sys_VerifyLock(szMessage , _VerifyCallBack_DoReconnect);

}
