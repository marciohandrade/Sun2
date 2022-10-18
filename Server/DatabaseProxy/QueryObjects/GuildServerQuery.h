
#pragma once
//#pragma pack(push, 1)

#include <Define.h>
#include "StructInQuery.h"

// 길드 정보 select - 길드 마크, 패널티 추가된 버전
class SelectGuildQuery: public QueryForUser
{
	__DECL_SAFECUSTOMPOOL_PTR(SelectGuildQuery)
public:
	enum
	{ 
		PARAM_ROW_NUM	= 1,
		PARAM_COL_NUM	= 1,

		RESULT_ROW_NUM	= 1,

#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
		//RESULT_COL_NUM = 21,
		RESULT_COL_NUM = 20,
#else
    #ifdef _NA_008334_20150608_SONNENSCHEIN
    	RESULT_COL_NUM = 18,
    #else
        RESULT_COL_NUM = 17,
    #endif //_NA_008334_20150608_SONNENSCHEIN
#endif
	};


	struct sPARAM
	{
		INT	m_Result;
	} pParam[PARAM_ROW_NUM];

	struct sQUERY
	{
		TCHAR				m_tszGuildName[MAX_GUILDNAME_LENGTH+1]; //1
		BYTE				m_GuildGrade;   //2
		TCHAR				m_tszGuildNotice[MAX_GUILDNOTICE_LENGTH+1]; //3
		UPTYPE				m_GuildUserPoint;   //4
		GPTYPE				m_GuildPoint;   //5
		BYTE				m_pGuildStyle[MAX_GUILDSTYLESTREAM_SIZE];   //6
		INT					m_RestrictedRight;  //7
		INT64				m_GuildMoney;       //8
#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
		INT					m_CHuntingDisCount; //9
		INT					m_CHuntingVCount; //10
		INT					m_CHuntingLCount; //11
		INT					m_CHuntingTotCount; //12
		INT					m_CHuntingPVPPoint; //13
		BYTE				m_ChuntingRanking; //14
#endif
		eGUILD_DELCHK		m_GuildDestroyWaitFlag; //9  //15
		GUILDMARKIDX		m_PatternIndex;//10 //16
		GUILDMARKIDX		m_BackGroundIndex;//11 //17
		TCHAR				m_tszGuildPenaltyTime[MAX_TIMEDATA_SIZE + 1]; //12 //18
		TCHAR				m_tszGuildDestroyTime[MAX_TIMEDATA_SIZE + 1]; //13 //19
		TCHAR				m_tszMasterChangeTime[MAX_TIMEDATA_SIZE + 1]; //14 //20

        WORD                m_DominationJoingCount;     //15    //21
        WORD                m_DominationDefenseCount;   //16    //22
        WORD                m_DominationHuntingCount;   //17    //23
#ifdef _NA_008334_20150608_SONNENSCHEIN
        BYTE                sonnenschein_phase_;        //18    //24
#endif //_NA_008334_20150608_SONNENSCHEIN
	}pResult[RESULT_ROW_NUM];

	ULONG uLength[RESULT_COL_NUM];

	_BEGIN_PARAM_DATA		(sPARAM, pParam, PARAM_COL_NUM)
		_BINDING_PARAM		(0, m_Result, SQL_PARAM_OUTPUT)
	_END_PARAM_DATA			()

	_BEGIN_BINDING_DATA		(sQUERY, pResult, uLength, RESULT_ROW_NUM, RESULT_COL_NUM)		
		_BINDING_COLUMN_PTR	(0, m_tszGuildName)
		_BINDING_COLUMN		(1, m_GuildGrade)
		_BINDING_COLUMN_PTR	(2, m_tszGuildNotice)
		_BINDING_COLUMN		(3, m_GuildUserPoint)
		_BINDING_COLUMN		(4, m_GuildPoint)
		_BINDING_COLUMN_PTR	(5, m_pGuildStyle)
		_BINDING_COLUMN		(6, m_RestrictedRight)
		_BINDING_COLUMN		(7, m_GuildMoney)

	#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
		_BINDING_COLUMN		(8, m_CHuntingDisCount)
		_BINDING_COLUMN		(9, m_CHuntingVCount)
		_BINDING_COLUMN		(10, m_CHuntingLCount)
		_BINDING_COLUMN		(11, m_CHuntingTotCount)
		_BINDING_COLUMN		(12, m_CHuntingPVPPoint)
		_BINDING_COLUMN		(13, m_ChuntingRanking)	
		_BINDING_COLUMN		(14, m_GuildDestroyWaitFlag)	
		_BINDING_COLUMN		(15, m_BackGroundIndex)	
		_BINDING_COLUMN		(16, m_PatternIndex)	
		_BINDING_COLUMN_PTR	(17, m_tszGuildPenaltyTime)		
		_BINDING_COLUMN_PTR	(18, m_tszGuildDestroyTime)
		_BINDING_COLUMN_PTR	(19, m_tszMasterChangeTime)
	#else
		
        _BINDING_COLUMN		(8, m_BackGroundIndex)	
        _BINDING_COLUMN		(9, m_PatternIndex)	
        _BINDING_COLUMN		(10, m_GuildDestroyWaitFlag)	
        _BINDING_COLUMN_PTR	(11, m_tszGuildPenaltyTime)		
        _BINDING_COLUMN_PTR	(12, m_tszGuildDestroyTime)
        _BINDING_COLUMN_PTR	(13, m_tszMasterChangeTime)
        _BINDING_COLUMN	    (14, m_DominationJoingCount)
        _BINDING_COLUMN	    (15, m_DominationDefenseCount)
        _BINDING_COLUMN	    (16, m_DominationHuntingCount)
    #ifdef _NA_008334_20150608_SONNENSCHEIN
        _BINDING_COLUMN     (17, sonnenschein_phase_)
    #endif //_NA_008334_20150608_SONNENSCHEIN
	#endif

	_END_BINDING_DATA()

public:
	GUILDGUID	GetGuildGuid() { return m_GuildGuid; }
	DWORD		GetSessionIndex() { return m_dwSessionIndex; }
	BOOL		ResultSuccess() { return (pParam[0].m_Result==0); }
	VOID		SetSessionIndex( DWORD idx ) { m_dwSessionIndex = idx; }
	VOID		SetGuildGuid( GUILDGUID GuildGuid ) { m_GuildGuid = GuildGuid;	}
	VOID		SetSelectType( BYTE byType ){ m_bySelectType = byType; }
	BYTE		GetSelectType(){ return m_bySelectType; }
	VOID		SetLoginCharGuid( CHARGUID CharGuid ){ m_LoginCharGuid = CharGuid; }
	CHARGUID	GetLoginCharGuid(){ return m_LoginCharGuid; }

private: 
	GUILDGUID	m_GuildGuid;
	DWORD		m_dwSessionIndex;
	BYTE		m_bySelectType;
	CHARGUID	m_LoginCharGuid;

}; 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class SelectGuildCorpsQuery : public QueryForUser
{
	__DECL_SAFECUSTOMPOOL_PTR(SelectGuildCorpsQuery);

public:
	enum
	{
		PARAM_ROW_NUM	= 1,
		PARAM_COL_NUM	= 1,

		RESULT_ROW_NUM	= MAX_GUILD_CORPS_NUM,
		RESULT_COL_NUM	= 2,	//CorpsNum, bRearCamp

		RETURNCODE_SUCCESS	= 0,
	};

	struct sPARAM
	{	
		INT m_Result;
	} pParam[PARAM_ROW_NUM];

	struct sQUERY
	{	
		BYTE		m_byCorpsNum;
		BYTE		m_bRearCamp;
	}pResult[RESULT_ROW_NUM];

	ULONG uLength[RESULT_COL_NUM];

	_BEGIN_PARAM_DATA	(sPARAM, pParam, PARAM_COL_NUM)
		_BINDING_PARAM	(0, m_Result, SQL_PARAM_OUTPUT)
	_END_PARAM_DATA		()

	_BEGIN_BINDING_DATA		(sQUERY, pResult, uLength, RESULT_ROW_NUM, RESULT_COL_NUM)
		_BINDING_COLUMN		(0, m_byCorpsNum)
		_BINDING_COLUMN		(1, m_bRearCamp)
	_END_BINDING_DATA		()

		inline BOOL ResultSuccess()	{ return (pParam[0].m_Result == 0); }
		inline INT	ResultCode()	{ return pParam[0].m_Result; }

public:
	VOID		SetGuildGuid(GUILDGUID GuildGuid)	{ m_GuildGuid = GuildGuid;	}
	GUILDGUID	GetGuildGuid()						{ return m_GuildGuid; }

	VOID		SetSessionIndex(DWORD idx)	{ m_dwSessionIndex = idx; }
	DWORD		GetSessionIndex()			{ return m_dwSessionIndex; }

	VOID					SetSelectGuildQuery( SelectGuildQuery * pQuery ) { m_pSelectGuildQuery = pQuery; }
	SelectGuildQuery*		GetSelectGuildQuery()			{ return m_pSelectGuildQuery; }

private:
	GUILDGUID			m_GuildGuid;
	DWORD				m_dwSessionIndex;
	SelectGuildQuery*	m_pSelectGuildQuery;
};

// 길드원 정보 리스트 select
class SelectGuildMemberQuery: public QueryForUser
{
	__DECL_SAFECUSTOMPOOL_PTR(SelectGuildMemberQuery)
public:
	enum
	{ 
		PARAM_ROW_NUM	= 1,
		PARAM_COL_NUM	= 1,

		RESULT_ROW_NUM	= MAX_GUILD_MEMBER_NUM,
		RESULT_COL_NUM = 13,
	};
	struct sPARAM
	{
		INT	m_Result;
	} pParam[PARAM_ROW_NUM];

	struct sQUERY
	{
		INT			m_CharGUID;
		TCHAR		m_tszCharName[MAX_CHARNAME_LENGTH+1];
		BYTE		m_byClass;
		LEVELTYPE	m_LV;
		BYTE		m_byChaoState;		// 카오상태~
		eGUILD_DUTY	m_eDuty;
		BYTE		m_byCorpsNum;
		bool		m_bRelation;		//관계
		bool		m_bRankUp;			//랭크업
		bool		m_bUpToGp;			//UP->GP전환
		bool		m_bUsePoint;		//UP/GP사용
		bool		m_bSiegeWarfare;	//공성전
		bool		m_bGuildMark;		//길드마크	
	}pResult[RESULT_ROW_NUM];

	ULONG uLength[RESULT_COL_NUM];

	_BEGIN_PARAM_DATA		(sPARAM, pParam, PARAM_COL_NUM)
		_BINDING_PARAM		(0, m_Result, SQL_PARAM_OUTPUT)
	_END_PARAM_DATA			()

	_BEGIN_BINDING_DATA		(sQUERY, pResult, uLength, RESULT_ROW_NUM, RESULT_COL_NUM)		
		_BINDING_COLUMN		(0, m_CharGUID)
		_BINDING_COLUMN_PTR	(1, m_tszCharName)
		_BINDING_COLUMN		(2, m_byClass)
		_BINDING_COLUMN		(3, m_LV)
		_BINDING_COLUMN		(4, m_byChaoState)
		_BINDING_COLUMN		(5, m_eDuty)
		_BINDING_COLUMN		(6, m_byCorpsNum)
		_BINDING_COLUMN		(7, m_bRelation)
		_BINDING_COLUMN		(8, m_bRankUp)
		_BINDING_COLUMN		(9, m_bUpToGp)
		_BINDING_COLUMN		(10, m_bUsePoint)
		_BINDING_COLUMN		(11, m_bSiegeWarfare)
		_BINDING_COLUMN		(12, m_bGuildMark)
	_END_BINDING_DATA		()

	inline BOOL				ResultSuccess() { return (pParam[0].m_Result==0); }

public:
	VOID					SetGuildGuid( GUILDGUID GuildGuid ) { m_GuildGuid = GuildGuid;	}
	GUILDGUID				GetGuildGuid()						{ return m_GuildGuid; }
	VOID					SetSelectGuildQuery( SelectGuildQuery * pQuery ) { m_pSelectGuildQuery = pQuery; }
	SelectGuildQuery*		GetSelectGuildQuery()			{ return m_pSelectGuildQuery; }
	VOID					SetSelectGuildCorpsQuery( SelectGuildCorpsQuery * pQuery ) { m_pSelectGuildCorpsQuery = pQuery; }
	SelectGuildCorpsQuery*	GetSelectGuildCorpsQuery()			{ return m_pSelectGuildCorpsQuery; }
	DWORD					GetSessionIndex()				{ return m_dwSessionIndex; }
	VOID					SetSessionIndex( DWORD idx )	{ m_dwSessionIndex = idx; }

private: 
	GUILDGUID				m_GuildGuid;
	DWORD					m_dwSessionIndex;
	SelectGuildQuery*		m_pSelectGuildQuery;
	SelectGuildCorpsQuery*	m_pSelectGuildCorpsQuery;
};

// 길드 생성 
class CreateGuildQuery: public QueryForUser
{
	__DECL_SAFECUSTOMPOOL_PTR(CreateGuildQuery)
public:
	enum
	{ 
		PARAM_ROW_NUM	= 1,
		PARAM_COL_NUM	= 1,

		RESULT_ROW_NUM	= 1,
		RESULT_COL_NUM	= 6,

		RETURNCODE_SUCCESS				= 0,
		RETURNCODE_DUPLICATE_NAME		= 1,
		RETURNCODE_JOIN_ANOTHER_GUILD	= 2,
	};
	struct sPARAM
	{
		INT	m_Result;
	} pParam[PARAM_ROW_NUM];

