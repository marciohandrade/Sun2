#include "SunClientPrecompiledHeader.h"

#include "./uiInventoryMan/uiInventoryMan.h"
#include "InventoryFilterDialog.h"
#include "Hero.h"
#include "SCSlotStruct.h"
#include "GlobalFunc.h"

const int kInsertRule_All = (1 << 0 | 1 << 1 | 1 << 2 | 1 << 3 | 1 << 4 | 1 << 5);

WzID InventoryFilterDialog::controls_id_[INVENTORY_FILTERS_MAX] = 
{
    //����1: ��ü����, ���, ����Ʈ, AC, ����/��þƮ, �Ϲ�, �Ҹ�
    StrToWzID("C002"),//FILTER_ALL_SLOT1
    StrToWzID("CT09"),//FILTER_EQUIPMENT_SLOT1
    StrToWzID("CT19"),//FILTER_QUEST_SLOT1
    StrToWzID("C000"),//FILTER_AC_SLOT1
    StrToWzID("CT29"),//FILTER_RESOURCE_SLOT1
    StrToWzID("CT39"),//FILTER_NORMAL_SLOT1
    StrToWzID("CT49"),//FILTER_WASTE_SLOT1
    //����2: ��ü����, ���, ����Ʈ, AC, ����/��þƮ, �Ϲ�, �Ҹ�
    StrToWzID("CT00"),//FILTER_ALL_SLOT2
    StrToWzID("CT10"),//FILTER_EQUIPMENT_SLOT2
    StrToWzID("CT20"),//FILTER_QUEST_SLOT2
    StrToWzID("C001"),//FILTER_AC_SLOT2
    StrToWzID("CT30"),//FILTER_RESOURCE_SLOT2
    StrToWzID("CT40"),//FILTER_NORMAL_SLOT2
    StrToWzID("CT50"),//FILTER_WASTE_SLOT2
    //����3: ��ü����, ���, ����Ʈ, AC, ����/��þƮ, �Ϲ�, �Ҹ�
    StrToWzID("CT02"),//FILTER_ALL_SLOT3
    StrToWzID("CT11"),//FILTER_EQUIPMENT_SLOT3
    StrToWzID("CT21"),//FILTER_QUEST_SLOT3
    StrToWzID("C003"),//FILTER_AC_SLOT3
    StrToWzID("CT31"),//FILTER_RESOURCE_SLOT3
    StrToWzID("CT41"),//FILTER_NORMAL_SLOT3
    StrToWzID("CT51"),//FILTER_WASTE_SLOT3
    //����4: ��ü����, ���, ����Ʈ, AC, ����/��þƮ, �Ϲ�, �Ҹ�
    StrToWzID("CT01"),//FILTER_ALL_SLOT4
    StrToWzID("CT12"),//FILTER_EQUIPMENT_SLOT4
    StrToWzID("CT22"),//FILTER_QUEST_SLOT4
    StrToWzID("C004"),//FILTER_AC_SLOT4
    StrToWzID("CT32"),//FILTER_RESOURCE_SLOT4
    StrToWzID("CT42"),//FILTER_NORMAL_SLOT4
    StrToWzID("CT52"),//FILTER_WASTE_SLOT4
    //����5: ��ü����, ���, ����Ʈ, AC, ����/��þƮ, �Ϲ�, �Ҹ�
    StrToWzID("CT06"),//FILTER_ALL_SLOT5
    StrToWzID("CT13"),//FILTER_EQUIPMENT_SLOT5
    StrToWzID("CT23"),//FILTER_QUEST_SLOT5
    StrToWzID("C005"),//FILTER_AC_SLOT5
    StrToWzID("CT33"),//FILTER_RESOURCE_SLOT5
    StrToWzID("CT43"),//FILTER_NORMAL_SLOT5
    StrToWzID("CT53"),//FILTER_WASTE_SLOT5
    //����6: ��ü����, ���, ����Ʈ, AC, ����/��þƮ, �Ϲ�, �Ҹ�
    StrToWzID("CT05"),//FILTER_ALL_SLOT6
    StrToWzID("CT14"),//FILTER_EQUIPMENT_SLOT6
    StrToWzID("CT24"),//FILTER_QUEST_SLOT6
    StrToWzID("C006"),//FILTER_AC_SLOT6
    StrToWzID("CT34"),//FILTER_RESOURCE_SLOT6
    StrToWzID("CT44"),//FILTER_NORMAL_SLOT6
    StrToWzID("CT54"),//FILTER_WASTE_SLOT6
    //����7: ��ü����, ���, ����Ʈ, AC, ����/��þƮ, �Ϲ�, �Ҹ�
    StrToWzID("CT04"),//FILTER_ALL_SLOT7
    StrToWzID("CT15"),//FILTER_EQUIPMENT_SLOT7
    StrToWzID("CT25"),//FILTER_QUEST_SLOT7
    StrToWzID("C007"),//FILTER_AC_SLOT7
    StrToWzID("CT35"),//FILTER_RESOURCE_SLOT7
    StrToWzID("CT45"),//FILTER_NORMAL_SLOT7
    StrToWzID("CT55"),//FILTER_WASTE_SLOT7
    //����8: ��ü����, ���, ����Ʈ, AC, ����/��þƮ, �Ϲ�, �Ҹ�
    StrToWzID("CT03"),//FILTER_ALL_SLOT8
    StrToWzID("CT16"),//FILTER_EQUIPMENT_SLOT8
    StrToWzID("CT26"),//FILTER_QUEST_SLOT8
    StrToWzID("C008"),//FILTER_AC_SLOT8
    StrToWzID("CT36"),//FILTER_RESOURCE_SLOT8
    StrToWzID("CT46"),//FILTER_NORMAL_SLOT8
    StrToWzID("CT56"),//FILTER_WASTE_SLOT8
    //����9: ��ü����, ���, ����Ʈ, AC, ����/��þƮ, �Ϲ�, �Ҹ�
    StrToWzID("CT07"),//FILTER_ALL_SLOT9
    StrToWzID("CT17"),//FILTER_EQUIPMENT_SLOT9
    StrToWzID("CT27"),//FILTER_QUEST_SLOT9
    StrToWzID("C009"),//FILTER_AC_SLOT9
    StrToWzID("CT37"),//FILTER_RESOURCE_SLOT9
    StrToWzID("CT47"),//FILTER_NORMAL_SLOT9
    StrToWzID("CT57"),//FILTER_WASTE_SLOT9
    //����0: ��ü����, ���, ����Ʈ, AC, ����/��þƮ, �Ϲ�, �Ҹ�
    StrToWzID("CT08"),//FILTER_ALL_SLOT10
    StrToWzID("CT18"),//FILTER_EQUIPMENT_SLOT10
    StrToWzID("CT28"),//FILTER_QUEST_SLOT10
    StrToWzID("C010"),//FILTER_AC_SLOT10
    StrToWzID("CT38"),//FILTER_RESOURCE_SLOT10
    StrToWzID("CT48"),//FILTER_NORMAL_SLOT10
    StrToWzID("CT58"),//FILTER_WASTE_SLOT10
	StrToWzID("B000"),//FILTER_OK
    StrToWzID("B999"),//FILTER_EXIT
};

