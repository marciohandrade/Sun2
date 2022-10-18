#ifndef     __uiQuestListContent_H__
#define     __uiQuestListContent_H__
//------------------------------------------------------------------------------
/**
    @class uiQuestListContent

    퀘스트 리스트의 내용

    (C) 2005 y2jinc 
*/
#include "uiBase.h"
#include "SCItemSlotContainer.h"
#include "Quest.h"
#include "QuestManager_Concrete.h"

#include "uiQuestListContent_def.h"

#include "FTextParser.h"
#include "FTextSentence.h"
#include "FTextRender.h"
#include "interfacemanager.h"
#include "globalfunc.h"
class uiQuestMan;

class uiQuestListContent :
    public uiBase
{
    enum eDIALOG_POS
    {
		eDLGPOS_CHK_MISSION,
		eDLGPOS_CHK_QUEST,		
		eDLGPOS_CHK_QUEST_MISSION,	// 임무
		eDLGPOS_CHK_QUEST_CONTENT,	// 내용
		eDLGPOS_LST_PROGRESS_QUEST,
		eDLGPOS_BTN_EXIT,
		eDLGPOS_BTN_CANCEL,
		eDLGPOS_CHK_QUEST_PREVIEW,

        DIALOG_MAX,
    };

	enum
	{
		QUEST_CATEGORY_BEGIN= eDLGPOS_CHK_MISSION,
		QUEST_CATEGORY_SIZE	= eDLGPOS_CHK_QUEST-QUEST_CATEGORY_BEGIN+1,    

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

    uiQuestListContent(InterfaceManager *pUIMan);
    virtual ~uiQuestListContent();

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
    uiQuestMan*     GetManager();

    //--------------------------------------------------------------------------
    // functions
    //
public:
	void					ClearInfo();
	// 메니저에서 공동으로 호출하는 함수.
	void					UpdateQuestInfo();

	void					RefreshFTextRender() { m_FTextRender.SetReadyForRender( FALSE ); }

protected:
	void					initInfo();
	void					updateInfo();
	void					updateMouseWheel();

	void					ListMessageProc( SI_MESSAGE * pMessage );

	void					refreshQuestList();
	void					refreshQuestContent();
	void					refreshQuestCancelBtn();
	void					refreshQuestPreviewCheck();

	void					clickChkCategory(POSTYPE pos);
	void					clickChkQuestType(POSTYPE pos);
	void					clickQuestList();
	void					clickRbtnQuestList();
	void					clickQuestPreviewCheck();
	void					clickQuestAbandon();

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


    //--------------------------------------------------------------------------
    // member vars
    //
private:
	BOOL                    m_bInitControls;
	TCHAR					m_szText[INTERFACE_STRING_LENGTH];
	TCHAR					m_szResult[INTERFACE_STRING_LENGTH];

	UI_CONTROL_LIST_PROPERTY(ProgressQuestList, StrToWzID("LS00"));
	UI_CONTROL_LIST_PROPERTY(QuestContentList, StrToWzID("LS01"));
	UI_CONTROL_VSCROLL_PROPERTY(ProgressQuestVScroll, StrToWzID("VS00"));
	UI_CONTROL_VSCROLL_PROPERTY(QuestContentVScroll, StrToWzID("VS02"));
	UI_CONTROL_STATIC_PROPERTY(QuestCount, StrToWzID("T004"));
	UI_CONTROL_BUTTON_PROPERTY(QuestCancelBtn, StrToWzID("B998"));
	UI_CONTROL_CHECK_BUTTON_PROPERTY(QuestPreviewChk, StrToWzID("CT00")); 

private:
	CCtrlButtonCheckWZ *	m_pCtrlCheckCategory[QUEST_CATEGORY_SIZE];
	CCtrlButtonCheckWZ *	m_pCtrlCheckQuestType[QUEST_CHECK_LIST_TYPE_SIZE];

	FTextParser				m_FTextParser;
	FTextSentence			m_FTextSentence;
	FTextRender				m_FTextRender;

	sScrollInfo				m_QuestListVscrInfo;
#ifdef _NA_007060_20140210_DIFFERENT_QUEST_WINDOW_OPENBUG_FIX
#else
	QUEST_GROUP_MAP			m_QuestGroup;
#endif //_NA_007060_20140210_DIFFERENT_QUEST_WINDOW_OPENBUG_FIX
};

//------------------------------------------------------------------------------
/**
*/
inline
POSTYPE
uiQuestListContent::getCtrlIDToPos( WzID wzId )
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
uiQuestListContent::getControl( POSTYPE AtPos )
{
    assert( AtPos < DIALOG_MAX );

    return GetControlWZ( m_wzId[AtPos] );
}

//------------------------------------------------------------------------------
/**
*/
inline
bool
uiQuestListContent::CheckControls()
{
    if (!this->m_bInitControls)
    {
        assert ("Unknown controls!!");
        return false;
    }

    return true;
}

//------------------------------------------------------------------------------
#endif  //  __uiQuestListContent_H__

