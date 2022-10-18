//=======================================================================================================================
/** 히어로 상태창
	@author
			유재영
	@since
			
	@remarks
*/

#ifndef __HERO_GAUGE_DIALOG_H__
#define __HERO_GAUGE_DIALOG_H__

#pragma once

#include "uiBase.h"
#include "globalfunc.h"
#include "interfacemanager.h"
#include "IconMgr.h"
#include "uibattlezoneman/uiBattleZoneMan_def.h"
#include "uibattlezoneman/uiBattleZoneMan.h"

#define WARNING_HP			20.0f
#define BLINK_GAP_TICK		1000

class HeroGaugeDialog : public uiBase
{
	enum eHGAUGE_POS
	{	
        HGAUGE_TEXT_LEVEL = 0,
		HGAUGE_TEXT_MP_OR_SP,
	    HGAUGE_RED,
        HGAUGE_BLUE,
        HGAUGE_EXP,

		HGAUGE_TEXT_CURRENT_HP,
		HGAUGE_TEXT_TOATAL_HP,
		HGAUGE_TEXT_CURRENT_MP,
		HGAUGE_TEXT_TOTAL_MP,
		HGAUGE_TEXT_CURRENT_EXP,
		HGAUGE_TEXT_TOTAL_EXP,

		HGAUGE_SKILLEFFECT0,
		HGAUGE_SKILLEFFECT1,
		HGAUGE_SKILLEFFECT2,
		HGAUGE_SKILLEFFECT3,
		HGAUGE_SKILLEFFECT4,
		HGAUGE_SKILLEFFECT5,
		HGAUGE_SKILLEFFECT6,
		HGAUGE_SKILLEFFECT7,
		HGAUGE_SKILLEFFECT8,
		HGAUGE_SKILLEFFECT9,

		HGAUGE_EXPBALL0,
		HGAUGE_EXPBALL1,
		HGAUGE_EXPBALL2,
		HGAUGE_EXPBALL3,
		HGAUGE_EXPBALL4,
		HGAUGE_EXPBALL5,
		HGAUGE_EXPBALL6,
		HGAUGE_EXPBALL7,
		HGAUGE_EXPBALL8,
		HGAUGE_EXPBALL9,

		HGAUGE_HEADIMAGE1,
		HGAUGE_HEADIMAGE2,
		HGAUGE_HEADIMAGE3,
		HGAUGE_HEADIMAGE4,
		HGAUGE_HEADIMAGE5,

        HGAUGE_PARTY_MARK,
		HGAUGE_PARTY_LEAVE,

		HGAUGE_CHK_MIC,

		HGAUGE_CHK_CHAT_MSG,
		HGAUGE_TEXT_CHAT_MSG,
		HGAUGE_BTN_CHAT_MSG,	// 영역체크를 위한 투명버튼 

		HGAUGE_CHK_PRESENT_MSG,

		HGAUGE_CHK_NOTE_MSG,

		HGAUGE_RESERVE_HP,

		HGAUGE_SKILLEFFECT_OPEN_BTN,
		HGAUGE_SKILLEFFECT_CLOSE_BTN,

		HGAUGE_SD_GAUGE,
		HGAUGE_TEXT_CURRENT_SD,
		HGAUGE_TEXT_TOATAL_SD,

        HGAUGE_MAX_LV_EXP,
		HGAUGE_MAX,
	};	
public:
	HeroGaugeDialog();
	virtual ~HeroGaugeDialog();

	VOID			Init( CDrawBase * pDrawBase );
	virtual VOID	MessageProc( SI_MESSAGE * pMessage );
	virtual VOID	NetworkProc( MSG_BASE * pMsg );

	virtual VOID	OnRenderSolarDialog();
	virtual VOID	OnUpdateSolarDialog();

    void Process(DWORD dwTick);

