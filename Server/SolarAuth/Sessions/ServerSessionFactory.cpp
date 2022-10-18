
#include "stdafx.h"
#include "ServerSessionFactory.h"
#include <Macro.h>

#include "TempServerSession.h"
#include "MasterServerSession.h"

ServerSessionFactory::ServerSessionFactory()
{
	m_pTempServerPool	= new CMemoryPoolFactory<TempServerSession>;
	m_pMasterServerPool	= new CMemoryPoolFactory<MasterServerSession>;

	m_pMasterServerPool->Initialize( 20, 10 );
	m_pTempServerPool->Initialize( 20, 10 );
}

ServerSessionFactory::~ServerSessionFactory()
{
	SAFE_DELETE( m_pTempServerPool );
	SAFE_DELETE( m_pMasterServerPool );
}

SERVER_SESSION_DELEGATOR	ServerSessionFactory::AllocServerSession( eSERVER_TYPE eServerType )
{
	ServerSession* pSession = NULL;

	switch( eServerType )
	{
	case TEMP_SERVER:
		{
			pSession = m_pTempServerPool->Alloc();
		}
		break;
	case MASTER_SERVER:
		{
			pSession = m_pMasterServerPool->Alloc();
		}
		break;
	default:
		{
			SUNLOG( eFULL_LOG,  "[ServerSessionFactory::AllocServerSession] �߸��� ���� Ÿ���̴�.Ǯ���� �Ҵ��� �� �����ϴ�." );
			return NULL;
		}
	}

	//pSession->Clear();
	pSession->Init();

	return pSession;
}

BOOL ServerSessionFactory::FreeServerSession( SERVER_SESSION_DELEGATOR pServerSession )
{
	if( pServerSession == NULL )
		return FALSE;

	switch( pServerSession->GetServerType() )
	{
	case TEMP_SERVER:
		return m_pTempServerPool->Free( (TempServerSession *)pServerSession() );
	case MASTER_SERVER:
		return m_pMasterServerPool->Free( (MasterServerSession *)pServerSession() );
	default:
		{
			SUNLOG( eFULL_LOG,  "[ServerSessionFactory::FreeServerSession] �߸��� Ÿ���� ���������� Free�� �� ����" );
		}
	}
	return FALSE;
}