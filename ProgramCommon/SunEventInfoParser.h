#pragma once

#include <Singleton.h>
#include <SolarFileReader.h>
#include <map>
#include <time.h>
#include <ScriptCode.IParser.h>
#include <ResultCode.h>

#ifdef _NA_007919_20141211_ADD_GMCOMMAND_EVENT
#include "GameOptionParser.h"
#endif //_NA_007919_20141211_ADD_GMCOMMAND_EVENT

// 이벤트 종류
enum eEVENT_MODE
{
	eEVENT_CREATECHAR,			// 캐릭터 생성시 특정 아이템 지급
	eEVENT_REGWEBITEM,			// 웹으로 아이템 등록시 경품 지급
	eEVENT_REGITEM_GETITEM,		// 아이템 등록하면 특정 아이템 지급
	eEVENT_REGITEM_RANDREWARD,	// 아이템 등록하면 랜덤하게 보상 지급
	eEVENT_CRAFTITEM,			// 아이템 제조 이벤트
	eEVENT_ENCHANTITEM,			// 아이템 인챈트 이벤트
	eEVENT_DECOMPITEM,			// 아이템 분해 이벤트
	eEVENT_PEAKTIME,			// 피크타임 이벤트
	eEVENT_COUPON,				// 게임 내 쿠폰 이벤트
	eEVENT_PCBANG_PEAKTIME,		// PC방 피크타임 이벤트
	eEVENT_DONATION_HEIM,		// 하임 등록 이벤트
	eEVENT_ITEM_SELECT,			// 아이템 선택 이벤트 (등록 없이 npc 선택만으로 이뤄지는 이벤트)
	eEVENT_BUFF_GRANT,			// 버프 부여 이벤트 (이벤트 ID 요청 없이 NPC 선택만으로 이뤄지는 이벤트)
#ifdef _NA_20081121_ABUMAN_NIJOT_EVENT
	eEVENT_GUILD_MISSION_EVENT = 13, // 미션 완료시 미션 진행이 모두 길드원이면 경험치 더 주는 이벤트.
#endif
    //_KR_0_20091021_FIRENDINVITATION_EXTEND
	eEVENT_FRIENDINVATION_EVENT = 14, // 친구초대후 같은 파티로 게임을 하면 추가 경험치를 주는 이벤트
#ifdef _NA_007992_20150127_EVENT_MONSTER_INVASION
    eEVENT_MONSTER_INVASION = 15,   // 몬스터침공 이벤트
#endif //_NA_007992_20150127_EVENT_MONSTER_INVASION
#ifdef _NA_008404_20150712_MINIGAME_SYSTEM
    eEVENT_MINIGAME_BINGO       = 16, // 빙고 이벤트
#endif // _NA_008404_20150712_MINIGAME_SYSTEM
    //
	eEVENT_MAX,					// 이벤트용 ID 최대값

	//	__NA001094_080904_TASK_SSQ_REWARD_PROCESS__
	//	T_EventUser DB Table 다목적으로 사용하기 위한 예약코드
	//	ref type)	eEVENT_MODE_MULTIUSE
	//	range)		eEVENT_MULTIUSE_START(10000) ~ 10002
};


enum eEVENT_DAY_FLAG
{
	eEVENT_DAY_FLAG_ALLDAY		= (0),			//< 0
	eEVENT_DAY_FLAG_SUNDAY		= (1<<0),		//< 1
	eEVENT_DAY_FLAG_MONDAY		= (1<<1),		//< 2
	eEVENT_DAY_FLAG_TUESDAY		= (1<<2),		//< 4
	eEVENT_DAY_FLAG_WEDNESDAY	= (1<<3),		//< 8
	eEVENT_DAY_FLAG_THURSDAY	= (1<<4),		//< 16
	eEVENT_DAY_FLAG_FRYDAY		= (1<<5),		//< 32
	eEVENT_DAY_FLAG_SATURDAY	= (1<<6),		//< 64
};

//#define _WHATTHEHELL_SUNEVENT_20060727	// 썬 이벤트 스크립트에서 지나간 이벤트는 읽지 않도록 수정중


// 각 이벤트 정보 저장용
struct EVENT_INFO
{
	CODETYPE		m_dwEventID;
    //npc정보를 사용하지 않는 몬스터침공(mode==15)에서는 npc코드를 mapcode로 사용한다
	CODETYPE		m_NpcCode;
	BYTE			m_byEventMode;

