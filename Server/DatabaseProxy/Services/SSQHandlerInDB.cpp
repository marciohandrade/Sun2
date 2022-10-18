#include "stdafx.h"
#include "SSQHandlerInDB.h"

#include <SunEventInfoParser.h>

//const DWORD		EVENT_MULTIUSE_EMPTYVALUE_FOR_SSQ = eEVENT_MULTIUSE_START;

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//

SSQHandlerInDB::SSQHandlerInDB()
{
}


SSQHandlerInDB::~SSQHandlerInDB()
{
}


//BOOLEAN
//SSQHandlerInDB::Rewarded( const eEVENT_MODE_MULTIUSE mode )	// 보상,복구 처리 완료상태로 전환
//{
//	if( m_bTransaction )
//		return FALSE;
//
//	switch( mode )
//	{
//	case eEVENT_MULTIUSE_SSQ_WINNER_REWARD:
//		ZeroMemory( &m_RewardInfo[SSQ_WINNER], sizeof(m_RewardInfo[SSQ_WINNER]) );
//		m_RewardInfo[SSQ_WINNER].m_WinnerReward.EventID	= eEVENT_MULTIUSE_SSQ_WINNER_REWARD;
//		m_RewardInfo[SSQ_WINNER].m_WinnerReward.Status	= TRUE;
//		break;
//	case eEVENT_MULTIUSE_SSQ_REFUND:
//		ZeroMemory( &m_RewardInfo[SSQ_REFUND], sizeof(m_RewardInfo[SSQ_REFUND]) );
//		m_RewardInfo[SSQ_REFUND].m_AdmissionFeeRefund.EventID = eEVENT_MULTIUSE_SSQ_REFUND;
//		m_RewardInfo[SSQ_REFUND].m_AdmissionFeeRefund.Status = TRUE;
//		break;
//	default:
//		ASSERT( !"Invalid SSQ Reward Process Type" );
//		return FALSE;
//	}
//	return TRUE;
//}


//BOOLEAN
//SSQHandlerInDB::GetRewardInfo( SSQ_RESERVED_REWARD_INFO& OUT rInfo ) const
//{
//	if( m_bTransaction )
//		return FALSE;
//
//	const DWORD winnerID = m_RewardInfo[SSQ_WINNER].m_WinnerReward.EventID;
//	const DWORD refundID = m_RewardInfo[SSQ_REFUND].m_AdmissionFeeRefund.EventID;
//
//	const BOOLEAN abnormal_status = (EVENT_MULTIUSE_EMPTYVALUE_FOR_SSQ != winnerID) && (EVENT_MULTIUSE_EMPTYVALUE_FOR_SSQ != refundID);
//	if( abnormal_status )
//	{
//		ASSERT( !"Abnormal Status in SSQReward" );
//		return FALSE;	// 이 문제를 해결해야 할 것이다.
//	}
//
//	rInfo = EVENT_MULTIUSE_EMPTYVALUE_FOR_SSQ != winnerID
//		?	m_RewardInfo[SSQ_WINNER]
//		:	m_RewardInfo[SSQ_REFUND]
//		;
//
//	return TRUE;
//}


struct SSQ_RESERVED_REWARD_INFO_FOR_DB
{	// 총 5개의 필드 사용 가능
	enum {};
#pragma pack(push, 1)
	struct EventStatusQueryStruct {	//EventStatusQuery::sQUERY의 구조
		INT		m_EventID;				// m_SSQRewardType
		INT		m_RegisterCnt;
		INT		m_DonateHeimRewardCnt;
		INT		m_DonatedHeims;
		INT		m_Status;					// bit
	};

	//	다음 정보는 EventStatusQueryStruct와 정확히 매치시킨 정보다.
	struct WinnerReward {
		CODETYPE	EventID;
		DWORD		Point;				// used) m_RegisterCnt				- 획득 포인트
		DWORD		Exp;				// used) m_DonateHeimRewardCnt		- 획득 경험치
		BYTE		MonsterKillNum;		// used) m_DonatedHeims:8			- 몬스터 킬 수
		BYTE		PlayerKillNum;		// used) m_DonatedHeims:8			- 플레이어 킬 수
		SLOTCODE	ItemCode;			// used) m_DonatedHeims:16			- 보상 아이템(판도라 x 1EA)
		BYTE		Status;
	};
	struct QuestFail {
		CODETYPE	EventID;
		DWORD		Point;				// used) m_RegisterCnt				- 획득 포인트
		DWORD		Exp;				// used) m_DonateHeimRewardCnt		- 획득 경험치
		BYTE		MonsterKillNum;		// used) m_DonatedHeims:8			- 몬스터 킬 수
		BYTE		PlayerKillNum;		// used) m_DonatedHeims:8			- 플레이어 킬 수
		WORD		__PAD__00;
		BYTE		Status;
	};
	struct AdmissionFeeRefund {
		CODETYPE	EventID;
		SLOTCODE	ItemCode;			// used) m_RegisterCnt				- 복구해줄 아이템
		WORD		__PAD__00;
		DWORD		__PAD__01;
		DWORD		Money;				// used) m_DonatedHeims				- 복구해줄 Heim
		BYTE		Status;
	};

