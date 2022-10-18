#pragma once


#include <Define.h>
#include "StructInQuery.h"

// 친구 목록 정보 select
class Query_Ranking_Select : public QueryResult
{
	__DECL_SAFECUSTOMPOOL_PTR(Query_Ranking_Select)
public:
	enum
	{ 
		PARAM_ROW_NUM	= 1,
#ifdef _NA_006801_20130705_RENEWAL_RANKINGSYSTEM
        PARAM_COL_NUM	= 3,
#else
        PARAM_COL_NUM	= 4,
#endif //_NA_006801_20130705_RENEWAL_RANKINGSYSTEM

		// 1. 명예의 전당
		// 2. 지난주 순위1,2,3
		// 3. 현재주 순위1~50
#ifdef _NA_006801_20130705_RENEWAL_RANKINGSYSTEM
        RESULT_ROW_NUM	= MAX_MISSION_NUM*3 + MAX_MISSION_NUM*50,
#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
        RESULT_COL_NUM	= 5 + 6*MAX_PARTYMEMBER_NUM,
#else
        RESULT_COL_NUM	= 4 + 6*MAX_PARTYMEMBER_NUM,
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
#else
        RESULT_ROW_NUM	= MAX_MISSION_NUM*1 + MAX_MISSION_NUM*3 + MAX_MISSION_NUM*50,
#endif //_NA_006801_20130705_RENEWAL_RANKINGSYSTEM
	};
#ifdef _NA_006801_20130705_RENEWAL_RANKINGSYSTEM
    enum {LAST_ROWS=0,TODAY_ROWS=1,MAX_ROWS=2 };
#else
    enum { FAME_ROWS=0,LAST_ROWS=1,TODAY_ROWS=2,MAX_ROWS=3 };
#endif //_NA_006801_20130705_RENEWAL_RANKINGSYSTEM
	struct sPARAM
	{
		SHORT	Result;
		SHORT	m_Rows[MAX_ROWS];
	} pParam[PARAM_ROW_NUM];

	struct sQUERY
	{
		RANKINGNO	m_MissionNo;
		RANKINGKEY	m_Key;
#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
        RANKINGTYPE ranking_type;
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
		DWORD		m_ClearTime;
		INT64		m_RegisterTime;

		// 맴버
		struct _MEMBER
		{
			BYTE		m_Reward;
			BYTE		m_Class;
			SHORT		m_LV;
			USERGUID	m_UserGuid;
			TCHAR		m_CharName[MAX_CHARNAME_LENGTH+1];
			TCHAR		m_GuildName[MAX_GUILDNAME_LENGTH+1];
		} m_CharInfo[MAX_PARTYMEMBER_NUM];

	}pResult[RESULT_ROW_NUM];

	void clear() 
	{
		ZeroMemory( pResult, sizeof(sQUERY)*RESULT_ROW_NUM );
	}
	ULONG uLength[RESULT_COL_NUM];

#ifdef _NA_006801_20130705_RENEWAL_RANKINGSYSTEM
    _BEGIN_PARAM_DATA		(sPARAM, pParam, PARAM_COL_NUM)
        _BINDING_PARAM		(0, Result, SQL_PARAM_OUTPUT)
        _BINDING_PARAM		(1, m_Rows[LAST_ROWS], SQL_PARAM_OUTPUT)
        _BINDING_PARAM		(2, m_Rows[TODAY_ROWS], SQL_PARAM_OUTPUT)
        _END_PARAM_DATA			()
#else
    _BEGIN_PARAM_DATA		(sPARAM, pParam, PARAM_COL_NUM)
        _BINDING_PARAM		(0, Result, SQL_PARAM_OUTPUT)
        _BINDING_PARAM		(1, m_Rows[FAME_ROWS], SQL_PARAM_OUTPUT)
        _BINDING_PARAM		(2, m_Rows[LAST_ROWS], SQL_PARAM_OUTPUT)
        _BINDING_PARAM		(3, m_Rows[TODAY_ROWS], SQL_PARAM_OUTPUT)
        _END_PARAM_DATA			()
#endif //_NA_006801_20130705_RENEWAL_RANKINGSYSTEM

