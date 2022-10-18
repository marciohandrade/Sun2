
#pragma once
//#pragma pack(push, 1)

#include <Define.h>
#include "StructInQuery.h"

//#ifdef _NA_0_20080612_WORLDSERVER_REFACTORING
// 친구 목록 정보 select
class SelectQueryCommunityList : public QueryResult
{
	__DECL_SAFECUSTOMPOOL_PTR(SelectQueryCommunityList )
public:
	enum
	{ 
		PARAM_ROW_NUM	= 1,
		PARAM_COL_NUM	= 1,

		RESULT_ROW_NUM = (MAX_FRIENDBLOCK_STAT_NUM),
		RESULT_COL_NUM	= 5,
	};

	struct sPARAM
	{
		INT m_Result;
	} pParam[PARAM_ROW_NUM];

	struct sQUERY
	{
		BYTE	 m_Sts;
		BYTE	m_byClass;
		LEVELTYPE m_Level;
		CHARGUID	m_FriendCharGuid;
		TCHAR m_ptszFriendCharName[MAX_CHARNAME_LENGTH+1];
	}pResult[RESULT_ROW_NUM];

	ULONG uLength[RESULT_COL_NUM];

	_BEGIN_PARAM_DATA	(sPARAM, pParam, PARAM_COL_NUM)
	_BINDING_PARAM		    (0, m_Result, SQL_PARAM_OUTPUT)
	_END_PARAM_DATA	     ()

	_BEGIN_BINDING_DATA	(sQUERY, pResult, uLength, RESULT_ROW_NUM, RESULT_COL_NUM)		
	_BINDING_COLUMN		(0, m_Sts)
	_BINDING_COLUMN		(1, m_byClass)
	_BINDING_COLUMN		(2, m_Level)
	_BINDING_COLUMN		(3, m_FriendCharGuid)
	_BINDING_COLUMN_PTR	(4, m_ptszFriendCharName)
	_END_BINDING_DATA		()

	VOID SetCharGuid( CHARGUID CharGuid )	{ m_CharGuid = CharGuid;	}
	BOOL ResultSuccess() { return (pParam[0].m_Result==0); }
	CHARGUID	GetCharGuid() { return m_CharGuid; }

private: 
	CHARGUID	m_CharGuid;
};

class SelectQueryContactMeList : public QueryResult
{
	__DECL_SAFECUSTOMPOOL_PTR(SelectQueryContactMeList)

public:
	enum
	{ 
		PARAM_ROW_NUM	= 1,
		PARAM_COL_NUM	= 1,

		RESULT_ROW_NUM = (MAX_FRIENDBLOCK_STAT_NUM),
		RESULT_COL_NUM	= 1,
	};

	struct sPARAM
	{
		INT m_Result;
	} pParam[PARAM_ROW_NUM];

	struct sQUERY
	{
		CHARGUID m_ContactMeCharGuid;

	} pResult[RESULT_ROW_NUM];

	ULONG uLength[RESULT_COL_NUM];

	_BEGIN_PARAM_DATA	(sPARAM, pParam, PARAM_COL_NUM)
	_BINDING_PARAM		    (0, m_Result, SQL_PARAM_OUTPUT)
	_END_PARAM_DATA	     ()

	_BEGIN_BINDING_DATA	(sQUERY, pResult, uLength, RESULT_ROW_NUM, RESULT_COL_NUM)		
	_BINDING_COLUMN			(0, m_ContactMeCharGuid)
	_END_BINDING_DATA		()

	void SetCharGuid( CHARGUID CharGuid )	{ m_CharGuid = CharGuid;	}
	BOOL ResultSuccess() { return (pParam[0].m_Result==0); }
	CHARGUID GetCharGuid() { return m_CharGuid; }

private:
	CHARGUID m_CharGuid;
};
//#else //_NA_0_20080612_WORLDSERVER_REFACTORING
// 친구 목록 정보 select
class Query_Friend_SelectList : public QueryResult
{
	__DECL_SAFECUSTOMPOOL_PTR(Query_Friend_SelectList)
public:
	enum
	{ 
		PARAM_ROW_NUM	= 1,
		PARAM_COL_NUM	= 1,

		RESULT_ROW_NUM	= (MAX_FRIENDBLOCK_STAT_NUM),
#ifdef _NA_006642_20120416_COMMUNITY_INFO_BUG_MODIFY
        RESULT_COL_NUM	= 7,
#else
		RESULT_COL_NUM	= 6,
#endif // _NA_006642_20120416_COMMUNITY_INFO_BUG_MODIFY

		
	};
	struct sPARAM
	{
		INT	m_Result;
	} pParam[PARAM_ROW_NUM];

	struct sQUERY
	{
		CHARGUID	m_FriendCharGuid;
		TCHAR		m_ptszFriendCharName[MAX_CHARNAME_LENGTH+1];
		BYTE		m_byClass;			//< 클래스
		LEVELTYPE	m_Level;			//< 레벨
		BYTE		m_Sts;
#ifdef _NA_006642_20120416_COMMUNITY_INFO_BUG_MODIFY
        BYTE        gender; //성별
#endif // _NA_006642_20120416_COMMUNITY_INFO_BUG_MODIFY
	}pResult[RESULT_ROW_NUM];

	ULONG uLength[RESULT_COL_NUM];

	_BEGIN_PARAM_DATA		(sPARAM, pParam, PARAM_COL_NUM)
		_BINDING_PARAM		(0, m_Result, SQL_PARAM_OUTPUT)
	_END_PARAM_DATA			()

	_BEGIN_BINDING_DATA		(sQUERY, pResult, uLength, RESULT_ROW_NUM, RESULT_COL_NUM)		
		_BINDING_COLUMN		(0, m_FriendCharGuid)
		_BINDING_COLUMN_PTR	(1, m_ptszFriendCharName)
		_BINDING_COLUMN		(2, m_byClass)
		_BINDING_COLUMN		(3, m_Level)
		_BINDING_COLUMN		(4, m_Sts)
#ifdef _NA_006642_20120416_COMMUNITY_INFO_BUG_MODIFY
        _BINDING_COLUMN		(5, gender)
#endif // _NA_006642_20120416_COMMUNITY_INFO_BUG_MODIFY
	_END_BINDING_DATA		()

	inline BOOL ResultSuccess() { return (pParam[0].m_Result==0); }
public:
	VOID		SetCharGuid( CHARGUID CharGuid )	{ m_CharGuid = CharGuid;	}
	CHARGUID	GetCharGuid()						{ return m_CharGuid; }
private: 
	CHARGUID	m_CharGuid;
};
//#endif //_NA_0_20080612_WORLDSERVER_REFACTORING

class Query_Friend_Request : public QueryResult
{
	__DECL_SAFECUSTOMPOOL_PTR(Query_Friend_Request)
public:
	enum
	{ 
		PARAM_ROW_NUM	= 1,
		PARAM_COL_NUM	= 1,

		RESULT_ROW_NUM	= 1,
#ifdef _NA_006642_20120416_COMMUNITY_INFO_BUG_MODIFY
		RESULT_COL_NUM	= 5,
#else
        RESULT_COL_NUM	= 4,
#endif // _NA_006642_20120416_COMMUNITY_INFO_BUG_MODIFY

		RETURNCODE_SUCCESS				= 0,
		RETURNCODE_NOT_EXIST_TARGET		= 1,
		RETURNCODE_JOIN_ANOTHER_LIST	= 2,
	};
	struct sPARAM
	{
		INT	m_Result;
	} pParam[PARAM_ROW_NUM];

	struct sQUERY
	{
		CHARGUID	m_FriendCharGuid;
		TCHAR		m_ptszFriendCharName[MAX_CHARNAME_LENGTH+1];
		BYTE		m_byClass;			//< 클래스
		LEVELTYPE	m_Level;			//< 레벨
#ifdef _NA_006642_20120416_COMMUNITY_INFO_BUG_MODIFY
        BYTE        m_Gender;
#endif // _NA_006642_20120416_COMMUNITY_INFO_BUG_MODIFY
	}pResult[RESULT_ROW_NUM];

	ULONG uLength[RESULT_COL_NUM];

	_BEGIN_PARAM_DATA		(sPARAM, pParam, PARAM_COL_NUM)
		_BINDING_PARAM		(0, m_Result, SQL_PARAM_OUTPUT)
	_END_PARAM_DATA			()

	_BEGIN_BINDING_DATA		(sQUERY, pResult, uLength, RESULT_ROW_NUM, RESULT_COL_NUM)		
		_BINDING_COLUMN		(0, m_FriendCharGuid)
		_BINDING_COLUMN_PTR	(1, m_ptszFriendCharName)
		_BINDING_COLUMN		(2, m_byClass)
        _BINDING_COLUMN		(3, m_Level)
#ifdef _NA_006642_20120416_COMMUNITY_INFO_BUG_MODIFY
        _BINDING_COLUMN		(4, m_Gender)
#endif // _NA_006642_20120416_COMMUNITY_INFO_BUG_MODIFY
	_END_BINDING_DATA		()

	inline INT ResultCode() { return pParam[0].m_Result; }

