#include "SunClientPrecompiledHeader.h"
#ifdef _NA_003923_20120130_GUILD_RENEWAL_MOD_WAREHOUSE_SLOT_NUM
#include "GuildSystem/uiGuildMan.h"
#include "GuildSystem/GuildSystemFunction.h"
#include "GuildSystem/GuildSystemData.h"
#include "uiGuildWareHouse.h"
#include <SCItemSlot.h>
#include "MouseHandler.h"
#include "ItemManager.h"
#include "InterfaceManager.h"
#include "SlotKeyGenerator.h"
#include "Mouse.h"
#include "CursorChangeTransact.h"
#include "hero.h"
#include "globalfunc.h"
#include "uiToolTipMan/uiToolTipMan.h"
#include "ItemManager.h"
#include "HeroActionInput.h"
#include "BitStream.hpp"

const DWORD uiGuildWareHouse::kDialog_ID = StrToWzID("0738");

//----------------------------------------------------------------------------
/**
*/
uiGuildWareHouse::uiGuildWareHouse(InterfaceManager* ui_manager)
	:uiBase(ui_manager),
	Render_ui_Draw_List_(false)
{
    item_unit_renderer_ = NULL;
}


//----------------------------------------------------------------------------
/**
*/
uiGuildWareHouse::~uiGuildWareHouse(void) 
{
}


//----------------------------------------------------------------------------
/**
*/
void uiGuildWareHouse::Init(CDrawBase* draw_base)
{
	uiBase::Init(draw_base);
    SCItemSlotContainer::Init(MAX_GUILD_WAREHOUSE_SLOT_NUM, SI_GUILDWAREHOUSE);

    InitRenderer();

    RegisterControlPosition(StrToWzID("C001"), kControl_CheckBT_C001);
    RegisterControlPosition(StrToWzID("C004"), kControl_CheckBT_C004);
    RegisterControlPosition(StrToWzID("C002"), kControl_CheckBT_C002);
    RegisterControlPosition(StrToWzID("C003"), kControl_CheckBT_C003);
    RegisterControlPosition(StrToWzID("C000"), kControl_CheckBT_C000);
    RegisterControlPosition(StrToWzID("B999"), kControl_Button_B999);
    RegisterControlPosition(StrToWzID("B001"), kControl_Button_B001);
    RegisterControlPosition(StrToWzID("C005"), kControl_CheckBT_C005);
    RegisterControlPosition(StrToWzID("C006"), kControl_CheckBT_C006);
    RegisterControlPosition(StrToWzID("B002"), kControl_Button_B002);
    RegisterControlPosition(StrToWzID("B003"), kControl_Button_B003);
    RegisterControlPosition(StrToWzID("B004"), kControl_Button_B004);
    RegisterControlPosition(StrToWzID("B007"), kControl_Button_B007);
    RegisterControlPosition(StrToWzID("B008"), kControl_Button_B008);
    RegisterControlPosition(StrToWzID("B009"), kControl_Button_B009);
    RegisterControlPosition(StrToWzID("B010"), kControl_Button_B010);
    RegisterControlPosition(StrToWzID("B011"), kControl_Button_B011);
    RegisterControlPosition(StrToWzID("B012"), kControl_Button_B012);
    RegisterControlPosition(StrToWzID("B013"), kControl_Button_B013);
    RegisterControlPosition(StrToWzID("B014"), kControl_Button_B014);
    RegisterControlPosition(StrToWzID("B015"), kControl_Button_B015);
    RegisterControlPosition(StrToWzID("B016"), kControl_Button_B016);
    RegisterControlPosition(StrToWzID("B017"), kControl_Button_B017);
    RegisterControlPosition(StrToWzID("B018"), kControl_Button_B018);
    RegisterControlPosition(StrToWzID("B019"), kControl_Button_B019);
    RegisterControlPosition(StrToWzID("B020"), kControl_Button_B020);
    RegisterControlPosition(StrToWzID("B021"), kControl_Button_B021);
    RegisterControlPosition(StrToWzID("B022"), kControl_Button_B022);
    RegisterControlPosition(StrToWzID("B023"), kControl_Button_B023);
    RegisterControlPosition(StrToWzID("B024"), kControl_Button_B024);
    RegisterControlPosition(StrToWzID("B025"), kControl_Button_B025);
    RegisterControlPosition(StrToWzID("B026"), kControl_Button_B026);
    RegisterControlPosition(StrToWzID("B027"), kControl_Button_B027);
    RegisterControlPosition(StrToWzID("B029"), kControl_Button_B029);
    RegisterControlPosition(StrToWzID("B030"), kControl_Button_B030);
    RegisterControlPosition(StrToWzID("B005"), kControl_Button_B005);
    RegisterControlPosition(StrToWzID("T009"), kControl_Text_T009);
    RegisterControlPosition(StrToWzID("S000"), kControl_Text_S000);
    RegisterControlPosition(StrToWzID("E000"), kControl_Edit_E000);
	RegisterControlPosition(StrToWzID("P033"), DIALOG_LOCK_PICTURE_P033);
	RegisterControlPosition(StrToWzID("P032"), DIALOG_LOCK_PICTURE_P032);
	RegisterControlPosition(StrToWzID("P034"), DIALOG_LOCK_PICTURE_P034);

    CControlWZ* button_control = NULL;
    for (POSTYPE position = kControl_CheckBT_C000; position < (kControl_CheckBT_C004 + 1); ++position)
    {
        button_control = GetControlWZ_byPosition(position);
        if (button_control == NULL)
        {
            continue;
        }

        button_control->EnableWindowWZ(false);
    }

    button_control = GetControlWZ_byPosition(kControl_Button_B000);
    if (button_control != NULL)
    {
        button_control->ShowWindowWZ(WZ_HIDE);
    }
    button_control = GetControlWZ_byPosition(kControl_Button_B006);
    if (button_control != NULL)
    {
        button_control->ShowWindowWZ(WZ_HIDE);
    }

	CCtrlPictureWZ* LockImage1= (CCtrlPictureWZ*)GetControlWZ_byPosition(DIALOG_LOCK_PICTURE_P033);
	if(LockImage1)
	{
		LockImage1->ShowWindowWZ(WZ_HIDE);
	}
	CCtrlPictureWZ* LockImage2= (CCtrlPictureWZ*)GetControlWZ_byPosition(DIALOG_LOCK_PICTURE_P032);
	if(LockImage2)
	{
		LockImage2->ShowWindowWZ(WZ_HIDE);
	}
	CCtrlPictureWZ* LockImage3= (CCtrlPictureWZ*)GetControlWZ_byPosition(DIALOG_LOCK_PICTURE_P034);
	if(LockImage3)
	{
		LockImage3->ShowWindowWZ(WZ_HIDE);
	}
}

