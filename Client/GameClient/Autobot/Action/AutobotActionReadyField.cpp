#include "SunClientPrecompiledHeader.h"
#include "AutobotActionReadyField.h"
#include "Autobot/Autobot.h"
#include "Autobot/Condition/AutobotConditionManager.h"
#include "Autobot/Area/Field/AutobotAreaField.h"
#include "Autobot/Action/Global/AutobotActionTargeting.h"
#include "Autobot/Passpoint/AutobotPasspoint.h"
#include "Autobot/Passpoint/AutobotPasspointManager.h"

#include "SCItemSlot.h"
#include "GlobalData.h"
#include "Hero.h"
#include "Monster.h"
#include "HeroActionInput.h"
#include "ItemManager.h"
#include "ItemInfoParser.h"
#include "ObjectManager.h"

//------------------------------------------------------------------------------
AutobotActionReadyField* AutobotActionReadyField::Instance()
{
  static AutobotActionReadyField instance;

  return &instance;
}
//------------------------------------------------------------------------------
AutobotActionReadyField::AutobotActionReadyField()
{
}
//------------------------------------------------------------------------------
AutobotActionReadyField::~AutobotActionReadyField()
{
}
//------------------------------------------------------------------------------
bool AutobotActionReadyField::Execute(AutobotArea* owner)
{
    if (g_pHero->IsDead() == TRUE)
    {
        owner->state_machine()->ChangeState("die");
        return false;
    }

    if (AutobotConditionManager::Instance()->hunting().hunting_type() == AutobotCondition_Hunting::kHuntingType_Party)
    {
        if (ProcessFollowParty(owner) == false)
        {
            return false;
        }
    }
    else if ((AutobotConditionManager::Instance()->hunting().hunting_type() == AutobotCondition_Hunting::kHuntingType_Free) || 
             (AutobotConditionManager::Instance()->hunting().hunting_type() == AutobotCondition_Hunting::kHuntingType_Area))
    {
        return ProcessHuntingFree(owner);
    }

    return true;
}
//------------------------------------------------------------------------------
bool AutobotActionReadyField::ProcessHuntingFree(AutobotArea* owner)
{
    if ((owner->GetAreaType() == AutobotArea::AutobotArea_Field) && //!< 필드 타입일경우에만 마을로 돌아가기 가능
        (AutobotConditionManager::Instance()->recall().IsReturnVillage() == true))
    {
        SCItemSlot* item_slot = ItemManager::Instance()->FindFirstItemSlot(SI_INVENTORY, 
                                                                            eITEMTYPE_SPECIAL, 
                                                                            eITEMWASTE_RETURNSCROLL);
        if ((item_slot != NULL) && (item_slot->GetNum() > 0))
        {
            owner->state_machine()->ChangeState("recall");
            return false;
        }
    }

    //------------------------------------------------------------------------------
    if ((AutobotActionTargeting::Instance()->IsEnemyTarget(NULL) == false) &&
        (AutobotConditionManager::Instance()->recovery().IsRelax() == true) && 
        (g_pHero->IsTransForm() == FALSE))
    {
        owner->state_machine()->ChangeState("relax");
        return false;
    }

    if (AutobotActionTargeting::Instance()->IsEnemyTarget(NULL))
    {
        owner->state_machine()->ChangeState("attack");
        return false;
    }

    //------------------------------------------------------------------------------
    AutobotPasspoint* check_point = AutobotPasspointManager::Instance()->Current();
    if (check_point != NULL)
    {
        owner->state_machine()->ChangeState("move");
        return false;
    }

    //------------------------------------------------------------------------------
    //! 이동할 자리를 만든다.
    Monster* monster = AutobotActionTargeting::Instance()->GetNearestMonster();
    if (monster != NULL)
    {
        AutobotPasspointManager::Instance()->Push(AutobotPasspoint::kPasspoint_Temporarily,
                                                    monster->GetObjectKey(),
                                                    monster->GetPosition(),
                                                    ACTION_ATTACK);

    }
    else
    {
        if (AutobotConditionManager::Instance()->hunting().hunting_type() == AutobotCondition_Hunting::kHuntingType_Free)
        {
            int next_tile_no = Autobot::Instance()->GetRandomDestinationTile(3);
            if (next_tile_no != -1)
            {
                AutobotPasspointManager::Instance()->Push(AutobotPasspoint::kPasspoint_Temporarily,
                                                            0,
                                                            g_pSunTerrain->x_pPathFinder->GetRandomPosInTile(next_tile_no),
                                                            ACTION_ATTACK);
            }
        }
        else //if (AutobotConditionManager::Instance()->hunting().hunting_type() == AutobotCondition_Hunting::kHuntingType_Area)
        {
            WzVector pos = Autobot::Instance()->GetRandomDestinationPosition(AutobotConditionManager::Instance()->hunting().area_range() * 0.7f);

            if (AutobotConditionManager::Instance()->hunting().IsValidArea(pos))
            {
                AutobotPasspointManager::Instance()->Push(AutobotPasspoint::kPasspoint_Temporarily,
                                                            0,
                                                            pos,
                                                            ACTION_ATTACK);
            }
            else
            {
                AutobotPasspointManager::Instance()->Push(AutobotPasspoint::kPasspoint_Temporarily,
                                                            0,
                                                            AutobotConditionManager::Instance()->hunting().area_position(),
                                                            ACTION_ATTACK);
            }
        }
       
    }


    return true;
}
//------------------------------------------------------------------------------
bool AutobotActionReadyField::ProcessFollowParty(AutobotArea* owner)
{
    Object* object = g_ObjectManager.GetObject(AutobotConditionManager::Instance()->hunting().GetFollowPlayer());
    if (object != NULL)
    {
        WzVector gab = g_pHero->GetPosition() - object->GetPosition();
        if(VectorLength(&gab) > DISTANCE_FOLLOW_PLAYER)
        {
            owner->state_machine()->ChangeState("follow");
            return false;
        }
    }
    else
    {
        //! 멀리 있다 위치를 찾아 간다.
        WzVector pos;
        if (AutobotConditionManager::Instance()->hunting().GetFollowPlayerPosition(pos))
        {
            int tile_no = GameFunc::GetNearestTile(pos, PTA_ONLY_JUMP | PTA_NO_WALK);

            if (tile_no >= 0)
            {
                WzVector random_pos = g_pSunTerrain->x_pPathFinder->GetRandomPosInTile(tile_no);

                AutobotPasspoint pass_point(AutobotPasspoint::kPasspoint_FindFollowPlayer, 
                                            0,
                                            random_pos,
                                            ACTION_PLAYERFOLLOW);

                if (pass_point.IsArrival(g_pHero->GetPosition()) == false)
                {
                    AutobotPasspointManager::Instance()->Push(pass_point);
                    owner->state_machine()->ChangeState("move");
                    return false;
                }
            }
        }
    }

    //------------------------------------------------------------------------------
    if ((AutobotActionTargeting::Instance()->IsEnemyTarget(NULL) == false) &&
        (AutobotConditionManager::Instance()->recovery().IsRelax() == true) && 
        (g_pHero->IsTransForm() == FALSE))
    {
        owner->state_machine()->ChangeState("relax");
        return false;
    }

    if (AutobotActionTargeting::Instance()->IsEnemyTarget(NULL))
    {
        owner->state_machine()->ChangeState("attack");
        return false;
    }

    return true;
}