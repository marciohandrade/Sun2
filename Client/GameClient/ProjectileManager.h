#pragma once
//------------------------------------------------------------------------------
/**
    발사체를 관리 한다.
    @author
        -준태,지완 
    @since
        -
    @remarks
        -2005.7.21 구조 변경 시작(지완)
*/

//#include "Projectile.h"
#include "ProjectileManagerEx.h"
//
//class Object;
//class Projectile;
//class Character;
//
//typedef	stdext::hash_map<DWORD, Projectile *>   Projectile_Map;
//typedef	Projectile_Map::iterator	    Projectile_Map_Itr;
//typedef	std::pair<DWORD, Projectile *>		Projectile_Map_Pair;
//
//class ProjectileManager
//{
//public:
//	ProjectileManager(void);
//	~ProjectileManager(void);
//
//	void            Clear();
//	void            Init();
//	void            Destroy();
//	void            Process(DWORD dwTick);
//	void            Render();
//	DWORD           CreateProjectile( const PROJECTILE_PARAM & param );
//	BOOL            Delete( DWORD dwObjectKey );
//
//    cProjectileBase* GetProjectile(DWORD dwKey);
//
//	//Projectile *    GetProjectile(DWORD dwKey);
//
//    Projectile_Map::iterator GetBegin();
//    Projectile_Map::iterator GetEnd();
//
//	HANDLE          GetTrailTexture();
//
//protected:
//	DWORD           m_dwLastKey;
//	Projectile_Map	m_hashmapProjectile;
//	HANDLE          m_hTrailTexture;        //경로 표시용 임시 택스쳐다.
//};
//
//inline Projectile_Map::iterator ProjectileManager::GetBegin()
//{
//    return m_hashmapProjectile.begin();
//}
//
//inline Projectile_Map::iterator ProjectileManager::GetEnd()
//{
//    return m_hashmapProjectile.end();
//}
//
//extern ProjectileManager g_ProjectileManager;
