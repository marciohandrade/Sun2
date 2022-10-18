#pragma once

#include <ScriptParserManager.h>

class PortalScriptParserManager : public ScriptParserManager
{
public:
	PortalScriptParserManager(void);
	~PortalScriptParserManager(void);

public:		// GAMESERVER�� Ưȭ��.. �� ������ Ưȭ�� �ļ� �ε� �Լ��� ���⼭ ����.
	virtual	VOID	InitParserPool( VOID* pPoolOption );
	virtual BOOL	RegisterParser();
	virtual	VOID	DestroyInstanceEx();
};
