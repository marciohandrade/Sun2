#pragma once
#if !defined(_NA_006802_20130704_REORGANIZATION_RANK_SYSTEM)
//------------------------------------------------------------------------------
//
//    작업자: 이지연
//    uiMissionRankList class
//------------------------------------------------------------------------------

#include "uibase.h"

class uiMissionRankMan;

//GS_BUG_FIX:미션순서를 정렬할 함수객체(템플릿안씀)
class MapListSort_Funter
{
public:
	inline BOOL operator()(sMAPINFO *lhs, sMAPINFO* rhs) const;
};

class uiMissionRankList: public uiBase
{
    enum eDIALOG_POS
    {
        DLGPOS_MRL_BTN_EXIT,

		DLGPOS_MRL_TXT_DATE,
		DLGPOS_MRL_TXT_AREA,
		DLGPOS_MRL_TXT_MISSION,
		DLGPOS_MRL_TXT_PAGE,

		DLGPOS_MRL_LST_FAME,
		DLGPOS_MRL_LST_WEEK,
		DLGPOS_MRL_LST_PREWEEK,
		DLGPOS_MRL_LST_MEMBER,
		DLGPOS_MRL_LST_AREA,
		DLGPOS_MRL_LST_MISSION,

		DLGPOS_MRL_VSCR_WEEK,
		DLGPOS_MRL_VSCR_PREWEEK,
		DLGPOS_MRL_VSCR_MEMBER,

		DLGPOS_MRL_BTN_AREA,
		DLGPOS_MRL_BTN_MISSION,
		DLGPOS_MRL_BTN_REWARD,
		DLGPOS_MRL_BTN_CLOSE,

		DLGPOS_MRL_BTN_PAGE_LEFT,
		DLGPOS_MRL_BTN_PAGE_RIGHT,

		DLGPOS_MRL_TXT_TITLE,

		DLGPOS_MRL_BTN_NAME1,
		DLGPOS_MRL_BTN_NAME2,
		DLGPOS_MRL_BTN_NAME3,

		DLGPOS_MRL_BTN_POINT1,
		DLGPOS_MRL_BTN_POINT2,
		DLGPOS_MRL_BTN_POINT3,

		DLGPOS_MRL_BTN_WEEK,
		DLGPOS_MRL_BTN_PREWEEK,

		DLGPOS_MRL_STATIC_RATE,
		DLGPOS_MRL_STATIC_PARTY_NUMBER,
		DLGPOS_MRL_STATIC_LEVEL,
		DLGPOS_MRL_STATIC_TIME,

		DLGPOS_MRL_STATIC_GUILD_LEVEL,
		DLGPOS_MRL_STATIC_GUILD,
		DLGPOS_MRL_STATIC_NAME,

		DLGPOS_MRL_STATIC_FLAME_LEVEL,

        DIALOG_MAX,
    };

    enum
    {
        POS_TXT_SIZE	= DLGPOS_MRL_TXT_PAGE - DLGPOS_MRL_TXT_DATE + 1,
		POS_LST_SIZE	= DLGPOS_MRL_LST_MISSION - DLGPOS_MRL_LST_FAME + 1,
		POS_VSCR_SIZE	= DLGPOS_MRL_VSCR_MEMBER - DLGPOS_MRL_VSCR_WEEK + 1,
		POS_BTN_SIZE	= DLGPOS_MRL_BTN_CLOSE - DLGPOS_MRL_BTN_AREA + 1,
		POS_LST_BTN_SIZE = DLGPOS_MRL_BTN_POINT3 - DLGPOS_MRL_BTN_NAME1 + 1,
    };

	enum eMRL_LST_BTN
	{
		MRL_LST_BTN_NAME1,
		MRL_LST_BTN_NAME2,
		MRL_LST_BTN_NAME3,
		
		MRL_LST_BTN_POINT1,
		MRL_LST_BTN_POINT2,
		MRL_LST_BTN_POINT3,
	};

	enum eMRL_TXT
	{
		MRL_TXT_DATE,
		MRL_TXT_AREA,
		MRL_TXT_MISSION,
		MRL_TXT_PAGE,
	};

