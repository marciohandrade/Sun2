#pragma once
#ifdef _NA_008016_20150202_OLD_RANKING_SYSTEM
//------------------------------------------------------------------------------
//
//    작업자: 이지연
//    uiMissionRankGamble class
//------------------------------------------------------------------------------

#include "uibase.h"

#if defined(_NA_006802_20130704_REORGANIZATION_RANK_SYSTEM)
class uiMissionRankMan2;
#else
class uiMissionRankMan;
#endif // _NA_006802_20130704_REORGANIZATION_RANK_SYSTEM

class uiMissionRankGamble: public uiBase
{
    enum eDIALOG_POS
    {
        DLGPOS_MRG_BTN_EXIT,

		DLGPOS_MRG_BTN_OK,
		DLGPOS_MRG_BTN_CANCEL,

		// 카드: 1등 앞면
		DLGPOS_MRG_BTN_FRONT11,
		DLGPOS_MRG_BTN_FRONT12,
		DLGPOS_MRG_BTN_FRONT13,

		// 카드: 1등 뒷면
		DLGPOS_MRG_BTN_BACK11,
		DLGPOS_MRG_BTN_BACK12,
		DLGPOS_MRG_BTN_BACK13,

		// 카드: 2등 앞면
		DLGPOS_MRG_BTN_FRONT21,
		DLGPOS_MRG_BTN_FRONT22,
		DLGPOS_MRG_BTN_FRONT23,

		// 카드: 2등 뒷면
		DLGPOS_MRG_BTN_BACK21,
		DLGPOS_MRG_BTN_BACK22,
		DLGPOS_MRG_BTN_BACK23,

		// 카드: 3등 앞면
		DLGPOS_MRG_BTN_FRONT31,
		DLGPOS_MRG_BTN_FRONT32,
		DLGPOS_MRG_BTN_FRONT33,

		// 카드: 3등 뒷면
		DLGPOS_MRG_BTN_BACK31,
		DLGPOS_MRG_BTN_BACK32,
		DLGPOS_MRG_BTN_BACK33,

        DIALOG_MAX,
    };

	enum
    {
        POS_BTN_SIZE = DLGPOS_MRG_BTN_BACK33 - DLGPOS_MRG_BTN_FRONT11 + 1,
    };

	enum MRG_BTN_CARD
	{
		// 카드: 1등 앞면
		MRG_BTN_FRONT11,
		MRG_BTN_FRONT12,
		MRG_BTN_FRONT13,

		// 카드: 1등 뒷면
		MRG_BTN_BACK11,
		MRG_BTN_BACK12,
		MRG_BTN_BACK13,

		// 카드: 2등 앞면
		MRG_BTN_FRONT21,
		MRG_BTN_FRONT22,
		MRG_BTN_FRONT23,

		// 카드: 2등 뒷면
		MRG_BTN_BACK21,
		MRG_BTN_BACK22,
		MRG_BTN_BACK23,

		// 카드: 3등 앞면
		MRG_BTN_FRONT31,
		MRG_BTN_FRONT32,
		MRG_BTN_FRONT33,

		// 카드: 3등 뒷면
		MRG_BTN_BACK31,
		MRG_BTN_BACK32,
		MRG_BTN_BACK33,
	};

public:
    uiMissionRankGamble(InterfaceManager *pUIMan);
    virtual ~uiMissionRankGamble();

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
#if defined(_NA_006802_20130704_REORGANIZATION_RANK_SYSTEM)
    uiMissionRankMan2*             GetManager();
#else
    uiMissionRankMan*             GetManager();
#endif // _NA_006802_20130704_REORGANIZATION_RANK_SYSTEM

    //--------------------------------------------------------------------------
    // functions
    //
private:
	BOOL					_isEnable();
	bool					_isValidCardControl();

	void					_hideAllCard();
	void					_showCardBack(DWORD dwRank);

	void					_setGambleCard();


public:

    //--------------------------------------------------------------------------
    // dialog functions
    //
public:

protected:
    virtual void	        OnShowWindow( BOOL val );
    virtual void            OnUpdateSolarDialog();

protected:
    CControlWZ *	        getControl( POSTYPE AtPos );
    POSTYPE			        getCtrlIDToPos( WzID wzId );
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
#if defined(_NA_006802_20130704_REORGANIZATION_RANK_SYSTEM)
    uiMissionRankMan2*      m_pMissionRankMan;
#else
    uiMissionRankMan*		m_pMissionRankMan;
#endif // _NA_006802_20130704_REORGANIZATION_RANK_SYSTEM

	CCtrlButtonWZ*			m_pBtnControls[POS_BTN_SIZE];

    bool                    m_bControlsInitialized;
	bool					m_bShow;
};

//------------------------------------------------------------------------------
/**
*/
inline
POSTYPE
uiMissionRankGamble::getCtrlIDToPos( WzID wzId )
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
uiMissionRankGamble::getControl( POSTYPE AtPos )
{
    ASSERT( AtPos < DIALOG_MAX );

    return GetControlWZ( m_wzId[AtPos] );
}

//------------------------------------------------------------------------------
/**
*/
#if defined(_NA_006802_20130704_REORGANIZATION_RANK_SYSTEM)
inline
uiMissionRankMan2*
uiMissionRankGamble::GetManager()
{
    return this->m_pMissionRankMan;
}
#else
inline
uiMissionRankMan*
uiMissionRankGamble::GetManager()
{
    return this->m_pMissionRankMan;
}
#endif // _NA_006802_20130704_REORGANIZATION_RANK_SYSTEM

//------------------------------------------------------------------------------
#endif // _NA_008016_20150202_OLD_RANKING_SYSTEM

