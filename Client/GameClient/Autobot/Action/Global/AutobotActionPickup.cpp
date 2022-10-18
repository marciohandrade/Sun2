#include "SunClientPrecompiledHeader.h"

#include "AutobotActionPickup.h"
#include "Autobot/Autobot.h"
#include "Autobot/Area/Field/AutobotAreaField.h"

#include "Hero.h"
#include "HeroActionInput.h"
#include "InventoryDialog.h"

//------------------------------------------------------------------------------
AutobotActionPickup* AutobotActionPickup::Instance()
{
  static AutobotActionPickup instance;

  return &instance;
}
//------------------------------------------------------------------------------
AutobotActionPickup::AutobotActionPickup()
{
}
//------------------------------------------------------------------------------
AutobotActionPickup::~AutobotActionPickup()
{
}
//------------------------------------------------------------------------------
bool AutobotActionPickup::Execute(AutobotArea* owner)
{
    InventoryDialog* inventory_dialog = GET_CAST_DIALOG(InventoryDialog, IM_INVENTORY_MANAGER::INVEN_DLG);
    if ((inventory_dialog == NULL) || (inventory_dialog->GetEmptySlotNum() <= 0))
    {
        return false;
    }

    return (g_HeroInput.OnSpacePushItem(TRUE) != TRUE);;
}
