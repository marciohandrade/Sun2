//------------------------------------------------------------------------------
//  SkillStateCoveredHeal.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------

#include "SunClientPrecompiledHeader.h"
#include "statebase.h"
#include "SkillState.h"
#include "SkillStateCoveredHeal.h"
#include "Character.h"
#include "ProjectileManagerEx.h"
#include "SkillInfoParser.h"

//
//BOOL SkillStateCoveredHeal::OnProcessCustemEvent( int nEventIndex, Object* pOwner, DWORD CurTime,DWORD dwTick )
//{
//	if( nEventIndex == 1 )
//	{
//
//		ReleaseCastingEffect();
//
//		Character* pCharacter = (Character*)pOwner;
//		Character* pTarget = (Character *)pCharacter->GetTargetObject();
//
//
//
//		BASE_SKILLINFO *pInfo = GetSkillInfo();
//		assert(pInfo);
//
//		PROJECTILE_PARAM param;
//
//		ZeroMemory(&param,sizeof(param));
//		param.fHeight = 1.0f;
//
//		param.vStartPos = pCharacter->GetVisiblePos() + pCharacter->GetDirection()*2;
//
//		if(pTarget)
//		{
//			param.vTargetPos = pTarget->GetVisiblePos();
//			param.vTargetPos.z += (pTarget->GetArrowTargetHeight());
//		}
//		else
//		{
//			param.vTargetPos=param.vStartPos+pCharacter->GetDirection()*4.0f;//타겟이 없을땐 앞으로 발사
//		}
//
//		param.dwLifeTime = 0;
//		param.dwMoveTime = 1500;
//		param.fSpeed = 3.0f;
//		param.dwType=PROJECTILE::PROJECTILEID_COMMON;
//		param.dwAttackSerial = pCharacter->GetCurrentAttackSerial();
//		param.dwOwner = pCharacter->GetObjectKey();
//		param.dwImageCode = pInfo->m_dwFlyingObjectCode;
//		param.effectID = StrToWzID("FE53");
//		param.bAllDamage=TRUE;
//		param.bAddNextSerial=TRUE;
//		param.bAllDamage=TRUE;
//		g_ProjectileManager.CreateProjectile(param);
//
//
//
//        cTargetPosition Position;
//        Position.SetTargetVisiblePosition( pCharacter->GetObjectKey(), TRUE );
//        Position.SetOffsetPosition( 0.0f, 0.0f, 0.95f );
//
//        g_ProjectileManagerEx.CreateEffect( pCharacter->GetObjectKey(), StrToWzID("K019"), 5000, Position );
//
//		return FALSE;
//	}
//
//	return TRUE;
//}
