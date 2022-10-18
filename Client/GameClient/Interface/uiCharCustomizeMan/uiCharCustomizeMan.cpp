#include "SunClientPrecompiledHeader.h"
#include "uiCharCustomizeMan.h"
#include "InterfaceManager.h"
#include "uiCharCustomizeDlg/uiCharCustomizeDlg.h"

const DWORD IM_CHAR_CUSTOMIZE_MGR::CHAR_CUSTOMIZE_DLG = StrToWzID("0370");

uiCharCustomizeMan::uiCharCustomizeMan(InterfaceManager * pUIMAN) :uiBaseMan(pUIMAN)
{
}

//------------------------------------------------------------------------------
/**
*/
void uiCharCustomizeMan::OnInitialize()
{
    CreateUIDialog<uiCharCustomizeDlg>(IM_CHAR_CUSTOMIZE_MGR::CHAR_CUSTOMIZE_DLG, 
                                       "Data\\Interface\\3_7_Character_Custom.iwz", this);
}