	struct sQUERY
	{
		INT		m_GuildGuid;
		BYTE	m_byCorpsNum;
		BYTE	m_byDuty;
		TCHAR	m_szGuildPenaltyTime[MAX_TIMEDATA_SIZE + 1];
		TCHAR	m_szDestroyRequestTime[MAX_TIMEDATA_SIZE + 1];
		TCHAR	m_szMasterChangeTime[MAX_TIMEDATA_SIZE + 1];
	}pResult[RESULT_ROW_NUM];

	ULONG uLength[RESULT_COL_NUM];

	_BEGIN_PARAM_DATA		(sPARAM, pParam, PARAM_COL_NUM)
		_BINDING_PARAM		(0, m_Result, SQL_PARAM_OUTPUT)
	_END_PARAM_DATA			()

	_BEGIN_BINDING_DATA		(sQUERY, pResult, uLength, RESULT_ROW_NUM, RESULT_COL_NUM)		
		_BINDING_COLUMN		(0, m_GuildGuid)
		_BINDING_COLUMN		(1, m_byCorpsNum)
		_BINDING_COLUMN		(2, m_byDuty)
		_BINDING_COLUMN_PTR	(3, m_szGuildPenaltyTime)		
		_BINDING_COLUMN_PTR	(4, m_szDestroyRequestTime)		
		_BINDING_COLUMN_PTR	(5, m_szMasterChangeTime)		
	_END_BINDING_DATA		()

	inline INT ResultCode() { return pParam[0].m_Result; }

public:
	VOID		SetCharGuid( CHARGUID guid )	{ m_CharGuid = guid;	}
	CHARGUID	GetCharGuid()					{ return m_CharGuid; }
	DWORD		GetSessionIndex()				{ return m_dwSessionIndex; }
	VOID		SetSessionIndex( DWORD idx )	{ m_dwSessionIndex = idx; }
	VOID		SetCharName( TCHAR* pszName ){ strncpy( m_szCharName, pszName, MAX_CHARNAME_LENGTH ); }
	TCHAR*		GetCharName(){ return m_szCharName; }
	VOID		SetGuildName( TCHAR* pszName ){ strncpy( m_szGuildName, pszName, MAX_GUILDNAME_LENGTH ); }
	TCHAR*		GetGuildName(){ return m_szGuildName; }
	VOID		SetLevel( LEVELTYPE lv ){ m_Level = lv; }
	LEVELTYPE	GetLevel(){ return m_Level; }
	VOID		SetClass( BYTE byClass ){ m_byClass = byClass; }
	BYTE		GetClass(){ return m_byClass; }

private:
	CHARGUID	m_CharGuid;
	DWORD		m_dwSessionIndex;
	TCHAR		m_szCharName[MAX_CHARNAME_LENGTH];
	TCHAR		m_szGuildName[MAX_GUILDNAME_LENGTH];
	BYTE		m_byClass;
	LEVELTYPE	m_Level;
};

// 길드 해체
class DestroyGuildQuery: public QueryForUser, public DBResultConvert<DestroyGuildQuery>
{
	__DECL_SAFECUSTOMPOOL_PTR(DestroyGuildQuery)
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
		INT	m_Result;
	} pParam[PARAM_ROW_NUM];

    struct sQUERY
	{
		TCHAR	m_tszDestroyTime[MAX_TIMEDATA_SIZE + 1];
	}pResult[RESULT_ROW_NUM];

	ULONG uLength[RESULT_COL_NUM];

	_BEGIN_PARAM_DATA		(sPARAM, pParam, PARAM_COL_NUM)
		_BINDING_PARAM		(0, m_Result, SQL_PARAM_OUTPUT)
	_END_PARAM_DATA			()

	_BEGIN_BINDING_DATA		(sQUERY, pResult, uLength, RESULT_ROW_NUM, RESULT_COL_NUM)		
	_BINDING_COLUMN			(0, m_tszDestroyTime)
	_END_BINDING_DATA		()

	inline BOOL ResultSuccess() { return (pParam[0].m_Result==0); }
	inline INT	ResultCode()	{ return (pParam[0].m_Result); }

public:
	VOID		SetGuildGuid( GUILDGUID GuildGuid ) { m_GuildGuid = GuildGuid;	}
	GUILDGUID	GetGuildGuid()						{ return m_GuildGuid; }
	DWORD		GetSessionIndex()				{ return m_dwSessionIndex; }
	VOID		SetSessionIndex( DWORD idx )	{ m_dwSessionIndex = idx; }

	VOID		SetMasterGuid(CHARGUID CharGuid)	{ m_MasterCharGuid = CharGuid; }
	CHARGUID	GetMasterGuid()						{ return m_MasterCharGuid; }

private:
	GUILDGUID	m_GuildGuid;
	DWORD		m_dwSessionIndex;
	CHARGUID	m_MasterCharGuid;
};

// 길드원 가입
class JoinGuildMemberQuery: public QueryForUser
{
	__DECL_SAFECUSTOMPOOL_PTR(JoinGuildMemberQuery)
public:
	enum
	{ 
		PARAM_ROW_NUM	= 1,
		PARAM_COL_NUM	= 1,

		RETURNCODE_SUCCESS				= 0,
		RETURNCODE_NOT_EXIST_GUILD		= 1,
		RETURNCODE_JOIN_ANOTHER_GUILD	= 2,
	};
	struct sPARAM
	{
		INT	m_Result;
	} pParam[PARAM_ROW_NUM];

	_BEGIN_PARAM_DATA		(sPARAM, pParam, PARAM_COL_NUM)
		_BINDING_PARAM		(0, m_Result, SQL_PARAM_OUTPUT)
	_END_PARAM_DATA			()

	_INIT_BINDING_DATA()

	inline INT ResultCode() { return pParam[0].m_Result; }
public:
	VOID		SetGuildGuid( GUILDGUID GuildGuid ) { m_GuildGuid = GuildGuid;	}
	GUILDGUID	GetGuildGuid()						{ return m_GuildGuid; }
	VOID		SetCharGuid( CHARGUID guid )		{ m_CharGuid = guid;	}
	CHARGUID	GetCharGuid()						{ return m_CharGuid; }
    inline void SetCharName(const TCHAR* p)	{
        _tcsncpy(m_tszCharName, p, _countof(m_tszCharName));
        m_tszCharName[_countof(m_tszCharName) - 1] = _T('\0');
    }
	inline const TCHAR* GetCharName() const           { return m_tszCharName; }
	inline DWORD  GetSessionIndex() const       { return m_dwSessionIndex; }
	VOID		SetSessionIndex( DWORD idx )	{ m_dwSessionIndex = idx; }
	VOID		SetDuty( eGUILD_DUTY Duty ){ m_Duty = Duty; }
	eGUILD_DUTY	GetDuty(){ return m_Duty; }
	VOID		SetClass( BYTE byClass ){ m_byClass = byClass; }
	BYTE		GetClass(){ return m_byClass; }
	VOID		SetLevel( LEVELTYPE lv ){ m_Level = lv; }
	LEVELTYPE	GetLevel(){ return m_Level; }
	VOID		SetCorpsNum( BYTE byCorpsNum ){ m_byCorpsNum = byCorpsNum; }
	BYTE		GetCorpsNum(){ return m_byCorpsNum; }

private:
	CHARGUID	m_CharGuid;
	GUILDGUID	m_GuildGuid;
	eGUILD_DUTY	m_Duty;
	BYTE		m_byClass;
	LEVELTYPE	m_Level;
	BYTE		m_byCorpsNum;

	TCHAR		m_tszCharName[MAX_CHARNAME_LENGTH + 1];
	DWORD		m_dwSessionIndex;
};

// 길드원 탈퇴
class WithdrawGuildMemberQuery: public QueryForUser
{
	__DECL_SAFECUSTOMPOOL_PTR(WithdrawGuildMemberQuery)
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
		INT	m_Result;
	} pParam[PARAM_ROW_NUM];

	struct sQUERY
	{
		TCHAR m_tszGuildPenaltyTime[MAX_TIMEDATA_SIZE + 1];
		TCHAR m_tszMemberPenaltyTime[MAX_TIMEDATA_SIZE + 1];
	} pResult[RESULT_ROW_NUM];

	ULONG uLength[RESULT_COL_NUM];

	_BEGIN_PARAM_DATA		(sPARAM, pParam, PARAM_COL_NUM)
		_BINDING_PARAM		(0, m_Result, SQL_PARAM_OUTPUT)
	_END_PARAM_DATA			()

	_BEGIN_BINDING_DATA	(sQUERY, pResult, uLength, RESULT_ROW_NUM, RESULT_COL_NUM)		
	    _BINDING_COLUMN_PTR	(0, m_tszGuildPenaltyTime)
        _BINDING_COLUMN_PTR (1, m_tszMemberPenaltyTime)
    _END_BINDING_DATA	()

	inline BOOL ResultSuccess() { return (pParam[0].m_Result==0); }
public:
	VOID		SetGuildGuid( GUILDGUID GuildGuid ) { m_GuildGuid = GuildGuid;	}
	GUILDGUID	GetGuildGuid()						{ return m_GuildGuid; }
	VOID		SetCharGuid( CHARGUID guid )		{ m_CharGuid = guid;	}
	CHARGUID	GetCharGuid()						{ return m_CharGuid; }
	VOID		SetCharName( TCHAR *ptszCharName )
	{
		memset( m_ptszCharName, 0, sizeof(m_ptszCharName) );
		memcpy( m_ptszCharName, ptszCharName, MAX_CHARNAME_LENGTH );
	}
	TCHAR*		GetCharName()						{ return m_ptszCharName; }
    VOID		SetOperCharGuid( CHARGUID guid )	{ m_OperCharGuid = guid;	}
    CHARGUID	GetOperCharGuid()					{ return m_OperCharGuid; }
	DWORD		GetSessionIndex()				{ return m_dwSessionIndex; }
	VOID		SetSessionIndex( DWORD idx )	{ m_dwSessionIndex = idx; }
	VOID		SetPenaltyType( BYTE byType ){ m_byPenaltyType = byType; }
	BYTE		GetPenaltyType(){ return m_byPenaltyType; }

private:
	CHARGUID	m_CharGuid;
    CHARGUID    m_OperCharGuid;
	GUILDGUID	m_GuildGuid;
	TCHAR		m_ptszCharName[MAX_CHARNAME_LENGTH+1];
	DWORD		m_dwSessionIndex;
	BYTE		m_byPenaltyType;
};


// 길드원 포지션 변경
class ChangeGuildPositionQuery : public QueryForUser
{
	__DECL_SAFECUSTOMPOOL_PTR(ChangeGuildPositionQuery)
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
	VOID		SetGuildGuid( GUILDGUID GuildGuid ) { m_GuildGuid = GuildGuid;	}
	GUILDGUID	GetGuildGuid()						{ return m_GuildGuid; }
	DWORD		GetSessionIndex()				{ return m_dwSessionIndex; }
	VOID		SetSessionIndex( DWORD idx )	{ m_dwSessionIndex = idx; }
private: 
	GUILDGUID	m_GuildGuid;
	DWORD		m_dwSessionIndex;
};

// 길드 공지 변경
class ChangeGuildNoticeQuery : public QueryForUser
{
	__DECL_SAFECUSTOMPOOL_PTR(ChangeGuildNoticeQuery)
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

	inline BOOL ResultSuccess() { return ( (pParam[0].m_Result==0) && (!IsError()) ); }
public:
	GUILDGUID GetGuildGuid() { return m_GuildGuid; }
	VOID SetGuildGuid( GUILDGUID GuildGuid ) { m_GuildGuid = GuildGuid;	}

	CHARGUID GetOperCharGuid() { return m_OperCharGuid; }
	VOID SetOperCharGuid( CHARGUID CharGuid ) { m_OperCharGuid = CharGuid;	}

	DWORD	 GetSessionIndex() { return m_dwSessionIndex; }
	VOID	 SetSessionIndex( DWORD idx ) { m_dwSessionIndex = idx; }

	eGUILD_NOTICE	GetNoticeType() { return m_eNoticeType; }
	VOID	 SetNoticeType(eGUILD_NOTICE eType)	{ m_eNoticeType = eType; }
	
	const TCHAR* GetNotice(){ return guild_notice_; }
	VOID	SetNotice(TCHAR* pNotice) {
        strncpy(guild_notice_, pNotice, _countof(guild_notice_));
        guild_notice_[_countof(guild_notice_) - 1] = '\0';
    }
	
private: 
	GUILDGUID m_GuildGuid;
	CHARGUID m_OperCharGuid;
	DWORD	 m_dwSessionIndex;
	eGUILD_NOTICE	m_eNoticeType;
	char guild_notice_[MAX_GUILDNOTICE_LENGTH + 1];
};

// 길드 UP 기부
class DonateUPQuery : public QueryForUser
{
	__DECL_SAFECUSTOMPOOL_PTR(DonateUPQuery)
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
		INT	m_Result;
	} pParam[PARAM_ROW_NUM];

	struct sQUERY
	{
		INT		m_TotalUP;
	}pResult[RESULT_ROW_NUM];

	ULONG uLength[RESULT_COL_NUM];

	_BEGIN_PARAM_DATA		(sPARAM, pParam, PARAM_COL_NUM)
		_BINDING_PARAM		(0, m_Result, SQL_PARAM_OUTPUT)
	_END_PARAM_DATA			()

	_BEGIN_BINDING_DATA		(sQUERY, pResult, uLength, RESULT_ROW_NUM, RESULT_COL_NUM)		
		_BINDING_COLUMN		(0, m_TotalUP)
	_END_BINDING_DATA		()

	inline BOOL ResultSuccess() { return ( (pParam[0].m_Result==0) && (!IsError()) ); }

	inline INT ResultCode() { return pParam[0].m_Result; }
