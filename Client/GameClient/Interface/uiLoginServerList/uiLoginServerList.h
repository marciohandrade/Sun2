#ifndef     __USER_INTERFACE_LOGINSERVERLIST_H__
#define     __USER_INTERFACE_LOGINSERVERLIST_H__
//------------------------------------------------------------------------------
/**
    @class uiLoginServerList

    login server list user intreface

    (C) 2005 ckbang
*/
#include "uiLoginServerList_def.h"
#include "PacketStruct_CL.h"
#include "uibase.h"

class uiLoginMan;
class uiLoginServerList :
    public uiBase
{
    enum eDIALOG_POS
    {
#ifdef _REMOVE_SERVER_LIST_CLOSE_BUTTON_FOR_CHANNELING
		DLGPOS_TXT_TITLE = 0,
#else
		DLGPOS_BTN_CLOSE = 0,
		DLGPOS_TXT_TITLE,
#endif 
        DLGPOS_TXT_HELP,
        DLGPOS_LST_SERVERS,
        DLGPOS_SCR_VERTICAL,

        DIALOG_MAX,
    };

    enum
    {
#ifdef _REMOVE_SERVER_LIST_CLOSE_BUTTON_FOR_CHANNELING
#else
		POS_BTN_BEGIN = DLGPOS_BTN_CLOSE,
		POS_BTN_END = DLGPOS_BTN_CLOSE + 1,
		POS_BTN_SIZE = POS_BTN_END - POS_BTN_BEGIN,
#endif 


        POS_TXT_BEGIN = DLGPOS_TXT_TITLE,
        POS_TXT_END = DLGPOS_TXT_HELP + 1,
        POS_TXT_SIZE = POS_TXT_END - POS_TXT_BEGIN,
    };

public:
    uiLoginServerList(InterfaceManager *pUIMan);
    virtual ~uiLoginServerList();

    //--------------------------------------------------------------------------
    // init/release
    //
public:
    virtual void            Init(CDrawBase* pDrawBase);
    virtual void            Release();
    void                    InitControls();
    bool                    CheckControls();

    //--------------------------------------------------------------------------
    // helper/comms.
    //
public:
    uiLoginMan*             GetManager();

	INT                     GetCurrentSelectedServerIdx();
    RECT                    GetFixRC();

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
    virtual void            OnUpdateSolarDialog();
	virtual void			OnRenderSolarDialog();

	CControlWZ *	        getControl( POSTYPE AtPos );
    POSTYPE			        getCtrlIDToPos( WzID wzId );
    CCtrlStaticWZ *         getCtrlText(POSTYPE pos);

protected:
    static WzID		        m_wzId[DIALOG_MAX];

    //--------------------------------------------------------------------------
    // procedure
    //
public:
    virtual void	        MessageProc( SI_MESSAGE * pMessage );   // Pure

    //--------------------------------------------------------------------------
    // member vars
    //
private:
	RECT                    m_fixRC;

#ifdef _REMOVE_SERVER_LIST_CLOSE_BUTTON_FOR_CHANNELING
#else
	CCtrlButtonWZ*          m_pBtnControls[POS_BTN_SIZE];
#endif     
    CCtrlStaticWZ*          m_pTxtControls[POS_TXT_SIZE];
    CCtrlListWZ*            m_pLstControl;
    CCtrlVScrollWZ*         m_pVScrollControl;

    bool                    m_bInitControls;

	IconImage *				m_pNewServerIcon;
};

//------------------------------------------------------------------------------
/**
*/
inline
bool
uiLoginServerList::CheckControls()
{
    if (!this->m_bInitControls)
    {
        assert ("Unknown controls!!");
        return false;
    }

    return true;
}

//------------------------------------------------------------------------------
/**
*/
inline
POSTYPE
uiLoginServerList::getCtrlIDToPos( WzID wzId )
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
uiLoginServerList::getControl( POSTYPE AtPos )
{
    assert( AtPos < DIALOG_MAX );

    return GetControlWZ( m_wzId[AtPos] );
}

//------------------------------------------------------------------------------
/**
*/
inline
RECT
uiLoginServerList::GetFixRC()
{
    return this->m_fixRC;
}

//------------------------------------------------------------------------------
#endif  //  __USER_INTERFACE_LOGINSERVERLIST_H__
