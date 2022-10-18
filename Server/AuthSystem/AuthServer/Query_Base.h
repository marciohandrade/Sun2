#pragma once

#include <extreme/IO/EventQueue.h>
#include <extreme/dbc/RecordSet.h>
#include "QueryStruct.h"


enum eQUERYTYPE
{
    QUERYTYPE_LOGINUSER,
    QUERYTYPE_USERSTS,
    QUERYTYPE_RECOVERSTS,
    QUERYTYPE_AUTHLOG,
    QUERYTYPE_CONTROLIP,
    QUERYTYPE_GAMEVERSION,
    QUERYTYPE_INCRESEAUTHFAILCOUNT,
    QUERYTYPE_AUTHLOG_UPDATE,
    QUERYTYPE_NETCATFE_LOG,   // Japan _NET_CAFE
    QUERYTYPE_NETCATFE_LOGIN, // (WAVERIX) (090708) (CHANGES) x (BUG-FIX)
    QUERYTYPE_NETCAFE_OTP_CHECK,    // Japan _JP000000_ONETIMEPASSWORD_AUTHFLOW_
    QUERYTYPE_USERID,   //__NA000000_SHOP_USERID_50_PASSING_PROC__
    QUERYTYPE_GSP_CHECK_AUTH,       // GSP DB   - Check AuthKey
    QUERYTYPE_GSP_ACCOUNT_STATE,    // GSP DB   - Change Login User State
    QUERYTYPE_GSP_CONNECT_LOG,      // GSP DB   - Logging User State
    QUERYTYPE_GSP_ACCOUNT_ID,       // GSP DB   - Get Account ID by GameID
    QUERYTYPE_GSP_ACCOUNT_ID_BY_GUID, // GSP DB   - Get Account ID by AccountGuid
	QUERYTYPE_TOKEN_UPDATE,		// 중국 토큰 인증, 토큰 값을 업데이트 한다. __CN_20100909_SECURITY_TOKEN__
    QUERYTYPE_PCROOM_IPLIST,    //피시방 GUID 로 아이피 목록을 가지고 온다.
    QUERYTYPE_CHECK_PASSWORD,   // _CN_ZY_AUTH_PROCESS
    QUERYTYPE_END,
    QUERYTYPE_MAX,
};

// eXtreme의 DB Lib는 최소단위가 2바이트가 되어야 한다. m_Record의 변수들은 BYTE를 쓰지 못함.
// 또한 adParamReturnValue으로 사용하는 파라미터도 4바이트 고정이다. 1BYTE를 사용하면 뒤에 변수들이 변경된다.


class QueryObject : public eXtreme::EventHandler
{
public:
	QueryObject( eQUERYTYPE type ) : m_queryType( type ) {}
	virtual ~QueryObject() {}

	VOID				Release()	{ m_recordSet.Release(); }

	virtual VOID		onResult()
	{
		DWORD DelayTime = ::GetTickCount() - m_StartTime;
		if( DelayTime > 1000*10 )
		{
			SUNLOG( eCRITICAL_LOG, "[onResult] Query Delay[%u] QueryType[%u]", DelayTime, m_queryType );
		}
	}

	virtual DB_TYPE		GetDBType()	= 0;

	DWORD				m_key;
	eQUERYTYPE			m_queryType;
	eXtreme::RecordSet	m_recordSet;
	std::string			m_query;
	DWORD				m_StartTime;
	BOOL				m_Success;
	std::string			m_spName;

private:
	void				HandleEvent( eXtreme::AsyncResult* result )	{ __UNUSED(result); }
	eXtreme::Socket*	GetHandle()		{ return 0;	}
	char*				GetBuffer()		{ return 0;	}
	int					GetBufferSize()	{ return 0;	}
};

// Query의 종료를 알리는 쿼리오브젝트
class QueryEnd : public QueryObject
{
public:
	QueryEnd() : QueryObject( QUERYTYPE_END )
	{
	}

	DB_TYPE				GetDBType()		{ return DB_TYPE_MAX; }
};















