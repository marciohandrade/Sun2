//------------------------------------------------------------------------------
//  SkillStatePoisonDagger.cpp
//  (C) 2008 Webzen Sunstudio
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "statebase.h"
#include "SkillState.h"
#include "SkillStateDarkSpark.h"
#include "SkillInfoParser.h"
#include "ProjectileManagerEx.h"
#include "ProjectileInfoParser.h"
#include "Character.h"
#include "Hero.h"



BOOL SkillStateDarkSpark::OnProcessCustemEvent( int nEventIndex, Object* pOwner, DWORD CurTime,DWORD dwTick )
{
	if( nEventIndex == 1 )
	{
		Character* pCharacter = (Character*)pOwner;
		Character* pTarget = (Character *)pCharacter->GetTargetObject();

        if( cProjectileUnit* pProjectile = g_ProjectileManagerEx.CreateSkillFlyingObject( pCharacter, pTarget, GetSkillInfo() ) )
        {
            pProjectile->SetCameraShakeHit( pOwner->GetObjectKey()==g_pHero->GetObjectKey() );
        }
        return FALSE;

        
  //      BOOL IsHeroCharacter = pOwner->GetObjectKey()==g_pHero->GetObjectKey();
		//
		//if( SkillScriptInfo* pInfo = GetSkillInfo() )
  //      {
		//    //�߻�ü�ڵ� ���
		//    BASE_ProjectileInfo *pInfoProjectIile = ProjectileInfoParser::Instance()->GetProjectileInfo(pInfo->m_dwFlyingObjectCode);
		//    if( pInfoProjectIile )
		//    {
  //              g_ProjectileManagerEx.Create

		//	    PROJECTILE_PARAM param;
		//	    ZeroMemory(&param,sizeof(param));
  //  				
		//	    param.fHeight = 0.03f;
		//	    //������ġ�� ����չٴ�
		//	    param.vStartPos = pCharacter->GetWeaponBonePos(0);
  //  			
		//	    Character *pTarget = (Character *)pCharacter->GetTargetObject();

		//	    if(pTarget)
		//	    {
		//		    param.vTargetPos = pTarget->GetVisiblePos();
		//		    param.vTargetPos.z += (pTarget->GetArrowTargetHeight());
		//		    param.dwTargetID=pTarget->GetObjectKey();
		//	    }
		//	    else
		//	    {
		//		    //Ÿ���� ������ ������ �߻�
		//		    param.vTargetPos=param.vStartPos+pCharacter->GetDirection()*4.0f;
		//	    }
  //  			
		//	    param.dwType=PROJECTILE::PROJECTILEID_COMMON;
		//	    param.dwAttackSerial = pCharacter->GetCurrentAttackSerial();
		//	    param.dwOwner = pCharacter->GetObjectKey();		
		//	    float fSkillRange=((float)pInfo->m_wSkillRange / 10.0f);
		//	    float fRate = VectorLength(&(param.vTargetPos-param.vStartPos))/fSkillRange;
		//	    param.dwMoveTime = (DWORD)(1000.f*fRate);
		//	    param.dwMoveTime = max(400,param.dwMoveTime);
		//	    param.dwMoveTime = min(1200,param.dwMoveTime);
  //  						
		//	    //��ũ��Ʈ �߻�ü���� ���Ժκ�	
		//	    param.dwImageCode = pInfoProjectIile->m_dwImageID;
		//	    param.effectID	  = pInfoProjectIile->m_wzidFlyingEffectID.id;
		//	    param.EndeffectID = pInfoProjectIile->m_wzidHitEffectID.id;
		//	    param.fSpeed	  = 5.0f;
  //  			
		//	    //��������üũ
		//	    if(pInfoProjectIile->m_bMakeTrail)
		//	    {
		//		    param.bMakeTrail			= pInfoProjectIile->m_bMakeTrail;
		//		    param.fTrailLength			= 0.8f;
		//	    }
  //  			
		//	    param.bAddNextSerial=TRUE;
		//	    g_ProjectileManager.CreateProjectile(param);

		//	    return FALSE;
  //          }
		//}
	}

	return TRUE;
}
