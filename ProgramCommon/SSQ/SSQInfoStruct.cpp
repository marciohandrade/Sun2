#include "stdafx.h"

#include "SSQInfoStruct.h"
#include "SSQInfoParser.h"

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	<SSQInfoStruct.cpp>
//	<Decriptions>
//	-	타락한 사원 SSQ(Survival Soloing Quest)
//	<Lists>
//	-	SSQInfo
//	-	SSQPattern
//

//───────────────────────────────────────────────────────────
//	<DEFINITIONs>
inline SSQInfo::WzTmFmt		_ConvertToWzTmFmt( const WORD hour, const WORD minute )
{
	return (SSQInfo::WzTmFmt)(hour)*100 + (minute);
}

//───────────────────────────────────────────────────────────
//	<INTERFACEs>
inline const SSQInfo::TimeRange* const
SSQInfo::_FindMatchTimeRange(
			const SSQInfo::TimeRange* const		pBegin
			, const SSQInfo::TimeRange* const	pEnd
			, const SSQInfo::WzTmFmt			calctime
			, DWORD* OUT						pIndex
			) const
{
	const TimeRange* const pFind =
		std::find_if( pBegin, pEnd, boost::bind( &TimeRange::OnTime, _1, calctime ) );

	if( pIndex )
		*pIndex = (DWORD)( ((BYTE*)pFind - (BYTE*)pBegin)/sizeof(TimeRange) );

	return pFind;
}

bool	SSQInfo::IsSSQOpenTime( const SYSTEMTIME* IN pSysTime, DWORD* OUT pIndex ) const
{
	if( !pSysTime )
	{
		SYSTEMTIME sysTime;
		util::TimeSync::GetLocalTime( &sysTime );
		pSysTime = &sysTime;
	}
	const WzTmFmt calctime = _ConvertToWzTmFmt( pSysTime->wHour, pSysTime->wMinute );

	const SSQInfo::TimeRange* const pEnd = &m_TimeRange[MI_ENTER_TIME_SECTION_MAX];
	const TimeRange* const pFind =
		_FindMatchTimeRange( &m_TimeRange[0], pEnd, calctime, pIndex );

	return pFind != pEnd;
}

bool	SSQInfo::IsNoticeTime( const SYSTEMTIME* IN pSysTime, DWORD* OUT pIndex ) const
{
	if( !pSysTime )
	{
		SYSTEMTIME sysTime;
		util::TimeSync::GetLocalTime( &sysTime );
		pSysTime = &sysTime;
	}
	const WzTmFmt calctime = _ConvertToWzTmFmt( pSysTime->wHour, pSysTime->wMinute );

	const SSQInfo::TimeRange* const pEnd = &m_CTimeNotice[MI_ENTER_TIME_SECTION_MAX];
	const TimeRange* const pFind =
		_FindMatchTimeRange( &m_CTimeNotice[0], pEnd, calctime, pIndex );

	return pFind != pEnd;
}

bool	SSQInfo::IsWaitingTime( const SYSTEMTIME* IN pSysTime, DWORD* OUT pIndex ) const
{
	if( !pSysTime )
	{
		SYSTEMTIME sysTime;
		util::TimeSync::GetLocalTime( &sysTime );
		pSysTime = &sysTime;
	}
	const WzTmFmt calctime = _ConvertToWzTmFmt( pSysTime->wHour, pSysTime->wMinute );

	const SSQInfo::TimeRange* const pEnd = &m_CTimeWaiting[MI_ENTER_TIME_SECTION_MAX];
	const TimeRange* const pFind =
		_FindMatchTimeRange( &m_CTimeWaiting[0], pEnd, calctime, pIndex );

	return pFind != pEnd;
}

bool	SSQInfo::IsBattleTime( const SYSTEMTIME* IN pSysTime, DWORD* OUT pIndex ) const
{
	if( !pSysTime )
	{
		SYSTEMTIME sysTime;
		util::TimeSync::GetLocalTime( &sysTime );
		pSysTime = &sysTime;
	}
	const WzTmFmt calctime = _ConvertToWzTmFmt( pSysTime->wHour, pSysTime->wMinute );

	const SSQInfo::TimeRange* const pEnd = &m_CTimeBattle[MI_ENTER_TIME_SECTION_MAX];
	const TimeRange* const pFind =
		_FindMatchTimeRange( &m_CTimeBattle[0], pEnd, calctime, pIndex );

	return pFind!=pEnd;
}


#ifdef _SERVER

