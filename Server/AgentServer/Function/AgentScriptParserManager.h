#pragma once

#include <ScriptParserManager.h>

class AgentScriptParserManager : public ScriptParserManager
{
public:
	AgentScriptParserManager(void);
	~AgentScriptParserManager(void);

public:
	static const DWORD	MAPINFOPARSER_POOL = 100;
	static const DWORD	REQUIREFIELDLEVELINFOPARSER_POOL = 100;
	static const DWORD	MATCHINFOPARSER_POOL = 5;
#ifdef _NA_20100307_BETAKEY_SYSTEM
    static const DWORD	CHARINFO_PARSER_POOL = 5;
#endif

public:
	virtual	VOID	InitParserPool( VOID* pPoolOption );
	virtual BOOL	RegisterParser();
	virtual	VOID	DestroyInstanceEx();
};
