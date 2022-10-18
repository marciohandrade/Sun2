#include "SunClientPrecompiledHeader.h"

#ifdef _NA_005988_20120901_VENDOR_SEARCH_SYSTEM
#include "uiVendorMan/uiVendorMan.h"
#include "uiVendorMan/uiVendorSearch/uiVendorSearch.h"

#include "GlobalFunc.h"
#include "ItemInfoParser.h"
#include <SCSlotContainer.h>
#include "ItemUnitRenderer.h"
#include "uiToolTipMan/uiToolTipMan.h"
#include "Mouse.h"
#include "Hero.h"
#include "Map.h"
#include "ItemManager.h"
#include "MouseCursor.h"
#include "InterfaceManager.h"
#include "GameFunc.h"
#include "..\Interface\uiVendorBuy\uiVendorBuy.h"

#ifdef _INTERNATIONAL_UI
const WzID c_wiListFont = StrToWzID("st10");
#else
const WzID c_wiListFont = StrToWzID("m212");
#endif//_INTERNATIONAL_UI

const int c_AnimationFullTime = 1;

COLORREF INPRICECOLORTABLE_[] = 
{
	RGBA(255,255,255,255),
	RGBA(0,176,240,255),
	RGBA(0,176,80,255),
	RGBA(255,0,255,255),
	RGBA(255,255,0,255),
	RGBA(227,108,10,255),
	RGBA(255,0,0,255)
#ifdef _NA_008678_20160203_ADD_HEIM_TRADINGVOLUME_NOTIFICATION_OPTION
	,RGBA(153,102,255,255)
#endif // _NA_008678_20160203_ADD_HEIM_TRADINGVOLUME_NOTIFICATION_OPTION

};
//------------------------------------------------------------------------------
void uiVendorSearch::RegisterControl()
{
    RegisterControlPosition(StrToWzID("P029"), kControl_Picture_P029);
    RegisterControlPosition(StrToWzID("P030"), kControl_Picture_P030);
    RegisterControlPosition(StrToWzID("P031"), kControl_Picture_P031);
    RegisterControlPosition(StrToWzID("P032"), kControl_Picture_P032);
    RegisterControlPosition(StrToWzID("P033"), kControl_Picture_P033);
    RegisterControlPosition(StrToWzID("P034"), kControl_Picture_P034);
    RegisterControlPosition(StrToWzID("P035"), kControl_Picture_P035);
    RegisterControlPosition(StrToWzID("P036"), kControl_Picture_P036);
    RegisterControlPosition(StrToWzID("P037"), kControl_Picture_P037);
    RegisterControlPosition(StrToWzID("P038"), kControl_Picture_P038);
    RegisterControlPosition(StrToWzID("P039"), kControl_Picture_P039);
    RegisterControlPosition(StrToWzID("P040"), kControl_Picture_P040);

    RegisterControlPosition(StrToWzID("B999"), kControl_Button_B999);
    RegisterControlPosition(StrToWzID("BT01"), kControl_Button_BT01);
    RegisterControlPosition(StrToWzID("BT00"), kControl_Button_BT00);
    RegisterControlPosition(StrToWzID("B003"), kControl_Button_B003);
    RegisterControlPosition(StrToWzID("BT02"), kControl_Button_BT02);
    RegisterControlPosition(StrToWzID("B000"), kControl_Button_B000);
    RegisterControlPosition(StrToWzID("B005"), kControl_Button_B005);
    RegisterControlPosition(StrToWzID("B002"), kControl_Button_B002);
    RegisterControlPosition(StrToWzID("B004"), kControl_Button_B004);
    RegisterControlPosition(StrToWzID("B006"), kControl_Button_B006);
    RegisterControlPosition(StrToWzID("B007"), kControl_Button_B007);
    RegisterControlPosition(StrToWzID("B008"), kControl_Button_B008);
    RegisterControlPosition(StrToWzID("B009"), kControl_Button_B009);
    RegisterControlPosition(StrToWzID("B001"), kControl_Text_B001);
    RegisterControlPosition(StrToWzID("ST00"), kControl_Text_ST00);
    RegisterControlPosition(StrToWzID("L000"), kControl_List_L000);
    RegisterControlPosition(StrToWzID("L001"), kControl_List_L001);
    RegisterControlPosition(StrToWzID("E000"), kControl_Edit_E000);
}

uiVendorSearch::uiVendorSearch(InterfaceManager* ui_manager) : uiBase(ui_manager),
item_unit_renderer_(NULL),
animatino_show_(false),
open_shop_(false),
//강제 오버플로
open_shopOwner_Key_(-1)
{
}

uiVendorSearch::~uiVendorSearch(void)
{
    Release();
}

