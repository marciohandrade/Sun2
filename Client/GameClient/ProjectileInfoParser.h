#pragma once

#include <SolarHashTable.h>
#include <Singleton.h>

struct BASE_ProjectileInfo 
{

	DWORD     m_dwKey;
	DWORD     m_dwImageID;
	tagWZID   m_wzidFlyingEffectID;
	tagWZID   m_wzidHitEffectID;
	BYTE      m_bMakeTrail;
	DWORD     m_dwProjectileType;
	float	  m_fSpeed;
};


class ProjectileInfoParser	: public Singleton<ProjectileInfoParser> ,public IParser
{
public:
	ProjectileInfoParser();
	~ProjectileInfoParser();


	VOID                    Init(DWORD dwPoolSize , char *szPackfileName);
	VOID                    Init(DWORD dwPoolSize);

	VOID                    Release();
	virtual	BOOL            LoadScript(eSCRIPT_CODE ScriptCode, BOOL bReload);


	BASE_ProjectileInfo*    GetProjectileInfo(DWORD dwKey);

private:
	VOID		            Unload();
	util::SolarHashTable<BASE_ProjectileInfo*>*     m_pDataTable;
};
