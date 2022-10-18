#pragma once

#include <SolarHashTable.h>
#include <Singleton.h>
#include <ServerStruct.h>

enum eSERVER_OPTION_TYPE
{
	eSERVER_OPTION_TYPE_PUBLIC_FIELD,		// 공식적인 필드옵션
	eSERVER_OPTION_TYPE_PUBLIC_BATTLE,
	eSERVER_OPTION_TYPE_PRIVATE_FIELD,		// 개인적인 필드옵션
	eSERVER_OPTION_TYPE_PRIVATE_BATTLE,
};

class ServerOptionParser : public util::Singleton<ServerOptionParser>
{
public:
	ServerOptionParser();
	~ServerOptionParser();

	BOOL							LoadPoolInfo( char * pszFileName, BYTE byServerType, BOOL bReload = FALSE );
	VOID							Reload();
	VOID							Release();

	inline SERVER_POOL_OPTION&		GetPoolInfo()	{ return m_ServerPoolOption; }

private:
	char					m_pszCommonFileName[MAX_PATH];
	char					m_pszPoolFileName[MAX_PATH];

	SERVER_POOL_OPTION		m_ServerPoolOption;
	eSERVER_OPTION_TYPE		m_ServerType;
};


