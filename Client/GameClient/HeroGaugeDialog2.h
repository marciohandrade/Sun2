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

#ifdef _NA_000000_20130114_RENEWER_UI

#include "uiBase.h"
#include "globalfunc.h"
#include "interfacemanager.h"
#include "IconMgr.h"
#include "uibattlezoneman/uiBattleZoneMan_def.h"
#include "uibattlezoneman/uiBattleZoneMan.h"

#define WARNING_HP			20.0f
#define BLINK_GAP_TICK		1000

class InterfaceManager;
class uiSkillMan;
class uiPerkDialog;
class ItemUnitRenderer;

struct stHITEFFECT
{

};

class HeroGaugeDialog2 : public uiBase
{
	enum eHGAUGE_POS
	{
		//sting관련
		eControl_Static_SD_S000,
		eControl_Static_HP_S001,
		eControl_Static_MP_S002,
		eControl_Static_None_S003,

		//SD,HP,MP,SP
		eControl_Picture_SD_P024,
		eControl_Picture_HP_P025,
		eControl_Picture_MP_P026,
		eControl_Picture_SP_P027,

		//패시브 슬롯 5개
		eControl_Button_PasSlot_B001,
		eControl_Button_PasSlot_B002,
		eControl_Button_PasSlot_B003,
		eControl_Button_PasSlot_B004,
		eControl_Button_PasSlot_B000,

		//버프 슬롯 20개
		eControl_Picture_Buff_P004,	
		eControl_Picture_Buff_P005,	
		eControl_Picture_Buff_P006,	
		eControl_Picture_Buff_P007,	
		eControl_Picture_Buff_P008,	
		eControl_Picture_Buff_P009,	
		eControl_Picture_Buff_P010,	
		eControl_Picture_Buff_P011,	
		eControl_Picture_Buff_P012,	
		eControl_Picture_Buff_P013,	
		eControl_Picture_Buff_P014,	
		eControl_Picture_Buff_P015,	
		eControl_Picture_Buff_P016,	
		eControl_Picture_Buff_P017,	
		eControl_Picture_Buff_P018,	
		eControl_Picture_Buff_P019,	
		eControl_Picture_Buff_P020,	
		eControl_Picture_Buff_P021,	
		eControl_Picture_Buff_P022,	
#ifdef _NA_008139_20150318_MODIFY_BUFF_ICON_UI
#else
		eControl_Picture_Buff_P023,	
#endif //_NA_008139_20150318_MODIFY_BUFF_ICON_UI

		eControl_Picture_Buff_P031,
		eControl_Picture_Buff_P032,
		eControl_Picture_Buff_P033,
		eControl_Picture_Buff_P034,
		eControl_Picture_Buff_P035,
		eControl_Picture_Buff_P036,
		eControl_Picture_Buff_P037,
		eControl_Picture_Buff_P038,
		eControl_Picture_Buff_P039,
		eControl_Picture_Buff_P040,

		eControl_Picture_Buff_P041,
		eControl_Picture_Buff_P042,
		eControl_Picture_Buff_P043,
		eControl_Picture_Buff_P044,
		eControl_Picture_Buff_P045,
		eControl_Picture_Buff_P046,
		eControl_Picture_Buff_P047,
		eControl_Picture_Buff_P048,
		eControl_Picture_Buff_P049,
#ifdef _NA_008139_20150318_MODIFY_BUFF_ICON_UI
#else
		eControl_Picture_Buff_P050,
#endif //_NA_008139_20150318_MODIFY_BUFF_ICON_UI


		//gauge bar 3
		eControl_Picture_SDGauge_PI13,
		eControl_Picture_HPGauge_P000,
		eControl_Picture_SPGauge_PI02,
		eControl_Picture_MPGauge_P001,

