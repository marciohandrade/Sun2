#pragma once

//=======================================================================================================================
/// QuestManager_Concrete class
/**
	@author
			Kim Min Wook < taiyo@webzen.com >	

	@modify
			Yu Jae Young < y2jinc@webzen.com >	
	@since
			2006. 4. 4
	@remarks
			- 로직에서 퀘스트를 다룰 때 사용하는 통로 클래스
	@description
			- QuestManager를 상속
			- QuestManager의 virtual void OnEvent( Quest * pQuest )를 구현 상속

	@scenario 
			1. DB로부터 (진행중and완료된)퀘스트의 정보를 받아서 Player마다 Quest객체를 QuestManager를 통해서 생성한다(CreateQuest())
			2. 초기화된 QuestManager_Concrete는 이벤트가 발생시에 로직의 이벤트(Event())를 호출해주고 몬스터 킬일 경우 클라이언트로 알려준다
			3. 클라이언트의 패킷의 요청에 따라 조건체크와 액션이 일어나고, 응답을 해준다
	@history
			- 
*/
//=======================================================================================================================

#include <QuestManager.h>
#include <QuestPacket.h>
#include "QuestCondition_Concrete.h"
#include "QuestAction_Concrete.h"
#include "QuestDefine.h"

class Quest;

class QuestManager_Concrete :
	public QuestManager
{
public:
	QuestManager_Concrete(QuestInfoManager * pmgr):QuestManager(pmgr){}
	virtual ~QuestManager_Concrete(){}

//	DB로 부터 왔을 때
// 퀘스트 객체를 미리 만들어서 놓는다.Manager에 Quest를 등록해둔다.
// 그리고 MonsterKillCondition이 add될때 Establish한다?
// 
//	VOID	SerializeStream( QUEST_PROGR_TOTAL_INFO & IN rTotalInfo, eQUEST_SERIALIZE eType );

// 클라이언트에서만 호출해준다. 굳이 서버에서는 시간 체크는 부하를 줄 필요없다.
//	void Update();

// 로직에서 호출해준다.
//	void Event( eQUEST_EVENT evt, QUEST_MSG * pMsg );
//	void Event( QCODE QCode, eQUEST_EVENT evt, QUEST_MSG * pMsg );

// 처리완료 ------------------------------------------------------------------------------------
// 클라이언트로 부터 조건체크 패킷이 온 순간 처리해 준다
//	BOOL CanAccept( QCODE qCode, void * pvParam );
//	BOOL CanComplete( QCODE qCode, void * pvParam );

	void Init(WORD wPoolSize);
	void RecvQuestStream(MSG_CG_QUEST_STREAM_CMD* pcmd);
#ifdef _NA_003027_20111013_HONOR_SYSTEM
    void RecvGroupQuestInfo(MSG_CG_QUEST_TODAYQUEST_INFO_CMD* group_quest_info);
    QCODE GetTodayQuestCode(WORD group_code);
#endif //_NA_003027_20111013_HONOR_SYSTEM

	inline void SetPlayer(Hero* pPlayer) { m_pPlayer = pPlayer; }
	void OnEventForClient(eQUEST_EVENT evt, QUEST_MSG* pMsg, Quest* pQuest);
	void OnEventQuestTime(Quest* pQuest);
  
protected:
	// 콜벡이 되는 부분
	virtual void OnEvent( eQUEST_EVENT evt, QUEST_MSG * pMsg, Quest * pQuest );

	// 하위 클래스에서 구현을 해야 하는 부분
	virtual QuestCondition * CreateQuestCondition( QuestConditionInfo * pInfo );
	virtual QuestAction * CreateQuestAction( QuestActionInfo * pInfo );
	virtual void DestroyQuestCondition( QuestCondition * pCondition );
	virtual void DestroyQuestAction( QuestAction * pAction );

private:
	Hero * m_pPlayer;

	// condition
	util::CMemoryPoolFactory<QUEST_CONDITION_REPEAT>		m_QUEST_CONDITION_REPEAT;
	util::CMemoryPoolFactory<QUEST_CONDITION_CHARLV>		m_QUEST_CONDITION_CHARLV;
	util::CMemoryPoolFactory<QUEST_CONDITION_CHARCLASS>		m_QUEST_CONDITION_CHARCLASS;
	util::CMemoryPoolFactory<QUEST_CONDITION_CHAO>			m_QUEST_CONDITION_CHAO;
	util::CMemoryPoolFactory<QUEST_CONDITION_QUEST>			m_QUEST_CONDITION_QUEST;
	util::CMemoryPoolFactory<QUEST_CONDITION_ITEM>			m_QUEST_CONDITION_ITEM;
	util::CMemoryPoolFactory<QUEST_CONDITION_CLASSITEM>		m_QUEST_CONDITION_CLASSITEM;
	util::CMemoryPoolFactory<QUEST_CONDITION_MONEY>			m_QUEST_CONDITION_MONEY;
	util::CMemoryPoolFactory<QUEST_CONDITION_KILLMONSTER>	m_QUEST_CONDITION_KILLMONSTER;
	util::CMemoryPoolFactory<QUEST_CONDITION_MISSION>		m_QUEST_CONDITION_MISSION;
	util::CMemoryPoolFactory<QUEST_CONDITION_AREA>			m_QUEST_CONDITION_AREA;
    util::CMemoryPoolFactory<QUEST_CONDITION_HONOR_RATING>  m_QUEST_CONDITION_HONOR_RATING;
#ifdef _NA_006607_20130402_ADD_QUESTTYPE
    util::CMemoryPoolFactory<QUEST_CONDITION_MISSION_CLEAR> m_QUEST_CONDITION_MISSION_CLEAR;
#endif // _NA_006607_20130402_ADD_QUESTTYPE
	
	// action
	util::CMemoryPoolFactory<QUEST_ACTION_ITEM>				m_QUEST_ACTION_ITEM;

#ifdef _NA_003027_20111013_HONOR_SYSTEM
    typedef STLX_MAP<WORD, QCODE>   GROUP_QUEST_MAP;
    GROUP_QUEST_MAP group_quest_map_;
#endif //_NA_003027_20111013_HONOR_SYSTEM
};

extern FILE *							g_pQuestStreamFile;
extern QuestManager_Concrete *			g_pQMGR_Temp;
extern QuestManager_Concrete *			g_pQMGR_Real;
extern QUEST_PROGR_TOTAL_INFO			g_ClientQuestProgressTotalInfo;
extern QUEST_COMPL_TOTAL_INFO			g_ClientQuestCompleteTotalInfo;

