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
//    // �ϴ� ������ �����ϰų�, ������ �ð��� �Ǹ� ��Ʈó��
//    if( GetRate() >= 1.0f || (m_Param.dwDamageTime > 0 && m_dwDelay > m_Param.dwDamageTime ))
//    {
//        ProcessHit();
//    }
//
//    if( m_Param.dwLifeTime > 0 )
//    {
//        // ������ Ÿ���� �����Ǿ��ִ� ���� �ð��� �Ǹ� ����
//        if( m_dwDelay >= m_Param.dwLifeTime )
//        {
//            ProcessHit();
//            return TRUE;
//        }
//    }
//    else
//    {
//        // �������� ��󿡰� �����ϸ� ����
//        if( GetRate() > 1.0f )
//        {
//            ProcessHit();
//            return TRUE;
//        }
//    }
//
//    // ��� ���ư���
//    return FALSE;
//}