public:
	VOID		SetGuildGuid( GUILDGUID GuildGuid ) { m_GuildGuid = GuildGuid;	}
	GUILDGUID	GetGuildGuid()						{ return m_GuildGuid; }
	DWORD		GetSessionIndex()				{ return m_dwSessionIndex; }
	VOID		SetSessionIndex( DWORD idx )	{ m_dwSessionIndex = idx; }
	CHARGUID	GetCharGuid()					{ return m_CharGuid; }
	VOID		SetCharGuid( CHARGUID guid )	{ m_CharGuid = guid;	}
	VOID		SetDonateUP( UPTYPE DonateUP ){ m_DonateUP = DonateUP; }
	UPTYPE		GetDonateUP(){ return m_DonateUP; }

private: 
	GUILDGUID	m_GuildGuid;
	DWORD		m_dwSessionIndex;
	CHARGUID	m_CharGuid;
	UPTYPE		m_DonateUP;
};

// UP->GP 변환
class ConvertUP2GPQuery : public QueryForUser
{
	__DECL_SAFECUSTOMPOOL_PTR(ConvertUP2GPQuery)
public:
	enum
	{ 
		PARAM_ROW_NUM	= 1,
		PARAM_COL_NUM	= 1,

		RESULT_ROW_NUM	= 1,
		RESULT_COL_NUM	= 2,
	};
	struct sPARAM
	{
		INT	m_Result;
	} pParam[PARAM_ROW_NUM];

	struct sQUERY
	{
		INT		m_RemainUP;
		INT		m_TotalGP;
	}pResult[RESULT_ROW_NUM];

	ULONG uLength[RESULT_COL_NUM];

	_BEGIN_PARAM_DATA		(sPARAM, pParam, PARAM_COL_NUM)
		_BINDING_PARAM		(0, m_Result, SQL_PARAM_OUTPUT)
	_END_PARAM_DATA			()

	_BEGIN_BINDING_DATA		(sQUERY, pResult, uLength, RESULT_ROW_NUM, RESULT_COL_NUM)		
		_BINDING_COLUMN		(0, m_RemainUP)
		_BINDING_COLUMN		(1, m_TotalGP)
	_END_BINDING_DATA		()

	inline BOOL ResultSuccess() { return ( (pParam[0].m_Result==0) && (!IsError()) ); }

	inline INT ResultCode() { return pParam[0].m_Result; }
public:
	VOID		SetGuildGuid( GUILDGUID GuildGuid ) { m_GuildGuid = GuildGuid;	}
	GUILDGUID	GetGuildGuid()						{ return m_GuildGuid; }
	DWORD		GetSessionIndex()				{ return m_dwSessionIndex; }
	VOID		SetSessionIndex( DWORD idx )	{ m_dwSessionIndex = idx; }
	CHARGUID	GetCharGuid()					{ return m_CharGuid; }
	VOID		SetCharGuid( CHARGUID guid )	{ m_CharGuid = guid;	}
private: 
	GUILDGUID	m_GuildGuid;
	DWORD		m_dwSessionIndex;
	CHARGUID	m_CharGuid;
};

// 랭크업
class GuildRankUpQuery : public QueryForUser
{
	__DECL_SAFECUSTOMPOOL_PTR(GuildRankUpQuery)
public:
	enum
	{ 
		PARAM_ROW_NUM	= 1,
		PARAM_COL_NUM	= 1,

		RESULT_ROW_NUM = 1,
		RESULT_COL_NUM  = 4,
	};

	struct sPARAM
	{
		INT	m_Result;
	} pParam[PARAM_ROW_NUM];

	struct sQUERY
	{
		BYTE		m_NowLevel;
		INT		m_RemainGP;
		INT		m_RemainUP;
		INT		m_SlotTab; //길드 창고 슬롯

	}pResult[RESULT_ROW_NUM];

	ULONG uLength[RESULT_COL_NUM];

	_BEGIN_PARAM_DATA		(sPARAM, pParam, PARAM_COL_NUM)
		_BINDING_PARAM			(0, m_Result, SQL_PARAM_OUTPUT)
	_END_PARAM_DATA			()

	_BEGIN_BINDING_DATA		(sQUERY, pResult, uLength, RESULT_ROW_NUM, RESULT_COL_NUM)		
		_BINDING_COLUMN		(0, m_NowLevel)
		_BINDING_COLUMN		(1, m_RemainGP)
		_BINDING_COLUMN		(2, m_RemainUP)
		_BINDING_COLUMN		(3, m_SlotTab)
	_END_BINDING_DATA		()

	inline BOOL ResultSuccess() { return (pParam[0].m_Result==0); }

	inline INT ResultCode() { return pParam[0].m_Result; }

public:
	VOID		SetGuildGuid( GUILDGUID GuildGuid ) { m_GuildGuid = GuildGuid;	}
	GUILDGUID	GetGuildGuid()						{ return m_GuildGuid; }
	DWORD		GetSessionIndex()				{ return m_dwSessionIndex; }
	VOID		SetSessionIndex( DWORD idx )	{ m_dwSessionIndex = idx; }
private: 
	GUILDGUID	m_GuildGuid;
	DWORD		m_dwSessionIndex;
};


#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
class UpdateGuildChuntingRecordQuery : public QueryForUser
{
	__DECL_SAFECUSTOMPOOL_PTR(UpdateGuildChuntingRecordQuery)
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

	inline BOOL ResultSuccess() { return ( (pParam[0].m_Result==0) && (!IsError()) ); }

public:
	VOID		SetGuildGuid( GUILDGUID GuildGuid ) { m_GuildGuid = GuildGuid;	}
	GUILDGUID	GetGuildGuid()						{ return m_GuildGuid; }
	DWORD		GetSessionIndex()				{ return m_dwSessionIndex; }
	VOID		SetSessionIndex( DWORD idx )	{ m_dwSessionIndex = idx; }

	VOID		SetPVPInfo( CHUNTING_PVP_INFO & PVPInfo )	{ m_ChuntingPVPInfo = PVPInfo; }
	CHUNTING_PVP_INFO &	GetPVPInfo()			{ return m_ChuntingPVPInfo; }
private: 
	GUILDGUID			m_GuildGuid;
	DWORD				m_dwSessionIndex;
	CHUNTING_PVP_INFO	m_ChuntingPVPInfo;
};
#endif

// 길드 관계
class RelationGuildQuery : public QueryForUser, public DBResultConvert<RelationGuildQuery>
{
	__DECL_SAFECUSTOMPOOL_PTR(RelationGuildQuery);
public:
	enum
	{
		PARAM_ROW_NUM	= 1,
		PARAM_COL_NUM	= 1,

		RESULT_ROW_NUM	= 1,
		RESULT_COL_NUM	= 3,
		
		RETURNCODE_SUCCESS	= 0,
	};
	struct sPARAM
	{
		INT m_Result;
	} pParam[PARAM_ROW_NUM];

	struct sQUERY
	{
		GUILDGUID	m_RELGuildGuid;
		TCHAR		tszRelGuildName[MAX_GUILDNAME_LENGTH + 1];
#ifdef _NA_003923_20120130_GUILD_RENEWAL
        TCHAR		relation_apply_time[MAX_TIMEDATA_SIZE + 1];
#endif
	}pResult[RESULT_ROW_NUM];

	ULONG uLength[RESULT_COL_NUM];

	_BEGIN_PARAM_DATA	(sPARAM, pParam, PARAM_COL_NUM)
		_BINDING_PARAM	(0, m_Result, SQL_PARAM_OUTPUT)
	_END_PARAM_DATA		()

	_BEGIN_BINDING_DATA	(sQUERY, pResult, uLength, RESULT_ROW_NUM, RESULT_COL_NUM)		
		_BINDING_COLUMN	(0, m_RELGuildGuid)
		_BINDING_COLUMN_PTR	(1, tszRelGuildName)
#ifdef _NA_003923_20120130_GUILD_RENEWAL
        _BINDING_COLUMN_PTR (2, relation_apply_time)
#endif
	_END_BINDING_DATA	()

	inline BOOL ResultSuccess()	{ return (pParam[0].m_Result == 0); }
	inline INT	ResultCode()	{ return pParam[0].m_Result; }

public:
	VOID		SetRelationType(eGUILD_RELATION eRelationType)	{ m_eRelationType = eRelationType; }	
	VOID		SetGuildGuid(GUILDGUID GuildGuid)		{ m_GuildGuid = GuildGuid; }
	VOID		SetRelationGuildName(TCHAR *ptszRelationGuildName){ memcpy(m_tszRelationGuildName, ptszRelationGuildName, sizeof(TCHAR) * MAX_GUILDNAME_LENGTH); }
	VOID		SetRelationGuild(GUILDGUID GuildGuid)	{ m_RelationGuildGuid = GuildGuid; }

	VOID		SetResponseUserKey(DWORD idx) { m_RequestUserkey = idx; }
	VOID		SetRequestUserKey(DWORD idx) { m_ResponseUserkey = idx; }



	GUILDGUID GetGuildGuid(){ return m_GuildGuid; }
	GUILDGUID GetRelationGuild() { return m_RelationGuildGuid; }
	CHARGUID GetRequestUserKey() { return m_RequestUserkey; }
	CHARGUID GetResponseUserKey() { return m_ResponseUserkey; }
	eGUILD_RELATION GetRelationType() { return m_eRelationType; }
	TCHAR* GetRelationGuildName(){ return m_tszRelationGuildName; }

	VOID	 SetSessionIndex(DWORD idx)	{ m_dwSessionIndex = idx;}
	DWORD	 GetSessionIndex() { return m_dwSessionIndex; }
private:
	eGUILD_RELATION m_eRelationType;
	GUILDGUID		m_GuildGuid;
	GUILDGUID		m_RelationGuildGuid;
	DWORD			m_dwSessionIndex;
	TCHAR			m_tszRelationGuildName[MAX_GUILDNAME_LENGTH + 1];
	DWORD m_RequestUserkey;
	DWORD m_ResponseUserkey;
};

// 길드 관계
class RelationBothAgainstGuildQuery : public QueryForUser, public DBResultConvert<RelationBothAgainstGuildQuery>
{
    __DECL_SAFECUSTOMPOOL_PTR(RelationBothAgainstGuildQuery);
public:
    enum
    {
        PARAM_ROW_NUM	= 1,
        PARAM_COL_NUM	= 1,

        RESULT_ROW_NUM	= 1,
        RESULT_COL_NUM	= 2,

        RETURNCODE_SUCCESS	= 0,
    };
    struct sPARAM
    {
        INT m_Result;
    } pParam[PARAM_ROW_NUM];

    struct sQUERY
    {
        TCHAR		relation_apply_time[MAX_TIMEDATA_SIZE + 1];
    }pResult[RESULT_ROW_NUM];

    ULONG uLength[RESULT_COL_NUM];

    _BEGIN_PARAM_DATA	(sPARAM, pParam, PARAM_COL_NUM)
        _BINDING_PARAM	(0, m_Result, SQL_PARAM_OUTPUT)
        _END_PARAM_DATA		()

        _BEGIN_BINDING_DATA	(sQUERY, pResult, uLength, RESULT_ROW_NUM, RESULT_COL_NUM)		
        _BINDING_COLUMN_PTR (0, relation_apply_time)
        _END_BINDING_DATA	()

        inline BOOL ResultSuccess()	{ return (pParam[0].m_Result == 0); }
    inline INT	ResultCode()	{ return pParam[0].m_Result; }


public:
    VOID SetRelationType(eGUILD_RELATION eRelationType)	{ m_eRelationType = eRelationType; }	
    eGUILD_RELATION GetRelationType() { return m_eRelationType; }

    VOID SetGuildGuid(GUILDGUID GuildGuid)		{ m_GuildGuid = GuildGuid; }
    GUILDGUID GetGuildGuid(){ return m_GuildGuid; }

    VOID SetRelationGuildName(TCHAR *ptszRelationGuildName){ memcpy(m_tszRelationGuildName, ptszRelationGuildName, sizeof(TCHAR) * MAX_GUILDNAME_LENGTH); }
    TCHAR* GetRelationGuildName(){ return m_tszRelationGuildName; }

    VOID SetRelationGuild(GUILDGUID GuildGuid)	{ m_RelationGuildGuid = GuildGuid; }
    GUILDGUID GetRelationGuild() { return m_RelationGuildGuid; }

    VOID SetResponseUserKey(DWORD idx) { m_RequestUserkey = idx; }
    CHARGUID GetResponseUserKey() { return m_ResponseUserkey; }

    VOID SetRequestUserKey(DWORD idx) { m_ResponseUserkey = idx; }
    CHARGUID GetRequestUserKey() { return m_RequestUserkey; }

    VOID SetSessionIndex(DWORD idx)	{ m_dwSessionIndex = idx;}
    DWORD GetSessionIndex() { return m_dwSessionIndex; }

private:
    eGUILD_RELATION m_eRelationType;
    GUILDGUID m_GuildGuid;
    GUILDGUID m_RelationGuildGuid;
    DWORD m_dwSessionIndex;
    TCHAR m_tszRelationGuildName[MAX_GUILDNAME_LENGTH + 1];
    DWORD m_RequestUserkey;
    DWORD m_ResponseUserkey;
};

