#pragma once
#ifndef __GAME_SERVER_QUERY_H__
#define __GAME_SERVER_QUERY_H__

//=======================================================================================================================
/// 쿼리를 추가할 때 해야 할 일!
/// 1. 쿼리 클래스를 만든다.
/// 2. 쿼리 풀 클래스에다가 스크립트에 맞추어 등록한다.
/// 3. 해더파일을 만들었을 경우 -> Stdafx.h에 쿼리 클래스 파일 헤더를 등록한다.
//=======================================================================================================================

#include <Define.h>
#include "QuestStruct.h" // DailyQuest

#include "StructInQuery.h"
#include "QueryPoolFactoryDefine.h"

// (CHANGES) (f100414.1L) (WAVERIX) remove unused query section 'Query_Char_Select'
//class Query_Char_Select : public QueryForUser


class Query_Warehouse_Select : public QueryForUser
{
	__DECL_SAFECUSTOMPOOL_PTR(Query_Warehouse_Select);
    //
    enum {
        _MAKE_REORDERING_PARAM_NO_START_(user_guid_, 0),
        //
        _MAKE_REORDERING_PARAM_NO_COUNTS_() // end of records. it equal to number of records
    };
    enum {
        _MAKE_REORDERING_BINDING_NO_START_(money_, 0),
        _MAKE_REORDERING_BINDING_NO_PTR_(password_),
        //
        _MAKE_REORDERING_BINDING_NO_COUNTS_()
    };
public:
	enum
	{ 
        PARAM_ROW_NUM = 1,
        PARAM_COL_NUM = _MAKE_REORDERING_PARAM_NO_COUNTS_(),
		RESULT_ROW_NUM = 1,
		RESULT_COL_NUM = _MAKE_REORDERING_BINDING_NO_COUNTS_(),
	};
    //
    struct sQueryParam
    {
        USERGUID user_guid_;
    } parameter_[PARAM_ROW_NUM];
    //
    struct sQueryResult
    {
        int64 money_;
        char password_[MAX_WAREHOUSE_INVENTORY_PWD_LENGTH + 1];
    } result_records_[RESULT_ROW_NUM];
    //
    ulong uLength[RESULT_COL_NUM];
    //
    _BEGIN_PARAM_DATA(sQueryParam, parameter_, PARAM_COL_NUM)
        _REORDERING_PARAM(user_guid_, SQL_PARAM_INPUT)
    _END_PARAM_DATA();
    //
    _BEGIN_BINDING_DATA(sQueryResult, result_records_, uLength, RESULT_ROW_NUM, RESULT_COL_NUM)
        _REORDERING_BINDING_COLUMN(money_)
        _REORDERING_BINDING_COLUMN_PTR(password_)
    _END_BINDING_DATA();
};



class Query_Password_Update : public QueryForUser
{
	__DECL_SAFECUSTOMPOOL_PTR(Query_Password_Update)
public:
	enum eTYPE_CMD
	{
		PWD_INVENTORY	= 1,
		PWD_WAREHOUSE	= 2,
	};

	enum
	{ 
		PASSWORD_ROW_NUM = 1,
		PASSWORD_COL_NUM = 4,
	};

	struct sPARAM
	{
		INT	m_Result;
	} pParam[PASSWORD_ROW_NUM];

	struct sQUERY_PASSWORD
	{
		BYTE			m_byType;	//eTYPE_CMD
		USERGUID		m_UserGUID;
		CHARGUID		m_CharGUID;
		CHAR			m_psPassword[MAX_WAREHOUSE_INVENTORY_PWD_LENGTH];
	} pPassword[PASSWORD_ROW_NUM];

	ULONG uPassswordLength[PASSWORD_COL_NUM];

	_BEGIN_PARAM_DATA		(sPARAM, pParam, PASSWORD_COL_NUM)
		_BINDING_PARAM		(0, m_Result, SQL_PARAM_OUTPUT)
	_END_PARAM_DATA			()

	_BEGIN_BINDING_DATA(sQUERY_PASSWORD, pPassword, uPassswordLength, PASSWORD_ROW_NUM, PASSWORD_COL_NUM)		
		_BINDING_COLUMN_PTR(0, m_byType)
		_BINDING_COLUMN(1, m_UserGUID)
		_BINDING_COLUMN(2, m_CharGUID)
		_BINDING_COLUMN(3, m_psPassword)
	_END_BINDING_DATA()

	inline BOOL ResultSuccess() { return (pParam[0].m_Result==0); }
};


class SelectEventQuery : public QueryForUser
{
	__DECL_SAFECUSTOMPOOL_PTR(SelectEventQuery)
public:
	enum
	{ 
		PARAM_ROW_NUM	= 1,
		PARAM_COL_NUM	= 1,

		RESULT_ROW_NUM	= MAX_EVENT_INVENTORY_SLOT_NUM,
		RESULT_COL_NUM	= 4,
	};

	struct sPARAM
	{
		INT	m_Result;
	} pParam[PARAM_ROW_NUM];

	struct sQUERY
	{
		INT		m_ItemSeq;
		INT		m_ItemCode;
		BYTE	m_ItemCount;
		INT		m_DateTime;
	}pResult[RESULT_ROW_NUM];

	ULONG uLength[RESULT_COL_NUM];

	_BEGIN_PARAM_DATA		(sPARAM, pParam, PARAM_COL_NUM)
		_BINDING_PARAM		(0, m_Result, SQL_PARAM_OUTPUT)
	_END_PARAM_DATA			()

	_BEGIN_BINDING_DATA(sQUERY, pResult, uLength, RESULT_ROW_NUM, RESULT_COL_NUM)		
		_BINDING_COLUMN(0, m_ItemSeq)
		_BINDING_COLUMN(1, m_ItemCode)
		_BINDING_COLUMN(2, m_ItemCount)
		_BINDING_COLUMN(3, m_DateTime)
	_END_BINDING_DATA()

	inline BOOL ResultSuccess() { return (pParam[0].m_Result==0); }
};

class UdpateEventQuery : public QueryForUser
{
	__DECL_SAFECUSTOMPOOL_PTR(UdpateEventQuery)
public:
	enum
	{ 
		PARAM_ROW_NUM	= 1,
		PARAM_COL_NUM	= 1,
	};

	struct sPARAM
	{
		INT	m_Result;
	} pParam[PARAM_ROW_NUM];


	_BEGIN_PARAM_DATA		(sPARAM, pParam, PARAM_COL_NUM)
		_BINDING_PARAM		(0, m_Result, SQL_PARAM_OUTPUT)
	_END_PARAM_DATA			()

	_INIT_BINDING_DATA()

	inline BOOL ResultSuccess() { return (pParam[0].m_Result==0); }
public:
	VOID		SetItemPos( POSTYPE pos )	{ m_ItemPos = pos;	}
	POSTYPE		GetItemPos()				{ return m_ItemPos; }
private:
	POSTYPE		m_ItemPos;
};

// 이벤트 상태 확인용
class EventStatusQuery : public QueryForUser
{
	__DECL_SAFECUSTOMPOOL_PTR(EventStatusQuery)
public:

#define _CALC_EVENT_STAUTS_QUERY_COL00		2

	#define _CALC_EVENT_STAUTS_QUERY_COL01		(_CALC_EVENT_STAUTS_QUERY_COL00+3)
	#define _NO_m_DonateHeimRewardCnt			(_CALC_EVENT_STAUTS_QUERY_COL00+0)
	#define _NO_m_DonatedHeims					(_CALC_EVENT_STAUTS_QUERY_COL00+1)
	#define _NO_m_Status						(_CALC_EVENT_STAUTS_QUERY_COL00+2)

#define _CALC_EVENT_STAUTS_QUERY_COL_LAST		(_CALC_EVENT_STAUTS_QUERY_COL01)

	enum
	{
		PARAM_ROW_NUM	= 1,
		PARAM_COL_NUM	= 1,

		RESULT_ROW_NUM	= _MAX_EVENT_ROW_NUM,
		RESULT_COL_NUM	= _CALC_EVENT_STAUTS_QUERY_COL_LAST,
	};

	struct sPARAM
	{
		INT	m_Result;
	} pParam[PARAM_ROW_NUM];

	struct sQUERY
	{
		INT		m_EventID;
		INT		m_RegisterCnt;
		INT		m_DonateHeimRewardCnt;
		INT		m_DonatedHeims;
		INT		m_Status;
	} pResult[RESULT_ROW_NUM];

	ULONG uLength[RESULT_COL_NUM];

	_BEGIN_PARAM_DATA		(sPARAM, pParam, PARAM_COL_NUM)
		_BINDING_PARAM		(0, m_Result, SQL_PARAM_OUTPUT)
	_END_PARAM_DATA			()

	_BEGIN_BINDING_DATA(sQUERY, pResult, uLength, RESULT_ROW_NUM, RESULT_COL_NUM)		
		_BINDING_COLUMN(0, m_EventID)
		_BINDING_COLUMN(1, m_RegisterCnt)
		_BINDING_COLUMN(_NO_m_DonateHeimRewardCnt, m_DonateHeimRewardCnt)
		_BINDING_COLUMN(_NO_m_DonatedHeims, m_DonatedHeims)
		_BINDING_COLUMN(_NO_m_Status, m_Status)
	_END_BINDING_DATA()

	inline	BOOL ResultSuccess() { return (pParam[0].m_Result==0); }
public:	// <-- 쿼리 주체의 캐릭터가 변경되는 사이에 쿼리 결과가 돌아올리는... 없겠지-.- 뭐 일단은 char_guid도 추가하자.
	inline	DWORD		GetCharGuid() { return m_CharGuid; }
	inline	VOID		SetCharGuid( const DWORD char_guid ) { m_CharGuid = char_guid; }
private:	DWORD		m_CharGuid;
};

// 이벤트 아이템 업데이트용
class EventItemRegQuery : public QueryForUser
{
	__DECL_SAFECUSTOMPOOL_PTR(EventItemRegQuery)
public:
	enum
	{
		PARAM_ROW_NUM	= 1,
		PARAM_COL_NUM	= 1,
	};

	struct sPARAM
	{
		INT		m_Result;
	} pParam[PARAM_ROW_NUM];


	_BEGIN_PARAM_DATA		(sPARAM, pParam, PARAM_COL_NUM)
		_BINDING_PARAM		(0, m_Result, SQL_PARAM_OUTPUT)
	_END_PARAM_DATA			()

	_INIT_BINDING_DATA()

	inline	BOOL		ResultSuccess() { return (pParam[0].m_Result==0); }
public:		VOID		SetEventID( DWORD dwEventID )	{ m_dwEventID = dwEventID;	}
			DWORD		GetEventID()				{ return m_dwEventID; }
	inline	DWORD		GetCharGuid()							{ return m_CharGuid; }
	inline	VOID		SetCharGuid( const DWORD char_guid )	{ m_CharGuid = char_guid; }
private:	DWORD		m_dwEventID;
			DWORD		m_CharGuid;
};

//------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------
// 미션 랭킹 관련 
//------------------------------------------------------------------------------------------------
// 파티 미션 랭크 정보 Insert
class Query_MissionRank_Insert : public QueryForUser
{
	__DECL_SAFECUSTOMPOOL_PTR(Query_MissionRank_Insert)
public:
	enum
	{
		PARAM_ROW_NUM	= 1,
		PARAM_COL_NUM	= 1,
	};

	struct sPARAM
	{
		INT		m_Result;
	} pParam[PARAM_ROW_NUM];


	_BEGIN_PARAM_DATA		(sPARAM, pParam, PARAM_COL_NUM)
		_BINDING_PARAM		(0, m_Result, SQL_PARAM_OUTPUT)
	_END_PARAM_DATA			()

	_INIT_BINDING_DATA()

	inline BOOL ResultSuccess() { return (pParam[0].m_Result==0); }
};

// 개인별 미션 기록 insert
class Query_MissionRecord_Insert : public QueryForUser
{
	__DECL_SAFECUSTOMPOOL_PTR(Query_MissionRecord_Insert)
public:
	enum
	{
		PARAM_ROW_NUM	= 1,
		PARAM_COL_NUM	= 1,
	};

	struct sPARAM
	{
		INT		m_Result;
	} pParam[PARAM_ROW_NUM];


	_BEGIN_PARAM_DATA		(sPARAM, pParam, PARAM_COL_NUM)
		_BINDING_PARAM		(0, m_Result, SQL_PARAM_OUTPUT)
	_END_PARAM_DATA			()

	_INIT_BINDING_DATA()

	inline BOOL ResultSuccess() { return (pParam[0].m_Result==0); }
};

// 개인의 미션별 기록 Select
class Query_MissionRecord_Select : public QueryForUser
{
	__DECL_SAFECUSTOMPOOL_PTR(Query_MissionRecord_Select)
public:
	enum
	{ 
		PARAM_ROW_NUM	= 1,
		PARAM_COL_NUM	= 1,

		RESULT_ROW_NUM	= MAX_MISSION_NUM,
		RESULT_COL_NUM	= 2,
	};

	struct sPARAM
	{
		INT	m_Result;
	} pParam[PARAM_ROW_NUM];

	struct sQUERY
	{
		INT		m_MissionNo;
		INT		m_ClearTime;
	}pResult[RESULT_ROW_NUM];

	ULONG uLength[RESULT_COL_NUM];

	_BEGIN_PARAM_DATA		(sPARAM, pParam, PARAM_COL_NUM)
		_BINDING_PARAM		(0, m_Result, SQL_PARAM_OUTPUT)
	_END_PARAM_DATA			()

	_BEGIN_BINDING_DATA(sQUERY, pResult, uLength, RESULT_ROW_NUM, RESULT_COL_NUM)		
	_BINDING_COLUMN(0, m_MissionNo)
	_BINDING_COLUMN(1, m_ClearTime)
	_END_BINDING_DATA()

	inline BOOL ResultSuccess() { return (pParam[0].m_Result==0); }
	inline BYTE GetType()				{ return m_Type; }
	inline VOID SetType( BYTE type )	{ m_Type=type; }
private:
	BYTE m_Type;
};

// 미션 3순위 보상 가져오기
class Query_MissionReward_Select : public QueryForUser
{
	__DECL_SAFECUSTOMPOOL_PTR(Query_MissionReward_Select)
public:
	enum
	{ 
		PARAM_ROW_NUM	= 1,
		PARAM_COL_NUM	= 1,

		RESULT_ROW_NUM	= 3*MAX_MISSION_NUM,	//< 3등까지 * 미션 맵 종류(6)
		RESULT_COL_NUM	= 4,
	};

	struct sPARAM
	{
		INT	m_Result;
	} pParam[PARAM_ROW_NUM];

	struct sQUERY
	{
		INT			m_MissionNo;
		USERGUID	m_UserGuid;
		CHARGUID	m_CharGuid;
		BYTE		m_RewardSts;
	}pResult[RESULT_ROW_NUM];

	ULONG uLength[RESULT_COL_NUM];

	_BEGIN_PARAM_DATA		(sPARAM, pParam, PARAM_COL_NUM)
		_BINDING_PARAM		(0, m_Result, SQL_PARAM_OUTPUT)
	_END_PARAM_DATA			()

	_BEGIN_BINDING_DATA(sQUERY, pResult, uLength, RESULT_ROW_NUM, RESULT_COL_NUM)		
	_BINDING_COLUMN(0, m_MissionNo)
	_BINDING_COLUMN(1, m_UserGuid)
	_BINDING_COLUMN(2, m_CharGuid)
	_BINDING_COLUMN(3, m_RewardSts)
	_END_BINDING_DATA()

	inline BOOL ResultSuccess() { return (pParam[0].m_Result==0); }
};

// 미션 보상 update
class Query_MissionReward_Update : public QueryForUser
{
	__DECL_SAFECUSTOMPOOL_PTR(Query_MissionReward_Update)
public:
	enum
	{
		PARAM_ROW_NUM	= 1,
		PARAM_COL_NUM	= 1,
	};

	struct sPARAM
	{
		INT		m_Result;
	} pParam[PARAM_ROW_NUM];


	_BEGIN_PARAM_DATA		(sPARAM, pParam, PARAM_COL_NUM)
		_BINDING_PARAM		(0, m_Result, SQL_PARAM_OUTPUT)
	_END_PARAM_DATA			()

	_INIT_BINDING_DATA()

	inline BOOL ResultSuccess() { return (pParam[0].m_Result==0); }
};