//----------------------------------------------------------------------------
/**
*/
void uiGuildWareHouse::FlushUnitDraw()
{
	if (item_unit_renderer_ == NULL)
	{
		return;
	}

    for (int slot_index = 0; slot_index < GetMaxSlotNum(); ++slot_index)
	{
		if (IsEmpty(slot_index) == FALSE)
        {
            DeleteSlot(slot_index, NULL);
        }
    }
}	

//----------------------------------------------------------------------------
/**
*/
void uiGuildWareHouse::SendWareHouseStart()
{
    if (IsVisible() == TRUE)
    {
        //	창고가 열린 상태입니다.
        TCHAR message[INTERFACE_STRING_LENGTH];
        g_InterfaceManager.GetInterfaceString(eST_WAREHOUSE_ALREADY_OPEN, message, INTERFACE_STRING_LENGTH);
        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, message);

        return;
    }

    MSG_CG_GUILD_WAREHOUSE_OPEN_SYN send_packek;
    send_packek.m_GuildIndex = GuildSystem::GetGuildSystemData().GetGuildInfo().m_GuildGuid;
    GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send_packek, sizeof(send_packek));
    GlobalFunc::DebugOutputMsg(eOUTPUT_MSG_CHAT, _T("SEND_CG_GUILD_WAREHOUSE_OPEN_SYN"));
}

//----------------------------------------------------------------------------
/**
*/
void uiGuildWareHouse::SendWareHouseEnd()
{
    STLX_SET<WareHouseControlFlag>::iterator end_itr = control_flag().end();
    if (control_flag().find(kUseStart) == end_itr)
    {
        return;
    }

    if (control_flag().find(kUseEnd) != end_itr)
    {
        return;
    }

    control_flag().insert(kUseEnd);

	// 창고를 열고 정보를 받아온 상태라면
    MSG_CG_GUILD_WAREHOUSE_CLOSE_SYN send_packek;
    send_packek.m_GuildIndex = GuildSystem::GetGuildSystemData().GetGuildInfo().m_GuildGuid;
    BOOL return_value = GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send_packek, sizeof(send_packek));

    if (return_value == TRUE)
    {
        GlobalFunc::DebugOutputMsg(eOUTPUT_MSG_CHAT, _T("SEND_CG_GUILD_WAREHOUSE_CLOSE_SYN"));
    }
    else
    {
        GlobalFunc::DebugOutputMsg(eOUTPUT_MSG_CHAT, _T("SEND_CG_GUILD_WAREHOUSE_CLOSE_SYN 못보냈다."));
    }
}

//----------------------------------------------------------------------------
/**
*/
void uiGuildWareHouse::SendWareHouseMoney(BYTE money_type)
{
    STLX_SET<WareHouseControlFlag>::iterator end_itr = control_flag().end();
    if (control_flag().find(kSavingMoney) != end_itr)
    {
        return;
    }

    MSG_CG_GUILD_WAREHOUSE_MONEY_SYN money_packet;
    money_packet.m_byType = money_type;
    money_packet.m_Money = money();

    control_flag().insert(kSavingMoney);

    GlobalFunc::SendPacket(CI_GAMESERVERIDX, &money_packet, sizeof(money_packet));
}