InventoryFilterDialog::InventoryFilterDialog(InterfaceManager* manager_ptr)
{
    charge_count_ = 0;
    for (int i = 0; i < SLOT_COUNT_MAX; ++i)
    {
        inventory_config_.configures[i] = kInsertRule_All;
    }
}

InventoryFilterDialog::~InventoryFilterDialog(void)
{
}

void InventoryFilterDialog::Init(CDrawBase* draw_base_ptr)
{
    charge_count_ = 0;
    InitControls();  
}

void InventoryFilterDialog::InitControls()
{
    DisableExtendFilterSlot();
}

void InventoryFilterDialog::DisableExtendFilterSlot()
{

    int i = 0, j = 0, position = 0;
    for (i = 0; i < SLOT_COUNT_MAX; ++i)
	{
        for (j = 0; j < CHECK_FILTER_MAX; ++j)
        {
            position = (i*CHECK_FILTER_MAX) + j;
		    slot_check_buttons_[i][j] = static_cast<CCtrlButtonCheckWZ*>(get_control(position));
		    assert(slot_check_buttons_[i][j]);
        }
        if (slot_check_buttons_[i][CHECK_FILTER_ALL] != NULL)
        {
            if (i < SLOT_COUNT_BASE)
            {
                slot_check_buttons_[i][CHECK_FILTER_ALL]->SetCheckState(true);
				for (int count = CHECK_FILTER_ALL; count < CHECK_FILTER_MAX; ++count)
				{
					slot_check_buttons_[i][count]->ShowWindowWZ(WZ_SHOW);
				}
            }
#ifdef _NA_000000_20130114_RENEWER_UI
#else
            else
            {
                //slot_check_buttons_[i][CHECK_FILTER_ALL]->SetCheckState(false);
                for (int count = CHECK_FILTER_ALL; count < CHECK_FILTER_MAX; ++count)
                {
                    slot_check_buttons_[i][count]->ShowWindowWZ(WZ_HIDE);
                }
            }
#endif
        }
	}
    charge_count_ = 0;

}