	_BEGIN_BINDING_DATA		(sQUERY, pResult, uLength, RESULT_ROW_NUM, RESULT_COL_NUM)
#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
        _BINDING_COLUMN		(0, m_MissionNo)
        _BINDING_COLUMN		(1, m_Key)
        _BINDING_COLUMN		(2, ranking_type)
        _BINDING_COLUMN		(3, m_ClearTime)
        _BINDING_COLUMN		(4, m_RegisterTime)

        _BINDING_COLUMN		(5, m_CharInfo[0].m_Reward)
        _BINDING_COLUMN		(6, m_CharInfo[0].m_Class)
        _BINDING_COLUMN		(7, m_CharInfo[0].m_LV)
        _BINDING_COLUMN		(8, m_CharInfo[0].m_UserGuid)
        _BINDING_COLUMN_PTR	(9, m_CharInfo[0].m_CharName)
        _BINDING_COLUMN_PTR	(10, m_CharInfo[0].m_GuildName)


        _BINDING_COLUMN		(11, m_CharInfo[1].m_Reward)
        _BINDING_COLUMN		(12, m_CharInfo[1].m_Class)
        _BINDING_COLUMN		(13, m_CharInfo[1].m_LV)
        _BINDING_COLUMN		(14, m_CharInfo[1].m_UserGuid)
        _BINDING_COLUMN_PTR	(15, m_CharInfo[1].m_CharName)
        _BINDING_COLUMN_PTR	(16, m_CharInfo[1].m_GuildName)

        _BINDING_COLUMN		(17, m_CharInfo[2].m_Reward)
        _BINDING_COLUMN		(18, m_CharInfo[2].m_Class)
        _BINDING_COLUMN		(19, m_CharInfo[2].m_LV)
        _BINDING_COLUMN		(20, m_CharInfo[2].m_UserGuid)
        _BINDING_COLUMN_PTR	(21, m_CharInfo[2].m_CharName)
        _BINDING_COLUMN_PTR	(22, m_CharInfo[2].m_GuildName)

        _BINDING_COLUMN		(23, m_CharInfo[3].m_Reward)
        _BINDING_COLUMN		(24, m_CharInfo[3].m_Class)
        _BINDING_COLUMN		(25, m_CharInfo[3].m_LV)
        _BINDING_COLUMN		(26, m_CharInfo[3].m_UserGuid)
        _BINDING_COLUMN_PTR	(27, m_CharInfo[3].m_CharName)
        _BINDING_COLUMN_PTR	(28, m_CharInfo[3].m_GuildName)

        _BINDING_COLUMN		(29, m_CharInfo[4].m_Reward)
        _BINDING_COLUMN		(30, m_CharInfo[4].m_Class)
        _BINDING_COLUMN		(31, m_CharInfo[4].m_LV)
        _BINDING_COLUMN		(32, m_CharInfo[4].m_UserGuid)
        _BINDING_COLUMN_PTR	(33, m_CharInfo[4].m_CharName)
        _BINDING_COLUMN_PTR	(34, m_CharInfo[4].m_GuildName)

        _BINDING_COLUMN		(35, m_CharInfo[5].m_Reward)
        _BINDING_COLUMN		(36, m_CharInfo[5].m_Class)
        _BINDING_COLUMN		(37, m_CharInfo[5].m_LV)
        _BINDING_COLUMN		(38, m_CharInfo[5].m_UserGuid)
        _BINDING_COLUMN_PTR	(39, m_CharInfo[5].m_CharName)
        _BINDING_COLUMN_PTR	(40, m_CharInfo[5].m_GuildName)

