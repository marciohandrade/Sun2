#include "SunClientPrecompiledHeader.h"
#ifdef _NA004034_20120102_POINT_WALLET_SYSTEM
#include "uiCharacterStatusCurrencyInfo.h"
#include "uiCharacterStatusMan/uiCharacterStatusMan.h"
#include "uiCurrencyItemInputNumber/uiCurrencyItemInputNumber.h"
#include "TextureListInfoParser.h"
#include "TextureListInfoHandleManager.h"
#include "GraphicResourceList.h"
#include "PointSystem/PacketStruct_CG_PointSystem.h"
#include "uiToolTipMan/uiToolTipMan.h"
#include "GlobalFunc.h"
#include "ItemManager.h"
#include "SCSlotContainer.h"
#include "SCItemSlot.h"
#include "ResultCode.h"
#include "GuildSystem/GuildSystemData.h"
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
#include "GuildSystem/uiGuildInstitution.h"
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION

const DWORD uiCharacterStatusCurrencyInfo::kDialog_ID = g_IDGenerator.GenerateID();
const int kListSellHeight = 27;
const int kIconImageSize = 22;

uiCharacterStatusCurrencyInfo::uiCharacterStatusCurrencyInfo(InterfaceManager* interface_manager)
:uiBase(interface_manager)
{

}

uiCharacterStatusCurrencyInfo::~uiCharacterStatusCurrencyInfo()
{

}

void uiCharacterStatusCurrencyInfo::Init(CDrawBase* draw_base)
{
    uiBase::Init(draw_base);

    RegisterControlPosition(StrToWzID("C000"), kControl_CheckBT_C000);
#ifdef _NA_008472_20150903_IMPROVED_BALANCE
	RegisterControlPosition(StrToWzID("C004"), kControl_CheckBT_C004);
#endif // _NA_008472_20150903_IMPROVED_BALANCE
    RegisterControlPosition(StrToWzID("C001"), kControl_CheckBT_C001);
    RegisterControlPosition(StrToWzID("C002"), kControl_CheckBT_C002);
    RegisterControlPosition(StrToWzID("C003"), kControl_CheckBT_C003);
    RegisterControlPosition(StrToWzID("L000"), kControl_List_L000);
    RegisterControlPosition(StrToWzID("V000"), kControl_VScroll_V000);

    CCtrlStaticWZ* text_control = static_cast<CCtrlStaticWZ*>(GetControlWZ(StrToWzID("T004")));
    if (text_control != NULL)
    {
        // 47656 종류
        text_control->SetTextID(47656);
    }
    text_control = static_cast<CCtrlStaticWZ*>(GetControlWZ(StrToWzID("S000")));
    if (text_control != NULL)
    {
        // 47657 소지 개수
        text_control->SetTextID(47657);
    }

    currency_info_menu().Release();

    LoadCurrencyIconTexture();
}

void uiCharacterStatusCurrencyInfo::Release()
{
    currency_info_menu().Release();
}

void uiCharacterStatusCurrencyInfo::OnShowWindow(BOOL is_show)
{
    if (is_show == TRUE)
    {
        RefreshTab();
    }
    else
    {

    }
}


void uiCharacterStatusCurrencyInfo::OnUpdateSolarDialog()
{
	update();
}

void uiCharacterStatusCurrencyInfo::update()
{
    currency_info_menu().UpdateMouseWheel();
}

void uiCharacterStatusCurrencyInfo::MessageProc(SI_MESSAGE* message)
{
    if (IsLock())
    {
        return;
    }

    if (GetMan() == NULL)
    {
        return;
    }

    TreeMenuGroup::TreeObject* tree_object = NULL;
    if (currency_info_menu().MessageProc(message, OUT tree_object) == true)
    {
    }

    switch (message->eResultMsg)
    {
    case RT_MSG_LBUTTONCLICK:
        OnLButtonClick(message);
        break;

	case RT_MSG_SB_THUMBTRACK:				//. 썸을 드래그 하고 있다.
	case RT_MSG_SB_ENDSCROLL:				//. 스크롤이 종료 되었다.
	case RT_MSG_SB_THUMBPOSITION:
		{
			switch( GetControlPosition_byID( message->dwCtrlID ))
			{
			case kControl_VScroll_V000:
				{	
					int a = 0;
					int b = 0;
				}
				break;
			}
		}
    default:
        SolarDialog::MessageProc(message);
        break;
    }
}

