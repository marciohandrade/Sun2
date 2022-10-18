//=======================================================================================================================
/// TargetDialog
/**
	@author
			유재영 < y2jinc@webzen.co.kr >
	@since

	@remarks
			- 		
*/


#pragma once
#include "uiBase.h"

class Object;
class Character;
class InterfaceManager;

class TargetDialog :
	public uiBase
{
public:
	enum eTARGET_POS
	{
		TARGET_PIC_GAUGE = 0, 
		TARGET_PIC_GAUGE_BACK,

#ifdef _NA_008139_20150318_MODIFY_BUFF_ICON_UI
#else
		TARGET_SKILL_EFFECT1, 
#endif //_NA_008139_20150318_MODIFY_BUFF_ICON_UI
		TARGET_SKILL_EFFECT2, 
		TARGET_SKILL_EFFECT3, 
		TARGET_SKILL_EFFECT4, 
		TARGET_SKILL_EFFECT5, 
		TARGET_SKILL_EFFECT6, 
		TARGET_SKILL_EFFECT7, 
		TARGET_SKILL_EFFECT8, 
		TARGET_SKILL_EFFECT9, 
		TARGET_SKILL_EFFECT10, 

#ifdef _NA_000000_20130114_RENEWER_UI
		TARGET_SKILL_EFFECT11, 
		TARGET_SKILL_EFFECT12, 
		TARGET_SKILL_EFFECT13, 
		TARGET_SKILL_EFFECT14, 
		TARGET_SKILL_EFFECT15, 
		TARGET_SKILL_EFFECT16, 
		TARGET_SKILL_EFFECT17, 
		TARGET_SKILL_EFFECT18, 
		TARGET_SKILL_EFFECT19, 
  #ifdef _NA_008139_20150318_MODIFY_BUFF_ICON_UI
  #else
		TARGET_SKILL_EFFECT20, 
		TARGET_SKILL_EFFECT21, 
  #endif //_NA_008139_20150318_MODIFY_BUFF_ICON_UI
		TARGET_SKILL_EFFECT22, 
		TARGET_SKILL_EFFECT23, 
		TARGET_SKILL_EFFECT24, 
		TARGET_SKILL_EFFECT25, 
		TARGET_SKILL_EFFECT26, 
		TARGET_SKILL_EFFECT27, 
		TARGET_SKILL_EFFECT28, 
		TARGET_SKILL_EFFECT29, 
		TARGET_SKILL_EFFECT30,
		TARGET_SKILL_EFFECT31, 
		TARGET_SKILL_EFFECT32, 
		TARGET_SKILL_EFFECT33, 
		TARGET_SKILL_EFFECT34, 
		TARGET_SKILL_EFFECT35, 
		TARGET_SKILL_EFFECT36, 
		TARGET_SKILL_EFFECT37, 
		TARGET_SKILL_EFFECT38, 
		TARGET_SKILL_EFFECT39, 
  #ifdef _NA_008139_20150318_MODIFY_BUFF_ICON_UI
  #else
		TARGET_SKILL_EFFECT40,
  #endif //_NA_008139_20150318_MODIFY_BUFF_ICON_UI
#endif
		TARGET_TEXT_INFO,
		TARGET_TEXT_HP_INFO,
		TARGET_PIC_ELI,
		TARGET_PIC_VAR,
		TARGET_PIC_SHA,
		TARGET_PIC_DRA,
		TARGET_PIC_BER,//은색
		TARGET_PIC_MON,
		TARGET_PIC_GUILD_MARK,
		TARGET_TEXT_MONSTER_INFO,

        TARGET_PICTURE_FIRE,//FIRE
        TARGET_PICTURE_WATER,//WATER
        TARGET_PICTURE_WIND,//WIND
        TARGET_PICTURE_EARTH,//EARTH
        TARGET_PICTURE_DARK,//DARK
        TARGET_PICTURE_GOLD,//금색
        TARGET_BUTTON_OPEN,//열기
        TARGET_BUTTON_CLOSE,//닫기

#ifdef _NA_000000_20130114_RENEWER_UI
		TARGET_SCORE_STRING_S001,
#else
#ifdef _NA_002935_20110704_ITEM_SCORE
        TARGET_SCORE_BG,        // 장비평점
        TARGET_SCORE_NUMBER1,
        TARGET_SCORE_NUMBER2,
        TARGET_SCORE_NUMBER3,
        TARGET_SCORE_NUMBER4,
        TARGET_SCORE_NUMBER5,
#endif //_NA_002935_20110704_ITEM_SCORE
#endif

#ifdef _NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO
        TARGET_PIC_SD_GAUGE, // 쉴드 게이지
        TARGET_TEXT_SD_INFO, // 쉴드 스트링
        TARGET_PIC_SECOND_GAUGE, // 밑 게이지
        TARGET_PIC_SECOND_GAUGE_BACK, // 밑 게이지 배경
        TARGET_TEXT_SECOND_HP_INFO, // 밑 스트링
#endif //_NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO

#ifdef _NA_007113_20140324_SHADOW_SKILL_BALLANCE_RENEWAL
        TARGET_BUTTON_SUMMON_CANCLE,    // 소환수 취소(역소환)
#endif //_NA_007113_20140324_SHADOW_SKILL_BALLANCE_RENEWAL

		Dialog_Max,
	};

#define MAX_TARGETDLG_SKILL_EFFECT 10

    enum kElement_Color
    {
        kElement_Color_Fire = 0,
        kElement_Color_Water,
        kElement_Color_Wind ,
        kElement_Color_Earth,
        kElement_Color_Dark,
        kElement_Color_Common,
        kElement_Color_Max,
    };

    enum kShow_Value
    {
        kShow_Value_Open,
        kShow_Value_Close,
        kShow_Value_Max,
    };

#ifdef _NA_000000_20130114_RENEWER_UI
#else
  #ifdef _NA_002935_20110704_ITEM_SCORE
    enum
    { 
        TARGET_SCORE_START = TARGET_SCORE_BG,
        TARGET_SCORE_END   = TARGET_SCORE_NUMBER5 +1,
    };
  #endif //_NA_002935_20110704_ITEM_SCORE
#endif

#ifdef _NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO
    enum
    {
        TARGET_SECOND_GAUGE_START = TARGET_PIC_SD_GAUGE,
        TARGET_SECOND_GAUGE_END = TARGET_TEXT_SECOND_HP_INFO,
    };
#endif //_NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO

public:
	TargetDialog(InterfaceManager *pUIMan);
	virtual ~TargetDialog(void);

	VOID			Init( CDrawBase * pDrawBase );
	virtual VOID	Release();

	virtual VOID	MessageProc( SI_MESSAGE * pMessage );
	virtual VOID	NetworkProc( MSG_BASE * pMsg );

	virtual VOID	OnUpdateSolarDialog();
	virtual VOID	OnRenderSolarDialog();
	virtual VOID    Process(DWORD dwTick);

    VOID			SetTargetName(TCHAR* pszTargetName);
	VOID			SetTargetKey(DWORD dwTargetKEY);
    DWORD           GetTargetKey();
    DWORD           GetColorText(Monster* monster_ptr);
	TCHAR*			GetTargetName();

    void            UpdateStatusIcon();
#ifdef _NA_000000_20130114_RENEWER_UI
	void			MoveWindowForce(float addx, float addy);
#endif

protected:
	CControlWZ *	getControl( POSTYPE AtPos );
	POSTYPE			getCtrlIDToPos( WzID wzId );

	virtual VOID	OnShowWindow( BOOL val );
	
	VOID			SetTargetInfo();
#ifdef _NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO
    VOID            SetHP(float hp_ratio ,bool is_player);
    VOID            SetSD(float sd_ratio); // 쉴드
#else
    VOID            SetHP( float fHPPercent ,float fRadio);
#endif //_NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO
	
    void            ShowAggroDialog();
	BOOL			SetPartyMember();

private:
	void			_showCharPopupMenu(BOOL bShow);
	void			_targetChange(BOOL bPlayer); 
	void			_renderGuildMark();

	bool			CheckShowState();

#ifdef _NA_000000_20130114_RENEWER_UI
	void            DrawItemScore(); //아이템 평점 출력
	void            UpdateItemScore();
	int             GetTargetItemScore();
	virtual void	MoveDefaultWindowPos();
#else
#ifdef _NA_002935_20110704_ITEM_SCORE
    void            DrawItemScore(); //아이템 평점 출력
    void            UpdateItemScore();
    int             GetTargetItemScore();
#endif //_NA_002935_20110704_ITEM_SCORE
#endif

private:
	DWORD			m_dwTargetKEY;
	DWORD			m_dwPrevTargetKEY;
	static WzID	    m_wzId[Dialog_Max];
	
	util::_tstring	m_dwTargetName;

	TCHAR			m_szResultInfo[INTERFACE_STRING_LENGTH];
	TCHAR			m_szResultHpInfo[INTERFACE_STRING_LENGTH];


    float			m_fGaugeSize;
	float			m_fGaugeBackSize;

	
	RECT			m_rcGauge;

    vector<SKILL_EFFECT_IMAGE> status_icon_container_;
	int             m_iHpRequestTime;

    CCtrlPictureWZ* picture_element_frames_[kElement_Color_Max];
    CCtrlButtonWZ* show_aggro_window_buttons_[kShow_Value_Max];
    bool is_show_aggro_;
};

inline
DWORD
TargetDialog::GetTargetKey()
{
    return m_dwTargetKEY;
}

inline TCHAR* TargetDialog::GetTargetName()
{
	return const_cast<TCHAR*>(m_dwTargetName.c_str());
}