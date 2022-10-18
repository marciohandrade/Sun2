
#include "stdafx.h"
#include "LogoutUserFactory.h"
#include <ServerStruct.h>
#include "LogoutUser.h"

LogoutUserFactory::LogoutUserFactory()
{
	m_pLogoutUserPool = new util::CMemoryPoolFactory<LogoutUser>;
}

LogoutUserFactory::~LogoutUserFactory()
{
	delete m_pLogoutUserPool;
}

VOID LogoutUserFactory::Release()
{
	m_pLogoutUserPool->Release();
}

VOID LogoutUserFactory::Init( DWORD dwUserSIze )
{
	m_pLogoutUserPool->Initialize( dwUserSIze, 0, "LogoutUser", AssertionLogger4Pool_Server );
}

LogoutUser* LogoutUserFactory::Alloc()
{
	LogoutUser* pLogoutUser = (LogoutUser*)m_pLogoutUserPool->Alloc();

	pLogoutUser->Init();

	return pLogoutUser;
}

VOID LogoutUserFactory::Free( LogoutUser* pLogoutUser )
{
	pLogoutUser->Release();

	m_pLogoutUserPool->Free( pLogoutUser );
}