//----------------------------------------------------------------------------
/**
*/
void uiGuildWareHouse::InitRenderer()
{
	// 임시
	if (item_unit_renderer_ != NULL)
	{
		SAFE_RELEASENDELETE(item_unit_renderer_);
	}

	item_unit_renderer_ = new ItemUnitRenderer;
	item_unit_renderer_->Init(GetDrawBase(), GetMaxSlotNum());
}

//----------------------------------------------------------------------------
/**
*/
void uiGuildWareHouse::Release()
{
    FlushUnitDraw();
    SAFE_RELEASENDELETE(item_unit_renderer_);
}

//----------------------------------------------------------------------------
/**
*/
void uiGuildWareHouse::MessageProc(SI_MESSAGE* message)
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
    default:
        SolarDialog::MessageProc(message);
        break;
    }
}

//----------------------------------------------------------------------------
/**
*/
bool uiGuildWareHouse::OnLButtonClick(SI_MESSAGE* message)
{
    bool result = true;
    DWORD control_position = GetControlPosition_byID(message->dwCtrlID);
    switch (control_position)
    {
    case kControl_CheckBT_C001:
    case kControl_CheckBT_C002:
    case kControl_CheckBT_C003:
    case kControl_CheckBT_C000:
    case kControl_CheckBT_C004:
        {
            // 탭 버튼
            if (CheckTab(control_position - kControl_CheckBT_C001) == true)
            {
                set_current_tab(control_position - kControl_CheckBT_C001);
                ReFreshRenderSlot();
            }
        }
        break;
    case kControl_Button_B001:
    case kControl_Button_B002:
    case kControl_Button_B003:
    case kControl_Button_B004:
    case kControl_Button_B007:
    case kControl_Button_B008:
    case kControl_Button_B009:
    case kControl_Button_B010:
    case kControl_Button_B011:
    case kControl_Button_B012:
    case kControl_Button_B013:
    case kControl_Button_B014:
    case kControl_Button_B015:
    case kControl_Button_B016:
    case kControl_Button_B017:
    case kControl_Button_B018:
    case kControl_Button_B019:
    case kControl_Button_B020:
    case kControl_Button_B021:
    case kControl_Button_B022:
    case kControl_Button_B023:
    case kControl_Button_B024:
    case kControl_Button_B025:
    case kControl_Button_B026:
    case kControl_Button_B027:
        {
            // 아이템슬롯
            CControlWZ* slot_control = GetControlWZ(message->dwCtrlID);
            if (slot_control != NULL)
            {
                POSTYPE slot_position = GetRealSlotPos(static_cast<POSTYPE>(control_position));
                RECT slot_rect = slot_control->GetSizeRect();
                MouseHandler::Instance()->ItemTransaction(SI_GUILDWAREHOUSE, 
                    slot_position, 
                    &slot_rect);
            }            
        }
        break;
    case kControl_CheckBT_C006:
        {
            // 아이템 입고
            MouseHandler::Instance()->SetMouseState(MouseHandler::GUILD_DEPOSIT);

            //	입고할 아이템을 선택해 주세요.
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, 
                g_InterfaceManager.GetInterfaceString(eST_TAKE_OUT_WAREHOUSE_ASK));

            g_KeyQueueManager.DeleteMsg(eDoType_Guild_WareHouse);
            keyMsg msg;
            ZeroMemory(&msg,sizeof(msg));
            msg.dwType = eDoType_Guild_WareHouse;
            msg.wParam = uiGuildWareHouse::kDialog_ID;
            msg.DoSomething = uiGuildWareHouse::ResetMouseState;
            g_KeyQueueManager.PushBack(msg);
        }
        break;
    case kControl_CheckBT_C005:
        {
            // 아이템 출고
            MouseHandler::Instance()->SetMouseState(MouseHandler::GUILD_WITHDRAW);

            //	출고할 아이템을 선택해 주세요.
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, 
                g_InterfaceManager.GetInterfaceString(eST_WAREHOUSING_OF_GOOD_ASK));

            g_KeyQueueManager.DeleteMsg(eDoType_Guild_WareHouse);
            keyMsg msg;
            ZeroMemory(&msg,sizeof(msg));
            msg.dwType = eDoType_Guild_WareHouse;
            msg.wParam = uiGuildWareHouse::kDialog_ID;
            msg.DoSomething = uiGuildWareHouse::ResetMouseState;
            g_KeyQueueManager.PushBack(msg);
        }
        break;
    case kControl_Button_B029:
        {
            // 하임 입금
            if (CheckInputMoney(MSG_CG_GUILD_WAREHOUSE_MONEY_SYN::PUTMONEY) == true)
            {
#ifdef _NA_008678_20160203_ADD_HEIM_TRADINGVOLUME_NOTIFICATION_OPTION
				if (GlobalFunc::HeimTradingvolumeCheck(money()))
				{
					uiSystemMan* ui_system_manager_ptr = GET_DIALOG_MANAGER(uiSystemMan, UIMAN_SYSTEM);
					if (ui_system_manager_ptr)
					{
						ui_system_manager_ptr->Sys_SystemHeimTradingvolumeGuildWareHouse(GetDialogKey(), this, money());
					}
				}
				else
				{
					SendWareHouseMoney(MSG_CG_GUILD_WAREHOUSE_MONEY_SYN::PUTMONEY);
				}
#else
                SendWareHouseMoney(MSG_CG_GUILD_WAREHOUSE_MONEY_SYN::PUTMONEY);
#endif // _NA_008678_20160203_ADD_HEIM_TRADINGVOLUME_NOTIFICATION_OPTION
            }
            
        }
        break;
    case kControl_Button_B030:
        {
            // 하임 출금
            if (CheckInputMoney(MSG_CG_GUILD_WAREHOUSE_MONEY_SYN::GETMONEY) == true)
            {
                SendWareHouseMoney(MSG_CG_GUILD_WAREHOUSE_MONEY_SYN::GETMONEY);
            }
        }
        break;
    case kControl_Button_B999:
    case kControl_Button_B005:
        {
            // 닫기
            SendWareHouseEnd();
        }
        break;
    default:
        {
            CCtrlDxIMEEditBox* money_edit = 
                static_cast<CCtrlDxIMEEditBox*>(GetControlWZ_byPosition(kControl_Edit_E000));
            if (money_edit != NULL)
            {
                if (IsOverControl(money_edit) == true)
                {
                    money_edit->ShowWindowWZ(WZ_SHOW);
                }
            }
        }
        break;
    }

    return true;
}

