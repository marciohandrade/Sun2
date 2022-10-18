#include "SunClientPrecompiledHeader.h"
#ifdef _NA_008283_20150511_NOTIFICATIONS_SYSTEM
#include "uiNotificationsSystemMan.h"
#include "uiNotificationsSystemDialog/uiNotificationsSystemDialog.h"
#include "uiNotificationsSystemListDialog/uiNotificationsSystemListDialog.h"
#include "MapInfoParser.h"
#include "InterfaceManager.h"
#include "NotificationInfoParser.h"

const DWORD IM_NOTIFICATIONS_SYSTEM_MANAGER::NOTIFICATIONS_SYSTEM_DIALOG = StrToWzID("2901");
const DWORD IM_NOTIFICATIONS_SYSTEM_MANAGER::NOTIFICATIONS_SYSTEM_LIST = StrToWzID("2902");

uiNotificationsSystemMan::uiNotificationsSystemMan(InterfaceManager* ui_manager) : 
uiBaseMan(ui_manager), 
ui_notifications_system_dialog_(NULL)
{
}

uiNotificationsSystemMan::~uiNotificationsSystemMan(void)
{
	if ( text_texture_vector_.empty() == false )
	{
		for ( unsigned int i = 0; i < text_texture_vector_.size(); ++i )
		{
			if ( text_texture_vector_[i] != INVALID_HANDLE_VALUE )
			{
				g_pSunRenderer->x_pManagerTexture->DestroyTexture( text_texture_vector_[i] );
				text_texture_vector_[i] = INVALID_HANDLE_VALUE;
			}
		}
		text_texture_vector_.clear();
	}
}

void uiNotificationsSystemMan::OnInitialize()
{
	ui_notifications_system_dialog_ = CreateUIDialog<uiNotificationsSystemDialog>(IM_NOTIFICATIONS_SYSTEM_MANAGER::NOTIFICATIONS_SYSTEM_DIALOG, 
		                                                                          "Data\\Interface\\29_1_impartment.iwz", 
																				  this);
	assert(ui_notifications_system_dialog_);

	ui_notifications_system_list_dialog_ = CreateUIDialog<uiNotificationsSystemListDialog>(IM_NOTIFICATIONS_SYSTEM_MANAGER::NOTIFICATIONS_SYSTEM_LIST,
		                                                                      "Data\\Interface\\29_2_impartment.iwz", 
																			   this);
	assert(ui_notifications_system_list_dialog_);
}

void uiNotificationsSystemMan::ShowNotificationsMessage( const NotificationInfo_ForClient& notification_info, const NotificationType& notification_type )
{
	std::vector<HANDLE>& text_texture_vector = get_text_texture_vector();
	if ( text_texture_vector.empty() == false )
	{
		for ( unsigned int i = 0; i < text_texture_vector.size(); ++i )
		{
			if ( text_texture_vector[i] != INVALID_HANDLE_VALUE )
			{
				g_pSunRenderer->x_pManagerTexture->DestroyTexture( text_texture_vector[i] );
				text_texture_vector[i] = INVALID_HANDLE_VALUE;
			}
		}
		text_texture_vector.clear();
	}

	WzColor out_line_color = WzColor_RGBA(0, 0, 0, 255);

	WzColor text_color = GetNotificationsTextColor(notification_info.info, notification_type);

	SIZE sz;

	RECT current_rect = {0, };

	TCHAR temp_string[INTERFACE_STRING_LENGTH] = {0, };

	g_pSunRenderer->Set2DMode(TRUE);
	ENUM_ALPHABLEND	eAlphablendMode =  g_pSunRenderer->GetAlphaBlendMode();
	g_pSunRenderer->SetAlphaBlendMode( AB_ALPHA );

	g_pSunRenderer->x_pManagerTextOut->StartToUseFont(StrToWzID("mn12"));

	g_pSunRenderer->x_pManagerTextOut->GetTextExtentPoint( notification_info.notification_string, lstrlen(notification_info.notification_string), &sz);

	sz.cx = (LONG)g_pSunRenderer->StdXToRealX((float)sz.cx);
	sz.cy = (LONG)g_pSunRenderer->StdYToRealY((float)sz.cy);

	HANDLE temp_handle = g_pSunRenderer->x_pManagerTexture->GenerateEmptyTexture( sz.cx,sz.cy, 4,  TLS_AS_EXTERNAL_RENDERTARGET  | TLS_NO_MIPMAP | TLS_NO_MULTITHREAD);

	if ( temp_handle != INVALID_HANDLE_VALUE )
	{
		g_pSunRenderer->x_pManagerTexture->SetRenderTarget(temp_handle);

		WzColor clear_color = 0x00000000;
		g_pSunRenderer->ClearRender(CR_DEFAULT, &clear_color, NULL, NULL, NULL);

		current_rect.left = 0;
		current_rect.top = 0;
		current_rect.right = current_rect.left + sz.cx;
		current_rect.bottom = current_rect.top + sz.cy;

		g_pSunRenderer->x_pManagerTextOut->DrawText(notification_info.notification_string, 
			&current_rect, 
			text_color, 
			WzColor_RGBA(0, 0, 0, 0),
			TP_HLEFT,
			ETS_SHADOW,
			out_line_color, 
			FALSE
			);
	}

	g_pSunRenderer->x_pManagerTextOut->EndUseFont();

	if ( temp_handle != INVALID_HANDLE_VALUE )
	{
		g_pSunRenderer->FlushReservedDrawCall(FALSE);

		g_pSunRenderer->x_pManagerTexture->SetRenderTarget();
	}

	text_texture_vector.push_back(temp_handle);

	g_pSunRenderer->SetAlphaBlendMode( eAlphablendMode );
	g_pSunRenderer->Set2DMode(FALSE);

	ui_notifications_system_dialog_->ShowInterface(TRUE);
	ui_notifications_system_dialog_->ResetNotificationsTextRect();
}