//------------------------------------------------------------------------------------------------
// 미션 랭킹 관련 
//------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------
class Query_Char_Exist : public QueryForUser
{
	__DECL_SAFECUSTOMPOOL_PTR(Query_Char_Exist)
public:
	enum
	{ 
		PARAM_ROW_NUM	= 1,
#ifdef _RU_20110801_SEND_GIFT_FRIEND_CHECK    
    #ifdef __NA000000_SHOP_USERID_50_PASSING_PROC__
        PARAM_COL_NUM   = 5,
    #else
        PARAM_COL_NUM	= 2,
    #endif
#else
    #ifdef __NA000000_SHOP_USERID_50_PASSING_PROC__
        PARAM_COL_NUM   = 4,
    #else
		PARAM_COL_NUM	= 2,
    #endif
#endif
	};

	struct sPARAM
	{
    #ifdef _RU_20110801_SEND_GIFT_FRIEND_CHECK    
        int m_Result;
    #endif
		int	m_UserGuid;
		int	m_CharGuid;
    #ifdef __NA000000_SHOP_USERID_50_PASSING_PROC__
        int m_CharClass;
        int m_CharLevel;
    #endif
	} pParam[PARAM_ROW_NUM];

#ifdef _RU_20110801_SEND_GIFT_FRIEND_CHECK    
    _BEGIN_PARAM_DATA		(sPARAM, pParam, PARAM_COL_NUM)    
        _BINDING_PARAM		(0, m_Result, SQL_PARAM_OUTPUT)
        _BINDING_PARAM		(1, m_UserGuid, SQL_PARAM_OUTPUT)
        _BINDING_PARAM		(2, m_CharGuid, SQL_PARAM_OUTPUT)
    #ifdef __NA000000_SHOP_USERID_50_PASSING_PROC__
        _BINDING_PARAM      (3, m_CharClass, SQL_PARAM_OUTPUT)
        _BINDING_PARAM      (4, m_CharLevel, SQL_PARAM_OUTPUT)
    #endif
    _END_PARAM_DATA			()
#else
    _BEGIN_PARAM_DATA		(sPARAM, pParam, PARAM_COL_NUM)    
        _BINDING_PARAM		(0, m_UserGuid, SQL_PARAM_OUTPUT)
        _BINDING_PARAM		(1, m_CharGuid, SQL_PARAM_OUTPUT)
    #ifdef __NA000000_SHOP_USERID_50_PASSING_PROC__
        _BINDING_PARAM      (2, m_CharClass, SQL_PARAM_OUTPUT)
        _BINDING_PARAM      (3, m_CharLevel, SQL_PARAM_OUTPUT)
    #endif
    _END_PARAM_DATA			()
#endif
	

	_INIT_BINDING_DATA();
public:
    inline void Clear() { ZeroMemory(pParam, sizeof(pParam)); } // Added by __NA000000_SHOP_USERID_50_PASSING_PROC__
	inline BOOL ResultSuccess() { return (pParam[0].m_UserGuid!=0); }

	USERGUID GetResultUserGuid() { return pParam[0].m_UserGuid; }
	USERGUID GetResultCharGuid() { return pParam[0].m_CharGuid; }
#ifdef _RU_20110801_SEND_GIFT_FRIEND_CHECK    
    inline int GetResult() { return (pParam[0].m_Result); }
#endif
#ifdef __NA000000_SHOP_USERID_50_PASSING_PROC__
    eCHAR_TYPE GetResultCharClass() const { return eCHAR_TYPE(pParam[0].m_CharClass); }
    LEVELTYPE GetResultCharLevel() const { return LEVELTYPE(pParam[0].m_CharLevel); }

    eCHARNAME_QUERY_ACTION_TYPE GetQueryActionType() const { return m_QueryActionType; }
    void SetQueryActionType(eCHARNAME_QUERY_ACTION_TYPE action) { m_QueryActionType = action; }
    eCHARNAME_QUERY_ACTION_TYPE m_QueryActionType;
    QueryUserIdInfo m_PassingBlock;
#endif
	void SetParamCharName(const TCHAR* p)
	{ 
        _tcsncpy(m_tszParamCharName, p, _countof(m_tszParamCharName));
        m_tszParamCharName[_countof(m_tszParamCharName) - 1] = _T('\0');
	}
	inline const TCHAR* GetParamCharName() const { return m_tszParamCharName; }
	__PROPERTY( BYTE, Type );
	TCHAR m_tszParamCharName[MAX_CHARNAME_LENGTH + 1];
};


//------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------
// 웨이 포인트 랭킹 관련 
//------------------------------------------------------------------------------------------------
class Query_WayPoint_Select : public QueryForUser
{
	__DECL_SAFECUSTOMPOOL_PTR(Query_WayPoint_Select)
public:
	enum
	{ 
		//PARAM_ROW_NUM	= 1,
		//PARAM_COL_NUM	= 1,

		RESULT_ROW_NUM	= MAX_MAP_NUM,
		RESULT_COL_NUM	= 1,
	};

	//struct sPARAM
	//{
	//	INT	m_Result;
	//} pParam[PARAM_ROW_NUM];

	struct sQUERY
	{
		INT			m_MapCode;
	}pResult[RESULT_ROW_NUM];

	ULONG uLength[RESULT_COL_NUM];

	//_BEGIN_PARAM_DATA		(sPARAM, pParam, PARAM_COL_NUM)
	//	_BINDING_PARAM		(0, m_Result, SQL_PARAM_OUTPUT)
	//_END_PARAM_DATA			()
	_INIT_PARAM_DATA()

	_BEGIN_BINDING_DATA(sQUERY, pResult, uLength, RESULT_ROW_NUM, RESULT_COL_NUM)		
	_BINDING_COLUMN(0, m_MapCode)
	_END_BINDING_DATA()

	//inline BOOL ResultSuccess() { return (pParam[0].m_Result==0); }
};

class Query_WayPoint_Insert : public QueryForUser
{
	__DECL_SAFECUSTOMPOOL_PTR(Query_WayPoint_Insert)
public:
	enum
	{
		PARAM_ROW_NUM	= 1,
		PARAM_COL_NUM	= 1,
	};

	struct sPARAM
	{
		INT		m_Result;
	} pParam[PARAM_ROW_NUM];


	_BEGIN_PARAM_DATA		(sPARAM, pParam, PARAM_COL_NUM)
		_BINDING_PARAM		(0, m_Result, SQL_PARAM_OUTPUT)
	_END_PARAM_DATA			()

	_INIT_BINDING_DATA()

	inline BOOL ResultSuccess() { return (pParam[0].m_Result==0); }
	__PROPERTY( MAPCODE, MapCode );
};

//{__NA001164_20081024_CHAR_WAY_POINT_SET_DB_QUERY
//------------------------------------------------------------------------------------------------
// 개인 포인트 관련 
//------------------------------------------------------------------------------------------------

class Query_Char_WayPoint_Select : public QueryForUser
{
	__DECL_SAFECUSTOMPOOL_PTR(Query_Char_WayPoint_Select)
public:
	enum
	{ 
		RESULT_ROW_NUM	= MAX_CHAR_WAYPOINT_NUM,
		RESULT_COL_NUM	= 5,
	};

	struct sQUERY
	{
		DWORD		m_IdxWayPoint;
		MAPCODE		m_MapCode;	
		SHORT		m_X;			
		SHORT		m_Y;		
		SHORT		m_Z;			
	}pResult[RESULT_ROW_NUM];

	ULONG uLength[RESULT_COL_NUM];

	_INIT_PARAM_DATA()

	_BEGIN_BINDING_DATA(sQUERY, pResult, uLength, RESULT_ROW_NUM, RESULT_COL_NUM)		
	_BINDING_COLUMN(0, m_IdxWayPoint)
	_BINDING_COLUMN(1, m_MapCode)
	_BINDING_COLUMN(2, m_X)
	_BINDING_COLUMN(3, m_Y)
	_BINDING_COLUMN(4, m_Z)
	_END_BINDING_DATA()
};

class Query_Char_WayPoint_Insert : public QueryForUser
{
	__DECL_SAFECUSTOMPOOL_PTR(Query_Char_WayPoint_Insert)
public:
	enum
	{
		PARAM_ROW_NUM	= 1,
		PARAM_COL_NUM	= 1,

		RESULT_ROW_NUM	= 1,
		RESULT_COL_NUM	= 1,
	};

	struct sPARAM
	{
		INT		m_Result;
	} pParam[PARAM_ROW_NUM];

	struct sData
	{
		INT		m_IdxWayPoint;
	} pResult[RESULT_ROW_NUM];

	ULONG uLength[RESULT_COL_NUM];

	_BEGIN_PARAM_DATA		(sPARAM, pParam,	PARAM_COL_NUM)
	_BINDING_PARAM			(0, m_Result,		SQL_PARAM_OUTPUT)
	_END_PARAM_DATA			()

	_BEGIN_BINDING_DATA		(sData, pResult, uLength, RESULT_ROW_NUM, RESULT_COL_NUM)		
	_BINDING_COLUMN			(0, m_IdxWayPoint)
	_END_BINDING_DATA		()

	inline BOOL ResultSuccess() { return (pParam[0].m_Result==0); }
	__PROPERTY( MAPCODE, MapCode );
	__PROPERTY( SHORT, X );
	__PROPERTY( SHORT, Y );
	__PROPERTY( SHORT, Z );
};

class Query_Char_WayPoint_Delete : public QueryForUser
{
	__DECL_SAFECUSTOMPOOL_PTR(Query_Char_WayPoint_Delete)
public:
	enum
	{
		PARAM_ROW_NUM	= 1,
		PARAM_COL_NUM	= 1,
	};

	struct sPARAM
	{
		INT		m_Result;
	} pParam[PARAM_ROW_NUM];


	_BEGIN_PARAM_DATA		(sPARAM, pParam, PARAM_COL_NUM)
	_BINDING_PARAM			(0, m_Result, SQL_PARAM_OUTPUT)
	_END_PARAM_DATA			()

	_INIT_BINDING_DATA()

	inline BOOL ResultSuccess() { return (pParam[0].m_Result==0); }
	__PROPERTY( DWORD, IdxWayPoint );
};

//------------------------------------------------------------------------------------------------
// 포인트 랭킹 관련 
//------------------------------------------------------------------------------------------------
class Query_Ranking_Select_Insert : public QueryForUser
{
	__DECL_SAFECUSTOMPOOL_PTR(Query_Ranking_Select_Insert)
public:
	enum
	{ 
		PARAM_ROW_NUM	= 1,
		PARAM_COL_NUM	= 2,

		RESULT_ROW_NUM	= 1,
		RESULT_COL_NUM	= 2,
	};
	
	struct sPARAM
	{
		int			m_Result;
		RANKINGKEY	m_Key;
	} pParam[PARAM_ROW_NUM];
	
	struct sData
	{
		DWORD		m_Point;
		USERGUID	m_UserGUID;
	} pResult[RESULT_ROW_NUM];

	ULONG uLength[RESULT_COL_NUM];

	_BEGIN_PARAM_DATA		(sPARAM, pParam, PARAM_COL_NUM)
		_BINDING_PARAM		(0, m_Result, SQL_PARAM_OUTPUT)
		_BINDING_PARAM		(1, m_Key, SQL_PARAM_OUTPUT)
	_END_PARAM_DATA			()

	_BEGIN_BINDING_DATA		(sData, pResult, uLength, RESULT_ROW_NUM, RESULT_COL_NUM)		
		_BINDING_COLUMN		(0, m_Point)
		_BINDING_COLUMN		(1, m_UserGUID)
	_END_BINDING_DATA		()

public:
	inline BOOL ResultSuccess()					{ return (pParam[0].m_Result==0); }	
	RANKINGNO	GetMissionNo()					{ return m_MissionNo; }
	VOID		SetMissionNo( RANKINGNO no )	{ m_MissionNo = no; }
#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
    RANKINGTYPE GetRankingType()                {return ranking_type_;}
    VOID        SetRankingType(RANKINGTYPE type){ranking_type_ = type;}
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR

private:
	RANKINGNO	m_MissionNo;
#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
    RANKINGTYPE ranking_type_;
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
};

//------------------------------------------------	------------------------------------------------
// FATE IITEM 관련 
//------------------------------------------------------------------------------------------------
class Query_FateItemIssue_Proc : public QueryForUser
{
	__DECL_SAFECUSTOMPOOL_PTR(Query_FateItemIssue_Proc)
public:

	enum
	{
		PARAM_ROW_NUM	= 1,
		PARAM_COL_NUM	= 1,
	};

	struct sPARAM
	{
		INT	m_Result;
	} pParam[PARAM_ROW_NUM];

	_BEGIN_PARAM_DATA		(sPARAM, pParam, PARAM_COL_NUM)
		_BINDING_PARAM		(0, m_Result, SQL_PARAM_OUTPUT)
	_END_PARAM_DATA			()

	_INIT_BINDING_DATA()

	inline INT GetResultSuccess() { return pParam[0].m_Result; } //0: 성공 // 1: 시스템 오류 2: 발급초과
	__PROPERTY( USERGUID,	UserGUID );
	__PROPERTY( POSTYPE,	ItemPos );
	__PROPERTY( DWORD,		SessionIndex );	
};
//
//------------------------------------------------------------------------------------------------
// 웨이 포인트 랭킹 관련 
//------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------

//
//------------------------------------------------------------------------------------------------
// 길드 창고 관련
//------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------

//////////////////////////////////////////////////////
class GuildWarehouseLogInsertQuery: public QueryResult
{
	__DECL_SAFECUSTOMPOOL_PTR(GuildWarehouseLogInsertQuery)

public:
	enum
	{ 
		PARAM_ROW_NUM	= 1,
		PARAM_COL_NUM	= 1,
	};

	struct sPARAM
	{
		INT	m_Result;
	} pParam[PARAM_ROW_NUM];

	_BEGIN_PARAM_DATA (sPARAM, pParam, PARAM_COL_NUM)
		_BINDING_PARAM   (0, m_Result, SQL_PARAM_OUTPUT)
		_END_PARAM_DATA    ()

		_INIT_BINDING_DATA()

		BOOL ResultSuccess() { return ( (pParam[0].m_Result==0) && (!IsError()) ); }
};

class GuildWarehouseTabExQuery: public QueryResult,public DBResultConvert<GuildWarehouseTabExQuery>
{
	__DECL_SAFECUSTOMPOOL_PTR(GuildWarehouseTabExQuery)

public:
	enum
	{ 
		PARAM_ROW_NUM	= 1,
		PARAM_COL_NUM	= 1,
	};

	struct sPARAM
	{
		INT	m_Result;
	} pParam[PARAM_ROW_NUM];

	_BEGIN_PARAM_DATA (sPARAM, pParam, PARAM_COL_NUM)
		_BINDING_PARAM   (0, m_Result, SQL_PARAM_OUTPUT)
	_END_PARAM_DATA    ()

	_INIT_BINDING_DATA()

	BOOL ResultSuccess() { return ( (pParam[0].m_Result==0) && (!IsError()) ); }
	
	BYTE GetSlotTabNum() { return m_SlotTabNum; }
	void SetSlotTabNum(BYTE num) { m_SlotTabNum = num; }

	GUILDGUID GetGuildGuid() { return m_GuildGuid; }
	void SetGuildGuid(DWORD num) { m_GuildGuid = num; }
	
	DWORD  GetUserKey() { return m_dwUserKey; }
	void SetUserKey(DWORD key) { m_dwUserKey = key; }

	void SetSlotPos( POSTYPE pos ){ m_SlotPos = pos; }
	void SetSlotIndex( SLOTIDX index ){ m_SlotIndex = index; }
	
	POSTYPE GetSlotPos(){ return m_SlotPos; }
	SLOTIDX GetSlotIndex(){ return m_SlotIndex; }

private:
	DWORD m_dwUserKey;
	BYTE m_SlotTabNum;
	GUILDGUID m_GuildGuid;
	SLOTIDX m_SlotIndex;
	POSTYPE m_SlotPos;
};

//{__NA001283_20090227_COLLECTION_LIST
//------------------------------------------------------------------------------------------------
// 채집물 리스트 관리
//------------------------------------------------------------------------------------------------

class Query_Collected_Select : public QueryForUser
{
	__DECL_SAFECUSTOMPOOL_PTR(Query_Collected_Select)
public:
	enum
	{ 
		RESULT_ROW_NUM	= MAX_COLLECTION_LIST_NUM,
		RESULT_COL_NUM	= 2,
	};

	struct sQUERY
	{
		//(lst1024) 4로 하는 바람에 끝1자리 짤렸음... TT
		TCHAR m_CollectionID[5];
		int	  m_Count;	
	}pResult[RESULT_ROW_NUM];

	ULONG uLength[RESULT_COL_NUM];

