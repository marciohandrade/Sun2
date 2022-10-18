

#ifndef _UI_QUEST_MAN
#define _UI_QUEST_MAN

//------------------------------------------------------------------------------
/**
    @class uiQuestMan

	@todo
		- ��� ������ �޴����� �������
		- ������ ���̾�α� â������ ���� ǥ�ó� ��� ������ �ֱ⸸ �ؾ��Ѵ�.

	@remark
		- ���� ���.
		  1. �ڵ� ���� ����Ʈ
		    1.1 ������ �⺻���� ���� �Ǿ� �ִ� ����Ʈ.
		    1.2 �������� ���� �ڵ� ����. 

		  2. NPC Code �� ���� ����Ʈ ����
          3. ����Ʈ ������ ��뿡 ���� ����
          4. ���� NPC �� ����(NPCID = 0) �ڵ� ���� ����Ʈ�� �ƴ� ����Ʈ�� ���� �����Ҷ� ���� ��û.
		    
    (C) 2006 y2jinc
*/
#include "uiBase_def.h"
#include "uiQuestMan_def.h"
#include "QuestDefine.h"


#include "QuestManager_Concrete.h"


class SolarDialog;
class uiQuestCommonForward;
class uiQuestReward;
class uiQuestListDlg;
#ifdef _NA_004644_20110506_QUEST_NAVIGATION
class uiQuestList;
#endif//_NA_004644_20110506_QUEST_NAVIGATION
class uiTimeQuest;
class uiQuestListMission;
class uiQuestTimeInfo;
class uiQuestAutoAcceptAlram;
class QUEST_CONDITION_ITEM;
class QUEST_CONDITION_KILLMONSTER;
class QUEST_CONDITION_QUEST;
class NPC;

enum eQUEST_GAIN_STATE
{
	QUEST_NONE = 0,
	QUEST_ACCEPT,		    // ����
	QUEST_PROGRESS,			// ����(���� �޾Ƽ� ������)
	QUEST_REWARD,			// �Ϸ�(���󰡴�)
	QUEST_INPERFECT_REWARD, // �̿Ϸ�(����Ұ�)	
	QUEST_AUTO_ACCEPT,		// �ڵ�����
	QUEST_ACCEPT_NONE_DISPLAY, // ���� ���� ���������� ���� ǥ������ �ʴ´�.
	QUEST_CANT_ACCEPT_LV,	// ���������� ������ �ȵǴ� ��Ȳ(�ٸ� Accept Condition�� ���� TRUE���� LV�� FALSE�� ��Ȳ)
    QUEST_CANT_ACCEPT_DAILYQUEST, // �Ϸ�� ��������Ʈ(�����ʱ�ȭ �������� ���� �� ����)

	QUEST_GAIN_MAX
};

enum eQUEST_CATEGORY
{
	QUEST_CATEGORY_MISSION = 0,
	QUEST_CATEGORY_QUEST,
    QUEST_CATEGORY_SIZE,
};

enum eQUEST_LIST_TYPE
{
	QUEST_LIST_MISSION = 0,
	QUEST_LIST_CONTENT,
};

enum eQUEST_WINDOW_TYPE
{
	QUEST_WINDOW_PLAYER = 0,
	QUEST_WINDOW_NPC,
};

enum 
{
    // �̸����� �켱���� : ����ƮŸ�Կ� ���� ����Ʈ�Ϸ�, ����óġ, �����ۼ���, ����Ž�� ����
    ORDER_CONDITION_DEFAULT   = 0,
    ORDER_CONDITION_AREA      = 1,
    ORDER_CONDITION_ITEM      = 2,
    ORDER_CONDITION_MONSTER   = 3,
    ORDER_CONDITION_QUEST     = 4,
};

struct KILL_MON_QUEST_INFO_4_NAVI
{
	DWORD dwQuestCode;
	DWORD dwMonCode[QUEST_CONDITION_KILLMONSTER_INFO::_MAX_CHECK_NUM];
	POINT ptMonPos[QUEST_CONDITION_KILLMONSTER_INFO::_MAX_CHECK_NUM];

	KILL_MON_QUEST_INFO_4_NAVI()
	{
		dwQuestCode = 0;
		for(int i = 0; i < QUEST_CONDITION_KILLMONSTER_INFO::_MAX_CHECK_NUM; ++i)
		{
			dwMonCode[i] = 0;
			ptMonPos[i].x = 0;
			ptMonPos[i].y = 0;
		}
	}
};

#include "uiBaseMan.h"

class uiQuestMan : public uiBaseMan
{
public:
	uiQuestMan(InterfaceManager * pUIMan);

