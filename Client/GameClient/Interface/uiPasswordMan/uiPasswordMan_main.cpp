#include "SunClientPrecompiledHeader.h"



#include "uiBaseMan.h"
#include "uiPasswordMan.h"

#include "uiPasswordDlg/uiPasswordDlg.h"

#include "GlobalFunc.h"
#include "ItemManager.h"
#include "interfacemanager.h"
#include "SceneBase.h"

const DWORD c_dwCompareID_Password						= StrToWzID("0016");
const char  c_strDlgFname_Password[]					= ("Data\\Interface\\1_6_VirtualKeyboard.iwz");


uiPasswordMan::uiPasswordMan(InterfaceManager *pUIMan):
	uiBaseMan(pUIMan)
{
}

//------------------------------------------------------------------------------
/**
*/
void uiPasswordMan::OnInitialize()
{
    m_pUIPasswordDlg = CreateUIDialog<uiPasswordDlg>(
        c_dwCompareID_Password, 
        c_strDlgFname_Password, 			
        this);
}

//------------------------------------------------------------------------------
/**
*/
BOOL uiPasswordMan::PasswordDlg(int iStringTableIndex, fn_Password _fn_password, int iMinCount, int iMaxCount, BOOL bShowPassChgBtn, int iCalled )
{
	TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0, };
	if(m_pUIPasswordDlg)
	{
		if(m_pUIPasswordDlg->IsVisibleWindow())
		{
			if(m_pUIPasswordDlg->m_fnPassword)
			{
				m_pUIPasswordDlg->m_fnPassword = NULL;
			}
		}

		m_pUIPasswordDlg->ShowInterface(TRUE);
		m_pUIPasswordDlg->m_fnPassword = _fn_password;
		g_InterfaceManager.GetInterfaceString( iStringTableIndex, szMessage, INTERFACE_STRING_LENGTH );

		m_pUIPasswordDlg->SetPassword(szMessage, iMinCount, iMaxCount, bShowPassChgBtn, iCalled);

		return TRUE;
	}
	return FALSE;
}

