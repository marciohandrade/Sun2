//------------------------------------------------------------------------------
//  SkillStateBeastAreaAttack.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------

#include "SunClientPrecompiledHeader.h"
#include "statebase.h"
#include "SkillState.h"
#include ".\SkillStateBeastAreaAttack.h"
#include "../Character.h"
#include "..\soundeffect.h"
#include "..\Projectile.h"
#include "..\ProjectileManager.h"
#include "SkillInfoParser.h"
using namespace PROJECTILE;
#include "nmath.h"
#include "vector.h"	
//------------------------------------------------------------------------------
/**
*/
SkillStateBeastAreaAttack::SkillStateBeastAreaAttack(int iLevel)
{
	m_iLevel = iLevel;
}

//------------------------------------------------------------------------------
/**
*/
SkillStateBeastAreaAttack::~SkillStateBeastAreaAttack()
{
}

//------------------------------------------------------------------------------
/**
*/
BOOL SkillStateBeastAreaAttack::CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState)
{
	return TRUE;
}

//------------------------------------------------------------------------------
/**
*/
STATECHANGE::TYPE SkillStateBeastAreaAttack::OnStart(Object* pOwner, DWORD CurTime)
{
	Character*	pChr = (Character*)pOwner;

	m_iHealCount = 0;

	Character *pTarget = (Character *)pChr->GetTargetObject();

	if (pTarget)
	{
		pChr->SetDirection(&pTarget->GetPosition());
	}


	//WzVector vTarget=pChr->GetCurrentAction()->SKILL.vTargePos;

	//pChr->SetDirection(&vTarget);


	assert(pChr->IsKindOfObject(MONSTER_OBJECT));

	g_pEffectManager->CreateEffect(StrToWzID("L001"),pChr->GetVisiblePos());	

	return StateSkill::OnStart(pOwner,CurTime);
}

//------------------------------------------------------------------------------
/**
*/
STATEPROCESS::TYPE SkillStateBeastAreaAttack::OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick)
{
	EnterProcess(pOwner,CurTime,dwTick);

	Character*	pChr = (Character*)pOwner;

	// 타격점 체크
	CheckEvent();

	static DWORD time=0;
	time+=dwTick;

	if (CheckNotProcessedHitEvent(1))  
	{
		m_iHitProcessed++;

		

		//Character *pTarget = (Character *)pChr->GetTargetObject();
		//if (pTarget)//타겟으로 발사체 쏘는 것들 죽을경우 생각해볼것 
		{
			//무기본이 없다 대충 밀자 
			WzVector pos=pChr->GetVisiblePos();
			pos+=pChr->GetDirection();
			pos.z+=4.0f;

			PROJECTILE_PARAM param;

			ZeroMemory(&param,sizeof(param));
			param.fAccelate=0.0f;

			param.fHeight = 1.0f;

			param.vStartPos = pos;		
			param.vTargetPos = pChr->GetPosition()+pChr->GetDirection()*30;
			
			param.vTargetPos.z+=1.0f;
			param.dwDemageTime=100;
			param.dwLifeTime = 0;
			param.dwMoveTime = 2000;							
			param.fSpeed = 3.0f;
			param.dwType=PROJECTILEID_COMMON;
			param.dwAttackSerial = pChr->GetCurrentAttackSerial();
			param.dwOwner = pChr->GetObjectKey();
			param.dwImageCode = 0;
			param.effectID = StrToWzID("L002");
			param.bAddNextSerial=FALSE;
			param.bAllDemage=TRUE;

			g_ProjectileManager.CreateProjectile(param);

			WzVector vDirection;
			float len;

			//
			param.vStartPos = pos;		
			param.vTargetPos = pChr->GetPosition()+pChr->GetDirection()*30;

			param.vTargetPos.z+=1.0f;

			vDirection = param.vTargetPos - param.vStartPos ;

			vDirection.z=0.0f;
			param.vTargetPos=param.vStartPos;

			len=VectorLength( &vDirection );

			vDirection.x = sin(WZ_PI/4.5+pChr->GetAngle());
			vDirection.y = -cos(WZ_PI/4.5+pChr->GetAngle());
			vDirection.z=0.0f;

			param.vTargetPos+= vDirection*len;
		
			g_ProjectileManager.CreateProjectile(param);
			//
			param.vStartPos = pos;		
			param.vTargetPos = pChr->GetPosition()+pChr->GetDirection()*30;
			param.vTargetPos.z+=1.0f;

			vDirection = param.vTargetPos - param.vStartPos ;

			vDirection.z=0.0f;
			param.vTargetPos=param.vStartPos;

			len=VectorLength( &vDirection );

			vDirection.x = sin(WZ_PI/2.25+pChr->GetAngle());
			vDirection.y = -cos(WZ_PI/2.25+pChr->GetAngle());
			vDirection.z=0.0f;

			param.vTargetPos+= vDirection*len;
			
			param.vTargetPos.z=param.vStartPos.z;

			g_ProjectileManager.CreateProjectile(param);

			//////////////////////////////////

			//
			param.vStartPos = pos;		
			param.vTargetPos = pChr->GetPosition()+pChr->GetDirection()*30;

			param.vTargetPos.z+=1.0f;

			vDirection = param.vTargetPos - param.vStartPos ;

			vDirection.z=0.0f;
			param.vTargetPos=param.vStartPos;

			len=VectorLength( &vDirection );

			vDirection.x = sin(WZ_PI*2-WZ_PI/4.5+pChr->GetAngle());
			vDirection.y = -cos(WZ_PI*2-WZ_PI/4.5+pChr->GetAngle());
			vDirection.z=0.0f;

			param.vTargetPos+= vDirection*len;
			
			param.vTargetPos.z=param.vStartPos.z;

			g_ProjectileManager.CreateProjectile(param);
			//
			param.vStartPos = pos;		
			param.vTargetPos = pChr->GetPosition()+pChr->GetDirection()*30;
			param.vTargetPos.z+=1.0f;

			vDirection = param.vTargetPos - param.vStartPos ;

			vDirection.z=0.0f;
			param.vTargetPos=param.vStartPos;

			len=VectorLength( &vDirection );

			vDirection.x = sin(WZ_PI*2-WZ_PI/2.25+pChr->GetAngle());
			vDirection.y = -cos(WZ_PI*2-WZ_PI/2.25+pChr->GetAngle());
			vDirection.z=0.0f;

			param.vTargetPos+= vDirection*len;
			
			param.vTargetPos.z=param.vStartPos.z;
			param.bAddNextSerial=TRUE;

			g_ProjectileManager.CreateProjectile(param);


			g_SoundEffect.Play(4,&pChr->GetPosition());
		}
	
	}

	return LeaveProcess(pOwner,CurTime,dwTick);
}

//------------------------------------------------------------------------------
/**
*/
STATECHANGE::TYPE SkillStateBeastAreaAttack::OnEnd(Object* pOwner, DWORD CurTime)
{
	Character*	pChr = (Character*)pOwner;

	return STATECHANGE::SUCCESS;//발사체에서 attackSerial 증가 시킬꺼다
}