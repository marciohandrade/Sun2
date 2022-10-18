#include "SunClientPrecompiledHeader.h"	 

#include "uiItemCompositeMan.h"

//#include "uiItemComposite/uiItemComposite.h"

#include "ItemCrystalizeListParser.h"
#include "ItemManager.h"
#include "interfacemanager.h"
#include "globalfunc.h"
#include "HeroActionInput.h"
#include "InventoryDialog.h"
#include "CooltimeManager.h"
#include "ItemCooltimeManager.h"
#include "SkillInfoParser.h"

void uiItemCompositeMan::_Callback_Crystalization_Confirm( uiSystemMessage::ButtonIndex button_index )
{
    if (button_index == uiSystemMessage::kConfirm_Button)
    {
        POSTYPE TargetPos = static_cast<POSTYPE>(uiSystemMessage::GetUserData());
        
        SCSlotContainer* container = ItemManager::Instance()->GetContainer(SI_INVENTORY);
        if (container == NULL)
        {
            return;
        }
        SCItemSlot& use_slot = static_cast<SCItemSlot&>(container->GetSlot(TargetPos));
        if (use_slot.IsContainValidItem() == FALSE)
        {
            return;
        }

        const BASE_ITEMINFO* selected_item_info_ptr = use_slot.GetItemInfo();

        if (selected_item_info_ptr == NULL)
        {
            return;
        }

#ifdef _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM
		SLOTCODE slot_code = ItemCrystalizeListParser::Instance()->GetCrystalizeCode(use_slot, use_slot.GetEnchant());
#else
        SLOTCODE slot_code = ItemCrystalizeListParser::Instance()->GetCrystalizeCode(use_slot);
#endif // _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM
        if (slot_code == 0)
        {
            return;
        }

        InventoryDialog* inventory_dialog_ptr = static_cast<InventoryDialog*>(GET_DIALOG(IM_INVENTORY_MANAGER::INVEN_DLG));
        if (inventory_dialog_ptr)
        {
            CooltimeStruct data;
            ZeroMemory(&data, sizeof(data));
            data.type = kCrystalization;
            //data.max_time = 8000;
            SkillScriptInfo* skill_script_info_ptr = 
                SkillInfoParser::Instance()->GetSkillInfo(35024);
            data.max_time = static_cast<float>(skill_script_info_ptr ? skill_script_info_ptr->m_dwCoolTime : 8000);
            data.Crystalization.slot_code = slot_code;
            data.Crystalization.target_pos = TargetPos;
            data.slot_index = SI_INVENTORY;
            data.item_pos = TargetPos;
            data.dealy_time = 1000;
            data.serial_key = use_slot.GetSerial();
            
            ItemCooltimeManager::Instance()->InsertItem(&data);
        }
    }
}
