#include "SunClientPrecompiledHeader.h"

#include "uiVendorList/uiVendorList.h"
#include "GlobalFunc.h"
#include "InterfaceManager.h"
#include "ChatDialog.h"
#include "uiChatMan/uiChatMan_def.h"

#ifdef _NA_0_20091208_VENDOR_ITEM_SEARCH
#include "ItemInfoParser.h"
#endif//_NA_0_20091208_VENDOR_ITEM_SEARCH
#ifdef _DH_SHOW_VENDOR_LIST_POPUP
#include "uiCharPopupMan/uiCharPopupMan.h"
#include "Hero.h"
#include "Object.h"
#include "ObjectManager.h"
#endif//_DH_SHOW_VENDOR_LIST_POPUP

#ifdef _NA_005988_20120901_VENDOR_SEARCH_SYSTEM
#include "../uiVendorMan/uiVendorMan_def.h"
#include "../uiVendorMan/uiVendorSearch/uiVendorSearch.h"
#endif

WzID uiVendorList::m_wzID[DLGPOS_MAX ] =
{
	StrToWzID("B002"),		//	DLGPOS_BTN_REFRESH,
	StrToWzID("B003"),		//	DLGPOS_BTN_FIND,
	StrToWzID("B004"),		//	DLGPOS_BTN_VIEW,
	StrToWzID("B001"),		//	DLGPOS_BTN_EXIT,
	StrToWzID("C001"),		//	DLGPOS_CHECK_ID,
	StrToWzID("C002"),		//	DLGPOS_CHECK_ROOM_NAME,
#ifdef _NA_0_20091208_VENDOR_ITEM_SEARCH
	StrToWzID("CT00"),		//	DLGPOS_CHECK_ITEM,
#endif//_NA_0_20091208_VENDOR_ITEM_SEARCH
	StrToWzID("L001"),		//	DLGPOS_LIST,
	StrToWzID("ED00"),		//	DLGPOS_EDIT_ID,
	StrToWzID("BT01"),		//	DLGPOS_BTN_LEFT,
	StrToWzID("BT00"),		//	DLGPOS_BTN_RIGHT
	StrToWzID("BT02"),		//	DLGPOS_BTN_ALL,
	StrToWzID("ST00"),		//	DLGPOS_TXT_PAGE,

	StrToWzID("T002"),		//	DLGPOS_REPORT_BUTTON_NUMBER
	StrToWzID("T003"),		//	DLGPOS_REPORT_BUTTON_VENDOR_NAME
	StrToWzID("T004"),		//	DLGPOS_REPORT_BUTTON_SELLER_NAME

#ifdef _NA_0_20091208_VENDOR_ITEM_SEARCH
	StrToWzID("PI00"),		//	DLGPOS_FIND_BACKIMG,
	StrToWzID("PI01"),		//	DLGPOS_FIND_SCROLLIMG,
	StrToWzID("LS00"),		//	DLGPOS_FIND_LIST,
	StrToWzID("VS00"),		//	DLGPOS_FIND_SCROLL,
#endif//_NA_0_20091208_VENDOR_ITEM_SEARCH
};					
			

