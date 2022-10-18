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
	2009/02/19	���ǰ� �ִ� �ڵ鷯�Լ����� ������ ���⿡ �ϳ��� �Լ��� �����մϴ�.	- ����
*/
void uiLoginMan::HandlerPKB_LOGIN_PROCESS()
{
	TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
	//5019	���������κ��� ������ �����ϴ�.\n\n�ٽ� �õ��� �ֽñ� �ٶ��ϴ�.\n������ ���ĵ�� ����� �˼��մϴ�.
	g_InterfaceManager.GetInterfaceString(eST_NO_RESPONSE_FROM_AUTH_SERVER , szMessage , INTERFACE_STRING_LENGTH );
	
	uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
	if(pSystemMan)
		pSystemMan->Sys_VerifyLock(szMessage , _VerifyCallBack_DoReconnect);

}
