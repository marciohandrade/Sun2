#pragma once


#include "Query_Base.h"


class QueryControlIp : public QueryObject
{
public:
	QueryControlIp() : QueryObject( QUERYTYPE_CONTROLIP )
	{
		m_recordSet.RegisterVariable( &m_Record.ti_Type, "ti_Type" );
		m_recordSet.RegisterVariable( m_Record.vc_IPAddress, "vc_IPAddress" );
		m_recordSet.RegisterVariable( &m_Record.i_IPAddress, "i_IPAddress" );
		m_recordSet.RegisterVariable( &m_Record.i_IPAddressCount, "i_IPAddressCount" );
	}

	VOID				onResult();
	DB_TYPE				GetDBType()		{ return DB_TYPE_RTT; }

	DB_CONTROL_IP_INFO	m_Record;
};

class QueryGameVersionUpdate : public QueryObject
{
public:
	QueryGameVersionUpdate() : QueryObject( QUERYTYPE_GAMEVERSION )
	{
		m_recordSet.RegisterVariable( &m_Record.ti_Type, "ti_Type" );
		m_recordSet.RegisterVariable( m_Record.vc_Version, "vc_Version" );
	}

	VOID				onResult();
	DB_TYPE				GetDBType()		{ return DB_TYPE_RTT; }

	DB_GAME_VERSION		m_Record;

private:
	VOID				ParseVersion( char* version, BYTE* out );
};
















