#pragma once

#include <Singleton.h>
#include <ServerStruct.h>

class ServerOptionParserEx : public util::Singleton<ServerOptionParserEx>, public IParser
{
public:
	ServerOptionParserEx(void);
	~ServerOptionParserEx(void);

	SERVER_COMMON_OPTION&	GetServerOption(){ return m_ServerOption; }
	virtual	VOID			Release();

	virtual	BOOL			LoadScript( eSCRIPT_CODE ScriptCode, BOOL bReload );
			BOOL			_Load( BOOL bReload );
private:
	SERVER_COMMON_OPTION	m_ServerOption;


};
