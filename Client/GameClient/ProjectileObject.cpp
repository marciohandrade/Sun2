#include "SunClientPrecompiledHeader.h"
#include "ProjectileObject.h"

cProjectileObject::cProjectileObject()
{

}

BOOL cProjectileObject::ProcessProjectileObject( DWORD TickTime )
{
    if( cProjectileBase::Process( TickTime ) == TRUE )
        return TRUE;

    
    cProjectileEffect::Process();
    cProjectileHit::Process();
    cProjectileUnitObject::Process( TickTime );

    return FALSE;
}

void cProjectileObject::DestroyProjectileObject()
{
    cProjectileHit::Destroy();
    cProjectileEffect::Destroy();
    cProjectileUnitObject::Destroy();
}

void cProjectileObject::RenderProjectileObject()
{
    cProjectileUnitObject::Render();
}

void cProjectileObject::SetActiveDelayTime( DWORD ActiveDelayTime )
{
    cProjectileBase::SetActiveDelayTime( ActiveDelayTime );
    cProjectileUnitObject::SetActiveUnitDelay( ActiveDelayTime );
    cProjectileEffect::SetEffectActiveDelay( ActiveDelayTime );
}

