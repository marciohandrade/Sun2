#include "SunClientPrecompiledHeader.h"
#include "AutobotActionMove.h"
#include "Autobot/Autobot.h"
#include "Autobot/Condition/AutobotConditionManager.h"
#include "Autobot/Area/Field/AutobotAreaField.h"
#include "Autobot/Action/Global/AutobotActionTargeting.h"
#include "Autobot/Passpoint/AutobotPasspointManager.h"

#include "Hero.h"
#include "HeroActionInput.h"
#include "ObjectManager.h"
#include "MapObject.h"

//------------------------------------------------------------------------------
AutobotActionMove* AutobotActionMove::Instance()
{
  static AutobotActionMove instance;

  return &instance;
}
//------------------------------------------------------------------------------
AutobotActionMove::AutobotActionMove()
{
}
//------------------------------------------------------------------------------
AutobotActionMove::~AutobotActionMove()
{
}
//------------------------------------------------------------------------------
void AutobotActionMove::Enter(AutobotArea* owner)
{
}
//------------------------------------------------------------------------------
void AutobotActionMove::Exit(AutobotArea* owner)
{
    AutobotPasspoint* current_point = AutobotPasspointManager::Instance()->Current();
    if (current_point != NULL)
    {
        if ((current_point->passpoint_type() == AutobotPasspoint::kPasspoint_Temporarily) || 
            (current_point->passpoint_type() == AutobotPasspoint::kPasspoint_Runaway) || 
            (current_point->passpoint_type() == AutobotPasspoint::kPasspoint_FindFollowPlayer))
        {
            AutobotPasspointManager::Instance()->Pop();
        }
    }
}
//------------------------------------------------------------------------------
bool AutobotActionMove::Execute(AutobotArea* owner)
{
    //------------------------------------------------------------------------------
    if (owner->GetAreaType() == AutobotArea::AutobotArea_Field)
    {
        if (ProcessField(owner) == false)
        {
            return false;
        }
    }
    
    //------------------------------------------------------------------------------
    //! 체크 포인트 확인
    AutobotPasspoint* check_point = AutobotPasspointManager::Instance()->Current();
    if (check_point == NULL) 
    {
        //AutobotPasspointManager::Instance()->Next();
        owner->state_machine()->ChangeState("ready");
        return false;
    }


    //------------------------------------------------------------------------------
    //! 가다가 서 있으면 다시 확인해준다.
    STATE::TYPE crrrent_state = g_pHero->GetCurState();
    if ((crrrent_state != STATE::MOVE) || 
        ((crrrent_state == STATE::MOVE) && 
            (check_point != NULL) && 
            (check_point->passpoint_type() == AutobotPasspoint::kPasspoint_Return))
        )
    {
        if (g_pHero->CannotMove() == FALSE)
        {
            ActionCheckPoint(check_point);

            DWORD cur_attribute = g_pSunTerrain->x_pPathFinder->GetAttribute(g_pHero->GetPathExplorer()->GetTile());
            DWORD next_attribute = g_pSunTerrain->x_pPathFinder->GetAttribute(g_pHero->GetPathExplorer()->GetNextTile());

            if ((cur_attribute & PTA_NO_WALK) || (next_attribute & PTA_NO_WALK) ||
                (cur_attribute & PTA_ONLY_JUMP) || (next_attribute & PTA_ONLY_JUMP))
            {
				g_pHero->m_JumpData.m_JumpFlag.AddForward();
                g_pHero->SetNextState(STATE::KEYBOARDJUMP, g_CurTime);
            }
        }
    }
        
    return true;
}
//------------------------------------------------------------------------------
bool AutobotActionMove::ProcessField(AutobotArea* owner)
{
    if (g_pHero->IsDead() == TRUE)
    {
        owner->state_machine()->ChangeState("ready");
        return false;
    }

    AutobotPasspoint* check_point = AutobotPasspointManager::Instance()->Current();
    if (check_point != NULL)
    {
        if (check_point->passpoint_type() == AutobotPasspoint::kPasspoint_FindFollowPlayer)
        {
            Object* object = g_ObjectManager.GetObject(AutobotConditionManager::Instance()->hunting().GetFollowPlayer());
            if (object != NULL)
            {
                //! 따라갈 플레이어를 찾았으면
                AutobotPasspointManager::Instance()->Next();
                owner->state_machine()->ChangeState("ready");
                return false;
            }
            return true;
        }
        else if (check_point->passpoint_type() == AutobotPasspoint::kPasspoint_Runaway)
        {
            if (AutobotActionTargeting::Instance()->HasAttackHeroByAvoidMonster() == false)
            {
                AutobotPasspointManager::Instance()->Next();
                owner->state_machine()->ChangeState("ready");
                return false;
            }
            return true;
        }
        else if (check_point->passpoint_type() == AutobotPasspoint::kPasspoint_Return)
        {
            if (AutobotActionTargeting::Instance()->IsEnemyTarget(NULL))
            {
                AutobotActionTargeting::Instance()->TargetOff();
            }

            return true;
        }
    }

    //------------------------------------------------------------------------------
    if (AutobotActionTargeting::Instance()->IsEnemyTarget(NULL))
    {
        owner->state_machine()->ChangeState("attack");
        return false;
    }

    if ((AutobotActionTargeting::Instance()->IsEnemyTarget(NULL) == false) &&
        (AutobotConditionManager::Instance()->recovery().IsRelax() == true) && 
        (g_pHero->IsTransForm() == FALSE))
    {
        owner->state_machine()->ChangeState("relax");

        return false;
    }

    return true;
}
//------------------------------------------------------------------------------
void AutobotActionMove::ActionCheckPoint(AutobotPasspoint* check_point)
{
    if (check_point == NULL)
    {
        return;
    }

    if (check_point->objectkey() > 0)
    {
        Object* object = g_ObjectManager.GetObject(check_point->objectkey());
        if (object != NULL) 
        {
            if (object->IsKindOfObject(MAP_OBJECT) == TRUE)
            {
                g_HeroInput.OnLClickedMapObject(static_cast<MapObject*>(object));
                g_HeroInput.ProcessQueueAction();
            }
            else if (object->IsKindOfObject(CHARACTER_OBJECT) == TRUE)
            {
                Character* character = static_cast<Character*>(object);
                if (character->IsDead() == FALSE)
                {
                    RELATIONKIND::TYPE relation_type = GameFunc::GetRelationKindOfHero(character);
                    g_HeroInput.OnLClickedCharacter(character, relation_type, false);
                    g_HeroInput.ProcessQueueAction();

                    AutobotPasspointManager::Instance()->Next();
                }
                else
                {
                    AutobotPasspointManager::Instance()->Next();
                }
            }
        }
        else
        {
            AutobotPasspointManager::Instance()->Next();
        }
    }
    else
    {
        g_HeroInput.SetAutoAttack(FALSE);
        g_HeroInput.OnLClickedPosition(&(check_point->position()));
        g_HeroInput.ProcessQueueAction();

        AutobotPasspointManager::Instance()->Next();

    }
}