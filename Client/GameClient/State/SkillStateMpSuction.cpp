#include "SunClientPrecompiledHeader.h"
//
//#include "statebase.h"
//#include "SkillState.h"
//#include "SkillStateFactory.h"
//#include "SkillInfoParser.h"
//#include "Hero.h"
//#include "Character.h"
//#include "Clone.h"
//#include "CloneManager.h"
//#include "GlobalFunc.h"
//#include "ProjectileManagerEx.h"
//#include "SkillStateMpSuction.h"
//
//SkillStateMpSuction::SkillStateMpSuction()
//{
//}
//
//SkillStateMpSuction::~SkillStateMpSuction(void)
//{
//}
//
//STATECHANGE::TYPE SkillStateMpSuction::OnStart(Object* pOwner, DWORD CurTime)
//{
//	Character*	pChr = (Character*)pOwner;
//
//	m_btAniState = 0;
//	m_dwTime = 0;
//
//	PROJECTILE_PARAM param;
//	ZeroMemory(&param,sizeof(param));
//
//	param.fHeight = 1.f;
//
//	Character* pTarget   = (Character *)pChr->GetTargetObject();
//	if(NULL == pChr || NULL == pTarget)
//	{
//		assert(!"AddSkillResult::SkillStateMpSuction");
//	}
//
//	param.dwOwner  = pTarget->GetObjectKey();
//	param.vStartPos = pTarget->GetArrowTargetPos();
//
//	param.vTargetPos = pChr->GetWeaponBonePos(0);
//	param.vTargetPos.z += (pChr->GetArrowTargetHeight());
//	param.dwTargetID = pChr->GetObjectKey();
//
//	param.dwType = PROJECTILE::PROJECTILEID_HPMP_SUCTION;
//	param.dwAttackSerial = pChr->GetCurrentAttackSerial();	
//
//	SetVector(&param.vDir, 0.f, 0.f, 1.f);
//
//	param.dwImageCode = 0;
//	param.effectID	  = StrToWzID("K805");
//
//	param.dwLifeTime  = 4000;
//	SkillScriptInfo* pInfo = GetSkillInfo();
//	if(pInfo)
//	{
//		BASE_ABILITYINFO * pAbilityInfo = pInfo->GetAbilityInfoByIndex(0);
//		if(pAbilityInfo)
//			param.dwLifeTime  = pAbilityInfo->m_iParam[2];
//	}
//
//	param.bAddNextSerial=TRUE;
//	g_ProjectileManager.CreateProjectile(param);
//
//	return StateSkill::OnStart(pOwner,CurTime);	
//}
//
//STATEPROCESS::TYPE SkillStateMpSuction::OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick)
//{
//	Character*	pChr = (Character*)pOwner;
//
//	EnterProcess(pOwner,CurTime,dwTick);
//
//	if( 90.f < GetAnimationRate() && 0 == m_btAniState)
//	{
//		pChr->SetAnimation(StrToWzID("S419"), TRUE);
//		SetAnimationRate(0.f);
//		m_btAniState = 1;
//	}
//
//	SkillScriptInfo *pInfo = GetSkillInfo();
//	if(pInfo)
//	{
//		BASE_ABILITYINFO * pAbilityInfo = 
//			pInfo->GetAbilityInfoByIndex(0);
//
//		if(pAbilityInfo)
//		{
//			if(m_dwTime >(DWORD)pAbilityInfo->m_iParam[2] && 1 == m_btAniState)
//			{
//				pChr->SetAnimation(StrToWzID("S420"));
//				m_btAniState = 2;
//			}
//		}
//	}
//
//	m_dwTime += dwTick;
//
//
//	// 이벤트체크
//	CheckEvent();
//
//	// 친다
//	ProcessHit(pChr,1);
//
//	if(g_pHero->GetObjectKey() == pChr->GetObjectKey())
//	{
//		Character* pTarget = (Character *)pChr->GetTargetObject();
//
//		if(NULL == pTarget)
//		{
//			g_pHero->SendStopMovePacket();
//			return STATEPROCESS::END;
//		}
//
//		if(pTarget->IsDead())
//		{
//			g_pHero->SendStopMovePacket();
//			return STATEPROCESS::END;
//		}
//
//		if(TRUE == g_pHero->CannotAct())
//		{
//			g_pHero->SendStopMovePacket();
//			return STATEPROCESS::END;
//		}
//
//		WzVector vLen = pChr->GetBonePos(14) - pTarget->GetArrowTargetPos(); //14 == Hand Bone Idx
//		float fLen = VectorLength(&vLen);
//		if(30.f < fLen)
//		{
//			g_pHero->SendStopMovePacket();
//			return STATEPROCESS::END;
//		}
//	}
//
//	return LeaveProcess(pOwner,CurTime,dwTick);
//}
