#pragma once


class cProjectileEffect
{
    BOOL m_Actived;
    DWORD m_ActiveDelayTime;
    DWORD m_StartedTime;

    DWORD m_CasterObjectKey;
    DWORD m_TargetObjectKey;

    WzID m_EffectID;
    HANDLE m_hEffect;

    BOOL m_StaticRotate;
    WzVector m_EffectRotate;
    WzVector m_EffectScale;

    BOOL m_FadeOut;

protected:
    virtual const WzVector& GetEffectPosition() const PURE;
    virtual const WzVector& GetEffectRotate() const PURE;
    virtual float GetEffectTotalLifeTimeRate() PURE;

    const WzVector& GetEffectScale() { return m_EffectScale; }

    void Process();
    void Destroy();

public:
    cProjectileEffect();

    HANDLE GetEffectHandle() { return m_hEffect; }

    BOOL CreateEffect( WzID EffectID, DWORD CasterObjectKey, DWORD TargetObjectKey );

    void SetEffectRotate( const WzVector& Rotate ) { m_EffectRotate = Rotate; m_StaticRotate = TRUE; }
    void SetEffectScale( float fScale ) { m_EffectScale.x = m_EffectScale.y = m_EffectScale.z = fScale; }
    void SetEffectScale( const WzVector& Scale ) { m_EffectScale = Scale; }

    void SetEffectActiveDelay( DWORD ActiveDelayTime ) { m_ActiveDelayTime = ActiveDelayTime; }
    void SetEffectFadeOut(BOOL Enable) { m_FadeOut = Enable; }
};



#include "ObjectEffectData.h"

class cProjectileUnitObject
{
    BOOL m_Actived;
    DWORD m_ActiveDelayTime;
    DWORD m_StartedTime;

    eProjectile_SRC m_ProjectileImageCode;

    CWzUnitDraw* m_pUnitDraw;
    EFFECT_PARAM m_UnitEffectParam[MAX_OBJECT_EFFECT];
    WzColor m_UnitColor;

    WzID m_AnimationID;
    BOOL m_AnimationLoop;

    DWORD m_NextAnimationTime;
    WzID m_NextAnimationID;
    BOOL m_NextAnimationLoop;

    BOOL m_FadeOut;
    ENUM_ALPHABLEND m_AlphaMode;

    BOOL m_StaticUnitRotate;
    WzVector m_UnitRotate;

    DWORD m_CurrentTickTime;

public:
    void SetUnitColor( WzColor Color ) { m_UnitColor = Color; }

    BOOL IsUnitFadeOut() const { return m_FadeOut; }
    void SetUnitFadeOut(BOOL Enable) { m_FadeOut = Enable; }

    void SetUnitAnimation( WzID AnimationID, BOOL Loop );
    void SetNextUnitAnimation(DWORD ChangeTime, WzID AnimationID, BOOL Loop );

    //BOOL CreateUnit( eProjectile_SRC ProjectileImageCode );
    void SetUnitCode( eProjectile_SRC ProjectileImageCode );
    void SetActiveUnitDelay( DWORD ActiveDelay ) { m_ActiveDelayTime = ActiveDelay; }

    void SetAlphaMode(ENUM_ALPHABLEND AlphaMode) { m_AlphaMode = AlphaMode; }

    void SetUnitRotate(const WzVector& Rotate) { m_UnitRotate = Rotate; m_StaticUnitRotate = TRUE; }



protected:
    cProjectileUnitObject();

    void Render();
    void Process(DWORD TickTime);
    void Destroy();

    virtual const WzVector& GetUnitObjectPosition() const PURE;
    virtual const WzVector& GetUnitObjectRotate() const PURE;
    virtual float GetUnitObjectLifeTimeRate() PURE;
    //const WzVector& GetUnitObjectPosition() PURE;

private:
    BOOL UpdateUnitAnimation( DWORD TickTime );
    void UpdateUnitEffectPosition( CWzUnitDraw* pUnitDraw, const WzVector& Position, const WzVector& Direction );

    void DestroyAllUnitEffect();
    void DestroyUnitEffect(int index);

};


class Clone;

class cProjectileCloneObject
{
    BOOL m_Actived;
    DWORD m_ActiveDelayTime;
    DWORD m_StartedTime;

    DWORD m_OriginalObjectKey;
    WzID m_AnimationID;
    DWORD m_CloneObjectKey;
    Clone* m_pClone;

    BOOL m_CloneFadeOut;

    ENUM_ALPHABLEND m_CloneBlendType;
    WzColor m_CloneColor;

public:
    cProjectileCloneObject();

    void Render();
    void Destroy();
    void Process( DWORD TickTime );

    void CreateClone( DWORD ObjectKey, WzID AnimationID );

    void SetCloneBlendType(const ENUM_ALPHABLEND& value) { m_CloneBlendType = value; }
    void SetCloneColor(WzColor value) { m_CloneColor = value; }

    BOOL IsCloneFadeOut() const { return m_CloneFadeOut; }
    void SetCloneFadeOut(BOOL value) { m_CloneFadeOut = value; }

    void SetActiveCloneDelayTime( DWORD ActiveDelayTime ) { m_ActiveDelayTime = ActiveDelayTime; }

    virtual float GetCloneLifeTimeRate() PURE;
    virtual const WzVector& GetClonePosition() const PURE;
    virtual const WzVector& GetCloneDirection() const PURE;

};

class cProjectileTrail
{
    WzSimpleMesh* m_pTrailMesh;
    float m_TrailLength;
    float m_TrailHeight;
    BYTE m_TrailColorRed;
    BYTE m_TrailColorGreen;
    BYTE m_TrailColorBlue;

    BOOL m_ReadyTrail;

public:
    BOOL CreateTrail( HANDLE hTailTexture, float TrailHeight, float TrailLength, BYTE ColorRed, BYTE ColorGreen, BYTE ColorBlue );

    void SetTrailHeight(float value) { m_TrailHeight = value; }

protected:
    cProjectileTrail();

    void Destroy();
    void Render();
    void Process();

    virtual const WzVector& GetArrowStartPosition() const PURE;
    virtual const WzVector& GetArrowTargetPosition() const PURE;
    virtual float GetArrowPositionRate() PURE;
};


class cProjectileSwordTrail
{
private:
    BOOL m_InitializedSwordTrail;
    DWORD m_LastTickTIme;

    DWORD m_TrailEffectDelay;
    WzVector m_TrailStartOffset;
    WzVector m_TrailEndOffset;
    WzVector m_TrailDirection;

public:
    void SetSwordTrail( const WzVector& Direction, const WzVector& StartOffset, const WzVector& EndOffset )
    {
        m_TrailStartOffset = StartOffset;
        m_TrailEndOffset = EndOffset;
        m_TrailDirection = Direction;
    };

    void SetSwordTrailLandEffectDelay( DWORD Delay )
    {
        m_TrailEffectDelay = Delay;
    }

protected:
    cProjectileSwordTrail();


    virtual HANDLE GetEffectHandle() PURE;
    virtual const WzVector& GetSwordTrailPosition() const PURE;

    void Destroy();
    void Render();
    void Process( DWORD TickTime );
};



