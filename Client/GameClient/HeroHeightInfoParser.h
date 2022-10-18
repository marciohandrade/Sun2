#pragma once

#include <SolarHashTable.h>
#include <Singleton.h>


const int MAX_HEIGHT_VARIATION = 5;

struct BASE_HeroHeightInfo {

	BYTE     m_bClass;
	float     m_fBase;
	float     m_fHeight[MAX_HEIGHT_VARIATION];
	

};


class HeroHeightInfoParser	: public Singleton<HeroHeightInfoParser> ,public IParser
{
public:
	HeroHeightInfoParser();
	~HeroHeightInfoParser();


	VOID            Init(DWORD dwPoolSize , char *szPackfileName);
	VOID            Init(DWORD dwPoolSize);

	VOID            Release();
    virtual	BOOL    LoadScript(eSCRIPT_CODE ScriptCode, BOOL bReload);

	BASE_HeroHeightInfo*       GetHeroHeightInfo(DWORD dwKey);

private:
	VOID		    Unload();
	util::SolarHashTable<BASE_HeroHeightInfo*>*     m_pDataTable;
};
