#ifndef     __USER_INTERFACE_BATTLEZONECHUNTINGSCOREBOARD_H__
#define     __USER_INTERFACE_BATTLEZONECHUNTINGSCOREBOARD_H__
//------------------------------------------------------------------------------
/**
    @class uiBattleZoneCHuntingScoreBoard

    interface for a competition hunting score board

    (C) 2006 ckbang
*/
#include "uiBattleZoneCHuntingScoreBoard_def.h"
#include "uibase.h"

class uiBattleZoneMan;
class uiBattleZoneCHuntingScoreBoard :
    public uiBase
{
    enum eDIALOG_POS
    {
        DLGPOS_LST_HOMETEAM_SCORE,
        DLGPOS_LST_AWAYTEAM_SCORE,
        DLGPOS_TXT_WIN_SCORE,
		DLGPOS_BTN_EXIT,
        DIALOG_MAX,
    };

public:
    uiBattleZoneCHuntingScoreBoard(InterfaceManager *pUIMan);
    virtual ~uiBattleZoneCHuntingScoreBoard();

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
    uiBattleZoneMan*        GetManager();

    //--------------------------------------------------------------------------
    // functions
    //
public:
    void                    Clear();
	void					ShowCompetitionPoint(int iCount, CompetitionPoint* cp, int iWin);

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
    //virtual void            UpdatePosition();

    //--------------------------------------------------------------------------
    // member vars
    //
private:
    CCtrlListWZ*            m_pLstHomeTeamScoreBoard;
    CCtrlListWZ*            m_pLstAwayTeamScoreBoard;
    CCtrlStaticWZ*          m_pTxtWinScore;

	CompetitionPoint		m_cp[CHUNTING_MAX_MEMBER_NUM*2];

    bool                    m_bInitControls;

	int						m_iMemberCnt;
};

//------------------------------------------------------------------------------
/**
*/
inline
POSTYPE
uiBattleZoneCHuntingScoreBoard::getCtrlIDToPos( WzID wzId )
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
uiBattleZoneCHuntingScoreBoard::getControl( POSTYPE AtPos )
{
    assert( AtPos < DIALOG_MAX );

    return GetControlWZ( m_wzId[AtPos] );
}

//------------------------------------------------------------------------------
/**
*/
inline
bool
uiBattleZoneCHuntingScoreBoard::CheckControls()
{
    if (!this->m_bInitControls)
    {
        assert ("Unknown controls!!");
        return false;
    }

    return true;
}

//------------------------------------------------------------------------------
#endif  //  __USER_INTERFACE_BATTLEZONECHUNTINGSCOREBOARD_H__