void InventoryFilterDialog::SetAllCheckAboutOneSlot(int slot_number, bool value)
{
    if (slot_number < 0 || slot_number >= SLOT_COUNT_MAX)
    {
        return;
    }
    for (int i = CHECK_FILTER_EQUIPMENT; i < CHECK_FILTER_MAX; ++i)
    {
        if (slot_check_buttons_[slot_number][i])
        {
            slot_check_buttons_[slot_number][i]->SetCheckState(value);
        }
    }
}

bool InventoryFilterDialog::GetAllCheckAboutOneSlot(int slot_number, bool all_filter_check)
{
    bool result = false;
    if (slot_number < 0 || slot_number >= SLOT_COUNT_MAX)
    {
        return false;
    }
    for (int i = CHECK_FILTER_EQUIPMENT; i < CHECK_FILTER_MAX; ++i)
    {
        if (slot_check_buttons_[slot_number][i]->GetCheckState() != all_filter_check)
        {
            return false;//�ϳ��� Ʋ����
        }
    }
    return true;
}

void InventoryFilterDialog::Release()
{
}

void InventoryFilterDialog::OnUpdateSolarDialog()
{
    int charge_count = g_pHero->GetCharInfo()->m_ExtraInventoryTabCount;
    if (charge_count_ != charge_count)
    {
        int start_slot_number = SLOT_COUNT_BASE + charge_count_;
        int end_slot_number = SLOT_COUNT_BASE + charge_count;
        for (int slot_number = start_slot_number; slot_number < end_slot_number; ++slot_number)
        {
            if (slot_check_buttons_[slot_number][CHECK_FILTER_ALL] != NULL)
            {
				slot_check_buttons_[slot_number][CHECK_FILTER_ALL]->ShowWindowWZ(WZ_SHOW);
				if (inventory_config_.configures[slot_number] == kInsertRule_All)
				{
					slot_check_buttons_[slot_number][CHECK_FILTER_ALL]->SetCheckState(true);
				}                
                for (int count = CHECK_FILTER_EQUIPMENT; count < CHECK_FILTER_MAX; ++count)
                {
                    slot_check_buttons_[slot_number][count]->ShowWindowWZ(WZ_SHOW);
                }
            }
        }
        charge_count_ = charge_count;
    }
}

