#include "SunClientPrecompiledHeader.h"

#include "statebase.h"
#include "StateFireCracker.h"
#include "Character.h"
#include "ProjectileManagerEx.h"
#include "ItemInfoParser.h"
#include "Map.h"
#include "GlobalFunc.h"
#include "Hero.h"

StateFireCracker::StateFireCracker(void)
{

}
StateFireCracker::~StateFireCracker(void)
{

}

BOOL StateFireCracker::CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState)
{
	return TRUE;
}

STATECHANGE::TYPE StateFireCracker::OnStart(Object* pOwner, DWORD CurTime)
{

	Character*	pChr = (Character*)pOwner;
	// 애니메이션 결정을 가장먼저해준다.
	if (pChr->IsKindOfObject(PLAYER_OBJECT))
	{
		Player * pPlayer = (Player *) pOwner;

		 DWORD dwItemcode =pChr->GetCurrentAction()->FIRECRACKER.dwItemCode;
		 BASE_ITEMINFO * pInfo = ItemInfoParser::Instance()->GetItemInfo(dwItemcode);
		 if( !pInfo )
			 return STATECHANGE::FAILED;


		switch( pInfo->m_wType)
		{
		case eITEMTYPE_FIRECRACKER:
			{
				pChr->SetAnimation(StrToWzID("I003"));
			}
			break;


		case eITEMTYPE_POLLEN:
		case eITEMTYPE_TORCH:
			{
				pChr->SetAnimation(StrToWzID("I002"));
			}
			break;
		case eITEMTYPE_MINE:
			{
				pChr->SetAnimation(StrToWzID("I004"));
			}
			break;
		}

	}
	else
	{
		return STATECHANGE::FAILED;
	}

	return STATECHANGE::SUCCESS;
}
STATEPROCESS::TYPE StateFireCracker::OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick)
{

	Character*	pChr = (Character*)pOwner;


	WzEvent event;	
	pChr->ProcessInterpolation(dwTick);
	float fRet = pChr->ProcessAnimation(dwTick, &event);

	if( fRet >= 100.f )
	{
		if (pChr->ProcessNextAction())
		{
			return STATEPROCESS::SUSPEND;
		}
		else
		{
			return STATEPROCESS::END;

		}
	}

	BOOL bCheckEvent=FALSE;
	if (event.m_iNumCustom)
	{
		for (int a = 0; a < event.m_iNumCustom; a++)
		{
			// 
			switch(event.m_Custom[a].m_iaParam3[0])
			{
				// 
			case 1:
				{
					bCheckEvent=TRUE;
				}
				break;
			}
		}
	}

	if( bCheckEvent )
	{

		DWORD dwEffecID = 0;
		DWORD dwEndID = 0;
		WzVector wvTargetPos;
		

		DWORD dwItemcode = pChr->GetCurrentAction()->FIRECRACKER.dwItemCode;
		BASE_ITEMINFO * pInfo = ItemInfoParser::Instance()->GetItemInfo(dwItemcode);
		if( pInfo )
		{
			switch( pInfo->m_wType)
			{
			case eITEMTYPE_FIRECRACKER:
				{
					wvTargetPos = pChr->GetVisiblePos() + pChr->GetDirection() *5.0f;
					wvTargetPos.z += 5.0f + pChr->GetMeshHeight();

                    cProjectileUnit* pProjectile = new cProjectileUnit();
                    pProjectile->SetArrowStartPosition( pChr->GetVisiblePos() );
                    pProjectile->SetArrowTargetPosition( wvTargetPos );
                    pProjectile->SetLifeTime( 500 );
                    pProjectile->CreateEffect( StrToWzID("E280"), pChr->GetObjectKey(), 0 );
                    if( pInfo->m_dwEffectCode != 0 )
                    {
                        pProjectile->SetHitData( pChr->GetObjectKey(), 0, pInfo->m_dwEffectCode, FALSE, 0, FALSE );
                    }
                    g_ProjectileManagerEx.InsertProjectile( pProjectile );
				}
				break;

			case eITEMTYPE_POLLEN:
				{
					GlobalFunc::CreateEffectBeUpToOption(StrToWzID("E300"), pChr->GetVisiblePos() );
				}
				break;

			case eITEMTYPE_TORCH:
				{
                    wvTargetPos = pChr->GetCurrentAction()->FIRECRACKER.vTargetPos;

                    //타일로 바꾸자.
                    float fT=0.0f;
                    int iMesh = g_pMap->GetWorldBase()->PathExp_GetMeshToStand( wvTargetPos, &fT, 100.0f,TMGO_NO_COLLISION);
                    if ( iMesh >= 0&&fT)//딛을수 있는 곳인가
                    {
                        wvTargetPos.z -= fT;
                    }

                    cProjectileTroch* pProjectile = new cProjectileTroch();
                    pProjectile->SetPosition( pChr->GetWeaponBonePos(0), wvTargetPos, 2.0f, 4.0f );
                    pProjectile->SetLifeTime( 500 );
                    pProjectile->CreateEffect( StrToWzID("E305"), pChr->GetObjectKey(), 0 );
                    g_ProjectileManagerEx.InsertProjectile( pProjectile );

				}
				break;
			}

			if( pInfo->m_wType != eITEMTYPE_TORCH)
			{
				if( g_pHero && g_pHero->GetObjectKey() == pChr->GetObjectKey() )
				{
					//아이템 스킬의 실질적인 사용에 대한 SYN(타격점에 맞쳐서 보낸다) 단, TORCH(횃불)은 발사체가 땅에 닿았을때 SYN을 보내게 된다.
					GlobalFunc::NET_SEND_CG_ITEM_USE_SKILL_ACTION_SYN((eITEMTYPE)pInfo->m_wType, pChr->GetCurrentAction()->FIRECRACKER.vTargetPos);
				}
			}
		}
	}


	return STATEPROCESS::HOLD;
}

STATECHANGE::TYPE StateFireCracker::OnEnd(Object* pOwner, DWORD CurTime)
{

	return STATECHANGE::SUCCESS;
}