void uiVendorList::OnShowWindow(BOOL val )
{
	if( val )
	{
		m_pList->DeleteItemAll();
		m_pEdit->ClearText();
		m_iPage = 1;
		keyMsg msg;
		ZeroMemory(&msg, sizeof(msg) );
		msg.dwType = eDoType_dialog_vendor_list;
		msg.wParam = InterfaceManager::DIALOG_VENDOR_LIST;
		msg.DoSomething = GlobalFunc::DoVendorList;
		g_KeyQueueManager.PushBack(msg);
	}
	else
	{
		if( m_bOpenVendorList )
			NET_SEND_CG_VENDOR_LIST_END_SYN();
		g_KeyQueueManager.DeleteMsg(eDoType_dialog_vendor_list);
#ifdef _NA_0_20091208_VENDOR_ITEM_SEARCH
        HideFindWord();
        set_search_end(true);   // 검색 종료 시키기
		HideFindWord();
        search_new_ = true;
		UpdateDialogSize();
		item_code_ = 0;
#endif//_NA_0_20091208_VENDOR_ITEM_SEARCH

#ifdef _DH_SHOW_VENDOR_LIST_POPUP
        uiBase *popup_menu = GET_DIALOG(IM_CHAR_POPUP_MANAGER::CHAR_POPUP_MENU);
		uiCharPopupMan* popup_menu_manager = GET_DIALOG_MANAGER(uiCharPopupMan, UIMAN_CHAR_POPUP);
        if (popup_menu && popup_menu_manager)
        {
            popup_menu->ShowInterface(FALSE);
        }
#endif//_DH_SHOW_VENDOR_LIST_POPUP
	}
}

#ifdef _DH_SHOW_VENDOR_LIST_POPUP
void uiVendorList::ShowCharacterPopupMenu(BOOL is_show, DWORD index)
{
    uiBase *popup_menu = GET_DIALOG(IM_CHAR_POPUP_MANAGER::CHAR_POPUP_MENU);

    if (popup_menu == NULL)
    {
        return;
    }

    if (index < 0 || index >= COUNT_VENDOR_IN_PAGE)
    {
        return;
    }
    
    if (is_show && g_pHero)
    {
        if (m_dwObjectKeyAtList[index] == g_pHero->GetObjectKey()) 
        {
            return;	// 내가 타겟이면 리턴
        }

        uiCharPopupMan* popup_menu_manager = GET_DIALOG_MANAGER(uiCharPopupMan, UIMAN_CHAR_POPUP);
        Object* pObj = g_ObjectManager.GetObject(m_dwObjectKeyAtList[index]);
        if (pObj)
        {
            if(!pObj->IsKindOfObject(PLAYER_OBJECT))			// 일반유저 아니면 리턴
            {
                return;
            }
        }
        else
        {
            TCHAR vendor_name[MAX_CHARNAME_LENGTH+1] = {0,};
            sprintf(vendor_name, m_pList->GetItemText(index, 2));
            if (popup_menu_manager && strcmp(vendor_name,"")!=0)
            {
                popup_menu_manager->CheckPopupType(0, vendor_name, PLAYER_OBJECT);//이름만 알지만 사람이니까 이름이 있는 것
            }
            else
            {
                return;//알수없어
            }
        }

        if (popup_menu_manager)
        {
            popup_menu_manager->set_is_vendor_popup(true);
            if (pObj)
            {
                popup_menu_manager->CheckPopupType(m_dwObjectKeyAtList[index], pObj->GetName(), pObj->GetObjectType());
            }
            else
            {
                popup_menu_manager->CheckPopupType(m_dwObjectKeyAtList[index]);
            }
            popup_menu->ShowInterface(TRUE);
        }
    }
    else
    {
        popup_menu->ShowInterface(FALSE);
    }
}
#endif//_DH_SHOW_VENDOR_LIST_POPUP

