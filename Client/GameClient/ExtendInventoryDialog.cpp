#include "SunClientPrecompiledHeader.h"

#include "ExtendInventoryDialog.h"
#include <SCItemSlot.h>
#include "InterfaceManager.h"
#include "uiInventoryMan/uiInventoryMan.h"
#include "inventorydialog.h"
#include "Hero.h"
#include "globalfunc.h"
#include "ItemUnitRenderer.h"
#include "MouseHandler.h"
#include "Mouse.h"
#include "CursorChangeTransact.h"
#include "uiToolTipMan/uiToolTipMan.h"
#include "uiSocketRemove/uiSocketRemove.h"
#include "ItemInfoParser.h"
#include "ItemManager.h"
#include "BattleScene.h"
#include "EnchantDialog2.h"

#include "EventInventoryDialog.h"
#include "ItemCompositeParser.h"
#include "HeroEquipmentContainer.h"
#include "uiPersonalWayPointWindow/uiPersonalWayPointWindow.h"

const DWORD c_dwFlashTick = 500;
//----------------------------------------------------------------------------
/**
*/
WzID ExtendInventoryDialog::controls_id_[EXTEND_INVENTORY_CONTROLS_MAX] = 
{ 
    StrToWzID("B020"),
    StrToWzID("B021"),
    StrToWzID("B022"),
    StrToWzID("B023"),
    StrToWzID("B024"),
    StrToWzID("B025"),
    StrToWzID("B026"),
    StrToWzID("B027"),
    StrToWzID("B028"),
    StrToWzID("B029"),
    StrToWzID("B030"),
    StrToWzID("B031"),
    StrToWzID("B032"),
    StrToWzID("B033"),
    StrToWzID("B034"),//1

    StrToWzID("BT20"),
    StrToWzID("BT19"),
    StrToWzID("BT18"),
    StrToWzID("BT17"),
    StrToWzID("BT16"),
    StrToWzID("BT11"),
    StrToWzID("BT12"),
    StrToWzID("BT13"),
    StrToWzID("BT14"),
    StrToWzID("BT15"),
    StrToWzID("BT10"),
    StrToWzID("BT09"),
    StrToWzID("BT08"),
    StrToWzID("BT07"),
    StrToWzID("BT06"),//2

    StrToWzID("BT36"),
    StrToWzID("BT37"),
    StrToWzID("BT38"),
    StrToWzID("BT39"),
    StrToWzID("BT40"),
    StrToWzID("BT45"),
    StrToWzID("BT44"),
    StrToWzID("BT43"),
    StrToWzID("BT42"),
    StrToWzID("BT41"),
    StrToWzID("BT46"),
    StrToWzID("BT47"),
    StrToWzID("BT48"),
    StrToWzID("BT49"),
    StrToWzID("BT50"),//3

    StrToWzID("BT22"),
    StrToWzID("BT21"),
    StrToWzID("BT23"),
    StrToWzID("BT24"),
    StrToWzID("BT25"),
    StrToWzID("BT30"),
    StrToWzID("BT29"),
    StrToWzID("BT28"),
    StrToWzID("BT27"),
    StrToWzID("BT26"),
    StrToWzID("BT31"),
    StrToWzID("BT32"),
    StrToWzID("BT33"),
    StrToWzID("BT34"),
    StrToWzID("BT35"),//4

    StrToWzID("BT82"),
    StrToWzID("BT81"),
    StrToWzID("BT83"),
    StrToWzID("BT84"),
    StrToWzID("BT85"),
    StrToWzID("BT86"),
    StrToWzID("BT87"),
    StrToWzID("BT88"),
    StrToWzID("BT89"),
    StrToWzID("BT90"),
    StrToWzID("BT91"),
    StrToWzID("BT92"),
    StrToWzID("BT93"),
    StrToWzID("BT94"),
    StrToWzID("BT95"),//5

    StrToWzID("BT80"),
    StrToWzID("BT79"),
    StrToWzID("BT78"),
    StrToWzID("BT77"),
    StrToWzID("BT76"),
    StrToWzID("BT75"),
    StrToWzID("BT74"),
    StrToWzID("BT73"),
    StrToWzID("BT72"),
    StrToWzID("BT71"),
    StrToWzID("BT70"),
    StrToWzID("BT69"),
    StrToWzID("BT68"),
    StrToWzID("BT67"),
    StrToWzID("BT66"),//6

    StrToWzID("BT51"),
    StrToWzID("BT52"),
    StrToWzID("BT53"),
    StrToWzID("BT54"),
    StrToWzID("BT55"),
    StrToWzID("BT60"),
    StrToWzID("BT59"),
    StrToWzID("BT58"),
    StrToWzID("BT57"),
    StrToWzID("BT56"),
    StrToWzID("BT61"),
    StrToWzID("BT62"),
    StrToWzID("BT63"),
    StrToWzID("BT64"),
    StrToWzID("BT65"),//7

    StrToWzID("B000"),
    StrToWzID("B001"),
    StrToWzID("B002"),
    StrToWzID("B003"),
    StrToWzID("B004"),
    StrToWzID("B005"),
    StrToWzID("B006"),
    StrToWzID("B007"),
    StrToWzID("B008"),
    StrToWzID("B009"),
    StrToWzID("B010"),
    StrToWzID("B011"),
    StrToWzID("B012"),
    StrToWzID("B013"),
    StrToWzID("B014"),//8

    StrToWzID("B047"),
    StrToWzID("B048"),
    StrToWzID("B049"),
    StrToWzID("B050"),
    StrToWzID("B051"),
    StrToWzID("B057"),
    StrToWzID("B058"),
    StrToWzID("B059"),
    StrToWzID("B060"),
    StrToWzID("B061"),
    StrToWzID("B067"),
    StrToWzID("B068"),
    StrToWzID("B069"),
    StrToWzID("B070"),
    StrToWzID("B071"),//9

    StrToWzID("B052"),
    StrToWzID("B053"),
    StrToWzID("B054"),
    StrToWzID("B055"),
    StrToWzID("B056"),
    StrToWzID("B062"),
    StrToWzID("B063"),
    StrToWzID("B064"),
    StrToWzID("B065"),
    StrToWzID("B066"),
    StrToWzID("B072"),
    StrToWzID("B073"),
    StrToWzID("B074"),
    StrToWzID("B075"),
    StrToWzID("B076"),

    StrToWzID("T999"),//title
    StrToWzID("BT01"),//unlock
    StrToWzID("B036"),//lock
    StrToWzID("BT04"),//설정필터
    StrToWzID("BT05"),//펼쳐보기
    StrToWzID("B037"),//휴지통
    StrToWzID("BT03"),//웨이포인트
    StrToWzID("BT00"),//eve off
    StrToWzID("BT02"),//eve on
    StrToWzID("T001"),//money
    StrToWzID("B015"),//INVEN_BT_EXIT

    StrToWzID("P000"),//lock blind0
    StrToWzID("P001"),//lock blind1
    StrToWzID("P003"),//lock blind2
    StrToWzID("P002"),//lock blind3
    StrToWzID("P004"),//lock blind4
    
};

