#pragma once

#include <SolarHashTable.h>
#include <Singleton.h>
#include <SolarFileReader.h>


struct BASE_MatchInfo {

	DWORD		m_dwID;
	DWORD		m_dwMatchLimitTime;
	BYTE		m_byMatchLimitCount;

};

class MatchInfoParser : public Singleton<MatchInfoParser> ,public BaseParser
{
public:
	MatchInfoParser();
	~MatchInfoParser();


	VOID           Init(DWORD dwPoolSize , TCHAR *szPackfileName);
	VOID           Release();
	BOOL           Load(char *pszFilename);

	BASE_MatchInfo*            GetMatchInfo(DWORD dwKey);

private:
	VOID		   Unload();
	util::SolarHashTable<BASE_MatchInfo *>		*m_pDataTable;
};