#include "SunClientPrecompiledHeader.h"
#ifdef _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
#include "uiAttendanceCheckDialog.h"
#include "uiAttendanceCheckMan.h"
#include "uiToolTipMan/uiToolTipMan.h"
#include "Mouse.h"
#include "GameFunc.h"
#include "GlobalFunc.h"
#include "SCItemSlot.h"
#include "AttendanceParser.h"
#include "FTextParser.h"
#include "FTextSentence.h"
#include "RewardInfoLIst.h"
#include "ItemTypeList.h"

uiAttendanceCheckDialog::uiAttendanceCheckDialog(InterfaceManager *ui_manager) :
uiBase(ui_manager)
{
    current_year_ = 0;
    current_month_ = 0;
    default_month_ = 0;

    scroll_control_ = NULL;
    calendar_control_ = NULL;

    RegisterControl();
}

uiAttendanceCheckDialog::~uiAttendanceCheckDialog(void)
{
}

VOID uiAttendanceCheckDialog::Init( CDrawBase* draw_base_ptr )
{
    // ��ũ��
    scroll_control_ = GetVScrollControlWZ(GetControlID_byPosition(eControl_VScroll_V002));
    if (scroll_control_)
    {
        scroll_control_->SetScrollPosWZ(0);
        scroll_control_->SetScrollRangeWZ(0,0);
    }

    calendar_control_ = GetControlWZ(GetControlID_byPosition(eControl_Picture_P009));

    CControlWZ* control = GetControlWZ_byPosition(eControl_Button_B001);
    if (control)
    {
        // 1793 ����ޱ�
        control->SetTextID(1793);
    }

    control = GetControlWZ_byPosition(eControl_Static_S045);
    if (control)
    {
        RECT control_rect = control->GetSizeRect();
        ftext_render_.Init(draw_base_ptr, control_rect);
        ftext_render_.SetMargin(0,0,0,0);
        ftext_render_.SetGap(0,0);
        ftext_render_.convertLineInfo();
    }
    
    InitCalendarSlot();
}

VOID uiAttendanceCheckDialog::Release()
{
}

VOID uiAttendanceCheckDialog::OnShowWindow( BOOL is_show )
{
    if (is_show)
    {
        keyMsg msg;
        ZeroMemory(&msg,sizeof(msg));
        msg.dwType = eDoType_Attendance_Check;
        msg.wParam = IM_ATTENDANCE_CHECK_MANAGER::ATTENDANCE_CHECK_DIALOG;
        msg.DoSomething = GlobalFunc::DoAttendanceCheckDialog;
        g_KeyQueueManager.PushBack(msg);
    }
    else
    {
        g_KeyQueueManager.DeleteMsg(eDoType_Attendance_Check);
    }
}

VOID uiAttendanceCheckDialog::MessageProc( SI_MESSAGE* message )
{
    if (IsLock())
        return;

    if (GetMan() == NULL)
        return;

    switch (message->eResultMsg)
    {
    case RT_MSG_LBUTTONCLICK:
        OnLButtonClick(message);
        break;
    case RT_MSG_SB_PAGETOP:			//. ���� ������ ������.
    case RT_MSG_SB_LINETOP:			//.���� ȭ��ǥ�� ������.
        {
            if(scroll_control_)
            {
                int pos = scroll_control_->GetScrollPosWZ();        // ���� ��ũ�� ���� �޾� �´�.
                int mmin = 0 , mmax = 0;
                scroll_control_->GetScrollRangeWZ( &mmin, &mmax );  // ��ũ�� ���� �ִ� �ּҸ� ��� �´�.

                if(pos>mmin)    // ���� �Էµ� ���μ� ���� ������ 
                {
                    --pos;      // ���� ���� ��Ű�� 
                    scroll_control_->SetScrollPosWZ(pos);
                    UpdateCalendar();
                }
            }
        }
        break;

    case RT_MSG_SB_PAGEBOTTOM:				//. �Ʒ��� ������ ������.
    case RT_MSG_SB_LINEBOTTOM:				//.�Ʒ��� ȭ��ǥ�� ������
        {
            if(scroll_control_)
            {
                int pos = scroll_control_->GetScrollPosWZ();        // ���� ��ũ�� ���� �޾� �´�.
                int mmin = 0 , mmax = 0;
                scroll_control_->GetScrollRangeWZ( &mmin, &mmax );  // ��ũ�� ���� �ִ� �ּҸ� ��� �´�.

                if(pos<mmax)    // ���� �Էµ� ���μ� ���� ������ 
                {
                    pos++;      // ���� ���� ��Ű�� 
                    scroll_control_->SetScrollPosWZ(pos);
                    UpdateCalendar();
                }
            }
        }
        break;
    case RT_MSG_SB_THUMBTRACK:				//. ���� �巡�� �ϰ� �ִ�.
    case RT_MSG_SB_THUMBPOSITION:			//. ���� �巡������ ���콺�� ���Ҵ�.
    case RT_MSG_SB_ENDSCROLL:				//. ��ũ���� ���� �Ǿ���.
        {
            if(scroll_control_)
            {
                int pos = scroll_control_->GetScrollPosWZ();        // ���� ��ũ�� ���� �޾� �´�.
                UpdateCalendar();
            }
        }
        break;
    case RT_MSG_MOUSEOVER:
        {
        }
        break;

    default:
        SolarDialog::MessageProc(message);
        break;
    }
}

