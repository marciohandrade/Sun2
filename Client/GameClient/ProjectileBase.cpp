#include "SunClientPrecompiledHeader.h"
#include "ProjectileBase.h"

#include "ObjectManager.h"
#include "Character.h"

#include "Camera.h"
#include "GlobalFunc.h"




cTargetPosition::cTargetPosition()
: m_Rotate(c_ZeroVector)
, m_Scale(c_UnitVector)
, m_Position(c_ZeroVector)
, m_OffsetPosition(c_ZeroVector)
, m_Type(nProjectile::TRACE_TYPE_NONE)
, m_Trace(FALSE)
, m_PositionType(eSKILLEFFECT_POS_NONE)
, m_BoneIndex(-1)
, m_TargetObjectKey(INVALID_OBJECT_KEY)
, m_SyncTargetAngle(FALSE)
{
}

void cTargetPosition::SetPosition( const WzVector& Position )
{
    m_Type = nProjectile::TRACE_TYPE_NONE;
    m_Trace = FALSE;

    m_Position = Position;
}

void cTargetPosition::SetTargetBonePosition( DWORD TargetObjectKey, eSKILLEFFECT_POS PositionType, int BoneIndex, BOOL Trace )
{
    m_Type = nProjectile::TRACE_TYPE_BONE;
    m_Trace = Trace;

    m_TargetObjectKey = TargetObjectKey;
    m_PositionType = PositionType;
    m_BoneIndex = BoneIndex;
}

void cTargetPosition::SetTargetVisiblePosition( DWORD TargetObjectKey, BOOL Trace )
{
    m_Type = nProjectile::TRACE_TYPE_TARGET_VISIBLE_POSITION;
    m_TargetObjectKey = TargetObjectKey;
    m_Trace = Trace;
}

void cTargetPosition::SetTargetArrowPosition( DWORD TargetObjectKey, BOOL Trace )
{
    m_Type = nProjectile::TRACE_TYPE_TARGET_ARROW_POSITION;
    m_TargetObjectKey = TargetObjectKey;
    m_Trace = Trace;
}

void cTargetPosition::SetTargetTopPosition( DWORD TargetObjectKey, BOOL Trace )
{
    m_Type = nProjectile::TRACE_TYPE_TARGET_TOP_POSITION;
    m_TargetObjectKey = TargetObjectKey;
    m_Trace = Trace;
}

void cTargetPosition::SetArrowPosition( const WzVector& Position, DWORD TargetObjectKey, BOOL Trace )
{
    m_Type = nProjectile::TRACE_TYPE_ARROW;
    m_TargetObjectKey = TargetObjectKey;
    m_Trace = Trace;
}