//───────────────────────────────────────────────────────────
//	<지진 발생 조건>
struct sEARTHQUAKE_CONDNODE
{
	SSQInfo::MINUTE_T	OverThan;	// 지연 시간
	SSQInfo::COUNT_T	RemainCnt;	// 남은 인원

	inline bool		OnSatisfiedCondition( const sEARTHQUAKE_CONDNODE& rQuery ) const
	{
		return (rQuery.RemainCnt<RemainCnt) || (OverThan<=rQuery.OverThan);
	}
};

static const sEARTHQUAKE_CONDNODE
sc_EQ_Cond[SSQInfo::MI_EARTHQUAKE_SECTION_MAX] =
{
	// 지연시간 | 남은 인원
	  {  3,			70	}	// 1단계
	, {  6,			50	}	// 2단계
	, {  9,			30	}	// 3단계
	, { 12,			10	}	// 4단계
};


bool
SSQInfo::IsSatisfiedEarthquakeCondition(
			const SSQInfo::MINUTE_T		elapsedTm
			, const SSQInfo::COUNT_T	curplayercnt
			) const	//for SERVER
{
	const sEARTHQUAKE_CONDNODE* const pEnd	= &sc_EQ_Cond[MI_EARTHQUAKE_SECTION_MAX];
	const sEARTHQUAKE_CONDNODE* pIt			= &sc_EQ_Cond[0];
	const sEARTHQUAKE_CONDNODE cmproot		= { elapsedTm, curplayercnt };
	for( ; pIt!=pEnd ; ++pIt )
	{
		const sEARTHQUAKE_CONDNODE& rCur	= *pIt;
		if( rCur.OnSatisfiedCondition( cmproot ) )
			break;
	}

	return pIt!=pEnd;
}


MONSTERCODE
SSQInfo::GetRandomRegenMonster() const
{
	const WORD rnd_val = (WORD)RandomNumberGenerator::GetRandomNumberRange( 0, m_SummonedNpcInfo.MaxRegenValue );

	const SSQInfo::SUMMONED_NPC_NODE* pIt = m_SummonedNpcInfo.List;
	const SSQInfo::SUMMONED_NPC_NODE* const pItEnd = &m_SummonedNpcInfo.List[m_SummonedNpcInfo.Count];
	for( ; pIt != pItEnd ; ++pIt )
	{
		const SSQInfo::SUMMONED_NPC_NODE& rNode = *pIt;
		if( rnd_val < rNode.m_SumRatio )
			break;
	}

	if( FlowControl::FCAssert( pIt != pItEnd ) )
	{
		const SSQInfo::SUMMONED_NPC_NODE& rNode = *pIt;
		return rNode.m_NPCCode;
	}

	return m_SummonedNpcInfo.List[0].m_NPCCode;
}


#endif //_SERVER

//
//───────────────────────────────────────────────────────────


//───────────────────────────────────────────────────────────
//

