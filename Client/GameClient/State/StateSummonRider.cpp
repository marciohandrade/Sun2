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
                m_dwMaxTime = pRiderInfo->spawn_timer_;//����Ÿ�Ӽ���
            }
        }
        else
        {
            return STATECHANGE::FAILED;
        }

        //! ������ ����
        g_pHero->SendWingsControlByAccessory(false, false); 
        
		CastDialog * pCastDlg =	(CastDialog * )g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_CASTING);
		if( pCastDlg && (FALSE == pCastDlg->IsVisibleWindow()) )
		{
			//11243	��ȯ��..
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

    // ��κ��� State���� ���������� ó���ϴ� ���º�ȭ�� ó���Ѵ�.
    if (pPlayer->CommonStateProcess())
    {
        return STATEPROCESS::SUSPEND;
    }

    //���ΰ��϶��� �������� üũ Player���� �ܺο��� ����ߵ�
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
            // �ð��� �Ǹ� ���� ��Ŷ�� �����ش�
            if( m_bWaitResult == FALSE )
            {
                MSG_CG_ITEM_RIDER_CONTROL_SYN SendMsg;
                //Ż�ͼ�ȯ��Ŷ(request==eReq_Summon)
                SendMsg.header.request = SendMsg.header.eReq_Summon;
                SendMsg.header.args.summon.slot_pos = g_pHero->GetCurrentAction()->RIDEROBJECT.ItemPos;
                GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendMsg, sizeof(SendMsg) );

                m_bWaitResult = TRUE;
            }

            // ������ ���� �� ���̵� ���°� �Ǹ� ����
            if(pPlayer->IsRiding())
            {
                return STATEPROCESS::END;
            }

            // �������� ���
            return STATEPROCESS::HOLD;
        }

        m_dwStartTime+=dwTick;

	}
    else
    {
        //safecode:��ȯĳ���û��¿��� Ÿ�»��¶�� �ִϳ����ش�
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