		//reverse bar 2
		eControl_Picture_HPRecGauge_PI01,
		eControl_Picture_MPSPRecGauge_P001,
		//얼굴
		eControl_Picture_HeadPI03,

#ifdef _NA_007019_20140107_PERK_CHANGE_SYSTEM
        eControl_Button_PerkStyleA,
        eControl_Button_PerkStyleB,
#endif // _NA_007019_20140107_PERK_CHANGE_SYSTEM

		HGAUGE_MAX,
	};	

public:
	HeroGaugeDialog2();
	virtual ~HeroGaugeDialog2();

	VOID			Init( CDrawBase * pDrawBase );
	virtual VOID	MessageProc( SI_MESSAGE * pMessage );
	virtual VOID	NetworkProc( MSG_BASE * pMsg );

	virtual VOID	OnRenderSolarDialog();
	virtual VOID	OnUpdateSolarDialog();
	virtual void			Release();
	virtual VOID	OnShowWindow( BOOL val );

protected:
	CControlWZ *	getControl( POSTYPE AtPos );
	POSTYPE			getCtrlIDToPos( WzID wzId );
	static WzID		m_wzId[HGAUGE_MAX];

	CCtrlPictureWZ* hero_hp_bar_;
	CCtrlPictureWZ* hero_SD_bar_;
	CCtrlPictureWZ* hero_SP_bar_;
	CCtrlPictureWZ* hero_MP_bar_;
	CCtrlPictureWZ* hero_hp_rev_bar_;

	CControlWZ*		hp_current_max_;
	CControlWZ*		sd_current_max_;
	CControlWZ*		mp_current_max_;
	
	HANDLE			m_hTextureHeadMask;
	HANDLE			m_hTextureGaugeEffect;

	RECT			Effect_Rect_Pos_[4][3];
	RECT			Rect_post_is_Perk_;
	
public:
	void MakeHitEffect();
	void SetParentdialog(uiBase* ui) { ui_parent_dialog_ = ui; }
	float hp_prev_percent() const { return hp_prev_percent_; }
	void hp_prev_percent_increase(float val);
	void set_hp_prev_percent(float val);
#ifdef _NA_007019_20140107_PERK_CHANGE_SYSTEM
    void set_perk_style_change(const int& style);
#endif // _NA_007019_20140107_PERK_CHANGE_SYSTEM
	void UpdateStatusIcon();

	void			SetHeroReserveHP( float val );
	VOID			SetHeroSD( float fHPPercent );
	VOID			SetHeroMP( float fMPPercent );
	VOID			SetHeroSP( float fSPPercent );
	VOID			SetHeroHP( float fHPPercent );
	VOID			SetLevel( DWORD dwLevel );

	VOID			UpdateHeroState();
	void			UpdateHitEffect();

	void			RenderText();
	void			RenderBarEffect();
	void			RenderCharacterHeader();
	void			RenderPerkSlot();
	void			RenderTooltip();

	void            Process(DWORD dwTick);

	void			MoveWindowForce(float addx, float addy);
	virtual void	MoveDefaultWindowPos();

private:
	float hp_prev_percent_;
	float hp_gauge_size_;
	float mp_gauge_size_;
	float sp_gauge_size_;
	float sd_gauge_size_;
	float hp_rev_size_;

	float hp_post_gauge_;
	float mp_post_gauge_;
	float sp_post_gauge_;
	float sd_post_gauge_;

	bool  use_effect_texture_[4];
	float Hit_Effect_Delay_[4];
	float Hit_Effect_Hight_[4];
	float Hit_Effect_width_[4];

	bool is_mouse_over_in_perk_slot_;
	POSTYPE mouse_over_perk_slot_;

	bool sd_using_character_;
	int mouseover_status_icon_index_;
	vector<SKILL_EFFECT_IMAGE> status_icon_container_;

	uiPerkDialog* ui_perk_dialog_;
	uiBase*		  ui_parent_dialog_;
};

#endif

#endif // __HERO_GAUGE_DIALOG_H__