#ifndef     __USER_INTERFACE_GUILDMAIN_H__
#define     __USER_INTERFACE_GUILDMAIN_H__
//------------------------------------------------------------------------------
/**
    @class uiGuildMain

    guild menu user interface

    (C) 2005 ckbang
*/
#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER

#else

#include "uiGuildMain_def.h"
#include "uiBase.h"
#include "uiGuildMan/uiGuildMan.h"

#ifdef _INTERNATIONAL_UI
    const WzID c_wiGuildFont = StrToWzID("st10");
#else
    const WzID c_wiGuildFont = StrToWzID("m212");
#endif//_INTERNATIONAL_UI


class uiGuildMan;
class uiGuildMain:
    public uiBase
{
    enum eDIALOG_POS
    {
        DLGPOS_LST_MEMBERS,
		DLGPOS_TXT_CURRENT_SELECT,
		DLGPOS_TXT_NUMBER_OF_GUILDMEMBER,
		DLGPOS_LST_CORP,
		DLGPOS_BTN_SORT_NAME,
		DLGPOS_BTN_SORT_LEVEL,
		DLGPOS_BTN_SORT_CLASS,
		DLGPOS_BTN_SORT_DUTY,
		DLGPOS_BTN_SORT_CONNECT,
        DLGPOS_CHKBTN_MEMBER_INFOS,
        DLGPOS_CHKBTN_GUILD_INFO,
        DLGPOS_CHKBTN_GUILD_STYLE,
        DLGPOS_CHKBTN_GUILD_MAN,

        DLGPOS_BTN_EXIT,
        DLGPOS_BTN_GUILD_INVITE,
        DLGPOS_BTN_GUILD_DELETE,
        
        DLGPOS_BTN_GUILD_EXIT,
        DLGPOS_BTN_GUILD_NOTICE,
        DLGPOS_BTN_DUTY_VEST,
		DLGPOS_BTN_GUILD_RELATIONSHIP,
		DLGPOS_BTN_GUILD_MASTER_DELEGATION,
		DLGPOS_BTN_GUILD_MEMBER_RIGHT_INFO,
		DLGPOS_BTN_GUILD_CORP_CHANGE,
		DLGPOS_BTN_CORP_LIST,
		DLGPOS_BTN_RIGHT_DELEGATION,


        DLGPOS_SCR_VERTICAL,

        DIALOG_MAX,
    };

    enum
    {
        POS_CHKBTN_BEGIN = DLGPOS_CHKBTN_MEMBER_INFOS,
        POS_CHKBTN_END = DLGPOS_CHKBTN_GUILD_MAN + 1,
        POS_CHKBTN_SIZE = POS_CHKBTN_END - POS_CHKBTN_BEGIN,

        POS_BTN_BEGIN = DLGPOS_BTN_EXIT,
        POS_BTN_END = DLGPOS_BTN_RIGHT_DELEGATION + 1,
        POS_BTN_SIZE = POS_BTN_END - POS_BTN_BEGIN,
    };

	enum eBTN
	{
		BTN_EXIT,
        BTN_GUILD_INVITE,
        BTN_GUILD_DELETE,
        BTN_GUILD_EXIT,
        BTN_GUILD_NOTICE,
        BTN_GUILD_DUTY_VEST,
		BTN_GUILD_RELATIONSHIP,
        BTN_GUILD_RIGHT_DELEGATION,
	};

public:
    uiGuildMain(InterfaceManager *pUIMan);
    virtual ~uiGuildMain();

    //--------------------------------	------------------------------------------
    // init/release
    //
public:
    virtual void            Init(CDrawBase* pDrawBase);
    virtual void            Release();
    virtual void			Reset();

protected:
    void                    InitControls();

    //--------------------------------------------------------------------------
    // helper/comms.
    //
public:
    uiGuildMan*             GetManager();

    //--------------------------------------------------------------------------
    // functions
    //
public:
    void                    updateMemberInfos();
    void                    renderMemberInfos();

	const TCHAR *			GetAttemptChageMasterName() { return m_strAttemptChageMasterName.c_str(); }
	void					SetMemberInfoButtonState(bool bState) {m_bClickMemberRightInfoButton = bState;}
	bool					GetMemberInfoButtonState(){return m_bClickMemberRightInfoButton;}
	stGUILD_MEMBER_INFO *	GetSelectedPacketForGuildMember();

    //--------------------------------------------------------------------------
    // dialog functions
    //
public:
	void					InsertAllCorps();
	void					SetListCaption();
	void					SetCurSelCorps(int iCorps) {m_iCurSelCorps = iCorps;}
	void					updateMouseWheel();

    int last_scrollbar_position() const { return last_scrollbar_position_; }
    void set_last_scrollbar_position(const int scrollbar_position) { last_scrollbar_position_ = scrollbar_position; }

    //! 함수 사용시 디파인 : _YMS_GUILD_DETAL_SPEC
    stGUILD_MEMBER_INFO* GetMemberInfo_byListboxCurSel();

protected:
    virtual void	        OnShowWindow( BOOL val );
    virtual void            OnUpdateSolarDialog();
    virtual void            OnRenderSolarDialog();

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

    //--------------------------------------------------------------------------
    // member vars
    //
private:

	void					OpenGuildNotice();						

	void					OnClickDutyVest();
	void					OnClickRightDelegation();
	void					OnClickGuildMasterDelegation();
	void					OnClickGuildMemberRightInfo();
	void					OnClickGuildCropChange();
    bool 					OnLDoubleButtonClick(SI_MESSAGE* message);
	util::_tstring			m_strAttemptChageMasterName;
	bool					m_bClickMemberRightInfoButton;

    uiGuildMan*             m_pGuildMan;

    CCtrlButtonCheckWZ*     m_pChkBtnControls[POS_CHKBTN_SIZE];

    CCtrlListWZ*            m_pLstControl;
	CCtrlListWZ *			m_pListCorps;
	int						m_iCurSelCorps;
	CCtrlStaticWZ *			m_pCaptinCurSel;
	CCtrlStaticWZ *			m_pStaticNumGuildMember;

    CCtrlButtonWZ*          m_pBtnControls[POS_BTN_SIZE];
    CCtrlVScrollWZ*         m_pScrollControl;

    bool                    m_bControlsInitialized;
	//bool					m_bShow;

	int						m_MaxPage;

	CControlWZ *			m_pSortName;
	CControlWZ *			m_pSortLevel;
	CControlWZ *			m_pSortClass;
	CControlWZ *			m_pSortDuty;
	CControlWZ *			m_pSortConnect;

    int last_scrollbar_position_;
};

//------------------------------------------------------------------------------
/**
*/
inline
POSTYPE
uiGuildMain::getCtrlIDToPos( WzID wzId )
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
uiGuildMain::getControl( POSTYPE AtPos )
{
    assert( AtPos < DIALOG_MAX );

    return GetControlWZ( m_wzId[AtPos] );
}

//------------------------------------------------------------------------------
/**
*/
inline
uiGuildMan*
uiGuildMain::GetManager()
{
    return this->m_pGuildMan;
}

#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
//------------------------------------------------------------------------------
#endif  //  __USER_INTERFACE_GUILDMAIN_H__
