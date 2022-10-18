#ifndef __USER_FACTORY_H__
#define __USER_FACTORY_H__

#pragma once

#include <Singleton.h>
#include <MemoryPoolFactory.h>
#include "User.h"

class User;
class TempUser;
class ActiveUser;

class UserFactory : public util::Singleton<UserFactory>
{
public:
	UserFactory();
	virtual ~UserFactory();

	VOID									Release();
	VOID									Init( DWORD dwUserSize );

	User*									Alloc( eUSER_TYPE type );
	VOID									Free( User* pUser );

protected:
	util::CMemoryPoolFactory<TempUser>		*m_pTempUserPool;
	util::CMemoryPoolFactory<ActiveUser>	*m_pActiveUserPool;
};

#endif // __USER_FACTORY_H__