    //--------------------------------------------------------------------------
    // init/release
    //
public:
    virtual void	OnInitialize();
    virtual void    OnRelease(); 

	virtual void	Process(DWORD dwTick);
    virtual void    InitPKBooleans();
	virtual void	PreRenderInterface( DWORD dwTick );
	

private:
    BOOL            _LoadUI();
    //--------------------------------------------------------------------------
    // functions
    //
public:
	// open
	SolarDialog *	OpenForwardWindow(MAPID MapID, FIELDID FieldID, CODETYPE NpcCode, DWORD dwNPCKey = 0);
	SolarDialog *	OpenRewardWindow( QuestInfo * pQuestInfo, eQUEST_GAIN_STATE eGainState );
	SolarDialog *	OpenRewardWindowForAutoAlram( Quest * pQuest );
	
	BOOL			IsOpenQuestList();
	BOOL			IsOpenQuestNPC();
	
	void			CloseQuestList();
	void			CloseQuestNPC();

	void			OpenQuestList();
	void			OpenForceQuestList();
	void			OpenForceMissionList();
    
	void			UpdateQuestAlram(BOOL bFlag );

	// update
	void			UpdateQuestInfo();
	void			UpdateMissionInfo();
	void			UpdateQuestTime();
	void			SetNaviQuest(Quest* pQ);

	void			EventItem();

	// pending info
  
    // �ڵ� ���� ��û / �Ϸ� ���� �ÿ��� ���.
    void		    SetNPCCode(CODETYPE NPCCode) { m_OpenNPCCode = NPCCode; }
	void			InitPendingNPCKey() { m_dwPendingOpenNPCKey = 0; }

	DWORD			GetPendingNPCKey()	const { return m_dwPendingOpenNPCKey; }
	CODETYPE		GetNPCCode()		const { return this->m_OpenNPCCode; }
	MAPID			GetMapID()			const { return this->m_MapID; }
	FIELDID			GetFieldID()		const { return this->m_FieldID; }

	void			SetPendingAbandonQuestCode(QCODE QuestCode)
	{ m_PendingQuestCode[ePKBOOL_QUEST_ABANDON] = QuestCode; }
	QCODE			GetPendingAbandonQuestCode() const 
	{ return m_PendingQuestCode[ePKBOOL_QUEST_ABANDON]; }

	// window pos
	void            SetPlayerWindowPos(const POINT& pt);
	const POINT&    GetPlayerWindowPos() const ;
	void            SetNPCWindowPos(const POINT& pt);
	const POINT&    GetNPCWindowPos() const;

	BOOL			IsPlayerWindowPosActivated() const;
	BOOL			IsNPCWindowPosActivated() const;	

	void            DeactivatedPlayerWindowPos();
	void            DeactivatedNPCWindowPos();

	eQUEST_GAIN_STATE GetQuestGainState( QCODE QuestCode, CODETYPE NPCCode, NPC * pNPC = NULL );

    void			RefreshNPCState();

	// Quest List
	const QUEST_LIST &	GetCompleteList() const			 { return m_CompleteQuestList; }
	const QUEST_LIST &	GetInperfectCompleteList() const { return m_InperfectCompleteQuestList; }
	const QUEST_LIST &	GetProgressList() const			 { return m_ProgressQuestList; }
	const QUEST_LIST &	GetCanAcceptList() const		 { return m_CanAcceptQuestList; }
#ifdef _NA_003027_20111013_HONOR_SYSTEM
    const QUEST_LIST&   GetCompleteDailyQuestList() const { return m_CompleteDailyQuestList; }
#endif //_NA_003027_20111013_HONOR_SYSTEM

	//--------------------------------------------------------------------------
	// net
	void			NET_SEND_CG_QUEST_ACCEPT_SYN(QCODE QuestCode, BOOL IsItemAccept = 0, POSTYPE Atpos = 0);
	void			NET_SEND_CG_QUEST_COMPLETE_SYN(QCODE QuestCode);

	void			NET_SEND_CG_QUEST_REWARD_SYN(QCODE QuestCode, BYTE bSelectedItem );
	void			NET_SEND_CG_QUEST_FAILED_SYN(QCODE QuestCode);
	void			NET_SEND_CG_QUEST_ABANDON_SYN(QCODE QuestCode,BOOL bForce=FALSE);
	void			NET_SEND_CG_QUEST_EVENT_AREA_SYN(QCODE QuestCode, DWORD AreaID);

