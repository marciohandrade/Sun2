#include "SunClientPrecompiledHeader.h"
#include "uiCharPopupMan.h"
#include "uiCharPopupMenu/uiCharPopupMenu.h"

//------------------------------------------------------------------------------
/**
*/
const DWORD IM_CHAR_POPUP_MANAGER::CHAR_POPUP_MENU = StrToWzID("0271");

BOOL uiCharPopupMan::_LoadUI()
{
	m_pUICharPopupMenu = CreateUIDialog<uiCharPopupMenu>(
							IM_CHAR_POPUP_MANAGER::CHAR_POPUP_MENU, 
							"Data\\Interface\\27_1_Personal_Option.iwz",
							this);
							
	if (m_pUICharPopupMenu == NULL)
	{
		assert("ĳ���� �˾��޴� UI �ε����");
		return FALSE;
	}
	return TRUE;
}