	_INIT_PARAM_DATA()

	_BEGIN_BINDING_DATA(sQUERY, pResult, uLength, RESULT_ROW_NUM, RESULT_COL_NUM)		
	_BINDING_COLUMN_PTR(0, m_CollectionID)
	_BINDING_COLUMN(1, m_Count)
	_END_BINDING_DATA()
};

class Query_Collected_Insert : public QueryForUser
{
	__DECL_SAFECUSTOMPOOL_PTR(Query_Collected_Insert)
public:
	enum
	{
		PARAM_ROW_NUM	= 1,
		PARAM_COL_NUM	= 1,
	};

	struct sPARAM
	{
		INT		m_Result;
	} pParam[PARAM_ROW_NUM];


	_BEGIN_PARAM_DATA		(sPARAM, pParam, PARAM_COL_NUM)
	_BINDING_PARAM		(0, m_Result, SQL_PARAM_OUTPUT)
	_END_PARAM_DATA			()

	_INIT_BINDING_DATA()

	inline BOOL ResultSuccess() { return (pParam[0].m_Result==0); }
	inline INT  GetResult()		{ return pParam[0].m_Result; }

	WzID GetCollectionID()					{ return m_CollectionID; }
	void SetCollectionID(WzID CollectionID)	{ m_CollectionID = CollectionID; }

private:
	WzID m_CollectionID;
};

typedef Query_Collected_Insert Query_Collected_Delete;

//}__NA001283_20090227_COLLECTION_LIST

// NOTE: used by _KR_0_20091021_FIRENDINVITATION_EXTEND, 친구 소환 시간 업데이트
class Query_Friend_Recall_Update : public QueryResult
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_Friend_Recall_Update)
public:
    enum
    { 
        PARAM_ROW_NUM	= 1,
        PARAM_COL_NUM	= 1,        
    };
    struct sPARAM
    {
        INT	m_Result;
    } pParam[PARAM_ROW_NUM];

    _BEGIN_PARAM_DATA		(sPARAM, pParam, PARAM_COL_NUM)
    _BINDING_PARAM		    (0, m_Result, SQL_PARAM_OUTPUT)
    _END_PARAM_DATA			()

    _INIT_BINDING_DATA()

    inline INT ResultCode() { return pParam[0].m_Result; }
    inline BOOL ResultSuccess() { return (pParam[0].m_Result==0); }
};

// NOTE: used by _KR_0_20091021_FIRENDINVITATION_EXTEND, 친구 소환 정보 얻기
class Query_Friend_Recall_Select : public QueryResult
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_Friend_Recall_Select)
public:
    enum
    { 
        PARAM_ROW_NUM	= 1,
        PARAM_COL_NUM	= 1,

        RESULT_ROW_NUM = (MAX_FRIENDINVITATE_REWARD_NUM),
        RESULT_COL_NUM	= 2,
    };
    struct sPARAM
    {
        INT	m_Result;
    } pParam[PARAM_ROW_NUM];

    struct sQUERY
    {        
        TCHAR	    tszRecallTime[MAX_MIDDATETIME_SIZE + 1];	//최근 소환 시간
        CHARGUID	FriendGUID;                                 //친구 GUID
    }pResult[RESULT_ROW_NUM];

    ULONG       uLength[RESULT_COL_NUM];

    _BEGIN_PARAM_DATA		(sPARAM, pParam, PARAM_COL_NUM)
    _BINDING_PARAM		    (0, m_Result, SQL_PARAM_OUTPUT)
    _END_PARAM_DATA			()

    _BEGIN_BINDING_DATA	(sQUERY, pResult, uLength, RESULT_ROW_NUM, RESULT_COL_NUM)		
    _BINDING_COLUMN_PTR	(0, tszRecallTime)
    _BINDING_COLUMN		(1, FriendGUID)    
    _END_BINDING_DATA	()

    inline INT ResultCode() { return pParam[0].m_Result; }
    inline BOOL ResultSuccess() { return (pParam[0].m_Result==0); }
public:
    inline VOID		SetCharGuid( CHARGUID CharGuid )	{ m_CharGuid = CharGuid;	}
    inline CHARGUID	GetCharGuid()						{ return m_CharGuid; }		
    inline DWORD  GetUserKey() { return m_dwUserKey; }
    inline void SetUserKey(DWORD key) { m_dwUserKey = key; }
private: 	
    CHARGUID	m_CharGuid;	
    DWORD       m_dwUserKey;
};

// NOTE: used by _KR_0_20091021_FIRENDINVITATION_EXTEND, 보상 아이템 받기
class Query_Friend_InvitationReward_Update : public QueryResult
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_Friend_InvitationReward_Update)
public:
    enum
    { 
        PARAM_ROW_NUM	= 1,
        PARAM_COL_NUM	= 1,        
    };
    struct sPARAM
    {
        INT	m_Result;
    } pParam[PARAM_ROW_NUM];

    _BEGIN_PARAM_DATA		(sPARAM, pParam, PARAM_COL_NUM)
    _BINDING_PARAM		    (0, m_Result, SQL_PARAM_OUTPUT)
    _END_PARAM_DATA			()

    _INIT_BINDING_DATA()

    inline INT ResultCode() { return pParam[0].m_Result; }
    inline BOOL ResultSuccess() { return (pParam[0].m_Result==0); }
public:
    inline VOID		SetCharGuid( CHARGUID CharGuid )	{ m_CharGuid = CharGuid;	}
    inline CHARGUID	GetCharGuid()						{ return m_CharGuid; }		
    inline VOID		SetFriendGuid( CHARGUID FriendGuid )	{ m_FriendGuid = FriendGuid;	}
    inline CHARGUID	GetFreindGuid()						{ return m_FriendGuid; }		
    inline VOID		SetLevel( LEVELTYPE Level )	{ m_Level = Level;	}
    inline LEVELTYPE	GetLevel()						{ return m_Level; }		
    inline DWORD  GetUserKey() { return m_dwUserKey; }
    inline void SetUserKey(DWORD key) { m_dwUserKey = key; }
private: 	
    CHARGUID	m_CharGuid;	
    CHARGUID	m_FriendGuid;	
    LEVELTYPE   m_Level;
    DWORD       m_dwUserKey;
};

// NOTE: used by _KR_0_20091021_FIRENDINVITATION_EXTEND, 친구 초대 시스템의 보상 아이템 목록
class Query_Friend_InvitationReward_Select : public QueryResult
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_Friend_InvitationReward_Select)
public:
    enum
    { 
        PARAM_ROW_NUM	= 1,
        PARAM_COL_NUM	= 1,

        RESULT_ROW_NUM = (MAX_FRIENDINVITATE_REWARD_NUM),
        RESULT_COL_NUM	= 5,
    };
    struct sPARAM
    {
        INT	m_Result;
    } pParam[PARAM_ROW_NUM];

    struct sQUERY
    {        
        CHARGUID	RecvRewardCharGuid;     //보상 받을 캐릭터 GUID
        CHARGUID	SendRewardCharGuid;     //보상을 받게 하는 캐릭터 GUID (레벨 달성하는 GUID)
        WORD        Level;                   //달성 레벨
        TCHAR       CharName[MAX_CHARNAME_LENGTH + 1]; //SendRewardCharGuid 의 캐릭터 명
        BYTE        RewardSts;              //보상 받았는지 여부 (1:받았다, 0:안 받았다.)
    }pResult[RESULT_ROW_NUM];

    ULONG       uLength[RESULT_COL_NUM];    

    _BEGIN_PARAM_DATA		(sPARAM, pParam, PARAM_COL_NUM)
    _BINDING_PARAM		(0, m_Result, SQL_PARAM_OUTPUT)
    _END_PARAM_DATA			()

    _BEGIN_BINDING_DATA	(sQUERY, pResult, uLength, RESULT_ROW_NUM, RESULT_COL_NUM)		
    _BINDING_COLUMN		(0, RecvRewardCharGuid)
    _BINDING_COLUMN		(1, SendRewardCharGuid)
    _BINDING_COLUMN		(2, Level)
    _BINDING_COLUMN_PTR	(3, CharName)
    _BINDING_COLUMN		(4, RewardSts)
    _END_BINDING_DATA	()

    inline INT ResultCode() { return pParam[0].m_Result; }
    inline BOOL ResultSuccess() { return (pParam[0].m_Result==0); }
public:
    inline VOID		SetCharGuid( CHARGUID CharGuid )	{ m_CharGuid = CharGuid;	}
    inline CHARGUID	GetCharGuid()						{ return m_CharGuid; }		
private: 	
    CHARGUID	m_CharGuid;	
};

#ifdef _NA_20100420_CHANGE_SYSTEM_MEMO_LOGIC
class Query_Domination_Reward_And_Delete : public QueryResult
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_Domination_Reward_And_Delete)
public:
    enum
    { 
        PARAM_ROW_NUM	= 1,
        PARAM_COL_NUM	= 1,

        RESULT_ROW_NUM  = 1,
        RESULT_COL_NUM	= 4,

        RETURN_CODE_SUCCESS = 0,
        RETURN_CODE_FAILURE = 1,
    };

    struct sPARAM {
        INT	result_value_;
    } pParam[PARAM_ROW_NUM];

    struct sQUERY
    {
        MAPCODE result_map_code_;
        CHARGUID result_char_guid_;
        MONEY result_amount_cost_;
        eSYSTEM_MEMO_TYPE result_reward_type_;
    } pResult[RESULT_ROW_NUM];

    ULONG uLength[RESULT_COL_NUM];

    _BEGIN_PARAM_DATA   (sPARAM, pParam, PARAM_COL_NUM)
        _BINDING_PARAM      (0, result_value_, SQL_PARAM_OUTPUT)
        _END_PARAM_DATA     ()

        _BEGIN_BINDING_DATA (sQUERY, pResult, uLength, RESULT_ROW_NUM, RESULT_COL_NUM)		
        _BINDING_COLUMN     (0, result_char_guid_)
        _BINDING_COLUMN     (1, result_amount_cost_)
        _BINDING_COLUMN     (2, result_reward_type_)
        _BINDING_COLUMN     (3, result_map_code_)
        _END_BINDING_DATA   ()

        BOOL ResultSuccess() const { return (pParam[0].result_value_ == RETURN_CODE_SUCCESS); }

        VOID SetCharGuid( CHARGUID char_guid ){ recv_char_guid_ = char_guid; }
        CHARGUID GetCharGuid() const { return recv_char_guid_; }
        
        VOID SetRewardType( eSYSTEM_MEMO_TYPE reward_type ){ recv_reward_type_ = reward_type; }
        eSYSTEM_MEMO_TYPE GetRewardType() const { return recv_reward_type_; }
        
        VOID SetMapCode(MAPCODE map_code){ recv_map_code_ = map_code; }
        MAPCODE GetMapCode() const { return recv_map_code_; }

        VOID SetMemoGuid(MEMOGUID memo_guid){ recv_memo_guid_ = memo_guid; }
        MEMOGUID GetMemoGuid() const { return recv_memo_guid_; }

public:
    CHARGUID recv_char_guid_;
    eSYSTEM_MEMO_TYPE recv_reward_type_;
    MAPCODE recv_map_code_;
    MEMOGUID recv_memo_guid_;
};

class Query_Lotto_Reward_And_Delete : public QueryResult
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_Lotto_Reward_And_Delete)
public:
    enum
    { 
        PARAM_ROW_NUM	= 1,
        PARAM_COL_NUM	= 3,

        RETURN_CODE_SUCCESS = 0,
        RETURN_CODE_FAILURE = 1,
    };

    struct sPARAM {
        INT	result_value_;
        CHARGUID result_char_guid_;
        INT64 result_prize_;
    } pParam[PARAM_ROW_NUM];

    _BEGIN_PARAM_DATA   (sPARAM, pParam, PARAM_COL_NUM)
        _BINDING_PARAM      (0, result_value_, SQL_PARAM_OUTPUT)
        _BINDING_PARAM      (1, result_char_guid_, SQL_PARAM_OUTPUT)
        _BINDING_PARAM      (2, result_prize_, SQL_PARAM_OUTPUT)
        _END_PARAM_DATA     ()

        _INIT_BINDING_DATA()

        BOOL ResultSuccess() { return (pParam[0].result_value_ == RETURN_CODE_SUCCESS); }

        VOID SetCharGuid(CHARGUID char_guid){ recv_char_guid_ = char_guid; }
        CHARGUID GetCharGuid() const { return recv_char_guid_; }

        VOID SetMemoGuid(MEMOGUID memo_guid){ recv_memo_guid_ = memo_guid; }
        MEMOGUID GetMemoGuid() const { return recv_memo_guid_; }

public:
    CHARGUID recv_char_guid_;
    MEMOGUID recv_memo_guid_;
};
#endif//_NA_20100420_CHANGE_SYSTEM_MEMO_LOGIC

//==================================================================================================
class Query_Perk_Select : public QueryForCharTransaction
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_Perk_Select);
    enum {
        PARAM_ROW_NUM   = 1,
        PARAM_COL_NUM   = 2,

        RESULT_ROW_NUM  = PerkDataList::kMaxNumberOfNodes,
        RESULT_COL_NUM	= 7,
    };
    struct sQueryParam {
        int result_;
        CHARGUID char_guid_;
    } parameters_[PARAM_ROW_NUM];

    struct sQueryResult : public nsQuery::PerkResultRecord
    {
    } result_records_[RESULT_ROW_NUM];

    ulong length_array_[RESULT_COL_NUM];

    _BEGIN_PARAM_DATA(sQueryParam, parameters_, PARAM_COL_NUM)
        _BINDING_PARAM(0, result_, SQL_PARAM_OUTPUT)
        _BINDING_PARAM(1, char_guid_, SQL_PARAM_INPUT)
    _END_PARAM_DATA();

    _BEGIN_BINDING_DATA(sQueryResult, result_records_, length_array_, RESULT_ROW_NUM, RESULT_COL_NUM)
        _BINDING_COLUMN(0, group_code_)
        _BINDING_COLUMN(1, level_)
        _BINDING_COLUMN(2, link_code_)
        _BINDING_COLUMN(3, exp_)
    _END_BINDING_DATA();
public:
    bool ResultSuccess() const { return (parameters_[0].result_ == 0); }
    virtual const nsQuery::ProxyTransaction::eTransaction transaction_type() const {
        return nsQuery::ProxyTransaction::eTrans_Perk_Select;
    }
}; //end of class 'Query_Perk_Select'

//==================================================================================================
class Query_Perk_Insert : public QueryForCharTransaction
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_Perk_Insert);
    enum {
        PARAM_ROW_NUM   = 1,
        PARAM_COL_NUM   = 9,
    };
    struct sQueryParam : public nsQuery::PerkResultRecord
    {
        int result_;
        CHARGUID char_guid_;
    } parameters_[PARAM_ROW_NUM];

    _BEGIN_PARAM_DATA(sQueryParam, parameters_, PARAM_COL_NUM)
        _BINDING_PARAM(0, result_, SQL_PARAM_OUTPUT)
        _BINDING_PARAM(1, char_guid_, SQL_PARAM_INPUT)
        _BINDING_PARAM(2, group_code_, SQL_PARAM_INPUT)
        _BINDING_PARAM(3, level_, SQL_PARAM_INPUT)
        _BINDING_PARAM(4, link_code_, SQL_PARAM_INPUT)
        _BINDING_PARAM(5, exp_, SQL_PARAM_INPUT)
    _END_PARAM_DATA();
    //
    _INIT_BINDING_DATA();
public:
    bool ResultSuccess() const { return (parameters_[0].result_ == 0); }
    virtual const nsQuery::ProxyTransaction::eTransaction transaction_type() const {
        return nsQuery::ProxyTransaction::eTrans_Perk_Insert;
    }
};

class Query_Perk_Delete : public QueryForCharTransaction
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_Perk_Delete);
    enum {
        PARAM_ROW_NUM   = 1,
        PARAM_COL_NUM   = 3,
    };
    struct sQueryParam : public nsQuery::PerkResultRecord
    {
        int result_;
        CHARGUID char_guid_;
    } parameters_[PARAM_ROW_NUM];

    _BEGIN_PARAM_DATA(sQueryParam, parameters_, PARAM_COL_NUM)
        _BINDING_PARAM(0, result_, SQL_PARAM_OUTPUT)
        _BINDING_PARAM(1, char_guid_, SQL_PARAM_INPUT)
        _BINDING_PARAM(2, group_code_, SQL_PARAM_INPUT)
    _END_PARAM_DATA();
    //
    _INIT_BINDING_DATA();