void uiNotificationsSystemMan::ToggleNotificationsList()
{
	if ( ui_notifications_system_list_dialog_ )
	{
		if ( ui_notifications_system_list_dialog_->IsVisible() == FALSE )
		{
			ui_notifications_system_list_dialog_->ShowInterface(TRUE);
		}
		else
		{
			ui_notifications_system_list_dialog_->ShowInterface(FALSE);
		}
	}
}

void uiNotificationsSystemMan::PushBackNotificationsMessage( std::string message, const NotificationInfo& info )
{
	const NotificationParserStruct* notification_parser_struct = 
		NotificationInfoParser::Instance()->GetNotification(info.m_item_code);

	if ( notification_parser_struct == NULL )
	{
		return;
	}

	if ( notification_parser_struct->save == 1 )
	{
		ui_notifications_system_list_dialog_->PushBackNotificationsMsg(message, info.m_notification_type, info);
	}

	NotificationInfo_ForClient info_for_client;
	info_for_client.info = info;
	strcpy(info_for_client.notification_string, message.c_str());
	notifications_list_.push_back(info_for_client);
}

void uiNotificationsSystemMan::PushBackNotificationsListMessage( std::string message, const NotificationInfo& info )
{
	if ( ui_notifications_system_list_dialog_ )
	{
		const NotificationParserStruct* notification_parser_struct = 
			NotificationInfoParser::Instance()->GetNotification(info.m_item_code);

		if ( notification_parser_struct == NULL )
		{
			return;
		}

		if ( notification_parser_struct->save == 1 )
		{
			ui_notifications_system_list_dialog_->PushBackNotificationsMsg(message, info.m_notification_type, info);
		}
	}
}

void uiNotificationsSystemMan::NetworkProc( MSG_BASE* message )
{
	switch ( message->m_byCategory )
	{
	case CW_NOTIFICATION:
		{
			switch ( message->m_byProtocol )
			{
			case CW_NOTIFICATION_PREV_ACK:
				{
					OnRecv_CW_NOTIFICATION_PREV_ACK(message);
				}
				break;

			case CW_NOTIFICATION_BRD:
				{
					OnRecv_CW_NOTIFICATION_BRD(message);
				}
				break;
			}
		}
		break;
	}
}

void uiNotificationsSystemMan::OnRecv_CW_NOTIFICATION_PREV_ACK( MSG_BASE* message )
{
	MSG_CW_NOTIFICATION_PREV_ACK* recv_packet = static_cast<MSG_CW_NOTIFICATION_PREV_ACK*>(message);

	for ( int i = 0; i < MAX_NOTIFICATION_COUNT; ++i )
	{
		TCHAR out_string[INTERFACE_STRING_LENGTH] = {0, };

		if ( recv_packet->infos[i].m_notification_type == eNotificationNone )
		{
			continue;
		}

		ParseNotificationsString(recv_packet->infos[i], out_string);

		PushBackNotificationsListMessage(out_string, recv_packet->infos[i]);
	}
}

