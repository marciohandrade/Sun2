#pragma once
//=======================================================================================================================
/// Quest class
/**
	@author
			Kim Min Wook < taiyo@webzen.com >	
	@since
			2006. 3. 25
	@remarks
			- 공유된 QuestInfo를 사용하는 각 GameObject 마다 가진 개별 객체
			- QuestManager에서 Quest관리

			- m_ExpiredTimer(타임퀘스트값) 는 Build시 할당되며, 보상(DoReward), 포기(DoAbandon), 실패(Update) 시 해제된다.
	@history
			- 2006.4.22 : void DoAbandon( void * pvParam ); 추가
*/
//=======================================================================================================================

#include "QuestInfo.h"
#include "QuestDefine.h"
#include "QuestStruct.h"
#include "timer.h"

#include <ResultCode.h>


class QuestCondition;
class QuestAction;
class QuestInfoManager;
class QuestManager;

typedef util::SolarHashTable<QuestCondition*>				QUEST_COND_HASH;
typedef util::SolarHashTable<QuestAction*>					QUEST_ACT_HASH;
typedef util::SolarHashTable<QuestCondition*>::iterator		QUEST_COND_HASH_IT;
typedef util::SolarHashTable<QuestAction*>::iterator		QUEST_ACT_HASH_IT;

#ifdef _NA_004157_20120409_QUEST_BANDWITH_INCREASING
typedef WORD QUESTCOUNT;
#else
typedef BYTE QUESTCOUNT;
#endif

enum eQUEST_STATE
{
	QS_NORMAL,
	QS_COMPLETED,
	QS_FAILED,
#ifdef _NA_007085_BUG_GUILD_DAILYQUEST_ACCEPT_CONDITION
    QS_NONE,   // 퀘스트 ACCEPT되지 않은 상태, 포기된 상태
    QS_MAX,
#endif //_NA_007085_BUG_GUILD_DAILYQUEST_ACCEPT_CONDITION
};

enum eDAILYQUEST_TYPE
{
    DAILYQUEST_NORMAL = 1,
    DAILYQUEST_GUILD,
#ifdef _NA_007407_20140710_CHALLENGE_DAILY_QUEST
    DAILYQUEST_CHALLENGE,
#endif //_NA_007407_20140710_CHALLENGE_DAILY_QUEST
};

class Quest
{
	__DECL_CUSTOMPOOL_PTR( Quest )
public:
	Quest(void);
	virtual ~Quest(void);

	void Init( QuestInfoManager * pInfoMgr, QuestManager * pMgr, QUESTPART_COMPL * pQCompl );

	// Quest가 유효한 상태에서 사용할 수 있음
	void Serialize( QUESTPART_PROGR & QProgrStream, eQUEST_SERIALIZE eType );
	void Build( const QUESTPART_PROGR * pStream );
	void Destroy();

	BOOL Update();
	BOOL Event( eQUEST_EVENT evt, QUEST_MSG * pMsg );

	RC::eQUEST_RESULT CanAccept( void * pvParam );
	RC::eQUEST_RESULT CanComplete( void * pvParam );

	// 자동으로 해주나? 외부에서 해주나? 고려!!!!
	void DoAccept( void * pvParam );
	void DoComplete( void * pvParam );
	void DoReward( void * pvParam );

	void DoAbandon( void * pvParam );

	
	inline QUEST_COND_HASH & GetAcceptContionHash()
	{
		return m_AcceptConditionHash;
	}
	inline QUEST_ACT_HASH  & GetAcceptActionHash()
	{
		return m_AcceptActionHash;
	}
	inline QUEST_COND_HASH & GetCompleteConditionHash()
	{
		return m_CompleteConditionHash;
	}

	inline QUEST_ACT_HASH  & GetCompleteActionHash()
	{
		return m_CompleteActionHash;
	}


#pragma warning ( push )
#pragma warning ( disable : 4311)
	inline void AddAcceptCondition( QuestCondition * pCondition ) { m_AcceptConditionHash.Add( pCondition, (DWORD)pCondition ); }
	inline void AddAcceptAction( QuestAction * pAction ) { m_AcceptActionHash.Add( pAction, (DWORD)pAction ); }
	inline void AddCompleteCondition( QuestCondition * pCondition ) { m_CompleteConditionHash.Add( pCondition, (DWORD)pCondition ); }
	inline void AddCompleteAction( QuestAction * pAction ) { m_CompleteActionHash.Add( pAction, (DWORD)pAction ); }
	inline void AddAbandonAction( QuestAction * pAction ) { m_AbandonActionHash.Add( pAction, (DWORD)pAction ); }
#pragma warning ( pop )
	void StartTimer( const QUESTPART_PROGR * pStream );
	void EndTimer();

	DWORD GetRemainedTime() { return m_ExpiredTimer.GetRemainedTime();	}
	util::Timer & GetTimer() { return m_ExpiredTimer; }

	inline QCODE GetQuestCode() { return m_pQuestInfo->GetQuestCode(); }
	inline DWORD GetRewardCode( const eCHAR_TYPE charType ) { return m_pQuestInfo->GetRewardCode( charType ); }
	inline QUESTCOUNT GetRepeatNum() { if(m_pRepeatedNum) return *m_pRepeatedNum; return 0; }
	
#ifdef _YJ_RENEWAL_QUEST_UI_001265
	inline BYTE	GetQType() { return m_pQuestInfo->GetQtype(); }
#endif //_YJ_RENEWAL_QUEST_UI_001265

	inline DWORD GetRewardMapCode() {return m_pQuestInfo->GetRewardMapCode();}
	inline DWORD GetKillMonsterMapCode(int idx) {
        // (CHANGES) (100220) (WAVERIX) #9499 fix up invalid copy operation \
        // which is not matched source and dest. types
        assert(idx < MAX_QUEST_KILL_MONSTER_MAP_CODE);
        return m_pQuestInfo->GetKillMonsterMapCode(static_cast<WORD>(idx));
    }
	inline bool IsTimeQuest() { return !!m_pQuestInfo->GetExpiredTime(); }
private:

	QUEST_COND_HASH m_AcceptConditionHash;
	QUEST_ACT_HASH	m_AcceptActionHash;
	QUEST_COND_HASH m_CompleteConditionHash;
	QUEST_ACT_HASH	m_CompleteActionHash;	//< 현재 count == 0
	util::Timer		m_ExpiredTimer;
	QUESTCOUNT*     m_pRepeatedNum;			//< 반복된 횟수
	QUEST_ACT_HASH	m_AbandonActionHash;

	__PTR_PROPERTY( QuestManager, QuestManager )
	__PTR_PROPERTY( QuestInfoManager, QuestInfoManager )
	__PTR_PROPERTY( QuestInfo, QuestInfo )
	__PROPERTY( eQUEST_STATE, QState )

};
