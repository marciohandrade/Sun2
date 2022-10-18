#include "SunClientPrecompiledHeader.h"
#include "uienduranceman.h"
#include "interfacemanager.h"
#include "EnduranceDIalog.h"

uiEnduranceMan::uiEnduranceMan(InterfaceManager *pUIMan):
uiBaseMan(pUIMan)
{
    m_pEnduranceDialog = NULL;
}


//------------------------------------------------------------------------------
/**
*/
void	uiEnduranceMan::OnInitialize()
{
    m_pEnduranceDialog = new EnduranceDIalog;
    if(!g_InterfaceManager.LoadInterface(
        m_pEnduranceDialog,
        "data\\interface\\23_1_endurance.iwz",
        StrToWzID("0231"),
        FALSE ) )
    {

    }

	if(m_pEnduranceDialog)
	{
		m_pEnduranceDialog->SetDialogKey(StrToWzID("0231"));
	}
	
}

