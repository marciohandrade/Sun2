//=======================================================================================================================
/// MainMenuOptionDlg
/**
	@author
			유재영 < y2jinc@webzen.co.kr >
	@since

	@remarks
			- 		
*/

#pragma once
#include "uiBase.h"
#include "interfacemanager.h"

class MainMenuOptionDlg : public uiBase
{
public:
	enum eMainMenuOption
	{
		//아이디가 겹치는건 최대한 겹치도록 하자
#ifdef _NA_000000_20130114_RENEWER_UI
		MAIN_MENU_OP_CASH_SHOP,//TI13
		MAIN_MENU_OP_BATTLE_ZONE,//I001
		MAIN_MENU_OP_CHAOS_ZONE,//I002
		MAIN_MENU_OP_INVENTORY,//TI00
		MAIN_MENU_OP_QUEST,//TI03
		MAIN_MENU_OP_SKILL,//TI01
		MAIN_MENU_OP_CHARACTER,//TI02
		MAIN_MENU_OP_AC,//TI12
		MAIN_MENU_OP_FULL_MAP,//I003
		MAIN_MENU_OP_MAP,//TI10
		MAIN_MENU_OP_FRIEND,//TI05
		MAIN_MENU_OP_GUILD,//TI06
		MAIN_MENU_OP_HELP,//TI11
		MAIN_MENU_SNS,
		MAIN_MENU_OP_CONFIG,//TI08
		MAIN_MENU_OP_LOGOUT,//TI09
#else
		MAIN_MENU_OP_HELP,
		MAIN_MENU_OP_INVENTORY,
		MAIN_MENU_OP_SKILL,
		MAIN_MENU_OP_CHARACTER,
		MAIN_MENU_OP_QUEST,
		MAIN_MENU_OP_FULL_MAP,
		MAIN_MENU_OP_MAP,
		MAIN_MENU_OP_FRIEND,
		MAIN_MENU_OP_GUILD,

		MAIN_MENU_OP_CONFIG,
		MAIN_MENU_OP_LOGOUT,
		MAIN_MENU_OP_AC,

		MAIN_MENU_OP_CASH_SHOP, 

		MAIN_MENU_CONTINENT_MAP, 

		MAIN_MENU_CREDIT,

        MAIN_MENU_SNS,
#endif//_NA_000000_20130114_RENEWER_UI
		Dialog_Max
	};

	MainMenuOptionDlg();
	virtual ~MainMenuOptionDlg(void);

	// 기본 virtual methods
	VOID			Init( CDrawBase * pDrawBase );
	virtual VOID	Release();

	virtual VOID	MessageProc( SI_MESSAGE * pMessage );
	virtual VOID	NetworkProc( MSG_BASE * pMsg );

	virtual VOID	OnShowWindow(BOOL val);

	virtual VOID	OnUpdateSolarDialog();

	VOID			ToggleShowWindow(InterfaceManager::eDIALOG_ID id);
	VOID			ToggleShowWindow(WzID id);
#ifdef _NA_000000_20130114_RENEWER_UI
	void			CheckModifyOption();
#endif
	// 기본 proected methods
protected:
	CControlWZ *	getControl( POSTYPE AtPos );
	POSTYPE			getCtrlIDToPos( WzID wzId );
	
	// 기본 attributes
private:
	static WzID	    m_wzId[Dialog_Max];
};
