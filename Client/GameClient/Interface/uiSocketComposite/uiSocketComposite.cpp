#include "SunClientPrecompiledHeader.h"

#include "InterfaceManager.h"
#include "uiSocketMan/uiSocketMan.h"
#include "uiSocketComposite/uiSocketComposite.h"
#include "GlobalFunc.h"
#include "dummyitemSlot.h"
#include "ItemUnitRenderer.h"
#include "ItemManager.h"
#include "Mouse.h"
#include "MouseHandler.h"
#include "SocketSystem/ZardComposeInfoParser.h"
#include "Hero.h"
#include "uiToolTipMan/uiToolTipMan.h"
#include "Application.h"
#include "../SoundEffect.h"

class uiSCItemSlot;
const DWORD IM_SOCKET_MANAGER::SOCKET_COMPOSITE = StrToWzID("7111");

const int kMAX_GAGUE_WIDTH = 338;
const int kMAX_GAGUE_WIDTH_DIVIDE_2 = kMAX_GAGUE_WIDTH/2;
const int kMAX_GAGUE_WIDTH_MULTIPLY_4 = kMAX_GAGUE_WIDTH * 4;
const float kMAX_PER_1_WIDTH = ((float)kMAX_GAGUE_WIDTH/ZardComposeInfo::kMaxRatio);
const int kBASE_X = 28;
const int kBASE_Y = 215;
const float kBASE_STICK_X = kBASE_X - 6.5f;
const int kSTICK_BASE_Y = 207;
const int kEFFECT_BASE_Y = 197;
const int kEFFECT_BASE_X = -45;

#ifdef _INTERNATIONAL_UI
    const WzID c_wiListFont2 = StrToWzID("st10");
#else
    const WzID c_wiListFont2 = StrToWzID("m212");
#endif//_INTERNATIONAL_UI

WzID uiSocketComposite::wz_control_id_[DIALOG_SOCKET_COMPOSITE_CONTROLS_MAX] = 
{
    StrToWzID("B003"),  //재료1         DIALOG_SOCKET_COMPOSITE_RESOURCE1
    StrToWzID("B000"),  //재료2         DIALOG_SOCKET_COMPOSITE_RESOURCE2
    StrToWzID("B002"),  //재료3         DIALOG_SOCKET_COMPOSITE_RESOURCE3
    StrToWzID("B004"),  //첨가제        DIALOG_SOCKET_COMPOSITE_ADD_RESOURCE
    StrToWzID("B005"),  //결과물        DIALOG_SOCKET_COMPOSITE_RESULT_ITEM
    StrToWzID("L001"),  //리스트        DIALOG_SOCKET_COMPOSITE_LIST
    StrToWzID("B999"),  //종료          DIALOG_SOCKET_COMPOSITE_EXIT
    StrToWzID("T999"),  //타이틀        DIALOG_SOCKET_COMPOSITE_TITTLE
    StrToWzID("BT00"),  //합성시작      DIALOG_SOCKET_COMPOSITE_START
    StrToWzID("B001"),  //취소          DIALOG_SOCKET_COMPOSITE_CANCLE
    StrToWzID("P001"),  //성공구간배경  DIALOG_SOCKET_COMPOSITE_SUCCESS_AREA_BACKGROUND
    StrToWzID("P002"),  //이동막대      DIALOG_SOCKET_COMPOSITE_MOVE_STICK
    StrToWzID("P003"),  //이펙트1       DIALOG_SOCKET_COMPOSITE_EFFECT_1
    StrToWzID("P004"),  //이펙트2       DIALOG_SOCKET_COMPOSITE_EFFECT_2
    StrToWzID("P005"),  //이펙트3       DIALOG_SOCKET_COMPO+SITE_EFFECT_3
    StrToWzID("P006"),  //이펙트4       DIALOG_SOCKET_COMPOSITE_EFFECT_4
};

uiSocketComposite::uiSocketComposite(InterfaceManager* ui_manager) : uiBase(ui_manager),
item_unit_renderer_(NULL)
{
}

uiSocketComposite::~uiSocketComposite()
{
    Release();
}