ExtendInventoryDialog::ExtendInventoryDialog(InterfaceManager* manager_ptr)
:uiBase(manager_ptr)
{
    inventory_manager_ptr_ = NULL;
    item_unit_renderer_ = NULL;
    this->inventory_manager_ptr_ = GET_DIALOG_MANAGER(uiInventoryMan, UIMAN_INVENTORY);
	assert(inventory_manager_ptr_);
}

ExtendInventoryDialog::~ExtendInventoryDialog(void)
{
    Release();
}

void ExtendInventoryDialog::Init(CDrawBase* draw_base_ptr)
{
    InitControls();

    if(event_button_ != NULL)
	{
		event_button_->ShowWindowWZ(WZ_HIDE);
	}
    flash_tick_for_event_inven_ = c_dwFlashTick;
	event_inven_on_ = false;
    event_inven_off_ = false;
    charge_count_ = 0;
}

void ExtendInventoryDialog::CheckChargeInventory(int charge_count)
{
    if (g_pHero == NULL)
    {
        return;
    }
    CCtrlImageStaticWZ* blind_background_image = NULL;

    int max_inven_blind = (EXTEND_INVEN_IMAGE_BLIND04 - EXTEND_INVEN_IMAGE_BLIND00) + 1;
    for(int i = 0; i < max_inven_blind; ++i)
	{
		blind_background_image = 
            static_cast<CCtrlImageStaticWZ*>(get_control(EXTEND_INVEN_IMAGE_BLIND00 + i));
		if (blind_background_image != NULL)
        {
            if (charge_count > i)
            {
                blind_background_image->ShowWindowWZ(WZ_HIDE);
            }
            else
            {
                blind_background_image->ShowWindowWZ(WZ_SHOW);
            }
        }
	}
}

