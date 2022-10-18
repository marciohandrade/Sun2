//------------------------------------------------------------------------------
//  SkillStateSonyVine.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------

#include "SunClientPrecompiledHeader.h"
#include "statebase.h"
#include "SkillState.h"
#include "SkillStateSonyVine.h"
#include "Character.h"
#include "Hero.h"
#include "ProjectileManagerEx.h"
#include "SkillInfoParser.h"
#include "SkillEffectFunc.h"


BOOL SkillStateSonyVine::OnProcessCustemEvent( int nEventIndex, Object* pOwner, DWORD CurTime,DWORD dwTick )
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

		//if(!GENERALPARAM->IsNet())
		//{	
		//	SkillScriptInfo *pInfo = GetSkillInfo();


		//	//가시덩굴 생성 
		//	PROJECTILE_PARAM param;
		//	ZeroMemory(&param,sizeof(param));
		//	param.fHeight = 0.0f;
		//	param.vStartPos =pCharacter->GetVisiblePos();		
		//	param.vTargetPos =pCharacter->GetVisiblePos();
		//	param.dwImageCode=eProjectile_SRC_THORN2;
		//	param.dwLifeTime = 2000;
		//	param.dwMoveTime = 2000;//못움직이는 시간 만큼
		//	param.fSpeed = 3.0f;
		//	param.dwType=PROJECTILE::PROJECTILEID_SONY_VINE;
		//	param.SONYVINE.aniID=StrToWzID("N001");
		//	param.dwAttackSerial =0;
		//	param.dwOwner = pCharacter->GetObjectKey();
		//	param.effectID =0;
		//	param.bAddNextSerial=TRUE;
		//	param.bAllDamage=TRUE;
		//	param.SONYVINE.fDirZ = float( rand()%628 );
		//	param.SONYVINE.fDirZ /= 100.0f;
		//	param.bFadeOut=TRUE;

		//	g_ProjectileManager.CreateProjectile(param);	
		//}
	}

	return TRUE;
}
