#include "SunClientPrecompiledHeader.h"
#include "ProjectileManagerEx.h"
#include "resourcemanager.h"
#include "Application.h"


#include "Hero.h"
#include "GameFunc.h"
#include "ObjectManager.h"

#include "SkillInfoParser.h"
#include "ProjectileInfoParser.h"



cProjectileManager g_ProjectileManagerEx;

cProjectileManager::cProjectileManager()
{
	m_LastKey = 1;
	m_hTrailTexture = INVALID_HANDLE_VALUE;
}

void cProjectileManager::Clear()
{
    stdext::hash_map<DWORD, cProjectileBase*>::iterator it;
	for( it = m_ProjectileMap.begin() ; it != m_ProjectileMap.end() ; ++it )
	{
        cProjectileBase* pProjectile = it->second;
        pProjectile->Destroy();
        delete pProjectile;
	}
	m_ProjectileMap.clear();

	m_LastKey = 1;
}


BOOL cProjectileManager::DeleteProjectile( DWORD dwProjectileKey )
{
    stdext::hash_map<DWORD, cProjectileBase*>::iterator it;
    for( it = m_ProjectileMap.begin() ; it != m_ProjectileMap.end() ; ++it )
    {
        cProjectileBase* pProjectile = it->second;
        if( pProjectile->GetKey() == dwProjectileKey )
        {
            m_ProjectileMap.erase(it);

            pProjectile->Destroy();
            delete pProjectile;

            return TRUE;
        }
    }

	return FALSE;
}

cProjectileBase* cProjectileManager::GetProjectile(DWORD dwKey)
{	
	stdext::hash_map<DWORD, cProjectileBase*>::iterator it = m_ProjectileMap.find( dwKey );
	if (it == m_ProjectileMap.end())
	{		
		return NULL;
	}
	else
	{	
		return it->second;
	}
}

void cProjectileManager::Process(DWORD dwTick)
{
    stdext::hash_map<DWORD, cProjectileBase*>::iterator it = m_ProjectileMap.begin();
    stdext::hash_map<DWORD, cProjectileBase*>::iterator delete_it;
    while( it != m_ProjectileMap.end() )
    {
        cProjectileBase* pProjectile = it->second;

        BOOL iskill = pProjectile->Process(dwTick);

        if( iskill == TRUE )
        {
            delete_it = it;

            ++it;

            m_ProjectileMap.erase(delete_it);

            pProjectile->Destroy();
            delete pProjectile;
        }
        else
        {
            ++it;
        }
    }
}

void cProjectileManager::Render()
{
	ENUM_CULL_FACE      eCullFaceMode;
	eCullFaceMode = g_pSunRenderer->GetCullFaceMode();
	ENUM_ALPHABLEND eAlphablendMode = g_pSunRenderer->GetAlphaBlendMode();
	g_pSunRenderer->SetAlphaBlendMode( AB_ALPHA, FALSE );
	WzColor oldcolor = g_pSunRenderer->GetDiffuseColor();
	g_pSunRenderer->SetCullFaceMode( CF_DRAW_ALL );

    stdext::hash_map<DWORD, cProjectileBase*>::iterator it = m_ProjectileMap.begin();
    while( it != m_ProjectileMap.end() )
    {
        cProjectileBase* pProjectile = it->second;
        pProjectile->Render();
        ++it;
    }

	g_pSunRenderer->SetCullFaceMode( eCullFaceMode );
	g_pSunRenderer->SetColor(oldcolor);
	g_pSunRenderer->SetAlphaBlendMode( eAlphablendMode, FALSE );
}

void cProjectileManager::Init()
{
	m_hTrailTexture = g_pSunRenderer->x_pManagerTexture->LoadTexture( "Data\\Arrow\\Alpha_Gradient.TGA" ,TLS_ORIGINAL_SIZE | TLS_NO_MULTITHREAD);
}

void cProjectileManager::Destroy()
{	
	if (m_hTrailTexture != INVALID_HANDLE_VALUE)
	{
		g_pSunRenderer->x_pManagerTexture->DestroyTexture( m_hTrailTexture );
		m_hTrailTexture = INVALID_HANDLE_VALUE;
	}

	Clear();
}

