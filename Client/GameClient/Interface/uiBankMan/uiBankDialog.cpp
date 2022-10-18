#include "SunClientPrecompiledHeader.h"
#ifdef _NA_003923_20120130_GUILD_RENEWAL_MOD_WAREHOUSE_SLOT_NUM
#include "uiBankMan/uiBankMan.h"
#include "uiBankDialog.h"
#include "uiPasswordMan/uiPassWordMan.h"
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

//----------------------------------------------------------------------------
/**
*/
uiBankDialog::uiBankDialog(InterfaceManager* ui_manager)
	:uiBase(ui_manager)
{
    item_unit_renderer_ = NULL;
}


//----------------------------------------------------------------------------
/**
*/
uiBankDialog::~uiBankDialog(void) 
{
}


//----------------------------------------------------------------------------
/**
*/
void uiBankDialog::Init(CDrawBase* draw_base)
{
	uiBase::Init(draw_base);
    SCItemSlotContainer::Init(MAX_WAREHOUSE_SLOT_NUM, SI_WAREHOUSE);

    RegisterControlPosition(StrToWzID("C001"), kControl_CheckBT_C001);
    RegisterControlPosition(StrToWzID("C004"), kControl_CheckBT_C004);
    RegisterControlPosition(StrToWzID("C002"), kControl_CheckBT_C002);
    RegisterControlPosition(StrToWzID("C003"), kControl_CheckBT_C003);
    RegisterControlPosition(StrToWzID("C000"), kControl_CheckBT_C000);
    RegisterControlPosition(StrToWzID("C005"), kControl_CheckBT_C005);
    RegisterControlPosition(StrToWzID("C006"), kControl_CheckBT_C006);
    RegisterControlPosition(StrToWzID("B999"), kControl_Button_B999);
    RegisterControlPosition(StrToWzID("B001"), kControl_Button_B001);
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
    RegisterControlPosition(StrToWzID("B000"), kControl_Button_B000);
    RegisterControlPosition(StrToWzID("B006"), kControl_Button_B006);
    RegisterControlPosition(StrToWzID("T009"), kControl_Text_T009);
    RegisterControlPosition(StrToWzID("S000"), kControl_Text_S000);
    RegisterControlPosition(StrToWzID("E000"), kControl_Edit_E000);
    RegisterControlPosition(StrToWzID("T999"), kControl_Text_T999);
	RegisterControlPosition(StrToWzID("P033"), DIALOG_LOCK_PICTURE_P033);
	RegisterControlPosition(StrToWzID("P032"), DIALOG_LOCK_PICTURE_P032);
	RegisterControlPosition(StrToWzID("P034"), DIALOG_LOCK_PICTURE_P034);


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
void uiBankDialog::FlushUnitDraw()
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
void uiBankDialog::SendWareHouseStart()
{
    uiBankMan* ui_bank_manager = static_cast<uiBankMan*>(GetMan());
    if (ui_bank_manager != NULL)
    {
        ui_bank_manager->Send_CG_WAREHOUSE_START_SYN();
    }	
}

//----------------------------------------------------------------------------
/**
*/
void uiBankDialog::SendWareHouseEnd()
{
    STLX_SET<BankControlFlag>::iterator end_itr = control_flag().end();
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
	MSG_CG_WAREHOUSE_END_SYN SendEndPacket;
	GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendEndPacket, sizeof(SendEndPacket));
}

//----------------------------------------------------------------------------
/**
*/
void uiBankDialog::SendWareHouseMoney(BYTE money_type)
{
    STLX_SET<BankControlFlag>::iterator end_itr = control_flag().end();
    if (control_flag().find(kSavingMoney) != end_itr)
    {
        return;
    }

    MSG_CG_WAREHOUSE_MONEY_SYN money_packet;
    money_packet.m_byType = money_type;
    money_packet.m_Money = money();

    control_flag().insert(kSavingMoney);

    GlobalFunc::SendPacket(CI_GAMESERVERIDX, &money_packet, sizeof(money_packet));
}

//----------------------------------------------------------------------------
/**
*/
void uiBankDialog::InitContainer()
{
    InitRenderer();
}

