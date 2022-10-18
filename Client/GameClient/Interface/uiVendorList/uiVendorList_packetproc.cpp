#include "SunClientPrecompiledHeader.h"

#include "uiVendorList/uiVendorList.h"
#include "Hero.h"
#include "GlobalFunc.h"
#include "InterfaceManager.h"
#include "uivendorman/uiVendorMan.h"
#include "uiVendorBuy/uiVendorBuy.h"

void uiVendorList::NetworkProc(MSG_BASE * pMsg )
{
	switch ( pMsg->m_byProtocol )
	{
	case CG_VENDOR_LIST_START_ACK:
		{
			m_bIsSend[IS_CG_VENDOR_LIST_START_SYN] = FALSE;
			m_bOpenVendorList = TRUE;
			ShowInterface(TRUE);
			NET_SEND_CG_VENDOR_REQUIRE_PAGE_SYN(1 , PAGE_REQUIRE_KIND_ALL);
		}
		break;

	case CG_VENDOR_LIST_START_NAK:
		{
			m_bIsSend[IS_CG_VENDOR_LIST_START_SYN] = FALSE;
			MSG_CG_VENDOR_LIST_START_NAK * pRecvPacket = (MSG_CG_VENDOR_LIST_START_NAK *) pMsg;
			ErrorCode(pRecvPacket->m_byErrorCode);
		}
		break;


	case CG_VENDOR_REQUIRE_PAGE_ACK:
		{
			m_bIsSend[IS_CG_VENDOR_REQUIRE_PAGE_SYN] = FALSE;
			MSG_CG_VENDOR_REQUIRE_PAGE_ACK * pRecvPacket = (MSG_CG_VENDOR_REQUIRE_PAGE_ACK *) pMsg;
			VENDOR_LIST_TOTAL_INFO VendorListRecv;
			ZeroMemory(&VendorListRecv, sizeof(VendorListRecv));
			pRecvPacket->ConvertForRecv(VendorListRecv);
			FillVendorListData(&VendorListRecv);

		}
		break;

	case CG_VENDOR_REQUIRE_PAGE_NAK:
		{
			m_pList->DeleteItemAll();
			m_bIsSend[IS_CG_VENDOR_REQUIRE_PAGE_SYN] = FALSE;
			MSG_CG_VENDOR_REQUIRE_PAGE_NAK * pRecvPacket = (MSG_CG_VENDOR_REQUIRE_PAGE_NAK *) pMsg;
			
		}
		break;

	case CG_VENDOR_SEARCH_ACK:
		{
			m_bIsSend[IS_CG_VENDOR_SEARCH_SYN] = FALSE;
			NET_SEND_CG_VENDOR_REQUIRE_PAGE_SYN(1 , m_iSearchType);
#ifdef _NA_0_20091208_VENDOR_ITEM_SEARCH
            HideFindWord();
#endif//_NA_0_20091208_VENDOR_ITEM_SEARCH
		}
		break;

	case CG_VENDOR_SEARCH_NAK:
		{
			m_bIsSend[IS_CG_VENDOR_SEARCH_SYN] = FALSE;
			MSG_CG_VENDOR_SEARCH_NAK * pRecvPacket = ( MSG_CG_VENDOR_SEARCH_NAK *) pMsg;
			ErrorCode(pRecvPacket->m_byErrorCode);
		}
		break;

	case CG_VENDOR_LIST_END_ACK:
		{
			m_bIsSend[IS_CG_VENDOR_LIST_END_SYN]  =FALSE;
			g_pHero->SetBeHaveState(PLAYER_BEHAVE_IDLE_STATE);
			m_bOpenVendorList = FALSE;
			ShowInterface(FALSE);
		}
		break;

	case CG_VENDOR_LIST_END_NAK:
		{
			m_bIsSend[IS_CG_VENDOR_LIST_END_SYN]  =FALSE;
			MSG_CG_VENDOR_LIST_END_NAK * pRecvPacket = (MSG_CG_VENDOR_LIST_END_NAK * ) pMsg;
			ErrorCode(pRecvPacket->m_byErrorCode);
		}
		break;
	}
}



void uiVendorList::NET_SEND_CG_VENDOR_LIST_START_SYN()
{
	if( m_bIsSend[IS_CG_VENDOR_LIST_START_SYN] )
		return;

	m_bIsSend[IS_CG_VENDOR_LIST_START_SYN] = TRUE;
 	MSG_CG_VENDOR_LIST_START_SYN send;
	GlobalFunc::SendPacket(CI_GAMESERVERIDX , &send , sizeof(send) );

}


