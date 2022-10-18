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
uiTradeMan::TEST_Get(POSTYPE pos)
{
	TCHAR	szMessage[INTERFACE_STRING_LENGTH];

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
        this->m_uiFunctionIndex = TRADE_MOD_PENDING_GET;
        this->m_modpendingGet.pos = pos;

        this->TEST_Modify();
        return true;
    }
    else
    {
        SEND_Get (pos);
    }

    return true;
}

//------------------------------------------------------------------------------
/**
*/
void
uiTradeMan::SEND_Get(POSTYPE pos)
{
    this->m_pUITrade->Lock();

    MSG_CG_TRADE_GET_SYN sendMsg;
    sendMsg.m_TradePos = pos;
    GlobalFunc::SendPacket( CI_GAMESERVERIDX, &sendMsg, sizeof (MSG_CG_TRADE_GET_SYN) );
}

//------------------------------------------------------------------------------
/**
*/
void
uiTradeMan::RECV_GetNak(DWORD errorCode)
{
    this->m_pUITrade->Unlock();
}

//------------------------------------------------------------------------------
/**
*/
void
uiTradeMan::RECV_Get1(POSTYPE pos)
{
    this->m_pUITrade->Unlock();

    MouseHandler::Instance()->RollbackItemAtHand();
    this->m_pUITrade->DeleteSlot(pos, NULL);
}

//------------------------------------------------------------------------------
/**
*/
void
uiTradeMan::RECV_Get2(POSTYPE pos)
{
    this->m_pUITrade->DeleteSlot(pos+this->m_pUITrade->GetSlotSize2(), NULL);
}
