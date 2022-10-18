#include "SunClientPrecompiledHeader.h"

#include "AutobotActionPotion.h"
#include "Autobot/Autobot.h"
#include "Autobot/Condition/AutobotConditionManager.h"
#include "Autobot/Area/Field/AutobotAreaField.h"

#include "SCItemSlot.h"
#include "GlobalData.h"
#include "Hero.h"
#include "HeroActionInput.h"
#include "mousehandler.h"
#include "InventoryDialog.h"
#include "ItemInfoParser.h"
#include "itemmanager.h"

//------------------------------------------------------------------------------
AutobotActionPotion* AutobotActionPotion::Instance()
{
  static AutobotActionPotion instance;

  return &instance;
}
//------------------------------------------------------------------------------
AutobotActionPotion::AutobotActionPotion()
{
}
//------------------------------------------------------------------------------
AutobotActionPotion::~AutobotActionPotion()
{
}
//------------------------------------------------------------------------------
void AutobotActionPotion::Enter(AutobotArea* owner)
{
}
//------------------------------------------------------------------------------
bool AutobotActionPotion::Execute(AutobotArea* owner)
{
    bool is_used = false;

    if ((is_used == false) && (AutobotConditionManager::Instance()->recovery().check_hp_potion()))
    {
        is_used = UsePotion(eITEMTYPE_WASTE, eITEMWASTE_HPPOTION);
    }
    if ((is_used == false) && (AutobotConditionManager::Instance()->recovery().check_mp_potion()))
    {
        is_used = UsePotion(eITEMTYPE_WASTE, eITEMWASTE_MPPOTION);
    }
    if ((is_used == false) && (AutobotConditionManager::Instance()->recovery().check_feed_potion()))
    {
        is_used = UsePotion(eITEMTYPE_PET_FEED, eITEMWASTE_PET_FEED);
    }
    return true;
}
//------------------------------------------------------------------------------
bool AutobotActionPotion::UsePotion(eITEMTYPE item_type, eWASTETYPE waste_type)
{
    SCItemSlot* item_slot = ItemManager::Instance()->FindFirstItemSlot(SI_INVENTORY, item_type, waste_type);
    if (item_slot != NULL)
    {
        return Autobot::Instance()->UseItem(item_slot);
    }

    return false;
}