//----------------------------------------------------------------------------
/**
*/
void uiBankDialog::ReleaseContainer()
{
    FlushUnitDraw();

    SAFE_RELEASENDELETE(item_unit_renderer_);
}

//----------------------------------------------------------------------------
/**
*/
void uiBankDialog::InitRenderer()
{
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
void uiBankDialog::Release()
{
    SAFE_RELEASENDELETE(item_unit_renderer_);
}

//----------------------------------------------------------------------------
/**
*/
void uiBankDialog::MessageProc(SI_MESSAGE* message)
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
bool uiBankDialog::OnLButtonClick(SI_MESSAGE* message)
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
            set_current_tab(control_position - kControl_CheckBT_C001);
            ReFreshRenderSlot();
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
                MouseHandler::Instance()->ItemTransaction(SI_WAREHOUSE, 
                    slot_position, 
                    &slot_rect);
            }            
        }
        break;
    case kControl_CheckBT_C006:
        {
            // 아이템 입고
            MouseHandler::Instance()->SetMouseState(MouseHandler::DEPOSIT);

            //	입고할 아이템을 선택해 주세요.
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, 
                g_InterfaceManager.GetInterfaceString(eST_TAKE_OUT_WAREHOUSE_ASK));

            g_KeyQueueManager.DeleteMsg(eDoType_dialog_bank);
            keyMsg msg;
            ZeroMemory(&msg,sizeof(msg));
            msg.dwType = eDoType_dialog_bank;
            msg.wParam = IM_BANK_MANAGER::BANK_DIALOG;
            msg.DoSomething = uiBankDialog::ResetMouseState;
            g_KeyQueueManager.PushBack(msg);
        }
        break;
    case kControl_CheckBT_C005:
        {
            // 아이템 출고
            MouseHandler::Instance()->SetMouseState(MouseHandler::WITHDRAW);

            //	출고할 아이템을 선택해 주세요.
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, 
                g_InterfaceManager.GetInterfaceString(eST_WAREHOUSING_OF_GOOD_ASK));

            g_KeyQueueManager.DeleteMsg(eDoType_dialog_bank);
            keyMsg msg;
            ZeroMemory(&msg,sizeof(msg));
            msg.dwType = eDoType_dialog_bank;
            msg.wParam = IM_BANK_MANAGER::BANK_DIALOG;
            msg.DoSomething = uiBankDialog::ResetMouseState;
            g_KeyQueueManager.PushBack(msg);
        }
        break;
    case kControl_Button_B029:
        {
            // 하임 입금
            if (CheckInputMoney(MSG_CG_WAREHOUSE_MONEY_SYN::PUTMONEY) == true)
            {
                SendWareHouseMoney(MSG_CG_WAREHOUSE_MONEY_SYN::PUTMONEY);
            }
            
        }
        break;
    case kControl_Button_B030:
        {
            // 하임 출금
            if (CheckInputMoney(MSG_CG_WAREHOUSE_MONEY_SYN::GETMONEY) == true)
            {
                SendWareHouseMoney(MSG_CG_WAREHOUSE_MONEY_SYN::GETMONEY);
            }
        }
        break;
    case kControl_Button_B000:
        {
            // 잠김 버튼
            // 5581 창고를 열림 상태로 변경하시겠습니까?
            AskPasswordLock(5581);
        }
        break;
    case kControl_Button_B006:
        {
            // 열림 버튼
            DWORD string_code;
            if (GetBankLockState() == MSG_CG_WAREHOUSE_START_ACK::ENABLEnEMPTY)
            {
                // 5584 창고 보호 기능을 사용하시겠습니까? 비밀번호를 설정할 경우, 게임 종료 시 자동 잠금 상태로 변경됩니다.
                string_code = 5584;
            }
            else
            {
                // 5580 창고를 잠금 상태로 변경하시겠습니까?							
                string_code = 5580;
            }
            AskPasswordLock(string_code);
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
void uiBankDialog::OnShowWindow(BOOL val)
{
	if (val)
	{
        InitMoney();
        InitLockButtonState();

		g_InterfaceManager.ShowWindow(InterfaceManager::DIALOG_SHOP, FALSE);
		g_InterfaceManager.ShowWindow(InterfaceManager::DIALOG_EQUIP, TRUE);

		keyMsg msg;
		ZeroMemory(&msg,sizeof(msg));
		msg.dwType=eDoType_dialog_bank;
		msg.wParam=IM_BANK_MANAGER::BANK_DIALOG;
		msg.DoSomething=GlobalFunc::DoBank;
		g_KeyQueueManager.PushBack(msg);
	}
	else
	{
        g_InterfaceManager.ShowWindow(InterfaceManager::DIALOG_EQUIP, FALSE);

        if (control_flag().find(kUseStart) != control_flag().end())
        {
            SendWareHouseEnd();
        }
        control_flag().clear();

		g_KeyQueueManager.DeleteMsg(eDoType_dialog_bank);
		MouseHandler::Instance()->RemoveMouseState(MouseHandler::WITHDRAW);
		MouseHandler::Instance()->RemoveMouseState(MouseHandler::DEPOSIT);
	}
}


//----------------------------------------------------------------------------
/**
*/
void uiBankDialog::NetworkProc(MSG_BASE* recv_packet)
{
	if(recv_packet->m_byCategory == CG_WAREHOUSE)
	{
        uiBankMan* bank_manager = static_cast<uiBankMan*>(GetMan());
        if (bank_manager != NULL)
        {
            bank_manager->ClearWaitPacket();
        }

		switch (recv_packet->m_byProtocol)
		{
		case CG_WAREHOUSE_START_ACK:
			{
				MSG_CG_WAREHOUSE_START_ACK* warehouse_packet = (MSG_CG_WAREHOUSE_START_ACK*)recv_packet;

				FlushUnitDraw();

				BitStream bitstream(warehouse_packet->m_BitStreamBuffer, 
                    warehouse_packet->m_Size, 
                    FALSE);
				SCItemSlotContainer::Serialize(bitstream, eSLOT_SERIALIZE_CLIENT_SAVE);
				set_balance(warehouse_packet->m_WarehouseMoney);
                SetBankLockState(warehouse_packet->m_byWarehouseLock);

				ShowInterfaceWithSound(TRUE);

                set_current_tab(0);
                ReFreshRenderSlot();

                control_flag().insert(kUseStart);
			}
			break;

		case CG_WAREHOUSE_START_NAK:
			{
				MSG_CG_WAREHOUSE_START_NAK* warehouse_packet = (MSG_CG_WAREHOUSE_START_NAK*)recv_packet;

				TCHAR message[INTERFACE_STRING_LENGTH] = {'\0', };
				
				g_InterfaceManager.GetStringWarehouseError(
                    warehouse_packet->m_dwErrorCode, message, INTERFACE_STRING_LENGTH);
				GlobalFunc::OutputMsg_Error(eOUTPUT_MSG_CHAT, message);

                ShowInterface(FALSE);

                control_flag().erase(kUseStart);
			}
			break;

		case CG_WAREHOUSE_END_ACK:
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

		case CG_WAREHOUSE_END_NAK:
			{
				MSG_CG_WAREHOUSE_END_NAK* warehouse_packet = (MSG_CG_WAREHOUSE_END_NAK*)recv_packet;

	#if defined (_DEBUG)
				GlobalFunc::DebugOutputMsg(eOUTPUT_MSG_CHAT,_T("특정이유로 창고 정보를 저장하지 못 하였습니다."));
	#endif

                TCHAR message[INTERFACE_STRING_LENGTH] = {'\0', };
                BOOL result = g_InterfaceManager.GetStringWarehouseError(
                    warehouse_packet->m_dwErrorCode, message, INTERFACE_STRING_LENGTH);
                if (result == TRUE)
                {
                    GlobalFunc::OutputMsg_Error(eOUTPUT_MSG_CHAT, message);
                }
                else
                {
                    ItemManager::Instance()->NoticeReasonItemErrorCode(warehouse_packet->m_dwErrorCode);
                }

                control_flag().erase(kUseStart);
                control_flag().erase(kUseEnd);
			}
			break;
        case CG_WAREHOUSE_MONEY_ACK:
            {
                MSG_CG_WAREHOUSE_MONEY_ACK* warehouse_packet = (MSG_CG_WAREHOUSE_MONEY_ACK*)recv_packet;
                if (g_pHero != NULL)
                {
                    g_pHero->SetMoney(warehouse_packet->m_InventoryMoney);
                    set_balance(warehouse_packet->m_WarehouseMoney);
                    InitMoney();
                    control_flag().erase(kSavingMoney);
                }
            }
            break;
        case CG_WAREHOUSE_MONEY_NAK:
            {
                MSG_CG_WAREHOUSE_MONEY_NAK* warehouse_packet = (MSG_CG_WAREHOUSE_MONEY_NAK*)recv_packet;
                
                control_flag().erase(kSavingMoney);

                TCHAR message[INTERFACE_STRING_LENGTH] = {'\0', };
                BOOL result = g_InterfaceManager.GetStringWarehouseError(
                    warehouse_packet->m_dwErrorCode, message, INTERFACE_STRING_LENGTH);
                if (result == TRUE)
                {
                    GlobalFunc::OutputMsg_Error(eOUTPUT_MSG_CHAT, message);
                }
                else
                {
                    ItemManager::Instance()->NoticeReasonItemErrorCode(warehouse_packet->m_dwErrorCode);
                }
            }
            break;
        case CG_WAREHOUSE_PWD_SETTING_ACK:
        case CG_WAREHOUSE_PWD_SETTING_NAK:
        case CG_WAREHOUSE_PWD_LOCKING_ACK:
        case CG_WAREHOUSE_PWD_LOCKING_NAK:
        case CG_WAREHOUSE_PWD_CHECK_ACK:
        case CG_WAREHOUSE_PWD_CHECK_NAK:
            {
                NetworkProc_Password(recv_packet);
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
void uiBankDialog::NetworkProc_Password(MSG_BASE* recv_packet)
{
    if(recv_packet->m_byCategory != CG_WAREHOUSE)
    {
        return;
    }

    uiBankMan* ui_bank_manager = static_cast<uiBankMan*>(GetMan());
    if (ui_bank_manager == NULL)
    {
        return;
    }

    switch (recv_packet->m_byProtocol)
    {
    case CG_WAREHOUSE_PWD_LOCKING_ACK:
        {
            MSG_CG_WAREHOUSE_PWD_LOCKING_ACK* warehouse_pwd_packet = (MSG_CG_WAREHOUSE_PWD_LOCKING_ACK*)recv_packet;

            TCHAR message[INTERFACE_STRING_LENGTH] = {'\0', };

            switch (warehouse_pwd_packet->m_byRetCode)
            {
            case MSG_CG_WAREHOUSE_PWD_LOCKING_ACK::RET_SUCCESS_LOCKED:
                {
                    SetBankLockState(MSG_CG_WAREHOUSE_START_ACK::DISABLEDnEXIST);
                    InitLockButtonState();
                    uiSystemMan* pSystemMan =
                        static_cast<uiSystemMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM));
                    if(pSystemMan)
                    {
                        //	창고가 잠금 상태로 변경되었습니다.
                        g_InterfaceManager.GetInterfaceString(5573, message, INTERFACE_STRING_LENGTH);
                        pSystemMan->Sys_Verify(message, NULL);
                    }
                    ui_bank_manager->ClearPassword();
                }
                break;

            case MSG_CG_WAREHOUSE_PWD_LOCKING_ACK::RET_NEED_PWD_FOR_LOCKING:
                {
                    ui_bank_manager->Send_CG_WAREHOUSE_PWD_SETTING_SYN(MSG_CG_WAREHOUSE_PWD_SETTING_SYN::SYNC_FIRST);
                }
                break;

            case MSG_CG_WAREHOUSE_PWD_LOCKING_ACK::RET_NEED_PWD_FOR_UNLOCKING:
                {
                    ui_bank_manager->Send_CG_WAREHOUSE_PWD_CHECK_SYN(MSG_CG_WAREHOUSE_PWD_CHECK_SYN::SYNC_FIRST);
                }
                break;
            }
        }
        break;
    case CG_WAREHOUSE_PWD_LOCKING_NAK:
        {
            MSG_CG_WAREHOUSE_PWD_LOCKING_NAK* warehouse_pwd_packet = (MSG_CG_WAREHOUSE_PWD_LOCKING_NAK*)recv_packet;

            TCHAR message[INTERFACE_STRING_LENGTH] = {'\0', };

            ui_bank_manager->ClearPassword();
            g_InterfaceManager.GetStringWarehouseError(warehouse_pwd_packet->m_byErrCode, message, INTERFACE_STRING_LENGTH);
            GlobalFunc::OutputMsg_Error(eOUTPUT_MSG_CHAT, message);
        }
        break;
    case CG_WAREHOUSE_PWD_SETTING_ACK:
        {
            TCHAR message[INTERFACE_STRING_LENGTH] = {'\0', };
            MSG_CG_WAREHOUSE_PWD_SETTING_ACK* warehouse_pwd_packet = (MSG_CG_WAREHOUSE_PWD_SETTING_ACK*)recv_packet;

            switch(warehouse_pwd_packet->m_byRetCode)
            {
            case MSG_CG_WAREHOUSE_PWD_SETTING_ACK::RET_FAIL:
            case MSG_CG_WAREHOUSE_PWD_SETTING_ACK::RET_CANCELED:
                ui_bank_manager->ClearPassword();
                break;

            case MSG_CG_WAREHOUSE_PWD_SETTING_ACK::RET_SUCCESS:
                {
                    ui_bank_manager->ClearPassword();
                    uiSystemMan* pSystemMan =
                        static_cast<uiSystemMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM));
                    if(pSystemMan)
                    {
                        //	비밀번호가 설정되었습니다.
                        g_InterfaceManager.GetInterfaceString(5556, message, INTERFACE_STRING_LENGTH);
                        pSystemMan->Sys_Verify(message, NULL);
                    }
                    SetBankLockState(MSG_CG_WAREHOUSE_START_ACK::ENABLEnEXIST);
                    InitLockButtonState();
                }
                break;

            case MSG_CG_WAREHOUSE_PWD_SETTING_ACK::RET_NEED_SSN:
                {
                    //	주민등록번호 뒷자리 입력
                    SetPasswordDialog(5579, eBankDlgState_Setting_SSN);
                }
                break;

            case MSG_CG_WAREHOUSE_PWD_SETTING_ACK::RET_INVALID_SSN:
                {
                    uiSystemMan* pSystemMan =
                        static_cast<uiSystemMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM));
                    if(pSystemMan)
                    {
                        ui_bank_manager->SetPasswordState(eBankDlgState_Setting_SSN);
                        //	주민등록번호가 일치하지 않습니다.
                        g_InterfaceManager.GetInterfaceString(5845, message, INTERFACE_STRING_LENGTH);
                        pSystemMan->Sys_Verify(message, GlobalFunc::_fn_uiBankPassword_invalidSSN);
                    }
                }
                break;

            case MSG_CG_WAREHOUSE_PWD_SETTING_ACK::RET_NEED_PWD:
                {
                    //	비밀번호를 설정해주세요
                    SetPasswordDialog(5578, eBankDlgState_Setting_PasswordCmp);
                }
                break;

            case MSG_CG_WAREHOUSE_PWD_SETTING_ACK::RET_INVALID_PWD:
                {
                    uiSystemMan* pSystemMan =
                        static_cast<uiSystemMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM));
                    if(pSystemMan)
                    {
                        ui_bank_manager->SetPasswordState(eBankDlgState_Lock);
                        //	비밀번호가 틀렸습니다.
                        g_InterfaceManager.GetInterfaceString(5555, message, INTERFACE_STRING_LENGTH);
                        pSystemMan->Sys_Verify(message, GlobalFunc::_fn_uiBankPassword_invalidPassword);
                    }
                }
                break;

            case MSG_CG_WAREHOUSE_PWD_SETTING_ACK::RET_SUCCESS_CLEAR:
                {
                    SetBankLockState(MSG_CG_WAREHOUSE_START_ACK::ENABLEnEMPTY);
                    InitLockButtonState();
                    uiSystemMan* pSystemMan =
                        static_cast<uiSystemMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM));
                    if(pSystemMan)
                    {
                        //	잠금 기능을 [사용 안 함]으로 변경하였습니다.
                        g_InterfaceManager.GetInterfaceString(5577, message, INTERFACE_STRING_LENGTH);
                        pSystemMan->Sys_Verify(message, NULL);
                    }
                }
                break;
            }
        }
        break;
    case CG_WAREHOUSE_PWD_SETTING_NAK:
        {
            ui_bank_manager->ClearPassword();
        }
        break;    
    case CG_WAREHOUSE_PWD_CHECK_ACK:
        {
            TCHAR message[INTERFACE_STRING_LENGTH] = {'\0', };
            MSG_CG_WAREHOUSE_PWD_CHECK_ACK* warehouse_pwd_packet = (MSG_CG_WAREHOUSE_PWD_CHECK_ACK*)recv_packet;

            ui_bank_manager->ClearPassword();

            switch(warehouse_pwd_packet->m_byRetCode)
            {
            case MSG_CG_WAREHOUSE_PWD_CHECK_ACK::RET_SUCCESS:
                {
                    SetBankLockState(MSG_CG_WAREHOUSE_START_ACK::ENABLEnEXIST);
                    InitLockButtonState();
                    uiSystemMan* pSystemMan =
                        static_cast<uiSystemMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM));
                    if(pSystemMan)
                    {
                        //	창고가 열림 상태로 변경되었습니다
                        g_InterfaceManager.GetInterfaceString(5575, message, INTERFACE_STRING_LENGTH);
                        pSystemMan->Sys_Verify(message, NULL);
                    }
                }
                break;

            case MSG_CG_WAREHOUSE_PWD_CHECK_ACK::RET_INVALID_PWD:
                {
                    uiSystemMan* pSystemMan =
                        static_cast<uiSystemMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM));
                    if(pSystemMan)
                    {
                        ui_bank_manager->SetPasswordState(eBankDlgState_Check_Unlock);
                        //	비밀번호가 틀렸습니다.
                        g_InterfaceManager.GetInterfaceString(5555, message, INTERFACE_STRING_LENGTH);
                        pSystemMan->Sys_Verify(message, GlobalFunc::_fn_uiBankPassword_invalidPassword);
                    }
                }
                break;

            case MSG_CG_WAREHOUSE_PWD_CHECK_ACK::RET_NEED_PWD:
                {
                    //	비밀번호를 입력해주세요
                    SetPasswordDialog(5551, eBankDlgState_Check_Unlock);
                }
                break;

            case MSG_CG_WAREHOUSE_PWD_CHECK_ACK::RET_NEED_SSN:
                {
                    //	주민등록번호 뒷자리 입력
                    SetPasswordDialog(5579, eBankDlgState_Check_Clear);
                }
                break;

            case MSG_CG_WAREHOUSE_PWD_CHECK_ACK::RET_INVALID_SSN:
                {
                    uiSystemMan* pSystemMan =
                        static_cast<uiSystemMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM));
                    if(pSystemMan)
                    {
                        ui_bank_manager->SetPasswordState(eBankDlgState_Check_Clear);
                        //	주민등록번호가 일치하지 않습니다.
                        g_InterfaceManager.GetInterfaceString(5845, message, INTERFACE_STRING_LENGTH);
                        pSystemMan->Sys_Verify(message, GlobalFunc::_fn_uiBankPassword_invalidSSN);
                    }
                }
                break;
            }
        }
        break;
    }
}