	inline BOOL ResultSuccess() { return (pParam[0].m_Result==RETURNCODE_SUCCESS); }
public:
	inline VOID		SetCharGuid( CHARGUID CharGuid )	{ m_CharGuid = CharGuid;	}
	inline CHARGUID	GetCharGuid()						{ return m_CharGuid; }
    inline const TCHAR* GetCharNameTo() const   { return m_tszCharName; }
    inline void SetCharNameTo(const TCHAR* p)	{
        _tcsncpy(m_tszCharName, p, _countof(m_tszCharName));
        m_tszCharName[_countof(m_tszCharName) - 1] = _T('\0');
    }
	inline VOID		SetKey( DWORD dwKey )				{ m_dwKey = dwKey;	}
	inline DWORD	GetKey()							{ return m_dwKey; }
private: 
	DWORD		m_dwKey;
	CHARGUID	m_CharGuid;
	TCHAR		m_tszCharName[MAX_CHARNAME_LENGTH + 1];
};

class Query_Friend_Delete : public QueryResult
{
	__DECL_SAFECUSTOMPOOL_PTR(Query_Friend_Delete)
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

	inline INT ResultCode() { return pParam[0].m_Result; }
	inline BOOL ResultSuccess() { return (pParam[0].m_Result==0); }
public:
	inline VOID		SetCharGuid( CHARGUID CharGuid )	{ m_CharGuid = CharGuid;	}
	inline CHARGUID	GetCharGuid()						{ return m_CharGuid; }
	inline VOID		SetCharGuidTo( CHARGUID CharGuid )	{ m_CharGuidTo = CharGuid;	}
	inline CHARGUID	GetCharGuidTo()						{ return m_CharGuidTo; }
    inline const TCHAR* GetCharNameTo() const   { return m_tszCharName; }
    inline void SetCharNameTo(const TCHAR* p)	{
        _tcsncpy(m_tszCharName, p, _countof(m_tszCharName));
        m_tszCharName[_countof(m_tszCharName) - 1] = _T('\0');
    }
	inline VOID		SetKey( DWORD dwKey )				{ m_dwKey = dwKey;	}
	inline DWORD	GetKey()							{ return m_dwKey; }
private: 
	DWORD		m_dwKey;
	CHARGUID	m_CharGuid;
	CHARGUID	m_CharGuidTo;
	TCHAR		m_tszCharName[MAX_CHARNAME_LENGTH + 1];
};

// NOTE: used by _KR_0_20091021_FIRENDINVITATION_EXTEND, 보상 아이템 정보 추가
class Query_Friend_InvitationReward_Insert : public QueryResult
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_Friend_InvitationReward_Insert)
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

        inline INT ResultCode() { return pParam[0].m_Result; }
        inline BOOL ResultSuccess() { return (pParam[0].m_Result==0); }
public:
    void SetSendGuid(CHARGUID send_guid) { m_SendGuid = send_guid; }
    CHARGUID GetSendGuid() const { return m_SendGuid; }
    void SetRecvGuid(CHARGUID recv_guid) { m_RecvGuid = recv_guid; }
    CHARGUID GetRecvGuid() const { return m_RecvGuid; }
    void SetKey(DWORD key) { m_Key = key; }
    DWORD GetKey() const { return m_Key; }
    void SetLevel(WORD level) { m_Level = level; }
    WORD GetLevel() const { return m_Level; } 
    void SetCharName(const TCHAR* char_name);
    const TCHAR* GetCharName() const { return m_ptszRecvName; }
    void SetMemo(const TCHAR* memo);
    const TCHAR* GetMemo() const { return m_ptszMemo; }
    void SetSession(LPVOID session) { m_Session = session; }
    LPVOID GetSession() const { return m_Session; }
private:    
    CHARGUID m_SendGuid;
    CHARGUID m_RecvGuid;
    DWORD m_Key;
    WORD  m_Level;
    TCHAR m_ptszRecvName[MAX_CHARNAME_LENGTH + 1];
    TCHAR m_ptszMemo[MAX_MEMO_LENGTH + 1];
    LPVOID m_Session;
};

inline void Query_Friend_InvitationReward_Insert::SetCharName(const TCHAR* char_name)
{
    _tcsncpy(m_ptszRecvName, char_name, _countof(m_ptszRecvName));
    m_ptszRecvName[_countof(m_ptszRecvName) - 1] = _T('\0');
}

inline void Query_Friend_InvitationReward_Insert::SetMemo(const TCHAR* memo)
{
    _tcsncpy(m_ptszMemo, memo, _countof(m_ptszMemo));
    m_ptszMemo[_countof(m_ptszMemo) - 1] = _T('\0');
}

// NOTE: used by _KR_0_20091021_FIRENDINVITATION_EXTEND, 친구 초대 해제
class Query_Friend_InvitationFree : public QueryResult
{
	__DECL_SAFECUSTOMPOOL_PTR(Query_Friend_InvitationFree)
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

	inline INT ResultCode() { return pParam[0].m_Result; }
	inline BOOL ResultSuccess() { return (pParam[0].m_Result==0); }
public:
	inline VOID		SetCharGuid( CHARGUID CharGuid )	{ m_CharGuid = CharGuid;	}
	inline CHARGUID	GetCharGuid()						{ return m_CharGuid; }		
private: 	
	CHARGUID	m_CharGuid;	
};

// NOTE: used by _KR_0_20091021_FIRENDINVITATION_EXTEND, 친구 초대 이벤트에서 캐릭터 생성 후 
class Query_Friend_InvitationInsert : public QueryResult
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_Friend_InvitationInsert)
public:
    enum
    { 
        PARAM_ROW_NUM	= 1,
        PARAM_COL_NUM	= 1,

        RESULT_ROW_NUM = (MAX_FRIEND_STAT_NUM),
        RESULT_COL_NUM	= 1,
    };
    struct sPARAM
    {
        INT	m_Result;
    } pParam[PARAM_ROW_NUM];

    
    struct sQUERY
    {        
        CHARGUID	m_FriendCharGuid;     
    }pResult[RESULT_ROW_NUM];

    ULONG       uLength[RESULT_COL_NUM];    

    _BEGIN_PARAM_DATA		(sPARAM, pParam, PARAM_COL_NUM)
        _BINDING_PARAM		(0, m_Result, SQL_PARAM_OUTPUT)
    _END_PARAM_DATA			()

    _BEGIN_BINDING_DATA	(sQUERY, pResult, uLength, RESULT_ROW_NUM, RESULT_COL_NUM)		
        _BINDING_COLUMN		(0, m_FriendCharGuid)
    _END_BINDING_DATA		()

    inline INT ResultCode() { return pParam[0].m_Result; }
    inline BOOL ResultSuccess() { return (pParam[0].m_Result==0); }
public:
    inline VOID		SetCharGuid( CHARGUID CharGuid )	{ m_CharGuid = CharGuid;	}
    inline CHARGUID	GetCharGuid()						{ return m_CharGuid; }	
    inline VOID		SetUserGuid( DWORD UserGuid )		{ m_UserGuid = UserGuid;	}
    inline DWORD	GetUserGuid()						{ return m_UserGuid; }    
    inline VOID		SetCharName( TCHAR * tszName )		{ memcpy( m_CharName, tszName, MAX_CHARNAME_LENGTH*sizeof(TCHAR) ); }
    inline TCHAR*	GetCharName()						{ return m_CharName; }    
    inline VOID		SetClass( BYTE Class )		        { m_Class = Class;	}
    inline BYTE 	GetClass()						    { return m_Class; }    
private:     
    
    DWORD		m_UserGuid;
    CHARGUID	m_CharGuid;	
    BYTE        m_Class;
    TCHAR       m_CharName[MAX_CHARNAME_LENGTH];    
};

//_NA_0_20100527_ADD_CHAR_RELATION_MEMO
class Query_Friend_Detail_Info_Select : public QueryResult
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_Friend_Detail_Info_Select)
public:
    enum
    { 
        PARAM_ROW_NUM	= 1,
        PARAM_COL_NUM	= 1,

        RESULT_ROW_NUM	= 1,
        RESULT_COL_NUM	= 4,

        RETURNCODE_SUCCESS				= 0,
        RETURNCODE_FAILED				= 1,
        RETURNCODE_FAILED_NOT_USE_CHAR  = 2,
    };
    struct sPARAM
    {
        INT	m_Result;
    } pParam[PARAM_ROW_NUM];

    struct sQUERY
    {
        TCHAR       m_ptszToCharName[MAX_CHARNAME_LENGTH+1];
        TCHAR		m_ptszMemo[MAX_FRIEND_MEMO_LENGHT+1];
        TCHAR		m_ptszToCharGuildName[MAX_GUILDNAME_LENGTH+1];
        BYTE		m_byToCharGuildDuty;
    }pResult[RESULT_ROW_NUM];

    ULONG uLength[RESULT_COL_NUM];

    inline INT ResultCode() { return pParam[0].m_Result; }

    _BEGIN_PARAM_DATA		(sPARAM, pParam, PARAM_COL_NUM)
    _BINDING_PARAM		    (0, m_Result, SQL_PARAM_OUTPUT)
    _END_PARAM_DATA			()

    _BEGIN_BINDING_DATA		(sQUERY, pResult, uLength, RESULT_ROW_NUM, RESULT_COL_NUM)		
    _BINDING_COLUMN_PTR     (0, m_ptszToCharName)
    _BINDING_COLUMN_PTR	    (1, m_ptszMemo)
    _BINDING_COLUMN_PTR	    (2, m_ptszToCharGuildName)
    _BINDING_COLUMN		    (3, m_byToCharGuildDuty)
    _END_BINDING_DATA		()

    inline BOOL ResultSuccess() { return (pParam[0].m_Result==RETURNCODE_SUCCESS); }

