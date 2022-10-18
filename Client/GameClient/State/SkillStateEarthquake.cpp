//------------------------------------------------------------------------------
//  SkillStateEarthQuake.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "statebase.h"
#include "SkillState.h"
#include "SkillStateEarthQuake.h"
#include "Character.h"
#include "Hero.h"
#include "Camera.h"
#include "GlobalFunc.h"


BOOL SkillStateEarthQuake::OnProcessCustemEvent( int nEventIndex, Object* pOwner, DWORD CurTime,DWORD dwTick )
{
	if( nEventIndex == 1 )
	{

		//// Release CastEffect
		//if(m_hCastHandle[1]!=INVALID_HANDLE_VALUE)
		//{
		//	g_pEffectManager->RemoveEffectPtr(m_hCastHandle[1]);
		//	m_hCastHandle[1]=INVALID_HANDLE_VALUE;
		//}


		Character* pCharacter = (Character*)pOwner;

		if (pCharacter == (Character *)g_pHero)
		{	
			g_Camera.SetVibeType(Camera::VIBE_TYPE_ALL);
			g_Camera.StartVibration(500);
		}


		GlobalFunc::CreateEffectBeUpToOption(StrToWzID("K350"), pCharacter->GetCurrentAction()->SKILL.vTargePos, pCharacter->GetObjectKey());
	}

	return TRUE;
}
