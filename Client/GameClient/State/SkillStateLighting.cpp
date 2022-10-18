//------------------------------------------------------------------------------
//  SkillStateLighting.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------

#include "SunClientPrecompiledHeader.h"
#include "statebase.h"
#include "SkillState.h"
#include "SkillStateLighting.h"
#include "Character.h"
#include "ProjectileManagerEx.h"
#include "SkillInfoParser.h"
#include "SkillEffectFunc.h"



BOOL SkillStateLighting::OnProcessCustemEvent( int nEventIndex, Object* pOwner, DWORD CurTime,DWORD dwTick )
{
	if( nEventIndex == 1 )
	{

		//Character* pCharacter = (Character*)pOwner;
		//Character* pTarget = (Character *)pCharacter->GetTargetObject();

		//WzVector pos = pCharacter->GetArrowBonePos(0);

		//if(pCharacter->GetVisiblePos()==pos)//무기본이 없으면 
		//{
		//	pos = pCharacter->GetWeaponBonePos(0);//손에서 발사			
		//}

		//SkillEffectFunc::CreateHandEffect( pCharacter, pCharacter->GetObjectKey(), StrToWzID("D000"), pos);


		//BASE_SKILLINFO *pInfo = GetSkillInfo();
		//assert(pInfo);

		//PROJECTILE_PARAM param;

		//ZeroMemory(&param,sizeof(param));
		//param.fHeight = 0.0f;
		//
		//if(pTarget)
		//	param.vStartPos = pTarget->GetVisiblePos();
		//else
		//	param.vStartPos = pCharacter->GetVisiblePos();
		//
		//param.vStartPos.z+=2.0f;

		//param.vTargetPos =param.vStartPos;
		//param.dwLifeTime = 1000;
		//param.dwMoveTime = 1000;

		//param.fSpeed = 3.0f;

		//param.dwType=PROJECTILE::PROJECTILEID_COMMON;
		//param.dwAttackSerial = pCharacter->GetCurrentAttackSerial();
		//param.dwOwner = pCharacter->GetObjectKey();
		//param.dwImageCode = pInfo->m_dwFlyingObjectCode;
		//param.effectID = StrToWzID("I031");
		//param.bAddNextSerial=FALSE;
		//param.bAllDamage=TRUE;
		//g_ProjectileManager.CreateProjectile(param);


		//ZeroMemory(&param,sizeof(param));
		//param.fHeight = 0.0f;

		//if(pTarget)
		//	param.vStartPos = pTarget->GetVisiblePos();
		//else
		//	param.vStartPos = pCharacter->GetVisiblePos();

		//param.vStartPos.z+=0.5f;
		//param.vTargetPos =param.vStartPos;
		//param.dwLifeTime = 5000;
		//param.dwMoveTime = 5000;
		//param.fSpeed = 3.0f;

		//param.dwType=PROJECTILE::PROJECTILEID_COMMON;
		//param.dwAttackSerial = pCharacter->GetCurrentAttackSerial();
		//param.dwOwner = pCharacter->GetObjectKey();
		//param.dwImageCode = pInfo->m_dwFlyingObjectCode;
		//param.effectID = StrToWzID("F040");
		//param.bAddNextSerial=FALSE;
		//param.bAllDamage=TRUE;
		//g_ProjectileManager.CreateProjectile(param);

		////

		//ZeroMemory(&param,sizeof(param));
		//param.fHeight = 0.0f;

		//if(pTarget)
		//	param.vStartPos = pTarget->GetVisiblePos();
		//else
		//	param.vStartPos = pCharacter->GetVisiblePos();

		//param.vStartPos.z+=0.1f;
		//param.vStartPos.x+=0.5f;

		//

		//param.vTargetPos =param.vStartPos;
		//param.dwLifeTime = 600;
		//param.dwMoveTime = 600;

		//param.fSpeed = 3.0f;

		//param.dwType=PROJECTILE::PROJECTILEID_COMMON;
		//param.dwAttackSerial = pCharacter->GetCurrentAttackSerial();
		//param.dwOwner = pCharacter->GetObjectKey();
		//param.dwImageCode = pInfo->m_dwFlyingObjectCode;
		//param.effectID = StrToWzID("F041");
		//
		//param.bAddNextSerial=FALSE;
		//param.bAllDamage=TRUE;
		//g_ProjectileManager.CreateProjectile(param);

		////
		//ZeroMemory(&param,sizeof(param));
		//param.fHeight = 0.0f;

		//if(pTarget)
		//	param.vStartPos = pTarget->GetVisiblePos();
		//else
		//	param.vStartPos = pCharacter->GetVisiblePos();

		//param.vStartPos.z+=0.1f;
		//param.vStartPos.x-=0.5f;
		//

		//param.vTargetPos =param.vStartPos;	
		//param.dwLifeTime = 600;
		//param.dwMoveTime = 600;

		//param.fSpeed = 3.0f;

		//param.dwType=PROJECTILE::PROJECTILEID_COMMON;
		//param.dwAttackSerial = pCharacter->GetCurrentAttackSerial();
		//param.dwOwner = pCharacter->GetObjectKey();
		//param.dwImageCode = pInfo->m_dwFlyingObjectCode;
		//param.effectID = StrToWzID("F041");
		//
		//param.bAddNextSerial=TRUE;
		//param.bAllDamage=TRUE;
		//g_ProjectileManager.CreateProjectile(param);
	}

	return FALSE;
}