void uiVendorSearch::Init(CDrawBase* drawbase)
{
    RegisterControl();

    //슬롯초기화
    SCItemSlotContainer::Init(kSlotTotalCount, SI_VENDOR_SEARCH);

    // 랜더러 초기화
    assert(!item_unit_renderer_);
    item_unit_renderer_ = new ItemUnitRenderer;
    item_unit_renderer_->Init(uiBase::GetDrawBase(), kSlotTotalCount);

    CCtrlListWZ* list_control = static_cast<CCtrlListWZ*>(GetControlWZ(GetControlID_byPosition(kControl_List_L001)));
    if (list_control)
    {
        //0.리스트 목록정보삽입
        list_control->DeleteItemAll();
        list_control->SetBeginDrawIndex(0);

        list_control->SetSellHeight(c_iListHeight);
        list_control->SetUseReport(true);
        list_control->SetReportUseCnt(1);
        list_control->SetHideReportRender(true);
        list_control->SetFontID(c_wiListFont);

        int index = list_control->InsertItemBack();
        //list_control->SetItemData(index, );
        DWORD text_style = DT_VCENTER | DT_CENTER;	// 기본값
        TCHAR string[INTERFACE_STRING_LENGTH] = {0,};
        
        GlobalFunc::SetReportButtonInfo(list_control, VENDOR_ITEM_NAME, 
            224, _T("search_type"));

        g_InterfaceManager.GetInterfaceString(2385, string);
        GlobalFunc::SetItem(list_control, index, 0,
            string,
            c_wiListFont,
            text_style,
            RGBA(255,255,255,0), 
            RGBA(255,255,255,255), 
            RGBA(255,255,255,255));

        index = list_control->InsertItemBack();
        g_InterfaceManager.GetInterfaceString(2386, string);
        GlobalFunc::SetItem(list_control, index, 0,
            string,
            c_wiListFont,
            text_style,
            RGBA(255,255,255,0), 
            RGBA(255,255,255,255), 
            RGBA(255,255,255,255));

        list_control->ShowWindowWZ(WZ_HIDE);

        SetSearchType(0);
    }

    pre_page_= 0;
    max_page_ = 1;
    page_ = 1;
    order_value_ = true;
    SetPage(page_);

    vendor_search_list_ptr_ = static_cast<CCtrlListWZ*>(GetControlWZ(GetControlID_byPosition(kControl_List_L000)));

	if(vendor_search_list_ptr_)
	{
		vendor_search_list_ptr_->SetColorSelBar(RGBA(150,150,150,150));
		vendor_search_list_ptr_->SetSellHeight(23);
		vendor_search_list_ptr_->SetUseReport(true);
		vendor_search_list_ptr_->SetReportUseCnt(uiVendorSearch::VENDOR_PLACE + 1);
		vendor_search_list_ptr_->SetHideReportRender(true);
		vendor_search_list_ptr_->SetFontID(c_wiListFont);
	}

	InitTexture();

	animation_refresh_timer_.On(c_AnimationFullTime);
	ShowAnimation(false);

    assert(kControl_List_L000);
}

void uiVendorSearch::Release()
{
    pre_page_ = 0;
    order_value_ = true;
    page_ = 1;
    SetPage(page_);
    max_page_ = 1;

	if(effect_texture_ != INVALID_HANDLE_VALUE)
	{
		g_pSunRenderer->x_pManagerTexture->DestroyTexture(effect_texture_);
		effect_texture_ = INVALID_HANDLE_VALUE;
	}

	if(item_unit_renderer_)
	{
		item_unit_renderer_->Release();
		delete item_unit_renderer_;
		item_unit_renderer_ = NULL;
	}
}

void uiVendorSearch::SetPage(int page)
{
    TCHAR string[256] = {0,};
    if (page > 0 && page <= max_page_)
    {

        CCtrlStaticWZ* static_control = static_cast<CCtrlStaticWZ*>(GetControlWZ(GetControlID_byPosition(kControl_Text_ST00)));
        if (static_control)
        {
            sprintf(string, "%d/%d", page,max_page_);
            static_control->SetTextWZ(string);
            page_ = page;

            if (page_ != pre_page_)
            {
                //페이지 요청 패킷 전송
                //패킷.....
                
				TCHAR string[INTERFACE_STRING_LENGTH] = {0};
				CCtrlDxIMEEditBox* edit_control = static_cast<CCtrlDxIMEEditBox*>(GetControlWZ(StrToWzID("E000")));
				if (edit_control)
				{
					GlobalFunc::GetText(edit_control, string, INTERFACE_STRING_LENGTH);

					pre_page_ = page_;

					SendPackSearchItem(page_,string);               
				} 
            }
        }
    }
}
//------------------------------------------------------------------------------
void uiVendorSearch::MessageProc(SI_MESSAGE* message)
{
    if (IsLock())
        return;

    switch (message->eResultMsg)
    {
    case RT_MSG_LBUTTONCLICK:
        OnLButtonClick(message);
        break;
	case RT_MSG_LBUTTONDCLICK:
		OnLButtonDClick(message);
		break;
	case RT_MSG_EDIT_ENTERKEYDOWN:
		OnKeyEnterInput(message);
		break;
    default:
        SolarDialog::MessageProc(message);
        break;
    }
}

