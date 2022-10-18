#pragma once
#include "uiBaseMan.h"

struct IM_ASSIST_MANAGER
{
	static const DWORD REPLY_GUILD_DIALOG;
	static const DWORD EXTRASTONE_USE_DIALOG;
	static const DWORD SOCKET_FILL_DIALOG;
	static const DWORD NPC_PORTAL_DIALOG;
	static const DWORD NORMAL_ENCHANT_DIALOG;
	static const DWORD ETHER_DEVICE_DIALOG;
	static const DWORD RIDER_WAXGLOSS_DIALOG;
    static const DWORD CHAR_CUSTOMIZE_DIALOG;
    static const DWORD ITEM_CUSTOMIZE_DIALOG;
	static const DWORD SSQ_OBSERVER_DIALOG;
	static const DWORD BUY_LOTTOTICKET_DIALOG;
    static const DWORD AIRSHIP_PORTAL_DIALOG;
    static const DWORD SOCKET_IDENTIFY_DIALOG;
    static const DWORD SOCKET_RECREATE_DIALOG;
	static const DWORD PK_POPUP_DIALOG;
    static const DWORD CHAR_SLOT_ADD;
    static const DWORD RETURN_PARTY_DIALOG;
};

class AskReplyGuildDialog;
class AskExtraStoneUse;
class AskSocketFillDialog;
class AskNPCPortal;
class AskNormalEnchantDialog;
class AskCharCustomize;
class AskAirShipPortal;
class AskItemCustomize;
class AskSSQObserverDialog;

class AskEtherDeviceDialog;
class AskRiderWaxGloss;

#ifdef _NA_0_20100115_HEIM_LOTTO_TICKET_PURCHASE
    class AskBuyLottoTicket;
#endif //_NA_0_20100115_HEIM_LOTTO_TICKET_PURCHASE
class AskSocketIdentifyDialog;
class AskSocketRecreateDialog;
class AskCharAddSlotDialog;

struct AuthSetverData;
class AskPkPopupDialog;
class AskReturnPartyDialog;

class uiAssistMan :	public uiBaseMan
{
public:
	uiAssistMan(InterfaceManager *pUIMan);


public:
    virtual void            OnInitialize();


	BOOL					ShowAskReplyGuildDlg(DWORD dwOperUserKey);
	BOOL					ShowAskExtraStoneUse(CODETYPE ItemCode, POSTYPE CrystalPos, POSTYPE TargetPos);
	BOOL					ShowAskSocketFillDialog(CODETYPE ItemCode, POSTYPE CrystalPos, POSTYPE TargetPos);
    BOOL					ShowAskSocketIdentifyDialog(CODETYPE ItemCode, POSTYPE CrystalPos, POSTYPE TargetPos);
    BOOL                    ShowAskSocketRecreateDialog(CODETYPE item_code, POSTYPE hermetism_position, POSTYPE target_position);
	BOOL					ShowAskNPCPortal(MAPCODE MapCode, MONEY fare);
	BOOL					ShowAskNormalEnchantDialog(eNormalEnchantType eType, CODETYPE ItemCode, POSTYPE CrystalPos, POSTYPE TargetPos);
	BOOL					ShowAskEtherDeviceDialog(CODETYPE ItemCode, POSTYPE WeaponPos, POSTYPE DeviceItemPos);
    BOOL					ShowAskRiderWaxGlossDialog(CODETYPE ItemCode, POSTYPE RiderPartsPos, POSTYPE WaxItemPos);
    void					ShowAskAirShipPortal(WORD portal_index, bool is_airship_zone, DWORD airship_target_object_key = 0);

    //------------------------------------------------------------------------------ 
    //! 1101 이전
	void ShowAskPkPopupDialog(bool is_show, POSTYPE pos, int svIdx);
    //! 1101 이후
    void ShowAskPkPopupDialog(bool is_show, AuthSetverData* auth_server_data);
    //------------------------------------------------------------------------------ 

    void ShowAskCharCustomize(WORD customize_type);
    
    void ShowAskCharChangeGender(BYTE change_gender, POSTYPE slot_position);

    void ShowAskAddCharSlot(POSTYPE slot_position);

    void ShowAskItemCustomize(BYTE customize_type,
                              POSTYPE slot_pos, 
                              POSTYPE use_pos);
    void GetItemCustomizePos(POSTYPE& slot_pos, POSTYPE& use_pos);

	bool					ShowAskSSQObserverDialog();

#ifdef _NA_0_20100115_HEIM_LOTTO_TICKET_PURCHASE
    BOOL					ShowAskBuyLottoTicket(BOOL bAutoNum);
#endif //_NA_0_20100115_HEIM_LOTTO_TICKET_PURCHASE

    BOOL                    ShowAskPartyReturnDialog();

#ifdef _NA004034_20120102_POINT_WALLET_SYSTEM
    static void ShowUseCurrencyItem(POSTYPE item_position);
#endif //_NA004034_20120102_POINT_WALLET_SYSTEM    

private:
	AskReplyGuildDialog*	m_pAskReplyGuildDlg;
	AskExtraStoneUse*		m_pAskExtraStoneUse;
	AskSocketFillDialog*	m_pAskSocketFillDialog;
	AskNPCPortal*			m_pAskNPCPortal;
	AskNormalEnchantDialog*	m_pAskNormalEnchantDialog;
	AskEtherDeviceDialog*   m_pAskEtherDeviceDialog;
    AskRiderWaxGloss*       m_pRiderWaxGloss;
    AskSocketIdentifyDialog* ask_socket_identify_dialog_;
    AskSocketRecreateDialog* ask_socket_recreate_dialog_;

    AskCharCustomize* char_customize_dlg;
    AskItemCustomize* item_customize_dlg;

    AskCharAddSlotDialog* char_add_slot_dlg;

    AskAirShipPortal* airship_dlg;

	AskSSQObserverDialog*	m_pAskSSQObserverDialog;

#ifdef _NA_0_20100115_HEIM_LOTTO_TICKET_PURCHASE
    AskBuyLottoTicket*      m_pAskBuyLottoTicket;
#endif //_NA_0_20100115_HEIM_LOTTO_TICKET_PURCHASE

	AskPkPopupDialog* pk_popup_dialog;
    AskReturnPartyDialog* return_party_dialog_;

	uiBase* m_pCurrentDialog;
};