void ExtendInventoryDialog::InitControls()
{
    money_text_control_ = static_cast<CCtrlStaticWZ*>(get_control(EXTEND_INVEN_BUTTON_MONEY));
    assert(money_text_control_);

    title_static_text_ = static_cast<CCtrlStaticWZ *>(get_control(EXTEND_INVEN_TEXT_TITLE));
	assert(title_static_text_);

    for(int i = 0; i < EXTEND_INVENTORY_LOCK_STATE_MAX; ++i)
	{
		lock_buttons_[i] = static_cast<CCtrlButtonCheckWZ*>(get_control(EXTEND_INVEN_BUTTON_UNLOCK + i));
		assert(lock_buttons_[i]);
	}

    event_button_ = (CCtrlButtonWZ*)get_control(EXTEND_INVEN_BUTTON_EVENT_INVEN_ON);
	assert(event_button_);
}

void ExtendInventoryDialog::InitRenderer()
{
    inventory_dialog_
        = GET_CAST_DIALOG(InventoryDialog, IM_INVENTORY_MANAGER::INVEN_DLG);

    if (inventory_dialog_ != NULL)
    {
        item_unit_renderer_ = inventory_dialog_->GetItemUnitRender();
    }
}

void ExtendInventoryDialog::Release()
{
}

uiInventoryMan* ExtendInventoryDialog::GetManager()
{
	return inventory_manager_ptr_;
}

void ExtendInventoryDialog::OnUpdateSolarDialog()
{
    if (item_unit_renderer_ != NULL)
    {
        item_unit_renderer_->Update();
    }
 
    if (inventory_dialog_ != NULL && money_text_control_ != NULL)
    {
        inventory_dialog_->updateMoney(money_text_control_);
    }

    int charge_count = g_pHero->GetCharInfo()->m_ExtraInventoryTabCount;
    if (charge_count_ != charge_count)
    {
        charge_count_ = charge_count;
        CheckChargeInventory(charge_count);// 변경된 경우(캐쉬아이템으로 사면)
    }
}

void ExtendInventoryDialog::OnRenderSolarDialog()
{
    /*if (item_unit_renderer_ != NULL)
    {
	    item_unit_renderer_->Render();
    }*/
    UpdateToolTip();
	if (inventory_dialog_ != NULL)
	{
		inventory_dialog_->InventoryUnitRender();
	}
    if((true == event_inven_on_) && (WZ_HIDE == event_button_->GetState()))
	{
		event_button_->ShowWindowWZ(WZ_SHOW);
	}
	else if((false == event_inven_on_) && (WZ_SHOW == event_button_->GetState()))
	{
		event_button_->ShowWindowWZ(WZ_HIDE);
	}
}

