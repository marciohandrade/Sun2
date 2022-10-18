#include "SunClientPrecompiledHeader.h"
#include "uiWebDialogMan.h"
#include "uiWebDialog/uiWebDialog.h"
#include "interfacemanager.h"
#ifdef _KIKI_UI_WEB_DIALOG


const DWORD IM_WEB_DIALOG_MANAGER::WEB_DIALOG = StrToWzID("0980");



BOOL uiWebDialogMan::_LoadUI()
{
	m_pUIWebDialog = CreateUIDialog<uiWebDialog>( IM_WEB_DIALOG_MANAGER::WEB_DIALOG, "Data\\Interface\\98_WebDiag.iwz", this );

	return TRUE;
}

#endif