	union {
		EventStatusQuery::sQUERY	m_QueryBlock;
		EventStatusQueryStruct		m_QueryBlockMatching;
		WinnerReward				m_WinnerReward;				// 승자 보상 정보, DB
		QuestFail					m_QuestFail;				// 퀘스트 실패 (정산정보)
		AdmissionFeeRefund			m_AdmissionFeeRefund;		// 입장료 환불 처리
	};
#pragma pack(pop)

	static VOID		Convert( SSQ_RESERVED_REWARD_INFO::WinnerReward& OUT rDest, const SSQ_RESERVED_REWARD_INFO_FOR_DB::WinnerReward& IN rSrc )
	{
		ZeroMemory( &rDest, sizeof(rDest) );
		rDest.EventID			= rSrc.EventID;
		rDest.Point				= rSrc.Point;
		rDest.Exp				= rSrc.Exp;
		rDest.MonsterKillNum	= rSrc.MonsterKillNum;
		rDest.PlayerKillNum		= rSrc.PlayerKillNum;
		rDest.ItemCode			= rSrc.ItemCode;
		rDest.Status			= rSrc.Status;
	}

	static VOID		Convert( SSQ_RESERVED_REWARD_INFO::QuestFail& OUT rDest, const SSQ_RESERVED_REWARD_INFO_FOR_DB::QuestFail& IN rSrc )
	{
		ZeroMemory( &rDest, sizeof(rDest) );
		rDest.EventID			= rSrc.EventID;
		rDest.Point				= rSrc.Point;
		rDest.Exp				= rSrc.Exp;
		rDest.MonsterKillNum	= rSrc.MonsterKillNum;
		rDest.PlayerKillNum		= rSrc.PlayerKillNum;
		rDest.Status			= rSrc.Status;
	}

	static VOID		Convert( SSQ_RESERVED_REWARD_INFO::AdmissionFeeRefund& OUT rDest, const SSQ_RESERVED_REWARD_INFO_FOR_DB::AdmissionFeeRefund& IN rSrc )
	{
		ZeroMemory( &rDest, sizeof(rDest) );
		rDest.EventID			= rSrc.EventID;
		rDest.ItemCode			= rSrc.ItemCode;
		rDest.Money				= rSrc.Money;
		rDest.Status			= rSrc.Status;
	}


	static VOID		Convert( SSQ_RESERVED_REWARD_INFO_FOR_DB::WinnerReward& OUT rDest, const SSQ_RESERVED_REWARD_INFO::WinnerReward& IN rSrc )
	{
		ZeroMemory( &rDest, sizeof(rDest) );
		rDest.EventID			= rSrc.EventID;
		rDest.Point				= rSrc.Point;
		rDest.Exp				= rSrc.Exp;
		rDest.MonsterKillNum	= rSrc.MonsterKillNum;
		rDest.PlayerKillNum		= rSrc.PlayerKillNum;
		rDest.ItemCode			= rSrc.ItemCode;
		rDest.Status			= rSrc.Status;
	}

	static VOID		Convert( SSQ_RESERVED_REWARD_INFO_FOR_DB::QuestFail& OUT rDest, const SSQ_RESERVED_REWARD_INFO::QuestFail& IN rSrc )
	{
		ZeroMemory( &rDest, sizeof(rDest) );
		rDest.EventID			= rSrc.EventID;
		rDest.Point				= rSrc.Point;
		rDest.Exp				= rSrc.Exp;
		rDest.MonsterKillNum	= rSrc.MonsterKillNum;
		rDest.PlayerKillNum		= rSrc.PlayerKillNum;
		rDest.Status			= rSrc.Status;
	}

	static VOID		Convert( SSQ_RESERVED_REWARD_INFO_FOR_DB::AdmissionFeeRefund& OUT rDest, const SSQ_RESERVED_REWARD_INFO::AdmissionFeeRefund& IN rSrc )
	{
		ZeroMemory( &rDest, sizeof(rDest) );
		rDest.EventID			= rSrc.EventID;
		rDest.ItemCode			= rSrc.ItemCode;
		rDest.Money				= rSrc.Money;
		rDest.Status			= rSrc.Status;
	}
};

BOOST_STATIC_ASSERT( sizeof(EventStatusQuery::sQUERY) == sizeof(SSQ_RESERVED_REWARD_INFO_FOR_DB::EventStatusQueryStruct) );