void uiVendorSearch::OnShowWindow(BOOL is_show)
{    
    if (is_show)
    {
        keyMsg msg;
        ZeroMemory(&msg,sizeof(msg));
        msg.dwType=eDoType_VendorSearch;
        msg.wParam=IM_VENDOR_MANAGER::VENDOR_SEARCH;
        msg.DoSomething=GlobalFunc::DoShowWindowEx;
        g_KeyQueueManager.PushBack(msg);

		vendor_array_.Clear();
		vendor_search_list_ptr_->DeleteItemAll();
		DeleteAllSlot();

		CCtrlDxIMEEditBox* edit_control = static_cast<CCtrlDxIMEEditBox*>(GetControlWZ(StrToWzID("E000")));
		if (edit_control)
		{
			GlobalFunc::SetText(edit_control,"");
		}

		CCtrlListWZ* list_control = static_cast<CCtrlListWZ*>(GetControlWZ(GetControlID_byPosition(kControl_List_L001)));
		if(list_control)
		{
			list_control->EnableWindowWZ(false);
		}

		CCtrlButtonWZ* list_Button = static_cast<CCtrlButtonWZ*>(GetControlWZ(GetControlID_byPosition(kControl_Button_B000)));
		if(list_Button)
		{
			list_Button->EnableWindowWZ(false);
		}

		memset(&isNowButClick_,0,sizeof(VENDOR_DATA));
    }
    else
    {
		open_shop_ = FALSE;
        g_KeyQueueManager.DeleteMsg(eDoType_VendorSearch);
    }
}
//------------------------------------------------------------------------------
bool uiVendorSearch::OnLButtonClick(SI_MESSAGE* message)
{
    bool result = true;
    DWORD control_position = GetControlPosition_byID(message->dwCtrlID);

    switch (control_position)
    {
    case kControl_Button_B999:
        {
            ShowInterface(FALSE);
        }
        break;
    case kControl_List_L001:
        {
			//검색기능은 잠시 막아둔다.
          /*  CCtrlListWZ* list_control = static_cast<CCtrlListWZ*>(GetControlWZ(GetControlID_byPosition(kControl_List_L001)));
            if (list_control)
            {
                int index = list_control->GetCurSel();
                if (index != -1)
                {
                    SetSearchType(index);
                }
            }
			*/
        }
        break;
    case kControl_Button_B004:
        {
            order_value_ = !order_value_;
            RefreshListData(vendor_array_);
        }
        break;
    case kControl_Button_B000:
    case kControl_Button_B005:
        {
			//검색기능은 잠시 막아둔다.
           /* CCtrlListWZ* list_control = static_cast<CCtrlListWZ*>(GetControlWZ(GetControlID_byPosition(kControl_List_L001)));
            if (list_control)
            {
                if (list_control->GetState() == WZ_SHOW)
                {
                    list_control->ShowWindowWZ(WZ_HIDE);
                }
                else
                {
                    list_control->ShowWindowWZ(WZ_SHOW);
                }
            }*/
        }
        break;
    case kControl_Button_BT00:
        {
			if(animatino_show_)
				return result;

            SetPage((page_ + 1));
        }
        break;
    case kControl_Button_BT01:
        {
			if(animatino_show_)
				return result;

            SetPage((page_ - 1));
        }
        break;
    case kControl_Button_B003:
        {
            TCHAR string[INTERFACE_STRING_LENGTH] = {0};
            CCtrlDxIMEEditBox* edit_control = static_cast<CCtrlDxIMEEditBox*>(GetControlWZ(StrToWzID("E000")));
            //검색 패킷 보내기
            if (edit_control)
            {
                GlobalFunc::GetText(edit_control, string, INTERFACE_STRING_LENGTH);
				SendPackSearchItem(1,string);
            }
        }
        break;
	case kControl_Button_BT02://구매하기
		{
			OnBuyItem();
		}
		break;
    }
    return result;
}

bool uiVendorSearch::OnLButtonDClick( SI_MESSAGE* message )
{
	bool result = true;
	DWORD control_position = GetControlPosition_byID(message->dwCtrlID);
	switch (control_position)
	 {
		case kControl_List_L000:
		{
			OnBuyItem();
		}
		break;
 	}

	return true;
}

bool SortPrice(const uiVendorSearch::VENDOR_DATA& member_info_ref_1, uiVendorSearch::VENDOR_DATA& member_info_ref2)
{
    return member_info_ref_1.price < member_info_ref2.price;
}
bool SortUnPrice(const uiVendorSearch::VENDOR_DATA& member_info_ref_1, uiVendorSearch::VENDOR_DATA& member_info_ref2)
{
    return member_info_ref_1.price > member_info_ref2.price;
}

void uiVendorSearch::SortList()
{
    if (order_value_)
    {
        std::sort(vendor_array_.Begin(), vendor_array_.End(), SortPrice);
    }
    else
    {
        std::sort(vendor_array_.Begin(), vendor_array_.End(), SortUnPrice);
    }
}

