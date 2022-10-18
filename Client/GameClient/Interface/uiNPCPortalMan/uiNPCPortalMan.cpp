#include "SunClientPrecompiledHeader.h"
#include "uinpcportalman.h"
#include "interfaceManager.h"
#include "NPCPortalDialog.h"

uiNPCPortalMan::uiNPCPortalMan(InterfaceManager *pUIMan):
uiBaseMan(pUIMan)
{
    m_pNPCPortalDialog = NULL;
}


//------------------------------------------------------------------------------
/**
*/
void	uiNPCPortalMan::OnInitialize()
{

    m_pNPCPortalDialog = new NPCPortalDialog;
    if( !g_InterfaceManager.LoadInterface( 
        m_pNPCPortalDialog, 
        "Data\\Interface\\52_Warp_Villige.iwz", 
        StrToWzID("0520"), 
        FALSE ) )
    {

    }
}

