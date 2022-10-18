#ifndef     __USER_INTERFACE_LOGINBACK_H__
#define     __USER_INTERFACE_LOGINBACK_H__
//------------------------------------------------------------------------------
/**
    @class uiLoginBack

    login background dialog

    (C) 2005 ckbang
*/
#include "uiLoginBack_def.h"
#include "uibase.h"
#include "interfacemanager.h"

class uiLoginBack :
    public uiBase
{
    enum eDIALOG_POS
    {
        DLGPOS_LST_VERSION=0,
#ifdef _REMOVE_REGISTER_BUTTON_FOR_CHANNELING
#else
        DLGPOS_BTN_REGISTER,
#endif

        DLGPOS_BTN_EXIT,

        DIALOG_MAX,
    };

    enum
    {
#ifdef _REMOVE_REGISTER_BUTTON_FOR_CHANNELING
		POS_BTN_BEGIN = DLGPOS_BTN_EXIT,
#else
		POS_BTN_BEGIN = DLGPOS_BTN_REGISTER,
#endif
		POS_BTN_END = DLGPOS_BTN_EXIT + 1,
        POS_BTN_SIZE = POS_BTN_END - POS_BTN_BEGIN,
    };

public:
    uiLoginBack(InterfaceManager *pUIMan);
    virtual ~uiLoginBack();

    //--------------------------------------------------------------------------
    // init/release
    //
public:
    virtual void            Init(CDrawBase* pDrawBase);
    virtual void            Release();
    virtual void            Process(DWORD dwTick);
    void                    InitControls();
    bool                    CheckControls();

    //--------------------------------------------------------------------------
    // helper/comms.
    //
public:
    uiLoginMan*             GetManager();

    //--------------------------------------------------------------------------
    // functions
    //
public:

    //--------------------------------------------------------------------------
    // dialog functions
    //
public:

protected:
    virtual void            OnShowWindow(BOOL val);
    virtual void            OnRenderSolarDialog();

protected:
    CControlWZ *	        getControl( POSTYPE AtPos );
    POSTYPE			        getCtrlIDToPos( WzID wzId );
    CCtrlStaticWZ *         getCtrlText(POSTYPE pos);
    static WzID		        m_wzId[DIALOG_MAX];

    //--------------------------------------------------------------------------
    // procedure
    //
public:
    virtual void	        MessageProc( SI_MESSAGE * pMessage );

    //--------------------------------------------------------------------------
    // member vars
    //
private:
    //CCtrlListWZ*            m_pLstControl;
    CCtrlButtonWZ*          m_pBtnControls[POS_BTN_SIZE];
    bool                    m_bInitControls;

    CTimerSimple            m_timerConnectLoginFronTimeout;
};

//------------------------------------------------------------------------------
/**
*/
inline
POSTYPE
uiLoginBack::getCtrlIDToPos( WzID wzId )
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
uiLoginBack::getControl( POSTYPE AtPos )
{
    assert( AtPos < DIALOG_MAX );

    return GetControlWZ( m_wzId[AtPos] );
}

//------------------------------------------------------------------------------
/**
*/
inline
bool
uiLoginBack::CheckControls()
{
    if (!this->m_bInitControls)
    {
        assert ("Unknown controls!!");
        return false;
    }

    return true;
}

//------------------------------------------------------------------------------
#endif  //  __USER_INTERFACE_LOGINBACK_H__
