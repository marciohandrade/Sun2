//------------------------------------------------------------------------------
//  SkillStateFrozenShield.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------

#include "SunClientPrecompiledHeader.h"
#include "statebase.h"
#include "SkillState.h"
#include "SkillStateFrozenShield.h"
#include "Character.h"
#include "ProjectileManagerEx.h"
#include "SkillEffectFunc.h"


BOOL SkillStateFrozenShield::OnProcessCustemEvent( int nEventIndex, Object* pOwner, DWORD CurTime,DWORD dwTick )
{
	if( nEventIndex == 1 )
	{
		ReleaseCastingEffect();


		Character* pCharacter = (Character*)pOwner;
		Character* pTarget = (Character *)pCharacter->GetTargetObject();


		WzVector pos = pCharacter->GetArrowBonePos(0);

		if(pCharacter->GetVisiblePos()==pos)//무기본이 없으면 
		{
			pos = pCharacter->GetWeaponBonePos(0);//손에서 발사			
		}

		SkillEffectFunc::CreateHandEffect( pCharacter, pCharacter->GetObjectKey(), StrToWzID("D000"), pos);

        cTargetPosition Position;
        Position.SetTargetVisiblePosition( pCharacter->GetObjectKey(), TRUE );
        Position.SetOffsetPosition( 0.0f, 0.0f, 0.95f );

        g_ProjectileManagerEx.CreateEffect( pCharacter->GetObjectKey(), StrToWzID("K390"), 5000, Position );
	}

	return TRUE;
}