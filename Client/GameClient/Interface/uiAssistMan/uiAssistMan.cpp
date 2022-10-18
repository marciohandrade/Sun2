#include "SunClientPrecompiledHeader.h"
#include "uiassistman.h"
#include "interfacemanager.h"
#include "AskReplyGuildDialog.h"
#include "AskExtraStoneUse.h"
#include "AskSocketFillDialog.h"
#include "AskNPCPortal.h"
#include "AskNormalEnchantDialog.h"
#include "AskEtherDevieDialog.h"
#include "AskRiderWaxGloss.h"

#include "AskSSQObserverDialog.h"
#ifdef _NA_0_20100115_HEIM_LOTTO_TICKET_PURCHASE
#include "AskBuyLottoTicket.h"
#endif //_NA_0_20100115_HEIM_LOTTO_TICKET_PURCHASE

#include "AskCharCustomize.h"
#include "uiCharCustomizeDlg/uiCharCustomizeDlg.h"

#include "AskAirShipPortal.h"

#include "AskItemCustomize.h"

#include "uiAskSocketIdentifyDialog/uiAskSocketIdentifyDialog.h"
#include "uiAskSocketRecreateDialog/uiAskSocketRecreateDialog.h"

#include "uiAskCharAddSlotDialog/uiAskCharAddSlotDialog.h"

#include "AskPkPopupDialog.h"
#include "AskReturnPartyDialog.h"

#ifdef _NA004034_20120102_POINT_WALLET_SYSTEM
#include "uiCurrencyItemInputNumber/uiCurrencyItemInputNumber.h"
#endif //_NA004034_20120102_POINT_WALLET_SYSTEM

const DWORD IM_ASSIST_MANAGER::REPLY_GUILD_DIALOG = g_IDGenerator.GenerateID();
const DWORD IM_ASSIST_MANAGER::EXTRASTONE_USE_DIALOG = g_IDGenerator.GenerateID();
const DWORD IM_ASSIST_MANAGER::SOCKET_FILL_DIALOG = g_IDGenerator.GenerateID();
const DWORD IM_ASSIST_MANAGER::NPC_PORTAL_DIALOG = g_IDGenerator.GenerateID();
const DWORD IM_ASSIST_MANAGER::NORMAL_ENCHANT_DIALOG = g_IDGenerator.GenerateID();
const DWORD IM_ASSIST_MANAGER::ETHER_DEVICE_DIALOG = g_IDGenerator.GenerateID();
const DWORD IM_ASSIST_MANAGER::RIDER_WAXGLOSS_DIALOG = g_IDGenerator.GenerateID();
const DWORD IM_ASSIST_MANAGER::SSQ_OBSERVER_DIALOG = g_IDGenerator.GenerateID();
const DWORD IM_ASSIST_MANAGER::BUY_LOTTOTICKET_DIALOG = g_IDGenerator.GenerateID();
const DWORD IM_ASSIST_MANAGER::CHAR_CUSTOMIZE_DIALOG = g_IDGenerator.GenerateID();
const DWORD IM_ASSIST_MANAGER::ITEM_CUSTOMIZE_DIALOG = g_IDGenerator.GenerateID();
const DWORD IM_ASSIST_MANAGER::AIRSHIP_PORTAL_DIALOG = g_IDGenerator.GenerateID();
const DWORD IM_ASSIST_MANAGER::SOCKET_IDENTIFY_DIALOG = g_IDGenerator.GenerateID();
const DWORD IM_ASSIST_MANAGER::SOCKET_RECREATE_DIALOG = g_IDGenerator.GenerateID();
const DWORD IM_ASSIST_MANAGER::PK_POPUP_DIALOG = g_IDGenerator.GenerateID();
const DWORD IM_ASSIST_MANAGER::CHAR_SLOT_ADD = g_IDGenerator.GenerateID();
const DWORD IM_ASSIST_MANAGER::RETURN_PARTY_DIALOG = g_IDGenerator.GenerateID();

uiAssistMan::uiAssistMan(InterfaceManager *pUIMan):
uiBaseMan(pUIMan)

