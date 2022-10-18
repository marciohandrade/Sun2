#include "SunClientPrecompiledHeader.h"

#include "statebase.h"
#include "SkillState.h"
#include "SkillStateFactory.h"
#include "Hero.h"
#include "Character.h"
#include "Clone.h"
#include "CloneManager.h"
#include "GlobalFunc.h"
#include "GameFunc.h"
#include "ObjectManager.h"
#include "HeroActionInput.h"
#include "SkillInfoParser.h"
#include "PlayerAttributes.h"
#include "SkillEffectFunc.h"
#include "InterfaceManager.h"

#include "SkillStateDestroyCrystalWarp.h"

SkillStateDestroyCrystalWarp::SkillStateDestroyCrystalWarp()
{
}

SkillStateDestroyCrystalWarp::~SkillStateDestroyCrystalWarp(void)
{
}

BOOL SkillStateDestroyCrystalWarp::CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState)
{

	return TRUE;
}

STATECHANGE::TYPE SkillStateDestroyCrystalWarp::OnStart(Object* pOwner, DWORD CurTime)
{
	Character *pChr = (Character *)pOwner;


	if (g_pHero)
	{
		if (g_pHero->GetObjectKey() != pChr->GetObjectKey())
		{
			DWORD dwAttackSerial = pChr->GetNextActionAttackSerial();

			pChr->SetCurrentAttackSerial(dwAttackSerial);
			pChr->SetNextAttackSerial(dwAttackSerial);
		}

		if(g_pHero->GetObjectKey() == pChr->GetObjectKey())
		{
			//////////////////////////////////////////////////////////////////////////
			MSG_CG_CRYSTALWARP_COMMAND_SYN SendPacket;
			SendPacket.m_wvPos = c_ZeroVector;
			SendPacket.m_dwCrystalWarpObjKey = g_pHero->GetTargetObject()->GetObjectKey();
			SendPacket.m_byCommand = eCRYSTALWARP_COMMAND_DESTROY;
			if(FALSE == GlobalFunc::SendPacket( CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket) ))
			{
				assert(!"패킷 전송 실패");
				return STATECHANGE::FAILED;
			}
			//////////////////////////////////////////////////////////////////////////
		}
	}

	pChr->OnUseSkill();


	BASE_SKILLINFO *pInfo = GetSkillInfo();
	assert(pInfo);

	//if(!pInfo)
	//{
	//	if(g_pHero==pOwner)
	//	{
	//		FILE *fp = fopen("SkillCrashLog.txt","wt");
	//		if (fp)
	//		{
	//			fprintf(fp,"Hero SkillCode 이상 SkillCode : %d",m_dwSkillCode);
	//			fclose(fp);
	//		}
	//	}
	//	else
	//	{
	//		FILE *fp = fopen("SkillCrashLog.txt","wt");
	//		if (fp)
	//		{
	//			fprintf(fp,"Character SkillCode 이상 SkillCode : %d",m_dwSkillCode);
	//			fclose(fp);
	//		}
	//	}
	//}

	if(pInfo)
	{
		const PLAYER_ACTION *pAction = pChr->GetCurrentAction();
		if(pInfo->m_wzidCastAniCode.id != StrToWzID("NULL"))//캐스팅코드가 있다면 
		{
			pChr->SetAnimation(pInfo->m_wzidCastAniCode.id,TRUE);
			m_isCastAni=TRUE;
			m_dwTotalCastTime=pInfo->m_wSkillCasting;//캐스팅 시간 

			if(pChr==g_pHero)
			{
				g_InterfaceManager.ShowWindow(InterfaceManager::DIALOG_CASTING,TRUE);
				//캐스팅 이벤트 삽입 

				keyMsg msg;
				ZeroMemory(&msg,sizeof(msg));
				msg.dwType=eDoType_cast_cancel;
				msg.DoSomething=GlobalFunc::DoCastCancel;
				//msg.bPriority=1;
				g_KeyQueueManager.PushBack(msg);
				g_pHero->SetCast(m_isCastAni);

			}


			// Casting Effect
			if( m_hCastHandle[0] != INVALID_HANDLE_VALUE )
			{
				g_pEffectManager->RemoveEffectPtr( m_hCastHandle[0] );
			}
			m_hCastHandle[0] = SkillEffectFunc::CreateCastingSkillEffect( m_dwSkillClassCode, pChr );

		}
		else
		{
			StartSkillAnimation( pChr );
		}

		if(pInfo->m_byTarget==SKILL_TARGET_ENEMY||
			pInfo->m_byTarget==SKILL_TARGET_FRIEND||
			pInfo->m_byTarget==SKILL_TARGET_FRIEND_CORPSE||
			pInfo->m_byTarget==SKILL_TARGET_REACHABLE_ENEMY||
			pInfo->m_byTarget==SKILL_TARGET_REACHABLE_FRIEND ||
			pInfo->m_byTarget==SKILL_TARGET_ENEMY_PLAYER 
			|| pInfo->m_byTarget==SKILL_TARGET_ENEMY_AND_ME 
			)
		{
			Character *pTarget = (Character *)pChr->GetTargetObject();

			if (pTarget)
			{
				pChr->SetDirection(pTarget->GetPosition());
			}

		}
		else if(pInfo->m_byTarget==SKILL_TARGET_AREA)
		{
			WzVector vTarget=pChr->GetCurrentAction()->SKILL.vTargePos;
			pChr->SetDirection(vTarget);
		}

		// Casting BottomEffect
		if( m_hCastHandle[1] != INVALID_HANDLE_VALUE )
		{
			g_pEffectManager->RemoveEffectPtr( m_hCastHandle[1] );
		}
		m_hCastHandle[1] = SkillEffectFunc::CreateCastingSkillBottomEffect( m_dwSkillClassCode, pChr );

		// StartSkillEffect
		m_dwStartProjectileKey = SkillEffectFunc::CreateStartSkillEffect( m_dwSkillClassCode, pChr );


		if( pInfo->m_szSkillMasterEffectName )
		{
			if( StrnCmp ( pInfo->m_szSkillMasterEffectName , _T("NULL") , MAX_SKILLNAME_LENGTH  ) != 0 )
			{
				GlobalFunc::CreateEffectBeUpToOption(StrToWzID(pInfo->m_szSkillMasterEffectName), pChr->GetBonePos(1),pChr->GetScale());
			}
		}

	}

	return STATECHANGE::SUCCESS;
}

STATEPROCESS::TYPE SkillStateDestroyCrystalWarp::OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick)
{
	EnterProcess(pOwner,CurTime,dwTick);

	Character*	pChr = (Character*)pOwner;
	CheckEvent();

	//첫번째이벤트에 발사체를 날린다.
	if (CheckNotProcessedHitEvent(1)) 
	{
		++m_iHitProcessed;
	}

    // 대부분의 State에서 공통적으로 처리하는 상태변화를 처리한다.
    if (pChr->CommonStateProcess())
    {
        return STATEPROCESS::SUSPEND;
    }

	return LeaveProcess(pOwner,CurTime,dwTick);
}

STATECHANGE::TYPE SkillStateDestroyCrystalWarp::OnEnd(Object* pOwner, DWORD CurTime)
{
	return STATECHANGE::SUCCESS;
}