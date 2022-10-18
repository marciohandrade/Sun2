#include "SunClientPrecompiledHeader.h"
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
#include "uiChaosZoneGoldRush.h"
#include "ChaosZoneSystem/PacketStruct_ChaosZoneSystem.h"
#include "ChaosSystem/ChaosSystemData.h"
#include "ChaosSystem/ChaosSystemFunction.h"

#include "ItemManager.h"
#include "GlobalFunc.h"
#include "uiToolTipMan/uiToolTipMan.h"
#include "BattleGroundInfoParser.h"
#include "MissionRewardParser.h"
#include "Mouse.h"
#include "ItemInfoParser.h"
#include "ItemUnitRenderer.h"
#include "RewardInfoList.h"
#include "ItemTypeList.h"
#include "Hero.h"

#include "ChaosZoneInfoParser.h"


const DWORD uiChaosZoneGoldRush::kDialog_ID = StrToWzID("0649");

uiChaosZoneGoldRush::uiChaosZoneGoldRush( InterfaceManager* ui_manager_ptr )    :   
ui_chaossystem_manager_(NULL),
once_make_list_(false),
scroll_control_ptr_(NULL),
list_control_ptr_(NULL),
item_unit_renderer_(NULL)
{
    RegisterControlPosition(StrToWzID("B999"), kControl_Button_B999);
    RegisterControlPosition(StrToWzID("T999"), kControl_Text_T999);
    RegisterControlPosition(StrToWzID("B005"), kControl_Button_B005);
    RegisterControlPosition(StrToWzID("V001"), kControl_VScroll_V001);
    RegisterControlPosition(StrToWzID("S000"), kControl_Text_S000);
    RegisterControlPosition(StrToWzID("P001"), kControl_Picture_P001);
    RegisterControlPosition(StrToWzID("P005"), kControl_Picture_P005);
    RegisterControlPosition(StrToWzID("P009"), kControl_Picture_P009);
    RegisterControlPosition(StrToWzID("L000"), kControl_List_L000);
    RegisterControlPosition(StrToWzID("L001"), kControl_List_L001);
    RegisterControlPosition(StrToWzID("S006"), kControl_Text_S006);
    RegisterControlPosition(StrToWzID("S009"), kControl_Text_S009);
    RegisterControlPosition(StrToWzID("S010"), kControl_Text_S010);
    RegisterControlPosition(StrToWzID("B000"), kControl_Button_B000);
    RegisterControlPosition(StrToWzID("B001"), kControl_Button_B001);
    RegisterControlPosition(StrToWzID("B002"), kControl_Button_B002);
    RegisterControlPosition(StrToWzID("B003"), kControl_Button_B003);
    RegisterControlPosition(StrToWzID("B004"), kControl_Button_B004);
    RegisterControlPosition(StrToWzID("B007"), kControl_Button_B007);

    RegisterControlPosition(StrToWzID("P024"), kControl_Picture_P024);
    RegisterControlPosition(StrToWzID("P025"), kControl_Picture_P025);
    RegisterControlPosition(StrToWzID("P026"), kControl_Picture_P026);
    RegisterControlPosition(StrToWzID("P027"), kControl_Picture_P027);
    RegisterControlPosition(StrToWzID("P028"), kControl_Picture_P028);
    RegisterControlPosition(StrToWzID("P029"), kControl_Picture_P029);
    RegisterControlPosition(StrToWzID("P030"), kControl_Picture_P030);
    RegisterControlPosition(StrToWzID("P031"), kControl_Picture_P031);
    RegisterControlPosition(StrToWzID("P032"), kControl_Picture_P032);
    RegisterControlPosition(StrToWzID("P033"), kControl_Picture_P033);
}
//------------------------------------------------------------------------------
uiChaosZoneGoldRush::~uiChaosZoneGoldRush()
{
    

}
//------------------------------------------------------------------------------
void uiChaosZoneGoldRush::Init( CDrawBase* draw_base_ptr )
{
    // 슬롯초기화
    SCItemSlotContainer::Init(kSlotTotalCount, SI_CHAOSZONE_GOLDRUSH);

    // 렌더러 초기화
    if (!item_unit_renderer_)
    {
        item_unit_renderer_ = new ItemUnitRenderer;
        item_unit_renderer_->Init(uiBase::GetDrawBase(), kSlotTotalCount);
    }

    CCtrlStaticWZ* static_control_ptr = NULL;
    static_control_ptr = static_cast<CCtrlStaticWZ*>(GetControlWZ(GetControlID_byPosition(kControl_Text_S000)));
    if (static_control_ptr)
    {        
        // 50126 골드러시        
        static_control_ptr->SetTextID(50126);
    }

    static_control_ptr = static_cast<CCtrlStaticWZ*>(GetControlWZ(GetControlID_byPosition(kControl_Text_S006)));
    if (static_control_ptr)
    {
        // 2714 패배 팀
        static_control_ptr->SetTextID(2714);        
    }

    static_control_ptr = static_cast<CCtrlStaticWZ*>(GetControlWZ(GetControlID_byPosition(kControl_Text_S009)));
    if (static_control_ptr)
    {
        // 2577 보상 
        static_control_ptr->SetTextID(2577);        
    }

    static_control_ptr = static_cast<CCtrlStaticWZ*>(GetControlWZ(GetControlID_byPosition(kControl_Text_S010)));
    if (static_control_ptr)
    {
        // 2713 승리 팀 
        static_control_ptr->SetTextID(2713);
    }

    //이미지 비활성화
    CCtrlPictureWZ* picture_control_ptr;
    picture_control_ptr = static_cast<CCtrlPictureWZ*>(GetControlWZ(GetControlID_byPosition(kControl_Picture_P009)));
    if (picture_control_ptr)
    {
        picture_control_ptr->ShowWindowWZ(WZ_HIDE);
    }
    picture_control_ptr = static_cast<CCtrlPictureWZ*>(GetControlWZ(GetControlID_byPosition(kControl_Picture_P001)));
    if (picture_control_ptr)
    {
        picture_control_ptr->ShowWindowWZ(WZ_HIDE);
    }

    //버튼 비활성화
    CCtrlButtonCheckWZ* check_control_ptr = NULL;
    check_control_ptr = static_cast<CCtrlButtonCheckWZ*>(GetControlWZ(GetControlID_byPosition(kControl_Button_B002)));
    if (check_control_ptr)
    {
        check_control_ptr->EnableWindowWZ(false);
    }    

    // 입장 예약
    SetReservationButton(false);

    // 전장 정보
    list_control_ptr_ = static_cast<CCtrlListWZ*>(GetControlWZ(GetControlID_byPosition(kControl_List_L001)));
    if (list_control_ptr_)
    {
        RECT list_rect = list_control_ptr_->GetSizeRect();
        ftext_render_.Init(draw_base_ptr, list_rect);

        ftext_render_.SetMargin(0, 0, 0, -5);
        ftext_render_.SetGap(0, 0);

        ftext_render_.convertLineInfo();
    }

    // 스크롤
    scroll_control_ptr_ = GetVScrollControlWZ(GetControlID_byPosition(kControl_VScroll_V001));
    if (scroll_control_ptr_)
    {
        scroll_control_ptr_->SetScrollPosWZ(0);
        scroll_control_ptr_->SetScrollRangeWZ(0, 1);
    }

    // 트리 메뉴 구성
    select_index_.menu_depth[3] = 1;
    select_index_.menu_depth[2] = 0;
    select_index_.menu_depth[1] = 0;
    select_index_.menu_depth[0] = 0;

    tree_menu().Init(BASE_TreeMenuInfo::kGroupType_Chaoszone_GoldRushTree,
        GetListControlWZ(GetControlID_byPosition(kControl_List_L000)),
        NULL,
        true,
        NULL);
    tree_menu().set_menu_height_gab(20);

	TCHAR string_buffer[BASE_TreeMenuInfo::kNameLength] = {0, };
    TCHAR temp_string[INTERFACE_STRING_LENGTH] = {0, };

    BYTE* menu_depth_count = ChaosZoneTreeParser::Instance()->max_menu_depth();
    const ChaosZoneTreeInfo* find_info_ptr;

    TreeMenuGroup::TreeObject* tree_object_ptr = NULL;

    // 트리 리스트 생성하기 /* 3단으로 하려면 주석을 지워 주세요 (현재1단) */
    if (false == once_make_list_)
    {
        once_make_list_ = true;

        TreeMenuInfoParser::Instance()->Remove(BASE_TreeMenuInfo::kGroupType_Chaoszone_GoldRushTree);

        for (BYTE SGroupCount = 1; SGroupCount <= menu_depth_count[1]; ++SGroupCount)
        {
            find_info_ptr = ChaosZoneTreeParser::Instance()->GetTreeMenu(3, SGroupCount, 0);
            if (find_info_ptr != NULL)
            {
                g_InterfaceManager.GetInterfaceString(find_info_ptr->subtitle_string_code, temp_string);
                Snprintf(string_buffer, sizeof(string_buffer), temp_string);

                // 전장 트리는 특별하게 2단계로 되어 있는걸 1단계로 표시하기 위해 index가 변형되어 있습니다.
                MakeChaoszoneGoldRushTree(SGroupCount, 0, 0, 0, 
                    0, 0, string_buffer, find_info_ptr->font_color, find_info_ptr->map_code);

            }
            else
            {
                break;
            }
        }
    }
    tree_menu().ApplyTreeGroup(BASE_TreeMenuInfo::kGroupType_Chaoszone_GoldRushTree);
    tree_menu().Fill();

    tree_object_ptr = tree_menu().Find(1, 0, 0, 0);
    if (tree_object_ptr != NULL)
    {
        tree_menu().Select(tree_object_ptr);        
        ChaosSystem::GetGoldRushData().set_select_mapcode(tree_object_ptr->info->user_data);
    }

    // 트리가 만들어진 후 활성화 여부 체크
    for (BYTE SGroupCount = 1; SGroupCount <= menu_depth_count[1]; ++SGroupCount)
    {
        find_info_ptr = ChaosZoneTreeParser::Instance()->GetTreeMenu(3, SGroupCount, 0);
        if (find_info_ptr != NULL)
        {
            tree_object_ptr = tree_menu().Find(SGroupCount, 0, 0, 0);
            if (NULL == tree_object_ptr)
            {
                continue;
            }

            if (0 == find_info_ptr->enter_active)
            {
                tree_object_ptr->is_enable = false;
                continue;                
            }
        }
    }
}
//------------------------------------------------------------------------------
void uiChaosZoneGoldRush::Release()
{
    tree_menu().Release();
    last_select_index_ = 0;

    if (item_unit_renderer_)
    {
        item_unit_renderer_->Release();
        SAFE_DELETE(item_unit_renderer_);        
    }
}
//------------------------------------------------------------------------------
void uiChaosZoneGoldRush::OnReset()
{
    // 캐릭터 선택 창 진입 시 호출됨.
    ChaosSystem::Function::SetGoldRushReservation(false);
}
//------------------------------------------------------------------------------
void uiChaosZoneGoldRush::NetworkProc( MSG_BASE* recv_packet )
{

}
//------------------------------------------------------------------------------
void uiChaosZoneGoldRush::OnUpdateSolarDialog()
{
    UpdateMouseWheel();

    CControlWZ* control_ptr = NULL;
    control_ptr = GetControlWZ(GetControlID_byPosition(kControl_Button_B005));

    if (control_ptr)
    {
        if (last_select_index_ != select_index_.menu_index) // 인덱스가 바뀐 경우에만 업데이트 한다.
        {
            last_select_index_ = select_index_.menu_index;

            control_ptr->EnableWindowWZ(true);
            // 요구 아이템 체크
            const ChaosZoneTreeInfo* find_info_ptr = ChaosZoneTreeParser::Instance()->GetTreeMenu(3, select_index_.menu_depth[3], 0);

            const ChaosZoneInfo* chaoszone_info = ChaosZoneInfoParser::Instance()->FindData(static_cast<MAPCODE>(find_info_ptr->map_code));

            if ((NULL == find_info_ptr) ||  (NULL == chaoszone_info))
            {
                return;
            }

            bool is_have = false;
            TreeMenuGroup::TreeObject* tree_object = tree_menu().Find(select_index_.menu_index);
            if (chaoszone_info->require_item[0] != 0 && tree_object && tree_object->is_enable)
            {
                for (int i = 0; i < MAX_REQUIRE_ITEM; ++i)
                {
                    if (find_info_ptr && chaoszone_info->require_item[i]) // 필요 아이템이 있는 상태에서 확인버튼을 켜고 아이템이 없어졌다면, 서버오류시지를 이용합니다.
                    {
                        POSTYPE inventory_position = INVALID_POSTYPE_VALUE;
                        BOOL result = ItemManager::Instance()->FindFirstItem(chaoszone_info->require_item[i], inventory_position);

                        if (result && inventory_position != INVALID_POSTYPE_VALUE) // 아이템을 가지고 있다.
                        {
                            is_have = true;
                            break;
                        }
                    }
                }
            }

            if (chaoszone_info->require_item[0] == 0 && tree_object && tree_object->is_enable) // 첫번째 것이 0이라면 true
            {
                is_have = true;
            }

            if (false == is_have)
            {
                control_ptr->EnableWindowWZ(false);
            }

            // 골드러쉬 설명
            InitGoldRushInfo(find_info_ptr);
        }
    }

}
//------------------------------------------------------------------------------
void uiChaosZoneGoldRush::OnRenderSolarDialog()
{
    if (item_unit_renderer_)
    {
        item_unit_renderer_->Render();
    }

    ftext_render_.Render();
    RenderTooltip();

    if (this->m_MouseOverObject.eMouseOverType != eMOUSE_OVER_MAX)
    {
        if (eMOUSE_OVER_SLOT == m_MouseOverObject.eMouseOverType)
        {
            uiToolTipMan* tooltip_manager_ptr = static_cast<uiToolTipMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_TOOLTIP));
            if (tooltip_manager_ptr)
            {
                tooltip_manager_ptr->RegisterSlotInfoTooltip(m_MouseOverObject.pSlot);
                tooltip_manager_ptr->SetTooltipDialogID(GetDialogKey());
            }
        }
    }

}
//------------------------------------------------------------------------------
void uiChaosZoneGoldRush::MessageProc( SI_MESSAGE* message )
{
    if (IsLock())
        return;

    if (NULL == GetMan())
        return;

    switch (message->eResultMsg)
    {
    case RT_MSG_LBUTTONCLICK:
        OnLButtonClick(message);
        break;
    case RT_MSG_SB_PAGETOP: // 윗쪽 몸통을 눌렀다.
    case RT_MSG_SB_LINETOP: // 윗쪽 화살표를 눌렀다.
        {
            if (scroll_control_ptr_)
            {
                int pos = scroll_control_ptr_->GetScrollPosWZ(); // 현재 스크롤 값을 받아 온다.
                int mmin = 0, mmax = 0;
                scroll_control_ptr_->GetScrollRangeWZ( &mmin, &mmax ); // 스크롤 값의 최대 최소를 얻어 온다.

                if (pos > mmin) // 현재 입력된 라인수 보다 작으면
                {
                    --pos;
                    scroll_control_ptr_->SetScrollPosWZ(pos);
                    ftext_render_.SetBeginLineIndex(pos);
                }
            }
        }
        break;

    case RT_MSG_SB_PAGEBOTTOM: // 아래쪽 몸통을 눌렀다.
    case RT_MSG_SB_LINEBOTTOM: // 아랫쪽 화살표를 눌렀다.
        {
            if (scroll_control_ptr_)
            {
                int pos = scroll_control_ptr_->GetScrollPosWZ(); // 현재 스크롤 값을 받아 온다.
                int mmin = 0, mmax = 0;
                scroll_control_ptr_->GetScrollRangeWZ( &mmin, &mmax ); // 스크롤 값의 최대 최소를 얻어 온다.

                if (pos < mmax) // 현재 입력된 라인수 보다 작으면
                {
                    ++pos;
                    scroll_control_ptr_->SetScrollPosWZ(pos);
                    ftext_render_.SetBeginLineIndex(pos);                    
                }
            }
        }
        break;
    case RT_MSG_SB_THUMBTRACK:    // 썸을 드래그 하고 있다.
    case RT_MSG_SB_THUMBPOSITION: // 썸을 드래그한 후 마우스를 놓았다.
    case RT_MSG_SB_ENDSCROLL:     // 스크롤이 종료 되었다.
        {
            if (scroll_control_ptr_)
            {
                int pos = scroll_control_ptr_->GetScrollPosWZ(); // 현재 스크롤 값을 받아 온다.
                ftext_render_.SetBeginLineIndex(pos);
            }            
        }
        break;
    case RT_MSG_MOUSEOVER:
        {
            POSTYPE position = static_cast<POSTYPE>(GetControlPosition_byID(message->dwCtrlID));
            if (position >= 0 && position < this->GetMaxSlotNum())
            {
                if (!IsEmpty(position))
                {
                    SCSlot& rSlot = GetSlot(position);
                    m_MouseOverObject.eMouseOverType = eMOUSE_OVER_SLOT;
                    m_MouseOverObject.pSlot = &rSlot;
                }
                else
                {
                    m_MouseOverObject.eMouseOverType = eMOUSE_OVER_MAX;
                }
            }
            else
            {
                m_MouseOverObject.eMouseOverType = eMOUSE_OVER_MAX;
            }
        }
        break;
    default:
        {
            SolarDialog::MessageProc(message);
        }
        break;
    }
}
//------------------------------------------------------------------------------
void uiChaosZoneGoldRush::OnShowWindow( BOOL is_show )
{
    if (is_show)
    {
        // 체크버튼
        CCtrlButtonCheckWZ* check_button_control_ptr = NULL;
        for (int i = kControl_Button_B000; i <= kControl_Button_B007; ++i)
        {
            check_button_control_ptr = static_cast<CCtrlButtonCheckWZ*>(GetControlWZ(GetControlID_byPosition(i)));
            if (check_button_control_ptr)
            {
                check_button_control_ptr->SetCheckState(false);
            }
        }

        check_button_control_ptr = static_cast<CCtrlButtonCheckWZ*>(GetControlWZ(GetControlID_byPosition(kControl_Button_B001)));
        if (check_button_control_ptr)
        {
            check_button_control_ptr->SetCheckState(true);
        }
    }
}
//------------------------------------------------------------------------------
void uiChaosZoneGoldRush::UpdateMouseWheel()
{
    if (NULL == list_control_ptr_ || NULL == scroll_control_ptr_)
    {
        return;
    }

    int scroll_max_line = 0;
    size_t list_max_line = ftext_render_.GetDrawLineInRect();
    if (ftext_render_.GetMaxLine() > list_max_line)
    {
        scroll_max_line = ftext_render_.GetMaxLine() - list_max_line;
    }
    scroll_control_ptr_->SetScrollRangeWZ(0, scroll_max_line);

    RECT list_rect = list_control_ptr_->GetSizeRect();
    ftext_render_.SetRect(list_rect);
    switch (GetMouseWheelStatus(&list_rect))
    {
    case MOUSE_WHEEL_UP:
        {
            int index = scroll_control_ptr_->GetScrollPosWZ();
            int scroll_min = 0;
            int scroll_max = 0;
            scroll_control_ptr_->GetScrollRangeWZ(&scroll_min, &scroll_max);
            if (index > scroll_min)
            {
                --index;
                ftext_render_.SetBeginLineIndex(index);
                scroll_control_ptr_->SetScrollPosWZ(index);
            }
        }
        break;
    case MOUSE_WHEEL_DOWN:
        {
            int index = scroll_control_ptr_->GetScrollPosWZ();
            int scroll_min = 0;
            int scroll_max = 0;
            scroll_control_ptr_->GetScrollRangeWZ(&scroll_min, &scroll_max);
            if (index < scroll_max)
            {
                ++index;
                ftext_render_.SetBeginLineIndex(index);
                scroll_control_ptr_->SetScrollPosWZ(index);
            }
        }
        break;
    default:
        {

        }
        break;
    }
}