void InventoryFilterDialog::MessageProc(SI_MESSAGE* message_ptr)
{
    switch (message_ptr->eResultMsg) 
	{
	case RT_MSG_RBUTTONCLICK:
	{
		return;
	}
	case RT_MSG_LBUTTONCLICK:
	{
        int slot_number = -1;
        POSTYPE position = get_control_id_to_position(message_ptr->dwCtrlID);
        slot_number = (int)(position/CHECK_FILTER_MAX);

        if (INVENTORY_FILTERS_MAX <= position)
        {
            return;
        }

        int charge_count = g_pHero->GetCharInfo()->m_ExtraInventoryTabCount;

        if (SLOT_COUNT_MAX <= slot_number)
        {
            if (FILTER_OK > position || FILTER_EXIT < position)
            {
                return;
            }
        }
        else if ((SLOT_COUNT_BASE + charge_count) <= slot_number)    // ���� �ʰ�
        {
            slot_check_buttons_[slot_number][CHECK_FILTER_ALL]->SetCheckState(false);
            SetAllCheckAboutOneSlot(slot_number, false);
            //7018 �ش� �κ��丮�� ����� �� �����ϴ�. ������ ������ ����ϼž� �մϴ�.
	        TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
	        g_InterfaceManager.GetInterfaceString(7018, szMessage, INTERFACE_STRING_LENGTH);
	        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
            return;
        }

        switch (position)
		{
		case FILTER_OK:
		{
			// ���� �� ����� ���� �ִٸ�? ������
			if (CheckItemInventoryConfigChange() == true)
			{
				SendItemInventoryConfigChangeSyn();
			}
		}
		break;
        case FILTER_EXIT:
        {
            ShowDialog(FALSE);
        }
        break;
        case FILTER_ALL_SLOT1:
        case FILTER_ALL_SLOT2:
        case FILTER_ALL_SLOT3:
        case FILTER_ALL_SLOT4:
        case FILTER_ALL_SLOT5:
        case FILTER_ALL_SLOT6:
        case FILTER_ALL_SLOT7:
        case FILTER_ALL_SLOT8:
        case FILTER_ALL_SLOT9:
        case FILTER_ALL_SLOT10:      
        {   
            if (slot_number >= 0 && slot_number < SLOT_COUNT_MAX)
            {
                if (slot_check_buttons_[slot_number][CHECK_FILTER_ALL]->GetCheckState() == true)
                {
                    SetAllCheckAboutOneSlot(slot_number, false);
                }
                else
                {   //��ü������ ���� ��ü������ �� �� ����.
                    slot_check_buttons_[slot_number][CHECK_FILTER_ALL]->SetCheckState(true);
                }
            }
        }
        break;
        default:
        {
            slot_number = (int)(position/CHECK_FILTER_MAX);
            if (slot_number >= 0 && slot_number < (SLOT_COUNT_BASE + charge_count))
            {
                bool all_filter_check = 
                    slot_check_buttons_[slot_number][CHECK_FILTER_ALL]->GetCheckState();
                bool all_check = false;
                if (all_filter_check == true)
                {   //��� 0�̾����
                    all_check = GetAllCheckAboutOneSlot(slot_number, false);//��ΰ� 0�̸� true
                    if (all_check == false)
                    {   //��ü üũ�� ����
                        slot_check_buttons_[slot_number][CHECK_FILTER_ALL]->SetCheckState(false);
                    }
                }
                else
                {   //��� 0�̾ƴϾ� ��, ��� 1�� �ƴϾ� ��
                    all_check = GetAllCheckAboutOneSlot(slot_number, true);//��ΰ� 1�̸� true
                    all_check |= GetAllCheckAboutOneSlot(slot_number, false);//��ΰ� 0�̸� true
                    if (all_check == true)
                    {   //��ü üũ�� �Ҵ�
                        slot_check_buttons_[slot_number][CHECK_FILTER_ALL]->SetCheckState(true);
                        SetAllCheckAboutOneSlot(slot_number, false);
                    }
                }
            }
        }
        break;
        }
    }
    default:
	{
		SolarDialog::MessageProc(message_ptr);
	}
	}//switch
}

void InventoryFilterDialog::OnShowWindow(BOOL val)
{
    SolarDialog::OnShowWindow(val);
    if (val)
    {
        keyMsg msg;
		ZeroMemory(&msg,sizeof(msg));
		msg.dwType=eDoType_dialog_inventory_filter;
		msg.wParam=IM_INVENTORY_MANAGER::INVEN_FILTER_DLG;
		msg.DoSomething=GlobalFunc::DoShowWindowEx;
		g_KeyQueueManager.PushBack(msg);
    }
    else
    {
        g_KeyQueueManager.DeleteMsg(eDoType_dialog_inventory_filter);
		RollbackItemInventoryConfig();
    }
}

