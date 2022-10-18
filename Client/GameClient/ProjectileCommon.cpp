#include "SunClientPrecompiledHeader.h"
#include "ProjectileCommon.h"

//cProjectileCommon::cProjectileCommon()
//{
//    SetTraceType( nProjectile::TRACE_TYPE_TARGET_ARROW_POSITION );
//}
//
//BOOL cProjectileCommon::OnProcessProjectile( DWORD dwTick )
//{
//    UpdateTargetPosition();
//    UpdateProjectilePosition();
//
//    ProcessMeshProjectile();
//    ProcessUnitProjectile();
//    ProcessEffectProjectile();
//
//    m_dwDelay += dwTick;
//    m_bUpdated = TRUE;
//
//
//    // 일단 적에게 도착하거나, 지정된 시간이 되면 히트처리
//    if( GetRate() >= 1.0f || (m_Param.dwDamageTime > 0 && m_dwDelay > m_Param.dwDamageTime ))
//    {
//        ProcessHit();
//    }
//
//    if( m_Param.dwLifeTime > 0 )
//    {
//        // 라이프 타임이 설정되어있는 경우는 시간이 되면 종료
//        if( m_dwDelay >= m_Param.dwLifeTime )
//        {
//            ProcessHit();
//            return TRUE;
//        }
//    }
//    else
//    {
//        // 나머지는 대상에게 도착하면 종료
//        if( GetRate() > 1.0f )
//        {
//            ProcessHit();
//            return TRUE;
//        }
//    }
//
//    // 계속 날아간다
//    return FALSE;
//}