//----------------------------------------------------------------------------
/**
*/
void uiBankDialog::SetBankLockState(int lock_state)
{
    uiBankMan* ui_bank_manager = static_cast<uiBankMan*>(GetMan());
    if (ui_bank_manager != NULL)
    {
        ui_bank_manager->SetBankLockState(lock_state);
    }
}

//----------------------------------------------------------------------------
/**
*/
int uiBankDialog::GetBankLockState()
{
    int lock_state = MSG_CG_WAREHOUSE_START_ACK::ENABLEnEMPTY;

    uiBankMan* ui_bank_manager = static_cast<uiBankMan*>(GetMan());
    if (ui_bank_manager != NULL)
    {
        lock_state = ui_bank_manager->GetBankLockState();
    }

    return lock_state;
}

//----------------------------------------------------------------------------
/**
*/
void uiBankDialog::AskPasswordLock(DWORD string_code)
{
    uiSystemMan* ui_system_manager = GET_DIALOG_MANAGER(uiSystemMan, UIMAN_SYSTEM);
    if (ui_system_manager != NULL)
    {
        TCHAR message[INTERFACE_STRING_LENGTH] = {'\0', };
        g_InterfaceManager.GetInterfaceString(string_code, message, INTERFACE_STRING_LENGTH);
        ui_system_manager->Sys_Confirm(message, GlobalFunc::_fn_uiBankPasswordLockingSyn);
    }
}

