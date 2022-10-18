#pragma once
#ifdef _NA_008016_20150202_OLD_RANKING_SYSTEM
//------------------------------------------------------------------------------
//
//    작업자: 이지연
//    uiMissionRankEnding class
//------------------------------------------------------------------------------

#include "uibase.h"

#if defined(_NA_006802_20130704_REORGANIZATION_RANK_SYSTEM)
class uiMissionRankMan2;
#else
class uiMissionRankMan;
#endif // _NA_006802_20130704_REORGANIZATION_RANK_SYSTEM

class uiMissionRankEnding: public uiBase
{
    enum eDIALOG_POS
    {
        DLGPOS_MRE_TXT_TIME,
        DLGPOS_MRE_TXT_RANK,
		DLGPOS_MRE_TXT_MASTER,
		DLGPOS_MRE_TXT_NUMBER,

		DLGPOS_MRE_TXT_CLEAR_TIME,
		DLGPOS_MRE_TXT_MON_DEC_TIME,
		DLGPOS_MRE_TXT_GEN_MON_KILL,
		DLGPOS_MRE_TXT_GEN_MON_TIME,
		DLGPOS_MRE_TXT_LEADER_MON_KILL,
		DLGPOS_MRE_TXT_LEADER_MON_TIME,
		DLGPOS_MRE_TXT_TOTAL_CLEAR_TIME,

		DLGPOS_MRE_BTN_CLOSE,

        DIALOG_MAX,
    };

    enum
    {
        POS_TXT_SIZE = DLGPOS_MRE_TXT_TOTAL_CLEAR_TIME - DLGPOS_MRE_TXT_TIME + 1,
    };

public:
    uiMissionRankEnding(InterfaceManager *pUIMan);
    virtual ~uiMissionRankEnding();

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
	void					_setData();

public:

    //--------------------------------------------------------------------------
    // dialog functions
    //
public:

protected:
    virtual void	        OnShowWindow( BOOL val );
    
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
uiMissionRankEnding::getCtrlIDToPos( WzID wzId )
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
uiMissionRankEnding::getControl( POSTYPE AtPos )
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
uiMissionRankEnding::GetManager()
{
    return this->m_pMissionRankMan;
}
#else
inline
uiMissionRankMan*
uiMissionRankEnding::GetManager()
{
    return this->m_pMissionRankMan;
}
#endif // _NA_006802_20130704_REORGANIZATION_RANK_SYSTEM

//------------------------------------------------------------------------------
#endif // _NA_008016_20150202_OLD_RANKING_SYSTEM