{


    m_pAskReplyGuildDlg = NULL;
    m_pAskExtraStoneUse	= NULL;
    m_pAskSocketFillDialog = NULL;

    m_pAskNPCPortal		= NULL;
    m_pAskNormalEnchantDialog = NULL;
    m_pAskEtherDeviceDialog = NULL;
    m_pRiderWaxGloss = NULL;

    char_customize_dlg = NULL;
    item_customize_dlg = NULL;


    m_pAskSSQObserverDialog = NULL;

#ifdef _NA_0_20100115_HEIM_LOTTO_TICKET_PURCHASE
    m_pAskBuyLottoTicket=NULL;
#endif //_NA_0_20100115_HEIM_LOTTO_TICKET_PURCHASE

    m_pCurrentDialog = NULL;
    airship_dlg = NULL;

    pk_popup_dialog = NULL;
    return_party_dialog_ = NULL;

}

void	uiAssistMan::OnInitialize()
{
    m_pAskReplyGuildDlg = CreateUIDialog<AskReplyGuildDialog>(IM_ASSIST_MANAGER::REPLY_GUILD_DIALOG, "Data\\Interface\\82_Asking_Window.iwz", this, FALSE, TRUE);
    SUN_ASSERT(m_pAskReplyGuildDlg);

    m_pAskExtraStoneUse = CreateUIDialog<AskExtraStoneUse>(IM_ASSIST_MANAGER::EXTRASTONE_USE_DIALOG, "Data\\Interface\\82_Asking_Window.iwz", this, FALSE, TRUE);
    SUN_ASSERT(m_pAskExtraStoneUse);

    m_pAskSocketFillDialog = CreateUIDialog<AskSocketFillDialog>(IM_ASSIST_MANAGER::SOCKET_FILL_DIALOG, "Data\\Interface\\82_Asking_Window.iwz", this, FALSE, TRUE);
    SUN_ASSERT(m_pAskSocketFillDialog);

    ask_socket_identify_dialog_ = CreateUIDialog<AskSocketIdentifyDialog>(IM_ASSIST_MANAGER::SOCKET_IDENTIFY_DIALOG, "Data\\Interface\\82_Asking_Window.iwz", this, FALSE, TRUE);
    SUN_ASSERT(ask_socket_identify_dialog_);
    ask_socket_recreate_dialog_ = CreateUIDialog<AskSocketRecreateDialog>(IM_ASSIST_MANAGER::SOCKET_RECREATE_DIALOG, "Data\\Interface\\82_Asking_Window.iwz", this, FALSE, TRUE);
    SUN_ASSERT(ask_socket_recreate_dialog_);

    m_pAskNPCPortal = CreateUIDialog<AskNPCPortal>(IM_ASSIST_MANAGER::NPC_PORTAL_DIALOG, "Data\\Interface\\82_Asking_Window.iwz", this, FALSE, TRUE);
    SUN_ASSERT(m_pAskNPCPortal);

    m_pAskNormalEnchantDialog = CreateUIDialog<AskNormalEnchantDialog>(IM_ASSIST_MANAGER::NORMAL_ENCHANT_DIALOG, "Data\\Interface\\82_Asking_Window.iwz", this, FALSE, TRUE);
    SUN_ASSERT(m_pAskNormalEnchantDialog);

    m_pAskEtherDeviceDialog = CreateUIDialog<AskEtherDeviceDialog>(IM_ASSIST_MANAGER::ETHER_DEVICE_DIALOG, "Data\\Interface\\82_Asking_Window.iwz", this, FALSE, TRUE);
    SUN_ASSERT(m_pAskEtherDeviceDialog);

    m_pRiderWaxGloss = CreateUIDialog<AskRiderWaxGloss>(IM_ASSIST_MANAGER::RIDER_WAXGLOSS_DIALOG, "Data\\Interface\\82_Asking_Window.iwz", this, FALSE, TRUE);
    SUN_ASSERT(m_pRiderWaxGloss);

    char_customize_dlg = CreateUIDialog<AskCharCustomize>(IM_ASSIST_MANAGER::CHAR_CUSTOMIZE_DIALOG,
        "Data\\Interface\\82_Asking_Window.iwz", this, FALSE, TRUE);
    SUN_ASSERT(char_customize_dlg);

    item_customize_dlg = CreateUIDialog<AskItemCustomize>(IM_ASSIST_MANAGER::ITEM_CUSTOMIZE_DIALOG,
        "Data\\Interface\\82_Asking_Window.iwz", this, FALSE, TRUE);
    SUN_ASSERT(item_customize_dlg);

    airship_dlg = CreateUIDialog<AskAirShipPortal>(IM_ASSIST_MANAGER::AIRSHIP_PORTAL_DIALOG,
        "Data\\Interface\\82_Asking_Window.iwz", this, FALSE, TRUE);
    SUN_ASSERT(airship_dlg);


    m_pAskSSQObserverDialog = CreateUIDialog<AskSSQObserverDialog>(IM_ASSIST_MANAGER::SSQ_OBSERVER_DIALOG, "Data\\Interface\\82_asking_window3.iwz", this, FALSE, TRUE);
    SUN_ASSERT(m_pAskSSQObserverDialog);

#ifdef _NA_0_20100115_HEIM_LOTTO_TICKET_PURCHASE
    m_pAskBuyLottoTicket = CreateUIDialog<AskBuyLottoTicket>(IM_ASSIST_MANAGER::BUY_LOTTOTICKET_DIALOG, "Data\\Interface\\82_asking_window4.iwz", this, FALSE, TRUE);
    SUN_ASSERT(m_pAskBuyLottoTicket);
#endif //_NA_0_20100115_HEIM_LOTTO_TICKET_PURCHASE

#ifdef _SCJ_PK_POPUP_DIALOG
    pk_popup_dialog = CreateUIDialog<AskPkPopupDialog>(IM_ASSIST_MANAGER::PK_POPUP_DIALOG,
        "Data\\Interface\\82_Asking_Window.iwz", this, FALSE, TRUE);
    SUN_ASSERT(pk_popup_dialog);
#endif //_SCJ_PK_POPUP_DIALOG

    char_add_slot_dlg = CreateUIDialog<AskCharAddSlotDialog>(IM_ASSIST_MANAGER::CHAR_SLOT_ADD,
        "Data\\Interface\\82_Asking_Window.iwz", this, FALSE, TRUE);
    SUN_ASSERT(char_add_slot_dlg);

    return_party_dialog_ = CreateUIDialog<AskReturnPartyDialog>(IM_ASSIST_MANAGER::RETURN_PARTY_DIALOG,
        "Data\\Interface\\82_Asking_Window.iwz", this, FALSE, TRUE);
    SUN_ASSERT(return_party_dialog_);

#ifdef _NA004034_20120102_POINT_WALLET_SYSTEM
    CreateUIDialog<uiCurrencyItemInputNumber>(uiCurrencyItemInputNumber::kDialog_ID,
        "Data\\Interface\\72_1_Trade_input.iwz", this, TRUE, TRUE);
#endif //_NA004034_20120102_POINT_WALLET_SYSTEM
}

