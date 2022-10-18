
#include "stdafx.h"
#include ".\LogoutUser.h"



LogoutUser::LogoutUser()
{
}

LogoutUser::~LogoutUser()
{
}

VOID LogoutUser::Init()
{
	memset(&m_LogoutAuthData, 0x00, sizeof(LOGOUT_AUTH_DATA));
}

VOID LogoutUser::Release()
{

}