        _BINDING_COLUMN		(41, m_CharInfo[6].m_Reward)
        _BINDING_COLUMN		(42, m_CharInfo[6].m_Class)
        _BINDING_COLUMN		(43, m_CharInfo[6].m_LV)
        _BINDING_COLUMN		(44, m_CharInfo[6].m_UserGuid)
        _BINDING_COLUMN_PTR	(45, m_CharInfo[6].m_CharName)
        _BINDING_COLUMN_PTR	(46, m_CharInfo[6].m_GuildName)

        _BINDING_COLUMN		(47, m_CharInfo[7].m_Reward)
        _BINDING_COLUMN		(48, m_CharInfo[7].m_Class)
        _BINDING_COLUMN		(49, m_CharInfo[7].m_LV)
        _BINDING_COLUMN		(50, m_CharInfo[7].m_UserGuid)
        _BINDING_COLUMN_PTR	(51, m_CharInfo[7].m_CharName)
        _BINDING_COLUMN_PTR	(52, m_CharInfo[7].m_GuildName)

        _BINDING_COLUMN		(53, m_CharInfo[8].m_Reward)
        _BINDING_COLUMN		(54, m_CharInfo[8].m_Class)
        _BINDING_COLUMN		(55, m_CharInfo[8].m_LV)
        _BINDING_COLUMN		(56, m_CharInfo[8].m_UserGuid)
        _BINDING_COLUMN_PTR	(57, m_CharInfo[8].m_CharName)
        _BINDING_COLUMN_PTR	(58, m_CharInfo[8].m_GuildName)

        _BINDING_COLUMN		(59, m_CharInfo[9].m_Reward)
        _BINDING_COLUMN		(60, m_CharInfo[9].m_Class)
        _BINDING_COLUMN		(61, m_CharInfo[9].m_LV)
        _BINDING_COLUMN		(62, m_CharInfo[9].m_UserGuid)
        _BINDING_COLUMN_PTR	(63, m_CharInfo[9].m_CharName)
        _BINDING_COLUMN_PTR	(64, m_CharInfo[9].m_GuildName)
#else
		_BINDING_COLUMN		(0, m_MissionNo)
		_BINDING_COLUMN		(1, m_Key)
		_BINDING_COLUMN		(2, m_ClearTime)
		_BINDING_COLUMN		(3, m_RegisterTime)

		_BINDING_COLUMN		(4, m_CharInfo[0].m_Reward)
		_BINDING_COLUMN		(5, m_CharInfo[0].m_Class)
		_BINDING_COLUMN		(6, m_CharInfo[0].m_LV)
		_BINDING_COLUMN		(7, m_CharInfo[0].m_UserGuid)
		_BINDING_COLUMN_PTR	(8, m_CharInfo[0].m_CharName)
		_BINDING_COLUMN_PTR	(9, m_CharInfo[0].m_GuildName)
		

		_BINDING_COLUMN		(10, m_CharInfo[1].m_Reward)
		_BINDING_COLUMN		(11, m_CharInfo[1].m_Class)
		_BINDING_COLUMN		(12, m_CharInfo[1].m_LV)
		_BINDING_COLUMN		(13, m_CharInfo[1].m_UserGuid)
		_BINDING_COLUMN_PTR	(14, m_CharInfo[1].m_CharName)
		_BINDING_COLUMN_PTR	(15, m_CharInfo[1].m_GuildName)

		_BINDING_COLUMN		(16, m_CharInfo[2].m_Reward)
		_BINDING_COLUMN		(17, m_CharInfo[2].m_Class)
		_BINDING_COLUMN		(18, m_CharInfo[2].m_LV)
		_BINDING_COLUMN		(19, m_CharInfo[2].m_UserGuid)
		_BINDING_COLUMN_PTR	(20, m_CharInfo[2].m_CharName)
		_BINDING_COLUMN_PTR	(21, m_CharInfo[2].m_GuildName)

