#include "SunClientPrecompiledHeader.h"
#include "uishopman.h"
#include "interfacemanager.h"
#include "ShopDialog.h"

uiShopMan::uiShopMan(InterfaceManager *pUIMan):
uiBaseMan(pUIMan)
{
    m_pShopDialog = NULL;
}


//------------------------------------------------------------------------------
/**
*/
void	uiShopMan::OnInitialize()
{
    m_pShopDialog = new ShopDialog;
    if( !g_InterfaceManager.LoadInterface( 
        m_pShopDialog, 
        "Data\\Interface\\44_Vendor_Window.iwz", 
        StrToWzID("0440"), 
        FALSE ) )
    {

    }

	m_pShopDialog->SetDialogKey(StrToWzID("0440"));
}
