#pragma once


#include "ProjectileBase.h"
#include "ProjectileEffect.h"


class cProjectileObject
    : public cProjectileBase
    , public cProjectileHit
    , public cProjectileEffect
    , public cProjectileUnitObject
{
protected:
    virtual const WzVector& GetObjectPosition() const PURE;
    virtual const WzVector& GetObjectRotate() const PURE;

protected:
    cProjectileObject();

    BOOL ProcessProjectileObject( DWORD TickTime );
    void DestroyProjectileObject();
    void RenderProjectileObject();

public:
    virtual void SetActiveDelayTime( DWORD ActiveDelayTime );

private:
    // cProjectileHit
    DWORD GetCurrentDamageTickTime() { return cProjectileBase::GetCurrentTickTime(); } 
    const WzVector& GetDamageEffectPosition() const { return GetObjectPosition(); }

    // cProjectileEffect
    const WzVector& GetEffectPosition() const { return GetObjectPosition(); }
    const WzVector& GetEffectRotate() const { return GetObjectRotate(); }
    float GetEffectTotalLifeTimeRate() { return cProjectileBase::GetTotalLifeTimeRate(); }

    // cProjectileUnitObject
    const WzVector& GetUnitObjectPosition() const { return GetObjectPosition(); }
    const WzVector& GetUnitObjectRotate() const { return GetObjectRotate(); }
    float GetUnitObjectLifeTimeRate() { return cProjectileBase::GetTotalLifeTimeRate(); }

};

