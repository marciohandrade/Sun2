#pragma once
//=======================================================================================================================
/// QuestManager class
/**
	@author
			Kim Min Wook < taiyo@webzen.com >	
	@since
			2006. 3. 25
	@remarks
			- QuestManager������ �������� ����Ʈ�� ����Ʈ�� ����
			  1.����Ʈ ���� �� �߰� Ȥ�� DB�κ��� Serialize(����Ʈ����), 
			  2.����Ʈ ����,�Ϸ� ���� üũ, 
			  3.����Ʈ ����, 
			  4.����Ʈ ����(����Ʈ �ı�)
	@history
			- 2006.3.27 : QuestManager�� ����ϴ� ��������, ����Ʈ ���� �̺�Ʈ�� �˷��ֱ� ����
			- 2006.3.28 : Quest Handler�� �־�� �ϴ� ������ �����ϴ�. QuestManager�� ��ħ
			- 2006.4.14(¥����ǳ�) : Complete Quest�� array�� 0 index ������� �ʰ� ����
	@caution
			- DoComplete()�� DoReward()�� ���� ����
			- ��������Ʈ�� �ڽ�����Ʈ�� ������ ����� �ϰ�, �ο��������� �������� �ʾƾ� �Ѵ�.
			- 2006.6.29 : DB�� complete Stream�� 0��° ��ġ�� ������ ������� �ʴ´�.
							���ӿ����� 0��° ��ġ�� ������ ������� �ʴ´�.

			- m_ProgressTimeQCode�� �Ҵ��� AddQuest�� �̸�, ������ RemoveQuest �� ����Ʈ�� ����,������̴�.
	@desc 
			1.���� ����Ʈ ���
			+ �θ� ��������
			- ������(ACCEPT) ó�� 				: �θ�(O)-> �ڽ�(O)
			- �Ϸ��(COMPLETE) ó�� 			: �θ�(O)-> �ڽ�(X:�̹̿Ϸ��)
			- �����(ABANDON) ���� ó�� 		: �θ�(O)-> �ڽ�(O)
			- �����(REWARD) ó��				: �θ�(O)-> �ڽ�(X:��������Ծ���)
			- ���� �Ϸ��(DESTROY) ���� ó�� 	: �θ�(O)-> �ڽ�(O)

			+ �ڽ��� ��������
			- ������(ACCEPT) ó�� 				: �ڽ�(X:�ڽĸ��������Ұ�)
			- �Ϸ��(COMPLETE) ó�� 			: �ڽ�(O)
			- �����(ABANDON) ���� ó�� 		: �ڽ�(X:�ڽĸ�������Ұ�)
			- �����(REWARD) ó��				: �ڽ�(X:��������Ծ���)
			- ���� �Ϸ��(DESTROY) ���� ó�� 	: �ڽ�(X:�ڽĸ��������Ұ�)

			// ���� ���� : �ڽ� ����Ʈ�� �ο� �������� �����ϸ� �ȵȴ�.
			// ���� : �θ�����Ʈ ����� �ڽ�����Ʈ�� �ο� ������ ������ �Һи��ϴ�.
*/
//=======================================================================================================================
#include "QuestDefine.h"
#include "QuestStruct.h"
#include "Quest.h"
#include "ResultCode.h"

class QuestCondition;
class QuestAction;
class QuestConditionInfo;
class QuestActionInfo;
class QuestInfoManager;

class QuestManager
{
	friend class Quest;
public:
	QuestManager(QuestInfoManager * pmgr);
	virtual ~QuestManager(void);

	void Update();

	void Event( eQUEST_EVENT evt, QUEST_MSG * pMsg );
	BOOL Event( QCODE QCode, eQUEST_EVENT evt, QUEST_MSG * pMsg );

	RC::eQUEST_RESULT CanAccept( QCODE qCode, void * pvParam );
	RC::eQUEST_RESULT CanComplete( QCODE qCode, void * pvParam );

	RC::eQUEST_RESULT CanDoAccept( QCODE qCode, void * pvParam );
	RC::eQUEST_RESULT CanDoComplete( QCODE qCode, void * pvParam );

	BOOL GmAccept( QCODE qCode, void * pvParam );
	BOOL GmComplete( QCODE QCode, void * pvParam );
	BOOL GmReset( QCODE QCode, void * pvParam );

	void DoReward( QCODE QCode/*������������Ʈ�ڵ�*/, void * pvParam );
    //{__NA001282_20090206_MODIFY_QUEST_TIME 
	void DoFailed( QCODE QCode );
	void DoAbandon( QCODE QCode );
    //}
	inline Quest * FindQuest( QCODE QCode ) { return m_QuestHash.GetData( QCode ); }
	Quest * CreateQuest( QCODE QuestCode );
	// ��ȯ�Ҷ� ���� : ��ȿ�� ����������!, ���ı��ϴ� �� �ƴ���!
	void DestroyQuest( QCODE QuestCode );
	void DestroyAll();		//< ��� �������� ����Ʈ�� �Ϸ�� ����Ʈ ���� �ʱ�ȭ

	inline int  QuestNum() 	 { return m_QuestHash.GetDataNum(); }
	inline BOOL IsQuestMax() { return QuestNum() >= QUESTPART_PROGR::MAX_QUEST_PROGR_NUM; }
    DWORD GetChildQuestNum(QCODE qCode);

	inline QUESTPART_COMPL * FindComplQuest( QCODE QCode ) { 
#ifdef _NA_004157_20120409_QUEST_BANDWITH_INCREASING
        COMPLETED_QUEST_MAP::iterator it = m_pCompletedQuest.find(QCode);
        if(it == m_pCompletedQuest.end())
            return NULL;

        return &it->second;
#else
		if(QCode<QUESTPART_COMPL::MAX_QUEST_COMPL_NUM)
			return &m_pCompletedQuest[QCode]; 
        return NULL; 
#endif
	}

#ifdef _NA_20100307_BETAKEY_SYSTEM
    INT32 GetBetakey(QCODE quest_code);
#endif

