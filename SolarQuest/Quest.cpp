#include "StdAfx.h"
#include ".\quest.h"
#include ".\QuestCondition.h"
#include ".\QuestAction.h"
#include ".\QuestManager.h"
#include ".\QuestInfoManager.h"

__IMPL_CUSTOMPOOL_PTR( Quest )
Quest::Quest(void):
	m_pRepeatedNum(NULL)
{
	m_AcceptConditionHash.Initialize(15);
	m_AcceptActionHash.Initialize(5);
	m_CompleteConditionHash.Initialize(10);
	m_CompleteActionHash.Initialize(5);
	m_AbandonActionHash.Initialize(3);
}

Quest::~Quest(void)
{
	assert( m_AcceptConditionHash.size() == 0 );
	assert( m_AcceptActionHash.size() == 0 );
	assert( m_CompleteConditionHash.size() == 0 );
	assert( m_CompleteActionHash.size() == 0 );
	assert( m_AbandonActionHash.size() == 0 );
}

void Quest::Init( QuestInfoManager * pInfoMgr, QuestManager * pMgr, QUESTPART_COMPL * pQCompl )
{
	SetQuestManager( pMgr );
	SetQuestInfoManager( pInfoMgr );
    SetQuestInfo(NULL);
	SetQState( QS_NORMAL );
	m_pRepeatedNum = &pQCompl->m_Num;
    m_ExpiredTimer.DisableCheckTime();
}


void Quest::Serialize( QUESTPART_PROGR & rQProgrStream, eQUEST_SERIALIZE eType )
{
	if( eType == QSERIALIZE_LOAD )
	{
		rQProgrStream.m_ExpireTime = m_ExpiredTimer.GetRemainedTime()/1000;
		rQProgrStream.m_QCode = GetQuestCode();
		rQProgrStream.m_QState = GetQState();

		QUEST_COND_HASH_IT it, end(m_CompleteConditionHash.end());
		for( it = m_CompleteConditionHash.begin() ; it != end ; ++it )
		{
			(*it)->CopyOut( rQProgrStream );
		}
	}
	else	//< SAVE
	{
		QuestInfo * pInfo = GetQuestInfoManager()->FindQuestInfo( rQProgrStream.m_QCode );
		if( !pInfo ) { return ; }

		SetQuestInfo( pInfo );
		Build( &rQProgrStream );
	}
}

void Quest::Destroy()
{
	QUEST_COND_HASH_IT it, end(m_AcceptConditionHash.end());
	for( it = m_AcceptConditionHash.begin() ; it != end ; ++it )
	{
		GetQuestManager()->DestroyQuestCondition((*it));
	}
	QUEST_ACT_HASH_IT it2, end2(m_AcceptActionHash.end());
	for( it2 = m_AcceptActionHash.begin() ; it2 != end2 ; ++it2 )
	{
		GetQuestManager()->DestroyQuestAction((*it2));
	}
	QUEST_COND_HASH_IT it3, end3(m_CompleteConditionHash.end());
	for( it3 = m_CompleteConditionHash.begin() ; it3 != end3 ; ++it3 )
	{
		GetQuestManager()->DestroyQuestCondition((*it3));
	}
	QUEST_ACT_HASH_IT it4, end4(m_CompleteActionHash.end());
	for( it4 = m_CompleteActionHash.begin() ; it4 != end4 ; ++it4 )
	{
		GetQuestManager()->DestroyQuestAction((*it4));
	}
	QUEST_ACT_HASH_IT it5, end5(m_AbandonActionHash.end());
	for( it5 = m_AbandonActionHash.begin() ; it5 != end5 ; ++it5 )
	{
		GetQuestManager()->DestroyQuestAction((*it5));
	}
	m_AcceptConditionHash.clear();
	m_AcceptActionHash.clear();
	m_CompleteConditionHash.clear();
	m_CompleteActionHash.clear();
	m_AbandonActionHash.clear();
}