//----------------------------------------------------------------------------
/**
*/
void uiGuildWareHouse::OnShowWindow(BOOL val)
{
	if (val)
	{
        InitMoney();

		g_InterfaceManager.ShowWindow(InterfaceManager::DIALOG_SHOP, FALSE);
		g_InterfaceManager.ShowWindow(InterfaceManager::DIALOG_EQUIP, TRUE);

		keyMsg msg;
		ZeroMemory(&msg,sizeof(msg));
		msg.dwType = eDoType_Guild_WareHouse;
        msg.wParam = uiGuildWareHouse::kDialog_ID;
		msg.DoSomething = GlobalFunc::DoGuildWareHouse;
		g_KeyQueueManager.PushBack(msg);

		CCtrlPictureWZ* LockImage1= (CCtrlPictureWZ*)GetControlWZ_byPosition(DIALOG_LOCK_PICTURE_P033);
		if(LockImage1)
		{
			LockImage1->ShowWindowWZ(WZ_HIDE);
		}
		CCtrlPictureWZ* LockImage2= (CCtrlPictureWZ*)GetControlWZ_byPosition(DIALOG_LOCK_PICTURE_P032);
		if(LockImage2)
		{
			LockImage2->ShowWindowWZ(WZ_HIDE);
		}
		CCtrlPictureWZ* LockImage3= (CCtrlPictureWZ*)GetControlWZ_byPosition(DIALOG_LOCK_PICTURE_P034);
		if(LockImage3)
		{
			LockImage3->ShowWindowWZ(WZ_HIDE);
		}

		Render_ui_Draw_List_ = false;
	}
	else
	{
        g_InterfaceManager.ShowWindow(InterfaceManager::DIALOG_EQUIP, FALSE);

        if (control_flag().find(kUseStart) != control_flag().end())
        {
            SendWareHouseEnd();
        }
        control_flag().clear();

		g_KeyQueueManager.DeleteMsg(eDoType_Guild_WareHouse);
		MouseHandler::Instance()->RemoveMouseState(MouseHandler::GUILD_WITHDRAW);
		MouseHandler::Instance()->RemoveMouseState(MouseHandler::GUILD_DEPOSIT);
	}
}

