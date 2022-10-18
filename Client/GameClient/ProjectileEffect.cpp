#include "SunClientPrecompiledHeader.h"
#include "ProjectileEffect.h"

#include "GlobalFunc.h"
#include "GameFunc.h"
#include "ResourceManager.h"


namespace nProjectileFunc
{
    void GetProjectileBlendColor( float TotalLifeTimeRate, OUT WzColor& Color )
    {
        if( TotalLifeTimeRate < 0.5f )
        {
            // 페이드아웃이 활성화 되어있는 경우에는 기존에 설정된 컬러는 무시됨

            return;
            //Color = 0xFFFFFFFF;
        }
        else
        {
            float fRate = (1.0f - TotalLifeTimeRate) * 2.0f; // 0~1
            if( fRate < 0.0f )
                fRate = 0.0f;
            if( fRate > 1.0f )
                fRate = 1.0f;

            BYTE Alpha = (BYTE)(255 * fRate);
            Color = WzColor_ChangeAlpha(Color, Alpha);
        }
    }
};

cProjectileEffect::cProjectileEffect()
: m_EffectID(0)
, m_hEffect(INVALID_HANDLE_VALUE)
, m_Actived(FALSE)
, m_ActiveDelayTime(0)
, m_EffectScale(c_UnitVector)
, m_StaticRotate(FALSE)
, m_EffectRotate(c_ZeroVector)
, m_FadeOut(FALSE)
{

}


BOOL cProjectileEffect::CreateEffect( WzID EffectID, DWORD CasterObjectKey, DWORD TargetObjectKey )
{
    m_StartedTime = GetCurrentTime();
    m_EffectID = EffectID;
    m_CasterObjectKey = CasterObjectKey;
    m_TargetObjectKey = TargetObjectKey;

    return TRUE;
}


void cProjectileEffect::Process()
{
    if( m_Actived == FALSE )
    {
        if( m_StartedTime + m_ActiveDelayTime <= GetCurrentTime() )
        {
            m_Actived = TRUE;

            if( m_EffectID != 0 )
            {
                if( m_StaticRotate == TRUE )
                {
                    m_hEffect = GlobalFunc::CreateEffectBeUpToOption( m_EffectID, GetEffectPosition(), GetEffectScale(), m_EffectRotate, m_CasterObjectKey, m_TargetObjectKey );
                }
                else
                {
                    m_hEffect = GlobalFunc::CreateEffectBeUpToOption( m_EffectID, GetEffectPosition(), GetEffectScale(), GetEffectRotate(), m_CasterObjectKey, m_TargetObjectKey );
                }
            }
        }
    }


    if( m_Actived == TRUE )
    {
        if( CEffectBody* pBody = g_pEffectManager->GetEffectPtrHandle(m_hEffect) )
        {
            WzColor Color = 0xFFFFFFFF;
            if( m_FadeOut == TRUE )
            {
                nProjectileFunc::GetProjectileBlendColor( GetEffectTotalLifeTimeRate(), Color );
            }
            pBody->SetColor( Color );

            if( m_StaticRotate == TRUE )
            {
                pBody->SetResultMatrix( GetEffectPosition(), m_EffectRotate, GetEffectScale() );
            }
            else
            {
                pBody->SetResultMatrix( GetEffectPosition(), GetEffectRotate(), GetEffectScale() );
            }

        }
    }
}

