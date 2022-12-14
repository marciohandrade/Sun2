#pragma once

#include <ITimeout.h>
#include <StructInPacket.h>
#include "../AreaTileLoadHelper.h"

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	<SSQInfoStruct.h>
//	<Decriptions>
//	-	타락한 사원 SSQ(Survival Soloing Quest)
//	<Lists>
//	-	SSQInfo
//	-	SSQPattern
//

//─────────────────────────────────────────
//	<DEFINITIONs>
const DWORD										SSQ_MAX_CHARACTERS	= 100;
const DWORD										SSQ_MAX_PLAYERS		=  70;
const DWORD										SSQ_NAME_LEN		=  31;
typedef util::cSTRING_BUFFER<SSQ_NAME_LEN+1>	SSQ_NAME;

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	<SSQInfo>
//
struct SSQInfo
{
	//─────────────────────────────────────────
	//	<DEFINITIONs>
//#pragma pack(push, 1)
	typedef WORD								INDEX_T;
	typedef MAPCODE								SSQCODE;
	enum eLEVEL { LV_MIN, LV_MAX, LV_RANGE };
	typedef LEVELTYPE							LEVEL_RANGE[LV_RANGE];

	enum eMAX_INFO
	{
		MI_ENTER_TIME_SECTION_MAX				=  4,
		MI_EARTHQUAKE_SECTION_MAX				=  4,
		MI_SUMMONED_NPC_SECTION_MAX				= 16,
		MI_SUCCESS_POINT_BY_GRADE				=  3,
		MI_REWARD_DROP_INFO_SECTION_MAX			=  5,
		MI_REWARD_WINNER_INFO_SECTION_MAX		=  3,
		MI_RESTRICT_SKILL_SECTION_MAX			= 20,
		MI_RESTRICT_ITEM_SECTION_MAX			= 15,
	};

	typedef WORD								WzTmFmt;	// 17:27 --> { 17*100 + 27 }
	struct TimeRange
	{
		WzTmFmt	Start, End;
		inline bool		OnTime( const WzTmFmt& tm ) const
		{
			return Start &&
                   Start <= tm && tm <= End;
		}
	};
	typedef TimeRange							TIME_RANGE[MI_ENTER_TIME_SECTION_MAX];

	typedef BYTE								MINUTE_T;
	typedef BYTE								SECOND_T;

	static const WzTmFmt						TIME_NOTICE_START				= 5;	// 스크립트상 시작 시간 정보에서 5분 전
	static const SECOND_T						TIME_BATTLE_START_COUNTDOWN		= 10;	// 10초 전

	typedef WORD								COUNT_T;
	enum ePLAYER_COUNT_LIMIT { PCL_MIN, PCL_MAX, PCL_RANGE, };
	typedef COUNT_T								PLAYER_COUNT_LIMIT[PCL_RANGE];
	typedef BYTE								RATIO_T;
	typedef WORD								RATIO_SUM_T;
	typedef BYTE								PAD_DUMMY_T;

	// 현재 갬블 관련 정보는 하나...
	struct GAMBLE_INFO
	{
		RATIO_T					m_ChargeRatio;
	};

	struct EARTHQUAKE_NODE
	{
		MINUTE_T				m_ElapsedTime;	// 지연 시간
		COUNT_T					m_RemainPlayers;// 남은 인원
		inline bool				OnSatisfiedCondition( const EARTHQUAKE_NODE& rQuery ) const
		{
			return (rQuery.m_RemainPlayers<m_RemainPlayers) || (m_ElapsedTime<=rQuery.m_ElapsedTime);
		}
	};
	typedef EARTHQUAKE_NODE						EARTHQUAKE_INFO[MI_EARTHQUAKE_SECTION_MAX];

	struct THUNDERBOLT_NODE
	{
		DAMAGETYPE				m_PlayerDamage;
		DAMAGETYPE				m_NpcDamage;
		SECOND_T				m_RaisePerXsec;						// x초당 1번 발생
	};

