#ifndef     __USER_INTERFACE_GUILDCREATE_H__
#define     __USER_INTERFACE_GUILDCREATE_H__
//------------------------------------------------------------------------------
/**
    @class uiGuildCreate

    guild create user interface

    (C) 2005 ckbang
*/
#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER

#else

#include "uiGuildCreate_def.h"
#include "uibase.h"
class uiGuildMan;
class uiGuildCreate:
    public uiBase
{
    enum eDIALOG_POS
    {
        DLGPOS_BTN_GUILD_CREATE,
        DLGPOS_BTN_GUILD_CANCEL,

        DLGPOS_TXT_LIMITED_LEVEL,   // 길드 창설을 위한 레벨 제한
        DLGPOS_TXT_LIMITED_HIME,    // 길드 창설을 위한 필요 하임
        
        DLGPOS_EDT_GUILD_NAME,      // 길드 이름

        DIALOG_MAX,
    };

    enum
    {
        POS_BTN_BEGIN = DLGPOS_BTN_GUILD_CREATE,
        POS_BTN_END = DLGPOS_BTN_GUILD_CANCEL + 1,
        POS_BTN_SIZE = POS_BTN_END - POS_BTN_BEGIN,

        POS_TXT_BEGIN = DLGPOS_TXT_LIMITED_LEVEL,
        POS_TXT_END = DLGPOS_TXT_LIMITED_HIME + 1,
        POS_TXT_SIZE = POS_TXT_END - POS_TXT_BEGIN,
    };

public:
    uiGuildCreate(InterfaceManager *pUIMan);
    virtual ~uiGuildCreate();

    //--------------------------------------------------------------------------
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
    TCHAR*                  GetInputGuildName();

    //--------------------------------------------------------------------------
    // functions
    //
public:
    TCHAR	                m_szGuildName[MAX_GUILDNAME_LENGTH+1];

    //--------------------------------------------------------------------------
    // dialog functions
    //
public:

protected:
    virtual void	        OnShowWindow( BOOL val );

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
    uiGuildMan*             m_pGuildMan;

    CCtrlDxIMEEditBox*      m_pEdtControl;
    CCtrlButtonWZ*          m_pBtnControls[POS_BTN_SIZE];
    CCtrlStaticWZ*          m_pTxtControls[POS_TXT_SIZE];

    bool                    m_bControlsInitialized;
};

//------------------------------------------------------------------------------
/**
*/
inline
POSTYPE
uiGuildCreate::getCtrlIDToPos( WzID wzId )
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
uiGuildCreate::getControl( POSTYPE AtPos )
{
    assert( AtPos < DIALOG_MAX );

    return GetControlWZ( m_wzId[AtPos] );
}

//------------------------------------------------------------------------------
/**
*/
inline
uiGuildMan*
uiGuildCreate::GetManager()
{
    return this->m_pGuildMan;
}

//------------------------------------------------------------------------------
/**
*/
inline
TCHAR*
uiGuildCreate::GetInputGuildName()
{
    return &this->m_szGuildName[0];
}

#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
//------------------------------------------------------------------------------
#endif  //  __USER_INTERFACE_GUILDCREATE_H__