void uiVendorList::MessageProc(SI_MESSAGE * pMessage )
{
	TCHAR szMessage[INTERFACE_STRING_LENGTH] = { 0,};
	switch( pMessage->eResultMsg )
	{
	case RT_MSG_LBUTTONCLICK:
		{
#ifdef _DH_SHOW_VENDOR_LIST_POPUP
            uiBase *popup_menu = GET_DIALOG(IM_CHAR_POPUP_MANAGER::CHAR_POPUP_MENU);
            uiCharPopupMan* popup_menu_manager = GET_DIALOG_MANAGER(uiCharPopupMan, UIMAN_CHAR_POPUP);
            if (popup_menu && popup_menu_manager)
            {
                popup_menu->ShowInterface(FALSE);
            }
#endif//_DH_SHOW_VENDOR_LIST_POPUP

			switch ( getControlIDToPos(pMessage->dwCtrlID) )
			{
			case DLGPOS_BTN_REFRESH:
				{
					m_iSearchType = PAGE_REQUIRE_KIND_ALL;
					this->NET_SEND_CG_VENDOR_REQUIRE_PAGE_SYN(m_iPage , PAGE_REQUIRE_KIND_ALL);
				}
				break;

			case DLGPOS_BTN_FIND:
				{
#ifdef _NA_0_20091208_VENDOR_ITEM_SEARCH
					DWORD item_code = 0;
#endif//_NA_0_20091208_VENDOR_ITEM_SEARCH

					TCHAR szFindMsg[MAX_VENDOR_SEARCH_LENGTH + 1];
					memset(szFindMsg , 0 , sizeof(szFindMsg) );
					GlobalFunc::GetText(m_pEdit , szFindMsg , MAX_VENDOR_SEARCH_LENGTH);
					szFindMsg[MAX_VENDOR_SEARCH_LENGTH] = '\0';
					int size = _tcslen(szFindMsg);
					if( size == 0 )
						return;

					if( size > 3 )
					{
						if( m_pCheck[__DLGPOS_CHECK_ID]->GetCheckState() )
						{
							m_iSearchType = PAGE_REQUIRE_KIND_ID;
						}
#ifdef _NA_0_20091208_VENDOR_ITEM_SEARCH
						else if(m_pCheck[__DLGPOS_CHECK_ITEM]->GetCheckState())
						{
							m_iSearchType = PAGE_REQUIRE_KIND_ITEM;
							item_code = item_code_;
						}
#endif//_NA_0_20091208_VENDOR_ITEM_SEARCH
						else
						{
							m_iSearchType = PAGE_REQUIRE_KIND_TITLE;
						}
						m_pList->DeleteItemAll();
						m_pList->SetCurSel(-1);
						m_pEdit->ClearText();

#ifdef _NA_0_20091208_VENDOR_ITEM_SEARCH
						this->NET_SEND_CG_VENDOR_SEARCH_SYN(szFindMsg, item_code);
						item_code_ = 0;
#else
						this->NET_SEND_CG_VENDOR_SEARCH_SYN(szFindMsg);
#endif//_NA_0_20091208_VENDOR_ITEM_SEARCH
					}
					else
					{
						// 5821	검색 단어의 길이가 너무 짧습니다.
						g_InterfaceManager.GetInterfaceString(5821 , szMessage ,INTERFACE_STRING_LENGTH);
						GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT , szMessage);
					}
				}
				break;

			case DLGPOS_BTN_VIEW:
				{
#ifdef _NA_005988_20120901_VENDOR_SEARCH_SYSTEM
					uiVendorSearch * pVendorSearch = GET_CAST_DIALOG( uiVendorSearch, IM_VENDOR_MANAGER::VENDOR_SEARCH );
					if(pVendorSearch && !pVendorSearch->IsVisibleWindow())
					{
						g_InterfaceManager.ShowDialog(IM_VENDOR_MANAGER::VENDOR_SEARCH, TRUE);
					}
#endif
				}
				break;

			case DLGPOS_BTN_EXIT:
				{
					NET_SEND_CG_VENDOR_LIST_END_SYN();
				}		
				break;


			case DLGPOS_BTN_LEFT:
				{
					if( m_iPage > 1)
					{
						m_iPage -= 1;
						this->NET_SEND_CG_VENDOR_REQUIRE_PAGE_SYN(m_iPage , m_iLastSearchType);
					}
				}
				break;

			case DLGPOS_BTN_RIGHT:
				{
					if( m_iPage < m_iMaxPage )
					{
						m_iPage += 1;
						this->NET_SEND_CG_VENDOR_REQUIRE_PAGE_SYN(m_iPage ,m_iLastSearchType);
					}
				}
				break;

			case DLGPOS_BTN_ALL:
				{
					m_iSearchType = PAGE_REQUIRE_KIND_ALL;
					this->NET_SEND_CG_VENDOR_REQUIRE_PAGE_SYN(1, m_iSearchType);
				}
				break;

			case DLGPOS_CHECK_ID:
				{
					m_iSearchType = PAGE_REQUIRE_KIND_ID;
					m_pCheck[__DLGPOS_CHECK_ID]->SetCheckState(TRUE);
					m_pCheck[__DLGPOS_CHECK_ROOM_NAME]->SetCheckState(FALSE);
#ifdef _NA_0_20091208_VENDOR_ITEM_SEARCH
					m_pCheck[__DLGPOS_CHECK_ITEM]->SetCheckState(FALSE);
                    HideFindWord();
                    set_search_end(true);   // 검색 종료 시키기
#endif//_NA_0_20091208_VENDOR_ITEM_SEARCH
				}
				break;

			case DLGPOS_CHECK_ROOM_NAME:
				{
					m_iSearchType = PAGE_REQUIRE_KIND_TITLE;
					m_pCheck[__DLGPOS_CHECK_ROOM_NAME]->SetCheckState(TRUE);
					m_pCheck[__DLGPOS_CHECK_ID]->SetCheckState(FALSE);
#ifdef _NA_0_20091208_VENDOR_ITEM_SEARCH
					m_pCheck[__DLGPOS_CHECK_ITEM]->SetCheckState(FALSE);
                    HideFindWord();
                    set_search_end(true);   // 검색 종료 시키기
#endif//_NA_0_20091208_VENDOR_ITEM_SEARCH
				}
				break;
#ifdef _NA_0_20091208_VENDOR_ITEM_SEARCH
			case DLGPOS_CHECK_ITEM:
				{
					m_iSearchType = __DLGPOS_CHECK_ITEM;
					m_pCheck[__DLGPOS_CHECK_ROOM_NAME]->SetCheckState(FALSE);
					m_pCheck[__DLGPOS_CHECK_ID]->SetCheckState(FALSE);
					m_pCheck[__DLGPOS_CHECK_ITEM]->SetCheckState(TRUE);
                    HideFindWord();
				}
				break;
            case DLGPOS_FIND_LIST:
                {   // 검색어 추가해주기
                    if (find_list_ptr_ && m_pEdit)
                    {
                        int select_index =  find_list_ptr_->GetCurSel();
                        _ITEM_NAME_INFO select_item_name_info = searched_results_[select_index];
                        TCHAR select_item_name[MAX_VENDOR_SEARCH_LENGTH+1] = "";
                        if (m_pEdit)
                        {
                            item_code_ = select_item_name_info.m_dwItemCode;
                            strncpy(select_item_name, select_item_name_info.m_stName.c_str(), MAX_VENDOR_SEARCH_LENGTH);
				            GlobalFunc::SetText(m_pEdit, select_item_name);
				            m_pEdit->SetTextWZ(select_item_name);
                        }
                    }
                }
#endif//_NA_0_20091208_VENDOR_ITEM_SEARCH
			}
		}
		break;

#ifdef _DH_SHOW_VENDOR_LIST_POPUP
    case RT_MSG_RBUTTONCLICK:
        {
            switch ( getControlIDToPos( pMessage->dwCtrlID ) )
            {
            case DLGPOS_LIST:
                {
                    int index  =  m_pList->GetCurSel();
                    if( index != -1 )
                    {	
                        ShowCharacterPopupMenu(true, index);
                    }
                }
                break;
            }
        }
        break;
#endif//_DH_SHOW_VENDOR_LIST_POPUP

	case RT_MSG_LBUTTONDCLICK:
		{
			switch ( getControlIDToPos( pMessage->dwCtrlID ) )
			{
			case DLGPOS_LIST:
				{
					int index  =  m_pList->GetCurSel();
					if( index != -1 )
					{	
						NET_SEND_CG_VENDOR_VIEW_START_SYN(index);
					}
				}
				break;
			}
		}
		break;

#ifdef _NA_0_20091208_VENDOR_ITEM_SEARCH
	case RT_MSG_SB_LINETOP:
		{
			switch (getControlIDToPos(pMessage->dwCtrlID) )
			{
			case DLGPOS_FIND_SCROLL:
				{
					if (find_list_ptr_->GetListRowCnt() > MAX_VISIBLE_LIST_NUM)
					{
						if (find_scroll_current_value_ > 0)
						{
							find_scroll_current_value_ -= 1;
							find_scroll_->SetScrollPosWZ(find_scroll_current_value_);
							find_list_ptr_->SetBeginDrawIndex(find_scroll_current_value_);
						}
					}
				}
				break;
			}
		}
		break;

	case RT_MSG_SB_LINEBOTTOM:
		{
			switch ( getControlIDToPos(pMessage->dwCtrlID) )
			{
			case DLGPOS_FIND_SCROLL:
				{
					if (find_list_ptr_->GetListRowCnt() > MAX_VISIBLE_LIST_NUM)
					{
						if (find_scroll_current_value_ < find_scroll_value_max_)
						{
							find_scroll_current_value_ += 1;
							find_scroll_->SetScrollPosWZ(find_scroll_current_value_);
							find_list_ptr_->SetBeginDrawIndex(find_scroll_current_value_);
						}
					}

				}
				break;
			}
		}
		break;

	case RT_MSG_SB_THUMBPOSITION:
		{
			switch ( getControlIDToPos( pMessage->dwCtrlID ) )
			{
			case DLGPOS_FIND_SCROLL:
				{
					find_scroll_current_value_ = find_scroll_->GetScrollPosWZ();
					find_list_ptr_->SetBeginDrawIndex(find_scroll_current_value_);
				}
				break;
			}
		}
		break;
#endif//_NA_0_20091208_VENDOR_ITEM_SEARCH


#ifdef	_K2P_USE_CHATWINDOW_USING_VENDORLIST
	case RT_MSG_EDIT_ENTERKEYDOWN:
		{
			ChatDialog *pDlg= GET_CAST_DIALOG(ChatDialog, IM_CHAT_MANAGER::CHAT_DIALOG);
			if ( pDlg && !pDlg->IsAlwaysOnTop() )
			{			  
				if( !pDlg->IsVisibleWindow() )			
				{
					pDlg->ShowDialog(TRUE);
				}
			}
		}
		break;
#else
	case RT_MSG_EDIT_ENTERKEYDOWN:
		{
#ifdef _NA_0_20091208_VENDOR_ITEM_SEARCH
			TCHAR find_message[MAX_VENDOR_SEARCH_LENGTH + 1];
			memset(find_message , 0 , sizeof(find_message) );
			GlobalFunc::GetText(m_pEdit , find_message , MAX_VENDOR_SEARCH_LENGTH);
			find_message[MAX_VENDOR_SEARCH_LENGTH] = '\0';
			int size = _tcslen(find_message);

            if (search_end())   // 이전 검색이 끝난 경우만 새로운 검색 가능
            {
			    if (size > 3)
			    {
				    if (m_pCheck[__DLGPOS_CHECK_ITEM]->GetCheckState())
				    {
                        if (_stricmp(find_message, search_string()) != 0)
                        {   // 방금 검색한 문자열이 아니라면
					        m_iSearchType = PAGE_REQUIRE_KIND_ITEM;
                            set_search_string(find_message);
                            set_search_end(false);
                        }
				    }
			    }
			    else
			    {
				    GlobalFunc::SetText(m_pEdit, find_message);
				    m_pEdit->ClearText();
				    HideFindWord();
				    UpdateDialogSize();
				    item_code_ = 0;
			    }
            }
#endif//_NA_0_20091208_VENDOR_ITEM_SEARCH
		}
		break;
#endif
	default:
		{
			SolarDialog::MessageProc(pMessage);
		}
		break;
	}
}
			
					
					
					
					
					