#include "SunClientPrecompiledHeader.h"
#include "AutobotActionReadyVillage.h"
#include "Autobot/Autobot.h"
#include "Autobot/Passpoint/AutobotPasspoint.h"
#include "Autobot/Passpoint/AutobotPasspointManager.h"
#include "Autobot/Condition/AutobotConditionManager.h"
#include "Autobot/Area/Village/AutobotAreaVillage.h"

#include "Hero.h"
#include "HeroActionInput.h"
#include "Map.h"
#include "NPC.h"
#include "NPCInfoParser.h"

//------------------------------------------------------------------------------
AutobotActionReadyVillage* AutobotActionReadyVillage::Instance()
{
  static AutobotActionReadyVillage instance;

  return &instance;
}
//------------------------------------------------------------------------------
AutobotActionReadyVillage::AutobotActionReadyVillage()
{
}
//------------------------------------------------------------------------------
AutobotActionReadyVillage::~AutobotActionReadyVillage()
{
}
//------------------------------------------------------------------------------
bool AutobotActionReadyVillage::Execute(AutobotArea* owner)
{
    AutobotPasspoint* check_point = AutobotPasspointManager::Instance()->Current();
    if (check_point != NULL)
    {
        owner->state_machine()->ChangeState("move");
        return false;
    }

    // 판매나 구매등 아직 남아있다면 상점으로
    if( Autobot::Instance()->HasSellItem(NULL) == true || 
        Autobot::Instance()->HasBuyItem(NULL,NULL) == true )
    {
        owner->state_machine()->ChangeState("shop");
        return false;
    }

    if (AutobotAreaVillage::Instance()->is_return_field() && 
        AutobotConditionManager::Instance()->recall().is_return_field())
    {
        owner->state_machine()->ChangeState("returnfield");
        return false;
    }
    else
    {
        Autobot::Instance()->Stop(false);
    }

    //if ((AutobotAreaVillage::Instance()->is_shop_finish() == false) && 
    //    ((Autobot::Instance()->HasSellItem(NULL) || Autobot::Instance()->HasBuyItem(NULL, NULL))))
    //{
    //    if (AutobotAreaVillage::Instance()->shop_npc() == NULL)
    //    {
    //        //! 잡상인 찾을때 까지 대기
    //    }
    //    else
    //    {
    //        int tile_no = GameFunc::GetNearestTile(AutobotAreaVillage::Instance()->shop_npc()->GetPosition(), PTA_ONLY_JUMP | PTA_NO_WALK);

    //        if (tile_no >= 0)
    //        {
    //            WzVector random_pos = g_pSunTerrain->x_pPathFinder->GetRandomPosInTile(tile_no);

    //            AutobotPasspoint pass_point(AutobotPasspoint::kPasspoint_Temporarily, 
    //                                        0,
    //                                        random_pos,
    //                                        ACTION_COLLECTION_OBJECT);

    //            if (pass_point.IsArrival(g_pHero->GetPosition()) == true)
    //            {
    //                owner->state_machine()->ChangeState("shop");
    //                return false;
    //            }
    //            else
    //            {
    //                AutobotPasspointManager::Instance()->Push(pass_point);
    //                owner->state_machine()->ChangeState("move");
    //                return false;
    //            }
    //        }
    //    }
    //}

    //if (AutobotAreaVillage::Instance()->is_return_field() && 
    //    AutobotConditionManager::Instance()->recall().is_return_field())
    //{
    //    owner->state_machine()->ChangeState("returnfield");
    //    return false;
    //}
    //else
    //{
    //    Autobot::Instance()->Stop(false);
    //}

    //if ((AutobotAreaVillage::Instance()->waypoint() != NULL) &&
    //    (AutobotConditionManager::Instance()->hunting().map_no() > 0))
    //{
    //    AutobotPasspoint pass_point(AutobotPasspoint::kPasspoint_Temporarily, 
    //                                AutobotAreaVillage::Instance()->waypoint()->GetObjectKey(), 
    //                                AutobotAreaVillage::Instance()->waypoint()->GetPosition(),
    //                                ACTION_WAYPOINT_MEET);

    //    if (pass_point.IsArrival(g_pHero->GetPosition()) == true)
    //    {
    //        owner->state_machine()->ChangeState("waypoint");
    //        return false;
    //    }
    //    else
    //    {
    //        AutobotPasspointManager::Instance()->Push(pass_point);
    //        owner->state_machine()->ChangeState("move");
    //        return false;
    //    }
    //}
    
    
    return true;
}