// 길드 직책 설정
class ChangeGuildDutyQuery : public QueryForUser
{
	__DECL_SAFECUSTOMPOOL_PTR(ChangeGuildDutyQuery);
public:
	enum
	{
		PARAM_ROW_NUM	= 1,
		PARAM_COL_NUM	= 1,
	};
	struct sPARAM
	{
		INT m_Result;
	} pParam[PARAM_ROW_NUM];

	_BEGIN_PARAM_DATA	(sPARAM, pParam, PARAM_COL_NUM)
		_BINDING_PARAM	(0, m_Result, SQL_PARAM_OUTPUT)
	_END_PARAM_DATA		()

	_INIT_BINDING_DATA()

	inline BOOL ResultSuccess()	{ return (pParam[0].m_Result == 0); }
	inline INT	ResultCode()	{ return pParam[0].m_Result; }

public:
	VOID	SetGuildGuid(GUILDGUID GuildGuid)		{ m_GuildGuid = GuildGuid; }
	VOID	SetOperCharGuid(CHARGUID CharGuid)		{ m_OperGuid = CharGuid; }
	VOID	SetTargetCharGuid(CHARGUID CharGuid)	{ m_TargetGuid = CharGuid; }
	VOID	SetOldDuty(eGUILD_DUTY eDuty)			{ m_eOldDuty = eDuty; }
	VOID	SetNewDuty(eGUILD_DUTY eDuty)			{ m_eNewDuty = eDuty; }

	GUILDGUID		GetGuildGuid()		{ return m_GuildGuid; }
	CHARGUID		GetOperCharGuid()	{ return m_OperGuid; }
	CHARGUID		GetTargetCharGuid()	{ return m_TargetGuid; }
	eGUILD_DUTY		GetOldDuty()		{ return m_eOldDuty; }
	eGUILD_DUTY		GetNewDuty()		{ return m_eNewDuty; }

	VOID		SetSessionIndex(DWORD idx)				{ m_dwSessionIndex = idx;}
	DWORD		GetSessionIndex()						{ return m_dwSessionIndex; }

private:
	GUILDGUID		m_GuildGuid;
	CHARGUID		m_OperGuid;
	CHARGUID		m_TargetGuid;
	eGUILD_DUTY		m_eNewDuty;
	eGUILD_DUTY		m_eOldDuty;

	DWORD			m_dwSessionIndex;
};

class ChangeGuildMasterQuery : public QueryForUser
{
	__DECL_SAFECUSTOMPOOL_PTR(ChangeGuildMasterQuery);
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
		INT m_Result;
	} pParam[PARAM_ROW_NUM];

	struct sQUERY
	{
		TCHAR	m_tszMasterChangeTime[MAX_TIMEDATA_SIZE+1];
	}pResult[RESULT_ROW_NUM];

	_BEGIN_PARAM_DATA	(sPARAM, pParam, PARAM_COL_NUM)
		_BINDING_PARAM	(0, m_Result, SQL_PARAM_OUTPUT)
	_END_PARAM_DATA		()

	ULONG uLength[RESULT_COL_NUM];

	_BEGIN_BINDING_DATA		( sQUERY, pResult, uLength, RESULT_ROW_NUM, RESULT_COL_NUM )
		_BINDING_COLUMN_PTR ( 0, m_tszMasterChangeTime)
	_END_BINDING_DATA		()

	inline BOOL ResultSuccess()	{ return (pParam[0].m_Result == 0); }
	inline INT	ResultCode()	{ return pParam[0].m_Result; }

public:
	VOID	SetGuildGuid(GUILDGUID GuildGuid)		{ m_GuildGuid = GuildGuid; }
	VOID	SetOperCharGuid(CHARGUID CharGuid)		{ m_OperCharGuid = CharGuid; }
	VOID	SetTargetCharGuid(CHARGUID CharGuid)	{ m_TargetCharGuid = CharGuid; }

	GUILDGUID		GetGuildGuid()		{ return m_GuildGuid; }
	CHARGUID		GetOperCharGuid()	{ return m_OperCharGuid; }
	CHARGUID		GetTargetCharGuid()	{ return m_TargetCharGuid; }

	VOID		SetSessionIndex(DWORD idx)				{ m_dwSessionIndex = idx;}
	DWORD		GetSessionIndex()						{ return m_dwSessionIndex; }

private:
	GUILDGUID		m_GuildGuid;
	CHARGUID		m_OperCharGuid;
	CHARGUID		m_TargetCharGuid;
	DWORD			m_dwSessionIndex;
};

//길드 군단 설정
class CorpsCreateQuery : public QueryForUser
{
	__DECL_SAFECUSTOMPOOL_PTR(CorpsCreateQuery);
public:
	enum
	{
		PARAM_ROW_NUM	= 1,
		PARAM_COL_NUM	= 1,

		RETURNCODE_SUCCESS	= 0,
	};
	struct sPARAM
	{
		INT m_Result;
	} pParam[PARAM_ROW_NUM];
	
	_BEGIN_PARAM_DATA	(sPARAM, pParam, PARAM_COL_NUM)
		_BINDING_PARAM	(0, m_Result, SQL_PARAM_OUTPUT)
	_END_PARAM_DATA		()

	_INIT_BINDING_DATA()

	inline BOOL ResultSuccess()	{ return (pParam[0].m_Result == 0); }
	inline INT	ResultCode()	{ return pParam[0].m_Result; }

public:
	VOID	SetGuildGuid(GUILDGUID GuildGuid)		{ m_GuildGuid = GuildGuid; }
	VOID	SetCorpsNum(BYTE CorpsNum)				{ m_byCorpsNum = CorpsNum; }
	VOID	SetTargetCharGuid( CHARGUID CharGuid )	{ m_TargetCharGuid = CharGuid; }

	GUILDGUID	GetGuildGuid()		{ return m_GuildGuid; }
	CHARGUID	GetTargetCharGuid()	{ return m_TargetCharGuid; }
	BYTE		GetCorpsNum()		{ return m_byCorpsNum; }

	VOID		SetSessionIndex(DWORD idx)				{ m_dwSessionIndex = idx;}
	DWORD		GetSessionIndex()						{ return m_dwSessionIndex; }

private:
	GUILDGUID			m_GuildGuid;	
	CHARGUID			m_TargetCharGuid;
	BYTE				m_byCorpsNum;
	DWORD				m_dwSessionIndex;
	CorpsCreateQuery*	m_pGuildCorpsCreateQuery;
};

class CorpsDestroyQuery : public QueryForUser
{
	__DECL_SAFECUSTOMPOOL_PTR(CorpsDestroyQuery);
public:
	enum
	{
		PARAM_ROW_NUM	= 1,
		PARAM_COL_NUM	= 1,

		RETURNCODE_SUCCESS	= 0,
	};

	struct sPARAM
	{
		INT m_Result;
	} pParam[PARAM_ROW_NUM];


	_BEGIN_PARAM_DATA	(sPARAM, pParam, PARAM_COL_NUM)
		_BINDING_PARAM	(0, m_Result, SQL_PARAM_OUTPUT)
	_END_PARAM_DATA		()

	_INIT_BINDING_DATA()

	inline BOOL ResultSuccess()	{ return (pParam[0].m_Result == 0); }
	inline INT	ResultCode()	{ return pParam[0].m_Result; }

public:
	VOID	SetGuildGuid(GUILDGUID GuildGuid)		{ m_GuildGuid = GuildGuid; }
	VOID	SetCorpsNum(BYTE byCorpsNum)			{ m_byCorpsNum = byCorpsNum; }

	GUILDGUID	GetGuildGuid()		{ return m_GuildGuid; }
	BYTE		GetCorpsNum()		{ return m_byCorpsNum; }

	VOID		SetSessionIndex(DWORD idx)				{ m_dwSessionIndex = idx;}
	DWORD		GetSessionIndex()						{ return m_dwSessionIndex; }
	VOID        SetOperCharGuid(CHARGUID operCharGuid) { m_OperCharGuid = operCharGuid; }
	CHARGUID    GetOperCharGuid() {return m_OperCharGuid;} 

private:
	GUILDGUID		m_GuildGuid;	
	CHARGUID		m_TargetGuid;
	BYTE			m_byCorpsNum;
	DWORD			m_dwSessionIndex;
	CHARGUID        m_OperCharGuid;

	CorpsDestroyQuery *m_pGuildCorpsCreateQuery;
};

class ChangeCommanderQuery : public QueryForUser
{
	__DECL_SAFECUSTOMPOOL_PTR(ChangeCommanderQuery);
public:
	enum
	{
		PARAM_ROW_NUM	= 1,
		PARAM_COL_NUM	= 1,

		RETURNCODE_SUCCESS	= 0,
	};

	struct sPARAM
	{
		INT m_Result;
	} pParam[PARAM_ROW_NUM];

	_BEGIN_PARAM_DATA	(sPARAM, pParam, PARAM_COL_NUM)
		_BINDING_PARAM	(0, m_Result, SQL_PARAM_OUTPUT)
	_END_PARAM_DATA		()

	_INIT_BINDING_DATA()

	inline BOOL ResultSuccess()	{ return (pParam[0].m_Result == 0); }
	inline INT	ResultCode()	{ return pParam[0].m_Result; }

public:
	VOID		SetSessionIndex(DWORD idx)				{ m_dwSessionIndex = idx;}
	DWORD		GetSessionIndex()						{ return m_dwSessionIndex; }

	VOID		SetGuildGuid(GUILDGUID GuildGuid)		{ m_GuildGuid = GuildGuid; }
	VOID		SetCharGuid(CHARGUID CharGuid)			{ m_CharGuid = CharGuid; }	
	VOID		SetCorpsNum(BYTE byCorpsNum)			{ m_byCorpsNum = byCorpsNum; }

	GUILDGUID	GetGuildGuid()		{ return m_GuildGuid; }
	CHARGUID	GetCharGuid()		{ return m_CharGuid; }
	BYTE		GetCorpsNum()		{ return m_byCorpsNum; }

private:
	DWORD				m_dwSessionIndex;
	CorpsCreateQuery*	m_pGuildCorpsCreateQuery;
	GUILDGUID			m_GuildGuid;
	BYTE				m_byCorpsNum;
	CHARGUID			m_CharGuid;
};

class ChangeCorpsQuery : public QueryForUser
{
	__DECL_SAFECUSTOMPOOL_PTR(ChangeCorpsQuery);
public:
	enum
	{
		PARAM_ROW_NUM	= 1,
		PARAM_COL_NUM	= 1,

		RETURNCODE_SUCCESS	= 0,
	};

	struct sPARAM
	{
		INT m_Result;
	} pParam[PARAM_ROW_NUM];

	_BEGIN_PARAM_DATA	(sPARAM, pParam, PARAM_COL_NUM)
		_BINDING_PARAM	(0, m_Result, SQL_PARAM_OUTPUT)
	_END_PARAM_DATA		()

	_INIT_BINDING_DATA()

	inline BOOL ResultSuccess()	{ return (pParam[0].m_Result == 0); }
	inline INT	ResultCode()	{ return pParam[0].m_Result; }

public:
	VOID		SetSessionIndex(DWORD idx)				{ m_dwSessionIndex = idx;}
	DWORD		GetSessionIndex()						{ return m_dwSessionIndex; }

	VOID		SetGuildGuid(GUILDGUID GuildGuid)		{ m_GuildGuid = GuildGuid; }
	VOID		SetCharGuid(CHARGUID CharGuid)			{ m_CharGuid = CharGuid; }	
	VOID		SetSourceCorpsNum(BYTE byCorpsNum)		{ m_bySourceCorpsNum = byCorpsNum; }
	VOID		SetTargetCorpsNum(BYTE byCorpsNum)		{ m_byTargetCorpsNum = byCorpsNum; }

	GUILDGUID	GetGuildGuid()		{ return m_GuildGuid; }
	CHARGUID	GetCharGuid()		{ return m_CharGuid; }
	BYTE		GetSourceCorpsNum()	{ return m_bySourceCorpsNum; }
	BYTE		GetTargetCorpsNum()	{ return m_byTargetCorpsNum; }

private:
	DWORD				m_dwSessionIndex;
	CorpsCreateQuery*	m_pGuildCorpsCreateQuery;
	GUILDGUID			m_GuildGuid;
	BYTE				m_bySourceCorpsNum;
	BYTE				m_byTargetCorpsNum;
	CHARGUID			m_CharGuid;
};

//훈련캠프 생성
class CreateRearCampQuery : public QueryForUser
{
	__DECL_SAFECUSTOMPOOL_PTR(CreateRearCampQuery);
public:
	enum
	{
		PARAM_ROW_NUM	= 1,
		PARAM_COL_NUM	= 1,

		RETURNCODE_SUCCESS	= 0,
	};

	struct sPARAM
	{
		INT m_Result;
	} pParam[PARAM_ROW_NUM];

	_BEGIN_PARAM_DATA	(sPARAM, pParam, PARAM_COL_NUM)
		_BINDING_PARAM	(0, m_Result, SQL_PARAM_OUTPUT)
	_END_PARAM_DATA		()

	_INIT_BINDING_DATA()

