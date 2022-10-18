#pragma once

#include <SolarHashTable.h>
#include <Singleton.h>

struct BASE_EnvironmentInfo {

	DWORD     m_dwKey;
	BYTE     m_bAmbientR;
	BYTE     m_bAmbientG;
	BYTE     m_bAmbientB;	
	BYTE     m_bSunLightR;
	BYTE     m_bSunLightG;
	BYTE     m_bSunLightB;
	float     m_fSunLightDirX;
	float     m_fSunLightDirY;
	float     m_fSunLightDirZ;

};


class EnvironmentInfoParser	: public Singleton<EnvironmentInfoParser> ,public IParser
{
public:
	EnvironmentInfoParser();
	~EnvironmentInfoParser();


	VOID                    Init(DWORD dwPoolSize,char *szPackfileName);
	VOID                    Init(DWORD dwPoolSize);

	VOID                    Release();
	virtual	BOOL            LoadScript( eSCRIPT_CODE ScriptCode, BOOL bReload );
	BASE_EnvironmentInfo*   GetEnvironmentInfo(DWORD dwKey);

private:
	VOID		            Unload();
	util::SolarHashTable<BASE_EnvironmentInfo*>*    m_pDataTable;
};