//----------------------------------------------------------------------------
/**
*/
void uiBankDialog::SetPasswordDialog(int string_code, int password_state)
{
    uiBankMan* ui_bank_manager = static_cast<uiBankMan*>(GetMan());
    if (ui_bank_manager == NULL)
    {
        return;
    }

    int count_min = 0;
    int count_max = MAX_SSN_COUNT;

    ui_bank_manager->SetPasswordState(password_state);
    if(password_state == eBankDlgState_Setting_SSN || password_state == eBankDlgState_Check_Clear)
    { 
        count_min = 0; 
        count_max = MAX_SSN_COUNT;
    }
    else
    { 
        count_min = 4; 
        count_max = MAX_WAREHOUSE_INVENTORY_PWD_LENGTH;
    }

    BOOL show_change_button = TRUE;

    show_change_button = (ui_bank_manager->GetBankLockState() & MSG_CG_WAREHOUSE_START_ACK::DISABLED);

    uiPasswordMan* ui_password_manager = GET_DIALOG_MANAGER(uiPasswordMan, UIMAN_PASSWORD);
    assert(ui_password_manager);
    
    if (ui_password_manager != NULL)
    {
        ui_password_manager->PasswordDlg(string_code, 
            GlobalFunc::_fn_uiBankPassword, 
            count_min, 
            count_max, 
            show_change_button, 
            ePassWordCalledState_WareHouse);
    }
}

