#pragma once

#include <SolarHashTable.h>
#include <Singleton.h>

#define MAX_QUEST_TEXT_LENGTH

struct QuestTextInfo 
{
     DWORD		m_dwCode;
     TCHAR *	m_pszText;

};


class QuestTextInfoParser	: public Singleton<QuestTextInfoParser> ,public IParser
{
public:
	QuestTextInfoParser();
	~QuestTextInfoParser();

	VOID                Init(DWORD dwPoolSize );

	VOID                Init(DWORD dwPoolSize , char *szPackfileName);
	VOID                Release();

	virtual	BOOL        LoadScript(eSCRIPT_CODE ScriptCode, BOOL bReload);

	QuestTextInfo*      GetQuestTextInfo(DWORD dwKey);

private:
	VOID		        Unload();
	util::SolarHashTable<QuestTextInfo*>*   m_pDataTable;
};
