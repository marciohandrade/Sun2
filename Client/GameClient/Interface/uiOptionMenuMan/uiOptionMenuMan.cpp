#include "SunClientPrecompiledHeader.h"
#include "uioptionmenuman.h"
#include "interfacemanager.h"
#include "OptionMenuDialog.h"
#include "OptionDialog.h"
#include "OptionGameDialog.h"
#include "OptionKeyHelpDialog.h"
#include "OptionSoundDialog.h"
#include "uiHotKeyDialog/uiHotKeyDialog.h"

#ifdef _NA_000000_20130114_RENEWER_UI
const DWORD IM_OPTION_MENU_MANAGER::HOT_KEY_BINDING_DIALOG      = StrToWzID("0456");
#else
const DWORD IM_OPTION_MENU_MANAGER::OPTION_MENU_DIALOG			= StrToWzID("0453");
const DWORD IM_OPTION_MENU_MANAGER::OPTION_DIALOG				= StrToWzID("0450");
const DWORD IM_OPTION_MENU_MANAGER::OPTION_GAME_DIALOG			= StrToWzID("0451");
const DWORD IM_OPTION_MENU_MANAGER::OPTION_KEY_HELP_DIALOG		= StrToWzID("0454");
const DWORD IM_OPTION_MENU_MANAGER::OPTION_SOUND_DIALOG			= StrToWzID("0455");
const DWORD IM_OPTION_MENU_MANAGER::HOT_KEY_BINDING_DIALOG      = StrToWzID("0456");
#endif


uiOptionMenuMan::uiOptionMenuMan(InterfaceManager *pUIMan):
uiBaseMan(pUIMan)
{
#ifdef _NA_000000_20130114_RENEWER_UI
#else
    m_pOptionMenuDialog		= NULL;
    m_pOptionDialog			= NULL;
    m_pOptionGameDialog		= NULL;
    m_pOptionKeyHelpDialog	= NULL;
    m_pOptionSoundDialog	= NULL;
#endif

}


void uiOptionMenuMan::OnInitialize()
{
#ifdef _NA_000000_20130114_RENEWER_UI
#else
	m_pOptionMenuDialog		= CreateUIDialog<OptionMenuDialog>( IM_OPTION_MENU_MANAGER::OPTION_MENU_DIALOG, "Data\\Interface\\45_3_Option_Menu.iwz", this, TRUE );
	m_pOptionDialog			= CreateUIDialog<OptionDialog>( IM_OPTION_MENU_MANAGER::OPTION_DIALOG, "Data\\Interface\\45_Option_Window.iwz", this, TRUE );
	m_pOptionGameDialog		= CreateUIDialog<OptionGameDialog>( IM_OPTION_MENU_MANAGER::OPTION_GAME_DIALOG, "Data\\Interface\\45_1_Game_Option.iwz", this, TRUE );
	m_pOptionKeyHelpDialog	= CreateUIDialog<OptionKeyHelpDialog>( IM_OPTION_MENU_MANAGER::OPTION_KEY_HELP_DIALOG, "Data\\Interface\\45_4_Key_Help.iwz", this, TRUE );
	m_pOptionSoundDialog	= CreateUIDialog<OptionSoundDialog>( IM_OPTION_MENU_MANAGER::OPTION_SOUND_DIALOG, "Data\\Interface\\45_5_Sound_Option.iwz", this, TRUE );

    uiBase* ui_dialog = CreateUIDialog<uiHotKeyDialog>(IM_OPTION_MENU_MANAGER::HOT_KEY_BINDING_DIALOG, "Data\\Interface\\45_6_Keybinding.iwz", this, TRUE);
#endif

}


