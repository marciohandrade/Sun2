#ifndef     __USER_INTERFACE_PARTYMEMBER_H__
#define     __USER_INTERFACE_PARTYMEMBER_H__
//------------------------------------------------------------------------------
/**
    @class uiPartyMember

    interface for a party's member.

    (C) 2005 ckbang
*/
#include "uiBase.h"
#include "uiPartyMember_def.h"
#include "..\\uiPartyMan\\uiPartyMan_def.h"

class Character;
class uiPartyMan;
class uiPartyMember :
    public uiBase
{
    enum eDIALOG_POS
    {
        DLGPOS_HPBAR,
        DLGPOS_BUFF01,
        DLGPOS_BUFF02,
        DLGPOS_BUFF03,
        DLGPOS_BUFF04,
        DLGPOS_BUFF05,
        DLGPOS_BUFF06,
        DLGPOS_BUFF07,
        DLGPOS_BUFF08,
        DLGPOS_BUFF09,
        DLGPOS_BUFF10,

        DLGPOS_FACE00,
        DLGPOS_FACE01,
        DLGPOS_FACE02,
        DLGPOS_FACE03,
        DLGPOS_FACE04,
        DLGPOS_FACE05, //_NA_004965_20120613_NEW_CHARACTER_MYSTIC
#ifdef _NA_000000_20130812_NEW_CHARACTER_HELLROID
        DLGPOS_FACE06,
#endif //_NA_000000_20130812_NEW_CHARACTER_HELLROID
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        DLGPOS_FACE07,
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

        DLGPOS_CLASS_MARK00,
        DLGPOS_CLASS_MARK01,
        DLGPOS_CLASS_MARK02,
        DLGPOS_CLASS_MARK03,
        DLGPOS_CLASS_MARK04,
        DLGPOS_CLASS_MARK05, //_NA_004965_20120613_NEW_CHARACTER_MYSTIC
#ifdef _NA_000000_20130812_NEW_CHARACTER_HELLROID
        DLGPOS_CLASS_MARK06,
#endif //_NA_000000_20130812_NEW_CHARACTER_HELLROID
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        DLGPOS_CLASS_MARK07,
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

        DLGPOS_TXT,
        DLGPOS_LEAVE,

        DLGPOS_MINE,
        DLGPOS_TARGETSHARE,
        DLGPOS_PCMARK,

#ifdef _NA_000000_20130114_RENEWER_UI
#else
		DLGPOS_MIC_ON,
#endif

        DLGPOS_MASTER_MARK,
        DLGPOS_BTN_SELECTED_MARK,

		DLGPOS_MPBAR,
		DLGPOS_LEVEL_TEXT,

        DLGPOS_HPBAR_BK,

#ifdef _NA_000000_20130114_RENEWER_UI
#else
		DLGPOS_LV_BK,
		DLGPOS_FACE_BK,
#endif
		DLGPOS_NAME,	

#ifdef _DH_BATTLEZONE2_
        DLGPOS_BATTLEZONE_READY,    //배틀존 레디
#endif //_DH_BATTLEZONE2_

#ifdef  _NA_000000_20130114_RENEWER_UI
		//추가 버프 스테이트
		DLGPOS_BUFF19,
		DLGPOS_BUFF20,
		DLGPOS_BUFF21,
		DLGPOS_BUFF22,
		DLGPOS_BUFF23,
		DLGPOS_BUFF24,
		DLGPOS_BUFF25,
		DLGPOS_BUFF26,
		DLGPOS_BUFF27,
		DLGPOS_BUFF28,

		DLGPOS_SPBAR,

#endif
		DIALOG_MAX,
    };

	enum BUF_CONTROLS
	{
		BUFF01 = 0,
		BUFF02,
		BUFF03,
		BUFF04,
		BUFF05,
		BUFF06,
		BUFF07,
		BUFF08,
		BUFF09,
		BUFF10,
#ifdef  _NA_000000_20130114_RENEWER_UI
		BUFF11,
		BUFF12,
		BUFF13,
		BUFF14,
		BUFF15,
		BUFF16,
		BUFF17,
		BUFF18,
		BUFF19,
		BUFF20,
#endif
		BUF_MAX,
	};

	enum FACE_CONTROLS
	{
		FACE00 = 0,
		FACE01,
		FACE02,
		FACE03,
		FACE04,
		FACE05,
		FACEBK,
		FACE_MAX,
	};

	enum GAGUE_CONTROLS
	{
		HPBAR = 0,
		MPBAR,
		HPMP_BK,
		GAGUE_MAX,
	};
#ifdef _DH_BATTLEZONE2_
    enum OUTPUT_ZONE_TYPE
    {
        OUTPUT_ZONE_NONE,    // 히어로와 같음 (출력 안함)
        OUTPUT_ZONE_FILED,   // 히어로는 배틀존, 대상은 필드
        OUTPUT_ZONE_BATTLEZONE, // 히어로는 필드, 대상은 배틀존
    };
#endif //_DH_BATTLEZONE2_
public:
    uiPartyMember(InterfaceManager *pUIMan);
    virtual ~uiPartyMember();

    //--------------------------------------------------------------------------
    // init/release~
    //
public:
    virtual void            Init(CDrawBase* pDrawBase);
    virtual void            Release();
	void					InitPartViewPos();
    void                    ShowFaceControl(bool visibility);
    void                    ShowGagueControl(bool visibility);

    //--------------------------------------------------------------------------
    // helper/comms.
    //
public:
    uiPartyMan*             GetManager();
    short                   GetMyDlgPos();

    const uicPartyMemberInfoPlus&   GetMyMemberInfo();

    //--------------------------------------------------------------------------
    // functions
    //
public:
    void                    SetTargeting();
    void                    doLeave();
    void                    TargetShare(bool bAttack);

    //--------------------------------------------------------------------------
    // dialog functions
    //
public:
    void                    ResetHPGauge();
	void                    ResetMPGauge();

    void                    OnChangeInfo();

	void					SetGap( int gap ) { m_nGap = gap; }
	int						GetGap() const { return m_nGap; }
	void					SetPartView();
    void                    UpdateStatusIcon();
#ifdef _DH_BATTLEZONE2_
    void                    SetOutputZoneType(OUTPUT_ZONE_TYPE output_zone_type);
    void                    CheckOutputZoneType();
#endif //_DH_BATTLEZONE2_

protected:
    virtual void	        OnShowWindow( BOOL val );
    virtual void            OnRenderSolarDialog();
    virtual void            OnUpdateSolarDialog();

    void                    updateInfos();
    void                    updateHP();
	void					updateMP();
#ifdef _NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
    void                    updateSD();
#endif //_NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS

    void                    renderStringInfo();

    void                    SetHP( float fHPPercent );
	void                    SetMP( float fMPPercent );
#ifdef _NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
    void                    SetSD( float fSDPercent );
#endif //_NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS

    CCtrlButtonWZ*          GetCtrlBtnLeave();

protected:
    CControlWZ *	        getControl( POSTYPE AtPos );
    POSTYPE			        getCtrlIDToPos( WzID wzId );
    static WzID		        m_wzId[DIALOG_MAX];

    //--------------------------------------------------------------------------
    // procedure
    //
public:
    virtual void	        NetworkProc( MSG_BASE * pMsg );         // Pure
    virtual void	        MessageProc( SI_MESSAGE * pMessage );   // Pure
    virtual void			MoveDefaultWindowPos();

    //--------------------------------------------------------------------------
    // member vars
    //
private:
    CCtrlButtonWZ*          m_ctrlBtnMaster;

	CCtrlStaticWZ*			m_ctrlLevelStatic;
    CCtrlPictureWZ*         m_ctrlGaugeBackground;
    float			        m_fGaugeSize;
    RECT                    m_rcInit;

	float			        m_fMPGaugeSize;
	RECT                    m_rcMPInit;

    vector<SKILL_EFFECT_IMAGE> status_icon_container_;

	int						m_nGap;
    DWORD                   m_blinkTick;

	float 				m_fTop;
	float 				m_fLeft;
	bool				m_bIsShowFace;
	bool				m_bIsShowGague;
	bool				m_bIsShowBuf;

	RECT 				m_rcName;
	RECT 				m_rcGague;
	RECT 				m_rcBuf;
	bool				m_bInitPartPos;
    bool                m_bHpBlinkType;

	RECT				rect_pos_init[4];
	RECT				rect_pos_string;

#ifdef _DH_BATTLEZONE2_
    bool                mouse_over_;
    OUTPUT_ZONE_TYPE output_zone_type_;
#endif //_DH_BATTLEZONE2_
};