public:
    bool ResultSuccess() const { return (parameters_[0].result_ == 0); }
    virtual const nsQuery::ProxyTransaction::eTransaction transaction_type() const {
        return nsQuery::ProxyTransaction::eTrans_Perk_Delete;
    }
};

class Query_Perk_Update : public QueryForCharTransaction
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_Perk_Update);
    enum {
        PARAM_ROW_NUM   = 1,
        PARAM_COL_NUM   = 9,
    };
    struct sQueryParam : public nsQuery::PerkResultRecord
    {
        int result_;
        CHARGUID char_guid_;
    } parameters_[PARAM_ROW_NUM];

    _BEGIN_PARAM_DATA(sQueryParam, parameters_, PARAM_COL_NUM)
        _BINDING_PARAM(0, result_, SQL_PARAM_OUTPUT)
        _BINDING_PARAM(1, char_guid_, SQL_PARAM_INPUT)
        _BINDING_PARAM(2, group_code_, SQL_PARAM_INPUT)
        _BINDING_PARAM(3, level_, SQL_PARAM_INPUT)
        _BINDING_PARAM(4, link_code_, SQL_PARAM_INPUT)
        _BINDING_PARAM(5, exp_, SQL_PARAM_INPUT)
    _END_PARAM_DATA();
    //
    _INIT_BINDING_DATA();
public:
    bool ResultSuccess() const { return (parameters_[0].result_ == 0); }
    virtual const nsQuery::ProxyTransaction::eTransaction transaction_type() const {
        return nsQuery::ProxyTransaction::eTrans_Perk_Update;
    }
};



class Query_ItemSlotInfo_Select : public QueryForCharTransaction
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_ItemSlotInfo_Select);
    static const int kMaxRows = max(MAX_INVENTORY_SLOT_NUM,
                                max(MAX_EQUIPMENT_SLOT_NUM,
                                max(MAX_WAREHOUSE_SLOT_NUM,
                                max(MAX_GUILD_WAREHOUSE_SLOT_NUM,
                                0))));
    enum {
        PARAM_ROW_NUM   = 1,
        PARAM_COL_NUM   = 3, // @RC, CharGuid, SlotType

        RESULT_ROW_NUM  = kMaxRows,
        RESULT_COL_NUM	= 7,
    };
    struct sQueryParam {
        int result_;
        int guid_;
        uint8_t slot_type_;
    } parameters_[PARAM_ROW_NUM];

    struct sQueryResult : public nsQuery::ItemSlotResultRecord
    {
    } result_records_[RESULT_ROW_NUM];

    ulong length_array_[RESULT_COL_NUM];

    _BEGIN_PARAM_DATA(sQueryParam, parameters_, PARAM_COL_NUM)
        _BINDING_PARAM(0, result_, SQL_PARAM_OUTPUT)
        _BINDING_PARAM(1, guid_, SQL_PARAM_INPUT)
        _BINDING_PARAM(2, slot_type_, SQL_PARAM_INPUT)
    _END_PARAM_DATA();

    _BEGIN_BINDING_DATA(sQueryResult, result_records_, length_array_, RESULT_ROW_NUM, RESULT_COL_NUM)
        _BINDING_COLUMN(0, slot_type_)
        _BINDING_COLUMN(1, pos_)
        _BINDING_COLUMN(2, serial_)
        _BINDING_COLUMN(3, code_)
        _BINDING_COLUMN(4, dura_or_num_)
        _BINDING_COLUMN(5, version_)
        _BINDING_COLUMN_PTR(6, option_stream_)
    _END_BINDING_DATA();
public:
    bool ResultSuccess() const { return (parameters_[0].result_ == 0); }
    virtual const nsQuery::ProxyTransaction::eTransaction transaction_type() const {
        return nsQuery::ProxyTransaction::eTrans_ItemSlotInfo_Select;
    }
};

class Query_ItemSlotInfo_Update : public QueryForCharTransaction
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_ItemSlotInfo_Update);
    enum {
        PARAM_ROW_NUM   = 1,
        PARAM_COL_NUM   = 9,
    };
    struct sQueryParam : public nsQuery::ItemSlotResultRecord
    {
        int result_;
        int guid_;
    } parameters_[PARAM_ROW_NUM];

    _BEGIN_PARAM_DATA(sQueryParam, parameters_, PARAM_COL_NUM)
        _BINDING_PARAM( 0, result_, SQL_PARAM_OUTPUT)
        _BINDING_PARAM( 1, guid_, SQL_PARAM_INPUT)
        _BINDING_PARAM( 2, slot_type_, SQL_PARAM_INPUT)
        _BINDING_PARAM( 3, pos_, SQL_PARAM_INPUT)
        _BINDING_PARAM( 4, serial_, SQL_PARAM_INPUT)
        _BINDING_PARAM( 5, code_, SQL_PARAM_INPUT)
        _BINDING_PARAM( 6, dura_or_num_, SQL_PARAM_INPUT)
        _BINDING_PARAM( 7, version_, SQL_PARAM_INPUT)
        _BINDING_PARAM_PTR( 8, option_stream_, SQL_PARAM_INPUT)
    _END_PARAM_DATA();
    //
    _INIT_BINDING_DATA();
public:
    bool ResultSuccess() const { return (parameters_[0].result_ == 0); }
    virtual const nsQuery::ProxyTransaction::eTransaction transaction_type() const {
        return nsQuery::ProxyTransaction::eTrans_ItemSlotInfo_Update;
    }
};

class Query_SlotInfo_Select : public QueryForCharTransaction
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_SlotInfo_Select);
    enum {
        PARAM_ROW_NUM   = 1,
        PARAM_COL_NUM   = 3, // @RC, CharGuid, SlotIndex

        RESULT_ROW_NUM  = 20, // temporary
        RESULT_COL_NUM	= 10,
    };
    struct sQueryParam {
        int result_;
        CHARGUID char_guid_;
        short slot_index_;
    } parameters_[PARAM_ROW_NUM];

    struct sQueryResult : public nsQuery::SlotResultRecord
    {
    } result_records_[RESULT_ROW_NUM];

    ulong length_array_[RESULT_COL_NUM];

    _BEGIN_PARAM_DATA(sQueryParam, parameters_, PARAM_COL_NUM)
        _BINDING_PARAM(0, result_, SQL_PARAM_OUTPUT)
        _BINDING_PARAM(1, char_guid_, SQL_PARAM_INPUT)
        _BINDING_PARAM(2, slot_index_, SQL_PARAM_INPUT)
    _END_PARAM_DATA();

    _BEGIN_BINDING_DATA(sQueryResult, result_records_, length_array_, RESULT_ROW_NUM, RESULT_COL_NUM)
        _BINDING_COLUMN(0, slot_index_)
        _BINDING_COLUMN(1, tab_order_)
        _BINDING_COLUMN(2, slot01_)
        _BINDING_COLUMN(3, slot02_)
        _BINDING_COLUMN(4, slot03_)
        _BINDING_COLUMN(5, slot04_)
        _BINDING_COLUMN(6, slot05_)
        _BINDING_COLUMN(7, slot06_)
        _BINDING_COLUMN(8, slot07_)
        _BINDING_COLUMN(9, slot08_)
    _END_BINDING_DATA();
public:
    bool ResultSuccess() const { return (parameters_[0].result_ == 0); }
    virtual const nsQuery::ProxyTransaction::eTransaction transaction_type() const {
        return nsQuery::ProxyTransaction::eTrans_SlotInfo_Select;
    }
};

class Query_SlotInfo_Insert : public QueryForCharTransaction
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_SlotInfo_Insert);
    enum {
        PARAM_ROW_NUM   = 1,
        PARAM_COL_NUM   = 12,
    };
    struct sQueryParam : public nsQuery::SlotResultRecord
    {
        int result_;
        CHARGUID char_guid_;
    } parameters_[PARAM_ROW_NUM];

    _BEGIN_PARAM_DATA(sQueryParam, parameters_, PARAM_COL_NUM)
        _BINDING_PARAM( 0, result_, SQL_PARAM_OUTPUT)
        _BINDING_PARAM( 1, char_guid_, SQL_PARAM_INPUT)
        _BINDING_PARAM( 2, slot_index_, SQL_PARAM_INPUT)
        _BINDING_PARAM( 3, tab_order_, SQL_PARAM_INPUT)
        _BINDING_PARAM( 4, slot01_, SQL_PARAM_INPUT)
        _BINDING_PARAM( 5, slot02_, SQL_PARAM_INPUT)
        _BINDING_PARAM( 6, slot03_, SQL_PARAM_INPUT)
        _BINDING_PARAM( 7, slot04_, SQL_PARAM_INPUT)
        _BINDING_PARAM( 8, slot05_, SQL_PARAM_INPUT)
        _BINDING_PARAM( 9, slot06_, SQL_PARAM_INPUT)
        _BINDING_PARAM(10, slot07_, SQL_PARAM_INPUT)
        _BINDING_PARAM(11, slot08_, SQL_PARAM_INPUT)
    _END_PARAM_DATA();
    //
    _INIT_BINDING_DATA();
public:
    bool ResultSuccess() const { return (parameters_[0].result_ == 0); }
    virtual const nsQuery::ProxyTransaction::eTransaction transaction_type() const {
        return nsQuery::ProxyTransaction::eTrans_SlotInfo_Insert;
    }
};

class Query_SlotInfo_Update : public QueryForCharTransaction
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_SlotInfo_Update);
    enum {
        PARAM_ROW_NUM   = 1,
        PARAM_COL_NUM   = 12,
    };
    struct sQueryParam : public nsQuery::SlotResultRecord
    {
        int result_;
        CHARGUID char_guid_;
    } parameters_[PARAM_ROW_NUM];

    _BEGIN_PARAM_DATA(sQueryParam, parameters_, PARAM_COL_NUM)
        _BINDING_PARAM( 0, result_, SQL_PARAM_OUTPUT)
        _BINDING_PARAM( 1, char_guid_, SQL_PARAM_INPUT)
        _BINDING_PARAM( 2, slot_index_, SQL_PARAM_INPUT)
        _BINDING_PARAM( 3, tab_order_, SQL_PARAM_INPUT)
        _BINDING_PARAM( 4, slot01_, SQL_PARAM_INPUT)
        _BINDING_PARAM( 5, slot02_, SQL_PARAM_INPUT)
        _BINDING_PARAM( 6, slot03_, SQL_PARAM_INPUT)
        _BINDING_PARAM( 7, slot04_, SQL_PARAM_INPUT)
        _BINDING_PARAM( 8, slot05_, SQL_PARAM_INPUT)
        _BINDING_PARAM( 9, slot06_, SQL_PARAM_INPUT)
        _BINDING_PARAM(10, slot07_, SQL_PARAM_INPUT)
        _BINDING_PARAM(11, slot08_, SQL_PARAM_INPUT)
    _END_PARAM_DATA();
    //
    _INIT_BINDING_DATA();
public:
    bool ResultSuccess() const { return (parameters_[0].result_ == 0); }
    virtual const nsQuery::ProxyTransaction::eTransaction transaction_type() const {
        return nsQuery::ProxyTransaction::eTrans_SlotInfo_Update;
    }
};

class Query_Quest_Select : public QueryForCharTransaction
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_Quest_Select);
    enum {
        PARAM_ROW_NUM   = 1,
        PARAM_COL_NUM   = 2, // @RC, CharGuid

        RESULT_ROW_NUM  = 1,
        RESULT_COL_NUM	= 3,
    };
    struct sQueryParam {
        int result_;
        CHARGUID char_guid_;
    } parameters_[PARAM_ROW_NUM];

    struct sQueryResult : public nsQuery::QuestResultRecord
    {
    } result_records_[RESULT_ROW_NUM];

    ulong length_array_[RESULT_COL_NUM];

    _BEGIN_PARAM_DATA(sQueryParam, parameters_, PARAM_COL_NUM)
        _BINDING_PARAM(0, result_, SQL_PARAM_OUTPUT)
        _BINDING_PARAM(1, char_guid_, SQL_PARAM_INPUT)
    _END_PARAM_DATA();

    _BEGIN_BINDING_DATA(sQueryResult, result_records_, length_array_, RESULT_ROW_NUM, RESULT_COL_NUM)
        _BINDING_COLUMN(0, quest_version_)
        _BINDING_COLUMN_PTR(1, quest_stream_progress_)
        _BINDING_COLUMN_PTR(2, quest_stream_complete_)
    _END_BINDING_DATA();
public:
    bool ResultSuccess() const { return (parameters_[0].result_ == 0); }
    virtual const nsQuery::ProxyTransaction::eTransaction transaction_type() const {
        return nsQuery::ProxyTransaction::eTrans_Quest_Select;
    }
};

class Query_Quest_Update : public QueryForCharTransaction
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_Quest_Update);
    enum {
        PARAM_ROW_NUM   = 1,
        PARAM_COL_NUM   = 5,
    };
    struct sQueryParam : public nsQuery::QuestResultRecord
    {
        int result_;
        CHARGUID char_guid_;
    } parameters_[PARAM_ROW_NUM];

    _BEGIN_PARAM_DATA(sQueryParam, parameters_, PARAM_COL_NUM)
        _BINDING_PARAM( 0, result_, SQL_PARAM_OUTPUT)
        _BINDING_PARAM( 1, char_guid_, SQL_PARAM_INPUT)
        _BINDING_PARAM( 2, quest_version_, SQL_PARAM_INPUT)
        _BINDING_PARAM_PTR(3, quest_stream_progress_, SQL_PARAM_INPUT)
        _BINDING_PARAM_PTR(4, quest_stream_complete_, SQL_PARAM_INPUT)
    _END_PARAM_DATA();
    //
    _INIT_BINDING_DATA();
public:
    bool ResultSuccess() const { return (parameters_[0].result_ == 0); }
    virtual const nsQuery::ProxyTransaction::eTransaction transaction_type() const {
        return nsQuery::ProxyTransaction::eTrans_Quest_Update;
    }
};

class Query_BattleRecords_Select : public QueryForCharTransaction
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_BattleRecords_Select);
    enum {
        PARAM_ROW_NUM   = 1,
        PARAM_COL_NUM   = 2, // @RC, CharGuid

        RESULT_ROW_NUM  = 1,
        RESULT_COL_NUM  = 13,
    };
    struct sQueryParam {
        int result_;
        CHARGUID char_guid_;
    } parameters_[PARAM_ROW_NUM];

    struct sQueryResult : public nsQuery::BattleRecordsResultRecord
    {
    } result_records_[RESULT_ROW_NUM];

    ulong length_array_[RESULT_COL_NUM];

    _BEGIN_PARAM_DATA(sQueryParam, parameters_, PARAM_COL_NUM)
        _BINDING_PARAM(0, result_, SQL_PARAM_OUTPUT)
        _BINDING_PARAM(1, char_guid_, SQL_PARAM_INPUT)
    _END_PARAM_DATA();

    _BEGIN_BINDING_DATA(sQueryResult, result_records_, length_array_, RESULT_ROW_NUM, RESULT_COL_NUM)
        // PVP
        _BINDING_COLUMN( 0, pvp_point_)
        _BINDING_COLUMN( 1, pvp_score_)
        _BINDING_COLUMN( 2, pvp_grade_)
        _BINDING_COLUMN( 3, pvp_max_series_)
        _BINDING_COLUMN( 4, pvp_max_kills_)
        _BINDING_COLUMN( 5, pvp_max_die_)
        _BINDING_COLUMN( 6, pvp_number_of_kills_)
        _BINDING_COLUMN( 7, pvp_number_of_dead_)
        _BINDING_COLUMN( 8, pvp_number_of_draws_)
        // 경쟁헌팅 전적
        _BINDING_COLUMN( 9, hunting_number_of_fails_)
        _BINDING_COLUMN(10, hunting_number_of_victories_)
        _BINDING_COLUMN(11, hunting_number_of_defeats_)
        _BINDING_COLUMN(12, hunting_number_of_joins_)
    _END_BINDING_DATA();
public:
    bool ResultSuccess() const { return (parameters_[0].result_ == 0); }
    virtual const nsQuery::ProxyTransaction::eTransaction transaction_type() const {
        return nsQuery::ProxyTransaction::eTrans_BattleRecords_Select;
    }
};