namespace nsSSQ
{

//───────────────────────────────────────────────────────────
VOID
SSQTicketTimeInfo::Init()
{
	tmTableIndex	= SSQInfo::MI_ENTER_TIME_SECTION_MAX;
	tmMode			= eTM_None;
	countdownTick	= 0;
	check_start_tick= 0;
	ZeroMemory( &systemTime, sizeof(systemTime) );
}


//───────────────────────────────────────────────────────────
const DWORD portal_close_timeout_c	=  4;	// 4'00"
//////////////////////////////////////////////////////////////////////////


//───────────────────────────────────────────────────────────


inline VOID
SSQTicketTimeInfo::_CalculationTime()
{
	const DWORD post_enterable_pre_close_interval = portal_close_timeout_c*60*1000;

	const DWORD cur_tick = GetTickCount();
	const DWORD elapsed = !check_start_tick ? ULONG_MAX : cur_tick - check_start_tick;

	( countdownTick > elapsed )
		?	countdownTick -= elapsed
		:	countdownTick = 0
		;
	check_start_tick = cur_tick;

	ZeroMemory( &systemTime, sizeof(systemTime) );
	if( !countdownTick )
	{
		tmMode = eTM_None;
		return;
	}

	DWORD pole_tick = countdownTick;
	if( post_enterable_pre_close_interval < countdownTick )
	{
		tmMode		= eTM_EnterableTimeout;
		pole_tick	-= post_enterable_pre_close_interval;
	}
	else
	{
		tmMode		= eTM_CloseTimeout;
	}


	const DWORD one_second	= 1;
	const DWORD one_minute	= 60*one_second;
	const DWORD one_hour	= 60*one_minute;

	DWORD calc_var = (DWORD)(pole_tick/1000);
	systemTime.wHour	= calc_var < one_hour	? 0 : WORD(calc_var/one_hour);
	calc_var %= one_hour;
	systemTime.wMinute	= calc_var < one_minute	? 0 : WORD(calc_var/one_minute);
	calc_var %= one_minute;
	systemTime.wSecond	= WORD(calc_var);
}


//───────────────────────────────────────────────────────────
//	condition	]	GetStatus() = { eTM_EnterableTimeout, eTM_CloseTimeout }
//	action		]	const SYSTEMTIME& countdown = GetTicketTimeInfo();
//				- used field = { countdown.wHour, countdown.wMinute, countdown.wSecond }
const SYSTEMTIME&
SSQTicketTimeInfo::GetTicketTimeInfo()
{
	switch( tmMode )
	{
	case eTM_Transaction:
		break;
	case eTM_EnterableTimeout:
	case eTM_CloseTimeout:
		_CalculationTime();
		break;
	case eTM_None:
	default:
		tmMode = eTM_None;
		ZeroMemory( &systemTime, sizeof(systemTime) );
		break;
	}
	return systemTime;
}


//───────────────────────────────────────────────────────────
//
//	서버로부터 받은 정보 설정.
VOID
SSQTicketTimeInfo::QueryResult( const SSQTicketInfo& rTicketInfo )
{
	static_cast<SSQTicketInfo&>(*this) = rTicketInfo;
	check_start_tick = GetTickCount();
}


//───────────────────────────────────────────────────────────
//

#ifdef _SERVER


//////////////////////////////////////////////////////////////////////////
const DWORD cTicketTimeRangeArrSize	= SSQInfo::MI_ENTER_TIME_SECTION_MAX + 1;
static SSQInfo::TimeRange sTicketTimeRange[cTicketTimeRangeArrSize] =
{
	0,
};
//////////////////////////////////////////////////////////////////////////


//───────────────────────────────────────────────────────────


BOOLEAN
SSQTicketTimeInfo::Update( const SYSTEMTIME* const pCurSysTime )	//	for Server
{
	const WORD acm24h = 24*60; // 스크립트 없으면? 죽는 거지 뭐...
	const SSQInfoParser::SSQINFO_TABLE& rTable = SSQInfoParser::Instance()->GetRefSSQInfo();
	if(rTable.empty())
		return FALSE;
	static const SSQInfo* const pSSQInfoPole = rTable.begin()->second;

	if(!sTicketTimeRange[cTicketTimeRangeArrSize-1].End)
	{	//calculation
		CopyMemory(&sTicketTimeRange, &pSSQInfoPole->m_TimeRange, sizeof(pSSQInfoPole->m_TimeRange));

		for(DWORD idx = 0; idx < SSQInfo::MI_ENTER_TIME_SECTION_MAX; ++idx)
		{
			SSQInfo::TimeRange& range = sTicketTimeRange[idx];
			_ConvertWzTmFmt2AcmTime(range);
            if(range.Start)
			    range.End = range.Start + portal_close_timeout_c;
		}

		{
			SSQInfo::TimeRange& range_s = sTicketTimeRange[0];
			SSQInfo::TimeRange& range_d = sTicketTimeRange[cTicketTimeRangeArrSize-1];
			range_d.Start	= range_s.Start + acm24h;
			range_d.End		= range_s.End + acm24h;
		}
	}

	const SSQInfo::WzTmFmt	check_time	= pCurSysTime->wHour * 60 + pCurSysTime->wMinute;

	DWORD find_index = 0;
	BOOLEAN pre_enterable = FALSE, post_enterable = FALSE;

	for( ; find_index < cTicketTimeRangeArrSize; ++find_index)
	{
		SSQInfo::TimeRange& range = sTicketTimeRange[find_index];
		pre_enterable	= check_time < range.Start;
		post_enterable	= !pre_enterable && (check_time < range.End);
		if( pre_enterable | post_enterable )
			break;
	}

	tmTableIndex = BYTE(find_index);
	tmMode = pre_enterable ? eTM_EnterableTimeout : eTM_CloseTimeout;
	//const SSQInfo::WzTmFmt countdownPole
	//	= pre_enterable
	//	?	sTicketTimeRange[find_index].Start
	//	:	sTicketTimeRange[find_index].End
	//	;
	const SSQInfo::WzTmFmt countdownPole //= sTicketTimeRange[find_index].End;
		= (sTicketTimeRange[find_index].End - check_time)
		?	(sTicketTimeRange[find_index].End - check_time) - 1
		:	0
		;

	countdownTick  = (DWORD)(60 - pCurSysTime->wSecond) * 1000;
	countdownTick += countdownPole * 60 * 1000;
	//countdownTick += (countdownPole-check_time)*60*1000;

	//printf( "Update info Idx[%u] Mod[%u] CountdownTick[%u]- \n", tmTableIndex, tmMode, countdownTick );

	return TRUE;
}
#endif //_SERVER

//───────────────────────────────────────────────────────────


}; // end namespace nsSSQ

