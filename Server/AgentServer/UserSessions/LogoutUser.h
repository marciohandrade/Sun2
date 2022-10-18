#ifndef __LOGOUT_USER_H__
#define __LOGOUT_USER_H__

#pragma once

#include <ServerStruct.h>


class LogoutUser
{
public:
	LogoutUser();
	~LogoutUser();

	VOID					Init();
	VOID					Release();

	LOGOUT_AUTH_DATA*		GetLOGOUT_AUTH_DATA() { return &m_LogoutAuthData; }
	VOID					SetLOGOUT_AUTH_DATA( LOGOUT_AUTH_DATA* pLogoutAuthDAta ) 
									{ memcpy(&m_LogoutAuthData, pLogoutAuthDAta, sizeof(LOGOUT_AUTH_DATA));}

private:
	LOGOUT_AUTH_DATA		m_LogoutAuthData;
};


#endif // __LOGOUT_USER_H__