VOID
SSQHandlerInDB::SetSerializedInfo( EventStatusQuery::sQUERY& IN rSource )
{
	if( m_bTransaction )
		return;

	const BOOLEAN isCompleted = !!rSource.m_Status;
	if( isCompleted )
		return;

	const DWORD event_type = rSource.m_EventID;
	switch( event_type )
	{
	case eEVENT_MULTIUSE_SSQ_WINNER_REWARD:
		{
			const SSQ_RESERVED_REWARD_INFO_FOR_DB::WinnerReward& rSrc =
				reinterpret_cast<SSQ_RESERVED_REWARD_INFO_FOR_DB::WinnerReward&>(rSource);
			SSQ_RESERVED_REWARD_INFO::WinnerReward& rDest = m_RewardInfo.m_WinnerReward;

			SSQ_RESERVED_REWARD_INFO_FOR_DB::Convert( rDest, rSrc );
		}
		break;
	case eEVENT_MULTIUSE_SSQ_QUEST_FAIL_INFO:
		{
			const SSQ_RESERVED_REWARD_INFO_FOR_DB::QuestFail& rSrc =
				reinterpret_cast<SSQ_RESERVED_REWARD_INFO_FOR_DB::QuestFail&>(rSource);
			SSQ_RESERVED_REWARD_INFO::QuestFail& rDest = m_RewardInfo.m_QuestFail;

			SSQ_RESERVED_REWARD_INFO_FOR_DB::Convert( rDest, rSrc );
		}
		break;
	case eEVENT_MULTIUSE_SSQ_REFUND:
		{
			const SSQ_RESERVED_REWARD_INFO_FOR_DB::AdmissionFeeRefund& rSrc =
				reinterpret_cast<SSQ_RESERVED_REWARD_INFO_FOR_DB::AdmissionFeeRefund&>(rSource);
			SSQ_RESERVED_REWARD_INFO::AdmissionFeeRefund& rDest = m_RewardInfo.m_AdmissionFeeRefund;

			SSQ_RESERVED_REWARD_INFO_FOR_DB::Convert( rDest, rSrc );
		}
		break;
	default:
		ASSERT( !"Invalid SSQ Reward Process Type" );
		return;
	}
}

BOOLEAN
SSQHandlerInDB::MakeQuery( STRING_SAFER_QUERY256& OUT rQueryBuff, const DWORD char_guid )
{
	const DWORD event_type = m_RewardInfo.m_SharedInfo.EventID;
	if( EVENT_MULTIUSE_EMPTYVALUE_FOR_SSQ == event_type )
		return FALSE;

	SSQ_RESERVED_REWARD_INFO_FOR_DB db_struct;
	ZeroMemory( &db_struct, sizeof(db_struct) );

	
	switch( event_type )
	{
	case eEVENT_MULTIUSE_SSQ_WINNER_REWARD:
		{
			const SSQ_RESERVED_REWARD_INFO::WinnerReward& rSrc = m_RewardInfo.m_WinnerReward;
			SSQ_RESERVED_REWARD_INFO_FOR_DB::WinnerReward& rDest = db_struct.m_WinnerReward;

			SSQ_RESERVED_REWARD_INFO_FOR_DB::Convert( rDest, rSrc );
		}
		break;
	case eEVENT_MULTIUSE_SSQ_QUEST_FAIL_INFO:
		{
			const SSQ_RESERVED_REWARD_INFO::QuestFail& rSrc = m_RewardInfo.m_QuestFail;
			SSQ_RESERVED_REWARD_INFO_FOR_DB::QuestFail& rDest = db_struct.m_QuestFail;

			SSQ_RESERVED_REWARD_INFO_FOR_DB::Convert( rDest, rSrc );
		}
		break;
	case eEVENT_MULTIUSE_SSQ_REFUND:
		{
			const SSQ_RESERVED_REWARD_INFO::AdmissionFeeRefund& rSrc = m_RewardInfo.m_AdmissionFeeRefund;
			SSQ_RESERVED_REWARD_INFO_FOR_DB::AdmissionFeeRefund& rDest = db_struct.m_AdmissionFeeRefund;

			SSQ_RESERVED_REWARD_INFO_FOR_DB::Convert( rDest, rSrc );
		}
		break;
	}

	SSQ_RESERVED_REWARD_INFO_FOR_DB::EventStatusQueryStruct& rMigration = db_struct.m_QueryBlockMatching;

	rQueryBuff.MakeString(
		"{?=call S_Event_Status_Reg(%d,%d,%d,%d,%d,%d)}"
		, char_guid
		, rMigration.m_EventID
		, rMigration.m_RegisterCnt
		, rMigration.m_DonateHeimRewardCnt
		, rMigration.m_DonatedHeims
		, rMigration.m_Status
		);

	return TRUE;
}

//--------------------------------------------------------------------------------------------------

void ISSQHandlerInDB::Init()
{
    if (m_pHandler == NULL) {
        m_pHandler = TAllocNew(SSQHandlerInDB);
    }
}

void ISSQHandlerInDB::Release()
{
    if (m_pHandler) {
        TAllocDelete(SSQHandlerInDB, m_pHandler);
    }
    m_pHandler = NULL;
}
