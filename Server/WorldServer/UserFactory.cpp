#include "stdafx.h"
#include ".\userfactory.h"
#include ".\User.h"
#include "TempUser.h"
#include "ActiveUser.h"

UserFactory::UserFactory()
{
	m_pTempUserPool		= new util::CMemoryPoolFactory<TempUser>;
	m_pActiveUserPool	= new util::CMemoryPoolFactory<ActiveUser>;
}

UserFactory::~UserFactory()
{
	delete m_pTempUserPool;
	delete m_pActiveUserPool;
}

VOID UserFactory::Release()
{
	m_pTempUserPool->Release();
	m_pActiveUserPool->Release();
}

VOID UserFactory::Init( DWORD dwUserSIze )
{
	//m_pTempUserPool->Initialize( 1, dwUserSIze/2+1 );
	//m_pActiveUserPool->Initialize( 1, dwUserSIze/2+1 );
	
	m_pTempUserPool->Initialize( dwUserSIze, dwUserSIze/2+1 );
	m_pActiveUserPool->Initialize( dwUserSIze, dwUserSIze/2+1 );
}

User* UserFactory::Alloc( eUSER_TYPE type )
{
	User *pUser = NULL;

	switch( type )
	{
	case TEMP_USER:
		pUser = (User*)m_pTempUserPool->Alloc();
		break;
	case ACTIVE_USER:
		pUser = (User*)m_pActiveUserPool->Alloc();
		break;
	default:
		assert( !"잘못된 유져 타입 Alloc" );
	}
	
	pUser->Init();

	return pUser;
}

VOID UserFactory::Free( User* pUser )
{
	pUser->Release();

	switch( pUser->GetUserType() )
	{
	case TEMP_USER:
		m_pTempUserPool->Free( (TempUser*)pUser );
		break;
	case ACTIVE_USER:
		m_pActiveUserPool->Free( (ActiveUser*)pUser );
		break;
	default:
		assert( !"잘못된 유져 타입 Free" );
	}
}
