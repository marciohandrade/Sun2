

#ifndef     __uiQuestListMission_H__
#define     __uiQuestListMission_H__
//------------------------------------------------------------------------------
/**
    @class uiQuestListMission

    퀘스트 내용

	@todo
		- 스크롤 작업해야 함.

    (C) 2005 y2jinc 
*/


#include "uiBase.h"
#include "uiQuestListMission_def.h"
#include "ItemComposeListParser.h"
#include "SCItemSlotContainer.h"
#include "Quest.h"
#include "QuestManager_Concrete.h"
#include "FTextParser.h"
#include "FTextSentence.h"
#include "FTextParam.h"
#include "FTextRender.h"
#include "interfacemanager.h"
#include "globalfunc.h"
class uiQuestMan;


class uiQuestListMission :
    public uiBase, public SCItemSlotContainer
{
    enum eDIALOG_POS
    {
		eDLGPOS_CHK_SELECT_ITEM_1,
		eDLGPOS_CHK_SELECT_ITEM_2,
		eDLGPOS_CHK_SELECT_ITEM_3,
		eDLGPOS_CHK_SELECT_ITEM_4,
		eDLGPOS_CHK_SELECT_ITEM_5,
		eDLGPOS_CHK_FIX_ITEM,

		eDLGPOS_CHK_QUEST_MISSION,	// 임무
		eDLGPOS_CHK_QUEST_CONTENT,	// 내용
		eDLGPOS_LST_MISSION,

		eDLGPOS_BTN_EXIT,
		eDLGPOS_BTN_CANCEL,
		eDLGPOS_TITLE,
        eDLGPOS_BTN_REWARD_POINT,
        DIALOG_MAX,
    };

    enum
    {
		SLOT_BEGIN			= eDLGPOS_CHK_SELECT_ITEM_1,
		SLOT_SIZE			= eDLGPOS_CHK_FIX_ITEM-SLOT_BEGIN+1,

		SLOT_SELECT_BEGIN	= eDLGPOS_CHK_SELECT_ITEM_1,

		SLOT_SELECT_SIZE	= eDLGPOS_CHK_SELECT_ITEM_5-eDLGPOS_CHK_SELECT_ITEM_1+1,
		MAX_SLOT			= SLOT_SIZE,

		SLOT_FIX_ITEM		= eDLGPOS_CHK_FIX_ITEM,

		QUEST_CHECK_LIST_TYPE_BEGIN= eDLGPOS_CHK_QUEST_MISSION,
		QUEST_CHECK_LIST_TYPE_SIZE = eDLGPOS_CHK_QUEST_CONTENT-QUEST_CHECK_LIST_TYPE_BEGIN+1,    
	};

public:
#ifdef _NA_007060_20140210_DIFFERENT_QUEST_WINDOW_OPENBUG_FIX
#else
	typedef std::map< CODETYPE, QUEST_LIST, SortQuestListForGroupOps >	QUEST_GROUP_MAP;
	typedef QUEST_GROUP_MAP::iterator									QUEST_GROUP_ITR;
	typedef std::pair<CODETYPE, QUEST_LIST>								QUEST_GROUP_PAIR;
#endif //_NA_007060_20140210_DIFFERENT_QUEST_WINDOW_OPENBUG_FIX

    uiQuestListMission(InterfaceManager *pUIMan);
    virtual ~uiQuestListMission();

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

	void					RefreshFTextRender() { m_FTextRender.SetReadyForRender( FALSE ); }

	// Item 관련
   	ItemUnitRenderer *		GetItemUnitRender() const;

	virtual RC::eSLOT_INSERT_RESULT			InsertSlot( POSTYPE AtPos, SCSlot & rSlot );									// 슬롯에 객체 추가
	BOOL									DeleteSlot( POSTYPE AtPos, SCSlot * pSlotOut );

	// 메니저에서 공동으로 호출하는 함수.
	void					UpdateQuestInfo();

	void					SetCurQuest(Quest* pQ){m_pCurQuest = pQ;}
	
protected:
	void					initInfo();
	void					updateInfo();
	void					updateMouseWheel();

	void					ListMessageProc( SI_MESSAGE * pMessage );

	void					refreshQuestMission();
	void					refreshQuestCancelBtn();

	void					clickChkQuestType(POSTYPE pos);
	void					clickQuestAbandon();

private:
	void					insertConditionItem(QUEST_CONDITION_ITEM * pConditionItem);
	void					insertConditionMoney(QUEST_CONDITION_MONEY * pConditionMoney);
	void					insertConditionKillMon(QUEST_CONDITION_KILLMONSTER * pConditionKillMon);
	void					insertConditionMission(QUEST_CONDITION_MISSION * pConditionMission);
	void					insertConditionQuest(QUEST_CONDITION_QUEST * pConditionQuest);

	void				    insertConditionArea(QUEST_CONDITION_AREA* pConditionArea, QuestInfo * pQuestInfo);
#ifdef _NA_006607_20130402_ADD_QUESTTYPE
    void                    insertConditionMissionClear(QUEST_CONDITION_MISSION_CLEAR* pConditionMissionClear, QuestInfo * pQuestInfo);
#endif //_NA_006607_20130402_ADD_QUESTTYPE

#ifdef _NA_004157_20120409_QUEST_BANDWITH_INCREASING
#else
    void					refreshUnionQuest( Quest * pQuest );
#endif //_NA_004157_20120409_QUEST_BANDWITH_INCREASING	
    
	void					refreshQuestInfoReward();

	//--------------------------------------------------------------------------
    // dialog functions
    //
public:

protected:
    virtual void	        OnShowWindow( BOOL val );
    virtual void            OnRenderSolarDialog();
#ifdef _INTERNATIONAL_UI
	void					toolTipForTitle(int iMouseX,int iMouseY);
#endif//_INTERNATIONAL_UI
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



    //--------------------------------------------------------------------------
    // member vars
    //
private:
	BOOL                    m_bInitControls;
	ItemUnitRenderer *		m_pItemUnitRenderer;
	TCHAR					m_szText[INTERFACE_STRING_LENGTH];
	TCHAR					m_szResult[INTERFACE_STRING_LENGTH];
	Quest*					m_pCurQuest;
	BYTE					m_btDlgState; //0 == 임무 , 1 == 내용

	UI_CONTROL_LIST_PROPERTY(QuestMissionList, StrToWzID("LS01"));
	UI_CONTROL_VSCROLL_PROPERTY(QuestMissionVScroll, StrToWzID("VS02"));
	UI_CONTROL_BUTTON_PROPERTY(RewardHeim, StrToWzID("T101"));
	UI_CONTROL_BUTTON_PROPERTY(QuestCancelBtn, StrToWzID("B998"));
	UI_CONTROL_BUTTON_PROPERTY(RewardExp, StrToWzID("T102"));	
	UI_CONTROL_STATIC_PROPERTY(RewardChao, StrToWzID("T201"));

	CCtrlButtonCheckWZ *	m_pCtrlCheckQuestType[QUEST_CHECK_LIST_TYPE_SIZE];
    CCtrlButtonWZ* reward_point_button_;

#ifdef _NA_007060_20140210_DIFFERENT_QUEST_WINDOW_OPENBUG_FIX
#else
	QUEST_GROUP_MAP			m_QuestGroup;
#endif //_NA_007060_20140210_DIFFERENT_QUEST_WINDOW_OPENBUG_FIX

	sScrollInfo				m_QuestListVscrInfo;

	FTextParser				m_FTextParser;
	FTextSentence			m_FTextSentence;
	FTextParam				m_FTextParam;
	FTextRender				m_FTextRender;

	void AddFText( DWORD dwColor, WzID wzFont, LPCTSTR szText ); 

};

//------------------------------------------------------------------------------
/**
*/
inline
POSTYPE
uiQuestListMission::getCtrlIDToPos( WzID wzId )
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
uiQuestListMission::getControl( POSTYPE AtPos )
{
    assert( AtPos < DIALOG_MAX );

    return GetControlWZ( m_wzId[AtPos] );
}

//------------------------------------------------------------------------------
/**
*/
inline
bool
uiQuestListMission::CheckControls()
{
    if (!this->m_bInitControls)
    {
        assert ("Unknown controls!!");
        return false;
    }

    return true;
}

//------------------------------------------------------------------------------
#endif  //  __uiQuestListMission_H__