uiChaosSystemMan* uiChaosZoneGoldRush::GetManager()
{
    if (NULL == ui_chaossystem_manager_)
    {
        ui_chaossystem_manager_ = static_cast<uiChaosSystemMan*>(this->GetUIMan()->GetUserInterfaceManager(UIMAN_CHAOS));
        assert (ui_chaossystem_manager_);
    }
    return ui_chaossystem_manager_;    
}

bool uiChaosZoneGoldRush::OnLButtonClick( SI_MESSAGE* message )
{
    bool result = true;    

    uiChaosSystemMan* chaos_system_manager_ptr = static_cast<uiChaosSystemMan*>(GetMan());

    int tab_index = -1;

    if (NULL == chaos_system_manager_ptr)
    {
        return false;        
    }

    TreeMenuGroup::TreeObject* tree_object_ptr = NULL;
    if ((tree_menu().MessageProc(message, OUT tree_object_ptr) == true) && (tree_object_ptr != NULL))
    {
        // 트리 변경시
        SelectIndex(tree_object_ptr);
    }    

    // 이 부분을 고민해 보자.
    switch (GetControlPosition_byID(message->dwCtrlID))
    {
    case kControl_Button_B999:
        {
            chaos_system_manager_ptr->ShowChaosZone(uiChaosZoneGoldRush::kDialog_ID, false);
        }
        break;
    case kControl_Button_B005:
        {
            // 예약 버튼
            ChaosSystem::Function::SendReservationPacket();
        }
        break;
    case kControl_Button_B000:
        {
            tab_index = 0;
        }
        break;
    case kControl_Button_B001:
        {
            tab_index = 1;
        }
        break;
    case kControl_Button_B002:
        {
            tab_index = 2;
        }
        break;
    case kControl_Button_B003:
        {
            tab_index = 3;
        }
        break;
    case kControl_Button_B004:
        {
            tab_index = 4;
        }
        break;
    case kControl_Button_B007:
        {
            tab_index = 5;
        }
        break;
    default:
        {

        }
        break;
    }

    if (tab_index == -1)
        return false;
    
    chaos_system_manager_ptr->OpenChaosZone(tab_index, true);
    

    return result;
}

