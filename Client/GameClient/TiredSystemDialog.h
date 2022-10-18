#pragma once

#include "uiBase.h"
#include "uibattlezoneman/uiBattleZoneMan.h"

#define BLOCK_PCBANG_USER_NOTICE_TIME	10

class InterfaceManager;

enum TiredSystemDialog_Pos
{
	TIREDSYSTEM_TIME,
	TIREDSYSTEM_PICTURE,
#ifdef _SCJ_EXP_EVENT_ENABLE_EFFECT
    TIREDSYSTEM_EXP_EVENT,
#endif //_SCJ_EXP_EVENT_ENABLE_EFFECT
	TIREDSYSTEM_DIALOG_MAX,
};


class TiredSystemDialog : public uiBase
{
	enum
	{
		eTIRED_NO,
		eTIRED_75PERSENT,
		eTIRED_100PERSENT,
		eTIRED_MAX,
	};

public:
	TiredSystemDialog(InterfaceManager * pUIman);
	~TiredSystemDialog(void);

	VOID					Init( CDrawBase * pDrawBase );
	virtual VOID			Release();
	virtual VOID			MessageProc( SI_MESSAGE * pMessage );
	virtual VOID			NetworkProc( MSG_BASE * pMsg );
	virtual VOID    		Process(DWORD dwTick);
	virtual VOID			OnUpdateSolarDialog();
	virtual VOID			OnRenderSolarDialog();
	virtual VOID			OnShowWindow(BOOL val);

	void					SetTime(int iTime) {m_iTime = iTime;};
	void					RenderTiredsystemMessage(int iTime);

	BOOL					GetUseFatigue() const;
	void					SetUseFatigue( BOOL bUseFatigue ) { m_bUseFatigue = bUseFatigue; }
	void					SetOffLineTime( DWORD time ) { m_dwOffLineTime = time; }

	void					SetOffLineTimeOfFatigueString( const std::string & str ) { m_strOffLineTimeOfFatigue = str; }
	void					OffLineTimeOfFatigueMessage( void );

	void					MoveuiForce(int _x,int _y);

#ifdef _SCJ_EXP_EVENT_ENABLE_EFFECT
    void                    InitPeakTimeEffect() {peak_start_effect_ = false;}
#endif //_SCJ_EXP_EVENT_ENABLE_EFFECT
protected:
	CControlWZ *			getControl( POSTYPE AtPos );
	POSTYPE					getCtrlIDToPos( WzID wzId );

	void					RenderTiredsystemMessage();
	void					RenderSystemTime();
	void					CalculateTiredLevel();
	int						GetTiredLevel();
	void					UpdateEachCaseTime();
	void					UpdatePictureGaugeBar();

	void					UpdateToolTip();
	
#ifdef _SCJ_EXP_EVENT_ENABLE_EFFECT
    void                    RenderPeakTime();
    void                    UpdatePeakTime();
#endif //_SCJ_EXP_EVENT_ENABLE_EFFECT
#ifdef _NA_007935_20150109_GM_COMMAND_EXP_EVENT
public:
	// setter/getter
	float                   get_gm_event_exp_ratio_() const { return gm_event_exp_ratio_; }
	void                    set_gm_event_exp_ratio_(const float& gm_event_exp_ratio) { gm_event_exp_ratio_ = gm_event_exp_ratio; }
#endif // _NA_007935_20150109_GM_COMMAND_EXP_EVENT

private:
	static WzID				m_wzId[TIREDSYSTEM_DIALOG_MAX];
	CCtrlStaticWZ*			m_pStatic;
	CCtrlPictureWZ*			m_pPicture;

	BOOL					m_bOncePrint[2];
	int						m_iTime;
	int						m_iTiredLevel;
	int 					m_iPendingTime;
	int 					m_iAllAreaTime;
	int 					m_iPendingAllAreaHour;
	int 					m_iPendingAllAreaTime;
	int						m_iPendingPCBangBlockNoticeTime;
	float					m_fPictureHeight;
	float   				m_fPictureWidth;
	float					m_fGaugeWidth;
	RECT 					m_rtPicture;
	RECT 					m_rtSizePicture;
	SYSTEMTIME				m_sysLocalTime;
	BOOL					m_bMouseOver;
	BOOL					m_bUseFatigue;
	DWORD					m_dwOffLineTime;
	static BOOL				m_bOnceMessage;				// 첨 로그인시 한번만 보여준다.
#ifdef _SCJ_EXP_EVENT_ENABLE_EFFECT
    COLORREF                base_text_color_;
    COLORREF                peak_time_text_color_;
    int                     peak_time_start_tick_;      //피크타임 활성화 시작 시간
    bool                    peak_start_effect_;         //피크타임 시작 이펙트가 시작되었는지
    float                   peak_reward_exp_rate;       //피크타임 경험치 증가율
#endif //_SCJ_EXP_EVENT_ENABLE_EFFECT
#ifdef _NA_007935_20150109_GM_COMMAND_EXP_EVENT
	float                   gm_event_exp_ratio_;        // GM이벤트 경험치를 저장.
#endif // _NA_007935_20150109_GM_COMMAND_EXP_EVENT
	
	std::string				m_strOffLineTimeOfFatigue;

	uiBattleZoneMan *		m_pBattlezoneMan;
};

