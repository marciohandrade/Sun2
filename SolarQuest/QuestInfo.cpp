#include "StdAfx.h"
#include ".\questinfo.h"
#include "QuestDefine.h"
#include <algorithm>

QuestInfo::QuestInfo(void)
{
#if !defined(_NA_004157_20120409_QUEST_BANDWITH_INCREASING)
	m_ChildQuestInfoHash.Initialize( 50 );
#endif
}

QuestInfo::~QuestInfo(void)
{
	
	Release();
}

void	QuestInfo::Release()
{
	for_each( m_AcceptConditionList.begin(), m_AcceptConditionList.end(), Deleter() );
	for_each( m_AcceptActionList.begin(), m_AcceptActionList.end(), Deleter() );
	for_each( m_CompleteConditionList.begin(), m_CompleteConditionList.end(), Deleter() );
	for_each( m_CompleteActionList.begin(), m_CompleteActionList.end(), Deleter() );
	for_each( m_AbandonActionList.begin(), m_AbandonActionList.end(), Deleter() );

	m_AcceptConditionList.clear();
	m_AcceptActionList.clear();
	m_CompleteConditionList.clear();
	m_CompleteActionList.clear();
	m_AbandonActionList.clear();

#if !defined(_NA_004157_20120409_QUEST_BANDWITH_INCREASING)
	m_ChildQuestInfoHash.clear();
#endif
}

DWORD	QuestInfo::GetRewardCode( const eCHAR_TYPE charType ) const
{
	switch( charType )
	{
	case eCHAR_BERSERKER:		return m_RewardCodes[eCHAR_BERSERKER-1];
	case eCHAR_DRAGON:			return m_RewardCodes[eCHAR_DRAGON-1];
	case eCHAR_SHADOW:			return m_RewardCodes[eCHAR_SHADOW-1];
	case eCHAR_VALKYRIE:		return m_RewardCodes[eCHAR_VALKYRIE-1];
	case eCHAR_ELEMENTALIST:	return m_RewardCodes[eCHAR_ELEMENTALIST-1];
    case eCHAR_MYSTIC:          return m_RewardCodes[eCHAR_MYSTIC-1]; // _NA_004965_20120613_NEW_CHARACTER_MYSTIC
    case eCHAR_HELLROID:        return m_RewardCodes[eCHAR_HELLROID-1]; //_NA_000000_20130812_NEW_CHARACTER_HELLROID
    case eCHAR_WITCHBLADE:      return m_RewardCodes[eCHAR_WITCHBLADE-1]; //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        
	};

	ASSERT( !"Unexpected CharType" );
	return 0;
}

VOID	QuestInfo::SetRewardCode( const eCHAR_TYPE charType, const DWORD rewardCode )
{
	switch( charType )
	{
	case eCHAR_BERSERKER:		m_RewardCodes[eCHAR_BERSERKER-1]	= rewardCode; return;
	case eCHAR_DRAGON:			m_RewardCodes[eCHAR_DRAGON-1]		= rewardCode; return;
	case eCHAR_SHADOW:			m_RewardCodes[eCHAR_SHADOW-1]		= rewardCode; return;
	case eCHAR_VALKYRIE:		m_RewardCodes[eCHAR_VALKYRIE-1]		= rewardCode; return;
	case eCHAR_ELEMENTALIST:	m_RewardCodes[eCHAR_ELEMENTALIST-1]	= rewardCode; return;
    case eCHAR_MYSTIC:          m_RewardCodes[eCHAR_MYSTIC-1]       = rewardCode; return; // _NA_004965_20120613_NEW_CHARACTER_MYSTIC
    case eCHAR_HELLROID:        m_RewardCodes[eCHAR_HELLROID-1]     = rewardCode; return; //_NA_000000_20130812_NEW_CHARACTER_HELLROID
    case eCHAR_WITCHBLADE:      m_RewardCodes[eCHAR_WITCHBLADE-1]   = rewardCode; return; //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        
	};

	ASSERT( !"Unexpected CharType" );
}

// __NA_0S0023_20081114_SPAWN_MONSTER_QUEST
QuestConditionInfo* QuestInfo::FindAcceptCondition( eQUEST_CONDITION_TYPE ConType )
{
	QUEST_COND_INFO_LIST_IT iter;
	QuestConditionInfo* pInfo = NULL;
	for( iter = m_AcceptConditionList.begin(); iter != m_AcceptConditionList.end(); ++iter )
	{
		pInfo = *iter;
		if( pInfo )
		{
			if( pInfo->GetType() == ConType )
				return pInfo;
		}
	}

	return NULL;
}

QuestActionInfo* QuestInfo::FindAcceptAction( eQUEST_ACTION_TYPE ActType )
{
	QUEST_ACT_INFO_LIST_IT iter;
	QuestActionInfo* pInfo = NULL;
	for( iter = m_AcceptActionList.begin(); iter != m_AcceptActionList.end(); ++iter )
	{
		pInfo = *iter;
		if( pInfo )
		{
			if( pInfo->GetType() == ActType )
				return pInfo;
		}
	}

	return NULL;
}

QuestConditionInfo* QuestInfo::FindCompleteCondition( eQUEST_CONDITION_TYPE ConType )
{
	QUEST_COND_INFO_LIST_IT iter;
	QuestConditionInfo* pInfo = NULL;
	for( iter = m_CompleteConditionList.begin(); iter != m_CompleteConditionList.end(); ++iter )
	{
		pInfo = *iter;
		if( pInfo )
		{
			if( pInfo->GetType() == ConType )
				return pInfo;
		}
	}

	return NULL;
}

QuestActionInfo* QuestInfo::FindCompleteAction( eQUEST_ACTION_TYPE ActType )
{
	QUEST_ACT_INFO_LIST_IT iter;
	QuestActionInfo* pInfo = NULL;
	for( iter = m_CompleteActionList.begin(); iter != m_CompleteActionList.end(); ++iter )
	{
		pInfo = *iter;
		if( pInfo )
		{
			if( pInfo->GetType() == ActType )
				return pInfo;
		}
	}

	return NULL;
}
