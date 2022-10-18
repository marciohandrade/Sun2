#pragma once

#include <SolarHashTable.h>
#include <Singleton.h>
#include <SolarFileReader.h>
#include <string>

struct AmbientSoundInfo 
{
    enum
    {
        MAX_DESC_LEN = 256
    };
    WORD     m_wNum;
    char     m_szDesc[MAX_DESC_LEN];
    WORD     m_wWorldCode;
    WORD     m_wAmbientCode;
};

class AmbientSoundInfoParser	: public Singleton<AmbientSoundInfoParser>, public IParser
{
public:
   AmbientSoundInfoParser();
   ~AmbientSoundInfoParser();

   VOID           Init(DWORD dwPoolSize);
   VOID			  Init(DWORD dwPoolSize, char * pszPackFileName);
   VOID           Release();  
   
private:
	VOID			Unload();
	util::SolarHashTable<AmbientSoundInfo *>		*m_pDataTable;
	BOOL			_Load( BOOL bReload );

public:
	virtual	BOOL	    LoadScript( eSCRIPT_CODE ScriptCode, BOOL bReload );
    DWORD               GetAmbientSoundCode(DWORD dwWorldCode);
    AmbientSoundInfo*   GetAmbientSoundInfo(DWORD dwWorldCode);
};
