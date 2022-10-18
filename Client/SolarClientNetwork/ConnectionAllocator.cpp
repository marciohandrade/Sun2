#include "StdAfx.h"
#include ".\connectionallocator.h"

ConnectionAllocator::ConnectionAllocator()
{
}

ConnectionAllocator::~ConnectionAllocator()
{
	ASSERT( NULL == m_ppConnectionPool );
	ASSERT( NULL == m_pConnectionObject );
}
