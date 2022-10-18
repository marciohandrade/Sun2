#pragma once


#include "Query_Base.h"



class QueryAuthLog : public QueryObject
{
public:
	QueryAuthLog() : QueryObject( QUERYTYPE_AUTHLOG )
	{
		m_dwLogSeq = 0;
		m_btLogout = 0;
        ZeroMemory(m_account, sizeof(m_account));
	}

	VOID				onResult();
	DB_TYPE				GetDBType()		{ return DB_TYPE_LOG; }

	DWORD				m_dwLogSeq;
	BYTE				m_btLogout;					// 쿼리가 어느시점(0 : 로그인, 1 : 로그아웃)에 실행되는지 여부
    TCHAR				m_account[ID_MAX_LEN + 1];
};

class QueryAuthLogUpdate : public QueryObject
{
public:
	QueryAuthLogUpdate() : QueryObject( QUERYTYPE_AUTHLOG_UPDATE )
	{
	}

	DB_TYPE				GetDBType()		{ return DB_TYPE_LOG; }
};


















