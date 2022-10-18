#include "SunClientPrecompiledHeader.h"

#include "AutobotActionTargeting.h"
#include "Autobot/Autobot.h"
#include "Autobot/Condition/AutobotCondition.h"
#include "Autobot/Condition/AutobotConditionManager.h"
#include "Autobot/Area/Field/AutobotAreaField.h"

#include "ObjectManager.h"
#include "Hero.h"
#include "Monster.h"
#include "HeroActionInput.h"
#include "SkillInfoParser.h"
#include "TargetDialog.h"
#include "uiTargetMan/uiTargetMan.h"
#include "uiPartyMan/uiPartyMan.h"
#include "GameFramework.h"
#include "SceneBase.h"

//------------------------------------------------------------------------------
AutobotActionTargeting* AutobotActionTargeting::Instance()
{
  static AutobotActionTargeting instance;

  return &instance;
}
//------------------------------------------------------------------------------
AutobotActionTargeting::AutobotActionTargeting()
{
}
//------------------------------------------------------------------------------
AutobotActionTargeting::~AutobotActionTargeting()
{
}
//------------------------------------------------------------------------------
bool AutobotActionTargeting::Execute(AutobotArea* owner)
{
    bool is_update = false;

    if (AutobotConditionManager::Instance()->hunting().hunting_type() == AutobotCondition_Hunting::kHuntingType_Max)
    {
        //! 사냥이 설정되어 있지 않음
        return true;
    }
    if (AutobotConditionManager::Instance()->skill().GetAttackCount() <= 0)
    {
        //! 등록된 공격 스킬이 없을경우에는 타겟팅을 하지 않음
        return true;
    }
    
    if (g_pHero->GetCurState() == STATE::MOVE)
    {
        is_update = true;
    }
    else if ((g_pHero->GetCurState() == STATE::IDLE) || (g_pHero->GetCurState() == STATE::SKILL))
    {
        TargetDialog* target_dialog = GET_CAST_DIALOG(TargetDialog, IM_TARGET_MANAGER::TARGET_DIALOG);
        if ((target_dialog == NULL) || (target_dialog->IsVisible() == false))
        {
            is_update = true;
        }
        else
        {
            Object* object = g_ObjectManager.GetObject(target_dialog->GetTargetKey());
            if((object != NULL) && (object->IsKindOfObject(PLAYER_OBJECT) == TRUE))
            {
                is_update = true;
            }
        }
    }
  
    if (is_update)
    {
        float range = AutobotConditionManager::Instance()->hunting().target_range();
        if (g_HeroInput.UpdateTargetList(range, g_HeroInput.GetForwardTargeting(), TRUE) == TRUE)
        {
            g_pHero->m_CurTargetInfo.dwObjectKey = 0;
            g_HeroInput.OnNextTarget();
            g_HeroInput.ProcessTargeting();
        }
    }

    return true;
}
//------------------------------------------------------------------------------
bool AutobotActionTargeting::IsEnemyTarget(OUT DWORD* object_key)
{
    TargetDialog* target_dialog = GET_CAST_DIALOG(TargetDialog, IM_TARGET_MANAGER::TARGET_DIALOG);
    if ((target_dialog == NULL) || (target_dialog->IsVisible() == FALSE))
    {
        return false;
    }

    if (target_dialog->GetTargetKey() == g_pHero->GetObjectKey())
    {
        return false;
    }

    //------------------------------------------------------------------------------
    //! 죽었는지 체크 한다.
    Object* object = g_ObjectManager.GetObject(target_dialog->GetTargetKey());
    if((object == NULL) || (object->IsKindOfObject(CHARACTER_OBJECT) == FALSE))
    {
        return false;
    }
    if (static_cast<Character*>(object)->IsDead() == TRUE)
    {
        target_dialog->ShowInterface(FALSE);
        return false;
    }
    //------------------------------------------------------------------------------

    if (object_key)
    {
        *object_key = target_dialog->GetTargetKey();
    }

    uiPartyMan* party_manager = GET_DIALOG_MANAGER(uiPartyMan, UIMAN_PARTY);
    if (party_manager != NULL)
    {
        return !(party_manager->IsPartyMember(target_dialog->GetTargetKey()));
    }

    return true;
}
//------------------------------------------------------------------------------
bool AutobotActionTargeting::IsFriendTarget(OUT DWORD* object_key)
{
    TargetDialog* target_dialog = GET_CAST_DIALOG(TargetDialog, IM_TARGET_MANAGER::TARGET_DIALOG);
    if ((target_dialog == NULL) || (target_dialog->IsVisible() == FALSE))
    {
        return false;
    }

    if (target_dialog->GetTargetKey() == g_pHero->GetObjectKey())
    {
        return true;
    }

    if (object_key)
    {
        *object_key = target_dialog->GetTargetKey();
    }

    uiPartyMan* party_manager = GET_DIALOG_MANAGER(uiPartyMan, UIMAN_PARTY);
    if (party_manager != NULL)
    {
        return (party_manager->IsPartyMember(target_dialog->GetTargetKey()));
    }

    return false;
}
//------------------------------------------------------------------------------
Monster* AutobotActionTargeting::GetNearestMonster()
{
    Monster* nearest_object = NULL;
    float nearest_length = 100000.0f;

    Object_Map_Itr begin = g_ObjectManager.GetBegin();
    Object_Map_Itr end = g_ObjectManager.GetEnd();

    for (; begin != end; ++begin)
    {
        Object* object = begin->second;

        if (object->IsKindOfObject(MONSTER_OBJECT) == FALSE)
        {
            continue;
        }

        if ((AutobotConditionManager::Instance()->hunting().IsAvoidMonster(object->GetObjectKey()) == true) || 
            (AutobotConditionManager::Instance()->hunting().IsMonsterInValidArea(object->GetObjectKey()) == false))
        {
            continue;
        }
        
        Monster* monster = static_cast<Monster*>(object);

        RELATIONKIND::TYPE relation_type = GameFramework::GetCurrentScene()->GetRelationKindOfCharacter(monster, FALSE);

        if (relation_type == RELATIONKIND::Enemy)
        {
            if ((monster->IsDead() == TRUE) || (monster->IsObserverMode() == TRUE))
            {
                continue;
            }

            WzVector gab = monster->GetPosition() - g_pHero->GetPosition();
            float length = VectorLength(&gab);
            if (length < nearest_length)
            {
                nearest_length = length;
                nearest_object = monster;
            }
        }
    }

    return nearest_object;
}
//------------------------------------------------------------------------------
bool AutobotActionTargeting::HasAttackHeroByAvoidMonster()
{
    if (g_pHero == NULL)
    {
        return false;
    }

    vector<DWORD> monster_vector;
    monster_vector.clear();

    if (GameFunc::GetMonstetListTargetPlayer(monster_vector, g_pHero->GetObjectKey()) > 0)
    {
        vector<DWORD>::iterator begin = monster_vector.begin();
        vector<DWORD>::iterator end = monster_vector.end();
        for (; begin != end; ++begin)
        {
            if (AutobotConditionManager::Instance()->hunting().IsAvoidMonster(*begin))
            {
                return true;
            }
        }
    }

    return false;
}
//------------------------------------------------------------------------------
void AutobotActionTargeting::TargetOff()
{
    TargetDialog* target_dialog = GET_CAST_DIALOG(TargetDialog, IM_TARGET_MANAGER::TARGET_DIALOG);
    if ((target_dialog != NULL) && (target_dialog->IsVisible() == TRUE))
    {
        target_dialog->ShowInterface(FALSE);
    }
}