public:
    inline VOID SetCharGuid( CHARGUID dwCharGuid ) { m_dwCharGuid = dwCharGuid; }
    inline VOID SetToCharGuid( CHARGUID dwToCharGuid ) { m_dwToCharGuid = dwToCharGuid; }

    inline CHARGUID	GetCharGuid() { return m_dwCharGuid; }
    inline CHARGUID	GetToCharGuid() { return m_dwToCharGuid; }

private: 
    CHARGUID m_dwCharGuid;
    CHARGUID m_dwToCharGuid;
};

class Query_Friend_Memo_Update : public QueryResult
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_Friend_Memo_Update)
public:
    enum
    { 
        PARAM_ROW_NUM	= 1,
        PARAM_COL_NUM	= 1,

        RETURNCODE_SUCCESS				= 0,
        RETURNCODE_FAILED				= 1,
        RETURNCODE_FAILED_NOT_USE_CHAR  = 2,
    };
    struct sPARAM
    {
        INT	m_Result;
    } pParam[PARAM_ROW_NUM];


    inline INT ResultCode() { return pParam[0].m_Result; }

    _BEGIN_PARAM_DATA		(sPARAM, pParam, PARAM_COL_NUM)
    _BINDING_PARAM		    (0, m_Result, SQL_PARAM_OUTPUT)
    _END_PARAM_DATA			()

    _INIT_BINDING_DATA()

    inline BOOL ResultSuccess() { return (pParam[0].m_Result==RETURNCODE_SUCCESS); }

public:
    inline VOID	SetCharGuid(CHARGUID dwCharGuid) { m_dwCharGuid = dwCharGuid; }
    inline VOID	SetToCharGuid(CHARGUID dwToCharGuid) { m_dwToCharGuid = dwToCharGuid; }

    inline CHARGUID	GetCharGuid() { return m_dwCharGuid; }
    inline CHARGUID	GetToCharGuid() { return m_dwToCharGuid; }

private: 
    CHARGUID m_dwCharGuid;
    CHARGUID m_dwToCharGuid;
};

/*
class Query_Friend_BlockChar : public QueryResult
{
	__DECL_SAFECUSTOMPOOL_PTR(Query_Friend_BlockChar)
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
		CHARGUID	m_BlockedCharGuid;
	}pResult[RESULT_ROW_NUM];

	ULONG uLength[RESULT_COL_NUM];

	_BEGIN_PARAM_DATA		(sPARAM, pParam, PARAM_COL_NUM)
		_BINDING_PARAM		(0, m_Result, SQL_PARAM_OUTPUT)
	_END_PARAM_DATA			()

	_BEGIN_BINDING_DATA		(sQUERY, pResult, uLength, RESULT_ROW_NUM, RESULT_COL_NUM)		
		_BINDING_COLUMN		(0, m_BlockedCharGuid)
	_END_BINDING_DATA		()

	inline BOOL ResultSuccess() { return (pParam[0].m_Result==0); }
public:
	inline VOID		SetCharGuid( CHARGUID CharGuid )	{ m_CharGuid = CharGuid;	}
	inline CHARGUID	GetCharGuid()						{ return m_CharGuid; }
	inline VOID		SetCharNameTo( TCHAR * tszName )	{ memcpy( m_tszBlockedCharName, tszName, MAX_CHARNAME_LENGTH*sizeof(TCHAR) ); }
	inline TCHAR *	GetCharNameTo()						{ return m_tszBlockedCharName; }
private: 
	CHARGUID	m_CharGuid;
	TCHAR		m_tszBlockedCharName[MAX_CHARNAME_LENGTH];
};

class Query_Friend_FreeChar : public QueryResult
{
	__DECL_SAFECUSTOMPOOL_PTR(Query_Friend_FreeChar)
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
	inline VOID		SetCharGuid( CHARGUID CharGuid )	{ m_CharGuid = CharGuid;	}
	inline CHARGUID	GetCharGuid()						{ return m_CharGuid; }
	inline VOID		SetCharNameTo( TCHAR * tszName )	{ memcpy( m_tszBlockedCharName, tszName, MAX_CHARNAME_LENGTH*sizeof(TCHAR) ); }
	inline TCHAR *	GetCharNameTo()						{ return m_tszBlockedCharName; }
private: 
	CHARGUID	m_CharGuid;
	TCHAR		m_tszBlockedCharName[MAX_CHARNAME_LENGTH];
};
*/
class Query_Chat_BlockChar : public QueryResult
{
	__DECL_SAFECUSTOMPOOL_PTR(Query_Chat_BlockChar)
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
		DWORD	m_dwResult;
	}pResult[RESULT_ROW_NUM];

	ULONG uLength[RESULT_COL_NUM];

	_BEGIN_PARAM_DATA		(sPARAM, pParam, PARAM_COL_NUM)
		_BINDING_PARAM		(0, m_Result, SQL_PARAM_OUTPUT)
	_END_PARAM_DATA			()

	_BEGIN_BINDING_DATA		(sQUERY, pResult, uLength, RESULT_ROW_NUM, RESULT_COL_NUM)		
		_BINDING_COLUMN		(0, m_dwResult)
	_END_BINDING_DATA		()

//	_INIT_BINDING_DATA()

	inline BOOL ResultSuccess() { return (pResult[0].m_dwResult == 0); }

    //__NA_000313_20070223_REALTIME_CHAT_BLOCK

public:
	inline VOID		SetGMGuid( CHARGUID CharGuid )		{ m_GMGuid = CharGuid;	}
	inline CHARGUID	GetGMGuid()							{ return m_GMGuid; }
	inline VOID		SetCharGuid( CHARGUID CharGuid )	{ m_CharGuid = CharGuid;	}
	inline CHARGUID	GetCharGuid()						{ return m_CharGuid; }
	inline VOID		SetBlockTime( DWORD dwTime )		{ m_dwBlockTime = dwTime; }
	inline DWORD	GetBlockTime()						{ return m_dwBlockTime; }
private: 
	USERGUID	m_GMGuid;
	CHARGUID	m_CharGuid;
	DWORD		m_dwBlockTime;
};

// 채팅 블럭 정보
class Query_ChatBlock_Info : public QueryResult
{
	__DECL_SAFECUSTOMPOOL_PTR(Query_ChatBlock_Info)
public:
	enum
	{ 
		PARAM_ROW_NUM	= 1,
		PARAM_COL_NUM	= 1,

		RESULT_ROW_NUM	= 1,
	    //__NA_000313_20070223_REALTIME_CHAT_BLOCK
		RESULT_COL_NUM	= 1,
	};
	struct sPARAM
	{
		INT	m_Result;
	} pParam[PARAM_ROW_NUM];

	struct sQUERY
	{	
        //__NA_000313_20070223_REALTIME_CHAT_BLOCK
		DWORD	m_dwRemainTime;	// 남은 시간
	}pResult[RESULT_ROW_NUM];

	ULONG uLength[RESULT_COL_NUM];

	_BEGIN_PARAM_DATA	(sPARAM, pParam, PARAM_COL_NUM)
		_BINDING_PARAM	(0, m_Result, SQL_PARAM_OUTPUT)
	_END_PARAM_DATA		()

	_BEGIN_BINDING_DATA	(sQUERY, pResult, uLength, RESULT_ROW_NUM, RESULT_COL_NUM)		
	     //__NA_000313_20070223_REALTIME_CHAT_BLOCK
		_BINDING_COLUMN	(0, m_dwRemainTime)
	_END_BINDING_DATA	()

	inline BOOL ResultSuccess() { return (pParam[0].m_Result < 0xffffffff); }
	inline BOOL ResultCode()	{ return (pParam[0].m_Result == 0); }
public:
	VOID		SetCharGuid( CHARGUID CharGuid )	{ m_CharGuid = CharGuid;	}
	CHARGUID	GetCharGuid()						{ return m_CharGuid; }

    //__NA_000313_20070223_REALTIME_CHAT_BLOCK
private: 
	CHARGUID	m_CharGuid;
    //__NA_000313_20070223_REALTIME_CHAT_BLOCK
};

/////////////////////////
// MEMO Query
class Query_Memo_Send : public QueryResult
{
	__DECL_SAFECUSTOMPOOL_PTR(Query_Memo_Send)
public:
	enum
	{ 
		PARAM_ROW_NUM	= 1,
		PARAM_COL_NUM	= 1,

		RESULT_ROW_NUM	= 1,
		RESULT_COL_NUM	= 6,

		RETURNCODE_SUCCESS				= 0,
		RETURNCODE_FAILED				= 1,
	};
	struct sPARAM
	{
		INT	m_Result;
	} pParam[PARAM_ROW_NUM];

