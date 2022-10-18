//=======================================================================================================================
/// MainDialog
/**
	@author
			유재영 < y2jinc@webzen.co.kr >
	@since

	@remarks
			- 		
*/


#ifndef __MAIN_DIALOG_H__
#define __MAIN_DIALOG_H__

#pragma once

#include "uiBase.h"


class MainDialog : public uiBase
{
private:
    DWORD m_DialogKey;

public:
	enum eMAIN_POS
	{	
        
#ifdef _NA_000000_20130114_RENEWER_UI
		MAIN_BTN_INVENTORY_TI04,
		MAIN_BTN_QUEST_TI03,
		MAIN_BTN_SKILL_TI02,
		MAIN_BTN_MYINFO_TI01,
#ifdef _NA_007086_20140318_MONSTERBOOK
        MAIN_BTN_MONSTERBOOK_I000,
#endif //_NA_007086_20140318_MONSTERBOOK
#ifdef _NA_007942_20150115_ADD_MAINMENU_DIALOG_ICON
        MAIN_BTN_COMMUNITY_I001,
#endif //_NA_007942_20150115_ADD_MAINMENU_DIALOG_ICON
		MAIN_BTN_MENU_TI00,
#else
		MAIN_TI_MENU = 0,
		MAIN_BTN_CASH_SHOP,	// CASH_SHOP
		MAIN_BTN_EXIT_OBSERVER,
		MAIN_PIC_EXIT_OBSERVER,
#endif
		MAIN_MAX,
	};

#ifdef _NA_000000_20130114_RENEWER_UI
	struct stTOOLTIPCONTROL
	{
		bool	isToolTip;
		RECT	TooltipPos;
		TCHAR	tooltipString[256];
		int		ui_position_;


		stTOOLTIPCONTROL()
		{
			isToolTip = false;
			memset(&TooltipPos,0,sizeof(RECT));
			memset(tooltipString,0,sizeof(TCHAR)*256);
		}
	};
#endif

	DWORD GetDialogKey() { return m_DialogKey; }
	void SetDialogKey( DWORD DialogKey ) { m_DialogKey = DialogKey; }

public:
	MainDialog();
	virtual ~MainDialog();

	VOID			Init(CDrawBase* pDrawBase);
    virtual VOID    Release();
	virtual VOID	MessageProc(SI_MESSAGE* pMessage);
	virtual VOID	NetworkProc(MSG_BASE* pMsg);
	virtual VOID	OnRenderSolarDialog();
	// x버튼을 클릭시 체크 상태를 해지 한다. -2005.05.01 지완
	BOOL            ResetCheckButton(eMAIN_POS pos);
    VOID            MainBtn_StatusSwitch(eMAIN_POS idx, bool bState);
	void			ShowObserverButton(BOOL bShow  = FALSE);
	void			RenderToolTip(void);
#ifdef _NA_000000_20130114_RENEWER_UI
	virtual VOID	OnShowWindow(BOOL val);
	void			CheckModifyOption();
#endif

protected:
	CControlWZ*	    getControl(POSTYPE AtPos);
	POSTYPE			getCtrlIDToPos(WzID wzId);
	static WzID		m_wzId[MAIN_MAX];

#ifdef _NA_000000_20130114_RENEWER_UI
	stTOOLTIPCONTROL tooltip_control_center_[MAIN_MAX];
#endif
	
};
#endif // __MAIN_DIALOG_H__