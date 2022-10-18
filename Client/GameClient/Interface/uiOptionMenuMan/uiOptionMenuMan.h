#pragma once
#include "uiBaseMan.h"


struct IM_OPTION_MENU_MANAGER
{
#ifdef _NA_000000_20130114_RENEWER_UI
#else
	static const DWORD OPTION_MENU_DIALOG;
	static const DWORD OPTION_DIALOG;
	static const DWORD OPTION_GAME_DIALOG;
	static const DWORD OPTION_KEY_HELP_DIALOG;
	static const DWORD OPTION_SOUND_DIALOG;
#endif
    static const DWORD HOT_KEY_BINDING_DIALOG;
};

class OptionMenuDialog;
class OptionDialog;
class OptionGameDialog;
class OptionKeyHelpDialog;
class OptionSoundDialog;

class uiOptionMenuMan : public uiBaseMan
{
public:
	uiOptionMenuMan(InterfaceManager *pUIMan);

public:
	virtual void			OnInitialize();

private:
#ifdef _NA_000000_20130114_RENEWER_UI
#else
	OptionMenuDialog *		m_pOptionMenuDialog;
	OptionDialog *			m_pOptionDialog;
	OptionGameDialog *		m_pOptionGameDialog;
	OptionKeyHelpDialog *	m_pOptionKeyHelpDialog;
	OptionSoundDialog *		m_pOptionSoundDialog;
#endif
};