	struct sQUERY
	{
		MEMOGUID	m_MemoGuid;
		CHARGUID	m_RecvGuid;
		CHARGUID	m_SendGuid;
		TCHAR		m_ptszSubject[MAX_SUBJECT_STRLENGTH];
		TCHAR		m_ptszMemo[MAX_MEMO_LENGTH + 1];
		TCHAR		m_ptszCreateDate[21];
	}pResult[RESULT_ROW_NUM];

	ULONG uLength[RESULT_COL_NUM];

	inline INT ResultCode() { return pParam[0].m_Result; }

	_BEGIN_PARAM_DATA		(sPARAM, pParam, PARAM_COL_NUM)
		_BINDING_PARAM		(0, m_Result, SQL_PARAM_OUTPUT)
	_END_PARAM_DATA			()

	_BEGIN_BINDING_DATA		(sQUERY, pResult, uLength, RESULT_ROW_NUM, RESULT_COL_NUM)		
		_BINDING_COLUMN		(0, m_MemoGuid)
		_BINDING_COLUMN		(1, m_RecvGuid)
		_BINDING_COLUMN		(2, m_SendGuid)
		_BINDING_COLUMN_PTR	(3, m_ptszSubject)
		_BINDING_COLUMN_PTR	(4, m_ptszMemo)
		_BINDING_COLUMN_PTR	(5, m_ptszCreateDate)
	_END_BINDING_DATA		()

//	_INIT_BINDING_DATA()

inline BOOL ResultSuccess() { return (pParam[0].m_Result==RETURNCODE_SUCCESS); }
public:
	VOID			SetCharGuid( CHARGUID CharGuid )	{ m_CharGuid = CharGuid;	}
	CHARGUID		GetCharGuid()						{ return m_CharGuid; }
	inline VOID		SetRecvName( TCHAR * tszName )	{ memcpy( m_tszRecvName, tszName, MAX_CHARNAME_LENGTH*sizeof(TCHAR) ); }
	inline TCHAR *	GetRecvName()						{ return m_tszRecvName; }
	inline VOID		SetKey( DWORD dwKey )				{ m_dwKey = dwKey;	}
	inline DWORD	GetKey()							{ return m_dwKey; }
private: 
	DWORD		m_dwKey;
	CHARGUID	m_CharGuid;
	TCHAR		m_tszRecvName[MAX_CHARNAME_LENGTH];
};

//{__NA001254_20090109_RTTG_FUNC_ADD
/////////////////////////
// MMEMO Query
class Query_MMemo_Send : public QueryResult
{
	__DECL_SAFECUSTOMPOOL_PTR(Query_MMemo_Send)
public:
	enum
	{ 
		PARAM_ROW_NUM	= 1,
		PARAM_COL_NUM	= 1,

		RESULT_ROW_NUM	= 10,
		RESULT_COL_NUM	= 7,

		RETURNCODE_SUCCESS	= 0,
		RETURNCODE_FAILED	= 1,

		MAX_RECVNAMES_LENGHT = 255,
	};
	struct sPARAM
	{
		INT			m_Result;
	} pParam[PARAM_ROW_NUM];

	struct sQUERY
	{
		INT			m_idx;
		TCHAR		m_RecvCharName[MAX_CHARNAME_LENGTH+1];
		CHARGUID	m_RecvGuid;
		MEMOGUID	m_MemoGuid;
		TCHAR		m_ptszSubject[MAX_SUBJECT_STRLENGTH];
		TCHAR		m_ptszCreateDate[21];
		INT			m_nError;
	}pResult[RESULT_ROW_NUM];

	ULONG uLength[RESULT_COL_NUM];

	inline INT ResultCode() { return pParam[0].m_Result; }

	_BEGIN_PARAM_DATA		(sPARAM, pParam, PARAM_COL_NUM)
	_BINDING_PARAM		(0, m_Result, SQL_PARAM_OUTPUT)
	_END_PARAM_DATA			()

	_BEGIN_BINDING_DATA		(sQUERY, pResult, uLength, RESULT_ROW_NUM, RESULT_COL_NUM)		
	_BINDING_COLUMN		(0, m_idx)
	_BINDING_COLUMN_PTR	(1, m_RecvCharName)
	_BINDING_COLUMN		(2, m_RecvGuid)
	_BINDING_COLUMN		(3, m_MemoGuid)
	_BINDING_COLUMN_PTR	(4, m_ptszSubject)
	_BINDING_COLUMN_PTR	(5, m_ptszCreateDate)
	_BINDING_COLUMN		(6, m_nError)
	_END_BINDING_DATA		()


	inline BOOL ResultSuccess() { return (pParam[0].m_Result==RETURNCODE_SUCCESS); }

public:
	inline VOID		SetKey( DWORD dwKey )	{ m_dwKey = dwKey;	}
	inline DWORD	GetKey()				{ return m_dwKey;	}

	inline VOID			SetInfo( MMEMO_INFOS& MMemoInfo )	{ m_MMemoInfo = MMemoInfo; }
	inline MMEMO_INFOS&	GetInfo()							{ return m_MMemoInfo; }


private: 
	DWORD		m_dwKey;
	MMEMO_INFOS	m_MMemoInfo;

};
//}__NA001254_20090109_RTTG_FUNC_ADD

// 쪽지 목록 확인
class Query_Memo_ListSelect : public QueryResult
{
	__DECL_SAFECUSTOMPOOL_PTR(Query_Memo_ListSelect)
public:
	enum
	{ 
		PARAM_ROW_NUM	= 1,
		PARAM_COL_NUM	= 1,

		RESULT_ROW_NUM	= MAX_MEMO_NUM,
		RESULT_COL_NUM	= 6,

		RETURNCODE_SUCCESS				= 0,
		RETURNCODE_FAILED				= 1,
	};
	struct sPARAM
	{
		INT	m_Result;
	} pParam[PARAM_ROW_NUM];

	struct sQUERY
	{
		MEMOGUID	m_MemoGuid;
		TCHAR		m_ptszSendName[MAX_CHARNAME_LENGTH+1];
		TCHAR		m_ptszSubject[MAX_SUBJECT_STRLENGTH];
		BYTE		m_bRead;
		TCHAR		m_ptszSendDate[21];
        CHARGUID    m_SenderGuid;
	}pResult[RESULT_ROW_NUM];

	ULONG uLength[RESULT_COL_NUM];

	inline INT ResultCode() { return pParam[0].m_Result; }

	_BEGIN_PARAM_DATA		(sPARAM, pParam, PARAM_COL_NUM)
		_BINDING_PARAM		(0, m_Result, SQL_PARAM_OUTPUT)
	_END_PARAM_DATA			()

	_BEGIN_BINDING_DATA		(sQUERY, pResult, uLength, RESULT_ROW_NUM, RESULT_COL_NUM)		
		_BINDING_COLUMN		(0, m_MemoGuid)
		_BINDING_COLUMN		(1, m_ptszSendName)
		_BINDING_COLUMN_PTR	(2, m_ptszSubject)
		_BINDING_COLUMN		(3, m_bRead)
		_BINDING_COLUMN_PTR	(4, m_ptszSendDate)
        _BINDING_COLUMN     (5, m_SenderGuid)
	_END_BINDING_DATA		()

//	_INIT_BINDING_DATA()

inline BOOL ResultSuccess() { return (pParam[0].m_Result==RETURNCODE_SUCCESS); }
public:
	inline VOID		SetCharGuid( CHARGUID CharGuid )	{ m_CharGuid = CharGuid;	}
	inline CHARGUID	GetCharGuid()						{ return m_CharGuid; }
	inline VOID		SetKey( DWORD dwKey )				{ m_dwKey = dwKey;	}
	inline DWORD	GetKey()							{ return m_dwKey; }
private: 
	DWORD		m_dwKey;
	CHARGUID	m_CharGuid;
};

// 쪽지 확인
class Query_Memo_Select : public QueryResult
{
	__DECL_SAFECUSTOMPOOL_PTR(Query_Memo_Select)
public:
	enum
	{ 
		PARAM_ROW_NUM	= 1,
		PARAM_COL_NUM	= 1,

		RESULT_ROW_NUM	= 1,
        RESULT_COL_NUM	= 4, //_NA_20100812_INTEGRATE_SYSTEM_MEMO_DOMINATION

		RETURNCODE_SUCCESS				= 0,
		RETURNCODE_FAILED				= 1,
	};
	struct sPARAM
	{
		INT	m_Result;
	} pParam[PARAM_ROW_NUM];

	struct sQUERY
	{
		TCHAR		        m_Memo[MAX_MEMO_LENGTH + 1];
        eSYSTEM_MEMO_TYPE   system_memo_type_;//시스템 메모 구분자
        SLOTCODE            recv_item_code_;//환급 받을 아이템 코드
        ULONGLONG           amount_cost_;//환급 받을 금액
	}pResult[RESULT_ROW_NUM];

	ULONG uLength[RESULT_COL_NUM];

	inline INT ResultCode() { return pParam[0].m_Result; }

	_BEGIN_PARAM_DATA		(sPARAM, pParam, PARAM_COL_NUM)
		_BINDING_PARAM		(0, m_Result, SQL_PARAM_OUTPUT)
	_END_PARAM_DATA			()

	_BEGIN_BINDING_DATA		(sQUERY, pResult, uLength, RESULT_ROW_NUM, RESULT_COL_NUM)		
		_BINDING_COLUMN_PTR	(0, m_Memo)
        _BINDING_COLUMN     (1, system_memo_type_)
        _BINDING_COLUMN     (2, recv_item_code_)
        _BINDING_COLUMN     (3, amount_cost_)
  	_END_BINDING_DATA		()

//	_INIT_BINDING_DATA()

inline BOOL ResultSuccess() { return (pParam[0].m_Result==RETURNCODE_SUCCESS); }
public:
	inline VOID		SetCharGuid( CHARGUID CharGuid )	{ m_CharGuid = CharGuid;	}
	inline CHARGUID	GetCharGuid()						{ return m_CharGuid; }
	inline VOID		SetMemoGuid( MEMOGUID MemoGuid )	{ m_MemoGuid = MemoGuid;	}
	inline MEMOGUID	GetMemoGuid()						{ return m_MemoGuid; }
	inline VOID		SetKey( DWORD dwKey )				{ m_dwKey = dwKey;	}
	inline DWORD	GetKey()							{ return m_dwKey; }
private: 
	DWORD		m_dwKey;
	CHARGUID	m_CharGuid;
	MEMOGUID	m_MemoGuid;
};

// 쪽지 삭제
class Query_Memo_Delete : public QueryResult
{
	__DECL_SAFECUSTOMPOOL_PTR(Query_Memo_Delete)
public:
	enum
	{ 
		PARAM_ROW_NUM	= 1,
		PARAM_COL_NUM	= 1,

//		RESULT_ROW_NUM	= 1,
//		RESULT_COL_NUM	= 0,

		RETURNCODE_SUCCESS				= 0,
		RETURNCODE_FAILED				= 1,
	};
	struct sPARAM
	{
		INT	m_Result;
	} pParam[PARAM_ROW_NUM];

//	struct sQUERY
//	{
//	}pResult[RESULT_ROW_NUM];

//	ULONG uLength[RESULT_COL_NUM];

	inline INT ResultCode() { return pParam[0].m_Result; }

	_BEGIN_PARAM_DATA		(sPARAM, pParam, PARAM_COL_NUM)
		_BINDING_PARAM		(0, m_Result, SQL_PARAM_OUTPUT)
	_END_PARAM_DATA			()

//	_BEGIN_BINDING_DATA		(sQUERY, pResult, uLength, RESULT_ROW_NUM, RESULT_COL_NUM)		
//		_BINDING_COLUMN_PTR	(0, m_ptszMemo)
//	_END_BINDING_DATA		()

	_INIT_BINDING_DATA()

inline BOOL ResultSuccess() { return (pParam[0].m_Result==RETURNCODE_SUCCESS); }
public:
	inline VOID		SetCharGuid( CHARGUID CharGuid )	{ m_CharGuid = CharGuid;	}
	inline CHARGUID	GetCharGuid()						{ return m_CharGuid; }
	inline VOID		SetMemoGuid( MEMOGUID MemoGuid )	{ m_MemoGuid = MemoGuid;	}
	inline MEMOGUID	GetMemoGuid()						{ return m_MemoGuid; }
	inline VOID		SetKey( DWORD dwKey )				{ m_dwKey = dwKey;	}
	inline DWORD	GetKey()							{ return m_dwKey; }
    // added by _NA_1798_20110114_SYSTEM_MEMO_ADDITIONAL_LOGGING
    inline void     SetMemoType(int memo_type)          { m_MemoType = memo_type; }
    inline int      GetMemoType() const                 { return m_MemoType; }
private: 
	DWORD		m_dwKey;
	CHARGUID	m_CharGuid;
	MEMOGUID	m_MemoGuid;
    int         m_MemoType;
};

class Query_Interception_Request : public QueryResult
{
	__DECL_SAFECUSTOMPOOL_PTR(Query_Interception_Request)
public:
	enum
	{ 
		PARAM_ROW_NUM	= 1,
		PARAM_COL_NUM	= 1,

		RESULT_ROW_NUM	= 1,
		RESULT_COL_NUM	= 1,

		RETURNCODE_SUCCESS				= 0,
		RETURNCODE_FAILED				= 1,
	};
	struct sPARAM
	{
		INT	m_Result;
	} pParam[PARAM_ROW_NUM];

	struct sQUERY
	{
		BOOL		m_bWhisperFlag;
	}pResult[RESULT_ROW_NUM];

	ULONG uLength[RESULT_COL_NUM];

	inline INT ResultCode() { return pParam[0].m_Result; }

	_BEGIN_PARAM_DATA		(sPARAM, pParam, PARAM_COL_NUM)
		_BINDING_PARAM		(0, m_Result, SQL_PARAM_OUTPUT)
	_END_PARAM_DATA			()

	_BEGIN_BINDING_DATA		(sQUERY, pResult, uLength, RESULT_ROW_NUM, RESULT_COL_NUM)		
		_BINDING_COLUMN		(0, m_bWhisperFlag)
	_END_BINDING_DATA		()

	//	_INIT_BINDING_DATA()

	inline BOOL ResultSuccess() { return (pParam[0].m_Result==RETURNCODE_SUCCESS); }

public:
	inline VOID		SetCharGuid( CHARGUID CharGuid )	{ m_CharGuid = CharGuid;	}
	inline CHARGUID	GetCharGuid()						{ return m_CharGuid; }	
	inline VOID		SetKey( DWORD dwKey )				{ m_dwKey = dwKey;	}
	inline DWORD	GetKey()							{ return m_dwKey; }
private: 
	DWORD		m_dwKey;
	CHARGUID	m_CharGuid;
};

class GuildWarehouseLogSelectQuery: public QueryResult
{
	__DECL_SAFECUSTOMPOOL_PTR(GuildWarehouseLogSelectQuery)

public:
	enum
	{
		PARAM_ROW_NUM	= 1,
		PARAM_COL_NUM	= 1,

		RESULT_ROW_NUM = MAX_GUILDWAREHOUSE_LOGLIST_NUM,
		RESULT_COL_NUM	 = 7,

		RETURNCODE_SUCCESS = 0,
		RETURNCODE_FAILED = 1,
	};

	struct sPARAM
	{
		INT m_Result;
	} pParam[PARAM_ROW_NUM];

	struct sQUERY
	{
		GuildWarehouseLogType m_LogType;
		INT m_ItemCode;
		INT m_LimiteCode;
		WORD m_ItemNum;
		MONEY m_Money;
		TCHAR m_UsedCharName[MAX_CHARNAME_LENGTH+1];		//사용한 유저
		TCHAR m_UsedTime[MAX_TIMEDATA_SIZE+1];				//사용한 시간
	} pResult[RESULT_ROW_NUM];

	_BEGIN_PARAM_DATA (sPARAM, pParam, PARAM_COL_NUM)
		_BINDING_PARAM   (0, m_Result, SQL_PARAM_OUTPUT)
	_END_PARAM_DATA    ()

	_BEGIN_BINDING_DATA		 (sQUERY, pResult, uLength, RESULT_ROW_NUM, RESULT_COL_NUM)		
		_BINDING_COLUMN		 (0, m_LogType)
		_BINDING_COLUMN		 (1, m_UsedCharName)
		_BINDING_COLUMN	 	 (2, m_ItemCode)
		_BINDING_COLUMN	 	 (3, m_ItemNum)
		_BINDING_COLUMN		 (4, m_Money)
		_BINDING_COLUMN_PTR	 (5, m_UsedTime)
		_BINDING_COLUMN		 (6, m_LimiteCode)
	_END_BINDING_DATA		     ()

public:
	ULONG uLength[RESULT_COL_NUM];
	
public:
	int ResultCode() { return pParam[0].m_Result;}
	void SetKey(DWORD dwKey) { m_ReqUserKey = dwKey;}
	void SetCharGuid(CHARGUID CharGuid) { m_ReqCharKey = CharGuid;	}
	void SetGuildGuid(CHARGUID GuildGuid) { m_GuildGuid = GuildGuid;}
	
	
	DWORD	GetKey() { return m_ReqUserKey; }
	CHARGUID GetCharGuid() { return m_ReqCharKey; }
	GUILDGUID GetGuildGuid() { return m_GuildGuid; }
	BOOL ResultSuccess() { return (pParam[0].m_Result==RETURNCODE_SUCCESS); }

private:
	DWORD m_ReqUserKey;
	GUILDGUID m_GuildGuid;
	CHARGUID m_ReqCharKey;
};


class GuildNoticeSelectQuery : public QueryResult
{
	__DECL_SAFECUSTOMPOOL_PTR(GuildNoticeSelectQuery)
public:
	enum
	{ 
		PARAM_ROW_NUM	= 1,
		PARAM_COL_NUM	= 1,

		RESULT_ROW_NUM = 1,
		RESULT_COL_NUM	 = 4,

		RETURNCODE_SUCCESS = 0,
		RETURNCODE_FAILED = 1,
	};
	struct sPARAM
	{
		INT	m_Result;
	} pParam[PARAM_ROW_NUM];

	struct sQUERY
	{
		GUILDGUID m_OperCharGuid;
		BYTE m_CorpsNumber;
		BYTE m_NoticeType;
		TCHAR	m_tszGuildNotice[MAX_GUILDNOTICE_LENGTH+1];