VOID uiAttendanceCheckDialog::OnUpdateSolarDialog()
{
    UpdateMouseWheel();
}

void uiAttendanceCheckDialog::OnRenderSolarDialog()
{
    CControlWZ* control = GetControlWZ_byPosition(eControl_Static_S045);
    if (control)
    {
        RECT control_rect = control->GetSizeRect();
        ftext_render_.SetRect(control_rect);
        ftext_render_.Render();
    }

    int mouse_x = Mouse::Instance()->GetMouseX();
    int mouse_y = Mouse::Instance()->GetMouseY();

    for (int i = 0; i < MAX_CALENDAR_SLOT; ++i)
    {
        calendar_slot[i].Render();

        if (calendar_slot[i].IsMouseOver(mouse_x, mouse_y))
        {
            SCItemSlot item;
            item.SetCode(static_cast<SLOTCODE>(calendar_slot[i].item_code()));
            DWORD item_type_index = calendar_slot[i].item_type_index();
            if (item_type_index >= RewardInfo::MIN_ITEM_TYPECODE)
            {
                const ItemType* item_type_info = ItemTypeList::Instance()->FindItemType(item_type_index);
                if (item_type_info)
                {
                    item.SetEnchant( item_type_info->GetEnchant() );
                    //item.SetRank(0); //item_type_info->GetRank()
                    nsSlot::ItemTypeChanger::ChangeItemByType(&item, item_type_info->GetItemType());
                }		
            }
            else
            {
                item.SetNum(static_cast<DURATYPE>(item_type_index));
            }

            item.SetDateTime(calendar_slot[i].item_limit_time() * 60); // ��

            uiToolTipMan* tooltip_man = GET_DIALOG_MANAGER(uiToolTipMan, UIMAN_TOOLTIP);
            if (tooltip_man)
            {
                tooltip_man->RegisterSlotInfoTooltip(&item);
                tooltip_man->SetTooltipDialogID(GetDialogKey());
            }
        }
    }
}
void uiAttendanceCheckDialog::RegisterControl()
{
    RegisterControlPosition(StrToWzID("S003"), eControl_Static_S003);   // ����
    RegisterControlPosition(StrToWzID("V002"), eControl_VScroll_V002);  // ��ũ��
    RegisterControlPosition(StrToWzID("S002"), eControl_Static_S002);   // ����� ����
    RegisterControlPosition(StrToWzID("S045"), eControl_Static_S045);   // ����� �ؽ�Ʈ?
    RegisterControlPosition(StrToWzID("P009"), eControl_Picture_P009);  // �޷� ���

    RegisterControlPosition(StrToWzID("B000"), eControl_Button_B000);  // �����޺���/����޺���
    RegisterControlPosition(StrToWzID("B001"), eControl_Button_B001);  // ����ޱ�
    RegisterControlPosition(StrToWzID("B002"), eControl_Button_B002);  // �ݱ�
    RegisterControlPosition(StrToWzID("B086"), eControl_Button_B086);  // x��ư

    RegisterControlPosition(StrToWzID("S000"), eControl_Static_S000);  // �Ͽ���
    RegisterControlPosition(StrToWzID("S001"), eControl_Static_S001);  // ������
    RegisterControlPosition(StrToWzID("S004"), eControl_Static_S004);  // ȭ����
    RegisterControlPosition(StrToWzID("S005"), eControl_Static_S005);  // ������
    RegisterControlPosition(StrToWzID("S006"), eControl_Static_S006);  // �����
    RegisterControlPosition(StrToWzID("S007"), eControl_Static_S007);  // �ݿ���
    RegisterControlPosition(StrToWzID("S008"), eControl_Static_S008);  // �����


    TCHAR temp_string[5] = {0,};
    int start_index = 0;

    // ��Ʈ���� ��ȣ�� ������ ����.
    // �� ��ȣ�� �ϵ��ڵ����� ��ŵ��. ��Ʈ���� ���ٺ��� �����ϱ⿣ �ʹ� ����...

    //������ũ  P043 ~ P078
    start_index = 43;
    for (int i = 0; i < 5; ++i)
    {
        for (int j = 0; j < 7; ++j)
        {
            if (start_index == 45)
            {
                ++start_index;
                --j;
                continue;
            }
            Sprintf(temp_string, "P%03d", start_index++);
            RegisterControlPosition(StrToWzID(temp_string), eControl_Picture_P043 + (i*7 + j));
        }
    }
    if (start_index - 1 != 78)
    {
        assert(!"uiAttendanceCheckDialog - Control Miss");
    }

    //���� P045 ~ P112
    start_index = 45;
    for (int i = 0; i < 5; ++i)
    {
        for (int j = 0; j < 7; ++j)
        {
            if (start_index == 46)
            {
                start_index = 79;  // 46->79
                --j;
                continue;
            }
            Sprintf(temp_string, "P%03d", start_index++);
            RegisterControlPosition(StrToWzID(temp_string), eControl_Picture_P045 + (i*7 + j));
        }
    }
    if (start_index - 1 != 112)
    {
        assert(!"uiAttendanceCheckDialog - Control Miss");
    }

    //��¥ ��Ʈ��  S009 ~ S043
    start_index = 9;
    for (int i = 0; i < 5; ++i)
    {
        for (int j = 0; j < 7; ++j)
        {
            Sprintf(temp_string, "S%03d", start_index++);
            RegisterControlPosition(StrToWzID(temp_string), eControl_Static_S009 + (i*7 + j));
        }
    }
    if (start_index - 1 != 43)
    {
        assert(!"uiAttendanceCheckDialog - Control Miss");
    }

    //���帶ũ  P113 ~ P149
    start_index = 113;
    for (int i = 0; i < 5; ++i)
    {
        for (int j = 0; j < 7; ++j)
        {
            if (start_index == 141 || start_index == 142)
            {
                ++start_index;
                --j;
                continue;
            }
            Sprintf(temp_string, "P%03d", start_index++);
            RegisterControlPosition(StrToWzID(temp_string), eControl_Picture_P113 + (i*7 + j));
        }
    }
    if (start_index - 1 != 149)
    {
        assert(!"uiAttendanceCheckDialog - Control Miss");
    }
}