bool uiCharacterStatusCurrencyInfo::OnLButtonClick(SI_MESSAGE* message)
{
    bool result = true;
    DWORD control_position = GetControlPosition_byID(message->dwCtrlID);
    switch (control_position)
    {
    case kControl_CheckBT_C000:
#ifdef _NA_008472_20150903_IMPROVED_BALANCE
	case kControl_CheckBT_C004:
#endif // _NA_008472_20150903_IMPROVED_BALANCE
    case kControl_CheckBT_C001:
    case kControl_CheckBT_C002:
    case kControl_CheckBT_C003:
        {
            uiCharacterStatusMan* ui_status_manager = GET_DIALOG_MANAGER(uiCharacterStatusMan, UIMAN_CHARACTERSTATUS);
            if (ui_status_manager != NULL)
            {
                TabCheckButtonControllManager& tab_manager = ui_status_manager->status_dialog_tab_manager();
                tab_manager.OnClickTab(control_position - kControl_CheckBT_C000);
            }
            RefreshTab();
        }
        break;
    default:
        {
        }
        break;
    }
    return result;
}

void uiCharacterStatusCurrencyInfo::NetworkProc(MSG_BASE* recv_packet)
{
    if (recv_packet == NULL)
    {
        return;
    }

    if ((recv_packet->m_byCategory != CG_CHARINFO) || 
        (recv_packet->m_byProtocol != CG_POINTWALLET_PACKET))
    {
        return;
    }

    MSG_CG_POINTWALLET_PACKET* point_packet = static_cast<MSG_CG_POINTWALLET_PACKET*>(recv_packet);
    switch (point_packet->packet_type)
    {
    case CG_POINTWALLET_SELECT_CMD:
        {
            MSG_CG_POINTWALLET_SELECT_CMD* character_point_cmd = 
                static_cast<MSG_CG_POINTWALLET_SELECT_CMD*>(point_packet);
            DWORD currency_code = 0;
            CurrencyPointInfoContainer::iterator found_itr;
            CurrencyPointInfoContainer::iterator end_itr = currency_info_container().end();
            for (int index = 0; index < character_point_cmd->count_; ++index)
            {
                currency_code = character_point_cmd->pointwallet_data[index].code_;
                
                found_itr = currency_info_container().find(currency_code);
                if (found_itr == end_itr)
                {
                    continue;
                }

                CurrencyPointInfo& currency_point_info = found_itr->second;
                currency_point_info.currency_point = character_point_cmd->pointwallet_data[index].point_;
            }

#ifdef _DEV_VER
            GlobalFunc::DebugOutputMsg(eOUTPUT_MSG_CHAT, 
                "MSG_CG_POINTWALLET_SELECT_CMD : count(%d)", 
                character_point_cmd->count_);
#endif
        }
        break;
    case CG_POINTWALLET_CHANGE_CMD:
        {
            MSG_CG_POINTWALLET_CHANGE_CMD* change_point_cmd = 
                static_cast<MSG_CG_POINTWALLET_CHANGE_CMD*>(point_packet);
            DWORD currency_code = change_point_cmd->code_;
            CurrencyPointInfoContainer::iterator found_itr = currency_info_container().find(currency_code);
            CurrencyPointInfoContainer::iterator end_itr = currency_info_container().end();
            if (found_itr != end_itr)
            {
                CurrencyPointInfo& currency_point_info = found_itr->second;
                
                TCHAR message[INTERFACE_STRING_LENGTH] = {'\0', };
                TCHAR point_string[INTERFACE_STRING_LENGTH] = {'\0', };
                int point_value = change_point_cmd->point_ - currency_point_info.currency_point;
                if (point_value > 0)
                {
                    _itot(point_value, point_string, 10);
                    g_InterfaceManager.GetInterfaceStringFormat(
                        message, 
                        INTERFACE_STRING_LENGTH - 1, 
                        48655, 
                        g_InterfaceManager.GetInterfaceString(currency_point_info.currency_info->currency_name_code), 
                        point_string);
                    GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, message);
                }
                else if (point_value < 0)
                {
                    point_value *= -1;
                    _itot(point_value, point_string, 10);
                    g_InterfaceManager.GetInterfaceStringFormat(
                        message, 
                        INTERFACE_STRING_LENGTH - 1, 
                        48656, 
                        g_InterfaceManager.GetInterfaceString(currency_point_info.currency_info->currency_name_code), 
                        point_string);
                    GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, message);
                }

                currency_point_info.currency_point = change_point_cmd->point_;
            }

#ifdef _DEV_VER
            GlobalFunc::DebugOutputMsg(eOUTPUT_MSG_CHAT, 
                "MSG_CG_POINTWALLET_CHANGE_CMD : code(%d), count(%d)", 
                change_point_cmd->code_, 
                change_point_cmd->point_);
#endif
        }
        break;
    case CG_POINTWALLET_CHANGE_WITH_COUNT_ACK:
        {
            MSG_CG_POINTWALLET_CHANGE_WITH_COUNT_ACK* change_count_ack = 
                static_cast<MSG_CG_POINTWALLET_CHANGE_WITH_COUNT_ACK*>(point_packet);

            if (change_count_ack->error_code_ == RC_ITEM_SUCCESS)
            {
                // 성공
                uiCurrencyItemInputNumber* currency_input = 
                    GET_CAST_DIALOG(uiCurrencyItemInputNumber, uiCurrencyItemInputNumber::kDialog_ID);
                if (currency_input != NULL)
                {
                    SCSlotContainer* inventory_container = ItemManager::Instance()->GetContainer(SI_INVENTORY);
                    if (inventory_container == NULL)
                    {
                        return;
                    }

                    if (change_count_ack->item_position_ >= inventory_container->GetMaxSlotNum())
                    {
                        return;
                    }

                    if (inventory_container->IsEmpty(change_count_ack->item_position_)) 
                    {
                        return;
                    }

                    SCSlot& item_slot = inventory_container->GetSlot(change_count_ack->item_position_);
                    if (change_count_ack->item_count_ > 0)
                    {
                        item_slot.SetNum(static_cast<DURATYPE>(change_count_ack->item_count_));
                        inventory_container->UpdateSlot(change_count_ack->item_position_, item_slot);
                    }
                    else
                    {
                        SCItemSlot delete_slot;
                        inventory_container->DeleteSlot(change_count_ack->item_position_, &delete_slot);
                    }
                }                
            }
            else
            {
                // 실패
                ItemManager::Instance()->NoticeReasonItemErrorCode(change_count_ack->error_code_);
            }
        }
        break;
    case CG_POINTWALLET_GUILDCOIN_DONATION_ACK:
        {
    #ifdef _NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL
            MSG_CG_POINTWALLET_GUILDCOIN_DONATION_ACK* coin_donation_ack = 
                static_cast<MSG_CG_POINTWALLET_GUILDCOIN_DONATION_ACK*>(point_packet);

            DWORD message_code = 0;
            switch (coin_donation_ack->result_code_)
            {
            case RC_ITEM_GUILDCOIN_DONATION_SUCCESS:
                {
                    // 7306 길드코인 기부가 성공적으로 완료 되었습니다.
                    TCHAR donation_message[INTERFACE_STRING_LENGTH] = {'\0', };
                    TCHAR donation_number[INTERFACE_STRING_LENGTH] = {'\0', };
                    _itot(coin_donation_ack->today_donation_count_, donation_number, 10);
                    g_InterfaceManager.GetInterfaceStringFormat(
                        donation_message, 
                        INTERFACE_STRING_LENGTH - 1,
                        7306,
                        donation_number);
                    GlobalFunc::VerifyMessage(donation_message, NULL);

                    return;
                }
                break;
            case RC_ITEM_GUILDCOIN_NOT_GUILD_MEMBER:
                {
                    // 7205 길드 멤버가 아닙니다.
                    message_code = 7205;
                }
                break;
            case RC_ITEM_GUILDCOIN_NOT_ENOUGH_COIN:
                {
                    // 7308 길드 코인 부족 안내문
                    message_code = 7308;
                }
                break;
            case RC_ITEM_GUILDCOIN_DONATION_COUNT_OVER:
                {
                    // 7329 일일 코인 기부 제한 안내문
                    message_code = 7329;
                }
                break;
            }

            if (message_code != 0)
            {
                GlobalFunc::VerifyMessage(message_code);
            }
            else
            {
        #ifdef _DEV_VER
                GlobalFunc::DebugOutputMsg(eOUTPUT_MSG_CHAT, 
                    "MSG_CG_POINTWALLET_GUILDCOIN_DONATION_ACK : result_code(%d)", 
                    coin_donation_ack->result_code_);
        #endif
            }
    #endif //_NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL
        }
    case CG_POINTWALLET_GUILDPOINT_CHANGE_BRD:
        {
            MSG_CG_POINTWALLET_GUILDPOINT_CHANGE_BRD* guild_coin_brd = 
                static_cast<MSG_CG_POINTWALLET_GUILDPOINT_CHANGE_BRD*>(point_packet);

#ifdef _NA004034_20120102_GUILD_POINT_SYSTEM
            if (guild_coin_brd->code_ == CURRENCY_INFO_DONATION_GUILDCOIN)
            {
                if (guild_coin_brd->uuid_ == 0)
                {
                    // 길드 경험치(길드코인)
                    if (GuildSystem::GetGuildSystemData().GetGuildInfo().m_GuildGuid == guild_coin_brd->guid_)
                    {
                        GuildSystem::GetGuildSystemData().GetGuildInfo().m_GuildCoin = guild_coin_brd->point_;
                    }
                }
                else
                {
                    // 길드원 공헌도(기부길드코인)
                    GUILD_MEMBER_INFO* member_info = 
                        GuildSystem::GetGuildSystemData().GetGuildMemberInfo(guild_coin_brd->uuid_);
                    if (member_info != NULL)
                    {
                        member_info->guild_coin_ = guild_coin_brd->point_;
                    }
                }
            }
            else if (guild_coin_brd->code_ == CURRENCY_INFO_GUILD_POINT)
            {
                if (guild_coin_brd->uuid_ == 0)
                {
                    // 길드 포인트(길드시설사용)
                    if (GuildSystem::GetGuildSystemData().GetGuildInfo().m_GuildGuid == guild_coin_brd->guid_)
                    {
                        GuildSystem::GetGuildSystemData().GetGuildInfo().m_GuildGP = guild_coin_brd->point_;
                    }
                }
            }
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
            uiGuildInstitution* ui_guild_institution = GET_CAST_DIALOG(uiGuildInstitution, uiGuildInstitution::kDialog_ID);
            if (ui_guild_institution)
            {
                ui_guild_institution->UpdateAll();
            }
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
#endif //_NA004034_20120102_GUILD_POINT_SYSTEM
        }
        break;
    default:
        {
        }
    }
}