//----------------------------------------------------------------------------
/**
*/
void uiGuildWareHouse::NetworkProc(MSG_BASE* recv_packet)
{
	if(recv_packet->m_byCategory == CG_GUILD)
	{
		switch (recv_packet->m_byProtocol)
		{
		case CG_GUILD_WAREHOUSE_OPEN_ACK:
			{
				MSG_CG_GUILD_WAREHOUSE_OPEN_ACK* warehouse_packet = (MSG_CG_GUILD_WAREHOUSE_OPEN_ACK*)recv_packet;

                ShowInterfaceWithSound(TRUE);

				FlushUnitDraw();

				BitStream bitstream(warehouse_packet->m_BitStreamBuffer, 
                    warehouse_packet->m_Size, 
                    FALSE);
				SCItemSlotContainer::Serialize(bitstream, eSLOT_SERIALIZE_CLIENT_SAVE);
				//2레벨과 3레벨에서의 최대 탭곗수
                set_max_tab(warehouse_packet->m_SlotTabNum);
				set_balance(warehouse_packet->m_WarehouseMoney);

                set_current_tab(0);
                ReFreshRenderSlot();

                control_flag().insert(kUseStart);
			}
			break;

		case CG_GUILD_WAREHOUSE_OPEN_NAK:
			{
				MSG_CG_GUILD_WAREHOUSE_OPEN_NAK* warehouse_packet = (MSG_CG_GUILD_WAREHOUSE_OPEN_NAK*)recv_packet;

                GuildSystem::Function::ProcessErrorCode(warehouse_packet->m_bErrorCode);

                ShowInterface(FALSE);

                control_flag().erase(kUseStart);
			}
			break;

		case CG_GUILD_WAREHOUSE_CLOSE_ACK:
			{
				// 성공적으로 창고를 끝냈다.
				ShowInterfaceWithSound(FALSE);
				
	#if defined (_DEBUG)
				GlobalFunc::DebugOutputMsg(eOUTPUT_MSG_CHAT,_T("성공적으로 창고를 저장하였습니다."));
	#endif

                control_flag().erase(kUseStart);
                control_flag().erase(kUseEnd);
			}
			break;

		case CG_GUILD_WAREHOUSE_CLOSE_NAK:
			{
				MSG_CG_GUILD_WAREHOUSE_CLOSE_NAK* warehouse_packet = (MSG_CG_GUILD_WAREHOUSE_CLOSE_NAK*)recv_packet;

	#if defined (_DEBUG)
				GlobalFunc::DebugOutputMsg(eOUTPUT_MSG_CHAT,_T("특정이유로 창고 정보를 저장하지 못 하였습니다."));
	#endif

                ShowInterfaceWithSound(FALSE);
                GuildSystem::Function::ProcessErrorCode(warehouse_packet->m_bErrorCode);

                control_flag().erase(kUseStart);
                control_flag().erase(kUseEnd);
			}
			break;
        case CG_GUILD_WAREHOUSE_MONEY_ACK:
            {
                MSG_CG_GUILD_WAREHOUSE_MONEY_ACK* warehouse_packet = (MSG_CG_GUILD_WAREHOUSE_MONEY_ACK*)recv_packet;
                if (g_pHero != NULL)
                {
                    g_pHero->SetMoney(warehouse_packet->m_InventoryMoney);
                    set_balance(warehouse_packet->m_WarehouseMoney);
                    InitMoney();
                    control_flag().erase(kSavingMoney);
                }
            }
            break;
        case CG_GUILD_WAREHOUSE_MONEY_NAK:
            {
                MSG_CG_GUILD_WAREHOUSE_MONEY_NAK* warehouse_packet = (MSG_CG_GUILD_WAREHOUSE_MONEY_NAK*)recv_packet;
                
                control_flag().erase(kSavingMoney);

                GuildSystem::Function::ProcessErrorCode(warehouse_packet->m_dwErrorCode);
            }
            break;

		default:
			{
			}
			break;
		}
	}
}

//----------------------------------------------------------------------------
/**
*/
void uiGuildWareHouse::OnUpdateSolarDialog()
{
    UpdateMouseState();

	g_HeroInput.ClearPickingTarget();

	item_unit_renderer_->Update();

    CCtrlButtonCheckWZ* tab_button = NULL;
    for (POSTYPE position = kControl_CheckBT_C001; position < (kControl_CheckBT_C004 + 1); ++position)
    {
        tab_button = static_cast<CCtrlButtonCheckWZ*>(GetControlWZ_byPosition(position));
        if (tab_button != NULL)
        {
            if ((position - kControl_CheckBT_C001) == current_tab())
            {
                tab_button->SetCheckState(true);
            }
            else
            {
                tab_button->SetCheckState(false);
            }
        }
    }

    TCHAR money_string[INTERFACE_STRING_LENGTH] = {'\0', };
    TCHAR temp_string[INTERFACE_STRING_LENGTH] = {'\0', };
    CControlWZ* money_notice = GetControlWZ_byPosition(kControl_Text_S000);
    CCtrlStaticWZ* balance_money = 
        static_cast<CCtrlStaticWZ*>(GetControlWZ_byPosition(kControl_Text_T009));
    CCtrlDxIMEEditBox* money_edit = 
        static_cast<CCtrlDxIMEEditBox*>(GetControlWZ_byPosition(kControl_Edit_E000));
    if ((money_edit != NULL) && 
        (money_notice != NULL) && 
        (balance_money != NULL))
    {
        NUMBERFMT number_format = {0, 0, 3, _T("."), _T(","), 1};
        money_edit->SetNumberic(true);
        money_edit->GetTextWZ(money_string, INTERFACE_STRING_LENGTH - 1);
        size_t string_length = _tcslen(money_string);
        size_t money_index = 0;
        size_t temp_index = 0;
        for ( ; money_index < string_length; ++money_index)
        {
            if (money_string[money_index] >= '0' && 
                money_string[money_index] <= '9')
            {
                temp_string[temp_index] = money_string[money_index];
                ++temp_index;
            }
        }
        temp_string[temp_index] = '\0';

        set_money(_ttoi64(temp_string));

        ENUM_STATEWZ edit_show_state = money_edit->GetState();
        if (edit_show_state == WZ_SHOW)
        {
            if (money() > 0)
            {
                ::GetNumberFormat(LOCALE_SYSTEM_DEFAULT, NULL, temp_string, &number_format, money_string, sizeof(money_string));
                money_edit->SetTextWZ(money_string);
            }
            else
            {
                money_edit->SetTextWZ("");
            }
            money_notice->ShowWindowWZ(WZ_HIDE);
        }
        else if (edit_show_state == WZ_HIDE)
        {
            money_notice->ShowWindowWZ(WZ_SHOW);
        }

        Snprintf(temp_string, INTERFACE_STRING_LENGTH - 1, "%I64u", balance());
        ::GetNumberFormat(LOCALE_SYSTEM_DEFAULT, NULL, temp_string, &number_format, money_string, sizeof(money_string));        
        balance_money->SetTextWZ(money_string);
    }
}

