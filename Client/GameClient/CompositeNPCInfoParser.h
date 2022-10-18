#pragma once

#include <SolarHashTable.h>
#include <Singleton.h>

#define MAX_COMPOSE_PARAM	5

struct BASE_CompositeNPCInfo {

     DWORD     m_dwNPCCode;
     DWORD     m_dwParams[MAX_COMPOSE_PARAM];
};


class CompositeNPCInfoParser	: public Singleton<CompositeNPCInfoParser> ,public IParser
{
public:
   CompositeNPCInfoParser();
   ~CompositeNPCInfoParser();


   VOID                     Init(DWORD dwPoolSize);
   VOID                     Init(DWORD dwPoolSize , char *szPackfileName);
   VOID                     Release();

   virtual	BOOL            LoadScript(eSCRIPT_CODE ScriptCode, BOOL bReload);

   BASE_CompositeNPCInfo*   GetCompositeNPCInfo(DWORD dwKey);

private:
    VOID		            Unload();
    util::SolarHashTable<BASE_CompositeNPCInfo*>*       m_pDataTable;
};
