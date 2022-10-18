#include "SunClientPrecompiledHeader.h"
#include "AutobotActionShop.h"
#include "Autobot/Autobot.h"
#include "Autobot/Area/Village/AutobotAreaVillage.h"
#include "Autobot/Condition/AutobotCondition.h"
#include "Autobot/Condition/AutobotConditionManager.h"
#include "Autobot/Passpoint/AutobotPasspoint.h"
#include "Autobot/Passpoint/AutobotPasspointManager.h"

#include "ItemInfoParser.h"
#include "Hero.h"
#include "NPC.h"
#include "HeroActionInput.h"
#include "InventoryDialog.h"
#include "MouseHandler.h"
#include "GameFramework.h"
#include "ItemManager.h"
#include "SceneBase.h"
#include "InterfaceManager.h"
#include "ShopDialog.h"
#include "PacketRequestManager.h"
#include "Interface/uiQuestCommonForward/uiQuestCommonForward.h"
#include "Interface/uiQuestMan/uiQuestMan.h"

//------------------------------------------------------------------------------
AutobotActionShop* AutobotActionShop::Instance()
{
  static AutobotActionShop instance;

  return &instance;
}
//------------------------------------------------------------------------------
AutobotActionShop::AutobotActionShop()
{
}
//------------------------------------------------------------------------------
AutobotActionShop::~AutobotActionShop()
{
}
//------------------------------------------------------------------------------
void AutobotActionShop::Enter(AutobotArea* owner)
{
}
//------------------------------------------------------------------------------
void AutobotActionShop::Exit(AutobotArea* owner)
{
}
//------------------------------------------------------------------------------
bool AutobotActionShop::Execute(AutobotArea* owner)
{
    if (PacketRequestManager::Instance()->is_sent(CG_ITEM, CG_ITEM_SELL_SYN) || 
        PacketRequestManager::Instance()->is_sent(CG_ITEM, CG_ITEM_BUY_SYN) || 
        PacketRequestManager::Instance()->is_sent(CG_ITEM, CG_ITEM_REPAIR_DURA_SYN))
    {
        return true;
    }


    SLOTCODE item_code = 0;
    NUMTYPE buy_count = 0;
    POSTYPE sell_item_position = INVALID_POSTYPE_VALUE;
    bool need_repair = false;

    Autobot::Instance()->HasBuyItem(&item_code, &buy_count);
    Autobot::Instance()->HasSellItem(&sell_item_position);
    need_repair = ItemManager::Instance()->IsExistRepairItem(SI_EQUIPMENT) == TRUE;


    NPC* shop_npc = NULL;

    if( item_code != 0 )
    {
        shop_npc = GameFunc::GetNearestShopNpcByItem(item_code);
    }
    else if( sell_item_position != INVALID_POSTYPE_VALUE || need_repair == true )
    {
        shop_npc = GameFunc::GetNearestShopNpc(eITEMTYPE_WASTE);
    }
    else
    {
        // 사거나 팔 아이템이 없다
        owner->state_machine()->ChangeState("ready");
        return false;
    }



    if( shop_npc == NULL )
    {
        // 상점을 찾을 수 없음
        Autobot::Instance()->Stop(false);
        return false;
    }

    int tile_index = GameFunc::GetNearestTile( shop_npc->GetPosition(), PTA_ONLY_JUMP | PTA_NO_WALK);
    if( tile_index < 0 )
    {
        // 위치로 이동할수 없음
        Autobot::Instance()->Stop(false);
        return false;
    }

    WzVector random_pos = g_pSunTerrain->x_pPathFinder->GetRandomPosInTile(tile_index);
    AutobotPasspoint pass_point(AutobotPasspoint::kPasspoint_Temporarily, shop_npc->GetObjectKey(), shop_npc->GetPosition(), ACTION_NPCMEET );

    // NPC가 멀리 있다면 이동
    if (pass_point.IsArrival(g_pHero->GetPosition()) == false)
    {
        AutobotPasspointManager::Instance()->Push(pass_point);
        owner->state_machine()->ChangeState("move");
        return false;
    }


    {
        uiQuestCommonForward* questcommonforward_dialog = GET_CAST_DIALOG(uiQuestCommonForward, IM_QUEST_MANAGER::QUEST_COMMON_FORWARD);
        if (questcommonforward_dialog && (questcommonforward_dialog->IsVisible() == TRUE))
        {
            questcommonforward_dialog->clickBtnJob(0);
            return true;
        }

        ShopDialog* shop_dialog = GET_CAST_DIALOG(ShopDialog, InterfaceManager::DIALOG_SHOP);
        InventoryDialog* inventory_dialog = GET_CAST_DIALOG(InventoryDialog, IM_INVENTORY_MANAGER::INVEN_DLG);
        if (shop_dialog == NULL || inventory_dialog == NULL)
        {
            // UI가 없어
            Autobot::Instance()->Stop(false);
            return false;
        }

        // 상점창이 열려있지 않다면 창을 연다
        if ((shop_dialog->IsVisible() == FALSE) || (inventory_dialog->IsVisible() == FALSE))
        {
            RELATIONKIND::TYPE relation_type = GameFunc::GetRelationKindOfHero(shop_npc);
            g_HeroInput.OnLClickedCharacter(shop_npc, relation_type, false);
            g_HeroInput.ProcessQueueAction();
            return true;
        }
    }



    if( sell_item_position != INVALID_POSTYPE_VALUE )
    {
        Autobot::Instance()->SellItem(sell_item_position);
    }
    else if( item_code != 0 )
    {
        if( Autobot::Instance()->BuyItem(item_code, buy_count) == false )
        {
            // 돈이 없어
            Autobot::Instance()->Stop(false);
        }
    }
    else if( need_repair == true )
    {
        if( Autobot::Instance()->RepairItemWearAll() )
        {
            // 돈이 없어
            Autobot::Instance()->Stop(false);
        }
    }


    //// 구매패킷을 보냄
    //if( Autobot::Instance()->BuyItem(item_code, buy_count) == false )
    //{
    //    // 아이템을 구매할 수 없음
    //    owner->state_machine()->ChangeState("stop");
    //    return false;
    //}

    //







    //if (AutobotAreaVillage::Instance()->is_shop_finish() == true)            
    //{
    //    owner->state_machine()->ChangeState("ready");
    //    return false;
    //}

    //AutobotPasspoint pass_point(AutobotPasspoint::kPasspoint_Temporarily, 
    //                            AutobotAreaVillage::Instance()->shop_npc()->GetObjectKey(), 
    //                            AutobotAreaVillage::Instance()->shop_npc()->GetPosition(),
    //                            ACTION_NPCMEET);
    //if (pass_point.IsArrival(g_pHero->GetPosition()) == false)
    //{
    //    owner->state_machine()->ChangeState("ready");
    //    return false;
    //}

    //uiQuestCommonForward* questcommonforward_dialog = GET_CAST_DIALOG(uiQuestCommonForward, IM_QUEST_MANAGER::QUEST_COMMON_FORWARD);
    //if (questcommonforward_dialog && (questcommonforward_dialog->IsVisible() == TRUE))
    //{
    //    questcommonforward_dialog->clickBtnJob(0);
    //    return true;
    //}

    //ShopDialog* shop_dialog = GET_CAST_DIALOG(ShopDialog, InterfaceManager::DIALOG_SHOP);
    //InventoryDialog* inventory_dialog = GET_CAST_DIALOG(InventoryDialog, IM_INVENTORY_MANAGER::INVEN_DLG);
    //if (shop_dialog == NULL || inventory_dialog == NULL)
    //{
    //    return true;
    //}
    //if ((shop_dialog->IsVisible() == FALSE) || (inventory_dialog->IsVisible() == FALSE))
    //{
    //    Character* character = static_cast<Character*>(AutobotAreaVillage::Instance()->shop_npc());
    //    RELATIONKIND::TYPE relation_type = GameFunc::GetRelationKindOfHero(character);
    //    g_HeroInput.OnLClickedCharacter(character, relation_type, false);
    //    g_HeroInput.ProcessQueueAction();
    //    return true;
    //}

    //if (PacketRequestManager::Instance()->is_sent(CG_ITEM, CG_ITEM_SELL_SYN) || 
    //    PacketRequestManager::Instance()->is_sent(CG_ITEM, CG_ITEM_BUY_SYN) || 
    //    PacketRequestManager::Instance()->is_sent(CG_ITEM, CG_ITEM_REPAIR_DURA_SYN))
    //{
    //    return true;
    //}

    //POSTYPE inventory_slot_pos = 0;
    //SLOTCODE item_code = 0;
    //NUMTYPE buy_count = 0;
    //if (Autobot::Instance()->HasSellItem(&inventory_slot_pos) == true)
    //{
    //    Autobot::Instance()->SellItem(inventory_slot_pos);
    //}
    //else if (ItemManager::Instance()->IsExistRepairItem(SI_EQUIPMENT) == TRUE)
    //{
    //    if (Autobot::Instance()->RepairItemWearAll() == false)
    //    {
    //        AutobotAreaVillage::Instance()->set_shop_finish(true);
    //    }
    //}
    //else if (Autobot::Instance()->HasBuyItem(&item_code, &buy_count) == true)
    //{
    //    if (Autobot::Instance()->BuyItem(item_code, buy_count) == false)
    //    {
    //        AutobotAreaVillage::Instance()->set_shop_finish(true);
    //    }
    //}
    //else
    //{
    //    AutobotAreaVillage::Instance()->set_shop_finish(true);
    //}

    return true;
}