//----------------------------------------------------------------------------
/**
*/
void uiGuildWareHouse::InitMoney()
{
    set_money(0);
    CCtrlDxIMEEditBox* money_edit = static_cast<CCtrlDxIMEEditBox*>(GetControlWZ_byPosition(kControl_Edit_E000));
    if (money_edit != NULL)
    {
        money_edit->SetNumberic(true);
        money_edit->SetTextWZ("");
        money_edit->SetLimitText(kMaxEditLength);
        money_edit->ShowWindowWZ(WZ_HIDE);
    }
}

//----------------------------------------------------------------------------
/**
*/
bool uiGuildWareHouse::CheckInputMoney(DWORD money_type)
{
    TCHAR string[INTERFACE_STRING_LENGTH] = {'\0', };
    if (money() == 0)
    {
        g_InterfaceManager.GetInterfaceString(eST_INPUT_MONEY, string);
        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, string);
        InitMoney();

        return false;
    }

    if (g_pHero == NULL)
    {
        return false;
    }

    if ((money_type == MSG_CG_GUILD_WAREHOUSE_MONEY_SYN::PUTMONEY) && 
        (money() > g_pHero->GetMoney()))
    {
        g_InterfaceManager.GetInterfaceString(eST_NOT_ENOUGH_MONEY, string);
        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, string);

        return false;
    }
    else if ((money_type == MSG_CG_GUILD_WAREHOUSE_MONEY_SYN::GETMONEY) && 
             (money() > balance()))
    {
        g_InterfaceManager.GetInterfaceString(eST_NOT_ENOUGH_MONEY, string);
        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, string);

        return false;
    }

    return true;
}

//----------------------------------------------------------------------------
/**
*/
bool uiGuildWareHouse::CheckTab(int select_tab)
{
    bool check_value = true;
    //레벨 다운되어도 꺼낼 수 있도록 풀어달라는 요청에 의해서 해제
    if (select_tab > max_tab())
    {
		CCtrlPictureWZ* LockImage1= (CCtrlPictureWZ*)GetControlWZ_byPosition(DIALOG_LOCK_PICTURE_P033);
		if(LockImage1)
		{
			LockImage1->ShowWindowWZ(WZ_SHOW);
			LockImage1->SetDiscardMessage(true);
		}
		CCtrlPictureWZ* LockImage2= (CCtrlPictureWZ*)GetControlWZ_byPosition(DIALOG_LOCK_PICTURE_P032);
		if(LockImage2)
		{
			LockImage2->ShowWindowWZ(WZ_SHOW);
			LockImage2->SetDiscardMessage(true);
		}
		CCtrlPictureWZ* LockImage3= (CCtrlPictureWZ*)GetControlWZ_byPosition(DIALOG_LOCK_PICTURE_P034);
		if(LockImage3)
		{
			LockImage3->ShowWindowWZ(WZ_SHOW);
			LockImage3->SetDiscardMessage(true);
		}
	}
	else
	{
		CCtrlPictureWZ* LockImage1= (CCtrlPictureWZ*)GetControlWZ_byPosition(DIALOG_LOCK_PICTURE_P033);
		if(LockImage1)
		{
			LockImage1->ShowWindowWZ(WZ_HIDE);
		}
		CCtrlPictureWZ* LockImage2= (CCtrlPictureWZ*)GetControlWZ_byPosition(DIALOG_LOCK_PICTURE_P032);
		if(LockImage2)
		{
			LockImage2->ShowWindowWZ(WZ_HIDE);
		}
		CCtrlPictureWZ* LockImage3= (CCtrlPictureWZ*)GetControlWZ_byPosition(DIALOG_LOCK_PICTURE_P034);
		if(LockImage3)
		{
			LockImage3->ShowWindowWZ(WZ_HIDE);
		}
	}
    
    return check_value;
}