//------------------------------------------------------------------------------
/**
*/
void uiSocketComposite::Init(CDrawBase* draw_base)
{
	uiBase::Init(draw_base);	

	socket_composite_list_ = (CCtrlListWZ*) get_control(DIALOG_SOCKET_COMPOSITE_LIST);  
	assert(socket_composite_list_);

    int index = 0;
    for(index = RESOURCE1_BUTTON; index < SOCKET_COMPOSITE_BUTTONS_MAX; ++index)
    {
        socket_composite_button_[index] = 
            (CCtrlButtonWZ*)get_control(DIALOG_SOCKET_COMPOSITE_RESOURCE1+index);
        assert(socket_composite_button_[index]);
    }

    for(index = SUCCESS_AREA_BACKGROUND; index < SOCKET_COMPOSITE_PICTURES_MAX; ++index)
    {
        socket_composite_picture_[SUCCESS_AREA_BACKGROUND+index] =
            (CCtrlPictureWZ*)get_control(DIALOG_SOCKET_COMPOSITE_SUCCESS_AREA_BACKGROUND+index);
        assert(socket_composite_picture_[index]);

        if (index >= EFFECT_1 && socket_composite_picture_[index])
        {
           socket_composite_picture_[index]->ShowWindowWZ(WZ_HIDE);
        }
    }

    // 슬롯 초기화
    DummySlotContainer::Init(SOCKET_COMPOSITE_SLOT_RESULT_MAX);   

    // 랜더러 초기화
    assert(!item_unit_renderer_);
    item_unit_renderer_ = new ItemUnitRenderer;
    item_unit_renderer_->Init(g_pSunRenderer, SCSlotContainer::GetMaxSlotNum());

    for (int i = 0; i < SOCKET_COMPOSITE_SLOT_RESOURCE_MAX; ++i)
    {
        item_inventory_position_[i] = INVALID_POSTYPE_VALUE;
    }
    additional_item_position_ = INVALID_POSTYPE_VALUE;
    fail_number_ = 0;
    SetGague(0);

    // 애니메이션 시간 지정
    animate_frame_count_ = 0;
    success_sound_count_ = 0;
    fail_sound_count_ = 0;
    //animate_total_frame_ = kMax_End_Count + 1;
    socket_composite_lock_ = false;
    save_inventory_item_info.Clear();
    result_string_code_ = 0;
    socket_composite_success_ = false;
    move_gap_x_ = kMAX_GAGUE_WIDTH/2;
    goal_position_ = 0.0f;
    animate_start_tick_ = 0;
    animate_end_tick_ = 0;
    animate_tick_gap_ = 0;

    for (int i = SOCKET_COMPOSITE_INFO_STRING1; i < INFORMATION_MAX; ++i)
    {
        Sprintf(message_[i], _T(""));
    }
}
//------------------------------------------------------------------------------
void uiSocketComposite::ResetAnimation()
{
    if (socket_composite_success_ == true)
    {
        socket_composite_success_ = false;
        ItemManager::Instance()->InsertInventoryTotalInfo(SOCKET_COMPOSTE, save_inventory_item_info);
        // 성공 사운드
        g_SoundEffect.PlayUI(UI_SOUND_ZARD_SUCCESS);
    }
    else
    {
        if (result_string_code_ != 0)
        {
            TCHAR message_buffer[INTERFACE_STRING_LENGTH] = {0,};
            g_InterfaceManager.GetInterfaceString(result_string_code_, message_buffer, INTERFACE_STRING_LENGTH);
            SetMessage(SOCKET_COMPOSITE_INFO_STRING3, message_buffer);
            // 실패 사운드
            g_SoundEffect.PlayUI(UI_SOUND_ZARD_FAIL);
        }
    }
}
//------------------------------------------------------------------------------
/**
*/
void uiSocketComposite::Release()
{
    SAFE_RELEASENDELETE(item_unit_renderer_);
}
//------------------------------------------------------------------------------
/**
*/
void uiSocketComposite::OnRenderSolarDialog()
{
    animate_now_tick_ = GetTickCount();

    if (!item_unit_renderer_)
    {
        return;
    }

    if (item_unit_renderer_)
    {
        item_unit_renderer_->Render();
    }

    if (animate_start_tick_ == 0)//이펙트 중에는 툴팁 표시 안함
    {
        if (this->m_MouseOverObject.eMouseOverType != eMOUSE_OVER_MAX &&
            m_MouseOverObject.eMouseOverType == eMOUSE_OVER_SLOT)
        {
            if (m_MouseOverObject.pSlot && m_MouseOverObject.pSlot->GetCode())
            {
                uiToolTipMan* tooltip_manager = 
                    (uiToolTipMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_TOOLTIP);
                if (tooltip_manager != NULL)
                {
                    tooltip_manager->RegisterTooltipItem(m_MouseOverObject.pSlot, FALSE);
					tooltip_manager->SetTooltipDialogID(GetDialogKey());

                }
            }
        }
    }
    int iMouseX = Mouse::Instance()->GetMouseX();
    int iMouseY = Mouse::Instance()->GetMouseY();

    // 스킵처리 
    // 인터페이스에서 Dialog 바탕에서는 Mouse Over가 오지 않는다.
    BOOL bMouseOver = FALSE;
    for (POSTYPE pos = SOCKET_COMPOSITE_SLOT_RESOURCE1; pos < SOCKET_COMPOSITE_SLOT_RESULT_MAX; ++pos)
    {
        CControlWZ* pControl = get_control(pos);

        RECT rc = pControl->GetSizeRect();

        if (rc.left <= iMouseX && rc.right >= iMouseX)
        {
            if (rc.top <= iMouseY && rc.bottom >= iMouseY)
            {
                bMouseOver = TRUE;
            }
        }
    }

    if (!bMouseOver)
    {
        m_MouseOverObject.eMouseOverType = eMOUSE_OVER_MAX;
    }
    
    if (animate_now_tick_ <= animate_end_tick_)
    {
        if (animate_tick_gap_ == 0)
        {
            animate_tick_gap_ = animate_now_tick_;
        }

        RECT rc;
        POINT pt = g_pApplication->Get2DModeMapping();
        SetRect(&rc, 0, 0, pt.x, pt.y);
        g_pSunRenderer->ColorFill(WzColor_RGBA(0,0,0,128), 0, 0, static_cast<float>(rc.right), static_cast<float>(rc.bottom));
        GetDialogWZ()->OnDraw(true);

        DWORD sub_animate_tick = animate_now_tick_ - animate_tick_gap_;
        if (sub_animate_tick >= 50)
        {
            int animate_count = (int)(sub_animate_tick/50.0f);
            
            for (int i = 0; i < animate_count; ++i)
            {
                AnimateEffect();
            }
            animate_tick_gap_ = animate_now_tick_;
        }
    }
    else
    {
        if (animate_start_tick_ != 0)
        {
            AnimateEffect();
            animate_start_tick_ = 0;
            socket_composite_lock_ = false;

            keyMsg msg;
            ZeroMemory(&msg,sizeof(msg));
            msg.dwType=eDoType_dialog_SocketComposite;
            msg.wParam=IM_SOCKET_MANAGER::SOCKET_COMPOSITE;
            msg.DoSomething=GlobalFunc::DoShowWindowEx;
            g_KeyQueueManager.PushBack(msg);

            TCHAR string[INTERFACE_STRING_LENGTH] = {0,};        
            //g_KeyQueueManager.DeleteMsg(eDoType_dialog_SocketComposite);

            animate_frame_count_ = 0;
            animate_tick_gap_ = 0;
            //animate_total_frame_ = kMax_End_Count + 1;
            save_inventory_item_info.Clear();
            DeleteResourceSlot();

            RECT dialog_rect;
            GetDialogWZ()->GetClientRect(&dialog_rect);
            RECT control_size = control_size = socket_composite_picture_[MOVE_STICK]->GetSize();

            /*float position_x = ((float)dialog_rect.left + kBASE_STICK_X + kMAX_GAGUE_WIDTH/2);
            MoveStick(dialog_rect, control_size, position_x);*/
            
            /*g_InterfaceManager.GetInterfaceString(2190, string);
            SetMessage(SOCKET_COMPOSITE_INFO_STRING1, string);
            SetMessage(SOCKET_COMPOSITE_INFO_STRING2, "");
            SetGague(0);*/
        }
    }
    /*else if (animate_total_frame_ >= kMax_Animate_Count && animate_total_frame_ < kMax_End_Count)
    {
        ++animate_frame_count_;
        ++animate_total_frame_;
    }
    else if (animate_total_frame_ == kMax_End_Count)
    {
        socket_composite_lock_ = false;

        keyMsg msg;
        ZeroMemory(&msg,sizeof(msg));
        msg.dwType=eDoType_dialog_SocketComposite;
        msg.wParam=IM_SOCKET_MANAGER::SOCKET_COMPOSITE;
        msg.DoSomething=GlobalFunc::DoShowWindowEx;
        g_KeyQueueManager.PushBack(msg);

        TCHAR string[INTERFACE_STRING_LENGTH] = {0,};        
        g_KeyQueueManager.DeleteMsg(eDoType_dialog_SocketComposite);

        animate_frame_count_ = 0;
        animate_total_frame_ = kMax_End_Count + 1;
        save_inventory_item_info.Clear();
        DeleteAllSlot();

        RECT dialog_rect;
        GetDialogWZ()->GetClientRect(&dialog_rect);
        RECT control_size = control_size = socket_composite_picture_[MOVE_STICK]->GetSize();

        socket_composite_picture_[MOVE_STICK]->MoveWindowWZ(
            (float)dialog_rect.left + kBASE_STICK_X + kMAX_GAGUE_WIDTH/2, 
            (float)dialog_rect.top + kSTICK_BASE_Y,
            (float)control_size.right, (float)control_size.bottom);

        g_InterfaceManager.GetInterfaceString(2190, string);
        SetMessage(SOCKET_COMPOSITE_INFO_STRING1, string);
        SetMessage(SOCKET_COMPOSITE_INFO_STRING2, "");
        SetGague(0);
    }*/
}
//------------------------------------------------------------------------------
/**
*/
void uiSocketComposite::Process(DWORD tick_count)
{
}
//------------------------------------------------------------------------------
/**
*/
void uiSocketComposite::OnShowWindow(BOOL open_value)
{
    SolarDialog::OnShowWindow(open_value);
	if(open_value)
	{
        socket_composite_lock_ = false; 
        g_InterfaceManager.ShowWindow(InterfaceManager::DIALOG_EQUIP, WZ_SHOW);
        keyMsg msg;
        ZeroMemory(&msg,sizeof(msg));
        msg.dwType=eDoType_dialog_SocketComposite;
        msg.wParam=IM_SOCKET_MANAGER::SOCKET_COMPOSITE;
        msg.DoSomething=GlobalFunc::DoShowWindowEx;
        g_KeyQueueManager.PushBack(msg);

        TCHAR string[INTERFACE_STRING_LENGTH] = {0,};
        // 2190 합성할 쟈드를 등록하세요.
        g_InterfaceManager.GetInterfaceString(2190, string);
        SetMessage(SOCKET_COMPOSITE_INFO_STRING1, string);
        ControlsStateInit();
        if (g_pHero)
        {
            g_pHero->SetBeHaveState(PLAYER_BEHAVE_SOCKET_COMPOSITE_STATE);
        }
	}
	else
	{
        g_InterfaceManager.ShowWindow(InterfaceManager::DIALOG_EQUIP, FALSE);
        g_KeyQueueManager.DeleteMsg(eDoType_dialog_SocketComposite);
        DeleteAllSlot();//아이템 모두 해제
        //ResetAnimation();
        if (g_pHero)
        {
            g_pHero->SetBeHaveState(PLAYER_BEHAVE_IDLE_STATE);
        }
	}
}
//------------------------------------------------------------------------------
/**
*/
void uiSocketComposite::MessageProc(SI_MESSAGE* message)
{
	switch (message->eResultMsg)
	{
	case RT_MSG_LBUTTONCLICK:
		{
            if (socket_composite_lock_ == true)
            {
                return;
            }
			switch (get_control_id_to_pos(message->dwCtrlID))
			{
            case DIALOG_SOCKET_COMPOSITE_EXIT:
				{
                    if (socket_composite_lock_ == false)
                    {
                        this->ShowDialog(FALSE);
                    }
				}
				break;
            case DIALOG_SOCKET_COMPOSITE_CANCLE:
                {
                    if (socket_composite_lock_ == false)
                    {
                        ControlsStateInit();
                        DeleteAllSlot();//아이템 모두 해제
                    }
                }
                break;
            case DIALOG_SOCKET_COMPOSITE_START:
                {
                    POSTYPE zard_position_list[ZardComposeInfoParser::kMaterialSize] = 
                    {
                        item_inventory_position_[SOCKET_COMPOSITE_SLOT_RESOURCE1],
                        item_inventory_position_[SOCKET_COMPOSITE_SLOT_RESOURCE2],
                        item_inventory_position_[SOCKET_COMPOSITE_SLOT_RESOURCE3]
                    };

                    MSG_CG_ITEM_COMPOSE_OR_DECOMPOSE_SYN request;
                    SCItemSlotContainer& item_slot_container = 
                        static_cast<SCItemSlotContainer&>(*(g_pHero->GetSlotContainer(SI_INVENTORY)));

                    TCHAR message_buffer[INTERFACE_STRING_LENGTH] = {0,};

                    // 합성 확률이 0인지 검사한다. 합성이 불가능한 아이템들이면 확률이 0이 나옴
                    // 아이템을 올려 놓는 사용자 이벤트에 이미 확률을 계산하는데 이것을 활용하지 않는 이유가 궁금함
                    // 따라서 확률이 uiSocketComposite의 멤버로 추가 되고
                    // 아이템들이 추가 되고 빠지는 시점에 확률을 계산하고
                    // 이미 계산된 확률이 0이면 서버로 요청을 보내지 않는 것이 바람직
                    // 서버로 요청을 보낸다 해도 실패 처리 되지만 네트웍 낭비.
                    {
                        const ZardComposeInfo::ZardRatio ratio = 
                            ZardComposeInfoParser::Instance()->GetZardCompositionSuccessRatio(
                                item_slot_container, zard_position_list, additional_item_position_, 0);
                        if (ratio == 0)
                        {
                            // 합성이 불가능한 아이템 입니다. 어쩌고 저쩌고 비슷한 메시지 찾아서 뿌릴것
                            //6487	재료가 부족합니다. 합성을 시작할 수 없습니다.
                            g_InterfaceManager.GetInterfaceString(6487, message_buffer,INTERFACE_STRING_LENGTH);
                            SetMessage(SOCKET_COMPOSITE_INFO_STRING4, message_buffer);
                            return;
                        }
                        DWORD gague_width = kMAX_GAGUE_WIDTH/4;
                        float gague_ratio = (float)ratio/1000;
                        kMax_End_Tick = 
                            (DWORD)(gague_width + (DWORD)(gague_width * gague_ratio)) * 50;// 4픽셀씩 이동시 총 애니메이션 횟수
                            // (+) 2픽셀씩 이동하는 분 만큼 추가 // (*) 50ms
                    }
                    
                    // 참고
                    {
                        // GetZardCompositionRequest()의 매개변수 zard_position_list의 개수는 정상적으로 넘어 갔음.
                        // 배열의 시작 주소를 매개변수로 받기 때문에 GetZardCompositionRequest() 안의 스택에서는
                        // zard_position_list를 배열 취급이 아니라 포인터로 취급함.
                        // 따라서 디버거가 배열 처럼 보여주지 않을 뿐 내부 로직은 정상 처리함.
                    }
                    bool is_excuteable = 
                        ZardComposeInfoParser::Instance()->GetZardCompositionRequest(item_slot_container,
                        zard_position_list, additional_item_position_, request);

                    if (is_excuteable == true)
                    {   //쟈드 합성 시작
                        socket_composite_lock_ = true;
                        GlobalFunc::SendPacket(CI_GAMESERVERIDX, &request, request.GetSize());
                        g_InterfaceManager.LockDlgs(IM_SOCKET_MANAGER::SOCKET_COMPOSITE); // 화면 락
                    }
                    else
                    {   //6487 재료가 부족합니다. 합성을 시작할 수 없습니다.
                        g_InterfaceManager.GetInterfaceString(6487, message_buffer,INTERFACE_STRING_LENGTH);
                        SetMessage(SOCKET_COMPOSITE_INFO_STRING4, message_buffer);
                    }
                }
                break;
            case DIALOG_SOCKET_COMPOSITE_RESOURCE1:
            case DIALOG_SOCKET_COMPOSITE_RESOURCE2:
            case DIALOG_SOCKET_COMPOSITE_RESOURCE3:
            case DIALOG_SOCKET_COMPOSITE_ADD_RESOURCE:
                //DIALOG_SOCKET_COMPOSITE_RESULT_ITEM,
                {
                    if (MouseHandler::Instance()->IsExistItemAtHand() == TRUE)
                    {
                        POSTYPE control_pos = 
                            (POSTYPE)(get_control_id_to_pos(message->dwCtrlID));
                        MouseHandler::MouseItem* mouse_item = MouseHandler::Instance()->GetMouseItem();                
                        CControlWZ* pControl = GetControlWZ(message->dwCtrlID);
                        if (pControl)
                        {
                            DeleteResultSlot();

                            RECT rcSlot = pControl->GetSizeRect();
                            bool is_ok = 
                                MouseHandler::Instance()->ItemSocketCompositeTransection(SI_SOCKET_COMPOSITE, 
                                control_pos, &rcSlot);

                            ZardComposeInfo::ZardRatio ratio = 0;
                            if (is_ok == true)
                            {
                                SCItemSlotContainer& item_slot_container = 
                                    static_cast<SCItemSlotContainer&>(*(g_pHero->GetSlotContainer(SI_INVENTORY)));

                                ratio = ZardComposeInfoParser::Instance()->GetZardCompositionSuccessRatio(item_slot_container, 
                                    item_inventory_position_, additional_item_position_, 0); // fail_number_ 은 서버에서만 알고 있을 것 클라에게 화면으로 보여주지 않음

                                ControlsStateInit();// Notice: SetGague와 순서가 바뀌면 안됨
                                SetGague(ratio);
                            }
                        }
                    }
                }
                break;
			}
		}
		break;
    default:
        {
            SolarDialog::MessageProc(message);
        }
	}
}
//------------------------------------------------------------------------------
/**
*/
void uiSocketComposite::NetworkProc(MSG_BASE* message)
{
    if (message->m_byCategory == CG_ITEM)
    {
        switch (message->m_byProtocol)
        {
        case CG_ITEM_ZARD_COMPOSE_ACK:
        {
            TCHAR message_buffer[INTERFACE_STRING_LENGTH] = {0,};
            MSG_CG_ITEM_ZARD_COMPOSE_ACK* socket_composite_result_packet =
                (MSG_CG_ITEM_ZARD_COMPOSE_ACK*)message;
            
            
            socket_composite_lock_ = false;
            switch(socket_composite_result_packet->result_code_)
            {
            case RC::RC_ITEM_SUCCESS_TO_COMPOSE_ZARD: //쟈드 합성 시도가 성공
                {
                    //6490 쟈드 합성이 시작되었습니다.
                    g_InterfaceManager.GetInterfaceString(6490, message_buffer,INTERFACE_STRING_LENGTH);
                    SetMessage(SOCKET_COMPOSITE_INFO_STRING2, message_buffer);
                    SetMessage(SOCKET_COMPOSITE_INFO_STRING3, "");
                    SetMessage(SOCKET_COMPOSITE_INFO_STRING4, "");
                    socket_composite_lock_ = true;
                    g_KeyQueueManager.DeleteMsg(eDoType_dialog_SocketComposite);
                }
                break;
            case RC::RC_ITEM_FAILED_TO_COMPOSE_ZARD: //쟈드 합성 시도가 실패
            case RC::RC_ITEM_INVALID_VALUE: //클라에서 요청한 값이 옳바르지 않음
                {   
                    SetMessage(SOCKET_COMPOSITE_INFO_STRING4, "합성을 시도 할 수가 없습니다.");
                    socket_composite_success_ = false;
                    g_InterfaceManager.UnLockDlgs();
                    return;
                }
                break;
            case RC::RC_ITEM_HAVENOTSPACE: //인벤토리에 보상을 받을 여유 공간이 없음
                {   //85215	인벤토리가 가득 찼습니다
                    result_string_code_ = 85215;
                    g_InterfaceManager.GetInterfaceString(result_string_code_, message_buffer,INTERFACE_STRING_LENGTH);
                    socket_composite_success_ = false;
                    SetMessage(SOCKET_COMPOSITE_INFO_STRING4, message_buffer);
                    g_InterfaceManager.UnLockDlgs();
                    return;
                }
                break;
            default:
                {
                    Sprintf(message_buffer, 
                        "Unknown result code[%d](uiSocketComposite, CG_ITEM_ZARD_COMPOSE_ACK)",
                        socket_composite_result_packet->result_code_);
                    socket_composite_success_ = false;
                    GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, message_buffer);
                    g_InterfaceManager.UnLockDlgs();
                    return;
                }
                break;
            }
            save_inventory_item_info = socket_composite_result_packet->added_inventory_items_;
            if (save_inventory_item_info.m_InvenCount > 0)
            {
                // 6493 축하합니다. %s가 생성되었습니다.
                result_string_code_ = 6493;
                fail_number_ = 0;
                socket_composite_success_ = true;
                RollGoalPosition(true);
            }
            else
            {
                // 6492 쟈드 합성에 실패했습니다.
                result_string_code_ = 6492;
                socket_composite_success_ = false;
                ++fail_number_;
                RollGoalPosition(false);
            }
        }
        break;
        }//switch(cube_result_packet->result_code_)

        animate_frame_count_ = 0;
        //animate_total_frame_ = 0;
        animate_start_tick_ = GetTickCount();
        animate_end_tick_ = animate_start_tick_ + 1500;//500ms 이펙트 동작
        ResetAnimation();

        /*if ((int)goal_position_%2 == 0)
        {
            move_gap_x_ = kMAX_GAGUE_WIDTH/2;
        }
        else
        {
            move_gap_x_ = kMAX_GAGUE_WIDTH + kMAX_GAGUE_WIDTH/2;
        }*/
    }
}

//------------------------------------------------------------------------------
SCSlotContainer* uiSocketComposite::GetSlotContainer()
{
    return static_cast<SCSlotContainer*>(this);
}

//------------------------------------------------------------------------------ 
RC::eSLOT_INSERT_RESULT uiSocketComposite::InsertSlot(POSTYPE at_pos, SCSlot& slot)
{
    if (item_unit_renderer_ == NULL)
    {
        assert(item_unit_renderer_);
        return RC::RC_INSERT_SLOT_FAILED;
    }
    /// 슬롯에 넣음
    RC::eSLOT_INSERT_RESULT eInsertSlotResult = DummySlotContainer::InsertSlot(at_pos, slot);
    DummyItemSlot & dummy_item_slot = (DummyItemSlot&)slot;

    // 락킹을 걸어라.
    SCSlotContainer* pFromContainer = 
        ItemManager::Instance()->GetContainer(dummy_item_slot.GetFromContainerIdx());
    if (pFromContainer && at_pos != SOCKET_COMPOSITE_SLOT_RESULT_ITEM)
    {
        SCItemSlot& rFromItemSlot = 
            (SCItemSlot &)pFromContainer->GetSlot(dummy_item_slot.GetFromPosition());

        rFromItemSlot.SetLock( TRUE );
    }

    /// 인벤토리
    CControlWZ* control = get_control(at_pos);
    RECT rect = control->GetSizeRect();

    DummyItemSlot& real_slot = (DummyItemSlot&)(GetSlot(at_pos));
    assert(real_slot.GetOrgSlot());

    item_unit_renderer_->AddItemUnit(real_slot.GetSerial(), real_slot.GetOrgSlot()->GetCode(), 
        &rect, &real_slot, control);

    if (at_pos >= SOCKET_COMPOSITE_SLOT_RESOURCE1 && 
        at_pos < SOCKET_COMPOSITE_SLOT_RESULT_MAX)
    {
        item_inventory_position_[at_pos] = real_slot.GetOrgSlot()->GetPos();
    }

    return eInsertSlotResult;
}
//------------------------------------------------------------------------------ 
BOOL uiSocketComposite::DeleteSlot(POSTYPE at_pos, SCSlot* slotOut)
{
    if (item_unit_renderer_ == NULL)
    {
        assert(item_unit_renderer_);
        return FALSE;
    }
    DummyItemSlot& delete_dummy_slot = (DummyItemSlot&)GetSlot(at_pos);

    // 락킹을 풀어라.
    SCSlotContainer* from_container = 
        ItemManager::Instance()->GetContainer(delete_dummy_slot.GetFromContainerIdx());
    if (from_container != NULL)
    {
        SCItemSlot& from_item_slot = 
            (SCItemSlot&)from_container->GetSlot(delete_dummy_slot.GetFromPosition());

        from_item_slot.SetLock( FALSE );
    }

    if (delete_dummy_slot.GetOrgSlot())
    {
        item_unit_renderer_->RemoveItemUnit(delete_dummy_slot.GetSerial());
        if (at_pos >= SOCKET_COMPOSITE_SLOT_RESOURCE1 && 
            at_pos < SOCKET_COMPOSITE_SLOT_RESOURCE_MAX)
        {
            item_inventory_position_[at_pos] = INVALID_POSTYPE_VALUE;
        }
        else if(SOCKET_COMPOSITE_SLOT_RESOURCE_MAX)
        {
            additional_item_position_ = INVALID_POSTYPE_VALUE;
        }
        //else 결과물은 위치저장 안함
    }
    return DummySlotContainer::DeleteSlot(at_pos, slotOut);
}
//------------------------------------------------------------------------------
void uiSocketComposite::DeleteAllSlot()
{
    if (item_unit_renderer_ == NULL)
    {
        assert(item_unit_renderer_);
        return;
    }
    for (int slot_pos = SOCKET_COMPOSITE_SLOT_RESOURCE1; 
        slot_pos < SOCKET_COMPOSITE_SLOT_RESULT_MAX; ++slot_pos)
    {
        if (!IsEmpty(slot_pos))
        {
            this->DeleteSlot(slot_pos, NULL);
        }
    }
}
//------------------------------------------------------------------------------
void uiSocketComposite::DeleteResourceSlot()
{
    if (item_unit_renderer_ == NULL)
    {
        assert(item_unit_renderer_);
        return;
    }
    for (int slot_pos = SOCKET_COMPOSITE_SLOT_RESOURCE1; 
        slot_pos < SOCKET_COMPOSITE_SLOT_RESULT_ITEM; ++slot_pos)
    {
        if (!IsEmpty(slot_pos))
        {
            this->DeleteSlot(slot_pos, NULL);
        }
    }
}
//------------------------------------------------------------------------------
void uiSocketComposite::DeleteResultSlot()
{
    if (item_unit_renderer_ == NULL)
    {
        assert(item_unit_renderer_);
        return;
    }
    
    if (!IsEmpty(SOCKET_COMPOSITE_SLOT_RESULT_ITEM))
    {
        this->DeleteSlot(SOCKET_COMPOSITE_SLOT_RESULT_ITEM, NULL);
    }
}

//------------------------------------------------------------------------------
void uiSocketComposite::OnSocketComposite_InventoryRightClick(const SLOTIDX from_container_index, 
                                                const POSTYPE from_container_position)
{
    if (socket_composite_lock_ == true)
    {
        return;
    }

    DeleteResultSlot();

    ZardComposeInfo::ZardRatio ratio = 0;
    bool is_ok = MouseHandler::Instance()->SocketCompositeInventoryRightClick(from_container_index, SI_SOCKET_COMPOSITE, 
        from_container_position);

    if (is_ok == true)
    {
        SCItemSlotContainer& item_slot_container = 
            static_cast<SCItemSlotContainer&>(*(g_pHero->GetSlotContainer(SI_INVENTORY)));

        ratio = ZardComposeInfoParser::Instance()->GetZardCompositionSuccessRatio(item_slot_container, 
            item_inventory_position_, additional_item_position_, 0); // fail_number_ 은 서버에서만 알고 있을 것 클라에게 화면으로 보여주지 않음
        ControlsStateInit(); // 주의: SetGague와 순서가 바뀌면 안됨
        SetGague(ratio);
    }
}
//------------------------------------------------------------------------------
void uiSocketComposite::SetMessage(int index, TCHAR* message)
{
    DWORD nTextStyle = DT_VCENTER | DT_CENTER;	// 기본값
    socket_composite_list_->DeleteItemAll();

    sprintf(message_[index], message);
    for (int i = SOCKET_COMPOSITE_INFO_STRING1; i < INFORMATION_MAX; ++i)
    {
        socket_composite_list_->InsertItemBack();
        GlobalFunc::SetItem(socket_composite_list_, i, 0,
            message_[i],
            c_wiListFont2,
            nTextStyle,
            RGBA(255,255,255,0), 
            RGBA(255,255,255,255), 
            RGBA(255,255,255,255));
    }
}
//------------------------------------------------------------------------------
void uiSocketComposite::SetGague(float ratio)
{
    float ratio_by_width = ratio * kMAX_PER_1_WIDTH;
    if (ratio_by_width > kMAX_GAGUE_WIDTH)
    {
        ratio_by_width = (float) kMAX_GAGUE_WIDTH;
    }

    RECT dialog_rect;
    GetDialogWZ()->GetClientRect(&dialog_rect);
    socket_composite_picture_[SUCCESS_AREA_BACKGROUND]->SetSizeWidth((long)ratio_by_width);
    RECT control_size = socket_composite_picture_[SUCCESS_AREA_BACKGROUND]->GetSize();

    gague_left_ = (kMAX_GAGUE_WIDTH-ratio_by_width)/2;
    gague_right_ = gague_left_ + ratio_by_width;

    socket_composite_picture_[SUCCESS_AREA_BACKGROUND]->MoveWindowWZ(
        (float)dialog_rect.left + kBASE_X + gague_left_, 
        (float)dialog_rect.top + kBASE_Y,
        (float)control_size.right, (float)control_size.bottom);
}
//------------------------------------------------------------------------------
void uiSocketComposite::AnimateEffect()
{
    RECT dialog_rect;
    GetDialogWZ()->GetClientRect(&dialog_rect);
    RECT control_size = socket_composite_picture_[MOVE_STICK]->GetSize();

    float position_x = 0;
    float move_gap_x = (float)((int)move_gap_x_ % kMAX_GAGUE_WIDTH);
    position_x = dialog_rect.left + kBASE_STICK_X + goal_position_;

    MoveStick(dialog_rect, control_size, position_x);   // 막대 이동
    MoveEffect(dialog_rect, control_size, position_x);  // 이펙트 이동

    ++animate_frame_count_;

    if (animate_now_tick_ >= animate_end_tick_)
    {
        TCHAR message_buffer[INTERFACE_STRING_LENGTH] = {0,};

        for (int i = EFFECT_1; i < SOCKET_COMPOSITE_PICTURES_MAX; ++i)
        {
            socket_composite_picture_[i]->ShowWindowWZ(WZ_HIDE);
        }
        control_size = socket_composite_picture_[MOVE_STICK]->GetSize();

        position_x = dialog_rect.left + kBASE_STICK_X + goal_position_;
        MoveStick(dialog_rect, control_size, position_x);   // 막대 이동

        //6491 쟈드 합성이 종료되었습니다.
        g_InterfaceManager.GetInterfaceString(6491, message_buffer,INTERFACE_STRING_LENGTH);
        SetMessage(SOCKET_COMPOSITE_INFO_STRING2, message_buffer);
        DeleteResourceSlot();   //재료 아이템 제거
        success_sound_count_ = 0;
        fail_sound_count_ = 0;
        g_InterfaceManager.UnLockDlgs();

        keyMsg msg;
        ZeroMemory(&msg,sizeof(msg));
        msg.dwType=eDoType_dialog_SocketComposite;
        msg.wParam=IM_SOCKET_MANAGER::SOCKET_COMPOSITE;
        msg.DoSomething=GlobalFunc::DoShowWindowEx;
        g_KeyQueueManager.PushBack(msg);
    }
}
//------------------------------------------------------------------------------
float uiSocketComposite::RollGoalPosition(bool is_success)
{
    int range_success = (int)(gague_right_ - gague_left_);
    int range_fail = kMAX_GAGUE_WIDTH - range_success;

    if (is_success) // 성공이면
    {
        if (range_success > 0)
        {
            goal_position_ = (float)(rand()%range_success) + (int)gague_left_;
        }
        else
        {
            goal_position_ = (float)gague_left_;
        }
        // 애매하지 않도록 값 보정하기
        if ((goal_position_ - gague_left_) < 10)
        {
            goal_position_ += 10;    // 성공구간 왼쪽에 가까우면 +10
        }
        else if ((goal_position_ - gague_left_ + 5) > range_success)
        {
            goal_position_ -= 10;    // 성공구간 오른쪽에 가까우면 -10
        }
    }
    else
    {
        if (range_fail > 0)
        {         
            goal_position_= (float)(rand()%range_fail);
        }
        else
        {
            goal_position_= 0.0f;
        }
        
        if (goal_position_ > (range_fail/2))
        {
            goal_position_ = (goal_position_/2) + (int)gague_right_;
        }
        // 애매하지 않도록 값 보정하기
        if ((goal_position_ + 10) >= gague_left_)
        {
            goal_position_ -= 10;    // 성공구간 왼쪽에 가까우면 -5
        }
        else if ((goal_position_ - 10) <= (gague_left_ + range_success))
        {
            goal_position_ += 10;    // 성공구간 오른쪽에 가까우면 +5
        }
    }

    if (goal_position_ < 0.0f)
    {
        goal_position_ = 0.0f;
    }
    else if (goal_position_ >= kMAX_GAGUE_WIDTH)
    {
        goal_position_ = kMAX_GAGUE_WIDTH - 4.0f;
    }

    float additional_time = goal_position_ / 4; // 1 차 실패구간
    if (goal_position_ > gague_left_ && goal_position_ <= gague_right_)
    {
        additional_time += ((goal_position_ - gague_left_) / 2);   // 성공구간
    }
    if (goal_position_ > gague_right_)
    {
        additional_time += (range_success / 2);  // 2차 실패구간
    }
    
    additional_time *= 50.0f;
    additional_time += (kMax_End_Tick/2);
    kMax_End_Tick += (int)additional_time;
#ifdef _DEBUG
    char string[128] = {0,};
    sprintf(string, "goal = %s, %d", is_success?"ok":"fail", (int)goal_position_);
    SetMessage(SOCKET_COMPOSITE_INFO_STRING3, string);
#endif//_DEBUG
    return goal_position_;
}
//------------------------------------------------------------------------------
void uiSocketComposite::InitAnimation()
{// 최초 화면 초기화

}
//------------------------------------------------------------------------------
void uiSocketComposite::BaseAnimation()
{// 기본 애니메이션 중

}
//------------------------------------------------------------------------------
void uiSocketComposite::AddtionalAnimation()
{// 추가 애니메이션

}
//------------------------------------------------------------------------------
void uiSocketComposite::MoveEffect(RECT dialog_rect, RECT control_size, float position_x)
{// 이펙트 이동시키기

    int count = animate_frame_count_/2;
    if (count >= 4)
    {
        count = 0;
        animate_frame_count_ = 0;
    }

    for (int i = EFFECT_1; i < SOCKET_COMPOSITE_PICTURES_MAX; ++i)
    {
        if (i == (EFFECT_1+count))
        {
            control_size = socket_composite_picture_[i]->GetSize();
            socket_composite_picture_[i]->ShowWindowWZ(WZ_SHOW);
            socket_composite_picture_[i]->MoveWindowWZ(
                (float)position_x + kEFFECT_BASE_X,
                (float)dialog_rect.top + kEFFECT_BASE_Y,
                (float)control_size.right, (float)control_size.bottom);
        }
        else
        {
            socket_composite_picture_[i]->ShowWindowWZ(WZ_HIDE);
        }
    }
}
//------------------------------------------------------------------------------
void uiSocketComposite::MoveStick(RECT dialog_rect, RECT control_size, float position_x)
{// 막대 이동시키기
    socket_composite_picture_[MOVE_STICK]->MoveWindowWZ(
        (float)position_x, 
        (float)dialog_rect.top + kSTICK_BASE_Y,
        (float)control_size.right, (float)control_size.bottom);
}
//----------------------------------------------------------------------------
void uiSocketComposite::MouseOver_MsgProc(SI_MESSAGE* message)
{
    switch (message->eResultMsg) 
    {
    case RT_MSG_MOUSEOVER:
        {
            POSTYPE control_pos = static_cast<POSTYPE>(get_control_id_to_pos(message->dwCtrlID));
            if (control_pos >= DIALOG_SOCKET_COMPOSITE_RESOURCE1 && control_pos < (DIALOG_SOCKET_COMPOSITE_RESULT_ITEM+1))
            {
                if (!IsEmpty(control_pos))
                {
                    DummyItemSlot& rSlot = (DummyItemSlot&)GetSlot(control_pos);
                    assert( rSlot.GetOrgSlot());

                    if(rSlot.GetOrgSlot())
                    {
                        m_MouseOverObject.eMouseOverType = eMOUSE_OVER_SLOT;
                        m_MouseOverObject.pSlot = rSlot.GetOrgSlot();
                    }
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
    }
}
//----------------------------------------------------------------------------
void uiSocketComposite::ControlsStateInit()
{
    RECT dialog_rect;
    GetDialogWZ()->GetClientRect(&dialog_rect);
    RECT control_size = control_size = socket_composite_picture_[MOVE_STICK]->GetSize();

    float position_x = ((float)dialog_rect.left + kBASE_STICK_X + kMAX_GAGUE_WIDTH/2);
    MoveStick(dialog_rect, control_size, position_x);

    SetMessage(SOCKET_COMPOSITE_INFO_STRING2, "");
    SetMessage(SOCKET_COMPOSITE_INFO_STRING3, "");
    SetMessage(SOCKET_COMPOSITE_INFO_STRING4, "");

    SetGague(0);
    DeleteResultSlot();
}
