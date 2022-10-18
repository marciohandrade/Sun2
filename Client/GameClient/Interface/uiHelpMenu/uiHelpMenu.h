#pragma once
//------------------------------------------------------------------------------
//
//    작업자: 이지연
//    uiHelpMenu class
//------------------------------------------------------------------------------

#include "uiHelpMenu_def.h"
#include "uibase.h"
class uiHelpMan;

class uiHelpMenu: public uiBase
{
    enum eDIALOG_POS
    {
        DLGPOS_BTN_ICON1,
        DLGPOS_BTN_ICON2,
		DLGPOS_BTN_ICON3,

        DIALOG_MAX,
    };

    enum
    {
        POS_BTN_SIZE = DLGPOS_BTN_ICON3 - DLGPOS_BTN_ICON1 + 1,
    };

public:
    uiHelpMenu(InterfaceManager *pUIMan);
    virtual ~uiHelpMenu();

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
    uiHelpMan*             GetManager();

    //--------------------------------------------------------------------------
    // functions
    //
public:

    //--------------------------------------------------------------------------
    // dialog functions
    //
private:
	BOOL					_isEnable();

public:

protected:
    virtual void	        OnShowWindow( BOOL val );
    virtual void            OnRenderSolarDialog();
	virtual void            Process(DWORD dwTick);

public:
	virtual void            UpdatePosition();

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
    uiHelpMan*				m_pHelpMan;

    CCtrlButtonWZ*          m_pBtnControls[POS_BTN_SIZE];

    bool                    m_bControlsInitialized;
};

//------------------------------------------------------------------------------
/**
*/
inline
POSTYPE
uiHelpMenu::getCtrlIDToPos( WzID wzId )
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
uiHelpMenu::getControl( POSTYPE AtPos )
{
    assert( AtPos < DIALOG_MAX );

    return GetControlWZ( m_wzId[AtPos] );
}

//------------------------------------------------------------------------------
/**
*/
inline
uiHelpMan*
uiHelpMenu::GetManager()
{
    return this->m_pHelpMan;
}

//------------------------------------------------------------------------------

