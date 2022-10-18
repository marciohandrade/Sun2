#include "SunClientPrecompiledHeader.h"
#ifdef _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
#include "CalendarSlot.h"
#include "ItemInfoParser.h"
#include "uiAttendanceCheckMan.h"
#include "RewardInfoList.h"
#include "ItemTypeList.h"
#include "InterfaceManager.h"

CalendarSlot::CalendarSlot()
{
    item_code_  = 0;
    item_type_index_ = 0;
    item_limit_time_ = 0;
    attendance_info_ = eATTENDANCE_INFO_MAX;

    icon_image_ = NULL;
    static_control_ = NULL;
    force_control_ = NULL;
    check_control_ = NULL;
    item_control_ = NULL;
}

CalendarSlot::~CalendarSlot()
{
    if (icon_image_ != NULL)
    {
        ICONMGR()->RemoveIcon(icon_image_);
        icon_image_ = NULL;
    }
}

void CalendarSlot::SetString(TCHAR* string)
{
    if (static_control())
    {
        static_control()->SetTextWZ(string);
    }
}
void CalendarSlot::set_check_control(CControlWZ* control)
{
    check_control_ = control;

    if (check_control_)
    {
        check_control_->ShowWindowWZ(WZ_HIDE);
    }
}
void CalendarSlot::SetForceType(int force_type)
{
    if (force_control() == NULL || static_control() == NULL)
    {
        return;
    }

    CCtrlStaticWZ* static_ = static_cast<CCtrlStaticWZ*>(static_control());

    switch (force_type)
    {
    case 1:
        {
            force_control()->ShowWindowWZ(WZ_HIDE);
            static_->SetTextColor(RGBA(255,255,255,255));
        }
        break;
    case 2:
        {
            force_control()->ShowWindowWZ(WZ_HIDE);
            static_->SetTextColor(RGBA(255,0,0,255));
        }
        break;
    case 3:
        {
            force_control()->ShowWindowWZ(WZ_SHOW);
            static_->SetTextColor(RGBA(255,255,255,255));
        }
        break;
    case 4:
        {
            force_control()->ShowWindowWZ(WZ_SHOW);
            static_->SetTextColor(RGBA(255,0,0,255));
        }
        break;
    default:
        {
            force_control()->ShowWindowWZ(WZ_HIDE);
            static_->SetTextColor(RGBA(255,255,255,255));
        }
        break;
    }
}

void CalendarSlot::ShowSlotImage(BOOL is_show)
{
    if (item_control())
    {
        item_control()->ShowWindowWZ(is_show ? WZ_SHOW : WZ_HIDE);
    }
}
void CalendarSlot::SetItem(DWORD reward_code)
{
    if (icon_image_ != NULL)
    {
        ICONMGR()->RemoveIcon(icon_image_);
        icon_image_ = NULL;
    }

    item_code_ = 0;
    item_type_index_ = 0;
    item_limit_time_ = 0;

    RewardInfo* reward_drop_info = 
        RewardInfoList::Instance()->FindRewardInfo(reward_code);
    if (reward_drop_info == NULL)
    {
        return;
    }
    if ((reward_drop_info->GetRefType() != RewardInfo::REF_TYPE_EVENT_ITEMCODE) &&
        (reward_drop_info->GetRefType() != RewardInfo::REF_TYPE_DATETIME))
    {
        return;
    }
    

    item_code_       = reward_drop_info->GetTypeCode(0);
    item_type_index_ = reward_drop_info->GetTypeIndex(0);
    item_limit_time_ = reward_drop_info->GetTimeLimit(0);

    BASE_ITEMINFO* item_info = ItemInfoParser::Instance()->GetItemInfo(item_code_);
    if (item_info == NULL)
    {
        item_code_ = 0;
        item_type_index_ = 0;
        item_limit_time_ = 0;
        return;
    }

    if (item_type_index_ >= RewardInfo::MIN_ITEM_TYPECODE)
    {
        const ItemType* item_type_info = ItemTypeList::Instance()->FindItemType(item_type_index_);
        if (item_type_info != NULL)
        {
            KindOfItemType::value_type kind_of_item = item_type_info->GetItemType();
            if ((kind_of_item == KindOfItemType::Etheria) ||
                (kind_of_item == KindOfItemType::EtheriaWithDivine))
            {
                if (item_info->m_wType == eITEMTYPE_PET) 
                {
                    // 펫+에테리아 아이템일 경우
                    icon_image_ = ICONMGR()->Add(34137);
                }
                else
                {
                    icon_image_ = ICONMGR()->Add(34136);
                    
                }
                return;
            }
        }
    }

    icon_image_ = ICONMGR()->Add(item_info->m_wVICode);
}

