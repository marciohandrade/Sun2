#include "SunClientPrecompiledHeader.h"
#include "ProjectileProcess.h"

//#include "ObjectManager.h"
//#include "Camera.h"
//#include "GlobalFunc.h"
//#include "GameFunc.h"
//#include "Hero.h"
//#include "HeroData.h"
//#include "GameFramework.h"
//#include "LoadScene.h"
//#include "Map.h"
//
//
//////////////////////////////////////////////////////////////////////////////////////////
//cProjectileEx::cProjectileEx()
//{
//    m_TraceType = nProjectile::TRACE_TYPE_NONE;
//}
//
//void cProjectileEx::UpdateTargetPosition()
//{
//    nProjectile::eTraceType type = GetTraceType();
//
//    if( type == nProjectile::TRACE_TYPE_NONE )
//    {
//        return;
//    }
//
//    if( GetTargetObjectKey() == 0 || GetTargetObjectKey() == INVALID_OBJECT_KEY )
//    {
//        return;
//    }
//
//    if( Character* pTarget = (Character*)g_ObjectManager.GetObject(m_Param.dwTargetID) )
//    {
//        if( type == nProjectile::TRACE_TYPE_TARGET_ARROW_POSITION )
//        {
//            SetTargetPosition( pTarget->GetArrowTargetPos() );
//        }
//        else if( type == nProjectile::TRACE_TYPE_TARGET_ARROW_POSITION_ONCE )
//        {
//            SetTargetPosition( pTarget->GetArrowTargetPos() );
//            SetTargetObjectKey( INVALID_OBJECT_KEY );
//        }
//        else if( type == nProjectile::TRACE_TYPE_TARGET_VISIBLE_POSITION )
//        {
//            SetTargetPosition( pTarget->GetVisiblePos() );
//        }
//        else if( type == nProjectile::TRACE_TYPE_TARGET_VISIBLE_POSITION_ONCE )
//        {
//            SetTargetPosition( pTarget->GetVisiblePos() );
//            SetTargetObjectKey( INVALID_OBJECT_KEY );
//        }
//        else if( type == nProjectile::TRACE_TYPE_TARGET_TOP_POSITION )
//        {
//            WzVector p = pTarget->GetVisiblePos();
//            p.z += pTarget->GetHeight();
//            SetTargetPosition( p );
//        }
//        else if( type == nProjectile::TRACE_TYPE_TARGET_TOP_POSITION_ONCE )
//        {
//            WzVector p = pTarget->GetVisiblePos();
//            p.z += pTarget->GetHeight();
//            SetTargetPosition( p );
//            SetTargetObjectKey( INVALID_OBJECT_KEY );
//        }
//    }
//}
//
//void cProjectileEx::UpdateProjectilePosition()
//{
//    // calc position, rate
//
//    float fRate;
//
//    if(m_Param.dwMoveTime)
//        fRate= (float)m_dwDelay / (float) m_Param.dwMoveTime;		
//    else
//        fRate=0.0f;
//
//    float fTrailRate = 1.0f - fRate; 
//    float fEndRate;
//
//    m_Param.fSpeed = max(1.0f,m_Param.fSpeed);
//
//    fRate *= m_Param.fSpeed;
//    fEndRate = fRate - m_Param.fTrailLength;
//
//    if (fRate >= 1.0f)
//    {
//        fRate = 1.0f;
//    }
//
//    if (fEndRate <= 0.0f)
//    {
//        fEndRate = 0.0f;
//    }
//
//    if (fEndRate >= 1.0f)
//    {
//        fEndRate = 1.0f;
//    }
//
//
//    SetProjectilePosition( m_Param.vStartPos * (1.0f - fRate) + m_Param.vTargetPos * (fRate) );
//
//    if(m_Param.bNotMove)
//    {
//        SetProjectilePosition( m_Param.vStartPos );
//    }
//
//    SetRate( fRate, fEndRate, fTrailRate );
//
//
//    // cacl rotate
//    WzVector vDest = GetTargetPosition() - GetStartPosition();
//    WzVector vTemp = vDest;
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
//    WzVector Rotate;
//    Rotate.x = -atan2(vDest.z,fLength);
//    Rotate.y = 0;
//    Rotate.z = Math_GetAngleFromVector(&vDest);
//    SetProjectileRotate( Rotate );
//
//
//
//
//    //WzVector vCurPos,vCurPos2,vDestPos,vDestPos2,vNewPos,vNewPos2,vNewEndPos,vNewEndPos2;
//    //vCurPos = vCurPos2 = m_Param.vStartPos;
//    //vDestPos = vDestPos2 = m_Param.vTargetPos;
//
//    //VectorNormalize(&vDest,&vDest);
//    //WzVector vDirection;
//
//    //VectorCrossProduct(&vDirection,&vDest,&vDir);
//    //VectorNormalize(&vDirection,&vDirection);
//
//    //WzVector CamPos = pCameraInfo->m_wvFrom;
//    //WzVector BeamSrc = m_Param.vStartPos;
//    //WzVector BeamDst = m_Param.vTargetPos;
//
//    //WzVector vBeamDir = BeamDst - BeamSrc;
//    //VectorNormalize(&vBeamDir,&vBeamDir);
//    //WzVector vBeamToCam = CamPos - BeamSrc;
//    //VectorNormalize(&vBeamToCam,&vBeamToCam);		
//    //WzVector vBeamUp;
//    //vBeamUp.x = 0.0f;
//    //vBeamUp.y = 0.0f;
//    //vBeamUp.z = 1.0f;
//
//    //WzVector vCurrDir = vBeamToCam - vBeamDir;
//
//    //float fAngle = acos(VectorDotProduct(&vBeamUp,&vCurrDir));
//
//    //if (vCurrDir.x < 0)
//    //    fAngle = fAngle;
//    //else if (vCurrDir.x > 0)
//    //    fAngle = -fAngle;
//    //else
//    //    fAngle = 0;
//
//    //WzMatrix matrix;
//
//    //VectorNormalize(&vCurrDir,&vCurrDir);
//    //MatrixRotation(&matrix,&vCurrDir,fAngle);
//
//    //vTemp.x = 0.0f;
//    //vTemp.y = 0.0f;
//    //vTemp.z = 1.0f;
//
//    //float fHeight = m_Param.fHeight * fTrailRate;
//
//    //vCurPos = vCurPos + vDirection * (fHeight / 2.0f);
//    //vCurPos2 = vCurPos2 - vDirection * (fHeight / 2.0f);
//
//    //vDestPos = vDestPos + vDirection * (fHeight / 2.0f);
//    //vDestPos2 = vDestPos2 - vDirection * (fHeight / 2.0f);
//
//    //vNewPos = vCurPos * (1.0f - fRate) + vDestPos * (fRate);
//    //vNewPos2 = vCurPos2 * (1.0f - fRate) + vDestPos2 * (fRate);
//
//    //vNewEndPos = vCurPos * (1.0f - fEndRate) + vDestPos * (fEndRate);
//    //vNewEndPos2 = vCurPos2 * (1.0f - fEndRate) + vDestPos2 * (fEndRate);
//
//}
//
//
//void cProjectileEx::ProcessMeshProjectile()
//{
//    if( m_pMesh == NULL )
//        return;
//
//    St_Camera* pCameraInfo = g_pSunRenderer->GetCameraInfo();
//
//    WzVector vCurPos,vCurPos2,vDestPos,vDestPos2,vNewPos,vNewPos2,vNewEndPos,vNewEndPos2;
//    vCurPos = vCurPos2 = GetStartPosition();
//    vDestPos = vDestPos2 = GetTargetPosition();
//
//    WzVector t = vDestPos - vCurPos;
//
//    VectorNormalize(&t,&t);
//
//    WzVector vDirection;
//    VectorCrossProduct( &vDirection, &t, &pCameraInfo->m_wvDir);
//    VectorNormalize( &vDirection, &vDirection );
//
//
//    WzVector CamPos = pCameraInfo->m_wvFrom;
//    WzVector BeamSrc = m_Param.vStartPos;
//    WzVector BeamDst = m_Param.vTargetPos;
//
//    WzVector vBeamDir = BeamDst - BeamSrc;
//    VectorNormalize(&vBeamDir,&vBeamDir);
//    WzVector vBeamToCam = CamPos - BeamSrc;
//    VectorNormalize(&vBeamToCam,&vBeamToCam);		
//    WzVector vBeamUp;
//    vBeamUp.x = 0.0f;
//    vBeamUp.y = 0.0f;
//    vBeamUp.z = 1.0f;
//
//    WzVector vCurrDir = vBeamToCam - vBeamDir;
//
//    float fAngle = acos(VectorDotProduct(&vBeamUp,&vCurrDir));
//
//    if (vCurrDir.x < 0)
//        fAngle = fAngle;
//    else if (vCurrDir.x > 0)
//        fAngle = -fAngle;
//    else
//        fAngle = 0;
//
//    WzMatrix matrix;
//
//    VectorNormalize(&vCurrDir,&vCurrDir);
//    MatrixRotation(&matrix,&vCurrDir,fAngle);
//
//    float fHeight = m_Param.fHeight * GetTailRate();
//
//    vCurPos = vCurPos + vDirection * (fHeight / 2.0f);
//    vCurPos2 = vCurPos2 - vDirection * (fHeight / 2.0f);
//
//    vDestPos = vDestPos + vDirection * (fHeight / 2.0f);
//    vDestPos2 = vDestPos2 - vDirection * (fHeight / 2.0f);
//
//
//    vNewPos = vCurPos * (1.0f - GetRate()) + vDestPos * (GetRate());
//    vNewPos2 = vCurPos2 * (1.0f - GetRate()) + vDestPos2 * (GetRate());
//
//    vNewEndPos = vCurPos * (1.0f - GetEndRate()) + vDestPos * (GetEndRate());
//    vNewEndPos2 = vCurPos2 * (1.0f - GetEndRate()) + vDestPos2 * (GetEndRate());
//
//
//    WzVector vArrowDir = vDestPos - vCurPos;
//
//    VectorNormalize(&vArrowDir,&vArrowDir);
//
//    WzVector vTrailOffset = vArrowDir * -0.3f;
//
//
//    // 매쉬 좌표에 대입
//    m_pMesh->m_pVertices[0] = vTrailOffset + vNewEndPos;
//    m_pMesh->m_pVertices[1] = vTrailOffset + vNewPos;
//    m_pMesh->m_pVertices[2] = vTrailOffset + vNewEndPos2;
//    m_pMesh->m_pVertices[3] = vTrailOffset + vNewPos;
//    m_pMesh->m_pVertices[4] = vTrailOffset + vNewPos2;
//    m_pMesh->m_pVertices[5] = vTrailOffset + vNewEndPos2;
//}
//
//void cProjectileEx::ProcessUnitProjectile()
//{
//    if( m_pUnitDraw ) 
//    {
//        m_pUnitDraw->SetPosition( GetProjectilePosition(), FALSE );
//        m_pUnitDraw->SetRotation( GetProjectileRotate(), TRUE );
//    }
//}
//
//void cProjectileEx::ProcessEffectProjectile()
//{
//    if( m_hEffect == INVALID_HANDLE_VALUE )
//        return;
//
//    if( CEffectBody* pBody = g_pEffectManager->GetEffectPtrHandle(m_hEffect) )
//    {
//        WzVector vRot = GetProjectileRotate();
//        WzVector vScale;
//        vScale.x = 1.0f;
//        vScale.y = 1.0f;
//        vScale.y = 1.0f;
//
//        if( m_Param.vScale != c_ZeroVector)
//        {
//            vScale = m_Param.vScale;
//        }
//
//        if( m_Param.vDir != c_ZeroVector)
//        {
//            vRot = m_Param.vDir;
//        }
//
//        pBody->SetResultMatrix( GetProjectilePosition(), vRot, vScale );
//
//        if( m_Param.bFadeOut )
//        {
//            DWORD alphaTime = m_Param.dwLifeTime / 2;
//            if( alphaTime == 0 )
//                alphaTime = 1;
//
//            DWORD startAlphaTime = m_Param.dwLifeTime-alphaTime;
//
//            if( m_dwDelay >= startAlphaTime )
//            {
//                WzColor color = pBody->GetColor();
//
//                float fRate = (float)(m_dwDelay - startAlphaTime) / (float) alphaTime;		
//
//                float alphaRate = 1.0f - fRate;
//
//                float fAlpha = Alpha_WzColor(color);
//
//                if( alphaRate < 0.0f)
//                {
//                    alphaRate = 0.0f;
//                }
//
//                if( alphaRate > 1.0f)
//                {
//                    alphaRate=1.0f;
//                }
//
//                BYTE Alpha = (BYTE)(255 * alphaRate);
//                color = WzColor_RGBA(Red_WzColor(color),Green_WzColor(color),Blue_WzColor(color),Alpha);
//                pBody->SetColor(color);
//            }
//        }
//    }
//}
//
//void cProjectileEx::ProcessHit()
//{
//    // 히트는 중간 또는 마지막에 1회만 처리된다.
//
//    if( ProcessDamageGroup() == TRUE )
//    {
//        if (m_Param.bCameraShake )
//        {
//            g_Camera.SetVibeType( Camera::VIBE_TYPE_VERTICAL );
//            g_Camera.StartVibration( 50, 0.07f );
//
//            m_Param.bCameraShake = FALSE;
//        }
//
//        if( m_Param.EndeffectID )
//        {
//            HANDLE hEffect = GlobalFunc::CreateEffectBeUpToOption( m_Param.EndeffectID, GetProjectilePosition(), m_Param.dwOwner, m_Param.dwTargetID );//!loop 이펙트 쓰지 말것    
//
//            if( CEffectBody *pBody = g_pEffectManager->GetEffectPtrHandle(hEffect) )
//            {
//                pBody->SetResultMatrix( GetProjectilePosition(), GetProjectilePosition(), c_UnitVector);
//            }
//
//            m_Param.EndeffectID = 0;
//        }
//
//        m_Param.dwDamageTime = 0;
//    }
//}
//
//
//
//
//
//////////////////////////////////////////////////////////////////////////////////////////
//cProjectileArrow::cProjectileArrow()
//{
//    SetTraceType( nProjectile::TRACE_TYPE_TARGET_ARROW_POSITION );
//}
//
//BOOL cProjectileArrow::OnProcessProjectile( DWORD dwTick )
//{
//    St_Camera * pCameraInfo = g_pSunRenderer->GetCameraInfo();
//
//    WzVector vTo    = pCameraInfo->m_wvTo;
//    WzVector vRight = pCameraInfo->m_wvRight;
//    WzVector vDown  = -pCameraInfo->m_wvUp;
//    WzVector vDir   = pCameraInfo->m_wvDir;
//
//    assert(m_Param.dwMoveTime);
//
//    UpdateTargetPosition();
//
//    float fRate;
//
//    if(m_Param.dwMoveTime)
//        fRate= (float)m_dwDelay / (float) m_Param.dwMoveTime;		
//    else
//        fRate=0.0f;
//
//    float fTrailRate = 1.0f - fRate; 
//    float fEndRate;
//
//    fRate *= m_Param.fSpeed;
//    fEndRate = fRate - m_Param.fTrailLength;
//
//    if (fRate >= 1.0f)
//    {
//        fRate = 1.0f;
//    }
//
//    if (fEndRate <= 0.0f)
//    {
//        fEndRate = 0.0f;
//    }
//
//    if (fEndRate >= 1.0f)
//    {
//        fEndRate = 1.0f;
//    }
//
//    WzVector vArrowPos;
//    WzVector vDest,vTemp;
//    float fArrowAngleZ,fArrowAngleX;
//
//    vDest =  m_Param.vTargetPos - m_Param.vStartPos;
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
//    vArrowPos = m_Param.vStartPos * (1.0f - fRate) + m_Param.vTargetPos * (fRate);
//
//    WzVector vCurPos,vCurPos2,vDestPos,vDestPos2,vNewPos,vNewPos2,vNewEndPos,vNewEndPos2;
//    vCurPos = vCurPos2 = m_Param.vStartPos;
//    vDestPos = vDestPos2 = m_Param.vTargetPos;
//
//    VectorNormalize(&vDest,&vDest);
//    WzVector vDirection;
//
//    VectorCrossProduct(&vDirection,&vDest,&vDir);
//    VectorNormalize(&vDirection,&vDirection);
//
//    WzVector CamPos = pCameraInfo->m_wvFrom;
//    WzVector BeamSrc = m_Param.vStartPos;
//    WzVector BeamDst = m_Param.vTargetPos;
//
//    WzVector vBeamDir = BeamDst - BeamSrc;
//    VectorNormalize(&vBeamDir,&vBeamDir);
//    WzVector vBeamToCam = CamPos - BeamSrc;
//    VectorNormalize(&vBeamToCam,&vBeamToCam);		
//    WzVector vBeamUp;
//    vBeamUp.x = 0.0f;
//    vBeamUp.y = 0.0f;
//    vBeamUp.z = 1.0f;
//
//    WzVector vCurrDir = vBeamToCam - vBeamDir;
//
//    float fAngle = acos(VectorDotProduct(&vBeamUp,&vCurrDir));
//
//    if (vCurrDir.x < 0)
//        fAngle = fAngle;
//    else if (vCurrDir.x > 0)
//        fAngle = -fAngle;
//    else
//        fAngle = 0;
//
//    WzMatrix matrix;
//
//    VectorNormalize(&vCurrDir,&vCurrDir);
//    MatrixRotation(&matrix,&vCurrDir,fAngle);
//
//    vTemp.x = 0.0f;
//    vTemp.y = 0.0f;
//    vTemp.z = 1.0f;
//
//    float fHeight = m_Param.fHeight * fTrailRate;
//
//    vCurPos = vCurPos + vDirection * (fHeight / 2.0f);
//    vCurPos2 = vCurPos2 - vDirection * (fHeight / 2.0f);
//
//    vDestPos = vDestPos + vDirection * (fHeight / 2.0f);
//    vDestPos2 = vDestPos2 - vDirection * (fHeight / 2.0f);
//
//    vNewPos = vCurPos * (1.0f - fRate) + vDestPos * (fRate);
//    vNewPos2 = vCurPos2 * (1.0f - fRate) + vDestPos2 * (fRate);
//
//    vNewEndPos = vCurPos * (1.0f - fEndRate) + vDestPos * (fEndRate);
//    vNewEndPos2 = vCurPos2 * (1.0f - fEndRate) + vDestPos2 * (fEndRate);
//
//    if(m_pMesh)
//    {
//        m_pMesh->m_pVertices[0] = vNewEndPos;
//        m_pMesh->m_pVertices[1] = vNewPos;
//        m_pMesh->m_pVertices[2] = vNewEndPos2;
//        m_pMesh->m_pVertices[3] = vNewPos;
//        m_pMesh->m_pVertices[4] = vNewPos2;
//        m_pMesh->m_pVertices[5] = vNewEndPos2;
//
//        WzVector vArrowDir = vDestPos - vCurPos;
//
//        VectorNormalize(&vArrowDir,&vArrowDir);
//
//        WzVector vTrailOffset = vArrowDir * -0.3f;
//
//        for (int a = 0; a < 6; a++)
//        {
//            m_pMesh->m_pVertices[a] = m_pMesh->m_pVertices[a] + vTrailOffset;
//
//        }
//    }
//
//    if (m_pUnitDraw) 
//    {
//        m_pUnitDraw->SetPosition(vArrowPos,FALSE);
//        m_pUnitDraw->SetRotation(fArrowAngleX,0,fArrowAngleZ);
//    }
//
//    m_dwDelay += dwTick;
//
//
//    if (m_hEffect != INVALID_HANDLE_VALUE)
//    {
//        CEffectBody *pBody = g_pEffectManager->GetEffectPtrHandle(m_hEffect);
//        if(pBody)
//        {
//            WzVector vRot;
//
//            vRot.x = fArrowAngleX;
//            vRot.y = 0.0f;
//            vRot.z = fArrowAngleZ;
//
//
//            WzVector vScale;
//
//            vScale.x = 1.0f;
//            vScale.y = 1.0f;
//            vScale.z = 1.0f;
//
//
//            if(m_Param.vScale != c_ZeroVector)
//            {
//                vScale=m_Param.vScale;
//            }
//
//            pBody->SetResultMatrix(vArrowPos,vRot,vScale);
//
//        }
//    }
//
//    m_bUpdated = TRUE;
//
//    // 라이프 타임이 있을 경우엔 대체 한다.
//    if(m_Param.dwLifeTime)
//    {
//        fRate = (float)m_dwDelay / (float) m_Param.dwLifeTime;		
//    }
//
//    if (fRate >= 1.0f)
//    {
//        if(m_Param.EndeffectID)
//        {
//            HANDLE hHandle = GlobalFunc::CreateEffectBeUpToOption(m_Param.EndeffectID,vArrowPos,m_Param.dwOwner,m_Param.dwTargetID);//!loop 이펙트 쓰지 말것    
//
//            if( CEffectBody* pBody = g_pEffectManager->GetEffectPtrHandle(hHandle) )
//            {
//                WzVector vRot;
//
//                vRot.x = fArrowAngleX;
//                vRot.y = 0.0f;
//                vRot.z = fArrowAngleZ;
//
//
//                WzVector vScale;
//
//                vScale.x = 1.0f;
//                vScale.y = 1.0f;
//                vScale.z = 1.0f;
//
//                pBody->SetResultMatrix(vArrowPos,vRot,vScale);
//
//            }
//        }
//
//        // 다중타겟일경우 다음타겟으로 넘어가고
//        if (m_bMultiTarget)
//        {
//            ProcessDamage(m_pdwMultiTargetID[m_iCurTargetIndex]);
//            m_iCurTargetIndex++;
//            // 모든 타겟을 다 지나쳤다 삭제
//            if (m_iCurTargetIndex >= m_iTargetNum)
//            {
//                return TRUE;
//            }
//            // 화살딜레이를 0으로 초기화해준다 다시
//            else
//            {
//                DWORD dwNextTargetID = m_pdwMultiTargetID[m_iCurTargetIndex];
//                Character *pTarget = (Character *)g_ObjectManager.GetObject(dwNextTargetID);
//                m_dwDelay = 0;
//                m_Param.vStartPos = m_Param.vTargetPos;				
//                if(pTarget)
//                {
//                    m_Param.vTargetPos = pTarget->GetVisiblePos();
//                    m_Param.vTargetPos.z +=(pTarget->GetArrowTargetHeight());
//                }
//            }
//        }
//        // 단일 타겟일경우 발사체를 삭제.
//        else
//        {
//            ProcessDamage();
//            return TRUE;
//        }
//    }
//    return FALSE;
//
//}
//
//////////////////////////////////////////////////////////////////////////////////////////
//cProjectileSonyVine::cProjectileSonyVine()
//{
//    SetTraceType( nProjectile::TRACE_TYPE_TARGET_ARROW_POSITION );
//}
//
//BOOL cProjectileSonyVine::OnProcessProjectile( DWORD dwTick )
//{
//    St_Camera * pCameraInfo = g_pSunRenderer->GetCameraInfo();
//
//    WzVector vTo    = pCameraInfo->m_wvTo;
//    WzVector vRight = pCameraInfo->m_wvRight;
//    WzVector vDown  = -pCameraInfo->m_wvUp;
//    WzVector vDir   = pCameraInfo->m_wvDir;
//
//    assert(m_Param.dwMoveTime);
//
//    UpdateTargetPosition();
//
//    float fRate;
//
//    if(m_Param.dwMoveTime)
//        fRate= (float)m_dwDelay / (float) m_Param.dwMoveTime;		
//    else
//        fRate=0.0f;
//
//    float fTrailRate = 1.0f - fRate; 
//    float fEndRate;
//
//    fRate *= m_Param.fSpeed;
//    fEndRate = fRate - m_Param.fTrailLength;
//
//    if (fRate >= 1.0f)
//    {
//        fRate = 1.0f;
//    }
//
//    if (fEndRate <= 0.0f)
//    {
//        fEndRate = 0.0f;
//    }
//
//    if (fEndRate >= 1.0f)
//    {
//        fEndRate = 1.0f;
//    }
//
//    WzVector vArrowPos;
//    WzVector vDest,vTemp;
//    float fArrowAngleZ,fArrowAngleX;
//
//    vDest =  m_Param.vTargetPos - m_Param.vStartPos;
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
//    vArrowPos = m_Param.vStartPos * (1.0f - fRate) + m_Param.vTargetPos * (fRate);
//
//    WzVector vCurPos,vCurPos2,vDestPos,vDestPos2,vNewPos,vNewPos2,vNewEndPos,vNewEndPos2;
//    vCurPos = vCurPos2 = m_Param.vStartPos;
//    vDestPos = vDestPos2 = m_Param.vTargetPos;
//
//    VectorNormalize(&vDest,&vDest);
//    WzVector vDirection;
//
//    VectorCrossProduct(&vDirection,&vDest,&vDir);
//    VectorNormalize(&vDirection,&vDirection);
//
//    WzVector CamPos = pCameraInfo->m_wvFrom;
//    WzVector BeamSrc = m_Param.vStartPos;
//    WzVector BeamDst = m_Param.vTargetPos;
//
//    WzVector vBeamDir = BeamDst - BeamSrc;
//    VectorNormalize(&vBeamDir,&vBeamDir);
//    WzVector vBeamToCam = CamPos - BeamSrc;
//    VectorNormalize(&vBeamToCam,&vBeamToCam);		
//    WzVector vBeamUp;
//    vBeamUp.x = 0.0f;
//    vBeamUp.y = 0.0f;
//    vBeamUp.z = 1.0f;
//
//    WzVector vCurrDir = vBeamToCam - vBeamDir;
//
//    float fAngle = acos(VectorDotProduct(&vBeamUp,&vCurrDir));
//
//    if (vCurrDir.x < 0)
//        fAngle = fAngle;
//    else if (vCurrDir.x > 0)
//        fAngle = -fAngle;
//    else
//        fAngle = 0;
//
//    WzMatrix matrix;
//
//    VectorNormalize(&vCurrDir,&vCurrDir);
//    MatrixRotation(&matrix,&vCurrDir,fAngle);
//
//    vTemp.x = 0.0f;
//    vTemp.y = 0.0f;
//    vTemp.z = 1.0f;
//
//    float fHeight = m_Param.fHeight * fTrailRate;
//
//    vCurPos = vCurPos + vDirection * (fHeight / 2.0f);
//    vCurPos2 = vCurPos2 - vDirection * (fHeight / 2.0f);
//
//    vDestPos = vDestPos + vDirection * (fHeight / 2.0f);
//    vDestPos2 = vDestPos2 - vDirection * (fHeight / 2.0f);
//
//    vNewPos = vCurPos * (1.0f - fRate) + vDestPos * (fRate);
//    vNewPos2 = vCurPos2 * (1.0f - fRate) + vDestPos2 * (fRate);
//
//    vNewEndPos = vCurPos * (1.0f - fEndRate) + vDestPos * (fEndRate);
//    vNewEndPos2 = vCurPos2 * (1.0f - fEndRate) + vDestPos2 * (fEndRate);
//
//    if (m_pUnitDraw) 
//    {
//        float	fLoopCheck;
//        fLoopCheck = 0.0f;
//        WzEvent Event;
//        ZeroMemory(&Event,sizeof(Event));
//
//        g_pSunRenderer->ProgressUnitFrame(m_pUnitDraw, ( float )dwTick, &Event, &fLoopCheck );
//
//        m_pUnitDraw->SetPosition(vArrowPos);
//
//        m_pUnitDraw->SetRotation(0.0f,0.0f,m_fAngleZ);
//
//        //유닛에 딸린 이펙트 처리 
//        if (Event.m_iNumEffect > 0)
//        {
//            for (int a = 0; a < Event.m_iNumEffect; a++)
//            {
//                int iIndex = Event.m_Effect[a].m_iEffectType;
//
//                assert(iIndex >= 0 && iIndex < MAX_OBJECT_EFFECT);
//
//                if (iIndex < 0)
//                {
//                    iIndex = 0;
//                }
//                else if (iIndex >= MAX_OBJECT_EFFECT)
//                {
//                    iIndex = MAX_OBJECT_EFFECT - 1;
//                }
//
//                if (Event.m_Effect[a].m_byEffectFlag & EFF_DESTROY)
//                {
//                    DestroyObjectEffect(iIndex);
//                }
//                else
//                {
//                    DestroyObjectEffect(iIndex);
//
//                    int boneindex = Event.m_Effect[a].m_iEffectBoneIndex;
//                    WzVector vPos;	
//                    GameFunc::GetBonePos(m_pUnitDraw,boneindex,&vPos);
//
//                    m_EffectParam[iIndex].hEffect = GlobalFunc::CreateEffectBeUpToOption(Event.m_Effect[a].m_wiEffectWzID,vPos,m_Param.dwOwner,m_Param.dwTargetID);
//                    m_EffectParam[iIndex].iBoneIndex = boneindex;
//                }
//            }
//
//        }
//    }
//
//    m_dwDelay += dwTick;
//
//
//    if (m_hEffect != INVALID_HANDLE_VALUE)
//    {
//        CEffectBody *pBody = g_pEffectManager->GetEffectPtrHandle(m_hEffect);
//        if(pBody)
//        {
//            WzVector vRot;
//
//            vRot.x = fArrowAngleX;
//            vRot.y = 0.0f;
//            vRot.z = fArrowAngleZ;
//
//
//            WzVector vScale;
//
//            vScale.x = 1.0f;
//            vScale.y = 1.0f;
//            vScale.z = 1.0f;
//
//
//            if(m_Param.vScale != c_ZeroVector)
//            {
//                vScale=m_Param.vScale;
//            }
//
//            pBody->SetResultMatrix(vArrowPos,vRot,vScale);
//
//        }
//    }
//
//    m_bUpdated = TRUE;
//
//    Object* pObj = g_ObjectManager.GetObject( m_Param.dwTargetID );
//
//    if(pObj && pObj->IsKindOfObject(CHARACTER_OBJECT))
//    {
//        Character *pChr=(Character *)pObj;
//        if(!pChr->CannotMove())
//        {
//            DWORD alphaTime=m_Param.dwLifeTime/4;
//            if(!alphaTime)
//                alphaTime=1;
//
//            DWORD startAlphaTime=m_Param.dwLifeTime-alphaTime;
//
//            if(m_dwDelay<startAlphaTime)
//            {
//                if(!m_Param.SONYVINE.bDeBuff)
//                {
//                    m_Param.SONYVINE.bDeBuff=TRUE;
//                    DWORD alphaTime=m_dwDelay/4;
//                    if(!alphaTime)
//                        alphaTime=1000;
//
//                    m_Param.dwLifeTime=m_Param.dwMoveTime=m_dwDelay+alphaTime;
//
//                }
//
//            }
//        }
//    }
//
//    // 라이프 타임이 있을 경우엔 대체 한다.
//    if(m_Param.dwLifeTime)
//    {
//        fRate = (float)m_dwDelay / (float) m_Param.dwLifeTime;		
//    }
//
//
//
//    if(m_Param.bFadeOut)
//    {
//        DWORD alphaTime=m_Param.dwLifeTime/4;
//        if(!alphaTime)
//            alphaTime=1;
//
//        DWORD startAlphaTime=m_Param.dwLifeTime-alphaTime;
//
//        if(m_dwDelay>=startAlphaTime)
//        {
//            float fRate = (float)(m_dwDelay-startAlphaTime) / (float) alphaTime;		
//
//            float alphaRate=1.0f-fRate;
//            if(alphaRate<0.0f)
//            {
//                alphaRate=0.0f;
//            }
//
//            if(alphaRate>1.0f)
//            {
//                alphaRate=1.0f;
//            }
//
//            BYTE Alpha = (BYTE)(255 * alphaRate);
//            m_wzColor = WzColor_RGBA(255,255,255,Alpha);
//        }
//    }
//
//    if (fRate >= 1.0f)
//    {
//        // 데미지 처리를 한다.
//        ProcessDamageGroup();
//        return TRUE;
//    }
//    return FALSE;
//}
//
//////////////////////////////////////////////////////////////////////////////////////////
//cProjectilePoisonThorn::cProjectilePoisonThorn()
//{
//    SetTraceType( nProjectile::TRACE_TYPE_TARGET_VISIBLE_POSITION_ONCE );
//}
//
//BOOL cProjectilePoisonThorn::OnProcessProjectile( DWORD dwTick )
//{
//    UpdateTargetPosition();
//
//    ASSERT(m_Param.dwMoveTime);
//
//    float fRate;
//
//    if(m_Param.dwMoveTime)
//        fRate= (float)m_dwDelay / (float) m_Param.dwMoveTime;		
//    else
//        fRate=0.0f;
//
//    float fTrailRate = 1.0f - fRate; 
//    float fEndRate;
//
//    fRate *= m_Param.fSpeed;
//    fEndRate = fRate - m_Param.fTrailLength;
//
//    if (fRate >= 1.0f)
//    {
//        fRate = 1.0f;
//    }
//
//    if (fEndRate <= 0.0f)
//    {
//        fEndRate = 0.0f;
//    }
//
//    if (fEndRate >= 1.0f)
//    {
//        fEndRate = 1.0f;
//    }
//
//    WzVector vArrowPos;
//    WzVector vDest,vTemp;
//    float fArrowAngleZ,fArrowAngleX;
//
//
//    vDest =  m_Param.vTargetPos - m_Param.vStartPos;
//
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
//    vArrowPos = m_Param.vStartPos * (1.0f - fRate) + m_Param.vTargetPos * (fRate);
//
//    WzVector vCurPos,vCurPos2,vDestPos,vDestPos2;
//    vCurPos = vCurPos2 = m_Param.vStartPos;
//    vDestPos = vDestPos2 = m_Param.vTargetPos;
//
//
//    if (m_pUnitDraw) 
//    {
//        float	fLoopCheck;
//        fLoopCheck = 0.0f;
//        WzEvent Event;
//        ZeroMemory(&Event,sizeof(Event));
//
//        g_pSunRenderer->ProgressUnitFrame(m_pUnitDraw, ( float )dwTick, &Event, &fLoopCheck );
//
//        m_pUnitDraw->SetPosition(m_Param.vStartPos);
//        m_pUnitDraw->SetRotation(fArrowAngleX,0.0f,fArrowAngleZ);
//
//
//        //유닛에 딸린 이펙트 처리 
//        if (Event.m_iNumEffect > 0)
//        {
//            for (int a = 0; a < Event.m_iNumEffect; a++)
//            {
//                int iIndex = Event.m_Effect[a].m_iEffectType;
//
//                ASSERT(iIndex >= 0 && iIndex < MAX_OBJECT_EFFECT);
//
//                if (iIndex < 0)
//                {
//                    iIndex = 0;
//                }
//                else if (iIndex >= MAX_OBJECT_EFFECT)
//                {
//                    iIndex = MAX_OBJECT_EFFECT - 1;
//                }
//
//                if (Event.m_Effect[a].m_byEffectFlag & EFF_DESTROY)
//                {
//                    DestroyObjectEffect(iIndex);
//                }
//                else
//                {
//                    DestroyObjectEffect(iIndex);
//
//                    int boneindex = Event.m_Effect[a].m_iEffectBoneIndex;
//                    WzVector vPos;	
//                    GameFunc::GetBonePos(m_pUnitDraw,boneindex,&vPos);
//
//
//                    m_EffectParam[iIndex].hEffect = GlobalFunc::CreateEffectBeUpToOption(Event.m_Effect[a].m_wiEffectWzID,vPos,m_Param.dwOwner,m_Param.dwTargetID);	
//                    m_EffectParam[iIndex].iBoneIndex = boneindex;
//                }
//            }
//
//        }
//    }
//
//    m_dwDelay += dwTick;
//
//
//    if (m_hEffect != INVALID_HANDLE_VALUE)
//    {
//        CEffectBody *pBody = g_pEffectManager->GetEffectPtrHandle(m_hEffect);
//        if(pBody)
//        {
//
//            WzVector vRot;
//
//            vRot.x = fArrowAngleX;
//            vRot.y = 0.0f;
//            vRot.z = fArrowAngleZ;
//
//
//            WzVector vScale;
//
//            vScale.x = 1.0f;
//            vScale.y = 1.0f;
//            vScale.z = 1.0f;
//
//
//            if(m_Param.vScale != c_ZeroVector)
//            {
//                vScale=m_Param.vScale;
//            }
//
//            pBody->SetResultMatrix(m_Param.vStartPos,vRot,vScale);
//        }
//    }
//
//    m_bUpdated = TRUE;
//
//
//    // 라이프 타임이 있을 경우엔 대체 한다.
//    if(m_Param.dwLifeTime)
//    {
//        fRate = (float)m_dwDelay / (float) m_Param.dwLifeTime;		
//    }
//
//    if(m_Param.bFadeOut)
//    {
//        DWORD alphaTime=m_Param.dwLifeTime/4;
//        if(!alphaTime)
//            alphaTime=1;
//
//        DWORD startAlphaTime=m_Param.dwLifeTime-alphaTime;
//
//        if(m_dwDelay>=startAlphaTime)
//        {
//            float fRate = (float)(m_dwDelay-startAlphaTime) / (float) alphaTime;		
//
//            float alphaRate=1.0f-fRate;
//            if(alphaRate<0.0f)
//            {
//                alphaRate=0.0f;
//            }
//
//            if(alphaRate>1.0f)
//            {
//                alphaRate=1.0f;
//            }
//
//            BYTE Alpha = (BYTE)(255 * alphaRate);
//            m_wzColor = WzColor_RGBA(255,255,255,Alpha);
//
//        }
//    }
//
//    if (fRate >= 1.0f)
//    {
//        // 데미지 처리를 한다.
//        ProcessDamageGroup();
//        return TRUE;
//    }
//
//    return FALSE;
//}
//
//
//////////////////////////////////////////////////////////////////////////////////////////
//cProjectilePoisonRain::cProjectilePoisonRain()
//{
//    SetTraceType( nProjectile::TRACE_TYPE_NONE );
//}
//
//BOOL cProjectilePoisonRain::OnProcessProjectile( DWORD dwTick )
//{
//    UpdateTargetPosition();
//
//    St_Camera * pCameraInfo = g_pSunRenderer->GetCameraInfo();
//
//    WzVector vTo    = pCameraInfo->m_wvTo;
//    WzVector vRight = pCameraInfo->m_wvRight;
//    WzVector vDown  = -pCameraInfo->m_wvUp;
//    WzVector vDir   = pCameraInfo->m_wvDir;
//
//    assert(m_Param.dwMoveTime);
//
//    float fRate;
//
//    if(m_Param.dwMoveTime)
//        fRate= (float)m_dwDelay / (float) m_Param.dwMoveTime;		
//    else
//        fRate=0.0f;
//
//    float fTrailRate = 1.0f - fRate; 
//    float fEndRate;
//
//    fRate *= m_Param.fSpeed;
//    fEndRate = fRate - m_Param.fTrailLength;
//
//    if (fRate >= 1.0f)
//    {
//        fRate = 1.0f;
//    }
//
//    if (fEndRate <= 0.0f)
//    {
//        fEndRate = 0.0f;
//    }
//
//    if (fEndRate >= 1.0f)
//    {
//        fEndRate = 1.0f;
//    }
//
//    WzVector vArrowPos;
//    WzVector vDest,vTemp;
//    float fArrowAngleZ,fArrowAngleX;
//
//    vDest =  m_Param.vTargetPos - m_Param.vStartPos;
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
//    vArrowPos = m_Param.vStartPos * (1.0f - fRate) + m_Param.vTargetPos * (fRate);
//
//    WzVector vCurPos,vCurPos2,vDestPos,vDestPos2,vNewPos,vNewPos2,vNewEndPos,vNewEndPos2;
//    vCurPos = vCurPos2 = m_Param.vStartPos;
//    vDestPos = vDestPos2 = m_Param.vTargetPos;
//
//    VectorNormalize(&vDest,&vDest);
//    WzVector vDirection;
//
//    VectorCrossProduct(&vDirection,&vDest,&vDir);
//    VectorNormalize(&vDirection,&vDirection);
//
//    WzVector CamPos = pCameraInfo->m_wvFrom;
//    WzVector BeamSrc = m_Param.vStartPos;
//    WzVector BeamDst = m_Param.vTargetPos;
//
//    WzVector vBeamDir = BeamDst - BeamSrc;
//    VectorNormalize(&vBeamDir,&vBeamDir);
//    WzVector vBeamToCam = CamPos - BeamSrc;
//    VectorNormalize(&vBeamToCam,&vBeamToCam);		
//    WzVector vBeamUp;
//    vBeamUp.x = 0.0f;
//    vBeamUp.y = 0.0f;
//    vBeamUp.z = 1.0f;
//
//    WzVector vCurrDir = vBeamToCam - vBeamDir;
//
//    float fAngle = acos(VectorDotProduct(&vBeamUp,&vCurrDir));
//
//    if (vCurrDir.x < 0)
//        fAngle = fAngle;
//    else if (vCurrDir.x > 0)
//        fAngle = -fAngle;
//    else
//        fAngle = 0;
//
//    WzMatrix matrix;
//
//    VectorNormalize(&vCurrDir,&vCurrDir);
//    MatrixRotation(&matrix,&vCurrDir,fAngle);
//
//    vTemp.x = 0.0f;
//    vTemp.y = 0.0f;
//    vTemp.z = 1.0f;
//
//    float fHeight = m_Param.fHeight * fTrailRate;
//
//    vCurPos = vCurPos + vDirection * (fHeight / 2.0f);
//    vCurPos2 = vCurPos2 - vDirection * (fHeight / 2.0f);
//
//    vDestPos = vDestPos + vDirection * (fHeight / 2.0f);
//    vDestPos2 = vDestPos2 - vDirection * (fHeight / 2.0f);
//
//    vNewPos = vCurPos * (1.0f - fRate) + vDestPos * (fRate);
//    vNewPos2 = vCurPos2 * (1.0f - fRate) + vDestPos2 * (fRate);
//
//    vNewEndPos = vCurPos * (1.0f - fEndRate) + vDestPos * (fEndRate);
//    vNewEndPos2 = vCurPos2 * (1.0f - fEndRate) + vDestPos2 * (fEndRate);
//
//    // 발사체가 적에게 닿았으면	
//    if(fRate >= 1.0f&&m_Param.POISON_RAIN.bAniChange)
//    {
//        if (m_Param.bCameraShake)
//        {
//            m_Param.bCameraShake=FALSE;
//            g_Camera.SetVibeType(Camera::VIBE_TYPE_VERTICAL);
//            g_Camera.StartVibration(40,0.03f);
//        }
//
//        m_Param.POISON_RAIN.bAniChange=FALSE;
//        SetAni((m_Param.POISON_RAIN.aniIDEnd));
//        ProcessDamageGroup();
//    }
//
//    if (m_pUnitDraw) 
//    {
//        float	fLoopCheck;
//        fLoopCheck = 0.0f;
//        WzEvent Event;
//        ZeroMemory(&Event,sizeof(Event));
//
//        g_pSunRenderer->ProgressUnitFrame(m_pUnitDraw, ( float )dwTick, &Event, &fLoopCheck );
//
//        m_pUnitDraw->SetPosition(vArrowPos);
//
//
//        //유닛에 딸린 이펙트 처리 
//        if (Event.m_iNumEffect > 0)
//        {
//            for (int a = 0; a < Event.m_iNumEffect; a++)
//            {
//                int iIndex = Event.m_Effect[a].m_iEffectType;
//
//                assert(iIndex >= 0 && iIndex < MAX_OBJECT_EFFECT);
//
//                if (iIndex < 0)
//                {
//                    iIndex = 0;
//                }
//                else if (iIndex >= MAX_OBJECT_EFFECT)
//                {
//                    iIndex = MAX_OBJECT_EFFECT - 1;
//                }
//
//                if (Event.m_Effect[a].m_byEffectFlag & EFF_DESTROY)
//                {
//                    DestroyObjectEffect(iIndex);
//                }
//                else
//                {
//                    DestroyObjectEffect(iIndex);
//
//                    int boneindex = Event.m_Effect[a].m_iEffectBoneIndex;
//                    WzVector vPos=vArrowPos;	
//                    GameFunc::GetBonePos(m_pUnitDraw,boneindex,&vPos);
//
//
//                    m_EffectParam[iIndex].hEffect= GlobalFunc::CreateEffectBeUpToOption(Event.m_Effect[a].m_wiEffectWzID,vPos,m_Param.dwOwner,m_Param.dwTargetID);//!loop 이펙트 쓰지 말것    
//
//                    m_EffectParam[iIndex].iBoneIndex = boneindex;
//                }
//            }
//        }
//    }
//
//    m_dwDelay += dwTick;
//
//
//    if (m_hEffect != INVALID_HANDLE_VALUE)
//    {
//        CEffectBody *pBody = g_pEffectManager->GetEffectPtrHandle(m_hEffect);
//        if(pBody)
//        {
//            WzVector vRot;
//
//            vRot.x = fArrowAngleX;
//            vRot.y = 0.0f;
//            vRot.z = fArrowAngleZ;
//
//
//            WzVector vScale;
//
//            vScale.x = 1.0f;
//            vScale.y = 1.0f;
//            vScale.z = 1.0f;
//
//
//            if(m_Param.vScale != c_ZeroVector)
//            {
//                vScale=m_Param.vScale;
//            }
//
//            pBody->SetResultMatrix(vArrowPos,vRot,vScale);
//
//        }
//    }
//
//    m_bUpdated = TRUE;
//
//
//    // 라이프 타임이 있을 경우엔 대체 한다.
//    if(m_Param.dwLifeTime)
//    {
//        fRate = (float)m_dwDelay / (float) m_Param.dwLifeTime;		
//    }
//
//    if(m_Param.bFadeOut)
//    {
//        DWORD alphaTime=m_Param.dwLifeTime/4;
//        if(!alphaTime)
//            alphaTime=1;
//
//        DWORD startAlphaTime=m_Param.dwLifeTime-alphaTime;
//
//        if(m_dwDelay>=startAlphaTime)
//        {
//            float fRate = (float)(m_dwDelay-startAlphaTime) / (float) alphaTime;		
//
//            float alphaRate=1.0f-fRate;
//            if(alphaRate<0.0f)
//            {
//                alphaRate=0.0f;
//            }
//
//            if(alphaRate>1.0f)
//            {
//                alphaRate=1.0f;
//            }
//
//            BYTE Alpha = (BYTE)(255 * alphaRate);
//            m_wzColor = WzColor_RGBA(255,255,255,Alpha);
//        }
//    }
//
//    if (fRate >= 1.0f)
//    {
//        return TRUE;
//    }
//
//    return FALSE;
//}
//
//////////////////////////////////////////////////////////////////////////////////////////
//cProjectileAttachmentWeapon::cProjectileAttachmentWeapon()
//{
//    SetTraceType( nProjectile::TRACE_TYPE_TARGET_ARROW_POSITION );
//}
//
//BOOL cProjectileAttachmentWeapon::OnProcessProjectile( DWORD dwTick )
//{
//    St_Camera * pCameraInfo = g_pSunRenderer->GetCameraInfo();
//
//    WzVector vTo    = pCameraInfo->m_wvTo;
//    WzVector vRight = pCameraInfo->m_wvRight;
//    WzVector vDown  = -pCameraInfo->m_wvUp;
//    WzVector vDir   = pCameraInfo->m_wvDir;
//
//    assert(m_Param.dwMoveTime);
//
//    UpdateTargetPosition();
//
//    float fRate;
//
//    if(m_Param.dwMoveTime)
//        fRate= (float)m_dwDelay / (float) m_Param.dwMoveTime;		
//    else
//        fRate=0.0f;
//
//    float fTrailRate = 1.0f - fRate; 
//    float fEndRate;
//
//    fRate *= m_Param.fSpeed;
//    fEndRate = fRate - m_Param.fTrailLength;
//
//    if (fRate >= 1.0f)
//    {
//        fRate = 1.0f;
//    }
//
//    if (fEndRate <= 0.0f)
//    {
//        fEndRate = 0.0f;
//    }
//
//    if (fEndRate >= 1.0f)
//    {
//        fEndRate = 1.0f;
//    }
//
//    WzVector vArrowPos;
//    WzVector vDest,vTemp;
//    float fArrowAngleZ,fArrowAngleX;
//
//    vDest =  m_Param.vTargetPos - m_Param.vStartPos;
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
//    vArrowPos = m_Param.vStartPos * (1.0f - fRate) + m_Param.vTargetPos * (fRate);
//
//    WzVector vCurPos,vCurPos2,vDestPos,vDestPos2,vNewPos,vNewPos2,vNewEndPos,vNewEndPos2;
//    vCurPos = vCurPos2 = m_Param.vStartPos;
//    vDestPos = vDestPos2 = m_Param.vTargetPos;
//
//    VectorNormalize(&vDest,&vDest);
//    WzVector vDirection;
//
//    VectorCrossProduct(&vDirection,&vDest,&vDir);
//    VectorNormalize(&vDirection,&vDirection);
//
//    WzVector CamPos = pCameraInfo->m_wvFrom;
//    WzVector BeamSrc = m_Param.vStartPos;
//    WzVector BeamDst = m_Param.vTargetPos;
//
//    WzVector vBeamDir = BeamDst - BeamSrc;
//    VectorNormalize(&vBeamDir,&vBeamDir);
//    WzVector vBeamToCam = CamPos - BeamSrc;
//    VectorNormalize(&vBeamToCam,&vBeamToCam);		
//    WzVector vBeamUp;
//    vBeamUp.x = 0.0f;
//    vBeamUp.y = 0.0f;
//    vBeamUp.z = 1.0f;
//
//    WzVector vCurrDir = vBeamToCam - vBeamDir;
//
//    float fAngle = acos(VectorDotProduct(&vBeamUp,&vCurrDir));
//
//    if (vCurrDir.x < 0)
//        fAngle = fAngle;
//    else if (vCurrDir.x > 0)
//        fAngle = -fAngle;
//    else
//        fAngle = 0;
//
//    WzMatrix matrix;
//
//    VectorNormalize(&vCurrDir,&vCurrDir);
//    MatrixRotation(&matrix,&vCurrDir,fAngle);
//
//    vTemp.x = 0.0f;
//    vTemp.y = 0.0f;
//    vTemp.z = 1.0f;
//
//    float fHeight = m_Param.fHeight * fTrailRate;
//
//    vCurPos = vCurPos + vDirection * (fHeight / 2.0f);
//    vCurPos2 = vCurPos2 - vDirection * (fHeight / 2.0f);
//
//    vDestPos = vDestPos + vDirection * (fHeight / 2.0f);
//    vDestPos2 = vDestPos2 - vDirection * (fHeight / 2.0f);
//
//    vNewPos = vCurPos * (1.0f - fRate) + vDestPos * (fRate);
//    vNewPos2 = vCurPos2 * (1.0f - fRate) + vDestPos2 * (fRate);
//
//    vNewEndPos = vCurPos * (1.0f - fEndRate) + vDestPos * (fEndRate);
//    vNewEndPos2 = vCurPos2 * (1.0f - fEndRate) + vDestPos2 * (fEndRate);
//
//
//    if (m_pUnitDraw) 
//    {
//        m_fAngleZ+=(WZ_PI/170.0f* dwTick);
//
//        // 뉘어 돌리기
//        ClampAngle(&m_fAngleZ);
//        m_pUnitDraw->SetPosition(vArrowPos,FALSE);
//        m_pUnitDraw->SetRotation(0.0f,WZ_PI / 2.0f,m_fAngleZ);
//        m_pUnitDraw->SetScale(1.2f,1.2f,1.2f);
//    }
//
//    m_dwDelay += dwTick;
//
//    if (m_hEffect != INVALID_HANDLE_VALUE)
//    {
//        CEffectBody *pBody = g_pEffectManager->GetEffectPtrHandle(m_hEffect);
//        if(pBody)
//        {
//            WzVector vRot;
//            vRot.x = fArrowAngleX;
//            vRot.y = 0.0f;
//            vRot.z = fArrowAngleZ;
//            WzVector vScale;
//            vScale.x = 1.0f;
//            vScale.y = 1.0f;
//            vScale.z = 1.0f;
//
//            if(m_Param.vScale != c_ZeroVector)
//            {
//                vScale=m_Param.vScale;
//            }
//            pBody->SetResultMatrix(vArrowPos,vRot,vScale);
//
//            if(m_Param.bFadeOut)
//            {
//                DWORD alphaTime=m_Param.dwLifeTime/2;
//                if(!alphaTime)
//                    alphaTime=1;
//
//                DWORD startAlphaTime=m_Param.dwLifeTime-alphaTime;
//
//                if(m_dwDelay>=startAlphaTime)
//                {
//                    WzColor color=pBody->GetColor();
//
//                    float fRate = (float)(m_dwDelay-startAlphaTime) / (float) alphaTime;		
//
//                    float alphaRate=1.0f-fRate;
//
//                    float fAlpha=Alpha_WzColor(color);
//
//                    if(alphaRate<0.0f)
//                    {
//                        alphaRate=0.0f;
//                    }
//
//                    if(alphaRate>1.0f)
//                    {
//                        alphaRate=1.0f;
//                    }
//
//                    BYTE Alpha = (BYTE)(255 * alphaRate);
//                    color = WzColor_RGBA(Red_WzColor(color),Green_WzColor(color),Blue_WzColor(color),Alpha);
//                    pBody->SetColor(color);
//
//                }
//            }
//
//
//        }
//    }
//
//    m_bUpdated = TRUE;
//
//    // 발사체가 적에게 닿았으면	
//    if(fRate >= 1.0f)
//    {
//        if (m_Param.bCameraShake)
//        {
//            m_Param.bCameraShake=FALSE;
//            g_Camera.SetVibeType(Camera::VIBE_TYPE_VERTICAL);
//            g_Camera.StartVibration(50,0.07f);
//        }
//
//        ProcessDamageGroup();
//    }
//
//    // 라이프 타임이 있을 경우엔 대체 한다.
//    if(m_Param.dwLifeTime)
//    {
//        fRate = (float)m_dwDelay / (float) m_Param.dwLifeTime;		
//    }
//
//    //데미지 시간이 있을경우는 바로 데미지 처리 
//    if(m_Param.dwDamageTime)
//    {
//        if(m_dwDelay>m_Param.dwDamageTime)
//        {
//
//            //데미지가 처리 되었으면
//            if(ProcessDamageGroup())
//            {
//                m_Param.dwDamageTime = 0;
//            }
//        }
//    }
//
//
//    if (fRate >= 1.0f)
//    {
//        // 데미지 처리를 한다.
//        ProcessDamageGroup();
//        if(m_Param.EndeffectID)
//        {
//            GlobalFunc::CreateEffectBeUpToOption(m_Param.EndeffectID,vArrowPos,m_Param.dwOwner,m_Param.dwTargetID);//!loop 이펙트 쓰지 말것    
//
//            m_Param.EndeffectID=0;
//        }
//        return TRUE;
//    }
//    return FALSE;
//}
//
//
//////////////////////////////////////////////////////////////////////////////////////////
//// 베지어 곡선을 그리는 발사체
//cProjectileBezier::cProjectileBezier()
//{
//
//}
//
//BOOL cProjectileBezier::OnProcessProjectile( DWORD dwTick )
//{
//    St_Camera * pCameraInfo = g_pSunRenderer->GetCameraInfo();
//
//    WzVector vTo    = pCameraInfo->m_wvTo;
//    WzVector vRight = pCameraInfo->m_wvRight;
//    WzVector vDown  = -pCameraInfo->m_wvUp;
//    WzVector vDir   = pCameraInfo->m_wvDir;
//
//    assert(m_Param.dwMoveTime);
//
//    UpdateTargetPosition();
//
//    float fRate;
//
//    if(m_Param.dwMoveTime)
//        fRate= (float)m_dwDelay / (float) m_Param.dwMoveTime;		
//    else
//        fRate=0.0f;
//
//    float fTrailRate = 1.0f - fRate; 
//    float fEndRate;
//
//    fRate *= m_Param.fSpeed;
//    fEndRate = fRate - m_Param.fTrailLength;
//
//    if (fRate >= 1.0f)
//    {
//        fRate = 1.0f;
//    }
//
//    if (fEndRate <= 0.0f)
//    {
//        fEndRate = 0.0f;
//    }
//
//    if (fEndRate >= 1.0f)
//    {
//        fEndRate = 1.0f;
//    }
//
//    WzVector vArrowPos;
//    WzVector vDest,vTemp;
//    float fArrowAngleZ,fArrowAngleX;
//
//    //베지어 기저식
//    vArrowPos = m_Param.vStartPos *((1.0f - fRate)*(1.0f - fRate)*(1.0f - fRate)) +
//        m_Param.BEZIER.vPos0*fRate*3*((1.0f - fRate)*(1.0f - fRate))+
//        m_Param.BEZIER.vPos1*3*fRate*fRate*(1.0f - fRate)+
//        +m_Param.vTargetPos * (fRate*fRate*fRate);
//
//    float fNextRate=fRate+0.1f;
//    min(fNextRate,1.0f);
//
//    WzVector vNextArrowPos= m_Param.vStartPos *((1.0f - fNextRate)*(1.0f - fNextRate)*(1.0f - fNextRate)) +
//        m_Param.BEZIER.vPos0*fNextRate*3*((1.0f - fNextRate)*(1.0f - fNextRate))+
//        m_Param.BEZIER.vPos1*3*fNextRate*fNextRate*(1.0f - fNextRate)+
//        +m_Param.vTargetPos * (fNextRate*fNextRate*fNextRate);
//
//    vDest = vNextArrowPos - vArrowPos;//이전 위치에서 현재 위치로 계산하자.
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
//    fArrowAngleX = -atan2(vDest.z,fLength);			//최종 목적지가 아닌 현재 목적지로 꺽자
//    fArrowAngleZ = Math_GetAngleFromVector(&vDest); //최종 목적지가 아닌 현재 목적지로 꺽자
//
//    WzVector vCurPos,vCurPos2,vDestPos,vDestPos2,vNewPos,vNewPos2,vNewEndPos,vNewEndPos2;
//    vCurPos = vCurPos2 = m_Param.vStartPos;
//    vDestPos = vDestPos2 = m_Param.vTargetPos;
//
//    VectorNormalize(&vDest,&vDest);
//    WzVector vDirection;
//
//    VectorCrossProduct(&vDirection,&vDest,&vDir);
//    VectorNormalize(&vDirection,&vDirection);
//
//    WzVector CamPos = pCameraInfo->m_wvFrom;
//    WzVector BeamSrc = m_Param.vStartPos;
//    WzVector BeamDst = m_Param.vTargetPos;
//
//    WzVector vBeamDir = BeamDst - BeamSrc;
//    VectorNormalize(&vBeamDir,&vBeamDir);
//    WzVector vBeamToCam = CamPos - BeamSrc;
//    VectorNormalize(&vBeamToCam,&vBeamToCam);		
//    WzVector vBeamUp;
//    vBeamUp.x = 0.0f;
//    vBeamUp.y = 0.0f;
//    vBeamUp.z = 1.0f;
//
//    WzVector vCurrDir = vBeamToCam - vBeamDir;
//
//    float fAngle = acos(VectorDotProduct(&vBeamUp,&vCurrDir));
//
//    if (vCurrDir.x < 0)
//        fAngle = fAngle;
//    else if (vCurrDir.x > 0)
//        fAngle = -fAngle;
//    else
//        fAngle = 0;
//
//    WzMatrix matrix;
//
//    VectorNormalize(&vCurrDir,&vCurrDir);
//    MatrixRotation(&matrix,&vCurrDir,fAngle);
//
//    vTemp.x = 0.0f;
//    vTemp.y = 0.0f;
//    vTemp.z = 1.0f;
//
//    float fHeight = m_Param.fHeight * fTrailRate;
//
//    vCurPos = vCurPos + vDirection * (fHeight / 2.0f);
//    vCurPos2 = vCurPos2 - vDirection * (fHeight / 2.0f);
//
//    vDestPos = vDestPos + vDirection * (fHeight / 2.0f);
//    vDestPos2 = vDestPos2 - vDirection * (fHeight / 2.0f);
//
//    vNewPos = vCurPos * (1.0f - fRate) + vDestPos * (fRate);
//    vNewPos2 = vCurPos2 * (1.0f - fRate) + vDestPos2 * (fRate);
//
//    vNewEndPos = vCurPos * (1.0f - fEndRate) + vDestPos * (fEndRate);
//    vNewEndPos2 = vCurPos2 * (1.0f - fEndRate) + vDestPos2 * (fEndRate);
//
//    if(m_pMesh)
//    {
//        m_pMesh->m_pVertices[0] = vNewEndPos;
//        m_pMesh->m_pVertices[1] = vNewPos;
//        m_pMesh->m_pVertices[2] = vNewEndPos2;
//        m_pMesh->m_pVertices[3] = vNewPos;
//        m_pMesh->m_pVertices[4] = vNewPos2;
//        m_pMesh->m_pVertices[5] = vNewEndPos2;
//
//        WzVector vArrowDir = vDestPos - vCurPos;
//
//        VectorNormalize(&vArrowDir,&vArrowDir);
//
//        WzVector vTrailOffset = vArrowDir * -0.3f;
//
//        for (int a = 0; a < 6; a++)
//        {
//            m_pMesh->m_pVertices[a] = m_pMesh->m_pVertices[a] + vTrailOffset;
//        }
//    }
//
//    if (m_pUnitDraw) 
//    {
//        m_pUnitDraw->SetPosition(vArrowPos,FALSE);
//        m_pUnitDraw->SetRotation(fArrowAngleX,0,fArrowAngleZ);
//    }
//
//    m_dwDelay += dwTick;
//
//    if (m_hEffect != INVALID_HANDLE_VALUE)
//    {
//        CEffectBody *pBody = g_pEffectManager->GetEffectPtrHandle(m_hEffect);
//        if(pBody)
//        {
//            WzVector vRot;
//            vRot.x = fArrowAngleX;
//            vRot.y = 0.0f;
//            vRot.z = fArrowAngleZ;
//            WzVector vScale;
//            vScale.x = 1.0f;
//            vScale.y = 1.0f;
//            vScale.z = 1.0f;
//
//            if(m_Param.vScale != c_ZeroVector)
//            {
//                vScale=m_Param.vScale;
//            }
//            pBody->SetResultMatrix(vArrowPos,vRot,vScale);
//
//            if(m_Param.bFadeOut)
//            {
//                DWORD alphaTime=m_Param.dwLifeTime/2;
//                if(!alphaTime)
//                    alphaTime=1;
//
//                DWORD startAlphaTime=m_Param.dwLifeTime-alphaTime;
//
//                if(m_dwDelay>=startAlphaTime)
//                {
//                    WzColor color=pBody->GetColor();
//
//                    float fRate = (float)(m_dwDelay-startAlphaTime) / (float) alphaTime;		
//
//                    float alphaRate=1.0f-fRate;
//
//                    float fAlpha=Alpha_WzColor(color);
//
//                    if(alphaRate<0.0f)
//                    {
//                        alphaRate=0.0f;
//                    }
//
//                    if(alphaRate>1.0f)
//                    {
//                        alphaRate=1.0f;
//                    }
//
//                    BYTE Alpha = (BYTE)(255 * alphaRate);
//                    color = WzColor_RGBA(Red_WzColor(color),Green_WzColor(color),Blue_WzColor(color),Alpha);
//                    pBody->SetColor(color);
//                }
//            }
//        }
//    }
//
//    m_bUpdated = TRUE;
//
//    // 발사체가 적에게 닿았으면	
//    if(fRate >= 1.0f)
//    {
//        if (m_Param.bCameraShake)
//        {
//            m_Param.bCameraShake=FALSE;
//            g_Camera.SetVibeType(Camera::VIBE_TYPE_VERTICAL);
//            g_Camera.StartVibration(50,0.07f);
//        }
//
//        ProcessDamageGroup();
//    }
//
//    // 라이프 타임이 있을 경우엔 대체 한다.
//    if(m_Param.dwLifeTime)
//    {
//        fRate = (float)m_dwDelay / (float) m_Param.dwLifeTime;		
//    }
//
//    //데미지 시간이 있을경우는 바로 데미지 처리 
//    if(m_Param.dwDamageTime)
//    {
//        if(m_dwDelay>m_Param.dwDamageTime)
//        {
//            //데미지가 처리 되었으면
//            if(ProcessDamageGroup())
//            {
//                m_Param.dwDamageTime = 0;
//            }
//        }
//    }
//
//
//    if (fRate >= 1.0f)
//    {
//        // 데미지 처리를 한다.
//        ProcessDamageGroup();
//        if(m_Param.EndeffectID)
//        {
//            GlobalFunc::CreateEffectBeUpToOption(m_Param.EndeffectID,vArrowPos,m_Param.dwOwner,m_Param.dwTargetID);//!loop 이펙트 쓰지 말것    
//            m_Param.EndeffectID=0;
//        }
//        return TRUE;
//    }
//    return FALSE;
//}
//
//
//////////////////////////////////////////////////////////////////////////////////////////
//cProjectileTroch::cProjectileTroch()
//{
//
//}
//
//BOOL cProjectileTroch::OnProcessProjectile( DWORD dwTick )
//{
//    St_Camera* pCameraInfo = g_pSunRenderer->GetCameraInfo();
//
//    WzVector vTo    = pCameraInfo->m_wvTo;
//    WzVector vRight = pCameraInfo->m_wvRight;
//    WzVector vDown  = -pCameraInfo->m_wvUp;
//    WzVector vDir   = pCameraInfo->m_wvDir;
//
//    assert(m_Param.dwMoveTime);
//
//    UpdateTargetPosition();
//
//    float fRate;
//
//    if(m_Param.dwMoveTime)
//        fRate= (float)m_dwDelay / (float) m_Param.dwMoveTime;		
//    else
//        fRate=0.0f;
//
//    float fTrailRate = 1.0f - fRate; 
//    float fEndRate;
//
//
//    fRate *= m_Param.fSpeed;
//    fEndRate = fRate - m_Param.fTrailLength;
//
//    if (fRate >= 1.0f)
//    {
//        fRate = 1.0f;
//    }
//
//    if (fEndRate <= 0.0f)
//    {
//        fEndRate = 0.0f;
//    }
//
//    if (fEndRate >= 1.0f)
//    {
//        fEndRate = 1.0f;
//    }
//
//    WzVector vArrowPos;
//    WzVector vDest,vTemp;
//    float fArrowAngleZ,fArrowAngleX;
//
//    //베지어 기저식
//    vArrowPos = m_Param.vStartPos *((1.0f - fRate)*(1.0f - fRate)*(1.0f - fRate)) +
//        m_Param.BEZIER.vPos0*fRate*3*((1.0f - fRate)*(1.0f - fRate))+
//        m_Param.BEZIER.vPos1*3*fRate*fRate*(1.0f - fRate)+
//        +m_Param.vTargetPos * (fRate*fRate*fRate);
//
//    float fNextRate=fRate+0.1f;
//    min(fNextRate,1.0f);
//
//    WzVector vNextArrowPos= m_Param.vStartPos *((1.0f - fNextRate)*(1.0f - fNextRate)*(1.0f - fNextRate)) +
//        m_Param.BEZIER.vPos0*fNextRate*3*((1.0f - fNextRate)*(1.0f - fNextRate))+
//        m_Param.BEZIER.vPos1*3*fNextRate*fNextRate*(1.0f - fNextRate)+
//        +m_Param.vTargetPos * (fNextRate*fNextRate*fNextRate);
//
//    vDest = vNextArrowPos - vArrowPos;//이전 위치에서 현재 위치로 계산하자.
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
//    fArrowAngleX = -atan2(vDest.z,fLength);			//최종 목적지가 아닌 현재 목적지로 꺽자
//    fArrowAngleZ = Math_GetAngleFromVector(&vDest); //최종 목적지가 아닌 현재 목적지로 꺽자
//
//    WzVector vCurPos,vCurPos2,vDestPos,vDestPos2,vNewPos,vNewPos2,vNewEndPos,vNewEndPos2;
//    vCurPos = vCurPos2 = m_Param.vStartPos;
//    vDestPos = vDestPos2 = m_Param.vTargetPos;
//
//    VectorNormalize(&vDest,&vDest);
//    WzVector vDirection;
//
//    VectorCrossProduct(&vDirection,&vDest,&vDir);
//    VectorNormalize(&vDirection,&vDirection);
//
//    WzVector CamPos = pCameraInfo->m_wvFrom;
//    WzVector BeamSrc = m_Param.vStartPos;
//    WzVector BeamDst = m_Param.vTargetPos;
//
//    WzVector vBeamDir = BeamDst - BeamSrc;
//    VectorNormalize(&vBeamDir,&vBeamDir);
//    WzVector vBeamToCam = CamPos - BeamSrc;
//    VectorNormalize(&vBeamToCam,&vBeamToCam);		
//    WzVector vBeamUp;
//    vBeamUp.x = 0.0f;
//    vBeamUp.y = 0.0f;
//    vBeamUp.z = 1.0f;
//
//    WzVector vCurrDir = vBeamToCam - vBeamDir;
//
//    float fAngle = acos(VectorDotProduct(&vBeamUp,&vCurrDir));
//
//    if (vCurrDir.x < 0)
//        fAngle = fAngle;
//    else if (vCurrDir.x > 0)
//        fAngle = -fAngle;
//    else
//        fAngle = 0;
//
//    WzMatrix matrix;
//
//    VectorNormalize(&vCurrDir,&vCurrDir);
//    MatrixRotation(&matrix,&vCurrDir,fAngle);
//
//    vTemp.x = 0.0f;
//    vTemp.y = 0.0f;
//    vTemp.z = 1.0f;
//
//    float fHeight = m_Param.fHeight * fTrailRate;
//
//    vCurPos = vCurPos + vDirection * (fHeight / 2.0f);
//    vCurPos2 = vCurPos2 - vDirection * (fHeight / 2.0f);
//
//    vDestPos = vDestPos + vDirection * (fHeight / 2.0f);
//    vDestPos2 = vDestPos2 - vDirection * (fHeight / 2.0f);
//
//    vNewPos = vCurPos * (1.0f - fRate) + vDestPos * (fRate);
//    vNewPos2 = vCurPos2 * (1.0f - fRate) + vDestPos2 * (fRate);
//
//    vNewEndPos = vCurPos * (1.0f - fEndRate) + vDestPos * (fEndRate);
//    vNewEndPos2 = vCurPos2 * (1.0f - fEndRate) + vDestPos2 * (fEndRate);
//
//    if(m_pMesh)
//    {
//        m_pMesh->m_pVertices[0] = vNewEndPos;
//        m_pMesh->m_pVertices[1] = vNewPos;
//        m_pMesh->m_pVertices[2] = vNewEndPos2;
//        m_pMesh->m_pVertices[3] = vNewPos;
//        m_pMesh->m_pVertices[4] = vNewPos2;
//        m_pMesh->m_pVertices[5] = vNewEndPos2;
//
//        WzVector vArrowDir = vDestPos - vCurPos;
//
//        VectorNormalize(&vArrowDir,&vArrowDir);
//
//        WzVector vTrailOffset = vArrowDir * -0.3f;
//
//        for (int a = 0; a < 6; a++)
//        {
//            m_pMesh->m_pVertices[a] = m_pMesh->m_pVertices[a] + vTrailOffset;
//        }
//    }
//
//    if (m_pUnitDraw) 
//    {
//        m_pUnitDraw->SetPosition(vArrowPos,FALSE);
//        m_pUnitDraw->SetRotation(fArrowAngleX,0,fArrowAngleZ);
//    }
//
//    m_dwDelay += dwTick;
//
//    if (m_hEffect != INVALID_HANDLE_VALUE)
//    {
//        CEffectBody *pBody = g_pEffectManager->GetEffectPtrHandle(m_hEffect);
//        if(pBody)
//        {
//            WzVector vRot;
//            vRot.x = fArrowAngleX;
//            vRot.y = 0.0f;
//            vRot.z = fArrowAngleZ;
//            WzVector vScale;
//            vScale.x = 1.0f;
//            vScale.y = 1.0f;
//            vScale.z = 1.0f;
//
//            if(m_Param.vScale != c_ZeroVector)
//            {
//                vScale=m_Param.vScale;
//            }
//            pBody->SetResultMatrix(vArrowPos,vRot,vScale);
//
//            if(m_Param.bFadeOut)
//            {
//                DWORD alphaTime=m_Param.dwLifeTime/2;
//                if(!alphaTime)
//                    alphaTime=1;
//
//                DWORD startAlphaTime=m_Param.dwLifeTime-alphaTime;
//
//                if(m_dwDelay>=startAlphaTime)
//                {
//                    WzColor color=pBody->GetColor();
//
//                    float fRate = (float)(m_dwDelay-startAlphaTime) / (float) alphaTime;		
//
//                    float alphaRate=1.0f-fRate;
//
//                    float fAlpha=Alpha_WzColor(color);
//
//                    if(alphaRate<0.0f)
//                    {
//                        alphaRate=0.0f;
//                    }
//
//                    if(alphaRate>1.0f)
//                    {
//                        alphaRate=1.0f;
//                    }
//
//                    BYTE Alpha = (BYTE)(255 * alphaRate);
//                    color = WzColor_RGBA(Red_WzColor(color),Green_WzColor(color),Blue_WzColor(color),Alpha);
//                    pBody->SetColor(color);
//                }
//            }
//        }
//    }
//
//    m_bUpdated = TRUE;
//
//    // 발사체가 적에게 닿았으면	
//    if(fRate >= 1.0f)
//    {
//        if (m_Param.bCameraShake)
//        {
//            m_Param.bCameraShake=FALSE;
//            g_Camera.SetVibeType(Camera::VIBE_TYPE_VERTICAL);
//            g_Camera.StartVibration(50,0.07f);
//        }
//
//        ProcessDamageGroup();
//    }
//
//    // 라이프 타임이 있을 경우엔 대체 한다.
//    if(m_Param.dwLifeTime)
//    {
//        fRate = (float)m_dwDelay / (float) m_Param.dwLifeTime;		
//    }
//
//    //데미지 시간이 있을경우는 바로 데미지 처리 
//    if(m_Param.dwDamageTime)
//    {
//        if(m_dwDelay>m_Param.dwDamageTime)
//        {
//            //데미지가 처리 되었으면
//            if(ProcessDamageGroup())
//            {
//                m_Param.dwDamageTime = 0;
//            }
//        }
//    }
//
//
//    if (fRate >= 1.0f)
//    {
//        // 데미지 처리를 한다.
//        ProcessDamageGroup();
//        if(m_Param.EndeffectID)
//        {
//            GlobalFunc::CreateEffectBeUpToOption(m_Param.EndeffectID,vArrowPos,m_Param.dwOwner,m_Param.dwTargetID);//!loop 이펙트 쓰지 말것    
//            m_Param.EndeffectID=0;
//        }
//
//        if( g_pHero->GetObjectKey() == m_Param.dwOwner )
//        {
//            GlobalFunc::NET_SEND_CG_ITEM_USE_SKILL_ACTION_SYN(eITEMTYPE_TORCH, vArrowPos);
//            m_Param.dwOwner =0;
//
//            // 횃불 지속 이펙트 나오기 전에 4방향으로 타는 이펙트 연출
//            float fperRotAngle = 1.57f;
//            for( int i = 0; i < 4 ; i++ )
//            {
//                WzMatrix wmRot;
//                MatrixIdentity(&wmRot);
//                MatrixRotationZ(&wmRot, fperRotAngle * (float) i);
//
//                WzVector wvTransform;
//                WzVector wvOffset;
//                wvOffset.x = 2.0f;
//                wvOffset.y = 2.0f;
//                wvOffset.z = 0.0f;
//                VectorTransformCoord(&wvTransform, &wvOffset,&wmRot);
//
//                WzVector wvEffectPos;
//                wvEffectPos.x = wvTransform.x + vArrowPos.x;
//                wvEffectPos.y = wvTransform.y + vArrowPos.y;
//                wvEffectPos.z = wvTransform.z + vArrowPos.z;
//
//                WzVector wvRot;
//                wvRot.x = 0.0f;
//                wvRot.y = 0.0f;
//                wvRot.z = fperRotAngle * (float) i;
//
//                WzVector wvScale;
//                wvScale.x = 1.0f;
//                wvScale.y = 1.0f;
//                wvScale.z = 1.0f;
//                GlobalFunc::CreateEffectBeUpToOption(StrToWzID("E310"), wvEffectPos,wvScale,wvRot);
//            }			
//
//            GlobalFunc::CreateEffectBeUpToOption(StrToWzID("E310"), vArrowPos);
//
//
//        }
//        return TRUE;
//    }
//    return FALSE;
//}
//
//
//////////////////////////////////////////////////////////////////////////////////////////
//cProjectilePoisonDagger::cProjectilePoisonDagger()
//{
//    SetTraceType( nProjectile::TRACE_TYPE_TARGET_ARROW_POSITION );
//}
//
//BOOL cProjectilePoisonDagger::OnProcessProjectile( DWORD dwTick )
//{
//    St_Camera * pCameraInfo = g_pSunRenderer->GetCameraInfo();
//
//    assert(m_Param.dwMoveTime);
//
//    UpdateTargetPosition();
//
//    float fRate;
//
//    if(m_Param.dwMoveTime)
//        fRate= (float)m_dwDelay / (float) m_Param.dwMoveTime;		
//    else
//        fRate=0.0f;
//
//    float fTrailRate = 1.0f - fRate; 
//    float fEndRate;
//
//    fRate *= m_Param.fSpeed;
//    fEndRate = fRate - m_Param.fTrailLength;
//
//    if (fRate >= 1.0f)
//    {
//        fRate = 1.0f;
//    }
//
//    if (fEndRate <= 0.0f)
//    {
//        fEndRate = 0.0f;
//    }
//
//    if (fEndRate >= 1.0f)
//    {
//        fEndRate = 1.0f;
//    }
//
//    WzVector vArrowPos;
//    WzVector vDest,vTemp;
//    float fArrowAngleZ,fArrowAngleX;
//
//    WzVector vPos0,vPos1,vPos2;
//
//    if(m_Param.POISON_DAGGER.bReturn)
//    {
//        vPos0=m_Param.vTargetPos;
//        vPos1=m_Param.POISON_DAGGER.vCtrl1;
//        vPos2=m_Param.vStartPos;
//    }
//    else
//    {
//        vPos0=m_Param.vStartPos;
//        vPos1=m_Param.POISON_DAGGER.vCtrl0;
//        vPos2=m_Param.vTargetPos;
//    }
//
//    float u_1=fRate;
//    float u_2=fRate*fRate;
//    float u_3=u_2*u_1;
//
//    //조절점 1개인 베지어 기저식
//    float mu = fRate;
//    vArrowPos= (vPos0 * (1 - mu) * (1 - mu) +   vPos1* (1 - mu) * mu*2 + vPos2 * mu * mu);
//
//    float fNextRate=fRate+0.1f;
//    min(fNextRate,1.0f);
//
//    u_1=fNextRate;
//    u_2=fNextRate*fNextRate;
//    u_3=fNextRate*u_1;
//
//    mu = fNextRate;
//    WzVector vNextArrowPos= (vPos0 * (1 - mu) * (1 - mu) +   vPos1* (1 - mu) * mu*2 + vPos2 * mu * mu);
//
//    vDest = vNextArrowPos - vArrowPos;//이전 위치에서 현재 위치로 계산하자.
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
//    fArrowAngleX = -atan2(vDest.z,fLength);			//최종 목적지가 아닌 현재 목적지로 꺽자
//    fArrowAngleZ = Math_GetAngleFromVector(&vDest); //최종 목적지가 아닌 현재 목적지로 꺽자
//
//    if(m_pUnitDraw)
//    {
//        m_fAngleZ+=(WZ_PI/180.0f*dwTick);
//        ClampAngle(&m_fAngleZ);
//        m_pUnitDraw->SetPosition(vArrowPos,FALSE);
//        m_pUnitDraw->SetRotation(0.0f,0.0f,m_fAngleZ);
//        m_pUnitDraw->SetScale(1.5f,1.5f,1.5f);
//    }
//
//    m_dwDelay += dwTick;
//
//    if (m_hEffect != INVALID_HANDLE_VALUE)
//    {
//        CEffectBody *pBody = g_pEffectManager->GetEffectPtrHandle(m_hEffect);
//        if(pBody)
//        {
//            WzVector vRot;
//            SetVector(&vRot,fArrowAngleX,0.0f,fArrowAngleZ);
//
//            WzVector vScale;
//            SetVector(&vScale,1.0f,1.0f,1.0f);
//
//            if(m_Param.vScale != c_ZeroVector)
//            {
//                vScale=m_Param.vScale;
//            }
//            pBody->SetResultMatrix(vArrowPos,vRot,vScale);
//        }
//    }
//
//    m_bUpdated = TRUE;
//
//    // 발사체가 적에게 닿았으면	
//    if(fRate >= 1.0f)
//    {
//        if(!m_Param.POISON_DAGGER.bReturn)
//        {	
//            //행위주체가 주인공일때는 흔들어준다.
//            if (m_Param.dwOwner == g_pHero->GetObjectKey())
//            {
//                m_Param.bCameraShake=FALSE;
//                g_Camera.SetVibeType(Camera::VIBE_TYPE_VERTICAL);
//                g_Camera.StartVibration(100,0.07f);
//            }
//            //!loop 이펙트 쓰지 말것
//            GlobalFunc::CreateEffectBeUpToOption(StrToWzID("K849"),vArrowPos,m_Param.dwOwner,m_Param.dwTargetID);
//            ProcessDamageGroup();
//            m_Param.POISON_DAGGER.bReturn = TRUE;
//            m_Param.dwMoveTime = m_Param.POISON_DAGGER.dwSecondTime;
//
//            m_dwDelay = 0;
//            fRate = 0.0f;
//            return FALSE;
//
//        }
//
//    }
//
//    // 라이프 타임이 있을 경우엔 대체 한다.
//    if(m_Param.dwLifeTime)
//    {
//        fRate = (float)m_dwDelay / (float) m_Param.dwLifeTime;		
//    }
//
//    //데미지 시간이 있을경우는 바로 데미지 처리 
//    if(m_Param.dwDamageTime)
//    {
//        if(m_dwDelay>m_Param.dwDamageTime && m_Param.POISON_DAGGER.bReturn)
//        {
//            //데미지가 처리 되었으면
//            if(ProcessDamageGroup())
//            {
//                m_Param.dwDamageTime = 0;
//            }
//        }
//    }
//
//
//    if (fRate >= 1.0f && m_Param.POISON_DAGGER.bReturn)
//    {
//        // 데미지 처리를 한다.
//        ProcessDamageGroup();
//        if(m_Param.EndeffectID)
//        {
//            GlobalFunc::CreateEffectBeUpToOption(m_Param.EndeffectID,vArrowPos,m_Param.dwOwner,m_Param.dwTargetID);//!loop 이펙트 쓰지 말것    
//            m_Param.EndeffectID=0;
//        }
//        return TRUE;
//    }
//    return FALSE;
//}
//
//////////////////////////////////////////////////////////////////////////////////////////
//cProjectileAirBlow::cProjectileAirBlow()
//{
//    SetTraceType( nProjectile::TRACE_TYPE_TARGET_TOP_POSITION );
//}
//
//BOOL cProjectileAirBlow::OnProcessProjectile( DWORD dwTick )
//{
//    UpdateTargetPosition();
//
//    St_Camera * pCameraInfo = g_pSunRenderer->GetCameraInfo();
//
//    WzVector vTo    = pCameraInfo->m_wvTo;
//    WzVector vRight = pCameraInfo->m_wvRight;
//    WzVector vDown  = -pCameraInfo->m_wvUp;
//    WzVector vDir   = pCameraInfo->m_wvDir;
//
//    assert(m_Param.dwMoveTime);
//
//    float fRate;
//
//    if(m_Param.dwMoveTime)
//        fRate= (float)m_dwDelay / (float) m_Param.dwMoveTime;		
//    else
//        fRate=0.0f;
//
//    float fTrailRate = 1.0f - fRate; 
//    float fEndRate;
//
//    fRate *= m_Param.fSpeed;
//    fEndRate = fRate - m_Param.fTrailLength;
//
//    if (fRate >= 1.0f)
//    {
//        fRate = 1.0f;
//    }
//
//    if (fEndRate <= 0.0f)
//    {
//        fEndRate = 0.0f;
//    }
//
//    if (fEndRate >= 1.0f)
//    {
//        fEndRate = 1.0f;
//    }
//
//    WzVector vArrowPos;
//    WzVector vDest,vTemp;
//    float fArrowAngleZ,fArrowAngleX;
//
//    vDest =  m_Param.vTargetPos - m_Param.vStartPos;
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
//    vArrowPos = m_Param.vStartPos * (1.0f - fRate) + m_Param.vTargetPos * (fRate);
//
//    WzVector vCurPos,vCurPos2,vDestPos,vDestPos2,vNewPos,vNewPos2,vNewEndPos,vNewEndPos2;
//    vCurPos = vCurPos2 = m_Param.vStartPos;
//    vDestPos = vDestPos2 = m_Param.vTargetPos;
//
//    VectorNormalize(&vDest,&vDest);
//    WzVector vDirection;
//
//    VectorCrossProduct(&vDirection,&vDest,&vDir);
//    VectorNormalize(&vDirection,&vDirection);
//
//    WzVector CamPos = pCameraInfo->m_wvFrom;
//    WzVector BeamSrc = m_Param.vStartPos;
//    WzVector BeamDst = m_Param.vTargetPos;
//
//    WzVector vBeamDir = BeamDst - BeamSrc;
//    VectorNormalize(&vBeamDir,&vBeamDir);
//    WzVector vBeamToCam = CamPos - BeamSrc;
//    VectorNormalize(&vBeamToCam,&vBeamToCam);		
//    WzVector vBeamUp;
//    vBeamUp.x = 0.0f;
//    vBeamUp.y = 0.0f;
//    vBeamUp.z = 1.0f;
//
//    WzVector vCurrDir = vBeamToCam - vBeamDir;
//
//    float fAngle = acos(VectorDotProduct(&vBeamUp,&vCurrDir));
//
//    if (vCurrDir.x < 0)
//        fAngle = fAngle;
//    else if (vCurrDir.x > 0)
//        fAngle = -fAngle;
//    else
//        fAngle = 0;
//
//    WzMatrix matrix;
//
//    VectorNormalize(&vCurrDir,&vCurrDir);
//    MatrixRotation(&matrix,&vCurrDir,fAngle);
//
//    vTemp.x = 0.0f;
//    vTemp.y = 0.0f;
//    vTemp.z = 1.0f;
//
//    float fHeight = m_Param.fHeight * fTrailRate;
//
//    vCurPos = vCurPos + vDirection * (fHeight / 2.0f);
//    vCurPos2 = vCurPos2 - vDirection * (fHeight / 2.0f);
//
//    vDestPos = vDestPos + vDirection * (fHeight / 2.0f);
//    vDestPos2 = vDestPos2 - vDirection * (fHeight / 2.0f);
//
//    vNewPos = vCurPos * (1.0f - fRate) + vDestPos * (fRate);
//    vNewPos2 = vCurPos2 * (1.0f - fRate) + vDestPos2 * (fRate);
//
//    vNewEndPos = vCurPos * (1.0f - fEndRate) + vDestPos * (fEndRate);
//    vNewEndPos2 = vCurPos2 * (1.0f - fEndRate) + vDestPos2 * (fEndRate);
//
//
//    if(m_pMesh)
//    {
//        m_pMesh->m_pVertices[0] = vNewEndPos;
//        m_pMesh->m_pVertices[1] = vNewPos;
//        m_pMesh->m_pVertices[2] = vNewEndPos2;
//        m_pMesh->m_pVertices[3] = vNewPos;
//        m_pMesh->m_pVertices[4] = vNewPos2;
//        m_pMesh->m_pVertices[5] = vNewEndPos2;
//
//
//
//        WzVector vArrowDir = vDestPos - vCurPos;
//
//        VectorNormalize(&vArrowDir,&vArrowDir);
//
//        WzVector vTrailOffset = vArrowDir * -0.3f;
//
//        for (int a = 0; a < 6; a++)
//        {
//            m_pMesh->m_pVertices[a] = m_pMesh->m_pVertices[a] + vTrailOffset;
//
//        }
//    }
//
//    if (m_pUnitDraw) 
//    {
//        m_pUnitDraw->SetPosition(vArrowPos,FALSE);
//        m_pUnitDraw->SetRotation(fArrowAngleX,0,fArrowAngleZ);
//    }
//
//    m_dwDelay += dwTick;
//
//    if (m_hEffect != INVALID_HANDLE_VALUE)
//    {
//        CEffectBody *pBody = g_pEffectManager->GetEffectPtrHandle(m_hEffect);
//        if(pBody)
//        {
//            WzVector vRot;
//            vRot.x = fArrowAngleX;
//            vRot.y = 0.0f;
//            vRot.z = fArrowAngleZ;
//            WzVector vScale;
//            vScale.x = 1.0f;
//            vScale.y = 1.0f;
//            vScale.z = 1.0f;
//
//            if(m_Param.vScale != c_ZeroVector)
//            {
//                vScale=m_Param.vScale;
//            }
//            pBody->SetResultMatrix(vArrowPos,vRot,vScale);
//
//            if(m_Param.bFadeOut)
//            {
//                DWORD alphaTime=m_Param.dwLifeTime/2;
//                if(!alphaTime)
//                    alphaTime=1;
//
//                DWORD startAlphaTime=m_Param.dwLifeTime-alphaTime;
//
//                if(m_dwDelay>=startAlphaTime)
//                {
//                    WzColor color=pBody->GetColor();
//
//                    float fRate = (float)(m_dwDelay-startAlphaTime) / (float) alphaTime;		
//
//                    float alphaRate=1.0f-fRate;
//
//                    if(alphaRate<0.0f)
//                    {
//                        alphaRate=0.0f;
//                    }
//
//                    if(alphaRate>1.0f)
//                    {
//                        alphaRate=1.0f;
//                    }
//
//                    BYTE Alpha = (BYTE)(255 * alphaRate);
//                    color = WzColor_RGBA(Red_WzColor(color),Green_WzColor(color),Blue_WzColor(color),Alpha);
//                }
//            }
//
//
//        }
//    }
//
//    m_bUpdated = TRUE;
//
//    // 발사체가 적에게 닿았으면	
//    if(fRate >= 1.0f)
//    {
//        if (m_Param.bCameraShake)
//        {
//            m_Param.bCameraShake=FALSE;
//            g_Camera.SetVibeType(Camera::VIBE_TYPE_VERTICAL);
//            g_Camera.StartVibration(50,0.07f);
//        }
//
//        ProcessDamageGroup();
//    }
//
//    // 라이프 타임이 있을 경우엔 대체 한다.
//    if(m_Param.dwLifeTime)
//    {
//        fRate = (float)m_dwDelay / (float) m_Param.dwLifeTime;		
//    }
//
//    //데미지 시간이 있을경우는 바로 데미지 처리 
//    if(m_Param.dwDamageTime)
//    {
//        if(m_dwDelay>m_Param.dwDamageTime)
//        {
//            //데미지가 처리 되었으면
//            if(ProcessDamageGroup())
//            {
//                m_Param.dwDamageTime=0;
//            }
//        }
//    }
//
//
//    Character* pTarget = (Character*)g_ObjectManager.GetObject( m_Param.dwTargetID );
//
//    if (fRate >= 1.0f)
//    {
//        // 데미지 처리를 한다.
//        ProcessDamageGroup();
//        if(m_Param.EndeffectID)
//        {
//            GlobalFunc::CreateEffectBeUpToOption(m_Param.EndeffectID,vArrowPos,m_Param.dwOwner,m_Param.dwTargetID);//!loop 이펙트 쓰지 말것    
//
//            m_Param.EndeffectID=0;
//        }
//
//        if(pTarget&&(pTarget->GetCurState()==STATE::AIR||pTarget->GetCurState()==STATE::GETUP))
//        {
//            pTarget->ToAirInfo(&m_Param.AIR_BLOW.TOAIR);
//        }
//
//        return TRUE;
//    }
//    return FALSE;
//}
//
//////////////////////////////////////////////////////////////////////////////////////////
//cProjectileQuestPortal::cProjectileQuestPortal()
//{
//
//}
//
//BOOL cProjectileQuestPortal::OnProcessProjectile( DWORD dwTick )
//{
//    //자신과 포탈과의 위치를 체크
//    if(!g_pHero) 
//        return TRUE;
//
//    //포탈과의 거리체크
//    WzVector vDistance =  g_pHero->GetVisiblePos()-m_Param.vStartPos;
//    float fDistance = VectorLength(&vDistance);
//
//    //거리가 0.8미터 이내일경우
//    if (fDistance <= 0.8f)
//    {
//        //Client->Server: (퀘스트or미션)포탈에 진입하겠다는SYN
//        MSG_CG_ZONE_EX_QUEST_PORTAL_ENTER_SYN SendPacket;
//        //포탈종류
//        SendPacket.syncType	= MSG_CG_ZONE_EX_QUEST_PORTAL_ENTER_SYN::eSYNC_TEMP_PORTAL;			
//        //포탈키값		
//        SendPacket.portalKey = m_Param.QUEST_POTAL.portalKey;
//        //맵코드
//        SendPacket.mapEnterancePortalIndex	= m_Param.QUEST_POTAL.enterableMapCode;		
//
//#ifdef _LTJ_HERO_INFO_BUGFIX
//        // no operation
//#else
//        //주인공정보저장
//        BattleScene::SaveHero(g_pHero);
//#endif
//        GlobalFunc::SendPacket( CI_GAMESERVERIDX, (BYTE *)&SendPacket, sizeof(SendPacket));
//        return TRUE;
//
//    }
//
//    if (m_hEffect != INVALID_HANDLE_VALUE)
//    {
//        CEffectBody *pBody = g_pEffectManager->GetEffectPtrHandle(m_hEffect);
//        if(pBody)
//        {
//            WzVector vScale;
//            SetVector(&vScale,1.0f,1.0f,1.0f);
//            pBody->SetResultMatrix(m_Param.vStartPos,m_Param.vDir,vScale);		
//        }
//    }
//    return FALSE;
//}
//
//
//////////////////////////////////////////////////////////////////////////////////////////
//cProjectileVitalSuction::cProjectileVitalSuction()
//{
//
//}
//
//BOOL cProjectileVitalSuction::OnProcessProjectile( DWORD dwTick )
//{
//    // 몬스터의 시체에서 구슬이 떠오른후 플레이어에게 흡수되는 연출을 위한 발사체
//
//    Character* pObj = (Character*)g_ObjectManager.GetObject(m_Param.dwTargetID);
//    if(!pObj)
//        return FALSE;
//
//    WzVector vLen = pObj->GetArrowTargetPos() - m_Param.VITAL_SUCTION.vPos;	// 목표와의 상대적 벡터
//    float fLen = VectorLength(&vLen);
//
//    m_dwDelay += dwTick;
//
//    WzVector wvTemp;
//    if(2000 > m_dwDelay)
//    {
//        WzVector wvUp;
//        SetVector(&wvUp, 0.f, 0.f, 1.f);
//        wvTemp = wvUp * (dwTick*m_Param.fSpeed);
//
//        m_Param.VITAL_SUCTION.vPos = m_Param.VITAL_SUCTION.vPos + wvTemp;
//    }
//    else
//    {
//        WzVector axis;	// 회전축
//        VectorCrossProduct( &axis, &m_Param.vDir, &vLen );
//
//        // 회전각 : 적당한 크기의 상수 각을 잡아 돌려준다.
//        float theta = 0.002f * (float)dwTick;
//
//        // 회전변환용 행렬 생성
//        WzMatrix matRotAxis;
//        MatrixRotation( &matRotAxis, &axis, theta );
//
//        if(10.f < fLen)
//        {
//            WzVector vTemp = m_Param.vDir;
//            VectorTransformCoord(&m_Param.vDir, &vTemp, &matRotAxis);
//        }
//        else 
//        {
//            VectorNormalize(&m_Param.vDir, &vLen);
//        }
//
//        wvTemp = m_Param.vDir * (dwTick*m_Param.fSpeed);
//        WzVector wvLenCheck = m_Param.VITAL_SUCTION.vPos + wvTemp;
//        WzVector wvLen1, wvLen2;
//        wvLen1 = wvLenCheck - m_Param.VITAL_SUCTION.vPos;
//        wvLen2 = pObj->GetArrowTargetPos() - m_Param.VITAL_SUCTION.vPos;
//        float fLen1 = VectorLength(&wvLen1);
//        float fLen2 = VectorLength(&wvLen2);
//        if(fLen1 > fLen2)
//            m_Param.VITAL_SUCTION.vPos = pObj->GetArrowTargetPos();
//        else
//            m_Param.VITAL_SUCTION.vPos = m_Param.VITAL_SUCTION.vPos + wvTemp;	
//    }
//
//    if (m_hEffect != INVALID_HANDLE_VALUE)
//    {
//        CEffectBody *pBody = g_pEffectManager->GetEffectPtrHandle(m_hEffect);
//        if(pBody)
//        {
//            WzVector vRot;
//            vRot.x = 0.f;
//            vRot.y = 0.f;
//            vRot.z = 0.f;
//            WzVector vScale;
//            vScale.x = 1.0f;
//            vScale.y = 1.0f;
//            vScale.z = 1.0f;
//
//            pBody->SetResultMatrix(m_Param.VITAL_SUCTION.vPos, vRot, vScale);
//        }
//    }
//
//    //////////////////////////////////////////////////////////////////////////
//    //////////////////////////////////////////////////////////////////////////
//
//    m_bUpdated = TRUE;
//
//    // 발사체가 적에게 닿았으면
//    vLen = pObj->GetArrowTargetPos() - m_Param.VITAL_SUCTION.vPos;	// 목표와의 상대적 벡터
//    fLen = VectorLength(&vLen);
//    if((fLen <= 0.5f || m_dwDelay > m_Param.dwLifeTime) && (2000 < m_dwDelay))
//    {
//        if (m_Param.bCameraShake)
//        {
//            m_Param.bCameraShake=FALSE;
//            g_Camera.SetVibeType(Camera::VIBE_TYPE_VERTICAL);
//            g_Camera.StartVibration(50,0.07f);
//        }
//
//        if(m_Param.EndeffectID)
//        {
//
//            GlobalFunc::CreateEffectBeUpToOption(m_Param.EndeffectID,m_Param.VITAL_SUCTION.vPos,m_Param.dwOwner,m_Param.dwTargetID);//!loop 이펙트 쓰지 말것    
//
//            m_Param.EndeffectID=0;
//        }
//
//        ProcessDamageGroup();
//
//        return TRUE;
//    }
//
//    return FALSE;
//}
//
//////////////////////////////////////////////////////////////////////////////////////////
//cProjectileHPMPSuction::cProjectileHPMPSuction()
//{
//}
//
//BOOL cProjectileHPMPSuction::OnProcessProjectile( DWORD dwTick )
//{
//    m_dwDelay += dwTick;
//
//    //////////////////////////////////////////////////////////////////////////
//    //////////////////////////////////////////////////////////////////////////
//    Character* pPlayer = (Player*)g_ObjectManager.GetObject(m_Param.dwTargetID);
//    Character* pMonster = (Character*)g_ObjectManager.GetObject(m_Param.dwOwner);
//    if(NULL == pPlayer || NULL == pMonster)
//        return TRUE;
//
//    WzVector vLen = pPlayer->GetBonePos(14) - pMonster->GetArrowTargetPos();	// 목표와의 상대적 벡터
//    float fLen = VectorLength(&vLen);
//    WzVector vDir; VectorNormalize(&vDir, &vLen);
//
//    if (m_hEffect != INVALID_HANDLE_VALUE)
//    {
//        CEffectBody *pBody = g_pEffectManager->GetEffectPtrHandle(m_hEffect);
//        if(pBody)
//        {
//#ifdef _YJ_SHADOW_SKILL_S00013
//            pBody->SetAllElementTargetPos(pPlayer->GetBonePos(14));
//#endif //_YJ_SHADOW_SKILL_S00013
//
//            if(m_dwDelay > (m_Param.dwLifeTime - 1000))
//            {
//                pBody->FadeOut();
//            }
//
//            //////////////////////////////////////////////////////////////////////////
//
//            float fTemp = pPlayer->GetBonePos(14).z - pMonster->GetArrowTargetPos().z;
//            WzVector vRot;
//            vRot.x = atan2(fLen, fTemp);
//            vRot.z = Math_GetAngleFromVector(&vDir);
//            vRot.y = 0.f;//0.0f;
//
//            WzVector vScale;
//            vScale.x = 1.0f;
//            vScale.y = 1.0f;
//            vScale.z = 1.0f;
//
//
//            pBody->SetResultMatrix(pMonster->GetArrowTargetPos(), vRot, vScale);
//            //////////////////////////////////////////////////////////////////////////
//        }
//    }
//
//    //////////////////////////////////////////////////////////////////////////
//    //////////////////////////////////////////////////////////////////////////
//
//    m_bUpdated = TRUE;
//
//    if(30.f < fLen)
//        return TRUE;
//
//    STATE::TYPE st = pPlayer->GetCurState();
//    if(STATE::IDLE			== st ||
//        STATE::KEYBOARDMOVE	== st ||
//        STATE::JUMP			== st || 
//        STATE::DEATH			== st ||
//        STATE::DOWN			== st ||
//        STATE::GETUP			== st ||
//        STATE::SIT			== st ||
//        STATE::EMOTICON		== st ||
//        STATE::MOVE			== st ||
//        STATE::KEYBOARDJUMP	== st)
//    {
//        return TRUE;
//    }
//
//    if( m_dwDelay > m_Param.dwLifeTime)
//    {
//        return TRUE;
//    }
//
//    return FALSE;
//}
//
//
//////////////////////////////////////////////////////////////////////////////////////////
//cProjectileSwordWave::cProjectileSwordWave()
//{
//}
//
//BOOL cProjectileSwordWave::OnProcessProjectile( DWORD dwTick )
//{
//    St_Camera * pCameraInfo = g_pSunRenderer->GetCameraInfo();
//
//    WzVector vTo    = pCameraInfo->m_wvTo;
//    WzVector vRight = pCameraInfo->m_wvRight;
//    WzVector vDown  = -pCameraInfo->m_wvUp;
//    WzVector vDir   = pCameraInfo->m_wvDir;	
//
//    assert(m_Param.dwMoveTime);
//
//    float fRate;
//
//    if(m_Param.dwMoveTime)
//        fRate= (float)m_dwDelay / (float) m_Param.dwMoveTime;		
//    else
//        fRate=0.0f;
//
//    float fTrailRate = 1.0f - fRate; 
//    float fEndRate;
//
//    fRate *= m_Param.fSpeed;
//    fEndRate = fRate - m_Param.fTrailLength;
//
//    if (fRate >= 1.0f)
//    {
//        fRate = 1.0f;
//    }
//
//    if (fEndRate <= 0.0f)
//    {
//        fEndRate = 0.0f;
//    }
//
//    if (fEndRate >= 1.0f)
//    {
//        fEndRate = 1.0f;
//    }
//
//    WzVector vArrowPos;
//    WzVector vDest,vTemp;
//    float fArrowAngleZ,fArrowAngleX;
//
//    vDest =  m_Param.vTargetPos - m_Param.vStartPos;
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
//    vArrowPos = m_Param.vStartPos * (1.0f - fRate) + m_Param.vTargetPos * (fRate);
//
//    WzVector vCurPos,vCurPos2,vDestPos,vDestPos2,vNewPos,vNewPos2,vNewEndPos,vNewEndPos2;
//    vCurPos = vCurPos2 = m_Param.vStartPos;
//    vDestPos = vDestPos2 = m_Param.vTargetPos;
//
//    VectorNormalize(&vDest,&vDest);
//    WzVector vDirection;
//
//    VectorCrossProduct(&vDirection,&vDest,&vDir);
//    VectorNormalize(&vDirection,&vDirection);
//
//    WzVector CamPos = pCameraInfo->m_wvFrom;
//    WzVector BeamSrc = m_Param.vStartPos;
//    WzVector BeamDst = m_Param.vTargetPos;
//
//    WzVector vBeamDir = BeamDst - BeamSrc;
//    VectorNormalize(&vBeamDir,&vBeamDir);
//    WzVector vBeamToCam = CamPos - BeamSrc;
//    VectorNormalize(&vBeamToCam,&vBeamToCam);		
//    WzVector vBeamUp;
//    vBeamUp.x = 0.0f;
//    vBeamUp.y = 0.0f;
//    vBeamUp.z = 1.0f;
//
//    WzVector vCurrDir = vBeamToCam - vBeamDir;
//
//    float fAngle = acos(VectorDotProduct(&vBeamUp,&vCurrDir));
//
//    if (vCurrDir.x < 0)
//        fAngle = fAngle;
//    else if (vCurrDir.x > 0)
//        fAngle = -fAngle;
//    else
//        fAngle = 0;
//
//    WzMatrix matrix;
//
//    VectorNormalize(&vCurrDir,&vCurrDir);
//    MatrixRotation(&matrix,&vCurrDir,fAngle);
//
//    vTemp.x = 0.0f;
//    vTemp.y = 0.0f;
//    vTemp.z = 1.0f;
//
//    float fHeight = m_Param.fHeight * fTrailRate;
//
//    vCurPos = vCurPos + vDirection * (fHeight / 2.0f);
//    vCurPos2 = vCurPos2 - vDirection * (fHeight / 2.0f);
//
//    vDestPos = vDestPos + vDirection * (fHeight / 2.0f);
//    vDestPos2 = vDestPos2 - vDirection * (fHeight / 2.0f);
//
//    vNewPos = vCurPos * (1.0f - fRate) + vDestPos * (fRate);
//    vNewPos2 = vCurPos2 * (1.0f - fRate) + vDestPos2 * (fRate);
//
//    vNewEndPos = vCurPos * (1.0f - fEndRate) + vDestPos * (fEndRate);
//    vNewEndPos2 = vCurPos2 * (1.0f - fEndRate) + vDestPos2 * (fEndRate);
//
//    if (m_pUnitDraw) 
//    {
//        m_pUnitDraw->SetPosition(vArrowPos,FALSE);
//        m_pUnitDraw->SetRotation(fArrowAngleX,0,fArrowAngleZ);
//    }
//
//    m_dwDelay += dwTick;
//
//    if (m_hEffect != INVALID_HANDLE_VALUE)
//    {
//        CEffectBody *pBody = g_pEffectManager->GetEffectPtrHandle(m_hEffect);
//        if(pBody)
//        {
//            WzVector vRot;
//            vRot.x = 0;
//            vRot.y = 0.0f;
//            vRot.z = 0;
//            WzVector vScale;
//            vScale.x = 1.0f;
//            vScale.y = 1.0f;
//            vScale.z = 1.0f;
//
//
//            if(m_Param.vScale != c_ZeroVector)
//            {
//                vScale=m_Param.vScale;
//            }
//
//            pBody->SetResultMatrix(vArrowPos,vRot,vScale);
//        }
//    }
//
//    m_bUpdated = TRUE;
//
//    // 라이프 타임이 있을 경우엔 대체 한다.
//    if(m_Param.dwLifeTime)
//    {
//        fRate = (float)m_dwDelay / (float) m_Param.dwLifeTime;		
//    }
//
//    if (fRate >= 1.0f)
//    {
//        // 데미지 처리를 한다.
//
//        return TRUE;
//    }
//    return FALSE;
//}
//
//
//////////////////////////////////////////////////////////////////////////////////////////
//cProjectileSwordTrail::cProjectileSwordTrail()
//{
//}
//
//BOOL cProjectileSwordTrail::OnProcessProjectile( DWORD dwTick )
//{
//    m_dwDelay += dwTick;
//
//    St_Camera * pCameraInfo = g_pSunRenderer->GetCameraInfo();
//
//    WzVector vTo    = pCameraInfo->m_wvTo;
//    WzVector vRight = pCameraInfo->m_wvRight;
//    WzVector vDown  = -pCameraInfo->m_wvUp;
//    WzVector vDir   = pCameraInfo->m_wvDir;
//
//    assert(m_Param.dwMoveTime);
//
//    float fRate;
//
//    if(m_Param.dwMoveTime)
//        fRate= (float)m_dwDelay / (float) m_Param.dwMoveTime;		
//    else
//        fRate=0.0f;
//
//    float fTrailRate = 1.0f - fRate; 
//    float fEndRate;
//
//    fRate *= m_Param.fSpeed;
//    fEndRate = fRate - m_Param.fTrailLength;
//
//    if (fRate >= 1.0f)
//    {
//        fRate = 1.0f;
//    }
//
//    if (fEndRate <= 0.0f)
//    {
//        fEndRate = 0.0f;
//    }
//
//    if (fEndRate >= 1.0f)
//    {
//        fEndRate = 1.0f;
//    }
//
//    WzVector vArrowPos;
//    WzVector vDest,vTemp;
//    float fArrowAngleZ,fArrowAngleX;
//
//    vDest =  m_Param.vTargetPos - m_Param.vStartPos;
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
//    vArrowPos = m_Param.vStartPos * (1.0f - fRate) + m_Param.vTargetPos * (fRate);
//
//    WzVector vCurPos,vCurPos2,vDestPos,vDestPos2,vNewPos,vNewPos2,vNewEndPos,vNewEndPos2;
//    vCurPos = vCurPos2 = m_Param.vStartPos;
//    vDestPos = vDestPos2 = m_Param.vTargetPos;
//
//    VectorNormalize(&vDest,&vDest);
//    WzVector vDirection;
//
//    VectorCrossProduct(&vDirection,&vDest,&vDir);
//    VectorNormalize(&vDirection,&vDirection);
//
//    WzVector CamPos = pCameraInfo->m_wvFrom;
//    WzVector BeamSrc = m_Param.vStartPos;
//    WzVector BeamDst = m_Param.vTargetPos;
//
//    WzVector vBeamDir = BeamDst - BeamSrc;
//    VectorNormalize(&vBeamDir,&vBeamDir);
//    WzVector vBeamToCam = CamPos - BeamSrc;
//    VectorNormalize(&vBeamToCam,&vBeamToCam);		
//    WzVector vBeamUp;
//    vBeamUp.x = 0.0f;
//    vBeamUp.y = 0.0f;
//    vBeamUp.z = 1.0f;
//
//    WzVector vCurrDir = vBeamToCam - vBeamDir;
//
//    float fAngle = acos(VectorDotProduct(&vBeamUp,&vCurrDir));
//
//    if (vCurrDir.x < 0)
//        fAngle = fAngle;
//    else if (vCurrDir.x > 0)
//        fAngle = -fAngle;
//    else
//        fAngle = 0;
//
//    WzMatrix matrix;
//
//    VectorNormalize(&vCurrDir,&vCurrDir);
//    MatrixRotation(&matrix,&vCurrDir,fAngle);
//
//    vTemp.x = 0.0f;
//    vTemp.y = 0.0f;
//    vTemp.z = 1.0f;
//
//    float fHeight = m_Param.fHeight * fTrailRate;
//
//    vCurPos = vCurPos + vDirection * (fHeight / 2.0f);
//    vCurPos2 = vCurPos2 - vDirection * (fHeight / 2.0f);
//
//    vDestPos = vDestPos + vDirection * (fHeight / 2.0f);
//    vDestPos2 = vDestPos2 - vDirection * (fHeight / 2.0f);
//
//    vNewPos = vCurPos * (1.0f - fRate) + vDestPos * (fRate);
//    vNewPos2 = vCurPos2 * (1.0f - fRate) + vDestPos2 * (fRate);
//
//    vNewEndPos = vCurPos * (1.0f - fEndRate) + vDestPos * (fEndRate);
//    vNewEndPos2 = vCurPos2 * (1.0f - fEndRate) + vDestPos2 * (fEndRate);
//
//
//
//
//    m_bUpdated = TRUE;
//
//    CEffectBody *pBody = g_pEffectManager->GetEffectPtrHandle(m_hEffect);
//    if(pBody)
//    {
//        CEElementFlatChain* pFChain = pBody->GetFlatChainPtr();
//        if( pFChain )
//        {
//
//            m_Param.SWORD_TRAIL.vEndPos =vArrowPos;
//            m_Param.SWORD_TRAIL.vStartPos =m_Param.SWORD_TRAIL.vEndPos;
//            m_Param.SWORD_TRAIL.vStartPos+=m_Param.SWORD_TRAIL.vAdditionalStartPos;
//            m_Param.SWORD_TRAIL.vEndPos+=m_Param.SWORD_TRAIL.vAdditionalEndPos;
//
//
//            WzVector dir=m_Param.SWORD_TRAIL.vDir;
//            //Todo:컨버트 함수 만들자 귀찮다.
//            vector3 dir2; 
//            dir2.x=dir.x;
//            dir2.y=dir.z;
//            dir2.z=dir.y;
//            dir2.norm(); //z축
//
//            vector3 up;
//            up.set(0.0f,1.0f,0.0f);//y축
//
//            vector3 temp=up*dir2;//x축
//            WzVector inc;
//            inc.x=temp.x;
//            inc.y=temp.z;
//            inc.z=temp.y;
//
//
//            m_Param.SWORD_TRAIL.vEndPos +=inc;
//            m_Param.SWORD_TRAIL.vStartPos-=inc;
//
//            pFChain->SetPosition(m_Param.SWORD_TRAIL.vStartPos,m_Param.SWORD_TRAIL.vEndPos);
//        }
//
//    }
//
//
//    if (m_Param.SWORD_TRAIL.dwTrailEffectDelay > 0)
//    {
//        if (m_dwDelay >= m_dwTrailEffectCount * m_Param.SWORD_TRAIL.dwTrailEffectDelay)
//        {
//            WzVector vResult;
//
//            g_pMap->GetTerrainHeight(&vArrowPos,&vResult);
//
//
//
//            GlobalFunc::CreateEffectBeUpToOption(StrToWzID("A030"),vResult,m_Param.dwOwner,m_Param.dwTargetID);//!loop 이펙트 쓰지 말것    
//
//
//            m_dwTrailEffectCount++;
//        }
//    }
//
//    // 발사체가 적에게 닿았으면	
//    if(fRate >= 1.0f)
//    {
//        if (m_Param.bCameraShake)
//        {
//            m_Param.bCameraShake=FALSE;
//            g_Camera.SetVibeType(Camera::VIBE_TYPE_VERTICAL);
//            g_Camera.StartVibration(50,0.07f);
//        }
//
//        ProcessDamageGroup();
//    }
//
//    // 라이프 타임이 있을 경우엔 대체 한다.
//    if(m_Param.dwLifeTime)
//    {
//        fRate = (float)m_dwDelay / (float) m_Param.dwLifeTime;		
//    }
//
//    //데미지 시간이 있을경우는 바로 데미지 처리 
//    if(m_Param.dwDamageTime)
//    {
//        if(m_dwDelay>m_Param.dwDamageTime)
//        {
//
//            //데미지가 처리 되었으면
//            if(ProcessDamageGroup())
//            {
//                m_Param.dwDamageTime=0;
//            }
//        }
//    }
//
//    if (fRate >= 1.0f)
//    {
//        // 데미지 처리를 한다.
//        ProcessDamageGroup();
//        return TRUE;
//    }
//
//
//    return FALSE;
//}