#include "SunClientPrecompiledHeader.h"

#include "uiPcbangPlayTimeMan.h"
#include "interfacemanager.h"
#include "PcbangPlayTimeDialog.h"

uiPcbangPlayTimeMan::uiPcbangPlayTimeMan(InterfaceManager *pUIMan):
uiBaseMan(pUIMan)

{
    m_pPcbangPlayTimeDialog = NULL;
}

//------------------------------------------------------------------------------
/**
*/
void	uiPcbangPlayTimeMan::OnInitialize()
{

    m_pPcbangPlayTimeDialog =new PcbangPlayTimeDialog;
    if( !g_InterfaceManager.LoadInterface( 
        m_pPcbangPlayTimeDialog, 
        "Data\\Interface\\99_2_Event_Clock.iwz", 
        StrToWzID("0992"), 
        FALSE ) )
    {

    }
}