void uiChaosZoneGoldRush::RenderTooltip()
{
    uiToolTipMan* tooltip_manager_ptr = GET_DIALOG_MANAGER(uiToolTipMan, UIMAN_TOOLTIP);
    if (NULL == tooltip_manager_ptr)
    {
        return;
    }

    int mouse_x = Mouse::Instance()->GetMouseX();
    int mouse_y = Mouse::Instance()->GetMouseY();

    int tab_controls[] = {kControl_Button_B000, kControl_Button_B001, 0, kControl_Button_B003, kControl_Button_B004, kControl_Button_B007 };
    CControlWZ* control_ptr = NULL;
    for (int index = 0; index < sizeof(tab_controls) / sizeof(tab_controls[0]); ++index)
    {
        if (0 == tab_controls[index])
        {
            continue;
        }
        control_ptr = GetControlWZ_byPosition(tab_controls[index]);
        if (control_ptr)
        {
            RECT position = control_ptr->GetSizeRect();
            if (position.left < mouse_x && position.right > mouse_x && 
                position.top < mouse_y && position.bottom > mouse_y)
            {
                // 2571 ~ 2576 / 이스라필의 대지 ~ 카오스 히스토리
                // 스트링 테이블 요청해야 한다.
                tooltip_manager_ptr->RegisterTooltipWithStringCode(2571 + index);
                tooltip_manager_ptr->SetTooltipDialogID(GetDialogKey());
            }
        }
    }
}