	struct CLOSED_SPACE_NODE
	{
		//DAMAGETYPE				m_PlayerDamage;
		//DAMAGETYPE				m_NpcDamage;
		FLOAT					m_PlayerDamages[MI_EARTHQUAKE_SECTION_MAX];	// damage = maxHP*(ratio/100)
		FLOAT					m_NpcDamages[MI_EARTHQUAKE_SECTION_MAX];	// damage = maxHP*(ratio/100)
		SECOND_T				m_RaisePerXsec;						// x초당 1번 발생 (현재 THUNDERBOLT와 동일하지만 차후 확장될 수 있으므로 따로 둔다.
	};

	struct SUMMONED_NPC_NODE
	{
		MONSTERCODE				m_NPCCode;
		RATIO_SUM_T				m_SumRatio;							// 이전 것과 비교한 증분 확률
	};
	struct SUMMONDED_NPC_INFO
	{
		RATIO_SUM_T				MaxRegenValue;
		COUNT_T					Count;
		SUMMONED_NPC_NODE		List[MI_SUMMONED_NPC_SECTION_MAX];
	};

	struct HUNTING_POINT_NODE
	{
		RATIO_T					m_HuntingPoint;
	};

	typedef HUNTING_POINT_NODE					PLAYER_HUNTING_INFO[eCHAR_TYPE_MAX];
	typedef HUNTING_POINT_NODE					NPC_HUNTING_INFO[eCHAR_TYPE_MAX];

	struct FLOAT_POINT_NODE
	{
		FLOAT					m_FloatPoint;
	};
	typedef FLOAT_POINT_NODE					EXP_RATIO_INFO[eCHAR_TYPE_MAX];
	typedef FLOAT_POINT_NODE					SUCCESS_POINT_INFO[MI_SUCCESS_POINT_BY_GRADE];

	typedef SLOTCODE							REWARD_CODE_T;
	//typedef REWARD_CODE_T						REWARD_NORMAL_CODE_INFO[MI_REWARD_DROP_INFO_SECTION_MAX];
	//typedef COUNT_T								REWARD_NORMAL_COUNT_INFO[MI_REWARD_NORMAL_INFO_SECTION_MAX];
	//typedef REWARD_CODE_T						REWARD_WINNER_CODE_INFO[MI_REWARD_WINNER_INFO_SECTION_MAX];
	//typedef COUNT_T								REWARD_WINNER_COUNT_INFO[MI_REWARD_WINNER_INFO_SECTION_MAX];

	struct REWARD_NODE
	{
		REWARD_CODE_T			m_RewardCode;
		COUNT_T					m_RewardLimit;
	};

	struct REWARD_DROP_INFO
	{
		COUNT_T					Count;
		REWARD_NODE				List[MI_REWARD_DROP_INFO_SECTION_MAX];
	};

	typedef REWARD_NODE							CHAR_REWARD_NODE[eCHAR_TYPE_MAX];
	typedef CHAR_REWARD_NODE					REWARD_WINNER_CHAR_INFO[MI_REWARD_WINNER_INFO_SECTION_MAX];

	//struct REWARD_WINNER_INFO
	//{
	//	COUNT_T					Count;
	//	REWARD_NODE				List[MI_REWARD_WINNER_INFO_SECTION_MAX];
	//};

	typedef WORD								SKILL_GROUPCODE;

	struct RESTRICT_SKILL_GROUP_INFO
	{
		COUNT_T					Count;
		SKILL_GROUPCODE			SkillGroupCodes[MI_RESTRICT_SKILL_SECTION_MAX];
		inline	BOOLEAN			IsPassed( const SKILL_GROUPCODE skill_group ) const;
	};

	struct RESTRICT_ITEM_TYPE_INFO
	{
		COUNT_T					Count;
		WORD					ItemTypes[MI_RESTRICT_ITEM_SECTION_MAX];
		inline	BOOLEAN			IsPassed( const WORD item_type ) const;
	};
//#pragma pack(pop)

