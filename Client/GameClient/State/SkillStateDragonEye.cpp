//------------------------------------------------------------------------------
//  SkillStateBloodRain.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "statebase.h"
#include "SkillState.h"
#include "SkillStateDragonEye.h"
#include "Character.h"
#include "CloneManager.h"
#include "Clone.h"

//------------------------------------------------------------------------------
/**
*/
STATECHANGE::TYPE SkillStateDragonEye::OnStart(Object* pOwner, DWORD CurTime)
{
	Character*	pCharacter = (Character*)pOwner;
	
	
	STATECHANGE::TYPE rt=StateSkill::OnStart(pOwner,CurTime);

	DWORD dwCloneKey = g_CloneManager.CreateClone(pCharacter);
	if ( Clone* pClone = g_CloneManager.GetClone(dwCloneKey) )
	{
		pClone->SetColor(WzColor_RGBA(255,255,255,255));
		pClone->SetLifeTime(1000);
		pClone->SetDisappear( 900 );
		WzVector vDir;
		vDir.x = 1.0f;
		vDir.y = 0.0f;
		vDir.z = 0.0f;
		pClone->Vibrate(vDir,0.05f);
		pClone->SetMoveSpeed(16.0f * SPEED_MULTIPLIER);
		pClone->SetColor(WzColor_RGBA(40,40,255,100));
		
	}

    
	return rt;
}

//------------------------------------------------------------------------------
/**
*/
void SkillStateDragonEye::PlayNonstopEffect(Object* pOwner, Object *pTarget)
{
	Character*	pCharacter = (Character*)pOwner;	

	DWORD dwCloneKey = g_CloneManager.CreateClone(pCharacter);

	if ( Clone* pClone = g_CloneManager.GetClone(dwCloneKey) )
	{
		pClone->SetColor(WzColor_RGBA(255,255,255,255));
		pClone->SetLifeTime(1000);

		WzVector vChrDir = pOwner->GetDirection();
		WzVector vDir;
		
		vDir.x = vChrDir.y;
		vDir.y = -vChrDir.x;
		vDir.z = 0.0f;

		pClone->SetPlayAni(TRUE);
		pClone->SetColor(WzColor_RGBA(40,40,255,160));
		pClone->SetAlphaBlend(AB_LIGHTING);
		pClone->AttachToObject(pOwner->GetObjectKey());
		pClone->Vibrate(vDir,1.0f);
		pClone->SetMoveSpeed(16.0f * SPEED_MULTIPLIER);
		
	}

	dwCloneKey = g_CloneManager.CreateClone(pCharacter);
	if ( Clone* pClone = g_CloneManager.GetClone(dwCloneKey) )
	{
		pClone->SetColor(WzColor_RGBA(255,255,255,255));
		pClone->SetLifeTime(1000);

		WzVector vChrDir = pOwner->GetDirection();
		WzVector vDir;
		
		vDir.x = -vChrDir.y;
		vDir.y = vChrDir.x;
		vDir.z = 0.0f;

		

		pClone->SetColor(WzColor_RGBA(40,40,255,160));
		pClone->SetAlphaBlend(AB_LIGHTING);
		pClone->AttachToObject(pOwner->GetObjectKey());
		pClone->Vibrate(vDir,0.6f);
		pClone->SetMoveSpeed(16.0f * SPEED_MULTIPLIER);
		
	}


}
