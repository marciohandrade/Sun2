#pragma once

#include <ITimeout.h>
#include <StructInPacket.h>
#include "../AreaTileLoadHelper.h"

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	<SSQInfoStruct.h>
//	<Decriptions>
//	-	顫塊и 餌錳 SSQ(Survival Soloing Quest)
//	<Lists>
//	-	SSQInfo
//	-	SSQPattern
//

//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式
//	<DEFINITIONs>
const DWORD										SSQ_MAX_CHARACTERS	= 100;
const DWORD										SSQ_MAX_PLAYERS		=  70;
const DWORD										SSQ_NAME_LEN		=  31;
typedef util::cSTRING_BUFFER<SSQ_NAME_LEN+1>	SSQ_NAME;

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	<SSQInfo>
//
struct SSQInfo
{
	//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式
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

	static const WzTmFmt						TIME_NOTICE_START				= 5;	// 蝶觼董お鼻 衛濛 衛除 薑爾縑憮 5碟 瞪
	static const SECOND_T						TIME_BATTLE_START_COUNTDOWN		= 10;	// 10蟾 瞪

	typedef WORD								COUNT_T;
	enum ePLAYER_COUNT_LIMIT { PCL_MIN, PCL_MAX, PCL_RANGE, };
	typedef COUNT_T								PLAYER_COUNT_LIMIT[PCL_RANGE];
	typedef BYTE								RATIO_T;
	typedef WORD								RATIO_SUM_T;
	typedef BYTE								PAD_DUMMY_T;

	// ⑷營 健綰 婦溼 薑爾朝 ж釭...
	struct GAMBLE_INFO
	{
		RATIO_T					m_ChargeRatio;
	};

	struct EARTHQUAKE_NODE
	{
		MINUTE_T				m_ElapsedTime;	// 雖翱 衛除
		COUNT_T					m_RemainPlayers;// 陴擎 檣錳
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
		SECOND_T				m_RaisePerXsec;						// x蟾渡 1廓 嫦儅
	};

	struct CLOSED_SPACE_NODE
	{
		//DAMAGETYPE				m_PlayerDamage;
		//DAMAGETYPE				m_NpcDamage;
		FLOAT					m_PlayerDamages[MI_EARTHQUAKE_SECTION_MAX];	// damage = maxHP*(ratio/100)
		FLOAT					m_NpcDamages[MI_EARTHQUAKE_SECTION_MAX];	// damage = maxHP*(ratio/100)
		SECOND_T				m_RaisePerXsec;						// x蟾渡 1廓 嫦儅 (⑷營 THUNDERBOLT諦 翕橾ж雖虜 離�� �挫撋� 熱 氈戲嘎煎 評煎 菩棻.
	};

	struct SUMMONED_NPC_NODE
	{
		MONSTERCODE				m_NPCCode;
		RATIO_SUM_T				m_SumRatio;							// 檜瞪 匙婁 綠掖и 隸碟 �捕�
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

