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
거래 수락 조건 검사
*/
bool
uiTradeMan::TEST_Accept()
{
	TCHAR	szMessage[INTERFACE_STRING_LENGTH];

    if (this->m_tradeInfo1.m_status == TRADE_STATUS_ACCEPT)
    {
		//	이미 거래를 수락하셨습니다.
		g_InterfaceManager.GetInterfaceString( eST_ALREADY_ACCEPT_TRADE, szMessage, INTERFACE_STRING_LENGTH );
        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
        return false;
    }

    if (this->m_tradeInfo1.m_status != TRADE_STATUS_PROPOSAL)
    {
		//	거래 수락은 먼저 제시를 하셔야 합니다.
		g_InterfaceManager.GetInterfaceString( eST_ACCEPT_TRADE, szMessage, INTERFACE_STRING_LENGTH );
        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
        return false;
    }

    if (this->m_tradeInfo2.m_status == TRADE_STATUS_IDLE)
    {
		//	모두 거래 제시해야만 수락하실 수 있습니다.
		g_InterfaceManager.GetInterfaceString( eST_ALL_ACCEPT_TRADE, szMessage, INTERFACE_STRING_LENGTH );
        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
        return false;
    }

    if (this->m_tradeInfo1.m_status == TRADE_STATUS_PROPOSAL)
    {
        this->SEND_Accept();
    }
    else
    {
        TCHAR   szMessage[INTERFACE_STRING_LENGTH];
        
        //	이미 거래를 수락했거나 아직 거래제시를 하지 않았습니다.
        g_InterfaceManager.GetInterfaceString( eST_TRADE_ACCEPT, szMessage, INTERFACE_STRING_LENGTH );
        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
    }

    return true;
}

//------------------------------------------------------------------------------
/**
거래 수락 패킷 전송
*/
void
uiTradeMan::SEND_Accept()
{
    this->m_pUITrade->Lock();

    MSG_CG_TRADE_ACCEPT_SYN sendMsg;
    GlobalFunc::SendPacket( CI_GAMESERVERIDX, &sendMsg, sizeof (MSG_CG_TRADE_ACCEPT_SYN) );
}

//------------------------------------------------------------------------------
/**
*/
void
uiTradeMan::RECV_AcceptNak(DWORD errorCode)
{
    this->m_pUITrade->Unlock();
}

//------------------------------------------------------------------------------
/**
거래 수락 전송 성공
*/
void
uiTradeMan::RECV_Accept1()
{
    this->m_pUITrade->Unlock();

    this->m_tradeInfo1.m_status = TRADE_STATUS_ACCEPT;
}

//------------------------------------------------------------------------------
/**
상대방의 거래 수락
*/
void
uiTradeMan::RECV_Accept2()
{
    this->m_tradeInfo2.m_status = TRADE_STATUS_ACCEPT;
}
