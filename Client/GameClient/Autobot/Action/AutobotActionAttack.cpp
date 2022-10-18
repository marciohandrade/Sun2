#include "SunClientPrecompiledHeader.h"
#include "AutobotActionAttack.h"
#include "Autobot/Autobot.h"
#include "Autobot/Passpoint/AutobotPasspoint.h"
#include "Autobot/Passpoint/AutobotPasspointManager.h"
#include "Autobot/Condition/AutobotConditionManager.h"
#include "Autobot/Area/Field/AutobotAreaField.h"
#include "Autobot/Action/Global/AutobotActionTargeting.h"

#include "Hero.h"
#include "HeroActionInput.h"
#include "TargetDialog.h"
#include "uiTargetMan/uiTargetMan.h"
#include "ObjectManager.h"
#include "SkillInfoParser.h"
#include "GameFramework.h"
#include "ItemManager.h"
#include "SceneBase.h"

//------------------------------------------------------------------------------
AutobotActionAttack* AutobotActionAttack::Instance()
{
  static AutobotActionAttack instance;

  return &instance;
}
//------------------------------------------------------------------------------
AutobotActionAttack::AutobotActionAttack()
{
}
//------------------------------------------------------------------------------
AutobotActionAttack::~AutobotActionAttack()
{
}
//------------------------------------------------------------------------------
void AutobotActionAttack::Enter(AutobotArea* owner)
{
    //owner->state_machine()->PopGlobalState(AutobotActionTargeting::Instance());
}
//------------------------------------------------------------------------------
void AutobotActionAttack::Exit(AutobotArea* owner)
{
    if (g_HeroInput.IsAttackSkillArea() == TRUE)
    {
        g_HeroInput.SetAreaSkill(FALSE);
        g_SkillQueue.ClearAll();
    }
    //owner->state_machine()->PushGlobalState(AutobotActionTargeting::Instance());
}
//------------------------------------------------------------------------------
bool AutobotActionAttack::Execute(AutobotArea* owner)
{
    if (g_pHero->IsDead() == TRUE)
    {
        owner->state_machine()->ChangeState("ready");
        return false;
    }

    //------------------------------------------------------------------------------
    if (AutobotConditionManager::Instance()->hunting().hunting_type() == AutobotCondition_Hunting::kHuntingType_Party)
    {
        if (ProcessFollowParty(owner) == false)
        {
            return false;
        }
    }
    else if (AutobotConditionManager::Instance()->hunting().hunting_type() == AutobotCondition_Hunting::kHuntingType_Area)
    {
        if (ProcessAreaRange(owner) == false)
        {
            return false;
        }
    }

    //------------------------------------------------------------------------------
    if (AutobotActionTargeting::Instance()->HasAttackHeroByAvoidMonster())
    {
        //! 런
        int next_tile_no = Autobot::Instance()->GetRandomDestinationTile(10);
        if (next_tile_no >= 0)
        {
            WzVector pos = g_pSunTerrain->x_pPathFinder->GetRandomPosInTile(next_tile_no);
            AutobotPasspointManager::Instance()->Push(AutobotPasspoint::kPasspoint_Runaway,
                                                        0,
                                                        pos,
                                                        ACTION_ATTACK);
        }

        owner->state_machine()->ChangeState("move");    //도착 지점까지 이동한다.
        return false;
    }
    
    if (AutobotActionTargeting::Instance()->IsEnemyTarget(NULL) == false)
    {
        owner->state_machine()->ChangeState("ready");
        return false;
    }

    //------------------------------------------------------------------------------
    if ((g_HeroInput.IsAttackSkillArea() == TRUE) && (g_HeroInput.IsPicking() == FALSE))
    {
        SLOTCODE skill_code = AutobotConditionManager::Instance()->skill().GetAttackSkill();
        WzVector target_pos;
        if (GetAreaSkillTargetPosition(skill_code, target_pos) == true)
        {
            g_HeroInput.OnLClickedPosition(&target_pos);
            g_HeroInput.ProcessQueueAction();
        }
        else
        {
            g_HeroInput.SetAreaSkill(FALSE);
            g_SkillQueue.ClearAll();
        }
    }
    else
    {
        SLOTCODE skill_code = AutobotConditionManager::Instance()->skill().GetAttackSkill();
        if (Autobot::Instance()->CanUseSkill(skill_code))
        {
            g_HeroInput.OnSkill(skill_code);
            g_HeroInput.ProcessQueueAction();
        }
    }

    return true;
}
//------------------------------------------------------------------------------
bool AutobotActionAttack::ProcessFollowParty(AutobotArea* owner)
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

    return true;
}
//------------------------------------------------------------------------------
bool AutobotActionAttack::ProcessAreaRange(AutobotArea* owner)
{
    float range = AutobotConditionManager::Instance()->hunting().area_range();
    WzVector pos = AutobotConditionManager::Instance()->hunting().area_position();
    WzVector gab = g_pHero->GetPosition() - pos;

    if (range < VectorLength(&gab))
    {
        AutobotPasspointManager::Instance()->Push(AutobotPasspoint::kPasspoint_Return,
                                                    0,
                                                    pos,
                                                    ACTION_ATTACK);

        owner->state_machine()->ChangeState("move");
        return false;
    }

    return true;
}
//------------------------------------------------------------------------------
bool AutobotActionAttack::GetAreaSkillTargetPosition(SLOTCODE skill_code, WzVector& target_pos)
{
    TargetDialog* target_dialog = GET_CAST_DIALOG(TargetDialog, IM_TARGET_MANAGER::TARGET_DIALOG);
    if ((target_dialog == NULL) || (target_dialog->IsVisible() == false))
    {
        return false;
    }

    Object* object = g_ObjectManager.GetObject(target_dialog->GetTargetKey());
    if (object == NULL)
    {
        return false;
    }

    SkillScriptInfo* skill_info = SkillInfoParser::Instance()->GetSkillInfo( skill_code );
    if (skill_info == NULL)
    {
        return false;
    }

    target_pos = object->GetPosition();

    float len = (float)skill_info->m_wSkillRange / 10.0f;
    WzVector vDistance = target_pos - g_pHero->GetPosition();
    float fDistance = VectorLength(&vDistance);

    if( fDistance >= len)
    {
        WzVector direction;
        VectorNormalize(&direction, &vDistance);
        target_pos = g_pHero->GetPosition() + (direction * (len - 0.1f));

        // 대상 Tile 얻기  
        int tile_no = g_pSunTerrain->x_pPathFinder->GetTileToStand(target_pos, NULL, -1.0f, 10.0f);
        if (tile_no < 0)
        {
            // 지정 할수 없는 지역 입니다.
            target_pos = g_pHero->GetPosition();
        }
    }

    return true;
}