//------------------------------------------------------------------------------
/** 비공정맵으로 이동
*/
void uiAssistMan::ShowAskAirShipPortal(WORD portal_index, bool is_airship_zone,  DWORD airship_target_object_key/*= 0 */)
{
    if (airship_dlg == NULL)
    {
        return;
    }
    airship_dlg->set_airship_zone(is_airship_zone);
    airship_dlg->set_portal_index(portal_index);
    airship_dlg->set_air_ship_target_object_key_(airship_target_object_key);
    airship_dlg->ShowInterface(TRUE);
}
//------------------------------------------------------------------------------ 
void uiAssistMan::ShowAskPkPopupDialog(bool is_show, POSTYPE pos, int svIdx)
{
    if (pk_popup_dialog == NULL)
    {
        return;
    }

	if(is_show == true)
	{		
		pk_popup_dialog->set_ctrl_id_pos(pos);
		pk_popup_dialog->set_server_index(svIdx);
		pk_popup_dialog->ShowInterface(TRUE);
	}
	else
	{	
		pk_popup_dialog->ShowInterface(FALSE);
	}
}
//------------------------------------------------------------------------------ 
void uiAssistMan::ShowAskPkPopupDialog(bool is_show, AuthSetverData* auth_server_data)
{
    if (pk_popup_dialog != NULL)
    {
        pk_popup_dialog->set_auth_server_data(auth_server_data);
        pk_popup_dialog->ShowInterface((BOOL)is_show);
    }
}
//------------------------------------------------------------------------------
/**
*/
BOOL	uiAssistMan::ShowAskReplyGuildDlg(DWORD dwOperUserKey)
{
	if (m_pCurrentDialog)
	{
		m_pCurrentDialog->ShowInterface(FALSE);
	}	
	m_pAskReplyGuildDlg->ShowInterface(TRUE);

	m_pAskReplyGuildDlg->SetInviteOperUserKey(dwOperUserKey);

	m_pCurrentDialog = m_pAskReplyGuildDlg;

	return TRUE;
}