	void			SEND_CG_COLLECT_ALLCOLLECTIVE_SYN();

#ifdef _NA_003027_20111013_HONOR_SYSTEM
    void            RECV_QUEST_DAILYQUEST_PACKET(MSG_BASE* recv_packet);
    void            RECV_QUEST_DAILYQUEST_INFO_PACKET(MSG_CG_QUEST_DAILYQUEST_PACKET* recv_packet);
#endif //_NA_003027_20111013_HONOR_SYSTEM    

protected:
	BOOL			getQuestListsMeetNPC();	
	void			clearQuestList();

public:
	BOOL			AddQuestForProgress( QCODE QuestCode );	
	void			DelQuestForProgress( QCODE QuestCode );

	void			ClearQuestInfos();
	void			SetQuestViewProgressRect();

	void			clearViewQuestForProgressContents();
	void			clearViewQuestForProgress();
    void			updateViewQuestForProgress();

protected:
	sQuestPreview * allocQuestPreview();
	void			freeQuestPreview( sQuestPreview * pInfo );

	BOOL			isExistQuestForProgress( QCODE QuestCode );
	void			renderViewQuestForProgress( DWORD dwTick );

	void			insertQuestNameForProgContents( Quest * pQuest);

    void			QuestCompletCheck();
	void			RefreshConditionItemForProgContents(QCODE quest_code);

	void			insertConditionItemForProgContents(QUEST_CONDITION_ITEM * pConditionItem);
	void			insertConditionMonForProgContents(QUEST_CONDITION_KILLMONSTER * pConditionKillMon);
	void			insertConditionQuestForProgContents(QUEST_CONDITION_QUEST * pConditionQuest);

	void			insertConditionAreaQuestForProgContents(QUEST_CONDITION_AREA* pConditionArea, QuestInfo * pQuestInfo);
#ifdef _NA_006607_20130402_ADD_QUESTTYPE
    void            insertConditionMissionClearForProgContents(QUEST_CONDITION_MISSION_CLEAR* pConditionMission, QuestInfo * pQuestInfo);
#endif // _NA_006607_20130402_ADD_QUESTTYPE

public:
    eQUEST_GAIN_STATE GetStateInQuestListForForward(QCODE QuestCode);

#ifdef _NA_000000_20120416_COLLECT_OBTAIN_QUESTCODE
    bool			ConditionItemCompleteCheck(QCODE quest_code, SLOTCODE item_code);     // Ư�� �������� ������ ���� ������ �Ϸ��Ͽ�����
#endif //_NA_000000_20120416_COLLECT_OBTAIN_QUESTCODE

public:
	// Auto Aceept Quest
	void			addAcceptAutoQuest(Quest * pQuest);
	void			ClearAcceptAuto();
	Quest *			GetAcceptAutoQuest();
	WORD			GetAcceptAutoQuestCount();

	void			RefreshFTextRender();

    //--------------------------------------------------------------------------
    // procedure
    //
public:
    virtual void	NetworkProc( MSG_BASE * pMsg );
    
    //--------------------------------------------------------------------------
    // member vars
    //

	void			ShowTimeQuest(BOOL bFlag);
	void			updateTimeQuest();
	void			renderQuestTime();
	void			QuestMessage(int iStringCode);


	void			ClearPendingQuestCompletePacket();
	bool			SetPendingQuestCompletePacket(QCODE qcode);
	bool			IsPendingQuestCompletePacket(QCODE qcode);
	void			DelPendingQuestCompletePacket(QCODE qcode);

    void            RemoveQuestItem(QCODE QuestCode);
    void            AddQuestItem(QUEST_CONDITION_ITEM* pConditionItem);
    bool            FindQuestItem(CODETYPE ItemCode);
    void            DeleteQuestItem(QUEST_CONDITION_ITEM* pConditionItem);
    void            UpdateQuestItem(QCODE QuestCode);

    std::vector<sQuestPreview*> get_quest_prog_content_map(){ return m_QuestViewProgContentMap; }
private:
	// UI 
	uiQuestCommonForward *	m_pUIQuestCommonForward;
	uiQuestReward *			m_pUIQuestReward;

	uiQuestListDlg *		m_pUIQuestListDlg;
#ifdef _NA_004644_20110506_QUEST_NAVIGATION
    uiQuestList* ui_quest_list_ptr_;
#endif//_NA_004644_20110506_QUEST_NAVIGATION
	uiQuestListMission*	    m_puiQuestListMission;
	uiQuestTimeInfo*		m_pUITimeInfo;
	uiQuestAutoAcceptAlram* m_pUIAutoAcceptAlram;
	uiTimeQuest*			m_pUITimeQuest;