void Quest::Build( const QUESTPART_PROGR * pStream )
{
	Destroy();

	if( pStream )
		SetQState((eQUEST_STATE)pStream->m_QState);
	
	StartTimer(pStream);

	QUEST_COND_INFO_LIST & rAcceptConditionList = GetQuestInfo()->_AcceptConditionList();
	QUEST_ACT_INFO_LIST & rAcceptActionList = GetQuestInfo()->_AcceptActionList();
	QUEST_COND_INFO_LIST & rCompleteConditionList = GetQuestInfo()->_CompleteConditionList();
	QUEST_ACT_INFO_LIST & rCompleteActionList = GetQuestInfo()->_CompleteActionList();
	QUEST_ACT_INFO_LIST & rAbandonActionList = GetQuestInfo()->_AbandonActionList();

	QUEST_COND_INFO_LIST_IT it, end(rAcceptConditionList.end());
	for( it = rAcceptConditionList.begin() ; it != end ; ++it )
	{
		QuestCondition * pCondition = GetQuestManager()->CreateQuestCondition((*it));
		if(!pCondition) continue;
		pCondition->SetQuest(this);
		pCondition->SetManager(GetQuestManager());
		pCondition->Init( pStream );
		AddAcceptCondition(pCondition);
	}
	QUEST_ACT_INFO_LIST_IT it2, end2(rAcceptActionList.end());
	for( it2 = rAcceptActionList.begin() ; it2 != end2 ; ++it2 )
	{
		QuestAction * pAction = GetQuestManager()->CreateQuestAction((*it2));
		if(!pAction) continue;
		pAction->SetQuest(this);
		pAction->SetManager(GetQuestManager());
		AddAcceptAction(pAction);
	}
	QUEST_COND_INFO_LIST_IT it3, end3(rCompleteConditionList.end());
	for( it3 = rCompleteConditionList.begin() ; it3 != end3 ; ++it3 )
	{
		QuestCondition * pCondition = GetQuestManager()->CreateQuestCondition((*it3));
		if(!pCondition) continue;
		pCondition->SetQuest(this);
		pCondition->SetManager(GetQuestManager());
		pCondition->Init( pStream );
		AddCompleteCondition(pCondition);
	}
	QUEST_ACT_INFO_LIST_IT it4, end4(rCompleteActionList.end());
	for( it4 = rCompleteActionList.begin() ; it4 != end4 ; ++it4 )
	{
		QuestAction * pAction = GetQuestManager()->CreateQuestAction((*it4));
		if(!pAction) continue;
		pAction->SetQuest(this);
		pAction->SetManager(GetQuestManager());
		AddCompleteAction(pAction);
	}

	QUEST_ACT_INFO_LIST_IT it5, end5(rAbandonActionList.end());
	for( it5 = rAbandonActionList.begin() ; it5 != end5 ; ++it5 )
	{
		QuestAction * pAction = GetQuestManager()->CreateQuestAction((*it5));
		if(!pAction) continue;
		pAction->SetQuest(this);
		pAction->SetManager(GetQuestManager());
		AddAbandonAction(pAction);
	}
}

BOOL Quest::Event( eQUEST_EVENT evt, QUEST_MSG * pMsg )
{
	QUEST_COND_HASH_IT it, end( m_CompleteConditionHash.end() );
	for( it = m_CompleteConditionHash.begin() ; it != end ; ++it )
	{
		if( FALSE != (*it)->Event(evt,pMsg) ) 
		{
			return TRUE;
		}
	}
	
	return FALSE;
}

RC::eQUEST_RESULT Quest::CanAccept( void * pvParam )
{
	QUEST_COND_HASH_IT it, end( m_AcceptConditionHash.end() );
	for( it = m_AcceptConditionHash.begin() ; it != end ; ++it )
	{
		RC::eQUEST_RESULT rt;		
		rt = (*it)->CanAccept(pvParam);
		if( rt != RC::RC_QUEST_SUCCESS ) 
		{
			return rt;
		}
	}

	return RC::RC_QUEST_SUCCESS;
}