void cProjectileEffect::Destroy()
{
    if( m_hEffect != INVALID_HANDLE_VALUE )
    {
        g_pEffectManager->RemoveEffectPtr(m_hEffect);
        m_hEffect = INVALID_HANDLE_VALUE;
    }
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
cProjectileUnitObject::cProjectileUnitObject()
: m_pUnitDraw(NULL)
, m_UnitColor(0xFFFFFFFF)
, m_FadeOut(FALSE)
, m_AlphaMode(AB_ALPHA)
, m_StaticUnitRotate(FALSE)
, m_UnitRotate(c_ZeroVector)
, m_NextAnimationTime(0)
, m_NextAnimationID(0)
, m_NextAnimationLoop(FALSE)
, m_CurrentTickTime(0)
, m_ProjectileImageCode(eProjectile_SRC_NONE)
{
    m_Actived = FALSE;
    m_ActiveDelayTime = 0;
    m_StartedTime = GetCurrentTime();


    for (int j = 0; j < MAX_OBJECT_EFFECT; ++j)
    {
        m_UnitEffectParam[j].hEffect = INVALID_HANDLE_VALUE;
        m_UnitEffectParam[j].iBoneIndex = 0;
        m_UnitEffectParam[j].bRotateWithBone = FALSE;
        m_UnitEffectParam[j].bOnlyPosition = FALSE;
        m_UnitEffectParam[j].bRotateWithBone = FALSE;
        m_UnitEffectParam[j].EffectID = 0;
        ZeroMemory(&m_UnitEffectParam[j].quatRot, sizeof(WzQuaternion));
        ZeroMemory(&m_UnitEffectParam[j].vOffset, sizeof(WzVector));
        ZeroMemory(&m_UnitEffectParam[j].vScale, sizeof(WzVector));
    }
}

void cProjectileUnitObject::Render()
{
    if( m_pUnitDraw)
    {
        ENUM_ALPHABLEND OldAlphaMode = g_pSunRenderer->GetAlphaBlendMode();
        g_pSunRenderer->SetAlphaBlendMode( m_AlphaMode, FALSE );

        g_pSunRenderer->SetColor(m_UnitColor);       
        g_pSunRenderer->RenderUnit(m_pUnitDraw);

        g_pSunRenderer->SetAlphaBlendMode( OldAlphaMode, FALSE );
    }
}

void cProjectileUnitObject::Process(DWORD TickTime)
{
    if( m_Actived == FALSE )
    {
        if( m_StartedTime + m_ActiveDelayTime <= GetCurrentTime() )
        {
            m_Actived = TRUE;

            // 첫번쨰 Process 에서 UnitDraw를 생성한다
            if( m_pUnitDraw == NULL && m_ProjectileImageCode != eProjectile_SRC_NONE )
            {
                RESOURCECODE code = (DWORD)m_ProjectileImageCode + ENGINE_RESOURCE_ID_OFFSET;
                m_pUnitDraw = ResourceManager::Instance()->AllocResourceUnitDraw(code); //화살

                if( m_pUnitDraw != NULL )
                {
                    m_pUnitDraw->SetCurrentAnimation( m_AnimationID, m_AnimationLoop );
                }
            }
        }
    }


    if( m_Actived == TRUE )
    {
        m_CurrentTickTime += TickTime;

        // UpdateUnitPosition
        if( m_pUnitDraw)
        {
            m_pUnitDraw->SetPosition( GetUnitObjectPosition(), FALSE );
            m_pUnitDraw->SetRotation( m_StaticUnitRotate ? m_UnitRotate : GetUnitObjectRotate(), TRUE );

            UpdateUnitAnimation( TickTime );
            UpdateUnitEffectPosition( m_pUnitDraw, GetUnitObjectPosition(), m_StaticUnitRotate ? m_UnitRotate : GetUnitObjectRotate() );

            if( IsUnitFadeOut() == TRUE )
            {
                nProjectileFunc::GetProjectileBlendColor( GetUnitObjectLifeTimeRate(), m_UnitColor );
            }

            if( m_NextAnimationTime != 0 )
            {
                if( m_CurrentTickTime >= m_NextAnimationTime )
                {
                    m_NextAnimationTime = 0;
                    SetUnitAnimation( m_NextAnimationID, m_NextAnimationLoop );
                }
            }
        }
    }
}

void cProjectileUnitObject::SetNextUnitAnimation(DWORD ChangeTime, WzID AnimationID, BOOL Loop )
{
    m_NextAnimationTime = ChangeTime;
    m_NextAnimationID = AnimationID; 
    m_NextAnimationLoop = Loop;
}

void cProjectileUnitObject::SetUnitAnimation(WzID AnimationID, BOOL Loop )
{
    m_AnimationID = AnimationID;
    m_AnimationLoop = Loop;

    //에니가 바뀔경우 이펙트 파괴
    DestroyAllUnitEffect();

    if( m_pUnitDraw != NULL )
    {
        m_pUnitDraw->SetCurrentAnimation(AnimationID, Loop);
    }
}

void cProjectileUnitObject::Destroy()
{
    if( m_pUnitDraw != NULL )
    {
		int UnitEffectCount = m_pUnitDraw->GetNumUnitEffectInfo();
		for( int i=0;i<UnitEffectCount;++i)
		{
			WzUnitEffectInfo* pUnitEffectInfo = m_pUnitDraw->GetUnitEffectInfo(i);
			if( pUnitEffectInfo->m_hEffect != INVALID_HANDLE_VALUE )
			{
				if( CEffectBody* pEffectInfo = g_pEffectManager->GetEffectPtrHandle( pUnitEffectInfo->m_hEffect ) )
				{
					pEffectInfo->SetFadeOutEffect( false );
				}

				g_pEffectManager->RemoveEffectPtr( pUnitEffectInfo->m_hEffect );
				pUnitEffectInfo->m_hEffect = INVALID_HANDLE_VALUE;
				pUnitEffectInfo->m_iNode = 0;
			}
		}

        ResourceManager::Instance()->FreeResourceUnitDraw( m_pUnitDraw );
        m_pUnitDraw = NULL;
    }

    DestroyAllUnitEffect();
}

void cProjectileUnitObject::SetUnitCode( eProjectile_SRC ProjectileImageCode )
{
    m_ProjectileImageCode = ProjectileImageCode;
}
//
//BOOL cProjectileUnitObject::CreateUnit( eProjectile_SRC ProjectileImageCode )
//{
//    if( (DWORD)ProjectileImageCode == 0 )
//    {
//        return FALSE;
//    }
//
//    RESOURCECODE code = (DWORD)ProjectileImageCode + ENGINE_RESOURCE_ID_OFFSET;
//    m_pUnitDraw = ResourceManager::Instance()->AllocResourceUnitDraw(code); //화살 
//
//    return m_pUnitDraw != NULL;
//}


void cProjectileUnitObject::UpdateUnitEffectPosition( CWzUnitDraw* pUnitDraw, const WzVector& Position, const WzVector& Direction )
{
    static const int FORCE_TRANSFROM = -2;

    for ( int j = 0; j < MAX_OBJECT_EFFECT; ++j )
    {
        EFFECT_PARAM& EffectParam = m_UnitEffectParam[j];

        if (EffectParam.hEffect != INVALID_HANDLE_VALUE)
        {
            if ( CEffectBody* pBody = g_pEffectManager->GetEffectPtrHandle(EffectParam.hEffect) )
            {
                // 본인스값이 강제 변환일때는 갱신하지않는다
                if (EffectParam.iBoneIndex == FORCE_TRANSFROM)
                {
                    continue;
                }

                WzVector wvPos, wvWorldPos;
                WzMatrix wmPosMat, wmRotMat, wmRot, wmScale, mat;

                wvPos = EffectParam.vOffset;


                if( EffectParam.iBoneIndex == -1 )
                {
                    wvWorldPos = Position;

                    WzVector rotation = {0.f, 0.f, Math_GetAngleFromVector(&Direction)};
                    MatrixRotationZYX(&wmRotMat, &rotation);

                    // 로테이션이 적용된 Offset만큼 이동
                    VectorTransformCoord(&rotation, &EffectParam.vOffset, &wmRotMat);
                    wvWorldPos += rotation;
                }
                else
                {
                    if( GameFunc::GetBoneMatrix( pUnitDraw, EffectParam.iBoneIndex, &mat ) == FALSE )
                    {
                        continue;
                    }

                    VectorTransformCoord( &wvWorldPos, &wvPos, &mat );
                }

                MatrixTranslation( &wmPosMat, wvWorldPos.x, wvWorldPos.y, wvWorldPos.z );


                if(EffectParam.bOnlyPosition)
                {
                    MatrixIdentity( &wmRotMat );
                }
                else if( EffectParam.bRotateWithBone )
                {
                    QuaternionToMatrix( &wmRot, &EffectParam.quatRot );
                    MatrixMultiply( &wmRotMat, &wmRot, &mat );
                    wmRotMat._41 = 0.0f;
                    wmRotMat._42 = 0.0f;
                    wmRotMat._43 = 0.0f;
                }
                else
                {
                    MatrixIdentity( &wmRotMat );

                    WzVector vPos;
                    vPos = wvWorldPos;

                    WzVector vDest,vTemp;
                    vDest = vPos + Direction*10.0f;
                    vDest =  vDest - vPos;
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

                    WzVector vRot;
                    vRot.x = -atan2(vDest.z,fLength);
                    vRot.z = Math_GetAngleFromVector(&vDest);
                    vRot.y = 0.0f;
                    MatrixRotationZYX(&wmRotMat,&vRot);

                }

                MatrixScaling( &wmScale, EffectParam.vScale.x, EffectParam.vScale.y, EffectParam.vScale.z );
                pBody->SetResultMatrix( &wmPosMat, &wmRotMat, &wmScale );

            }
            else
            {
                //자동 NULL셋팅
                EffectParam.hEffect = INVALID_HANDLE_VALUE;
            }
        }
    }
}

BOOL cProjectileUnitObject::UpdateUnitAnimation( DWORD TickTime )
{
    if( m_pUnitDraw != NULL )
    {
        g_pSunRenderer->AnimateUnit( m_pUnitDraw );

        float 	fLoopCheck;
        fLoopCheck = 0.0f;
        WzEvent Event;
        ZeroMemory(&Event,sizeof(Event));
        g_pSunRenderer->ProgressUnitFrame(m_pUnitDraw, ( float )TickTime, &Event, &fLoopCheck );


        //유닛에 딸린 이펙트 생성, 삭제
        if (Event.m_iNumEffect > 0)
        {
            for (int a = 0; a < Event.m_iNumEffect; a++)
            {
                int iIndex = Event.m_Effect[a].m_iEffectType;

                assert(iIndex >= 0 && iIndex < MAX_OBJECT_EFFECT);

                if (iIndex < 0)
                {
                    iIndex = 0;
                }
                else if (iIndex >= MAX_OBJECT_EFFECT)
                {
                    iIndex = MAX_OBJECT_EFFECT - 1;
                }

                if (Event.m_Effect[a].m_byEffectFlag & EFF_DESTROY)
                {
                    DestroyUnitEffect(iIndex);
                }
                else
                {
                    DestroyUnitEffect(iIndex);

                    int boneindex = Event.m_Effect[a].m_iEffectBoneIndex;
                    WzVector vPos;	
                    GameFunc::GetBonePos( m_pUnitDraw, boneindex, &vPos);

                    m_UnitEffectParam[iIndex].hEffect = GlobalFunc::CreateEffectBeUpToOption( Event.m_Effect[a].m_wiEffectWzID, vPos, 0, 0 );
                    m_UnitEffectParam[iIndex].iBoneIndex = boneindex;
                }
            }

        }
    }

    //if( m_StaticRotate == TRUE )
    //{
    //    WzVector vPos = pTargetObject->GetBonePos( m_BoneIndex );
    //    WzMatrix wmPosMat, wmRotMat, wmScale, mat;
    //    mat = pTargetObject->GetBoneMatrix( m_BoneIndex );
    //    wmRotMat = pTargetObject->GetBoneMatrix( m_BoneIndex );
    //    wmRotMat._41 = 0.0f;
    //    wmRotMat._42 = 0.0f;
    //    wmRotMat._43 = 0.0f;

    //    MatrixIdentity( &wmPosMat );
    //    MatrixTranslation( &wmPosMat, vPos.x, vPos.y, vPos.z );

    //    MatrixIdentity( &wmPosMat );
    //    MatrixScaling( &wmScale,1.0f,1.0f,1.0f);

    //    pBody->SetResultMatrix( &wmPosMat, &wmRotMat, &wmScale );

    //}

    return TRUE;
}



void cProjectileUnitObject::DestroyUnitEffect(int index)
{
    assert(index >= 0 && index < MAX_OBJECT_EFFECT);

    if( m_UnitEffectParam[index].hEffect != INVALID_HANDLE_VALUE)
    {
        g_pEffectManager->RemoveEffectPtr(m_UnitEffectParam[index].hEffect);

        m_UnitEffectParam[index].hEffect = INVALID_HANDLE_VALUE;
        m_UnitEffectParam[index].iBoneIndex = 0;
    }
}

void cProjectileUnitObject::DestroyAllUnitEffect()
{
    for (int a = 0; a < MAX_OBJECT_EFFECT; a++)
    {		
        DestroyUnitEffect(a);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "ObjectManager.h"
#include "CloneManager.h"
#include "Clone.h"

cProjectileCloneObject::cProjectileCloneObject()
: m_OriginalObjectKey(INVALID_OBJECT_KEY)
, m_CloneObjectKey(INVALID_OBJECT_KEY)
, m_pClone(NULL)
, m_CloneColor(0xFFFFFFFF)
, m_CloneBlendType(AB_LIGHTING)
{
    m_Actived = FALSE;
    m_ActiveDelayTime = 0;
    m_StartedTime = GetCurrentTime();

}

void cProjectileCloneObject::CreateClone( DWORD ObjectKey, WzID AnimationID )
{
    m_OriginalObjectKey = ObjectKey;
    m_AnimationID = AnimationID;
}

void cProjectileCloneObject::Render()
{
    if( m_pClone != NULL )
    {
        m_pClone->Render();
    }
}

void cProjectileCloneObject::Process( DWORD TickTime )
{
    if( m_Actived == FALSE )
    {
        if( m_StartedTime + m_ActiveDelayTime <= GetCurrentTime() )
        {
            m_Actived = TRUE;


            if( Object* object = g_ObjectManager.GetObject( m_OriginalObjectKey ) )
            {
                m_pClone = g_CloneManager.CreateNotManagedClone( object );
                //m_pClone->SetPosition( GetClonePosition() );
                //m_pClone->SetDirection( GetClonePosition() + GetCloneDirection() );

                m_pClone->SetPlayAni(TRUE);
                m_pClone->SetAnimation( m_AnimationID, FALSE );

                m_pClone->SetAlphaBlend(m_CloneBlendType);
                //WzColor color = WzColor_RGBA(255,255,255,220);
                //m_pClone->SetAlphaBlend(AB_LIGHTING);
                //m_pClone->SetColor(color);
            }
        }
    }


    if( m_pClone != NULL )
    {
        WzColor color = m_CloneColor;
        if( IsCloneFadeOut() == TRUE )
        {
            nProjectileFunc::GetProjectileBlendColor( GetCloneLifeTimeRate(), color );
        }
        m_pClone->SetColor(color);

        m_pClone->SetCurPos( GetClonePosition() );
        m_pClone->SetRotation( Math_GetAngleFromVector( &GetCloneDirection() ) );
        //m_pClone->SetPosition( GetClonePosition() );
        //m_pClone->SetDirection( GetClonePosition() + GetCloneDirection() );
        m_pClone->Process( TickTime );
    }
}

void cProjectileCloneObject::Destroy()
{
    if( m_pClone != NULL )
    {
        m_pClone->Destroy();
        delete m_pClone;
        m_pClone = NULL;
    }
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
cProjectileTrail::cProjectileTrail()
: m_pTrailMesh(NULL)
, m_TrailHeight(0.0f)
, m_TrailLength(0.0f)
, m_TrailColorRed(255)
, m_TrailColorGreen(255)
, m_TrailColorBlue(255)
, m_ReadyTrail(FALSE)
{
}


BOOL cProjectileTrail::CreateTrail( HANDLE hTailTexture, float TrailHeight, float TrailLength, BYTE ColorRed, BYTE ColorGreen, BYTE ColorBlue )
{
    if( TrailHeight <= 0.0f || TrailLength <= 0.0f)
        return FALSE;

    m_pTrailMesh = New_SimpleMesh(2);

    m_pTrailMesh->m_pUVVerts[0].u = 0.0f;
    m_pTrailMesh->m_pUVVerts[0].v = 1.0f;

    m_pTrailMesh->m_pUVVerts[1].u = 0.0f;
    m_pTrailMesh->m_pUVVerts[1].v = 0.0f;

    m_pTrailMesh->m_pUVVerts[2].u = 1.0f;
    m_pTrailMesh->m_pUVVerts[2].v = 1.0f;

    m_pTrailMesh->m_pUVVerts[3].u = 0.0f;
    m_pTrailMesh->m_pUVVerts[3].v = 0.0f;

    m_pTrailMesh->m_pUVVerts[4].u = 1.0f;
    m_pTrailMesh->m_pUVVerts[4].v = 0.0f;

    m_pTrailMesh->m_pUVVerts[5].u = 1.0f;
    m_pTrailMesh->m_pUVVerts[5].v = 1.0f;

    m_pTrailMesh->m_hTexDiffuse = hTailTexture;

    m_TrailHeight = TrailHeight;
    m_TrailLength = TrailLength;

    m_TrailColorRed = ColorRed;
    m_TrailColorGreen = ColorGreen;
    m_TrailColorBlue = ColorBlue;

    return TRUE;
}


void cProjectileTrail::Destroy()
{
    if( m_pTrailMesh)
    {
        Delete_SimpleMesh(m_pTrailMesh);
        m_pTrailMesh = NULL;
    }	

}

void cProjectileTrail::Render()
{
    if( m_pTrailMesh )
    {
        if( m_ReadyTrail == TRUE )
        {
            BYTE FirstAlpha = 200;
            float fRate = 1.0f - (GetArrowPositionRate());		
            if (fRate <= 0.0f)
            {
                fRate = 0.0f;
            }
            BYTE Alpha = (BYTE)((float)FirstAlpha * fRate);

            g_pSunRenderer->SetColor( m_TrailColorRed, m_TrailColorGreen, m_TrailColorBlue, Alpha );
            g_pSunRenderer->RealizeRenderSimpleMesh(m_pTrailMesh);
        }
    }
}

void cProjectileTrail::Process()
{
    if( m_pTrailMesh == NULL )
        return;


    St_Camera* pCameraInfo = g_pSunRenderer->GetCameraInfo();
    WzVector CameraDirection = pCameraInfo->m_wvDir;


    float fRate = GetArrowPositionRate();

    float fTrailHeadRate = 1.0f - fRate;
    float fTrailEndRate = fRate - m_TrailLength;

    if( fTrailEndRate <= 0.0f)
    {
        fTrailEndRate = 0.0f;
    }

    if( fTrailEndRate >= 1.0f)
    {
        fTrailEndRate = 1.0f;
    }


    WzVector vCurPos,vCurPos2,vDestPos,vDestPos2,vNewPos,vNewPos2,vNewEndPos,vNewEndPos2;
    vCurPos = vCurPos2 = GetArrowStartPosition();
    vDestPos = vDestPos2 = GetArrowTargetPosition();

    WzVector vDest = vDestPos - vCurPos;
    VectorNormalize(&vDest,&vDest);
    WzVector vDirection;

    VectorCrossProduct(&vDirection,&vDest,&CameraDirection);
    VectorNormalize(&vDirection,&vDirection);

    WzVector CamPos = pCameraInfo->m_wvFrom;
    WzVector BeamSrc = GetArrowStartPosition();
    WzVector BeamDst = GetArrowTargetPosition();

    WzVector vBeamDir = BeamDst - BeamSrc;
    VectorNormalize(&vBeamDir,&vBeamDir);
    WzVector vBeamToCam = CamPos - BeamSrc;
    VectorNormalize(&vBeamToCam,&vBeamToCam);		
    WzVector vBeamUp;
    vBeamUp.x = 0.0f;
    vBeamUp.y = 0.0f;
    vBeamUp.z = 1.0f;

    WzVector vCurrDir = vBeamToCam - vBeamDir;

    float fAngle = acos(VectorDotProduct(&vBeamUp,&vCurrDir));

    if (vCurrDir.x < 0)
        fAngle = fAngle;
    else if (vCurrDir.x > 0)
        fAngle = -fAngle;
    else
        fAngle = 0;

    WzMatrix matrix;

    VectorNormalize(&vCurrDir,&vCurrDir);
    MatrixRotation(&matrix,&vCurrDir,fAngle);

    WzVector vTemp;
    vTemp.x = 0.0f;
    vTemp.y = 0.0f;
    vTemp.z = 1.0f;

    float fTrailHeight = m_TrailHeight * fTrailHeadRate;

    vCurPos = vCurPos + vDirection * (fTrailHeight / 2.0f);
    vCurPos2 = vCurPos2 - vDirection * (fTrailHeight / 2.0f);

    vDestPos = vDestPos + vDirection * (fTrailHeight / 2.0f);
    vDestPos2 = vDestPos2 - vDirection * (fTrailHeight / 2.0f);

    vNewPos = vCurPos * (1.0f - fRate) + vDestPos * (fRate);
    vNewPos2 = vCurPos2 * (1.0f - fRate) + vDestPos2 * (fRate);

    vNewEndPos = vCurPos * (1.0f - fTrailEndRate) + vDestPos * (fTrailEndRate);
    vNewEndPos2 = vCurPos2 * (1.0f - fTrailEndRate) + vDestPos2 * (fTrailEndRate);


    // 매쉬 좌표에 대입
    WzVector vArrowDir = vDestPos - vCurPos;
    VectorNormalize(&vArrowDir,&vArrowDir);

    WzVector vTrailOffset = vArrowDir * -0.3f;

    m_pTrailMesh->m_pVertices[0] = vTrailOffset + vNewEndPos;
    m_pTrailMesh->m_pVertices[1] = vTrailOffset + vNewPos;
    m_pTrailMesh->m_pVertices[2] = vTrailOffset + vNewEndPos2;
    m_pTrailMesh->m_pVertices[3] = vTrailOffset + vNewPos;
    m_pTrailMesh->m_pVertices[4] = vTrailOffset + vNewPos2;
    m_pTrailMesh->m_pVertices[5] = vTrailOffset + vNewEndPos2;

    m_ReadyTrail = TRUE;
}


#include "Map.h"

cProjectileSwordTrail::cProjectileSwordTrail()
: m_LastTickTIme(0)
, m_InitializedSwordTrail(FALSE)
{
}

void cProjectileSwordTrail::Process( DWORD TickTime )
{
    WzVector CurrentPosition = GetSwordTrailPosition();

    if( CEffectBody* pBody = g_pEffectManager->GetEffectPtrHandle( GetEffectHandle() ) )
    {
        if( CEElementFlatChain* pFChain = pBody->GetFlatChainPtr() )
        {
            if( m_InitializedSwordTrail == FALSE )
            {
                pBody->SetRenderType(AR_REFRACTION_GLASS);
                pBody->SetRefractionRate(0.5f);		// 굴절률.(0에 가까워 질수록 투명)
                pBody->SetRefractionAlpha(1.0f);	// 1.0 에 가까워질수록 본 이미지에 가까워짐

                pFChain->SetLifeInfo(0, 3000);
                pFChain->SetTailNum(40);							
                pFChain->SetSplineLength(0.1f);

                m_InitializedSwordTrail = TRUE;
            }

            pFChain->SetPosition( CurrentPosition + m_TrailStartOffset, CurrentPosition + m_TrailEndOffset );
        }
    }

    if( m_TrailEffectDelay > 0 )
    {
        if( GetCurrentTime() - m_LastTickTIme >= m_TrailEffectDelay )
        {
            WzVector vResult;
            g_pMap->GetTerrainHeight( &CurrentPosition, &vResult);

            GlobalFunc::CreateEffectBeUpToOption( StrToWzID("A030"), vResult, 0, 0 );

            m_LastTickTIme = GetCurrentTime();
        }
    }
}

void cProjectileSwordTrail::Destroy()
{
}

void cProjectileSwordTrail::Render()
{

}


