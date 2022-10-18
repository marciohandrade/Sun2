#include "SunClientPrecompiledHeader.h"
#include "ProjectileArrow.h"

//
//cProjectileArrow::cProjectileArrow()
//: m_EffectHandle(INVALID_HANDLE_VALUE)
//, m_pUnitDraw(NULL)
//{
//
//}
////
////BOOL cProjectileArrow::CreateUnit( eProjectile_SRC ProjectileImageCode )
////{
////    RESOURCECODE code = (DWORD)ProjectileImageCode + ENGINE_RESOURCE_ID_OFFSET;
////    m_pUnitDraw = ResourceManager::Instance()->AllocResourceUnitDraw(code); //화살 
////
////    return m_pUnitDraw != NULL;
////}
////
////BOOL cProjectileArrow::CreateEffect( DWORD CasterObjectKey, DWORD TargetObjectKey, WzID EffectID )
////{
////    if( GetPositionData().UpdatePosition() == FALSE )
////        return FALSE;
////
////    // 루프이벡트 사용금지
////    m_EffectHandle = GlobalFunc::CreateEffectBeUpToOption( EffectID, GetPositionData().GetPosition(), CasterObjectKey, TargetObjectKey );
////
////    if( m_hEffect == INVALID_HANDLE_VALUE )
////        return FALSE;
////
////    if( UpdateEffectPosition() == FALSE )
////        return FALSE;
////
////    return TRUE;
////}
//
//
//BOOL cProjectileArrow::Create()
//{
//    return TRUE;
//}
//
//void cProjectileArrow::Destroy()
//{
//    if (m_EffectHandle != INVALID_HANDLE_VALUE )
//    {
//        g_pEffectManager->RemoveEffectPtr(m_hEffect);
//        m_EffectHandle = INVALID_HANDLE_VALUE;
//    }
//
//    if( m_pUnitDraw != NULL )
//    {
//        ResourceManager::Instance()->FreeResourceUnitDraw( m_pUnitDraw );
//        m_pUnitDraw = NULL;
//    }
//}
//
//BOOL cProjectileArrow::Process( DWORD TickTime )
//{
//    if(m_pUnitDraw)
//    {
//        //if (m_bUseExtraMatrix)
//        //{
//        //    g_pSunRenderer->AnimateUnit(m_pUnitDraw,0,&m_ExtraMatrix);
//        //}
//        //else
//        //{
//            g_pSunRenderer->AnimateUnit(m_pUnitDraw,0,0);
//        //}		 
//    }
//
//
//    if( UpdateEffectPosition() == FALSE )
//        return TRUE;    // 삭제
//
//    ProgressTick( TickTime );
//
//    return IsTimeout();
//}
//
//BOOL cProjectileArrow::UpdateEffectPosition()
//{
//    if( m_hEffect == INVALID_HANDLE_VALUE )
//        return FALSE;
//
//    if( GetPositionData().UpdatePosition() == FALSE )
//        return FALSE;
//
//    CEffectBody* pBody = g_pEffectManager->GetEffectPtrHandle(m_hEffect);
//    if( pBody == NULL )
//        return FALSE;
//
//    WzVector TargetPosition = GetPositionData().GetPosition();
//
//    float fRate = GetLifeTimeRate();
//
//    //fRate *= m_Param.fSpeed;
//
//    //if (fRate >= 1.0f)
//    //{
//    //    fRate = 1.0f;
//    //}
//
//    WzVector vDest,vTemp;
//    float fArrowAngleZ,fArrowAngleX;
//
//    vDest = TargetPosition - m_StartPosition;
//    vTemp = vDest;
//    vTemp.z = 0.0f;
//
//    float fLength;
//
//    if (vTemp.x == 0.0f && vTemp.y == 0.0f)
//    {
//        fLength = 0.0f;
//    }
//    else
//    {
//        fLength = VectorLength(&vTemp);
//    }
//
//    fArrowAngleX = -atan2(vDest.z,fLength);
//    fArrowAngleZ = Math_GetAngleFromVector(&vDest);
//
//    WzVector ProjectilePosition = m_StartPosition * (1.0f - fRate) + TargetPosition * (fRate);
//
//    WzVector ProjectileRotate;
//    Rotate.x = fArrowAngleX;
//    Rotate.y = 0.0f;
//    Rotate.z = fArrowAngleZ;
//
//    WzVector ProjectileScale = c_UnitVector;
//    pBody->SetResultMatrix( ProjectilePosition, ProjectileRotate, ProjectileScale);
//
//
//
//
//    //if( m_StaticRotate == TRUE )
//    //{
//    //    WzVector vPos = pTargetObject->GetBonePos( m_BoneIndex );
//    //    WzMatrix wmPosMat, wmRotMat, wmScale, mat;
//    //    mat = pTargetObject->GetBoneMatrix( m_BoneIndex );
//    //    wmRotMat = pTargetObject->GetBoneMatrix( m_BoneIndex );
//    //    wmRotMat._41 = 0.0f;
//    //    wmRotMat._42 = 0.0f;
//    //    wmRotMat._43 = 0.0f;
//
//    //    MatrixIdentity( &wmPosMat );
//    //    MatrixTranslation( &wmPosMat, vPos.x, vPos.y, vPos.z );
//
//    //    MatrixIdentity( &wmPosMat );
//    //    MatrixScaling( &wmScale,1.0f,1.0f,1.0f);
//
//    //    pBody->SetResultMatrix( &wmPosMat, &wmRotMat, &wmScale );
//
//    //}
//
//    pBody->SetResultMatrix( GetPositionData().GetPosition() + m_PositionOffset, GetPositionData().GetRotate(), GetPositionData().GetScale() );
//
//
//    return TRUE;
//}
