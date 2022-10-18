#pragma once

namespace nProjectile
{
    enum eTraceType
    {
        TRACE_TYPE_NONE,                            // 대상을 추적하거나 하지 않는다
        TRACE_TYPE_TARGET_VISIBLE_POSITION,         // 대상의 Visible 포시션으로 항상추적
        TRACE_TYPE_TARGET_VISIBLE_POSITION_ONCE,    // 대상의 Visible 포지션을 목표로함 (첫번째 위치로 목표위치가 결정되고 이후 호밍하지 않음)
        TRACE_TYPE_TARGET_ARROW_POSITION,           // 대상의 Arrow 포지션을 항상추적
        TRACE_TYPE_TARGET_ARROW_POSITION_ONCE,      // 대상의 Arrow 포지션을 목표로함 (첫번째 위치로 목표위치가 결정되고 이후 호밍하지 않음)
        TRACE_TYPE_TARGET_TOP_POSITION,
        TRACE_TYPE_TARGET_TOP_POSITION_ONCE,
        TRACE_TYPE_BONE,
        TRACE_TYPE_ARROW,
    };
};

class cTargetPosition
{
    nProjectile::eTraceType m_Type;

    WzVector m_Position;
    WzVector m_Rotate;
    WzVector m_Scale;

    BOOL m_SyncTargetAngle;
    BOOL m_Trace;

    DWORD m_TargetObjectKey;
    eSKILLEFFECT_POS m_PositionType;
    int m_BoneIndex;

    WzVector m_OffsetPosition;

public:
    cTargetPosition();

    void SetPosition( const WzVector& Position );
    void SetTargetBonePosition( DWORD TargetObjectKey, eSKILLEFFECT_POS PositionType, int BoneIndex, BOOL Trace );
    void SetTargetVisiblePosition( DWORD TargetObjectKey, BOOL Trace );
    void SetTargetArrowPosition( DWORD TargetObjectKey, BOOL Trace );
    void SetTargetTopPosition( DWORD TargetObjectKey, BOOL Trace );
    void SetArrowPosition( const WzVector& Position, DWORD TargetObjectKey, BOOL Trace );


    WzVector GetOffsetPosition() const { return m_OffsetPosition; }
    void SetOffsetPosition(const WzVector& value) { m_OffsetPosition = value; }
    void SetOffsetPosition( float x, float y, float z ) { m_OffsetPosition.x = x; m_OffsetPosition.y = y; m_OffsetPosition.z = z; }

    void SetSyncTargetAngle( BOOL SyncTargetAngle ) { m_SyncTargetAngle = m_SyncTargetAngle; }
    void SetTrace(BOOL Trace) { m_Trace = Trace; }

    BOOL UpdatePosition();

    const WzVector& GetPosition() const { return m_Position; }
    const WzVector& GetRotate() const { return m_Rotate; }
    const WzVector& GetScale() const { return m_Scale; }
};

#include "Camera.h"

class cProjectileHit
{
    BOOL m_IsDamageProcessed;

    DWORD m_OwnerObjectKey;
    DWORD m_AttackSerial;
    BOOL m_IsAddNextAttackSerial;
    DWORD m_DamageTime;
    WzID m_HitEffectID;
    BOOL m_IsCameraShakeHit;

    Camera::VIBE_TYPE m_CameraViberationType;
    float m_CameraViberationTime;
    float m_CameraViberationPower;

protected:
    cProjectileHit();

    virtual DWORD GetCurrentDamageTickTime() PURE;
    virtual const WzVector& GetDamageEffectPosition() const PURE;

    void Process();
    void Destroy();

public:
    void SetHitData( DWORD AttackObjectKey, DWORD AttackSerial, WzID DamageEffectID, BOOL CameraShake, DWORD MaxWaitTime, BOOL IsAddNextAttackSerial );
    void SetAddNextAttackSerial( BOOL IsAddNextAttackSerial ) { m_IsAddNextAttackSerial = IsAddNextAttackSerial; }
    void SetAllDamage( BOOL IsProcessAllDamage ) {}
    void SetCameraShakeHit( BOOL UseCameraShakeHit ) { m_IsCameraShakeHit = UseCameraShakeHit; }
    void SetCameraShakeType( Camera::VIBE_TYPE ViberationType, float ViberationTime, float ViberationPower );

private:
    void _ProcessDamage();

};

class cProjectileTarget
{
    cTargetPosition m_PositionData;

protected:
    cProjectileTarget();

    void Process();
    void Destroy() {}