void uiVendorList::NET_SEND_CG_VENDOR_LIST_END_SYN()
{

	m_bIsSend[IS_CG_VENDOR_LIST_END_SYN] = TRUE;
	MSG_CG_VENDOR_LIST_END_SYN send;
	GlobalFunc::SendPacket(CI_GAMESERVERIDX , &send , sizeof(send) );
}


void uiVendorList::NET_SEND_CG_VENDOR_REQUIRE_PAGE_SYN(int page , int kind)
{
	if( m_bIsSend[IS_CG_VENDOR_REQUIRE_PAGE_SYN] )
		return;

	m_iPage = page;
	m_iLastSearchType = kind;
	m_bIsSend[IS_CG_VENDOR_REQUIRE_PAGE_SYN] = TRUE;
	MSG_CG_VENDOR_REQUIRE_PAGE_SYN send;
	send.m_byKind = kind ;
	send.m_byPage = page;
	GlobalFunc::SendPacket(CI_GAMESERVERIDX , &send, sizeof(send) );
}

#ifdef _NA_0_20091208_VENDOR_ITEM_SEARCH
void uiVendorList::NET_SEND_CG_VENDOR_SEARCH_SYN(TCHAR* pFindName, DWORD item_code)
#else
void uiVendorList::NET_SEND_CG_VENDOR_SEARCH_SYN(TCHAR* pFindName)
#endif//_NA_0_20091208_VENDOR_ITEM_SEARCH
{
	if (m_bIsSend[IS_CG_VENDOR_SEARCH_SYN])
    {
		return;
    }
#ifdef _NA_0_20091208_VENDOR_ITEM_SEARCH
	if (m_iSearchType == PAGE_REQUIRE_KIND_ITEM)
	{
        int select_index =  find_list_ptr_->GetCurSel();
        if (select_index < 0)
        {
            return;
        }
        _ITEM_NAME_INFO select_item_name_info = searched_results_[select_index];
        if (item_code == 0 || _stricmp(select_item_name_info.m_stName.c_str(), pFindName) != 0)
        {
		    if (_tcslen(pFindName) > 3)
		    {
			    if (m_pCheck[__DLGPOS_CHECK_ITEM]->GetCheckState())
			    {
				    m_iSearchType = PAGE_REQUIRE_KIND_ITEM;
                    set_search_string(pFindName);
                    search_new_ = true;
                    set_search_end(false);
			    }
		    }
		    else
		    {
			    GlobalFunc::SetText(m_pEdit, "");
			    m_pEdit->ClearText();
			    HideFindWord();
			    item_code_ = 0;
		    }
            return;
        }
	}
#endif//_NA_0_20091208_VENDOR_ITEM_SEARCH
	m_bIsSend[IS_CG_VENDOR_SEARCH_SYN] = TRUE;
	MSG_CG_VENDOR_SEARCH_SYN send;
	StrnCopy(send.m_sPattern , pFindName , MAX_VENDOR_SEARCH_LENGTH);
	send.m_ebyKind = m_iSearchType;
#ifdef _NA_0_20091208_VENDOR_ITEM_SEARCH
	send.m_ItemCode = (SLOTCODE)item_code;
#endif//_NA_0_20091208_VENDOR_ITEM_SEARCH

	GlobalFunc::SendPacket(CI_GAMESERVERIDX , &send , sizeof(send) );
}


void uiVendorList::NET_SEND_CG_VENDOR_VIEW_START_SYN(int index)
{
	uiVendorBuy* pVendorBuy	= GET_CAST_DIALOG( uiVendorBuy, IM_VENDOR_MANAGER::VENDOR_BUY );
	if( pVendorBuy )
	{
		if( pVendorBuy->IsVisible() )
		{
			pVendorBuy->QueryViewEnd();
		}
	}

	if( index >= 0 && index < COUNT_VENDOR_IN_PAGE)
	{
		uiVendorMan * puiVendorMan = GET_DIALOG_MANAGER(uiVendorMan, UIMAN_VENDOR);

		if (puiVendorMan)
		{
			puiVendorMan->SetPendingSaler( m_dwObjectKeyAtList[index]);
			MSG_CG_VENDOR_VIEW_START_SYN sendMsg;
			sendMsg.m_byCategory    = CG_VENDOR;
			sendMsg.m_byProtocol    = CG_VENDOR_VIEW_START_SYN;
			sendMsg.m_dwPlayerKey   = m_dwObjectKeyAtList[index];
			GlobalFunc::SendPacket( CI_GAMESERVERIDX, &sendMsg, sizeof(sendMsg) );
		}
	}
}
