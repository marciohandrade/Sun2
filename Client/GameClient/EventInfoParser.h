#pragma once

#include <SolarHashTable.h>
#include <Singleton.h>


typedef struct _EVENTINFO
{
	WORD     wText;
	WORD     wSound;
	WORD     wMovie;
	

	void Load(util::SolarFileReader &sr);

} EVENTINFO;

const int MAX_EVENT_INFO = 5;

struct BASE_EventInfo {

	DWORD     m_dwCode;

	EVENTINFO m_EventInfo[MAX_EVENT_INFO];
	

};


class EventInfoParser	: public Singleton<EventInfoParser> ,public IParser
{
public:
	EventInfoParser();
	~EventInfoParser();


	VOID                Init(DWORD dwPoolSize , char *szPackfileName);
	VOID                Init(DWORD dwPoolSize);

	VOID                Release(); 
	virtual	BOOL        LoadScript( eSCRIPT_CODE ScriptCode, BOOL bReload );
	BASE_EventInfo*     GetEventInfo(DWORD dwKey);

	void                InitPlayedEvent();

	BOOL                IsPlayed(DWORD dwEvent);
	void                AddPlayedList(DWORD dwEvent);
	

private:
	VOID		        Unload();
	util::SolarHashTable<BASE_EventInfo*>*      m_pDataTable;
	vector<DWORD>       m_vecPlayedList;
};