		_BINDING_COLUMN		(22, m_CharInfo[3].m_Reward)
		_BINDING_COLUMN		(23, m_CharInfo[3].m_Class)
		_BINDING_COLUMN		(24, m_CharInfo[3].m_LV)
		_BINDING_COLUMN		(25, m_CharInfo[3].m_UserGuid)
		_BINDING_COLUMN_PTR	(26, m_CharInfo[3].m_CharName)
		_BINDING_COLUMN_PTR	(27, m_CharInfo[3].m_GuildName)

		_BINDING_COLUMN		(28, m_CharInfo[4].m_Reward)
		_BINDING_COLUMN		(29, m_CharInfo[4].m_Class)
		_BINDING_COLUMN		(30, m_CharInfo[4].m_LV)
		_BINDING_COLUMN		(31, m_CharInfo[4].m_UserGuid)
		_BINDING_COLUMN_PTR	(32, m_CharInfo[4].m_CharName)
		_BINDING_COLUMN_PTR	(33, m_CharInfo[4].m_GuildName)

		_BINDING_COLUMN		(34, m_CharInfo[5].m_Reward)
		_BINDING_COLUMN		(35, m_CharInfo[5].m_Class)
		_BINDING_COLUMN		(36, m_CharInfo[5].m_LV)
		_BINDING_COLUMN		(37, m_CharInfo[5].m_UserGuid)
		_BINDING_COLUMN_PTR	(38, m_CharInfo[5].m_CharName)
		_BINDING_COLUMN_PTR	(39, m_CharInfo[5].m_GuildName)

		_BINDING_COLUMN		(40, m_CharInfo[6].m_Reward)
		_BINDING_COLUMN		(41, m_CharInfo[6].m_Class)
		_BINDING_COLUMN		(42, m_CharInfo[6].m_LV)
		_BINDING_COLUMN		(43, m_CharInfo[6].m_UserGuid)
		_BINDING_COLUMN_PTR	(44, m_CharInfo[6].m_CharName)
		_BINDING_COLUMN_PTR	(45, m_CharInfo[6].m_GuildName)

		_BINDING_COLUMN		(46, m_CharInfo[7].m_Reward)
		_BINDING_COLUMN		(47, m_CharInfo[7].m_Class)
		_BINDING_COLUMN		(48, m_CharInfo[7].m_LV)
		_BINDING_COLUMN		(49, m_CharInfo[7].m_UserGuid)
		_BINDING_COLUMN_PTR	(50, m_CharInfo[7].m_CharName)
		_BINDING_COLUMN_PTR	(51, m_CharInfo[7].m_GuildName)

		_BINDING_COLUMN		(52, m_CharInfo[8].m_Reward)
		_BINDING_COLUMN		(53, m_CharInfo[8].m_Class)
		_BINDING_COLUMN		(54, m_CharInfo[8].m_LV)
		_BINDING_COLUMN		(55, m_CharInfo[8].m_UserGuid)
		_BINDING_COLUMN_PTR	(56, m_CharInfo[8].m_CharName)
		_BINDING_COLUMN_PTR	(57, m_CharInfo[8].m_GuildName)

		_BINDING_COLUMN		(58, m_CharInfo[9].m_Reward)
		_BINDING_COLUMN		(59, m_CharInfo[9].m_Class)
		_BINDING_COLUMN		(60, m_CharInfo[9].m_LV)
		_BINDING_COLUMN		(61, m_CharInfo[9].m_UserGuid)
		_BINDING_COLUMN_PTR	(62, m_CharInfo[9].m_CharName)
		_BINDING_COLUMN_PTR	(63, m_CharInfo[9].m_GuildName)
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR

	_END_BINDING_DATA		()
//
//public:
//	DWORD		GetSessionIndex()					{ return m_dwSessionIndex; }
//	VOID		SetSessionIndex( DWORD idx )		{ m_dwSessionIndex = idx; }
//private: 
//	DWORD		m_dwSessionIndex;
};