//----------------------------------------------------------------------------
/**
*/
void uiBankDialog::InitLockButtonState()
{
    CControlWZ* lock_button = GetControlWZ_byPosition(kControl_Button_B000);
    CControlWZ* unlock_button = GetControlWZ_byPosition(kControl_Button_B006);
#ifdef _GS_GSP_REMOVE_LOCKBTN_
    //락버튼 없다
    if (lock_button != NULL)
    {
        lock_button->ShowWindowWZ(WZ_HIDE);
    }
    if (unlock_button != NULL)
    {
        unlock_button->ShowWindowWZ(WZ_HIDE);
    }
#else
    if( GetBankLockState() == MSG_CG_WAREHOUSE_START_ACK::DISABLEDnEXIST )
    {
        if (lock_button != NULL)
        {
            lock_button->ShowWindowWZ(WZ_SHOW);
        }
        if (unlock_button != NULL)
        {
            unlock_button->ShowWindowWZ(WZ_HIDE);
        }
    }
    else
    {
        if (lock_button != NULL)
        {
            lock_button->ShowWindowWZ(WZ_HIDE);
        }
        if (unlock_button != NULL)
        {
            unlock_button->ShowWindowWZ(WZ_SHOW);
        }
    }
#endif//_GS_GSP_REMOVE_LOCKBTN_

    CCtrlStaticWZ* title_text = static_cast<CCtrlStaticWZ*>(GetControlWZ_byPosition(kControl_Text_T999));
    if (title_text != NULL)
    {
        TCHAR message[INTERFACE_STRING_LENGTH] = {'\0', };
        if (GetBankLockState() == MSG_CG_WAREHOUSE_START_ACK::DISABLEDnEXIST)
        {
            // 창고(잠김)
            g_InterfaceManager.GetInterfaceString(1189, message, INTERFACE_STRING_LENGTH);
        }
        else
        {
            // 창고(열림)
            g_InterfaceManager.GetInterfaceString(1190, message, INTERFACE_STRING_LENGTH);
        }

        GlobalFunc::SetCaption(title_text, message);
    }
}