void uiAttendanceCheckDialog::InitCalendarSlot()
{
    for (int i = 0; i < MAX_CALENDAR_SLOT; ++i)
    {
        calendar_slot[i].set_force_control(GetControlWZ(GetControlID_byPosition(eControl_Picture_P043+i)));
        calendar_slot[i].set_item_control(GetControlWZ(GetControlID_byPosition(eControl_Picture_P045+i)));
        calendar_slot[i].set_static_control(GetControlWZ(GetControlID_byPosition(eControl_Static_S009+i)));
        calendar_slot[i].set_check_control(GetControlWZ(GetControlID_byPosition(eControl_Picture_P113+i)));
    }

    ReleaseCalendarSlot();
}

void uiAttendanceCheckDialog::ReleaseCalendarSlot()
{
    for (int i = 0; i < MAX_CALENDAR_SLOT; ++i)
    {
        // �� ĭ
        calendar_slot[i].SetForceType(1);
        calendar_slot[i].SetString("");
        calendar_slot[i].SetItem(0);
        calendar_slot[i].ShowCheckType(eATTENDANCE_INFO_NOT_ATTEND);
        calendar_slot[i].ShowSlotImage(FALSE);
    }

    UpdateCalendar_Title(0);
}

//------------------------------------------------------------------------------
void uiAttendanceCheckDialog::UpdateMouseWheel()
{
    if (calendar_control_ == NULL || scroll_control_ == NULL)
    {
        return;
    }

    RECT list_rect = calendar_control_->GetSizeRect();
    switch (GetMouseWheelStatus(&list_rect))
    {
    case MOUSE_WHEEL_UP:
        {
            int index = scroll_control_->GetScrollPosWZ();
            int scroll_min = 0;
            int scroll_max = 0;
            scroll_control_->GetScrollRangeWZ(&scroll_min, &scroll_max);
            if (index > scroll_min)
            {
                --index;
                scroll_control_->SetScrollPosWZ(index);
                UpdateCalendar();
            }
        }
        break;
    case MOUSE_WHEEL_DOWN:
        {
            int index = scroll_control_->GetScrollPosWZ();
            int scroll_min = 0;
            int scroll_max = 0;
            scroll_control_->GetScrollRangeWZ(&scroll_min, &scroll_max);
            if (index < scroll_max)
            {
                ++index;
                scroll_control_->SetScrollPosWZ(index);
                UpdateCalendar();
            }
        }
        break;
    default:
        break;
    }
}
bool uiAttendanceCheckDialog::OnLButtonClick(SI_MESSAGE* message)
{
    if (message == NULL)
    {
        return false;
    }

    bool result = true;
    POSTYPE position = static_cast<POSTYPE>(GetControlPosition_byID(message->dwCtrlID));
    switch (position)
    {
    case eControl_Button_B000:  // �����޺���/����޺���
        {
            if (default_month_ == current_month_)
            {
                if (current_month_ + 1 > 12)
                {
                    //current_year_++;
                    //current_month_ = 1;
                    UpdateCalendar(current_year_ + 1, 1);
                    break;
                }

                UpdateCalendar(current_year_, current_month_ + 1);
            }
            else
            {
                if (current_month_ - 1 < 1)
                {
                    //current_year_--;
                    //current_month_ = 12;
                    UpdateCalendar(current_year_ - 1, 12);
                    break;
                }

                UpdateCalendar(current_year_, current_month_ - 1);
                
            }
        }
        break;
    case eControl_Button_B001:  // ����ޱ�
        {
            //
            TCHAR string[INTERFACE_STRING_LENGTH] = {0,};
            uiSystemMan* system_manager = GET_DIALOG_MANAGER(uiSystemMan, UIMAN_SYSTEM);
            if(system_manager)
            {
                uiAttendanceCheckMan* manager = static_cast<uiAttendanceCheckMan*>(GetMan());
                if ((manager != NULL) &&
                    (manager->recv_attendance_info().GetYear() == current_year_) &&
                    (manager->recv_attendance_info().GetMonth() == current_month_) &&
                    manager->recv_attendance_info().GetAttendanceInfoCount(eATTENDANCE_INFO_ATTENDED) != 0)
                {
                    // 5854 ������ �⼮üũ ���� �����۱��� ��� �޾����ϴ�.\n���� �����ðڽ��ϱ�?
                    g_InterfaceManager.GetInterfaceString(5854, string, INTERFACE_STRING_LENGTH);
                    system_manager->Sys_Confirm(string, GlobalFunc::AttendanceCheckRewardConfirm);
                }
                else
                {
                    // 5855 �̹� ��� ������ �޾ҽ��ϴ�.
                    g_InterfaceManager.GetInterfaceString(5855, string, INTERFACE_STRING_LENGTH);
                    system_manager->Sys_Verify(string);
                }
            }
        }
        break;
    case eControl_Button_B002:  // �ݱ�
    case eControl_Button_B086:  // x��ư
        {
            ShowInterface(FALSE);
        }
        break;
    default:
        {
        }
        break;
    }
    
    return result;
}
void uiAttendanceCheckDialog::UpdateCalendar()
{
    UpdateCalendar(current_year_, current_month_);
}
void uiAttendanceCheckDialog::UpdateCalendar(int year, int month)
{
    if (year < 0 || month < 1 || month > 12)
    {
        return;
    }

    const ATTENDANCE* attendance_info = AttendanceParser::Instance()->GetAttendance(year, month);
    if (attendance_info == NULL)
    {
#ifdef _DEV_VER
        TCHAR message[INTERFACE_STRING_LENGTH] = {0,};
        Sprintf(message, "����!! �⼮üũ - �������� �ʴ� ��� �Դϴ�.(year:%d, month:%d)", year, month);
        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,message);
#endif //_DEV_VER
        ReleaseCalendarSlot();
        current_year_ = year;
        current_month_ = month;
        return;
    }

    // ��ũ�� ���� + ù ������ ��¥ ���
    UpdateCalendar_Scroll(year, month, attendance_info->type);

    int first_day = 0;