	std::vector<KILL_MON_QUEST_INFO_4_NAVI*> m_vecKillMonQuest4Navi;
	void SetKillMonQuest4Navi(DWORD QCode, DWORD MonCode, POINT MonPos);
    void RemoveKillMonQuest4Navi(DWORD quest_code);
public:
	void ClearKillMonQuest4Navi();
	int GetKillMonQuest4NaviCnt(){return m_vecKillMonQuest4Navi.size();}
	KILL_MON_QUEST_INFO_4_NAVI* GetKillMonQuest4Navi(int idx)
	{
		if(static_cast<size_t>(idx) >= m_vecKillMonQuest4Navi.size())
		{
			assert(!"idx >= m_vecKillMonQuest4Navi.size()");
			return NULL;
		}

		return m_vecKillMonQuest4Navi[idx];
	}
private:

	// pending variable
	// npc quest window
	DWORD					m_dwPendingOpenNPCKey;
	CODETYPE				m_OpenNPCCode;
	MAPID					m_MapID;
	FIELDID					m_FieldID;
	Quest *					m_pPendingQuest;

	__PROPERTY(BOOL,        CheckTime);


	TYPEDEF_MAP(QUEST_VIEW_PROGRESS, QCODE, Quest *);
	enum { MAX_QUEST_PREVIEW_COUNT = 5 };

	__PROPERTY(BOOL,				QuestViewProgress);
	QUEST_VIEW_PROGRESS_MAP			m_QuestViewProgressMap;
	std::vector<sQuestPreview *>	m_QuestViewProgContentMap;
	POINT							m_PtQuestViewProgress;		// ǥ���� ��ġ.(ǥ���� �κ��� ���� ��ġ)
	// ǥ���� ��ġ������ ������ Size(�������� �����Ѵ�.)
	// ( m_PtQuestViewProgress.x - m_QuestViewProgressSize.cx)
	// ( m_PtQuestViewProgress.y + m_QuestViewProgressSize.cy)
	SIZE							m_QuestViewProgressSize;		

	util::CMemoryPoolFactory<sQuestPreview> *	m_pQuestPreviewPool;

    	
	// player quest window
	__PROPERTY(eQUEST_CATEGORY,  QuestCategory);
	__PROPERTY(eQUEST_LIST_TYPE, QuestListType);
	__PROPERTY(int,				 CurSelectQuestIndex);
	__PROPERTY(int,				 CurSelectMissionIndex);
	__PROPERTY(int,				 CurSelectMissionSubIndex);
	__PROPERTY(QCODE,			 CurSelectQuestCode);
	__PROPERTY(BOOL,			 ForwardAutoAlram);
    __PROPERTY(int,			     CurBeginDrawProgressList);
    __PROPERTY(int,			     CurPosProgressListVscr);

	// common
	QCODE					m_PendingQuestCode[ePKBOOL_QUEST_MAX];    
	CODETYPE				m_PendingArea; 

	// auto accept 
	std::list<QCODE>		m_PendingAutoQuestList;

	uiQuestWindowPosition	m_CurPlayerWindowPoint;
	uiQuestWindowPosition	m_CurNPCWindowPoint;

	QUEST_LIST				m_CompleteQuestList;
	QUEST_LIST				m_InperfectCompleteQuestList;
	QUEST_LIST				m_ProgressQuestList;		
	QUEST_LIST				m_CanAcceptQuestList;
#ifdef _NA_003027_20111013_HONOR_SYSTEM
    QUEST_LIST              m_CompleteDailyQuestList;
#endif //_NA_003027_20111013_HONOR_SYSTEM

    std::multiset<CODETYPE> m_Activated_Quest_Item;

	TCHAR					m_szMessage[INTERFACE_STRING_LENGTH];
	TCHAR					m_szResult[INTERFACE_STRING_LENGTH];
	TCHAR					m_szText[INTERFACE_STRING_LENGTH];	

	std::vector<QCODE> m_vecSendCompletPacket;

#ifdef _SCJ_120104_SORT_QUEST_VIEW_PROGRESS
    typedef STLX_VECTOR<QuestCondition*> QUEST_CONDITION_VECTOR;
    QUEST_CONDITION_VECTOR quest_condition_vector;
#endif //_SCJ_120104_SORT_QUEST_VIEW_PROGRESS

#ifdef _NA_008302_20150601_QUEST_COMPLET_SYN_RECAST
private:
    void SetRefreshQuestCheckTimer() { refresh_quest_check_timer_->SetTimer(1000); }
    util::Timer* refresh_quest_check_timer_;
    bool refresh_quest_check_pending_; //�Ϸ� ���û �ߺ� ����(���ѷ��� ���)
#endif //_NA_008302_20150601_QUEST_COMPLET_SYN_RECAST
};

extern void RefreshNPCStateForQuest();
extern BOOL UpdateAutoAcceptRequest();
extern BOOL UpdateAutoRewardRequest();



#endif //_UI_QUEST_MAN