	//─────────────────────────────────────────
	//	<FIELDs>
	//
	INDEX_T							m_Index;					// 인덱스
	SSQCODE							m_MapCode;					// 입장 맵코드
	LEVEL_RANGE						m_LevelRange;				// 레벨 (최소, 최대)
	MONEY							m_Cost;						// 입장료
	TIME_RANGE						m_TimeRange;				// 입장시간 1,2,...
	MINUTE_T						m_WatingTime;				// 전투대기시간
	MINUTE_T						m_QuestPlayTime;			// 전체플레이시간
	PLAYER_COUNT_LIMIT				m_PlayerCountInfo;			// 입장인원 제한값
	GAMBLE_INFO						m_GamebleInfo;				// 갬블 정보
	EARTHQUAKE_INFO					m_EarthquakeInfo;			// 지진조건 1,2,...
	THUNDERBOLT_NODE				m_ThunderboltInfo;			// 낙뢰 정보
	CLOSED_SPACE_NODE				m_ClosedSpaceInfo;			// 결계 정보
	SUMMONDED_NPC_INFO				m_SummonedNpcInfo;			// 플레이어역할 수행하는 몬스터 정보
	PLAYER_HUNTING_INFO				m_PlayerHuntingInfo;		// Player vs. Player별 사냥 포인트 상수값
	NPC_HUNTING_INFO				m_NpcHuntingInfo;			// Player vs. Monster별 사냥 포인트 상수값
	EXP_RATIO_INFO					m_ExpRatioInfo;				// 캐릭터별 사냥 포인트 배율
	SUCCESS_POINT_INFO				m_SuccessPointInfo;			// 퀘스트 성공 포인트 (1등, 2등, 3등...)
    REWARD_NODE                     m_DefaultRewardInfo;        // 참가자 기본 보상 정보
	REWARD_DROP_INFO				m_RewardDropInfo;			// 보상 일반 정보
	REWARD_WINNER_CHAR_INFO			m_RewardWinnerInfo;			// 최종 승리자 보상 정보
	//REWARD_WINNER_INFO				m_RewardWinnerInfo;			// 최종 승리자 보상 정보
	RESTRICT_SKILL_GROUP_INFO		m_RestrictSkillGroupInfo;	// 사용이 제한되는 스킬 그룹
	RESTRICT_ITEM_TYPE_INFO			m_RestrictItemTypeInfo;		// 사용이 제한되는 아이템 타입

	//─────────────────────────────────────────
	//	시간 계산 편의를 위한 필드s <- 스크립트에는 정의되지 않음.
	TIME_RANGE						m_CTimeNotice;				// 공지 시작 시간 : 1분 단위
	TIME_RANGE						m_CTimeWaiting;				// 전투 대기 시간
	TIME_RANGE						m_CTimeBattle;				// 전투 시간

	SSQ_NAME						m_SSQName;					// 입장 이름, ExtraInfo (for Debug)

