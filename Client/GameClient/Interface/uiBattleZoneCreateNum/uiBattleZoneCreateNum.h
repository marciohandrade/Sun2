#ifndef     __USER_INTERFACE_BATTLEZONECREATENUM_H__
#define     __USER_INTERFACE_BATTLEZONECREATENUM_H__
//------------------------------------------------------------------------------
/**
    @class uiBattleZoneCreateNum

    베틀존 생성시 인원수 설정

    (C) 2005 ckbang
*/
#include "uiBattleZoneCreateNum_def.h"
#include "uiBase.h"
#include "interfacemanager.h"
class uiBattleZoneCreateNum :
    public uiBase
{
    enum eDIALOG_POS
    {
        DLGPOS_BTN_LV1=0,
        DLGPOS_BTN_LV2,
        DLGPOS_BTN_LV3,
        DLGPOS_BTN_LV4,
        DLGPOS_BTN_LV5,
        DLGPOS_BTN_LV6,
        DLGPOS_BTN_LV7,
        DLGPOS_BTN_LV8,
        DLGPOS_BTN_LV9,
        DLGPOS_BTN_LV10,

        DIALOG_MAX,
    };

public:
    uiBattleZoneCreateNum(InterfaceManager *pUIMan);
    virtual ~uiBattleZoneCreateNum();

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
    virtual void            update();

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
uiBattleZoneCreateNum::getCtrlIDToPos( WzID wzId )
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
uiBattleZoneCreateNum::getControl( POSTYPE AtPos )
{
    assert( AtPos < DIALOG_MAX );

    return GetControlWZ( m_wzId[AtPos] );
}

//------------------------------------------------------------------------------
#endif  //  __USER_INTERFACE_BATTLEZONECREATENUM_H__
