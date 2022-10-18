#include "SunClientPrecompiledHeader.h"
#include "interfacemanager.h"
#include "uiEventMan.h"
#include "uiEventWindow/uiEventWindow.h"
#include "uiEventItemSelect/uiEventItemSelect.h"
#ifdef __NA_001027_20080423_RANDOM_REWARD_EVENT_
#include "uiGuildEventDialog/uiGuildEventDialog.h"
#endif

#include "ui_XMas_Confirm/ui_XMas_Confirm.h"
#include "ui_XMas_SantaDlg/ui_XMas_SantaDlg.h"
#include "ui_XMas_DonationConfirm/ui_XMas_DonationConfirm.h"
#include "ui_XMas_DonationReward/ui_XMas_DonationReward.h"
#include "ui_XMas_SoloCoupleReward/ui_XMas_SoloCoupleReward.h"
#include "ui_XMas_SantaReward/ui_XMas_SantaReward.h"
#include "uiBuffEvent/uiBuffEvent.h"
#include "uiEventFriendWindow.h"

const DWORD IM_EVENT_MANAGER::EVENT_WINDOW						= StrToWzID("0990");

const DWORD IM_EVENT_MANAGER::EVENT_ITEM_SELECT					= g_IDGenerator.GenerateID();

#ifdef __NA_001027_20080423_RANDOM_REWARD_EVENT_
const DWORD IM_EVENT_MANAGER::GUILD_EVENT						= StrToWzID("7317");
#endif

const DWORD IM_EVENT_MANAGER::EVENT_X_MAS_CONFIRM				= StrToWzID("0994");
const DWORD IM_EVENT_MANAGER::EVENT_X_MAS_SANTA_DLG				= StrToWzID("0995");
const DWORD IM_EVENT_MANAGER::EVENT_X_MAS_DONATION_CONFIRM		= StrToWzID("0996");
const DWORD IM_EVENT_MANAGER::EVENT_X_MAS_DONATION_REWARD		= StrToWzID("0997");
const DWORD IM_EVENT_MANAGER::EVENT_X_MAS_SOLO_COUPLE_REWARD	= StrToWzID("0998");
const DWORD IM_EVENT_MANAGER::EVENT_X_MAS_SANTA_REWARD			= StrToWzID("0999");
const DWORD IM_EVENT_MANAGER::EVENT_BUFF						= StrToWzID("9910");
const DWORD  IM_EVENT_MANAGER::EVENT_FRIEND						= g_IDGenerator.GenerateID();

BOOL uiEventMan::_LoadUI()
{
	m_pUIEventWindow			= CreateUIDialog<uiEventWindow>( IM_EVENT_MANAGER::EVENT_WINDOW, "Data\\Interface\\99_Event.iwz", this );

	m_pUIEventItemSelect		= CreateUIDialog<uiEventItemSelect>( IM_EVENT_MANAGER::EVENT_ITEM_SELECT, "Data\\Interface\\99_1_Event_Itemselect.iwz", this, FALSE, TRUE );

#ifdef __NA_001027_20080423_RANDOM_REWARD_EVENT_
	m_pUIGuildEventWindow		= CreateUIDialog<uiGuildEventDialog>( IM_EVENT_MANAGER::GUILD_EVENT, "Data\\Interface\\73_17_GuildWarManager.iwz", this );
#endif
	m_pUI_XMas_Confirm			= CreateUIDialog<ui_XMas_Confirm>( IM_EVENT_MANAGER::EVENT_X_MAS_CONFIRM, "Data\\Interface\\99_4_Event_Chris.iwz", this );
	m_pUI_XMas_SantaDlg			= CreateUIDialog<ui_XMas_SantaDlg>( IM_EVENT_MANAGER::EVENT_X_MAS_SANTA_DLG, "Data\\Interface\\99_5_Event_Chris.iwz", this );
	m_pUI_XMas_DonationConfirm	= CreateUIDialog<ui_XMas_DonationConfirm>( IM_EVENT_MANAGER::EVENT_X_MAS_DONATION_CONFIRM, "Data\\Interface\\99_6_Event_Chris.iwz", this );
	m_pUI_XMas_DonationReward	= CreateUIDialog<ui_XMas_DonationReward>( IM_EVENT_MANAGER::EVENT_X_MAS_DONATION_REWARD, "Data\\Interface\\99_7_Event_Chris.iwz", this );
	m_pUI_XMas_SoloCoupleReward	= CreateUIDialog<ui_XMas_SoloCoupleReward>( IM_EVENT_MANAGER::EVENT_X_MAS_SOLO_COUPLE_REWARD, "Data\\Interface\\99_8_Event_Chris.iwz", this );
	m_pUI_XMas_SantaReward		= CreateUIDialog<ui_XMas_SantaReward>( IM_EVENT_MANAGER::EVENT_X_MAS_SANTA_REWARD, "Data\\Interface\\99_9_Event_Chris.iwz", this );
	m_pUIBuffEvent				= CreateUIDialog<uiBuffEvent>( IM_EVENT_MANAGER::EVENT_BUFF, "Data\\Interface\\99_10_Event_Crystal.iwz", this );
	m_pUIFriendEvent			= CreateUIDialog<uiEventFriend>( IM_EVENT_MANAGER::EVENT_FRIEND, "Data\\Interface\\99_1_Event_Itemselect.iwz", this, FALSE, TRUE );

	return TRUE;
}

