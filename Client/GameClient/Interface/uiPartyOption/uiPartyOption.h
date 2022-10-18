#ifndef     __USER_INTERFACE_PARTYOPTION_H__
#define     __USER_INTERFACE_PARTYOPTION_H__
//------------------------------------------------------------------------------
/**
    @class uiPartyOption

    interface for party option.

    (C) 2005 ckbang
*/
#include "uiBase.h"
#include "uiPartyOption_def.h"

class uiPartyMan;
class uiPartyOption :
    public uiBase
{
    enum eDIALOG_POS
    {
        DLGPOS_LEAVE,
        DLGPOS_FORCEDEXPLUSION,
        DLGPOS_DESTROY,
        DLGPOS_MIGRATION,

        DIALOG_MAX,
    };

public:
    uiPartyOption(InterfaceManager *pUIMan);
    virtual ~uiPartyOption();

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
    uiPartyMan*             GetManager();

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
};

//------------------------------------------------------------------------------
/**
*/
inline
POSTYPE
uiPartyOption::getCtrlIDToPos( WzID wzId )
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
uiPartyOption::getControl( POSTYPE AtPos )
{
    assert( AtPos < DIALOG_MAX );

    return GetControlWZ( m_wzId[AtPos] );
}

//------------------------------------------------------------------------------
#endif  //  __USER_INTERFACE_PartyOption_H__
