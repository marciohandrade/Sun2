#pragma once
//=============================================================================================================================
/// DB Serial 관리자
/**
	@author
			Min-Wook Kim <taiyo@webzen.co.kr>
	@since
			2005. 4. 7 
	@remarks
			- 사용범위 : [m_CurSerial, m_MaxSerial) : ++m_CurSerial; 아니라 m_CurSerial++;
	@history
			- 2006.6.13(토고전) : 시작시리얼~끝시리얼의 group을 관리하게 변경
*/
//=============================================================================================================================
#include <deque>
#include "QueryObjects/StructInQuery.h"

typedef std::deque<DBSERIAL>	SERIAL_QUEUE;

class SolarDatabaseInDBP;
class DBProxyServerEx;

class DBSerialManager
{
public:
			DBSerialManager(SolarDatabaseInDBP& rDB, DBProxyServerEx& db_proxy);
			~DBSerialManager(void){}

			void		ExecuteDBQuery();
			void		DBResult( bool bSuccess, DBSERIAL DBMinSerial, DBSERIAL DBMaxSerial );

			DBSERIAL	AllocSerial();

			void		Output();

private:

			SolarDatabaseInDBP&	m_DB;
			DBProxyServerEx&	m_DB_Prxoy;
			BOOL				m_bQuerying;		//< 쿼리중

			SERIAL_QUEUE		m_MinQueue;
			SERIAL_QUEUE		m_MaxQueue;

private:
			__PROPERTY( INT64, Interval );
			__PROPERTY( DBSERIAL, CurSerial );
			__PROPERTY( DBSERIAL, MaxSerial );
			__PROPERTY( INT64, RemainSerialNum );
    __DISABLE_COPY(DBSerialManager);
};

extern DBSerialManager* g_pDBSerialManager;
