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
	BYTE				m_btLogout;					// ������ �������(0 : �α���, 1 : �α׾ƿ�)�� ����Ǵ��� ����
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


