void uiVendorSearch::SetSearchType(BYTE type)
{
    CCtrlButtonWZ* button = static_cast<CCtrlButtonWZ*>(GetControlWZ(GetControlID_byPosition(kControl_Button_B005)));
    if (button)
    {
        if (type == 0)
        {
            button->SetTextID(2385); // 2384: 개인상점 물품 검색xp
        }
        else
        {
            button->SetTextID(2386); // 2385: 아이템 이름으로 찾기
        }
        set_search_type(type);
    }
}
//=====================================================================================
//	1.리스트컨트롤 데이터삽입
//=====================================================================================
void uiVendorSearch::SetListData(int index,int sub_index,TCHAR* string)
{
    DWORD text_style = DT_VCENTER | DT_CENTER;	// 기본값

	COLORREF ResultColor = RGBA(255,255,255,255);
	TCHAR szResult[INTERFACE_STRING_LENGTH] ={0,};

	bool isPriceItem = FALSE;

	switch(sub_index)
	{
	case VENDOR_PRICE:
	case VENDOR_TOTAL_PRICE:
		{
			NUMBERFMT nFmt = { 0, 0, 3, _T("."), _T(","), 1 };

			int Lensize = strlen(string);
			ResultColor = INPRICECOLORTABLE_[GetItemPriceColorLevel(Lensize)];

			::GetNumberFormat(
				LOCALE_SYSTEM_DEFAULT, 
				NULL, 
				string, 
				&nFmt, 
				szResult, 
				sizeof(szResult));

			isPriceItem = TRUE;
		}
		break;
	default:
		ResultColor = RGBA(255,255,255,255);
	}
	

	
    GlobalFunc::SetItem(vendor_search_list_ptr_, index, sub_index,
		(isPriceItem == TRUE ? szResult : string),
        c_wiListFont,
        text_style,
        RGBA(255,255,255,0), 
        ResultColor, 
        RGBA(255,255,255,255));
}
//=====================================================================================
//	2.컨트롤 갱신
//=====================================================================================
void uiVendorSearch::RefreshListData(nArray<VENDOR_DATA> &ArrayType)
{
    if(!vendor_search_list_ptr_)
    {
        return;
    }


    //VENDOR_DATA stData;
    TCHAR temp_buffer[INTERFACE_STRING_LENGTH] = {0,};
    util::_tstring number_string;

    //1.카운팅
    int max_count = ArrayType.Size();

    //0.리스트 목록정보삽입
    vendor_search_list_ptr_->DeleteItemAll();
    vendor_search_list_ptr_->SetBeginDrawIndex(0);
	DeleteAllSlot();

    if (max_count <= 0)
    {
		//패킷이 없을경우는 리스트 정리함.
		vendor_search_list_ptr_->DeleteItemAll();
		vendor_search_list_ptr_->SetBeginDrawIndex(0);
		DeleteAllSlot();

        return;
    }

    GlobalFunc::SetReportButtonInfo(vendor_search_list_ptr_, VENDOR_ITEM_NAME, 
        c_iListWidth[VENDOR_ITEM_NAME], _T("item_name"));
    GlobalFunc::SetReportButtonInfo(vendor_search_list_ptr_, VENDOR_NAME, 
        c_iListWidth[VENDOR_NAME], _T("vendor_name"));
    GlobalFunc::SetReportButtonInfo(vendor_search_list_ptr_, VENDOR_PRICE, 
        c_iListWidth[VENDOR_PRICE], _T("price"));
    GlobalFunc::SetReportButtonInfo(vendor_search_list_ptr_, VENDOR_COUNT, 
        c_iListWidth[VENDOR_COUNT], _T("count"));
    GlobalFunc::SetReportButtonInfo(vendor_search_list_ptr_, VENDOR_TOTAL_PRICE, 
        c_iListWidth[VENDOR_TOTAL_PRICE], _T("total_price"));
    GlobalFunc::SetReportButtonInfo(vendor_search_list_ptr_, VENDOR_PLACE, 
        c_iListWidth[VENDOR_PLACE], _T("place"));

   // SortList();//정렬

    //2.데이터삽입
    int iAdd = 0;
    for(int line_index = 0; line_index < max_count; ++ line_index)
    {
        int index = vendor_search_list_ptr_->InsertItemBack();
        vendor_search_list_ptr_->SetItemData(index, &(ArrayType[line_index]));
        ++iAdd;
        for(int row_index=0; row_index < (VENDOR_PLACE+1); ++row_index)
        {

			TCHAR tempwarning[256] ={0,};
            ZeroMemory(temp_buffer, sizeof(temp_buffer));

			DeleteSlot(line_index, NULL);
			InsertSlot(line_index,ArrayType[line_index].RenderInfo,ArrayType[line_index].item_code);

//			SCSlot& real_slot = GetSlot(line_index);
			

            switch(row_index)
            {
            case VENDOR_ITEM_NAME:	// 아이템 이름
                {
// 					if(GameFunc::IsCheckItem(real_slot))
// 					{
// 						Sprintf(temp_buffer,"%s-%s", ArrayType[line_index].Item_Name_,g_InterfaceManager.GetInterfaceString(400938));
// 					}
// 					else
//					{
						Sprintf(temp_buffer,"%s",ArrayType[line_index].Item_Name_);
//					}
                }
                break;
            case VENDOR_NAME:
                {
					Snprintf(temp_buffer,INTERFACE_STRING_LENGTH-1,_T("%s"), ArrayType[line_index].vendor_name);	
                }
                break;
            case VENDOR_COUNT:
                {
                    Snprintf(temp_buffer,INTERFACE_STRING_LENGTH-1, "%d", ArrayType[line_index].count);
                }
                break;
            case VENDOR_PRICE:
                {                    
                    Snprintf(temp_buffer,INTERFACE_STRING_LENGTH-1, _T("%I64u"), ArrayType[line_index].price);
                }
                break;
            case VENDOR_TOTAL_PRICE:
                {                    
                    Snprintf(temp_buffer,INTERFACE_STRING_LENGTH-1, _T("%I64u"), ArrayType[line_index].total_price);
                }
                break;
            case VENDOR_PLACE:
                {
                    const sMAPINFO *map_info = 
                        MapInfoParser::Instance()->FindMapInfo(ArrayType[line_index].place);

                    if (map_info)
                    {
                        g_InterfaceManager.GetInterfaceString(map_info->dwNCode, temp_buffer, INTERFACE_STRING_LENGTH);
                    }
                    else
                    {
                        sprintf(temp_buffer, "Unknown Map Code");
                    }
                }
                break;
			}

            SetListData(line_index, row_index, temp_buffer);
        }
    }
}