class Query_BattleRecords_Update : public QueryForCharTransaction
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_BattleRecords_Update);
    enum {
        PARAM_ROW_NUM   = 1,
        PARAM_COL_NUM   = 15,
    };
    struct sQueryParam : public nsQuery::BattleRecordsResultRecord
    {
        int result_;
        CHARGUID char_guid_;
    } parameters_[PARAM_ROW_NUM];

    _BEGIN_PARAM_DATA(sQueryParam, parameters_, PARAM_COL_NUM)
        _BINDING_PARAM( 0, result_, SQL_PARAM_OUTPUT)
        _BINDING_PARAM( 1, char_guid_, SQL_PARAM_INPUT)
        // PVP
        _BINDING_PARAM( 2, pvp_point_, SQL_PARAM_INPUT)
        _BINDING_PARAM( 3, pvp_score_, SQL_PARAM_INPUT)
        _BINDING_PARAM( 4, pvp_grade_, SQL_PARAM_INPUT)
        _BINDING_PARAM( 5, pvp_max_series_, SQL_PARAM_INPUT)
        _BINDING_PARAM( 6, pvp_max_kills_, SQL_PARAM_INPUT)
        _BINDING_PARAM( 7, pvp_max_die_, SQL_PARAM_INPUT)
        _BINDING_PARAM( 8, pvp_number_of_kills_, SQL_PARAM_INPUT)
        _BINDING_PARAM( 9, pvp_number_of_dead_, SQL_PARAM_INPUT)
        _BINDING_PARAM(10, pvp_number_of_draws_, SQL_PARAM_INPUT)
        // 경쟁헌팅 전적
        _BINDING_PARAM(11, hunting_number_of_fails_, SQL_PARAM_INPUT)
        _BINDING_PARAM(12, hunting_number_of_victories_, SQL_PARAM_INPUT)
        _BINDING_PARAM(13, hunting_number_of_defeats_, SQL_PARAM_INPUT)
        _BINDING_PARAM(14, hunting_number_of_joins_, SQL_PARAM_INPUT)
    _END_PARAM_DATA();
    //
    _INIT_BINDING_DATA();
public:
    bool ResultSuccess() const { return (parameters_[0].result_ == 0); }
    virtual const nsQuery::ProxyTransaction::eTransaction transaction_type() const {
        return nsQuery::ProxyTransaction::eTrans_BattleRecords_Update;
    }
};

class Query_Char_State_Select : public QueryForCharTransaction
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_Char_State_Select);
    enum {
        PARAM_ROW_NUM   = 1,
        PARAM_COL_NUM   = 2,

        RESULT_ROW_NUM  = MAX_DBSAVE_STATEINFO_NUM,
        RESULT_COL_NUM	= 5,
    };
    struct sQueryParam {
        int result_;
        CHARGUID char_guid_;
    } parameters_[PARAM_ROW_NUM];

    struct sQueryResult : public nsQuery::CharStateResultRecord
    {
    } result_records_[RESULT_ROW_NUM];

    ulong length_array_[RESULT_COL_NUM];

    _BEGIN_PARAM_DATA(sQueryParam, parameters_, PARAM_COL_NUM)
        _BINDING_PARAM(0, result_, SQL_PARAM_OUTPUT)
        _BINDING_PARAM(1, char_guid_, SQL_PARAM_INPUT)
    _END_PARAM_DATA();

    _BEGIN_BINDING_DATA(sQueryResult, result_records_, length_array_, RESULT_ROW_NUM, RESULT_COL_NUM)
        _BINDING_COLUMN(0, state_code_)
        _BINDING_COLUMN(1, skill_code_)
        _BINDING_COLUMN(2, ability_index_with_option_)
        _BINDING_COLUMN(3, remained_time_)
        _BINDING_COLUMN(4, interval_)
    _END_BINDING_DATA();
public:
    bool ResultSuccess() const { return (parameters_[0].result_ == 0); }
    virtual const nsQuery::ProxyTransaction::eTransaction transaction_type() const {
        return nsQuery::ProxyTransaction::eTrans_Char_State_Select;
    }
}; //end of class 'Query_Char_State_Select'

//==================================================================================================
class Query_Char_State_Insert : public QueryForCharTransaction
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_Char_State_Insert);
    enum {
        PARAM_ROW_NUM   = 1,
        PARAM_COL_NUM   = 7,
    };
    struct sQueryParam : public nsQuery::CharStateResultRecord
    {
        int result_;
        CHARGUID char_guid_;
    } parameters_[PARAM_ROW_NUM];

    _BEGIN_PARAM_DATA(sQueryParam, parameters_, PARAM_COL_NUM)
        _BINDING_PARAM(0, result_, SQL_PARAM_OUTPUT)
        _BINDING_PARAM(1, char_guid_, SQL_PARAM_INPUT)
        _BINDING_PARAM(2, state_code_, SQL_PARAM_INPUT)
        _BINDING_PARAM(3, skill_code_, SQL_PARAM_INPUT)
        _BINDING_PARAM(4, ability_index_with_option_, SQL_PARAM_INPUT)
        _BINDING_PARAM(5, remained_time_, SQL_PARAM_INPUT)
        _BINDING_PARAM(6, interval_, SQL_PARAM_INPUT)
    _END_PARAM_DATA();
    //
    _INIT_BINDING_DATA();
public:
    bool ResultSuccess() const { return (parameters_[0].result_ == 0); }
    virtual const nsQuery::ProxyTransaction::eTransaction transaction_type() const {
        return nsQuery::ProxyTransaction::eTrans_Char_State_Insert;
    }
};

class Query_Char_State_Delete : public QueryForCharTransaction
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_Char_State_Delete);
    enum {
        PARAM_ROW_NUM   = 1,
        PARAM_COL_NUM   = 3,
    };
    struct sQueryParam : public nsQuery::CharStateResultRecord
    {
        int result_;
        CHARGUID char_guid_;
    } parameters_[PARAM_ROW_NUM];

    _BEGIN_PARAM_DATA(sQueryParam, parameters_, PARAM_COL_NUM)
        _BINDING_PARAM(0, result_, SQL_PARAM_OUTPUT)
        _BINDING_PARAM(1, char_guid_, SQL_PARAM_INPUT)
        _BINDING_PARAM(2, state_code_, SQL_PARAM_INPUT)
    _END_PARAM_DATA();
    //
    _INIT_BINDING_DATA();
public:
    bool ResultSuccess() const { return (parameters_[0].result_ == 0); }
    virtual const nsQuery::ProxyTransaction::eTransaction transaction_type() const {
        return nsQuery::ProxyTransaction::eTrans_Char_State_Delete;
    }
};

class Query_Char_State_Update : public QueryForCharTransaction
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_Char_State_Update);
    enum {
        PARAM_ROW_NUM   = 1,
        PARAM_COL_NUM   = 7,
    };
    struct sQueryParam : public nsQuery::CharStateResultRecord
    {
        int result_;
        CHARGUID char_guid_;
    } parameters_[PARAM_ROW_NUM];

    _BEGIN_PARAM_DATA(sQueryParam, parameters_, PARAM_COL_NUM)
        _BINDING_PARAM(0, result_, SQL_PARAM_OUTPUT)
        _BINDING_PARAM(1, char_guid_, SQL_PARAM_INPUT)
        _BINDING_PARAM(2, state_code_, SQL_PARAM_INPUT)
        _BINDING_PARAM(3, skill_code_, SQL_PARAM_INPUT)
        _BINDING_PARAM(4, ability_index_with_option_, SQL_PARAM_INPUT)
        _BINDING_PARAM(5, remained_time_, SQL_PARAM_INPUT)
        _BINDING_PARAM(6, interval_, SQL_PARAM_INPUT)
    _END_PARAM_DATA();
    //
    _INIT_BINDING_DATA();
public:
    bool ResultSuccess() const { return (parameters_[0].result_ == 0); }
    virtual const nsQuery::ProxyTransaction::eTransaction transaction_type() const {
        return nsQuery::ProxyTransaction::eTrans_Char_State_Update;
    }
};


class Query_MissionInfo_Select : public QueryForCharTransaction
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_MissionInfo_Select);
    enum {
        PARAM_ROW_NUM   = 1,
        PARAM_COL_NUM   = 2, // @RC, CharGuid

        RESULT_ROW_NUM  = MAX_MISSION_NUM, // temporary
        RESULT_COL_NUM	= 4,
    };
    struct sQueryParam {
        int result_;
        CHARGUID char_guid_;
        short slot_index_;
    } parameters_[PARAM_ROW_NUM];

    struct sQueryResult : public nsQuery::MissionResultRecord
    {
    } result_records_[RESULT_ROW_NUM];

    ulong length_array_[RESULT_COL_NUM];

    _BEGIN_PARAM_DATA(sQueryParam, parameters_, PARAM_COL_NUM)
        _BINDING_PARAM(0, result_, SQL_PARAM_OUTPUT)
        _BINDING_PARAM(1, char_guid_, SQL_PARAM_INPUT)
    _END_PARAM_DATA();

    _BEGIN_BINDING_DATA(sQueryResult, result_records_, length_array_, RESULT_ROW_NUM, RESULT_COL_NUM)
        _BINDING_COLUMN(0, code)
        _BINDING_COLUMN(1, state)
        _BINDING_COLUMN(2, point)
        _BINDING_COLUMN(3, hidden)
    _END_BINDING_DATA();
public:
    bool ResultSuccess() const { return (parameters_[0].result_ == 0); }
    virtual const nsQuery::ProxyTransaction::eTransaction transaction_type() const {
        return nsQuery::ProxyTransaction::eTrans_MissionInfo_Select;
    }
};

class Query_MissionInfo_Insert : public QueryForCharTransaction
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_MissionInfo_Insert);
    enum {
        PARAM_ROW_NUM   = 1,
        PARAM_COL_NUM   = 6,
    };
    struct sQueryParam : public nsQuery::MissionResultRecord
    {
        int result_;
        CHARGUID char_guid_;
    } parameters_[PARAM_ROW_NUM];

    _BEGIN_PARAM_DATA(sQueryParam, parameters_, PARAM_COL_NUM)
        _BINDING_PARAM(0, result_, SQL_PARAM_OUTPUT)
        _BINDING_PARAM(1, char_guid_, SQL_PARAM_INPUT)
        _BINDING_PARAM(2, code, SQL_PARAM_INPUT)
        _BINDING_PARAM(3, state, SQL_PARAM_INPUT)
        _BINDING_PARAM(4, point, SQL_PARAM_INPUT)
        _BINDING_PARAM(5, hidden, SQL_PARAM_INPUT)
        _END_PARAM_DATA();
    //
    _INIT_BINDING_DATA();
public:
    bool ResultSuccess() const { return (parameters_[0].result_ == 0); }
    virtual const nsQuery::ProxyTransaction::eTransaction transaction_type() const {
        return nsQuery::ProxyTransaction::eTrans_MissionInfo_Insert;
    }
};

class Query_MissionInfo_Update : public QueryForCharTransaction
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_MissionInfo_Update);
    enum {
        PARAM_ROW_NUM   = 1,
        PARAM_COL_NUM   = 6,
    };
    struct sQueryParam : public nsQuery::MissionResultRecord
    {
        int result_;
        CHARGUID char_guid_;
    } parameters_[PARAM_ROW_NUM];

    _BEGIN_PARAM_DATA(sQueryParam, parameters_, PARAM_COL_NUM)
        _BINDING_PARAM(0, result_, SQL_PARAM_OUTPUT)
        _BINDING_PARAM(1, char_guid_, SQL_PARAM_INPUT)
        _BINDING_PARAM(2, code, SQL_PARAM_INPUT)
        _BINDING_PARAM(3, state, SQL_PARAM_INPUT)
        _BINDING_PARAM(4, point, SQL_PARAM_INPUT)
        _BINDING_PARAM(5, hidden, SQL_PARAM_INPUT)
    _END_PARAM_DATA();
    //
    _INIT_BINDING_DATA();
public:
    bool ResultSuccess() const { return (parameters_[0].result_ == 0); }
    virtual const nsQuery::ProxyTransaction::eTransaction transaction_type() const {
        return nsQuery::ProxyTransaction::eTrans_MissionInfo_Update;
    }
};

//==================================================================================================

//_NA_20100812_INTEGRATE_SYSTEM_MEMO_DOMINATION
class Query_System_Memo_Reward_Item_Update_Select : public QueryResult
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_System_Memo_Reward_Item_Update_Select)
public:
    enum
    { 
        PARAM_ROW_NUM       = 1,
        PARAM_COL_NUM       = 1,

        RESULT_ROW_NUM      = 1,
        RESULT_COL_NUM      = 1,

        RETURNCODE_SUCCESS  = 0,
        RETURNCODE_FAILED   = 1,
    };
    struct sPARAM
    {
        INT	result_value;
    } parameters_[PARAM_ROW_NUM];


    _BEGIN_PARAM_DATA		(sPARAM, parameters_, PARAM_COL_NUM)
        _BINDING_PARAM		(0, result_value, SQL_PARAM_OUTPUT)
    _END_PARAM_DATA			()

    _INIT_BINDING_DATA();

    inline bool ResultSuccess() {return (parameters_[0].result_value == RETURNCODE_SUCCESS);}
};

class Query_System_Memo_Reward_Item_Check_Select : public QueryResult
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_System_Memo_Reward_Item_Check_Select)
public:
    enum
    { 
        PARAM_ROW_NUM       = 1,
        PARAM_COL_NUM       = 1,

        RESULT_ROW_NUM      = 1,
        RESULT_COL_NUM      = 3,

        RETURNCODE_SUCCESS  = 0,
        RETURNCODE_FAILED   = 1,
    };
    struct sPARAM
    {
        INT	result_value;
    } parameters_[PARAM_ROW_NUM];

    struct sQUERY
    {
        eSYSTEM_MEMO_TYPE   system_memo_type_;
        SLOTCODE            recv_item_code_;
        ULONGLONG           amount_cost_;
    }result_records_[RESULT_ROW_NUM];

    ULONG length_array_[RESULT_COL_NUM];

    inline INT ResultCode() {return parameters_[0].result_value;}

    _BEGIN_PARAM_DATA		(sPARAM, parameters_, PARAM_COL_NUM)
        _BINDING_PARAM		(0, result_value, SQL_PARAM_OUTPUT)
    _END_PARAM_DATA		    ()

    _BEGIN_BINDING_DATA		(sQUERY, result_records_, length_array_, RESULT_ROW_NUM, RESULT_COL_NUM)		
        _BINDING_COLUMN     (0, system_memo_type_)
        _BINDING_COLUMN     (1, recv_item_code_)
        _BINDING_COLUMN     (2, amount_cost_)
    _END_BINDING_DATA	    ()

    inline bool ResultSuccess() {return (parameters_[0].result_value == RETURNCODE_SUCCESS);}

public:
    inline VOID SetCharGuid(CHARGUID char_guid)	{char_guid_ = char_guid;}
    inline CHARGUID	GetCharGuid() {return char_guid_;}
    inline VOID	SetMemoGuid(MEMOGUID memo_guid)	{memo_guid_ = memo_guid;}
    inline MEMOGUID	GetMemoGuid() {return memo_guid_;}
    // added by _NA_20100901_ADD_GAMELOG_FOR_SYSTEM_MEMO_TYPE
    inline VOID SetMemoContents(const TCHAR* memo_contents)
    {
        _tcsncpy(memo_contents_, memo_contents, _countof(memo_contents_));
        memo_contents_[_countof(memo_contents_) - 1] = '\0';
    }
    inline char* GetMemoContents() {return memo_contents_;}

private: 
    CHARGUID char_guid_;
    MEMOGUID memo_guid_;
    //_NA_20100901_ADD_GAMELOG_FOR_SYSTEM_MEMO_TYPE
    TCHAR memo_contents_[MAX_MEMO_LENGTH + 1];//게임 로그를 위한 자료로 사용
};

#ifdef C_NA_0_20100520_ACSYSTEM_CHANGED
class Query_Char_Achievement_Select : public QueryForCharTransaction
{
	__DECL_SAFECUSTOMPOOL_PTR(Query_Char_Achievement_Select);
	enum {
		PARAM_ROW_NUM   = 1,
		PARAM_COL_NUM   = 2,

		RESULT_ROW_NUM  = MAX_ACHIEVEMENT_COUNT,
#ifdef _NA_001990_ACSYSTEM_ADD
        RESULT_COL_NUM	= 13,
#else
		RESULT_COL_NUM	= 11,
#endif //_NA_001990_ACSYSTEM_ADD
	};
	struct sQueryParam {
		int result_;
		CHARGUID char_guid_;
	} parameters_[PARAM_ROW_NUM];

	struct sQueryResult : public nsQuery::CharAchievementResultRecord
	{
	} result_records_[RESULT_ROW_NUM];

	ulong length_array_[RESULT_COL_NUM];

