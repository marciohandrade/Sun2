#pragma once
#include "uiBaseMan.h"
#include "uiChatMan/uiChatMan_def.h"

class ChatMenuDialog;
class ChatDialog;
class SystemChatDialog;
class ChatPropertyDialog;
class ChatPersonalOption;
class uiChatMan :
	public uiBaseMan
{
public:
	uiChatMan(InterfaceManager *pUIMan);

public:
    void                    OnInitialize();

public:
	ChatMenuDialog*			m_pChatMenuDlg;
	ChatDialog*				m_pChatDlg;
	SystemChatDialog *		m_pSystemChatDlg;
	ChatPropertyDialog *	m_pChatProperty;
	ChatPersonalOption*     m_pChatPersonalOption;
    void                    MoveChatDialog(int value);
};
