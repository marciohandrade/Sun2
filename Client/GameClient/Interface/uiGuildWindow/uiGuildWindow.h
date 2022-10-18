#ifndef     __USER_INTERFACE_GUILDWINDOW_H__
#define     __USER_INTERFACE_GUILDWINDOW_H__
//------------------------------------------------------------------------------
/**
    @class uiGuildWindow

    guild window user interface

    (C) 2005 ckbang
*/
#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER

#else

#include "uiGuildWindow_def.h"
#include "uiBase.h"
#include "GlobalFunc.h"
#include "InterfaceManager.h"

class uiGuildMan;
class uiGuildWindow :
    public uiBase
{
    enum eDIALOG_POS
    {
        DLGPOS_CHKBTN_MEMBER_INFOS,
        DLGPOS_CHKBTN_GUILD_INFO,
        DLGPOS_CHKBTN_GUILD_STYLE,
        DLGPOS_CHKBTN_GUILD_MAN,

        DLGPOS_BTN_EXIT,

        DLGPOS_TXT_GUILD_NAME,
        DLGPOS_TXT_MASTER_NAME,
        DLGPOS_TXT_GUILD_LEVEL,
        DLGPOS_TXT_NUM_MEMBERS,
        DLGPOS_TXT_GUILD_UP,
        DLGPOS_TXT_PERSON_UP,
        DLGPOS_TXT_GUILD_OWN_GP,
        DLGPOS_TXT_DOMINATION_MAP_COUNT,
        DLGPOS_TXT_DOMINATION_JOIN_COUNT,
        DLGPOS_TXT_DOMINATION_DEFENSE_COUNT,
        DLGPOS_TXT_DOMINATION_BOSS_HUNTTING_COUNT,

#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
		DLGPOS_TXT_GUILD_PVP_RESULT,
		DLGPOS_TXT_GUILD_PVP_POINT,
#endif
		
		DLGPOS_LST_GUILD_NOTICE,

		DLGPOS_VSCROLL_NOTICE,

		DLGPOS_TI_GUILD_MARK_REGISTER,
		DLGPOS_TI_GUILD_GP_EXCHANGE,
		DLGPOS_TI_GUILD_UP_DONATE,
        DLGPOS_TI_GUILD_DOMINATION_MANAGEMENT,

        DIALOG_MAX,
    };

    enum
    {
        POS_CHKBTN_BEGIN = DLGPOS_CHKBTN_MEMBER_INFOS,
        POS_CHKBTN_END = DLGPOS_CHKBTN_GUILD_MAN + 1,
        POS_CHKBTN_SIZE = POS_CHKBTN_END - POS_CHKBTN_BEGIN,

        POS_TXT_BEGIN = DLGPOS_TXT_GUILD_NAME,

		POS_TXT_END = DLGPOS_LST_GUILD_NOTICE,
        POS_TXT_SIZE = POS_TXT_END - POS_TXT_BEGIN,
		
    };

public:
    uiGuildWindow(InterfaceManager *pUIMan);
    virtual ~uiGuildWindow();

    //--------------------------------	------------------------------------------
    // init/release
    //
public:
    virtual void            Init(CDrawBase* pDrawBase);
    virtual void            Release();

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
    void                    updateGuildInfo();
	void					setGuildNotice();

    //--------------------------------------------------------------------------
    // dialog functions
    //
public:

protected:
    virtual void	        OnShowWindow( BOOL val );
    virtual void            OnUpdateSolarDialog();

	void					refreshGuildNotice();
    void                    ClickDominationManagementButton();

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
    uiGuildMan*                     m_pGuildMan;

    CCtrlStaticWZ*                  m_pTxtControls[POS_TXT_SIZE];
    CCtrlButtonCheckWZ*             m_pChkBtnControls[POS_CHKBTN_SIZE];
    CCtrlButtonWZ*                  m_pExitBtnControl;

    bool                            m_bControlsInitialized;
	//bool							m_bShow;

	UI_CONTROL_STATIC_PROPERTY(GuildGradeName, StrToWzID("ST00"));

	CCtrlListWZ *					m_pLstControlNotice;
	std::vector<util::_tstring>		m_vstrNotice;
	CCtrlVScrollWZ*					m_pScroll;

};

//------------------------------------------------------------------------------
/**
*/
inline
POSTYPE
uiGuildWindow::getCtrlIDToPos( WzID wzId )
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
uiGuildWindow::getControl( POSTYPE AtPos )
{
    assert( AtPos < DIALOG_MAX );

    return GetControlWZ( m_wzId[AtPos] );
}

//------------------------------------------------------------------------------
/**
*/
inline
uiGuildMan*
uiGuildWindow::GetManager()
{
    return this->m_pGuildMan;
}

#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
//------------------------------------------------------------------------------
#endif  //  __USER_INTERFACE_GUILDWINDOW_H__
