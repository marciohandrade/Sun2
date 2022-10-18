#include "SunClientPrecompiledHeader.h"

#include "AutobotActionInterval.h"
#include "Autobot/Autobot.h"
#include "Autobot/Condition/AutobotConditionManager.h"
#include "Autobot/Area/Field/AutobotAreaField.h"

#include "Hero.h"
#include "HeroActionInput.h"
#include "mousehandler.h"
#include "InventoryDialog.h"
#include "ItemInfoParser.h"
#include "ItemManager.h"
#include "ObjectManager.h"
#include "SkillInfoParser.h"
#include "Interface/uiPartyMan/uiPartyMan.h"

//------------------------------------------------------------------------------
AutobotActionInterval* AutobotActionInterval::Instance()
{
  static AutobotActionInterval instance;

  return &instance;
}
//------------------------------------------------------------------------------
AutobotActionInterval::AutobotActionInterval()
{
}
//------------------------------------------------------------------------------
AutobotActionInterval::~AutobotActionInterval()
{
}
//------------------------------------------------------------------------------
bool AutobotActionInterval::Execute(AutobotArea* owner)
{
    bool is_used = false;

    //------------------------------------------------------------------------------
    if( AutobotCondition_Skill::Slot* slot = AutobotConditionManager::Instance()->skill().GetIntervalSkill() )
    {
        if( slot->slot_type == ST_ITEM )
        {
            SCItemSlot* item_slot = ItemManager::Instance()->FindFirstItemSlot(SI_INVENTORY, slot->slot_code);
            if (item_slot != NULL)
            {
                if( Autobot::Instance()->UseItem(item_slot) == true )
                {
                    slot->last_use_millisecond = clock_function::GetTickCount();
                }
            }
        }
        else if( slot->slot_type == ST_SKILL )
        {
            if( ProcessSkill(owner, slot->slot_code, NULL) == true )
            {
                slot->last_use_millisecond = clock_function::GetTickCount();
            }
        }
    }

    return true;
}
//------------------------------------------------------------------------------
bool AutobotActionInterval::ProcessSkill(AutobotArea* owner, SLOTCODE skill_code, DWORD object_key)
{
    ROOT_SKILLINFO* root_info = SkillInfoParser::Instance()->GetInfo(skill_code);

    if ((root_info != NULL) && 
        (root_info->IsSkill() == true) && 
        (Autobot::Instance()->CanUseSkill(skill_code)))
    {
        SkillScriptInfo* skill_info = static_cast<SkillScriptInfo*>(root_info);
        DWORD backup_objectkey = g_HeroInput.GetCurrentTarget();

        if (object_key > 0)
        {
            g_HeroInput.SetCurrentTarget(object_key);
            g_HeroInput.ProcessTargeting();
        }
        else
        {
            if (skill_info->m_byTarget != SKILL_TARGET_AREA)
            {
                g_HeroInput.SetCurrentTarget(g_pHero->GetObjectKey());
                g_HeroInput.ProcessTargeting();
            }
        }

        g_HeroInput.OnSkill(skill_code);
        g_HeroInput.ProcessQueueAction();

        if (backup_objectkey != g_HeroInput.GetCurrentTarget())
        {
            g_HeroInput.SetCurrentTarget(backup_objectkey);
            g_HeroInput.ProcessTargeting();
        }

        return true;
    }
    return false;
}
//------------------------------------------------------------------------------
DWORD AutobotActionInterval::GetDeadMemberKey()
{
    uiPartyMan* party_manager = GET_DIALOG_MANAGER(uiPartyMan, UIMAN_PARTY);
    if (party_manager != NULL)
    {
        STLX_VECTOR<uicPartyMemberInfoPlus>::iterator begin = party_manager->GetPartyListRef().begin();
        STLX_VECTOR<uicPartyMemberInfoPlus>::iterator end = party_manager->GetPartyListRef().end();

        for (; begin != end; ++begin)
        {
            if (begin->Get().m_dwMemberKey == g_pHero->GetObjectKey())
            {
                continue;
            }

            if (begin->IsDead() == TRUE)
            {
                return begin->Get().m_dwMemberKey;
            }
        }
    }

    return 0;
}