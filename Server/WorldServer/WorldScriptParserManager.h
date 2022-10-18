#pragma once

#include <ScriptParserManager.h>

class WorldScriptParserManager : public ScriptParserManager
{
public:
	WorldScriptParserManager(void);
	~WorldScriptParserManager(void);

public:
	static const DWORD MAPINFO_PARSER_POOL = 100;

public:		
	virtual	VOID	InitParserPool( VOID* pPoolOption );
	virtual BOOL	RegisterParser();
	virtual	VOID	DestroyInstanceEx();
};