void uiChaosZoneGoldRush::SetReservationButton( const bool& is_resvervation )
{
    CControlWZ* control_ptr = GetControlWZ(GetControlID_byPosition(kControl_Button_B005));
    if (control_ptr)
    {
        // 2584 입장예약
        if (is_resvervation == false)
        {
            // 2584 입장예약
            control_ptr->SetTextID(2584);
        }
        else
        {
            // 2585 예약취소
            control_ptr->SetTextID(2585);
        }
    }
}

void uiChaosZoneGoldRush::MakeChaoszoneGoldRushTree( BYTE depth_3, BYTE depth_2, BYTE depth_1, BYTE depth_0, int depth, DWORD string_code, TCHAR* string, const DWORD* font_colors, DWORD map_code )
{
    BASE_TreeMenuInfo tree_menu_info;
    ZeroMemory(&tree_menu_info, sizeof(tree_menu_info));

    tree_menu_info.font_id[BASE_TreeMenuInfo::kFontType_Enable] = StrToWzID("st10");
    tree_menu_info.font_id[BASE_TreeMenuInfo::kFontType_Disable] = StrToWzID("st10");
    tree_menu_info.font_id[BASE_TreeMenuInfo::kFontType_Over] = StrToWzID("st10");
    tree_menu_info.font_id[BASE_TreeMenuInfo::kFontType_Select] = StrToWzID("st10");

    tree_menu_info.font_color[BASE_TreeMenuInfo::kFontType_Enable] = font_colors[ChaosZoneTreeInfo::kFontType_Enable];
    tree_menu_info.font_color[BASE_TreeMenuInfo::kFontType_Disable] = font_colors[ChaosZoneTreeInfo::kFontType_Disable];
    tree_menu_info.font_color[BASE_TreeMenuInfo::kFontType_Over] = font_colors[ChaosZoneTreeInfo::kFontType_Over];
    tree_menu_info.font_color[BASE_TreeMenuInfo::kFontType_Select] = font_colors[ChaosZoneTreeInfo::kFontType_Select];


    tree_menu_info.image_code[BASE_TreeMenuInfo::kIconType_Enable] = 0;
    tree_menu_info.image_code[BASE_TreeMenuInfo::kIconType_Disable] = 10007;
    tree_menu_info.image_code[BASE_TreeMenuInfo::kIconType_Collpase] = 10002;
    tree_menu_info.image_code[BASE_TreeMenuInfo::kIconType_Unfold] = 10003;
    tree_menu_info.image_code[BASE_TreeMenuInfo::kIconType_CheckOn] = 0;
    tree_menu_info.image_code[BASE_TreeMenuInfo::kIconType_CheckOff] = 0;
    tree_menu_info.image_code[BASE_TreeMenuInfo::kIconType_Cursor] = 10008;

    tree_menu_info.tree_group = BASE_TreeMenuInfo::kGroupType_Chaoszone_GoldRushTree;
    tree_menu_info.menu_depth[3] = depth_3;
    tree_menu_info.menu_depth[2] = depth_2;
    tree_menu_info.menu_depth[1] = depth_1;
    tree_menu_info.menu_depth[0] = depth_0;
    tree_menu_info.string_code = string_code;

    if (0 == string_code)
    {
        strncpy(tree_menu_info.string_name, string, sizeof(tree_menu_info.string_name));
    }

    switch (depth)
    {
    case 0:
        {
            tree_menu_info.menu_type = BASE_TreeMenuInfo::kMenuType_CheckButton;
            tree_menu_info.menu_depth_width = 0;

            tree_menu_info.image_code[BASE_TreeMenuInfo::kIconType_CheckOn] = 10006;
            tree_menu_info.image_code[BASE_TreeMenuInfo::kIconType_CheckOff] = 10005;
        }
        break;
    default:
        {
            tree_menu_info.menu_type = BASE_TreeMenuInfo::kMenuType_None;
        }
        break;
    }

    // user_data 에 맵코드 저장
    tree_menu_info.user_data = map_code;

    TreeMenuInfoParser::Instance()->Insert(tree_menu_info);
}