void ExtendInventoryDialog::Process(DWORD tick_count)
{
    if (event_inven_off_)
	{
		if ((DWORD)flash_tick_for_event_inven_ > tick_count && IsVisible())
		{
			flash_tick_for_event_inven_ -= tick_count;
			if (flash_tick_for_event_inven_ <= 100)
			{
				if (event_inven_on_ == true)
                {
                    event_inven_on_ = false;
                }
                else
                {
                    event_inven_on_ = true;
                }
				flash_tick_for_event_inven_ = c_dwFlashTick;
			}
		}
	}
	else
	{
		event_inven_on_ = FALSE;
	}
}

void ExtendInventoryDialog::MessageProc(SI_MESSAGE* message_ptr)
{
    static BOOL double_clicked = FALSE;
    switch (message_ptr->eResultMsg) 
	{
        case RT_MSG_LBUTTONDCLICK:
        {
            POSTYPE control_position = get_control_id_to_position(message_ptr->dwCtrlID);
		    if (inventory_dialog_ == NULL || inventory_dialog_->GetCurInvenContainer() != EXTEND_INVENTORY)
		    {
			    return;
		    }
		    CControlWZ* control_ptr = get_control(control_position);
		    if (control_ptr == NULL)
		    {
			    return;
		    }
		    double_clicked = TRUE;
            if (inventory_dialog_ != NULL)
            {
                inventory_dialog_->DoubleClickSlotBtn(control_position);
            }
        }
        break;

	    case RT_MSG_LBUTTONCLICK:
	    {
            POSTYPE control_position = get_control_id_to_position(message_ptr->dwCtrlID);
            if ((control_position < EXTEND_INVENTORY_MAX) && (double_clicked == FALSE))
		    {
                CControlWZ* pControl = get_control(control_position);
	            if (pControl != NULL)
                {
			        inventory_dialog_->ClickSlotBtn(control_position, pControl->GetSizeRect());
                }
		    }
            else
            {
			    if (double_clicked)
			    {
				    double_clicked = FALSE;
			    }

                POSTYPE normal_inventory_control_position = EXTEND_INVENTORY_CONTROLS_MAX;

		        switch (control_position)
		        {
                case EXTEND_INVEN_BUTTON_VIEW_OPTION:
                    normal_inventory_control_position = InventoryDialog::INVEN_BT_EXTEND;
                    break;

                case EXTEND_INVEN_BUTTON_FILTER:
                    normal_inventory_control_position = InventoryDialog::INVEN_BT_SETTING;
                    break;

           	    case EXTEND_INVEN_BUTTON_EVENT_INVEN_OFF:
                    normal_inventory_control_position = InventoryDialog::INVEN_BT_OPEN_EVENT;
                    break;

	            case EXTEND_INVEN_BUTTON_EVENT_INVEN_ON: 
                    normal_inventory_control_position = InventoryDialog::INVEN_FLASH_BT_OPEN_EVENT;
		            break;

        	    case EXTEND_INVEN_BUTTON_WASTE:
			        normal_inventory_control_position = InventoryDialog::INVEN_BT_WASTE_BASKET;
		            break;

           	    case EXTEND_INVEN_BUTTON_UNLOCK:
			        normal_inventory_control_position = InventoryDialog::INVEN_BT_UNLOCK;
		            break;

	            case EXTEND_INVEN_BUTTON_LOCK:
			        normal_inventory_control_position = InventoryDialog::INVEN_BT_LOCK;		
		            break;

                case EXTEND_INVEN_BUTTON_EXIT:
                    ShowDialog(FALSE);
                    break;

	            case EXTEND_INVEN_BUTTON_WAY_POINT:
                    normal_inventory_control_position = InventoryDialog::INVEN_PERSONAL_WAYPOINT_BTN;
                    break;

                default:
                    break;

                }//switch

                if (normal_inventory_control_position != EXTEND_INVENTORY_CONTROLS_MAX)
                {
                    if (inventory_dialog_ != NULL)
                    {
                        inventory_dialog_->ClickETCBtn(normal_inventory_control_position);
                    }
                }
            }
        }
	    break;

        case RT_MSG_RBUTTONCLICK:
	    {
		    POSTYPE control_position = get_control_id_to_position(message_ptr->dwCtrlID);
		    CControlWZ* control_ptr = get_control(control_position);
		    if (control_ptr == NULL)
		    {
			    return;
		    }
            if (control_position >= EXTEND_INVENTORY_MAX) 
            {
                return;
            }
            if (inventory_dialog_ != NULL)
            {
                inventory_dialog_->RightClickProcess(control_position);
            }
	    }
	    break;

        default:
	    {
		    SolarDialog::MessageProc(message_ptr);
	    }
        break;

	}//switch
}