	inline BOOL ResultSuccess()	{ return (pParam[0].m_Result == 0); }
	inline INT	ResultCode()	{ return pParam[0].m_Result; }

public:
	VOID		SetSessionIndex(DWORD idx)				{ m_dwSessionIndex = idx;}
	DWORD		GetSessionIndex()						{ return m_dwSessionIndex; }
	VOID		SetGuildGuid( GUILDGUID guid ){ m_GuildGuid = guid; }
	GUILDGUID	GetGuildGuid(){ return m_GuildGuid; }
	VOID		SetCorpsNum( BYTE byCorpsNum ){ m_byCorpsNum = byCorpsNum; }
	BYTE		GetCorpsNum(){ return m_byCorpsNum; }
	VOID		SetTargetCharGuid( CHARGUID CharGuid ){ m_TargetCharGuid = CharGuid; }
	CHARGUID	GetTargetCharGuid(){ return m_TargetCharGuid; }

private:
	DWORD					m_dwSessionIndex;
	CreateRearCampQuery*	m_pGuildCreateRearCampQuery;
	GUILDGUID				m_GuildGuid;
	BYTE					m_byCorpsNum;
	CHARGUID				m_TargetCharGuid;
};

//훈련캠프 생성
class DestroyRearCampQuery : public QueryForUser
{
	__DECL_SAFECUSTOMPOOL_PTR(DestroyRearCampQuery);
public:
	enum
	{
		PARAM_ROW_NUM	= 1,
		PARAM_COL_NUM	= 1,

		RETURNCODE_SUCCESS	= 0,
	};

	struct sPARAM
	{
		INT m_Result;
	} pParam[PARAM_ROW_NUM];

	_BEGIN_PARAM_DATA	(sPARAM, pParam, PARAM_COL_NUM)
		_BINDING_PARAM	(0, m_Result, SQL_PARAM_OUTPUT)
		_END_PARAM_DATA		()

		_INIT_BINDING_DATA()

		inline BOOL ResultSuccess()	{ return (pParam[0].m_Result == 0); }
		inline INT	ResultCode()	{ return pParam[0].m_Result; }

public:
	VOID		SetSessionIndex(DWORD idx)				{ m_dwSessionIndex = idx;}
	DWORD		GetSessionIndex()						{ return m_dwSessionIndex; }
	VOID		SetGuildGuid( GUILDGUID guid ){ m_GuildGuid = guid; }
	GUILDGUID	GetGuildGuid(){ return m_GuildGuid; }
	VOID		SetCorpsNum( BYTE byCorpsNum ){ m_byCorpsNum = byCorpsNum; }
	BYTE		GetCorpsNum(){ return m_byCorpsNum; }

	VOID        SetOperCharGuid(CHARGUID operCharGuid) { m_OperCharGuid = operCharGuid; }
	CHARGUID    GetOperCharGuid() {return m_OperCharGuid;} 

private:
	DWORD					m_dwSessionIndex;
	DestroyRearCampQuery*	m_pGuildDestroyRearCampQuery;
	GUILDGUID				m_GuildGuid;
	BYTE					m_byCorpsNum;

	CHARGUID                m_OperCharGuid;
};

//길드포인트 세팅(GM명령어)
class GuildPointQuery : public QueryForUser
{
	__DECL_SAFECUSTOMPOOL_PTR(GuildPointQuery);
public:
	enum
	{
		PARAM_ROW_NUM	= 1,
		PARAM_COL_NUM	= 1,

		RETURNCODE_SUCCESS	= 0,
	};

	struct sPARAM
	{
		INT m_Result;
	} pParam[PARAM_ROW_NUM];

	_BEGIN_PARAM_DATA	(sPARAM, pParam, PARAM_COL_NUM)
		_BINDING_PARAM	(0, m_Result, SQL_PARAM_OUTPUT)
	_END_PARAM_DATA		()

	_INIT_BINDING_DATA()

	inline BOOL ResultSuccess()	{ return (pParam[0].m_Result == 0); }
	inline INT	ResultCode()	{ return pParam[0].m_Result; }

public:
	VOID		SetSessionIndex(DWORD idx)				{ m_dwSessionIndex = idx;}
	DWORD		GetSessionIndex()						{ return m_dwSessionIndex; }
	VOID		SetGuildGuid( GUILDGUID guid ){ m_GuildGuid = guid; }
	GUILDGUID	GetGuildGuid(){ return m_GuildGuid; }
	VOID		SetUP( UPTYPE up ){ m_UP = up; }
	UPTYPE		GetUP(){ return m_UP; }
	VOID		SetGP( GPTYPE gp ){ m_GP = gp; }
	GPTYPE		GetGP(){ return m_GP; }

private:
	DWORD					m_dwSessionIndex;
	GuildPointQuery*		m_pGuildPointQuery;
	GUILDGUID				m_GuildGuid;
	UPTYPE					m_UP;
	GPTYPE					m_GP;
};


class GuildMarkQuery : public QueryForUser
{
	__DECL_SAFECUSTOMPOOL_PTR(GuildMarkQuery);

	enum
	{
		PARAM_ROW_NUM	= 1,
		PARAM_COL_NUM	= 1,

		RETURNCODE_SUCCESS	= 0,
	};

	struct sPARAM
	{
		INT m_Result;
	} pParam[PARAM_ROW_NUM];

	_BEGIN_PARAM_DATA	(sPARAM, pParam, PARAM_COL_NUM)
		_BINDING_PARAM	(0, m_Result, SQL_PARAM_OUTPUT)
	_END_PARAM_DATA	()

	_INIT_BINDING_DATA()

	INT ResultCode()	{ return pParam[0].m_Result; }
	BOOL ResultSuccess()	{ return (pParam[0].m_Result == 0); }

public:
	void SetGuildGuid( GUILDGUID guid ){ m_GuildGuid = guid; }
	void SetSlotPos( POSTYPE pos ){ m_SlotPos = pos; }
	void SetSlotIndex( SLOTIDX index ){ m_SlotIndex = index; }
	void SetSessionIndex(DWORD idx)	 { m_dwSessionIndex = idx;}
	void SetMarkPatternIndex(GUILDMARKIDX ImgIndex) { m_PatternIndex  = ImgIndex; }
	void SetMarkBackgroundIndxe(GUILDMARKIDX ImgIndex) { m_BackgroundIndex = ImgIndex; }
	
	DWORD GetSessionIndex() { return m_dwSessionIndex; }
	GUILDGUID	GetGuildGuid(){ return m_GuildGuid; }
	POSTYPE GetSlotPos(){ return m_SlotPos; }
	SLOTIDX GetSlotIndex(){ return m_SlotIndex; }
	GUILDMARKIDX GetMarkPatternIndex() { return m_PatternIndex; }
	GUILDMARKIDX GetMarkBackgroundIndxe() { return m_BackgroundIndex; }

private:
	DWORD m_dwSessionIndex;
	GUILDGUID m_GuildGuid;
	GUILDMARKIDX m_BackgroundIndex;
	SLOTIDX m_SlotIndex;
	POSTYPE m_SlotPos;
	GUILDMARKIDX m_PatternIndex;
	GuildMarkQuery* m_GuildMarkQuery;
};


class SelectGuildRelationQuery : public QueryForUser
{
	__DECL_SAFECUSTOMPOOL_PTR(SelectGuildRelationQuery);

public:
	enum
	{
		PARAM_ROW_NUM	= 1,
		PARAM_COL_NUM	= 1,

		RESULT_ROW_NUM	= MAX_ALLIENCE_GUILD_NUM + MAX_HOSTILITY_GUILD_NUM + MAX_ONE_SIDE_HOSTILITY_NUM,
		RESULT_COL_NUM	= 5,

		RETURNCODE_SUCCESS	= 0,
	};

	struct sPARAM
	{	
		INT m_Result;
	} pParam[PARAM_ROW_NUM];

	struct sQUERY
	{	
		GUILDGUID RelGuildGuid;
		eGUILD_RELATION eRelationType;
		ePROCLAIM_GUILD m_eProcClaim;
		TCHAR tszRelGuildName[MAX_GUILDNAME_LENGTH + 1];
#ifdef _NA_003923_20120130_GUILD_RENEWAL
        TCHAR tszRelationApplyTime[MAX_TIMEDATA_SIZE + 1];
#endif

		sQUERY() : 
		RelGuildGuid(INVALID_GUILDGUID),
			m_eProcClaim(NONE_PROCLAIM_GUILD),
			eRelationType(eGUILD_RELATION_NONE){}

	}pResult[RESULT_ROW_NUM];

	ULONG uLength[RESULT_COL_NUM];

	_BEGIN_PARAM_DATA	(sPARAM, pParam, PARAM_COL_NUM)
		_BINDING_PARAM	(0, m_Result, SQL_PARAM_OUTPUT)
	_END_PARAM_DATA		()

	_BEGIN_BINDING_DATA		(sQUERY, pResult, uLength, RESULT_ROW_NUM, RESULT_COL_NUM)
		_BINDING_COLUMN		(0, RelGuildGuid)
		_BINDING_COLUMN_PTR (1, tszRelGuildName)
		_BINDING_COLUMN		(2, eRelationType)
		_BINDING_COLUMN		(3, m_eProcClaim)
#ifdef _NA_003923_20120130_GUILD_RENEWAL
        _BINDING_COLUMN_PTR (4, tszRelationApplyTime)
#endif
	_END_BINDING_DATA		()

	//_INIT_BINDING_DATA()

	inline BOOL ResultSuccess()	{ return (pParam[0].m_Result == 0); }
	inline INT	ResultCode()	{ return pParam[0].m_Result; }

public:
	VOID SetGuildGuid(GUILDGUID GuildGuid) { m_GuildGuid = GuildGuid;	}
	VOID SetSessionIndex(DWORD idx)	{ m_dwSessionIndex = idx; }

	GUILDGUID	GetGuildGuid()			{ return m_GuildGuid; }
	DWORD		GetSessionIndex()		{ return m_dwSessionIndex; }

private:
	GUILDGUID	m_GuildGuid;
	DWORD		m_dwSessionIndex;

};

class GuildRecoveryQuery : public QueryForUser
{
	__DECL_SAFECUSTOMPOOL_PTR(GuildRecoveryQuery);

public:
	enum
	{
		PARAM_ROW_NUM	= 1,
		PARAM_COL_NUM	= 1,

		RETURNCODE_SUCCESS	= 0,
	};

	struct sPARAM
	{	
		INT m_Result;
	} pParam[PARAM_ROW_NUM];

	_BEGIN_PARAM_DATA	(sPARAM, pParam, PARAM_COL_NUM)
		_BINDING_PARAM	(0, m_Result, SQL_PARAM_OUTPUT)
	_END_PARAM_DATA		()

    _INIT_BINDING_DATA()

	inline BOOL ResultSuccess()	{ return (pParam[0].m_Result == 0); }
	inline INT	ResultCode()	{ return pParam[0].m_Result; }

public:
	VOID		SetGuildGuid(GUILDGUID GuildGuid)	{ m_GuildGuid = GuildGuid;	}
	GUILDGUID	GetGuildGuid()						{ return m_GuildGuid; }

	VOID		SetMasterGuid(CHARGUID MasterGuid)	{ m_MasterGuid = MasterGuid; }
	CHARGUID	GetMasterGuid()						{ return m_MasterGuid; }

	VOID	SetSessionIndex(DWORD idx)	{ m_dwSessionIndex = idx; }
	DWORD	GetSessionIndex()			{ return m_dwSessionIndex; }

private:
	GUILDGUID	m_GuildGuid;
	DWORD		m_dwSessionIndex;
	CHARGUID	m_MasterGuid;
};

class GuildDestroyCompleteQuery : public QueryForUser
{
	__DECL_SAFECUSTOMPOOL_PTR(GuildDestroyCompleteQuery);

public:
	enum
	{
		PARAM_ROW_NUM	= 1,
		PARAM_COL_NUM	= 1,

		RESULT_ROW_NUM	= 1,
		RESULT_COL_NUM		= 1,

		RETURNCODE_SUCCESS	= 0,
	};

	struct sPARAM
	{	
		INT m_Result;
	} pParam[PARAM_ROW_NUM];

	struct sQUERY
	{
		INT m_GuildMasterUserKey;

	}pResult[RESULT_ROW_NUM];

	ULONG uLength[RESULT_COL_NUM];

	_BEGIN_PARAM_DATA	(sPARAM, pParam, PARAM_COL_NUM)
		_BINDING_PARAM		(0, m_Result, SQL_PARAM_OUTPUT)
	_END_PARAM_DATA		()

	_BEGIN_BINDING_DATA	(sQUERY, pResult, uLength, RESULT_ROW_NUM, RESULT_COL_NUM)
		_BINDING_COLUMN	(0, m_GuildMasterUserKey)
	_END_BINDING_DATA		()

	BOOL ResultSuccess()	{ return (pParam[0].m_Result == 0); }
	INT	ResultCode()	{ return pParam[0].m_Result; }

public:
	GUILDGUID	GetGuildGuid() { return m_GuildGuid; }
	CHARGUID GetMasterGuid()	{ return m_MasterGuid; }
	DWORD GetSessionIndex() { return m_dwSessionIndex; }

	VOID SetSessionIndex(DWORD idx)	{ m_dwSessionIndex = idx; }
	VOID SetGuildGuid(GUILDGUID GuildGuid)	{ m_GuildGuid = GuildGuid;	}
	VOID SetMasterGuid(CHARGUID MasterGuid)	{ m_MasterGuid = MasterGuid; }

private:
	GUILDGUID	m_GuildGuid;
	DWORD		m_dwSessionIndex;
	CHARGUID	m_MasterGuid;
};