	VOID	ClearProgrStream();
	VOID	ClearComplStream();

	VOID	SerializeProgrStream( BYTE * pProgrStream, WORD wSize, eQUEST_SERIALIZE eType );
	VOID	SerializeComplStream( BYTE * pComplStream, WORD wSize, eQUEST_SERIALIZE eType );
	VOID	SerializeStream( QUEST_PROGR_TOTAL_INFO & IN rTotalInfo, eQUEST_SERIALIZE eType );
	VOID	SerializeStream( QUEST_COMPL_TOTAL_INFO & IN rTotalInfo, eQUEST_SERIALIZE eType );

	// ����Ʈ �Ϸ� ��Ʈ�� ���� ����
	void	IncComplStream( QCODE QCode );
	void	DecComplStream( QCODE QCode );

    //{__NA001282_20090206_MODIFY_QUEST_TIME 
			BOOL IsTimeQuest(QCODE QCode);
	inline QCODE GetProgressTimeQuest() { return m_ProgressTimeQCode; }
    //}

#ifdef _NA_003027_20111013_HONOR_SYSTEM
    BOOL GetCompleteQuestCount(WORD& OUT normal_quest, WORD& OUT daily_quest);
    BOOL SetCompleteQuestCount(WORD normal_quest, WORD daily_quest);
    BOOL IncCompleteQuestCount(QCODE questcode);
    void SerializeDailyQuestStream(QUEST_DAILY_INFO* stream, WORD& count, eSERIALIZE eType);
    const QUEST_DAILY_INFO* const GetDailyQuestData(QCODE code);
    void  SetDailyQuestData(QUEST_DAILY_INFO* daily_quest_info);
    RC::eQUEST_RESULT CanDoAcceptDailyQuest(QCODE QCode, void * pvParam);
    void DoAcceptDailyQuest(QCODE questcode);
    void DoCompleteDailyQuest(QCODE questcode);
    DWORD GetDailyQuestCompleteCountToday(BYTE daily_quest_type, DWORD* OUT today_complete_count = NULL);
    void DoAbandonDailyQuest(QCODE questcode);
    BOOL IsCompletedDailyQuest(QCODE questcode, QuestInfo* quest_info);
    BOOL IsCompletedDailyQuestGroup(WORD quest_group_code);
    BOOL HaveDoneDailyQuest(int day);
#endif//_NA_003027_20111013_HONOR_SYSTEM

    //_NA_007909_20141218_SUNLOG_QUESTLIST
    VOID PrintLogProgressQuestList();

protected:
	inline void AddQuest( Quest * pQuest, QCODE QCode ) { 
		m_QuestHash.Add( pQuest, QCode );
		if( pQuest->IsTimeQuest() )
			m_ProgressTimeQCode = QCode;
	}

	inline void RemoveQuest( QCODE QCode )	{ 
		m_QuestHash.Remove( QCode ); 
		if( m_ProgressTimeQCode == QCode )
			m_ProgressTimeQCode = 0;
	}
    template<class Operator>
    void ForeachProgressQuest(Operator& opr);

	void DestroyQuest( Quest * pQuest );

	// ���� Ŭ�������� ������ �ؾ� �ϴ� �κ�
	virtual QuestCondition * CreateQuestCondition( QuestConditionInfo * pInfo )
    {
        return NULL;
    }
	virtual QuestAction * CreateQuestAction( QuestActionInfo * pInfo )
    {
        return NULL;
    }
	virtual void DestroyQuestCondition( QuestCondition * pCondition )
    {
    }
	virtual void DestroyQuestAction( QuestAction * pAction )
    {
    }

	// ����Ʈ �Ϸ�, ���п� ���� �̺�Ʈ �˸�
	virtual void OnEvent( eQUEST_EVENT evt, QUEST_MSG * pMsg, Quest * pQuest )
    {
    }

	QuestInfoManager * GetQuestInfoManager() { return m_pQuestInfoManager; }
private:
	QuestInfoManager *	m_pQuestInfoManager;
	QUEST_HASH			m_QuestHash;						

#ifdef _NA_004157_20120409_QUEST_BANDWITH_INCREASING
    typedef STLX_MAP<QCODE, QUESTPART_COMPL_EX> COMPLETED_QUEST_MAP;
    COMPLETED_QUEST_MAP m_pCompletedQuest;
#else
	QUESTPART_COMPL		m_pCompletedQuest[QUESTPART_COMPL::MAX_QUEST_COMPL_NUM];
#endif //_NA_004157_20120409_QUEST_BANDWITH_INCREASING

#ifdef _NA_003027_20111013_HONOR_SYSTEM
    struct
    {
        WORD daily_quest;
        WORD normal_quest;
    } completed_quest_count;
    
    typedef STLX_HASH_MAP<QCODE, QUEST_DAILY_INFO> DAILY_QUEST_MAP;
    DAILY_QUEST_MAP daily_quest_data_;
#endif//_NA_003027_20111013_HONOR_SYSTEM

	//static util::CMemoryPoolFactory<Quest> s_QuestPool;
	QCODE				m_ProgressTimeQCode;						// ���� ������ Ÿ�� ����Ʈ (������ : 0) 
};

template<class Operator>
void
QuestManager::ForeachProgressQuest(Operator& opr)
{
    m_QuestHash.SetFirst();
    while(Quest* pQuest = m_QuestHash.GetNext())
        opr(pQuest);
}

