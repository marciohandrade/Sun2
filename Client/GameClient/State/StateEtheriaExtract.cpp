//------------------------------------------------------------------------------
//  StateSit.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"

#include "statebase.h"
#include "StateEtheriaExtract.h"
#include "player.h"
#include "hero.h"
#include "CastDialog.h"
#include "HeroActionInput.h"
#include "CharInfoParser.h"
#include "InterfaceManager.h"
#include "GlobalFunc.h"
#include "InventoryDialog.h"
#include "ItemManager.h"
#include <SCItemSlot.h>
//------------------------------------------------------------------------------
/**
*/
StateEtheriaExtract::StateEtheriaExtract(void)
{
}

//------------------------------------------------------------------------------
/**
*/
StateEtheriaExtract::~StateEtheriaExtract(void)
{
}
//------------------------------------------------------------------------------
/**
*/
BOOL StateEtheriaExtract::CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState)
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
STATECHANGE::TYPE StateEtheriaExtract::OnStart(Object* pOwner, DWORD CurTime)
{
    Player*	player_ptr = (Player*)pOwner;

    if (player_ptr == NULL ||
        player_ptr->IsKindOfObject(PLAYER_OBJECT) == FALSE)
    {
        return STATECHANGE::FAILED;
    }

    player_ptr->set_etheria_extract_state((BYTE)kExtart_Start);
    player_ptr->set_etheria_extract_time(0);
    player_ptr->SetAnimation(StrToWzID("C007"), FALSE);
    
    CastDialog* cast_dlg = (CastDialog*)g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_CASTING);   

    if (cast_dlg &&
        (player_ptr == g_pHero))
    {
        m_bOldLockState = g_pHero->IsLockSkill();
        g_pHero->SetLockSkill(TRUE);

        if (cast_dlg == NULL || 
           (cast_dlg->IsVisibleWindow() == TRUE))
        {
            return STATECHANGE::SUCCESS;
        }
        
        //6096	추출중..
        TCHAR message[INTERFACE_STRING_LENGTH + 1] = {0,};
        g_InterfaceManager.GetInterfaceString(6096, message);
        cast_dlg->SetText(message);
        cast_dlg->SetGage(0.f);
        cast_dlg->ShowWindow(TRUE);
        cast_dlg->SetCastType(CastDialog::DO_Etheria);

        keyMsg msg;
        ZeroMemory(&msg,sizeof(msg));
        msg.dwType=eDoType_cast_cancel;
        msg.DoSomething=GlobalFunc::DoCastCancel;
        g_pHero->SetCast(TRUE);
        g_KeyQueueManager.PushBack(msg);
     }
    
    return STATECHANGE::SUCCESS;
}

//------------------------------------------------------------------------------
/**
*/
STATEPROCESS::TYPE StateEtheriaExtract::OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick)
{
    Player*	player_ptr = (Player*)pOwner;

    if (player_ptr == NULL ||
        player_ptr->IsKindOfObject(PLAYER_OBJECT) == FALSE)
    {
        return STATEPROCESS::END;
    }

    float prograss_state = player_ptr->ProcessAnimation(dwTick);

    eExtract state = (eExtract)player_ptr->etheria_extract_state();

    

    if (prograss_state >= 100.f)
    {
        //루프애니로 전이
        if (state == kExtart_Start)
        {
            player_ptr->SetAnimation(StrToWzID("C008"), TRUE);
            player_ptr->set_etheria_extract_state(kExtart_Loop);
            return STATEPROCESS::HOLD;
        }
        else if (state == kExtart_End)
        {
            return STATEPROCESS::END;
        }
    }

   
    if (state != kExtart_End)
    {
        DWORD current_time = player_ptr->etheria_extract_time();
        float rate_time =  (float)current_time / EtheriaControl::kCastingDelay;
        bool is_hero = (player_ptr == g_pHero);

        // 캐스팅 시간체크
        if (rate_time > 1.0f)
        {
            rate_time = 1.f;
            player_ptr->SetAnimation(StrToWzID("C009"), FALSE);
            player_ptr->set_etheria_extract_state(kExtart_End);
         
            if (is_hero)
            {
                POSTYPE slot_pos =  g_pHero->GetCurrentAction()->ETHERIA.slot_pos;
                POSTYPE etherealizer_pos =  g_pHero->GetCurrentAction()->ETHERIA.etherealizer_pos;
                ItemManager::Instance()->SendEtheriaExtractPacket(slot_pos, etherealizer_pos);
            }
            return STATEPROCESS::HOLD;
        }
       
        if (is_hero)
        {
            CastDialog* cast_dlg = (CastDialog*)g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_CASTING);
            cast_dlg->SetGage(rate_time*100.0f);
        }

        player_ptr->set_etheria_extract_time(current_time + dwTick);
     }

    // 대부분의 State에서 공통적으로 처리하는 상태변화를 처리한다.
    if (player_ptr->CommonStateProcess())
    {
        return STATEPROCESS::SUSPEND;
    }

    return STATEPROCESS::HOLD;
}
//------------------------------------------------------------------------------
/**
*/
STATECHANGE::TYPE StateEtheriaExtract::OnEnd(Object* pOwner, DWORD CurTime)
{
    Player*	player_ptr = (Player*)pOwner;

    if (player_ptr &&
        player_ptr->IsKindOfObject(PLAYER_OBJECT))
    {
        if(player_ptr == g_pHero)
        {
            g_SkillQueue.ClearAll();
            g_HeroInput.SetAutoAttack( FALSE );
            g_pHero->SetLockSkill(m_bOldLockState);
            g_pHero->SetCast(FALSE);
            g_InterfaceManager.ShowWindow(InterfaceManager::DIALOG_CASTING, FALSE);
            uiInventoryMan* inventory_manager = GET_DIALOG_MANAGER(uiInventoryMan, UIMAN_INVENTORY);
            if (inventory_manager != NULL)
            {
                inventory_manager->UnlockInventoryUI();
            }
        }
        
        player_ptr->set_etheria_extract_time(0);
        player_ptr->set_etheria_extract_state(kExtart_Start);
    }

    return STATECHANGE::SUCCESS;
}