//----------------------------------------------------------------------------
/**
*/
void uiGuildWareHouse::OnRenderSolarDialog()
{
    if (item_unit_renderer_ != NULL)
    {
	    item_unit_renderer_->Render();
    }

    uiToolTipMan* tooltip_manager = GET_DIALOG_MANAGER(uiToolTipMan, UIMAN_TOOLTIP);
    if (tooltip_manager)
    {
        int iMouseX = Mouse::Instance()->GetMouseX();
        int iMouseY = Mouse::Instance()->GetMouseY();

        POSTYPE slot_pos = kControl_Button_B001;
        for( ; slot_pos <= kControl_Button_B027; ++slot_pos)
        {
            CControlWZ* ctrl_ptr = GetControlWZ_byPosition(slot_pos);
            if (ctrl_ptr == NULL)
            {
                continue;
            }

            if (ctrl_ptr->GetState() == WZ_HIDE)
            {
                continue;
            }

            RECT rc = ctrl_ptr->GetSizeRect();

            if ((rc.left <= iMouseX) && 
                (rc.right >= iMouseX) && 
                (rc.top <= iMouseY) && 
                (rc.bottom >= iMouseY))
            {
                SCSlot& item_slot = 
                    GetSlot(static_cast<POSTYPE>(slot_pos + (current_tab() * kMaxItemSlot)));
                tooltip_manager->RegisterTooltipItem(&item_slot, FALSE);
				tooltip_manager->SetTooltipDialogID(GetDialogKey());

            }
        }
    }
}

//----------------------------------------------------------------------------
/**
*/
POSTYPE	uiGuildWareHouse::GetRealSlotPos(POSTYPE slot_position)
{
	return slot_position + (current_tab() * kMaxItemSlot);
}


//----------------------------------------------------------------------------
/**
*/
POSTYPE	uiGuildWareHouse::GetRelativeSlotPos(POSTYPE slot_position)
{
	return slot_position % kMaxItemSlot;
}

//----------------------------------------------------------------------------
/**
*/
RC::eSLOT_INSERT_RESULT uiGuildWareHouse::InsertSlot(POSTYPE slot_position, SCSlot& item_slot)
{
	/// 슬롯에 넣음
	RC::eSLOT_INSERT_RESULT insert_result = SCSlotContainer::InsertSlot(slot_position, item_slot);

	// 클라에서 따로 시리얼을 발급한다.
	DBSERIAL slot_serial = g_SlotKeyGenerator.GetKey();
	SCSlot& slot_ref = GetSlot(slot_position);
	slot_ref.SetSerial(slot_serial);

	/// 인벤토리	
	CControlWZ* solt_control = GetControlWZ_byPosition(GetRelativeSlotPos(slot_position));
	RECT control_rect = solt_control->GetSizeRect();

	if(item_unit_renderer_)
	{
		item_unit_renderer_->AddItemUnit(
            slot_ref.GetSerial(), slot_ref.GetCode(), &control_rect, &slot_ref, solt_control);
	}
	else
	{
		assert(item_unit_renderer_);
	}

	ReFreshRenderSlot();

	return insert_result;
}


//----------------------------------------------------------------------------
/**
*/
BOOL uiGuildWareHouse::DeleteSlot(POSTYPE slot_position, SCSlot* item_slot)
{
	if (IsEmpty(slot_position) == TRUE)
	{
		return FALSE;
	}

	SCItemSlot& slot_ref = (SCItemSlot&)(GetSlot(slot_position));

	if(item_unit_renderer_)
	{
		item_unit_renderer_->RemoveItemUnit(slot_ref.GetSerial());
	}
	else
	{
		assert(item_unit_renderer_);
	}

	g_SlotKeyGenerator.Restore(static_cast<DWORD>(slot_ref.GetSerial()));

	SCSlotContainer::DeleteSlot(slot_position, item_slot);

	return TRUE;
}

//----------------------------------------------------------------------------
/**
*/
void uiGuildWareHouse::OnInsert(SCSlot& IN item_slot)
{ 
	ItemManager::Instance()->AddUpdateItemSlot(&item_slot);
}

//----------------------------------------------------------------------------
/**
*/
void uiGuildWareHouse::OnDelete(SCSlot& IN item_slot)
{
	ItemManager::Instance()->RemoveUpdateItemSlot(&item_slot);
}