    const WzVector& GetPosition() const { return m_PositionData.GetPosition(); }
    const WzVector& GetRotate() const { return m_PositionData.GetRotate(); }
    const WzVector& GetScale() const { return m_PositionData.GetScale(); }

public:
//    const cTargetPosition& GetPositionData() const { return m_PositionData; }
//    cTargetPosition& GetPositionData() { return m_PositionData; }
    void SetPositionData( const cTargetPosition& PositionData ) { m_PositionData = PositionData; }
    void SetTargetPosition( const WzVector& TargetPosition ) { m_PositionData.SetPosition(TargetPosition); }
    void SetTargetObjectKey( DWORD TargetObjectKey, BOOL Trace ) { m_PositionData.SetTargetArrowPosition( TargetObjectKey, Trace ); }

};

class cProjectileBezier
{
    enum eBezierType
    {
        TYPE_DUAL_CONTROL,
        TYPE_SINGLE_CONTROL,
        TYPE_TRACE_OBJECT_SINGLE_CONTROL,
    };

    eBezierType m_Type;

    WzVector m_StartPosition;
    WzVector m_TargetPosition;
    WzVector m_CurvePosition1;
    WzVector m_CurvePosition2;

    WzVector m_CurrentPosition;
    WzVector m_CurrentRotate;

    DWORD m_TargetObjectKey;

public:
    void SetPosition( const WzVector& StartPosition, const WzVector& TargetPosition, float Height1, float Height2 );
    void SetBezierCourse( const WzVector& StartPosition, const WzVector& TargetPosition, const WzVector& ControlPoint1, const WzVector& ControlPoint2 );
    void SetBezierCourseSingleControl( const WzVector& StartPosition, const WzVector& TargetPosition, const WzVector& ControlPoint1 );
    void SetBezierCourseSingleControl( const WzVector& StartPosition, const WzVector& ControlPoint, DWORD TargetObjectKey );


protected:
    cProjectileBezier();

    virtual float GetArrowMoveTimeRate() PURE;


	const WzVector& GetBezierStartPosition() const { return m_StartPosition; }
	const WzVector& GetBezierTargetPosition() const { return m_TargetPosition; }

    const WzVector& GetCurrentPosition() const { return m_CurrentPosition; }
    const WzVector& GetCurrentRotate() const { return m_CurrentRotate; }

    void Process();
    void Destroy() {}
};


class cProjectileArrow
{
    WzVector m_ArrowPosition;
    WzVector m_ArrowRotate;
    WzVector m_ArrowScale;

    WzVector m_StartPosition;
    cTargetPosition m_TargetPosition;

public:
    void SetArrowStartPosition( const WzVector& StartPosition ) { m_StartPosition = StartPosition; }
    void SetArrowTargetPosition( const WzVector& TargetPosition ) { m_TargetPosition.SetPosition( TargetPosition ); }
    void SetArrowTargetObjectKey( DWORD TargetObjectKey, BOOL Trace ) { m_TargetPosition.SetTargetArrowPosition( TargetObjectKey, Trace ); }

protected:
    cProjectileArrow();

    virtual float GetArrowMoveTimeRate() PURE;

    void Process();
    void Destroy() {}

protected:
    const WzVector& GetArrowPosition() const { return m_ArrowPosition; }
    const WzVector& GetArrowRotate() const { return m_ArrowRotate; }
    const WzVector& GetArrowScale() const { return m_ArrowScale; }

    const WzVector& GetArrowStartPosition() const { return m_StartPosition; }
    const WzVector& GetArrowTargetPosition() const { return m_TargetPosition.GetPosition(); }

};


class cProjectileBase
{
    DWORD m_Key;

    DWORD m_Delay;
    DWORD m_LifeTime;
    DWORD m_RemainTime;

    BOOL m_IsActived;
    DWORD m_ActiveDelay;


protected:
    cProjectileBase();

    BOOL IsActived() { return m_IsActived; }

    BOOL IsTimeout();
    DWORD GetCurrentTickTime();
    //float GetLifeTimeRate();
    float GetMovingTimeRate();
    float GetTotalLifeTimeRate();

public:
    virtual void Destroy() {};
    virtual void Render() {};
    virtual BOOL Process( DWORD dwTick );

    void SetKey(DWORD value) { m_Key = value; }
    DWORD GetKey() { return m_Key; }

    virtual void SetActiveDelayTime( DWORD ActiveDelayTime );
    void SetLifeTime( DWORD LifeTime );
    void SetRemainTime( DWORD RemainTime );

};

