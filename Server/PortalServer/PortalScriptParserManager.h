#pragma once

#include <ScriptParserManager.h>

class PortalScriptParserManager : public ScriptParserManager
{
public:
	PortalScriptParserManager(void);
	~PortalScriptParserManager(void);

public:		// GAMESERVER에 특화됨.. 각 서버에 특화된 파서 로드 함수를 여기서 선언.
	virtual	VOID	InitParserPool( VOID* pPoolOption );
	virtual BOOL	RegisterParser();
	virtual	VOID	DestroyInstanceEx();
};
