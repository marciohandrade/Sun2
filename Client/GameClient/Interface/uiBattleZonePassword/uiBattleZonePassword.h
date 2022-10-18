#ifndef     __USER_INTERFACE_BATTLEZONEPASSWORD_H__
#define     __USER_INTERFACE_BATTLEZONEPASSWORD_H__
//------------------------------------------------------------------------------
/**
    @class uiBattleZonePassword

    Battle Zone Password

    (C) 2005 ckbang
*/
#include "uiBattleZoneList/uiBattleZoneList_def.h"
#include "uiBattleZonePassword_def.h"
#include "uibase.h"

class uiBattleZoneMan;
class uiBattleZonePassword :
    public uiBase
{
    enum eDIALOG_POS
    {
        DLGPOS_EDT_PASSWORD=0,
        DLGPOS_BTN_OK,
        DLGPOS_BTN_CANCEL,

        DIALOG_MAX,
    };

public:
    uiBattleZonePassword(InterfaceManager *pUIMan);
    virtual ~uiBattleZonePassword();

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
	uiBattleZoneMan*        GetManager();

    //--------------------------------------------------------------------------
    // functions
    //
public:
    

    //--------------------------------------------------------------------------
    // dialog functions
    //
public:
	virtual void            OnUpdateSolarDialog();

protected:
    virtual void	        OnShowWindow( BOOL val );

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
    util::_tstring             m_strPassword;
};

//------------------------------------------------------------------------------
/**
*/
inline
POSTYPE
uiBattleZonePassword::getCtrlIDToPos( WzID wzId )
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
uiBattleZonePassword::getControl( POSTYPE AtPos )
{
    assert( AtPos < DIALOG_MAX );

    return GetControlWZ( m_wzId[AtPos] );
}

//------------------------------------------------------------------------------
#endif  //  __USER_INTERFACE_BATTLEZONEPASSWORD_H__
