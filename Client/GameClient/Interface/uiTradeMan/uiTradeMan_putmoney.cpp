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
#include "GlobalFunc.h"
#include "InterfaceManager.h"
#ifdef _NA_008678_20160203_ADD_HEIM_TRADINGVOLUME_NOTIFICATION_OPTION
#include "uiSystemMan/uiSystemMan.h"
#endif // _NA_008678_20160203_ADD_HEIM_TRADINGVOLUME_NOTIFICATION_OPTION
//------------------------------------------------------------------------------
/**
*/
bool
uiTradeMan::TEST_PutMoney(MONEY money)
{
	TCHAR	szMessage[INTERFACE_STRING_LENGTH];

    if (this->m_tradeInfo1.m_status == TRADE_STATUS_ACCEPT)
    {
		//	거래 수락 후 더이상 수정 하실 수 없습니다.
		g_InterfaceManager.GetInterfaceString( eST_CANNOT_EDIT, szMessage, INTERFACE_STRING_LENGTH );
		GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
        return false;
    }

    if (this->m_tradeInfo1.m_status != TRADE_STATUS_IDLE)
    {
#ifdef _NA_008678_20160203_ADD_HEIM_TRADINGVOLUME_NOTIFICATION_OPTION
		if (GlobalFunc::HeimTradingvolumeCheck(money))
		{
			uiSystemMan* ui_system_manager_ptr = GET_DIALOG_MANAGER(uiSystemMan, UIMAN_SYSTEM);
			if (ui_system_manager_ptr)
			{
				uiTradeMan* ui_trade_manager_ptr = GET_DIALOG_MANAGER(uiTradeMan, UIMAN_TRADE);
				if (ui_trade_manager_ptr == NULL)
				{
					return false;
				}
				DWORD parent_dialog_key = ui_trade_manager_ptr->GetTradeDlgKey();
				if (parent_dialog_key != 0)
				{
					ui_system_manager_ptr->Sys_SystemHeimTradingvolumePersonalTrade(parent_dialog_key, this, money);
				}
			}
			return false;
		}
		else
		{
			this->m_bModifyPending = true;
			this->m_uiFunctionIndex = TRADE_MOD_PENDING_PUTMONEY;
			this->m_modpendingPutMoney.money = money;

			this->TEST_Modify();
			return true;
		}
#else
        this->m_bModifyPending = true;
        this->m_uiFunctionIndex = TRADE_MOD_PENDING_PUTMONEY;
        this->m_modpendingPutMoney.money = money;

        this->TEST_Modify();
        return true;
#endif // _NA_008678_20160203_ADD_HEIM_TRADINGVOLUME_NOTIFICATION_OPTION
    }
    else
    {
#ifdef _NA_008678_20160203_ADD_HEIM_TRADINGVOLUME_NOTIFICATION_OPTION
		if (GlobalFunc::HeimTradingvolumeCheck(money))
		{
			uiSystemMan* ui_system_manager_ptr = GET_DIALOG_MANAGER(uiSystemMan, UIMAN_SYSTEM);
			if (ui_system_manager_ptr)
			{
				uiTradeMan* ui_trade_manager_ptr = GET_DIALOG_MANAGER(uiTradeMan, UIMAN_TRADE);
				if (ui_trade_manager_ptr == NULL)
				{
					return false;
				}
				DWORD parent_dialog_key = ui_trade_manager_ptr->GetTradeDlgKey();
				if (parent_dialog_key != 0)
				{
					ui_system_manager_ptr->Sys_SystemHeimTradingvolumePersonalTrade(parent_dialog_key, this, money);
				}
			}
		}
		else
		{
			SEND_PutMoney (money);
		}
#else
        SEND_PutMoney (money);
#endif // _NA_008678_20160203_ADD_HEIM_TRADINGVOLUME_NOTIFICATION_OPTION
    }

    return true;
}

//------------------------------------------------------------------------------
/**
*/
void
uiTradeMan::SEND_PutMoney(MONEY money)
{
    MSG_CG_TRADE_PUT_MONEY_SYN sendMsg;
    sendMsg.m_money         = money;

    GlobalFunc::SendPacket( CI_GAMESERVERIDX, &sendMsg, sizeof (MSG_CG_TRADE_PUT_MONEY_SYN) );
}

//------------------------------------------------------------------------------
/**
*/
void
uiTradeMan::RECV_PutMoneyNak(DWORD errorCode)
{
}

//------------------------------------------------------------------------------
/**
*/
void
uiTradeMan::RECV_PutMoney1(MONEY money)
{
    this->m_tradeInfo1.m_DealMoney = money;
}

//------------------------------------------------------------------------------
/**
*/
void
uiTradeMan::RECV_PutMoney2(MONEY money)
{
    this->m_tradeInfo2.m_DealMoney = money;
}