class GuildRightUpdateQuery : public QueryForUser
{
	__DECL_SAFECUSTOMPOOL_PTR(GuildRightUpdateQuery);

public:
	enum
	{
		PARAM_ROW_NUM	= 1,
		PARAM_COL_NUM	= 1,

		RETURNCODE_SUCCESS	= 0,
	};

	struct sPARAM
	{	
		INT m_Result;
	} pParam[PARAM_ROW_NUM];

	_BEGIN_PARAM_DATA	(sPARAM, pParam, PARAM_COL_NUM)
	_BINDING_PARAM	(0, m_Result, SQL_PARAM_OUTPUT)
	_END_PARAM_DATA		()

	_INIT_BINDING_DATA()

	inline BOOL ResultSuccess()	{ return (pParam[0].m_Result == 0); }
	inline INT	ResultCode()	{ return pParam[0].m_Result; }

public:
	VOID							SetGuildGuid(GUILDGUID GuildGuid)	{ m_GuildGuid = GuildGuid;	}
	GUILDGUID						GetGuildGuid()						{ return m_GuildGuid; }

	VOID							SetTargetCharGuid(CHARGUID CharGuid)	{ m_TargetCharGuid = CharGuid; }
	CHARGUID						GetTargetCharGuid()						{ return m_TargetCharGuid; }

	VOID							SetSessionIndex(DWORD idx)	{ m_dwSessionIndex = idx; }
	DWORD							GetSessionIndex()			{ return m_dwSessionIndex; }

	VOID							SetDelegateRight( GUILD_RIGHT_USE_INFO* pInfo ){ CopyMemory( m_DelegateRight, pInfo, sizeof(m_DelegateRight) ); }
	VOID							GetDelegateRight( OUT GUILD_RIGHT_USE_INFO* pInfo, int nSize ){ CopyMemory( pInfo, m_DelegateRight, nSize );}

private:
	GUILDGUID						m_GuildGuid;
	DWORD							m_dwSessionIndex;
	CHARGUID						m_TargetCharGuid;
	GUILD_RIGHT_USE_INFO			m_DelegateRight[MAX_GUILD_DELEGATE_RIGHT];
};

class Query_GuildWarehouse_ItemSlotInfo_Select : public Query_ItemSlotInfo_Select
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_GuildWarehouse_ItemSlotInfo_Select);
public:
    GUILDGUID guild_guid_;
    DWORD session_index_;
};

class GuildWarehouseSelectQuery : public QueryForUser
{
    __DECL_SAFECUSTOMPOOL_PTR(GuildWarehouseSelectQuery);
    enum {
        _MAKE_REORDERING_PARAM_NO_START_(guild_guid_, 0),
        //
        _MAKE_REORDERING_PARAM_NO_COUNTS_()
    };
    enum {
        _MAKE_REORDERING_BINDING_NO_START_(money_, 0),
        _MAKE_REORDERING_BINDING_NO_(number_of_slot_tabs_),
        //
        _MAKE_REORDERING_BINDING_NO_COUNTS_()
    };

public:
    enum
    {
        PARAM_ROW_NUM = 1,
        PARAM_COL_NUM = _MAKE_REORDERING_PARAM_NO_COUNTS_(),
        //
        WAREHOUSE_ROW_NUM = 1,
        WAREHOUSE_COL_NUM = _MAKE_REORDERING_BINDING_NO_COUNTS_(),
    };

    struct sQueryParam
    {
        GUILDGUID guild_guid_;
    } parameters_[PARAM_ROW_NUM];
    //
    struct sQueryResult
    {
        int64 money_;
        short number_of_slot_tabs_;
    } result_records_[WAREHOUSE_ROW_NUM];
    //
    ulong uLength[WAREHOUSE_COL_NUM];
    //
    _BEGIN_PARAM_DATA(sQueryParam, parameters_, PARAM_COL_NUM)
        _REORDERING_PARAM(guild_guid_, SQL_PARAM_INPUT)
    _END_PARAM_DATA();
    //
    _BEGIN_BINDING_DATA(sQueryResult, result_records_, uLength, WAREHOUSE_ROW_NUM, WAREHOUSE_COL_NUM)	
        _REORDERING_BINDING_COLUMN(money_)
        _REORDERING_BINDING_COLUMN(number_of_slot_tabs_)
    _END_BINDING_DATA();

public:
    GUILDGUID GetGuildGuid() const { return parameters_[0].guild_guid_; }
    CHARGUID GetCharGuid() const { return char_guid_; }
    DWORD GetSessionIndex() const { return session_index_; }

    void SetCharGuid(CHARGUID char_guid){ char_guid_ = char_guid; }
    void SetSessionIndex(DWORD session_index) { session_index_ = session_index; }

private:
    CHARGUID char_guid_;
    DWORD session_index_;
};

//{__NA000000_20081106_GUILD_NAME_CHANGE
// 1: 시스템오류
// 2: 동일한 길드명존재
// 3: 공백으로 넘어오는 경우

class ChangeGuildName: public QueryForUser
{
	__DECL_SAFECUSTOMPOOL_PTR(ChangeGuildName)
public:

    ChangeGuildName() 
        : m_TargetPos(0)
        , m_OperCharGuid(0)
        , m_GuildGuid(0)
        , m_dwSessionIndex(0)
    {
        m_szGuildName[0] = '\0';
        memset( pParam, -1, PARAM_ROW_NUM );
    }

    ~ChangeGuildName() {
            m_TargetPos = 0;
            m_OperCharGuid = 0;
            m_GuildGuid = 0;
            m_dwSessionIndex = 0;
            m_szGuildName[0] = '\0';
            memset( pParam, -1, PARAM_ROW_NUM );
        }

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
	_BINDING_PARAM			(0, m_Result, SQL_PARAM_OUTPUT)
	_END_PARAM_DATA			()

	_INIT_BINDING_DATA()


	inline BOOL ResultSuccess() { return (pParam[0].m_Result==0); }
	inline INT  GetResult()		{ return pParam[0].m_Result;}

public:
	VOID		SetGuildGuid( GUILDGUID guid )	{ m_GuildGuid = guid;	}
	GUILDGUID	GetGuildGuid()					{ return m_GuildGuid; }
	VOID		SetCharGuid( CHARGUID guid )	{ m_OperCharGuid = guid;	}
	GUILDGUID	GetCharGuid()					{ return m_OperCharGuid; }
	DWORD		GetSessionIndex()				{ return m_dwSessionIndex; }
	VOID		SetSessionIndex( DWORD idx )	{ m_dwSessionIndex = idx; }
	VOID		SetGuildName( TCHAR* pszName )	{ _tcsncpy( m_szGuildName, pszName, MAX_GUILDNAME_LENGTH ); m_szGuildName[MAX_GUILDNAME_LENGTH] = '\0'; }
	TCHAR*		GetGuildName()					{ return m_szGuildName; }
	POSTYPE		GetTargetPos()						{ return m_TargetPos; }
	VOID		SetTargetPos( POSTYPE TargetPos )	{ m_TargetPos = TargetPos; }


private:
	POSTYPE		m_TargetPos;
	CHARGUID	m_OperCharGuid;
	GUILDGUID	m_GuildGuid;
	DWORD		m_dwSessionIndex;
	TCHAR		m_szGuildName[MAX_GUILDNAME_LENGTH+1];
};


/************************************************************************/
/*                      지역 점령전 쿼리 클레스                         */
/************************************************************************/

//_KR_001385_20091013_DOMINATION_QUERY

/*
* 지역 점령전 정보를 가져 온다 : DB의 모든 내용을 Select한다(MAX_DOMINATION_MAP)
*/
class Query_Domination_Info_Select : public QueryResult
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_Domination_Info_Select)
public:
    enum
    { 
        PARAM_ROW_NUM	= 1,
        PARAM_COL_NUM	= 1,

        RESULT_ROW_NUM  = MAX_DOMINATION_MAP,
        RESULT_COL_NUM	= 16, //_NA_0_20100705_DOMINATION_EVENT   

        RETURN_CODE_SUCCESS = 0,
        RETURN_CODE_FAILURE = 1,
    };

    struct sPARAM
    {
        INT	m_Result;
    } pParam[PARAM_ROW_NUM];

    struct sQUERY
    { 
        int map_code; //지역 코드
        GUILDGUID defense_guild_guid; //길드 Guid
        TCHAR defense_guild_name[MAX_GUILDNAME_LENGTH+1]; //길드명
        TCHAR defense_master_name[MAX_CHARNAME_LENGTH+1]; //길드 마스터명
        CHARGUID defense_master_guid; //길드 마스터Guid
        GUILDMARKIDX defense_background_index; //길드 마크 배경
        GUILDMARKIDX defense_pattern_index; //길드 마크 패턴
        GUILDGUID attack_guild_guid; //길드 Guid
        TCHAR attack_guild_name[MAX_GUILDNAME_LENGTH+1]; //길드명
        int defense_wall_A; //방벽 A
        int defense_wall_B; //방벽 B
        int defense_wall_C; //방벽 C
        int defense_wall_totalpoint; //방벽에 쓸수 있는 총 데이터
        // added by _NA_0_20100705_DOMINATION_EVENT = {
        short event_state; //점령이벤트 활성화 여부
        int event_day_of_week; //점령이벤트 발동 요일(0~6)
        int event_time; //점령이벤트 발동 시간(wzTime)
        // }
    } pResult[RESULT_ROW_NUM];

    ULONG uLength[RESULT_COL_NUM];

    _BEGIN_PARAM_DATA   (sPARAM, pParam, PARAM_COL_NUM)
        _BINDING_PARAM      (0, m_Result, SQL_PARAM_OUTPUT)
    _END_PARAM_DATA     ()

    _BEGIN_BINDING_DATA (sQUERY, pResult, uLength, RESULT_ROW_NUM, RESULT_COL_NUM)		
        _BINDING_COLUMN     (0, map_code)
        _BINDING_COLUMN     (1, defense_guild_guid)
        _BINDING_COLUMN_PTR	(2, defense_guild_name)
        _BINDING_COLUMN_PTR	(3, defense_master_name)
        _BINDING_COLUMN     (4, defense_master_guid)
        _BINDING_COLUMN     (5, defense_background_index)
        _BINDING_COLUMN     (6, defense_pattern_index)
        _BINDING_COLUMN     (7, attack_guild_guid)
        _BINDING_COLUMN_PTR	(8, attack_guild_name)
        _BINDING_COLUMN     (9, defense_wall_A)
        _BINDING_COLUMN     (10, defense_wall_B)
        _BINDING_COLUMN     (11, defense_wall_C)
        _BINDING_COLUMN     (12, defense_wall_totalpoint)
        _BINDING_COLUMN     (13, event_state)
        _BINDING_COLUMN     (14, event_day_of_week)
        _BINDING_COLUMN     (15, event_time)
    _END_BINDING_DATA   ()

        BOOL ResultSuccess() { return (pParam[0].m_Result == RETURN_CODE_SUCCESS); }
};

/*
* 지역 점령전 내용을 업데이트시킨다 현재 방벽 데이터를 저장하는 형식으로 쓰인다.
*/
class Query_Domination_Info_Update : public QueryResult
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_Domination_Info_Update)
public:
    enum
    { 
        PARAM_ROW_NUM	= 1,
        PARAM_COL_NUM	= 1,

        RETURN_CODE_SUCCESS = 0,
        RETURN_CODE_FAILURE = 1,
    };

    struct sPARAM {
        INT	m_Result;
    } pParam[PARAM_ROW_NUM];

    _BEGIN_PARAM_DATA   (sPARAM, pParam, PARAM_COL_NUM)
        _BINDING_PARAM      (0, m_Result, SQL_PARAM_OUTPUT)
    _END_PARAM_DATA     ()

    _INIT_BINDING_DATA()

    bool ResultSuccess() {return (pParam[0].m_Result == RETURN_CODE_SUCCESS);}
    
    void SetDominationInfo(const DOMINATION_INFO& domination_info){domination_info_ = domination_info;} 
    const DOMINATION_INFO& GetDominationInfo()const {return domination_info_;} 

    void SetDominationInfoType(eDOMINATION_EVENT_TYPE  event_type){event_type_ = event_type;} 
    const eDOMINATION_EVENT_TYPE& GetDominationInfoType()const {return event_type_;} 

    void SetCharGuid(DWORD char_guid){char_guid_ = char_guid;}
    const DWORD& GetCharGuid()const {return char_guid_;}

    void SetItemCode(SLOTCODE item_code){item_code_ = item_code;}
    const SLOTCODE& GetItemCode(){return item_code_;}

private:
    DOMINATION_INFO domination_info_;
    eDOMINATION_EVENT_TYPE event_type_;
    DWORD char_guid_;
    SLOTCODE item_code_;

};

/*
* 지역 점령전 내용을 업데이트시킨다 현재 방벽 데이터를 저장하는 형식으로 쓰인다.
*/
class Query_Domination_Info_DefenseWall_Update : public QueryResult
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_Domination_Info_DefenseWall_Update)
public:
    enum
    { 
        PARAM_ROW_NUM	= 1,
        PARAM_COL_NUM	= 1,

        RETURN_CODE_SUCCESS = 0,
        RETURN_CODE_FAILURE = 1,
    };

    struct sPARAM {
        INT	m_Result;
    } pParam[PARAM_ROW_NUM];

    _BEGIN_PARAM_DATA   (sPARAM, pParam, PARAM_COL_NUM)
        _BINDING_PARAM      (0, m_Result, SQL_PARAM_OUTPUT)
    _END_PARAM_DATA     ()
        
    _INIT_BINDING_DATA()

        BOOL ResultSuccess() { return (pParam[0].m_Result == RETURN_CODE_SUCCESS); }
};

