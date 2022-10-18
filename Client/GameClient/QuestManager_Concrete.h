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
			- �������� ����Ʈ�� �ٷ� �� ����ϴ� ��� Ŭ����
	@description
			- QuestManager�� ���
			- QuestManager�� virtual void OnEvent( Quest * pQuest )�� ���� ���

	@scenario 
			1. DB�κ��� (������and�Ϸ��)����Ʈ�� ������ �޾Ƽ� Player���� Quest��ü�� QuestManager�� ���ؼ� �����Ѵ�(CreateQuest())
			2. �ʱ�ȭ�� QuestManager_Concrete�� �̺�Ʈ�� �߻��ÿ� ������ �̺�Ʈ(Event())�� ȣ�����ְ� ���� ų�� ��� Ŭ���̾�Ʈ�� �˷��ش�
			3. Ŭ���̾�Ʈ�� ��Ŷ�� ��û�� ���� ����üũ�� �׼��� �Ͼ��, ������ ���ش�
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

//	DB�� ���� ���� ��
// ����Ʈ ��ü�� �̸� ���� ���´�.Manager�� Quest�� ����صд�.
// �׸��� MonsterKillCondition�� add�ɶ� Establish�Ѵ�?
// 
//	VOID	SerializeStream( QUEST_PROGR_TOTAL_INFO & IN rTotalInfo, eQUEST_SERIALIZE eType );

// Ŭ���̾�Ʈ������ ȣ�����ش�. ���� ���������� �ð� üũ�� ���ϸ� �� �ʿ����.
//	void Update();

// �������� ȣ�����ش�.
//	void Event( eQUEST_EVENT evt, QUEST_MSG * pMsg );
//	void Event( QCODE QCode, eQUEST_EVENT evt, QUEST_MSG * pMsg );

// ó���Ϸ� ------------------------------------------------------------------------------------
// Ŭ���̾�Ʈ�� ���� ����üũ ��Ŷ�� �� ���� ó���� �ش�
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
	// �ݺ��� �Ǵ� �κ�
	virtual void OnEvent( eQUEST_EVENT evt, QUEST_MSG * pMsg, Quest * pQuest );

	// ���� Ŭ�������� ������ �ؾ� �ϴ� �κ�
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

