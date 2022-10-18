#ifndef     __USER_INTERFACE_GUILDMENU_H__
#define     __USER_INTERFACE_GUILDMENU_H__
//------------------------------------------------------------------------------
/**
    @class uiGuildMenu

    guild menu user interface

    (C) 2005 ckbang
*/
#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER

#else

#include "uiGuildMenu_def.h"
#include "uibase.h"

class uiGuildMan;
class uiGuildMenu:
    public uiBase
{
    enum eDIALOG_POS
    {
		DLGPOS_BTN_EXIT,
		DLGPOS_BTN_GUILD_CREATE_AND_DESTORY,
		DLGPOS_BTN_GUILD_RANK_UP,
		DLGPOS_BTN_CORP_AND_CAMP_CREATE,
		DLGPOS_BTN_CORP_AND_CAMP_DESTORY,
		DLGPOS_BTN_GUILD_MARK,
		DLGPOS_BTN_GUILD_WAREHOUSE,
		DLGPOS_LST_GUILD_NOTICE,

        DIALOG_MAX,
    };

    enum
    {
        POS_BTN_BEGIN = DLGPOS_BTN_EXIT,
        POS_BTN_END = DLGPOS_BTN_GUILD_WAREHOUSE + 1,
        POS_BTN_SIZE = POS_BTN_END - POS_BTN_BEGIN,
    };

public:
    uiGuildMenu(InterfaceManager *pUIMan);
    virtual ~uiGuildMenu();

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

    //--------------------------------------------------------------------------
    // dialog functions
    //
public:

protected:
    virtual void	        OnShowWindow( BOOL val );
    virtual void            OnUpdateSolarDialog();

protected:
    CControlWZ *	        getControl( POSTYPE AtPos );
    POSTYPE			        getCtrlIDToPos( WzID wzId );
    static WzID		        m_wzId[DIALOG_MAX];

	void					OnClickCorpNCampDestory();
	void					OnClickCorpNCampCreate();
	void					OnClickGuildCreateNDestory();
	void					UpdateButtonShowNHide();
	void					UpdateButtonCaption();

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
    uiGuildMan*             m_pGuildMan;
    CCtrlListWZ*            m_pListControl;
    CCtrlButtonWZ*          m_pBtnControls[POS_BTN_SIZE];

    bool                    m_bControlsInitialized;

	bool					m_bShow;
};

//------------------------------------------------------------------------------
/**
*/
inline
POSTYPE
uiGuildMenu::getCtrlIDToPos( WzID wzId )
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
uiGuildMenu::getControl( POSTYPE AtPos )
{
    assert( AtPos < DIALOG_MAX );

    return GetControlWZ( m_wzId[AtPos] );
}

//------------------------------------------------------------------------------
/**
*/
inline
uiGuildMan*
uiGuildMenu::GetManager()
{
    return this->m_pGuildMan;
}


#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
//------------------------------------------------------------------------------
#endif  //  __USER_INTERFACE_GUILDMENU_H__
