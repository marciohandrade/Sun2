#include "StdAfx.h"

#ifdef _NA_008283_20150511_NOTIFICATIONS_SYSTEM
#include "NotificationInfoParser.h"

BOOL NotificationInfoParser::LoadScript( eSCRIPT_CODE script_code, BOOL is_reload )
{
    __UNUSED((script_code, is_reload));

    int row_size = GetRowSize();
    for (int script_row = 0; script_row < row_size; ++script_row)
    {
        NotificationParserStruct notification;

        GetData(script_row, notification.item_code, "ItemCode");

        GetData(script_row, notification.awakening, "Awakening");

        GetData(script_row, notification.enchant, "Enchant");

        int temp_value = 0;
        GetData(script_row, temp_value, "Drop");
        notification.drop = (temp_value != 0) ? true : false;

		DWORD temp_color = 0;
		TCHAR temp_string[256] = {0, };
		sscanf(GetDataString("AwakeningColor", script_row), "%x", &temp_color);
		notification.awakening_color = temp_color;
		sscanf(GetDataString("EnchantColor", script_row), "%x", &temp_color);
		notification.enchant_color = temp_color;
		sscanf(GetDataString("DropColor", script_row), "%x", &temp_color);
		notification.drop_color = temp_color;
		sscanf(GetDataString("CraftColor", script_row), "%x", &temp_color);
		notification.craft_color = temp_color;
		sscanf(GetDataString("EvolutionColor", script_row), "%x", &temp_color);
		notification.evolution_color = temp_color;
		sscanf(GetDataString("BoxColor", script_row), "%x", &temp_color);
		notification.box_color = temp_color;
		sscanf(GetDataString("ACRewardColor", script_row), "%x", &temp_color);
		notification.ac_reward_color = temp_color;
		sscanf(GetDataString("MissionRewardColor", script_row), "%x", &temp_color);
		notification.mission_reward_color = temp_color;
		sscanf(GetDataString("BingoRewardColor", script_row), "%x", &temp_color);
		notification.bingo_reward_color = temp_color;

        temp_value = 0;
        GetData(script_row, temp_value, "Craft");
        notification.craft = (temp_value != 0) ? true : false;

        temp_value = 0;
        GetData(script_row, temp_value, "Evolution");
        notification.evolution = (temp_value != 0) ? true : false;

        temp_value = 0;
        GetData(script_row, temp_value, "Box");
        notification.box = (temp_value != 0) ? true : false;

        temp_value = 0;
        GetData(script_row, temp_value, "ACReward");
        notification.ac_reward = (temp_value != 0) ? true : false;

        temp_value = 0;
        GetData(script_row, temp_value, "MissionReward");
        notification.mission_reward = (temp_value != 0) ? true : false;

		temp_value = 0;
		GetData(script_row, temp_value, "BingoReward");
		notification.bingo_reward = (temp_value != 0) ? true : false;
        
        temp_value = 0;
        GetData(script_row, temp_value, "Save");
        notification.save = (temp_value != 0) ? true : false;

        GetData(script_row, notification.ui_type, "UIType");

        if (IsValidNotification(notification) == false)
        {
            SUNLOG(eCRITICAL_LOG, _T("[%s] Invalid Notification (item code:%d, row:%d)"),
                __FUNCTION__, notification.item_code, script_row);
            ASSERT(false);
            continue;
        }

        m_notification_map.insert(std::make_pair(notification.item_code, notification));
    }

    return TRUE;
}

const bool NotificationInfoParser::IsValidNotification( NotificationParserStruct& notification_info )
{
    if (m_notification_map.find(notification_info.item_code) != m_notification_map.end())
    {
        return false;
    }

    if (notification_info.awakening > MAX_AWAKENING_GRADE)
    {
        return false;
    }

    if (notification_info.enchant > MAX_ENCHANT_GRADE)
    {
        return false;
    }
    
    return true;
}

