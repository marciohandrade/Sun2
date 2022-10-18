#pragma once

#include <Singleton.h>
#include <ScriptParserManager.h>

class GameScriptParserManager : public util::Singleton<GameScriptParserManager>, public ScriptParserManager
{
public:
	GameScriptParserManager(void);
	~GameScriptParserManager(void);

public:
	static const DWORD EXTRASTONEOPTIONINFO_PARSER_POOL = 100;
	static const DWORD ITEMCOMPOSELIST_PARSER_POOL = 100;
	static const DWORD ITEMCRYSTALIZELIST_PARSER_POOL = 100;
	static const DWORD EXPVALUEINFO_PARSER_POOL = 2000;
	static const DWORD CHUNTINGREWARD_PARSER_POOL = 300;
#ifdef C_NA_0_20100520_ACSYSTEM_CHANGED
	static const DWORD AREACONQUESTINFO_PARSER_POOL = 300;
#else
	static const DWORD AREACONQUESTINFO_PARSER_POOL = 10;
#endif
	static const DWORD ARMORDAMAGE_PARSER_POOL = 100;
	static const DWORD CHARINFO_PARSER_POOL = 5;
	static const DWORD MONSTERSUMMON_PARSER_POOL = 100;

public:
	virtual	VOID	InitParserPool( VOID* pPoolOption );
	virtual BOOL	RegisterParser();
	virtual	VOID	DestroyInstanceEx();
};
