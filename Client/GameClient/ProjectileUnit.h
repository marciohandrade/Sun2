#pragma once

#include "ProjectileObject.h"

DWORD CalcProjectileMoveTimeByRange( const WzVector& StartPosition, const WzVector& TargetPosition, float MaxRange, DWORD MoveTime );


class cProjectileUnit 
    : public cProjectileObject
    , public cProjectileArrow
    , public cProjectileTrail
{
private:
    // cProjectileObject
    const WzVector& GetObjectPosition() const { return cProjectileArrow::GetArrowPosition(); }
    const WzVector& GetObjectRotate() const { return cProjectileArrow::GetArrowRotate(); }

    // cProjectileArrow
    float GetArrowMoveTimeRate() { return cProjectileBase::GetMovingTimeRate(); }; 

    // cProjectileTrail
    const WzVector& GetArrowStartPosition() const { return cProjectileArrow::GetArrowStartPosition(); }
    const WzVector& GetArrowTargetPosition() const { return cProjectileArrow::GetArrowTargetPosition(); };
    float GetArrowPositionRate() { return cProjectileBase::GetMovingTimeRate(); }

public:
    cProjectileUnit();

protected:
    virtual void Destroy();
    virtual BOOL Process( DWORD TickTime );
    virtual void Render();
};


class cProjectileBezierUnit
    : public cProjectileObject
    , public cProjectileBezier
	, public cProjectileTrail
{
protected:
    // cProjectileObject
    const WzVector& GetObjectPosition() const { return cProjectileBezier::GetCurrentPosition(); }
    const WzVector& GetObjectRotate() const { return cProjectileBezier::GetCurrentRotate(); }

    // cProjectileBezier
    float GetArrowMoveTimeRate() { return cProjectileBase::GetMovingTimeRate(); }; 

	// cProjectileTrail
	const WzVector& GetArrowStartPosition() const { return cProjectileBezier::GetBezierStartPosition(); }
	const WzVector& GetArrowTargetPosition() const { return cProjectileBezier::GetBezierTargetPosition(); };
	float GetArrowPositionRate() { return cProjectileBase::GetMovingTimeRate(); }

public:
    cProjectileBezierUnit();

protected:
    virtual void Destroy();
    virtual BOOL Process( DWORD TickTime );
    virtual void Render();
};

#include "CharacterDef.h"

class cProjectileAirBlow
    : public cProjectileUnit
{
    DWORD m_BlowTargetObjectKey;
	float m_fHeightLimit;
	float m_fUpperForce;

public:
    cProjectileAirBlow();

    DWORD GetBlowTargetObjectKey() const { return m_BlowTargetObjectKey; }
    void SetBlowTargetObjectKey(DWORD value) { m_BlowTargetObjectKey = value; }

	void SetBlowInfo( float fUpperForce, float fHeightLimit ) { m_fUpperForce = fUpperForce; m_fHeightLimit = fHeightLimit; }

    BOOL Process( DWORD TickTime );
};


class cProjectileTroch
    : public cProjectileBezierUnit
{
public:
    cProjectileTroch() {}

protected:
    virtual BOOL Process( DWORD TickTime );
};


class cProjectileSwordTrailUnit
    : public cProjectileUnit
    , public cProjectileSwordTrail
{
public:
    cProjectileSwordTrailUnit();

    HANDLE GetEffectHandle() { return cProjectileEffect::GetEffectHandle(); }
    const WzVector& GetSwordTrailPosition() const { return cProjectileArrow::GetArrowPosition(); }

protected:

    void Destroy();
    BOOL Process( DWORD TickTime );
    void Render();

};