//------------------------------------------------------------------------------
/**
*/
inline
POSTYPE
uiPartyMember::getCtrlIDToPos( WzID wzId )
{
    for( POSTYPE i = 0 ; i < DIALOG_MAX ; ++i )
    {
        if( m_wzId[i] == wzId )
        {
            return i;
        }
    }
    return DIALOG_MAX;
}

//------------------------------------------------------------------------------
/**
*/
inline
CControlWZ *
uiPartyMember::getControl( POSTYPE AtPos )
{
    assert( AtPos < DIALOG_MAX );

    return GetControlWZ( m_wzId[AtPos] );
}

//------------------------------------------------------------------------------
/**
*/
inline
void
uiPartyMember::ResetHPGauge()
{
    this->m_fGaugeSize = -1;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
uiPartyMember::ResetMPGauge()
{
    this->m_fMPGaugeSize = -1;
}
//------------------------------------------------------------------------------

#ifdef _DH_BATTLEZONE2_
inline
void
uiPartyMember::SetOutputZoneType(OUTPUT_ZONE_TYPE output_zone_type)
{
    output_zone_type_ = output_zone_type;
}
#endif //_DH_BATTLEZONE2_
//------------------------------------------------------------------------------

#endif  //  __USER_INTERFACE_PARTYMEMBER_H__