		sQUERY() : m_OperCharGuid(0),m_CorpsNumber(0), m_NoticeType(0)
		{}
	}pResult[RESULT_ROW_NUM];

	ULONG uLength[RESULT_COL_NUM];

	inline INT ResultCode() { return pParam[0].m_Result; }

	_BEGIN_PARAM_DATA		(sPARAM, pParam, PARAM_COL_NUM)
		_BINDING_PARAM		(0, m_Result, SQL_PARAM_OUTPUT)
	_END_PARAM_DATA			()

	_BEGIN_BINDING_DATA			(sQUERY, pResult, uLength, RESULT_ROW_NUM, RESULT_COL_NUM)		
		_BINDING_COLUMN			(0, m_OperCharGuid)
		_BINDING_COLUMN			(1, m_CorpsNumber)
		_BINDING_COLUMN			(2, m_NoticeType)
		_BINDING_COLUMN_PTR		(3, m_tszGuildNotice)
	_END_BINDING_DATA		()

	
public:
	void SetGuildGuid(CHARGUID GuildGuid) { m_GuildGuid = GuildGuid;}
	void SetCharGuid(CHARGUID CharGuid) { m_OperCharGuid = CharGuid;	}
	void SetGuildNotice() { ZeroMemory(pResult[0].m_tszGuildNotice, MAX_GUILDNOTICE_LENGTH); }

	CHARGUID GetCharGuid() { return m_OperCharGuid; }
	GUILDGUID GetGuildGuid() { return m_GuildGuid; }
	BOOL ResultSuccess() { return (pParam[0].m_Result==RETURNCODE_SUCCESS); }

private:
	GUILDGUID m_GuildGuid;
	CHARGUID m_OperCharGuid;
};

//#ifdef _NA_0_20080612_WORLDSERVER_REFACTORING
class CommunityDataQuery : public QueryResult
{
	__DECL_SAFECUSTOMPOOL_PTR(CommunityDataQuery)
public:
	enum
	{ 
		PARAM_ROW_NUM	= 1,
		PARAM_COL_NUM	= 1,

		RESULT_ROW_NUM = 1,
		RESULT_COL_NUM	= 2,

		RETURNCODE_SUCCESS = 0,
		RETURNCODE_FAILED = 1,
	};
	struct sPARAM
	{
		INT	m_Result;
	} pParam[PARAM_ROW_NUM];

	struct sQUERY
	{
		BOOL m_bWhisperFlag;
		BYTE m_RevengePoint;
	}pResult[RESULT_ROW_NUM];

	ULONG uLength[RESULT_COL_NUM];


	_BEGIN_PARAM_DATA	(sPARAM, pParam, PARAM_COL_NUM)
		_BINDING_PARAM		(0, m_Result, SQL_PARAM_OUTPUT)
	_END_PARAM_DATA		()

	_BEGIN_BINDING_DATA	(sQUERY, pResult, uLength, RESULT_ROW_NUM, RESULT_COL_NUM)		
	_BINDING_COLUMN		(0, m_bWhisperFlag)
	_BINDING_COLUMN		(1, m_RevengePoint)
	_END_BINDING_DATA		()

public:
	BOOL ResultSuccess() { return (pParam[0].m_Result==RETURNCODE_SUCCESS); }
	VOID SetCharGuid( CHARGUID CharGuid )	{ m_CharGuid = CharGuid;	}
	CHARGUID GetCharGuid() { return m_CharGuid; }
	INT ResultCode() { return pParam[0].m_Result; }

private: 
	CHARGUID	m_CharGuid;
};

//////////////////////////////////////////////////////////////////////////
//그룹 맺기 시스템
//////////////////////////////////////////////////////////////////////////

//그룹 맴버 추가 쿼리 랩핑 클레스 
class Query_Group_Insert : public QueryResult
{
 	__DECL_SAFECUSTOMPOOL_PTR(Query_Group_Insert)
public:
	enum
	{ 
		PARAM_ROW_NUM			= 1, 					
		PARAM_COL_NUM			= 1,                                                    //리턴 값에 대한 row/col, 리턴값이 1가지일때		

		RETURNCODE_SUCCESS		= 0,
		RETURNCODE_FAIL			= 1,
		RETURNCODE_NOT_EXIST	= 2,                                                    //리턴 값에 대한 종류
	};

	struct sPARAM
	{
		INT	m_Result;
	} pParam[PARAM_ROW_NUM];                                                            //리턴값 구조체

	_BEGIN_PARAM_DATA		(sPARAM, pParam, PARAM_COL_NUM)
		_BINDING_PARAM		(0, m_Result, SQL_PARAM_OUTPUT)	                            //리턴값을 DB의 값과 연결
	_END_PARAM_DATA			()

	_INIT_BINDING_DATA()									                            //리턴되는 DB데이터가 없을 경우 
	
	inline INT ResultCode() { return pParam[0].m_Result; }	                            //리턴값 처리 함수
	inline BOOL ResultSuccess() { return (pParam[0].m_Result==RETURNCODE_SUCCESS); }	//성공

public:
	inline VOID		SetCharGuid( CHARGUID fromCharGuid, CHARGUID toCharGuid )			//사용자 정의 함수 中	
	{ 
		m_fromCharGuid = fromCharGuid;
		m_toCharGuid = toCharGuid;
	}
	inline CHARGUID	GetfromCharGuid()					{ return m_fromCharGuid; }		//사용자 정의 함수 中	
	inline CHARGUID	GettoCharGuid()						{ return m_toCharGuid; }		//사용자 정의 함수 中	
private: 
	CHARGUID	m_fromCharGuid;															//사용자 정의 변수 中	
	CHARGUID	m_toCharGuid;															//사용자 정의 변수 中	

};

//그룹 맴버 삭제 쿼리 랩핑 클레스 
class Query_Group_Delete : public QueryResult
{
	__DECL_SAFECUSTOMPOOL_PTR(Query_Group_Delete)
public:
	enum
	{ 
		PARAM_ROW_NUM			= 1, 				
		PARAM_COL_NUM			= 1,						                            //리턴 값에 대한 row/col, 리턴값이 1가지일때		

		RETURNCODE_SUCCESS		= 0,
		RETURNCODE_FAIL			= 1,						                            //리턴 값의 종류
	};

	struct sPARAM
	{
		INT	m_Result;
	} pParam[PARAM_ROW_NUM];								                            //리턴값 구조체

	_BEGIN_PARAM_DATA		(sPARAM, pParam, PARAM_COL_NUM)
		_BINDING_PARAM		(0, m_Result, SQL_PARAM_OUTPUT)	                            //리턴값을 DB의 처리 값과 연결
	_END_PARAM_DATA			()

	_INIT_BINDING_DATA()									                            //리턴되는 DB데이터가 없을 경우 

	inline INT ResultCode() { return pParam[0].m_Result; }								//리턴값 처리 함수
	inline BOOL ResultSuccess() { return (pParam[0].m_Result == RETURNCODE_SUCCESS); }	//성공

public:
	inline VOID		SetCharGuid( CHARGUID fromCharGuid, CHARGUID toCharGuid )			//사용자 정의 함수 中	
	{ 
		m_fromCharGuid = fromCharGuid;
		m_toCharGuid = toCharGuid;
	}
	inline CHARGUID	GetfromCharGuid()					{ return m_fromCharGuid; }		//사용자 정의 함수 中	
	inline CHARGUID	GettoCharGuid()						{ return m_toCharGuid; }		//사용자 정의 함수 中	
private: 
	CHARGUID	m_fromCharGuid;															//사용자 정의 변수 中	
	CHARGUID	m_toCharGuid;															//사용자 정의 변수 中	

};
//그룹 리스트 요청 쿼리 
class Query_Group_Select : public QueryResult
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_Group_Select )

