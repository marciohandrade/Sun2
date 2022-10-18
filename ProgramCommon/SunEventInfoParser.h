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

// �̺�Ʈ ����
enum eEVENT_MODE
{
	eEVENT_CREATECHAR,			// ĳ���� ������ Ư�� ������ ����
	eEVENT_REGWEBITEM,			// ������ ������ ��Ͻ� ��ǰ ����
	eEVENT_REGITEM_GETITEM,		// ������ ����ϸ� Ư�� ������ ����
	eEVENT_REGITEM_RANDREWARD,	// ������ ����ϸ� �����ϰ� ���� ����
	eEVENT_CRAFTITEM,			// ������ ���� �̺�Ʈ
	eEVENT_ENCHANTITEM,			// ������ ��æƮ �̺�Ʈ
	eEVENT_DECOMPITEM,			// ������ ���� �̺�Ʈ
	eEVENT_PEAKTIME,			// ��ũŸ�� �̺�Ʈ
	eEVENT_COUPON,				// ���� �� ���� �̺�Ʈ
	eEVENT_PCBANG_PEAKTIME,		// PC�� ��ũŸ�� �̺�Ʈ
	eEVENT_DONATION_HEIM,		// ���� ��� �̺�Ʈ
	eEVENT_ITEM_SELECT,			// ������ ���� �̺�Ʈ (��� ���� npc ���ø����� �̷����� �̺�Ʈ)
	eEVENT_BUFF_GRANT,			// ���� �ο� �̺�Ʈ (�̺�Ʈ ID ��û ���� NPC ���ø����� �̷����� �̺�Ʈ)
#ifdef _NA_20081121_ABUMAN_NIJOT_EVENT
	eEVENT_GUILD_MISSION_EVENT = 13, // �̼� �Ϸ�� �̼� ������ ��� �����̸� ����ġ �� �ִ� �̺�Ʈ.
#endif
    //_KR_0_20091021_FIRENDINVITATION_EXTEND
	eEVENT_FRIENDINVATION_EVENT = 14, // ģ���ʴ��� ���� ��Ƽ�� ������ �ϸ� �߰� ����ġ�� �ִ� �̺�Ʈ
#ifdef _NA_007992_20150127_EVENT_MONSTER_INVASION
    eEVENT_MONSTER_INVASION = 15,   // ����ħ�� �̺�Ʈ
#endif //_NA_007992_20150127_EVENT_MONSTER_INVASION
#ifdef _NA_008404_20150712_MINIGAME_SYSTEM
    eEVENT_MINIGAME_BINGO       = 16, // ���� �̺�Ʈ
#endif // _NA_008404_20150712_MINIGAME_SYSTEM
    //
	eEVENT_MAX,					// �̺�Ʈ�� ID �ִ밪

	//	__NA001094_080904_TASK_SSQ_REWARD_PROCESS__
	//	T_EventUser DB Table �ٸ������� ����ϱ� ���� �����ڵ�
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

//#define _WHATTHEHELL_SUNEVENT_20060727	// �� �̺�Ʈ ��ũ��Ʈ���� ������ �̺�Ʈ�� ���� �ʵ��� ������


// �� �̺�Ʈ ���� �����
struct EVENT_INFO
{
	CODETYPE		m_dwEventID;
    //npc������ ������� �ʴ� ����ħ��(mode==15)������ npc�ڵ带 mapcode�� ����Ѵ�
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
	float			m_fRewardExpRate;		// ��ũŸ�� ����ġ ������
	float			m_fRewardExpRate2;		// �ű�ĳ�� ����ġ ������
	float			m_fRewardHimeRate;
	float			m_fRewardItemDropRate;
	CODETYPE		m_dwRewardInfo[_MAX_EVENT_REWARD];	// �̸� ����, (����) ���� �ڵ� -> (����) ���� ����_�����ڵ�+��Ÿ(�̰��� �̺�Ʈ ��忡 ���� ����)
	CODETYPE		m_dwPCRoomRewardInfo[_MAX_EVENT_REWARD];
	CODETYPE		m_EventRewardCode;
	BYTE			m_IsDeleteFlagForRewardedItem;
	enum { _MAX_DELETE_ITEM_CODES = 5, };
	SLOTCODE		m_DeleteRewaredItemCode[_MAX_DELETE_ITEM_CODES]; 

	enum { _MAX_EXPRATE_NUM = 3 };
	float			m_fPlayTimeExpRate[_MAX_EXPRATE_NUM];	// (071120) ���� ������ �ʴ´�.

#ifdef _NA_007919_20141211_ADD_GMCOMMAND_EVENT
    bool is_gm_event_;      // ��ũ��Ʈ���� ���� �̺�Ʈ�� false / GM�� ���� �̺�Ʈ�̸� true
    bool is_pre_notify_;    // �̺�Ʈ ���� "5���� �˸�" ���࿩�� (false: ���� �˸�������)

