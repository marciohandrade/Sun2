#pragma once

#include <Struct.h>
#include <SunEventInfoParser.h>	// 타입만 참조한다.

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	<SSQHandlerCommon>
//
class SSQHandlerCommon
{
public:		SSQHandlerCommon();
			~SSQHandlerCommon();
public:
	static const DWORD				EVENT_MULTIUSE_EMPTYVALUE_FOR_SSQ = eEVENT_MULTIUSE_START;
	inline static BOOLEAN			IsSSQEventID( const DWORD eventID );
	inline	eEVENT_MODE_MULTIUSE	GetSSQEventID();

public:		BOOLEAN					Rewarded();	// 보상,복구 처리 완료상태로 전환
			BOOLEAN					GetRewardInfo( SSQ_RESERVED_REWARD_INFO& OUT rInfo ) const;
			BOOLEAN					SetRewardInfo( const SSQ_RESERVED_REWARD_INFO& IN rInfo );	// Game등에서 변경된 정보를 설정할 것이다.

	inline	VOID					BeginTransaction() { m_bTransaction = TRUE; }
	inline	VOID					EndTransaction() { m_bTransaction = FALSE; }
	inline	BOOLEAN					IsTransaction() { return m_bTransaction; }

protected:	BOOLEAN						m_bTransaction;
			//enum eARRIDX { SSQ_WINNER, SSQ_REFUND, MAX_ARR_SIZE };
			//SSQ_RESERVED_REWARD_INFO	m_RewardInfo[MAX_ARR_SIZE];
			SSQ_RESERVED_REWARD_INFO	m_RewardInfo;
};


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	<SSQHandlerBase>
//
template< typename _TYPE >
class SSQHandlerBase : public SSQHandlerCommon
{
};


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//
inline BOOLEAN
SSQHandlerCommon::IsSSQEventID( const DWORD eventID )
{
	return
		(    eEVENT_MULTIUSE_SSQ_WINNER_REWARD	== eventID )
		|| ( eEVENT_MULTIUSE_SSQ_QUEST_FAIL_INFO== eventID )
		|| ( eEVENT_MULTIUSE_SSQ_REFUND			== eventID )
		;
}

inline eEVENT_MODE_MULTIUSE
SSQHandlerCommon::GetSSQEventID()
{
	return (eEVENT_MODE_MULTIUSE)m_RewardInfo.m_SharedInfo.EventID;
}

