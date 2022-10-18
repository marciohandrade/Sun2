#include "SunClientPrecompiledHeader.h"

#ifdef __NA_001027_20080423_RANDOM_REWARD_EVENT_
#include "uiGuildEventDialog.h"
#include "InterfaceManager.h"
#include "uiSystemMan/uiSystemMan.h"
#include "GlobalFunc.h"

#include "ExchangeRewardInfo.h"
#include "GlobalFunc.h"
#include "ItemInfoParser.h"
#include "GameConst_Color.h"
#include "uiEventMan/uiEventMan_def.h"


void _fn_GuildEventReward(bool bYes )
{
	if( bYes )
	{
		uiGuildEventDialog * pGuildEvnetDialog = GET_CAST_DIALOG( uiGuildEventDialog, IM_EVENT_MANAGER::GUILD_EVENT );
		
		if( pGuildEvnetDialog )
		{
			pGuildEvnetDialog->NET_SEND_CG_ITEM_EXCHANGE_REWARD_ITEM_SYN();
		}
	}
	else
	{
		TCHAR szMessage[INTERFACE_STRING_LENGTH];
		// 아이템 보상을 취소 하였습니다
		g_InterfaceManager.GetInterfaceString( 5380, szMessage );
		GlobalFunc::OutputMsg_Notice( eOUTPUT_MSG_CHAT , szMessage );
	}
}

WzID uiGuildEventDialog::m_wzID[DLGPOS_MAX] = 
{
	StrToWzID("BT00"),
	StrToWzID("B999"),
	StrToWzID("LS00"),
	StrToWzID("VS00"),
	StrToWzID("LS01"),

	StrToWzID("ST00"),
	StrToWzID("ST01"),

};
//------------------------------------------------------------------------------
/**
*/
uiGuildEventDialog::uiGuildEventDialog(InterfaceManager * pUIMan) : uiBase(pUIMan)
{

}
//------------------------------------------------------------------------------
/**
*/
uiGuildEventDialog::~uiGuildEventDialog()
{

}
//------------------------------------------------------------------------------
/**
*/
void uiGuildEventDialog::Init(CDrawBase* pDrawBase)
{
    uiBase::Init(pDrawBase);

	m_pButton =(CCtrlButtonWZ *) getControl(DLGPOS_BTN_OK);
	assert( m_pButton );

	m_pList = (CCtrlListWZ *) getControl(DLGPOS_LIST);
	m_pVScroll = ( CCtrlVScrollWZ *) getControl(DLGPOS_VSCROLL);

	CCtrlListWZ	* pTextList = (CCtrlListWZ *) getControl( DLGPOS_TEXT_LIST );

	if( pTextList )
	{
		TCHAR szMessage[INTERFACE_STRING_LENGTH] = { '\0', };

		g_InterfaceManager.GetInterfaceString( 200236, szMessage );

		GlobalFunc::AddDescribeString( pTextList, szMessage, StrToWzID("mn12"), DT_LEFT | DT_VCENTER,
  								   RGBA(255,255,255,0), 
								   RGBA(255,255,255,255), 
								   RGBA(255,255,255,255) );
	}

	m_ClientExchangInfo.itemCode = 0;
	m_ClientExchangInfo.itemNum = 0;

	m_iCurScrollValue = 0;
	m_iMaxScrollValue = 0;
	m_bShow = FALSE;
	RegisterItemList();

}
//------------------------------------------------------------------------------
/**
*/
void uiGuildEventDialog::Release()
{

}
//------------------------------------------------------------------------------
/**
*/
void uiGuildEventDialog::OnUpdateSolarDialog()
{
	if( ! m_bShow )
	{
		ShowInterface(FALSE);
	}
}
//------------------------------------------------------------------------------
/**
*/
void uiGuildEventDialog::OnRenderSolarDialog()
{
	if( ! m_bShow )
	{
		ShowInterface(FALSE);
	}
}
//------------------------------------------------------------------------------
/**
*/
void uiGuildEventDialog::Process(DWORD dwTick)
{

}
//------------------------------------------------------------------------------
/**
*/
void uiGuildEventDialog::OnShowWindow(BOOL val)
{
	if( val )
	{
		MoveCenter();
		keyMsg msg;
		ZeroMemory(&msg,sizeof(msg));
		msg.dwType = eDoType_dialog_GuildEventWindow;
		msg.wParam = InterfaceManager::DIALOG_GUILD_EVENT_WINDOW;
		msg.DoSomething = GlobalFunc::DoShowWindow;
		g_KeyQueueManager.PushBack(msg);
		m_bShow = TRUE;
	}
	else
	{
		g_KeyQueueManager.DeleteMsg(eDoType_dialog_GuildEventWindow);
		m_bShow = FALSE;
	}
}
//------------------------------------------------------------------------------
/**
*/
void uiGuildEventDialog::MessageProc(SI_MESSAGE * pMessage)
{
	switch ( pMessage->eResultMsg )
	{
	case RT_MSG_LBUTTONCLICK:
		{
			switch( getControlIDToPos( pMessage->dwCtrlID) )
			{
			case DLGPOS_BTN_OK:
				{
					uiSystemMan * pSystemMan = ( uiSystemMan *) g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
					if( pSystemMan )
					{
						if(  m_pList->GetCurSel() == -1)
							return;
						EXCHANGE_REWARDINFO * pData = (EXCHANGE_REWARDINFO *)m_pList->GetItemData(m_pList->GetCurSel());
						if( ! pData )
							return;
						
						m_ClientExchangInfo.itemCode = pData->m_ItemCode;
						m_ClientExchangInfo.itemNum = pData->m_ItemCount;
						TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,} , szItemName[INTERFACE_STRING_LENGTH] = {0,}, string[INTERFACE_STRING_LENGTH];

						g_InterfaceManager.GetItemName(m_ClientExchangInfo.itemCode , szItemName , INTERFACE_STRING_LENGTH);
						
						// %s %d 개를 보상하시겠습니까?
						g_InterfaceManager.GetInterfaceString( 5381, string );
						Snprintf( szMessage , INTERFACE_STRING_LENGTH -1 , string, szItemName , m_ClientExchangInfo.itemNum);
						
						pSystemMan->Sys_ConfirmLock(szMessage ,_fn_GuildEventReward );
					}
				}
				break;
			case DLGPOS_BTN_EXIT:
				{
					m_bShow = FALSE;
				}
				break;
			}
		}
		break;
	case RT_MSG_SB_LINEBOTTOM:
		{
			switch( getControlIDToPos( pMessage->dwCtrlID ) )
			{
			case DLGPOS_VSCROLL:
				{
					if( m_iCurScrollValue < m_iMaxScrollValue) 
					{
						m_iCurScrollValue++;
						m_pVScroll->SetScrollPosWZ(m_iCurScrollValue);
						m_pList->SetBeginDrawIndex(m_iCurScrollValue);
					}
					
				}
				break;
			}
		}
		break;

	case RT_MSG_SB_LINETOP:
		{
			switch( getControlIDToPos( pMessage->dwCtrlID ) )
			{
			case DLGPOS_VSCROLL:
				{
					if( m_iCurScrollValue > 0 )
					{
						m_iCurScrollValue--;
						m_pVScroll->SetScrollPosWZ(m_iCurScrollValue);
						m_pList->SetBeginDrawIndex(m_iCurScrollValue);
					}
				}
				break;
			}
		}
		break;

	case RT_MSG_SB_THUMBPOSITION:
		{
			switch( getControlIDToPos( pMessage->dwCtrlID ) )
			{
			case DLGPOS_VSCROLL:
				{
					m_iCurScrollValue = m_pVScroll->GetScrollPosWZ();
					m_pList->SetBeginDrawIndex(m_iCurScrollValue);
				}
				break;
			}
		}
		break;
	}
}
//------------------------------------------------------------------------------
/**
*/
void uiGuildEventDialog::NetworkProc(MSG_BASE * pMsg)
{

}

