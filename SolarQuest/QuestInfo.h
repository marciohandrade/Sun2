#pragma once
//=======================================================================================================================
/// QuestInfo
/**
	@author
			Kim Min Wook < taiyo@webzen.com >	
	@since
			2006. 3. 22~
	@history
			- 2006.5.19 : 항목 퀘스트 액션 타입 QuestAcceptType 추가( 0:일반, 1:레벨업자동수락타입 )
*/
//=======================================================================================================================

#include "QuestConditionInfo.h"
#include "QuestActionInfo.h"

#include <list>
typedef STLX_LIST<QuestConditionInfo*>				QUEST_COND_INFO_LIST;
typedef STLX_LIST<QuestActionInfo*>					QUEST_ACT_INFO_LIST;
typedef STLX_LIST<QuestConditionInfo*>::iterator	QUEST_COND_INFO_LIST_IT;
typedef STLX_LIST<QuestActionInfo*>::iterator		QUEST_ACT_INFO_LIST_IT;
//typedef util::SolarHashTable<QuestConditionInfo*>	QUEST_COND_HASH;
//typedef util::SolarHashTable<QuestActionInfo*>		QUEST_ACT_HASH;

class QuestInfo
{
public:
	QuestInfo(void);
	virtual ~QuestInfo(void);

	inline void AddAcceptCondition( QuestConditionInfo * pCondition )	{ m_AcceptConditionList.push_back(pCondition); }
	inline void AddAcceptAction( QuestActionInfo * pAction )			{ m_AcceptActionList.push_back(pAction); }
	inline void AddCompleteCondition( QuestConditionInfo * pCondition ) { m_CompleteConditionList.push_back(pCondition); }
	inline void AddCompleteAction( QuestActionInfo * pAction )			{ m_CompleteActionList.push_back(pAction); }
	inline void AddAbandonAction( QuestActionInfo * pAction )			{ m_AbandonActionList.push_back(pAction); }

	QuestConditionInfo * FindAcceptCondition( eQUEST_CONDITION_TYPE ConType );
	QuestActionInfo * FindAcceptAction( eQUEST_ACTION_TYPE ActType );
	QuestConditionInfo * FindCompleteCondition( eQUEST_CONDITION_TYPE ConType );
	QuestActionInfo * FindCompleteAction( eQUEST_ACTION_TYPE ActType );
	// loader에서 등록해서 release에서 메모리 해제
	void Release();

#if !defined(_NA_004157_20120409_QUEST_BANDWITH_INCREASING)
	inline void FindChildQuestInfo( QCODE QuestCode )		{ m_ChildQuestInfoHash.GetData( QuestCode ); }
	inline void AddChildQuestInfo( QuestInfo * pChild )		{ m_ChildQuestInfoHash.Add( pChild, pChild->GetQuestCode() ); }
	inline void RemoveChildQuestInfo( QCODE QuestCode )		{ m_ChildQuestInfoHash.Remove( QuestCode ); }
	inline QUEST_INFO_HASH & GetChildQuestInfoHash()		{ return m_ChildQuestInfoHash; }
	inline BOOL IsUnionQuest() 								{ return (BOOL)m_ChildQuestInfoHash.size(); }
#endif
	inline BOOL IsChildQuest() 								{ return (BOOL)GetParentQuestCode(); }
#ifdef _NA_003027_20111013_HONOR_SYSTEM
    inline BOOL IsDailyQuest()                              { return (GetDailyQuest() != 0); }
#endif //_NA_003027_20111013_HONOR_SYSTEM

private:
	friend class QuestManager;
	friend class QuestScriptLoader;
	friend class Quest;
	inline QUEST_COND_INFO_LIST & _AcceptConditionList()	{ return m_AcceptConditionList; }
	inline QUEST_ACT_INFO_LIST & _AcceptActionList()		{ return m_AcceptActionList; }
	inline QUEST_COND_INFO_LIST & _CompleteConditionList()	{ return m_CompleteConditionList; }
	inline QUEST_ACT_INFO_LIST & _CompleteActionList()		{ return m_CompleteActionList; }
	inline QUEST_ACT_INFO_LIST & _AbandonActionList()		{ return m_AbandonActionList; }

