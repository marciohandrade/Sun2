#ifndef     __USER_INTERFACE_SYSTEM_VERIFY_H__
#define     __USER_INTERFACE_SYSTEM_VERIFY_H__
//------------------------------------------------------------------------------
/**
    @class uiSystemVerify

    system verify

    (C) 2005 ckbang
*/
#include "uiSystemVerify_def.h"
#include "uibase.h"
#include "interfacemanager.h"
#include "uiSystemMan/uiSystemMan.h"
#include "GlobalFunc.h"

enum CLOSE_TYPE
{
	CT_GOTOLOGIN	= 0,
	CT_GOTOWINDOW,	
};

class uiSystemVerify :
    public uiBase
{
    enum eDIALOG_POS
    {
        DLGPOS_BTN_VERIFY=0,
        DLGPOS_LST_VERIFY_MSG,

        DIALOG_MAX,
    };

public:
    uiSystemVerify(InterfaceManager *pUIMan);
    virtual ~uiSystemVerify();

    //--------------------------------------------------------------------------
    // init/release
    //
public:
    virtual void            Init(CDrawBase* pDrawBase);
    virtual void            Release();
    virtual void            Process(DWORD dwTick);
    void                    Clear();
	void                    ClearToken();
    void                    Exit();

    //--------------------------------------------------------------------------
    // helper/comms.
    //
public:
	fn_SysVerify			m_fnSysVerify;
    void                    SetVerifyMessage(const TCHAR * pszMessage);
    void                    SetLockingEffect();
    bool                    GetLockingEffect();
    void                    SetShutdownAfterVerify();

    //--------------------------------------------------------------------------
    // functions
    //
public:
    void                    RenderRect(RECT rc, WzColor color);

	void					SetTitle(LPCTSTR pszTitle);
	void					SetSubject(LPCTSTR pszSubject);
	void					SetBtnStringID(DWORD dwOkID);
	
	void					SetCloseType( int type ) { m_CloseType = type; }
	void					ReSetShutDownTime( DWORD dwTime );


    //--------------------------------------------------------------------------
    // dialog functions
    //
public:

protected:
    virtual void	        OnShowWindow( BOOL val );
    virtual void            OnUpdateSolarDialog();
    virtual void            OnRenderSolarDialog();
	void					CloseWindow();

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
    uiSystemMan*            m_pMyMan;
    bool                    m_bShutdownAfterVerify;
    bool                    m_bLockingEffect;
    WzSimpleMesh            *m_pMessageMesh;

    std::vector<util::_tstring> m_vstr;

#ifdef _INTERNATIONAL_UI
	std::vector<util::_tstring> m_vSubstr;
#endif//_INTERNATIONAL_UI

	util::_tstring			m_strTitle;
	util::_tstring			m_strSubject;
	DWORD					m_dwOKStrID;
	CCtrlStaticWZ *			m_pCtrlOKStatic;

    CTimerSimple            m_timerWarmup;
	CTimerSimple            m_timerExit;
    bool                    m_bReadyWarmup;
	int						m_CloseType;
};

//------------------------------------------------------------------------------
/**
*/
inline
void
uiSystemVerify::SetVerifyMessage(const TCHAR * pszMessage)
{
	SetBtnStringID( this->m_dwOKStrID );

	RECT rcCtrl;
	CControlWZ * pLstControl = getControl( DLGPOS_LST_VERIFY_MSG );
	if ( pLstControl )
	{
		rcCtrl = pLstControl->GetSizeRect();

		int iCtrlWidth = rcCtrl.right - rcCtrl.left - 14;

		GlobalFunc::ProcessWordWrapByFontSize( &this->m_vstr, const_cast<TCHAR *>(pszMessage), c_wiSystemMsgFont, _tcslen(pszMessage), iCtrlWidth );
	}
}

//------------------------------------------------------------------------------
/**
*/
inline
POSTYPE
uiSystemVerify::getCtrlIDToPos( WzID wzId )
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
uiSystemVerify::getControl( POSTYPE AtPos )
{
    assert( AtPos < DIALOG_MAX );

    return GetControlWZ( m_wzId[AtPos] );
}

//------------------------------------------------------------------------------
/**
*/
inline
void
uiSystemVerify::SetLockingEffect()
{
    this->m_bLockingEffect = true;
}

//------------------------------------------------------------------------------
/**
*/
inline
bool
uiSystemVerify::GetLockingEffect()
{
    return this->m_bLockingEffect;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
uiSystemVerify::SetShutdownAfterVerify()
{
    this->m_bShutdownAfterVerify = true;
}


//------------------------------------------------------------------------------
#endif  //  __USER_INTERFACE_SYSTEM_VERIFY_H__
