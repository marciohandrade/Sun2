#include "SunClientPrecompiledHeader.h"

#include "interfacemanager.h"
#include "uiFateItemSystem/uiFateItemSystem.h"
#include "uiFateItemSystemMan/uiFateItemSystemMan.h"
 
const DWORD IM_FATEITEM_SYSTEM::FATEITEM_DLG = StrToWzID("0741");

uiFateItemSystemMan::uiFateItemSystemMan(InterfaceManager *pUIMan):
uiBaseMan(pUIMan)
{
    m_pUIFateItemSystem = NULL;
}

//------------------------------------------------------------------------------
/**
*/
void uiFateItemSystemMan::OnInitialize()
{
	m_pUIFateItemSystem		= CreateUIDialog<uiFateItemSystem>(IM_FATEITEM_SYSTEM::FATEITEM_DLG,"Data\\Interface\\74_1_Fate.iwz", this ); 
}
