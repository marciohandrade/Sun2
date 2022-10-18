//------------------------------------------------------------------------------
//  SkillStateBladeFury.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "statebase.h"
#include "SkillState.h"
#include "SkillStateBladeFury.h"
#include "Character.h"
#include "ProjectileManagerEx.h"
#include "ProjectileInfoParser.h"
#include "SkillInfoParser.h"


BOOL SkillStateBladeFury::OnProcessCustemEvent( int nEventIndex, Object* pOwner, DWORD CurTime,DWORD dwTick )
{
    if( nEventIndex == 1 || nEventIndex == 2 )
    {
        Character* pCharacter = (Character*)pOwner;
        Character* pTarget = (Character *)pCharacter->GetTargetObject();

        if (cProjectileUnit* pProjectile  = g_ProjectileManagerEx.CreateSkillFlyingObject( pCharacter, pTarget, GetSkillInfo()))
        {
            pProjectile->SetAddNextAttackSerial(TRUE);
        }

        return FALSE;
    }

    return TRUE;
}
