//------------------------------------------------------------------------------
//  uiItemShopDiv.cpp
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"


#include "MouseHandler.h"
#include "uiItemShopDiv.h"
#include "uiItemDivMan/uiItemDivMan.h"
#include "InventoryDialog.h"
#include "globalfunc.h"
#include "interfacemanager.h"
#include "SCItemSlot.h"
#include "ItemManager.h"
#include "ShopDialog.h"
#include <SCSlotContainer.h>
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
#include "Hero.h"
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
//------------------------------------------------------------------------------
WzID uiItemShopDiv::wzId_[DIALOG_MAX] = 
{ 
    StrToWzID("TI00"),  // DLGPOS_BTN_OK
    StrToWzID("TI01"),  // DLGPOS_BTN_CANCEL
    StrToWzID("S000"),  // DLGPOS_TEXT_MAX_NUMBER
    StrToWzID("S001"),  // DLGPOS_TEXT_HIEM
    StrToWzID("E000"),  // DLGPOS_EDT_NUM
};

//------------------------------------------------------------------------------
/**
*/
uiItemShopDiv::uiItemShopDiv(InterfaceManager *ui_manager_ptr):
    uiBase(ui_manager_ptr)
{
    price_ = 0;
    pre_divide_number_ = 0;
}

//------------------------------------------------------------------------------
/**
*/
uiItemShopDiv::~uiItemShopDiv()
{
    this->Release();
}

//------------------------------------------------------------------------------
/**
*/
void uiItemShopDiv::Init(CDrawBase* draw_base_ptr)
{
    uiBase::Init(draw_base_ptr);
    InitControls();
}

//------------------------------------------------------------------------------
/**
*/
void uiItemShopDiv::InitControls()
{
    edit_control_ = static_cast<CCtrlDxIMEEditBox *>(this->get_control(DLGPOS_EDT_NUM));
    assert (edit_control_);

    static_text_controls_[0] = static_cast<CCtrlStaticWZ*>(this->get_control(DLGPOS_TEXT_MAX_NUMBER));
    assert (static_text_controls_[0]);
    static_text_controls_[1] = static_cast<CCtrlStaticWZ*>(this->get_control(DLGPOS_TEXT_HIEM));
    assert (static_text_controls_[1]);

    ok_button_ = static_cast<CCtrlButtonWZ *>(this->get_control(DLGPOS_BTN_OK));
    assert(ok_button_);

    cancel_button_ = static_cast<CCtrlButtonWZ *>(this->get_control(DLGPOS_BTN_CANCEL));
    assert(cancel_button_);

    this->init_controls_ = true;
}

//------------------------------------------------------------------------------
/**
*/
void uiItemShopDiv::Release()
{
    price_ = 0;
    pre_divide_number_ = 0;
}


//------------------------------------------------------------------------------
/**
    called to show request
*/
void uiItemShopDiv::OnShowWindow(BOOL value)
{
    if (value)
    {
        if (!this->CheckControls())
        {
            return;
        }
        //UpdatePosition();

        //SolarDialog::MoveCenter();

        edit_control_->ClearText();
        edit_control_->SetFontID(StrToWzID("mn12"));
        edit_control_->SetFocusWZ();

        TCHAR message[INTERFACE_STRING_LENGTH];
        TCHAR temp_string[INTERFACE_STRING_LENGTH];
        Sprintf(message, _T("%d"), divide_num_);

        GlobalFunc::SetText(edit_control_, message);
        
        TCHAR message_max[INTERFACE_STRING_LENGTH];
        // 987 : (해당 아이템은 최대 "%d"개씩 구입이 가능합니다.)
        g_InterfaceManager.GetInterfaceString(987, message, MAX_MESSAGE_LENGTH );
	    Sprintf(message_max, message, divide_num_);
        static_text_controls_[0]->SetTextWZ(message_max);
        
        TCHAR message_price[INTERFACE_STRING_LENGTH];
        // 985 : ("%d"하임)
        g_InterfaceManager.GetInterfaceString(985, temp_string, MAX_MESSAGE_LENGTH);
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
        // 길드 시설 효과
        if (g_pHero && 
            g_pHero->GetPlayerAttribute()->GetAttrValue(eATTR_SHOP_BUY_HEIM_RATIO) != 0 && 
            (divide_num_ * price_) > 0)
        {            
            MONEY buy_need_money = divide_num_ * price_;
            int ratio = g_pHero->GetPlayerAttribute()->GetAttrValue(eATTR_SHOP_BUY_HEIM_RATIO);
            MONEY percent_value = buy_need_money / 100 * price_;
            buy_need_money += percent_value;
            GlobalFunc::SetPointText(message, buy_need_money);
        }
        else
        {
            GlobalFunc::SetPointText(message, (divide_num_ * price_));
        }
#else
        GlobalFunc::SetPointText(message, (divide_num_ * price_));
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
	    Sprintf(message_price, temp_string, message);
        static_text_controls_[1]->SetTextWZ(message_price);

        keyMsg key_msg;
        ZeroMemory(&key_msg,sizeof(key_msg));
        key_msg.dwType = eDoType_dialog_itemshop_div;
        key_msg.wParam = IM_ITEMDIV_MANAGER::ITEMSHOPDIV;
        key_msg.DoSomething = GlobalFunc::DoShowWindowEx;
        g_KeyQueueManager.PushBack(key_msg);

		this->GetUIMan()->AttachFirstDialog(IM_ITEMDIV_MANAGER::ITEMSHOPDIV);
    }
    else
    {
		this->GetUIMan()->DeAttachFirstDialog(IM_ITEMDIV_MANAGER::ITEMSHOPDIV);
        g_KeyQueueManager.DeleteMsg(eDoType_dialog_itemshop_div);
    }
}