void ExtendInventoryDialog::ShowExtendInventory()
{
    HeroEquipmentContainer* hero_equip = g_pHero->GetHeroEquipContainer();
    // 캐릭터 장착창이 열려 있지 않은 경우만 토글한다.
    if (hero_equip && !hero_equip->IsVisible())
    {
        return;
        //hero_equip->ShowDialog(TRUE);
    }

    // 큰인벤이 열려 있다 -> 큰인벤을 닫고 작은인벤을 연다.
    // 큰인벤이 닫혀 있다 -> 작은인벤을 닫고 큰인벤을 연다.
    if (GetManager())
    {
        GetManager()->ToggleExtendInvenDlg();
    }
}

void ExtendInventoryDialog::OnShowWindow(BOOL value)
{
    SolarDialog::OnShowWindow(value);
    if (value != FALSE)
    {
        if(inventory_dialog_ != NULL)
        {
            inventory_dialog_->InitTitleByLockState(title_static_text_, 
                lock_buttons_[EXTEND_INVENTORY_LOCK_STATE_LOCK],
		        lock_buttons_[EXTEND_INVENTORY_LOCK_STATE_UNLOCK]);
        }
        //MSG_CG_EVENT_QUERY_EXIST_SYN는 받지 않는다. 기본인벤에서 받아 처리한다.
        RefreshExtendInventoryRenderUnit();
    }
    else
    {
        if (inventory_dialog_ != NULL)
        {
            inventory_dialog_->HideWindowProcess();
        }
    }
}

void ExtendInventoryDialog::RefreshRenderSlot()
{
    if (!this->IsVisibleWindow())
	{
		return;
	}
	RefreshExtendInventoryRenderUnit();
}

void ExtendInventoryDialog::RefreshExtendInventoryRenderUnit()
{
	ItemUnitRenderer::RenderUnit* pRender = NULL;
	RECT			rcSlot;
	CControlWZ*		pCtrl = NULL;

    if (inventory_dialog_ == NULL)
    {
        return;
    }

	for (int i = 0; i < inventory_dialog_->GetMaxSlotNum(); ++i)
	{
		SCSlot& rSlot = inventory_dialog_->GetSlot(i);
		if (inventory_dialog_->IsEmpty(i))
		{
			continue;
		}
        if (inventory_dialog_->GetCurInvenContainer() == 0 && // 0: InventoryDialog::INVENTORY
            (i < EXTEND_INVENTORY_MAX))
		{
			// 보여줘야할 목록
			if(item_unit_renderer_ == NULL)
            {
				continue;
            }
			pRender = item_unit_renderer_->GetItemUnit(rSlot.GetSerial());

            pCtrl = get_control(i);//i가 바로 위치
			rcSlot = pCtrl->GetSizeRect();
			if (pRender == NULL)
			{
				pRender = item_unit_renderer_->AddItemUnit(rSlot.GetSerial(), rSlot.GetCode(), &rcSlot, &rSlot ,pCtrl);
			}
            else
            {
                item_unit_renderer_->ChangePositionItemUnit(rSlot.GetSerial(), &rcSlot, pCtrl);
            }

			if (pRender && pRender != MouseHandler::Instance()->GetMouseItem()->m_pRenderUnit)
            {
				pRender->SetShow(TRUE);
            }
		}
	}
}

