#include "SunClientPrecompiledHeader.h"
#include "AutobotActionRecall.h"
#include "Autobot/Autobot.h"
#include "Autobot/Area/Field/AutobotAreaField.h"
#include "Autobot/Area/Village/AutobotAreaVillage.h"
#include "Autobot/Condition/AutobotCondition.h"
#include "Autobot/Condition/AutobotConditionManager.h"
#include "Autobot/Action/Global/AutobotActionTargeting.h"
#include "Autobot/Action/Global/AutobotActionPickup.h"
#include "Autobot/Action/Global/AutobotActionPotion.h"
#include "Autobot/Action/Global/AutobotActionInterval.h"

#include "SCItemSlot.h"
#include "GlobalData.h"
#include "Hero.h"
#include "HeroActionInput.h"
#include "InventoryDialog.h"
#include "MouseHandler.h"
#include "GameFramework.h"
#include "ItemManager.h"
#include "SceneBase.h"
#include "ItemInfoParser.h"
#include "CooltimeManager.h"
#include "GlobalFunc.h"
#include "CastLeaveFieldDialog.h"

//------------------------------------------------------------------------------
AutobotActionRecall* AutobotActionRecall::Instance()
{
    static AutobotActionRecall instance;

    return &instance;
}
//------------------------------------------------------------------------------
AutobotActionRecall::AutobotActionRecall()
{
}
//------------------------------------------------------------------------------
AutobotActionRecall::~AutobotActionRecall()
{
}


void _OnRecallCancel(DWORD wParam,DWORD lParam)
{
    CastLeaveFieldDialog* castleave_dialog = GET_CAST_DIALOG(CastLeaveFieldDialog, StrToWzID("0281"));    
    if (castleave_dialog == NULL)
    {
        return;
    }

    castleave_dialog->ShowWindow(FALSE);

    Autobot::Instance()->Stop(false);
}


bool AutobotActionRecall::UseRecallItem()
{
    SCItemSlot* item_slot = ItemManager::Instance()->FindFirstItemSlot(SI_INVENTORY, eITEMTYPE_SPECIAL, eITEMWASTE_RETURNSCROLL);
    if( item_slot == NULL )
        return false;

    if( item_slot->GetNum() <= 0 )
        return false;

    BASE_ITEMINFO* item_info = ItemInfoParser::Instance()->GetItemInfo(item_slot->GetCode());
    if (item_info == NULL)
    {
        return false;
    }

    util::Timer* cool_timer = xGetCooltimeManager()->GetItemCooltime(item_info->m_wCoolTimeType);
    if ((cool_timer != NULL) && (cool_timer->IsExpired(FALSE) == FALSE)) 
    {
        return false;
    }

    if (g_pHero->CanUseItem(item_info) == FALSE)
    {
        return false;
    }

    keyMsg sendMsg;
    ZeroMemory( &sendMsg, sizeof( sendMsg ) );
    sendMsg.dwType		= eDoType_Delay_Use_Return_Stone;
    sendMsg.DoSomething	= _OnRecallCancel;
    sendMsg.lParam		= InterfaceManager::DIALOG_DELAY_CASTING;
    sendMsg.wParam		= eDoType_Delay_Use_Return_Stone;
    g_KeyQueueManager.PushBack( sendMsg );

    ItemManager::Instance()->SetPendingUseItemSlotIdx(item_slot->GetSlotIdx());
    ItemManager::Instance()->SetPendingUseItemPos(item_slot->GetPos());

    //아이템 락이 성공 하면 케스팅 창을 띄운다
    ItemManager::Instance()->SetItemLock(item_slot->GetSlotIdx(), item_slot->GetPos(), true);
    StartLeaveFieldCastTimer( eDoType_Delay_Use_Return_Stone, DELAY_LEAVE_FIELD_TICK );

    return true;
}



//------------------------------------------------------------------------------
void AutobotActionRecall::Enter(AutobotArea* owner)
{
    if( UseRecallItem() == false )
    {
        owner->state_machine()->ChangeState("ready");
    }
}

//------------------------------------------------------------------------------
void AutobotActionRecall::Exit(AutobotArea* owner)
{
    AutobotAreaVillage::Instance()->set_return_field(true);
}

//------------------------------------------------------------------------------
bool AutobotActionRecall::Execute(AutobotArea* owner)
{
    CastLeaveFieldDialog* cast_dialog = 
        (CastLeaveFieldDialog *)g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_DELAY_CASTING);

    if( cast_dialog->IsVisible() == FALSE )
    {
        owner->state_machine()->ChangeState("ready");
        return false;
    }

    if (g_pHero->IsDead() == TRUE)
    {
        owner->state_machine()->ChangeState("ready");
        return false;
    }

    return true;
}

//------------------------------------------------------------------------------
bool AutobotActionRecall::NetworkProc(AutobotArea* owner, MSG_BASE* packet)
{
    if (packet->m_byCategory == CG_ITEM)
    {
        switch (packet->m_byProtocol)
        {
        case CG_ITEM_USE_NAK:
            {
                AutobotAreaVillage::Instance()->set_return_field(false);
                owner->state_machine()->ChangeState("ready");
            }
            break;
        }
    }

    return true;
}