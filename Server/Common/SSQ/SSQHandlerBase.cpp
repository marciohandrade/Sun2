#include "stdafx.h"
#include "SSQHandlerBase.h"

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//
SSQHandlerCommon::SSQHandlerCommon()
	: m_bTransaction(FALSE)
{
	ZeroMemory( &m_RewardInfo, sizeof(m_RewardInfo) );
	m_RewardInfo.m_SharedInfo.EventID = EVENT_MULTIUSE_EMPTYVALUE_FOR_SSQ;
}


SSQHandlerCommon::~SSQHandlerCommon()
{
}


BOOLEAN
SSQHandlerCommon::Rewarded()	// 보상,복구 처리 완료상태로 전환
{
	if( m_bTransaction )
		return FALSE;

	const eEVENT_MODE_MULTIUSE event_mode = (eEVENT_MODE_MULTIUSE)m_RewardInfo.m_SharedInfo.EventID;
	ZeroMemory( &m_RewardInfo, sizeof(m_RewardInfo) );
	m_RewardInfo.m_SharedInfo.EventID = event_mode;
	m_RewardInfo.m_SharedInfo.Status = TRUE;
	return TRUE;
}


BOOLEAN
SSQHandlerCommon::GetRewardInfo( SSQ_RESERVED_REWARD_INFO& OUT rInfo ) const
{
	if( m_bTransaction )
		return FALSE;

	const SSQ_RESERVED_REWARD_INFO::SharedField& rSharedInfo = m_RewardInfo.m_SharedInfo;
	const bool bExistReward = (EVENT_MULTIUSE_EMPTYVALUE_FOR_SSQ != rSharedInfo.EventID);
	const bool bNeedReward = (FALSE == rSharedInfo.Status);

	if( bExistReward&bNeedReward )
	{
		rInfo = m_RewardInfo;
		return TRUE;
	}

	ASSERT( (bExistReward|bNeedReward) );
	return FALSE;
}


BOOLEAN
SSQHandlerCommon::SetRewardInfo( const SSQ_RESERVED_REWARD_INFO& IN rInfo )
{
	if( m_bTransaction )
		return FALSE;

	SSQ_RESERVED_REWARD_INFO::SharedField& rSharedInfo = m_RewardInfo.m_SharedInfo;
	const BOOLEAN isRegistered = (EVENT_MULTIUSE_EMPTYVALUE_FOR_SSQ != rSharedInfo.EventID) && (FALSE == rSharedInfo.Status);
	if( isRegistered )
		return TRUE;

	const DWORD mode = rInfo.m_SharedInfo.EventID;
	switch( mode )
	{
	case eEVENT_MULTIUSE_SSQ_WINNER_REWARD:
	case eEVENT_MULTIUSE_SSQ_QUEST_FAIL_INFO:
	case eEVENT_MULTIUSE_SSQ_REFUND:
		m_RewardInfo = rInfo;
		break;
	default:
		ASSERT( !"Invalid SSQ Reward Process Type" );
		return FALSE;
	}
	return TRUE;
}