	_BEGIN_PARAM_DATA(sQueryParam, parameters_, PARAM_COL_NUM)
		_BINDING_PARAM(0, result_, SQL_PARAM_OUTPUT)
		_BINDING_PARAM(1, char_guid_, SQL_PARAM_INPUT)
	_END_PARAM_DATA();

	_BEGIN_BINDING_DATA(sQueryResult, result_records_, length_array_, RESULT_ROW_NUM, RESULT_COL_NUM)
		_BINDING_COLUMN(0, index_number_)
		_BINDING_COLUMN(1, is_reward_)
		_BINDING_COLUMN(2, object_value_[0])
		_BINDING_COLUMN(3, object_value_[1])
		_BINDING_COLUMN(4, object_value_[2])
		_BINDING_COLUMN(5, object_value_[3])
		_BINDING_COLUMN(6, object_value_[4])
		_BINDING_COLUMN(7, object_value_[5])
		_BINDING_COLUMN(8, object_value_[6])
		_BINDING_COLUMN(9, object_value_[7])
#ifdef _NA_001990_ACSYSTEM_ADD
        _BINDING_COLUMN(10, object_value_[8])
        _BINDING_COLUMN(11, object_value_[9])
#endif
	_END_BINDING_DATA();
public:
	bool ResultSuccess() const { return (parameters_[0].result_ == 0); }
	virtual const nsQuery::ProxyTransaction::eTransaction transaction_type() const {
		return nsQuery::ProxyTransaction::eTrans_Achievement_Select;
	}
}; 


class Query_Char_Achievement_Update : public QueryForCharTransaction
{
	__DECL_SAFECUSTOMPOOL_PTR(Query_Char_Achievement_Update);
	enum {
		PARAM_ROW_NUM   = 1,
#ifdef _NA_001990_ACSYSTEM_ADD
		PARAM_COL_NUM   = 14,
#else
		PARAM_COL_NUM   = 12,
#endif //_NA_001990_ACSYSTEM_ADD
	};

	struct sQueryParam : public nsQuery::CharAchievementInputParam
	{			
		int			result_;		
		CHARGUID	character_guid_;
	} parameters_[PARAM_ROW_NUM];

	_BEGIN_PARAM_DATA(sQueryParam, parameters_, PARAM_COL_NUM)
		_BINDING_PARAM( 0, result_			, SQL_PARAM_OUTPUT)
		_BINDING_PARAM( 1, character_guid_	, SQL_PARAM_INPUT)
		_BINDING_PARAM( 2, index_number_	, SQL_PARAM_INPUT)
		_BINDING_PARAM( 3, is_reward_		, SQL_PARAM_INPUT)
		_BINDING_PARAM( 4, object_value_[0]	, SQL_PARAM_INPUT)
		_BINDING_PARAM( 5, object_value_[1], SQL_PARAM_INPUT)
		_BINDING_PARAM( 6, object_value_[2], SQL_PARAM_INPUT)
		_BINDING_PARAM( 7, object_value_[3], SQL_PARAM_INPUT)
		_BINDING_PARAM( 8, object_value_[4], SQL_PARAM_INPUT)
		_BINDING_PARAM( 9, object_value_[5], SQL_PARAM_INPUT)
		_BINDING_PARAM( 10,	object_value_[6], SQL_PARAM_INPUT)
		_BINDING_PARAM( 11,	object_value_[7], SQL_PARAM_INPUT)
#ifdef _NA_001990_ACSYSTEM_ADD
        _BINDING_PARAM( 12,	object_value_[8], SQL_PARAM_INPUT)
        _BINDING_PARAM( 13,	object_value_[9], SQL_PARAM_INPUT)
#endif //_NA_001990_ACSYSTEM_ADD
	_END_PARAM_DATA();

	_INIT_BINDING_DATA();
public:
	bool ResultSuccess() const { return (parameters_[0].result_ == 0); }
	virtual const nsQuery::ProxyTransaction::eTransaction transaction_type() const {
		return nsQuery::ProxyTransaction::eTrans_Achievement_Update;
	}	
};

#endif //C_NA_0_20100520_ACSYSTEM_CHANGED

// 아이템 합성 또는 분해 내역 삽입 쿼리
class CompositionOrDecompositionHistoryInsertQuery : public QueryForUser
{
    __DECL_SAFECUSTOMPOOL_PTR(CompositionOrDecompositionHistoryInsertQuery)

    struct Parameter
    {
        enum { kNumberOfRow = 1, kNumberOfColumn = 1 };
        enum { kSuccess = 0 };
        typedef int Result;
        Result result;
    } parameters[Parameter::kNumberOfRow];

    _BEGIN_PARAM_DATA(Parameter, parameters, Parameter::kNumberOfColumn)
        _BINDING_PARAM(0, result, SQL_PARAM_OUTPUT)
    _END_PARAM_DATA()

    _INIT_BINDING_DATA()
public:
    bool IsSuccess() const
    {
        return !IsError() && (parameters[0].result == Parameter::kSuccess);
    }
};
// 합성 또는 분해 내역들 수정 쿼리
typedef CompositionOrDecompositionHistoryInsertQuery CompositionOrDecompositionHistoryUpdateQuery;
// 합성 또는 분해 내역들 얻어오기 쿼리
class CompositionOrDecompositionHistorySelectQuery : public QueryForUser
{
    __DECL_SAFECUSTOMPOOL_PTR(CompositionOrDecompositionHistorySelectQuery)

    struct Parameter
    {
        enum { kNumberOfRow = 1, kNumberOfColumn = 1 };
        enum { kSuccess = 0 };
        typedef int Result;
        Result result;
    } parameters[Parameter::kNumberOfRow];

    _BEGIN_PARAM_DATA(Parameter, parameters, Parameter::kNumberOfColumn)
        _BINDING_PARAM(0, result, SQL_PARAM_OUTPUT)
    _END_PARAM_DATA()

public:
    struct ResultSet
    {
        typedef ItemCompositionOrDecompositionHistory HistoryInfo;
        enum { kNumberOfRow = HistoryInfo::kMaxHistories };
        enum { kMaxUsedItemListStringSize = 255 };
        enum ColumnIndex
        {
            kHistoryID = 0, 
            kRequestType, 
            kIsMemorized, 
            kIsComposition, 
            kHistoryDateTime, 
            kUsedItemList, 
            kNumberOfColumn
        };
        HistoryInfo::HistoryID history_id; // 내역구분자
        HistoryInfo::RequestType request_type; // 요청종류
        BYTE is_memorized; // 기억여부
        BYTE is_composition; // 합성여부
        HistoryInfo::HistoryDateTime history_date_time; // 내역일자
        TCHAR used_item_list_string[kMaxUsedItemListStringSize]; // 사용된아이템목록문자열
    } result_set[ResultSet::kNumberOfRow];
    ulong column_length_array[ResultSet::kNumberOfColumn];

    _BEGIN_BINDING_DATA(
        ResultSet, result_set, column_length_array, ResultSet::kNumberOfRow, ResultSet::kNumberOfColumn)
        _BINDING_COLUMN(ResultSet::kHistoryID, history_id)
        _BINDING_COLUMN(ResultSet::kRequestType, request_type)
        _BINDING_COLUMN(ResultSet::kIsMemorized, is_memorized)
        _BINDING_COLUMN(ResultSet::kIsComposition, is_composition)
        _BINDING_COLUMN(ResultSet::kHistoryDateTime, history_date_time)
        _BINDING_COLUMN_PTR(ResultSet::kUsedItemList, used_item_list_string)
    _END_BINDING_DATA()
public:
    bool IsSuccess() const
    {
        return !IsError() && (parameters[0].result == Parameter::kSuccess);
    }
};

// 캐릭터 슬롯 확장 쿼리
class ExtendCharSlotQuery : public QueryForUser
{
public:
    ExtendCharSlotQuery() : extend_item_position_(INVALID_SLOT_POS)
    {
    }
    int GetResult() const
    {
        if (IsError())
        {
            enum { kEtcError = 2 };
            return kEtcError;
        }
        return parameters[0].result;
    }
    POSTYPE GetExtendItemPosition() const
    {
        return extend_item_position_;
    }
    void SetExtendItemPosition(const POSTYPE extend_item_position)
    {
        extend_item_position_ = extend_item_position;
    }

private:
    __DECL_SAFECUSTOMPOOL_PTR(ExtendCharSlotQuery)

    struct Parameter
    {
        enum { kNumberOfRow = 1, kNumberOfColumn = 1 };
        int result;
    } parameters[Parameter::kNumberOfRow];
    
    _BEGIN_PARAM_DATA(Parameter, parameters, Parameter::kNumberOfColumn)
        _BINDING_PARAM(0, result, SQL_PARAM_OUTPUT)
    _END_PARAM_DATA()
    
    _INIT_BINDING_DATA()

    POSTYPE extend_item_position_; // 슬롯 확장 아이템을 사용했다면 인벤토리 위치
};

#ifdef _NA_003027_20111013_HONOR_SYSTEM
class Query_HonorTitle_Select : public QueryForUser
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_HonorTitle_Select);
    enum {
        PARAM_COL_NUM   = 2,

        RESULT_ROW_NUM  = MAX_HONORTITLE_COUNT,
        RESULT_COL_NUM	= 1,
    };
    struct sQueryParam {
        int result_;
        CHARGUID char_guid_;
    } parameters;

    struct sQueryResult
    {
        HONOR_TITLE honor_title;
    } result_records_[RESULT_ROW_NUM];

    ulong length_array_[RESULT_COL_NUM];

    _BEGIN_PARAM_DATA(sQueryParam, &parameters, PARAM_COL_NUM)
        _BINDING_PARAM(0, result_, SQL_PARAM_OUTPUT)
        _BINDING_PARAM(1, char_guid_, SQL_PARAM_INPUT)
    _END_PARAM_DATA();

    _BEGIN_BINDING_DATA(sQueryResult, result_records_, length_array_, RESULT_ROW_NUM, RESULT_COL_NUM)
        _BINDING_COLUMN(0, honor_title.title_index_)
    _END_BINDING_DATA();
public:
}; 

class Query_HonorTitle_AddOrRemove : public QueryForUser
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_HonorTitle_AddOrRemove)

    struct Parameter
    {
        enum { kNumberOfColumn = 3 };
        int result;
        CHARGUID char_guid;
        WORD title_index;
    } parameters;

    _BEGIN_PARAM_DATA(Parameter, &parameters, Parameter::kNumberOfColumn)
        _BINDING_PARAM(0, result, SQL_PARAM_OUTPUT)
        _BINDING_PARAM(1, char_guid, SQL_PARAM_INPUT)
        _BINDING_PARAM(2, title_index, SQL_PARAM_INPUT)
    _END_PARAM_DATA()

    _INIT_BINDING_DATA()

public:
    BYTE sub_type;
};

class Query_FameRepute_Select : public QueryForUser
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_FameRepute_Select);
    enum {
        PARAM_COL_NUM   = 2,

        RESULT_ROW_NUM  = MAX_HONOR_POINT_INFO_COUNT,
        RESULT_COL_NUM	= 3,
    };
    struct sQueryParam {
        int result_;
        CHARGUID char_guid_;
    } parameters;

    struct sQueryResult
    {
        HONOR_POINT_INFO honor_points;
    } result_records_[RESULT_ROW_NUM];

    ulong length_array_[RESULT_COL_NUM];

    _BEGIN_PARAM_DATA(sQueryParam, &parameters, PARAM_COL_NUM)
        _BINDING_PARAM(0, result_, SQL_PARAM_OUTPUT)
        _BINDING_PARAM(1, char_guid_, SQL_PARAM_INPUT)
        _END_PARAM_DATA();

    _BEGIN_BINDING_DATA(sQueryResult, result_records_, length_array_, RESULT_ROW_NUM, RESULT_COL_NUM)
        _BINDING_COLUMN(0, honor_points.honor_point_code_)
        _BINDING_COLUMN(1, honor_points.honor_point_)
        _BINDING_COLUMN(2, honor_points.type)
        _END_BINDING_DATA();
public:
}; 

class Query_FameRepute_Set : public QueryForUser
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_FameRepute_Set);
    enum {
        PARAM_COL_NUM   = 5,
    };
    struct sQueryParam {
        int result_;
        CHARGUID char_guid_;
        BYTE type_;
        short code_;
        int point_;
    } parameters;

    _BEGIN_PARAM_DATA(sQueryParam, &parameters, PARAM_COL_NUM)
        _BINDING_PARAM(0, result_, SQL_PARAM_OUTPUT)
        _BINDING_PARAM(1, char_guid_, SQL_PARAM_INPUT)
        _BINDING_PARAM(2, type_, SQL_PARAM_INPUT)
        _BINDING_PARAM(3, code_, SQL_PARAM_INPUT)
        _BINDING_PARAM(4, point_, SQL_PARAM_INPUT)
    _END_PARAM_DATA();

    _INIT_BINDING_DATA()
public:
}; 

// DailyQuest
class Query_DailyQuest_Select : public QueryForUser
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_DailyQuest_Select);
    enum {
        PARAM_COL_NUM   = 2,

        RESULT_ROW_NUM  = MAX_DAILYQUEST_INFO_COUNT,
        RESULT_COL_NUM	= 4,
    };
    struct sQueryParam {
        int result_;
        CHARGUID char_guid_;
    } parameters;

    struct sQueryResult
    {
        WORD code;
        BYTE status;
        CHAR startdatetime[MAX_TIMEDATA_SIZE+1];
        CHAR enddatetime[MAX_TIMEDATA_SIZE+1];
    } result_records_[RESULT_ROW_NUM];

    ulong length_array_[RESULT_COL_NUM];

    _BEGIN_PARAM_DATA(sQueryParam, &parameters, PARAM_COL_NUM)
        _BINDING_PARAM(0, result_, SQL_PARAM_OUTPUT)
        _BINDING_PARAM(1, char_guid_, SQL_PARAM_INPUT)
        _END_PARAM_DATA();

    _BEGIN_BINDING_DATA(sQueryResult, result_records_, length_array_, RESULT_ROW_NUM, RESULT_COL_NUM)
        _BINDING_COLUMN(0, code)
        _BINDING_COLUMN(1, status)
        _BINDING_COLUMN_PTR(2, startdatetime)
        _BINDING_COLUMN_PTR(3, enddatetime)
        _END_BINDING_DATA();
public:
};

class Query_DailyQuest_Update : public QueryForUser
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_DailyQuest_Update);
    enum {
        PARAM_COL_NUM   = 6,
    };
    struct sQueryParam {
        int result_;
        CHARGUID char_guid_;
        BYTE type_;
        short code_;
        CHAR startdatetime[MAX_DATETIME_SIZE+1];
        CHAR enddatetime[MAX_DATETIME_SIZE+1];
    } parameters;

    _BEGIN_PARAM_DATA(sQueryParam, &parameters, PARAM_COL_NUM)
        _BINDING_PARAM(0, result_, SQL_PARAM_OUTPUT)
        _BINDING_PARAM(1, char_guid_, SQL_PARAM_INPUT)
        _BINDING_PARAM(2, code_, SQL_PARAM_INPUT)
        _BINDING_PARAM(3, type_, SQL_PARAM_INPUT)
        _BINDING_PARAM_PTR_WITH_SIZE(4, startdatetime,  MAX_DATETIME_SIZE, SQL_PARAM_INPUT)
        _BINDING_PARAM_PTR_WITH_SIZE(5, enddatetime,    MAX_DATETIME_SIZE, SQL_PARAM_INPUT)
    _END_PARAM_DATA();

    _INIT_BINDING_DATA()
public:
};
#endif //_NA_003027_20111013_HONOR_SYSTEM


#ifdef _NA_0_20110613_GAME_SHOP_RENEWAL
class Query_Character_Exist : public QueryForUser
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_Character_Exist)
public:
    enum
    { 
        PARAM_ROW_NUM	= 1,
        PARAM_COL_NUM   = 4,
    };

    struct sPARAM
    {
        int	m_UserGuid;
        int	m_CharGuid;
        int m_CharClass;
        int m_CharLevel;
    } pParam[PARAM_ROW_NUM];

    _BEGIN_PARAM_DATA		(sPARAM, pParam, PARAM_COL_NUM)    
        _BINDING_PARAM		(0, m_UserGuid, SQL_PARAM_OUTPUT)
        _BINDING_PARAM		(1, m_CharGuid, SQL_PARAM_OUTPUT)
        _BINDING_PARAM      (2, m_CharClass, SQL_PARAM_OUTPUT)
        _BINDING_PARAM      (3, m_CharLevel, SQL_PARAM_OUTPUT)
        _END_PARAM_DATA			()

        _INIT_BINDING_DATA();