const bool NotificationInfoParser::IsNotificationItem( const SCItemSlot& item_slot, NotificationType notification_type )
{
    NOTIFICATION_MAP::iterator cur_iter = m_notification_map.find(item_slot.GetItemCode());
    if (cur_iter == m_notification_map.end())
    {
        return false;
    }

    switch(notification_type)
    {
    case eNotificationNone:
        return false;
    case eNotificationEnchant:      // 인챈트
        if (cur_iter->second.enchant != 0
            && item_slot.GetEnchant() >= cur_iter->second.enchant)
        {
            return true;
        }
        break;
    case eNotificationAwakening:    // 각성
        if (cur_iter->second.awakening != 0
            && item_slot.GetAwakening() >= cur_iter->second.awakening)
        {
            return true;
        }
        break;
    case eNotificationDrop:         // 드랍
        if (cur_iter->second.drop == true)
        {
            return true;
        }
        break;
    case eNotificationCraft:        // 제작
        if (cur_iter->second.craft == true)
        {
            return true;
        }
        break;
    case eNotificationEvolution:    // 진화
        if (cur_iter->second.evolution == true)
        {
            return true;
        }
        break;
    case eNotificationBox:          // 상자
        if (cur_iter->second.box == true)
        {
            return true;
        }
        break;
    case eNotificationAcReward:     // AC 보상
        if (cur_iter->second.ac_reward == true)
        {
            return true;
        }
        break;
    case eNotificationMissionReward:// 미션 보상
        if (cur_iter->second.mission_reward == true)
        {
            return true;
        }
        break;
	case eNotificationBingoReward:  // 빙고 보상
		if (cur_iter->second.bingo_reward == true)
		{
			return true;
		}
		break;
    }
    return false;
}

const bool NotificationInfoParser::IsNotificationItem(
    SLOTCODE item_code, NotificationType notification_type,BYTE enchant, BYTE awakening )
{

    NOTIFICATION_MAP::iterator cur_iter = m_notification_map.find(item_code);
    if (cur_iter == m_notification_map.end())
    {
        return false;
    }

    switch(notification_type)
    {
    case eNotificationNone:
        return false;
    case eNotificationEnchant:      // 인챈트
        if (cur_iter->second.enchant != 0
            && enchant >= cur_iter->second.enchant)
        {
            return true;
        }
        break;
    case eNotificationAwakening:    // 각성
        if (cur_iter->second.awakening != 0
            && awakening >= cur_iter->second.awakening)
        {
            return true;
        }
        break;
    case eNotificationDrop:         // 드랍
        if (cur_iter->second.drop == true)
        {
            return true;
        }
        break;
    case eNotificationCraft:        // 제작
        if (cur_iter->second.craft == true)
        {
            return true;
        }
        break;
    case eNotificationEvolution:    // 진화
        if (cur_iter->second.evolution == true)
        {
            return true;
        }
        break;
    case eNotificationBox:          // 상자
        if (cur_iter->second.box == true)
        {
            return true;
        }
        break;
    case eNotificationAcReward:     // AC 보상
        if (cur_iter->second.ac_reward == true)
        {
            return true;
        }
        break;
    case eNotificationMissionReward:// 미션 보상
        if (cur_iter->second.mission_reward == true)
        {
            return true;
        }
		break;
	case eNotificationBingoReward:  // 빙고 보상
		if (cur_iter->second.bingo_reward == true)
		{
			return true;
		}
        break;
    }

    return false;
}

const BYTE NotificationInfoParser::GetUIType( SLOTCODE item_code )
{
    NOTIFICATION_MAP::iterator cur_iter = m_notification_map.find(item_code);
    if (cur_iter == m_notification_map.end())
    {
        return 0;
    }

    return cur_iter->second.ui_type;
}

const NotificationParserStruct* NotificationInfoParser::GetNotification( SLOTCODE item_code )
{
    NOTIFICATION_MAP::iterator cur_iter = m_notification_map.find(item_code);
    if (cur_iter == m_notification_map.end())
    {
        return NULL;
    }

    return &cur_iter->second;
}

const bool NotificationInfoParser::IsSaveNotification( SLOTCODE item_code )
{
    NOTIFICATION_MAP::iterator cur_iter = m_notification_map.find(item_code);
    if (cur_iter == m_notification_map.end())
    {
        return false;
    }

    return cur_iter->second.save;
}


#endif // _NA_008283_20150511_NOTIFICATIONS_SYSTEM