	//─────────────────────────────────────────
	//	<INTERFACEs>
public:
	inline	bool			IsAcceptLevel( const LEVELTYPE level ) const
			{ return (m_LevelRange[LV_MIN]<=level) && (level<=m_LevelRange[LV_MAX]); }
public:		bool			IsSSQOpenTime( const SYSTEMTIME* IN pSysTime=NULL, DWORD* OUT pIndex=NULL ) const;
			bool			IsNoticeTime( const SYSTEMTIME* IN pSysTime=NULL, DWORD* OUT pIndex=NULL ) const;
			bool			IsWaitingTime( const SYSTEMTIME* IN pSysTime=NULL, DWORD* OUT pIndex=NULL ) const;
			bool			IsBattleTime( const SYSTEMTIME* IN pSysTime=NULL, DWORD* OUT pIndex=NULL ) const;
public:
	inline	bool			IsValidCountForEnter( const COUNT_T curplayercnt ) const	// for SERVER
			{ return (curplayercnt<=m_PlayerCountInfo[PCL_MAX]); }
	inline	bool			IsValidCountForStart( const COUNT_T curplayercnt ) const	// for SERVER
			{ return (m_PlayerCountInfo[PCL_MIN]<=curplayercnt) && (curplayercnt<=m_PlayerCountInfo[PCL_MAX]); }
	inline	COUNT_T			GetSummonMobCount( const COUNT_T curplayercnt ) const	// for SERVER
			{ return SSQ_MAX_CHARACTERS-curplayercnt ; }
public:		bool			IsSatisfiedEarthquakeCondition( const MINUTE_T elapsedTm, const COUNT_T curplayercnt ) const;	//for SERVER
			MONSTERCODE		GetRandomRegenMonster() const;

private:
	inline const TimeRange* const	_FindMatchTimeRange(
								const TimeRange* const pBegin
								, const TimeRange* const pEnd
								, const WzTmFmt calcTime
								, DWORD* OUT pIndex
								) const;
};	// end SSQInfo


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
inline BOOLEAN
SSQInfo::RESTRICT_SKILL_GROUP_INFO::IsPassed( const SSQInfo::SKILL_GROUPCODE skill_group ) const
{
	const SKILL_GROUPCODE* pIt = &SkillGroupCodes[0];
	const SKILL_GROUPCODE* const pItEnd = &SkillGroupCodes[Count];
	for( ; pIt!=pItEnd ; ++pIt )
		if( *pIt == skill_group )
			return FALSE;
	return TRUE;
}

inline BOOLEAN
SSQInfo::RESTRICT_ITEM_TYPE_INFO::IsPassed( const WORD item_type ) const
{
	const WORD* pIt = &ItemTypes[0];
	const WORD* const pItEnd = &ItemTypes[Count];
	for( ; pIt!=pItEnd ; ++pIt )
		if( *pIt == item_type )
			return FALSE;
	return TRUE;
}





//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	<SSQPattern>
//
struct SSQPattern
{
	//─────────────────────────────────────────
	//	<DEFINITIONs>
//#pragma pack(push, 1)
	typedef WORD								INDEX_T;
	typedef WORD								STEP_T;
	typedef INT									OBJECT_ID_T;

	enum eMAX_INFO
	{
		MI_OBJECT_COUNT_MAX						= 25,
		MI_STEP_MAX								=  4,
	};

	struct sSINGLE_STEP
	{
		STEP_T				m_Step;
		WORD				m_ListSize;
		OBJECT_ID_T			m_List[MI_OBJECT_COUNT_MAX];
	};

	typedef sSINGLE_STEP						STEP_GROUP[MI_STEP_MAX];
//#pragma pack(pop)

	//─────────────────────────────────────────
	//	<FIELDs>
	//
	INDEX_T										m_Index;		// 해당 패턴 인덱스
	STEP_T										m_BanLiftStep;	// 결계 제거 단계 (단계 수)
	STEP_GROUP									m_StepGroup;	// 개별 단계 + 오브젝트 정보
	//	이름은... 필요 없을 듯... 있으면 그때 추가하기로 한다.

};	// end SSQPattern


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━


namespace nsSSQ {

class SSQTicketTimeInfo : public SSQTicketInfo
{
private:	DWORD				check_start_tick;
			SYSTEMTIME			systemTime;		// 클라 카운트다운 정보 얻기
public:		SSQTicketTimeInfo() { Init(); }
			VOID				Init();
	//	서버로의 정보 요청 필요한지 여부 (eTM_None일 경우만)
	inline	eTM					GetStatus() const	{ return (eTM)tmMode; }
	//	서버로의 정보 요청 중으로 설정.
	inline	VOID				SetStatus_Queried()	{ tmMode = eTM_Transaction; }

			//	condition	]	GetStatus() = { eTM_EnterableTimeout, eTM_CloseTimeout }
			//	action		]	const SYSTEMTIME& countdown = GetTicketTimeInfo();
			//				- used field = { countdown.wHour, countdown.wMinute, countdown.wSecond }
	const	SYSTEMTIME&			GetTicketTimeInfo();
			//	서버로부터 받은 정보 설정.
			VOID				QueryResult( const SSQTicketInfo& rTicketInfo );