void uiGuildEventDialog::RegisterItemList()
{
	m_pList->DeleteItemAll();

	m_pList->SetSellHeight(20);
	m_pList->SetUseReport(true);
	m_pList->SetReportUseCnt(2);
	m_pList->SetHideReportRender(TRUE);

	CControlWZ * pButton0 = getControl( DLGPOS_TEXT_BUTTON0 );
	CControlWZ * pButton1 = getControl( DLGPOS_TEXT_BUTTON1 );

	RECT rc0 = pButton0->GetSize();
	RECT rc1 = pButton1->GetSize();

	// 아이템명
	GlobalFunc::SetReportButtonInfo( m_pList, 0, rc0.right, g_InterfaceManager.GetInterfaceString( 1204 ) );
	// 갯수
	GlobalFunc::SetReportButtonInfo( m_pList, 1, rc1.right, g_InterfaceManager.GetInterfaceString( 1205 ) );

	EXCHANGE_REWARDINFO_HASH * pHash = ExchangeRewardInfoParser::Instance()->GetHashTable();
	if( ! pHash )
		return;

	EXCHANGE_REWARDINFO_HASH::iterator itr;

	for ( itr = pHash->begin() ; itr != pHash->end() ; ++itr)
	{
		EXCHANGE_REWARDINFO * pData = *itr;
		if( ! pData )
			continue;
		BASE_ITEMINFO * pItemInfo = ItemInfoParser::Instance()->GetItemInfo( pData->m_ItemCode );
		if( pItemInfo )
		{
			int index = m_pList->InsertItemBack();
			TCHAR szItemName[INTERFACE_STRING_LENGTH] = {0,};
			g_InterfaceManager.GetInterfaceString( pItemInfo->m_NCode , szItemName , INTERFACE_STRING_LENGTH );
			GlobalFunc::SetItem(m_pList , index , 0 ,szItemName ,StrToWzID("mn12") ,DT_VCENTER |  DT_CENTER ,
				c_InterfaceBKColor , c_InterfaceTextColor , c_InterfaceSelColor);

			TCHAR szItemNum[INTERFACE_STRING_LENGTH] = {0,};
			Snprintf(szItemNum , INTERFACE_STRING_LENGTH -1 , _T("%d") , pData->m_ItemCount);
			GlobalFunc::SetItem(m_pList , index , 1 ,szItemNum ,StrToWzID("mn12") ,DT_VCENTER |  DT_CENTER ,
				c_InterfaceBKColor , c_InterfaceTextColor , c_InterfaceSelColor);

			m_pList->SetItemData( index , pData);
		}
	}

	m_iMaxScrollValue = max( 0 , m_pList->GetListRowCnt() - MAX_VIEW_LIST_ROW_COUNT);
	m_pVScroll->SetScrollRangeWZ(0, m_iMaxScrollValue);
	m_pVScroll->SetScrollPosWZ(0);
}

void uiGuildEventDialog::NET_SEND_CG_ITEM_EXCHANGE_REWARD_ITEM_SYN()
{
	MSG_CG_ITEM_EXCHANGE_REWARD_ITEM_SYN SendPacket;
	SendPacket.m_ItemCode = m_ClientExchangInfo.itemCode;
	SendPacket.m_Count = m_ClientExchangInfo.itemNum;
	GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket));
}
#endif