void uiVendorSearch::OnUpdateSolarDialog()
{
	UpdateToolTip();

	{
		if (list_refresh_timer_.SpentTime() > 2000)// 2.0초 이내에 재요청 불가
		{
			animation_refresh_timer_.Off();
			list_refresh_timer_.Off();
			

			MouseCursor::Instance()->ShowCursor(TRUE);

			order_value_ = true;
			RefreshListData(vendor_array_);

			ShowAnimation(false);
		}
	}

    
}

void uiVendorSearch::UpdateToolTip()
{
    int iMouseX = Mouse::Instance()->GetMouseX();
    int iMouseY = Mouse::Instance()->GetMouseY();

    // 스킵처리 
    // 인터페이스에서 Dialog 바탕에서는 Mouse Over가 오지 않는다.
    bool is_mouse_over = false;
    int loop_end = kControl_Size;
    for (POSTYPE position = 0; position < loop_end; ++position)
    {
        CControlWZ* control = GetControlWZ(GetControlID_byPosition(position));
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

    if (!is_mouse_over)
    {
        //tooltip_string_code_ = 0;
        m_MouseOverObject.eMouseOverType = eMOUSE_OVER_MAX;
    }
    else
    {
        /*if (tooltip_string_code_ > 0)
        {
        uiToolTipMan* tooltip_manager = GET_DIALOG_MANAGER(uiToolTipMan, UIMAN_TOOLTIP);
        if (tooltip_manager)
        {
        tooltip_manager->RegisterTooltipForWareNInventoryLock(tooltip_string_code_);
        }
        }*/
    }
}

void uiVendorSearch::MouseOver_MsgProc(SI_MESSAGE* message)
{
    switch (message->eResultMsg) 
    {
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
    }
}

void uiVendorSearch::OnRenderSolarDialog()
{
    if (!item_unit_renderer_)
    {
        return;
    }

    item_unit_renderer_->Render();

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

	PlayAnimation();
}

RC::eSLOT_INSERT_RESULT uiVendorSearch::InsertSlot(POSTYPE at_pos, RENDER_ITEMSTREAMEX& RenderInfo,CODETYPE ItemCodes)
{
	DeleteSlot(at_pos,NULL);

	static DWORD SERIAL = 20000000;
	

	SCItemSlot SlotItemData;

	SlotItemData.Copy(RenderInfo);
	SlotItemData.SetSerial(++SERIAL);
	SlotItemData.SetSlotType(ST_ITEM);
	//SlotItemData.SetCode((SLOTCODE)ItemCodes);

    /// 슬롯에 넣음
    RC::eSLOT_INSERT_RESULT eInsertSlotResult = SCSlotContainer::InsertSlot(at_pos, SlotItemData);

    // HERO이기 때문에 Render

    // 시리얼은 설정되서 온다.
    /// 인벤토리
    CControlWZ* control_ptr = GetControlWZ(GetControlID_byPosition(at_pos));

	if(control_ptr)
	{
		RECT rect = control_ptr->GetSizeRect();

		SCSlot& real_slot = GetSlot(at_pos);

		if(item_unit_renderer_)
		{
		    item_unit_renderer_->AddItemUnit(SlotItemData.GetSerial(), SlotItemData.GetCode(), &rect, &real_slot,true ,control_ptr);
		}
		else
		{
		    assert(item_unit_renderer_);
		}
		ReFreshRenderSlot();
	}

    return eInsertSlotResult;
}
//------------------------------------------------------------------------------ 
BOOL uiVendorSearch::DeleteSlot(POSTYPE at_pos, SCSlot* slot_out)
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
void uiVendorSearch::DeleteAllSlot()
{
    if (item_unit_renderer_ == NULL)
    {
        assert(item_unit_renderer_);
        return;
    }
    for (int slot_pos = kControl_Picture_P029; slot_pos <= kControl_Picture_P040; ++slot_pos)
    {
        this->DeleteSlot(slot_pos, NULL);
    }
}

void uiVendorSearch::ReFreshRenderSlot()
{
    ItemUnitRenderer::RenderUnit* item_unit_render = NULL;
    RECT slot_rect;
    CControlWZ* control_ptr = NULL;

    for (int i = 0; i < GetMaxSlotNum(); ++i)
    {
        SCSlot& rSlot = GetSlot(i);
        if (IsEmpty(i))
        {
            continue;
        }

        {
            // 보여줘야할 목록
            if(item_unit_renderer_ == NULL)
            {
                continue;
            }
            item_unit_render = item_unit_renderer_->GetItemUnit(rSlot.GetSerial());

            control_ptr = GetControlWZ(GetControlID_byPosition(i)); //i가 바로 위치
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
}

void uiVendorSearch::NetworkProc(MSG_BASE* message_ptr)
{
    switch (message_ptr->m_byProtocol)
    {
    case CG_VENDOR_ENHANCED_SEARCH_ACK://요청결과:
        {
 			MSG_CG_VENDOR_ENHANCED_SEARCH_ACK* PRecvMsg = (MSG_CG_VENDOR_ENHANCED_SEARCH_ACK*)message_ptr;

			//검색이 시작되면 현재 있는 아이템은 모두 지운다?

			DeleteAllSlot();
			vendor_search_list_ptr_->DeleteItemAll();
			//어레이의 값은 모두 클리어
			vendor_array_.Clear();

			//검색시 이전패이지와 현재패이지는 같다.
            pre_page_= PRecvMsg->search_result.current_page_ + 1;
			page_ = PRecvMsg->search_result.current_page_ + 1;

			int ItemCount	= PRecvMsg->search_result.item_count_;
            max_page_ = PRecvMsg->search_result.total_page_ + 1;

			TCHAR string[256] = {0,};
			CCtrlStaticWZ* static_control = static_cast<CCtrlStaticWZ*>(GetControlWZ(GetControlID_byPosition(kControl_Text_ST00)));
			if (static_control)
			{
				sprintf(string, "%d/%d", page_,max_page_);
				static_control->SetTextWZ(string);
			}


            for (int i = 0; i < ItemCount; ++i)
            {
                VENDOR_DATA vendor;
				sprintf(vendor.vendor_name, PRecvMsg->search_result.vendor_items[i].vendor_info_.m_pszID);
				vendor.item_code = PRecvMsg->search_result.vendor_items[i].item_info_ex_.m_Stream.m_Stream.Part.wCode;

				{
					ItemInfoParser* const pItemInfoParser = ItemInfoParser::Instance();
					BASE_ITEMINFO* item_info_ = pItemInfoParser->GetItemInfo(vendor.item_code);

                    if(item_info_->IsWeapon() || item_info_->IsArmor() ||
                        item_info_->IsAccessory() || item_info_->IsSpecialAccessory() ||
                        item_info_->IsRing() || item_info_->IsNecklace())
						vendor.count = 1;
					else
						vendor.count =  PRecvMsg->search_result.vendor_items[i].item_info_ex_.m_Stream.m_Stream.Part.byDura;
				}

				vendor.price = PRecvMsg->search_result.vendor_items[i].item_info_ex_.m_Money;
				vendor.total_price = PRecvMsg->search_result.vendor_items[i].item_info_ex_.m_Money * vendor.count;
				vendor.place =  PRecvMsg->search_result.vendor_items[i].vendor_info_.zone_key_;
				vendor.PlayerKeyValue_ = PRecvMsg->search_result.vendor_items[i].vendor_info_.m_dwPlayerKey;
				vendor.BuySlotPos_ = PRecvMsg->search_result.vendor_items[i].item_info_ex_.m_Stream.m_Pos;

				vendor.RenderInfo = PRecvMsg->search_result.vendor_items[i].item_info_ex_.m_Stream.m_Stream;
				vendor.SlotInfo.Copy(PRecvMsg->search_result.vendor_items[i].item_info_ex_.m_Stream.m_Stream);
			
				TCHAR tempEnchanName[512];
				TCHAR ItemName[512];

				memset(tempEnchanName,0,sizeof(TCHAR)*512);
				SLOTCODE code = vendor.SlotInfo.GetItemCode();
				int inchant = (int)vendor.SlotInfo.GetEnchant();

				GameFunc::GetItemName(&vendor.SlotInfo,vendor.SlotInfo.GetItemInfo(),ItemName,512);

				Sprintf(tempEnchanName,"%s+%d",ItemName,inchant);
				sprintf(vendor.Item_Name_, inchant > 0 ? tempEnchanName : ItemName );

				vendor_array_.PushBack(vendor);
            }
        }
        break;
    }
}

__forceinline int uiVendorSearch::GetItemPriceColorLevel( int stringLen )
{
	switch(stringLen)
	{
	case VECTOR_STRING_PRICE_LOW1:
	case VECTOR_STRING_PRICE_LOW2:
	case VECTOR_STRING_PRICE_LOW3:
	case VECTOR_STRING_PRICE_LOW4:
	case VECTOR_STRING_PRICE_LOW5:
		return 0;

	case VECTOR_STRING_PRICE_MID:
		return 1;

	case VECTOR_STRING_PRICE_HIGH:
		return 2;

	case VECTOR_STRING_PRICE_VERYHIGH:
		return 3;
	case VECTOR_STRING_PRICE_VERYHIGH2:
		return 4;
	case VECTOR_STRING_PRICE_VERYHIGH3:
		return 5;
	case VECTOR_STRING_PRICE_VERYHIGH4:
		return 6;
#ifdef _NA_008678_20160203_ADD_HEIM_TRADINGVOLUME_NOTIFICATION_OPTION
	case VECTOR_STRING_PRICE_VERYHIGH5:
		return 7;
#endif // _NA_008678_20160203_ADD_HEIM_TRADINGVOLUME_NOTIFICATION_OPTION
	}

#ifdef _NA_008678_20160203_ADD_HEIM_TRADINGVOLUME_NOTIFICATION_OPTION
	return 7;
#else
	return 6;
#endif // _NA_008678_20160203_ADD_HEIM_TRADINGVOLUME_NOTIFICATION_OPTION
}

void uiVendorSearch::OnBuyItem()
{
	CCtrlListWZ* ItemInfoList = static_cast<CCtrlListWZ*>(GetControlWZ_byPosition(kControl_List_L000));
	if (ItemInfoList )
	{

		DWORD FieldID = g_pMap->GetCurMMCode();

		int selected_index = ItemInfoList->GetCurSel();

		if(selected_index == -1)
			return;

		VENDOR_DATA SellItemData = vendor_array_[selected_index];

#ifdef _NA_008571_20151116_MODIFY_VENDOR_SEARCH
#else
		if(FieldID != SellItemData.place)
		{
			TCHAR notice_message[INTERFACE_STRING_LENGTH] = {'\0', };
			TCHAR temp_buffer[INTERFACE_STRING_LENGTH] = {'\0', };

			const sMAPINFO *map_info = MapInfoParser::Instance()->FindMapInfo(SellItemData.place);

			if (map_info)
			{
				g_InterfaceManager.GetInterfaceString(map_info->dwNCode, temp_buffer, INTERFACE_STRING_LENGTH);
			}

			g_InterfaceManager.GetInterfaceStringFormat(notice_message,INTERFACE_STRING_LENGTH,
														2395,temp_buffer);

			uiSystemMan* system_manager = 
				static_cast<uiSystemMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM));

			if (system_manager)
			{
				system_manager->Sys_VerifyLock(notice_message);
			}

			return;
		}
#endif // _NA_008571_20151116_MODIFY_VENDOR_SEARCH

		bool is_focus_change = FALSE;

		if (open_shopOwner_Key_ == SellItemData.PlayerKeyValue_)
		{
			is_focus_change = TRUE;
		}

		bool is_show_packet = FALSE;

		uiVendorBuy* pVendorBuy	= GET_CAST_DIALOG( uiVendorBuy, IM_VENDOR_MANAGER::VENDOR_BUY );
		if( pVendorBuy )
		{
			if ( !pVendorBuy->IsVisibleWindow() )
			{
				is_show_packet = TRUE;
			}
			else
			{
				if (is_focus_change == FALSE )
				{
					pVendorBuy->Exit();
					pVendorBuy->ShowDialog(FALSE);
					is_show_packet = TRUE;
				}
				else
				{
					pVendorBuy->SetBuyItemsSearchFoucs(SellItemData.BuySlotPos_,SellItemData.count,SellItemData.price);
				}
			}

			isNowButClick_ = SellItemData;

			uiVendorMan * puiVendorMan = GET_DIALOG_MANAGER(uiVendorMan, UIMAN_VENDOR);

			if (puiVendorMan && is_show_packet)
			{
				open_shopOwner_Key_ = SellItemData.PlayerKeyValue_;
				puiVendorMan->SetPendingSaler( SellItemData.PlayerKeyValue_);
				MSG_CG_VENDOR_VIEW_START_SYN sendMsg;
				sendMsg.m_byCategory    = CG_VENDOR;
				sendMsg.m_byProtocol    = CG_VENDOR_VIEW_START_SYN;
				sendMsg.m_dwPlayerKey   = SellItemData.PlayerKeyValue_;
				GlobalFunc::SendPacket( CI_GAMESERVERIDX, &sendMsg, sizeof(sendMsg) );
			}
		}
	}

	
}

void uiVendorSearch::OnKeyEnterInput( SI_MESSAGE* message )
{
	TCHAR string[INTERFACE_STRING_LENGTH] = {0};
	CCtrlDxIMEEditBox* edit_control = static_cast<CCtrlDxIMEEditBox*>(GetControlWZ(StrToWzID("E000")));
	//검색 패킷 보내기
	if (edit_control)
	{
		//검색 요청은 무조건 1패이지 부터 다시 시작한다.
		GlobalFunc::GetText(edit_control, string, INTERFACE_STRING_LENGTH);
		SendPackSearchItem(1,string);
	}
}

void uiVendorSearch::PlayAnimation()
{
	
	static FRECT img_rect = {0,};
	img_rect.left = 
		(float)GetControlWZ(GetControlID_byPosition(kControl_List_L000))->GetSizeRect().left;
	img_rect.top = 
		(float)GetControlWZ(GetControlID_byPosition(kControl_List_L000))->GetSizeRect().top;
	img_rect.right = 
		(float)GetControlWZ(GetControlID_byPosition(kControl_List_L000))->GetSizeRect().right;
	img_rect.bottom = 
		(float)GetControlWZ(GetControlID_byPosition(kControl_List_L000))->GetSizeRect().bottom;

	img_rect.left = img_rect.left + ((img_rect.right - img_rect.left - ANIMATION_FRAME_WIDTH) / 2) + 0.5f;
	img_rect.top = img_rect.top + ((img_rect.bottom - img_rect.top - ANIMATION_FRAME_HEIGHT) / 2);

	int frame = 30;//초당 애니메이션 프래임
	int count = ((animation_refresh_timer_.SpentTime() / frame) % ANIMATION_FRAME);

	if (animatino_show_)
	{
		g_pSunRenderer->RenderTexture(effect_texture_, img_rect.left, img_rect.top, 
			ANIMATION_FRAME_WIDTH, ANIMATION_FRAME_HEIGHT, 
			texture_rectangles_[count].left, texture_rectangles_[count].top, 
			texture_rectangles_[count].right, texture_rectangles_[count].bottom);
		++count;
	}
	else
	{
		//nop
	}
}

void uiVendorSearch::ShowAnimation(bool is_value)
{
	animatino_show_ = is_value;
}

void uiVendorSearch::InitTexture()
{
	effect_texture_ = g_pSunRenderer->x_pManagerTexture->LoadTexture(
		"Data\\Interface\\Loading.tga", TLS_ORIGINAL_SIZE | TLS_NO_MIPMAP | TLS_NO_MULTITHREAD);

	// 애니메이션 텍스쳐 위치 (12프레임) 프레임당 (40 * 40) 4 x 3 개
	int index = 0;
	float width = 0.0f;
	float height = 0.0f;

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			width = (j) * ANIMATION_FRAME_WIDTH;
			height = (i) * ANIMATION_FRAME_HEIGHT;

			texture_rectangles_[index].left = width+1;
			texture_rectangles_[index].top = height+1;
			texture_rectangles_[index].right = ANIMATION_FRAME_WIDTH-1;
			texture_rectangles_[index].bottom = ANIMATION_FRAME_HEIGHT-1;

			++index;
			if (index >= ANIMATION_FRAME)
			{
				break;
			}
		}
	}
}