void uiNotificationsSystemMan::OnRecv_CW_NOTIFICATION_BRD( MSG_BASE* message )
{
	MSG_CW_NOTIFICATION_BRD* recv_packet = static_cast<MSG_CW_NOTIFICATION_BRD*>(message);

	TCHAR out_string[INTERFACE_STRING_LENGTH] = {0, };

	ParseNotificationsString(recv_packet->info, out_string);

	PushBackNotificationsMessage(out_string, recv_packet->info);
}

void uiNotificationsSystemMan::ParseNotificationsString( const NotificationInfo& notification_info, OUT TCHAR* out_string )
{
	TCHAR format_string[INTERFACE_STRING_LENGTH] = {0, };
	TCHAR item_name[MAX_ITEMNAME_LENGTH] = {0, };

	switch ( notification_info.m_notification_type )
	{
	case eNotificationNone:
		{
			// no operation
		}
		break;
	case eNotificationEnchant:
		{
			// 3642 : ¢∫%s¥‘¿Ã "%s" +%s¿Œ√¶∆Æø° º∫∞¯«œºÃΩ¿¥œ¥Ÿ!
			g_InterfaceManager.GetInterfaceString(3642, format_string);

			g_InterfaceManager.GetItemName(notification_info.m_item_code, item_name, MAX_ITEMNAME_LENGTH);

			TCHAR enchant_number[INTERFACE_STRING_LENGTH] = {0, };
			_itoa(notification_info.m_optione_value1, enchant_number, 10);

			Snprintf(out_string, INTERFACE_STRING_LENGTH-1, format_string, notification_info.m_char_name, item_name, enchant_number);
		}
		break;
	case eNotificationAwakening:
		{
			// 3643 : ¢∫%s¥‘¿Ã "%s" +%s∞¢º∫ø° º∫∞¯«œºÃΩ¿¥œ¥Ÿ!
			g_InterfaceManager.GetInterfaceString(3643, format_string);

			g_InterfaceManager.GetItemName(notification_info.m_item_code, item_name, MAX_ITEMNAME_LENGTH);

			TCHAR awakening_number[INTERFACE_STRING_LENGTH] = {0, };
			_itoa(notification_info.m_optione_value1, awakening_number, 10);

			Snprintf(out_string, INTERFACE_STRING_LENGTH-1, format_string, notification_info.m_char_name, item_name, awakening_number);
		}
		break;
	case eNotificationDrop:
		{
			// 3644 : ¢∫%s¥‘¿Ã "%s"∏¶ %sø°º≠ »πµÊ«œºÃΩ¿¥œ¥Ÿ!
			g_InterfaceManager.GetInterfaceString(3644, format_string);

			TCHAR map_name[INTERFACE_STRING_LENGTH] = {0, };

			g_InterfaceManager.GetItemName(notification_info.m_item_code, item_name, MAX_ITEMNAME_LENGTH );

			const sMAPINFO* map_info = MapInfoParser::Instance()->FindMapInfo(notification_info.m_map_code);
			if (map_info)
			{
				g_InterfaceManager.GetInterfaceString(map_info->dwNCode, map_name, INTERFACE_STRING_LENGTH);
			}

			Snprintf(out_string, INTERFACE_STRING_LENGTH-1, format_string, notification_info.m_char_name, item_name, map_name);
		}
		break;
	case eNotificationCraft:
		{
			// 3645 : ¢∫%s¥‘¿Ã "%s" ¡¶¿€ø° º∫∞¯«œºÃΩ¿¥œ¥Ÿ!
			g_InterfaceManager.GetInterfaceString(3645, format_string);

			g_InterfaceManager.GetItemName(notification_info.m_item_code, item_name, MAX_ITEMNAME_LENGTH);

			Snprintf(out_string, INTERFACE_STRING_LENGTH-1, format_string, notification_info.m_char_name, item_name);
		}
		break;
	case eNotificationEvolution:
		{
			// 3646 : ¢∫%s¥‘¿Ã "%s" ¡¯»≠ø° º∫∞¯«œºÃΩ¿¥œ¥Ÿ!
			g_InterfaceManager.GetInterfaceString(3646, format_string);

			g_InterfaceManager.GetItemName(notification_info.m_item_code, item_name, MAX_ITEMNAME_LENGTH);

			Snprintf(out_string, INTERFACE_STRING_LENGTH-1, format_string, notification_info.m_char_name, item_name);
		}
		break;
	case eNotificationBox:
		{
			// 3647 : ¢∫%s¥‘¿Ã "%s"∏¶ ªÛ¿⁄ø°º≠ »πµÊ«œºÃΩ¿¥œ¥Ÿ!
			g_InterfaceManager.GetInterfaceString(3647, format_string);

			g_InterfaceManager.GetItemName(notification_info.m_item_code, item_name, MAX_ITEMNAME_LENGTH);

			Snprintf(out_string, INTERFACE_STRING_LENGTH-1, format_string, notification_info.m_char_name, item_name);
		}
		break;
	case eNotificationAcReward:
		{
			// 3648 : ¢∫%s¥‘¿Ã "%s"∏¶ AC∫∏ªÛ¿∏∑Œ »πµÊ«œºÃΩ¿¥œ¥Ÿ!
			g_InterfaceManager.GetInterfaceString(3648, format_string);

			g_InterfaceManager.GetItemName(notification_info.m_item_code, item_name, MAX_ITEMNAME_LENGTH);

			Snprintf(out_string, INTERFACE_STRING_LENGTH-1, format_string, notification_info.m_char_name, item_name);
		}
		break;
	case eNotificationMissionReward:
		{
			// 3649 : ¢∫%s¥‘¿Ã "%s"∏¶ %sø°º≠ »πµÊ«œºÃΩ¿¥œ¥Ÿ!
			g_InterfaceManager.GetInterfaceString(3649, format_string);

			g_InterfaceManager.GetItemName(notification_info.m_item_code, item_name, MAX_ITEMNAME_LENGTH);

			TCHAR map_name[INTERFACE_STRING_LENGTH] = {0, };

			g_InterfaceManager.GetItemName(notification_info.m_item_code, item_name, MAX_ITEMNAME_LENGTH );

			const sMAPINFO* map_info = MapInfoParser::Instance()->FindMapInfo(notification_info.m_map_code);
			if (map_info)
			{
				g_InterfaceManager.GetInterfaceString(map_info->dwNCode, map_name, INTERFACE_STRING_LENGTH);
			}

			Snprintf(out_string, INTERFACE_STRING_LENGTH-1, format_string, notification_info.m_char_name, item_name, map_name);
		}
		break;
	case eNotificationBingoReward:
		{
			// 3650 : ¢∫%s¥‘¿Ã "%s"∏¶ ∫˘∞Ì∫∏ªÛ¿∏∑Œ »πµÊ«œºÃΩ¿¥œ¥Ÿ!
			g_InterfaceManager.GetInterfaceString(3650, format_string);

			g_InterfaceManager.GetItemName(notification_info.m_item_code, item_name, MAX_ITEMNAME_LENGTH);

			Snprintf(out_string, INTERFACE_STRING_LENGTH-1, format_string, notification_info.m_char_name, item_name);
		}
		break;
	}
}