BOOL cTargetPosition::UpdatePosition()
{
    if( m_Type == nProjectile::TRACE_TYPE_NONE )
        return TRUE;

    if( m_TargetObjectKey == 0 || m_TargetObjectKey == INVALID_OBJECT_KEY )
    {
        return TRUE;
    }

    Character* pTarget = (Character*)g_ObjectManager.GetObject( m_TargetObjectKey );
    if( pTarget == NULL )
        return FALSE;

    if( m_Type == nProjectile::TRACE_TYPE_TARGET_ARROW_POSITION )
    {
        m_Position = pTarget->GetArrowTargetPos();
        m_Position += m_OffsetPosition;
    }
    else if( m_Type == nProjectile::TRACE_TYPE_TARGET_VISIBLE_POSITION )
    {
        m_Position = pTarget->GetVisiblePos();
        m_Position += m_OffsetPosition;
    }
    else if( m_Type == nProjectile::TRACE_TYPE_TARGET_TOP_POSITION )
    {
        m_Position = pTarget->GetVisiblePos();
        m_Position.z += pTarget->GetHeight();
        m_Position += m_OffsetPosition;
    }
    else if( m_Type == nProjectile::TRACE_TYPE_BONE )
    {
        if( m_PositionType == eSKILLEFFECT_POS_BONE_INDEX )
        {
            m_Position = pTarget->GetBonePos( m_BoneIndex );
            m_Position += m_OffsetPosition;
        }
        else if( m_PositionType == eSKILLEFFECT_POS_RHAND ||
            m_PositionType == eSKILLEFFECT_POS_LHAND ||
            m_PositionType == eSKILLEFFECT_POS_WEAPON )
        {
            m_Position = pTarget->GetSkillEffectPos( m_PositionType, m_BoneIndex );
            m_Position += m_OffsetPosition;

            WzVector vDest,vTemp;
            vDest = m_Position + pTarget->GetDirection()*10.0f;
            vDest = vDest - m_Position;
            vTemp = vDest;
            vTemp.z = 0.0f;

            float fLength;
            if (vTemp.x == 0.0f && vTemp.y == 0.0f)
            {
                fLength = 0.0f;
            }
            else
            {
                fLength = VectorLength(&vTemp);
            }

            float fArrowAngleX = -atan2(vDest.z,fLength);
            float fArrowAngleZ = Math_GetAngleFromVector(&vDest);

            m_Rotate.x = fArrowAngleX;
            m_Rotate.y = 0.0f;
            m_Rotate.z = fArrowAngleZ;

            m_Scale = c_UnitVector;
        }
        else
        {
            // �ġ
            m_Position = pTarget->GetSkillEffectPos( m_PositionType, m_BoneIndex );
            m_Position += m_OffsetPosition;

            m_Rotate.y = 0.0f;
            m_Rotate.z = pTarget->GetAngle();;
            m_Rotate.x = pTarget->GetAngleX();
        }
    }

    if( m_SyncTargetAngle == TRUE )
    {
        m_Rotate.y = 0.0f;
        m_Rotate.z = pTarget->GetAngle();
        m_Rotate.x = pTarget->GetAngleX();
    }

    if( m_Trace == FALSE )
    {
        m_TargetObjectKey = INVALID_OBJECT_KEY;
    }

    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
cProjectileHit::cProjectileHit()
{
    m_IsDamageProcessed = FALSE;

    m_OwnerObjectKey = 0;
    m_AttackSerial = 0;
    m_HitEffectID = 0;
    m_IsCameraShakeHit = FALSE;
    m_DamageTime = 0;
    m_IsAddNextAttackSerial = FALSE;

    m_CameraViberationType = Camera::VIBE_TYPE_VERTICAL;
    m_CameraViberationTime = 50.f;
    m_CameraViberationPower = 0.07f;
}

void cProjectileHit::SetHitData( DWORD AttackObjectKey, DWORD AttackSerial, WzID EffectID, BOOL CameraShake, DWORD MaxWaitTime, BOOL IsAddNextAttackSerial )
{
    m_OwnerObjectKey = AttackObjectKey;
    m_AttackSerial = AttackSerial;
    m_IsCameraShakeHit = CameraShake;
    m_DamageTime = MaxWaitTime;
    m_HitEffectID = EffectID;
    m_IsAddNextAttackSerial = IsAddNextAttackSerial;
}

void cProjectileHit::Process()
{
    if( m_DamageTime != 0 )
    {
        if( GetCurrentDamageTickTime() >= m_DamageTime )
        {
            _ProcessDamage();
        }
    }
}

void cProjectileHit::Destroy() 
{
    _ProcessDamage(); 
}

void cProjectileHit::SetCameraShakeType( Camera::VIBE_TYPE ViberationType, float ViberationTime, float ViberationPower )
{
    m_IsCameraShakeHit = TRUE;
    m_CameraViberationType = ViberationType;
    m_CameraViberationTime = ViberationTime;
    m_CameraViberationPower = ViberationPower;
}

void cProjectileHit::_ProcessDamage()
{
    if( m_IsDamageProcessed == TRUE )
        return;

    m_IsDamageProcessed = TRUE;

    if( m_OwnerObjectKey != 0 )
    {
        if( Object* pObject = (Object*)g_ObjectManager.GetObject( m_OwnerObjectKey ) )
        {
            if( pObject->IsKindOfObject( CHARACTER_OBJECT ) )
            {
                Character* pCharacter = (Character*)pObject;

                pCharacter->ProcessAttackResult_Group( m_AttackSerial );

                if( m_IsAddNextAttackSerial )
                {
                    pCharacter->AddNextAttackSerial();
                }
            }
        }
    }

    if( m_IsCameraShakeHit == TRUE )
    {
        g_Camera.SetVibeType( m_CameraViberationType );
        g_Camera.StartVibration( m_CameraViberationTime, m_CameraViberationPower );

        m_IsCameraShakeHit = FALSE;
    }

    if( m_HitEffectID != 0 )
    {
        GlobalFunc::CreateEffectBeUpToOption( m_HitEffectID, GetDamageEffectPosition(), 0, 0 );
        m_HitEffectID = 0;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
cProjectileTarget::cProjectileTarget()
{

}

void cProjectileTarget::Process()
{
    m_PositionData.UpdatePosition();
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
cProjectileBezier::cProjectileBezier()
: m_Type(TYPE_DUAL_CONTROL)
, m_TargetObjectKey(INVALID_OBJECT_KEY)
{

}

void cProjectileBezier::SetBezierCourseSingleControl( const WzVector& StartPosition, const WzVector& ControlPoint, DWORD TargetObjectKey )
{
    m_Type = TYPE_TRACE_OBJECT_SINGLE_CONTROL;

    m_StartPosition = StartPosition;

    m_CurvePosition1 = ControlPoint;
    m_TargetObjectKey = TargetObjectKey;
}


void cProjectileBezier::SetBezierCourseSingleControl( const WzVector& StartPosition, const WzVector& TargetPosition, const WzVector& ControlPoint1 )
{
    m_Type = TYPE_SINGLE_CONTROL;

    m_StartPosition = StartPosition;
    m_TargetPosition = TargetPosition;

    m_CurvePosition1 = ControlPoint1;
}

void cProjectileBezier::SetBezierCourse( const WzVector& StartPosition, const WzVector& TargetPosition, const WzVector& ControlPoint1, const WzVector& ControlPoint2 )
{
    m_Type = TYPE_DUAL_CONTROL;

    m_StartPosition = StartPosition;
    m_TargetPosition = TargetPosition;

    m_CurvePosition1 = ControlPoint1;
    m_CurvePosition2 = ControlPoint2;
}

void cProjectileBezier::SetPosition( const WzVector& StartPosition, const WzVector& TargetPosition, float Height1, float Height2 )
{
    m_Type = TYPE_DUAL_CONTROL;

    m_StartPosition = StartPosition;
    m_TargetPosition = TargetPosition;

    m_CurvePosition1 = StartPosition;
    m_CurvePosition2 = TargetPosition;
    m_CurvePosition1.z += Height1;
    m_CurvePosition2.z += Height2;
}

void _CalcBezierCurvePosition( float fRate, const WzVector& p0, const WzVector& p1, const WzVector& p2, OUT WzVector& result )
{
    //조절점 1개인 베지어 기저식
    result = (p0 * (1.0f - fRate) * (1.0f - fRate) +   p1* (1.0f - fRate) * fRate*2.0f + p2 * fRate * fRate);
};

void _CalcBezierCurvePosition( float fRate, const WzVector& p0, const WzVector& p1, const WzVector& p2, const WzVector& p3, OUT WzVector& result )
{
    result = 
        p0 * ((1.0f - fRate) * (1.0f - fRate) * (1.0f - fRate)) +
        p1 * fRate * 3.0f * ((1.0f - fRate) * (1.0f - fRate)) +
        p2 * 3.0f * fRate * fRate * (1.0f - fRate) +
        p3 * (fRate*fRate*fRate);
};

void _CalcArrowPosition( const WzVector& StartPosition, const WzVector& TargetPosition, float fRate, OUT WzVector& Position )
{
    Position = StartPosition * (1.0f - fRate) + TargetPosition * (fRate);
}

void _CalcDirectionRotate( const WzVector& StartPosition, const WzVector& TargetPosition, OUT WzVector& Rotate )
{
    WzVector vDest = TargetPosition - StartPosition;

    float fLength;

    if ( vDest.x == 0.0f && vDest.y == 0.0f )
    {
        fLength = 0.0f;
    }
    else
    {
        WzVector vTemp = vDest;
        vTemp.z = 0.0f;

        fLength = VectorLength(&vTemp);
    }

    float fArrowAngleX = -atan2(vDest.z,fLength);
    float fArrowAngleZ = Math_GetAngleFromVector(&vDest);

    Rotate.x = fArrowAngleX;
    Rotate.y = 0.0f;
    Rotate.z = fArrowAngleZ;
}

void cProjectileBezier::Process()
{
    float fRate = GetArrowMoveTimeRate();

    //베지어 기저식

    if( m_Type == TYPE_DUAL_CONTROL )
    {
        _CalcBezierCurvePosition( fRate, m_StartPosition, m_CurvePosition1, m_CurvePosition2, m_TargetPosition, m_CurrentPosition );
        _CalcDirectionRotate( m_StartPosition, m_TargetPosition, m_CurrentRotate );
    }
    else if( m_Type == TYPE_SINGLE_CONTROL )
    {
        _CalcBezierCurvePosition( fRate, m_StartPosition, m_CurvePosition1, m_TargetPosition, m_CurrentPosition );
        _CalcDirectionRotate( m_StartPosition, m_TargetPosition, m_CurrentRotate );
    }
    else if( m_Type == TYPE_TRACE_OBJECT_SINGLE_CONTROL )
    {
        if( Character* pTarget = (Character*)g_ObjectManager.GetObject( m_TargetObjectKey ) )
        {
            WzVector TargetPosition = pTarget->GetArrowTargetPos();

            _CalcBezierCurvePosition( fRate, m_StartPosition, m_CurvePosition1, TargetPosition, m_CurrentPosition );
            _CalcDirectionRotate( m_StartPosition, TargetPosition, m_CurrentRotate );
        }
    }
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
cProjectileArrow::cProjectileArrow()
{

}


void cProjectileArrow::Process()
{
    m_TargetPosition.UpdatePosition();

    // UpdateProjectionPosition
    float fRate = GetArrowMoveTimeRate();

    _CalcArrowPosition( m_StartPosition, m_TargetPosition.GetPosition(), GetArrowMoveTimeRate(), m_ArrowPosition );
    _CalcDirectionRotate( m_StartPosition, m_TargetPosition.GetPosition(), m_ArrowRotate );

    m_ArrowScale = c_UnitVector;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


cProjectileBase::cProjectileBase()
: m_IsActived(FALSE)
, m_Delay(0)
, m_LifeTime(0)
, m_ActiveDelay(0)
, m_RemainTime(0)
{
}

BOOL cProjectileBase::Process( DWORD dwTick )
{
    m_Delay += dwTick;

    BOOL Result = IsTimeout();
    if( Result == TRUE )
        return TRUE;

    if( m_IsActived == FALSE )
    {
        if( m_Delay >= m_ActiveDelay )
        {
            m_IsActived = TRUE;
        }
    }

    return FALSE;
}

float cProjectileBase::GetMovingTimeRate()
{
    if( m_IsActived == FALSE )
    {
        return 0.0f;
    }

    return m_LifeTime > 0 ? min((float)((float)(m_Delay-m_ActiveDelay) / (float)m_LifeTime), 1.0f ) : 0.0f; 
}

float cProjectileBase::GetTotalLifeTimeRate()
{
    if( m_IsActived == FALSE )
    {
        return 0.0f;
    }

    return m_LifeTime > 0 ? min((float)((float)(m_Delay-m_ActiveDelay) / (float)(m_LifeTime+m_RemainTime) ), 1.0f ) : 0.0f; 
}

//float cProjectileBase::GetLifeTimeRate() 
//{
//    if( m_IsActived == FALSE )
//    {
//        return 0.0f;
//    }
//
//    return m_LifeTime > 0 ? min((float)((float)(m_Delay-m_ActiveDelay) / (float)m_LifeTime), 1.0f ) : 0.0f; 
//}

BOOL cProjectileBase::IsTimeout() 
{
    if( m_IsActived == FALSE )
        return FALSE;

    if( m_Delay - m_ActiveDelay >= m_LifeTime+m_RemainTime )
        return TRUE;

    return FALSE;
}

DWORD cProjectileBase::GetCurrentTickTime() 
{
    if( m_IsActived == FALSE )
        return 0;

    return m_Delay - m_ActiveDelay;
}

void cProjectileBase::SetRemainTime( DWORD RemainTime )
{
    m_RemainTime = RemainTime; 
}

void cProjectileBase::SetActiveDelayTime( DWORD ActiveDelayTime )
{
    m_ActiveDelay = ActiveDelayTime; 
}

void cProjectileBase::SetLifeTime( DWORD LifeTime ) 
{
    m_LifeTime = LifeTime; 
}


