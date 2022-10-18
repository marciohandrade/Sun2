#pragma once

#include "ProjectileBase.h"
#include "ProjectileUnit.h"
#include "ProjectileChaseTarget.h"

class SkillScriptInfo;

class cProjectileManager
{
public:
	cProjectileManager();

	void            Clear();
	void            Init();
	void            Destroy();
	void            Process( DWORD dwTick );
	void            Render();
	BOOL            DeleteProjectile( DWORD dwObjectKey );

	cProjectileBase* GetProjectile(DWORD dwKey);

	HANDLE          GetTrailTexture();

    cProjectileBase* CreateEffect( DWORD TargetID, WzID EffectID, DWORD LifeTime, const cTargetPosition& Position );
    cProjectileBase* CreatePortalForHero();
    cProjectileBase* CreateQuestPortal( const WzVector& PortalPosition, DWORD OwenerKey, DWORD LifeTime, PORTALKEY PortalKey, MAPCODE MapCode );

    cProjectileBase* CreateFieldEffect( const WzVector& Position, WzID EffectID, DWORD LifeTime, WzID EndEffectID, DWORD DamageTime, DWORD ActiveDelay );
    cProjectileBase* CreateFieldEffect( const WzVector& Position, WzID EffectID, DWORD LifeTime );


    cProjectileBase* CreateMoveEffect( const WzVector& StartPosition, const WzVector& TargetPosition, WzID EffectID, DWORD LifeTime, DWORD AttackObjectKey, DWORD AttackSerial );

    cProjectileUnit* CreateSkillFlyingObject( Character* pCharacter, Character* pTarget, SkillScriptInfo* pSkillInfo );
    

protected:
	DWORD m_LastKey;
    stdext::hash_map<DWORD, cProjectileBase*> m_ProjectileMap;

	HANDLE m_hTrailTexture;        //경로 표시용 임시 택스쳐다.

public:
    void InsertProjectile(cProjectileBase* pProjectile);
};

extern cProjectileManager g_ProjectileManagerEx;