RC::eQUEST_RESULT Quest::CanComplete( void * pvParam )
{
	Update();
	//(lst1024)(090306) 현재 퀘스트 상태 - QS_COMPLETED 임 -> (QS_FAILED 인가?) 로 체크 변경.
	if( QS_FAILED == m_QState )
		return RC::RC_QUEST_FAILED;

	QUEST_COND_HASH_IT it, end( m_CompleteConditionHash.end() );
	for( it = m_CompleteConditionHash.begin() ; it != end ; ++it )
	{
		RC::eQUEST_RESULT rt;
		rt = (*it)->CanComplete(pvParam);
		if( rt != RC::RC_QUEST_SUCCESS ) 
		{
			return rt;
		}
	}

	return RC::RC_QUEST_SUCCESS;
}

void Quest::DoAccept( void * pvParam )
{
	if( GetQuestInfo()->IsChildQuest() )
		return ;

	SetQState( QS_NORMAL );

    QUEST_ACT_HASH_IT it( m_AcceptActionHash.begin() );
	QUEST_ACT_HASH_IT end( m_AcceptActionHash.end() );

	for( it ; it != end ; ++it )
	{
		(*it)->DoAccept(pvParam);
	}
}

// 완료 시점에 해줌
void Quest::DoComplete( void * pvParam )
{

	__UNUSED(pvParam);
   
	SetQState( QS_COMPLETED );

	// 자식 퀘스트 일 경우, 완료 기록함
	if( GetQuestInfo()->IsChildQuest() )
	{
		GetQuestManager()->IncComplStream( GetQuestCode() );
	}
}

void Quest::DoReward( void * pvParam )
{
	EndTimer();

	if( GetQuestInfo()->IsChildQuest() )
		return;

	QUEST_ACT_HASH_IT it, end( m_CompleteActionHash.end() );
	for( it = m_CompleteActionHash.begin() ; it != end ; ++it )
	{
		(*it)->DoComplete(pvParam);
	}
}

void Quest::DoAbandon( void * pvParam )
{
	EndTimer();

	if( GetQuestInfo()->IsChildQuest() )
		return;

	QUEST_ACT_HASH_IT it, end( m_AbandonActionHash.end() );
	for( it = m_AbandonActionHash.begin() ; it != end ; ++it )
	{
		(*it)->DoComplete(pvParam);
	}

#if !defined(_NA_004157_20120409_QUEST_BANDWITH_INCREASING)
	// 제한 조건 : 자식 퀘스트는 부여 아이템이 존재하면 안된다.
	// 이유 : 부모퀘스트 포기시 자식퀘스트의 부여 아이템 삭제가 불분명하다.
	Quest * pChild = NULL;
	QUEST_INFO_HASH_ITR it1(GetQuestInfo()->m_ChildQuestInfoHash.begin()), 
		end1(GetQuestInfo()->m_ChildQuestInfoHash.end());
	for( ; it1 != end1 ; ++it1 )
	{
		pChild = GetQuestManager()->FindQuest( (*it1)->GetQuestCode() );
		assert(pChild);
		if( pChild && pChild->GetQState() == QS_COMPLETED )
			GetQuestManager()->DecComplStream( (*it1)->GetQuestCode() );
	}
#endif
}

void Quest::StartTimer( const QUESTPART_PROGR * pStream ) 
{ 
	assert( m_pQuestInfo != NULL );
	m_ExpiredTimer.Clear();

	if( IsTimeQuest() )
	{
		if( pStream )
			m_ExpiredTimer.SetTimer( pStream->m_ExpireTime*1000 );
		else
			m_ExpiredTimer.SetTimer( m_pQuestInfo->GetExpiredTime()*1000 ); 

	}
	else
		m_ExpiredTimer.DisableCheckTime();
}

void Quest::EndTimer() 
{ 
	m_ExpiredTimer.DisableCheckTime();
}


BOOL Quest::Update()
{
	if(	m_ExpiredTimer.IsExpiredManual(FALSE) )
	{
		SetQState( QS_FAILED );
		return FALSE;
	}
	return TRUE;
}