#ifdef _NA_000000_20150428_ATTENDANCE_CHECK_SYSTEM_SUPERPOSITION_TYPE_MODIFY
    first_day = GameFunc::GetDayOfTheWeek(year, month, 1);
    if (scroll_control_->GetScrollPosWZ() > 0)
    {
        //��ũ�� ����������
        first_day -= 7;
    }
#else
    if (attendance_info->type == eATTENDANCE_CALENDAR)
    {
        first_day = GameFunc::GetDayOfTheWeek(year, month, 1);
        if (scroll_control_->GetScrollPosWZ() > 0)
        {
            //��ũ�� ����������
            first_day -= 7;
        }
    }
    else
    {
        // �������� 1��° ĭ�� 1����
        first_day = 0;
    }
#endif //_NA_000000_20150428_ATTENDANCE_CHECK_SYSTEM_SUPERPOSITION_TYPE_MODIFY
    
    // ������ �����ִ� ��¥ �̸� ���
    int last_reward_day = 0;
    for (int i = ATTENDANCE::kDayMaxCount; i > 0; --i)
    {
        if (attendance_info->day_reward_index[i-1] != 0)
        {
            last_reward_day = i;
            break;
        }
    }


    //////////////////////////////////////////////////////////////////////////
    // �޷� ������Ʈ
    //////////////////////////////////////////////////////////////////////////

    current_year_ = year;
    current_month_ = month;
    
    UpdateCalendar_Title(attendance_info->title);
    UpdateCalendar_Explanation_Title(attendance_info->type);
    UpdateCalendar_Explanation(attendance_info->explanation);
    UpdateCalendar_DayoftheWeek(attendance_info->type);
    UpdateCalendar_Button();

    

    // ��¥���Լ��� index - �����ε���, day - �ش����
    for (int index = 0; index < MAX_CALENDAR_SLOT; ++index)
    {
        int day = index - first_day + 1;

        if (day > 0 && day <= GameFunc::GetLastDayOfTheMonth(year, month))
        {
            // ����Ÿ��
#ifdef _NA_000000_20150428_ATTENDANCE_CHECK_SYSTEM_SUPERPOSITION_TYPE_MODIFY
            if (true) /*Ÿ�� ������� true*/
#else
            if (attendance_info->type == eATTENDANCE_CALENDAR)
#endif //_NA_000000_20150428_ATTENDANCE_CHECK_SYSTEM_SUPERPOSITION_TYPE_MODIFY
            {
                BYTE force_type = attendance_info->day_emphasis[day-1];
                if ((index % 7 == 0) &&
                    (force_type == 1 || force_type == 3))
                {
                    // �Ͽ����� ������
                    ++force_type;
                }
                calendar_slot[index].SetForceType(force_type);
            }
            else
            {
                calendar_slot[index].SetForceType(attendance_info->day_emphasis[day-1]);
            }

            // ��¥
            TCHAR day_string[10] = {0,};
            _itot(day, day_string, 10);
            if (attendance_info->type == eATTENDANCE_CALENDAR)
            {
                // 3514 %s
                g_InterfaceManager.GetInterfaceStringFormat(day_string, 10, 3514, day_string);
                calendar_slot[index].SetString(day_string);
            }
            else
            {
                // �������� ������ �����ִ� ������ ���������� ǥ����
                if (day <= last_reward_day)
                {
                    // 3515 %s����
                    g_InterfaceManager.GetInterfaceStringFormat(day_string, 10, 3515, day_string);
                    calendar_slot[index].SetString(day_string);
                }
                else
                {
                    calendar_slot[index].SetString("");
                }
            }


            // ������
            calendar_slot[index].SetItem(attendance_info->day_reward_index[day-1]);
            calendar_slot[index].ShowSlotImage(attendance_info->day_reward_index[day-1] ? TRUE : FALSE);

            // �⼮üũ����
            uiAttendanceCheckMan* manager = static_cast<uiAttendanceCheckMan*>(GetMan());
            if ((manager != NULL) &&
                (manager->recv_attendance_info().GetYear() == year) &&
                (manager->recv_attendance_info().GetMonth() == month))
            {
                if (attendance_info->type == eATTENDANCE_CALENDAR)
                {
                    eATTENDANCE_INFO* check = manager->recv_attendance_info().GetAttendance(day-1);

                    if (check != NULL)
                    {
                        calendar_slot[index].ShowCheckType(*check);
                    }
                }
                else //attendance_info->type == eATTENDANCE_SUPERPOSITION
                {
                    int reward_end = manager->recv_attendance_info().GetAttendanceInfoCount(eATTENDANCE_INFO_ALEADY_GOT_REWARD);
                    int reward_ready = manager->recv_attendance_info().GetAttendanceInfoCount(eATTENDANCE_INFO_ATTENDED);

#ifdef _NA_000000_20150428_ATTENDANCE_CHECK_SYSTEM_SUPERPOSITION_TYPE_MODIFY
                    if (reward_end + first_day > index)
                    {
                        calendar_slot[index].ShowCheckType(eATTENDANCE_INFO_ALEADY_GOT_REWARD);
                    }
                    else if (reward_end + reward_ready + first_day> index)
                    {
                        calendar_slot[index].ShowCheckType(eATTENDANCE_INFO_ATTENDED);
                    }
                    else
                    {
                        calendar_slot[index].ShowCheckType(eATTENDANCE_INFO_NOT_ATTEND);
                    }
#else
                    if (reward_end > index)
                    {
                        calendar_slot[index].ShowCheckType(eATTENDANCE_INFO_ALEADY_GOT_REWARD);
                    }
                    else if (reward_end + reward_ready > index)
                    {
                        calendar_slot[index].ShowCheckType(eATTENDANCE_INFO_ATTENDED);
                    }
                    else
                    {
                        calendar_slot[index].ShowCheckType(eATTENDANCE_INFO_NOT_ATTEND);
                    }
#endif //
                }
            }
            else
            {
                calendar_slot[index].ShowCheckType(eATTENDANCE_INFO_NOT_ATTEND);
            }
        }
        else
        {
            // �� ĭ
            calendar_slot[index].SetForceType(1);
            calendar_slot[index].SetString("");
            calendar_slot[index].SetItem(0);
            calendar_slot[index].ShowCheckType(eATTENDANCE_INFO_NOT_ATTEND);
            calendar_slot[index].ShowSlotImage(FALSE);
        }
    }
}

void uiAttendanceCheckDialog::UpdateCalendar_Scroll(int year, int month, eATTENDANCE_TYPE type)
{
    int first_day = GameFunc::GetDayOfTheWeek(year, month, 1);

    if (first_day < 0 || scroll_control_ == NULL)
    {
        return;
    }
#ifdef _NA_000000_20150428_ATTENDANCE_CHECK_SYSTEM_SUPERPOSITION_TYPE_MODIFY
    // �ʰ� �� �ִ� ���̸� ��ũ�� ����
    if (first_day > 4)
#else
    // �޷��� + �ʰ� �� �ִ� ���̸� ��ũ�� ����
    if (first_day > 4 && type == eATTENDANCE_CALENDAR)
#endif //_NA_000000_20150428_ATTENDANCE_CHECK_SYSTEM_SUPERPOSITION_TYPE_MODIFY
    {
        if (current_year_ != year || current_month_ != month)
        {
            // ���� �ٲ������ ��ũ�� 0��ġ��
            scroll_control_->SetScrollPosWZ(0);
            scroll_control_->SetScrollRangeWZ(0,1);
            scroll_control_->SetChangeSizeThumb(85);
        }
    }
    else
    {
        scroll_control_->SetScrollPosWZ(0);
        scroll_control_->SetScrollRangeWZ(0,0);
    }
}

void uiAttendanceCheckDialog::UpdateCalendar_Title(int string_id)
{
    // ����
    CControlWZ* title_control = GetControlWZ_byPosition(eControl_Static_S003);
    if (title_control)
    {
        title_control->SetTextID(string_id);
    }
}

void uiAttendanceCheckDialog::UpdateCalendar_Explanation_Title(eATTENDANCE_TYPE type)
{
    //eControl_Static_S002,  // ����� ����
    CControlWZ* title_control = GetControlWZ_byPosition(eControl_Static_S002);
    if (title_control)
    {
        if (type == eATTENDANCE_CALENDAR)
        {
            // 3512 �޷���
            title_control->SetTextID(3512);
        }
        else
        {
            // 3513 ������
            title_control->SetTextID(3513);
        }
    }
}

void uiAttendanceCheckDialog::UpdateCalendar_Explanation(int string_id)
{
    ftext_render_.ClearSentence();

    CControlWZ* control = GetControlWZ_byPosition(eControl_Static_S045);
    if (control)
    {
        FTextParser parser;
        FTextSentence sentence;
        TCHAR string[INTERFACE_STRING_LENGTH]= " ";
        BOOL result = false;

        g_InterfaceManager.GetInterfaceString(string_id, string, INTERFACE_STRING_LENGTH);

        result = parser.Parser(string, sentence);
        if (result == TRUE)
        {
            ftext_render_.Append(sentence);
        }
    }
    ftext_render_.convertLineInfo();
}

void uiAttendanceCheckDialog::UpdateCalendar_DayoftheWeek(eATTENDANCE_TYPE type)
{
    // ���� ǥ��/����
    for (int i = 0; i < 7; ++i)
    {
        CControlWZ* control = GetControlWZ_byPosition(eControl_Static_S000 + i);
        if (control)
        {
#ifdef _NA_000000_20150428_ATTENDANCE_CHECK_SYSTEM_SUPERPOSITION_TYPE_MODIFY
            control->ShowWindowWZ(WZ_SHOW);
#else
            control->ShowWindowWZ(type == eATTENDANCE_CALENDAR ? WZ_SHOW : WZ_HIDE);
#endif //_NA_000000_20150428_ATTENDANCE_CHECK_SYSTEM_SUPERPOSITION_TYPE_MODIFY
        }
    }
}

void uiAttendanceCheckDialog::UpdateCalendar_Button()
{
    CControlWZ* control = GetControlWZ_byPosition(eControl_Button_B000);
    if (control)
    {
        // 3258 ������ ���� Ȯ��
        // 3259 ���ư���
        control->SetTextID(default_month_ == current_month_ ? 3258 : 3259);
    }


    control = GetControlWZ_byPosition(eControl_Button_B001);
    if (control)
    {
        // ����ޱ� ��ư Ȱ��ȭ/��Ȱ��ȭ
        uiAttendanceCheckMan* manager = static_cast<uiAttendanceCheckMan*>(GetMan());
        if ((manager != NULL) &&
            (manager->recv_attendance_info().GetYear() == current_year_) &&
            (manager->recv_attendance_info().GetMonth() == current_month_))
        {
            control->EnableWindowWZ(true);
        }
        else
        {
            control->EnableWindowWZ(false);
        }
    }
}

#endif // _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
