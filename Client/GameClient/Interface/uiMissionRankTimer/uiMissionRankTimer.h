#pragma once
//------------------------------------------------------------------------------
//
//    작업자: 이지연
//    uiMissionRankTimer class
//------------------------------------------------------------------------------

#ifdef _NA_008016_20150202_OLD_RANKING_SYSTEM
#include "uibase.h"

#if defined(_NA_006802_20130704_REORGANIZATION_RANK_SYSTEM)
class uiMissionRankMan2;
#else
class uiMissionRankMan;
#endif // _NA_006802_20130704_REORGANIZATION_RANK_SYSTEM

class uiMissionRankTimer: public uiBase
{
    enum eDIALOG_POS
    {
        DLGPOS_MRT_TXT_BEST,
		DLGPOS_MRT_TXT_PROGRESS,
        DLGPOS_MRT_TXT_BEST_PICTURE,
        DLGPOS_MRT_TXT_PROGRESS_PICTURE,

        DIALOG_MAX,
    };

    enum
    {
        POS_TXT_SIZE = DLGPOS_MRT_TXT_PROGRESS - DLGPOS_MRT_TXT_BEST + 1,
    };

public:
    uiMissionRankTimer(InterfaceManager *pUIMan);
    virtual ~uiMissionRankTimer();

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

public:

    //--------------------------------------------------------------------------
    // dialog functions
    //
public:
	void					UpdateTime();
	void					UpdateBestTime();

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
    virtual void	        MessageProc( SI_MESSAGE * pMessage );   // Pure

    //--------------------------------------------------------------------------
    // member vars
    //
private:
#if defined(_NA_006802_20130704_REORGANIZATION_RANK_SYSTEM)
    uiMissionRankMan2*		m_pMissionRankMan;
#else
    uiMissionRankMan*		m_pMissionRankMan;
#endif // _NA_006802_20130704_REORGANIZATION_RANK_SYSTEM

	CCtrlStaticWZ*			m_pTxtControl[POS_TXT_SIZE];

    bool                    m_bControlsInitialized;
};

//------------------------------------------------------------------------------
/**
*/
inline
POSTYPE
uiMissionRankTimer::getCtrlIDToPos( WzID wzId )
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
uiMissionRankTimer::getControl( POSTYPE AtPos )
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
uiMissionRankTimer::GetManager()
{
    return this->m_pMissionRankMan;
}
#else
inline
uiMissionRankMan*
uiMissionRankTimer::GetManager()
{
    return this->m_pMissionRankMan;
}
#endif // _NA_006802_20130704_REORGANIZATION_RANK_SYSTEM

//------------------------------------------------------------------------------
#endif // _NA_008016_20150202_OLD_RANKING_SYSTEM