bool uiChaosZoneGoldRush::SelectIndex( TreeMenuGroup::TreeObject* tree_object )
{
    CControlWZ* control_ptr = GetControlWZ(GetControlID_byPosition(kControl_Button_B005));
    if (control_ptr)
    {
        if (false == tree_object->is_enable)
        {
            control_ptr->EnableWindowWZ(false);
        }
        else
        {
            control_ptr->EnableWindowWZ(true);
        }
    }

    TreeMenuGroup::TreeObject* find_tree_object_ptr = tree_menu().Find(select_index_.menu_index);
    // 이전에 선택했던 것은 체크 끄기
    if (find_tree_object_ptr)
    {
        find_tree_object_ptr->is_check = false;
    }
    // 선택한것은 항상 체크
    select_index_.menu_index = tree_object->info->menu_index;
    if (tree_object)
    {
        tree_object->is_check = true;        
        ChaosSystem::GetGoldRushData().set_select_mapcode(tree_object->info->user_data);
    }

    return true;
}

void uiChaosZoneGoldRush::InitGoldRushInfo( const ChaosZoneTreeInfo* chaoszone_info )
{
    // 골드러쉬 설명
    ftext_render_.ClearSentence();

    FTextParser parser;
    FTextSentence sentence;
    FTextSentence temp_sentence;

    TCHAR temp_string[INTERFACE_STRING_LENGTH] = " ";
    TCHAR str_format[INTERFACE_STRING_LENGTH] = {0, };
    BOOL result = false;

    // 빈줄
    parser.Parser(temp_string, temp_sentence);

    // 이름
    g_InterfaceManager.GetInterfaceString(chaoszone_info->subtitle_string_code, temp_string, INTERFACE_STRING_LENGTH);
    result = parser.Parser(temp_string, sentence);
    if (TRUE == result)
    {
        ftext_render_.Append(sentence);
    }

    // 설명
    if (chaoszone_info->tooltip_string_code != 0)
    {
        // 스트링에 # 대신 <RETRUN> 으로 넣어주세요
        g_InterfaceManager.GetInterfaceString(chaoszone_info->tooltip_string_code, temp_string, INTERFACE_STRING_LENGTH);
        result = parser.Parser(temp_string, sentence);
        if (TRUE == result)
        {
            ftext_render_.Append(temp_sentence); // 빈줄
            ftext_render_.Append(sentence);
        }
    }

    // 입장가능 레벨
    const sMAPINFO* map_info_ptr = MapInfoParser::Instance()->FindMapInfo(static_cast<MAPCODE>(chaoszone_info->map_code));
    if (map_info_ptr)
    {
        // 1780 입장레벨
        g_InterfaceManager.GetInterfaceString(1780, temp_string, INTERFACE_STRING_LENGTH);
        sprintf(str_format, "%s: %d~%d", temp_string, map_info_ptr->minLV, map_info_ptr->maxLV);
        result = parser.Parser(str_format, sentence);
        if (TRUE == result)
        {
            ftext_render_.Append(temp_sentence); // 빈줄
            ftext_render_.Append(sentence);
        }
    }

    const ChaosZoneInfo* zone_info = ChaosZoneInfoParser::Instance()->FindData(static_cast<MAPCODE>(chaoszone_info->map_code));

    //권장 레벨
    // 646	권장레벨
    g_InterfaceManager.GetInterfaceString(646, temp_string, INTERFACE_STRING_LENGTH );
    sprintf(str_format, "%s: %d-%d", temp_string, zone_info->min_level, zone_info->max_level);
    result = parser.Parser(str_format, sentence);
    if (result == TRUE)
    {
        ftext_render_.Append(sentence);
    }

    // 생성에 필요한 아이템
    bool is_have_require_item = false;
    int find_item_number = 0;

    const BASE_ITEMINFO* item_info = NULL;
    WzColor find_color = WzColor_RGBA(0,0,0,0);

    for (int i = 0; i < MAX_REQUIRE_ITEM; ++i)
    {
        if (zone_info->require_item[i] != 0)
        {
            item_info = ItemInfoParser::Instance()->GetItemInfo(zone_info->require_item[i]);
            if (item_info)
            {
                //아이템 찾기
                SCItemSlot* item_slot = ItemManager::Instance()->FindFirstItemSlot(SI_INVENTORY, static_cast<SLOTCODE>(zone_info->require_item[i]));
                if (item_slot && item_slot->GetNum() > 0)
                {
                    //nop
                    is_have_require_item = true;
                    find_item_number = i;
                    break;
                }
            }
        }
    }

    if (item_info)
    {
        if (is_have_require_item == false)
        {
            find_color = WzColor_RGBA(255,0,0,255);
        }

        //2488 생성에 필요한 아이템
        g_InterfaceManager.GetInterfaceString(2488, temp_string, INTERFACE_STRING_LENGTH);
        TCHAR item_name[INTERFACE_STRING_LENGTH]={0,};
        g_InterfaceManager.GetItemName(zone_info->require_item[find_item_number], item_name, INTERFACE_STRING_LENGTH);
        sprintf(str_format, "%s %s", temp_string, item_name);
        
        result = parser.Parser(temp_string, sentence);
        if (result == TRUE)
        {
            ftext_render_.Append(temp_sentence); //빈줄
            ftext_render_.Append(sentence);
        }

        // 이미지 출력은 현재 미구현 (FText에 이미지 삽입 미구현이므로 일단 생략)
        //RegisterTooltip(str_format, -1, iNameLineHeight, FontID_ItemName, green, TOOLTIP_BG_WZCOLOR, 0);

        /*RegisterTooltipImage(item_info->m_wVICode, kTexture_Rectangle_SocketItem,
            textures_rectangles_[kTexture_Rectangle_SocketEmpty].right,
            textures_rectangles_[kTexture_Rectangle_SocketEmpty].bottom,
            (c_dwTOOLTIP_EXTRA_INFO_DONOT_CHANGE_LINE | c_dwTOOLTIP_EXTRA_INFO_HANDLE_IMAGE),
            0.0f, textures_rectangles_[kTexture_Rectangle_SocketEmpty].top - 3.0f, find_color);*/

        //RegisterTooltip(szMessage, -1, iNameLineHeight, FontID_ItemName, green, TOOLTIP_BG_WZCOLOR, c_dwTOOLTIP_EXTRA_INFO_DONOT_CHANGE_LINE);
#ifdef _DEV_VER
  //#ifdef _SCJ_SHOW_ITEM_ID
        for (int i = 0; i < MAX_REQUIRE_ITEM; ++i)
        {
            if (zone_info->require_item[i] != 0)
            {
                item_info = ItemInfoParser::Instance()->GetItemInfo(zone_info->require_item[i]);
                if (item_info)
                {
                    if (CGeneralGameParam::Instance()->IsShowItemId())
                    {
                        Snprintf(temp_string, INTERFACE_STRING_LENGTH-1, _T("<ITEMID:%ld>"), item_info->m_Code);
                        result = parser.Parser(temp_string, sentence);
                        if (result == TRUE)
                        {
                            ftext_render_.Append(temp_sentence); //빈줄
                            ftext_render_.Append(sentence);
                        }
                    }
                }
            }
        }
  //#endif //_SCJ_SHOW_ITEM_ID
#endif //#ifdef _DEV_VER
    }

    ftext_render_.convertLineInfo();
    int scroll_range = ftext_render_.GetMaxLine() - ftext_render_.GetDrawLineInRect();
    scroll_control_ptr_->SetScrollRangeWZ(0,scroll_range);
    scroll_control_ptr_->SetScrollPosWZ(0);

    
    // 보상 슬롯 갱신
    DeleteAllSlot();
    const BattleGroundInfo* battle_ground_info =
        BattleGroundInfoParser::Instance()->FindData(static_cast<MAPCODE>(chaoszone_info->map_code));
    if (battle_ground_info)
    {
        // 승자 보상
        MissionRewardInfo * mission_reward_info = 
            MissionRewardParser::Instance()->GetMissionReward((BYTE)battle_ground_info->win_item_code);
        if (mission_reward_info)
        {
            int hero_class = static_cast<int>(g_pHero->GetClass());
            int reward_code = mission_reward_info->CLASSs[hero_class].REWARDs[0].m_wRewardCode;

            RewardInfo* reward_info = RewardInfoList::Instance()->FindRewardInfo(reward_code);
            if (reward_info)
            {
                for (int i = 0; i < RewardInfo::MAX_DROP_INDEX_NUM; ++i)
                {
                    if (0 == reward_info->GetTypeCode(i))
                    {
                        continue;
                    }

                    InsertSlot(kControl_Picture_P024 + i, reward_info->GetTypeCode(i), reward_info->GetTypeIndex(i));
                }
            }
        }

        // 기본 보상
        mission_reward_info = 
            MissionRewardParser::Instance()->GetMissionReward((BYTE)battle_ground_info->base_item_code);
        if (mission_reward_info)
        {
            int hero_class = static_cast<int>(g_pHero->GetClass());
            int reward_code = mission_reward_info->CLASSs[hero_class].REWARDs[0].m_wRewardCode;

            RewardInfo* reward_info = RewardInfoList::Instance()->FindRewardInfo(reward_code);
            if (reward_info)
            {
                for (int i = 0; i < RewardInfo::MAX_DROP_INDEX_NUM; ++i)
                {
                    if (0 == reward_info->GetTypeCode(i))
                    {
                        continue;
                    }

                    InsertSlot(kControl_Picture_P029 + i, reward_info->GetTypeCode(i), reward_info->GetTypeIndex(i));
                }
            }
        }

    }
}

