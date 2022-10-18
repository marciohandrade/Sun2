#pragma once

#include <ScriptParserManager.h>

class GuildScriptParserManager : public ScriptParserManager
{
public:
	GuildScriptParserManager(void);
	~GuildScriptParserManager(void);

public:		// GAMESERVER에 특화됨.. 각 서버에 특화된 파서 로드 함수를 여기서 선언.
	virtual	VOID	InitParserPool( VOID* pPoolOption );
	virtual BOOL	RegisterParser();
	virtual	VOID	DestroyInstanceEx();
};

