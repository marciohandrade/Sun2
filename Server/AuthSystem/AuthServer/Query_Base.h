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
	QUERYTYPE_TOKEN_UPDATE,		// �߱� ��ū ����, ��ū ���� ������Ʈ �Ѵ�. __CN_20100909_SECURITY_TOKEN__
    QUERYTYPE_PCROOM_IPLIST,    //�ǽù� GUID �� ������ ����� ������ �´�.
    QUERYTYPE_CHECK_PASSWORD,   // _CN_ZY_AUTH_PROCESS
    QUERYTYPE_END,
    QUERYTYPE_MAX,
};

// eXtreme�� DB Lib�� �ּҴ����� 2����Ʈ�� �Ǿ�� �Ѵ�. m_Record�� �������� BYTE�� ���� ����.
// ���� adParamReturnValue���� ����ϴ� �Ķ���͵� 4����Ʈ �����̴�. 1BYTE�� ����ϸ� �ڿ� �������� ����ȴ�.


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

// Query�� ���Ḧ �˸��� ����������Ʈ
class QueryEnd : public QueryObject
{
public:
	QueryEnd() : QueryObject( QUERYTYPE_END )
	{
	}

	DB_TYPE				GetDBType()		{ return DB_TYPE_MAX; }
};