void uiCharacterStatusCurrencyInfo::RefreshTab()
{
    CCtrlButtonCheckWZ* check_button = NULL;
    for (int position = kControl_CheckBT_C000; position < (kControl_CheckBT_C003 + 1); ++position)
    {
        check_button = 
            static_cast<CCtrlButtonCheckWZ*>(GetControlWZ(GetControlID_byPosition(position)));
        if (check_button == NULL)
        {
            continue;
        }
        check_button->SetCheckState((position == kControl_CheckBT_C003) ? (true) : (false));
    }
}

void uiCharacterStatusCurrencyInfo::BuildCurrencyPointInfo()
{
    currency_info_container().clear();

    CurrencyPointInfo new_currency_point_info;
    CurrencyInfoParser::CurrencyInfoMap& currency_info_map = CurrencyInfoParser::Instance()->GetCurrencyInfoMap();
    CurrencyInfoParser::CurrencyInfoMap::iterator currency_info_itr = currency_info_map.begin();
    CurrencyInfoParser::CurrencyInfoMap::iterator currency_info_end_itr = currency_info_map.end();
    for ( ; currency_info_itr != currency_info_end_itr; ++currency_info_itr)
    {
        CurrencyInfoParser::CurrencyInfo* currency_info = currency_info_itr->second;
        if (currency_info == NULL)
        {
            continue;
        }

        new_currency_point_info.currency_code = currency_info->currency_code;
        new_currency_point_info.currency_point = 0;
        new_currency_point_info.currency_info = currency_info;
        
        CurrencyIconTextureContainer::iterator found_itr = 
            currency_icon_texture_container().find(currency_info->currency_code);
        if (found_itr != currency_icon_texture_container().end())
        {
            new_currency_point_info.currency_icon_texture = found_itr->second;
        }
        else
        {
            new_currency_point_info.currency_icon_texture = INVALID_HANDLE_VALUE;
        }

        currency_info_container().insert(
            CurrencyPointInfoContainer::value_type(currency_info->currency_code, new_currency_point_info));
    }
}