RC::eSLOT_INSERT_RESULT uiChaosZoneGoldRush::InsertSlot( POSTYPE at_pos, DWORD type_code, DWORD type_index )
{
    DeleteSlot(at_pos,NULL);

    BASE_ITEMINFO* item_info = ItemInfoParser::Instance()->GetItemInfo(type_code);
    if (item_info == NULL)
    {
        return RC_INSERT_SLOT_FAILED;
    }

    static DWORD SERIAL = 10000000;
    SCItemSlot item_slot;
    item_slot.SetSlotType(ST_ITEM);
    item_slot.SetCode(static_cast<SLOTCODE>(type_code));
    item_slot.SetSerial(++SERIAL);

    if (type_index >= RewardInfo::MIN_ITEM_TYPECODE)
    {
        // 장비/펫의 아이템타입/커스터마이징 아이템 처리
        const ItemType* item_type_info = ItemTypeList::Instance()->FindItemType(type_index);
        {
            KindOfItemType::value_type kind_of_item = item_type_info->GetItemType();

            nsSlot::ItemTypeChanger::ChangeItemByType(&item_slot, kind_of_item);
        }
        item_slot.SetEnchant( item_type_info->GetEnchant() );
        item_slot.SetNum(1);
    }
    else
    {
        item_slot.SetNum(static_cast<DURATYPE>(type_index));
    }

    // 슬롯에 넣음
    eSLOT_INSERT_RESULT slot_insert_result = SCSlotContainer::InsertSlot(at_pos, item_slot);

    CControlWZ* control_ptr = GetControlWZ(GetControlID_byPosition(at_pos));

    if(control_ptr)
    {
        RECT rect = control_ptr->GetSizeRect();

        SCSlot& real_slot = GetSlot(at_pos);

        if (item_unit_renderer_)
        {
            item_unit_renderer_->AddItemUnit(item_slot.GetSerial(), item_slot.GetCode(), &rect, &real_slot, control_ptr);
        }
        else
        {
            assert(item_unit_renderer_);
        }
        ReFreshRendereSlot();
    }

    return slot_insert_result;
}

