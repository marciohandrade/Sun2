#ifdef _CK_MOD_UI_LOGIN
#else

#ifndef     __USER_INTERFACE_LOGINNOTICE_H__
#define     __USER_INTERFACE_LOGINNOTICE_H__
//------------------------------------------------------------------------------
/**
    @class uiLoginNotice

    login notice user intreface

    (C) 2005 ckbang
*/
#include "uiLoginNotice_def.h"

class uiLoginNotice :
    public uiBase
{
    enum eDIALOG_POS
    {
        DLGPOS_BTN_GO_HOMEPAGE=0,
        DLGPOS_BTN_CLOSE,
        DLGPOS_TXT_NOTICE,
        DLGPOS_LST_NOTICE,
        DLGPOS_SCR_VERTICAL,

        DIALOG_MAX,
    };

public:
    uiLoginNotice(InterfaceManager *pUIMan);
    virtual ~uiLoginNotice();

    //--------------------------------------------------------------------------
    // init/release
    //
public:
    virtual void            Init(CDrawBase* pDrawBase);
    virtual void            Release();

    //--------------------------------------------------------------------------
    // helper/comms.
    //
public:

    //--------------------------------------------------------------------------
    // functions
    //
public:

    //--------------------------------------------------------------------------
    // dialog functions
    //
public:

protected:
    virtual void	        showWindow( BOOL val );

protected:
    CControlWZ *	        getControl( POSTYPE AtPos );
    POSTYPE			        getCtrlIDToPos( WzID wzId );
    CCtrlStaticWZ *         getCtrlText(POSTYPE pos);
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
};

//------------------------------------------------------------------------------
/**
*/
inline
POSTYPE
uiLoginNotice::getCtrlIDToPos( WzID wzId )
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
uiLoginNotice::getControl( POSTYPE AtPos )
{
    ASSERT( AtPos < DIALOG_MAX );

    return GetControlWZ( m_wzId[AtPos] );
}

//------------------------------------------------------------------------------
#endif  //  __USER_INTERFACE_LOGINNOTICE_H__

#endif