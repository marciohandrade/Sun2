#pragma once

#include "uiBaseMan.h"
#include "uiInventoryMan/uiInventoryDef.h"

class InventoryDialog;
class EventInventoryDialog;
class LotteryInventoryDialog;
class RiderInventoryDialog;

class ExtendInventoryDialog;
class InventoryFilterDialog;
class uiHelmetShow;

#ifdef _NA_002935_20110704_ITEM_SCORE
  class OtherPlayerEquipmentContainer;
#endif //_NA_002935_20110704_ITEM_SCORE

class uiPersonalWayPointWindow;

class uiInventoryMan :
	public uiBaseMan
{
public:
	uiInventoryMan(InterfaceManager *pUIMan);


public:
	virtual	void			OnInitialize();


	void					Send_CG_ITEM_INVENTORY_PWD_LOCKING_SYN( void );
	void					Send_CG_ITEM_INVENTORY_PWD_CHECK_SYN( BYTE ReqCode, TCHAR* szPassword = NULL);
	void					Send_CG_ITEM_INVENTORY_PWD_SETTING_SYN( BYTE ReqCode, TCHAR* szPassword = NULL );
	void					SetPasswordState( int iPasswordDlgState )		{m_iPasswordDlgState = iPasswordDlgState;}
	int						GetPasswordState( void )						{return m_iPasswordDlgState;}

	void					SetPasswordStr(TCHAR* strPassword);
	TCHAR*					GetPAsswordStr( void )				{return m_strPassword;}
	void					ClearPassword();
		
	void					Send_CG_ITEM_HAMMER_OF_ALCHEMIST_SYN( POSTYPE itemPos );

    void                    SetInventoryLockState( PLAYERINFO_FOR_PACKET::eINVEN_LOCK_MODE iInventoryLockState );
    PLAYERINFO_FOR_PACKET::eINVEN_LOCK_MODE GetInventoryLockState( void );

    void                    set_open_password_confirm(bool is_open);
	void					SetRiderInvenDlg(BOOL bOpen);

#ifdef _NA_002935_20110704_ITEM_SCORE
    void                    SetOtherPlayerEquipInventoryDialog(Player* player_pointer);
#endif //_NA_002935_20110704_ITEM_SCORE
#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE
	BOOL                    GetOtherPlayerEquipmentContainerVisible();
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE

    void                    ToggleExtendInvenDlg();
    void                    ToggleFilterInvenDlg();
    bool is_normal_inventory_type(){ return is_normal_inventory_type_; }
    void set_normal_inventory_type(bool set_type){ is_normal_inventory_type_ = set_type; }

    void                    ToggleWayPointInvenDlg();
    void                    PasswordLockingSyn(DWORD dwStrIndex);

    void                    LockInventoryUI();
    void                    UnlockInventoryUI();

    void                    LockInventorySlot(POSTYPE slot_pos);
    void                    UnlockInventorySlot(POSTYPE slot_pos);

private:
    BOOL                    _LoadUI();

	TCHAR					m_strPassword[PASSWD_MAX_LEN + 1];

	int						m_iPasswordDlgState;
	InventoryDialog*		m_pInvenDlg;
	EventInventoryDialog*	m_pEventInvenDlg;
	LotteryInventoryDialog* m_pLotteryInvenDlg;
	RiderInventoryDialog*   m_pRiderInvenDlg;

    ExtendInventoryDialog*  m_pExtendInvenDlg;
    InventoryFilterDialog*  m_pInvenFilterDlg;
    bool is_normal_inventory_type_;

#ifdef _NA_002935_20110704_ITEM_SCORE
    OtherPlayerEquipmentContainer* other_player_equipment_dialog_;
#endif //_NA_002935_20110704_ITEM_SCORE
#ifdef  _NA_000000_20130114_RENEWER_UI
	uiHelmetShow*			m_puiHelmet;
#endif

    uiPersonalWayPointWindow*   m_pPersonalWayPoint;
    bool                        open_password_confirm_;
};

inline void uiInventoryMan::set_open_password_confirm(bool is_open)
{
    open_password_confirm_ = is_open;
}