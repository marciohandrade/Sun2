#pragma once
//------------------------------------------------------------------------------
//
//    작업자: 이지연
//    uiWayPointWindow class
//------------------------------------------------------------------------------

#include "uibase.h"

#include "uiWayPointMan/uiWayPointMan.h"


class uiWayPointWindow: public uiBase
{
    enum eDIALOG_POS
    {
        DLGPOS_WP_BTN_EXIT,
		DLGPOS_WP_BTN_OK,
		DLGPOS_WP_BTN_CANCEL,

		DLGPOS_WP_LST,
			
		DLGPOS_WP_VSCROLL,

		DLGPOS_AREA_NAME,
		DLGPOS_COST,

        DIALOG_WP_MAX,
    };

    enum
    {
        POS_WP_BTN_SIZE = DLGPOS_WP_BTN_CANCEL - DLGPOS_WP_BTN_EXIT + 1,
    };

	enum
	{
		WP_BTN_EXIT = 0,
		WP_BTN_OK,
		WP_BTN_CANCEL,
	};

public:
    uiWayPointWindow(InterfaceManager *pUIMan);
    virtual ~uiWayPointWindow();

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
    uiWayPointMan*			GetManager();
    stWayPointData*         GetWayPointData(MAPID dest_mapid, FIELDID dest_fieldid);

    //--------------------------------------------------------------------------
    // functions
    //
private:
	BOOL					_isEnable();
	void					UpdateMouseWheel();
	void					DownScroll(int nRange);
	void					UpScroll(int nRange);
    void                    MoveWayPoint();
public:

    //--------------------------------------------------------------------------
    // dialog functions
    //
public:
	void					RefreshList();

protected:
    virtual void	        OnShowWindow( BOOL val );
    virtual void            OnUpdateSolarDialog();

protected:
    CControlWZ *	        getControl( POSTYPE AtPos );
    POSTYPE			        getCtrlIDToPos( WzID wzId );
    static WzID		        m_wzId[DIALOG_WP_MAX];


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
    uiWayPointMan*					m_pWayPointMan;

	CCtrlButtonWZ*					m_pBtnControls[POS_WP_BTN_SIZE];
    CCtrlListWZ*					m_pListControl;
	CCtrlVScrollWZ*					m_pScroll;

	std::vector<stWayPointData>		m_vecListData;

    bool							m_bControlsInitialized;
	BOOL							m_bOpen;

	int								m_iSelectedIndex;
	DWORD							m_dwConfirm_MoveWayPointKey;
};

//------------------------------------------------------------------------------
/**
*/
inline
POSTYPE
uiWayPointWindow::getCtrlIDToPos( WzID wzId )
{
    for( POSTYPE i = 0 ; i < DIALOG_WP_MAX ; ++i )
    {
        if( m_wzId[i] == wzId )
        {
            return i;
        }
    }
    return DIALOG_WP_MAX;
}

//------------------------------------------------------------------------------
/**
*/
inline
CControlWZ *
uiWayPointWindow::getControl( POSTYPE AtPos )
{
    ASSERT( AtPos < DIALOG_WP_MAX );

    return GetControlWZ( m_wzId[AtPos] );
}

//------------------------------------------------------------------------------
/**
*/
inline
uiWayPointMan*
uiWayPointWindow::GetManager()
{
    return this->m_pWayPointMan;
}

//------------------------------------------------------------------------------

