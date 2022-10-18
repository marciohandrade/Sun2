#ifndef     __USER_INTERFACE_LOGINACCOUNT_H__
#define     __USER_INTERFACE_LOGINACCOUNT_H__
//------------------------------------------------------------------------------
/**
    @class uiLoginAccount

    input account user interface

    (C) 2005 ckbang
*/
#include "uiLoginAccount_def.h"
#include "uibase.h"

class uiLoginMan;
class uiLoginAccount :
    public uiBase
{
    enum eDIALOG_POS
    {
        DLGPOS_BTN_CONNECT=0,
        DLGPOS_EDT_ID,
        DLGPOS_EDT_PW,
        DLGPOS_LST_VERSION,

        DIALOG_MAX,
    };

    enum
    {
        POS_EDT_BEGIN = DLGPOS_EDT_ID,
        POS_EDT_END = DLGPOS_EDT_PW + 1,
        POS_EDT_SIZE = POS_EDT_END - POS_EDT_BEGIN,
    };

public:
    uiLoginAccount(InterfaceManager *pUIMan);
    virtual ~uiLoginAccount();

    //--------------------------------------------------------------------------
    // init/release
    //
public:
    virtual void            Init(CDrawBase* pDrawBase);
    virtual VOID            Process(DWORD dwTick);
    virtual void            Release();
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
    void                    ConnectBtnOn();
    void                    ConnectBtnOff();
    void                    ClearAll();
    void                    SetFocusID(bool clear=true);
    void                    SetFocusPW(bool clear=true);

    void SetEnableConnectButton();
    void SetDisableConnectButton();
    bool IsEnableConnectButton();

    //--------------------------------------------------------------------------
    // dialog functions
    //
public:

protected:
    virtual void            OnShowWindow(BOOL val);
	BOOL					IsInvalidPasswordType(TCHAR * pszPassword);
	BOOL					IsIncludeChiness(TCHAR * pszMessage);
	void					connectLogin();


protected:
    CControlWZ *	        getControl( POSTYPE AtPos );
    POSTYPE			        getCtrlIDToPos( WzID wzId );
    CCtrlStaticWZ *         getCtrlText(POSTYPE pos);
    static WzID		        m_wzId[DIALOG_MAX];

    //--------------------------------------------------------------------------
    // procedure
    //
public:
    virtual void	        MessageProc( SI_MESSAGE * pMessage );   // Pure
	void					hideCheckonShow(bool hide);
	float					GetTransTime() { return m_fTrans; }
	
    //--------------------------------------------------------------------------
    // member vars
    //
private:
    CCtrlDxIMEEditBox*      m_pEdtControls[POS_EDT_SIZE];
    CCtrlButtonWZ*          m_pBtnControl;

    float                   m_fTrans;
    DWORD                   m_dwTransAcc;
    DWORD                   m_dwTransEnd;

    bool                    m_bInitControls;

	bool					hide_check_mouse_;

	
};

//------------------------------------------------------------------------------
/**
*/
inline
POSTYPE
uiLoginAccount::getCtrlIDToPos( WzID wzId )
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
uiLoginAccount::getControl( POSTYPE AtPos )
{
    //assert( AtPos < DIALOG_MAX );

    return GetControlWZ( m_wzId[AtPos] );
}

//------------------------------------------------------------------------------
/**
*/
inline
bool
uiLoginAccount::CheckControls()
{
    if (!this->m_bInitControls)
    {
        assert ("Unknown controls!!");
        return false;
    }

    return true;
}


//------------------------------------------------------------------------------
#endif  //  __USER_INTERFACE_LOGINACCOUNT_H__
