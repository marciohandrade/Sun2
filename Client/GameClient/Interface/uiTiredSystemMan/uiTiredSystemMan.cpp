#include "SunClientPrecompiledHeader.h"
#include "uitiredsystemman.h"
#include "interfacemanager.h"
#include "TiredSystemDialog.h"

const DWORD IM_TIRED_SYSTEM_MANAGER::TIRED_SYSTEM = StrToWzID("0212");

uiTiredSystemMan::uiTiredSystemMan(InterfaceManager *pUIMan):
uiBaseMan(pUIMan)
{
    m_pTiredSystemDialog = NULL;
}


//------------------------------------------------------------------------------
/**
*/
void	uiTiredSystemMan::OnInitialize(void)
{
	  m_pTiredSystemDialog = CreateUIDialog<TiredSystemDialog>(
								IM_TIRED_SYSTEM_MANAGER::TIRED_SYSTEM,
								"Data\\Interface\\21_2_Clock.iwz", 
								this, TRUE);
	  assert(m_pTiredSystemDialog);
}