		#ifdef _SERVER
			BOOLEAN				Update( const SYSTEMTIME* const pCurSysTime );	//	for Server
	inline const SSQTicketInfo&	GetTicketInfo() const { return *this; }
		#endif
private:
	inline	VOID				_CalculationTime();
};

}; // end namespace nsSSQ


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━


namespace nsSSQ { namespace Observer {

struct Info
{
	enum eRequest {
		eRequest_None			= 0,
		eRequest_Observer		,		// not Observer
		eRequest_Leave			,		// always
		eRequest_ChangeCoord	,		// in ObserverMode
		eRequest_Max
	};

	enum eKeyCmd {
		eKeyCmd_Q				= 0,
		eKeyCmd_W				= 1,
		eKeyCmd_E				= 2,
		eKeyCmd_Max				= 3,
	};

	enum eCoordinates {
		eCoord_0				= 0,
		eCoord_1				= 1,
		eCoord_2				= 2,
		eCoord_3				= 3,
		eCoord_Slots			= 4,
	};

	enum eInfo {
		eInfo_Delay_NextKeyCmd	= 3,	// 3sec
		eInfo_Delay_QuestLeave	= 5,
	};
};

struct Controller
{
	//	<returns>
	//		true : { x | x<eCoord_Slots }
	//		false: eCoord_Slots
	static const Info::eCoordinates	GetNextIndex( const Info::eCoordinates current_idx, const Info::eKeyCmd inputed_cmd );
	//	<returns>
	//		true : 해당 index에 연결된 좌표
	//		false: NULL
	static const WzVector*			GetPosByIndex( const Info::eCoordinates inputed_idx );

	//	for Client
	//	C->G Packet Structure
};

}}; // end namespace nsSSQ::Observer


namespace nsSSQ { namespace Rules {

typedef Observer::Info	Info;

enum eC2G_ReqRule {
	eC2G_ReqRule_None						= 0,
	eC2G_ReqRule_BlockRequest				= 1<< 0,
	eC2G_ReqRule_CondInSSQField				= 1<< 2,
	eC2G_ReqRule_CondInFieldServer			= 1<< 3,
	eC2G_ReqRule_CondObserverModeOn			= 1<< 5,
	eC2G_ReqRule_CondObserverModeOff		= 1<< 6,
	eC2G_ReqRule_CondDeadState				= 1<< 7,
};

struct C2G_RequestRule
{
	Info::eRequest		ReqType;
	eC2G_ReqRule		RuleSet;
};

const eC2G_ReqRule		GetC2G_RequestRule( const Info::eRequest request );

}}; //end namespace nsSSQ::Rules


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━


namespace nsSSQ
{
	inline VOID		_ConvertWzTmFmt2AcmTime( SSQInfo::TimeRange& INOUT rValue )
	{
		const WORD bound	=100;
		const WORD hour		= 60;
		SSQInfo::WzTmFmt	x;

		// = (x / B) * H + (x % B)
		
		x = rValue.Start;
		x = (SSQInfo::WzTmFmt) ( ((x / bound) * hour) + (x % bound) );
		rValue.Start = x;

		x = rValue.End;
		x = (SSQInfo::WzTmFmt) ( ((x / bound) * hour) + (x % bound) );
		rValue.End = x;
	}

	inline VOID		_ConvertAcmTime2WzTmFmt( SSQInfo::TimeRange& INOUT rValue )
	{
		const WORD bound	=100;
		const WORD hour		= 60;
		SSQInfo::WzTmFmt	x;

		// = ( (x / H) * B ) + ( x % H )

		x = rValue.Start;
		x = (SSQInfo::WzTmFmt) ( ( (x / hour) * bound ) + ( x % hour ) );
		rValue.Start = x;

		x = rValue.End;
		x = (SSQInfo::WzTmFmt) ( ( (x / hour) * bound ) + ( x % hour ) );
		rValue.End = x;
	}
};


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//
#include "SSQInfoStruct.hxx"