//----------------------------------------------------------------------------
/**
*/
void uiGuildWareHouse::ReFreshRenderSlot()
{
	if (IsVisibleWindow() == FALSE)
	{
		return;
	}

    if (item_unit_renderer_ == NULL)
    {
        return;
    }

	ItemUnitRenderer::RenderUnit* render_unit = NULL;
	RECT control_rect;
	CControlWZ* slot_control = NULL;
    int show_slot_index = current_tab() * kMaxItemSlot;
	for (int slot_index = 0; slot_index < GetMaxSlotNum(); ++slot_index)
	{
		SCSlot& item_slot = SCSlotContainer::GetSlot(slot_index);
		if (item_slot.GetSerial() > 0)
		{
            render_unit = item_unit_renderer_->GetItemUnit(item_slot.GetSerial());

            if (render_unit == NULL)
            {
                slot_control = GetControlWZ_byPosition(GetRelativeSlotPos(slot_index));
                control_rect = slot_control->GetSizeRect();

                item_unit_renderer_->AddItemUnit(
                    item_slot.GetSerial(), 
                    item_slot.GetCode(), 
                    &control_rect, 
                    &item_slot, 
                    slot_control);
            }

            if (render_unit != NULL)
            {
                if ((slot_index >= show_slot_index) && 
                    (slot_index < (show_slot_index + kMaxItemSlot)) &&
                    (MouseHandler::Instance()->GetMouseItem()->m_pRenderUnit != render_unit))
                {
                    render_unit->SetShow(TRUE);
                }
                else
                {
                    render_unit->SetShow(FALSE);
                }                
            }
		}
	}
}

//----------------------------------------------------------------------------
/**
*/
void uiGuildWareHouse::Serialize(POSTYPE slot_position, BitStream& bitstream, eSLOT_SERIALIZE serialize_type)
{
	SCItemSlotContainer::Serialize(slot_position, bitstream, serialize_type);

	if (serialize_type == eSLOT_SERIALIZE_CLIENT_SAVE)
	{
		// 클라에서 따로 시리얼을 발급한다.
		DBSERIAL slot_serial = g_SlotKeyGenerator.GetKey();
		SCSlot & slot_ref = GetSlot(slot_position);
		slot_ref.SetSerial(slot_serial);

		/// 인벤토리	
		CControlWZ* slot_control = GetControlWZ_byPosition(GetRelativeSlotPos(slot_position));
		RECT control_rect = slot_control->GetSizeRect();

		if(item_unit_renderer_)
		{
			item_unit_renderer_->AddItemUnit(
                slot_ref.GetSerial(), slot_ref.GetCode(), &control_rect, &slot_ref, slot_control);
		}
		else
		{
			assert(item_unit_renderer_);
		}
	}
}

void uiGuildWareHouse::UpdateMouseState()
{
    CCtrlButtonCheckWZ* deposit_check = 
        static_cast<CCtrlButtonCheckWZ*>(GetControlWZ_byPosition(kControl_CheckBT_C006));
    CCtrlButtonCheckWZ* withdraw_check = 
        static_cast<CCtrlButtonCheckWZ*>(GetControlWZ_byPosition(kControl_CheckBT_C005));
    if ((deposit_check == NULL) || 
        (withdraw_check == NULL))
    {
        assert(deposit_check);
        assert(withdraw_check);
    }


    MouseHandler::MOUSE_STATE mouse_sate = MouseHandler::Instance()->GetMouseState();
    if ((mouse_sate == MouseHandler::DEPOSIT) || 
        (mouse_sate == MouseHandler::GUILD_DEPOSIT))
    {
        // 입고
        deposit_check->SetCheckState(true);
        withdraw_check->SetCheckState(false);
    }
    else if ((mouse_sate == MouseHandler::WITHDRAW) || 
        (mouse_sate == MouseHandler::GUILD_WITHDRAW))
    {
        // 출고
        deposit_check->SetCheckState(false);
        withdraw_check->SetCheckState(true);
    }
    else
    {
        deposit_check->SetCheckState(false);
        withdraw_check->SetCheckState(false);
    }
}

void uiGuildWareHouse::ResetMouseState(DWORD l_param, DWORD r_param)
{
    MouseHandler::Instance()->RemoveMouseState(MouseHandler::GUILD_WITHDRAW);
    MouseHandler::Instance()->RemoveMouseState(MouseHandler::GUILD_DEPOSIT);    

    g_KeyQueueManager.DeleteMsg(eDoType_Guild_WareHouse);

    keyMsg msg;
    ZeroMemory(&msg,sizeof(msg));
    msg.dwType = eDoType_Guild_WareHouse;
    msg.wParam = uiGuildWareHouse::kDialog_ID;
    msg.DoSomething = GlobalFunc::DoGuildWareHouse;
    g_KeyQueueManager.PushBack(msg);
}

#ifdef _NA_008678_20160203_ADD_HEIM_TRADINGVOLUME_NOTIFICATION_OPTION
void uiGuildWareHouse::HeimTradingvolumeCancel()
{
	InitMoney();
	control_flag().erase(uiGuildWareHouse::kSavingMoney);
}
#endif // _NA_008678_20160203_ADD_HEIM_TRADINGVOLUME_NOTIFICATION_OPTION

#endif //_NA_003923_20120130_GUILD_RENEWAL_MOD_WAREHOUSE_SLOT_NUM
