#ifndef     __UIQUESTCOMMONFORWARD_H__
#define     __UIQUESTCOMMONFORWARD_H__
//------------------------------------------------------------------------------
/**
    @class uiQuestCommonForward

    interface for Item Composite

    (C) 2005 y2jinc 
*/
#include "uiBase.h"


#include "uiQuestCommonForward_def.h"	
#include "FTextParser.h"
#include "FTextSentence.h"
#include "FTextRender.h"
#include "globalfunc.h"
#include "interfacemanager.h"
class uiQuestMan;

#define MAX_QUEST_LIST_LINE	12

struct sFORWARD_INFO
{
	eNPC_TYPE	type;
};

class uiQuestCommonForward :
    public uiBase
{
    enum eDIALOG_POS
    {
        eDLG_BTN_JOB,
        eDLG_BTN_TAB,
#ifdef _NA_003027_20111013_HONOR_SYSTEM
        eDLG_TXT_INIT_DATE,
#endif //_NA_003027_20111013_HONOR_SYSTEM
		eDLG_BTN_EXIT,
		eDLG_LST_QUEST,
#ifdef _GS_ADD_QUEST_SCROLL_1143
		eDLG_SCR_QUEST,
#endif//_GS_ADD_QUEST_SCROLL_1143
        DIALOG_MAX,
    };

    enum
    {
        kAcceptQuest = 0, 
        kCantAcceptLevel, 
        kCantAcceptDailyQuestMaxCount, 
        kCompletedDailyQuestMaxCount, 
        kCantAcceptGuildDailyQuestMaxCount, 
        kCompletedGuildDailyQuestMaxCount, 
    };

public:
    uiQuestCommonForward(InterfaceManager *pUIMan);
    virtual ~uiQuestCommonForward();

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
    uiQuestMan*				GetManager();

    //--------------------------------------------------------------------------
    // functions
    //
public:
	void					ClearInfo();
    void					clickBtnJob(POSTYPE pos);
#ifdef _NA_008559_20151103_QUEST_AUTO_ACCEPT_PAGE_FUNC_ADD
	void                    AutoclickFirstQuest();
	bool                    AutoFirstRewardQuest();
#endif // _NA_008559_20151103_QUEST_AUTO_ACCEPT_PAGE_FUNC_ADD

protected:
	void					initInfo();
	void					initQuestInfo();
	void					registerConversation();
	void					registerMissionList();
	void					registerRewardList();
#ifdef _NA_003027_20111013_HONOR_SYSTEM
    void                    registerDailyQuestInitDate();
#endif //_NA_003027_20111013_HONOR_SYSTEM

	void					updateInfo();

	void					hideAllBtn();

	void					clickList();

	void					UpdateMouseWheel();
	void					DownScroll(int nRange=1);
	void					UpScroll(int nRange=1);
	void					registerScrollRange();

	//--------------------------------------------------------------------------
    // dialog functions
    //
public:

protected:
    virtual void	        OnShowWindow( BOOL val );
    virtual void            OnRenderSolarDialog();
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
    //virtual void            UpdatePosition();
	virtual VOID			MouseOver_MsgProc( SI_MESSAGE * pMessage );
	void					renderTooltip();
private:
	// control property
	UI_CONTROL_STATIC_PROPERTY(Title, StrToWzID("T001"));
	UI_CONTROL_STATIC_PROPERTY(Conversation, StrToWzID("T003"));
	UI_CONTROL_LIST_PROPERTY(QuestList, StrToWzID("L001"));
	
	CCtrlButtonWZ *			m_pBTN_Tab;
	CCtrlButtonWZ *			m_pBTN_Job;

    //--------------------------------------------------------------------------
    // member vars
    //													  
private:
	BOOL						m_bInitControls;
	ItemUnitRenderer *			m_pItemUnitRenderer;
	TCHAR						m_szText[INTERFACE_STRING_LENGTH];
	TCHAR						m_szResult[INTERFACE_STRING_LENGTH];

	std::vector<sFORWARD_INFO>	m_VecForward;

	FTextParser					m_FTextParser;
	FTextSentence				m_FTextSentence;
	FTextRender					m_FTextRender;
	CCtrlVScrollWZ*				m_pScroll;
	
};

//------------------------------------------------------------------------------
/**
*/
inline
POSTYPE
uiQuestCommonForward::getCtrlIDToPos( WzID wzId )
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
uiQuestCommonForward::getControl( POSTYPE AtPos )
{
    assert( AtPos < DIALOG_MAX );

    return GetControlWZ( m_wzId[AtPos] );
}

//------------------------------------------------------------------------------
/**
*/
inline
bool
uiQuestCommonForward::CheckControls()
{
    if (!this->m_bInitControls)
    {
        assert ("Unknown controls!!");
        return false;
    }

    return true;
}

//------------------------------------------------------------------------------


#endif // __UIQUESTCOMMONFORWARD_H__