bool InventoryFilterDialog::CheckItemInventoryConfigChange()
{
    if (g_pHero == NULL)
    {
        return false;
    }
    nsSlot::InventoryConfig current_inventory_config;
    bool changed = false;
    int charge_count = g_pHero->GetCharInfo()->m_ExtraInventoryTabCount + SLOT_COUNT_BASE;
    int slot_number = 0;
    for (slot_number = 0; slot_number < charge_count; ++slot_number)    // ����
    {
        current_inventory_config.configures[slot_number] = inventory_config_.configures[slot_number];

        if (slot_check_buttons_[slot_number][CHECK_FILTER_ALL]->GetCheckState() == true)
        {
            inventory_config_.configures[slot_number] = kInsertRule_All;
        }
        else
        {
            inventory_config_.configures[slot_number] = 0;
            for (int item_rule = 0; item_rule < (CHECK_FILTER_MAX-1); ++item_rule)
            {
                if (slot_check_buttons_[slot_number][item_rule+1]->GetCheckState() == true)
                {
                    inventory_config_.configures[slot_number] |= (1<< (item_rule));
                }
            }
        }
    }

    for (slot_number = 0; slot_number < charge_count; ++slot_number)    // ��
    {
        if (current_inventory_config.configures[slot_number] != inventory_config_.configures[slot_number])
        {   // �ٸ��ٸ�
            return true;
        }
    }
    return false;
}

void InventoryFilterDialog::SendItemInventoryConfigChangeSyn()
{
    MSG_CG_ITEM_INVENTORY_CONFIG_CHANGE_SYN send_packet;
    for (int slot_number = 0; slot_number < SLOT_COUNT_MAX; ++slot_number)    // ����
    {
        send_packet.changed.configures[slot_number] = inventory_config_.configures[slot_number];
    }
    GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send_packet, sizeof(MSG_CG_ITEM_INVENTORY_CONFIG_CHANGE_SYN));
}

void InventoryFilterDialog::SetItemInventoryConfig(nsSlot::InventoryConfig inventory_config_packet)
{
    DisableExtendFilterSlot();
    for (int slot_number = 0; slot_number < SLOT_COUNT_MAX; ++slot_number)    // ����
    {
        inventory_config_.configures[slot_number] = inventory_config_packet.configures[slot_number];
    }
    // ����
    int charge_count = g_pHero->GetCharInfo()->m_ExtraInventoryTabCount + SLOT_COUNT_BASE;
    for (int slot_number = 0; slot_number < SLOT_COUNT_MAX; ++slot_number)
    {
        if (slot_number < charge_count)
        {
            if (inventory_config_.configures[slot_number] == kInsertRule_All)
            {
                slot_check_buttons_[slot_number][CHECK_FILTER_ALL]->SetCheckState(true);
                SetAllCheckAboutOneSlot(slot_number, false);
            }
            else
            {
                slot_check_buttons_[slot_number][CHECK_FILTER_ALL]->SetCheckState(false);
                for (int j = 0; j < CHECK_FILTER_MAX-1; ++j)
                {
                    slot_check_buttons_[slot_number][j+1]->SetCheckState(
                        ((inventory_config_.configures[slot_number] >> j)&1));
                }
            }
        }
        else
        {
            slot_check_buttons_[slot_number][CHECK_FILTER_ALL]->SetCheckState(false);
            SetAllCheckAboutOneSlot(slot_number, false);
        }
    }
}

void InventoryFilterDialog::RollbackItemInventoryConfig()
{
	if (!g_pHero)
	{
		return;
	}
	// ����
	int charge_count = g_pHero->GetCharInfo()->m_ExtraInventoryTabCount + SLOT_COUNT_BASE;
	for (int slot_number = 0; slot_number < SLOT_COUNT_MAX; ++slot_number)
	{
		if (slot_number < charge_count)
		{
			if (inventory_config_.configures[slot_number] == kInsertRule_All)
			{
				slot_check_buttons_[slot_number][CHECK_FILTER_ALL]->SetCheckState(true);
				SetAllCheckAboutOneSlot(slot_number, false);
			}
			else
			{
				slot_check_buttons_[slot_number][CHECK_FILTER_ALL]->SetCheckState(false);
				for (int j = 0; j < CHECK_FILTER_MAX-1; ++j)
				{
					slot_check_buttons_[slot_number][j+1]->SetCheckState(
						((inventory_config_.configures[slot_number] >> j)&1));
				}
			}
		}
		else
		{
			slot_check_buttons_[slot_number][CHECK_FILTER_ALL]->SetCheckState(false);
			SetAllCheckAboutOneSlot(slot_number, false);
		}
	}
}

void InventoryFilterDialog::Process(DWORD tick_count)
{
}

uiInventoryMan*	InventoryFilterDialog::GetManager()
{
    return NULL;
}
