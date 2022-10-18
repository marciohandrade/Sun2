#include "SunClientPrecompiledHeader.h"

#include "AutobotActionShouting.h"
#include "Autobot/Autobot.h"
#include "Autobot/Condition/AutobotConditionManager.h"

#include "InterfaceManager.h"
#include "ChatDialog.h"
#include "ItemManager.h"
#include "Interface/uiChatMan/uiChatMan.h"

//------------------------------------------------------------------------------
AutobotActionShouting* AutobotActionShouting::Instance()
{
  static AutobotActionShouting instance;

  return &instance;
}
//------------------------------------------------------------------------------
AutobotActionShouting::AutobotActionShouting()
{
}
//------------------------------------------------------------------------------
AutobotActionShouting::~AutobotActionShouting()
{
}
//------------------------------------------------------------------------------
bool AutobotActionShouting::Execute(AutobotArea* owner)
{
    static DWORD current_time = clock_function::GetTickCount();
    static DWORD last_shouting_time = clock_function::GetTickCount();

    if (AutobotConditionManager::Instance()->community().IsShouting() == false)
    {
        return true;
    }

    current_time = clock_function::GetTickCount();

    if (current_time >= (last_shouting_time + AutobotConditionManager::Instance()->community().shouting_second() * 1000))
    {
        ChatDialog* chat_dialog = GET_CAST_DIALOG(ChatDialog, IM_CHAT_MANAGER::CHAT_DIALOG);
        if (chat_dialog != NULL)
        {
            bool is_possible_shout = false;

            if (AutobotConditionManager::Instance()->community().shouting_type() == AutobotCondition_Community::kShouting_World)
            {
                is_possible_shout = (ItemManager::Instance()->FindFirstItemSlot(SI_INVENTORY, eITEMTYPE_CHAT_SHOUT) != NULL) ||
                                    (ItemManager::Instance()->FindFirstItemSlot(SI_INVENTORY, eITEMTYPE_CHAT_SHOUT_CONSUME) != NULL);
            }
            else if (AutobotConditionManager::Instance()->community().shouting_type() == AutobotCondition_Community::kShouting_Normal)
            {
                is_possible_shout = true;
            }

            if (is_possible_shout)
            {
                chat_dialog->SendShoutMsg(AutobotConditionManager::Instance()->community().shouting_message(), 
                                            AutobotConditionManager::Instance()->community().shouting_type());

                last_shouting_time = current_time;
            }
        }
    }

    return true;
}