void uiCharacterStatusCurrencyInfo::InitCurrencyInfo()
{
    currency_info_menu().Release();

    // 화폐 포인트 메뉴 갱신
    BuildCurrencyPointInfo();
    RefreshCurrencyInfoMenu();

    // 화폐 포인트 메뉴 초기화
    currency_info_menu().Init(BASE_TreeMenuInfo::kGroupType_Currency_Info, 
        static_cast<CCtrlListWZ*>(GetControlWZ_byPosition(kControl_List_L000)), 
        static_cast<CCtrlVScrollWZ*>(GetControlWZ_byPosition(kControl_VScroll_V000)), 
        false, 
        uiCharacterStatusCurrencyInfo::Callback_CurrencyInfoMenuRender);

    currency_info_menu().set_menu_height_gab(kListSellHeight);

    // 화폐 포인트 메뉴 채우기
    currency_info_menu().Fill();
}

void uiCharacterStatusCurrencyInfo::RefreshCurrencyInfoMenu()
{
    TreeMenuInfoParser::Instance()->Remove(BASE_TreeMenuInfo::kGroupType_Currency_Info);

    std::vector<BASE_TreeMenuInfo*> template_info_container;
    TreeMenuInfoParser::Instance()->GetTreeMenu(template_info_container, 
        BASE_TreeMenuInfo::kGroupType_Currency_Info_Template);

    if (template_info_container.begin() == template_info_container.end())
    {
        return;
    }

    BASE_TreeMenuInfo* template_folder_info = template_info_container.front();
    BASE_TreeMenuInfo* template_info = template_info_container.back();
    if ((template_folder_info == NULL) || 
        (template_info == NULL) || 
        (template_folder_info == template_info))
    {
        return;
    }

    // 화폐 카테고리/포인트 넣기
    BASE_TreeMenuInfo new_category_menu_info = (*template_folder_info);
    BASE_TreeMenuInfo new_currency_menu_info = (*template_info);
    new_category_menu_info.tree_group = BASE_TreeMenuInfo::kGroupType_Currency_Info;
    new_currency_menu_info.tree_group = BASE_TreeMenuInfo::kGroupType_Currency_Info;
    BYTE current_category = 0;

    CurrencyInfoParser::CurrencyCategoryMap& currency_info_map = CurrencyInfoParser::Instance()->GetCurrencyCategoryMap();
    CurrencyInfoParser::CurrencyCategoryMap::iterator category_map_itr = currency_info_map.begin();
    CurrencyInfoParser::CurrencyCategoryMap::iterator category_map_end = currency_info_map.end();
    for ( ; category_map_itr != category_map_end; ++category_map_itr)
    {
        CurrencyPointInfoContainer::iterator found_itr = 
            currency_info_container().find(category_map_itr->second->currency_code);

        if (found_itr == currency_info_container().end())
        {
            continue;
        }

        CurrencyPointInfo& currency_info = found_itr->second;
        CurrencyInfoParser::CurrencyInfo* currency_paser_info = currency_info.currency_info;
        if (currency_paser_info == NULL)
        {
            continue;
        }

        if (current_category != currency_paser_info->currency_category)
        {
            // 카테고리 폴더 넣기
            current_category = currency_paser_info->currency_category;
            new_category_menu_info.menu_depth[BASE_TreeMenuInfo::kFirstSubMenuIndex] = current_category;
            new_category_menu_info.string_code = currency_paser_info->currency_category_name_code;
            TreeMenuInfoParser::Instance()->Insert(new_category_menu_info);
        }

        // 포인트 넣기
        new_currency_menu_info.menu_depth[BASE_TreeMenuInfo::kFirstSubMenuIndex] = current_category;
        new_currency_menu_info.menu_depth[BASE_TreeMenuInfo::kSecondSubMenuIndex] = (BYTE)currency_paser_info->currency_code;
        new_currency_menu_info.string_code = currency_paser_info->currency_name_code;
        new_currency_menu_info.user_data = (DWORD)(&currency_info);
        TreeMenuInfoParser::Instance()->Insert(new_currency_menu_info);
    }
}