BOOL uiChaosZoneGoldRush::DeleteSlot( POSTYPE at_pos, SCSlot* slot_out )
{
    if (item_unit_renderer_)
    {
        item_unit_renderer_->RemoveItemUnit(GetSlot(at_pos).GetSerial());
    }

    SCSlotContainer::DeleteSlot(at_pos, slot_out);

    ReFreshRendereSlot();
    return TRUE;    
}

void uiChaosZoneGoldRush::DeleteAllSlot()
{
    if (item_unit_renderer_ == NULL)
    {
        assert(item_unit_renderer_);
        return;
    }
    for (int slot_pos = kControl_Picture_P024; slot_pos <= kControl_Picture_P033; ++slot_pos)
    {
        this->DeleteSlot(slot_pos, NULL);
    }
}

void uiChaosZoneGoldRush::ReFreshRendereSlot()
{
    ItemUnitRenderer::RenderUnit* item_unit_render = NULL;
    RECT slot_rect;
    CControlWZ* control_ptr = NULL;

    for (int i = kControl_Picture_P024; i <= kControl_Picture_P033; ++i)
    {
        SCSlot& rSlot = GetSlot(i);
        if (IsEmpty(i))
        {
            continue;
        }

        // 보여줘야할 목록
        if(item_unit_renderer_ == NULL)
        {
            continue;
        }
        item_unit_render = item_unit_renderer_->GetItemUnit(rSlot.GetSerial());

        control_ptr = GetControlWZ(GetControlID_byPosition(i));
        if (control_ptr == NULL)
        {
            continue;
        }

        slot_rect = control_ptr->GetSizeRect();
        if (item_unit_render == NULL)
        {
            item_unit_render = item_unit_renderer_->AddItemUnit(rSlot.GetSerial(), 
                rSlot.GetCode(), &slot_rect, &rSlot ,control_ptr);
        }
        else
        {
            item_unit_renderer_->ChangePositionItemUnit(rSlot.GetSerial(), &slot_rect, control_ptr);
        }
    }
}
//------------------------------------------------------------------------------
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH