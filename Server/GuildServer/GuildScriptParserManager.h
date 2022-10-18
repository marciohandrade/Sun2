#pragma once

#include <ScriptParserManager.h>

class GuildScriptParserManager : public ScriptParserManager
{
public:
	GuildScriptParserManager(void);
	~GuildScriptParserManager(void);

public:		// GAMESERVER�� Ưȭ��.. �� ������ Ưȭ�� �ļ� �ε� �Լ��� ���⼭ ����.
	virtual	VOID	InitParserPool( VOID* pPoolOption );
	virtual BOOL	RegisterParser();
	virtual	VOID	DestroyInstanceEx();
};

