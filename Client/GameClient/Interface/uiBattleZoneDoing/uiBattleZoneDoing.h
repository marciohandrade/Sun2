#pragma once
//------------------------------------------------------------------------------
//
//    작업자: 이지연
//    uiBattleZoneDoing class
//------------------------------------------------------------------------------

#include "uiBattleZoneDoing_def.h"
#include "uibase.h"


class uiBattleZoneMan;

class uiBattleZoneDoing: public uiBase
{
    enum eDIALOG_POS
    {
        DLGPOS_BLUE,
        DLGPOS_RED,

        DIALOG_MAX,
    };

    enum
    {
        POS_BTN_SIZE = DLGPOS_RED - DLGPOS_BLUE + 1,
    };

public:
    uiBattleZoneDoing(InterfaceManager *pUIMan);
    virtual ~uiBattleZoneDoing();

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
    uiBattleZoneMan*             GetManager();

    //--------------------------------------------------------------------------
    // functions
    //
private:
	BOOL					_isEnable();

public:
	void					SetKillPoint(int iHomeKillPoint, int iAwayKillPoint);

    //--------------------------------------------------------------------------
    // dialog functions
    //
public:

protected:
    virtual void	        OnShowWindow( BOOL val );
    virtual void            OnUpdateSolarDialog();
    virtual void            OnRenderSolarDialog();

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
    uiBattleZoneMan*		m_pBattleZoneMan;
    CCtrlButtonWZ*          m_pBtnControls[POS_BTN_SIZE];

    bool                    m_bControlsInitialized;

	int						m_iHomeKillPoint;
	int						m_iAwayKillPoint;

	RECT                    m_rcInit;
	RECT					m_WinPos;
};

//------------------------------------------------------------------------------
/**
*/
inline
POSTYPE
uiBattleZoneDoing::getCtrlIDToPos( WzID wzId )
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
uiBattleZoneDoing::getControl( POSTYPE AtPos )
{
    assert( AtPos < DIALOG_MAX );

    return GetControlWZ( m_wzId[AtPos] );
}

//------------------------------------------------------------------------------
/**
*/
inline
uiBattleZoneMan*
uiBattleZoneDoing::GetManager()
{
    return this->m_pBattleZoneMan;
}

//------------------------------------------------------------------------------