class Query_Ranking_Insert : public QueryForUser
{
	__DECL_SAFECUSTOMPOOL_PTR(Query_Ranking_Insert)
public:
	enum
	{ 
		PARAM_ROW_NUM	= 1,
		PARAM_COL_NUM	= 2,
	};
	struct sPARAM
	{
		int			m_Result;
		RANKINGKEY	m_Key;
	} pParam[PARAM_ROW_NUM];

	_BEGIN_PARAM_DATA		(sPARAM, pParam, PARAM_COL_NUM)
		_BINDING_PARAM		(0, m_Result, SQL_PARAM_OUTPUT)
		_BINDING_PARAM		(1, m_Key, SQL_PARAM_OUTPUT)
	_END_PARAM_DATA			()

	_INIT_BINDING_DATA()

public:
	inline BOOL ResultSuccess() { return (pParam[0].m_Result==0); }
	DWORD		GetSessionIndex()					{ return m_dwSessionIndex; }
	VOID		SetSessionIndex( DWORD idx )		{ m_dwSessionIndex = idx; }
	RANKINGKEY	GetOldRankingKey()					{ return m_OldKey; }
	VOID		SetOldRankingKey( RANKINGKEY key )	{ m_OldKey = key; }
	RANKINGNO	GetMissionNo()					{ return m_MissionNo; }
	VOID		SetMissionNo( RANKINGNO no )	{ m_MissionNo = no; }
private: 
	DWORD		m_dwSessionIndex;
	RANKINGKEY	m_OldKey;
	RANKINGNO	m_MissionNo;
};


class Query_Ranking_Reward_Update : public QueryForUser
{
	__DECL_SAFECUSTOMPOOL_PTR(Query_Ranking_Reward_Update)
public:
	enum
	{ 
		PARAM_ROW_NUM	= 1,
		PARAM_COL_NUM	= 1,
	};
	struct sPARAM
	{
		int			m_Result;
	} pParam[PARAM_ROW_NUM];

	_BEGIN_PARAM_DATA		(sPARAM, pParam, PARAM_COL_NUM)
		_BINDING_PARAM		(0, m_Result, SQL_PARAM_OUTPUT)
	_END_PARAM_DATA			()

	_INIT_BINDING_DATA()

public:
	inline BOOL ResultSuccess() { return (pParam[0].m_Result==0); }

    inline const TCHAR* GetCharName() const   { return m_tszCharName; }
    inline void SetCharName(const TCHAR* p)	{
        _tcsncpy(m_tszCharName, p, _countof(m_tszCharName));
        m_tszCharName[_countof(m_tszCharName) - 1] = _T('\0');
    }
private:
	__PROPERTY( DWORD, SessionIndex );
	__PROPERTY( RANKINGKEY, Key );
	__PROPERTY( RANKINGNO, MissionNo );
#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
    __PROPERTY( RANKINGTYPE, RankingType);
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
private:
	TCHAR		m_tszCharName[MAX_CHARNAME_LENGTH + 1];
};

class Query_Ranking_Register_Update : public QueryForUser
{
	__DECL_SAFECUSTOMPOOL_PTR(Query_Ranking_Register_Update)
public:
	enum
	{ 
		PARAM_ROW_NUM	= 1,
		PARAM_COL_NUM	= 1,
	};
	
	struct sPARAM
	{
		int			m_Result;
	} pParam[PARAM_ROW_NUM];

	_BEGIN_PARAM_DATA		(sPARAM, pParam, PARAM_COL_NUM)
		_BINDING_PARAM		(0, m_Result, SQL_PARAM_OUTPUT)
	_END_PARAM_DATA			()

	_INIT_BINDING_DATA()

public:
	inline BOOL ResultSuccess() { return (pParam[0].m_Result==0); }	
private:
	__PROPERTY( DWORD, SessionIndex );
	__PROPERTY( DWORD, TempKey );
	__PROPERTY( RANKINGKEY, DBKey );
	__PROPERTY( RANKINGNO, MissionNo );
#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
    __PROPERTY( RANKINGTYPE, Ranking_Type );
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
	
};