	__time64_t		m_start_time;
	__time64_t		m_end_time;
	WORD			m_wEventDayFlag;

	BYTE			m_byCountMax;

	enum eAPPLY_LEVEL { LEVEL_MIN = 0, LEVEL_MAX, LEVEL_RANGE, };
	WORD			m_byApplyToLvRng[LEVEL_RANGE];

	CODETYPE		m_dwRegisterItemIndex[_MAX_EVENT_REGITEM];
	WORD			m_wRegisterItemNum[_MAX_EVENT_REGITEM];

	WORD			m_wPeakTimeStart;
	WORD			m_wPeakTimeEnd;

	CODETYPE		m_dwItemCraftIndex;
	CODETYPE		m_dwItemEnchantIndex;
	CODETYPE		m_dwItemDecompoIndex;
	float			m_fRewardExpRate;		// 피크타임 경험치 증가율
	float			m_fRewardExpRate2;		// 신규캐릭 경험치 증가율
	float			m_fRewardHimeRate;
	float			m_fRewardItemDropRate;
	CODETYPE		m_dwRewardInfo[_MAX_EVENT_REWARD];	// 이름 변경, (기존) 보상 코드 -> (변경) 보상 정보_보상코드+기타(이것은 이벤트 모드에 따라 구분)
	CODETYPE		m_dwPCRoomRewardInfo[_MAX_EVENT_REWARD];
	CODETYPE		m_EventRewardCode;
	BYTE			m_IsDeleteFlagForRewardedItem;
	enum { _MAX_DELETE_ITEM_CODES = 5, };
	SLOTCODE		m_DeleteRewaredItemCode[_MAX_DELETE_ITEM_CODES]; 

	enum { _MAX_EXPRATE_NUM = 3 };
	float			m_fPlayTimeExpRate[_MAX_EXPRATE_NUM];	// (071120) 현재 사용되지 않는다.

#ifdef _NA_007919_20141211_ADD_GMCOMMAND_EVENT
    bool is_gm_event_;      // 스크립트에서 읽은 이벤트면 false / GM이 만든 이벤트이면 true
    bool is_pre_notify_;    // 이벤트 시작 "5분전 알림" 실행여부 (false: 아직 알리지않음)

    // GM이벤트의 정보를 세팅한다.
    // EventID는 별도로 세팅할 것.
    BOOL SetGMEventWithoutID(eEVENT_MODE event_mode, int start_date, WORD start_time, int end_date, 
                             WORD end_time, int reward_ratio);
#endif //_NA_007919_20141211_ADD_GMCOMMAND_EVENT

    eEVENT_DAY_FLAG GetDayFlag(int tm_wday);
    BOOL CheckEvent(time_t curtime);
    BOOL CheckEvent();
    BOOL CheckLevel(const LEVELTYPE playerLevel);
};

inline eEVENT_DAY_FLAG EVENT_INFO::GetDayFlag(int tm_wday)
{
    switch(tm_wday)
    {
    case 0: return eEVENT_DAY_FLAG_SUNDAY;
    case 1: return eEVENT_DAY_FLAG_MONDAY;
    case 2: return eEVENT_DAY_FLAG_TUESDAY;
    case 3: return eEVENT_DAY_FLAG_WEDNESDAY;
    case 4: return eEVENT_DAY_FLAG_THURSDAY;
    case 5: return eEVENT_DAY_FLAG_FRYDAY;
    case 6: return eEVENT_DAY_FLAG_SATURDAY;
    }
    return eEVENT_DAY_FLAG_SUNDAY;
}

inline BOOL EVENT_INFO::CheckEvent()
{
    time_t cur_time;
    util::TimeSync::time(&cur_time);

    return CheckEvent(cur_time);
}

inline BOOL EVENT_INFO::CheckLevel(const LEVELTYPE playerLevel)
{
    return (m_byApplyToLvRng[LEVEL_MIN] <= playerLevel) &&
                                          (playerLevel <= m_byApplyToLvRng[LEVEL_MAX]);
}
//==================================================================================================