public:

    enum
    { 
        PARAM_ROW_NUM	= 1,
        PARAM_COL_NUM	= 1,                                                            //리턴값

        RESULT_ROW_NUM = (MAX_GROUP_MEMBER),
        RESULT_COL_NUM	= 9,                                                            //Result row / col 값
    };

    struct sPARAM
    {
        INT m_Result;
    } pParam[PARAM_ROW_NUM];

    struct sQUERY
    {
        CHARGUID        m_SrcCharGUID;
        CHARGUID	    m_DestCharGUID;                                                 //해당 유저와 그룹 관계에 있는 CharGUID
        DWORD           m_AccumEXP;                                                     //해당 유저가 맴버가 게임에 없을때 쌓이는 보너스 경험치 
        eGROUP_STAT     m_GroupSTAT;                                                    //해당 유저와 관계를 나타내는 STAT
        TCHAR           m_ptszGroupCharName[MAX_CHARNAME_LENGTH+1];                     //그룹 맴버의 케릭터 이름
        BYTE            m_MemberClass;                                                  //그룹 멤버의 클레스
        LEVELTYPE       m_MemberLevel;                                                  //그룹 멤버의 레벨
        TCHAR	        m_MemberLastDate[MAX_TIMEDATA_SIZE+1];                          //그룹 맴버가 접속 시간
        TCHAR           m_UserLastDate[MAX_TIMEDATA_SIZE+1];                            //해당 유저의 마지막 접속 시간
    }pResult[RESULT_ROW_NUM];

    ULONG uLength[RESULT_COL_NUM];

    _BEGIN_PARAM_DATA	(sPARAM, pParam, PARAM_COL_NUM)
        _BINDING_PARAM	    (0, m_Result, SQL_PARAM_OUTPUT)                             //리턴값 바인딩  
    _END_PARAM_DATA	        ()

    _BEGIN_BINDING_DATA	(sQUERY, pResult, uLength, RESULT_ROW_NUM, RESULT_COL_NUM)		//row 와 col에 해당하는 만큼 바인딩한다.
        _BINDING_COLUMN		(0, m_SrcCharGUID)                                          //해당 유저 바인딩(불필요 할 수 있다)
        _BINDING_COLUMN		(1, m_DestCharGUID)                                         //유저의 그룹 유저 바인딩
        _BINDING_COLUMN		(2, m_AccumEXP)                                             //축적 경험치 바인딩
        _BINDING_COLUMN		(3, m_GroupSTAT)                                            //그룹내 상태(관계) 바인딩
        _BINDING_COLUMN_PTR (4, m_ptszGroupCharName)                                    //이름 바인딩
        _BINDING_COLUMN		(5, m_MemberClass)                                          //멤버 클레스 바인딩
        _BINDING_COLUMN		(6, m_MemberLevel)                                          //멤버 레벨 바인딩
        _BINDING_COLUMN     (7, m_MemberLastDate)                                       //접속 시간 바인딩
        _BINDING_COLUMN     (8, m_UserLastDate)
    _END_BINDING_DATA		()

    VOID SetCharGuid( CHARGUID CharGUID ) { m_CharGUID = CharGUID; }
    BOOL ResultSuccess() { return (pParam[0].m_Result==0); }
    CHARGUID GetCharGuid() { return m_CharGUID; }

private: 
    CHARGUID	m_CharGUID;
};

//그룹 추가 경험치 저장 랩핑 클레스
class Query_Group_AccumExp_Insert : public QueryResult
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_Group_AccumExp_Insert)
public:
    enum
    { 
        PARAM_ROW_NUM			= 1, 					
        PARAM_COL_NUM			= 1,                                                    //리턴 값에 대한 row/col, 리턴값이 1가지일때		

        RETURNCODE_SUCCESS		= 0,
        RETURNCODE_FAIL			= 1,                                                    //리턴 값에 대한 종류
    };

    struct sPARAM
    {
        INT	m_Result;
    } pParam[PARAM_ROW_NUM];                                                            //리턴값 구조체

    _BEGIN_PARAM_DATA		(sPARAM, pParam, PARAM_COL_NUM)
        _BINDING_PARAM		(0, m_Result, SQL_PARAM_OUTPUT)	                            //리턴값을 DB의 값과 연결
    _END_PARAM_DATA			()

    _INIT_BINDING_DATA()									                            //리턴되는 DB데이터가 없을 경우 

    inline INT ResultCode() { return pParam[0].m_Result; }	                            //리턴값 처리 함수

public:
    VOID SetUserKey( DWORD userKey ) { m_UserKey = userKey; }                           //유저 키값 세팅
    DWORD GetUserKey( ) { return m_UserKey; }
private:
    DWORD m_UserKey;
};

class Query_Group_AccumExp_Update : public QueryResult
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_Group_AccumExp_Update)
public:
    enum
    { 
        PARAM_ROW_NUM			= 1, 					
        PARAM_COL_NUM			= 1,	

        RETURNCODE_SUCCESS		= 0,
        RETURNCODE_FAIL			= 1,                                                
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
    VOID SetCharGuid( CHARGUID charGuid ) { m_CharGuid = charGuid; }                       
    CHARGUID GetCharGuid( ) { return m_CharGuid; }
private:
    CHARGUID m_CharGuid;
};

// used by __NA_001359_20090619_HEIM_LOTTO_SYSTEM
// 하임 로또 응모 내역 로딩
class Query_LottoEntries_Select : public QueryResult
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_LottoEntries_Select)
public:
    enum
    { 
        PARAM_ROW_NUM	= 1,
        PARAM_COL_NUM	= 4,

        RESULT_ROW_NUM = (LOTTO_NUM_RECENT_DRAWS * LOTTO_MAX_TICKETS),
        RESULT_COL_NUM	= 7,
    };

    struct sPARAM
    {
        INT	m_Result;
        INT m_NumTickets;
        INT m_NumUsedTickets;
        INT m_EffectivePayment;
    } pParam[PARAM_ROW_NUM];

    struct sQUERY
    {
        INT32   m_DrawNum;
        BYTE    m_Status;
        INT64   m_Prize;
        BYTE    m_PickedNum1;
        BYTE    m_PickedNum2;
        BYTE    m_PickedNum3;
        BYTE    m_PickedNum4;
    } pResult[RESULT_ROW_NUM];

    ULONG uLength[RESULT_COL_NUM];

    _BEGIN_PARAM_DATA   (sPARAM, pParam, PARAM_COL_NUM)
        _BINDING_PARAM      (0, m_Result, SQL_PARAM_OUTPUT)
        _BINDING_PARAM      (1, m_NumTickets, SQL_PARAM_OUTPUT)
        _BINDING_PARAM      (2, m_NumUsedTickets, SQL_PARAM_OUTPUT)
        _BINDING_PARAM      (3, m_EffectivePayment, SQL_PARAM_OUTPUT)
    _END_PARAM_DATA     ()

    _BEGIN_BINDING_DATA (sQUERY, pResult, uLength, RESULT_ROW_NUM, RESULT_COL_NUM)		
        _BINDING_COLUMN     (0, m_DrawNum)
        _BINDING_COLUMN     (1, m_Status)
        _BINDING_COLUMN     (2, m_Prize)
        _BINDING_COLUMN     (3, m_PickedNum1)
        _BINDING_COLUMN     (4, m_PickedNum2)
        _BINDING_COLUMN     (5, m_PickedNum3)
        _BINDING_COLUMN     (6, m_PickedNum4)
    _END_BINDING_DATA   ()

    BOOL ResultSuccess() { return (pParam[0].m_Result == 0); }

public:
    BYTE     flag;
    CHARGUID charGuid;
    BYTE     inPreLoading;
};

// used by __NA_001359_20090619_HEIM_LOTTO_SYSTEM, 하임 로또 결과 정보 로딩
class Query_LottoResults_Select : public QueryResult
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_LottoResults_Select)
public:
    enum
    { 
        PARAM_ROW_NUM	= 1,
        PARAM_COL_NUM	= 1,

        RESULT_ROW_NUM = LOTTO_NUM_RECENT_DRAWS,
        RESULT_COL_NUM	= 8,
    };

    struct sPARAM
    {
        INT	m_Result;
    } pParam[PARAM_ROW_NUM];

    struct sQUERY
    {
        INT32   m_DrawNum;
        INT64   m_TotalAccum;
        INT32   m_NumWinners;
        INT64   m_Jackpot;
        BYTE    m_WinningNum1;
        BYTE    m_WinningNum2;
        BYTE    m_WinningNum3;
        BYTE    m_WinningNum4;
    } pResult[RESULT_ROW_NUM];

    ULONG uLength[RESULT_COL_NUM];

    _BEGIN_PARAM_DATA   (sPARAM, pParam, PARAM_COL_NUM)
        _BINDING_PARAM      (0, m_Result, SQL_PARAM_OUTPUT)
    _END_PARAM_DATA     ()

    _BEGIN_BINDING_DATA (sQUERY, pResult, uLength, RESULT_ROW_NUM, RESULT_COL_NUM)		
        _BINDING_COLUMN     (0, m_DrawNum)
        _BINDING_COLUMN     (1, m_TotalAccum)
        _BINDING_COLUMN     (2, m_NumWinners)
        _BINDING_COLUMN     (3, m_Jackpot)
        _BINDING_COLUMN     (4, m_WinningNum1)
        _BINDING_COLUMN     (5, m_WinningNum2)
        _BINDING_COLUMN     (6, m_WinningNum3)
        _BINDING_COLUMN     (7, m_WinningNum4)
    _END_BINDING_DATA   ()

    BOOL ResultSuccess() { return (pParam[0].m_Result == 0); }
};

// used by __NA_001359_20090619_HEIM_LOTTO_SYSTEM, 하임 로또 당첨자 목록 로딩
class Query_LottoWinners_Select : public QueryResult
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_LottoWinners_Select)
public:
    enum
    { 
        PARAM_ROW_NUM	= 1,
        PARAM_COL_NUM	= 2,

        RESULT_ROW_NUM  = LOTTO_NUM_WINNERS_PER_PAGE,
        RESULT_COL_NUM	= 7,
    };

    struct sPARAM
    {
        INT	m_Result;
        INT m_EndPage;
    } pParam[PARAM_ROW_NUM];

    struct sQUERY
    {
        INT64   m_Idx;
        CHARGUID m_CharGuid;
        BYTE    m_Status;
        BYTE    m_PickedNum1;
        BYTE    m_PickedNum2;
        BYTE    m_PickedNum3;
        BYTE    m_PickedNum4;
    } pResult[RESULT_ROW_NUM];

    ULONG uLength[RESULT_COL_NUM];

    _BEGIN_PARAM_DATA   (sPARAM, pParam, PARAM_COL_NUM)
        _BINDING_PARAM      (0, m_Result, SQL_PARAM_OUTPUT)
        _BINDING_PARAM      (1, m_EndPage, SQL_PARAM_OUTPUT)
    _END_PARAM_DATA     ()

    _BEGIN_BINDING_DATA (sQUERY, pResult, uLength, RESULT_ROW_NUM, RESULT_COL_NUM)		
        _BINDING_COLUMN     (0, m_Idx)
        _BINDING_COLUMN     (1, m_CharGuid)
        _BINDING_COLUMN     (2, m_Status)
        _BINDING_COLUMN     (3, m_PickedNum1)
        _BINDING_COLUMN     (4, m_PickedNum2)
        _BINDING_COLUMN     (5, m_PickedNum3)
        _BINDING_COLUMN     (6, m_PickedNum4)
    _END_BINDING_DATA   ()

    BOOL ResultSuccess() { return (pParam[0].m_Result == 0); }

