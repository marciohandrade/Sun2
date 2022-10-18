#include "SunClientPrecompiledHeader.h"
#include "uimenuman.h"
#include "interfacemanager.h"
#include "MainDialog.h"
#include "MainMenuOptionDlg.h"

#ifdef _NA_000000_20130114_RENEWER_UI
#include "MainDialog2.h"
#endif

#include "GlobalFunc.h"
#include "SystemDialog.h"

#include "SUNCredit/CreditDialog.h"

#include "uiTutorialSkip.h"

const DWORD IM_MENU_MANAGER::MAIN_DLG				= StrToWzID("0240");
const DWORD IM_MENU_MANAGER::MAIN_DLG_2				= StrToWzID("0250");
const DWORD IM_MENU_MANAGER::MAIN_MENU_OPTION_DLG	= StrToWzID("0241");
const DWORD IM_MENU_MANAGER::SYSTEM_DLG				= StrToWzID("0800");
const DWORD IM_MENU_MANAGER::CREDIT_DLG				= g_IDGenerator.GenerateID();


uiMenuMan::uiMenuMan(InterfaceManager *pUIMan):
uiBaseMan(pUIMan)
{
    m_pMainDlg				= NULL;
    m_pMainOptionDlg		= NULL;
    m_pSystemDlg			= NULL;
}

void uiMenuMan::OnInitialize()
{
    m_pMainDlg = new MainDialog;
    if( !g_InterfaceManager.LoadInterface( 
        m_pMainDlg, 
        "Data\\Interface\\24_Menu.iwz", 
        StrToWzID("0240"), 
        FALSE ) )
    {

    }
    m_pMainDlg->SetDialogKey(StrToWzID("0240"));

    m_pMainOptionDlg = new MainMenuOptionDlg;
    if( !g_InterfaceManager.LoadInterface( 
        m_pMainOptionDlg, 
        "Data\\Interface\\24_1_Menu_Option.iwz", 
        StrToWzID("0241"), 
        FALSE ) )
    {
    }

    m_pSystemDlg = new SystemDialog;
    if (!g_InterfaceManager.LoadInterface( 
        m_pSystemDlg, 
        "Data\\Interface\\80_loggout.iwz", 
        StrToWzID("0800"), 
        FALSE))
    {
    }


    m_pCreditDlg = CreateUIDialog<CreditDialog>( 
        IM_MENU_MANAGER::CREDIT_DLG,
        "Data\\Interface\\46_Full_Map.iwz",
        this, TRUE , TRUE);

#ifdef _YJW_TUTORIAL_SKIP
    CreateUIDialog<uiTutorialSkip>(uiTutorialSkip::kDialog_ID, "Data\\Interface\\24_3_Skip.iwz", this, FALSE);
#endif //_YJW_TUTORIAL_SKIP

#ifdef _NA_000000_20130114_RENEWER_UI
	m_pMaindialog2 = new MainDialog2();
	if( !g_InterfaceManager.LoadInterface( 
		m_pMaindialog2,
		"Data\\Interface\\25_Menu.iwz", 
		StrToWzID("0250"), 
		FALSE ) )
	{

	}
	m_pMaindialog2->SetDialogKey(StrToWzID("0250"));
#endif

}

//------------------------------------------------------------------------------
/**
*/
void	uiMenuMan::Send_CG_ZONE_EX_TUTORIAL_FIELD_LEAVE_SYN()
{
	MSG_CG_ZONE_EX_TUTORIAL_FIELD_LEAVE_SYN sync;
	GlobalFunc::SendPacket(CI_GAMESERVERIDX, &sync, sizeof (sync));
}

//------------------------------------------------------------------------------
/**
*/
void	uiMenuMan::Send_CG_ZONE_EX_TUTORIAL_MISSION_LEAVE_SYN()
{
	MSG_CG_ZONE_EX_TUTORIAL_MISSION_LEAVE_SYN sync;
	GlobalFunc::SendPacket(CI_GAMESERVERIDX, &sync, sizeof (sync));
}
#ifdef _NA_000000_20130114_RENEWER_UI
void uiMenuMan::KeyModifyChange()
{
	if (m_pMainDlg)
	{
		m_pMainDlg->CheckModifyOption();
	}

	if (m_pMaindialog2)
	{
		m_pMaindialog2->CheckModifyOption();
	}

	if (m_pMainOptionDlg)
	{
		m_pMainOptionDlg->CheckModifyOption();
	}

	if(m_pSystemDlg)
	{
		m_pSystemDlg->CheckModifyOption();
	}
	
}
#endif