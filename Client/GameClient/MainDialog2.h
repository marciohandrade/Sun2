#ifndef __MAIN_DIALOG2_H__
#define __MAIN_DIALOG2_H__
#pragma once

#ifdef _NA_000000_20130114_RENEWER_UI
#include "uiBase.h"

class MainDialog2 : public uiBase
{
private:
	DWORD m_DialogKey;

public:
	enum eMAIN_POS
	{	
		MAIN_BTN_ITEMSHOP_TI00,
		MAIN_BTN_BATTLEZONE_TI04,
		MAIN_BTN_CHAOSZONE_TI03,
		MAIN_BTN_WORLDMAP_TI02,
#ifdef _NA_007942_20150115_ADD_MAINMENU_DIALOG_ICON
        MAIN_BTN_CONTINENT_TI01,
#else
		MAIN_BTN_COMMUNITY_TI01,
#endif //_NA_007942_20150115_ADD_MAINMENU_DIALOG_ICON
#ifdef _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
        MAIN_BTN_ATTENDANCE_I000,
#else
  #ifdef _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM_CLOSE
              MAIN_BTN_ATTENDANCE_I000,
  #endif //_NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM_CLOSE
#endif //_NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM
		MAIN_BTN_SUN_RANKING_I001,
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM
#ifdef _NA_008405_20150716_MINIGAME_BINGO
		MAIN_BTN_MINIGAME_I002, 
#endif // _NA_008405_20150716_MINIGAME_BINGO

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
	MainDialog2();
	virtual ~MainDialog2();

	VOID			Init(CDrawBase* pDrawBase);
	virtual VOID    Release();
	virtual VOID	MessageProc(SI_MESSAGE* pMessage);
	virtual VOID	OnRenderSolarDialog();
	virtual VOID	OnUpdateSolarDialog();

	void			RenderToolTipBattle(void);
	void			RenderToolTipChaos(void);
#ifdef _NA_008405_20150716_MINIGAME_BINGO
	void            RenderToolTipMiniGame(void);
#endif // _NA_008405_20150716_MINIGAME_BINGO

	void			BattleZoneToDisAbleButton(bool state);
#ifdef _NA_008405_20150716_MINIGAME_BINGO
	void            MiniGameToDisAbleButton(bool state);
#endif // _NA_008405_20150716_MINIGAME_BINGO

	virtual VOID	OnShowWindow(BOOL val);
	void			CheckModifyOption();
private:
	CCtrlButtonWZ*	chaos_zone_button_;
	CCtrlButtonWZ*  battle_zone_button_;
#ifdef _NA_008405_20150716_MINIGAME_BINGO
	CCtrlButtonWZ*  mini_game_button_;
#endif // _NA_008405_20150716_MINIGAME_BINGO

	POSTYPE			Tooltip_pick_pos_;

	bool			render_tooltip_[MAIN_MAX];

	stTOOLTIPCONTROL tooltip_control_center_[MAIN_MAX];

protected:
	CControlWZ*	    getControl(POSTYPE AtPos);
	POSTYPE			getCtrlIDToPos(WzID wzId);
	static WzID		m_wzId[MAIN_MAX];
};
#endif//#ifdef _NA_000000_20130114_RENEWER_UI
#endif