public:
    WORD wDrawNum;
    INT  iPage;
};

// used by __NA_001359_20090619_HEIM_LOTTO_SYSTEM, 하임 로또 응모 처리
class Query_LottoEntry_Insert : public QueryResult
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_LottoEntry_Insert)
public:
    enum
    { 
        PARAM_ROW_NUM	= 1,
        PARAM_COL_NUM	= 1,

        RETURN_CODE_SUCCESS = 0,
        RETURN_CODE_FAILURE = 1,
        RETURN_CODE_NOT_ENOUGH_TICKET = 2,
    };

    struct sPARAM {
        INT	m_Result;
    } pParam[PARAM_ROW_NUM];

    _BEGIN_PARAM_DATA   (sPARAM, pParam, PARAM_COL_NUM)
        _BINDING_PARAM      (0, m_Result, SQL_PARAM_OUTPUT)
    _END_PARAM_DATA     ()

    _INIT_BINDING_DATA()

    BOOL ResultSuccess() { return (pParam[0].m_Result == RETURN_CODE_SUCCESS); }

public:
    DWORD userKey;
    CHARGUID charGuid;
    WORD drawNum;
    BYTE isAuto;
    BYTE status;
    BYTE pickedNum[LOTTO_NUM_PICKS];
};

// used by __NA_001359_20090619_HEIM_LOTTO_SYSTEM, 하임 로또 추첨 및 이월 처리
class Query_LottoResults_Insert : public QueryResult
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_LottoResults_Insert)
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

// used by __NA_001359_20090619_HEIM_LOTTO_SYSTEM, 하임 로또 총 적립액 갱신
class Query_LottoTotal_Update : public QueryResult
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_LottoTotal_Update)
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

// used by __NA_001359_20090619_HEIM_LOTTO_SYSTEM, 하임 로또 복권 용지 수 증가
class Query_LottoTicket_Insert : public QueryResult
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_LottoTicket_Insert)
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

public:
    CHARGUID charGuid;
    BYTE     numTickets;
};

// used by __NA_001359_20090619_HEIM_LOTTO_SYSTEM, 하임 로또 당첨금 지급
class Query_LottoMoney_Insert : public QueryResult
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_LottoMoney_Insert)
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
        INT	m_Result;
    } pParam[PARAM_ROW_NUM];

    struct sQUERY {
        TCHAR m_RecvName[MAX_CHARNAME_LENGTH + 1];
    } pResult[RESULT_ROW_NUM];

    ULONG uLength[RESULT_COL_NUM];

    _BEGIN_PARAM_DATA   (sPARAM, pParam, PARAM_COL_NUM)
        _BINDING_PARAM      (0, m_Result, SQL_PARAM_OUTPUT)
    _END_PARAM_DATA     ()

    _BEGIN_BINDING_DATA (sQUERY, pResult, uLength, RESULT_ROW_NUM, RESULT_COL_NUM)		
        _BINDING_COLUMN_PTR (0, m_RecvName)
    _END_BINDING_DATA   ()

    BOOL ResultSuccess() { return (pParam[0].m_Result == RETURN_CODE_SUCCESS); }

public:
    DWORD dbUserKey;
    INT64 entryIdx;
    WORD  drawNum;
    CHARGUID charGuid;
    BYTE  status;
    INT64 prize;
};

// used by __NA_001359_20090619_HEIM_LOTTO_SYSTEM, 하임 로또 당첨안내 쪽지 목록 로딩
class Query_LottoNotice_Select : public QueryResult
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_LottoNotice_Select)
public:
    enum
    { 
        PARAM_ROW_NUM	= 1,
        PARAM_COL_NUM	= 1,

        RESULT_ROW_NUM  = LOTTO_MAX_NOTICE,
        RESULT_COL_NUM	= 4,
    };

    struct sPARAM
    {
        INT	m_Result;
    } pParam[PARAM_ROW_NUM];

    struct sQUERY
    {
        INT64 m_EntryIdx;
        INT32 m_DrawNum;
        BYTE  m_Rank;
        INT64 m_Prize;
    } pResult[RESULT_ROW_NUM];

    ULONG uLength[RESULT_COL_NUM];

    _BEGIN_PARAM_DATA   (sPARAM, pParam, PARAM_COL_NUM)
        _BINDING_PARAM      (0, m_Result, SQL_PARAM_OUTPUT)
    _END_PARAM_DATA     ()

    _BEGIN_BINDING_DATA (sQUERY, pResult, uLength, RESULT_ROW_NUM, RESULT_COL_NUM)		
        _BINDING_COLUMN     (0, m_EntryIdx)
        _BINDING_COLUMN     (1, m_DrawNum)
        _BINDING_COLUMN     (2, m_Rank)
        _BINDING_COLUMN     (3, m_Prize)
    _END_BINDING_DATA   ()

    BOOL ResultSuccess() { return (pParam[0].m_Result == 0); }

public:
    CHARGUID iCharGuid;
};

// used by __NA_001359_20090619_HEIM_LOTTO_SYSTEM, 하임 로또 당첨안내 쪽지 삭제
class Query_LottoNotice_Delete : public QueryResult
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_LottoNotice_Delete)
public:
    enum
    { 
        PARAM_ROW_NUM	= 1,
        PARAM_COL_NUM	= 3,

        RETURN_CODE_SUCCESS = 0,
        RETURN_CODE_FAILURE = 1,
    };

    struct sPARAM {
        INT	m_Result;
        CHARGUID m_CharGuid;
        INT64 m_Prize;
    } pParam[PARAM_ROW_NUM];

    _BEGIN_PARAM_DATA   (sPARAM, pParam, PARAM_COL_NUM)
        _BINDING_PARAM      (0, m_Result, SQL_PARAM_OUTPUT)
        _BINDING_PARAM      (1, m_CharGuid, SQL_PARAM_OUTPUT)
        _BINDING_PARAM      (2, m_Prize, SQL_PARAM_OUTPUT)
    _END_PARAM_DATA     ()

    _INIT_BINDING_DATA()

    BOOL ResultSuccess() { return (pParam[0].m_Result == RETURN_CODE_SUCCESS); }

public:
    CHARGUID charGuid;
};

// used by __NA_001359_20090619_HEIM_LOTTO_SYSTEM, 캐릭터 유효 소비 하임 액수 저장
class Query_LottoPayment_Update : public QueryResult
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_LottoPayment_Update)
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

class Query_Domination_AuctionMoney_Delete : public QueryResult
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_Domination_AuctionMoney_Delete)
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
        MAPCODE m_MapCode;
        CHARGUID m_CharGuid;
        MONEY m_AmountMoney;
        eSYSTEM_MEMO_TYPE RewardFlag;
    } pResult[RESULT_ROW_NUM];

    ULONG uLength[RESULT_COL_NUM];

    _BEGIN_PARAM_DATA   (sPARAM, pParam, PARAM_COL_NUM)
        _BINDING_PARAM      (0, m_Result, SQL_PARAM_OUTPUT)
        _END_PARAM_DATA     ()

    _BEGIN_BINDING_DATA (sQUERY, pResult, uLength, RESULT_ROW_NUM, RESULT_COL_NUM)		
        _BINDING_COLUMN     (0, m_CharGuid)
        _BINDING_COLUMN     (1, m_AmountMoney)
        _BINDING_COLUMN     (2, RewardFlag)
        _BINDING_COLUMN     (3, m_MapCode)
    _END_BINDING_DATA   ()

        BOOL ResultSuccess() { return (pParam[0].m_Result == RETURN_CODE_SUCCESS); }
        VOID SetCharGuid( CHARGUID charGuid ){ m_recvCharGuid = charGuid; }
        CHARGUID GetCharGuid() { return m_recvCharGuid; }
        VOID SetMoneyType( eSYSTEM_MEMO_TYPE type ){ m_TypeMoney = type; }
        eSYSTEM_MEMO_TYPE GetMoneyType(){ return m_TypeMoney; }
        VOID SetMapCode(MAPCODE mapCode){ m_RequestMapCode = mapCode; }
        MAPCODE GetMapCode(){ return m_RequestMapCode; }

public:
    CHARGUID m_recvCharGuid;
    eSYSTEM_MEMO_TYPE m_TypeMoney;
    MAPCODE m_RequestMapCode;
};
