//------------------------------------------------------------------------------
//  ProjecttileManager.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
//#include "Projectile.h"
//#include "Character.h"
//#include "AppearanceManager.h"
//#include "ResourceManager.h"
//#include "ObjectManager.h"
//#include "Character.h"
//#include "Map.h"
//#include "GameFunc.h"
//#include "Hero.h"
//#include "Camera.h"
//#include "GlobalFunc.h"
//#include "InterfaceManager.h"
//#include "BattleScene.h"
//#include "LoadScene.h"
//#include "HeroData.h"
//
//using namespace PROJECTILE;
//float fRate;
//
//
////------------------------------------------------------------------------------
///**
//*/
//Projectile::Projectile(void)
//{
//	m_hEffect = INVALID_HANDLE_VALUE;
//	m_AlphaMode = AB_ALPHA;
//	m_pMesh = NULL;
//	m_pUnitDraw = NULL;
//	m_bDamageProcessed = FALSE;
//	m_bMultiTarget = FALSE;	
//	m_bActive =TRUE;
//	m_bUseExtraMatrix = FALSE;
//	InitializeEffect();
//	dwProjectileKey=0;
//}
//
////------------------------------------------------------------------------------
///**
//*/
//Projectile::~Projectile(void)
//{
//}
//
////------------------------------------------------------------------------------
///**
//*/
//void Projectile::Destroy()
//{
//    // 데미지 처리가 되지않았다면 처리하도록 한다
//	if(m_Param.bGroupDamage)
//	{
//		ProcessDamageGroup();
//	}
//	else
//	{
//		ProcessDamage();
//	}
//	
//	Character *pChr = (Character *)g_ObjectManager.GetObject(m_Param.dwOwner);
// //   if (pChr != NULL)
// //   {
// //       pChr->ProcessAttackResult(m_Param.dwAttackSerial, FALSE, TRUE );
// //   }
//
//	// 데미지 처리를 한다.
//	if (m_Param.bAddNextSerial && pChr)
//	{
//		pChr->AddNextAttackSerial();
//	}			
//
//	if (m_pMesh)
//	{
//		Delete_SimpleMesh(m_pMesh);
//		m_pMesh = NULL;
//	}	
//
//	if (m_hEffect != INVALID_HANDLE_VALUE)
//	{
//		g_pEffectManager->RemoveEffectPtr(m_hEffect);
//		m_hEffect = INVALID_HANDLE_VALUE;
//	}
//
//#ifdef _KIKI_DELETE_PROJECTILEID_POWER_WAVE_
//    DoNothing();
//#else //_KIKI_DELETE_PROJECTILEID_POWER_WAVE_
//	int size=m_hEffectArray.Size();
//
//	for(int i=0;i<size;i++)
//	{
//		if(m_hEffectArray[i]!=INVALID_HANDLE_VALUE)
//		{
//			g_pEffectManager->RemoveEffectPtr(m_hEffectArray[i]);
//		}
//	}
//
//	m_hEffectArray.Clear();
//#endif //_KIKI_DELETE_PROJECTILEID_POWER_WAVE_
//
//	DestroyAllEffect();
//}
//
////------------------------------------------------------------------------------
///**
//*/
//BOOL Projectile::Render()
//{
//	if(!m_bActive||!m_bUpdated)  
//		return FALSE;
//
//    if (m_pUnitDraw)
//    {
//		ENUM_ALPHABLEND eAlphablendMode = g_pSunRenderer->GetAlphaBlendMode();
//		g_pSunRenderer->SetAlphaBlendMode( m_Param.AlphaMode, FALSE );
//
//		g_pSunRenderer->SetColor(m_wzColor);       
//        g_pSunRenderer->RenderUnit(m_pUnitDraw);
//
//		g_pSunRenderer->SetAlphaBlendMode( eAlphablendMode, FALSE );
//	}
//
//    // 하드 코딩 고치자.
//    if ((m_Param.bMakeTrail))
//        RenderTrail();
//
//	return TRUE;
//}
//
////------------------------------------------------------------------------------
///**
//*/
//BOOL Projectile::RenderTrail()
//{   
//    if (m_Param.bMakeTrail && m_pMesh)
//    {
//        BYTE Alpha;
//        BYTE FirstAlpha = 200;
//        float fRate;
//        fRate = 1.0f - ((float)m_dwDelay / (float) m_Param.dwMoveTime);		
//        if (fRate <= 0.0f)
//        {
//            fRate = 0.0f;
//        }
//        Alpha = (BYTE)((float)FirstAlpha * fRate);
//        g_pSunRenderer->SetColor(m_Param.bR,m_Param.bG,m_Param.bB,Alpha);
//
//        g_pSunRenderer->RealizeRenderSimpleMesh(m_pMesh);
//    }
//
//    return TRUE;
//}
//
//
//
//BOOL Projectile::ProcessUpperBlowWave(DWORD dwTick)
//{
//
//    return TRUE;
//}
//
//void Projectile::SetAni(WzID aniId,BOOL bLoop)
//{
//	//에니가 바뀔경우 이펙트 파괴
//	DestroyAllEffect();
//
//	if(m_pUnitDraw)
//	{
//		m_pUnitDraw->SetCurrentAnimation(aniId,bLoop);
//	}
//}
////------------------------------------------------------------------------------
///**
//Todo:정리 할것 
//*/
//BOOL Projectile::ProcessPoisonRain(DWORD dwTick)
//{
//	St_Camera * pCameraInfo = g_pSunRenderer->GetCameraInfo();
//
//	WzVector vTo    = pCameraInfo->m_wvTo;
//	WzVector vRight = pCameraInfo->m_wvRight;
//	WzVector vDown  = -pCameraInfo->m_wvUp;
//	WzVector vDir   = pCameraInfo->m_wvDir;
//
//	assert(m_Param.dwMoveTime);
//
//	float fRate;
//
//	if(m_Param.dwMoveTime)
//		fRate= (float)m_dwDelay / (float) m_Param.dwMoveTime;		
//	else
//		fRate=0.0f;
//
//	float fTrailRate = 1.0f - fRate; 
//	float fEndRate;
//
//	fRate *= m_Param.fSpeed;
//	fEndRate = fRate - m_Param.fTrailLength;
//
//	if (fRate >= 1.0f)
//	{
//		fRate = 1.0f;
//	}
//
//	if (fEndRate <= 0.0f)
//	{
//		fEndRate = 0.0f;
//	}
//
//	if (fEndRate >= 1.0f)
//	{
//		fEndRate = 1.0f;
//	}
//
//	WzVector vArrowPos;
//	WzVector vDest,vTemp;
//	float fArrowAngleZ,fArrowAngleX;
//
//	vDest =  m_Param.vTargetPos - m_Param.vStartPos;
//	vTemp = vDest;
//	vTemp.z = 0.0f;
//
//	float fLength;
//
//	if (vTemp.x == 0.0f && vTemp.y == 0.0f)
//	{
//		fLength = 0.0f;
//	}
//	else
//	{
//		fLength = VectorLength(&vTemp);
//	}
//
//	fArrowAngleX = -atan2(vDest.z,fLength);
//	fArrowAngleZ = Math_GetAngleFromVector(&vDest);
//	vArrowPos = m_Param.vStartPos * (1.0f - fRate) + m_Param.vTargetPos * (fRate);
//
//	WzVector vCurPos,vCurPos2,vDestPos,vDestPos2,vNewPos,vNewPos2,vNewEndPos,vNewEndPos2;
//	vCurPos = vCurPos2 = m_Param.vStartPos;
//	vDestPos = vDestPos2 = m_Param.vTargetPos;
//
//	VectorNormalize(&vDest,&vDest);
//	WzVector vDirection;
//
//	VectorCrossProduct(&vDirection,&vDest,&vDir);
//	VectorNormalize(&vDirection,&vDirection);
//
//	WzVector CamPos = pCameraInfo->m_wvFrom;
//	WzVector BeamSrc = m_Param.vStartPos;
//	WzVector BeamDst = m_Param.vTargetPos;
//
//	WzVector vBeamDir = BeamDst - BeamSrc;
//	VectorNormalize(&vBeamDir,&vBeamDir);
//	WzVector vBeamToCam = CamPos - BeamSrc;
//	VectorNormalize(&vBeamToCam,&vBeamToCam);		
//	WzVector vBeamUp;
//	vBeamUp.x = 0.0f;
//	vBeamUp.y = 0.0f;
//	vBeamUp.z = 1.0f;
//
//	WzVector vCurrDir = vBeamToCam - vBeamDir;
//
//	float fAngle = acos(VectorDotProduct(&vBeamUp,&vCurrDir));
//
//	if (vCurrDir.x < 0)
//		fAngle = fAngle;
//	else if (vCurrDir.x > 0)
//		fAngle = -fAngle;
//	else
//		fAngle = 0;
//
//	WzMatrix matrix;
//
//	VectorNormalize(&vCurrDir,&vCurrDir);
//	MatrixRotation(&matrix,&vCurrDir,fAngle);
//
//	vTemp.x = 0.0f;
//	vTemp.y = 0.0f;
//	vTemp.z = 1.0f;
//	
//	float fHeight = m_Param.fHeight * fTrailRate;
//
//	vCurPos = vCurPos + vDirection * (fHeight / 2.0f);
//	vCurPos2 = vCurPos2 - vDirection * (fHeight / 2.0f);
//
//	vDestPos = vDestPos + vDirection * (fHeight / 2.0f);
//	vDestPos2 = vDestPos2 - vDirection * (fHeight / 2.0f);
//
//	vNewPos = vCurPos * (1.0f - fRate) + vDestPos * (fRate);
//	vNewPos2 = vCurPos2 * (1.0f - fRate) + vDestPos2 * (fRate);
//
//	vNewEndPos = vCurPos * (1.0f - fEndRate) + vDestPos * (fEndRate);
//	vNewEndPos2 = vCurPos2 * (1.0f - fEndRate) + vDestPos2 * (fEndRate);
//
//	// 발사체가 적에게 닿았으면	
//	if(fRate >= 1.0f&&m_Param.POISON_RAIN.bAniChange)
//	{
//		if (m_Param.bCameraShake)
//		{
//			m_Param.bCameraShake=FALSE;
//			g_Camera.SetVibeType(Camera::VIBE_TYPE_VERTICAL);
//			g_Camera.StartVibration(40,0.03f);
//		}
//
//		m_Param.POISON_RAIN.bAniChange=FALSE;
//		SetAni((m_Param.POISON_RAIN.aniIDEnd));
//		ProcessDamageGroup();
//	}
//
//	if (m_pUnitDraw) 
//	{
//		float	fLoopCheck;
//		fLoopCheck = 0.0f;
//		WzEvent Event;
//		ZeroMemory(&Event,sizeof(Event));
//
//		g_pSunRenderer->ProgressUnitFrame(m_pUnitDraw, ( float )dwTick, &Event, &fLoopCheck );
//
//		m_pUnitDraw->SetPosition(vArrowPos);
//
//
//		//유닛에 딸린 이펙트 처리 
//		if (Event.m_iNumEffect > 0)
//		{
//			for (int a = 0; a < Event.m_iNumEffect; a++)
//			{
//				int iIndex = Event.m_Effect[a].m_iEffectType;
//
//				assert(iIndex >= 0 && iIndex < MAX_OBJECT_EFFECT);
//
//				if (iIndex < 0)
//				{
//					iIndex = 0;
//				}
//				else if (iIndex >= MAX_OBJECT_EFFECT)
//				{
//					iIndex = MAX_OBJECT_EFFECT - 1;
//				}
//
//				if (Event.m_Effect[a].m_byEffectFlag & EFF_DESTROY)
//				{
//					DestroyObjectEffect(iIndex);
//				}
//				else
//				{
//					DestroyObjectEffect(iIndex);
//
//					int boneindex = Event.m_Effect[a].m_iEffectBoneIndex;
//					WzVector vPos=vArrowPos;	
//					GameFunc::GetBonePos(m_pUnitDraw,boneindex,&vPos);
//
//
//					m_EffectParam[iIndex].hEffect= GlobalFunc::CreateEffectBeUpToOption(Event.m_Effect[a].m_wiEffectWzID,vPos,m_Param.dwOwner,m_Param.dwTargetID);//!loop 이펙트 쓰지 말것    
//
//					m_EffectParam[iIndex].iBoneIndex = boneindex;
//				}
//			}
//		}
//	}
//
//	m_dwDelay += dwTick;
//
//	
//	if (m_hEffect != INVALID_HANDLE_VALUE)
//	{
//		CEffectBody *pBody = g_pEffectManager->GetEffectPtrHandle(m_hEffect);
//		if(pBody)
//		{
//			WzVector vRot;
//
//			vRot.x = fArrowAngleX;
//			vRot.y = 0.0f;
//			vRot.z = fArrowAngleZ;
//
//
//			WzVector vScale;
//
//			vScale.x = 1.0f;
//			vScale.y = 1.0f;
//			vScale.z = 1.0f;
//
//
//			if(m_Param.vScale != c_ZeroVector)
//			{
//				vScale=m_Param.vScale;
//			}
//
//			pBody->SetResultMatrix(vArrowPos,vRot,vScale);
//
//		}
//	}
//
//	m_bUpdated = TRUE;
//
//
//	// 라이프 타임이 있을 경우엔 대체 한다.
//	if(m_Param.dwLifeTime)
//	{
//		fRate = (float)m_dwDelay / (float) m_Param.dwLifeTime;		
//	}
//
//	if(m_Param.bFadeOut)
//	{
//		DWORD alphaTime=m_Param.dwLifeTime/4;
//		if(!alphaTime)
//			alphaTime=1;
//
//		DWORD startAlphaTime=m_Param.dwLifeTime-alphaTime;
//
//		if(m_dwDelay>=startAlphaTime)
//		{
//			float fRate = (float)(m_dwDelay-startAlphaTime) / (float) alphaTime;		
//
//			float alphaRate=1.0f-fRate;
//			if(alphaRate<0.0f)
//			{
//				alphaRate=0.0f;
//			}
//
//			if(alphaRate>1.0f)
//			{
//				alphaRate=1.0f;
//			}
//
//			BYTE Alpha = (BYTE)(255 * alphaRate);
//			m_wzColor = WzColor_RGBA(255,255,255,Alpha);
//		}
//	}
//
//	if (fRate >= 1.0f)
//	{
//		return TRUE;
//	}
//
//	return FALSE;
//}
//
//
//
//
////------------------------------------------------------------------------------
///**
//*/
//BOOL Projectile::ProcessPoisonThorn(DWORD dwTick)
//{
//	ASSERT(m_Param.dwMoveTime);
//
//	float fRate;
//
//	if(m_Param.dwMoveTime)
//		fRate= (float)m_dwDelay / (float) m_Param.dwMoveTime;		
//	else
//		fRate=0.0f;
//
//	float fTrailRate = 1.0f - fRate; 
//	float fEndRate;
//
//	fRate *= m_Param.fSpeed;
//	fEndRate = fRate - m_Param.fTrailLength;
//
//	if (fRate >= 1.0f)
//	{
//		fRate = 1.0f;
//	}
//
//	if (fEndRate <= 0.0f)
//	{
//		fEndRate = 0.0f;
//	}
//
//	if (fEndRate >= 1.0f)
//	{
//		fEndRate = 1.0f;
//	}
//
//	WzVector vArrowPos;
//	WzVector vDest,vTemp;
//	float fArrowAngleZ,fArrowAngleX;
//
//	Object *pObject = g_ObjectManager.GetObject(m_Param.dwTargetID);
//
//	if(pObject)
//	{
//		m_Param.dwTargetID=0;//
//		m_Param.vTargetPos =  pObject->GetVisiblePos();
//	}
//
//	vDest =  m_Param.vTargetPos - m_Param.vStartPos;
//
//	vTemp = vDest;
//	vTemp.z = 0.0f;
//
//	float fLength;
//
//	if (vTemp.x == 0.0f && vTemp.y == 0.0f)
//	{
//		fLength = 0.0f;
//	}
//	else
//	{
//		fLength = VectorLength(&vTemp);
//	}
//
//	fArrowAngleX = -atan2(vDest.z,fLength);
//	fArrowAngleZ = Math_GetAngleFromVector(&vDest);
//	vArrowPos = m_Param.vStartPos * (1.0f - fRate) + m_Param.vTargetPos * (fRate);
//
//	WzVector vCurPos,vCurPos2,vDestPos,vDestPos2;
//	vCurPos = vCurPos2 = m_Param.vStartPos;
//	vDestPos = vDestPos2 = m_Param.vTargetPos;
//
//
//	if (m_pUnitDraw) 
//	{
//		float	fLoopCheck;
//		fLoopCheck = 0.0f;
//		WzEvent Event;
//		ZeroMemory(&Event,sizeof(Event));
//
//		g_pSunRenderer->ProgressUnitFrame(m_pUnitDraw, ( float )dwTick, &Event, &fLoopCheck );
//
//		m_pUnitDraw->SetPosition(m_Param.vStartPos);
//		m_pUnitDraw->SetRotation(fArrowAngleX,0.0f,fArrowAngleZ);
//
//
//		//유닛에 딸린 이펙트 처리 
//		if (Event.m_iNumEffect > 0)
//		{
//			for (int a = 0; a < Event.m_iNumEffect; a++)
//			{
//				int iIndex = Event.m_Effect[a].m_iEffectType;
//
//				ASSERT(iIndex >= 0 && iIndex < MAX_OBJECT_EFFECT);
//
//				if (iIndex < 0)
//				{
//					iIndex = 0;
//				}
//				else if (iIndex >= MAX_OBJECT_EFFECT)
//				{
//					iIndex = MAX_OBJECT_EFFECT - 1;
//				}
//
//				if (Event.m_Effect[a].m_byEffectFlag & EFF_DESTROY)
//				{
//					DestroyObjectEffect(iIndex);
//				}
//				else
//				{
//					DestroyObjectEffect(iIndex);
//
//					int boneindex = Event.m_Effect[a].m_iEffectBoneIndex;
//					WzVector vPos;	
//					GameFunc::GetBonePos(m_pUnitDraw,boneindex,&vPos);
//
//
//					m_EffectParam[iIndex].hEffect = GlobalFunc::CreateEffectBeUpToOption(Event.m_Effect[a].m_wiEffectWzID,vPos,m_Param.dwOwner,m_Param.dwTargetID);	
//					m_EffectParam[iIndex].iBoneIndex = boneindex;
//				}
//			}
//
//		}
//	}
//
//	m_dwDelay += dwTick;
//
//
//	if (m_hEffect != INVALID_HANDLE_VALUE)
//	{
//		CEffectBody *pBody = g_pEffectManager->GetEffectPtrHandle(m_hEffect);
//		if(pBody)
//		{
//		
//			WzVector vRot;
//
//			vRot.x = fArrowAngleX;
//			vRot.y = 0.0f;
//			vRot.z = fArrowAngleZ;
//
//
//			WzVector vScale;
//
//			vScale.x = 1.0f;
//			vScale.y = 1.0f;
//			vScale.z = 1.0f;
//
//
//			if(m_Param.vScale != c_ZeroVector)
//			{
//				vScale=m_Param.vScale;
//			}
//
//			pBody->SetResultMatrix(m_Param.vStartPos,vRot,vScale);
//		}
//	}
//
//	m_bUpdated = TRUE;
//
//
//	// 라이프 타임이 있을 경우엔 대체 한다.
//	if(m_Param.dwLifeTime)
//	{
//		fRate = (float)m_dwDelay / (float) m_Param.dwLifeTime;		
//	}
//
//	if(m_Param.bFadeOut)
//	{
//		DWORD alphaTime=m_Param.dwLifeTime/4;
//		if(!alphaTime)
//			alphaTime=1;
//
//		DWORD startAlphaTime=m_Param.dwLifeTime-alphaTime;
//
//		if(m_dwDelay>=startAlphaTime)
//		{
//			float fRate = (float)(m_dwDelay-startAlphaTime) / (float) alphaTime;		
//
//			float alphaRate=1.0f-fRate;
//			if(alphaRate<0.0f)
//			{
//				alphaRate=0.0f;
//			}
//
//			if(alphaRate>1.0f)
//			{
//				alphaRate=1.0f;
//			}
//
//			BYTE Alpha = (BYTE)(255 * alphaRate);
//			m_wzColor = WzColor_RGBA(255,255,255,Alpha);
//
//		}
//	}
//
//	if (fRate >= 1.0f)
//	{
//		// 데미지 처리를 한다.
//		ProcessDamageGroup();
//		return TRUE;
//	}
//	return FALSE;
//}
//
//
////------------------------------------------------------------------------------
///**
//Todo:정리 할것 
//*/
//BOOL Projectile::ProcessSonyVine(DWORD dwTick)
//{
//	St_Camera * pCameraInfo = g_pSunRenderer->GetCameraInfo();
//
//	WzVector vTo    = pCameraInfo->m_wvTo;
//	WzVector vRight = pCameraInfo->m_wvRight;
//	WzVector vDown  = -pCameraInfo->m_wvUp;
//	WzVector vDir   = pCameraInfo->m_wvDir;
//
//	assert(m_Param.dwMoveTime);
//
//	Object *pObj=g_ObjectManager.GetObject(m_Param.dwTargetID);
//
//	if(pObj)
//	{
//		m_Param.vTargetPos=m_Param.vStartPos = pObj->GetVisiblePos();
//	}
//
//	float fRate;
//
//	if(m_Param.dwMoveTime)
//		fRate= (float)m_dwDelay / (float) m_Param.dwMoveTime;		
//	else
//		fRate=0.0f;
//
//	float fTrailRate = 1.0f - fRate; 
//	float fEndRate;
//
//	fRate *= m_Param.fSpeed;
//	fEndRate = fRate - m_Param.fTrailLength;
//
//	if (fRate >= 1.0f)
//	{
//		fRate = 1.0f;
//	}
//
//	if (fEndRate <= 0.0f)
//	{
//		fEndRate = 0.0f;
//	}
//
//	if (fEndRate >= 1.0f)
//	{
//		fEndRate = 1.0f;
//	}
//
//	WzVector vArrowPos;
//	WzVector vDest,vTemp;
//	float fArrowAngleZ,fArrowAngleX;
//
//	vDest =  m_Param.vTargetPos - m_Param.vStartPos;
//	vTemp = vDest;
//	vTemp.z = 0.0f;
//
//	float fLength;
//
//	if (vTemp.x == 0.0f && vTemp.y == 0.0f)
//	{
//		fLength = 0.0f;
//	}
//	else
//	{
//		fLength = VectorLength(&vTemp);
//	}
//
//	fArrowAngleX = -atan2(vDest.z,fLength);
//	fArrowAngleZ = Math_GetAngleFromVector(&vDest);
//	vArrowPos = m_Param.vStartPos * (1.0f - fRate) + m_Param.vTargetPos * (fRate);
//
//	WzVector vCurPos,vCurPos2,vDestPos,vDestPos2,vNewPos,vNewPos2,vNewEndPos,vNewEndPos2;
//	vCurPos = vCurPos2 = m_Param.vStartPos;
//	vDestPos = vDestPos2 = m_Param.vTargetPos;
//
//	VectorNormalize(&vDest,&vDest);
//	WzVector vDirection;
//
//	VectorCrossProduct(&vDirection,&vDest,&vDir);
//	VectorNormalize(&vDirection,&vDirection);
//
//	WzVector CamPos = pCameraInfo->m_wvFrom;
//	WzVector BeamSrc = m_Param.vStartPos;
//	WzVector BeamDst = m_Param.vTargetPos;
//
//	WzVector vBeamDir = BeamDst - BeamSrc;
//	VectorNormalize(&vBeamDir,&vBeamDir);
//	WzVector vBeamToCam = CamPos - BeamSrc;
//	VectorNormalize(&vBeamToCam,&vBeamToCam);		
//	WzVector vBeamUp;
//	vBeamUp.x = 0.0f;
//	vBeamUp.y = 0.0f;
//	vBeamUp.z = 1.0f;
//
//	WzVector vCurrDir = vBeamToCam - vBeamDir;
//
//	float fAngle = acos(VectorDotProduct(&vBeamUp,&vCurrDir));
//
//	if (vCurrDir.x < 0)
//		fAngle = fAngle;
//	else if (vCurrDir.x > 0)
//		fAngle = -fAngle;
//	else
//		fAngle = 0;
//
//	WzMatrix matrix;
//
//	VectorNormalize(&vCurrDir,&vCurrDir);
//	MatrixRotation(&matrix,&vCurrDir,fAngle);
//
//	vTemp.x = 0.0f;
//	vTemp.y = 0.0f;
//	vTemp.z = 1.0f;
//
//	float fHeight = m_Param.fHeight * fTrailRate;
//
//	vCurPos = vCurPos + vDirection * (fHeight / 2.0f);
//	vCurPos2 = vCurPos2 - vDirection * (fHeight / 2.0f);
//
//	vDestPos = vDestPos + vDirection * (fHeight / 2.0f);
//	vDestPos2 = vDestPos2 - vDirection * (fHeight / 2.0f);
//
//	vNewPos = vCurPos * (1.0f - fRate) + vDestPos * (fRate);
//	vNewPos2 = vCurPos2 * (1.0f - fRate) + vDestPos2 * (fRate);
//
//	vNewEndPos = vCurPos * (1.0f - fEndRate) + vDestPos * (fEndRate);
//	vNewEndPos2 = vCurPos2 * (1.0f - fEndRate) + vDestPos2 * (fEndRate);
//
//	if (m_pUnitDraw) 
//	{
//		float	fLoopCheck;
//		fLoopCheck = 0.0f;
//		WzEvent Event;
//		ZeroMemory(&Event,sizeof(Event));
//
//		g_pSunRenderer->ProgressUnitFrame(m_pUnitDraw, ( float )dwTick, &Event, &fLoopCheck );
//	
//		m_pUnitDraw->SetPosition(vArrowPos);
//
//		m_pUnitDraw->SetRotation(0.0f,0.0f,m_fAngleZ);
//
//		//유닛에 딸린 이펙트 처리 
//		if (Event.m_iNumEffect > 0)
//		{
//			for (int a = 0; a < Event.m_iNumEffect; a++)
//			{
//				int iIndex = Event.m_Effect[a].m_iEffectType;
//
//				assert(iIndex >= 0 && iIndex < MAX_OBJECT_EFFECT);
//
//				if (iIndex < 0)
//				{
//					iIndex = 0;
//				}
//				else if (iIndex >= MAX_OBJECT_EFFECT)
//				{
//					iIndex = MAX_OBJECT_EFFECT - 1;
//				}
//
//				if (Event.m_Effect[a].m_byEffectFlag & EFF_DESTROY)
//				{
//					DestroyObjectEffect(iIndex);
//				}
//				else
//				{
//					DestroyObjectEffect(iIndex);
//
//					int boneindex = Event.m_Effect[a].m_iEffectBoneIndex;
//					WzVector vPos;	
//					GameFunc::GetBonePos(m_pUnitDraw,boneindex,&vPos);
//
//					m_EffectParam[iIndex].hEffect = GlobalFunc::CreateEffectBeUpToOption(Event.m_Effect[a].m_wiEffectWzID,vPos,m_Param.dwOwner,m_Param.dwTargetID);
//					m_EffectParam[iIndex].iBoneIndex = boneindex;
//				}
//			}
//
//		}
//	}
//
//	m_dwDelay += dwTick;
//
//
//	if (m_hEffect != INVALID_HANDLE_VALUE)
//	{
//		CEffectBody *pBody = g_pEffectManager->GetEffectPtrHandle(m_hEffect);
//		if(pBody)
//		{
//			WzVector vRot;
//
//			vRot.x = fArrowAngleX;
//			vRot.y = 0.0f;
//			vRot.z = fArrowAngleZ;
//
//
//			WzVector vScale;
//
//			vScale.x = 1.0f;
//			vScale.y = 1.0f;
//			vScale.z = 1.0f;
//
//
//			if(m_Param.vScale != c_ZeroVector)
//			{
//				vScale=m_Param.vScale;
//			}
//
//			pBody->SetResultMatrix(vArrowPos,vRot,vScale);
//
//		}
//	}
//
//	m_bUpdated = TRUE;
//
//	if(pObj && pObj->IsKindOfObject(CHARACTER_OBJECT))
//	{
//		Character *pChr=(Character *)pObj;
//	    if(!pChr->CannotMove())
//		{
//			DWORD alphaTime=m_Param.dwLifeTime/4;
//			if(!alphaTime)
//				alphaTime=1;
//
//			DWORD startAlphaTime=m_Param.dwLifeTime-alphaTime;
//
//			if(m_dwDelay<startAlphaTime)
//			{
//				if(!m_Param.SONYVINE.bDeBuff)
//				{
//					m_Param.SONYVINE.bDeBuff=TRUE;
//					DWORD alphaTime=m_dwDelay/4;
//					if(!alphaTime)
//						alphaTime=1000;
//
//					m_Param.dwLifeTime=m_Param.dwMoveTime=m_dwDelay+alphaTime;
//
//				}
//
//			}
//		}
//	}
//
//	// 라이프 타임이 있을 경우엔 대체 한다.
//	if(m_Param.dwLifeTime)
//	{
//		fRate = (float)m_dwDelay / (float) m_Param.dwLifeTime;		
//	}
//
//
//
//	if(m_Param.bFadeOut)
//	{
//		DWORD alphaTime=m_Param.dwLifeTime/4;
//		if(!alphaTime)
//			alphaTime=1;
//
//		DWORD startAlphaTime=m_Param.dwLifeTime-alphaTime;
//
//		if(m_dwDelay>=startAlphaTime)
//		{
//			float fRate = (float)(m_dwDelay-startAlphaTime) / (float) alphaTime;		
//
//			float alphaRate=1.0f-fRate;
//			if(alphaRate<0.0f)
//			{
//				alphaRate=0.0f;
//			}
//
//			if(alphaRate>1.0f)
//			{
//				alphaRate=1.0f;
//			}
//
//			BYTE Alpha = (BYTE)(255 * alphaRate);
//			m_wzColor = WzColor_RGBA(255,255,255,Alpha);
//		}
//	}
//
//	if (fRate >= 1.0f)
//	{
//		// 데미지 처리를 한다.
//		ProcessDamageGroup();
//		return TRUE;
//	}
//	return FALSE;
//}
//
//
////------------------------------------------------------------------------------
///**
//	베이지어 곡석을 그리는 발사체 연출 
//*/
//BOOL Projectile::ProcessBezier(DWORD dwTick)
//{
//	St_Camera * pCameraInfo = g_pSunRenderer->GetCameraInfo();
//
//	WzVector vTo    = pCameraInfo->m_wvTo;
//	WzVector vRight = pCameraInfo->m_wvRight;
//	WzVector vDown  = -pCameraInfo->m_wvUp;
//	WzVector vDir   = pCameraInfo->m_wvDir;
//
//	assert(m_Param.dwMoveTime);
//	
//	if(m_Param.dwTargetID)//타겟 쫒아가기
//	{
//		Character *pTarget =  (Character *)g_ObjectManager.GetObject(m_Param.dwTargetID);
//		if (pTarget)
//		{
//			m_Param.vTargetPos = pTarget->GetArrowTargetPos();
//		}
//	}
//
//	float fRate;
//
//	if(m_Param.dwMoveTime)
//		fRate= (float)m_dwDelay / (float) m_Param.dwMoveTime;		
//	else
//		fRate=0.0f;
//
//	float fTrailRate = 1.0f - fRate; 
//	float fEndRate;
//
//	fRate *= m_Param.fSpeed;
//	fEndRate = fRate - m_Param.fTrailLength;
//
//	if (fRate >= 1.0f)
//	{
//		fRate = 1.0f;
//	}
//
//	if (fEndRate <= 0.0f)
//	{
//		fEndRate = 0.0f;
//	}
//
//	if (fEndRate >= 1.0f)
//	{
//		fEndRate = 1.0f;
//	}
//
//	WzVector vArrowPos;
//	WzVector vDest,vTemp;
//	float fArrowAngleZ,fArrowAngleX;
//	
//	//베지어 기저식
//	vArrowPos = m_Param.vStartPos *((1.0f - fRate)*(1.0f - fRate)*(1.0f - fRate)) +
//				m_Param.BEZIER.vPos0*fRate*3*((1.0f - fRate)*(1.0f - fRate))+
//				m_Param.BEZIER.vPos1*3*fRate*fRate*(1.0f - fRate)+
//                +m_Param.vTargetPos * (fRate*fRate*fRate);
//
//	float fNextRate=fRate+0.1f;
//	min(fNextRate,1.0f);
//
//	WzVector vNextArrowPos= m_Param.vStartPos *((1.0f - fNextRate)*(1.0f - fNextRate)*(1.0f - fNextRate)) +
//		m_Param.BEZIER.vPos0*fNextRate*3*((1.0f - fNextRate)*(1.0f - fNextRate))+
//		m_Param.BEZIER.vPos1*3*fNextRate*fNextRate*(1.0f - fNextRate)+
//		+m_Param.vTargetPos * (fNextRate*fNextRate*fNextRate);
//
//	vDest = vNextArrowPos - vArrowPos;//이전 위치에서 현재 위치로 계산하자.
//	vTemp = vDest;
//	vTemp.z = 0.0f;
//
//	float fLength;
//
//	if (vTemp.x == 0.0f && vTemp.y == 0.0f)
//	{
//		fLength = 0.0f;
//	}
//	else
//	{
//		fLength = VectorLength(&vTemp);
//	}
//
//	fArrowAngleX = -atan2(vDest.z,fLength);			//최종 목적지가 아닌 현재 목적지로 꺽자
//	fArrowAngleZ = Math_GetAngleFromVector(&vDest); //최종 목적지가 아닌 현재 목적지로 꺽자
//
//	WzVector vCurPos,vCurPos2,vDestPos,vDestPos2,vNewPos,vNewPos2,vNewEndPos,vNewEndPos2;
//	vCurPos = vCurPos2 = m_Param.vStartPos;
//	vDestPos = vDestPos2 = m_Param.vTargetPos;
//
//	VectorNormalize(&vDest,&vDest);
//	WzVector vDirection;
//
//	VectorCrossProduct(&vDirection,&vDest,&vDir);
//	VectorNormalize(&vDirection,&vDirection);
//
//	WzVector CamPos = pCameraInfo->m_wvFrom;
//	WzVector BeamSrc = m_Param.vStartPos;
//	WzVector BeamDst = m_Param.vTargetPos;
//
//	WzVector vBeamDir = BeamDst - BeamSrc;
//	VectorNormalize(&vBeamDir,&vBeamDir);
//	WzVector vBeamToCam = CamPos - BeamSrc;
//	VectorNormalize(&vBeamToCam,&vBeamToCam);		
//	WzVector vBeamUp;
//	vBeamUp.x = 0.0f;
//	vBeamUp.y = 0.0f;
//	vBeamUp.z = 1.0f;
//
//	WzVector vCurrDir = vBeamToCam - vBeamDir;
//
//	float fAngle = acos(VectorDotProduct(&vBeamUp,&vCurrDir));
//
//	if (vCurrDir.x < 0)
//		fAngle = fAngle;
//	else if (vCurrDir.x > 0)
//		fAngle = -fAngle;
//	else
//		fAngle = 0;
//
//	WzMatrix matrix;
//
//	VectorNormalize(&vCurrDir,&vCurrDir);
//	MatrixRotation(&matrix,&vCurrDir,fAngle);
//
//	vTemp.x = 0.0f;
//	vTemp.y = 0.0f;
//	vTemp.z = 1.0f;
//	
//	float fHeight = m_Param.fHeight * fTrailRate;
//
//	vCurPos = vCurPos + vDirection * (fHeight / 2.0f);
//	vCurPos2 = vCurPos2 - vDirection * (fHeight / 2.0f);
//
//	vDestPos = vDestPos + vDirection * (fHeight / 2.0f);
//	vDestPos2 = vDestPos2 - vDirection * (fHeight / 2.0f);
//
//	vNewPos = vCurPos * (1.0f - fRate) + vDestPos * (fRate);
//	vNewPos2 = vCurPos2 * (1.0f - fRate) + vDestPos2 * (fRate);
//
//	vNewEndPos = vCurPos * (1.0f - fEndRate) + vDestPos * (fEndRate);
//	vNewEndPos2 = vCurPos2 * (1.0f - fEndRate) + vDestPos2 * (fEndRate);
//
//	if(m_pMesh)
//	{
//		m_pMesh->m_pVertices[0] = vNewEndPos;
//		m_pMesh->m_pVertices[1] = vNewPos;
//		m_pMesh->m_pVertices[2] = vNewEndPos2;
//		m_pMesh->m_pVertices[3] = vNewPos;
//		m_pMesh->m_pVertices[4] = vNewPos2;
//		m_pMesh->m_pVertices[5] = vNewEndPos2;
//
//		WzVector vArrowDir = vDestPos - vCurPos;
//
//		VectorNormalize(&vArrowDir,&vArrowDir);
//
//		WzVector vTrailOffset = vArrowDir * -0.3f;
//
//		for (int a = 0; a < 6; a++)
//		{
//			m_pMesh->m_pVertices[a] = m_pMesh->m_pVertices[a] + vTrailOffset;
//		}
//	}
//
//	if (m_pUnitDraw) 
//	{
//		m_pUnitDraw->SetPosition(vArrowPos,FALSE);
//		m_pUnitDraw->SetRotation(fArrowAngleX,0,fArrowAngleZ);
//	}
//
//	m_dwDelay += dwTick;
//
//	if (m_hEffect != INVALID_HANDLE_VALUE)
//	{
//		CEffectBody *pBody = g_pEffectManager->GetEffectPtrHandle(m_hEffect);
//		if(pBody)
//		{
//			WzVector vRot;
//			vRot.x = fArrowAngleX;
//			vRot.y = 0.0f;
//			vRot.z = fArrowAngleZ;
//			WzVector vScale;
//			vScale.x = 1.0f;
//			vScale.y = 1.0f;
//			vScale.z = 1.0f;
//
//			if(m_Param.vScale != c_ZeroVector)
//			{
//				vScale=m_Param.vScale;
//			}
//			pBody->SetResultMatrix(vArrowPos,vRot,vScale);
//
//			if(m_Param.bFadeOut)
//			{
//				DWORD alphaTime=m_Param.dwLifeTime/2;
//				if(!alphaTime)
//					alphaTime=1;
//
//				DWORD startAlphaTime=m_Param.dwLifeTime-alphaTime;
//
//				if(m_dwDelay>=startAlphaTime)
//				{
//					WzColor color=pBody->GetColor();
//
//					float fRate = (float)(m_dwDelay-startAlphaTime) / (float) alphaTime;		
//
//					float alphaRate=1.0f-fRate;
//
//					float fAlpha=Alpha_WzColor(color);
//
//					if(alphaRate<0.0f)
//					{
//						alphaRate=0.0f;
//					}
//
//					if(alphaRate>1.0f)
//					{
//						alphaRate=1.0f;
//					}
//
//					BYTE Alpha = (BYTE)(255 * alphaRate);
//					color = WzColor_RGBA(Red_WzColor(color),Green_WzColor(color),Blue_WzColor(color),Alpha);
//					pBody->SetColor(color);
//				}
//			}
//		}
//	}
//
//	m_bUpdated = TRUE;
//
//	// 발사체가 적에게 닿았으면	
//	if(fRate >= 1.0f)
//	{
//		if (m_Param.bCameraShake)
//		{
//			m_Param.bCameraShake=FALSE;
//			g_Camera.SetVibeType(Camera::VIBE_TYPE_VERTICAL);
//			g_Camera.StartVibration(50,0.07f);
//		}
//
//		ProcessDamageGroup();
//	}
//
//	// 라이프 타임이 있을 경우엔 대체 한다.
//	if(m_Param.dwLifeTime)
//	{
//		fRate = (float)m_dwDelay / (float) m_Param.dwLifeTime;		
//	}
//
//	//데미지 시간이 있을경우는 바로 데미지 처리 
//	if(m_Param.dwDamageTime)
//	{
//		if(m_dwDelay>m_Param.dwDamageTime)
//		{
//			//데미지가 처리 되었으면
//			if(ProcessDamageGroup())
//			{
//				if(m_Param.bDamageLoop)
//					m_Param.dwDamageTime=+m_dwDelay;
//				else 
//					m_Param.dwDamageTime=0;
//
//			}
//		}
//	}
//
//
//	if (fRate >= 1.0f)
//	{
//		// 데미지 처리를 한다.
//		ProcessDamageGroup();
//		if(m_Param.EndeffectID)
//		{
//			GlobalFunc::CreateEffectBeUpToOption(m_Param.EndeffectID,vArrowPos,m_Param.dwOwner,m_Param.dwTargetID);//!loop 이펙트 쓰지 말것    
//			m_Param.EndeffectID=0;
//		}
//		return TRUE;
//	}
//	return FALSE;
//}
//
////------------------------------------------------------------------------------
///**
//	포이즌대거 발사체 되돌아오기
//*/
//BOOL Projectile::ProcessPoisonDagger(DWORD dwTick)
//{
//	St_Camera * pCameraInfo = g_pSunRenderer->GetCameraInfo();
//
//	assert(m_Param.dwMoveTime);
//
//	//타겟 쫒아가기
//	if(m_Param.dwTargetID)
//	{
//		Character *pTarget =  (Character *)g_ObjectManager.GetObject(m_Param.dwTargetID);
//		if (pTarget)
//		{
//			m_Param.vTargetPos = pTarget->GetArrowTargetPos();
//		}
//	}
//
//	float fRate;
//
//	if(m_Param.dwMoveTime)
//		fRate= (float)m_dwDelay / (float) m_Param.dwMoveTime;		
//	else
//		fRate=0.0f;
//
//	float fTrailRate = 1.0f - fRate; 
//	float fEndRate;
//
//	fRate *= m_Param.fSpeed;
//	fEndRate = fRate - m_Param.fTrailLength;
//
//	if (fRate >= 1.0f)
//	{
//		fRate = 1.0f;
//	}
//
//	if (fEndRate <= 0.0f)
//	{
//		fEndRate = 0.0f;
//	}
//
//	if (fEndRate >= 1.0f)
//	{
//		fEndRate = 1.0f;
//	}
//
//	WzVector vArrowPos;
//	WzVector vDest,vTemp;
//	float fArrowAngleZ,fArrowAngleX;
//
//	WzVector vPos0,vPos1,vPos2;
//
//	if(m_Param.POISON_DAGGER.bReturn)
//	{
//		vPos0=m_Param.vTargetPos;
//		vPos1=m_Param.POISON_DAGGER.vCtrl1;
//		vPos2=m_Param.vStartPos;
//	}
//	else
//	{
//		vPos0=m_Param.vStartPos;
//		vPos1=m_Param.POISON_DAGGER.vCtrl0;
//		vPos2=m_Param.vTargetPos;
//	}
//
//	float u_1=fRate;
//	float u_2=fRate*fRate;
//	float u_3=u_2*u_1;
//
//	//조절점 1개인 베지어 기저식
//	float mu = fRate;
//	vArrowPos= (vPos0 * (1 - mu) * (1 - mu) +   vPos1* (1 - mu) * mu*2 + vPos2 * mu * mu);
//
//	float fNextRate=fRate+0.1f;
//	min(fNextRate,1.0f);
//
//	u_1=fNextRate;
//	u_2=fNextRate*fNextRate;
//	u_3=fNextRate*u_1;
//
//	mu = fNextRate;
//	WzVector vNextArrowPos= (vPos0 * (1 - mu) * (1 - mu) +   vPos1* (1 - mu) * mu*2 + vPos2 * mu * mu);
//
//	vDest = vNextArrowPos - vArrowPos;//이전 위치에서 현재 위치로 계산하자.
//	vTemp = vDest;
//	vTemp.z = 0.0f;
//
//	float fLength;
//
//	if (vTemp.x == 0.0f && vTemp.y == 0.0f)
//	{
//		fLength = 0.0f;
//	}
//	else
//	{
//		fLength = VectorLength(&vTemp);
//	}
//
//	fArrowAngleX = -atan2(vDest.z,fLength);			//최종 목적지가 아닌 현재 목적지로 꺽자
//	fArrowAngleZ = Math_GetAngleFromVector(&vDest); //최종 목적지가 아닌 현재 목적지로 꺽자
//
//	if(m_pUnitDraw)
//	{
//		m_fAngleZ+=(WZ_PI/180.0f*dwTick);
//		ClampAngle(&m_fAngleZ);
//		m_pUnitDraw->SetPosition(vArrowPos,FALSE);
//		m_pUnitDraw->SetRotation(0.0f,0.0f,m_fAngleZ);
//		m_pUnitDraw->SetScale(1.5f,1.5f,1.5f);
//	}
//
//	m_dwDelay += dwTick;
//
//	if (m_hEffect != INVALID_HANDLE_VALUE)
//	{
//		CEffectBody *pBody = g_pEffectManager->GetEffectPtrHandle(m_hEffect);
//		if(pBody)
//		{
//			WzVector vRot;
//			SetVector(&vRot,fArrowAngleX,0.0f,fArrowAngleZ);
//			
//			WzVector vScale;
//			SetVector(&vScale,1.0f,1.0f,1.0f);
//		
//			if(m_Param.vScale != c_ZeroVector)
//			{
//				vScale=m_Param.vScale;
//			}
//			pBody->SetResultMatrix(vArrowPos,vRot,vScale);
//		}
//	}
//	
//	m_bUpdated = TRUE;
//
//	// 발사체가 적에게 닿았으면	
//	if(fRate >= 1.0f)
//	{
//		if(!m_Param.POISON_DAGGER.bReturn)
//		{	
//			//행위주체가 주인공일때는 흔들어준다.
//			if (m_Param.dwOwner == g_pHero->GetObjectKey())
//			{
//				m_Param.bCameraShake=FALSE;
//				g_Camera.SetVibeType(Camera::VIBE_TYPE_VERTICAL);
//				g_Camera.StartVibration(100,0.07f);
//			}
//			//!loop 이펙트 쓰지 말것
//			GlobalFunc::CreateEffectBeUpToOption(StrToWzID("K849"),vArrowPos,m_Param.dwOwner,m_Param.dwTargetID);
//			ProcessDamageGroup();
//			m_Param.POISON_DAGGER.bReturn = TRUE;
//			m_Param.dwMoveTime = m_Param.POISON_DAGGER.dwSecondTime;
//			
//			m_dwDelay = 0;
//			fRate = 0.0f;
//			return FALSE;
//
//		}
//
//	}
//
//	// 라이프 타임이 있을 경우엔 대체 한다.
//	if(m_Param.dwLifeTime)
//	{
//		fRate = (float)m_dwDelay / (float) m_Param.dwLifeTime;		
//	}
//
//	//데미지 시간이 있을경우는 바로 데미지 처리 
//	if(m_Param.dwDamageTime)
//	{
//		if(m_dwDelay>m_Param.dwDamageTime && m_Param.POISON_DAGGER.bReturn)
//		{
//			//데미지가 처리 되었으면
//			if(ProcessDamageGroup())
//			{
//				m_Param.dwDamageTime = (m_Param.bDamageLoop ? (m_Param.dwDamageTime=+m_dwDelay):(0));
//			}
//		}
//	}
//
//
//	if (fRate >= 1.0f && m_Param.POISON_DAGGER.bReturn)
//	{
//		// 데미지 처리를 한다.
//		ProcessDamageGroup();
//		if(m_Param.EndeffectID)
//		{
//			GlobalFunc::CreateEffectBeUpToOption(m_Param.EndeffectID,vArrowPos,m_Param.dwOwner,m_Param.dwTargetID);//!loop 이펙트 쓰지 말것    
//			m_Param.EndeffectID=0;
//		}
//		return TRUE;
//	}
//	return FALSE;
//}
//
//BOOL Projectile::ProcessTroch(DWORD dwTick)
//{
//	St_Camera* pCameraInfo = g_pSunRenderer->GetCameraInfo();
//
//	WzVector vTo    = pCameraInfo->m_wvTo;
//	WzVector vRight = pCameraInfo->m_wvRight;
//	WzVector vDown  = -pCameraInfo->m_wvUp;
//	WzVector vDir   = pCameraInfo->m_wvDir;
//
//	assert(m_Param.dwMoveTime);
//
//	if(m_Param.dwTargetID)//타겟 쫒아가기
//	{
//		Character *pTarget =  (Character *)g_ObjectManager.GetObject(m_Param.dwTargetID);
//		if (pTarget)
//		{
//			m_Param.vTargetPos = pTarget->GetArrowTargetPos();
//		}
//	}
//
//	float fRate;
//
//	if(m_Param.dwMoveTime)
//		fRate= (float)m_dwDelay / (float) m_Param.dwMoveTime;		
//	else
//		fRate=0.0f;
//
//	float fTrailRate = 1.0f - fRate; 
//	float fEndRate;
//
//	
//	fRate *= m_Param.fSpeed;
//	fEndRate = fRate - m_Param.fTrailLength;
//
//	if (fRate >= 1.0f)
//	{
//		fRate = 1.0f;
//	}
//
//	if (fEndRate <= 0.0f)
//	{
//		fEndRate = 0.0f;
//	}
//
//	if (fEndRate >= 1.0f)
//	{
//		fEndRate = 1.0f;
//	}
//
//	WzVector vArrowPos;
//	WzVector vDest,vTemp;
//	float fArrowAngleZ,fArrowAngleX;
//
//	//베지어 기저식
//	vArrowPos = m_Param.vStartPos *((1.0f - fRate)*(1.0f - fRate)*(1.0f - fRate)) +
//		m_Param.BEZIER.vPos0*fRate*3*((1.0f - fRate)*(1.0f - fRate))+
//		m_Param.BEZIER.vPos1*3*fRate*fRate*(1.0f - fRate)+
//		+m_Param.vTargetPos * (fRate*fRate*fRate);
//
//	float fNextRate=fRate+0.1f;
//	min(fNextRate,1.0f);
//
//	WzVector vNextArrowPos= m_Param.vStartPos *((1.0f - fNextRate)*(1.0f - fNextRate)*(1.0f - fNextRate)) +
//		m_Param.BEZIER.vPos0*fNextRate*3*((1.0f - fNextRate)*(1.0f - fNextRate))+
//		m_Param.BEZIER.vPos1*3*fNextRate*fNextRate*(1.0f - fNextRate)+
//		+m_Param.vTargetPos * (fNextRate*fNextRate*fNextRate);
//
//	vDest = vNextArrowPos - vArrowPos;//이전 위치에서 현재 위치로 계산하자.
//	vTemp = vDest;
//	vTemp.z = 0.0f;
//
//	float fLength;
//
//	if (vTemp.x == 0.0f && vTemp.y == 0.0f)
//	{
//		fLength = 0.0f;
//	}
//	else
//	{
//		fLength = VectorLength(&vTemp);
//	}
//
//	fArrowAngleX = -atan2(vDest.z,fLength);			//최종 목적지가 아닌 현재 목적지로 꺽자
//	fArrowAngleZ = Math_GetAngleFromVector(&vDest); //최종 목적지가 아닌 현재 목적지로 꺽자
//
//	WzVector vCurPos,vCurPos2,vDestPos,vDestPos2,vNewPos,vNewPos2,vNewEndPos,vNewEndPos2;
//	vCurPos = vCurPos2 = m_Param.vStartPos;
//	vDestPos = vDestPos2 = m_Param.vTargetPos;
//
//	VectorNormalize(&vDest,&vDest);
//	WzVector vDirection;
//
//	VectorCrossProduct(&vDirection,&vDest,&vDir);
//	VectorNormalize(&vDirection,&vDirection);
//
//	WzVector CamPos = pCameraInfo->m_wvFrom;
//	WzVector BeamSrc = m_Param.vStartPos;
//	WzVector BeamDst = m_Param.vTargetPos;
//
//	WzVector vBeamDir = BeamDst - BeamSrc;
//	VectorNormalize(&vBeamDir,&vBeamDir);
//	WzVector vBeamToCam = CamPos - BeamSrc;
//	VectorNormalize(&vBeamToCam,&vBeamToCam);		
//	WzVector vBeamUp;
//	vBeamUp.x = 0.0f;
//	vBeamUp.y = 0.0f;
//	vBeamUp.z = 1.0f;
//
//	WzVector vCurrDir = vBeamToCam - vBeamDir;
//
//	float fAngle = acos(VectorDotProduct(&vBeamUp,&vCurrDir));
//
//	if (vCurrDir.x < 0)
//		fAngle = fAngle;
//	else if (vCurrDir.x > 0)
//		fAngle = -fAngle;
//	else
//		fAngle = 0;
//
//	WzMatrix matrix;
//
//	VectorNormalize(&vCurrDir,&vCurrDir);
//	MatrixRotation(&matrix,&vCurrDir,fAngle);
//
//	vTemp.x = 0.0f;
//	vTemp.y = 0.0f;
//	vTemp.z = 1.0f;
//	
//	float fHeight = m_Param.fHeight * fTrailRate;
//
//	vCurPos = vCurPos + vDirection * (fHeight / 2.0f);
//	vCurPos2 = vCurPos2 - vDirection * (fHeight / 2.0f);
//
//	vDestPos = vDestPos + vDirection * (fHeight / 2.0f);
//	vDestPos2 = vDestPos2 - vDirection * (fHeight / 2.0f);
//
//	vNewPos = vCurPos * (1.0f - fRate) + vDestPos * (fRate);
//	vNewPos2 = vCurPos2 * (1.0f - fRate) + vDestPos2 * (fRate);
//
//	vNewEndPos = vCurPos * (1.0f - fEndRate) + vDestPos * (fEndRate);
//	vNewEndPos2 = vCurPos2 * (1.0f - fEndRate) + vDestPos2 * (fEndRate);
//
//	if(m_pMesh)
//	{
//		m_pMesh->m_pVertices[0] = vNewEndPos;
//		m_pMesh->m_pVertices[1] = vNewPos;
//		m_pMesh->m_pVertices[2] = vNewEndPos2;
//		m_pMesh->m_pVertices[3] = vNewPos;
//		m_pMesh->m_pVertices[4] = vNewPos2;
//		m_pMesh->m_pVertices[5] = vNewEndPos2;
//
//		WzVector vArrowDir = vDestPos - vCurPos;
//
//		VectorNormalize(&vArrowDir,&vArrowDir);
//
//		WzVector vTrailOffset = vArrowDir * -0.3f;
//
//		for (int a = 0; a < 6; a++)
//		{
//			m_pMesh->m_pVertices[a] = m_pMesh->m_pVertices[a] + vTrailOffset;
//		}
//	}
//
//	if (m_pUnitDraw) 
//	{
//		m_pUnitDraw->SetPosition(vArrowPos,FALSE);
//		m_pUnitDraw->SetRotation(fArrowAngleX,0,fArrowAngleZ);
//	}
//
//	m_dwDelay += dwTick;
//
//	if (m_hEffect != INVALID_HANDLE_VALUE)
//	{
//		CEffectBody *pBody = g_pEffectManager->GetEffectPtrHandle(m_hEffect);
//		if(pBody)
//		{
//			WzVector vRot;
//			vRot.x = fArrowAngleX;
//			vRot.y = 0.0f;
//			vRot.z = fArrowAngleZ;
//			WzVector vScale;
//			vScale.x = 1.0f;
//			vScale.y = 1.0f;
//			vScale.z = 1.0f;
//
//			if(m_Param.vScale != c_ZeroVector)
//			{
//				vScale=m_Param.vScale;
//			}
//			pBody->SetResultMatrix(vArrowPos,vRot,vScale);
//
//			if(m_Param.bFadeOut)
//			{
//				DWORD alphaTime=m_Param.dwLifeTime/2;
//				if(!alphaTime)
//					alphaTime=1;
//
//				DWORD startAlphaTime=m_Param.dwLifeTime-alphaTime;
//
//				if(m_dwDelay>=startAlphaTime)
//				{
//					WzColor color=pBody->GetColor();
//
//					float fRate = (float)(m_dwDelay-startAlphaTime) / (float) alphaTime;		
//
//					float alphaRate=1.0f-fRate;
//
//					float fAlpha=Alpha_WzColor(color);
//
//					if(alphaRate<0.0f)
//					{
//						alphaRate=0.0f;
//					}
//
//					if(alphaRate>1.0f)
//					{
//						alphaRate=1.0f;
//					}
//
//					BYTE Alpha = (BYTE)(255 * alphaRate);
//					color = WzColor_RGBA(Red_WzColor(color),Green_WzColor(color),Blue_WzColor(color),Alpha);
//					pBody->SetColor(color);
//				}
//			}
//		}
//	}
//
//	m_bUpdated = TRUE;
//
//	// 발사체가 적에게 닿았으면	
//	if(fRate >= 1.0f)
//	{
//		if (m_Param.bCameraShake)
//		{
//			m_Param.bCameraShake=FALSE;
//			g_Camera.SetVibeType(Camera::VIBE_TYPE_VERTICAL);
//			g_Camera.StartVibration(50,0.07f);
//		}
//
//		ProcessDamageGroup();
//	}
//
//	// 라이프 타임이 있을 경우엔 대체 한다.
//	if(m_Param.dwLifeTime)
//	{
//		fRate = (float)m_dwDelay / (float) m_Param.dwLifeTime;		
//	}
//
//	//데미지 시간이 있을경우는 바로 데미지 처리 
//	if(m_Param.dwDamageTime)
//	{
//		if(m_dwDelay>m_Param.dwDamageTime)
//		{
//			//데미지가 처리 되었으면
//			if(ProcessDamageGroup())
//			{
//				if(m_Param.bDamageLoop)
//					m_Param.dwDamageTime=+m_dwDelay;
//				else 
//					m_Param.dwDamageTime=0;
//
//			}
//		}
//	}
//
//
//	if (fRate >= 1.0f)
//	{
//		// 데미지 처리를 한다.
//		ProcessDamageGroup();
//		if(m_Param.EndeffectID)
//		{
//			GlobalFunc::CreateEffectBeUpToOption(m_Param.EndeffectID,vArrowPos,m_Param.dwOwner,m_Param.dwTargetID);//!loop 이펙트 쓰지 말것    
//			m_Param.EndeffectID=0;
//		}
//
//		if( g_pHero->GetObjectKey() == m_Param.dwOwner )
//		{
//			GlobalFunc::NET_SEND_CG_ITEM_USE_SKILL_ACTION_SYN(eITEMTYPE_TORCH, vArrowPos);
//			m_Param.dwOwner =0;
//			
//			// 횃불 지속 이펙트 나오기 전에 4방향으로 타는 이펙트 연출
//			float fperRotAngle = 1.57f;
//			for( int i = 0; i < 4 ; i++ )
//			{
//				WzMatrix wmRot;
//				MatrixIdentity(&wmRot);
//				MatrixRotationZ(&wmRot, fperRotAngle * (float) i);
//
//				WzVector wvTransform;
//				WzVector wvOffset;
//				wvOffset.x = 2.0f;
//				wvOffset.y = 2.0f;
//				wvOffset.z = 0.0f;
//				VectorTransformCoord(&wvTransform, &wvOffset,&wmRot);
//
//				WzVector wvEffectPos;
//				wvEffectPos.x = wvTransform.x + vArrowPos.x;
//				wvEffectPos.y = wvTransform.y + vArrowPos.y;
//				wvEffectPos.z = wvTransform.z + vArrowPos.z;
//
//				WzVector wvRot;
//				wvRot.x = 0.0f;
//				wvRot.y = 0.0f;
//				wvRot.z = fperRotAngle * (float) i;
//
//				WzVector wvScale;
//				wvScale.x = 1.0f;
//				wvScale.y = 1.0f;
//				wvScale.z = 1.0f;
//				GlobalFunc::CreateEffectBeUpToOption(StrToWzID("E310"), wvEffectPos,wvScale,wvRot);
//			}			
//
//			GlobalFunc::CreateEffectBeUpToOption(StrToWzID("E310"), vArrowPos);
//
//
//		}
//		return TRUE;
//	}
//	return FALSE;
//}
//
//
//
////------------------------------------------------------------------------------
///**
//
//*/
//BOOL Projectile::ProcessAttachmentWeapon(DWORD dwTick)
//{
//	St_Camera * pCameraInfo = g_pSunRenderer->GetCameraInfo();
//
//	WzVector vTo    = pCameraInfo->m_wvTo;
//	WzVector vRight = pCameraInfo->m_wvRight;
//	WzVector vDown  = -pCameraInfo->m_wvUp;
//	WzVector vDir   = pCameraInfo->m_wvDir;
//
//	assert(m_Param.dwMoveTime);
//
//	if(m_Param.dwTargetID)//타겟 쫒아가기
//	{
//		Character *pTarget =  (Character *)g_ObjectManager.GetObject(m_Param.dwTargetID);
//		if (pTarget)
//		{
//			m_Param.vTargetPos = pTarget->GetArrowTargetPos();
//
//		}
//	}
//
//	float fRate;
//
//	if(m_Param.dwMoveTime)
//		fRate= (float)m_dwDelay / (float) m_Param.dwMoveTime;		
//	else
//		fRate=0.0f;
//
//	float fTrailRate = 1.0f - fRate; 
//	float fEndRate;
//
//	fRate *= m_Param.fSpeed;
//	fEndRate = fRate - m_Param.fTrailLength;
//
//	if (fRate >= 1.0f)
//	{
//		fRate = 1.0f;
//	}
//
//	if (fEndRate <= 0.0f)
//	{
//		fEndRate = 0.0f;
//	}
//
//	if (fEndRate >= 1.0f)
//	{
//		fEndRate = 1.0f;
//	}
//
//	WzVector vArrowPos;
//	WzVector vDest,vTemp;
//	float fArrowAngleZ,fArrowAngleX;
//
//	vDest =  m_Param.vTargetPos - m_Param.vStartPos;
//	vTemp = vDest;
//	vTemp.z = 0.0f;
//
//	float fLength;
//
//	if (vTemp.x == 0.0f && vTemp.y == 0.0f)
//	{
//		fLength = 0.0f;
//	}
//	else
//	{
//		fLength = VectorLength(&vTemp);
//	}
//
//	fArrowAngleX = -atan2(vDest.z,fLength);
//	fArrowAngleZ = Math_GetAngleFromVector(&vDest);
//	vArrowPos = m_Param.vStartPos * (1.0f - fRate) + m_Param.vTargetPos * (fRate);
//
//	WzVector vCurPos,vCurPos2,vDestPos,vDestPos2,vNewPos,vNewPos2,vNewEndPos,vNewEndPos2;
//	vCurPos = vCurPos2 = m_Param.vStartPos;
//	vDestPos = vDestPos2 = m_Param.vTargetPos;
//
//	VectorNormalize(&vDest,&vDest);
//	WzVector vDirection;
//
//	VectorCrossProduct(&vDirection,&vDest,&vDir);
//	VectorNormalize(&vDirection,&vDirection);
//
//	WzVector CamPos = pCameraInfo->m_wvFrom;
//	WzVector BeamSrc = m_Param.vStartPos;
//	WzVector BeamDst = m_Param.vTargetPos;
//
//	WzVector vBeamDir = BeamDst - BeamSrc;
//	VectorNormalize(&vBeamDir,&vBeamDir);
//	WzVector vBeamToCam = CamPos - BeamSrc;
//	VectorNormalize(&vBeamToCam,&vBeamToCam);		
//	WzVector vBeamUp;
//	vBeamUp.x = 0.0f;
//	vBeamUp.y = 0.0f;
//	vBeamUp.z = 1.0f;
//
//	WzVector vCurrDir = vBeamToCam - vBeamDir;
//
//	float fAngle = acos(VectorDotProduct(&vBeamUp,&vCurrDir));
//
//	if (vCurrDir.x < 0)
//		fAngle = fAngle;
//	else if (vCurrDir.x > 0)
//		fAngle = -fAngle;
//	else
//		fAngle = 0;
//
//	WzMatrix matrix;
//
//	VectorNormalize(&vCurrDir,&vCurrDir);
//	MatrixRotation(&matrix,&vCurrDir,fAngle);
//
//	vTemp.x = 0.0f;
//	vTemp.y = 0.0f;
//	vTemp.z = 1.0f;
//
//	float fHeight = m_Param.fHeight * fTrailRate;
//
//	vCurPos = vCurPos + vDirection * (fHeight / 2.0f);
//	vCurPos2 = vCurPos2 - vDirection * (fHeight / 2.0f);
//
//	vDestPos = vDestPos + vDirection * (fHeight / 2.0f);
//	vDestPos2 = vDestPos2 - vDirection * (fHeight / 2.0f);
//
//	vNewPos = vCurPos * (1.0f - fRate) + vDestPos * (fRate);
//	vNewPos2 = vCurPos2 * (1.0f - fRate) + vDestPos2 * (fRate);
//
//	vNewEndPos = vCurPos * (1.0f - fEndRate) + vDestPos * (fEndRate);
//	vNewEndPos2 = vCurPos2 * (1.0f - fEndRate) + vDestPos2 * (fEndRate);
//
//
//	if (m_pUnitDraw) 
//	{
//		m_fAngleZ+=(WZ_PI/170.0f* dwTick);
//
//		// 뉘어 돌리기
//		ClampAngle(&m_fAngleZ);
//		m_pUnitDraw->SetPosition(vArrowPos,FALSE);
//		m_pUnitDraw->SetRotation(0.0f,WZ_PI / 2.0f,m_fAngleZ);
//		m_pUnitDraw->SetScale(1.2f,1.2f,1.2f);
//	}
//
//	m_dwDelay += dwTick;
//
//	if (m_hEffect != INVALID_HANDLE_VALUE)
//	{
//		CEffectBody *pBody = g_pEffectManager->GetEffectPtrHandle(m_hEffect);
//		if(pBody)
//		{
//			WzVector vRot;
//			vRot.x = fArrowAngleX;
//			vRot.y = 0.0f;
//			vRot.z = fArrowAngleZ;
//			WzVector vScale;
//			vScale.x = 1.0f;
//			vScale.y = 1.0f;
//			vScale.z = 1.0f;
//
//			if(m_Param.vScale != c_ZeroVector)
//			{
//				vScale=m_Param.vScale;
//			}
//			pBody->SetResultMatrix(vArrowPos,vRot,vScale);
//
//			if(m_Param.bFadeOut)
//			{
//				DWORD alphaTime=m_Param.dwLifeTime/2;
//				if(!alphaTime)
//					alphaTime=1;
//
//				DWORD startAlphaTime=m_Param.dwLifeTime-alphaTime;
//
//				if(m_dwDelay>=startAlphaTime)
//				{
//					WzColor color=pBody->GetColor();
//
//					float fRate = (float)(m_dwDelay-startAlphaTime) / (float) alphaTime;		
//
//					float alphaRate=1.0f-fRate;
//
//					float fAlpha=Alpha_WzColor(color);
//
//					if(alphaRate<0.0f)
//					{
//						alphaRate=0.0f;
//					}
//
//					if(alphaRate>1.0f)
//					{
//						alphaRate=1.0f;
//					}
//
//					BYTE Alpha = (BYTE)(255 * alphaRate);
//					color = WzColor_RGBA(Red_WzColor(color),Green_WzColor(color),Blue_WzColor(color),Alpha);
//					pBody->SetColor(color);
//
//				}
//			}
//
//
//		}
//	}
//
//	m_bUpdated = TRUE;
//
//	// 발사체가 적에게 닿았으면	
//	if(fRate >= 1.0f)
//	{
//		if (m_Param.bCameraShake)
//		{
//			m_Param.bCameraShake=FALSE;
//			g_Camera.SetVibeType(Camera::VIBE_TYPE_VERTICAL);
//			g_Camera.StartVibration(50,0.07f);
//		}
//
//		ProcessDamageGroup();
//	}
//
//	// 라이프 타임이 있을 경우엔 대체 한다.
//	if(m_Param.dwLifeTime)
//	{
//		fRate = (float)m_dwDelay / (float) m_Param.dwLifeTime;		
//	}
//
//	//데미지 시간이 있을경우는 바로 데미지 처리 
//	if(m_Param.dwDamageTime)
//	{
//		if(m_dwDelay>m_Param.dwDamageTime)
//		{
//
//			//데미지가 처리 되었으면
//			if(ProcessDamageGroup())
//			{
//				if(m_Param.bDamageLoop)
//					m_Param.dwDamageTime=+m_dwDelay;
//				else 
//					m_Param.dwDamageTime=0;
//
//			}
//		}
//	}
//
//
//	if (fRate >= 1.0f)
//	{
//		// 데미지 처리를 한다.
//		ProcessDamageGroup();
//		if(m_Param.EndeffectID)
//		{
//			GlobalFunc::CreateEffectBeUpToOption(m_Param.EndeffectID,vArrowPos,m_Param.dwOwner,m_Param.dwTargetID);//!loop 이펙트 쓰지 말것    
//
//			m_Param.EndeffectID=0;
//		}
//		return TRUE;
//	}
//	return FALSE;
//}
//
//
////------------------------------------------------------------------------------
///**
//	Todo:정리 할것 
//*/
//BOOL Projectile::ProcessCommon(DWORD dwTick)
//{
//	St_Camera * pCameraInfo = g_pSunRenderer->GetCameraInfo();
//
//	WzVector vTo    = pCameraInfo->m_wvTo;
//	WzVector vRight = pCameraInfo->m_wvRight;
//	WzVector vDown  = -pCameraInfo->m_wvUp;
//	WzVector vDir   = pCameraInfo->m_wvDir;
//
//	if(m_Param.dwTargetID)//타겟 쫒아가기
//	{
//		Character *pTarget =  (Character *)g_ObjectManager.GetObject(m_Param.dwTargetID);
//		if (pTarget)
//		{
//			m_Param.vTargetPos = pTarget->GetArrowTargetPos();
//
//		}
//	}
//
//	float fRate;
//
//	if(m_Param.dwMoveTime)
//		fRate= (float)m_dwDelay / (float) m_Param.dwMoveTime;		
//	else
//		fRate=0.0f;
//
//	float fTrailRate = 1.0f - fRate; 
//	float fEndRate;
//
//	m_Param.fSpeed=max(1.0f,m_Param.fSpeed);
//
//	fRate *= m_Param.fSpeed;
//	fEndRate = fRate - m_Param.fTrailLength;
//
//	if (fRate >= 1.0f)
//	{
//		fRate = 1.0f;
//	}
//
//	if (fEndRate <= 0.0f)
//	{
//		fEndRate = 0.0f;
//	}
//
//	if (fEndRate >= 1.0f)
//	{
//		fEndRate = 1.0f;
//	}
//
//	WzVector vArrowPos;
//	WzVector vDest,vTemp;
//	float fArrowAngleZ,fArrowAngleX;
//
//	vDest =  m_Param.vTargetPos - m_Param.vStartPos;
//	vTemp = vDest;
//	vTemp.z = 0.0f;
//
//	float fLength;
//
//	if (vTemp.x == 0.0f && vTemp.y == 0.0f)
//	{
//		fLength = 0.0f;
//	}
//	else
//	{
//		fLength = VectorLength(&vTemp);
//	}
//
//	fArrowAngleX = -atan2(vDest.z,fLength);
//	fArrowAngleZ = Math_GetAngleFromVector(&vDest);
//	vArrowPos = m_Param.vStartPos * (1.0f - fRate) + m_Param.vTargetPos * (fRate);
//
//	if(m_Param.bNotMove)
//	{
//		vArrowPos= m_Param.vStartPos;
//	}
//
//	WzVector vCurPos,vCurPos2,vDestPos,vDestPos2,vNewPos,vNewPos2,vNewEndPos,vNewEndPos2;
//	vCurPos = vCurPos2 = m_Param.vStartPos;
//	vDestPos = vDestPos2 = m_Param.vTargetPos;
//
//	VectorNormalize(&vDest,&vDest);
//	WzVector vDirection;
//
//	VectorCrossProduct(&vDirection,&vDest,&vDir);
//	VectorNormalize(&vDirection,&vDirection);
//
//	WzVector CamPos = pCameraInfo->m_wvFrom;
//	WzVector BeamSrc = m_Param.vStartPos;
//	WzVector BeamDst = m_Param.vTargetPos;
//
//	WzVector vBeamDir = BeamDst - BeamSrc;
//	VectorNormalize(&vBeamDir,&vBeamDir);
//	WzVector vBeamToCam = CamPos - BeamSrc;
//	VectorNormalize(&vBeamToCam,&vBeamToCam);		
//	WzVector vBeamUp;
//	vBeamUp.x = 0.0f;
//	vBeamUp.y = 0.0f;
//	vBeamUp.z = 1.0f;
//
//	WzVector vCurrDir = vBeamToCam - vBeamDir;
//
//	float fAngle = acos(VectorDotProduct(&vBeamUp,&vCurrDir));
//
//	if (vCurrDir.x < 0)
//		fAngle = fAngle;
//	else if (vCurrDir.x > 0)
//		fAngle = -fAngle;
//	else
//		fAngle = 0;
//
//	WzMatrix matrix;
//
//	VectorNormalize(&vCurrDir,&vCurrDir);
//	MatrixRotation(&matrix,&vCurrDir,fAngle);
//
//	vTemp.x = 0.0f;
//	vTemp.y = 0.0f;
//	vTemp.z = 1.0f;
//
//	float fHeight = m_Param.fHeight * fTrailRate;
//
//	vCurPos = vCurPos + vDirection * (fHeight / 2.0f);
//	vCurPos2 = vCurPos2 - vDirection * (fHeight / 2.0f);
//
//	vDestPos = vDestPos + vDirection * (fHeight / 2.0f);
//	vDestPos2 = vDestPos2 - vDirection * (fHeight / 2.0f);
//
//	vNewPos = vCurPos * (1.0f - fRate) + vDestPos * (fRate);
//	vNewPos2 = vCurPos2 * (1.0f - fRate) + vDestPos2 * (fRate);
//
//	vNewEndPos = vCurPos * (1.0f - fEndRate) + vDestPos * (fEndRate);
//	vNewEndPos2 = vCurPos2 * (1.0f - fEndRate) + vDestPos2 * (fEndRate);
//
//
//	if(m_pMesh)
//	{
//		m_pMesh->m_pVertices[0] = vNewEndPos;
//		m_pMesh->m_pVertices[1] = vNewPos;
//		m_pMesh->m_pVertices[2] = vNewEndPos2;
//		m_pMesh->m_pVertices[3] = vNewPos;
//		m_pMesh->m_pVertices[4] = vNewPos2;
//		m_pMesh->m_pVertices[5] = vNewEndPos2;
//
//		WzVector vArrowDir = vDestPos - vCurPos;
//
//		VectorNormalize(&vArrowDir,&vArrowDir);
//
//		WzVector vTrailOffset = vArrowDir * -0.3f;
//
//		for (int a = 0; a < 6; a++)
//		{
//			m_pMesh->m_pVertices[a] = m_pMesh->m_pVertices[a] + vTrailOffset;
//
//		}
//	}
//
//	if (m_pUnitDraw) 
//	{
//		m_pUnitDraw->SetPosition(vArrowPos,FALSE);
//		m_pUnitDraw->SetRotation(fArrowAngleX,0,fArrowAngleZ);
//	}
//
//	m_dwDelay += dwTick;
//
//	if (m_hEffect != INVALID_HANDLE_VALUE)
//	{
//		CEffectBody *pBody = g_pEffectManager->GetEffectPtrHandle(m_hEffect);
//		if(pBody)
//		{
//			WzVector vRot;
//			vRot.x = fArrowAngleX;
//			vRot.y = 0.0f;
//			vRot.z = fArrowAngleZ;
//			WzVector vScale;
//			vScale.x = 1.0f;
//			vScale.y = 1.0f;
//			vScale.z = 1.0f;
//
//			if(m_Param.vScale != c_ZeroVector)
//			{
//				vScale=m_Param.vScale;
//			}
//
//			if(m_Param.vDir != c_ZeroVector)
//			{
//				vRot=m_Param.vDir;
//			}
//
//			pBody->SetResultMatrix(vArrowPos,vRot,vScale);
//
//			if(m_Param.bFadeOut)
//			{
//				DWORD alphaTime=m_Param.dwLifeTime/2;
//				if(!alphaTime)
//					alphaTime=1;
//
//				DWORD startAlphaTime=m_Param.dwLifeTime-alphaTime;
//
//				if(m_dwDelay>=startAlphaTime)
//				{
//					WzColor color=pBody->GetColor();
//
//					float fRate = (float)(m_dwDelay-startAlphaTime) / (float) alphaTime;		
//
//					float alphaRate=1.0f-fRate;
//
//					float fAlpha=Alpha_WzColor(color);
//				
//					if(alphaRate<0.0f)
//					{
//						alphaRate=0.0f;
//					}
//
//					if(alphaRate>1.0f)
//					{
//						alphaRate=1.0f;
//					}
//
//					BYTE Alpha = (BYTE)(255 * alphaRate);
//					color = WzColor_RGBA(Red_WzColor(color),Green_WzColor(color),Blue_WzColor(color),Alpha);
//					pBody->SetColor(color);
//				}
//			}
//
//
//		}
//	}
//
//	m_bUpdated = TRUE;
//
//	// 발사체가 적에게 닿았으면	
//	if(fRate >= 1.0f)
//	{
//		if (m_Param.bCameraShake)
//		{
//			m_Param.bCameraShake=FALSE;
//			g_Camera.SetVibeType(Camera::VIBE_TYPE_VERTICAL);
//			g_Camera.StartVibration(50,0.07f);
//		}
//
//		if(m_Param.EndeffectID)
//		{
//
//			GlobalFunc::CreateEffectBeUpToOption(m_Param.EndeffectID,vArrowPos,m_Param.dwOwner,m_Param.dwTargetID);//!loop 이펙트 쓰지 말것    
//
//			m_Param.EndeffectID=0;
//		}
//
//		ProcessDamageGroup();
//	}
//
//	// 라이프 타임이 있을 경우엔 대체 한다.
//	if(m_Param.dwLifeTime)
//	{
//		fRate = (float)m_dwDelay / (float) m_Param.dwLifeTime;		
//	}
//
//	//데미지 시간이 있을경우는 바로 데미지 처리 
//	if(m_Param.dwDamageTime)
//	{
//		if(m_dwDelay>m_Param.dwDamageTime)
//		{
//			
//			//데미지가 처리 되었으면
//			if(ProcessDamageGroup())
//			{
//				if(m_Param.EndeffectID)
//				{
//
//					GlobalFunc::CreateEffectBeUpToOption(m_Param.EndeffectID,vArrowPos,m_Param.dwOwner,m_Param.dwTargetID);//!loop 이펙트 쓰지 말것    
//
//					m_Param.EndeffectID=0;
//				}
//				
//				if(m_Param.bDamageLoop)
//					m_Param.dwDamageTime=+m_dwDelay;
//				else 
//					m_Param.dwDamageTime=0;
//
//			}
//		}
//	}
//
//	
//	if (fRate >= 1.0f)
//	{
//		// 데미지 처리를 한다.
//		ProcessDamageGroup();
//		if(m_Param.EndeffectID)
//		{
//			GlobalFunc::CreateEffectBeUpToOption(m_Param.EndeffectID,vArrowPos,m_Param.dwOwner,m_Param.dwTargetID);//!loop 이펙트 쓰지 말것    
//			m_Param.EndeffectID=0;
//
//		}
//		return TRUE;
//	}
//	return FALSE;
//}
//
////------------------------------------------------------------------------------
///**
// 
//*/
//BOOL Projectile::ProcessAirBlow(DWORD dwTick)
//{
//
//	Character *pTarget=(Character *)g_ObjectManager.GetObject(m_Param.AIR_BLOW.dwTargetObjectKey);
//
//	if(pTarget)//위치 갱신 
//	{
//		m_Param.vTargetPos=pTarget->GetVisiblePos();
//		m_Param.vTargetPos.z+=pTarget->GetHeight();
//	}
//
//	St_Camera * pCameraInfo = g_pSunRenderer->GetCameraInfo();
//
//	WzVector vTo    = pCameraInfo->m_wvTo;
//	WzVector vRight = pCameraInfo->m_wvRight;
//	WzVector vDown  = -pCameraInfo->m_wvUp;
//	WzVector vDir   = pCameraInfo->m_wvDir;
//
//	assert(m_Param.dwMoveTime);
//
//	float fRate;
//
//	if(m_Param.dwMoveTime)
//		fRate= (float)m_dwDelay / (float) m_Param.dwMoveTime;		
//	else
//		fRate=0.0f;
//
//	float fTrailRate = 1.0f - fRate; 
//	float fEndRate;
//
//	fRate *= m_Param.fSpeed;
//	fEndRate = fRate - m_Param.fTrailLength;
//
//	if (fRate >= 1.0f)
//	{
//		fRate = 1.0f;
//	}
//
//	if (fEndRate <= 0.0f)
//	{
//		fEndRate = 0.0f;
//	}
//
//	if (fEndRate >= 1.0f)
//	{
//		fEndRate = 1.0f;
//	}
//
//	WzVector vArrowPos;
//	WzVector vDest,vTemp;
//	float fArrowAngleZ,fArrowAngleX;
//
//	vDest =  m_Param.vTargetPos - m_Param.vStartPos;
//	vTemp = vDest;
//	vTemp.z = 0.0f;
//
//	float fLength;
//
//	if (vTemp.x == 0.0f && vTemp.y == 0.0f)
//	{
//		fLength = 0.0f;
//	}
//	else
//	{
//		fLength = VectorLength(&vTemp);
//	}
//
//	fArrowAngleX = -atan2(vDest.z,fLength);
//	fArrowAngleZ = Math_GetAngleFromVector(&vDest);
//	vArrowPos = m_Param.vStartPos * (1.0f - fRate) + m_Param.vTargetPos * (fRate);
//
//	WzVector vCurPos,vCurPos2,vDestPos,vDestPos2,vNewPos,vNewPos2,vNewEndPos,vNewEndPos2;
//	vCurPos = vCurPos2 = m_Param.vStartPos;
//	vDestPos = vDestPos2 = m_Param.vTargetPos;
//
//	VectorNormalize(&vDest,&vDest);
//	WzVector vDirection;
//
//	VectorCrossProduct(&vDirection,&vDest,&vDir);
//	VectorNormalize(&vDirection,&vDirection);
//
//	WzVector CamPos = pCameraInfo->m_wvFrom;
//	WzVector BeamSrc = m_Param.vStartPos;
//	WzVector BeamDst = m_Param.vTargetPos;
//
//	WzVector vBeamDir = BeamDst - BeamSrc;
//	VectorNormalize(&vBeamDir,&vBeamDir);
//	WzVector vBeamToCam = CamPos - BeamSrc;
//	VectorNormalize(&vBeamToCam,&vBeamToCam);		
//	WzVector vBeamUp;
//	vBeamUp.x = 0.0f;
//	vBeamUp.y = 0.0f;
//	vBeamUp.z = 1.0f;
//
//	WzVector vCurrDir = vBeamToCam - vBeamDir;
//
//	float fAngle = acos(VectorDotProduct(&vBeamUp,&vCurrDir));
//
//	if (vCurrDir.x < 0)
//		fAngle = fAngle;
//	else if (vCurrDir.x > 0)
//		fAngle = -fAngle;
//	else
//		fAngle = 0;
//
//	WzMatrix matrix;
//
//	VectorNormalize(&vCurrDir,&vCurrDir);
//	MatrixRotation(&matrix,&vCurrDir,fAngle);
//
//	vTemp.x = 0.0f;
//	vTemp.y = 0.0f;
//	vTemp.z = 1.0f;
//
//	float fHeight = m_Param.fHeight * fTrailRate;
//
//	vCurPos = vCurPos + vDirection * (fHeight / 2.0f);
//	vCurPos2 = vCurPos2 - vDirection * (fHeight / 2.0f);
//
//	vDestPos = vDestPos + vDirection * (fHeight / 2.0f);
//	vDestPos2 = vDestPos2 - vDirection * (fHeight / 2.0f);
//
//	vNewPos = vCurPos * (1.0f - fRate) + vDestPos * (fRate);
//	vNewPos2 = vCurPos2 * (1.0f - fRate) + vDestPos2 * (fRate);
//
//	vNewEndPos = vCurPos * (1.0f - fEndRate) + vDestPos * (fEndRate);
//	vNewEndPos2 = vCurPos2 * (1.0f - fEndRate) + vDestPos2 * (fEndRate);
//
//
//	if(m_pMesh)
//	{
//		m_pMesh->m_pVertices[0] = vNewEndPos;
//		m_pMesh->m_pVertices[1] = vNewPos;
//		m_pMesh->m_pVertices[2] = vNewEndPos2;
//		m_pMesh->m_pVertices[3] = vNewPos;
//		m_pMesh->m_pVertices[4] = vNewPos2;
//		m_pMesh->m_pVertices[5] = vNewEndPos2;
//
//
//
//		WzVector vArrowDir = vDestPos - vCurPos;
//
//		VectorNormalize(&vArrowDir,&vArrowDir);
//
//		WzVector vTrailOffset = vArrowDir * -0.3f;
//
//		for (int a = 0; a < 6; a++)
//		{
//			m_pMesh->m_pVertices[a] = m_pMesh->m_pVertices[a] + vTrailOffset;
//
//		}
//	}
//
//	if (m_pUnitDraw) 
//	{
//		m_pUnitDraw->SetPosition(vArrowPos,FALSE);
//		m_pUnitDraw->SetRotation(fArrowAngleX,0,fArrowAngleZ);
//	}
//
//	m_dwDelay += dwTick;
//
//	if (m_hEffect != INVALID_HANDLE_VALUE)
//	{
//		CEffectBody *pBody = g_pEffectManager->GetEffectPtrHandle(m_hEffect);
//		if(pBody)
//		{
//			WzVector vRot;
//			vRot.x = fArrowAngleX;
//			vRot.y = 0.0f;
//			vRot.z = fArrowAngleZ;
//			WzVector vScale;
//			vScale.x = 1.0f;
//			vScale.y = 1.0f;
//			vScale.z = 1.0f;
//
//			if(m_Param.vScale != c_ZeroVector)
//			{
//				vScale=m_Param.vScale;
//			}
//			pBody->SetResultMatrix(vArrowPos,vRot,vScale);
//
//			if(m_Param.bFadeOut)
//			{
//				DWORD alphaTime=m_Param.dwLifeTime/2;
//				if(!alphaTime)
//					alphaTime=1;
//
//				DWORD startAlphaTime=m_Param.dwLifeTime-alphaTime;
//
//				if(m_dwDelay>=startAlphaTime)
//				{
//					WzColor color=pBody->GetColor();
//
//					float fRate = (float)(m_dwDelay-startAlphaTime) / (float) alphaTime;		
//
//					float alphaRate=1.0f-fRate;
//
//					float fAlpha=Alpha_WzColor(color);
//
//					if(alphaRate<0.0f)
//					{
//						alphaRate=0.0f;
//					}
//
//					if(alphaRate>1.0f)
//					{
//						alphaRate=1.0f;
//					}
//
//					BYTE Alpha = (BYTE)(255 * alphaRate);
//					color = WzColor_RGBA(Red_WzColor(color),Green_WzColor(color),Blue_WzColor(color),Alpha);
//				}
//			}
//
//
//		}
//	}
//
//	m_bUpdated = TRUE;
//
//	// 발사체가 적에게 닿았으면	
//	if(fRate >= 1.0f)
//	{
//		if (m_Param.bCameraShake)
//		{
//			m_Param.bCameraShake=FALSE;
//			g_Camera.SetVibeType(Camera::VIBE_TYPE_VERTICAL);
//			g_Camera.StartVibration(50,0.07f);
//		}
//
//		ProcessDamageGroup();
//	}
//
//	// 라이프 타임이 있을 경우엔 대체 한다.
//	if(m_Param.dwLifeTime)
//	{
//		fRate = (float)m_dwDelay / (float) m_Param.dwLifeTime;		
//	}
//
//	//데미지 시간이 있을경우는 바로 데미지 처리 
//	if(m_Param.dwDamageTime)
//	{
//		if(m_dwDelay>m_Param.dwDamageTime)
//		{
//			//데미지가 처리 되었으면
//			if(ProcessDamageGroup())
//			{
//				if(m_Param.bDamageLoop)
//					m_Param.dwDamageTime=+m_dwDelay;
//				else 
//					m_Param.dwDamageTime=0;
//			}
//		}
//	}
//
//
//	if (fRate >= 1.0f)
//	{
//		// 데미지 처리를 한다.
//		ProcessDamageGroup();
//		if(m_Param.EndeffectID)
//		{
//			GlobalFunc::CreateEffectBeUpToOption(m_Param.EndeffectID,vArrowPos,m_Param.dwOwner,m_Param.dwTargetID);//!loop 이펙트 쓰지 말것    
//
//			m_Param.EndeffectID=0;
//		}
//
//		if(pTarget&&(pTarget->GetCurState()==STATE::AIR||pTarget->GetCurState()==STATE::GETUP))
//		{
//			pTarget->ToAirInfo(&m_Param.AIR_BLOW.TOAIR);
//		}
//
//		return TRUE;
//	}
//	return FALSE;
//}
////------------------------------------------------------------------------------
///**
//*/
//BOOL Projectile::ProcessSwordTrail(DWORD dwTick)
//{
//	m_dwDelay += dwTick;
//
//	St_Camera * pCameraInfo = g_pSunRenderer->GetCameraInfo();
//
//	WzVector vTo    = pCameraInfo->m_wvTo;
//	WzVector vRight = pCameraInfo->m_wvRight;
//	WzVector vDown  = -pCameraInfo->m_wvUp;
//	WzVector vDir   = pCameraInfo->m_wvDir;
//
//	assert(m_Param.dwMoveTime);
//
//	float fRate;
//
//	if(m_Param.dwMoveTime)
//		fRate= (float)m_dwDelay / (float) m_Param.dwMoveTime;		
//	else
//		fRate=0.0f;
//
//	float fTrailRate = 1.0f - fRate; 
//	float fEndRate;
//
//	fRate *= m_Param.fSpeed;
//	fEndRate = fRate - m_Param.fTrailLength;
//
//	if (fRate >= 1.0f)
//	{
//		fRate = 1.0f;
//	}
//
//	if (fEndRate <= 0.0f)
//	{
//		fEndRate = 0.0f;
//	}
//
//	if (fEndRate >= 1.0f)
//	{
//		fEndRate = 1.0f;
//	}
//
//	WzVector vArrowPos;
//	WzVector vDest,vTemp;
//	float fArrowAngleZ,fArrowAngleX;
//
//	vDest =  m_Param.vTargetPos - m_Param.vStartPos;
//	vTemp = vDest;
//	vTemp.z = 0.0f;
//
//	float fLength;
//
//	if (vTemp.x == 0.0f && vTemp.y == 0.0f)
//	{
//		fLength = 0.0f;
//	}
//	else
//	{
//		fLength = VectorLength(&vTemp);
//	}
//
//	fArrowAngleX = -atan2(vDest.z,fLength);
//	fArrowAngleZ = Math_GetAngleFromVector(&vDest);
//	vArrowPos = m_Param.vStartPos * (1.0f - fRate) + m_Param.vTargetPos * (fRate);
//
//	WzVector vCurPos,vCurPos2,vDestPos,vDestPos2,vNewPos,vNewPos2,vNewEndPos,vNewEndPos2;
//	vCurPos = vCurPos2 = m_Param.vStartPos;
//	vDestPos = vDestPos2 = m_Param.vTargetPos;
//
//	VectorNormalize(&vDest,&vDest);
//	WzVector vDirection;
//
//	VectorCrossProduct(&vDirection,&vDest,&vDir);
//	VectorNormalize(&vDirection,&vDirection);
//
//	WzVector CamPos = pCameraInfo->m_wvFrom;
//	WzVector BeamSrc = m_Param.vStartPos;
//	WzVector BeamDst = m_Param.vTargetPos;
//
//	WzVector vBeamDir = BeamDst - BeamSrc;
//	VectorNormalize(&vBeamDir,&vBeamDir);
//	WzVector vBeamToCam = CamPos - BeamSrc;
//	VectorNormalize(&vBeamToCam,&vBeamToCam);		
//	WzVector vBeamUp;
//	vBeamUp.x = 0.0f;
//	vBeamUp.y = 0.0f;
//	vBeamUp.z = 1.0f;
//
//	WzVector vCurrDir = vBeamToCam - vBeamDir;
//
//	float fAngle = acos(VectorDotProduct(&vBeamUp,&vCurrDir));
//
//	if (vCurrDir.x < 0)
//		fAngle = fAngle;
//	else if (vCurrDir.x > 0)
//		fAngle = -fAngle;
//	else
//		fAngle = 0;
//
//	WzMatrix matrix;
//
//	VectorNormalize(&vCurrDir,&vCurrDir);
//	MatrixRotation(&matrix,&vCurrDir,fAngle);
//
//	vTemp.x = 0.0f;
//	vTemp.y = 0.0f;
//	vTemp.z = 1.0f;
//
//	float fHeight = m_Param.fHeight * fTrailRate;
//
//	vCurPos = vCurPos + vDirection * (fHeight / 2.0f);
//	vCurPos2 = vCurPos2 - vDirection * (fHeight / 2.0f);
//
//	vDestPos = vDestPos + vDirection * (fHeight / 2.0f);
//	vDestPos2 = vDestPos2 - vDirection * (fHeight / 2.0f);
//
//	vNewPos = vCurPos * (1.0f - fRate) + vDestPos * (fRate);
//	vNewPos2 = vCurPos2 * (1.0f - fRate) + vDestPos2 * (fRate);
//
//	vNewEndPos = vCurPos * (1.0f - fEndRate) + vDestPos * (fEndRate);
//	vNewEndPos2 = vCurPos2 * (1.0f - fEndRate) + vDestPos2 * (fEndRate);
//
//
//	
//
//	m_bUpdated = TRUE;
//	
//	CEffectBody *pBody = g_pEffectManager->GetEffectPtrHandle(m_hEffect);
//	if(pBody)
//	{
//		m_Param.SWORD_TRAIL.vEndPos =vArrowPos;
//		m_Param.SWORD_TRAIL.vStartPos =m_Param.SWORD_TRAIL.vEndPos;
//		m_Param.SWORD_TRAIL.vStartPos+=m_Param.SWORD_TRAIL.vAdditionalStartPos;
//		m_Param.SWORD_TRAIL.vEndPos+=m_Param.SWORD_TRAIL.vAdditionalEndPos;
//
//
//		WzVector dir=m_Param.SWORD_TRAIL.vDir;
//		//Todo:컨버트 함수 만들자 귀찮다.
//		vector3 dir2; 
//		dir2.x=dir.x;
//		dir2.y=dir.z;
//		dir2.z=dir.y;
//		dir2.norm(); //z축
//
//		vector3 up;
//		up.set(0.0f,1.0f,0.0f);//y축
//
//		vector3 temp=up*dir2;//x축
//		WzVector inc;
//		inc.x=temp.x;
//		inc.y=temp.z;
//		inc.z=temp.y;
//	
//
//		m_Param.SWORD_TRAIL.vEndPos +=inc;
//		m_Param.SWORD_TRAIL.vStartPos-=inc;
//
//		CEElementFlatChain* pFChain = pBody->GetFlatChainPtr();
//        if( pFChain )
//        {
//			pFChain->SetPosition(m_Param.SWORD_TRAIL.vStartPos,m_Param.SWORD_TRAIL.vEndPos);
//        }
//
//	}
//
//
//	if (m_Param.SWORD_TRAIL.dwTrailEffectDelay > 0)
//	{
//		if (m_dwDelay >= m_dwTrailEffectCount * m_Param.SWORD_TRAIL.dwTrailEffectDelay)
//		{
//			WzVector vResult;
//			
//			g_pMap->GetTerrainHeight(&vArrowPos,&vResult);
//			
//
//
//			GlobalFunc::CreateEffectBeUpToOption(StrToWzID("A030"),vResult,m_Param.dwOwner,m_Param.dwTargetID);//!loop 이펙트 쓰지 말것    
//
//
//			m_dwTrailEffectCount++;
//		}
//	}
//
//	// 발사체가 적에게 닿았으면	
//	if(fRate >= 1.0f)
//	{
//		if (m_Param.bCameraShake)
//		{
//			m_Param.bCameraShake=FALSE;
//			g_Camera.SetVibeType(Camera::VIBE_TYPE_VERTICAL);
//			g_Camera.StartVibration(50,0.07f);
//		}
//
//		ProcessDamageGroup();
//	}
//
//	// 라이프 타임이 있을 경우엔 대체 한다.
//	if(m_Param.dwLifeTime)
//	{
//		fRate = (float)m_dwDelay / (float) m_Param.dwLifeTime;		
//	}
//
//	//데미지 시간이 있을경우는 바로 데미지 처리 
//	if(m_Param.dwDamageTime)
//	{
//		if(m_dwDelay>m_Param.dwDamageTime)
//		{
//
//			//데미지가 처리 되었으면
//			if(ProcessDamageGroup())
//			{
//				if(m_Param.bDamageLoop)
//					m_Param.dwDamageTime=+m_dwDelay;
//				else 
//					m_Param.dwDamageTime=0;
//			}
//		}
//	}
//
//	if (fRate >= 1.0f)
//	{
//		// 데미지 처리를 한다.
//		ProcessDamageGroup();
//		return TRUE;
//	}
//
//
//	return FALSE;
//}
//
////------------------------------------------------------------------------------
///**
//*/
//BOOL Projectile::ProcessSwordWave(DWORD dwTick)
//{
//	St_Camera * pCameraInfo = g_pSunRenderer->GetCameraInfo();
//
//	WzVector vTo    = pCameraInfo->m_wvTo;
//	WzVector vRight = pCameraInfo->m_wvRight;
//	WzVector vDown  = -pCameraInfo->m_wvUp;
//	WzVector vDir   = pCameraInfo->m_wvDir;	
//
//	assert(m_Param.dwMoveTime);
//
//	float fRate;
//
//	if(m_Param.dwMoveTime)
//		fRate= (float)m_dwDelay / (float) m_Param.dwMoveTime;		
//	else
//		fRate=0.0f;
//
//	float fTrailRate = 1.0f - fRate; 
//	float fEndRate;
//
//	fRate *= m_Param.fSpeed;
//	fEndRate = fRate - m_Param.fTrailLength;
//
//	if (fRate >= 1.0f)
//	{
//		fRate = 1.0f;
//	}
//
//	if (fEndRate <= 0.0f)
//	{
//		fEndRate = 0.0f;
//	}
//
//	if (fEndRate >= 1.0f)
//	{
//		fEndRate = 1.0f;
//	}
//
//	WzVector vArrowPos;
//	WzVector vDest,vTemp;
//	float fArrowAngleZ,fArrowAngleX;
//
//	vDest =  m_Param.vTargetPos - m_Param.vStartPos;
//	vTemp = vDest;
//	vTemp.z = 0.0f;
//
//	float fLength;
//
//	if (vTemp.x == 0.0f && vTemp.y == 0.0f)
//	{
//		fLength = 0.0f;
//	}
//	else
//	{
//		fLength = VectorLength(&vTemp);
//	}
//
//	fArrowAngleX = -atan2(vDest.z,fLength);
//	fArrowAngleZ = Math_GetAngleFromVector(&vDest);
//	vArrowPos = m_Param.vStartPos * (1.0f - fRate) + m_Param.vTargetPos * (fRate);
//
//	WzVector vCurPos,vCurPos2,vDestPos,vDestPos2,vNewPos,vNewPos2,vNewEndPos,vNewEndPos2;
//	vCurPos = vCurPos2 = m_Param.vStartPos;
//	vDestPos = vDestPos2 = m_Param.vTargetPos;
//
//	VectorNormalize(&vDest,&vDest);
//	WzVector vDirection;
//
//	VectorCrossProduct(&vDirection,&vDest,&vDir);
//	VectorNormalize(&vDirection,&vDirection);
//
//	WzVector CamPos = pCameraInfo->m_wvFrom;
//	WzVector BeamSrc = m_Param.vStartPos;
//	WzVector BeamDst = m_Param.vTargetPos;
//
//	WzVector vBeamDir = BeamDst - BeamSrc;
//	VectorNormalize(&vBeamDir,&vBeamDir);
//	WzVector vBeamToCam = CamPos - BeamSrc;
//	VectorNormalize(&vBeamToCam,&vBeamToCam);		
//	WzVector vBeamUp;
//	vBeamUp.x = 0.0f;
//	vBeamUp.y = 0.0f;
//	vBeamUp.z = 1.0f;
//
//	WzVector vCurrDir = vBeamToCam - vBeamDir;
//
//	float fAngle = acos(VectorDotProduct(&vBeamUp,&vCurrDir));
//
//	if (vCurrDir.x < 0)
//		fAngle = fAngle;
//	else if (vCurrDir.x > 0)
//		fAngle = -fAngle;
//	else
//		fAngle = 0;
//
//	WzMatrix matrix;
//
//	VectorNormalize(&vCurrDir,&vCurrDir);
//	MatrixRotation(&matrix,&vCurrDir,fAngle);
//
//	vTemp.x = 0.0f;
//	vTemp.y = 0.0f;
//	vTemp.z = 1.0f;
//
//	float fHeight = m_Param.fHeight * fTrailRate;
//
//	vCurPos = vCurPos + vDirection * (fHeight / 2.0f);
//	vCurPos2 = vCurPos2 - vDirection * (fHeight / 2.0f);
//
//	vDestPos = vDestPos + vDirection * (fHeight / 2.0f);
//	vDestPos2 = vDestPos2 - vDirection * (fHeight / 2.0f);
//
//	vNewPos = vCurPos * (1.0f - fRate) + vDestPos * (fRate);
//	vNewPos2 = vCurPos2 * (1.0f - fRate) + vDestPos2 * (fRate);
//
//	vNewEndPos = vCurPos * (1.0f - fEndRate) + vDestPos * (fEndRate);
//	vNewEndPos2 = vCurPos2 * (1.0f - fEndRate) + vDestPos2 * (fEndRate);
//
//	if (m_pUnitDraw) 
//	{
//		m_pUnitDraw->SetPosition(vArrowPos,FALSE);
//		m_pUnitDraw->SetRotation(fArrowAngleX,0,fArrowAngleZ);
//	}
//
//	m_dwDelay += dwTick;
//
//	if (m_hEffect != INVALID_HANDLE_VALUE)
//	{
//		CEffectBody *pBody = g_pEffectManager->GetEffectPtrHandle(m_hEffect);
//		if(pBody)
//		{
//			WzVector vRot;
//			vRot.x = 0;
//			vRot.y = 0.0f;
//			vRot.z = 0;
//			WzVector vScale;
//			vScale.x = 1.0f;
//			vScale.y = 1.0f;
//			vScale.z = 1.0f;
//
//
//			if(m_Param.vScale != c_ZeroVector)
//			{
//				vScale=m_Param.vScale;
//			}
//
//			pBody->SetResultMatrix(vArrowPos,vRot,vScale);
//		}
//	}
//
//	m_bUpdated = TRUE;
//
//	// 라이프 타임이 있을 경우엔 대체 한다.
//	if(m_Param.dwLifeTime)
//	{
//		fRate = (float)m_dwDelay / (float) m_Param.dwLifeTime;		
//	}
//
//	if (fRate >= 1.0f)
//	{
//		// 데미지 처리를 한다.
//
//		return TRUE;
//	}
//	return FALSE;
//}
//
////------------------------------------------------------------------------------
///**
//Todo:정리 할것 
//*/
//BOOL Projectile::ProcessChaseTarget(DWORD dwTick)
//{
//	Character * pObj = (Character *)g_ObjectManager.GetObject(m_Param.CHASE_TARGET.dwTargetObjectKey);
//    //타겟이 없으면 종료
//	if(pObj==NULL)
//		return TRUE;
//
//	if(m_Param.vDir != c_ZeroVector)
//	{
//		m_Param.vDir.z=pObj->GetAngle(); 
//		m_Param.vDir.x=pObj->GetAngleX();
//	}
//
//	WzVector pos=pObj->GetVisiblePos();
//
//	if(m_Param.CHASE_TARGET.dwPosType)
//	{
//		pos=pObj->GetSkillEffectPos(m_Param.CHASE_TARGET.dwPosType, m_Param.CHASE_TARGET.dwBoneIndex );
//	}
//
//	pos+=m_Param.CHASE_TARGET.vPostion;//기준점으로 부터 얼마나 떨어 질것인가...
//
//
//	WzVector vDest,vTemp;
//	float fArrowAngleZ,fArrowAngleX;	
//	vDest =  m_Param.vTargetPos - m_Param.vStartPos;
//	vTemp = vDest;
//	vTemp.z = 0.0f;
//
//	float fLength;
//
//	if (vTemp.x == 0.0f && vTemp.y == 0.0f)
//	{
//		fLength = 0.0f;
//	}
//	else
//	{
//		fLength = VectorLength(&vTemp);
//	}
//
//	fArrowAngleX = -atan2(vDest.z,fLength);
//	fArrowAngleZ = Math_GetAngleFromVector(&vDest);
//
//
//
//	if (m_pUnitDraw) 
//	{
//		float	fLoopCheck;
//		fLoopCheck = 0.0f;
//		WzEvent Event;
//		ZeroMemory(&Event,sizeof(Event));
//
//		g_pSunRenderer->ProgressUnitFrame(m_pUnitDraw, ( float )dwTick, &Event, &fLoopCheck );
//
//		m_pUnitDraw->SetPosition(pos);
//
//
//		//유닛에 딸린 이펙트 처리 
//		if (Event.m_iNumEffect > 0)
//		{
//			for (int a = 0; a < Event.m_iNumEffect; a++)
//			{
//				int iIndex = Event.m_Effect[a].m_iEffectType;
//
//				assert(iIndex >= 0 && iIndex < MAX_OBJECT_EFFECT);
//
//				if (iIndex < 0)
//				{
//					iIndex = 0;
//				}
//				else if (iIndex >= MAX_OBJECT_EFFECT)
//				{
//					iIndex = MAX_OBJECT_EFFECT - 1;
//				}
//
//				if (Event.m_Effect[a].m_byEffectFlag & EFF_DESTROY)
//				{
//					DestroyObjectEffect(iIndex);
//				}
//				else
//				{
//					DestroyObjectEffect(iIndex);
//
//					int boneindex = Event.m_Effect[a].m_iEffectBoneIndex;
//					WzVector vPos;	
//					GameFunc::GetBonePos(m_pUnitDraw,boneindex,&vPos);
//
//
//
//					m_EffectParam[iIndex].hEffect =GlobalFunc::CreateEffectBeUpToOption(Event.m_Effect[a].m_wiEffectWzID,vPos,m_Param.dwOwner,m_Param.dwTargetID);//!loop 이펙트 쓰지 말것    
//
//					
//					
//					m_EffectParam[iIndex].iBoneIndex = boneindex;
//				}
//			}
//
//		}
//	}
//
//	if (m_hEffect != INVALID_HANDLE_VALUE)
//	{
//		CEffectBody *pBody = g_pEffectManager->GetEffectPtrHandle(m_hEffect);
//		if(pBody)
//		{
//			WzVector vRot;
//			vRot.x = fArrowAngleX;
//			vRot.y = 0.0f;
//			vRot.z = fArrowAngleZ;
//
//			WzVector vScale;
//			vScale.x = 1.0f;
//			vScale.y = 1.0f;
//			vScale.z = 1.0f;
//
//			if(m_Param.vScale != c_ZeroVector)
//			{
//				vScale=m_Param.vScale;
//			}
//
//			if(m_Param.vDir != c_ZeroVector)
//			{
//				vRot=m_Param.vDir;
//			}
//
//			if(m_Param.CHASE_TARGET.dwPosType==eSKILLEFFECT_POS_BONE_INDEX) //요렇게 하면 된다던데..
//			{
//				if(m_Param.CHASE_TARGET.bRot)
//				{
//					WzVector vPos;
//					vPos=pObj->GetBonePos(m_Param.CHASE_TARGET.dwBoneIndex);
//
//					WzMatrix wmPosMat, wmRotMat, wmScale, mat;
//					mat = pObj->GetBoneMatrix(m_Param.CHASE_TARGET.dwBoneIndex);
//					wmRotMat=pObj->GetBoneMatrix(m_Param.CHASE_TARGET.dwBoneIndex);
//					wmRotMat._41 = 0.0f;
//					wmRotMat._42 = 0.0f;
//					wmRotMat._43 = 0.0f;
//
//					MatrixIdentity( &wmPosMat );
//					MatrixTranslation( &wmPosMat, vPos.x, vPos.y, vPos.z );
//
//					MatrixIdentity( &wmPosMat );
//					MatrixScaling( &wmScale,1.0f,1.0f,1.0f);
//
//					pBody->SetResultMatrix( &wmPosMat, &wmRotMat, &wmScale );
//
//				}
//				else
//				{
//					WzVector vPos;
//					vPos=pObj->GetBonePos(m_Param.CHASE_TARGET.dwBoneIndex);
//					WzVector vDest,vTemp;
//					vDest=vPos+pObj->GetDirection()*10.0f;
//					vDest =  vDest - vPos;
//					vTemp = vDest;
//					vTemp.z = 0.0f;
//
//					float fLength;
//					if (vTemp.x == 0.0f && vTemp.y == 0.0f)
//					{
//						fLength = 0.0f;
//					}
//					else
//					{
//						fLength = VectorLength(&vTemp);
//					}
//
//					fArrowAngleX = -atan2(vDest.z,fLength);
//					fArrowAngleZ = Math_GetAngleFromVector(&vDest);
//
//					WzVector vRot;
//					vRot.x = 0.0f;
//					vRot.y = 0.0f;
//					vRot.z = 0.0f;
//
//					WzVector vScale;
//					vScale.x = 1.0f;
//					vScale.y = 1.0f;
//					vScale.z = 1.0f;
//
//					pBody->SetResultMatrix(vPos,vRot,vScale);
//				}				
//			}
//			else if(m_Param.CHASE_TARGET.dwPosType==eSKILLEFFECT_POS_RHAND||
//				    m_Param.CHASE_TARGET.dwPosType==eSKILLEFFECT_POS_LHAND||
//					m_Param.CHASE_TARGET.dwPosType==eSKILLEFFECT_POS_WEAPON)
//			{
//				WzVector vDest,vTemp;
//				vDest=pos+pObj->GetDirection()*10.0f;
//				vDest =  vDest - pos;
//				vTemp = vDest;
//				vTemp.z = 0.0f;
//
//				float fLength;
//				if (vTemp.x == 0.0f && vTemp.y == 0.0f)
//				{
//					fLength = 0.0f;
//				}
//				else
//				{
//					fLength = VectorLength(&vTemp);
//				}
//
//				fArrowAngleX = -atan2(vDest.z,fLength);
//				fArrowAngleZ = Math_GetAngleFromVector(&vDest);
//
//				WzVector vRot;
//				vRot.x = fArrowAngleX;
//				vRot.y = 0.0f;
//				vRot.z = fArrowAngleZ;
//
//				WzVector vScale;
//				vScale.x = 1.0f;
//				vScale.y = 1.0f;
//				vScale.z = 1.0f;
//
//				pBody->SetResultMatrix(pos,vRot,vScale);
//			}
//			else
//			{
//				pBody->SetResultMatrix(pos,vRot,vScale);
//			}
//		}
//	}
//
//	m_bUpdated = TRUE;
//	m_dwDelay += dwTick;
//
//	// 라이프 타임이 있을 경우엔 대체 한다.
//	if(m_Param.dwLifeTime)
//	{
//		fRate = (float)m_dwDelay / (float) m_Param.dwLifeTime;		
//	}
//
//	if (fRate >= 1.0f)
//	{
//		return TRUE;
//	}
//	return FALSE;
//}
////------------------------------------------------------------------------------
///**  타임라인-  퀘스트 포탈
//*/
//BOOL Projectile::ProcessQuestPortal(DWORD dwTick)
//{
//	//자신과 포탈과의 위치를 체크
//	if(!g_pHero) return TRUE;
//
//	//포탈과의 거리체크
//	WzVector vDistance =  g_pHero->GetVisiblePos()-m_Param.vStartPos;
//	float fDistance = VectorLength(&vDistance);
//	
//	//거리가 0.8미터 이내일경우
//	if (fDistance <= 0.8f)
//	{
//		//Client->Server: (퀘스트or미션)포탈에 진입하겠다는SYN
//		MSG_CG_ZONE_EX_QUEST_PORTAL_ENTER_SYN SendPacket;
//		//포탈종류
//		SendPacket.syncType	= MSG_CG_ZONE_EX_QUEST_PORTAL_ENTER_SYN::eSYNC_TEMP_PORTAL;			
//		//포탈키값		
//		SendPacket.portalKey = m_Param.QUEST_POTAL.portalKey;
//		//맵코드
//		SendPacket.mapEnterancePortalIndex	= m_Param.QUEST_POTAL.enterableMapCode;		
//		
//#ifdef _LTJ_HERO_INFO_BUGFIX
//		// no operation
//#else
//		//주인공정보저장
//		BattleScene::SaveHero(g_pHero);
//#endif
//		GlobalFunc::SendPacket( CI_GAMESERVERIDX, (BYTE *)&SendPacket, sizeof(SendPacket));
//		return TRUE;
//
//	}
//
//	if (m_hEffect != INVALID_HANDLE_VALUE)
//	{
//		CEffectBody *pBody = g_pEffectManager->GetEffectPtrHandle(m_hEffect);
//		if(pBody)
//		{
//			WzVector vScale;
//			SetVector(&vScale,1.0f,1.0f,1.0f);
//			pBody->SetResultMatrix(m_Param.vStartPos,m_Param.vDir,vScale);		
//		}
//	}
//	return FALSE;
//}
////------------------------------------------------------------------------------
///**
//*/
//BOOL Projectile::ProcessPortal(DWORD dwTick)
//{
//	Character * pObj=(Character *)g_ObjectManager.GetObject(m_Param.dwOwner);
//
//	//타겟이 없으면 종료
//	if(pObj==NULL)
//		return TRUE;
//
//
//	WzVector vSrc=pObj->GetVisiblePos();
//	vSrc.z+=1.0f;
//
//	WzVector vDistance =  vSrc-m_Param.vStartPos;
//	float fDistance = VectorLength(&vDistance);
//
//
//	if (fDistance <= 1.0f)
//	{
//		//패킷 날릴 것 
//		if (GENERALPARAM->IsNet() == FALSE)
//		{
//            xGetHeroData()->SetObjectKey( INVALID_OBJECT_KEY );
//			g_LoadScene.SetNextSceneAfterLoaded(SCENE_TYPE_SSQ, 12001, 61001);
//			GameFramework::ChangeScene(SCENE_TYPE_LOAD);
//			return TRUE;
//		}
//		else
//		{
//            //타락한 사원
//            if (pObj == g_pHero)
//            {
//                g_pHero->SendWingsControlByRider(false, false);
//                g_pHero->SendWingsControlByAccessory(false, false);
//            }
//
//			MSG_CG_ZONE_EX_SSQ_JOIN_SYN	SendPacket;
//			GlobalFunc::SendPacket(CI_GAMESERVERIDX, (BYTE*)&SendPacket, sizeof(SendPacket)); 
//			return TRUE;
//		}
//	}
//
//	if (m_hEffect != INVALID_HANDLE_VALUE)
//	{
//		CEffectBody* pBody = g_pEffectManager->GetEffectPtrHandle(m_hEffect);
//		if (pBody)
//		{
//			WzVector vScale;
//			vScale.x = 1.0f;
//			vScale.y = 1.0f;
//			vScale.z = 1.0f;
//			WzVector vPos=m_Param.vStartPos;
//
//			pBody->SetResultMatrix(vPos, m_Param.vDir, vScale);		
//		}
//	}
//
//	// 라이프 타임이 있을 경우엔 대체 한다.
//	if(m_Param.dwLifeTime)
//	{
//		fRate = (float)m_dwDelay / (float) m_Param.dwLifeTime;		
//	}
//
//	if (fRate >= 1.0f)
//	{
//		return TRUE;
//	}
//
//	return FALSE;
//}
//
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
//// 바이탈 셕션(시체들 몸에서 정기 구슬(?)이 빠져나와 Player한테 흡수되는 연출)
//
//BOOL Projectile::ProcessVitalSuction(DWORD dwTick)
//{
//	//////////////////////////////////////////////////////////////////////////
//	//////////////////////////////////////////////////////////////////////////
//	Character* pObj = (Character*)g_ObjectManager.GetObject(m_Param.dwTargetID);
//	if(!pObj)
//		return FALSE;
//
//	WzVector vLen = pObj->GetArrowTargetPos() - m_Param.VITAL_SUCTION.vPos;	// 목표와의 상대적 벡터
//	float fLen = VectorLength(&vLen);
//
//	m_dwDelay += dwTick;
//
//	WzVector wvTemp;
//	if(2000 > m_dwDelay)
//	{
//		WzVector wvUp;
//		SetVector(&wvUp, 0.f, 0.f, 1.f);
//		wvTemp = wvUp * (dwTick*m_Param.fSpeed);
//
//		m_Param.VITAL_SUCTION.vPos = m_Param.VITAL_SUCTION.vPos + wvTemp;
//	}
//	else
//	{
//		WzVector axis;	// 회전축
//		VectorCrossProduct( &axis, &m_Param.vDir, &vLen );
//
//		// 회전각 : 적당한 크기의 상수 각을 잡아 돌려준다.
//		float theta = 0.002f * (float)dwTick;
//
//		// 회전변환용 행렬 생성
//		WzMatrix matRotAxis;
//		MatrixRotation( &matRotAxis, &axis, theta );
//
//		if(10.f < fLen)
//		{
//			WzVector vTemp = m_Param.vDir;
//			VectorTransformCoord(&m_Param.vDir, &vTemp, &matRotAxis);
//		}
//		else 
//		{
//			VectorNormalize(&m_Param.vDir, &vLen);
//		}
//
//		m_Param.fSpeed += m_Param.fAccelate;
//		wvTemp = m_Param.vDir * (dwTick*m_Param.fSpeed);
//		WzVector wvLenCheck = m_Param.VITAL_SUCTION.vPos + wvTemp;
//		WzVector wvLen1, wvLen2;
//		wvLen1 = wvLenCheck - m_Param.VITAL_SUCTION.vPos;
//		wvLen2 = pObj->GetArrowTargetPos() - m_Param.VITAL_SUCTION.vPos;
//		float fLen1 = VectorLength(&wvLen1);
//		float fLen2 = VectorLength(&wvLen2);
//		if(fLen1 > fLen2)
//			m_Param.VITAL_SUCTION.vPos = pObj->GetArrowTargetPos();
//		else
//			m_Param.VITAL_SUCTION.vPos = m_Param.VITAL_SUCTION.vPos + wvTemp;	
//	}
//
//	if (m_hEffect != INVALID_HANDLE_VALUE)
//	{
//		CEffectBody *pBody = g_pEffectManager->GetEffectPtrHandle(m_hEffect);
//		if(pBody)
//		{
//			WzVector vRot;
//			vRot.x = 0.f;
//			vRot.y = 0.f;
//			vRot.z = 0.f;
//			WzVector vScale;
//			vScale.x = 1.0f;
//			vScale.y = 1.0f;
//			vScale.z = 1.0f;
//
//			pBody->SetResultMatrix(m_Param.VITAL_SUCTION.vPos, vRot, vScale);
//		}
//	}
//
//	//////////////////////////////////////////////////////////////////////////
//	//////////////////////////////////////////////////////////////////////////
//
//	m_bUpdated = TRUE;
//
//	// 발사체가 적에게 닿았으면
//	vLen = pObj->GetArrowTargetPos() - m_Param.VITAL_SUCTION.vPos;	// 목표와의 상대적 벡터
//	fLen = VectorLength(&vLen);
//	if((fLen <= 0.5f || m_dwDelay > m_Param.dwLifeTime) && (2000 < m_dwDelay))
//	{
//		if (m_Param.bCameraShake)
//		{
//			m_Param.bCameraShake=FALSE;
//			g_Camera.SetVibeType(Camera::VIBE_TYPE_VERTICAL);
//			g_Camera.StartVibration(50,0.07f);
//		}
//
//		if(m_Param.EndeffectID)
//		{
//
//			GlobalFunc::CreateEffectBeUpToOption(m_Param.EndeffectID,m_Param.VITAL_SUCTION.vPos,m_Param.dwOwner,m_Param.dwTargetID);//!loop 이펙트 쓰지 말것    
//
//			m_Param.EndeffectID=0;
//		}
//
//		ProcessDamageGroup();
//
//		return TRUE;
//	}
//
//	return FALSE;
//}
//
////////////////////////////////////////////////////////////////////////////
//// 
//
//BOOL Projectile::ProcessHPMPSuction(DWORD dwTick)
//{
//	m_dwDelay += dwTick;
//
//	//////////////////////////////////////////////////////////////////////////
//	//////////////////////////////////////////////////////////////////////////
//	Character* pPlayer = (Player*)g_ObjectManager.GetObject(m_Param.dwTargetID);
//	Character* pMonster = (Character*)g_ObjectManager.GetObject(m_Param.dwOwner);
//	if(NULL == pPlayer || NULL == pMonster)
//		return TRUE;
//
//	WzVector vLen = pPlayer->GetBonePos(14) - pMonster->GetArrowTargetPos();	// 목표와의 상대적 벡터
//	float fLen = VectorLength(&vLen);
//	WzVector vDir; VectorNormalize(&vDir, &vLen);
//
//	if (m_hEffect != INVALID_HANDLE_VALUE)
//	{
//		CEffectBody *pBody = g_pEffectManager->GetEffectPtrHandle(m_hEffect);
//		if(pBody)
//		{
//#ifdef _YJ_SHADOW_SKILL_S00013
//			pBody->SetAllElementTargetPos(pPlayer->GetBonePos(14));
//#endif //_YJ_SHADOW_SKILL_S00013
//
//			if(m_dwDelay > (m_Param.dwLifeTime - 1000))
//            {
//                pBody->FadeOut();
//            }
//
//			//////////////////////////////////////////////////////////////////////////
//
//			float fTemp = pPlayer->GetBonePos(14).z - pMonster->GetArrowTargetPos().z;
//			WzVector vRot;
//			vRot.x = atan2(fLen, fTemp);
//			vRot.z = Math_GetAngleFromVector(&vDir);
//			vRot.y = 0.f;//0.0f;
//
//			WzVector vScale;
//			vScale.x = 1.0f;
//			vScale.y = 1.0f;
//			vScale.z = 1.0f;
//
//			
//			pBody->SetResultMatrix(pMonster->GetArrowTargetPos(), vRot, vScale);
//			//////////////////////////////////////////////////////////////////////////
//		}
//	}
//
//	//////////////////////////////////////////////////////////////////////////
//	//////////////////////////////////////////////////////////////////////////
//
//	m_bUpdated = TRUE;
//	
//	if(30.f < fLen)
//		return TRUE;
//
//	STATE::TYPE st = pPlayer->GetCurState();
//	if(STATE::IDLE			== st ||
//	   STATE::KEYBOARDMOVE	== st ||
//	   STATE::JUMP			== st || 
//	   STATE::DEATH			== st ||
//	   STATE::DOWN			== st ||
//	   STATE::GETUP			== st ||
//	   STATE::SIT			== st ||
//	   STATE::EMOTICON		== st ||
//	   STATE::MOVE			== st ||
//	   STATE::KEYBOARDJUMP	== st)
//	{
//		return TRUE;
//	}
//
//	if( m_dwDelay > m_Param.dwLifeTime)
//	{
//		return TRUE;
//	}
//
//	return FALSE;
//}
//
////------------------------------------------------------------------------------
///**
//    Todo:정리 할것 
//*/
//BOOL Projectile::ProcessArrow(DWORD dwTick)
//{
//    St_Camera * pCameraInfo = g_pSunRenderer->GetCameraInfo();
//
//    WzVector vTo    = pCameraInfo->m_wvTo;
//    WzVector vRight = pCameraInfo->m_wvRight;
//    WzVector vDown  = -pCameraInfo->m_wvUp;
//    WzVector vDir   = pCameraInfo->m_wvDir;
//
//	assert(m_Param.dwMoveTime);
//	if(m_Param.dwTargetID)//타겟 쫒아가기
//	{
//		Character *pTarget =  (Character *)g_ObjectManager.GetObject(m_Param.dwTargetID);
//		if (pTarget)
//		{
//			m_Param.vTargetPos = pTarget->GetArrowTargetPos();
//		}
//	}
//
//    float fRate;
//	
//	if(m_Param.dwMoveTime)
//		fRate= (float)m_dwDelay / (float) m_Param.dwMoveTime;		
//	else
//		fRate=0.0f;
//
//	float fTrailRate = 1.0f - fRate; 
//    float fEndRate;
//
//	fRate *= m_Param.fSpeed;
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
//	WzVector vArrowPos;
//	WzVector vDest,vTemp;
//	float fArrowAngleZ,fArrowAngleX;
//
//	vDest =  m_Param.vTargetPos - m_Param.vStartPos;
//	vTemp = vDest;
//	vTemp.z = 0.0f;
//
//	float fLength;
//
//	if (vTemp.x == 0.0f && vTemp.y == 0.0f)
//	{
//		fLength = 0.0f;
//	}
//	else
//	{
//		fLength = VectorLength(&vTemp);
//	}
//
//	fArrowAngleX = -atan2(vDest.z,fLength);
//	fArrowAngleZ = Math_GetAngleFromVector(&vDest);
//	vArrowPos = m_Param.vStartPos * (1.0f - fRate) + m_Param.vTargetPos * (fRate);
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
//	MatrixRotation(&matrix,&vCurrDir,fAngle);
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
//	if(m_pMesh)
//	{
//		m_pMesh->m_pVertices[0] = vNewEndPos;
//		m_pMesh->m_pVertices[1] = vNewPos;
//		m_pMesh->m_pVertices[2] = vNewEndPos2;
//		m_pMesh->m_pVertices[3] = vNewPos;
//		m_pMesh->m_pVertices[4] = vNewPos2;
//		m_pMesh->m_pVertices[5] = vNewEndPos2;
//
//		WzVector vArrowDir = vDestPos - vCurPos;
//
//	    VectorNormalize(&vArrowDir,&vArrowDir);
//
//		WzVector vTrailOffset = vArrowDir * -0.3f;
//
//		for (int a = 0; a < 6; a++)
//		{
//			m_pMesh->m_pVertices[a] = m_pMesh->m_pVertices[a] + vTrailOffset;
//
//		}
//	}
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
//	if (m_hEffect != INVALID_HANDLE_VALUE)
//	{
//		CEffectBody *pBody = g_pEffectManager->GetEffectPtrHandle(m_hEffect);
//		if(pBody)
//		{
//			WzVector vRot;
//
//			vRot.x = fArrowAngleX;
//			vRot.y = 0.0f;
//			vRot.z = fArrowAngleZ;
//
//		
//			WzVector vScale;
//
//			vScale.x = 1.0f;
//			vScale.y = 1.0f;
//			vScale.z = 1.0f;
//			
//			
//			if(m_Param.vScale != c_ZeroVector)
//			{
//				vScale=m_Param.vScale;
//			}
//
//			pBody->SetResultMatrix(vArrowPos,vRot,vScale);
//			
//		}
//	}
//
//    m_bUpdated = TRUE;
//
//	// 라이프 타임이 있을 경우엔 대체 한다.
//	if(m_Param.dwLifeTime)
//	{
//		fRate = (float)m_dwDelay / (float) m_Param.dwLifeTime;		
//	}
//
//    if (fRate >= 1.0f)
//    {
//		if(m_Param.EndeffectID)
//		{
//			HANDLE hHandle;
//			
//
//			hHandle= GlobalFunc::CreateEffectBeUpToOption(m_Param.EndeffectID,vArrowPos,m_Param.dwOwner,m_Param.dwTargetID);//!loop 이펙트 쓰지 말것    
//
//			
//			CEffectBody *pBody = g_pEffectManager->GetEffectPtrHandle(hHandle);
//			if(pBody)
//			{
//				WzVector vRot;
//
//				vRot.x = fArrowAngleX;
//				vRot.y = 0.0f;
//				vRot.z = fArrowAngleZ;
//
//
//				WzVector vScale;
//
//				vScale.x = 1.0f;
//				vScale.y = 1.0f;
//				vScale.z = 1.0f;
//
//				pBody->SetResultMatrix(vArrowPos,vRot,vScale);
//
//			}
//		}
//
//        // 다중타겟일경우 다음타겟으로 넘어가고
//        if (m_bMultiTarget)
//        {
//			ProcessDamage(m_pdwMultiTargetID[m_iCurTargetIndex]);
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
//				if(pTarget)
//				{
//					m_Param.vTargetPos = pTarget->GetVisiblePos();
//					m_Param.vTargetPos.z +=(pTarget->GetArrowTargetHeight());
//				}
//            }
//        }
//        // 단일 타겟일경우 발사체를 삭제.
//        else
//        {
//			ProcessDamage();
//            return TRUE;
//        }
//    }
//    return FALSE;
//}
//
////------------------------------------------------------------------------------
///**
//Todo:정리 할것 
//*/
//#ifdef _KIKI_DELETE_PROJECTILEID_POWER_WAVE_
//    // empty
//#else // _KIKI_DELETE_PROJECTILEID_POWER_WAVE_
//BOOL Projectile::ProcessLighting(DWORD dwTick)
//{
//
//	assert(m_Param.dwMoveTime);
//
//	float fRate;
//
//	if(m_Param.dwMoveTime)
//		fRate= (float)m_dwDelay / (float) m_Param.dwMoveTime;		
//	else
//		fRate=0.0f;
//
//	float fTrailRate = 1.0f - fRate; 
//
//	fRate *= m_Param.fSpeed;
//
//	if (fRate >= 1.0f)
//	{
//		fRate = 1.0f;
//	}
//
//	WzVector vArrowPos;
//	WzVector vDest,vTemp;
//	float fArrowAngleZ,fArrowAngleX;
//
//	vDest =  m_Param.vTargetPos - m_Param.vStartPos;
//	vTemp = vDest;
//	vTemp.z = 0.0f;
//
//	float fLength;
//
//	if (vTemp.x == 0.0f && vTemp.y == 0.0f)
//	{
//		fLength = 0.0f;
//	}
//	else
//	{
//		fLength = VectorLength(&vTemp);
//	}
//
//	fArrowAngleX = -atan2(vDest.z,fLength);
//	fArrowAngleZ = Math_GetAngleFromVector(&vDest);
//
//
//	vArrowPos = m_Param.vStartPos * (1.0f - fRate) + m_Param.vTargetPos * (fRate);
//
//	m_dwDelay += dwTick;//경과 시간 
//
//	if (m_Param.POWER_WAVE.dwEffectDelay > 0)
//	{
//		if (m_dwDelay >= m_dwTrailEffectCount * m_Param.POWER_WAVE.dwEffectDelay)
//		{
//			WzVector vDirection;
//			float    fAngleZ;
//
//			vDirection = m_Param.vTargetPos - vArrowPos;
//			
//			vDirection.z = 0.0f;
//			if( vDirection != c_ZeroVector )
//			{
//				fAngleZ = 0.0f;				
//			}
//			fAngleZ = Math_GetAngleFromVector( &vDirection );
//
//			vDirection.x = sin(fAngleZ);
//			vDirection.y = -cos(fAngleZ);
//			vDirection.z = 0.0f;
//
//			CreateEffect(vArrowPos);
//			m_dwTrailEffectCount++;
//		}
//	}
//
//	m_bUpdated = TRUE;
//	if (m_hEffect != INVALID_HANDLE_VALUE)
//	{
//		CEffectBody *pBody = g_pEffectManager->GetEffectPtrHandle(m_hEffect);
//		if(pBody)
//		{
//			WzVector vRot;
//
//			vRot.x = fArrowAngleX;
//			vRot.y = 0.0f;
//			vRot.z = fArrowAngleZ;
//
//			WzVector vScale;
//
//			vScale.x = 1.0f;
//			vScale.y = 1.0f;
//			vScale.z = 1.0f;
//
//			if(m_Param.vScale != c_ZeroVector)
//			{
//				vScale=m_Param.vScale;
//			}
//
//			pBody->SetResultMatrix(vArrowPos,vRot,vScale);
//		}
//	}
//
//	if (fRate >= 1.0f)
//	{
//		// 데미지 처리를 한다.
//		ProcessDamageGroup();
//
//		return TRUE;
//	}
//
//	return FALSE;
//}
//
//#endif // _KIKI_DELETE_PROJECTILEID_POWER_WAVE_
//
////------------------------------------------------------------------------------
///**
//	Todo:인자를 늘/릴것인가..프로세스 루틴을 여러개 둘것인가...세부 구현 들어가면 정하자 일단은 하나로
//*/
//BOOL Projectile::Process(DWORD dwTick)
//{
//	m_dwCurrentTime+=dwTick;
//
//	if(!m_bActive&&m_dwCurrentTime>=m_Param.dwActiveDelayTime)
//	{
//		CreateEffect(&m_Param);
//		m_bActive=TRUE;
//	}
//
//	//비활성화 상태에선 
//	if(!m_bActive)  
//    {
//        return FALSE;
//    }
//		
//	if(m_pUnitDraw)
//	{
//		 if (m_bUseExtraMatrix)
//		 {
//			 g_pSunRenderer->AnimateUnit(m_pUnitDraw,0,&m_ExtraMatrix);
//		 }
//		 else
//		 {
//			 g_pSunRenderer->AnimateUnit(m_pUnitDraw,0,0);
//		 }		 
//	}
//
//	UpdateEffectPos();
//
//    BOOL rt=FALSE;
//
//    switch(m_Param.dwType)
//    {
//    case PROJECTILEID_ARROW:
//        {
//            rt=ProcessArrow(dwTick);
//        }
//        break;
//
//	case PROJECTILEID_COMMON:
//		{
//			rt=ProcessCommon(dwTick);
//		}
//		break;
//
//	case PROJECTILEID_AIRBLOW:
//		{
//			rt=ProcessAirBlow(dwTick);
//		}
//		break;
//
//   // case PROJECTILEID_SWORD_WAVE:
//   //     {
//			//rt=ProcessSwordWave(dwTick);
//   //     }
//   //     break;
//
//    case PROJECTILEID_POISON_RAIN:
//        {
//			rt=ProcessPoisonRain(dwTick);
//        }
//        break;
//
//#ifdef _KIKI_DELETE_PROJECTILEID_POWER_WAVE_
//        DoNothing();
//#else
//    case PROJECTILEID_POWER_WAVE:
//        {
//			rt=ProcessLighting(dwTick);
//        }
//        break;
//#endif
//
//	case PROJECTILEID_PORTAL:
//		{
//			rt=ProcessPortal(dwTick);
//		}
//		break;
//	
//	case PROJECTILEID_QUEST_POTAL:
//		{
//			rt=ProcessQuestPortal(dwTick);
//		}
//		break;
//
//	case PROJECTILEID_CHASE_TARGET:
//		{
//			rt=ProcessChaseTarget(dwTick);
//		}
//		break;
//
//	case PROJECTILEID_SONY_VINE:
//		{
//			rt=ProcessSonyVine(dwTick);
//		}
//		break;
//
//	case PROJECTILEID_POISON_THORN:
//		{
//			rt = ProcessPoisonThorn(dwTick);
//		}
//		break;
//
//	case PROJECTILEID_SWORD_TRAIL:
//		{
//			rt = ProcessSwordTrail(dwTick);
//		}
//		break;
//
//	case PROJECTILEID_BEZIER:
//		{
//			rt = ProcessBezier(dwTick);
//		}
//		break;
//
//	case PROJECTILEID_BEZIER_TORCH:
//		{
//			rt = ProcessTroch(dwTick);
//		}
//		break;
//
//
//	case PROJECTILEID_ATTACHMENT_WEAPON:
//		{
//			rt = ProcessAttachmentWeapon(dwTick);
//		}
//		break;
//
//	case PROJECTILEID_POISONDAGGER:
//		{
//			rt = ProcessPoisonDagger(dwTick);
//		}
//		break;
//
//	case PROJECTILEID_VITAL_SUCTION:
//		{
//			rt = ProcessVitalSuction(dwTick);
//		}
//		break;
//
//	case PROJECTILEID_HPMP_SUCTION:
//		{
//			rt = ProcessHPMPSuction(dwTick);
//		}
//		break;
//	}
//
//   return rt;
//}
//
////------------------------------------------------------------------------------
///**
//*/
//void Projectile::Create( const PROJECTILE_PARAM & param )
//{	
//	m_Param = param;
//	m_dwDelay = 0;
//	m_bUpdated = FALSE;
//    m_dwTrailEffectCount = 0;
//	m_dwCurrentTime=0;
//	m_wzColor = 0xffffffff;
//    fRate=0.0f;
//	m_fAngleZ=0.0f;
//
//	if(param.dwActiveDelayTime) //딜레이 타임이 있다면 
//    {
//        m_bActive = FALSE;        //비활성화 상태로 만든다.
//    }
//	else 
//	{
//		CreateEffect(&param);
//		m_bActive = TRUE;
//	}
//
//    if ((m_Param.bMakeTrail))
//        CreateTrail(param);
//
//	if(param.dwType==PROJECTILEID_SONY_VINE)
//	{
//		m_Param.SONYVINE.fDirZ = (float)(rand() % 628);
//		m_Param.SONYVINE.fDirZ /= 100.0f;
//
//		SetAni(m_Param.SONYVINE.aniID);
//		ClampAngle(&m_Param.SONYVINE.fDirZ);
//		m_fAngleZ=m_Param.SONYVINE.fDirZ;
//	}
//	else if(param.dwType==PROJECTILEID_POISON_THORN)
//	{
//		SetAni(m_Param.POISON_THORN.aniID);
//	}
//	else if(param.dwType==PROJECTILEID_POISON_RAIN)
//	{
//		SetAni(m_Param.POISON_RAIN.aniIDStart);
//		ClampAngle(&m_Param.POISON_RAIN.fDirZ);
//		m_fAngleZ=m_Param.POISON_RAIN.fDirZ;
//	}
//}
//
//
////------------------------------------------------------------------------------
///**
//*/
//void Projectile::CreateTrail(PROJECTILE_PARAM param)
//{
//    m_pMesh = New_SimpleMesh(2);
//
//    m_pMesh->m_pUVVerts[0].u = 0.0f;
//    m_pMesh->m_pUVVerts[0].v = 1.0f;
//
//    m_pMesh->m_pUVVerts[1].u = 0.0f;
//    m_pMesh->m_pUVVerts[1].v = 0.0f;
//
//    m_pMesh->m_pUVVerts[2].u = 1.0f;
//    m_pMesh->m_pUVVerts[2].v = 1.0f;
//
//    m_pMesh->m_pUVVerts[3].u = 0.0f;
//    m_pMesh->m_pUVVerts[3].v = 0.0f;
//
//    m_pMesh->m_pUVVerts[4].u = 1.0f;
//    m_pMesh->m_pUVVerts[4].v = 0.0f;
//
//    m_pMesh->m_pUVVerts[5].u = 1.0f;
//    m_pMesh->m_pUVVerts[5].v = 1.0f;
//}
//
////------------------------------------------------------------------------------
///**
//*/
//void Projectile::SetUnitDraw(CWzUnitDraw  *pDraw)
//{
//	m_pUnitDraw = pDraw;
//}
//
////------------------------------------------------------------------------------
///**
//*/
//void Projectile::SetTexture(HANDLE hTexture)
//{
//	if(m_pMesh)
//		m_pMesh->m_hTexDiffuse = hTexture;
//}
//
////------------------------------------------------------------------------------
///**
//*/
//BOOL Projectile::ProcessDamage()
//{	
//	if( m_bDamageProcessed )
//		return FALSE;
//
//    BOOL result = FALSE;
//
//	if (m_Param.dwOwner)
//	{
//		if( Character *pChr = (Character *)g_ObjectManager.GetObject(m_Param.dwOwner) )
//		{
//			result = pChr->ProcessAttackResult( m_Param.dwAttackSerial, m_Param.bAllDamage );
//		}		
//	}	
//
//    if( result == TRUE )
//    {
//        m_bDamageProcessed = TRUE;
//    }
//    return result;
//}
//
//BOOL Projectile::ProcessDamageGroup()
//{
//	if( m_bDamageProcessed )
//		return FALSE;
//	BOOL result = FALSE;
//	if (m_Param.dwOwner)
//	{
//		Character *pChr = (Character *)g_ObjectManager.GetObject(m_Param.dwOwner);
//		if (pChr)
//		{
//			result = pChr->ProcessAttackResult_Group(m_Param.dwAttackSerial);
//		}
//	}
//
//    if( result )
//    {
//        m_bDamageProcessed = TRUE;
//    }
//
//	return result;
//}
//
//BOOL Projectile::ProcessDamage(DWORD dwTarget)
//{
//    if( m_bDamageProcessed )
//        return FALSE;
//	
//    BOOL result = FALSE;
//	if (m_Param.dwOwner)
//	{
//		Character *pChr = (Character *)g_ObjectManager.GetObject(m_Param.dwOwner);
//		if (pChr)
//		{
//			result = pChr->ProcessAttackResult_Target(m_Param.dwAttackSerial, dwTarget );
//		}
//	}
//
//    if( result == TRUE )
//        m_bDamageProcessed = TRUE;
//    return result;
//}
//
////------------------------------------------------------------------------------
///**
//*/
//void Projectile::SetMultiTarget(int iTargetNum,DWORD *pdwTargetList)
//{
//	assert(iTargetNum > 0 && iTargetNum < MAX_MULTITARGET);
//
//	m_bMultiTarget = TRUE;
//	m_iTargetNum = iTargetNum;
//	m_iCurTargetIndex = 0;
//
//	memcpy(m_pdwMultiTargetID,pdwTargetList,sizeof(DWORD) * iTargetNum);
//}
//
////------------------------------------------------------------------------------
///**
//*/
//void Projectile::GenerateWave(WzVector vPos)
//{
//	WzVector vResult;
//    g_pMap->GetTerrainHeight(&vPos,&vResult);
//}
//
////------------------------------------------------------------------------------
///**
//*/
//void Projectile::CreateEffect( const PROJECTILE_PARAM * pParam )
//{
//	if (pParam->effectID)
//	{
//		 //.3 효과 핸들로 효과 바디의 포인터를 얻어온다. 포인터가 NULL이 오면 효과가 없거나 라이프가 다 해 죽은것임.
//
//		m_hEffect= GlobalFunc::CreateEffectBeUpToOption(pParam->effectID,pParam->vStartPos,pParam->dwOwner,pParam->dwTargetID);//!loop 이펙트 쓰지 말것    
//
//		if(m_hEffect!=INVALID_HANDLE_VALUE)
//		{
//			CEffectBody* pBody = g_pEffectManager->GetEffectPtrHandle(m_hEffect);
//			if( pBody)
//			{
//				WzVector vDest,vTemp;
//				float fArrowAngleZ,fArrowAngleX;
//
//				vDest =  m_Param.vTargetPos - m_Param.vStartPos;
//				vTemp = vDest;
//				vTemp.z = 0.0f;
//
//				float fLength;
//
//				if (vTemp.x == 0.0f && vTemp.y == 0.0f)
//				{
//					fLength = 0.0f;
//				}
//				else
//				{
//					fLength = VectorLength(&vTemp);
//				}
//
//				fArrowAngleX = -atan2(vDest.z,fLength);
//				fArrowAngleZ = Math_GetAngleFromVector(&vDest);
//
//				WzVector vRot;
//				vRot.x = fArrowAngleX;
//				vRot.y = 0.0f;
//				vRot.z = fArrowAngleZ;
//				WzVector vScale;
//				vScale.x = 1.0f;
//				vScale.y = 1.0f;
//				vScale.z = 1.0f;
//
//				if(m_Param.vScale != c_ZeroVector)
//				{
//					vScale=m_Param.vScale;
//				}
//
//				if(m_Param.vDir != c_ZeroVector)
//				{
//					vRot=m_Param.vDir;
//				}
//
//
//				if(pParam->dwType==PROJECTILEID_SWORD_TRAIL)
//				{
//					pBody->SetRenderType(AR_REFRACTION_GLASS);
//					pBody->SetRefractionRate(0.5f);		// 굴절률.(0에 가까워 질수록 투명)
//					pBody->SetRefractionAlpha(1.0f);	// 1.0 에 가까워질수록 본 이미지에 가까워짐
//
//					CEElementFlatChain* pFChain = pBody->GetFlatChainPtr();
//                    assert( pFChain );
//
//                    if( pFChain )
//                    {
//					    pFChain->SetLifeInfo(0, 3000);
//					    pFChain->SetTailNum(40);							
//					    pFChain->SetSplineLength(0.1f);
//
//					    pFChain->SetPosition(pParam->SWORD_TRAIL.vStartPos,pParam->SWORD_TRAIL.vEndPos);				
//                    }
//
//				}
//				else
//				{
//					pBody->SetResultMatrix(m_Param.vStartPos,vRot,vScale);
//				}
//			}
//		}
//	}
//}
//
//#ifdef _KIKI_DELETE_PROJECTILEID_POWER_WAVE_
//    // empty
//#else // _KIKI_DELETE_PROJECTILEID_POWER_WAVE_
////------------------------------------------------------------------------------
///**
//*/
//void Projectile::CreateEffect( const WzVector & vPos )
//{
//	if (m_Param.effectID)
//	{
//		//.3 효과 핸들로 효과 바디의 포인터를 얻어온다. 
//        // 포인터가 NULL이 오면 효과가 없거나 라이프가 다 해 죽은것임.
//
//		HANDLE hEffect;
//
//		hEffect= GlobalFunc::CreateEffectBeUpToOption(m_Param.effectID,vPos,m_Param.dwOwner,m_Param.dwTargetID);//!loop 이펙트 쓰지 말것    
//
//		m_hEffectArray.Append(hEffect);
//
//		WzVector rPos;
//		rPos.x = (float)(rand() % 2);
//		rPos.y = (float)(rand() % 2);
//		rPos.z = (float)(rand() % 2);
//
//		rPos.x /= 100.0f;
//		rPos.y /= 100.0f;
//		rPos.z /= 100.0f;
//	
//		int r = rand() % 4;
//
//		if(r==0)
//		{
//			rPos.x*=-1;
//		}
//		else if(r==1)
//		{
//			rPos.y*=-1;
//		}
//		else if(r==2)
//		{
//			rPos.z*=-1;
//		}
//
//		WzVector vTemp = vPos;
//		vTemp += rPos;
//	
//
//		hEffect = GlobalFunc::CreateEffectBeUpToOption( m_Param.effectID, vTemp, m_Param.dwOwner, m_Param.dwTargetID );//!loop 이펙트 쓰지 말것    
//		m_hEffectArray.Append(hEffect);
//	}
//}
//#endif //_KIKI_DELETE_PROJECTILEID_POWER_WAVE_
//
////유닛용 이펙트 부분
//void Projectile::InitializeEffect()
//{
//	for (int a = 0; a < MAX_OBJECT_EFFECT; a++)
//	{
//		m_EffectParam[a].hEffect = INVALID_HANDLE_VALUE;
//		m_EffectParam[a].iBoneIndex = 0;
//	}
//}
//
//void Projectile::DestroyAllEffect()
//{
//	for (int a = 0; a < MAX_OBJECT_EFFECT; a++)
//	{		
//		DestroyObjectEffect(a);
//	}
//}
//
//void Projectile::DestroyObjectEffect(int index)
//{
//	assert(index >= 0 && index < MAX_OBJECT_EFFECT);
//
//	if (m_EffectParam[index].hEffect != INVALID_HANDLE_VALUE)
//	{
//		g_pEffectManager->RemoveEffectPtr(m_EffectParam[index].hEffect);
//		m_EffectParam[index].hEffect = INVALID_HANDLE_VALUE;
//		m_EffectParam[index].iBoneIndex = 0;
//	}
//}
//
//void Projectile::UpdateEffectPos()
//{
//	if(!m_pUnitDraw)
//		return;
//
//	for (int a = 0; a < MAX_OBJECT_EFFECT; a++)
//	{
//		if (m_EffectParam[a].hEffect != INVALID_HANDLE_VALUE)
//		{
//			CEffectBody* pBody = g_pEffectManager->GetEffectPtrHandle(m_EffectParam[a].hEffect);
//
//			if (pBody) 
//            {
//				WzVector vRot;
//				vRot.x = 0.0f;
//				vRot.y = 0.0f;
//				vRot.z = 0.0f;
//
//				WzVector vScale;
//				vScale.x = 1.0f;
//				vScale.y = 1.0f;
//				vScale.z = 1.0f;
//
//				WzVector vPos;
//
//				if(m_Param.vScale != c_ZeroVector)
//				{
//					vScale=m_Param.vScale;
//				}
//
//				if(m_Param.vDir != c_ZeroVector)
//				{
//					vRot=m_Param.vDir;
//				}
//				
//				if( GameFunc::GetBonePos(m_pUnitDraw, m_EffectParam[a].iBoneIndex, &vPos) )
//                {
//					pBody->SetResultMatrix(vPos, vRot, vScale);
//                }
//			}
//		}
//	}
//}