    // GM�̺�Ʈ�� ������ �����Ѵ�.
    // EventID�� ������ ������ ��.
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
//		�������� ������ ��ũ��Ʈ

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
//	ROUND_T			m_MaxGroupRound;	// ��ü �׷� ������ �ִ� ������� �� �� �ִ°�?
//	MinMaxArray		m_MinBoundaries;	// �� �׷��� �ּ� �ٿ����
//	MinMaxArray		m_MaxBoundaries;	// �� �׷��� �ִ� �ٿ����
//	CountLimitArray	m_LimitCounts;
//	DropCodeArray	m_DropCodes;
//};

struct EVENT_MINMAX_TYPE_INFO
{
	enum { _MAX_GROUPNODE_COUNT = 15, _MAX_GAPNODE_COUNT = _MAX_GROUPNODE_COUNT+1 };
	enum { _TOTAL_ROUND_UNLIMITED = 255, };	// ��ü �׷� ������ ���Ѿ��� �� �� �ֵ��� �Ѵ�.
	typedef DWORD			INDEX_T;
	typedef DWORD			ROUND_T;
	typedef DWORD			HEIM_BOUND_T;

	INDEX_T				m_Index;
	ROUND_T				m_MaxGroupRound;					// ��ü �׷� ������ �ִ� ������� �� �� �ִ°�?
	HEIM_BOUND_T		m_LimitHeimPerDonation;				// ��ν� �ּ� �ݾ�
#ifdef _NA_20100307_BETAKEY_SYSTEM
    INT32               m_BeytaKey;
#endif
	struct NODE
	{
		HEIM_BOUND_T	Min;								// �� �׷��� �ּ� �ٿ����
		HEIM_BOUND_T	Max;								// �� �׷��� �ִ� �ٿ����
	}					m_Nodes[_MAX_GROUPNODE_COUNT];		// �� �׷��� MinMax
	ROUND_T				m_Limits[_MAX_GROUPNODE_COUNT];		// �� �׷쿡�� ������� �� �ִ� ȸ��
	DWORD				m_DropCodes[_MAX_GROUPNODE_COUNT];	// �� �׷쿡�� �������� ����ޱ� ���� DropCode

	struct GAP												// �� �׷��� ���̻��̿� �� ���� ���� ���� �װ� ����ϱ� ���� ����
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
		CNTL_FAIL			= 0,	// ���� ���� �� ����.
		CNTL_UPDATE			,		// ������ ���� ���� ������Ʈ�� �ȴ�.
		CNTL_SUCCESS		,		// ������ �޴´�. (�׷��� ������� drop�ڵ忡 ������ ��)
	};
	eCONTROL			m_Result;
	RC::eEVENT_RESULT	m_ResultCode;			// <- FAIL�� ��� ���� ����
	DWORD				m_DropCode;				// ���� ��� �ڵ�
	DWORD				m_RewaredCount;			// �ش� �׷쿡�� ����� ȸ��
	DWORD				m_CalcedDonatedMoney;	// ���������� ���� �� ��ξ�, ���� ��Ȳ�� �߻� ���� ( calced != donated + add )
	DWORD				m_TotalRounds;			// ��ü �׷� ���� ȸ��
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
	const DWORD			curTotalRounds;		// (��) ��ü �׷� ���� ȸ��
	const BOOL			bSkipCheckHeims;	// �ּ� ��� ���� �������� �˻� (�� ���������� FALSE)

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


// �̺�ƮID ������ ����
typedef std::map<CODETYPE, EVENT_INFO>		EVENTINFO_MAP;
typedef std::pair<CODETYPE, EVENT_INFO>		EVENTINFO_PAIR;
typedef EVENTINFO_MAP::iterator				EVENTINFO_MAP_ITER;

typedef std::vector<EVENT_INFO*>			EVENT_INFO_VEC;
typedef EVENT_INFO_VEC::iterator			EVENT_INFO_VEC_ITER;

// �̺�Ʈ ��忡 ���� ����
typedef std::multimap<BYTE, EVENT_SUB_INFO>	EVENT_SUB_INFO_MAP;
typedef std::pair<BYTE, EVENT_SUB_INFO>		EVENT_SUB_INFO_PAIR;
typedef EVENT_SUB_INFO_MAP::iterator		EVENT_SUB_INFO_ITER;

//-------------------------------------------------------------------------------------------------
// �̺�Ʈ ���� �ļ�
//-------------------------------------------------------------------------------------------------
class SunEventInfoParser : public util::Singleton<SunEventInfoParser>, public IParser
{
public:		SunEventInfoParser();
			~SunEventInfoParser();

			VOID					Init( char * pszPackFileName );
			VOID					Release();
			VOID					Reload();

			EVENT_INFO*				FindEventInfo( CODETYPE dwEventID );
			EVENT_INFO*				FindEventInfoFromMode( eEVENT_MODE eMode );	//�� ����� ù��° �̺�Ʈ ������ �����Ѵ�.
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
