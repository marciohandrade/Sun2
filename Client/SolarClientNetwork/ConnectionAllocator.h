#pragma once

#include "Connection.h"

class ConnectionAllocator
{
public:
	ConnectionAllocator();
	virtual ~ConnectionAllocator();

	VOID Initialize( int iPoolSize )
	{
		m_iAvailableIndex			= m_iPoolSize = iPoolSize;
		m_ppConnectionPool			= new Connection*[ iPoolSize ];
		m_pConnectionObject			= new Connection[ iPoolSize ];
		for( int i = 0 ; i < iPoolSize ; ++i )
		{
			m_ppConnectionPool[i] = &m_pConnectionObject[i];
			m_ppConnectionPool[i]->SetArrayIndex(i);
		}
	}

	VOID Release()
	{
		if( m_ppConnectionPool )
		{
			delete [] m_ppConnectionPool;
			m_ppConnectionPool = NULL;
		}
		if( m_pConnectionObject )
		{
			delete [] m_pConnectionObject;
			m_pConnectionObject = NULL;
		}
		
	}

	inline Connection * GetConnection( DWORD dwIndex )
	{
		return &m_pConnectionObject[ dwIndex ];
	}

	// 사용하지 않는 Connection을 넘겨줌
	inline Connection * AllocConnection()
	{
		if( --m_iAvailableIndex == -1 )
		{
			m_iAvailableIndex = 0;
			return NULL;
		}
		else
		{
			return m_ppConnectionPool[m_iAvailableIndex];
		}
	}

	// 
	inline BOOL FreeConnection( Connection * pConn )
	{
		if( m_iAvailableIndex  < m_iPoolSize )
			m_ppConnectionPool[ m_iAvailableIndex++ ] = pConn;
		else
			return FALSE;
		return TRUE;
	}
	

private:
	int					m_iAvailableIndex;
	int					m_iPoolSize;
	Connection **		m_ppConnectionPool;
	Connection *		m_pConnectionObject;
};