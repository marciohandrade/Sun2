#include "SunClientPrecompiledHeader.h"
#include "ProjectileUnit.h"
#ifdef _NA_007619_20140916_PROCESS_CHANGE_AIRBLOW_DOWN
#include "ProjectileManagerEx.h"
#endif // _NA_007619_20140916_PROCESS_CHANGE_AIRBLOW_DOWN


// MoveTime은 MaxRange일때의 도달시간 1000이 1초
// 100보다 작은 값은 반환되지 않는다
DWORD CalcProjectileMoveTimeByRange( const WzVector& StartPosition, const WzVector& TargetPosition, float MaxRange, DWORD MoveTime )
{
	WzVector Length = TargetPosition - StartPosition;
	float fLength = VectorLength(&Length);

	float fMoveTime = (float)MoveTime * (fLength/MaxRange);
	return max(100,(DWORD)fMoveTime);
}

cProjectileUnit::cProjectileUnit()
{

}

void cProjectileUnit::Destroy()
{
    cProjectileArrow::Destroy();
    cProjectileTrail::Destroy();
    cProjectileObject::DestroyProjectileObject();
}

void cProjectileUnit::Render()
{
    cProjectileObject::RenderProjectileObject();
    cProjectileTrail::Render();
}

BOOL cProjectileUnit::Process( DWORD TickTime )
{
    cProjectileArrow::Process();
    cProjectileTrail::Process();

    if( cProjectileObject::ProcessProjectileObject( TickTime ) == TRUE )
        return TRUE;

    return FALSE;
}


cProjectileSwordTrailUnit::cProjectileSwordTrailUnit()
{
}

void cProjectileSwordTrailUnit::Destroy()
{
    cProjectileSwordTrail::Destroy();
    cProjectileUnit::Destroy();
}

BOOL cProjectileSwordTrailUnit::Process( DWORD TickTime )
{
    if( cProjectileUnit::Process( TickTime ) == TRUE )
        return TRUE;

    cProjectileSwordTrail::Process( TickTime );

    return FALSE;
}

void cProjectileSwordTrailUnit::Render()
{
    cProjectileUnit::Render();
    cProjectileSwordTrail::Render();
}



cProjectileBezierUnit::cProjectileBezierUnit()
{

}

void cProjectileBezierUnit::Destroy()
{
    cProjectileBezier::Destroy();
	cProjectileTrail::Destroy();
    cProjectileObject::DestroyProjectileObject();
}

void cProjectileBezierUnit::Render()
{
	cProjectileTrail::Render();
    cProjectileObject::RenderProjectileObject();
}

BOOL cProjectileBezierUnit::Process( DWORD TickTime )
{
    cProjectileBezier::Process();
	cProjectileTrail::Process();
    if( cProjectileObject::ProcessProjectileObject( TickTime ) == TRUE )
    {
        return TRUE;
    }

    return FALSE;
}


#include "ObjectManager.h"
#include "Character.h"

cProjectileAirBlow::cProjectileAirBlow()
: m_BlowTargetObjectKey(0)
, m_fHeightLimit(0.0f)
, m_fUpperForce(0.0f)
{
}

BOOL cProjectileAirBlow::Process( DWORD TickTime )
{
    if( cProjectileUnit::Process( TickTime ) == TRUE )
    {
        if( Character* pTarget = (Character*)g_ObjectManager.GetObject( GetBlowTargetObjectKey() ) )
        {
            if( pTarget->GetCurState() == STATE::AIR )
            {
				pTarget->AddAirForce( m_fUpperForce, m_fHeightLimit );
#ifdef _NA_006688_20130502_1302_SKILL_RENEWAL_AIR_ANIMATION_RENEWAL
                switch( pTarget->GetWeaponAnimationCode() )
                {
                case '0':	pTarget->SetAnimation( "HS02", FALSE, TRUE); break;
                case '1':	pTarget->SetAnimation( "HS12", FALSE, TRUE); break;
                case '2':	pTarget->SetAnimation( "HS22", FALSE, TRUE); break;
                case '3':	pTarget->SetAnimation( "HS32", FALSE, TRUE); break;
                }
#endif //_NA_006688_20130502_1302_SKILL_RENEWAL_AIR_ANIMATION_RENEWAL
            }

#ifdef _NA_007619_20140916_PROCESS_CHANGE_AIRBLOW_DOWN
            cTargetPosition Position;
            Position.SetTargetVisiblePosition( pTarget->GetObjectKey(), TRUE );
            g_ProjectileManagerEx.CreateEffect( pTarget->GetObjectKey(), StrToWzID("KF61"), 5000, Position );
#endif // _NA_007619_20140916_PROCESS_CHANGE_AIRBLOW_DOWN
        }
        return TRUE;
    }

    return FALSE;
}

#include "GlobalFunc.h"

BOOL cProjectileTroch::Process( DWORD TickTime )
{
    if( cProjectileBezierUnit::Process( TickTime ) == TRUE )
    {
        WzVector Position = GetObjectPosition();

		GlobalFunc::NET_SEND_CG_ITEM_USE_SKILL_ACTION_SYN(eITEMTYPE_TORCH, Position);
		
		// 횃불 지속 이펙트 나오기 전에 4방향으로 타는 이펙트 연출
		float fperRotAngle = 1.57f;
		for( int i = 0; i < 4 ; i++ )
		{
			WzMatrix wmRot;
			MatrixIdentity(&wmRot);
			MatrixRotationZ(&wmRot, fperRotAngle * (float) i);

			WzVector wvTransform;
			WzVector wvOffset;
			wvOffset.x = 2.0f;
			wvOffset.y = 2.0f;
			wvOffset.z = 0.0f;
			VectorTransformCoord(&wvTransform, &wvOffset,&wmRot);

			WzVector wvEffectPos;
			wvEffectPos.x = wvTransform.x + Position.x;
			wvEffectPos.y = wvTransform.y + Position.y;
			wvEffectPos.z = wvTransform.z + Position.z;

			WzVector wvRot;
			wvRot.x = 0.0f;
			wvRot.y = 0.0f;
			wvRot.z = fperRotAngle * (float) i;

			WzVector wvScale;
			wvScale.x = 1.0f;
			wvScale.y = 1.0f;
			wvScale.z = 1.0f;
			GlobalFunc::CreateEffectBeUpToOption(StrToWzID("E310"), wvEffectPos,wvScale,wvRot);
		}			

		GlobalFunc::CreateEffectBeUpToOption(StrToWzID("E310"), Position);

        return TRUE;
    }

    return FALSE;
}