void uiNotificationsSystemMan::CheckNextNotificationsShow()
{
	if ( notifications_list_.empty() == false && text_texture_vector_.empty() == true )
	{
		std::list<NotificationInfo_ForClient>::iterator iter = notifications_list_.begin();

		ShowNotificationsMessage((*iter), (*iter).info.m_notification_type);

		if ( NotificationInfoParser::Instance()->GetUIType((*iter).info.m_item_code) == 2 )
		{
			ui_notifications_system_dialog_->SpecialTextUIShow();
		}
	}
}

void uiNotificationsSystemMan::ClearTextTextureVector()
{
	std::vector<HANDLE>& text_texture_vector = get_text_texture_vector();
	if ( text_texture_vector.empty() == false )
	{
		for ( unsigned int i = 0; i < text_texture_vector.size(); ++i )
		{
			if ( text_texture_vector[i] != INVALID_HANDLE_VALUE )
			{
				g_pSunRenderer->x_pManagerTexture->DestroyTexture( text_texture_vector[i] );
				text_texture_vector[i] = INVALID_HANDLE_VALUE;
			}
		}
		text_texture_vector.clear();
	}
}

void uiNotificationsSystemMan::DeleteNotificationsListBegin()
{
	std::list<NotificationInfo_ForClient>::iterator iter = notifications_list_.begin();

	if ( notifications_list_.empty() == false )
	{
		iter = notifications_list_.erase(iter);
	}
}

