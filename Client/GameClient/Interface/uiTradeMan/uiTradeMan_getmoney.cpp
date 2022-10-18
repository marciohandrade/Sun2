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
//------------------------------------------------------------------------------
/**
*/
bool
uiTradeMan::TEST_GetMoney(MONEY money)
{
    return true;
}

//------------------------------------------------------------------------------
/**
*/
void
uiTradeMan::SEND_GetMoney(MONEY money)
{
    MSG_CG_TRADE_GET_MONEY_SYN sendMsg;
    sendMsg.m_money         = money;

    GlobalFunc::SendPacket( CI_GAMESERVERIDX, &sendMsg, sizeof (MSG_CG_TRADE_GET_MONEY_SYN) );
}

//------------------------------------------------------------------------------
/**
*/
void
uiTradeMan::RECV_GetMoneyNak(DWORD errorCode)
{
}

//------------------------------------------------------------------------------
/**
*/
void
uiTradeMan::RECV_GetMoney1(MONEY money)
{
    this->m_tradeInfo1.m_DealMoney = money;
}

//------------------------------------------------------------------------------
/**
*/
void
uiTradeMan::RECV_GetMoney2(MONEY money)
{
    this->m_tradeInfo2.m_DealMoney = money;
}