void ExtendInventoryDialog::UpdateToolTip()
{
	int iMouseX = Mouse::Instance()->GetMouseX();
	int iMouseY = Mouse::Instance()->GetMouseY();

	// 스킵처리 
	// 인터페이스에서 Dialog 바탕에서는 Mouse Over가 오지 않는다.
	bool is_mouse_over = false;
    int loop_end = EXTEND_INVENTORY_CONTROLS_MAX;
	for (POSTYPE position = 0; position < loop_end; ++position)
	{
		CControlWZ* control = get_control(position);
		if(control)
		{
			RECT control_rect = control->GetSizeRect();
			if (control_rect.left <= iMouseX && control_rect.right >= iMouseX)
			{
				if (control_rect.top <= iMouseY && control_rect.bottom >= iMouseY)
				{
					is_mouse_over = true;
					break;
				}
			}
		}
	}

    if (inventory_dialog_ != NULL)
    {
        inventory_dialog_->ProcessToolTip(is_mouse_over, GetDialogKey());
    }
}

void ExtendInventoryDialog::MouseOver_MsgProc(SI_MESSAGE* pMessage)
{
    if (inventory_dialog_ == NULL)
    {
        return;
    }

    switch(pMessage->eResultMsg) 
	{
	case RT_MSG_MOUSEOVER:
		{
			POSTYPE position = get_control_id_to_position(pMessage->dwCtrlID);
			// 0~ EXTEND_INVENTORY_MAX 까지 아이템 정보를 보여준다.
			if (position < inventory_dialog_->GetMaxSlotNum() && 
				position >= 0 && position < EXTEND_INVENTORY_MAX)
			{
				inventory_dialog_->MouseOverProcess(position);				
			}
			else
			{
                POSTYPE normal_inventory_position = 0;

                switch (position)
                {
                case EXTEND_INVEN_BUTTON_UNLOCK:
                    normal_inventory_position = InventoryDialog::INVEN_BT_UNLOCK;
                    break;

                case EXTEND_INVEN_BUTTON_LOCK:
                    normal_inventory_position = InventoryDialog::INVEN_BT_LOCK;
                    break;

                case EXTEND_INVEN_BUTTON_WAY_POINT:
                    normal_inventory_position = InventoryDialog::INVEN_PERSONAL_WAYPOINT_BTN;
                    break;

				case EXTEND_INVEN_BUTTON_FILTER:
					normal_inventory_position = InventoryDialog::INVEN_BT_SETTING;
					break;

				case EXTEND_INVEN_BUTTON_VIEW_OPTION:
					normal_inventory_position = InventoryDialog::INVEN_BT_EXTEND;
					break;

				case EXTEND_INVEN_BUTTON_WASTE:
					normal_inventory_position = InventoryDialog::INVEN_BT_WASTE_BASKET;
					break;

				case EXTEND_INVEN_BUTTON_EVENT_INVEN_OFF:
				case EXTEND_INVEN_BUTTON_EVENT_INVEN_ON:
					normal_inventory_position = InventoryDialog::INVEN_BT_OPEN_EVENT;
					break;

                default:
                    break;

                }

                if ((normal_inventory_position != 0) && (inventory_dialog_ != NULL))
                {
				    inventory_dialog_->SetTooltipString(
                        inventory_dialog_->getTooltipString(normal_inventory_position));
                }
                else
                {
                    inventory_dialog_->SetTooltipString(0);
                }

                inventory_dialog_->SetInventoryMouseOverType(eMOUSE_OVER_MAX);
			}
		}
		break;
	}
}

void ExtendInventoryDialog::InitTitleByLockState()
{
	if(inventory_dialog_ != NULL)
    {
        inventory_dialog_->InitTitleByLockState(title_static_text_, 
            lock_buttons_[EXTEND_INVENTORY_LOCK_STATE_LOCK],
		    lock_buttons_[EXTEND_INVENTORY_LOCK_STATE_UNLOCK]);
    }
}

//void ExtendInventoryDialog::UpdatePosition()
//{
//	// 장착 창 뜰때 포지션 지정해준다.
//	// 스킵~
//}
