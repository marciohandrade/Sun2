//------------------------------------------------------------------------------				    
//  작업자: 이지연
//	2006/06/14
//------------------------------------------------------------------------------

#pragma once

#include <SolarHashTable.h>
#include <Singleton.h>

#pragma pack(push,1)

struct INDUN_OPTION 
{
	 DWORD	  mdwIndex;
	 char	  msDesc[128];
     MAPCODE  mwMapCode;
     DWORD    mdwLayer;
};

#pragma pack(pop)

class InDunOptionParser	: public Singleton<InDunOptionParser> ,public IParser
{
public:
   InDunOptionParser();
   ~InDunOptionParser();

   VOID								Init(DWORD dwPoolSize , char *szPackfileName);
   VOID								Init(DWORD dwPoolSize);

   VOID								Release();

private:
	VOID							Unload();
	util::SolarHashTable<INDUN_OPTION *>		*m_pDataTable;
	BOOL							_Load( BOOL bReload );
public:
	virtual	BOOL					LoadScript( eSCRIPT_CODE ScriptCode, BOOL bReload );
	INDUN_OPTION*					GetInDunOption(DWORD dwIndex);
	
};
