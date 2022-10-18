#include "SunClientPrecompiledHeader.h"

#include "uiItemStatChangeSysMan/uiItemStatChangeSysMan.h"
#include "uiItemStatChangeSys/uiItemStatChangeSys.h"
#include "interfacemanager.h"

const DWORD  IM_ITEM_STAT_CHANGE_MANAGER::ITEM_STAT_CHANGE			= StrToWzID("0766");

uiItemStatChangeSysMan::uiItemStatChangeSysMan(InterfaceManager *pUIMan):
uiBaseMan(pUIMan)
{
}

//------------------------------------------------------------------------------
/**
*/
void uiItemStatChangeSysMan::OnInitialize()
{
	m_pItemStatChangeSysDlg = CreateUIDialog<uiItemStatChangeSys>( IM_ITEM_STAT_CHANGE_MANAGER::ITEM_STAT_CHANGE, "Data\\Interface\\76_6_Item_ChangeUP.iwz", this );
}

