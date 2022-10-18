//------------------------------------------------------------------------------
//  uiTradeMan_proposal.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "uiTradeMan.h"
#include "TargetDialog.h"
#include "ObjectManager.h"
#include "AskReplyTradeDialog.h"
#include "InputNumTradeDialog.h"
#include "MouseHandler.h"
#include "InventoryDialog.h"
#ifdef _NA_008678_20160203_ADD_HEIM_TRADINGVOLUME_NOTIFICATION_OPTION
#else
#include "uiTrade/uiTrade.h"
#endif // _NA_008678_20160203_ADD_HEIM_TRADINGVOLUME_NOTIFICATION_OPTION
#include "SCItemSlot.h"
#include "globalfunc.h"
#include "InterfaceManager.h"
//------------------------------------------------------------------------------
/**
*/
bool
uiTradeMan::TEST_Put(POSTYPE pos)
{
	TCHAR	szMessage[INTERFACE_STRING_LENGTH];

    if (this->m_pUITrade->IsInInventoryItem(pos))
    {
		//	이미 올려져 있는 아이템입니다.
		g_InterfaceManager.GetInterfaceString( eST_ALREADY_INVENTORY_ITEM, szMessage, INTERFACE_STRING_LENGTH );
        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
        MouseHandler::Instance()->RollbackItemAtHand();
        return false;
    }

    if (this->m_tradeInfo1.m_status == TRADE_STATUS_ACCEPT)
    {
		//	거래 수락 후 더이상 수정 하실 수 없습니다.
		g_InterfaceManager.GetInterfaceString( eST_CANNOT_EDIT, szMessage, INTERFACE_STRING_LENGTH );
        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
        MouseHandler::Instance()->RollbackItemAtHand();
        return false;
    }

    if (this->m_tradeInfo1.m_status != TRADE_STATUS_IDLE)
    {
        this->m_bModifyPending = true;
        this->m_uiFunctionIndex = TRADE_MOD_PENDING_PUT;
        this->m_modpendingPut.pos = pos;
        
        this->TEST_Modify();
        return true;
    }
    else
    {
        SEND_Put (pos);
    }

    return true;
}

//------------------------------------------------------------------------------
/**
*/
void
uiTradeMan::SEND_Put(POSTYPE pos)
{
    this->m_pUITrade->Lock();

    MSG_CG_TRADE_PUT_SYN sendMsg;
    sendMsg.m_OrgPos = pos;
    GlobalFunc::SendPacket( CI_GAMESERVERIDX, &sendMsg, sizeof(MSG_CG_TRADE_PUT_SYN) );
}

//------------------------------------------------------------------------------
/**
*/
void
uiTradeMan::RECV_PutNak(DWORD errorCode)
{
    this->m_pUITrade->Unlock();
}

//------------------------------------------------------------------------------
/**
    내가 제시한 창에 올림
*/
void
uiTradeMan::RECV_Put1(POSTYPE tradePos, POSTYPE invenPos)
{
    this->m_pUITrade->Unlock();

    InventoryDialog *pInven = GET_CAST_DIALOG( InventoryDialog, IM_INVENTORY_MANAGER::INVEN_DLG );
        
    this->m_pUITrade->InsertSlot(tradePos, pInven->GetSlot(invenPos));
    MouseHandler::Instance()->RollbackItemAtHand();
}

//------------------------------------------------------------------------------
/**
    상대방이 제시한 창에 올림
*/
void
uiTradeMan::RECV_Put2( POSTYPE tradePos, RENDER_ITEMSTREAMEX itemStream )
{
    SCItemSlot it(itemStream);
    this->m_pUITrade->InsertSlot(tradePos+this->m_pUITrade->GetSlotSize2(), (SCSlot &)it);
}