/*
* 지역 점령전 내용을 업데이트시킨다 현재 방벽 데이터를 저장하는 형식으로 쓰인다.
*/
class Query_Domination_Info_DisGuild_Update : public QueryResult
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_Domination_Info_DisGuild_Update)
public:
    enum
    { 
        PARAM_ROW_NUM	= 1,
        PARAM_COL_NUM	= 1,

        RETURN_CODE_SUCCESS = 0,
        RETURN_CODE_FAILURE = 1,
    };

    struct sPARAM {
        INT	m_Result;
    } pParam[PARAM_ROW_NUM];

    _BEGIN_PARAM_DATA   (sPARAM, pParam, PARAM_COL_NUM)
        _BINDING_PARAM      (0, m_Result, SQL_PARAM_OUTPUT)
    _END_PARAM_DATA     ()

    _INIT_BINDING_DATA()

    BOOL ResultSuccess() { return (pParam[0].m_Result == RETURN_CODE_SUCCESS); }
};

/*
* 지역 점령전 내용을 업데이트시킨다 현재 방벽 데이터를 저장하는 형식으로 쓰인다.
*/
class Query_Domination_Info_TotalPoint_Update : public QueryResult
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_Domination_Info_TotalPoint_Update)
public:
    enum
    { 
        PARAM_ROW_NUM	= 1,
        PARAM_COL_NUM	= 1,

        RETURN_CODE_SUCCESS = 0,
        RETURN_CODE_FAILURE = 1,
    };

    struct sPARAM {
        INT	m_Result;
    } pParam[PARAM_ROW_NUM];

    _BEGIN_PARAM_DATA   (sPARAM, pParam, PARAM_COL_NUM)
        _BINDING_PARAM      (0, m_Result, SQL_PARAM_OUTPUT)
    _END_PARAM_DATA     ()

    _INIT_BINDING_DATA()

    BOOL ResultSuccess() { return (pParam[0].m_Result == RETURN_CODE_SUCCESS); }
};

/*
* 지역 점령전 내용중 적Guild의 Guid를 저장한다.
*/

class Query_Domination_Info_Insert : public QueryResult
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_Domination_Info_Insert)
public:
    enum
    { 
        PARAM_ROW_NUM	= 1,
        PARAM_COL_NUM	= 1,

        RETURN_CODE_SUCCESS = 0,
        RETURN_CODE_FAILURE = 1,
    };

    struct sPARAM {
        INT	m_Result;
    } pParam[PARAM_ROW_NUM];

    _BEGIN_PARAM_DATA   (sPARAM, pParam, PARAM_COL_NUM)
        _BINDING_PARAM      (0, m_Result, SQL_PARAM_OUTPUT)
    _END_PARAM_DATA     ()

    _INIT_BINDING_DATA()

    BOOL ResultSuccess() { return (pParam[0].m_Result == RETURN_CODE_SUCCESS); }
};

/*
* 지역 점령전 경매 데이터를 가져온다. 맵코드별로 로드하게 된다.
*/

class Query_Domination_Info_Auction_Select : public QueryResult
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_Domination_Info_Auction_Select)
public:
    enum
    { 
        PARAM_ROW_NUM	= 1,
        PARAM_COL_NUM	= 1,

        RESULT_ROW_NUM  = MAX_DOMINATION_AUCTION,
        RESULT_COL_NUM	= 9,

        RETURN_CODE_SUCCESS = 0,
        RETURN_CODE_FAILURE = 1,
    };

    struct sPARAM
    {
        int	m_Result;
    } pParam[PARAM_ROW_NUM];

    struct sQUERY
    {  
        int             map_code;//지역 코드  
        GUILDGUID       guild_guid;//입찰 길드
        TCHAR  	        guild_name[MAX_GUILDNAME_LENGTH+1];//지역 점령 길드명
        int64_t         tender_cost;//입찰 금액
        TCHAR		    auction_apply_time[MAX_TIMEDATA_SIZE + 1];//입찰 시간
        GUILDMARKIDX    pattern_index;//길드 마크 패턴
        GUILDMARKIDX    background_index;//길드 마크 배경
        TCHAR  		    master_name[MAX_CHARNAME_LENGTH+1];//길드 마스터명
        CHARGUID        master_guid;//길드 마스터 Guid
    } pResult[RESULT_ROW_NUM];

    ULONG uLength[RESULT_COL_NUM];

    _BEGIN_PARAM_DATA   (sPARAM, pParam, PARAM_COL_NUM)
        _BINDING_PARAM      (0, m_Result, SQL_PARAM_OUTPUT)
    _END_PARAM_DATA     ()

    _BEGIN_BINDING_DATA (sQUERY, pResult, uLength, RESULT_ROW_NUM, RESULT_COL_NUM)		
        _BINDING_COLUMN     (0, guild_guid)
        _BINDING_COLUMN     (1, background_index)
        _BINDING_COLUMN     (2, pattern_index)
        _BINDING_COLUMN_PTR	(3, guild_name)
        _BINDING_COLUMN_PTR	(4, master_name)
        _BINDING_COLUMN     (5, master_guid)
        _BINDING_COLUMN_PTR	(6, auction_apply_time)
        _BINDING_COLUMN     (7, tender_cost)
        _BINDING_COLUMN     (8, map_code)
    _END_BINDING_DATA ()

    BOOL ResultSuccess()const {return (pParam[0].m_Result == RETURN_CODE_SUCCESS);}
    VOID SetMapCode(MAPCODE map_code) {map_code_ = map_code;}
    MAPCODE GetMapCode()const {return map_code_;}
private:
    MAPCODE map_code_;
};

/*
* 경매 데이터를 업데이트 하게 된다. 길드 Guid가 중복되게 되면 Update한다.
*/
class Query_Domination_Info_Action_Update : public QueryResult
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_Domination_Info_Action_Update)
public:
    enum
    { 
        PARAM_ROW_NUM	= 1,
        PARAM_COL_NUM	= 1,

        RETURN_CODE_SUCCESS = 0,
        RETURN_CODE_FAILURE = 1,
    };

    struct sPARAM {
        INT	m_Result;
    } pParam[PARAM_ROW_NUM];

    _BEGIN_PARAM_DATA   (sPARAM, pParam, PARAM_COL_NUM)
        _BINDING_PARAM      (0, m_Result, SQL_PARAM_OUTPUT)
    _END_PARAM_DATA     ()

    _INIT_BINDING_DATA()

    BOOL ResultSuccess() { return (pParam[0].m_Result == RETURN_CODE_SUCCESS); }

        void SetActionInfo(const DominationAuctionInfo& actionInfo) {
            m_curDominationInfo = actionInfo;
        }
        const DominationAuctionInfo& GetActionInfo( ) const { return m_curDominationInfo; }
        VOID SetUserKey( CHARGUID UserKey ){ m_dwUserKey = UserKey; }
        CHARGUID GetUserKey( ){ return m_dwUserKey; }

#ifdef _CH_20100420_CHANGE_DOMINATION_APPLY_WAY
        POSTYPE                 m_TokenPos;//배팅 아이템 인벤토리 위치
        int number_of_betting_item; // 배팅 아이템 개수
#endif

private:
    DominationAuctionInfo m_curDominationInfo;
    CHARGUID m_dwUserKey;
};

/*
* 경매 데이터 모두를 지워준다.
*/
class Query_Domination_Info_Action_Init : public QueryResult
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_Domination_Info_Action_Init)
public:
    enum
    { 
        PARAM_ROW_NUM	= 1,
        PARAM_COL_NUM	= 1,

        RETURN_CODE_SUCCESS = 0,
        RETURN_CODE_FAILURE = 1,
    };

    struct sPARAM {
        INT	m_Result;
    } pParam[PARAM_ROW_NUM];

    _BEGIN_PARAM_DATA   (sPARAM, pParam, PARAM_COL_NUM)
        _BINDING_PARAM      (0, m_Result, SQL_PARAM_OUTPUT)
        _END_PARAM_DATA     ()

        _INIT_BINDING_DATA()

        BOOL ResultSuccess() { return (pParam[0].m_Result == RETURN_CODE_SUCCESS); }
        VOID SetMapCode( MAPCODE mapCode ){ m_MapCode = mapCode; }
        MAPCODE GetMapCode(){ return m_MapCode; }

private:
    MAPCODE m_MapCode;
};

/*
* 경매 금액 보상/환급 리스트 업데이트 한다.
*/
class Query_Domination_ActionMoney_Update : public QueryResult
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_Domination_ActionMoney_Update)
public:
    enum
    { 
        PARAM_ROW_NUM	= 1,
        PARAM_COL_NUM	= 1,

        RETURN_CODE_SUCCESS = 0,
        RETURN_CODE_FAILURE = 1,
    };

    struct sPARAM {
        INT	m_Result;
    } pParam[PARAM_ROW_NUM];

    _BEGIN_PARAM_DATA   (sPARAM, pParam, PARAM_COL_NUM)
        _BINDING_PARAM      (0, m_Result, SQL_PARAM_OUTPUT)
        _END_PARAM_DATA     ()

        _INIT_BINDING_DATA()
        
public:
    BOOL ResultSuccess() { return (pParam[0].m_Result == RETURN_CODE_SUCCESS); }

};
/*
* 경매 금액 보상/환급 리스트를 지운다.
*/
class Query_Domination_ReturnMoney_Delete : public QueryResult
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_Domination_ReturnMoney_Delete)
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
        INT	m_Result;
    } pParam[PARAM_ROW_NUM];

    struct sQUERY
    {
        int m_MapCode;
        CHARGUID m_CharGuid;
        MONEY m_AmountMoney;
        eSYSTEM_MEMO_TYPE RewardFlag;
    } pResult[RESULT_ROW_NUM];

    ULONG uLength[RESULT_COL_NUM];

    _BEGIN_PARAM_DATA   (sPARAM, pParam, PARAM_COL_NUM)
        _BINDING_PARAM      (0, m_Result, SQL_PARAM_OUTPUT)
    _END_PARAM_DATA     ()

    _BEGIN_BINDING_DATA (sQUERY, pResult, uLength, RESULT_ROW_NUM, RESULT_COL_NUM)		
        _BINDING_COLUMN     (0, m_MapCode)
        _BINDING_COLUMN     (1, m_CharGuid)
        _BINDING_COLUMN     (2, m_AmountMoney)
        _BINDING_COLUMN     (3, RewardFlag)
    _END_BINDING_DATA   ()

public:
    BOOL ResultSuccess() {return (pParam[0].m_Result == RETURN_CODE_SUCCESS);}
    VOID SetDominationInfo(const DOMINATION_INFO& dominationInfo){m_DominationInfo = dominationInfo;}
    DOMINATION_INFO GetDominationInfo(){return m_DominationInfo;}

private:
    DOMINATION_INFO m_DominationInfo;
};

//_NA_20100604_BETTER_DOMINATION_MANAGER
/*
 * 지역 점령전 길드 관련 정보를 업데이트 한다.
 */
class Query_Domination_Increase_Count : public QueryResult
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_Domination_Increase_Count)
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
        GUILDGUID guild_guid_;
        WORD domination_join_count_;
        WORD domination_defense_count_;
        WORD domination_hunting_count_;
    } pResult[RESULT_ROW_NUM];

    ULONG uLength[RESULT_COL_NUM];

    _BEGIN_PARAM_DATA   (sPARAM, pParam, PARAM_COL_NUM)
        _BINDING_PARAM      (0, result_value_, SQL_PARAM_OUTPUT)
        _END_PARAM_DATA     ()

        _BEGIN_BINDING_DATA (sQUERY, pResult, uLength, RESULT_ROW_NUM, RESULT_COL_NUM)		
        _BINDING_COLUMN     (0, guild_guid_)
        _BINDING_COLUMN     (1, domination_join_count_)
        _BINDING_COLUMN     (2, domination_defense_count_)
        _BINDING_COLUMN     (3, domination_hunting_count_)
        _END_BINDING_DATA   ()

        BOOL ResultSuccess() { return pParam[0].result_value_; }

        void SetGuildGuid(GUILDGUID guild_guid){ guild_guid_ = guild_guid;}
        const GUILDGUID GetGuildGuid(){ return guild_guid_; }

private:
    GUILDGUID guild_guid_;
};

//_NA_20110406_CHANGE_RULE_FOR_DOMINATION_JOIN

class Query_Domination_MemberList_Select : public QueryResult
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_Domination_MemberList_Select)
public:
    enum
    { 
        PARAM_ROW_NUM	= 1,
        PARAM_COL_NUM	= 1,

        RESULT_ROW_NUM  = MAX_DOMINATION_MEMBER*2,
        RESULT_COL_NUM	= 5,

        RETURN_CODE_SUCCESS = 0,
        RETURN_CODE_FAILURE = 1,
    };

    struct sPARAM
    {
        int result_value_;
    }result_param[PARAM_ROW_NUM];

    struct sQUERY
    {  
        DominationMemberInfo member_info;
    }select_record[RESULT_ROW_NUM];

    ULONG data_length[RESULT_COL_NUM];

    _BEGIN_PARAM_DATA (sPARAM, result_param, PARAM_COL_NUM)
        _BINDING_PARAM (0, result_value_, SQL_PARAM_OUTPUT)
    _END_PARAM_DATA ()

    _BEGIN_BINDING_DATA (sQUERY, select_record, data_length, RESULT_ROW_NUM, RESULT_COL_NUM)		
        _BINDING_COLUMN (0, member_info.guild_type_)
        _BINDING_COLUMN (1, member_info.member_guid_)
        _BINDING_COLUMN (2, member_info.member_level_)
        _BINDING_COLUMN (3, member_info.member_class_)
        _BINDING_COLUMN_PTR	(4, member_info.member_name_)
    _END_BINDING_DATA ()
    
    BOOL ResultSuccess()const {return (result_param[0].result_value_ == RETURN_CODE_SUCCESS);}
    VOID SetMapCode(MAPCODE map_code) {map_code_ = map_code;}
    MAPCODE GetMapCode()const {return map_code_;}