//------------------------------------------------------------------------------
/**
*/
void uiItemShopDiv::MessageProc(SI_MESSAGE * message_ptr)
{
    switch (message_ptr->eResultMsg) 
    {
    case RT_MSG_EDIT_ENTERKEYDOWN:
        {
            switch (get_control_id_to_position(message_ptr->dwCtrlID))
            {
            case DLGPOS_EDT_NUM:
                {
                    on_ok_button_click();
                }
                break;
            }
        }
        break;

    case RT_MSG_LBUTTONCLICK:
        {
            switch (get_control_id_to_position(message_ptr->dwCtrlID))
            {
			case DLGPOS_BTN_OK:
				{
                    on_ok_button_click();
				}
				break;
            
            case DLGPOS_BTN_CANCEL:
                {
                    ShowInterface(FALSE);
                }
                break;
            }
        }
        break;

    default:
        {
            SolarDialog::MessageProc(message_ptr);
        }
        break;
    }
}

////------------------------------------------------------------------------------
///**
//*/
//void uiItemShopDiv::UpdatePosition()
//{
//    MoveWindowWZ(733, 289);
//}

//------------------------------------------------------------------------------
/**
*/
void uiItemShopDiv::OnUpdateSolarDialog()
{
    DWORD div_number = 0;
    TCHAR message[INTERFACE_STRING_LENGTH];
    TCHAR temp_string[INTERFACE_STRING_LENGTH];
    
    GlobalFunc::GetText(edit_control_, message, sizeof(message));
    div_number = atoi(message);
    _itoa(div_number, temp_string, 10);

    if (div_number != pre_divide_number_ || strlen(message) != strlen(temp_string))
    {
        if (div_number > divide_num_)
        {
           div_number = divide_num_;
        }
        pre_divide_number_ = div_number;
        TCHAR number_string[INTERFACE_STRING_LENGTH];        
        Sprintf(number_string, "%d", div_number);
        if (div_number != 0)
        {
           GlobalFunc::SetText(edit_control_, number_string);
        }
        else
        {
            GlobalFunc::SetText(edit_control_, "");
        }
        TCHAR message_price[INTERFACE_STRING_LENGTH];
        // 985 : ("%s"하임)
        g_InterfaceManager.GetInterfaceString(985, temp_string, MAX_MESSAGE_LENGTH);
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
        // 길드 시설 효과
        if (g_pHero && 
            g_pHero->GetPlayerAttribute()->GetAttrValue(eATTR_SHOP_BUY_HEIM_RATIO) != 0 && 
            (divide_num_ * price_) > 0)
        {            
            MONEY buy_need_money = divide_num_ * price_;
            int ratio = g_pHero->GetPlayerAttribute()->GetAttrValue(eATTR_SHOP_BUY_HEIM_RATIO);
            bool is_plus = false;
            if (ratio > 0)
                is_plus = true;
            ratio = abs(ratio);
            MONEY percent_value = buy_need_money / 100 * ratio;
            if (is_plus)
            {
                buy_need_money += percent_value;
            }
            else
            {
                buy_need_money -= percent_value;
            }
            GlobalFunc::SetPointText(message, buy_need_money);
        }
        else
        {
            GlobalFunc::SetPointText(message, (div_number * price_));
        }
#else
        GlobalFunc::SetPointText(message, (div_number * price_));
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
	    Sprintf(message_price, temp_string, message);
        static_text_controls_[1]->SetTextWZ(message_price);
    }
    else
    {
        if (div_number == 0 && message[0] != '\0')
        {
            GlobalFunc::SetText(edit_control_, "");
        }
    }
}