void uiCharacterStatusCurrencyInfo::LoadCurrencyIconTexture()
{
    CurrencyInfoParser::CurrencyCategoryMap& currency_info_map = CurrencyInfoParser::Instance()->GetCurrencyCategoryMap();
    CurrencyInfoParser::CurrencyCategoryMap::iterator currency_info_itr = currency_info_map.begin();
    CurrencyInfoParser::CurrencyCategoryMap::iterator currency_info_end_itr = currency_info_map.end();
    for ( ; currency_info_itr != currency_info_end_itr; ++currency_info_itr)
    {
        CurrencyInfoParser::CurrencyInfo* currency_info = currency_info_itr->second;
        if (currency_info == NULL)
        {
            continue;
        }

        HANDLE icon_texture = INVALID_HANDLE_VALUE;
        RESOURCE_INFO* resource_info = 
            GraphicResourceList::Instance()->GetItemInfo(currency_info->currency_icon_code);
        if (resource_info != NULL)
        {
            icon_texture = GetDrawBase()->x_pManagerTexture->LoadTexture(resource_info->szFilename, 
                TLS_ORIGINAL_SIZE | TLS_NO_MIPMAP | TLS_NO_MULTITHREAD);
        }

        if (icon_texture != INVALID_HANDLE_VALUE)
        {
            currency_icon_texture_container().insert(
                CurrencyIconTextureContainer::value_type(currency_info->currency_code, icon_texture));
        }        
    }
}

