#include "SunClientPrecompiledHeader.h"
#include "uichatman.h"
#include "interfacemanager.h"
#include "ChatMenu.h"
#include "ChatDialog.h"
#include "SystemChatDialog.h"
#include "ChatPersonalOption.h"

const DWORD	IM_CHAT_MANAGER::CHAT_DIALOG				= StrToWzID("0200");
const DWORD	IM_CHAT_MANAGER::CHAT_MENU_DIALOG			= StrToWzID("0201");
const DWORD	IM_CHAT_MANAGER::SYSTEM_CHAT_DIALOG			= StrToWzID("0203");
const DWORD	IM_CHAT_MANAGER::CHAT_PROPERTY_DIALOG		= StrToWzID("0204");
const DWORD	IM_CHAT_MANAGER::CHAT_PERSONAL_OPTION		= StrToWzID("0206");

uiChatMan::uiChatMan(InterfaceManager *pUIMan):
uiBaseMan(pUIMan)
{
    m_pChatMenuDlg		  = NULL;
    m_pChatDlg			  = NULL;
    m_pSystemChatDlg	  = NULL;
    m_pChatPersonalOption = NULL;
}

void uiChatMan::OnInitialize()
{
    m_pChatMenuDlg = CreateUIDialog<ChatMenuDialog>(
        IM_CHAT_MANAGER::CHAT_MENU_DIALOG, 
        "Data\\Interface\\20_1_Chat_menu.iwz", 
        this, TRUE);
    assert(m_pChatMenuDlg);

    m_pSystemChatDlg = CreateUIDialog<SystemChatDialog>(
        IM_CHAT_MANAGER::SYSTEM_CHAT_DIALOG, 
        "data\\interface\\20_3_system_message.iwz", 
        this, TRUE);
    assert(m_pSystemChatDlg);

    m_pChatProperty = CreateUIDialog<ChatPropertyDialog>(
        IM_CHAT_MANAGER::CHAT_PROPERTY_DIALOG, 
        "Data\\Interface\\20_4_Chat_menu.iwz", 
        this, TRUE);
    assert(m_pChatProperty);

    m_pChatPersonalOption = CreateUIDialog<ChatPersonalOption>(
        IM_CHAT_MANAGER::CHAT_PERSONAL_OPTION, 
        "Data\\Interface\\20_6_Chat_Personal_Option.iwz", 
        this, TRUE);
    assert(m_pChatPersonalOption);

    m_pChatDlg = CreateUIDialog<ChatDialog>(
        IM_CHAT_MANAGER::CHAT_DIALOG, 
        "Data\\Interface\\20_0_Chat.iwz", 
        this, TRUE);
    assert(m_pChatDlg);
}


//------------------------------------------------------------------------------
/**
*/
void uiChatMan::MoveChatDialog(int value)
{
    int gap_value = 0;
    if (value == 1)
    {
        gap_value = c_iOneLineYPostionToQuickSlot;
    }
    else// value = 2
    {
        gap_value = c_iTwoLineYPostionToQuickSlot;
    }

    if (m_pChatDlg != NULL)
    {
        m_pChatDlg->SetGapOfDialogYPosition(gap_value);
        m_pChatDlg->MoveDefaultPosition();
    }
    if (m_pSystemChatDlg != NULL)
    {
#ifdef _NA_008283_20150511_NOTIFICATIONS_SYSTEM
		m_pSystemChatDlg->SetGapOfDialogYPosition(c_iTwoLineYPostionToQuickSlot + c_iAddYSizeofSystemChat);
#else
        m_pSystemChatDlg->SetGapOfDialogYPosition(gap_value + c_iAddYSizeofSystemChat);
#endif // _NA_008283_20150511_NOTIFICATIONS_SYSTEM
        m_pSystemChatDlg->MoveDefaultPosition();
    }
    if (m_pChatProperty != NULL)
    {
        m_pChatProperty->MoveDefaultPosition();
    }

    if (m_pChatPersonalOption != NULL)
    {
        m_pChatPersonalOption->MoveDefaultPosition();
    }
}