	//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式
	//	<FIELDs>
	//
	INDEX_T							m_Index;					// 檣策蝶
	SSQCODE							m_MapCode;					// 殮濰 裘囀萄
	LEVEL_RANGE						m_LevelRange;				// 溯漣 (譆模, 譆渠)
	MONEY							m_Cost;						// 殮濰猿
	TIME_RANGE						m_TimeRange;				// 殮濰衛除 1,2,...
	MINUTE_T						m_WatingTime;				// 瞪癱渠晦衛除
	MINUTE_T						m_QuestPlayTime;			// 瞪羹Ы溯檜衛除
	PLAYER_COUNT_LIMIT				m_PlayerCountInfo;			// 殮濰檣錳 薯и高
	GAMBLE_INFO						m_GamebleInfo;				// 健綰 薑爾
	EARTHQUAKE_INFO					m_EarthquakeInfo;			// 雖霞褻勒 1,2,...
	THUNDERBOLT_NODE				m_ThunderboltInfo;			// 釩煆 薑爾
	CLOSED_SPACE_NODE				m_ClosedSpaceInfo;			// 唸啗 薑爾
	SUMMONDED_NPC_INFO				m_SummonedNpcInfo;			// Ы溯檜橫羲й 熱чж朝 跨蝶攪 薑爾
	PLAYER_HUNTING_INFO				m_PlayerHuntingInfo;		// Player vs. Player滌 餌傖 ん檣お 鼻熱高
	NPC_HUNTING_INFO				m_NpcHuntingInfo;			// Player vs. Monster滌 餌傖 ん檣お 鼻熱高
	EXP_RATIO_INFO					m_ExpRatioInfo;				// 議葛攪滌 餌傖 ん檣お 寡徽
	SUCCESS_POINT_INFO				m_SuccessPointInfo;			// 蠡蝶お 撩奢 ん檣お (1蛔, 2蛔, 3蛔...)
    REWARD_NODE                     m_DefaultRewardInfo;        // 霤陛濠 晦獄 爾鼻 薑爾
	REWARD_DROP_INFO				m_RewardDropInfo;			// 爾鼻 橾奩 薑爾
	REWARD_WINNER_CHAR_INFO			m_RewardWinnerInfo;			// 譆謙 蝓葬濠 爾鼻 薑爾
	//REWARD_WINNER_INFO				m_RewardWinnerInfo;			// 譆謙 蝓葬濠 爾鼻 薑爾
	RESTRICT_SKILL_GROUP_INFO		m_RestrictSkillGroupInfo;	// 餌辨檜 薯и腎朝 蝶鑒 斜瑜
	RESTRICT_ITEM_TYPE_INFO			m_RestrictItemTypeInfo;		// 餌辨檜 薯и腎朝 嬴檜蠱 顫殮

	//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式
	//	衛除 啗骯 ら曖蒂 嬪и в萄s <- 蝶觼董お縑朝 薑曖腎雖 彊擠.
	TIME_RANGE						m_CTimeNotice;				// 奢雖 衛濛 衛除 : 1碟 欽嬪
	TIME_RANGE						m_CTimeWaiting;				// 瞪癱 渠晦 衛除
	TIME_RANGE						m_CTimeBattle;				// 瞪癱 衛除

	SSQ_NAME						m_SSQName;					// 殮濰 檜葷, ExtraInfo (for Debug)

	//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式
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


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
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





//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	<SSQPattern>
//
struct SSQPattern
{
	//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式
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

	//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式
	//	<FIELDs>
	//
	INDEX_T										m_Index;		// п渡 ぬ欐 檣策蝶
	STEP_T										m_BanLiftStep;	// 唸啗 薯剪 欽啗 (欽啗 熱)
	STEP_GROUP									m_StepGroup;	// 偃滌 欽啗 + 螃粽薛お 薑爾
	//	檜葷擎... в蹂 橈擊 蛭... 氈戲賊 斜陽 蹺陛ж晦煎 и棻.

};	// end SSQPattern


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收


namespace nsSSQ {

class SSQTicketTimeInfo : public SSQTicketInfo
{
private:	DWORD				check_start_tick;
			SYSTEMTIME			systemTime;		// 贗塭 蘋遴お棻遴 薑爾 橢晦
public:		SSQTicketTimeInfo() { Init(); }
			VOID				Init();
	//	憮幗煎曖 薑爾 蹂羶 в蹂и雖 罹睡 (eTM_None橾 唳辦虜)
	inline	eTM					GetStatus() const	{ return (eTM)tmMode; }
	//	憮幗煎曖 薑爾 蹂羶 醞戲煎 撲薑.
	inline	VOID				SetStatus_Queried()	{ tmMode = eTM_Transaction; }

			//	condition	]	GetStatus() = { eTM_EnterableTimeout, eTM_CloseTimeout }
			//	action		]	const SYSTEMTIME& countdown = GetTicketTimeInfo();
			//				- used field = { countdown.wHour, countdown.wMinute, countdown.wSecond }
	const	SYSTEMTIME&			GetTicketTimeInfo();
			//	憮幗煎睡攪 嫡擎 薑爾 撲薑.
			VOID				QueryResult( const SSQTicketInfo& rTicketInfo );

		#ifdef _SERVER
			BOOLEAN				Update( const SYSTEMTIME* const pCurSysTime );	//	for Server
	inline const SSQTicketInfo&	GetTicketInfo() const { return *this; }
		#endif
private:
	inline	VOID				_CalculationTime();
};

}; // end namespace nsSSQ


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收


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
	//		true : п渡 index縑 翱唸脹 謝ル
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


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收


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


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//
#include "SSQInfoStruct.hxx"




