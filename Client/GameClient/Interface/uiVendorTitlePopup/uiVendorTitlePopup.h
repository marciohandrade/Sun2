#ifndef     __USER_INTERFACE_VENDORTITLEPOPUP_H__
#define     __USER_INTERFACE_VENDORTITLEPOPUP_H__
//------------------------------------------------------------------------------
/**
    @class uiVendorTitlePopup

    personal vendor title popup interface

    (C) 2005 ckbang
*/
#include "uiBase.h"
#include "uiVendorTitlePopup_def.h"

class uiVendorTitlePopup :
    public uiBase
{
    enum eDIALOG_POS
    {
        DIALOG_TXT_HELP_PLZ_INPUT=0,
        DIALOG_BTN_OK,
        DIALOG_BTN_CANCEL,
        DIALOG_EDIT_TITLE_MESSAGE,

        DIALOG_MAX,
    };

public:
    uiVendorTitlePopup(InterfaceManager *pUIMan);
    virtual ~uiVendorTitlePopup();

    enum ASK_STATUS
    {
        ASK_STATUS_NORMAL=0,
        ASK_STATUS_OK,
        ASK_STATUS_CANCEL,
    };

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
    ASK_STATUS      GetStatus();

    //--------------------------------------------------------------------------
    // functions
    //
public:

    //--------------------------------------------------------------------------
    // dialog functions
    //
public:
    void                    Exit();

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

private:
	BOOL					SendVendorTitle( void );

    //--------------------------------------------------------------------------
    // member vars
    //
private:
    bool                    m_bFocus;
    ASK_STATUS              m_status;
};

//------------------------------------------------------------------------------
/**
*/
inline
POSTYPE
uiVendorTitlePopup::getCtrlIDToPos( WzID wzId )
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
uiVendorTitlePopup::getControl( POSTYPE AtPos )
{
    assert( AtPos < DIALOG_MAX );

    return GetControlWZ( m_wzId[AtPos] );
}

//------------------------------------------------------------------------------
/**
*/
inline
uiVendorTitlePopup::ASK_STATUS
uiVendorTitlePopup::GetStatus()
{
    return m_status;
}

//------------------------------------------------------------------------------
#endif  //  __USER_INTERFACE_VENDORTITLEPOPUP_H__
