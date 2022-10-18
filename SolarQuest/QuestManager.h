#pragma once
//=======================================================================================================================
/// QuestManager class
/**
	@author
			Kim Min Wook < taiyo@webzen.com >	
	@since
			2006. 3. 25
	@remarks
			- QuestManager에서는 진행중인 퀘스트의 리스트를 관리
			  1.퀘스트 수락 후 추가 혹은 DB로부터 Serialize(퀘스트생성), 
			  2.퀘스트 진행,완료 조건 체크, 
			  3.퀘스트 보상, 
			  4.퀘스트 포기(퀘스트 파괴)
	@history
			- 2006.3.27 : QuestManager를 상속하는 구조변경, 퀘스트 상태 이벤트를 알려주기 위해
			- 2006.3.28 : Quest Handler가 있어야 하는 이유가 부족하다. QuestManager에 합침
			- 2006.4.14(짜장면의날) : Complete Quest의 array의 0 index 사용하지 않게 변경
	@caution
			- DoComplete()와 DoReward()의 차이 주의
			- 연합퀘스트의 자식퀘스트는 보상이 없어야 하고, 부여아이템이 제공되지 않아야 한다.
			- 2006.6.29 : DB의 complete Stream의 0번째 위치의 정보는 사용하지 않는다.
							게임에서도 0번째 위치의 정보는 사용하지 않는다.

			- m_ProgressTimeQCode의 할당은 AddQuest시 이며, 해제는 RemoveQuest 와 퀘스트의 실패,포기시이다.
	@desc 
			1.연합 퀘스트 사례
			+ 부모를 기준으로
			- 수락시(ACCEPT) 처리 				: 부모(O)-> 자식(O)
			- 완료시(COMPLETE) 처리 			: 부모(O)-> 자식(X:이미완료됨)
			- 포기시(ABANDON) 삭제 처리 		: 부모(O)-> 자식(O)
			- 보상시(REWARD) 처리				: 부모(O)-> 자식(X:보상받을게없음)
			- 보상 완료시(DESTROY) 삭제 처리 	: 부모(O)-> 자식(O)

			+ 자식을 기준으로
			- 수락시(ACCEPT) 처리 				: 자식(X:자식먼저수락불가)
			- 완료시(COMPLETE) 처리 			: 자식(O)
			- 포기시(ABANDON) 삭제 처리 		: 자식(X:자식먼저포기불가)
			- 보상시(REWARD) 처리				: 자식(X:보상받을게없음)
			- 보상 완료시(DESTROY) 삭제 처리 	: 자식(X:자식먼저삭제불가)

			// 제한 조건 : 자식 퀘스트는 부여 아이템이 존재하면 안된다.
			// 이유 : 부모퀘스트 포기시 자식퀘스트의 부여 아이템 삭제가 불분명하다.
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

	void DoReward( QCODE QCode/*진행중인퀘스트코드*/, void * pvParam );
    //{__NA001282_20090206_MODIFY_QUEST_TIME 
	void DoFailed( QCODE QCode );
	void DoAbandon( QCODE QCode );
    //}
	inline Quest * FindQuest( QCODE QCode ) { return m_QuestHash.GetData( QCode ); }
	Quest * CreateQuest( QCODE QuestCode );
	// 반환할때 주의 : 유효한 포인터인지!, 재파괴하는 건 아닌지!
	void DestroyQuest( QCODE QuestCode );
	void DestroyAll();		//< 모든 진행중인 퀘스트와 완료된 퀘스트 정보 초기화

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

	// 퀘스트 완료 스트림 정보 변경
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

	// 하위 클래스에서 구현을 해야 하는 부분
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

	// 퀘스트 완료, 실패에 따른 이벤트 알림
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
	QCODE				m_ProgressTimeQCode;						// 현재 진행중 타임 퀘스트 (없으면 : 0) 
};

template<class Operator>
void
QuestManager::ForeachProgressQuest(Operator& opr)
{
    m_QuestHash.SetFirst();
    while(Quest* pQuest = m_QuestHash.GetNext())
        opr(pQuest);
}

