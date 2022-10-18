#pragma once
//------------------------------------------------------------------------------
//
//    작업자: 이지연
//    uiBattleZoneWinDialog class
//------------------------------------------------------------------------------

#include "uiBattleZoneWinDialog_def.h"
#include "uibase.h"

class uiBattleZoneMan;


typedef struct _CHUNTING_REWARD_ITEM
{
	DWORD dwCode;
	DWORD dwCount;
	IconImage *pImage;
} CHUNTING_REWARD_ITEM;

class uiBattleZoneMan;
class uiBattleZoneWinDialog: public uiBase
{
    enum eDIALOG_POS
    {
        DLGPOS_WIN,
        DLGPOS_EXP,
		DLGPOS_MONEY,

		DLGPOS_ITEM,

		DLGPOS_BTN_EXIT,

        DIALOG_MAX,
    };

    enum
    {
        POS_TXT_SIZE = DLGPOS_MONEY - DLGPOS_WIN + 1,
    };

public:
    uiBattleZoneWinDialog(InterfaceManager *pUIMan);
    virtual ~uiBattleZoneWinDialog();

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
	void					_setResult(int iWin, int iExp, int iMoney, CODETYPE dwCode, int iItemCount);
public:
	void					ShowResultWin(int iWin, int iExp, int iMoney, CODETYPE dwCode, int iItemCount);
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
	CCtrlStaticWZ*          m_pTxtControls[POS_TXT_SIZE];

    bool                    m_bControlsInitialized;

	CHUNTING_REWARD_ITEM	m_RewardItem;
};

//------------------------------------------------------------------------------
/**
*/
inline
POSTYPE
uiBattleZoneWinDialog::getCtrlIDToPos( WzID wzId )
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
uiBattleZoneWinDialog::getControl( POSTYPE AtPos )
{
    assert( AtPos < DIALOG_MAX );

    return GetControlWZ( m_wzId[AtPos] );
}

//------------------------------------------------------------------------------
/**
*/
inline
uiBattleZoneMan*
uiBattleZoneWinDialog::GetManager()
{
    return this->m_pBattleZoneMan;
}

//------------------------------------------------------------------------------

