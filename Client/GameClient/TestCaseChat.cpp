#include "SunClientPrecompiledHeader.h"
#include "testcasechat.h"
#include "ChatDialog.h"
#include "interfacemanager.h"
#include "uiChatMan/uiChatMan_def.h"
//-------------------------------------------------------------------------------------------
/**
*/
TestCaseChat::TestCaseChat(void)
{
	this->SetName(_T("CHAT"));
}

//-------------------------------------------------------------------------------------------
/**
*/
TestCaseChat::~TestCaseChat(void)
{
}


//-------------------------------------------------------------------------------------------
/**
*/
void 
TestCaseChat::ProcessKeyInput(DWORD dwTick)
{
	if (g_Input.GetState( DIK_LCONTROL, KS_DOWN ) || g_Input.GetState( DIK_RCONTROL, KS_DOWN ) ||
		g_Input.GetState( DIK_LCONTROL, IKS_DOWN ) || g_Input.GetState( DIK_RCONTROL, IKS_DOWN ) )
	{
		ChatDialog * pDlg = GET_CAST_DIALOG(ChatDialog, IM_CHAT_MANAGER::CHAT_DIALOG);
		if ( pDlg )
		{
			static int i;
			i++;

			TCHAR szText[INTERFACE_STRING_LENGTH];
			Sprintf( szText, _T("TEST CHAT MESSAGE(%d)"), i);

			pDlg->AddString( szText );
		}
	}
}

BOOL TestCaseChat::EntMessage(WORD wMessage, int iParam1, int iParam2, int iParam3/* = 0*/, int iParam4 /*= 0*/)
{
	return TRUE;
}