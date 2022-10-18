#pragma once

#include <ScriptParserManager.h>


class ClientScriptParserManager : public ScriptParserManager
{
public:
	ClientScriptParserManager(void);
	~ClientScriptParserManager(void);

public:
	virtual	VOID	InitParserPool( VOID* pPoolOption );
	virtual BOOL	RegisterParser();
	virtual	VOID	DestroyInstanceEx();

    bool            ReloadScript(const char* old_filename, const char* new_filename, int pool_size);
#ifdef _DH_OS_LANGUAGETYPE
    bool UpdateMessageScript();
#endif//_DH_OS_LANGUAGETYPE
};