public:
    inline void Clear() { ZeroMemory(pParam, sizeof(pParam)); } // Added by __NA000000_SHOP_USERID_50_PASSING_PROC__
    inline BOOL ResultSuccess() { return (pParam[0].m_UserGuid!=0); }
    void SetParamCharName(const TCHAR* p)
    { 
        _tcsncpy(m_tszParamCharName, p, _countof(m_tszParamCharName));
        m_tszParamCharName[_countof(m_tszParamCharName) - 1] = _T('\0');
    }
    inline const TCHAR* GetParamCharName() const { return m_tszParamCharName; }
    TCHAR m_tszParamCharName[MAX_CHARNAME_LENGTH + 1];
};
#endif // _NA_0_20110613_GAME_SHOP_RENEWAL

#ifdef _NA004034_20120102_POINT_WALLET_SYSTEM
class Query_PointWallet_Select : public QueryForUser
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_PointWallet_Select);
    enum {
        PARAM_COL_NUM   = 2,
        RESULT_ROW_NUM  = MAX_POINTWALLET_COUNT,
        RESULT_COL_NUM	= 2,
    };

    struct sQueryParam {
        int result_;
        CHARGUID char_guid_;
    } parameters;

    struct sQueryResult
    {
        DWORD code;
        DWORD point;
    } result_records_[RESULT_ROW_NUM];

    ulong length_array_[RESULT_COL_NUM];

    _BEGIN_PARAM_DATA(sQueryParam, &parameters, PARAM_COL_NUM)
        _BINDING_PARAM(0, result_, SQL_PARAM_OUTPUT)
        _BINDING_PARAM(1, char_guid_, SQL_PARAM_INPUT)
    _END_PARAM_DATA();

    _BEGIN_BINDING_DATA(sQueryResult, result_records_, length_array_, RESULT_ROW_NUM, RESULT_COL_NUM)
        _BINDING_COLUMN(0, code)
        _BINDING_COLUMN(1, point)
    _END_BINDING_DATA();
public:
}; 

class Query_PointWallet_Update : public QueryForUser
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_PointWallet_Update);

    enum {
        PARAM_COL_NUM   = 4,
    };

    struct sQueryParam {
        int result_;
        CHARGUID char_guid_;
        DWORD code_;
        DWORD point_;
    } parameters;

    _BEGIN_PARAM_DATA(sQueryParam, &parameters, PARAM_COL_NUM)
        _BINDING_PARAM(0, result_, SQL_PARAM_OUTPUT)
        _BINDING_PARAM(1, char_guid_, SQL_PARAM_INPUT)
        _BINDING_PARAM(2, code_, SQL_PARAM_INPUT)
        _BINDING_PARAM(3, point_, SQL_PARAM_INPUT)
    _END_PARAM_DATA();

    _INIT_BINDING_DATA()
public:
};
#endif //_NA004034_20120102_POINT_WALLET_SYSTEM

#ifdef _NA004034_20120102_GUILD_POINT_SYSTEM
class Query_GuildPoint_Donation : public QueryForUser
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_GuildPoint_Donation);

    struct sQueryParam {
        int result_;
        int donation_count_of_day_;
    } parameters;

    _BEGIN_PARAM_DATA(sQueryParam, &parameters, 2 /*PARAM_COL_NUM*/)
        _BINDING_PARAM(0, result_, SQL_PARAM_OUTPUT);
        _BINDING_PARAM(1, donation_count_of_day_, SQL_PARAM_OUTPUT);
    _END_PARAM_DATA();

    _INIT_BINDING_DATA();

public:
    INT donation_item_count_;
};

struct DBRObject : public QueryResult
{
    BYTE packet_type;
};

class Query_GuildPoint_Select : public DBRObject
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_GuildPoint_Select);

    enum { 
        RESULT_COL_NUM = 4, 
        RESULT_ROW_NUM = MAX_GUILD_MEMBER_NUM, 
    };

    struct sQueryParam {
        int result_;
        DWORD guid_;
        DWORD code_;
    } parameters;

    struct sQueryResult
    {
        DWORD guid;
        DWORD uuid;
        DWORD code;
        DWORD point;
    } result_records_[RESULT_ROW_NUM];

    ulong length_array_[RESULT_COL_NUM];

    _BEGIN_PARAM_DATA(sQueryParam, &parameters, 3 /*PARAM_COL_NUM*/)
        _BINDING_PARAM(0, result_, SQL_PARAM_OUTPUT);
        _BINDING_PARAM(1, guid_, SQL_PARAM_INPUT);
        _BINDING_PARAM(2, code_, SQL_PARAM_INPUT);
    _END_PARAM_DATA();

    _BEGIN_BINDING_DATA(sQueryResult, result_records_, length_array_, RESULT_ROW_NUM, RESULT_COL_NUM)
        _BINDING_COLUMN(0, guid)
        _BINDING_COLUMN(1, uuid)
        _BINDING_COLUMN(2, code)
        _BINDING_COLUMN(3, point)
    _END_BINDING_DATA();
public:
};

class Query_GuildPoint_Change : public DBRObject
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_GuildPoint_Change);

    struct sQueryParam {
        int result_;
    } parameters;

    _BEGIN_PARAM_DATA(sQueryParam, &parameters, 1 /*PARAM_COL_NUM*/)
        _BINDING_PARAM(0, result_, SQL_PARAM_OUTPUT);
    _END_PARAM_DATA();

    _INIT_BINDING_DATA();
public:
};
typedef Query_GuildPoint_Change Query_GuildPoint_Delete;

class Query_GuildLevel_Change : public DBRObject
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_GuildLevel_Change);

    struct sQueryParam {
        int result_;
    } parameters;

    _BEGIN_PARAM_DATA(sQueryParam, &parameters, 1 /*PARAM_COL_NUM*/)
        _BINDING_PARAM(0, result_, SQL_PARAM_OUTPUT);
    _END_PARAM_DATA();

    _INIT_BINDING_DATA();
public:
};

class Query_GuildRanking_Select : public QueryResult
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_GuildRanking_Select);

    struct sQueryParam {
        int result_;
    } parameters;

    struct sQueryResult
    {
        GUILD_RANKING_INFO guild_rank_info;
    } result_records_[GUILD_RANKING_INFO::MAX_GUILD_RANKING_INFO];

    ulong length_array_[GUILD_RANKING_INFO::MAX_GUILD_RANKING_INFO];

    _BEGIN_PARAM_DATA(sQueryParam, &parameters, 1 /*PARAM_COL_NUM*/)
        _BINDING_PARAM(0, result_, SQL_PARAM_OUTPUT);
    _END_PARAM_DATA();

    _BEGIN_BINDING_DATA(sQueryResult, result_records_, length_array_, \
            GUILD_RANKING_INFO::MAX_GUILD_RANKING_INFO, 7 /*RESULT_COL_NUM*/)
        _BINDING_COLUMN(0, guild_rank_info.guild_guid)
        _BINDING_COLUMN(1, guild_rank_info.guild_name)
        _BINDING_COLUMN(2, guild_rank_info.master_name)
        _BINDING_COLUMN(3, guild_rank_info.guild_level)
        _BINDING_COLUMN(4, guild_rank_info.domination_count)
        _BINDING_COLUMN(5, guild_rank_info.guild_point)
        _BINDING_COLUMN(6, guild_rank_info.member_count)
    _END_BINDING_DATA();
public:
};
#endif //_NA004034_20120102_GUILD_POINT_SYSTEM

#ifdef _NA_004034_20120102_GUILD_INTRODUCTION
class Query_GuildIntroduction_Update : public DBRObject
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_GuildIntroduction_Update);

    struct sQueryParam {
        int result_;
    } parameters;

    _BEGIN_PARAM_DATA(sQueryParam, &parameters, 1 /*PARAM_COL_NUM*/)
        _BINDING_PARAM(0, result_, SQL_PARAM_OUTPUT);
    _END_PARAM_DATA();

    _INIT_BINDING_DATA();
public:
};

class Query_GuildIntroduction_Select : public DBRObject
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_GuildIntroduction_Select);

    enum { RESULT_ROW_NUM = 1 };

    struct sQueryParam {
        int result_;
        CHAR* introduction_;
    } parameters;

    struct sQueryResult
    {
        CHAR introduction_[MAX_GUILD_INTRODUCTION_LENGTH+1];
    } result_records[RESULT_ROW_NUM];

    ulong length_array[RESULT_ROW_NUM];

    GUILDGUID guild_guid;

    _BEGIN_PARAM_DATA(sQueryParam, &parameters, 2 /*PARAM_COL_NUM*/)
        _BINDING_PARAM(0, result_, SQL_PARAM_OUTPUT);
        _BINDING_PARAM_PTR(1, introduction_, SQL_PARAM_OUTPUT);
    _END_PARAM_DATA();

    _BEGIN_BINDING_DATA(sQueryResult, result_records, length_array, RESULT_ROW_NUM, 1)
        _BINDING_COLUMN_PTR(0, introduction_)
    _END_BINDING_DATA();
public:
};
#endif //_NA_004034_20120102_GUILD_INTRODUCTION

class Query_Character_NameChange_Request : public QueryForUser
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_Character_NameChange_Request);

    struct sQueryParam {
        int result_;
        CHARGUID char_guid_;
    } parameters;

    _BEGIN_PARAM_DATA(sQueryParam, &parameters, 2/*PARAM_COL_NUM*/)
        _BINDING_PARAM(0, result_, SQL_PARAM_OUTPUT)
        _BINDING_PARAM(1, char_guid_, SQL_PARAM_INPUT)
    _END_PARAM_DATA();

    _INIT_BINDING_DATA();
public:
    SLOTIDX slot_index;
    POSTYPE item_pos;
}; 

//~! _NA_000000_20130429_DOMINATION_PENALTY
class Query_GuildPenalty_Update : public DBRObject
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_GuildPenalty_Update);

    struct sQueryParam {
        int result_;
    } parameters;

    _BEGIN_PARAM_DATA(sQueryParam, &parameters, 1/*PARAM_COL_NUM*/)
        _BINDING_PARAM(0, result_, SQL_PARAM_OUTPUT)
    _END_PARAM_DATA();

    _INIT_BINDING_DATA();
};
//~!_NA_000000_20130429_DOMINATION_PENALTY

#ifdef _NA_000000_20140120_SMART_NPC_SYSTEM
class Query_Attendance_Update : public QueryForUser
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_Attendance_Update);

    struct sQueryParam {
        int result_;
    } parameters;

    _BEGIN_PARAM_DATA(sQueryParam, &parameters, 1/*PARAM_COL_NUM*/)
        _BINDING_PARAM(0, result_, SQL_PARAM_OUTPUT)
    _END_PARAM_DATA();

    _INIT_BINDING_DATA();
public:
    DWORD CID;
    DWORD NID;
    DWORD EID;
    DWORD STEP;
};
#endif //_NA_000000_20140120_SMART_NPC_SYSTEM

#ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION
class Query_Attendance_Update_Mondschien : public QueryResult
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_Attendance_Update_Mondschien);

    struct sQueryParam {
        int result_;
    } parameters;

    _BEGIN_PARAM_DATA(sQueryParam, &parameters, 1/*PARAM_COL_NUM*/)
        _BINDING_PARAM(0, result_, SQL_PARAM_OUTPUT)
        _END_PARAM_DATA();

    _INIT_BINDING_DATA();
public:
    GUILDGUID guild_guid;
};
#endif //_NA_007128_20140326_MONDSCHIEN_DOMINATION

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION

#endif //_NA_007330_20140620_GUILD_SYSTEM_EXTENSION

class Query_GuildJoinRequest_Insert : public DBRObject
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_GuildJoinRequest_Insert);

    enum { RESULT_ROW_NUM = 1 };

    struct sQueryParam {
        int         result_;
        CHARGUID    char_guid_;
        GUILDGUID   guild_guid_;
    } parameters;

    struct sQueryResult
    {
        CHAR request_time_[MAX_DATETIME_SIZE+1];
    } result_records[RESULT_ROW_NUM];

    ulong length_array[RESULT_ROW_NUM];

    _BEGIN_PARAM_DATA(sQueryParam, &parameters, 3 /*PARAM_COL_NUM*/)
        _BINDING_PARAM(0, result_, SQL_PARAM_OUTPUT);
        _BINDING_PARAM(1, char_guid_, SQL_PARAM_INPUT);
        _BINDING_PARAM(2, guild_guid_, SQL_PARAM_INPUT);
    _END_PARAM_DATA();

    _BEGIN_BINDING_DATA(sQueryResult, result_records, length_array, RESULT_ROW_NUM, 1)
        _BINDING_COLUMN_PTR(0, request_time_);
    _END_BINDING_DATA();

    GUILD_JOIN_REQUEST_INFO request_info_;

public:
    void SetGuildJoinRequestInfo(GUILD_JOIN_REQUEST_INFO request_info)
    {
        request_info_ = request_info;
    }
    GUILD_JOIN_REQUEST_INFO GetGuildJoiinRequestInfo()
    {
        return request_info_;
    }
};

class Query_GuildJoinRequest_Delete : public DBRObject
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_GuildJoinRequest_Delete);

    struct sQueryParam {
        int         result_;
        CHARGUID    char_guid_;
        GUILDGUID   guild_guid_;
    } parameters;

    _BEGIN_PARAM_DATA(sQueryParam, &parameters, 3 /*PARAM_COL_NUM*/)
        _BINDING_PARAM(0, result_, SQL_PARAM_OUTPUT);
        _BINDING_PARAM(1, char_guid_, SQL_PARAM_INPUT);
        _BINDING_PARAM(2, guild_guid_, SQL_PARAM_INPUT);
    _END_PARAM_DATA();

    _INIT_BINDING_DATA();

    CHARGUID char_guid_;
    CHARGUID target_guid_;
    GUILDGUID guild_guid_;
    eGUILD_JOIN_REJECT_TYPE reject_type_;

public:
    void SetCharGuid(CHARGUID char_guid)
    {
        char_guid_ = char_guid;
    }
    CHARGUID GetCharGuid()
    {
        return char_guid_;
    }
    void SetTargetGuid(CHARGUID target_guid)
    {
        target_guid_ = target_guid;
    }
    CHARGUID GetTargetGuid()
    {
        return target_guid_;
    }
    void SetGuildGuid(GUILDGUID guild_guid)
    {
        guild_guid_ = guild_guid;
    }
    GUILDGUID GetGuildGuid()
    {
        return guild_guid_;
    }
    void SetRejectType(eGUILD_JOIN_REJECT_TYPE reject_type)
    {
        reject_type_ = reject_type;
    }
    eGUILD_JOIN_REJECT_TYPE GetRejectType()
    {
        return reject_type_;
    }
};

class Query_GuildJoinRequest_Select : public DBRObject
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_GuildJoinRequest_Select);

    enum { RESULT_ROW_NUM = MAX_GUILD_JOIN_REQUEST_NUM };

    struct sQueryParam {
        int result_;
        GUILDGUID guild_guid_;
    } parameters;

    struct sQueryResult
    {
        CHARGUID    char_guid_;
        TCHAR       char_name_[MAX_CHARNAME_LENGTH + 1];
        LEVELTYPE   char_level_;
        BYTE        char_class_;
        CHAR        request_time_[MAX_DATETIME_SIZE + 1];
        GUILDGUID   guild_guid_;
    } result_records[RESULT_ROW_NUM];

    ulong length_array[RESULT_ROW_NUM];

    GUILDGUID guild_guid_;

    _BEGIN_PARAM_DATA(sQueryParam, &parameters, 2 /*PARAM_COL_NUM*/)
        _BINDING_PARAM(0, result_, SQL_PARAM_OUTPUT);
        _BINDING_PARAM(1, guild_guid_, SQL_PARAM_INPUT);
    _END_PARAM_DATA();

    _BEGIN_BINDING_DATA(sQueryResult, result_records, length_array, RESULT_ROW_NUM, 6)
        _BINDING_COLUMN(0, char_guid_);
        _BINDING_COLUMN_PTR(1, char_name_);
        _BINDING_COLUMN(2, char_level_);
        _BINDING_COLUMN(3, char_class_);
        _BINDING_COLUMN_PTR(4, request_time_);
        _BINDING_COLUMN(5, guild_guid_);
    _END_BINDING_DATA();

public:
    void SetGuildGuid(GUILDGUID guild_guid)
    {
        guild_guid_ = guild_guid;
    }
    GUILDGUID GetGuildGuid()
    {
        return guild_guid_;
    }
};