struct EVENT_SUB_INFO
{
	CODETYPE		m_dwEventID;
	BYTE			m_byEventMode;
	BYTE			m_byCountMax;
	CODETYPE		m_dwRegisterItemIndex[5];
	WORD			m_wRegisterItemNum[5];
};


//	<EventRewardInfo>
//		구간설정 형식의 스크립트

//struct EVENT_MINMAX_TYPE_INFO
//{
//	enum { _MAX_GROUPNODE_COUNT = 15, };
//	typedef DWORD			INDEX_T;
//	typedef DWORD			ROUND_T;
//	typedef DWORD			HEIM_BOUND_T;
//	typedef HEIM_BOUND_T	MinMaxArray[_MAX_GROUPNODE_COUNT];
//	typedef MinMaxArray		DropCodeArray;
//	typedef MinMaxArray		CountLimitArray;
//
//	INDEX_T			m_Index;
//	ROUND_T			m_MaxGroupRound;	// 전체 그룹 행정을 최대 몇번까지 할 수 있는가?
//	MinMaxArray		m_MinBoundaries;	// 각 그룹의 최소 바운더리
//	MinMaxArray		m_MaxBoundaries;	// 각 그룹의 최대 바운더리
//	CountLimitArray	m_LimitCounts;
//	DropCodeArray	m_DropCodes;
//};

struct EVENT_MINMAX_TYPE_INFO
{
	enum { _MAX_GROUPNODE_COUNT = 15, _MAX_GAPNODE_COUNT = _MAX_GROUPNODE_COUNT+1 };
	enum { _TOTAL_ROUND_UNLIMITED = 255, };	// 전체 그룹 행정을 제한없이 할 수 있도록 한다.
	typedef DWORD			INDEX_T;
	typedef DWORD			ROUND_T;
	typedef DWORD			HEIM_BOUND_T;

	INDEX_T				m_Index;
	ROUND_T				m_MaxGroupRound;					// 전체 그룹 행정을 최대 몇번까지 할 수 있는가?
	HEIM_BOUND_T		m_LimitHeimPerDonation;				// 기부시 최소 금액
#ifdef _NA_20100307_BETAKEY_SYSTEM
    INT32               m_BeytaKey;
#endif
	struct NODE
	{
		HEIM_BOUND_T	Min;								// 각 그룹의 최소 바운더리
		HEIM_BOUND_T	Max;								// 각 그룹의 최대 바운더리
	}					m_Nodes[_MAX_GROUPNODE_COUNT];		// 각 그룹의 MinMax
	ROUND_T				m_Limits[_MAX_GROUPNODE_COUNT];		// 각 그룹에서 보상받을 수 있는 회수
	DWORD				m_DropCodes[_MAX_GROUPNODE_COUNT];	// 각 그룹에서 랜덤으로 보상받기 위한 DropCode

	struct GAP												// 각 그룹의 사이사이에 무 보상 공간 존재 그걸 계산하기 위한 구조
	{
		HEIM_BOUND_T	Min;
		HEIM_BOUND_T	Max;
	}					m_Gaps[_MAX_GAPNODE_COUNT];
};

typedef std::map<EVENT_MINMAX_TYPE_INFO::INDEX_T, EVENT_MINMAX_TYPE_INFO>	EVENT_MINMAX_MAP;
typedef EVENT_MINMAX_MAP::iterator											EVENT_MINMAX_MAP_IT;

struct sEVENT_MINMAX_RESULT
{
	enum eCONTROL {
		CNTL_FAIL			= 0,	// 보상 받을 수 없다.
		CNTL_UPDATE			,		// 보상은 없이 정보 업데이트만 된다.
		CNTL_SUCCESS		,		// 보상을 받는다. (그룹은 상관없이 drop코드에 집중할 것)
	};
	eCONTROL			m_Result;
	RC::eEVENT_RESULT	m_ResultCode;			// <- FAIL일 경우 세부 에러
	DWORD				m_DropCode;				// 보상 드랍 코드
	DWORD				m_RewaredCount;			// 해당 그룹에서 보상된 회수
	DWORD				m_CalcedDonatedMoney;	// 내부적으로 계산된 총 기부액, 다음 상황이 발생 가능 ( calced != donated + add )
	DWORD				m_TotalRounds;			// 전체 그룹 행정 회수
	inline VOID		SetInfo( eCONTROL IN ctrl, DWORD IN dropCode, DWORD IN rewardedCnt, DWORD IN calcedMoney, DWORD IN totalRounts )
	{ m_Result = ctrl; m_ResultCode=(RC::eEVENT_RESULT)0; m_DropCode = dropCode ; m_RewaredCount = rewardedCnt; m_CalcedDonatedMoney = calcedMoney; m_TotalRounds = totalRounts; }
};