uiCharacterStatusCurrencyInfo::CurrencyPointInfo* uiCharacterStatusCurrencyInfo::GetCurrencyPointInfo(DWORD currency_code)
{
    CurrencyPointInfoContainer::iterator found_itr = currency_info_container().find(currency_code);
    if (found_itr != currency_info_container().end())
    {
        return &(found_itr->second);
    }

    return NULL;
}

void uiCharacterStatusCurrencyInfo::Callback_CurrencyInfoMenuRender( CCtrlListWZ* list_control, CCtrlListWZ::CListSellData* sell_data, int sell_index, RECT& render_area )
{
    if (list_control == NULL || sell_data == NULL || sell_data->user_data == NULL) 
    {
        return;
    }

    TreeMenuGroup::TreeObject* tree_object = static_cast<TreeMenuGroup::TreeObject*>(sell_data->user_data);
    if (tree_object == NULL)
    {
        return;
    }

    
    int select_sell = list_control->GetCurSel();
    int over_sell = list_control->GetOverSel();

    bool is_folder = tree_object->info->is_folder();
    BASE_TreeMenuInfo::Icon_Type image_type = BASE_TreeMenuInfo::kIconType_Enable; 
    BASE_TextureListInfo* tex_info = NULL; 
    HANDLE tex_handle = INVALID_HANDLE_VALUE;
    BYTE menu_index = tree_object->info->menu_depth[3];
    DWORD currency_point = 0;

    DWORD font_ID = StrToWzID("mn12");
    COLORREF font_color = sell_data->SelFontColor;
    font_ID = tree_object->info->font_id[BASE_TreeMenuInfo::kFontType_Enable];
    font_color = tree_object->info->font_color[BASE_TreeMenuInfo::kFontType_Enable];

    g_pSunRenderer->x_pManagerTextOut->StartToUseFont(font_ID);
    RECT dest_area = render_area;

    if (is_folder == true)
    {
        // 카테고리 출력
        // 카테고리 배경 출력
        image_type = BASE_TreeMenuInfo::kIconType_Enable;
        tex_handle = TextureListInfoHandleManager::Instance()->GetTexture(tree_object->info->image_code[image_type], 
            tex_info);
        if ((tex_handle != INVALID_HANDLE_VALUE) && (tex_info != NULL))
        {
            dest_area.right -= dest_area.left;
            dest_area.bottom -= dest_area.top;

            g_pSunRenderer->RenderTexture(tex_handle, 
                (float)dest_area.left, 
                (float)dest_area.top, 
                (float)dest_area.right,
                (float)dest_area.bottom,
                (float)tex_info->tex_size.left,
                (float)tex_info->tex_size.top,
                (float)tex_info->tex_size.right,
                (float)tex_info->tex_size.bottom);
        }
        
        // 카테고리 이름 출력
        dest_area = render_area;
        dest_area.left += kListSellHeight;
        g_pSunRenderer->x_pManagerTextOut->DrawText(sell_data->stdStrText.c_str(),
            &dest_area,
            font_color,
            WzColor_RGBA(0, 0, 0, 0),
            TP_HLEFT | TP_VCENTER);

        // 폴더 이미지 출력
        image_type = tree_object->is_unfold ? BASE_TreeMenuInfo::kIconType_Unfold : BASE_TreeMenuInfo::kIconType_Collpase;
        tex_handle = TextureListInfoHandleManager::Instance()->GetTexture(tree_object->info->image_code[image_type], 
            tex_info);
        if ((tex_handle != INVALID_HANDLE_VALUE) && (tex_info != NULL))
        {
            int offset = (kListSellHeight - tex_info->tex_size.bottom) / 2;
            dest_area = render_area;
            dest_area.left += offset;
            dest_area.top += offset;
            dest_area.right = tex_info->tex_size.right;
            dest_area.bottom = tex_info->tex_size.bottom;

            g_pSunRenderer->RenderTexture(tex_handle, 
                (float)dest_area.left, 
                (float)dest_area.top, 
                (float)dest_area.right,
                (float)dest_area.bottom,
                (float)tex_info->tex_size.left,
                (float)tex_info->tex_size.top,
                (float)tex_info->tex_size.right,
                (float)tex_info->tex_size.bottom);
        }
    }
    else
    {
        // 포인트 출력
        DWORD currency_point = 0;
        CurrencyPointInfo* currency_point_info = (CurrencyPointInfo*)(tree_object->info->user_data);
        if (currency_point_info != NULL)
        {
            
            tex_handle = currency_point_info->currency_icon_texture;
            currency_point = currency_point_info->currency_point;
        }

        // 화폐 아이콘 이미지 출력
        if (tex_handle != INVALID_HANDLE_VALUE)
        {
            int offset = (kListSellHeight - kIconImageSize) / 2;
            dest_area = render_area;
            dest_area.left += kListSellHeight;
            dest_area.top += offset;
            dest_area.right = kIconImageSize;
            dest_area.bottom = kIconImageSize-1;

            g_pSunRenderer->RenderTexture(tex_handle, 
                (float)dest_area.left, 
                (float)dest_area.top, 
                (float)dest_area.right,
                (float)dest_area.bottom,
                0.0f,
                0.0f,
                32.0f,
                32.0f);
        }

        // 화폐 이름 및 포인트 출력
        if (currency_point < 1)
        {
            font_ID = tree_object->info->font_id[BASE_TreeMenuInfo::kFontType_Disable];
            font_color = tree_object->info->font_color[BASE_TreeMenuInfo::kFontType_Disable];
            g_pSunRenderer->x_pManagerTextOut->StartToUseFont(font_ID);
        }

        TCHAR message[INTERFACE_STRING_LENGTH] = {'\0', };
        dest_area = render_area;
        dest_area.left += (kListSellHeight * 2);
        g_pSunRenderer->x_pManagerTextOut->DrawText(sell_data->stdStrText.c_str(),
            &dest_area,
            font_color,
            WzColor_RGBA(0, 0, 0, 0),
            TP_HLEFT | TP_VCENTER);

        TCHAR number_format[INTERFACE_STRING_LENGTH] = {'\0', };
        NUMBERFMT nFmt = { 0, 0, 3, _T("."), _T(","), 1 };
        Snprintf(message, INTERFACE_STRING_LENGTH - 1, "%d", currency_point);
        ::GetNumberFormat(
            LOCALE_SYSTEM_DEFAULT, 
            NULL, 
            message, 
            &nFmt, 
            number_format, 
            sizeof(number_format));

        dest_area.right -= kListSellHeight;
        g_pSunRenderer->x_pManagerTextOut->DrawText(number_format,
            &dest_area,
            font_color,
            WzColor_RGBA(0, 0, 0, 0),
            TP_HRIGHT | TP_VCENTER);

        // 툴팁 출력
        if (over_sell == sell_index)
        {
            uiToolTipMan* ui_tooltip_manager = GET_DIALOG_MANAGER(uiToolTipMan, UIMAN_TOOLTIP);
            if (ui_tooltip_manager != NULL)
            {
                ui_tooltip_manager->RegisterTooltipCurrencyDescription(currency_point_info->currency_info);
				ui_tooltip_manager->SetTooltipDialogID(0);
            }
        }
    }

    g_pSunRenderer->x_pManagerTextOut->EndUseFont();
}

#endif //_NA004034_20120102_POINT_WALLET_SYSTEM

