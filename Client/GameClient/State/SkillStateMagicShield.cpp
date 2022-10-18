//------------------------------------------------------------------------------
//  SkillStateMagicShield.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------

#include "SunClientPrecompiledHeader.h"
#include "statebase.h"
#include "SkillState.h"
#include "SkillStateMagicShield.h"
#include "Character.h"
#include "ProjectileManagerEx.h"
#include "SkillInfoParser.h"
#include "SkillEffectFunc.h"


BOOL SkillStateMagicShield::OnProcessCustemEvent( int nEventIndex, Object* pOwner, DWORD CurTime,DWORD dwTick )
{
	if( nEventIndex == 1 )
	{
		Character* pCharacter = (Character*)pOwner;

		WzVector pos = pCharacter->GetArrowBonePos(0);

		if(pCharacter->GetVisiblePos()==pos)//무기본이 없으면 
		{
			pos = pCharacter->GetWeaponBonePos(0);//손에서 발사			
		}
		
		SkillEffectFunc::CreateHandEffect( pCharacter, pCharacter->GetObjectKey(), StrToWzID("D004"), pos);


        cTargetPosition Position;
        Position.SetTargetVisiblePosition( pCharacter->GetObjectKey(), TRUE );
        Position.SetOffsetPosition( 0.0f, 0.0f, 0.95f );

        g_ProjectileManagerEx.CreateEffect( pCharacter->GetObjectKey(), StrToWzID("K360"), 5000, Position );

	}

	return TRUE;
}