void CalendarSlot::Render()
{
    // 아이템 아이콘
    if (icon_image_ && item_control() && item_code() != 0)
    {
        RECT control_rect = item_control()->GetSizeRect();
        control_rect.left +=2;
        control_rect.top +=2;
        control_rect.right -=2;
        control_rect.bottom -=2;
        icon_image_->Render(&control_rect);

        if (item_type_index_ >= RewardInfo::MIN_ITEM_TYPECODE)
        {
            const ItemType* item_type_info = ItemTypeList::Instance()->FindItemType(item_type_index_);
            if (item_type_info != NULL)
            {
                KindOfItemType::value_type kind_of_item = item_type_info->GetItemType();
                if (kind_of_item == KindOfItemType::Shell)
                {
                    icon_image_->RenderCoverImage(&control_rect, WzColor_RGBA(0, 0, 0, 175));
                }
            }
        }
        else
        {
            if (item_type_index_ > 1)
            {
                TCHAR string[10] = {0,};
                Sprintf(string, _T("%d"), item_type_index_);
                control_rect.top += 18;
                g_pSunRenderer->x_pManagerTextOut->StartToUseFont( StrToWzID("mn12") );
                g_pSunRenderer->x_pManagerTextOut->DrawText(string, &control_rect,
                    WzColor_RGBA(255,255,255,255), WzColor_RGBA(0,0,0,0), TP_HRIGHT);
                g_pSunRenderer->x_pManagerTextOut->EndUseFont();
            }
        }
    }

    // 체크마크
    if (check_control() && g_InterfaceManager.common_bg01_texture() != INVALID_HANDLE_VALUE)
    {
        if (attendance_info_ == eATTENDANCE_INFO_ALEADY_GOT_REWARD)
        {
            RECT rect = check_control()->GetSizeRect();
            
            g_pSunRenderer->RenderTexture( 
                g_InterfaceManager.common_bg01_texture(),
                static_cast<float>(rect.left),
                static_cast<float>(rect.top),
                static_cast<float>(rect.right - rect.left),
                static_cast<float>(rect.bottom - rect.top),
                486.0f,
                744.0f,
                48.0f,
                48.0f,
                NULL,
                0.0f
                );
        }
        if (attendance_info_ == eATTENDANCE_INFO_ATTENDED)
        {
            RECT rect = check_control()->GetSizeRect();
            
            DWORD current_tick = clock_function::GetTickCount();
            const DWORD half_freq = 1000;
            const DWORD stop_time = 200;
            BYTE alpha = 0;

            current_tick %= (half_freq*2 + stop_time);

            if (current_tick < half_freq)
            {
                alpha = static_cast<BYTE>(255 * (current_tick / static_cast<float>(half_freq)));
            }
            else if (current_tick < half_freq + stop_time)
            {
                alpha = 255;
            }
            else
            {
                alpha = static_cast<BYTE>(255 * (1 - ((current_tick - half_freq - stop_time) / static_cast<float>(half_freq))));
            }

            WzColor color[4] = {
                WzColor_RGBA(255,255,255,alpha),
                WzColor_RGBA(255,255,255,alpha),
                WzColor_RGBA(255,255,255,alpha),
                WzColor_RGBA(255,255,255,alpha)
            };

            g_pSunRenderer->RenderTexture( 
                g_InterfaceManager.common_bg01_texture(),
                static_cast<float>(rect.left),
                static_cast<float>(rect.top),
                static_cast<float>(rect.right - rect.left),
                static_cast<float>(rect.bottom - rect.top),
                536.0f,
                744.0f,
                48.0f,
                48.0f,
                color,
                0.0f
                );
        }

    }
}

BOOL CalendarSlot::IsMouseOver(int mouse_x, int mouse_y)
{
    if (item_control() && item_code() != 0)
    {
        RECT control_rect = item_control()->GetSizeRect();
        control_rect.left +=2;
        control_rect.top +=2;
        control_rect.right -=4;
        control_rect.bottom -=4;
        
        if (control_rect.left <= mouse_x && control_rect.right >= mouse_x &&
            control_rect.top <= mouse_y && control_rect.bottom >= mouse_y)
        {
            return TRUE;
        }
    }

    return FALSE;
}
//////////////////////////////////////////////////////////////////////////


#endif // _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM

