#include "SunClientPrecompiledHeader.h"

#ifdef _NA_000000_20140120_SMART_NPC_SYSTEM
#include "uiSmartNpcDialogMan/uiSmartNpcDialogMan.h"
#include "uiSmartNpcDialogMan/uiSmartNpcDialogMenu.h"
#include "uiSmartNpcDialogMan/uiSmartNpcDialog.h"
#include "InterfaceManager.h"
#include "ObjectManager.h"
#include "PacketStruct_CG_SmartNPC.h"
#include "Hero.h"
#include "GlobalFunc.h"

uiSmartNpcDialogMenu::uiSmartNpcDialogMenu(InterfaceManager* ui_manager) : uiSmartNpcDialog(ui_manager),
scroll_control_(NULL),
list_control_(NULL),
button_count_(0)
{
    dialog_type_ = SmartDialogType_Menu;

    RegisterControlPosition(StrToWzID("B999"), kControl_Button_B999);
    RegisterControlPosition(StrToWzID("T999"), kControl_Text_T999);
    RegisterControlPosition(StrToWzID("L000"), kControl_List_L000);
    RegisterControlPosition(StrToWzID("V001"), kControl_VScroll_V001);

    RegisterControlPosition(StrToWzID("B001"), kControl_Button_B001);
    RegisterControlPosition(StrToWzID("B002"), kControl_Button_B002);
    RegisterControlPosition(StrToWzID("B003"), kControl_Button_B003);
    RegisterControlPosition(StrToWzID("B004"), kControl_Button_B004);
    RegisterControlPosition(StrToWzID("B005"), kControl_Button_B005);
    RegisterControlPosition(StrToWzID("B006"), kControl_Button_B006);
    
    for (int i = 0; i < uiSmartNpcDialogMan::MaxMenuButton; ++i)
    {
        button_event_id[i].event_id = 0;
        button_event_id[i].step = 0;
    }
}

uiSmartNpcDialogMenu::~uiSmartNpcDialogMenu()
{
}
VOID uiSmartNpcDialogMenu::Init(CDrawBase* pDrawBase)
{
    draw_ = pDrawBase;

    uiBase::Init(pDrawBase);
}
BOOL uiSmartNpcDialogMenu::CreateControls(BYTE sub_type)
{
    CDialogWZ* dialog_ = GetDialogWZ();

    // 아직 sub_type 미구현. 1가지 모먕임

    // ui 무늬 - 중앙상단
    CCtrlPictureWZ* picture_control = CreateContorlPicture(IM_SMARTNPCDIALOG_MANAGER::SMARTNPCDIALOG_MENU, StrToWzID("P000"));
    if (picture_control == NULL)
    {
        return FALSE;
    }
    else
    {
        picture_control->MoveWindowWZ(dialog_->GetDlgX() + 115, dialog_->GetDlgY(), 100, 20);
        SI_TSUBINFO image_sub_info;
        image_sub_info.Initialize();
        image_sub_info.m_blIs = true;

        image_sub_info.m_sImageName = "Common_bg01.tga";
        image_sub_info.m_blUseGradation = false;
        image_sub_info.m_fTileCornerX = 0;
        image_sub_info.m_fTileCornerY = 0;

        image_sub_info.m_nType = SI_TSUBINFO::DrawType_Whole;
        image_sub_info.m_sFRect.fTopX = 430;
        image_sub_info.m_sFRect.fTopY = 940;
        image_sub_info.m_sFRect.fWidth = 100;
        image_sub_info.m_sFRect.fHeight = 20;
        image_sub_info.m_color = 0xffffffff; //RGBA(255,255,255,255);
        picture_control->SetCtrlDetailImageFormat(E_BASE, &image_sub_info);

        picture_control->SetDiscardMessage(true);
    }
    // ui 무늬 - 좌측상단
    picture_control = CreateContorlPicture(IM_SMARTNPCDIALOG_MANAGER::SMARTNPCDIALOG_MENU, StrToWzID("P001"));
    if (picture_control == NULL)
    {
        return FALSE;
    }
    else
    {
        picture_control->MoveWindowWZ(dialog_->GetDlgX() + 12, dialog_->GetDlgY() + 11, 100, 20);
        SI_TSUBINFO image_sub_info;
        image_sub_info.Initialize();
        image_sub_info.m_blIs = true;

        image_sub_info.m_sImageName = "Common_bg01.tga";
        image_sub_info.m_blUseGradation = false;
        image_sub_info.m_fTileCornerX = 0;
        image_sub_info.m_fTileCornerY = 0;

        image_sub_info.m_nType = SI_TSUBINFO::DrawType_Whole;
        image_sub_info.m_sFRect.fTopX = 430;
        image_sub_info.m_sFRect.fTopY = 960;
        image_sub_info.m_sFRect.fWidth = 100;
        image_sub_info.m_sFRect.fHeight = 20;
        image_sub_info.m_color = 0xffffffff; //RGBA(255,255,255,255);
        picture_control->SetCtrlDetailImageFormat(E_BASE, &image_sub_info);

        picture_control->SetDiscardMessage(true);
    }
    // ui 무늬 - 우측상단
    picture_control = CreateContorlPicture(IM_SMARTNPCDIALOG_MANAGER::SMARTNPCDIALOG_MENU, StrToWzID("P002"));
    if (picture_control == NULL)
    {
        return FALSE;
    }
    else
    {
        picture_control->MoveWindowWZ(dialog_->GetDlgX() + 198, dialog_->GetDlgY() + 11, 100, 20);
        SI_TSUBINFO image_sub_info;
        image_sub_info.Initialize();
        image_sub_info.m_blIs = true;

        image_sub_info.m_sImageName = "Common_bg01.tga";
        image_sub_info.m_blUseGradation = false;
        image_sub_info.m_fTileCornerX = 0;
        image_sub_info.m_fTileCornerY = 0;

        image_sub_info.m_nType = SI_TSUBINFO::DrawType_Whole;
        image_sub_info.m_sFRect.fTopX = 430;
        image_sub_info.m_sFRect.fTopY = 980;
        image_sub_info.m_sFRect.fWidth = 100;
        image_sub_info.m_sFRect.fHeight = 20;
        image_sub_info.m_color = 0xffffffff; //RGBA(255,255,255,255);
        picture_control->SetCtrlDetailImageFormat(E_BASE, &image_sub_info);

        picture_control->SetDiscardMessage(true);
    }

    // 창 이름
    CCtrlStaticWZ* static_control = CreateContorlStatic(IM_SMARTNPCDIALOG_MANAGER::SMARTNPCDIALOG_MENU, StrToWzID("T999"));
    if (static_control == NULL)
    {
        return FALSE;
    }
    else
    {
        static_control->MoveWindowWZ(dialog_->GetDlgX() + 39, dialog_->GetDlgY() + 12, 233, 17);
    }

    // 리스트
    CCtrlListWZ* list_control = CreateContorlList(IM_SMARTNPCDIALOG_MANAGER::SMARTNPCDIALOG_MENU, StrToWzID("L000"));
    if (list_control == NULL)
    {
        return FALSE;
    }
    else
    {
        list_control->MoveWindowWZ(dialog_->GetDlgX() + 27, dialog_->GetDlgY() + 49, 241, 192);
        list_control_ = list_control;
        RECT list_rect = list_control_->GetSizeRect();
        ftext_render_.Init(draw_, list_rect);

        ftext_render_.SetMargin(0,0,0,-5);
        ftext_render_.SetGap(0,0);

        ftext_render_.convertLineInfo();
    }

    // 리스트 뒤에 이미지1
    picture_control = CreateContorlPicture(IM_SMARTNPCDIALOG_MANAGER::SMARTNPCDIALOG_MENU, StrToWzID("P003"));
    if (picture_control == NULL)
    {
        return FALSE;
    }
    else
    {
        picture_control->MoveWindowWZ(dialog_->GetDlgX() + 21, dialog_->GetDlgY() + 41, 268, 207);
        SI_TSUBINFO image_sub_info;
        image_sub_info.Initialize();
        image_sub_info.m_blIs = true;

        image_sub_info.m_sImageName = "Common_bg01.tga";
        image_sub_info.m_blUseGradation = false;
        image_sub_info.m_fTileCornerX = 5;
        image_sub_info.m_fTileCornerY = 5;

        image_sub_info.m_nType = SI_TSUBINFO::DrawType_TileExpand;
        image_sub_info.m_sFRect.fTopX = 398;
        image_sub_info.m_sFRect.fTopY = 147;
        image_sub_info.m_sFRect.fWidth = 17;
        image_sub_info.m_sFRect.fHeight = 17;
        image_sub_info.m_color = 0xffffffff; //RGBA(255,255,255,255);
        picture_control->SetCtrlDetailImageFormat(E_BASE, &image_sub_info);
    }

    // 리스트 뒤에 이미지2
    picture_control = CreateContorlPicture(IM_SMARTNPCDIALOG_MANAGER::SMARTNPCDIALOG_MENU, StrToWzID("P004"));
    if (picture_control == NULL)
    {
        return FALSE;
    }
    else
    {
        picture_control->MoveWindowWZ(dialog_->GetDlgX() + 13, dialog_->GetDlgY() + 33, 284, 223);
        SI_TSUBINFO image_sub_info;
        image_sub_info.Initialize();
        image_sub_info.m_blIs = true;

        image_sub_info.m_sImageName = "Common_bg01.tga";
        image_sub_info.m_blUseGradation = false;
        image_sub_info.m_fTileCornerX = 12;
        image_sub_info.m_fTileCornerY = 12;

        image_sub_info.m_nType = SI_TSUBINFO::DrawType_TileExpand;
        image_sub_info.m_sFRect.fTopX = 390;
        image_sub_info.m_sFRect.fTopY = 0;
        image_sub_info.m_sFRect.fWidth = 26;
        image_sub_info.m_sFRect.fHeight = 26;
        image_sub_info.m_color = 0xffffffff; //RGBA(255,255,255,255);
        picture_control->SetCtrlDetailImageFormat(E_BASE, &image_sub_info);
    }

    // 스크롤 뒤에 이미지
    picture_control = CreateContorlPicture(IM_SMARTNPCDIALOG_MANAGER::SMARTNPCDIALOG_MENU, StrToWzID("P005"));
    if (picture_control == NULL)
    {
        return FALSE;
    }
    else
    {
        picture_control->MoveWindowWZ(dialog_->GetDlgX() + 271, dialog_->GetDlgY() + 45, 14, 199);
        SI_TSUBINFO image_sub_info;
        image_sub_info.Initialize();
        image_sub_info.m_blIs = true;

        image_sub_info.m_sImageName = "all_butten4.tga";
        image_sub_info.m_blUseGradation = false;
        image_sub_info.m_fTileCornerX = 7;
        image_sub_info.m_fTileCornerY = 20;

        image_sub_info.m_nType = SI_TSUBINFO::DrawType_TileExpand;
        image_sub_info.m_sFRect.fTopX = 117;
        image_sub_info.m_sFRect.fTopY = 30;
        image_sub_info.m_sFRect.fWidth = 16;
        image_sub_info.m_sFRect.fHeight = 42;
        image_sub_info.m_color = 0xffffffff; //RGBA(255,255,255,255);
        picture_control->SetCtrlDetailImageFormat(E_BASE, &image_sub_info);
    }
    
    // 스크롤
    CCtrlVScrollWZ* scroll_control = CreateContorlVScroll(IM_SMARTNPCDIALOG_MANAGER::SMARTNPCDIALOG_MENU, StrToWzID("V001"));
    if (scroll_control == NULL)
    {
        return FALSE;
    }
    else
    {
        scroll_control->MoveWindowWZ(dialog_->GetDlgX() + 273, dialog_->GetDlgY() + 47, 10, 195);
        scroll_control_ = scroll_control;
        scroll_control_->SetScrollPosWZ(0);
        scroll_control_->SetScrollRangeWZ(0,1);
    }

    // x 버튼
    CCtrlButtonWZ* button_control = CreateContorlCloseButton(IM_SMARTNPCDIALOG_MANAGER::SMARTNPCDIALOG_MENU, StrToWzID("B999"));
    if (button_control == NULL)
    {
        return FALSE;
    }
    else
    {
        button_control->MoveWindowWZ(dialog_->GetDlgX() + 278, dialog_->GetDlgY() + 13, 17, 17);
    }

    // 버튼 뒤에 이미지1
    picture_control = CreateContorlPicture(IM_SMARTNPCDIALOG_MANAGER::SMARTNPCDIALOG_MENU, StrToWzID("P006"));
    if (picture_control == NULL)
    {
        return FALSE;
    }
    else
    {
        picture_control->MoveWindowWZ(
            dialog_->GetDlgX() + 21,
            dialog_->GetDlgY() + 259,
            268,
            button_count_ * 30.0f);
        SI_TSUBINFO image_sub_info;
        image_sub_info.Initialize();
        image_sub_info.m_blIs = true;

        image_sub_info.m_sImageName = "Common_bg01.tga";
        image_sub_info.m_blUseGradation = false;
        image_sub_info.m_fTileCornerX = 5;
        image_sub_info.m_fTileCornerY = 5;

        image_sub_info.m_nType = SI_TSUBINFO::DrawType_TileExpand;
        image_sub_info.m_sFRect.fTopX = 398;
        image_sub_info.m_sFRect.fTopY = 147;
        image_sub_info.m_sFRect.fWidth = 17;
        image_sub_info.m_sFRect.fHeight = 17;
        image_sub_info.m_color = 0xffffffff; //RGBA(255,255,255,255);
        picture_control->SetCtrlDetailImageFormat(E_BASE, &image_sub_info);
    }

    // 버튼 뒤에 이미지2
    picture_control = CreateContorlPicture(IM_SMARTNPCDIALOG_MANAGER::SMARTNPCDIALOG_MENU, StrToWzID("P007"));
    if (picture_control == NULL)
    {
        return FALSE;
    }
    else
    {
        picture_control->MoveWindowWZ(
            dialog_->GetDlgX() + 13,
            dialog_->GetDlgY() + 251,
            284,
            15.0f + (button_count_ * 30));
        SI_TSUBINFO image_sub_info;
        image_sub_info.Initialize();
        image_sub_info.m_blIs = true;

        image_sub_info.m_sImageName = "Common_bg01.tga";
        image_sub_info.m_blUseGradation = false;
        image_sub_info.m_fTileCornerX = 12;
        image_sub_info.m_fTileCornerY = 12;

        image_sub_info.m_nType = SI_TSUBINFO::DrawType_TileExpand;
        image_sub_info.m_sFRect.fTopX = 390;
        image_sub_info.m_sFRect.fTopY = 0;
        image_sub_info.m_sFRect.fWidth = 26;
        image_sub_info.m_sFRect.fHeight = 26;
        image_sub_info.m_color = 0xffffffff; //RGBA(255,255,255,255);
        picture_control->SetCtrlDetailImageFormat(E_BASE, &image_sub_info);
    }

    // 버튼
    for (int i = 0; i < button_count_; ++i)
    {
        button_control = CreateContorlButton(IM_SMARTNPCDIALOG_MANAGER::SMARTNPCDIALOG_MENU, GetControlID_byPosition(i + kControl_Button_B001));
        if (button_control == NULL)
        {
            return FALSE;
        }
        else
        {
            button_control->MoveWindowWZ(dialog_->GetDlgX() + 23, dialog_->GetDlgY() + 440.0f - ((6-i)*30), 264, 29);
        }
    }
    


    return TRUE;
}
void uiSmartNpcDialogMenu::MessageProc(SI_MESSAGE* message)
{
    if (IsLock())
        return;

    switch (message->eResultMsg)
    {
    case RT_MSG_LBUTTONCLICK:
        OnLButtonClick(message);
        break;
    case RT_MSG_SB_PAGETOP:			//. 윗쪽 몸통을 눌렀다.
    case RT_MSG_SB_LINETOP:			//.윗쪽 화살표를 눌렀다.
        {
            if(scroll_control_)
            {
                int pos = scroll_control_->GetScrollPosWZ();        // 현재 스크롤 값을 받아 온다.
                int mmin = 0 , mmax = 0;
                scroll_control_->GetScrollRangeWZ( &mmin, &mmax );  // 스크롤 값의 최대 최소를 얻어 온다.

                if(pos>mmin)    // 현재 입력된 라인수 보다 작으면 
                {
                    --pos;      // 한줄 감소 시키고 
                    scroll_control_->SetScrollPosWZ(pos);
                    ftext_render_.SetBeginLineIndex(pos);
                }
            }
        }
        break;
    case RT_MSG_SB_PAGEBOTTOM:				//. 아래쪽 몸통을 눌렀다.
    case RT_MSG_SB_LINEBOTTOM:				//.아랫쪽 화살표를 눌렀다
        {
            if(scroll_control_)
            {
                int pos = scroll_control_->GetScrollPosWZ();        // 현재 스크롤 값을 받아 온다.
                int mmin = 0 , mmax = 0;
                scroll_control_->GetScrollRangeWZ( &mmin, &mmax );  // 스크롤 값의 최대 최소를 얻어 온다.

                if(pos<mmax)    // 현재 입력된 라인수 보다 작으면 
                {
                    pos++;      // 한줄 증가 시키고 
                    scroll_control_->SetScrollPosWZ(pos);
                    ftext_render_.SetBeginLineIndex(pos);
                }
            }
        }
        break;
    case RT_MSG_SB_THUMBTRACK:				//. 썸을 드래그 하고 있다.
    case RT_MSG_SB_THUMBPOSITION:			//. 썸을 드래그한후 마우스를 놓았다.
    case RT_MSG_SB_ENDSCROLL:				//. 스크롤이 종료 되었다.
        {
            if(scroll_control_)
            {
                int pos = scroll_control_->GetScrollPosWZ();        // 현재 스크롤 값을 받아 온다.
                ftext_render_.SetBeginLineIndex(pos);
            }
        }
        break;
        break;
    default:
        SolarDialog::MessageProc(message);
        break;
    }
}
bool uiSmartNpcDialogMenu::OnLButtonClick(SI_MESSAGE* message)
{
    switch (GetControlPosition_byID(message->dwCtrlID))
    {
    case kControl_Button_B999:
        {
            g_InterfaceManager.DeleteInterface(IM_SMARTNPCDIALOG_MANAGER::SMARTNPCDIALOG_MENU);
        }
        break;
    case kControl_Button_B001:
    case kControl_Button_B002:
    case kControl_Button_B003:
    case kControl_Button_B004:
    case kControl_Button_B005:
    case kControl_Button_B006:
        {
            DWORD index = GetControlPosition_byID(message->dwCtrlID);
            EVENT_IDSTEP& event_id = button_event_id[index - kControl_Button_B001];
            
            MSG_CG_NPCDIALOG_ON_CLICK_MENU_CMD send_packet(
                g_pHero->GetObjectKey(),
                GetOwnerKey(),
                event_id.event_id,
                event_id.step);
            GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send_packet, sizeof(send_packet));
            g_InterfaceManager.DeleteInterface(IM_SMARTNPCDIALOG_MANAGER::SMARTNPCDIALOG_MENU);
        }
        break;
    default:
        {
        }
        break;
    }

    return true;
}
void uiSmartNpcDialogMenu::OnRenderSolarDialog()
{
    ftext_render_.Render();
}
void uiSmartNpcDialogMenu::OnUpdateSolarDialog()
{
    UpdateMouseWheel();

    if (GetOwnerKey() != 0)
    {
        Object* object = (g_ObjectManager.GetObject(GetOwnerKey()));
        if (object && g_pHero)
        {
            WzVector vDiff = object->GetPosition() - g_pHero->GetVisiblePos();
            float distance = VectorLength2(&vDiff);
            if (distance > (DISTANCE_NPC_MEET * DISTANCE_NPC_MEET))
            {
                //g_InterfaceManager.DeleteInterface(IM_SMARTNPCDIALOG_MANAGER::SMARTNPCDIALOG_MENU);
                SetDestroyWindow(TRUE);
                return;
            }
        }
    }
}
void uiSmartNpcDialogMenu::OnShowWindow(BOOL val)
{
    if (val)
    {
        keyMsg msg;
        ZeroMemory(&msg,sizeof(msg));
        msg.dwType=eDoType_SmartMenu_Dialog;
        msg.wParam=IM_SMARTNPCDIALOG_MANAGER::SMARTNPCDIALOG_MENU;
        msg.DoSomething=GlobalFunc::DoSmartNpcMenuDialog;
        g_KeyQueueManager.PushBack(msg);
    }
    else
    {
        g_KeyQueueManager.DeleteMsg(eDoType_SmartMenu_Dialog);
    }
}
void uiSmartNpcDialogMenu::UpdateMouseWheel()
{
    if(list_control_ == NULL || scroll_control_ == NULL)
    {
        return;
    }

    int scroll_max_line = 0;
    size_t list_max_line = ftext_render_.GetDrawLineInRect();
    if (ftext_render_.GetMaxLine() > list_max_line)
    {
        scroll_max_line = ftext_render_.GetMaxLine() - list_max_line;
    }
    scroll_control_->SetScrollRangeWZ(0, scroll_max_line);

    RECT list_rect = list_control_->GetSizeRect();
    ftext_render_.SetRect(list_rect);
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
                ftext_render_.SetBeginLineIndex(index);
                scroll_control_->SetScrollPosWZ(index);
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
                ftext_render_.SetBeginLineIndex(index);
                scroll_control_->SetScrollPosWZ(index);
            }
        }
        break;
    default:
        break;
    }
}

void uiSmartNpcDialogMenu::SetControlText(BYTE contorl_index, const TCHAR *text_massage)
{
    if (contorl_index >= kControl_Size)
    {
        return;
    }
    switch (contorl_index)
    {
    case kControl_Text_T999:
        {
            CCtrlStaticWZ* control = static_cast<CCtrlStaticWZ*>(GetControlWZ_byPosition(contorl_index));
            if (control)
            {
                control->SetTextWZ(text_massage);
            }
        }
        break;
    case kControl_Button_B001:
    case kControl_Button_B002:
    case kControl_Button_B003:
    case kControl_Button_B004:
    case kControl_Button_B005:
    case kControl_Button_B006:
        {
            CCtrlButtonWZ* control = static_cast<CCtrlButtonWZ*>(GetControlWZ_byPosition(contorl_index));
            if (control)
            {
                control->SetTextWZ(text_massage);
            }
        }
        break;
    case kControl_List_L000:
        {
            SetTextMessage(text_massage);
        }
        break;
    }
    
}
void uiSmartNpcDialogMenu::SetControlEventID(BYTE contorl_index, DWORD event_id, DWORD step)
{
    if (contorl_index >= kControl_Size)
    {
        return;
    }
    switch (contorl_index)
    {
    case kControl_Button_B001:
    case kControl_Button_B002:
    case kControl_Button_B003:
    case kControl_Button_B004:
    case kControl_Button_B005:
    case kControl_Button_B006:
        {
            CCtrlButtonWZ* control = static_cast<CCtrlButtonWZ*>(GetControlWZ_byPosition(contorl_index));
            if (control)
            {
                int index = contorl_index - kControl_Button_B001;
                button_event_id[index].event_id = event_id;
                button_event_id[index].step = step;
            }
        }
        break;
    }
}

void uiSmartNpcDialogMenu::SetTextMessage(const TCHAR *text_massage)
{
    if (list_control_ == NULL || scroll_control_ == NULL)
    {
        return;
    }

    ftext_render_.ClearSentence();
    ftext_render_.SetBeginLineIndex(0);

    FTextParser parser;
    FTextSentence sentence;

    TCHAR str_format[INTERFACE_STRING_LENGTH]={0,};

    BOOL result = parser.Parser(text_massage, sentence);
    if (result == TRUE)
    {
        ftext_render_.Append(sentence);
        ftext_render_.convertLineInfo();
    }
    
    int scroll_range = ftext_render_.GetMaxLine() - ftext_render_.GetDrawLineInRect();
    scroll_control_->SetScrollRangeWZ(0,scroll_range);
    scroll_control_->SetScrollPosWZ(0);
}

#endif //_NA_000000_20140120_SMART_NPC_SYSTEM*
