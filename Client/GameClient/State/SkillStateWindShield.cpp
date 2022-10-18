//------------------------------------------------------------------------------
//  SkillStateWindShield.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------

#include "SunClientPrecompiledHeader.h"
#include "statebase.h"
#include "SkillState.h"
#include "SkillStateFactory.h"
#include "SkillStateWindShield.h"
#include "Character.h"
#include "ProjectileManagerEx.h"
#include "SkillInfoParser.h"



BOOL SkillStateWindShield::OnProcessCustemEvent( int nEventIndex, Object* pOwner, DWORD CurTime,DWORD dwTick )
{
	if( nEventIndex == 1 )
	{

		Character* pCharacter = (Character*)pOwner;

		WzVector pos = pCharacter->GetArrowBonePos(0);

		if(pCharacter->GetVisiblePos()==pos)//무기본이 없으면 
		{
			pos = pCharacter->GetWeaponBonePos(0);//손에서 발사			
		}

		WzVector dest=pCharacter->GetVisiblePos()+pCharacter->GetDirection()*2.0f;
		dest.z=pos.z;

        cTargetPosition Position;
        Position.SetTargetVisiblePosition( pCharacter->GetObjectKey(), TRUE );

        g_ProjectileManagerEx.CreateEffect( pCharacter->GetObjectKey(), StrToWzID("K370"), 5000, Position );
	}

	return TRUE;
}
