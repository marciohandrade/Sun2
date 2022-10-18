//------------------------------------------------------------------------------
//  SkillStateTeleport.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------

#include "SunClientPrecompiledHeader.h"
#include "statebase.h"
#include "SkillState.h"
#include "SkillStateTeleport.h"
#include "Character.h"
#include "Hero.h"
#include "HeroActionInput.h"
#include "Camera.h"


STATECHANGE::TYPE SkillStateTeleport::OnStart(Object* pOwner, DWORD CurTime)
{
	Character* pCharacter = (Character*)pOwner;
	

	STATECHANGE::TYPE rt=StateSkill::OnStart(pOwner,CurTime);

    if (pCharacter == (Character *)g_pHero)
    {			
		if(g_HeroInput.IsSpecialMove())
		{
			g_HeroInput.ResetSpecialMove();
		}
		else
		{
			WzVector vTarget=pCharacter->GetCurrentAction()->SKILL.vTargePos;
			pCharacter->SetDirection(vTarget);
		}
    }
	else
	{
		WzVector vTarget=pCharacter->GetCurrentAction()->SKILL.vTargePos;
		pCharacter->SetDirection(vTarget); //���� ĳ���ʹ� ������ ������ ���� 
	}
	

	//Todo:Ưȭ�� ó���� �Ѵ�.
	if( GENERALPARAM->IsNet()==FALSE)// �̱� ��� �ϰ�� 
	{
		pCharacter->SetPosition(g_HeroInput.GetPickingPosition());
		g_Camera.InitSpringCamera();
	}

	return rt;
}
