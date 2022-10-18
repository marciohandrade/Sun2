#ifndef __HERO_GAUGE_DIALOG2_CONTROL_H__
#define __HERO_GAUGE_DIALOG2_CONTROL_H__

#pragma once

#ifdef _NA_000000_20130114_RENEWER_UI

#include "uiBase.h"
#include "globalfunc.h"
#include "interfacemanager.h"

#define	TOOLTIP_STRING_NOTE_CODE	5798	
#define BLINK_RANGE_TICK	500

//클릭이벤트 처리
#define WM_CLICK_EVENT WM_USER +1

class HeroGaugeDialog2Control : public uiBase
{
	enum
	{
		//레벨
		eControl_TEXT_LevelT012,
		//버튼
		eControl_Button_B100,
		eControl_Picture_P029,
		eControl_Button_B101,
		//체크
		eControl_Button_C100,
		eControl_Button_C200,
		eControl_Button_CT01,

		HGAUGE_MAX,
	};
public:
	HeroGaugeDialog2Control(void);
	virtual ~HeroGaugeDialog2Control(void);

	VOID			Init( CDrawBase * pDrawBase );
	virtual VOID	MessageProc( SI_MESSAGE * pMessage );
	virtual VOID	NetworkProc( MSG_BASE * pMsg );

	virtual VOID	OnRenderSolarDialog();
	virtual VOID	OnUpdateSolarDialog();

	void			Render();

	void            Process(DWORD dwTick);
	virtual VOID	OnShowWindow( BOOL val );

	void			CheckChattingMessage(DWORD Tick);
	void			CheckNoteMessage(DWORD Tick);
	void			CheckPresentMessage(DWORD Tick);
	void			LeaveParty();
	void			SetLevel( DWORD dwLevel );
	void			MoveWindowForce(float addx, float addy);
	virtual void	MoveDefaultWindowPos();
	void			GetMessage(DWORD messageID,LPARAM lParam,WPARAM wParam);
    void            Render_Tooltip();
public:
	CControlWZ *	getControl( POSTYPE AtPos );
	POSTYPE			getCtrlIDToPos( WzID wzId );
	static WzID		m_wzId[HGAUGE_MAX];

private:
	CCtrlButtonCheckWZ *	m_pChkChatMsg;
	CCtrlButtonCheckWZ*		m_pChkPresentMsg;
	CCtrlButtonCheckWZ*		m_pChkNoteMsg;
	CCtrlStaticWZ*			m_pCtrlPartyMark;

	map<int,int>			   tooltip_index_map_;
	int							tooltip_string_code_;

	UI_CONTROL_BUTTON_PROPERTY( PCModeBtn, StrToWzID("B100") );
	UI_CONTROL_BUTTON_PROPERTY( LeavePartyBtn, StrToWzID("B101") );	

};
#endif

#endif//__HERO_GAUGE_DIALOG2_CONTROL_H__