//------------------------------------------------------------------------------
/**
*/
void uiItemShopDiv::on_ok_button_click()
{
	TCHAR   message[INTERFACE_STRING_LENGTH];
    TCHAR   temp_string[INTERFACE_STRING_LENGTH];

    GlobalFunc::GetText(edit_control_, temp_string, INTERFACE_STRING_LENGTH);

    if (lstrlen(temp_string)==0)
    {
		// [알림] 분할하실 개수를 입력하세요 5127
		g_InterfaceManager.GetInterfaceString(eST_INPUT_DIV_NUM, message ,INTERFACE_STRING_LENGTH );
        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, message);
    }
    else
    {
        int a_number = (int)_ttoi64(temp_string);

        if (a_number < 1 || a_number > divide_num_)
        {
		    // 잘못된 정보입니다. 5346 (현재 있는 메시지 중에 그나마)
            // 개수를 다시 입력하세요 등의 메시지를 뿌리는게 더 적당
		    g_InterfaceManager.GetInterfaceString(5346, message ,INTERFACE_STRING_LENGTH );
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, message);

            Sprintf (temp_string, _T("%d"), divide_num_);
            GlobalFunc::SetText(edit_control_, temp_string);

            return;
        }
        SCSlotContainer* at_container = ItemManager::Instance()->GetContainer(from_slot_idx_);
		assert(at_container);

		SolarDialog* container_dialog = ItemManager::Instance()->GetItemContainerDialog(from_slot_idx_);
		assert(container_dialog);
        SCItemSlot& ref_slot1 = (SCItemSlot &)at_container->GetSlot(from_postype_);
	    ShopDialog* shop_dialog = static_cast<ShopDialog*>(container_dialog);
	    DWORD dwShopID = (DWORD)shop_dialog->GetShopID();
	    SHOPINFO* shop_info = (SHOPINFO *)ShopInfoParser::Instance()->GetShopList(dwShopID);
	    SHOPINFO::SHOP_ITEM* shop_item = shop_info->GetItem(ref_slot1.GetCode());
	    if (!shop_item)
	    {
		    assert(!"ShopInfo.txt 확인!!");
		    return;
	    }
        
#ifdef _NA_008678_20160203_ADD_HEIM_TRADINGVOLUME_NOTIFICATION_OPTION
		if (GlobalFunc::HeimTradingvolumeCheck(price_ * ref_slot1.GetMaxNum()))
		{
			uiSystemMan* ui_system_manager_ptr = GET_DIALOG_MANAGER(uiSystemMan, UIMAN_SYSTEM);
			if (ui_system_manager_ptr)
			{
				ui_system_manager_ptr->Sys_SystemHeimTradingvolumeNpcStore(shop_dialog->GetDialogKey(), 
					                                                       a_number, 
					                                                       from_slot_idx_, 
																		   from_postype_, 
																		   slot_type_, 
																		   slot_code_, 
																		   price_ * a_number);
			}
		}
		else
		{
			ItemManager::Instance()->SendBuyMsg(
				shop_dialog ->GetShopID(), 
				ref_slot1.GetCode(), shop_dialog->GetCurrentTab(), shop_dialog->GetRelativeSlotPos(from_postype_),
				a_number);
		}
#else
	    ItemManager::Instance()->SendBuyMsg(
		    shop_dialog ->GetShopID(), 
		    ref_slot1.GetCode(), shop_dialog->GetCurrentTab(), shop_dialog->GetRelativeSlotPos(from_postype_),
            a_number);
#endif // _NA_008678_20160203_ADD_HEIM_TRADINGVOLUME_NOTIFICATION_OPTION

        this->ShowWindow(FALSE);
    }
}
//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