	VOID			SetHeroHP( float fHPPercent );
	VOID			SetHeroMP( float fMPPercent );
	VOID            SetHeroExp( float fExpPercent ); 
	VOID			SetHeroName( char * pszHeroName );
	VOID			SetLevel( DWORD dwLevel );
	void            SetExpBallCount(int iCount);
	VOID            SetHeroSD( float fPercent);
	void			SetHeroReserveHP( float val );

    void             UpdateStatusIcon();
    void            GetSkillEffectArea(int status_index, RECT* icon_rect);	

	void            MakeHitEffect();

	int				getTooltipString(POSTYPE pos);
	void			updateTooltip();

	void			RenderGaugeNumber();

    float hp_prev_percent() const { return hp_prev_percent_; }
    void hp_prev_percent_increase(float val);
    void set_hp_prev_percent(float val);

protected:
	void			LeaveParty();
	void			RenderSuperRoomIcon();

protected:
	CControlWZ *	getControl( POSTYPE AtPos );
	POSTYPE			getCtrlIDToPos( WzID wzId );
	static WzID		m_wzId[HGAUGE_MAX];


	float			m_fHPGaugeSize;
	float			m_fMPGaugeSize;
	float			m_fExpGaugeSize;
	TCHAR			m_szCharName[MAX_CHARNAME_LENGTH + 1];
	TCHAR			m_szTemp[512];
	TCHAR			m_szRenderInfo[256];

	BASE_PLAYERINFO	m_BasePlayerInfo;
	
	int             m_iExpBallCount;

	BOOL			m_bBlinkHPWarning;
	BOOL			m_bHPShowFlag;
	util::Timer 	m_BlinkTimer;


	int             m_iHitEffectDelay;
	float           m_fHitEffectPosX;
	float           m_fHitEffectPosY;

	// 맞았을때
	HANDLE          m_hTextureHitEffect;
	HANDLE          m_hTextureHeadMask;

	RECT			m_rtSkillEffect;
	BOOL			m_bAllSkillEffectView;
	BYTE			m_btSkillEffectCnt;
	BYTE			mouseover_status_icon_index_;	//마우스온되있는 아이콘(0~29중 하나)

private:
	void			CheckNoteMessage( DWORD dwTick );
	void			CheckChatMessage( DWORD dwTick );
	void			CheckPresentMessage( DWORD dwTick );

private:

	float				m_fSDGaugeSize;
	CCtrlStaticWZ *		m_pCtrlTextSDInfo[2];
	CCtrlStaticWZ * m_pCtrlTextHPInfo[2];
	CCtrlStaticWZ * m_pCtrlTextMPInfo[2];
	CCtrlStaticWZ * m_pCtrlTextEXPInfo[2];
	CCtrlButtonWZ* m_pCtrlSEOpenBtn;
	CCtrlButtonWZ* m_pCtrlSECloseBtn;
	CCtrlButtonCheckWZ *	m_pMicButton;

	CCtrlButtonCheckWZ *	m_pChkChatMsg;

	CCtrlButtonCheckWZ*		m_pChkPresentMsg;

	CCtrlButtonCheckWZ*		m_pChkNoteMsg;
    CControlWZ*             m_pCtrlExpImage;
	bool					m_bChatState;
	DWORD					m_dwChatShowTick;
	bool					m_bPresentState;
	DWORD					m_dwPresentShowTick;
	bool					m_bNoteState;
	DWORD					m_dwShowTick;

	CCtrlStaticWZ *			m_pCtrlTextChatMsg;

	map<int,int>			m_UseTooltipCtrls;
	int						m_iTooltipStringCode;

	RECT					m_rtDialog;
	uiBattleZoneMan *		m_pBattleZoneMan;

    CCtrlStaticWZ* m_pCtrlPartyMark;
	UI_CONTROL_BUTTON_PROPERTY( PCModeBtn, StrToWzID("B100") );
	UI_CONTROL_BUTTON_PROPERTY( LeavePartyBtn, StrToWzID("B101") );	

    DWORD                   m_blinkTick;

    vector<SKILL_EFFECT_IMAGE> status_icon_container_;

    float hp_prev_percent_;
};
#endif // __HERO_GAUGE_DIALOG_H__