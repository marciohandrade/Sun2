#include "SunClientPrecompiledHeader.h"
#ifdef _NA004034_20120102_POINT_WALLET_SYSTEM

#include "uiCurrencyItemInputNumber.h"
#include "ItemManager.h"
#include "GlobalFunc.h"
#include "SCSlotContainer.h"
#include "PointSystem/PacketStruct_CG_PointSystem.h"

const DWORD uiCurrencyItemInputNumber::kDialog_ID = g_IDGenerator.GenerateID();

uiCurrencyItemInputNumber::uiCurrencyItemInputNumber(InterfaceManager* interface_manager)
    :uiBase(interface_manager)
{

}

uiCurrencyItemInputNumber::~uiCurrencyItemInputNumber()
{

}

void uiCurrencyItemInputNumber::Init(CDrawBase* draw_base)
{
    RegisterControlPosition(StrToWzID("T999"), kControl_Text_T999);
    RegisterControlPosition(StrToWzID("T001"), kControl_Text_T001);
    RegisterControlPosition(StrToWzID("B001"), kControl_Button_B001);
    RegisterControlPosition(StrToWzID("B005"), kControl_Button_B005);
    RegisterControlPosition(StrToWzID("TI00"), kControl_Button_TI00);
    RegisterControlPosition(StrToWzID("TI01"), kControl_Button_TI01);
    RegisterControlPosition(StrToWzID("B004"), kControl_Button_B004);
    RegisterControlPosition(StrToWzID("E000"), kControl_Edit_E000);

    CCtrlStaticWZ* text_control = NULL;
    text_control = static_cast<CCtrlStaticWZ*>(GetControlWZ_byPosition(kControl_Text_T999));
    if (text_control != NULL)
    {
        // 47658 화폐 지갑
        text_control->SetTextID(47658);
    }
    text_control = static_cast<CCtrlStaticWZ*>(GetControlWZ_byPosition(kControl_Text_T001));
    if (text_control != NULL)
    {
        // 47659 지갑에 넣을 수량을 입력하십시오.
        text_control->SetTextID(47659);
    }
}

void uiCurrencyItemInputNumber::Release()
{

}

void uiCurrencyItemInputNumber::OnShowWindow(BOOL is_show)
{
    if (is_show == TRUE)
    {
        keyMsg key_message;
        ZeroMemory(&key_message, sizeof(key_message));
        key_message.dwType = uiCurrencyItemInputNumber::kDialog_ID;
        key_message.wParam = uiCurrencyItemInputNumber::kDialog_ID;
        key_message.DoSomething = GlobalFunc::DoShowWindowEx;
        g_KeyQueueManager.PushBack(key_message);
    }
    else
    {
        g_KeyQueueManager.DeleteMsg(uiCurrencyItemInputNumber::kDialog_ID);
    }
}

void uiCurrencyItemInputNumber::OnUpdateSolarDialog()
{
    CCtrlDxIMEEditBox* edit_control = 
        static_cast<CCtrlDxIMEEditBox*>(GetControlWZ_byPosition(kControl_Edit_E000));
    if (edit_control != NULL)
    {
        TCHAR use_num_string[INTERFACE_STRING_LENGTH] = {'\0', };
        edit_control->GetTextWZA(use_num_string, INTERFACE_STRING_LENGTH - 1);
        set_use_item_num(_ttoi(use_num_string));
    }
}

void uiCurrencyItemInputNumber::MessageProc(SI_MESSAGE* message)
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

bool uiCurrencyItemInputNumber::OnLButtonClick(SI_MESSAGE* message)
{
    bool result = true;
    DWORD control_position = GetControlPosition_byID(message->dwCtrlID);
    switch (control_position)
    {
    case kControl_Button_B001:
        {
            // 증가버튼
            DURATYPE use_num = use_item_num();
            if (use_num != (DURATYPE)(-1)) // 255 제한체크
            {
                ++use_num;
                if (use_num > current_item_num())
                {
                    use_num = current_item_num();
                }
                set_use_item_num(use_num);

                CCtrlDxIMEEditBox* edit_control = 
                    static_cast<CCtrlDxIMEEditBox*>(GetControlWZ_byPosition(kControl_Edit_E000));
                if (edit_control != NULL)
                {
                    TCHAR use_num_string[INTERFACE_STRING_LENGTH] = {'\0', };
                    _itot(use_num, use_num_string, 10);
                    edit_control->SetTextWZ(use_num_string);
                    edit_control->SetNumberic(true);
                }
            }
        }
        break;
    case kControl_Button_B005:
        {
            // 감소버튼
            DURATYPE use_num = use_item_num();
            if (use_num > 1) // 1 제한체크
            {
                --use_num;
                set_use_item_num(use_num);

                CCtrlDxIMEEditBox* edit_control = 
                    static_cast<CCtrlDxIMEEditBox*>(GetControlWZ_byPosition(kControl_Edit_E000));
                if (edit_control != NULL)
                {
                    TCHAR use_num_string[INTERFACE_STRING_LENGTH] = {'\0', };
                    _itot(use_num, use_num_string, 10);
                    edit_control->SetTextWZ(use_num_string);
                    edit_control->SetNumberic(true);
                }
            }
        }
        break;
    case kControl_Button_TI00:
        {
            // 확인
            if (use_item_num() > 0)
            {
                SEND_CG_POINTWALLET_CHANGE_WITH_COUNT_SYN();
            }
            
            ShowInterface(FALSE);
        }
        break;
    case kControl_Button_TI01:
    case kControl_Button_B004:
        {
            // 취소 및 닫기
            ShowInterface(FALSE);
        }
        break;
    }
    return result;
}

void uiCurrencyItemInputNumber::SEND_CG_POINTWALLET_CHANGE_WITH_COUNT_SYN()
{
    MSG_CG_POINTWALLET_CHANGE_WITH_COUNT_SYN send_packet(
        SI_INVENTORY, currency_item_position(), use_item_num());
    GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send_packet, sizeof(send_packet));
}

void uiCurrencyItemInputNumber::UseCurrencyItem(POSTYPE item_position)
{
    SCSlotContainer* inventory_container = ItemManager::Instance()->GetContainer(SI_INVENTORY);
    if (inventory_container == NULL)
    {
        return;
    }

    if (item_position >= inventory_container->GetMaxSlotNum())
    {
        return;
    }

    if (inventory_container->IsEmpty(item_position)) 
    {
        return;
    }

    CCtrlDxIMEEditBox* edit_control = 
        static_cast<CCtrlDxIMEEditBox*>(GetControlWZ_byPosition(kControl_Edit_E000));
    if (edit_control == NULL)
    {
        return;
    }

    SCSlot& item_slot = inventory_container->GetSlot(item_position);
    DURATYPE item_num = item_slot.GetNum();

    set_use_item_num(item_num);
    set_currency_item_position(item_position);
    set_current_item_num(item_num);

    TCHAR use_num_string[INTERFACE_STRING_LENGTH] = {'\0', };
    _itot(item_num, use_num_string, 10);
    edit_control->SetTextWZ(use_num_string);
    edit_control->SetNumberic(true);
}

#endif //_NA004034_20120102_POINT_WALLET_SYSTEM