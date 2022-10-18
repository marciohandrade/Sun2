#include "SunClientPrecompiledHeader.h"
#include "uicompositeman.h"
#include "interfacemanager.h"
#include "ItemETCCompositeDlg.h"
#include "SelectCompositeMenu.h"
uiCompositeMan::uiCompositeMan(InterfaceManager *pUIMan):
uiBaseMan(pUIMan)
{
    m_pETCComposeDialog = NULL;
    m_pSelectCompositeMenu = NULL;
}

void uiCompositeMan::OnInitialize()
{
    m_pETCComposeDialog = new ItemETCCompositeDlg;
    if( !g_InterfaceManager.LoadInterface( 
        m_pETCComposeDialog, 
        "Data\\Interface\\71_7_socket.iwz", 
        StrToWzID("0717"), 
        FALSE ) )
    {

    }

	m_pETCComposeDialog->SetDialogKey(StrToWzID("0717"));

    m_pSelectCompositeMenu = new SelectCompositeMenu;
    if( !g_InterfaceManager.LoadInterface( 
        m_pSelectCompositeMenu, 
        "Data\\Interface\\71_2_Rank_menu.iwz", 
        StrToWzID("0712"), 
        FALSE ) )
    {

    }
	m_pETCComposeDialog->SetDialogKey(StrToWzID("0712"));
}

