//------------------------------------------------------------------------------
//  StateSit.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"

#include "statebase.h"
#include "StateSummonRider.h"
#include "player.h"
#include "hero.h"
#include "CastDialog.h"
#include "RiderContainer.h"
#include "HeroActionInput.h"
#include "RidingInfoParser.h"
#include "CharInfoParser.h"
#include "InterfaceManager.h"
#include "GlobalFunc.h"
#include <SCItemSlot.h>
//------------------------------------------------------------------------------
/**
*/
StateSummonRider::StateSummonRider(void):
m_dwStartTime(0),
m_dwMaxTime(0),
m_bWaitResult(0)
{
}

//------------------------------------------------------------------------------
/**
*/
StateSummonRider::~StateSummonRider(void)
{
}
//------------------------------------------------------------------------------
/**
*/
BOOL StateSummonRider::CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState)
{
	switch(pToTransiteState->GetStateKind())
	{
	case 0:
		break;
	}
	return TRUE;
}

//------------------------------------------------------------------------------
/**
*/
STATECHANGE::TYPE StateSummonRider::OnStart(Object* pOwner, DWORD CurTime)
{
	Player*	pPlayer = (Player*)pOwner;
	assert(pPlayer->IsKindOfObject(PLAYER_OBJECT));
	
    
    if(!pPlayer)
    {
        return STATECHANGE::FAILED;
    }
    
    INIT_CHARINFO *pInfo = CharInfoParser::Instance()->GetInitCharInfo(pPlayer->GetClass());

    if(pInfo)
    {
        pPlayer->SetAnimation(pInfo->m_RiderSummonCode,TRUE);
    }
	
    m_dwStartTime = 0;
    m_bWaitResult = FALSE;
	
    if(pPlayer==g_pHero)
	{
        m_bOldLockState = g_pHero->IsLockSkill();
        g_pHero->SetLockSkill(TRUE);

        SCItemSlot & rItemSlot =  (SCItemSlot &)g_pHero->GetSlotContainer(SI_INVENTORY)->GetSlot(g_pHero->GetCurrentAction()->RIDEROBJECT.ItemPos);
        
        if(rItemSlot.IsContainValidItem() && rItemSlot.IsRiderItem())
        {
            const  sRiderInfo* pRiderInfo = RidingInfoParser::Instance()->GetInfo(rItemSlot.GetItemInfo()->m_wSkillCode);
            assert(pRiderInfo);
            m_dwMaxTime	  = 5000; 
            if(pRiderInfo)
            {
                m_dwMaxTime = pRiderInfo->spawn_timer_;//스폰타임설정
            }
        }
        else
        {
            return STATECHANGE::FAILED;
        }

        //! 날개는 해제
        g_pHero->SendWingsControlByAccessory(false, false); 
        
		CastDialog * pCastDlg =	(CastDialog * )g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_CASTING);
		if( pCastDlg && (FALSE == pCastDlg->IsVisibleWindow()) )
		{
			//11243	소환중..
			TCHAR szCollect[INTERFACE_STRING_LENGTH + 1] = {0,};
			g_InterfaceManager.GetInterfaceString(11243, szCollect);
			pCastDlg->SetText(szCollect);
			pCastDlg->SetGage(0.f);
			pCastDlg->ShowWindow(TRUE);

			keyMsg msg;
			ZeroMemory(&msg,sizeof(msg));
			msg.dwType=eDoType_cast_cancel;
			msg.DoSomething=GlobalFunc::DoCastCancel;
            g_pHero->SetCast(TRUE);
			g_KeyQueueManager.PushBack(msg);
		}
	}

	return STATECHANGE::SUCCESS;
}

//------------------------------------------------------------------------------
/**
*/
STATEPROCESS::TYPE StateSummonRider::OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick)
{
	Player*	pPlayer = (Player*)pOwner;
	assert(pPlayer->IsKindOfObject(PLAYER_OBJECT));

	pPlayer->ProcessAnimation(dwTick);

    // 대부분의 State에서 공통적으로 처리하는 상태변화를 처리한다.
    if (pPlayer->CommonStateProcess())
    {
        return STATEPROCESS::SUSPEND;
    }

    //주인공일때만 게이지를 체크 Player들은 외부에서 해줘야됨
	if(pPlayer == g_pHero)
	{
        if( g_pHero->m_RiderInfo.m_bCancelSummon == TRUE )
        {
            return STATEPROCESS::END;
        }

        float fRate =  (float)m_dwStartTime / m_dwMaxTime;

        if (fRate < 0.0f)
        {
            fRate = 0.0f;
        }
        else if(fRate > 1.0f)
        {
            fRate = 1.0f;
        }

		CastDialog * pCastDlg =	(CastDialog * )g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_CASTING);
		if(pCastDlg)
		{
			pCastDlg->SetGage(fRate*100.0f);
		}

        if(m_dwStartTime > m_dwMaxTime)
        {
            // 시간이 되면 변신 패킷을 보내준다
            if( m_bWaitResult == FALSE )
            {
                MSG_CG_ITEM_RIDER_CONTROL_SYN SendMsg;
                //탈것소환패킷(request==eReq_Summon)
                SendMsg.header.request = SendMsg.header.eReq_Summon;
                SendMsg.header.args.summon.slot_pos = g_pHero->GetCurrentAction()->RIDEROBJECT.ItemPos;
                GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendMsg, sizeof(SendMsg) );

                m_bWaitResult = TRUE;
            }

            // 응답을 받은 후 라이딩 상태가 되면 종료
            if(pPlayer->IsRiding())
            {
                return STATEPROCESS::END;
            }

            // 나머지는 대기
            return STATEPROCESS::HOLD;
        }

        m_dwStartTime+=dwTick;

	}
    else
    {
        //safecode:소환캐스팅상태에서 타는상태라면 애니끝내준다
        if(pPlayer->IsRiding())
        {
            return STATEPROCESS::END;
        }
    }

    return STATEPROCESS::HOLD;
}
//------------------------------------------------------------------------------
/**
*/
STATECHANGE::TYPE StateSummonRider::OnEnd(Object* pOwner, DWORD CurTime)
{
    if(pOwner == g_pHero)
    {
        g_SkillQueue.ClearAll();
        g_HeroInput.SetAutoAttack( FALSE );
        g_pHero->SetLockSkill(m_bOldLockState);
        g_pHero->SetCast(FALSE);
        g_InterfaceManager.ShowWindow(InterfaceManager::DIALOG_CASTING,FALSE);
    }
	return STATECHANGE::SUCCESS;
}


