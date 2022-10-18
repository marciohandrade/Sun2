#ifndef __RESOURCE_INFO_LIST_H__
#define __RESOURCE_INFO_LIST_H__

#include <SolarHashTable.h>
#include <Singleton.h>
#include <SolarFileReader.h>

#define MAX_RESOURE_FILENAME          128

typedef struct _RESOURCE_INFO
{
	RESOURCECODE dwCode;
	char szFilename[MAX_RESOURE_FILENAME];
	int  dwTotalRefCount;

} RESOURCE_INFO;

class GraphicResourceList	: public Singleton<GraphicResourceList>, public IParser
{
public:
	GraphicResourceList();
	~GraphicResourceList();

	VOID				Init(DWORD dwItemInfoPoolSize);
	VOID				Init(DWORD dwItemInfoPoolSize, char * pszPackFileName);
	VOID				Release();

	virtual	BOOL        LoadScript(eSCRIPT_CODE ScriptCode, BOOL bReload);

	RESOURCE_INFO*      GetItemInfo(CODETYPE ItemCode) { return m_pResourceHashTable->GetData(ItemCode); }

    BOOL                AddItemInfo(CODETYPE ItemCode, char* pszResourceFileName);

    util::SolarHashTable<RESOURCE_INFO *>::iterator Begin() 
    {         
        return m_pResourceHashTable->begin(); 
    }

    util::SolarHashTable<RESOURCE_INFO *>::iterator End()   
    {         
        return m_pResourceHashTable->end(); 
    }

private:
	VOID									Unload();
	util::SolarHashTable<RESOURCE_INFO*>*	m_pResourceHashTable;
};

#endif 