//----------------------------------------------------------------------------
/**
*/
void uiBankDialog::OnUpdateSolarDialog()
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
void uiBankDialog::InitMoney()
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
bool uiBankDialog::CheckInputMoney(DWORD money_type)
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

    if ((money_type == MSG_CG_WAREHOUSE_MONEY_SYN::PUTMONEY) && 
        (money() > g_pHero->GetMoney()))
    {
        g_InterfaceManager.GetInterfaceString(eST_NOT_ENOUGH_MONEY, string);
        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, string);

        return false;
    }
    else if ((money_type == MSG_CG_WAREHOUSE_MONEY_SYN::GETMONEY) && 
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
void uiBankDialog::OnRenderSolarDialog()
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
        for( ; slot_pos <= kControl_Button_B006; ++slot_pos)
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
                if (slot_pos == kControl_Button_B000)
                {
                    // 잠김
                    tooltip_manager->RegisterTooltipForWareNInventoryLock(5569);
					tooltip_manager->SetTooltipDialogID(GetDialogKey());
                }
                else if (slot_pos == kControl_Button_B006)
                {
                    tooltip_manager->RegisterTooltipForWareNInventoryLock(5570);
					tooltip_manager->SetTooltipDialogID(GetDialogKey());
                }
                else
                {
                    SCSlot& item_slot = 
                        GetSlot(static_cast<POSTYPE>(slot_pos + (current_tab() * kMaxItemSlot)));
                    tooltip_manager->RegisterTooltipItem(&item_slot, FALSE);
					tooltip_manager->SetTooltipDialogID(GetDialogKey());

                }
            }
        }
    }
}

