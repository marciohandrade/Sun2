#include "SunClientPrecompiledHeader.h"

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#include "uiChaosZoneSSQ.h"
#include <ssq/SSQInfoStruct.h>
#include <ssq/SSQInfoParser.h>
#include "ItemUnitRenderer.h"
#include "ItemInfoParser.h"
#include "ItemManager.h"
#include "ChaosSystem/ChaosSystemMan.h"
#include "GlobalFunc.h"
#include "uiToolTipMan/uiToolTipMan.h"
#include "Mouse.h"
#include "ChaosSystem/ChaosZoneTreeParser.h"
#include "InstanceDungeonScene.h"
#include "Hero.h"
#include "ItemTypeList.h"
#include "RewardInfoList.h"

const DWORD uiChaosZoneSSQ::kDialog_ID = StrToWzID("0643");

uiChaosZoneSSQ::uiChaosZoneSSQ(InterfaceManager* ui_manager) :
item_unit_renderer_(NULL)
{
    RegisterControlPosition(StrToWzID("P021"), kControl_Picture_P021);
    RegisterControlPosition(StrToWzID("P023"), kControl_Picture_P023);
    RegisterControlPosition(StrToWzID("P024"), kControl_Picture_P024);
    RegisterControlPosition(StrToWzID("P025"), kControl_Picture_P025);

    RegisterControlPosition(StrToWzID("B999"), kControl_Button_B999);
    RegisterControlPosition(StrToWzID("T999"), kControl_Text_T999);
    RegisterControlPosition(StrToWzID("B005"), kControl_Button_B005);
    RegisterControlPosition(StrToWzID("S000"), kControl_Text_S000);
    RegisterControlPosition(StrToWzID("P001"), kControl_Picture_P001);
    RegisterControlPosition(StrToWzID("P005"), kControl_Picture_P005);
    RegisterControlPosition(StrToWzID("P019"), kControl_Picture_P019);
    RegisterControlPosition(StrToWzID("L000"), kControl_List_L000);
    RegisterControlPosition(StrToWzID("S004"), kControl_Text_S004);
    RegisterControlPosition(StrToWzID("S001"), kControl_Text_S001);
    RegisterControlPosition(StrToWzID("S002"), kControl_Text_S002);
    RegisterControlPosition(StrToWzID("S005"), kControl_Text_S005);
    RegisterControlPosition(StrToWzID("S003"), kControl_Text_S003);
    RegisterControlPosition(StrToWzID("S006"), kControl_Text_S006);
    RegisterControlPosition(StrToWzID("P008"), kControl_Picture_P008);
    RegisterControlPosition(StrToWzID("P009"), kControl_Picture_P009);
    RegisterControlPosition(StrToWzID("P011"), kControl_Picture_P011);
    RegisterControlPosition(StrToWzID("P012"), kControl_Picture_P012);
    RegisterControlPosition(StrToWzID("P013"), kControl_Picture_P013);
    RegisterControlPosition(StrToWzID("P014"), kControl_Picture_P014);
    RegisterControlPosition(StrToWzID("B000"), kControl_Button_B000);
    RegisterControlPosition(StrToWzID("B001"), kControl_Button_B001);
    RegisterControlPosition(StrToWzID("B002"), kControl_Button_B002);
    RegisterControlPosition(StrToWzID("B003"), kControl_Button_B003);
    RegisterControlPosition(StrToWzID("B004"), kControl_Button_B004);
    RegisterControlPosition(StrToWzID("B007"), kControl_Button_B007);

    loaded_texture_ = false;
    number_texture_ = INVALID_HANDLE_VALUE;
}
//------------------------------------------------------------------------------
uiChaosZoneSSQ::~uiChaosZoneSSQ()
{
}
//------------------------------------------------------------------------------
void uiChaosZoneSSQ::Init(CDrawBase* draw_base)
{
    draw_base_ = draw_base;

    //슬롯초기화
    SCItemSlotContainer::Init(kSlotTotalCount, SI_CHAOSZONE_SSQ);

    // 랜더러 초기화
    assert(!item_unit_renderer_);
    item_unit_renderer_ = new ItemUnitRenderer;
    item_unit_renderer_->Init(uiBase::GetDrawBase(), kSlotTotalCount);

    //보상 슬롯
    InitRewardItemSlot();


    //버튼 비활성화
    CCtrlButtonCheckWZ* check_control = NULL;
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
#else
    check_control = static_cast<CCtrlButtonCheckWZ*>(GetControlWZ(GetControlID_byPosition(kControl_Button_B001)));
    if (check_control)
    {
        check_control->EnableWindowWZ(false);
    }
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
    check_control = static_cast<CCtrlButtonCheckWZ*>(GetControlWZ(GetControlID_byPosition(kControl_Button_B002)));
    if (check_control)
    {
        check_control->EnableWindowWZ(false);
    }
#ifdef _NA_006088_20120910_CHAOS_ZONE_FREE_PVP
#else
    check_control = static_cast<CCtrlButtonCheckWZ*>(GetControlWZ(GetControlID_byPosition(kControl_Button_B004)));
    if (check_control)
    {
        check_control->EnableWindowWZ(false);
    }
#endif //_NA_006088_20120910_CHAOS_ZONE_FREE_PVP

    check_control = static_cast<CCtrlButtonCheckWZ*>(GetControlWZ(GetControlID_byPosition(kControl_Button_B005)));
    if (check_control)
    {
        // 2590 입장하기
        check_control->SetTextID(2590);
    }

    //이미지 비활성화
    CCtrlPictureWZ* picture_control;
    picture_control = static_cast<CCtrlPictureWZ*>(GetControlWZ(GetControlID_byPosition(kControl_Picture_P001)));
    if (picture_control)
    {
        picture_control->ShowWindowWZ(WZ_HIDE);
    }
    picture_control = static_cast<CCtrlPictureWZ*>(GetControlWZ(GetControlID_byPosition(kControl_Picture_P005)));
    if (picture_control)
    {
        picture_control->ShowWindowWZ(WZ_HIDE);
    }

    CControlWZ* control_pointer = static_cast<CCtrlStaticWZ*>(GetControlWZ(GetControlID_byPosition(kControl_Text_S002)));
    if (control_pointer)
    {
        // 2715 우승
        control_pointer->SetTextID(2715);
    }
    control_pointer = static_cast<CCtrlStaticWZ*>(GetControlWZ(GetControlID_byPosition(kControl_Text_S005)));
    if (control_pointer)
    {
        // 2716 2등
        control_pointer->SetTextID(2716);
    }
    control_pointer = static_cast<CCtrlStaticWZ*>(GetControlWZ(GetControlID_byPosition(kControl_Text_S003)));
    if (control_pointer)
    {
        // 2717 3등
        control_pointer->SetTextID(2717);
    }
    control_pointer = static_cast<CCtrlStaticWZ*>(GetControlWZ(GetControlID_byPosition(kControl_Text_S006)));
    if (control_pointer)
    {
        // 2718 기본
        control_pointer->SetTextID(2718);
    }

    //남은 입장 시작/종료 시간
    control_pointer = static_cast<CCtrlStaticWZ*>(GetControlWZ(GetControlID_byPosition(kControl_Text_S004)));
    if (control_pointer)
    {
        // 2586  남은 입장 시작 시간
        TCHAR temp_string[INTERFACE_STRING_LENGTH];
        g_InterfaceManager.GetInterfaceString(2586, temp_string);
        control_pointer->SetTextWZ(temp_string);
    }

    // 시간 관련 초기화
    ssq_current_state = SSQTIMEINFO::eTM::eTM_None;
    ZeroMemory(&ssq_system_time, sizeof(SYSTEMTIME));

    // 숫자 이미지 로드
    if (loaded_texture_ == false)
    {
        number_texture_ = g_pSunRenderer->x_pManagerTexture->LoadTexture(
            "Data\\Interface\\64_chaos_Zone.tga",
            TLS_ORIGINAL_SIZE | TLS_NO_MIPMAP | TLS_NO_MULTITHREAD);
        loaded_texture_ = true;
    }
}
//-----------------------------------------------------------------------------
void uiChaosZoneSSQ::Release()
{
    if( number_texture_ != INVALID_HANDLE_VALUE )
    {
        if(draw_base_ && draw_base_->x_pManagerTexture)
        {
            draw_base_->x_pManagerTexture->DestroyTexture(number_texture_);
        }
        loaded_texture_ = false;
        number_texture_ = INVALID_HANDLE_VALUE;
    }
}
//------------------------------------------------------------------------------
void uiChaosZoneSSQ::NetworkProc(struct MSG_BASE *message)
{
}
//------------------------------------------------------------------------------
void uiChaosZoneSSQ::OnUpdateSolarDialog()
{
    UpdateSSQInfo();
}
//------------------------------------------------------------------------------
void uiChaosZoneSSQ::OnRenderSolarDialog()
{
    if (item_unit_renderer_)
    {
        item_unit_renderer_->Render();
    }

    RenderSSQTime();
    RenderTooltip();

    if (this->m_MouseOverObject.eMouseOverType != eMOUSE_OVER_MAX)
    {
        if (eMOUSE_OVER_SLOT == m_MouseOverObject.eMouseOverType)
        {
            uiToolTipMan* tooltip_manager = (uiToolTipMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_TOOLTIP);
            if (tooltip_manager)
            {
                tooltip_manager->RegisterSlotInfoTooltip(m_MouseOverObject.pSlot);
                tooltip_manager->SetTooltipDialogID(GetDialogKey());
            }
        }
    }
}
void uiChaosZoneSSQ::OnShowWindow(BOOL is_show)
{
    if (is_show)
    {
        //체크버튼
        CCtrlButtonCheckWZ* check_control = NULL;
        for (int i = kControl_Button_B000;i <= kControl_Button_B007; ++i)
        {
            check_control = static_cast<CCtrlButtonCheckWZ*>(GetControlWZ(GetControlID_byPosition(i)));
            if (check_control)
            {
                check_control->SetCheckState(false);
            }
        }

        check_control = static_cast<CCtrlButtonCheckWZ*>(GetControlWZ(GetControlID_byPosition(kControl_Button_B003)));
        if (check_control)
        {
            check_control->SetCheckState(true);
        }
    }
}
//------------------------------------------------------------------------------
void uiChaosZoneSSQ::MessageProc(SI_MESSAGE* message)
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
        SolarDialog::MessageProc(message);
        break;
    }
}


//------------------------------------------------------------------------------
bool uiChaosZoneSSQ::OnLButtonClick(SI_MESSAGE* message)
{
    bool result = true;
    int tab_index = -1;

    uiChaosSystemMan* chaos_manager = static_cast<uiChaosSystemMan*>(GetMan());
    if (chaos_manager == NULL)
    {
        return false;
    }

    switch (GetControlPosition_byID(message->dwCtrlID))
    {
    case kControl_Button_B999:
        {
            chaos_manager->ShowChaosZone(uiChaosZoneSSQ::kDialog_ID, false);
        }
        break;
    case kControl_Button_B005:
        {
            //타락한 사원
            MSG_CG_ZONE_EX_SSQ_JOIN_INFO_SYN ssq_send_packet;
            POSTYPE output_position = INVALID_SLOT_POS;
            ssq_send_packet.m_ItemPos = 0;
            GlobalFunc::SendPacket(CI_GAMESERVERIDX, &ssq_send_packet, sizeof(ssq_send_packet));
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
    if (tab_index != -1)
    {
        chaos_manager->OpenChaosZone(tab_index, true);
    }
    return result;
}
//-----------------------------------------------------------------------------
void uiChaosZoneSSQ::UpdateSSQInfo()
{
    int checkTime = (int)(g_InstanceDungeonScene.GetStartCheckTime() + 
        (g_InstanceDungeonScene.GetStartProcessTick() - clock_function::GetTickCount())) / 1000;

    if(checkTime < 0)
    {
        g_InstanceDungeonScene.Send_MSG_CG_ETC_GET_BATTLE_OPEN_TIME_SYN();
        return;
    }

    g_SsqScene.GetSSQTimeInfo(ssq_system_time, ssq_current_state);

    // 남은 입장 시작/종료 시간
    CCtrlStaticWZ* control_pointer = NULL;
    control_pointer = static_cast<CCtrlStaticWZ*>(GetControlWZ(GetControlID_byPosition(kControl_Text_S004)));
    if (control_pointer)
    {
        TCHAR temp_string[INTERFACE_STRING_LENGTH];
        if (ssq_current_state == SSQTIMEINFO::eTM::eTM_EnterableTimeout)
        {
            // 2586  남은 입장 시작 시간
            g_InterfaceManager.GetInterfaceString(2586, temp_string);
            control_pointer->SetTextColor(WzColor_RGBA(255,255,255,255));
        }
        else if (ssq_current_state == SSQTIMEINFO::eTM::eTM_CloseTimeout)
        {
            // 2587  남은 입장 종료 시간
            g_InterfaceManager.GetInterfaceString(2587, temp_string);
            control_pointer->SetTextColor(RGBA(217,117,0,255));
        }
        else
        {
            ZeroMemory(temp_string, sizeof(temp_string));
        }
        control_pointer->SetTextWZ(temp_string);
    }
}

//-----------------------------------------------------------------------------
void uiChaosZoneSSQ::RenderSSQTime()
{
    if (number_texture_ == INVALID_HANDLE_VALUE)
    {
        return;
    }

    // 시간 표시
    WORD ssq_time_hour_ = ssq_system_time.wHour;
    WORD ssq_time_minute_ = ssq_system_time.wMinute;
    WORD ssq_time_second_ = ssq_system_time.wSecond;

    const int PICTURE_SIZE_X = 22;
    const int PICTURE_SIZE_Y = 33;

    float picture_source_height = 446.0f;    
    if (ssq_current_state == SSQTIMEINFO::eTM::eTM_CloseTimeout)
    {
        picture_source_height += PICTURE_SIZE_Y;
    }

    // 현재 시간 출력
    RECT render_position;
    CCtrlPictureWZ* picture_control;

    //if (ssq_time_hour_ > 10)
    {
        picture_control = static_cast<CCtrlPictureWZ*>(GetControlWZ(GetControlID_byPosition(kControl_Picture_P008)));
        if (picture_control)
        {
            render_position = picture_control->GetSize();
            g_pSunRenderer->RenderTexture(number_texture_,
                static_cast<float>(render_position.left),          //DEST(찍을위치)
                static_cast<float>(render_position.top),
                static_cast<float>(render_position.right),
                static_cast<float>(render_position.bottom),
                static_cast<float>(ssq_time_hour_/10 * PICTURE_SIZE_X),	       //SRC
                picture_source_height,
                static_cast<float>(render_position.right),
                static_cast<float>(render_position.bottom));
        }
    }
    //if (ssq_time_hour_ > 0)
    {
        picture_control = static_cast<CCtrlPictureWZ*>(GetControlWZ(GetControlID_byPosition(kControl_Picture_P009)));
        if (picture_control)
        {
            render_position = picture_control->GetSize();
            g_pSunRenderer->RenderTexture(number_texture_,
                static_cast<float>(render_position.left),          //DEST(찍을위치)
                static_cast<float>(render_position.top),
                static_cast<float>(render_position.right),
                static_cast<float>(render_position.bottom),
                static_cast<float>(ssq_time_hour_%10 * PICTURE_SIZE_X),	       //SRC
                picture_source_height,
                static_cast<float>(render_position.right),
                static_cast<float>(render_position.bottom));
        }
    }
    //if (ssq_time_minute_ > 10)
    {
        picture_control = static_cast<CCtrlPictureWZ*>(GetControlWZ(GetControlID_byPosition(kControl_Picture_P011)));
        if (picture_control)
        {
            render_position = picture_control->GetSize();
            g_pSunRenderer->RenderTexture(number_texture_,
                static_cast<float>(render_position.left),          //DEST(찍을위치)
                static_cast<float>(render_position.top),
                static_cast<float>(render_position.right),
                static_cast<float>(render_position.bottom),
                static_cast<float>(ssq_time_minute_/10 * PICTURE_SIZE_X),	       //SRC
                picture_source_height,
                static_cast<float>(render_position.right),
                static_cast<float>(render_position.bottom));
        }
    }
    //if (ssq_time_minute_ > 0)
    {
        picture_control = static_cast<CCtrlPictureWZ*>(GetControlWZ(GetControlID_byPosition(kControl_Picture_P012)));
        if (picture_control)
        {
            render_position = picture_control->GetSize();
            g_pSunRenderer->RenderTexture(number_texture_,
                static_cast<float>(render_position.left),          //DEST(찍을위치)
                static_cast<float>(render_position.top),
                static_cast<float>(render_position.right),
                static_cast<float>(render_position.bottom),
                static_cast<float>(ssq_time_minute_%10 * PICTURE_SIZE_X),	       //SRC
                picture_source_height,
                static_cast<float>(render_position.right),
                static_cast<float>(render_position.bottom));
        }
    }
    //if (ssq_time_second_ > 10)
    {
        picture_control = static_cast<CCtrlPictureWZ*>(GetControlWZ(GetControlID_byPosition(kControl_Picture_P013)));
        if (picture_control)
        {
            render_position = picture_control->GetSize();
            g_pSunRenderer->RenderTexture(number_texture_,
                static_cast<float>(render_position.left),          //DEST(찍을위치)
                static_cast<float>(render_position.top),
                static_cast<float>(render_position.right),
                static_cast<float>(render_position.bottom),
                static_cast<float>(ssq_time_second_/10 * PICTURE_SIZE_X),	       //SRC
                picture_source_height,
                static_cast<float>(render_position.right),
                static_cast<float>(render_position.bottom));
        }
    }
    //if (ssq_time_second_ > 0)
    {
        picture_control = static_cast<CCtrlPictureWZ*>(GetControlWZ(GetControlID_byPosition(kControl_Picture_P014)));
        if (picture_control)
        {
            render_position = picture_control->GetSize();
            g_pSunRenderer->RenderTexture(number_texture_,
                static_cast<float>(render_position.left),          //DEST(찍을위치)
                static_cast<float>(render_position.top),
                static_cast<float>(render_position.right),
                static_cast<float>(render_position.bottom),
                static_cast<float>(ssq_time_second_%10 * PICTURE_SIZE_X),	       //SRC
                picture_source_height,
                static_cast<float>(render_position.right),
                static_cast<float>(render_position.bottom));
        }
    }
}

//-----------------------------------------------------------------------------
void uiChaosZoneSSQ::RenderTooltip()
{
    uiToolTipMan* tooltip_manager = GET_DIALOG_MANAGER(uiToolTipMan, UIMAN_TOOLTIP);
    if (tooltip_manager == NULL)
    {
        return;
    }

    int mouse_x = Mouse::Instance()->GetMouseX();
    int mouse_y = Mouse::Instance()->GetMouseY();

#ifdef _NA_006088_20120910_CHAOS_ZONE_FREE_PVP
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
    int tap_controls[] = {kControl_Button_B000, kControl_Button_B001,0 /*kControl_Button_B002*/,
        kControl_Button_B003, kControl_Button_B004, kControl_Button_B007};
#else
    int tap_controls[] = {kControl_Button_B000,0 /*kControl_Button_B001*/,0 /*kControl_Button_B002*/,
        kControl_Button_B003, kControl_Button_B004, kControl_Button_B007};
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
#else
    int tap_controls[] = {kControl_Button_B000,0 /*kControl_Button_B001*/,0 /*kControl_Button_B002*/,
        kControl_Button_B003,0 /*kControl_Button_B004*/, kControl_Button_B007};
#endif //_NA_006088_20120910_CHAOS_ZONE_FREE_PVP

    CControlWZ* control;
    for (int index = 0; index < sizeof(tap_controls)/sizeof(tap_controls[0]); ++index)
    {
        if (tap_controls[index] == 0)
        {
            continue;
        }
        control = GetControlWZ_byPosition(tap_controls[index]);
        if (control)
        {
            RECT position = control->GetSizeRect();
            if (position.left < mouse_x && position.right > mouse_x &&
                position.top < mouse_y && position.bottom > mouse_y)
            {
                // 2571 ~ 2576 / 이스라필의 대지~ 카오스 히스토리
                tooltip_manager->RegisterTooltipWithStringCode(2571 + index);
				tooltip_manager->SetTooltipDialogID(GetDialogKey());
            }
        }
    }
}
//-----------------------------------------------------------------------------
void uiChaosZoneSSQ::InitRewardItemSlot()
{
    // 보상 슬롯 갱신
    DeleteAllSlot();

    const SSQInfo* ssq_info = SSQInfoParser::Instance()->FindSSQInfo(12001); // 타락한사원 맵코드 : 12001, 현재 ChaosZoneTree 에 타락한사원 맵코드 입력 안됨
    if (ssq_info == NULL)
    {
        return;
    }

    SLOTCODE slot_code = 0;
    DWORD type_index = 0;
    // 승자 ~ 3등 보상
    for (int i = 0; i < SSQInfo::MI_REWARD_WINNER_INFO_SECTION_MAX; ++i)
    {
        int hero_class = g_pHero->GetClass();
        slot_code = ssq_info->m_RewardWinnerInfo[i][hero_class].m_RewardCode;
        type_index = ssq_info->m_RewardWinnerInfo[i][hero_class].m_RewardLimit;
        InsertSlot(kControl_Picture_P021 + i, slot_code, type_index);
    }

    // 기본 보상
    slot_code = ssq_info->m_DefaultRewardInfo.m_RewardCode;
    type_index = ssq_info->m_DefaultRewardInfo.m_RewardLimit;
    InsertSlot(kControl_Picture_P025, slot_code, type_index);

}
//-----------------------------------------------------------------------------
RC::eSLOT_INSERT_RESULT uiChaosZoneSSQ::InsertSlot(POSTYPE at_pos, DWORD type_code, DWORD type_index)
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
            item_unit_renderer_->AddItemUnit(item_slot.GetSerial(), item_slot.GetCode(), &rect, &real_slot,true ,control_ptr);
        }
        else
        {
            assert(item_unit_renderer_);
        }
        ReFreshRenderSlot();
    }

    return slot_insert_result;
}
//------------------------------------------------------------------------------ 
BOOL uiChaosZoneSSQ::DeleteSlot(POSTYPE at_pos, SCSlot* slot_out)
{
    if (item_unit_renderer_)
    {
        item_unit_renderer_->RemoveItemUnit(GetSlot(at_pos).GetSerial());
    }

    SCSlotContainer::DeleteSlot(at_pos, slot_out);

    ReFreshRenderSlot();
    return TRUE;
}
//------------------------------------------------------------------------------
void uiChaosZoneSSQ::DeleteAllSlot()
{
    if (item_unit_renderer_ == NULL)
    {
        assert(item_unit_renderer_);
        return;
    }
    for (int slot_pos = kControl_Picture_P021; slot_pos <= kControl_Picture_P025; ++slot_pos)
    {
        this->DeleteSlot(slot_pos, NULL);
    }
}
//------------------------------------------------------------------------------
void uiChaosZoneSSQ::ReFreshRenderSlot()
{
    ItemUnitRenderer::RenderUnit* item_unit_render = NULL;
    RECT slot_rect;
    CControlWZ* control_ptr = NULL;

    for (int i = kControl_Picture_P021; i <= kControl_Picture_P025; ++i)
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
#endif//_NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL