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
    StrToWzID("B003"),  //���1         DIALOG_SOCKET_COMPOSITE_RESOURCE1
    StrToWzID("B000"),  //���2         DIALOG_SOCKET_COMPOSITE_RESOURCE2
    StrToWzID("B002"),  //���3         DIALOG_SOCKET_COMPOSITE_RESOURCE3
    StrToWzID("B004"),  //÷����        DIALOG_SOCKET_COMPOSITE_ADD_RESOURCE
    StrToWzID("B005"),  //�����        DIALOG_SOCKET_COMPOSITE_RESULT_ITEM
    StrToWzID("L001"),  //����Ʈ        DIALOG_SOCKET_COMPOSITE_LIST
    StrToWzID("B999"),  //����          DIALOG_SOCKET_COMPOSITE_EXIT
    StrToWzID("T999"),  //Ÿ��Ʋ        DIALOG_SOCKET_COMPOSITE_TITTLE
    StrToWzID("BT00"),  //�ռ�����      DIALOG_SOCKET_COMPOSITE_START
    StrToWzID("B001"),  //���          DIALOG_SOCKET_COMPOSITE_CANCLE
    StrToWzID("P001"),  //�����������  DIALOG_SOCKET_COMPOSITE_SUCCESS_AREA_BACKGROUND
    StrToWzID("P002"),  //�̵�����      DIALOG_SOCKET_COMPOSITE_MOVE_STICK
    StrToWzID("P003"),  //����Ʈ1       DIALOG_SOCKET_COMPOSITE_EFFECT_1
    StrToWzID("P004"),  //����Ʈ2       DIALOG_SOCKET_COMPOSITE_EFFECT_2
    StrToWzID("P005"),  //����Ʈ3       DIALOG_SOCKET_COMPO+SITE_EFFECT_3
    StrToWzID("P006"),  //����Ʈ4       DIALOG_SOCKET_COMPOSITE_EFFECT_4
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

    // ���� �ʱ�ȭ
    DummySlotContainer::Init(SOCKET_COMPOSITE_SLOT_RESULT_MAX);   

    // ������ �ʱ�ȭ
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

    // �ִϸ��̼� �ð� ����
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
        // ���� ����
        g_SoundEffect.PlayUI(UI_SOUND_ZARD_SUCCESS);
    }
    else
    {
        if (result_string_code_ != 0)
        {
            TCHAR message_buffer[INTERFACE_STRING_LENGTH] = {0,};
            g_InterfaceManager.GetInterfaceString(result_string_code_, message_buffer, INTERFACE_STRING_LENGTH);
            SetMessage(SOCKET_COMPOSITE_INFO_STRING3, message_buffer);
            // ���� ����
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

    if (animate_start_tick_ == 0)//����Ʈ �߿��� ���� ǥ�� ����
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

    // ��ŵó�� 
    // �������̽����� Dialog ���������� Mouse Over�� ���� �ʴ´�.
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
        // 2190 �ռ��� ��带 ����ϼ���.
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
        DeleteAllSlot();//������ ��� ����
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
                        DeleteAllSlot();//������ ��� ����
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

                    // �ռ� Ȯ���� 0���� �˻��Ѵ�. �ռ��� �Ұ����� �����۵��̸� Ȯ���� 0�� ����
                    // �������� �÷� ���� ����� �̺�Ʈ�� �̹� Ȯ���� ����ϴµ� �̰��� Ȱ������ �ʴ� ������ �ñ���
                    // ���� Ȯ���� uiSocketComposite�� ����� �߰� �ǰ�
                    // �����۵��� �߰� �ǰ� ������ ������ Ȯ���� ����ϰ�
                    // �̹� ���� Ȯ���� 0�̸� ������ ��û�� ������ �ʴ� ���� �ٶ���
                    // ������ ��û�� ������ �ص� ���� ó�� ������ ��Ʈ�� ����.
                    {
                        const ZardComposeInfo::ZardRatio ratio = 
                            ZardComposeInfoParser::Instance()->GetZardCompositionSuccessRatio(
                                item_slot_container, zard_position_list, additional_item_position_, 0);
                        if (ratio == 0)
                        {
                            // �ռ��� �Ұ����� ������ �Դϴ�. ��¼�� ��¼�� ����� �޽��� ã�Ƽ� �Ѹ���
                            //6487	��ᰡ �����մϴ�. �ռ��� ������ �� �����ϴ�.
                            g_InterfaceManager.GetInterfaceString(6487, message_buffer,INTERFACE_STRING_LENGTH);
                            SetMessage(SOCKET_COMPOSITE_INFO_STRING4, message_buffer);
                            return;
                        }
                        DWORD gague_width = kMAX_GAGUE_WIDTH/4;
                        float gague_ratio = (float)ratio/1000;
                        kMax_End_Tick = 
                            (DWORD)(gague_width + (DWORD)(gague_width * gague_ratio)) * 50;// 4�ȼ��� �̵��� �� �ִϸ��̼� Ƚ��
                            // (+) 2�ȼ��� �̵��ϴ� �� ��ŭ �߰� // (*) 50ms
                    }
                    
                    // ����
                    {
                        // GetZardCompositionRequest()�� �Ű����� zard_position_list�� ������ ���������� �Ѿ� ����.
                        // �迭�� ���� �ּҸ� �Ű������� �ޱ� ������ GetZardCompositionRequest() ���� ���ÿ�����
                        // zard_position_list�� �迭 ����� �ƴ϶� �����ͷ� �����.
                        // ���� ����Ű� �迭 ó�� �������� ���� �� ���� ������ ���� ó����.
                    }
                    bool is_excuteable = 
                        ZardComposeInfoParser::Instance()->GetZardCompositionRequest(item_slot_container,
                        zard_position_list, additional_item_position_, request);

                    if (is_excuteable == true)
                    {   //��� �ռ� ����
                        socket_composite_lock_ = true;
                        GlobalFunc::SendPacket(CI_GAMESERVERIDX, &request, request.GetSize());
                        g_InterfaceManager.LockDlgs(IM_SOCKET_MANAGER::SOCKET_COMPOSITE); // ȭ�� ��
                    }
                    else
                    {   //6487 ��ᰡ �����մϴ�. �ռ��� ������ �� �����ϴ�.
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
                                    item_inventory_position_, additional_item_position_, 0); // fail_number_ �� ���������� �˰� ���� �� Ŭ�󿡰� ȭ������ �������� ����

                                ControlsStateInit();// Notice: SetGague�� ������ �ٲ�� �ȵ�
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
            case RC::RC_ITEM_SUCCESS_TO_COMPOSE_ZARD: //��� �ռ� �õ��� ����
                {
                    //6490 ��� �ռ��� ���۵Ǿ����ϴ�.
                    g_InterfaceManager.GetInterfaceString(6490, message_buffer,INTERFACE_STRING_LENGTH);
                    SetMessage(SOCKET_COMPOSITE_INFO_STRING2, message_buffer);
                    SetMessage(SOCKET_COMPOSITE_INFO_STRING3, "");
                    SetMessage(SOCKET_COMPOSITE_INFO_STRING4, "");
                    socket_composite_lock_ = true;
                    g_KeyQueueManager.DeleteMsg(eDoType_dialog_SocketComposite);
                }
                break;
            case RC::RC_ITEM_FAILED_TO_COMPOSE_ZARD: //��� �ռ� �õ��� ����
            case RC::RC_ITEM_INVALID_VALUE: //Ŭ�󿡼� ��û�� ���� �ǹٸ��� ����
                {   
                    SetMessage(SOCKET_COMPOSITE_INFO_STRING4, "�ռ��� �õ� �� ���� �����ϴ�.");
                    socket_composite_success_ = false;
                    g_InterfaceManager.UnLockDlgs();
                    return;
                }
                break;
            case RC::RC_ITEM_HAVENOTSPACE: //�κ��丮�� ������ ���� ���� ������ ����
                {   //85215	�κ��丮�� ���� á���ϴ�
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
                // 6493 �����մϴ�. %s�� �����Ǿ����ϴ�.
                result_string_code_ = 6493;
                fail_number_ = 0;
                socket_composite_success_ = true;
                RollGoalPosition(true);
            }
            else
            {
                // 6492 ��� �ռ��� �����߽��ϴ�.
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
        animate_end_tick_ = animate_start_tick_ + 1500;//500ms ����Ʈ ����
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
    /// ���Կ� ����
    RC::eSLOT_INSERT_RESULT eInsertSlotResult = DummySlotContainer::InsertSlot(at_pos, slot);
    DummyItemSlot & dummy_item_slot = (DummyItemSlot&)slot;

    // ��ŷ�� �ɾ��.
    SCSlotContainer* pFromContainer = 
        ItemManager::Instance()->GetContainer(dummy_item_slot.GetFromContainerIdx());
    if (pFromContainer && at_pos != SOCKET_COMPOSITE_SLOT_RESULT_ITEM)
    {
        SCItemSlot& rFromItemSlot = 
            (SCItemSlot &)pFromContainer->GetSlot(dummy_item_slot.GetFromPosition());

        rFromItemSlot.SetLock( TRUE );
    }

    /// �κ��丮
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

    // ��ŷ�� Ǯ���.
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
        //else ������� ��ġ���� ����
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
            item_inventory_position_, additional_item_position_, 0); // fail_number_ �� ���������� �˰� ���� �� Ŭ�󿡰� ȭ������ �������� ����
        ControlsStateInit(); // ����: SetGague�� ������ �ٲ�� �ȵ�
        SetGague(ratio);
    }
}
//------------------------------------------------------------------------------
void uiSocketComposite::SetMessage(int index, TCHAR* message)
{
    DWORD nTextStyle = DT_VCENTER | DT_CENTER;	// �⺻��
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

    MoveStick(dialog_rect, control_size, position_x);   // ���� �̵�
    MoveEffect(dialog_rect, control_size, position_x);  // ����Ʈ �̵�

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
        MoveStick(dialog_rect, control_size, position_x);   // ���� �̵�

        //6491 ��� �ռ��� ����Ǿ����ϴ�.
        g_InterfaceManager.GetInterfaceString(6491, message_buffer,INTERFACE_STRING_LENGTH);
        SetMessage(SOCKET_COMPOSITE_INFO_STRING2, message_buffer);
        DeleteResourceSlot();   //��� ������ ����
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

    if (is_success) // �����̸�
    {
        if (range_success > 0)
        {
            goal_position_ = (float)(rand()%range_success) + (int)gague_left_;
        }
        else
        {
            goal_position_ = (float)gague_left_;
        }
        // �ָ����� �ʵ��� �� �����ϱ�
        if ((goal_position_ - gague_left_) < 10)
        {
            goal_position_ += 10;    // �������� ���ʿ� ������ +10
        }
        else if ((goal_position_ - gague_left_ + 5) > range_success)
        {
            goal_position_ -= 10;    // �������� �����ʿ� ������ -10
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
        // �ָ����� �ʵ��� �� �����ϱ�
        if ((goal_position_ + 10) >= gague_left_)
        {
            goal_position_ -= 10;    // �������� ���ʿ� ������ -5
        }
        else if ((goal_position_ - 10) <= (gague_left_ + range_success))
        {
            goal_position_ += 10;    // �������� �����ʿ� ������ +5
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

    float additional_time = goal_position_ / 4; // 1 �� ���б���
    if (goal_position_ > gague_left_ && goal_position_ <= gague_right_)
    {
        additional_time += ((goal_position_ - gague_left_) / 2);   // ��������
    }
    if (goal_position_ > gague_right_)
    {
        additional_time += (range_success / 2);  // 2�� ���б���
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
{// ���� ȭ�� �ʱ�ȭ

}
//------------------------------------------------------------------------------
void uiSocketComposite::BaseAnimation()
{// �⺻ �ִϸ��̼� ��

}
//------------------------------------------------------------------------------
void uiSocketComposite::AddtionalAnimation()
{// �߰� �ִϸ��̼�

}
//------------------------------------------------------------------------------
void uiSocketComposite::MoveEffect(RECT dialog_rect, RECT control_size, float position_x)
{// ����Ʈ �̵���Ű��

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
{// ���� �̵���Ű��
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
