//------------------------------------------------------------------------------
//  SkillStateRevival.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------

#include "SunClientPrecompiledHeader.h"
#include "statebase.h"
#include "SkillState.h"
#include "SkillStateRevival.h"
#include "Character.h"
#include "SkillEffectFunc.h"


BOOL SkillStateRevival::OnProcessCustemEvent( int nEventIndex, Object* pOwner, DWORD CurTime,DWORD dwTick )
{
	if( nEventIndex == 1 )
	{
		Character* pCharacter = (Character*)pOwner;

		WzVector pos = pCharacter->GetArrowBonePos(0);

		if(pCharacter->GetVisiblePos()==pos)//���⺻�� ������ 
		{
			pos = pCharacter->GetWeaponBonePos(0);//�տ��� �߻�			
		}

		SkillEffectFunc::CreateHandEffect( pCharacter, pCharacter->GetObjectKey(), StrToWzID("D004"), pos);
	}

	return TRUE;
}