//----------------------------------------------------------------------------
/**
*/
POSTYPE	uiBankDialog::GetRealSlotPos(POSTYPE slot_position)
{
	return slot_position + (current_tab() * kMaxItemSlot);
}


//----------------------------------------------------------------------------
/**
*/
POSTYPE	uiBankDialog::GetRelativeSlotPos(POSTYPE slot_position)
{
	return slot_position % kMaxItemSlot;
}

//----------------------------------------------------------------------------
/**
*/
RC::eSLOT_INSERT_RESULT uiBankDialog::InsertSlot(POSTYPE slot_position, SCSlot& item_slot)
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
BOOL uiBankDialog::DeleteSlot(POSTYPE slot_position, SCSlot* item_slot)
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
void uiBankDialog::OnInsert(SCSlot& IN item_slot)
{ 
	ItemManager::Instance()->AddUpdateItemSlot(&item_slot);
}

//----------------------------------------------------------------------------
/**
*/
void uiBankDialog::OnDelete(SCSlot& IN item_slot)
{
	ItemManager::Instance()->RemoveUpdateItemSlot(&item_slot);
}


//----------------------------------------------------------------------------
/**
*/
void uiBankDialog::ReFreshRenderSlot()
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
void uiBankDialog::Serialize(POSTYPE slot_position, BitStream& bitstream, eSLOT_SERIALIZE serialize_type)
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

void uiBankDialog::UpdateMouseState()
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

void uiBankDialog::ResetMouseState(DWORD l_param, DWORD r_param)
{
    MouseHandler::Instance()->RemoveMouseState(MouseHandler::WITHDRAW);
    MouseHandler::Instance()->RemoveMouseState(MouseHandler::DEPOSIT);    

    g_KeyQueueManager.DeleteMsg(eDoType_dialog_bank);

    keyMsg msg;
    ZeroMemory(&msg,sizeof(msg));
    msg.dwType = eDoType_dialog_bank;
    msg.wParam = IM_BANK_MANAGER::BANK_DIALOG;
    msg.DoSomething = GlobalFunc::DoBank;
    g_KeyQueueManager.PushBack(msg);
}

#endif //_NA_003923_20120130_GUILD_RENEWAL_MOD_WAREHOUSE_SLOT_NUM
