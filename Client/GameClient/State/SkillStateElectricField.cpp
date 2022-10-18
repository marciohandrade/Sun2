//------------------------------------------------------------------------------
//  SkillStateElectricField.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------

#include "SunClientPrecompiledHeader.h"
#include "statebase.h"
#include "SkillState.h"
#include "SkillStateElectricField.h"
#include "Character.h"
#include "Hero.h"
#include "soundeffect.h"
#include "MissionScene.h"
#include "ProjectileManagerEx.h"
#include "GlobalFunc.h"



BOOL SkillStateElectricField::OnProcessCustemEvent( int nEventIndex, Object* pOwner, DWORD CurTime,DWORD dwTick )
{
	if( nEventIndex == 1 )
	{
		//ReleaseCastingEffect();

		Character* pCharacter = (Character*)pOwner;
		//Character* pTarget = (Character *)pCharacter->GetTargetObject();


		//WzVector pos = pCharacter->GetArrowBonePos(0);
		//if(pCharacter->GetVisiblePos()==pos)//무기본이 없으면 
		//{
		//	pos = pCharacter->GetWeaponBonePos(0);//손에서 발사			
		//}
		//

        cTargetPosition Position;
        Position.SetTargetVisiblePosition( pCharacter->GetObjectKey(), TRUE );
        Position.SetOffsetPosition( 0.0f, 0.0f, 0.95f );

        cProjectileChaseTarget* pProjectile = new cProjectileChaseTarget();
        pProjectile->SetLifeTime( 2666 );
        pProjectile->SetPositionData( Position );
        pProjectile->CreateEffect( StrToWzID("k380"), pCharacter->GetObjectKey(), 0 );
        g_ProjectileManagerEx.InsertProjectile( pProjectile );
    }

	return TRUE;
}