HANDLE cProjectileManager::GetTrailTexture()
{
	return m_hTrailTexture;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void cProjectileManager::InsertProjectile(cProjectileBase* pProjectile)
{
    DWORD dwProjectileKey = m_LastKey++;
    pProjectile->SetKey( dwProjectileKey );

    m_ProjectileMap.insert( std::pair<DWORD, cProjectileBase*>( dwProjectileKey, pProjectile ) );
}

cProjectileBase* cProjectileManager::CreateEffect( DWORD TargetID, WzID EffectID, DWORD LifeTime, const cTargetPosition& Position )
{
    cProjectileChaseTarget* pProjectile = new cProjectileChaseTarget();
    pProjectile->SetLifeTime( LifeTime );
    pProjectile->SetPositionData( Position );
    pProjectile->CreateEffect( EffectID, TargetID, TargetID );

    InsertProjectile( pProjectile );

    return pProjectile;
}

cProjectileBase* cProjectileManager::CreatePortalForHero()
{
    WzVector vPos = g_pHero->GetVisiblePos();
    vPos += g_pHero->GetDirection() * 3.0f;

    vPos = GameFunc::FindStandPosition( g_pHero->GetVisiblePos(), vPos);
    vPos.z += 1.0f;

    cTargetPosition Position;
    Position.SetPosition( vPos );

    cProjectilePortal* pProjectile = new cProjectilePortal();
    pProjectile->SetLifeTime( 100000 );
    pProjectile->SetTargetPosition( vPos );
    pProjectile->CreateEffect( StrToWzID("O027"), g_pHero->GetObjectKey(), g_pHero->GetObjectKey() );

    InsertProjectile( pProjectile );

    return pProjectile;
}

cProjectileBase* cProjectileManager::CreateQuestPortal( const WzVector& PortalPosition, DWORD OwenerKey, DWORD LifeTime, PORTALKEY PortalKey, MAPCODE MapCode )
{
    WzVector p = PortalPosition;
    if( Player* pPlayer = (Player*)g_ObjectManager.GetObject( OwenerKey ) )
    {
        p = pPlayer->GetVisiblePos();
        p += pPlayer->GetDirection() * 1.5f;
        p = GameFunc::FindStandPosition( pPlayer->GetVisiblePos(), p );
    }

    cProjectileQuestPortal* pProjectile = new cProjectileQuestPortal( OwenerKey, PortalKey, MapCode );
    pProjectile->SetLifeTime( LifeTime );
    pProjectile->SetTargetPosition( p );
    pProjectile->CreateEffect( StrToWzID("A665"), OwenerKey, OwenerKey );

    InsertProjectile( pProjectile );

    return pProjectile;
}

cProjectileBase* cProjectileManager::CreateFieldEffect( const WzVector& Position, WzID EffectID, DWORD LifeTime )
{
    cProjectileChaseTarget* pProjectile = new cProjectileChaseTarget();
    pProjectile->SetTargetPosition( Position );
    pProjectile->SetLifeTime( LifeTime );
    pProjectile->CreateEffect( EffectID, 0, 0 );

    InsertProjectile( pProjectile );

    return pProjectile;

}

cProjectileBase* cProjectileManager::CreateFieldEffect( const WzVector& Position, WzID EffectID, DWORD LifeTime, WzID EndEffectID, DWORD DamageTime, DWORD ActiveDelay )
{
    cProjectileChaseTarget* pProjectile = new cProjectileChaseTarget();
    pProjectile->SetTargetPosition( Position );
    pProjectile->SetLifeTime( LifeTime );
    pProjectile->SetHitData( 0, 0, EndEffectID, FALSE, DamageTime, FALSE );
    pProjectile->CreateEffect( EffectID, 0, 0 );
    pProjectile->SetActiveDelayTime( ActiveDelay );

    InsertProjectile( pProjectile );

    return pProjectile;
}

cProjectileBase* cProjectileManager::CreateMoveEffect( const WzVector& StartPosition, const WzVector& TargetPosition, WzID EffectID, DWORD LifeTime, DWORD AttackObjectKey, DWORD AttackSerial )
{
    cProjectileUnit* pProjectile = new cProjectileUnit();
    pProjectile->SetLifeTime( LifeTime );
    pProjectile->SetArrowStartPosition( StartPosition );
    pProjectile->SetArrowTargetPosition( TargetPosition );

    pProjectile->CreateEffect( EffectID, 0, 0 );
    pProjectile->SetHitData( AttackObjectKey, AttackSerial, 0, FALSE, 0, TRUE );

    InsertProjectile( pProjectile );

    return pProjectile;

}

cProjectileUnit* cProjectileManager::CreateSkillFlyingObject( Character* pCharacter, Character* pTarget, SkillScriptInfo* pSkillInfo )
{
    if( pCharacter == NULL || pTarget == NULL || pSkillInfo == NULL )
        return NULL;


    //발사체코드 얻기
    BASE_ProjectileInfo* pProjectileInfo = ProjectileInfoParser::Instance()->GetProjectileInfo(pSkillInfo->m_dwFlyingObjectCode);
    if( pProjectileInfo == NULL )
        return NULL;


    float fSkillRange = ((float)pSkillInfo->m_wSkillRange / 10.0f);
    float fRate = VectorLength(&(pTarget->GetVisiblePos() - pCharacter->GetVisiblePos()))/fSkillRange;
    DWORD MoveTime = min(1200,max(200,(DWORD)(333.f*fRate/pProjectileInfo->m_fSpeed)));



    cProjectileUnit* pProjectile = new cProjectileUnit();
    pProjectile->SetArrowStartPosition( pCharacter->GetWeaponBonePos(0) );
    pProjectile->SetArrowTargetObjectKey( pTarget->GetObjectKey(), TRUE );

    pProjectile->SetLifeTime( MoveTime ); // 일단 대충
    pProjectile->SetHitData( pCharacter->GetObjectKey(), pCharacter->GetCurrentAttackSerial(), pProjectileInfo->m_wzidHitEffectID.id, FALSE, 0, FALSE );
    
    pProjectile->CreateEffect( pProjectileInfo->m_wzidFlyingEffectID.id, pCharacter->GetObjectKey(), pTarget->GetObjectKey() );
    pProjectile->SetUnitCode( (eProjectile_SRC)pProjectileInfo->m_dwImageID );

    if( pProjectileInfo->m_bMakeTrail == TRUE )
    {
        pProjectile->CreateTrail( m_hTrailTexture, 0.03f, 0.8f, 100, 150, 255 );
    }

    InsertProjectile( pProjectile );

    return pProjectile;


}
