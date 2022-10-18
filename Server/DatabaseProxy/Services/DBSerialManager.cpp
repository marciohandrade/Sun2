#include "StdAfx.h"

#include "QueryObjects/SolarDatabaseInDBP.h"

#include "DBProxyServerEx.h"
#include "DBSerialManager.h"
#include "QueryObjects/DatabaseProxyQuery.h"

//==================================================================================================

DBSerialManager* g_pDBSerialManager = NULL;

//==================================================================================================

DBSerialManager::DBSerialManager(SolarDatabaseInDBP& rDB, DBProxyServerEx& db_proxy):
	m_DB(rDB),
	m_DB_Prxoy(db_proxy),
	m_bQuerying(FALSE),
	m_Interval(0),
	m_CurSerial(0),
	m_MaxSerial(0),
	m_RemainSerialNum(0)
{
}

void	DBSerialManager::ExecuteDBQuery()
{
	// 쿼리중...
	m_bQuerying = TRUE;

	STRING_SAFER_QUERY32 pszQueryBuffer;
	pszQueryBuffer.MakeString( _T("S_CreateItemSeq %u"), m_Interval );

	CreateItemSerialQuery* pQuery = CreateItemSerialQuery::ALLOC();
	pQuery->SetIndex( DBProxyServerEx::QUERY_SELECTDBSERIAL );
	pQuery->SetVoidObject( &m_DB_Prxoy );
	pQuery->SetQuery( pszQueryBuffer );

	m_DB.DBQuery( pQuery );
}

void	DBSerialManager::DBResult( bool bSuccess, DBSERIAL DBMinSerial, DBSERIAL DBMaxSerial )
{
	if( bSuccess )
	{
		assert( DBMinSerial < DBMaxSerial );

		if( 0 == m_RemainSerialNum )
		{
			m_CurSerial = DBMinSerial;
			m_MaxSerial = DBMaxSerial;
		}
		else
		{
			m_MinQueue.push_back( DBMinSerial );
			m_MaxQueue.push_back( DBMaxSerial );
		}
		m_RemainSerialNum += (DBMaxSerial-DBMinSerial);
	}

	m_bQuerying = FALSE;
}

DBSERIAL DBSerialManager::AllocSerial()
{
	// 발급 조건 체크
	if( !m_bQuerying ) 
	{
		// 남아있는 시리얼이 별로 없을 때, 발급
		if( m_RemainSerialNum < m_Interval/2+1 )
		{
			ExecuteDBQuery();
		}
	}

	if( m_RemainSerialNum > 0 )
	{
		--m_RemainSerialNum;

		if( m_CurSerial < m_MaxSerial )
		{
			return m_CurSerial++;
		}
		else if( m_MinQueue.size() && m_MaxQueue.size() )
		{
			m_CurSerial = *m_MinQueue.begin();
			m_MaxSerial = *m_MaxQueue.begin();
			m_MinQueue.pop_front();
			m_MaxQueue.pop_front();
			return m_CurSerial++;
		}
		else
		{
			//	(WAVERIX)(080506)
			//	-	... 64bit 자료형 입니다만...? -.-;;;
			SUNLOG( eCRITICAL_LOG, "[AllocSerial] Can't generate DBSerial! CurSerial[%u] MaxSerial[%u]", m_CurSerial, m_MaxSerial );
		}
	}

	return 0;
}

void	DBSerialManager::Output()
{
	SUNLOG( eCRITICAL_LOG, _T("[Serial] Interval:%u, Remain:%u, Cur:%u, Max%u"),
		GetInterval(), GetRemainSerialNum(), GetCurSerial(), GetMaxSerial() );

	do {
		SERIAL_QUEUE::iterator it, end(m_MinQueue.end());
		for( it = m_MinQueue.begin() ; it != end ; ++it )
		{
			SUNLOG( eCRITICAL_LOG, "[Serial] QMin: %u", *it );
		}
	} while(0);

	do {
		SERIAL_QUEUE::iterator it, end(m_MaxQueue.end());
		for( it = m_MaxQueue.begin() ; it != end ; ++it )
		{
			SUNLOG( eCRITICAL_LOG, "[Serial] QMax: %u", *it );
		}
	} while(0);
}

