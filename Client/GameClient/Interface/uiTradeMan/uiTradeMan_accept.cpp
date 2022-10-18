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
�ŷ� ���� ���� �˻�
*/
bool
uiTradeMan::TEST_Accept()
{
	TCHAR	szMessage[INTERFACE_STRING_LENGTH];

    if (this->m_tradeInfo1.m_status == TRADE_STATUS_ACCEPT)
    {
		//	�̹� �ŷ��� �����ϼ̽��ϴ�.
		g_InterfaceManager.GetInterfaceString( eST_ALREADY_ACCEPT_TRADE, szMessage, INTERFACE_STRING_LENGTH );
        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
        return false;
    }

    if (this->m_tradeInfo1.m_status != TRADE_STATUS_PROPOSAL)
    {
		//	�ŷ� ������ ���� ���ø� �ϼž� �մϴ�.
		g_InterfaceManager.GetInterfaceString( eST_ACCEPT_TRADE, szMessage, INTERFACE_STRING_LENGTH );
        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
        return false;
    }

    if (this->m_tradeInfo2.m_status == TRADE_STATUS_IDLE)
    {
		//	��� �ŷ� �����ؾ߸� �����Ͻ� �� �ֽ��ϴ�.
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
        
        //	�̹� �ŷ��� �����߰ų� ���� �ŷ����ø� ���� �ʾҽ��ϴ�.
        g_InterfaceManager.GetInterfaceString( eST_TRADE_ACCEPT, szMessage, INTERFACE_STRING_LENGTH );
        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
    }

    return true;
}

//------------------------------------------------------------------------------
/**
�ŷ� ���� ��Ŷ ����
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
�ŷ� ���� ���� ����
*/
void
uiTradeMan::RECV_Accept1()
{
    this->m_pUITrade->Unlock();

    this->m_tradeInfo1.m_status = TRADE_STATUS_ACCEPT;
}

//------------------------------------------------------------------------------
/**
������ �ŷ� ����
*/
void
uiTradeMan::RECV_Accept2()
{
    this->m_tradeInfo2.m_status = TRADE_STATUS_ACCEPT;
}
