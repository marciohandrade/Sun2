#ifndef __LOGOUT_USER_FACTORY_H__
#define __LOGOUT_USER_FACTORY_H__

#pragma once

#include <Singleton.h>
#include <MemoryPoolFactory.h>
#include <ServerStruct.h>

class LogoutUser;

class LogoutUserFactory : public util::Singleton<LogoutUserFactory>
{
public:
	LogoutUserFactory();
	virtual ~LogoutUserFactory();

	VOID									Release();
	VOID									Init( DWORD dwUserSIze );

	LogoutUser*								Alloc();
	VOID									Free( LogoutUser* pLogoutUser );

protected:
	util::CMemoryPoolFactory<LogoutUser>*			m_pLogoutUserPool;
};

#endif // __LOGOUT_USER_FACTORY_H__