BOOL	uiAssistMan::ShowAskExtraStoneUse(CODETYPE ItemCode, POSTYPE CrystalPos, POSTYPE TargetPos)
{
	if (m_pCurrentDialog)
	{
		m_pCurrentDialog->ShowInterface(FALSE);
	}

	m_pAskExtraStoneUse->SetInfo(ItemCode, CrystalPos, TargetPos);
	m_pAskExtraStoneUse->ShowInterface(TRUE);

	m_pCurrentDialog = m_pAskExtraStoneUse;
	
	return TRUE;
}

BOOL	uiAssistMan::ShowAskSocketFillDialog(CODETYPE ItemCode, POSTYPE CrystalPos, POSTYPE TargetPos)
{
	if (m_pCurrentDialog)
	{
		m_pCurrentDialog->ShowInterface(FALSE);
	}

	m_pAskSocketFillDialog->SetInfo(ItemCode, CrystalPos, TargetPos);
	m_pAskSocketFillDialog->ShowInterface(TRUE);
	
	m_pCurrentDialog = m_pAskSocketFillDialog;

	return TRUE;
}
BOOL	uiAssistMan::ShowAskSocketIdentifyDialog(CODETYPE ItemCode, POSTYPE CrystalPos, POSTYPE TargetPos)
{
    if (m_pCurrentDialog)
    {
        m_pCurrentDialog->ShowInterface(FALSE);
    }

    ask_socket_identify_dialog_->SetInfo(ItemCode, CrystalPos, TargetPos);
    ask_socket_identify_dialog_->ShowInterface(TRUE);

    m_pCurrentDialog = ask_socket_identify_dialog_;

    return TRUE;
}

BOOL uiAssistMan::ShowAskSocketRecreateDialog(CODETYPE item_code, POSTYPE hermetism_position, POSTYPE target_position)
{
    if (m_pCurrentDialog)
    {
        m_pCurrentDialog->ShowInterface(FALSE);
    }

    ask_socket_recreate_dialog_->SetInfo(item_code, hermetism_position, target_position);
    ask_socket_recreate_dialog_->ShowInterface(TRUE);

    m_pCurrentDialog = ask_socket_recreate_dialog_;

    return TRUE;
}

BOOL	uiAssistMan::ShowAskNPCPortal(MAPCODE MapCode, MONEY fare)
{
	if (m_pCurrentDialog)
	{
		m_pCurrentDialog->ShowInterface(FALSE);
	}

	m_pAskNPCPortal->SetMoveVillage(MapCode, fare);
	m_pAskNPCPortal->ShowInterface(TRUE);
	
	m_pCurrentDialog = m_pAskNPCPortal;

	return TRUE;
}	

BOOL	uiAssistMan::ShowAskNormalEnchantDialog(eNormalEnchantType eType, CODETYPE ItemCode, POSTYPE CrystalPos, POSTYPE TargetPos)
{
	if (m_pCurrentDialog)
	{
		m_pCurrentDialog->ShowInterface(FALSE);
	}

	m_pAskNormalEnchantDialog->SetEnchantType( eType );
	m_pAskNormalEnchantDialog->SetEnchantInfo(ItemCode, CrystalPos, TargetPos);
	m_pAskNormalEnchantDialog->ShowInterface(TRUE);

	m_pCurrentDialog = m_pAskNormalEnchantDialog;

	return TRUE;
}