struct sEVENT_MINMAX_QUERY
{
	const DWORD			linkedID;
	const DWORD			donatedMoney;		// 
	const DWORD			addinMoney;			// 
	const EVENT_INFO*	pEventInfo;
	const DWORD			curRewardedCount;
	const DWORD			curTotalRounds;		// (현) 전체 그룹 행정 회수
	const BOOL			bSkipCheckHeims;	// 최소 기부 하임 제약조건 검사 (최 상위에서는 FALSE)

	sEVENT_MINMAX_QUERY(
		const DWORD	_linkedID, const DWORD _donatedMoney, const DWORD _addinMoney,
		const EVENT_INFO* _pEventInfo, const DWORD _curRewardedCount, const DWORD _curTotalRounds,
		const BOOL _bSkipCheckHeims = FALSE )
		: bSkipCheckHeims(_bSkipCheckHeims),
		linkedID(_linkedID), donatedMoney(_donatedMoney), addinMoney(_addinMoney),
		pEventInfo(_pEventInfo), curRewardedCount(_curRewardedCount), curTotalRounds(_curTotalRounds)
	{
	}
    //
    __DISABLE_COPY(sEVENT_MINMAX_QUERY);
};


// 이벤트ID 순으로 정렬
typedef std::map<CODETYPE, EVENT_INFO>		EVENTINFO_MAP;
typedef std::pair<CODETYPE, EVENT_INFO>		EVENTINFO_PAIR;
typedef EVENTINFO_MAP::iterator				EVENTINFO_MAP_ITER;

typedef std::vector<EVENT_INFO*>			EVENT_INFO_VEC;
typedef EVENT_INFO_VEC::iterator			EVENT_INFO_VEC_ITER;

// 이벤트 모드에 따라 묶기
typedef std::multimap<BYTE, EVENT_SUB_INFO>	EVENT_SUB_INFO_MAP;
typedef std::pair<BYTE, EVENT_SUB_INFO>		EVENT_SUB_INFO_PAIR;
typedef EVENT_SUB_INFO_MAP::iterator		EVENT_SUB_INFO_ITER;

//-------------------------------------------------------------------------------------------------
// 이벤트 정보 파서
//-------------------------------------------------------------------------------------------------
class SunEventInfoParser : public util::Singleton<SunEventInfoParser>, public IParser
{
public:		SunEventInfoParser();
			~SunEventInfoParser();

			VOID					Init( char * pszPackFileName );
			VOID					Release();
			VOID					Reload();

			EVENT_INFO*				FindEventInfo( CODETYPE dwEventID );
			EVENT_INFO*				FindEventInfoFromMode( eEVENT_MODE eMode );	//각 모드의 첫번째 이벤트 정보를 리턴한다.
			EVENT_INFO_VEC*			FindEventInfoFromNpcType( CODETYPE dwNpcType );

			EVENTINFO_MAP*			GetEventInfoMap() { return &m_mapEventInfo; }
			EVENT_SUB_INFO_MAP*		GetEventSubInfoMap() { return &m_mapEventSubInfo; }
#if defined(_SERVER)
public:		VOID					FindMinMaxInfoByBoundary(
												sEVENT_MINMAX_QUERY& IN rARG,
												sEVENT_MINMAX_RESULT& OUT rResult );

			BOOL _LoadEventReward( BOOL bReload );
#endif
	
public:
	BOOL LoadScript( eSCRIPT_CODE ScriptCode, BOOL bReload );

private:
	BOOL _Load( BOOL bReload );

			VOID					Unload();

private:	EVENTINFO_MAP			m_mapEventInfo;
			EVENT_SUB_INFO_MAP		m_mapEventSubInfo;
			EVENT_INFO_VEC			m_vecFindedEventInfo;
#if defined(_SERVER)
			EVENT_MINMAX_MAP		m_MinMaxMap;
#endif
			CHAR					m_pszEventFileName[MAX_PATH];
};
