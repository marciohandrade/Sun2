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
//    // ������ ó���� �����ʾҴٸ� ó���ϵ��� �Ѵ�
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
//	// ������ ó���� �Ѵ�.
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
//    // �ϵ� �ڵ� ��ġ��.
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
//	//���ϰ� �ٲ��� ����Ʈ �ı�
//	DestroyAllEffect();
//
//	if(m_pUnitDraw)
//	{
//		m_pUnitDraw->SetCurrentAnimation(aniId,bLoop);
//	}
//}
////------------------------------------------------------------------------------
///**
//Todo:���� �Ұ� 
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
//	// �߻�ü�� ������ �������	
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
//		//���ֿ� ���� ����Ʈ ó�� 
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
//					m_EffectParam[iIndex].hEffect= GlobalFunc::CreateEffectBeUpToOption(Event.m_Effect[a].m_wiEffectWzID,vPos,m_Param.dwOwner,m_Param.dwTargetID);//!loop ����Ʈ ���� ����    
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
//	// ������ Ÿ���� ���� ��쿣 ��ü �Ѵ�.
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
//		//���ֿ� ���� ����Ʈ ó�� 
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
//	// ������ Ÿ���� ���� ��쿣 ��ü �Ѵ�.
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
//		// ������ ó���� �Ѵ�.
//		ProcessDamageGroup();
//		return TRUE;
//	}
//	return FALSE;
//}
//
//
////------------------------------------------------------------------------------
///**
//Todo:���� �Ұ� 
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
//		//���ֿ� ���� ����Ʈ ó�� 
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
//	// ������ Ÿ���� ���� ��쿣 ��ü �Ѵ�.
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
//		// ������ ó���� �Ѵ�.
//		ProcessDamageGroup();
//		return TRUE;
//	}
//	return FALSE;
//}
//
//
////------------------------------------------------------------------------------
///**
//	�������� ��� �׸��� �߻�ü ���� 
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
//	if(m_Param.dwTargetID)//Ÿ�� �i�ư���
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
//	//������ ������
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
//	vDest = vNextArrowPos - vArrowPos;//���� ��ġ���� ���� ��ġ�� �������.
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
//	fArrowAngleX = -atan2(vDest.z,fLength);			//���� �������� �ƴ� ���� �������� ����
//	fArrowAngleZ = Math_GetAngleFromVector(&vDest); //���� �������� �ƴ� ���� �������� ����
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
//	// �߻�ü�� ������ �������	
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
//	// ������ Ÿ���� ���� ��쿣 ��ü �Ѵ�.
//	if(m_Param.dwLifeTime)
//	{
//		fRate = (float)m_dwDelay / (float) m_Param.dwLifeTime;		
//	}
//
//	//������ �ð��� �������� �ٷ� ������ ó�� 
//	if(m_Param.dwDamageTime)
//	{
//		if(m_dwDelay>m_Param.dwDamageTime)
//		{
//			//�������� ó�� �Ǿ�����
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
//		// ������ ó���� �Ѵ�.
//		ProcessDamageGroup();
//		if(m_Param.EndeffectID)
//		{
//			GlobalFunc::CreateEffectBeUpToOption(m_Param.EndeffectID,vArrowPos,m_Param.dwOwner,m_Param.dwTargetID);//!loop ����Ʈ ���� ����    
//			m_Param.EndeffectID=0;
//		}
//		return TRUE;
//	}
//	return FALSE;
//}
//
////------------------------------------------------------------------------------
///**
//	�������� �߻�ü �ǵ��ƿ���
//*/
//BOOL Projectile::ProcessPoisonDagger(DWORD dwTick)
//{
//	St_Camera * pCameraInfo = g_pSunRenderer->GetCameraInfo();
//
//	assert(m_Param.dwMoveTime);
//
//	//Ÿ�� �i�ư���
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
//	//������ 1���� ������ ������
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
//	vDest = vNextArrowPos - vArrowPos;//���� ��ġ���� ���� ��ġ�� �������.
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
//	fArrowAngleX = -atan2(vDest.z,fLength);			//���� �������� �ƴ� ���� �������� ����
//	fArrowAngleZ = Math_GetAngleFromVector(&vDest); //���� �������� �ƴ� ���� �������� ����
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
//	// �߻�ü�� ������ �������	
//	if(fRate >= 1.0f)
//	{
//		if(!m_Param.POISON_DAGGER.bReturn)
//		{	
//			//������ü�� ���ΰ��϶��� �����ش�.
//			if (m_Param.dwOwner == g_pHero->GetObjectKey())
//			{
//				m_Param.bCameraShake=FALSE;
//				g_Camera.SetVibeType(Camera::VIBE_TYPE_VERTICAL);
//				g_Camera.StartVibration(100,0.07f);
//			}
//			//!loop ����Ʈ ���� ����
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
//	// ������ Ÿ���� ���� ��쿣 ��ü �Ѵ�.
//	if(m_Param.dwLifeTime)
//	{
//		fRate = (float)m_dwDelay / (float) m_Param.dwLifeTime;		
//	}
//
//	//������ �ð��� �������� �ٷ� ������ ó�� 
//	if(m_Param.dwDamageTime)
//	{
//		if(m_dwDelay>m_Param.dwDamageTime && m_Param.POISON_DAGGER.bReturn)
//		{
//			//�������� ó�� �Ǿ�����
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
//		// ������ ó���� �Ѵ�.
//		ProcessDamageGroup();
//		if(m_Param.EndeffectID)
//		{
//			GlobalFunc::CreateEffectBeUpToOption(m_Param.EndeffectID,vArrowPos,m_Param.dwOwner,m_Param.dwTargetID);//!loop ����Ʈ ���� ����    
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
//	if(m_Param.dwTargetID)//Ÿ�� �i�ư���
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
//	//������ ������
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
//	vDest = vNextArrowPos - vArrowPos;//���� ��ġ���� ���� ��ġ�� �������.
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
//	fArrowAngleX = -atan2(vDest.z,fLength);			//���� �������� �ƴ� ���� �������� ����
//	fArrowAngleZ = Math_GetAngleFromVector(&vDest); //���� �������� �ƴ� ���� �������� ����
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
//	// �߻�ü�� ������ �������	
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
//	// ������ Ÿ���� ���� ��쿣 ��ü �Ѵ�.
//	if(m_Param.dwLifeTime)
//	{
//		fRate = (float)m_dwDelay / (float) m_Param.dwLifeTime;		
//	}
//
//	//������ �ð��� �������� �ٷ� ������ ó�� 
//	if(m_Param.dwDamageTime)
//	{
//		if(m_dwDelay>m_Param.dwDamageTime)
//		{
//			//�������� ó�� �Ǿ�����
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
//		// ������ ó���� �Ѵ�.
//		ProcessDamageGroup();
//		if(m_Param.EndeffectID)
//		{
//			GlobalFunc::CreateEffectBeUpToOption(m_Param.EndeffectID,vArrowPos,m_Param.dwOwner,m_Param.dwTargetID);//!loop ����Ʈ ���� ����    
//			m_Param.EndeffectID=0;
//		}
//
//		if( g_pHero->GetObjectKey() == m_Param.dwOwner )
//		{
//			GlobalFunc::NET_SEND_CG_ITEM_USE_SKILL_ACTION_SYN(eITEMTYPE_TORCH, vArrowPos);
//			m_Param.dwOwner =0;
//			
//			// ȶ�� ���� ����Ʈ ������ ���� 4�������� Ÿ�� ����Ʈ ����
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
//	if(m_Param.dwTargetID)//Ÿ�� �i�ư���
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
//		// ���� ������
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
//	// �߻�ü�� ������ �������	
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
//	// ������ Ÿ���� ���� ��쿣 ��ü �Ѵ�.
//	if(m_Param.dwLifeTime)
//	{
//		fRate = (float)m_dwDelay / (float) m_Param.dwLifeTime;		
//	}
//
//	//������ �ð��� �������� �ٷ� ������ ó�� 
//	if(m_Param.dwDamageTime)
//	{
//		if(m_dwDelay>m_Param.dwDamageTime)
//		{
//
//			//�������� ó�� �Ǿ�����
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
//		// ������ ó���� �Ѵ�.
//		ProcessDamageGroup();
//		if(m_Param.EndeffectID)
//		{
//			GlobalFunc::CreateEffectBeUpToOption(m_Param.EndeffectID,vArrowPos,m_Param.dwOwner,m_Param.dwTargetID);//!loop ����Ʈ ���� ����    
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
//	Todo:���� �Ұ� 
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
//	if(m_Param.dwTargetID)//Ÿ�� �i�ư���
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
//	// �߻�ü�� ������ �������	
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
//			GlobalFunc::CreateEffectBeUpToOption(m_Param.EndeffectID,vArrowPos,m_Param.dwOwner,m_Param.dwTargetID);//!loop ����Ʈ ���� ����    
//
//			m_Param.EndeffectID=0;
//		}
//
//		ProcessDamageGroup();
//	}
//
//	// ������ Ÿ���� ���� ��쿣 ��ü �Ѵ�.
//	if(m_Param.dwLifeTime)
//	{
//		fRate = (float)m_dwDelay / (float) m_Param.dwLifeTime;		
//	}
//
//	//������ �ð��� �������� �ٷ� ������ ó�� 
//	if(m_Param.dwDamageTime)
//	{
//		if(m_dwDelay>m_Param.dwDamageTime)
//		{
//			
//			//�������� ó�� �Ǿ�����
//			if(ProcessDamageGroup())
//			{
//				if(m_Param.EndeffectID)
//				{
//
//					GlobalFunc::CreateEffectBeUpToOption(m_Param.EndeffectID,vArrowPos,m_Param.dwOwner,m_Param.dwTargetID);//!loop ����Ʈ ���� ����    
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
//		// ������ ó���� �Ѵ�.
//		ProcessDamageGroup();
//		if(m_Param.EndeffectID)
//		{
//			GlobalFunc::CreateEffectBeUpToOption(m_Param.EndeffectID,vArrowPos,m_Param.dwOwner,m_Param.dwTargetID);//!loop ����Ʈ ���� ����    
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
//	if(pTarget)//��ġ ���� 
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
//	// �߻�ü�� ������ �������	
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
//	// ������ Ÿ���� ���� ��쿣 ��ü �Ѵ�.
//	if(m_Param.dwLifeTime)
//	{
//		fRate = (float)m_dwDelay / (float) m_Param.dwLifeTime;		
//	}
//
//	//������ �ð��� �������� �ٷ� ������ ó�� 
//	if(m_Param.dwDamageTime)
//	{
//		if(m_dwDelay>m_Param.dwDamageTime)
//		{
//			//�������� ó�� �Ǿ�����
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
//		// ������ ó���� �Ѵ�.
//		ProcessDamageGroup();
//		if(m_Param.EndeffectID)
//		{
//			GlobalFunc::CreateEffectBeUpToOption(m_Param.EndeffectID,vArrowPos,m_Param.dwOwner,m_Param.dwTargetID);//!loop ����Ʈ ���� ����    
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
//		//Todo:����Ʈ �Լ� ������ ������.
//		vector3 dir2; 
//		dir2.x=dir.x;
//		dir2.y=dir.z;
//		dir2.z=dir.y;
//		dir2.norm(); //z��
//
//		vector3 up;
//		up.set(0.0f,1.0f,0.0f);//y��
//
//		vector3 temp=up*dir2;//x��
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
//			GlobalFunc::CreateEffectBeUpToOption(StrToWzID("A030"),vResult,m_Param.dwOwner,m_Param.dwTargetID);//!loop ����Ʈ ���� ����    
//
//
//			m_dwTrailEffectCount++;
//		}
//	}
//
//	// �߻�ü�� ������ �������	
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
//	// ������ Ÿ���� ���� ��쿣 ��ü �Ѵ�.
//	if(m_Param.dwLifeTime)
//	{
//		fRate = (float)m_dwDelay / (float) m_Param.dwLifeTime;		
//	}
//
//	//������ �ð��� �������� �ٷ� ������ ó�� 
//	if(m_Param.dwDamageTime)
//	{
//		if(m_dwDelay>m_Param.dwDamageTime)
//		{
//
//			//�������� ó�� �Ǿ�����
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
//		// ������ ó���� �Ѵ�.
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
//	// ������ Ÿ���� ���� ��쿣 ��ü �Ѵ�.
//	if(m_Param.dwLifeTime)
//	{
//		fRate = (float)m_dwDelay / (float) m_Param.dwLifeTime;		
//	}
//
//	if (fRate >= 1.0f)
//	{
//		// ������ ó���� �Ѵ�.
//
//		return TRUE;
//	}
//	return FALSE;
//}
//
////------------------------------------------------------------------------------
///**
//Todo:���� �Ұ� 
//*/
//BOOL Projectile::ProcessChaseTarget(DWORD dwTick)
//{
//	Character * pObj = (Character *)g_ObjectManager.GetObject(m_Param.CHASE_TARGET.dwTargetObjectKey);
//    //Ÿ���� ������ ����
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
//	pos+=m_Param.CHASE_TARGET.vPostion;//���������� ���� �󸶳� ���� �����ΰ�...
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
//		//���ֿ� ���� ����Ʈ ó�� 
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
//					m_EffectParam[iIndex].hEffect =GlobalFunc::CreateEffectBeUpToOption(Event.m_Effect[a].m_wiEffectWzID,vPos,m_Param.dwOwner,m_Param.dwTargetID);//!loop ����Ʈ ���� ����    
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
//			if(m_Param.CHASE_TARGET.dwPosType==eSKILLEFFECT_POS_BONE_INDEX) //�䷸�� �ϸ� �ȴٴ���..
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
//	// ������ Ÿ���� ���� ��쿣 ��ü �Ѵ�.
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
///**  Ÿ�Ӷ���-  ����Ʈ ��Ż
//*/
//BOOL Projectile::ProcessQuestPortal(DWORD dwTick)
//{
//	//�ڽŰ� ��Ż���� ��ġ�� üũ
//	if(!g_pHero) return TRUE;
//
//	//��Ż���� �Ÿ�üũ
//	WzVector vDistance =  g_pHero->GetVisiblePos()-m_Param.vStartPos;
//	float fDistance = VectorLength(&vDistance);
//	
//	//�Ÿ��� 0.8���� �̳��ϰ��
//	if (fDistance <= 0.8f)
//	{
//		//Client->Server: (����Ʈor�̼�)��Ż�� �����ϰڴٴ�SYN
//		MSG_CG_ZONE_EX_QUEST_PORTAL_ENTER_SYN SendPacket;
//		//��Ż����
//		SendPacket.syncType	= MSG_CG_ZONE_EX_QUEST_PORTAL_ENTER_SYN::eSYNC_TEMP_PORTAL;			
//		//��ŻŰ��		
//		SendPacket.portalKey = m_Param.QUEST_POTAL.portalKey;
//		//���ڵ�
//		SendPacket.mapEnterancePortalIndex	= m_Param.QUEST_POTAL.enterableMapCode;		
//		
//#ifdef _LTJ_HERO_INFO_BUGFIX
//		// no operation
//#else
//		//���ΰ���������
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
//	//Ÿ���� ������ ����
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
//		//��Ŷ ���� �� 
//		if (GENERALPARAM->IsNet() == FALSE)
//		{
//            xGetHeroData()->SetObjectKey( INVALID_OBJECT_KEY );
//			g_LoadScene.SetNextSceneAfterLoaded(SCENE_TYPE_SSQ, 12001, 61001);
//			GameFramework::ChangeScene(SCENE_TYPE_LOAD);
//			return TRUE;
//		}
//		else
//		{
//            //Ÿ���� ���
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
//	// ������ Ÿ���� ���� ��쿣 ��ü �Ѵ�.
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
//// ����Ż �Ƽ�(��ü�� ������ ���� ����(?)�� �������� Player���� ����Ǵ� ����)
//
//BOOL Projectile::ProcessVitalSuction(DWORD dwTick)
//{
//	//////////////////////////////////////////////////////////////////////////
//	//////////////////////////////////////////////////////////////////////////
//	Character* pObj = (Character*)g_ObjectManager.GetObject(m_Param.dwTargetID);
//	if(!pObj)
//		return FALSE;
//
//	WzVector vLen = pObj->GetArrowTargetPos() - m_Param.VITAL_SUCTION.vPos;	// ��ǥ���� ����� ����
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
//		WzVector axis;	// ȸ����
//		VectorCrossProduct( &axis, &m_Param.vDir, &vLen );
//
//		// ȸ���� : ������ ũ���� ��� ���� ��� �����ش�.
//		float theta = 0.002f * (float)dwTick;
//
//		// ȸ����ȯ�� ��� ����
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
//	// �߻�ü�� ������ �������
//	vLen = pObj->GetArrowTargetPos() - m_Param.VITAL_SUCTION.vPos;	// ��ǥ���� ����� ����
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
//			GlobalFunc::CreateEffectBeUpToOption(m_Param.EndeffectID,m_Param.VITAL_SUCTION.vPos,m_Param.dwOwner,m_Param.dwTargetID);//!loop ����Ʈ ���� ����    
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
//	WzVector vLen = pPlayer->GetBonePos(14) - pMonster->GetArrowTargetPos();	// ��ǥ���� ����� ����
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
//    Todo:���� �Ұ� 
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
//	if(m_Param.dwTargetID)//Ÿ�� �i�ư���
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
//	// ������ Ÿ���� ���� ��쿣 ��ü �Ѵ�.
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
//			hHandle= GlobalFunc::CreateEffectBeUpToOption(m_Param.EndeffectID,vArrowPos,m_Param.dwOwner,m_Param.dwTargetID);//!loop ����Ʈ ���� ����    
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
//        // ����Ÿ���ϰ�� ����Ÿ������ �Ѿ��
//        if (m_bMultiTarget)
//        {
//			ProcessDamage(m_pdwMultiTargetID[m_iCurTargetIndex]);
//            m_iCurTargetIndex++;
//            // ��� Ÿ���� �� �����ƴ� ����
//            if (m_iCurTargetIndex >= m_iTargetNum)
//            {
//                return TRUE;
//            }
//            // ȭ������̸� 0���� �ʱ�ȭ���ش� �ٽ�
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
//        // ���� Ÿ���ϰ�� �߻�ü�� ����.
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
//Todo:���� �Ұ� 
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
//	m_dwDelay += dwTick;//��� �ð� 
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
//		// ������ ó���� �Ѵ�.
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
//	Todo:���ڸ� ��/�����ΰ�..���μ��� ��ƾ�� ������ �Ѱ��ΰ�...���� ���� ���� ������ �ϴ��� �ϳ���
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
//	//��Ȱ��ȭ ���¿��� 
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
//	if(param.dwActiveDelayTime) //������ Ÿ���� �ִٸ� 
//    {
//        m_bActive = FALSE;        //��Ȱ��ȭ ���·� �����.
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
//		 //.3 ȿ�� �ڵ�� ȿ�� �ٵ��� �����͸� ���´�. �����Ͱ� NULL�� ���� ȿ���� ���ų� �������� �� �� ��������.
//
//		m_hEffect= GlobalFunc::CreateEffectBeUpToOption(pParam->effectID,pParam->vStartPos,pParam->dwOwner,pParam->dwTargetID);//!loop ����Ʈ ���� ����    
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
//					pBody->SetRefractionRate(0.5f);		// ������.(0�� ����� ������ ����)
//					pBody->SetRefractionAlpha(1.0f);	// 1.0 �� ����������� �� �̹����� �������
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
//		//.3 ȿ�� �ڵ�� ȿ�� �ٵ��� �����͸� ���´�. 
//        // �����Ͱ� NULL�� ���� ȿ���� ���ų� �������� �� �� ��������.
//
//		HANDLE hEffect;
//
//		hEffect= GlobalFunc::CreateEffectBeUpToOption(m_Param.effectID,vPos,m_Param.dwOwner,m_Param.dwTargetID);//!loop ����Ʈ ���� ����    
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
//		hEffect = GlobalFunc::CreateEffectBeUpToOption( m_Param.effectID, vTemp, m_Param.dwOwner, m_Param.dwTargetID );//!loop ����Ʈ ���� ����    
//		m_hEffectArray.Append(hEffect);
//	}
//}
//#endif //_KIKI_DELETE_PROJECTILEID_POWER_WAVE_
//
////���ֿ� ����Ʈ �κ�
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