BOOL	uiAssistMan::ShowAskEtherDeviceDialog(CODETYPE ItemCode, POSTYPE WeaponPos, POSTYPE DeviceItemPos)
{
	if (m_pCurrentDialog)
	{
		m_pCurrentDialog->ShowInterface(FALSE);
	}

	m_pAskEtherDeviceDialog->SetInfo(ItemCode, WeaponPos, DeviceItemPos);
	m_pAskEtherDeviceDialog->ShowInterface(TRUE);

	m_pCurrentDialog = m_pAskEtherDeviceDialog;

	return TRUE;

}

BOOL	uiAssistMan::ShowAskRiderWaxGlossDialog(CODETYPE ItemCode, POSTYPE RiderPartsPos, POSTYPE WaxItemPos)
{
	if (m_pCurrentDialog)
	{
		m_pCurrentDialog->ShowInterface(FALSE);
	}

    m_pRiderWaxGloss->SetRiderInfo(ItemCode, RiderPartsPos, WaxItemPos);
	m_pRiderWaxGloss->ShowInterface(TRUE);

	m_pCurrentDialog = m_pRiderWaxGloss;

    return TRUE;

}

void uiAssistMan::ShowAskCharCustomize(WORD customize_type)
{
    if (char_customize_dlg)
    {
        char_customize_dlg->set_customize_type(customize_type);
        char_customize_dlg->ShowInterface(TRUE);
    }
}

void uiAssistMan::ShowAskCharChangeGender(BYTE change_gender, POSTYPE slot_position)
{
    if (char_customize_dlg)
    {
        char_customize_dlg->set_customize_type(KGender);
        char_customize_dlg->set_change_gender(change_gender);
        char_customize_dlg->set_slot_position(slot_position);
        char_customize_dlg->ShowInterface(TRUE);
    }
}

void uiAssistMan::ShowAskAddCharSlot(POSTYPE slot_position)
{
    if (char_add_slot_dlg)
    {
        char_add_slot_dlg->set_item_position(slot_position);
        char_add_slot_dlg->ShowInterface(TRUE);
    }
}


void uiAssistMan::ShowAskItemCustomize(BYTE customize_type, 
                                            POSTYPE slot_pos, 
                                            POSTYPE use_pos)
{
    if (item_customize_dlg == NULL)
    {
        return;
    }
    
    item_customize_dlg->set_customize_type(customize_type, slot_pos, use_pos);
    item_customize_dlg->ShowInterface(TRUE);
}

void uiAssistMan::GetItemCustomizePos(POSTYPE& OUT slot_pos, POSTYPE& OUT use_pos)
{
    slot_pos = item_customize_dlg->slot_pos();
    use_pos = item_customize_dlg->use_pos();
}

bool uiAssistMan::ShowAskSSQObserverDialog()
{
	m_pAskSSQObserverDialog->ShowInterface(TRUE);

	return true;
}


#ifdef _NA_0_20100115_HEIM_LOTTO_TICKET_PURCHASE
BOOL uiAssistMan::ShowAskBuyLottoTicket(BOOL bAutoNum)
{
	if (m_pCurrentDialog)
	{
		m_pCurrentDialog->ShowInterface(FALSE);
	}

    m_pAskBuyLottoTicket->SetInfo(bAutoNum);
    m_pAskBuyLottoTicket->ShowInterface(TRUE);
    
	m_pCurrentDialog = m_pAskBuyLottoTicket;

    return true;
}    
#endif//_NA_0_20100115_HEIM_LOTTO_TICKET_PURCHASE

BOOL uiAssistMan::ShowAskPartyReturnDialog()
{
    if (return_party_dialog_ != NULL)
    {
        return_party_dialog_->ShowInterface(TRUE);
    }
    return TRUE;
}

#ifdef _NA004034_20120102_POINT_WALLET_SYSTEM
void uiAssistMan::ShowUseCurrencyItem(POSTYPE item_position)
{
    uiCurrencyItemInputNumber* currency_input = 
        GET_CAST_DIALOG(uiCurrencyItemInputNumber, uiCurrencyItemInputNumber::kDialog_ID);
    if (currency_input == NULL)
    {
        return;
    }

    currency_input->UseCurrencyItem(item_position);
    currency_input->ShowInterface(TRUE);
}
#endif //_NA004034_20120102_POINT_WALLET_SYSTEM