private:
    MAPCODE map_code_;
};


class Query_Domination_Member_Insert : public QueryResult
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_Domination_Member_Insert)
public:
    enum
    { 
        PARAM_ROW_NUM	= 1,
        PARAM_COL_NUM	= 1,

        RETURN_CODE_SUCCESS = 0,
        RETURN_CODE_FAILURE = 1,
    };

    struct sPARAM {
        INT	result_value_;
    } result_param[PARAM_ROW_NUM];

    _BEGIN_PARAM_DATA (sPARAM, result_param, PARAM_COL_NUM)
        _BINDING_PARAM (0, result_value_, SQL_PARAM_OUTPUT)
    _END_PARAM_DATA ()

    _INIT_BINDING_DATA ()
    
    BOOL ResultSuccess() {return (result_param[0].result_value_ == RETURN_CODE_SUCCESS);}
    void SetMemberInfo(const DominationMemberInfo& member_info){member_info_ = member_info;}
    const DominationMemberInfo& GetMemberInfo()const{return member_info_;}
    void SetMapCode(const MAPCODE map_code){map_code_ = map_code;}
    MAPCODE GetMapCode(){return map_code_;}

private:
    DominationMemberInfo member_info_;
    MAPCODE map_code_;
};


class Query_Domination_Member_Delete : public QueryResult
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_Domination_Member_Delete)
public:
    enum
    { 
        PARAM_ROW_NUM	= 1,
        PARAM_COL_NUM	= 1,

        RETURN_CODE_SUCCESS = 0,
        RETURN_CODE_FAILURE = 1,
    };

    struct sPARAM {
        INT	result_value_;
    } result_param[PARAM_ROW_NUM];

    _BEGIN_PARAM_DATA (sPARAM, result_param, PARAM_COL_NUM)
        _BINDING_PARAM (0, result_value_, SQL_PARAM_OUTPUT)
    _END_PARAM_DATA ()

    _INIT_BINDING_DATA ()

    BOOL ResultSuccess() {return (result_param[0].result_value_ == RETURN_CODE_SUCCESS);}
    void SetMapCode (const MAPCODE map_code){map_code_ = map_code;}
    MAPCODE GetMapCode()const{return map_code_;}
    void SetMemberGuid(const CHARGUID member_guid){member_guid_ = member_guid;}
    CHARGUID GetMemberGuid()const{return member_guid_;}
    void SetRequesterGuid(const CHARGUID requester_guid) { requester_guid_ = requester_guid; }
    CHARGUID GetRequesterGuid() const { return requester_guid_; }

private:
    MAPCODE map_code_;
    CHARGUID member_guid_;
    CHARGUID requester_guid_;
};

class Query_Domination_MemberList_Init : public QueryResult
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_Domination_MemberList_Init)
public:
    enum
    { 
        PARAM_ROW_NUM	= 1,
        PARAM_COL_NUM	= 1,

        RETURN_CODE_SUCCESS = 0,
        RETURN_CODE_FAILURE = 1,
    };

    struct sPARAM {
        INT	result_value_;
    } result_param[PARAM_ROW_NUM];

    _BEGIN_PARAM_DATA (sPARAM, result_param, PARAM_COL_NUM)
        _BINDING_PARAM (0, result_value_, SQL_PARAM_OUTPUT)
    _END_PARAM_DATA ()

    _INIT_BINDING_DATA ()

    BOOL ResultSuccess() {return (result_param[0].result_value_ == RETURN_CODE_SUCCESS);}
};

#ifdef _NA_003923_20120130_GUILD_RENEWAL
class Query_GuildInGameLog_Insert : public QueryResult
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_GuildInGameLog_Insert)
public:
    enum
    { 
        PARAM_ROW_NUM	= 1,
        PARAM_COL_NUM	= 1,

        RESULT_ROW_NUM  = 1,
        RESULT_COL_NUM	= 1,

        RETURN_CODE_SUCCESS = 0,
        RETURN_CODE_FAILURE = 1,
    };


    struct sPARAM {
        INT	result_value_;
    } result_param[PARAM_ROW_NUM];

    struct sQUERY
    {
        TCHAR log_time_[MAX_TIMEDATA_SIZE+1];				//사용한 시간
    } select_record[RESULT_ROW_NUM];

    ULONG data_length[RESULT_COL_NUM];

    _BEGIN_PARAM_DATA (sPARAM, result_param, PARAM_ROW_NUM)
        _BINDING_PARAM (0, result_value_, SQL_PARAM_OUTPUT)
    _END_PARAM_DATA ()

    _BEGIN_BINDING_DATA (sQUERY, select_record, data_length, RESULT_ROW_NUM, RESULT_COL_NUM)		
        _BINDING_COLUMN_PTR	 (0, log_time_)
    _END_BINDING_DATA ()
    
public:
    BOOL ResultSuccess() {return (result_param[0].result_value_ == RETURN_CODE_SUCCESS);}

    GuildInGameLogData GetLogData(){ return guild_ingame_log_data_; }
    void SetLogData(const GuildInGameLogData& input_data){ guild_ingame_log_data_ = input_data; }

    GUILDGUID GetGuildGuid(){ return guild_guid_; }
    void SetGuildGuid(const GUILDGUID& guild_guid){ guild_guid_ = guild_guid; }

private:
    GuildInGameLogData guild_ingame_log_data_;
    GUILDGUID guild_guid_;
};

class Query_GuildInGameLog_Select: public QueryResult
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_GuildInGameLog_Select)

public:
    enum
    {
        PARAM_ROW_NUM	= 1,
        PARAM_COL_NUM	= 1,

        RESULT_ROW_NUM = MAX_IN_GAME_GUILD_LOG * GuildMaxLog,
        RESULT_COL_NUM	 = 6,

        RETURNCODE_SUCCESS = 0,
        RETURNCODE_FAILED = 1,
    };

    struct sPARAM
    {
        INT result_value_;
    } result_param[PARAM_ROW_NUM];

    struct sQUERY
    {
        BYTE log_type_;//GuildInGameLogType
        TCHAR string_param_[MAX_IN_GAME_LOG_PARAM_STR];
        DWORD param1_;
        DWORD param2_;
        DWORD param3_;
        TCHAR log_time_[MAX_TIMEDATA_SIZE+1];
    } select_record[RESULT_ROW_NUM];

    ULONG data_length[RESULT_COL_NUM];

    _BEGIN_PARAM_DATA (sPARAM, result_param, PARAM_COL_NUM)
        _BINDING_PARAM (0, result_value_, SQL_PARAM_OUTPUT)
    _END_PARAM_DATA ()

    _BEGIN_BINDING_DATA (sQUERY, select_record, data_length, RESULT_ROW_NUM, RESULT_COL_NUM)		
        _BINDING_COLUMN	(0, log_type_)
        _BINDING_COLUMN_PTR (1, string_param_)
        _BINDING_COLUMN (2, param1_)
        _BINDING_COLUMN	(3, param2_)
        _BINDING_COLUMN	(4, param3_)
        _BINDING_COLUMN_PTR (5, log_time_)
    _END_BINDING_DATA ()

public:
    BOOL ResultSuccess() {return (result_param[0].result_value_ == RETURNCODE_SUCCESS);}

    GUILDGUID GetGuildGuid() {return guild_guid_;}
    void SetGuildGuid(const GUILDGUID guild_guid) {guild_guid_ = guild_guid;}

private:
    GUILDGUID guild_guid_;
};

#endif//_NA_003923_20120130_GUILD_RENEWAL


#ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION

class Query_Continent_Lord_Info_Select : public QueryResult
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_Continent_Lord_Info_Select)
public:
    enum
    { 
        PARAM_ROW_NUM	= 1,
        PARAM_COL_NUM	= 1,

        RESULT_ROW_NUM  = MAX_ALLIENCE_GUILD_NUM,
        RESULT_COL_NUM	= 1,

        RETURN_CODE_SUCCESS = 0,
        RETURN_CODE_FAILURE = 1,    //해당 GUID길드가 DB에 없음
        RETURN_CODE_ERROR = 2,      //시스템 오류
    };

    struct sPARAM
    {
        int result_value_;
    }result_param[PARAM_ROW_NUM];

    struct sQUERY
    {  
        TCHAR guild_name_[MAX_GUILDNAME_LENGTH + 1];
    }select_record[RESULT_ROW_NUM];

    ULONG data_length[RESULT_COL_NUM];

    _BEGIN_PARAM_DATA (sPARAM, result_param, PARAM_COL_NUM)
        _BINDING_PARAM (0, result_value_, SQL_PARAM_OUTPUT)
    _END_PARAM_DATA ()

    _BEGIN_BINDING_DATA (sQUERY, select_record, data_length, RESULT_ROW_NUM, RESULT_COL_NUM)		
        _BINDING_COLUMN_PTR	(0, guild_name_)
    _END_BINDING_DATA ()

    BOOL ResultSuccess()const {return (result_param[0].result_value_ == RETURN_CODE_SUCCESS);}
    VOID SetLordGuildGuid(MAPCODE lord_guild_guid) {lord_guild_guid_ = lord_guild_guid;}
    MAPCODE GetLordGuildGuid()const {return lord_guild_guid_;}
    VOID SetContinentNumber(int continent_number) {continent_number_ = continent_number;}
    int GetContinentNumber()const {return continent_number_;}
    VOID SetMapCode(MAPCODE map_code) {map_code_ = map_code;}
    int GetMapCode()const {return map_code_;}
    VOID SetSendGameServer(MAPCODE send_game_server) {send_game_server_ = send_game_server;}
    bool GetSendGameServer()const {return send_game_server_;}
    VOID SetLordGuildName(const TCHAR* guild_name) 
    {
        util::GUILDNAMECOPY(lord_guild_name_, guild_name);
    }
    const TCHAR* GetLordGuildName()const {return lord_guild_name_;}
    

private:
    bool      send_game_server_;
    int       continent_number_;
    MAPCODE   map_code_;
    GUILDGUID lord_guild_guid_;
    TCHAR     lord_guild_name_[MAX_GUILDNAME_LENGTH + 1];
};

#endif //_NA_007128_20140326_MONDSCHIEN_DOMINATION

#ifdef _NA_008334_20150608_SONNENSCHEIN
class SonnenScheinInitializeQuery: public QueryForUser
{
    __DECL_SAFECUSTOMPOOL_PTR(SonnenScheinInitializeQuery)
public:
    enum
    { 
        PARAM_ROW_NUM	= 1,
        PARAM_COL_NUM	= 1,
    };

    struct sPARAM
    {
        INT	result;
    } parameter[PARAM_ROW_NUM];

    _BEGIN_PARAM_DATA		(sPARAM, parameter, PARAM_COL_NUM)
        _BINDING_PARAM		(0, result, SQL_PARAM_OUTPUT)
    _END_PARAM_DATA			()

    _INIT_BINDING_DATA()

public:
    DWORD		GetSessionIndex() { return m_dwSessionIndex; }
    BOOL		ResultSuccess() { return (parameter[0].result==0); }
    VOID		SetSessionIndex( DWORD idx ) { m_dwSessionIndex = idx; }
    
private: 
    DWORD		m_dwSessionIndex;
}; 

class SonnenScheinUpdateQuery: public QueryForUser
{
    __DECL_SAFECUSTOMPOOL_PTR(SonnenScheinUpdateQuery)
public:
    //Parameter
    enum
    { 
        PARAM_ROW_NUM	= 1,
        PARAM_COL_NUM	= 1,
    };

    struct sPARAM
    {
        INT	result;
    } parameter[PARAM_ROW_NUM];

    _BEGIN_PARAM_DATA		(sPARAM, parameter, PARAM_COL_NUM)
        _BINDING_PARAM		(0, result, SQL_PARAM_OUTPUT)
    _END_PARAM_DATA			()

    _INIT_BINDING_DATA()

public:
    DWORD		GetSessionIndex() { return m_dwSessionIndex; }
    BOOL		ResultSuccess() { return (parameter[0].result==0); }
    VOID		SetSessionIndex( DWORD idx ) { m_dwSessionIndex = idx; }

    VOID        SetGuildGuid(GUILDGUID guid) {guild_guid_ = guid;}
    GUILDGUID   GetGuildGuid() {return guild_guid_;}
    VOID        SetSonnenScheinPhase(BYTE phase) {sonnenschein_phase_ = phase;}
    BYTE        GetSonnenScheinPhase() {return sonnenschein_phase_;}
private: 
    DWORD		m_dwSessionIndex;

    GUILDGUID guild_guid_;
    BYTE      sonnenschein_phase_;
};
#endif //_NA_008334_20150608_SONNENSCHEIN
//#pragma pack(pop)