WzColor uiNotificationsSystemMan::GetNotificationsTextColor( const NotificationInfo& info, const NotificationType& notification_type )
{
	WzColor text_color = 0;

	const NotificationParserStruct* notification_parser_struct = NotificationInfoParser::Instance()->GetNotification(info.m_item_code);

	switch ( notification_type )
	{
	case eNotificationNone:
		{
			text_color = WzColor_RGBA(255, 255, 0, 255);
		}
		break;

	case eNotificationEnchant: // ¿Œ√¶∆Æ
		{
			text_color = WzColor_RGBA(255, 255, 0, 255);
			if ( notification_parser_struct )
			{
				text_color = WzColor_RGBA(Red_WzColor(notification_parser_struct->enchant_color),
					Green_WzColor(notification_parser_struct->enchant_color),
					Blue_WzColor(notification_parser_struct->enchant_color),
					Alpha_WzColor(notification_parser_struct->enchant_color));
			}
		}
		break;

	case eNotificationAwakening: // ∞¢º∫
		{
			text_color = WzColor_RGBA(255, 255, 0, 255);
			if ( notification_parser_struct )
			{
				text_color = WzColor_RGBA(Red_WzColor(notification_parser_struct->awakening_color),
					Green_WzColor(notification_parser_struct->awakening_color),
					Blue_WzColor(notification_parser_struct->awakening_color),
					Alpha_WzColor(notification_parser_struct->awakening_color));
			}
		}
		break;

	case eNotificationDrop: // µÂ∂¯
		{
			text_color = WzColor_RGBA(255, 0, 0, 255);
			if ( notification_parser_struct )
			{
				text_color = WzColor_RGBA(Red_WzColor(notification_parser_struct->drop_color),
					Green_WzColor(notification_parser_struct->drop_color),
					Blue_WzColor(notification_parser_struct->drop_color),
					Alpha_WzColor(notification_parser_struct->drop_color));
			}
		}
		break;

	case eNotificationCraft: // ¡¶¿€
		{
			text_color = WzColor_RGBA(255, 255, 0, 255);
			if ( notification_parser_struct )
			{
				text_color = WzColor_RGBA(Red_WzColor(notification_parser_struct->craft_color),
					Green_WzColor(notification_parser_struct->craft_color),
					Blue_WzColor(notification_parser_struct->craft_color),
					Alpha_WzColor(notification_parser_struct->craft_color));
			}
		}
		break;

	case eNotificationEvolution: // ¡¯»≠
		{
			text_color = WzColor_RGBA(255, 255, 0, 255);
			if ( notification_parser_struct )
			{
				text_color = WzColor_RGBA(Red_WzColor(notification_parser_struct->evolution_color),
					Green_WzColor(notification_parser_struct->evolution_color),
					Blue_WzColor(notification_parser_struct->evolution_color),
					Alpha_WzColor(notification_parser_struct->evolution_color));
			}
		}
		break;

	case eNotificationBox: // ªÛ¿⁄
		{
			text_color = WzColor_RGBA(255, 255, 0, 255);
			if ( notification_parser_struct )
			{
				text_color = WzColor_RGBA(Red_WzColor(notification_parser_struct->box_color),
					Green_WzColor(notification_parser_struct->box_color),
					Blue_WzColor(notification_parser_struct->box_color),
					Alpha_WzColor(notification_parser_struct->box_color));
			}
		}
		break;

	case eNotificationAcReward: // AC∫∏ªÛ
		{
			text_color = WzColor_RGBA(255, 255, 0, 255);
			if ( notification_parser_struct )
			{
				text_color = WzColor_RGBA(Red_WzColor(notification_parser_struct->ac_reward_color),
					Green_WzColor(notification_parser_struct->ac_reward_color),
					Blue_WzColor(notification_parser_struct->ac_reward_color),
					Alpha_WzColor(notification_parser_struct->ac_reward_color));
			}
		}
		break;

	case eNotificationMissionReward: // πÃº«∫∏ªÛ
		{
			text_color = WzColor_RGBA(255, 0, 0, 255);
			if ( notification_parser_struct )
			{
				text_color = WzColor_RGBA(Red_WzColor(notification_parser_struct->mission_reward_color),
					Green_WzColor(notification_parser_struct->mission_reward_color),
					Blue_WzColor(notification_parser_struct->mission_reward_color),
					Alpha_WzColor(notification_parser_struct->mission_reward_color));
			}
		}
		break;

	case eNotificationBingoReward: // ∫˘∞Ì∫∏ªÛ
		{
			text_color = WzColor_RGBA(255, 0, 0, 255);
			if ( notification_parser_struct )
			{
				text_color = WzColor_RGBA(Red_WzColor(notification_parser_struct->bingo_reward_color),
					Green_WzColor(notification_parser_struct->bingo_reward_color),
					Blue_WzColor(notification_parser_struct->bingo_reward_color),
					Alpha_WzColor(notification_parser_struct->bingo_reward_color));
			}
		}
		break;

	default:
		{
			text_color = WzColor_RGBA(255, 255, 0, 255);
		}
		break;
	}

	return text_color;
}

#endif // _NA_008283_20150511_NOTIFICATIONS_SYSTEM