	QUEST_COND_INFO_LIST m_AcceptConditionList;
	QUEST_ACT_INFO_LIST	m_AcceptActionList;
	QUEST_COND_INFO_LIST m_CompleteConditionList;
	QUEST_ACT_INFO_LIST	m_CompleteActionList;	//< 현재 count == 0
	QUEST_ACT_INFO_LIST	m_AbandonActionList;

#if !defined(_NA_004157_20120409_QUEST_BANDWITH_INCREASING)
	QUEST_INFO_HASH m_ChildQuestInfoHash;
#endif

	char m_pszQuestName[MAX_QUEST_NAME_LENGTH];

	__PROPERTY( QCODE, QuestCode )
#ifdef _NA_20100307_BETAKEY_SYSTEM
    __PROPERTY( INT32, BetaKey )
#endif
#ifdef _YJ_RENEWAL_QUEST_UI_001265
		__PROPERTY(BYTE, Qtype)
#endif //_YJ_RENEWAL_QUEST_UI_001265
	__PROPERTY( DWORD, QNCode )
	__PROPERTY( BYTE, QDisplay )
	__PROPERTY( QLEVEL, QLevel )
	__PROPERTY( QCODE, ParentQuestCode )
	__PROPERTY( eQUEST_ACCEPT_TYPE, QuestAcceptType )			//< 퀘스트 타입
#ifdef C_NA_0_20100520_ACSYSTEM_CHANGED
	__PROPERTY( BYTE, ACCode )				//< 소속 AC 코드
	__PROPERTY( WORD, ACType )
#else
	__PROPERTY( DWORD, ACCode )				//< 소속 AC 코드
	__PROPERTY( eQTYPE, QType )
#endif
#ifdef _NA_003027_20111013_HONOR_SYSTEM
    __PROPERTY( BYTE, ACMaxRepeatNum )
    __PROPERTY( BYTE, DailyQuest )  // 0:일반 1:일일퀘스트 2:길드퀘스트 3: 도전일일퀘스트
    __PROPERTY( WORD, SetBackTime ) // 초기화시간: ex) 0430 (=04시30분)
    __PROPERTY( BYTE, SetBackDay )  // 초기화요일 1:매일 2~8:월~일 (0:사용안함)
    __PROPERTY( WORD, SquadCode )   // 일일퀘스트 그룹코드(같은 그룹코드의 퀘스트중 하나를 부여)
#endif //_NA_003027_20111013_HONOR_SYSTEM
	__PROPERTY( DWORD, QImgCode )
	__PROPERTY( DWORD, GroupCode )
	__PROPERTY( WORD, ContinueQuestCode )

	__PROPERTY(DWORD, RewardMapCode)
    // (CHANGES) (100220) (WAVERIX) #9499 fix up invalid copy operation \
    // which is not matched source and destination types. changes DWORD to WORD
	__ARRAY_PROPERTY( WORD, MAX_QUEST_KILL_MONSTER_MAP_CODE, KillMonsterMapCode )

	__PROPERTY( DWORD, NPCCode_Start )
	__PROPERTY( DWORD, NPCCode_Reward )

	__PROPERTY( DWORD, ExpiredTime )
#ifdef _DH_QUEST_ACCEPT_MESSAGE
    __PROPERTY( DWORD, Accept_Text )		//< 수락시 보여야 할 텍스트
#endif//_DH_QUEST_ACCEPT_MESSAGE
	__PROPERTY( DWORD, Compl_Text )			//< 완료시 보여야 할 텍스트
private:
	DWORD				m_RewardCodes[eCHAR_TYPE_MAX-1];	// 5
public:
	DWORD				GetRewardCode( const eCHAR_TYPE charType ) const;
	VOID				SetRewardCode( const eCHAR_TYPE charType, const DWORD rewardCode );
	__PROPERTY( DWORD, SCode_Start )
	__PROPERTY( DWORD, SCode_Progr1 )
	__PROPERTY( DWORD, SCode_Progr2 )
	__PROPERTY( DWORD, SCode_Compl )
	__PROPERTY( DWORD, SCode_Brief )
    __ARRAY_PROPERTY( DWORD, 3, TISMissionText )
};