//───────────────────────────────────────────────────────────


namespace nsSSQ { namespace Observer {

struct NextIndexes
{
	union {
		struct {
			BYTE	Q, W, E;
		};
		BYTE		Indexes[3];
	};
};

const Info::eCoordinates
Controller::GetNextIndex( const Info::eCoordinates current_idx, const Info::eKeyCmd inputed_cmd )
{
	static const NextIndexes	cKeyCmdTable[Info::eCoord_Slots] =
	{
		//	    	Q				W				E			   curPos
		{	Info::eCoord_1,	Info::eCoord_2,	Info::eCoord_3,	},	/*eCoord_0*/
		{	Info::eCoord_2,	Info::eCoord_3,	Info::eCoord_0,	},	/*eCoord_1*/
		{	Info::eCoord_3,	Info::eCoord_0,	Info::eCoord_1,	},	/*eCoord_2*/
		{	Info::eCoord_0,	Info::eCoord_1,	Info::eCoord_2,	},	/*eCoord_3*/
	};

	if(    ((DWORD)current_idx < (DWORD)Info::eCoord_Slots)
		&& ((DWORD)inputed_cmd < (DWORD)Info::eKeyCmd_Max)
		)
	{
		return (Info::eCoordinates)cKeyCmdTable[current_idx].Indexes[inputed_cmd];
	}

	return Info::eCoord_Slots;
};


const WzVector*
Controller::GetPosByIndex( const Info::eCoordinates inputed_idx )
{
	static const WzVector cObserverCoordinateTable[Info::eCoord_Slots] =
	{	//		X		Y		Z
		{	200.00f, 177.42f, 8.77f		},	/*eCoord_0*/
		{	176.96f, 199.87f, 8.77f		},	/*eCoord_1*/
		{	200.00f, 223.44f, 8.77f		},	/*eCoord_2*/
		{	222.10f, 199.89f, 8.77f		},	/*eCoord_3*/
	};

	return ((DWORD)inputed_idx < (DWORD)Info::eCoord_Slots)
		?	&cObserverCoordinateTable[inputed_idx]
		:	0
		;
};

} }; // end namespace nsSSQ::Observer


namespace nsSSQ { namespace Rules {
	typedef Observer::Info	Info;

const eC2G_ReqRule
GetC2G_RequestRule( const Info::eRequest request )
{
	static const C2G_RequestRule
	C2G_RequestRuleSet[Info::eRequest_Max] =
	{
		{ Info::eRequest_None			
			, eC2G_ReqRule
				( eC2G_ReqRule_BlockRequest
				)
		},
		{ Info::eRequest_Observer		
			, eC2G_ReqRule
				( eC2G_ReqRule_CondInSSQField
				| eC2G_ReqRule_CondObserverModeOff
				| eC2G_ReqRule_CondDeadState
				)
		},
		{ Info::eRequest_Leave		
			, eC2G_ReqRule
				( eC2G_ReqRule_CondInSSQField
				| eC2G_ReqRule_CondDeadState
				)
		},
		{ Info::eRequest_ChangeCoord	
			, eC2G_ReqRule
				( eC2G_ReqRule_CondInSSQField
				| eC2G_ReqRule_CondObserverModeOn
				| eC2G_ReqRule_CondDeadState
				)
		},
	};

	//////////////////////////////////////////////////////////////////////////
	const eC2G_ReqRule ruleset
		= (DWORD)request < (DWORD)Info::eRequest_Max
		?	C2G_RequestRuleSet[request].RuleSet
		:	C2G_RequestRuleSet[Info::eRequest_None].RuleSet
		;

	return ruleset;
}

}}; //end namespace nsSSQ::Rules

//───────────────────────────────────────────────────────────