class Query_GuildFacility_Update : public DBRObject
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_GuildFacility_Update);

    struct sQueryParam {
        int         result_;
        GUILDGUID   guild_guid_;
        int         facility_code_;
        int         add_level_;
    } parameters;

    _BEGIN_PARAM_DATA(sQueryParam, &parameters, 4 /*PARAM_COL_NUM*/)
        _BINDING_PARAM(0, result_, SQL_PARAM_OUTPUT);
        _BINDING_PARAM(1, guild_guid_, SQL_PARAM_INPUT);
        _BINDING_PARAM(2, facility_code_, SQL_PARAM_INPUT);
        _BINDING_PARAM(3, add_level_, SQL_PARAM_INPUT);
    _END_PARAM_DATA();

    _INIT_BINDING_DATA();

    CHARGUID  char_guid_;
    GUILDGUID guild_guid_;
    FLEVEL    current_level_;
    FLEVEL    add_level_;
    FCODE     facility_code_;

public:
    void SetCharGuid(CHARGUID char_guid){ char_guid_ = char_guid; }
    CHARGUID GetCharGuid(){ return char_guid_; }
    void SetCurLevel(FLEVEL cur_level){ current_level_ = cur_level; }
    FLEVEL GetCurLevel(){ return current_level_; }
    void SetAddLevel(int add_level){ add_level_ = add_level; }
    FLEVEL GetAddLevel(){ return add_level_; }
    void SetGuildGuid(GUILDGUID guild_guid){ guild_guid_ = guild_guid; }
    GUILDGUID GetGuildGuid(){ return guild_guid_; }
    void SetFacilityCode(FCODE facility_code){ facility_code_ = facility_code; }
    FCODE GetFacilityCode(){ return facility_code_; }
};

class Query_GuildFacility_Select : public DBRObject
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_GuildFacility_Select);

    enum { RESULT_ROW_NUM = MAX_GUILD_FACILITY };

    struct sQueryParam {
        int result_;
        GUILDGUID guild_guid_;
    } parameters;

    struct sQueryResult
    {
        int facility_code;
        int facility_level;
    } result_records[RESULT_ROW_NUM];

    ulong length_array[RESULT_ROW_NUM];

    GUILDGUID guild_guid_;

    _BEGIN_PARAM_DATA(sQueryParam, &parameters, 2 /*PARAM_COL_NUM*/)
        _BINDING_PARAM(0, result_, SQL_PARAM_OUTPUT);
        _BINDING_PARAM(1, guild_guid_, SQL_PARAM_INPUT);
    _END_PARAM_DATA();

    _BEGIN_BINDING_DATA(sQueryResult, result_records, length_array, RESULT_ROW_NUM, 2)
        _BINDING_COLUMN(0, facility_code);
        _BINDING_COLUMN(1, facility_level);
    _END_BINDING_DATA();

public:
    void SetGuildGuid(GUILDGUID guild_guid)
    {
        guild_guid_ = guild_guid;
    }
    GUILDGUID GetGuildGuid()
    {
        return guild_guid_;
    }
};

#ifdef _NA_008012_20150204_SUN_RANKING_SYSTEM

class Query_SunRanking_Update : public DBRObject
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_SunRanking_Update);

    enum
    {
        RESULT_COL_NUM = 1,
    };

    struct sQueryParam {
        int         result_;
    } parameters;

    _BEGIN_PARAM_DATA(sQueryParam, &parameters, 1 /*PARAM_COL_NUM*/)
        _BINDING_PARAM(0, result_, SQL_PARAM_OUTPUT);
    _END_PARAM_DATA();

    _INIT_BINDING_DATA();

    SunRankingPlayerInfo info_;

public:
    void SetRankInfo(SunRankingPlayerInfo into){ info_ = into; }
    SunRankingPlayerInfo GetRankInfo(){ return info_; }
};


class Query_SunRanking_Select : public DBRObject
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_SunRanking_Select);
    Query_SunRanking_Select()
    {
        make_one_row = false;
    }

    enum
    {
        RESULT_COL_NUM = 6,
        RESULT_ROW_NUM = 1,
    };

    struct sQueryParam
    {
        int         result_;
    } parameters;

    struct sQueryResult
    {
        CHARGUID  char_guid;
        int       ranking_type;
        SCORE     score;
        char      char_name[MAX_CHARNAME_LENGTH + 1];
        int       class_code;
        GUILDGUID guild_guid;
        
    } result_records_[RESULT_ROW_NUM];

    ulong uLength[RESULT_COL_NUM];

    _BEGIN_PARAM_DATA(sQueryParam, &parameters, 1 /*PARAM_COL_NUM*/)
        _BINDING_PARAM(0, result_, SQL_PARAM_OUTPUT);
    _END_PARAM_DATA();

    virtual inline ULONG GetMaxRowNum() const 
    {
        /* unused */
        return 1000000; 
    }
    virtual inline ULONG GetMaxColNum() const { return RESULT_COL_NUM; }
    virtual inline BYTE* GetBuffer(ULONG ulRow) 
    { 
        return (BYTE *)&result_records_[ulRow]; 
    }
    virtual inline ULONG* GetColumnLengthPtr(ULONG col)
    {
        return &uLength[col];
    }
    virtual VOID GetColumnInfo(ULONG row, ULONG col, ULONG*& pColPtr, LONG& lSize, SHORT& sColType);

    std::map<CHARGUID, SunRankingPlayerInfo> infos;
    bool make_one_row;

public:
    VOID GetLastColumnInfo();
    std::map<CHARGUID, SunRankingPlayerInfo>& GetInfos() { return infos; };
};

class Query_SunRankingGrade_Init : public DBRObject
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_SunRankingGrade_Init)

    struct sQueryParam
    {
        int         result_;
    } parameters;

    _BEGIN_PARAM_DATA(sQueryParam, &parameters, 1 /*PARAM_COL_NUM*/)
        _BINDING_PARAM(0, result_, SQL_PARAM_OUTPUT);
    _END_PARAM_DATA();

    _INIT_BINDING_DATA ()
};

class Query_SunRankingGrade_Insert : public DBRObject
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_SunRankingGrade_Insert)

    struct sQueryParam
    {
        int         result_;
    } parameters;

    _BEGIN_PARAM_DATA(sQueryParam, &parameters, 1 /*PARAM_COL_NUM*/)
        _BINDING_PARAM(0, result_, SQL_PARAM_OUTPUT);
    _END_PARAM_DATA();

    _INIT_BINDING_DATA ()

    eSUN_RANKING_TYPE m_rank_type;
    eSUN_RANKING_GRADE_TYPE m_grade;
public:
    void SetSunRankingType(eSUN_RANKING_TYPE rank_type){ m_rank_type = rank_type; };
    eSUN_RANKING_TYPE GetSunRankingType(){ return m_rank_type; };
    void SetSunRankingGrade(eSUN_RANKING_GRADE_TYPE grade){ m_grade = grade; };
    eSUN_RANKING_GRADE_TYPE GetSunRankingGrade(){ return m_grade; };
};

class Query_SunRanking_Init : public DBRObject
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_SunRanking_Init)

    struct sQueryParam
    {
        int         result_;
    } parameters;

    _BEGIN_PARAM_DATA(sQueryParam, &parameters, 1 /*PARAM_COL_NUM*/)
        _BINDING_PARAM(0, result_, SQL_PARAM_OUTPUT);
    _END_PARAM_DATA();

    _INIT_BINDING_DATA ()
};

class Query_SunRankingGrade_Select : public DBRObject
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_SunRankingGrade_Select);
    Query_SunRankingGrade_Select()
    {
        make_one_row = false;
    }

    enum
    {
        RESULT_COL_NUM = 4,
        RESULT_ROW_NUM = 1,
    };

    struct sQueryParam
    {
        int         result_;
    } parameters;

    struct sQueryResult
    {
        CHARGUID  char_guid;
        int       ranking_type;
        int       grade;
        SCORE     score;

    } result_records_[RESULT_ROW_NUM];

    ulong uLength[RESULT_COL_NUM];

    _BEGIN_PARAM_DATA(sQueryParam, &parameters, 1 /*PARAM_COL_NUM*/)
        _BINDING_PARAM(0, result_, SQL_PARAM_OUTPUT);
    _END_PARAM_DATA();

    virtual inline ULONG GetMaxRowNum() const 
    {
        /* unused */
        return 1000000; 
    }
    virtual inline ULONG GetMaxColNum() const { return RESULT_COL_NUM; }
    virtual inline BYTE* GetBuffer(ULONG ulRow) 
    { 
        return (BYTE *)&result_records_[ulRow]; 
    }
    virtual inline ULONG* GetColumnLengthPtr(ULONG col)
    {
        return &uLength[col];
    }
    virtual VOID GetColumnInfo(ULONG row, ULONG col, ULONG*& pColPtr, LONG& lSize, SHORT& sColType);

    std::map<std::pair<CHARGUID, int>, SunRankingGradeInfo> infos;
    bool make_one_row;

public:
    VOID GetLastColumnInfo();
    std::map<std::pair<CHARGUID, int>, SunRankingGradeInfo>& GetInfos() { return infos; };
};

class Query_StatueCharInfo_Select : public DBRObject
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_StatueCharInfo_Select)

    struct sQueryParam {
        int result_;
        eSTATUE_TYPE statue_type;
        eSTATUE_INDEX statue_index;
        CHARGUID char_guid;
        SCORE    score;
    } parameters;

    _BEGIN_PARAM_DATA(sQueryParam, &parameters, 5 /*PARAM_COL_NUM*/)
        _BINDING_PARAM(0, result_, SQL_PARAM_OUTPUT);
        _BINDING_PARAM(1, statue_type, SQL_PARAM_INPUT);
        _BINDING_PARAM(2, statue_index, SQL_PARAM_INPUT);
        _BINDING_PARAM(3, char_guid, SQL_PARAM_INPUT);
        _BINDING_PARAM(4, score, SQL_PARAM_INPUT);
    _END_PARAM_DATA();

    _INIT_BINDING_DATA ()
};

class Query_StatueItemSlotInfo_Insert : public DBRObject
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_StatueItemSlotInfo_Insert)

    struct sQueryParam {
        int result_;
        eSTATUE_TYPE statue_type;
        eSTATUE_INDEX statue_index;
        CHARGUID char_guid;
    } parameters;

    _BEGIN_PARAM_DATA(sQueryParam, &parameters, 4 /*PARAM_COL_NUM*/)
        _BINDING_PARAM(0, result_, SQL_PARAM_OUTPUT);
        _BINDING_PARAM(1, statue_type, SQL_PARAM_INPUT);
        _BINDING_PARAM(2, statue_index, SQL_PARAM_INPUT);
        _BINDING_PARAM(3, char_guid, SQL_PARAM_INPUT);
    _END_PARAM_DATA();

    _INIT_BINDING_DATA ()
};

class Query_StatueItemSlotInfo_Select : public DBRObject
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_StatueItemSlotInfo_Select)

    static const int kMaxRows = max(MAX_INVENTORY_SLOT_NUM,
        max(MAX_EQUIPMENT_SLOT_NUM,
        max(MAX_WAREHOUSE_SLOT_NUM,
        max(MAX_GUILD_WAREHOUSE_SLOT_NUM,
        0))));

    enum {
        PARAM_ROW_NUM   = 1,
        PARAM_COL_NUM   = 1, // @RC

        RESULT_ROW_NUM  = kMaxRows,
        RESULT_COL_NUM	= 10,
    };

    struct sQueryParam {
        int result_;
    } parameters_[PARAM_ROW_NUM];

    struct sQueryResult : public nsQuery::ItemSlotResultRecord
    {
        int statue_type;
        int statue_index;
        CHARGUID char_guid;
    } result_records_[RESULT_ROW_NUM];

    ulong length_array_[RESULT_COL_NUM];

    _BEGIN_PARAM_DATA(sQueryParam, parameters_, PARAM_COL_NUM)
        _BINDING_PARAM(0, result_, SQL_PARAM_OUTPUT)
    _END_PARAM_DATA();

    _BEGIN_BINDING_DATA(sQueryResult, result_records_, length_array_, RESULT_ROW_NUM, RESULT_COL_NUM)
        _BINDING_COLUMN(0, statue_type)
        _BINDING_COLUMN(1, statue_index)
        _BINDING_COLUMN(2, char_guid)
        _BINDING_COLUMN(3, slot_type_)
        _BINDING_COLUMN(4, pos_)
        _BINDING_COLUMN(5, serial_)
        _BINDING_COLUMN(6, code_)
        _BINDING_COLUMN(7, dura_or_num_)
        _BINDING_COLUMN(8, version_)
        _BINDING_COLUMN_PTR(9, option_stream_)
    _END_BINDING_DATA();
public:
    bool ResultSuccess() const { return (parameters_[0].result_ == 0); }
};
class Query_Statue_Char_Select : public QueryForUser
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_Statue_Char_Select);
    //
    enum 
    {
        _MAKE_REORDERING_BINDING_NO_START_(m_UserGuid, 0),
        _MAKE_REORDERING_BINDING_NO_(m_bySlot),
        _MAKE_REORDERING_BINDING_NO_(m_byClass),
        _MAKE_REORDERING_BINDING_NO_(m_CharGuid),
        _MAKE_REORDERING_BINDING_NO_PTR_(result_info_char_name_),
        _MAKE_REORDERING_BINDING_NO_(m_byHeight),
        _MAKE_REORDERING_BINDING_NO_(m_byFace),
        _MAKE_REORDERING_BINDING_NO_(m_byHair),
        _MAKE_REORDERING_BINDING_NO_PTR_(result_info_guild_name_),
        _MAKE_REORDERING_BINDING_NO_(m_GuildGuid),
        _MAKE_REORDERING_BINDING_NO_(gender), 
        _MAKE_REORDERING_BINDING_NO_(m_PatternMarkIndex),
        _MAKE_REORDERING_BINDING_NO_(m_BackGroundMarkIndex),
        _MAKE_REORDERING_BINDING_NO_(m_total_equip_score),
        _MAKE_REORDERING_BINDING_NO_COUNTS_() // end of records. it equal to number of records
    };

    struct Parameter
    {
        enum { kNumberOfRow = 1, kNumberOfColumn = 2 };
        int result_;
        CHARGUID char_guid;
    } parameters[Parameter::kNumberOfRow];

    _BEGIN_PARAM_DATA(Parameter, parameters, Parameter::kNumberOfColumn)
        _BINDING_PARAM(0, result_, SQL_PARAM_OUTPUT);
        _BINDING_PARAM(1, char_guid, SQL_PARAM_INPUT)
    _END_PARAM_DATA()
public:
    eSTATUE_TYPE      statue_type;
    eSTATUE_INDEX     statue_index;
    _EQUIP_TOTAL_INFO equip_info;

    enum
    {
        RESULT_ROW_NUM = 1,
        RESULT_COL_NUM = _MAKE_REORDERING_BINDING_NO_COUNTS_(),
    };
    //
    struct sQUERY_RESULT : public sQUERY_CHARACTER
    {
        int m_PatternMarkIndex;
        int m_BackGroundMarkIndex;
        SCORE m_total_equip_score;
        int _dummy_field_;
    } pResult[RESULT_ROW_NUM];
    //
    ulong uLength[RESULT_COL_NUM];
    //
    //
    _BEGIN_BINDING_DATA(sQUERY_RESULT, pResult, uLength, RESULT_ROW_NUM, RESULT_COL_NUM)
        _REORDERING_BINDING_COLUMN(m_UserGuid)
        _REORDERING_BINDING_COLUMN(m_bySlot)
        _REORDERING_BINDING_COLUMN(m_byClass)
        _REORDERING_BINDING_COLUMN(m_CharGuid)
        _REORDERING_BINDING_COLUMN_PTR(result_info_char_name_)
        _REORDERING_BINDING_COLUMN(m_byHeight)
        _REORDERING_BINDING_COLUMN(m_byFace)
        _REORDERING_BINDING_COLUMN(m_byHair)
        _REORDERING_BINDING_COLUMN_PTR(result_info_guild_name_)
        _REORDERING_BINDING_COLUMN(m_GuildGuid)
        _REORDERING_BINDING_COLUMN(gender)
        _REORDERING_BINDING_COLUMN(m_PatternMarkIndex)
        _REORDERING_BINDING_COLUMN(m_BackGroundMarkIndex)
        _REORDERING_BINDING_COLUMN(m_total_equip_score)
    _END_BINDING_DATA();
    //
}; //end of class 'Query_User_Select'
#endif // _NA_008012_20150204_SUN_RANKING_SYSTEM

#endif // __GAME_SERVER_QUERY_H__