	enum eMRL_LST
	{
		MRL_LST_FAME,
		MRL_LST_WEEK,
		MRL_LST_PREWEEK,
		MRL_LST_MEMBER,
		MRL_LST_AREA,
		MRL_LST_MISSION,
	};

	enum eMRL_VSCR
	{
		MRL_VSCR_WEEK,
		MRL_VSCR_PREWEEK,
		MRL_VSCR_MEMBER,
	};

	enum eMRL_BTN
	{
		MRL_BTN_AREA,
		MRL_BTN_MISSION,
		MRL_BTN_REWARD,
		MRL_BTN_CLOSE,
	};

	enum eFAME_LST
	{
		eFAME_LST_PARTY_NUM,
		eFAME_LST_LEVEL,
		eFAME_LST_TIME,

		eFAME_LST_MAX,
	};

	enum eWEEK_LST
	{
		eWEEK_LST_RANK,
		eWEEK_LST_PARTY_NUM,
		eWEEK_LST_LEVEL,
		eWEEK_LST_TIME,

		eWEEK_LST_MAX,
	};

	enum eMEMBER_LST
	{
		eMEMBER_LST_LEVEL,
		eMEMBER_LST_GUILD,
		eMEMBER_LST_NAME,

		eMEMBER_LST_MAX,
	};

public:
    uiMissionRankList(InterfaceManager *pUIMan);
    virtual ~uiMissionRankList();

    //--------------------------------------------------------------------------
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
    uiMissionRankMan*             GetManager();

    //--------------------------------------------------------------------------
    // functions
    //
private:
	BOOL					_isEnable();
	void					_prepareDisplay();
	void					_initRankListSelect();
	void					_initRankList();
	void					_initMemberList();
	void					_initPageSet();
	void					_setTitle();
	void					_UpdateMouseWheel(eMRL_LST eListType);
	void					_IncreaseScroll(eMRL_VSCR eScrollType);
	void					_DecreaseScroll(eMRL_VSCR eScrollType);

public:
	void					SetAreaList();
	void					SetMissionList();
	void					UpdateMissionText(bool bPrepare);
	void					UpdateMissionSelectList();
	void					UpdatePageText();

	void					UpdateRankList();
	void					UpdateMemberList();

	void					UpdateFinishDay();

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
    uiMissionRankMan*		m_pMissionRankMan;

	CCtrlStaticWZ*			m_pTxtControl[POS_TXT_SIZE];
    CCtrlListWZ*			m_pListControl[POS_LST_SIZE];
    CCtrlButtonWZ*			m_pBtnControls[POS_BTN_SIZE];
	CCtrlVScrollWZ*			m_pScroll[POS_VSCR_SIZE];

	CCtrlStaticWZ*			m_pTxtTitle;
	CCtrlButtonWZ*			m_pLstBtns[POS_LST_BTN_SIZE];
	int						m_iCurEventMainListIndex;
	int						m_iCurEventSubListIndex;

    bool                    m_bControlsInitialized;
	bool					m_bShow;

	bool					m_bShowMainList;
	bool					m_bShowSubList;

	int						m_iCurMainListIndex;
	int						m_iCurSubListIndex;
	MAPCODE					m_MissionNo;		// 미션맵번호

#ifdef	_K2P_NEW_RANKING_UI
	CCtrlButtonCheckWZ *	m_pWeekCheckButton;
	CCtrlButtonCheckWZ *	m_pPreWeekCheckButton;
#endif

};

//------------------------------------------------------------------------------
/**
*/
inline
POSTYPE
uiMissionRankList::getCtrlIDToPos( WzID wzId )
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
uiMissionRankList::getControl( POSTYPE AtPos )
{
    ASSERT( AtPos < DIALOG_MAX );

    return GetControlWZ( m_wzId[AtPos] );
}

//------------------------------------------------------------------------------
/**
*/
inline
uiMissionRankMan*
uiMissionRankList::GetManager()
{
    return this->m_pMissionRankMan;
}

//------------------------------------------------------------------------------
#endif // _NA_006802_20130704_REORGANIZATION_RANK_SYSTEM