__forceinline void uiVendorSearch::SendPackSearchItem( int page, const TCHAR* SearchName ,bool inForce )
{
	if(animatino_show_ && !inForce)
		return;

	MouseCursor::Instance()->ShowCursor(FALSE);

	ShowAnimation(true);
	animation_refresh_timer_.Off();
	animation_refresh_timer_.On(c_AnimationFullTime);
	list_refresh_timer_.Off();
	list_refresh_timer_.On(c_AnimationFullTime);

	int LenSize = CheckByte(SearchName);
	if(LenSize < 4 && !inForce)
		return;

	Snprintf(EnterSearchInput_, INTERFACE_STRING_LENGTH, SearchName);

	MSG_CG_VENDOR_ENHANCED_SEARCH_SYN SendPacket;
	SendPacket.page = page - 1;
	Snprintf(SendPacket.search_items, INTERFACE_STRING_LENGTH, SearchName);
	GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket));
}

__forceinline int uiVendorSearch::CheckByte( const char* charset )
{
	const char* ByteisChar = charset;
	int len = strlen(charset);
	
	int MaxByte = 0;
	for (int idx = 0 ; idx < len ; ++idx)
	{
		//상위 1바이트
		byte of = (byte)((ByteisChar[idx] & 0xff00) >> 8);

		if(of == 0) // 상위 1바이트가 0이면 는 알파벳
			MaxByte++;
		else
			MaxByte += 2;
	}

	return MaxByte;
}

void uiVendorSearch::RewindSearchPacket()
{
	//재검색에 한하여 현재 패이지를 다시 갱신 시킨다.
	if(strcmp(EnterSearchInput_,"SUN") == 0)
		SendPackSearchItem(page_,EnterSearchInput_,true);
	else
		SendPackSearchItem(page_,EnterSearchInput_);
}
#endif//_NA_005988_20120901_VENDOR_SEARCH_SYSTEM