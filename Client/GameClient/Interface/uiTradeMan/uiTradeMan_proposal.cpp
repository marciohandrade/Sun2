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
uiTradeMan::TEST_Proposal()
{
    if (this->m_tradeInfo1.m_status == TRADE_STATUS_PROPOSAL)
    {
		this->TEST_Modify();
		return true;
    }

    if (this->m_tradeInfo1.m_status == TRADE_STATUS_IDLE)
    {
        this->SEND_Proposal();
    }
    else
    {
        TCHAR   szMessage[INTERFACE_STRING_LENGTH];
        //	�̹� �ŷ��� �����߰ų� ������ �����Դϴ�. �������� �ø��ų� �����ø� �ڵ� �������� ��ȯ�˴ϴ�.
        g_InterfaceManager.GetInterfaceString( eST_TRADE_PROPOSAL, szMessage, INTERFACE_STRING_LENGTH );
        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
    }

    return true;
}

//------------------------------------------------------------------------------
/**
�ŷ� ���� ��Ŷ ����
*/
void
uiTradeMan::SEND_Proposal()
{
    this->m_pUITrade->Lock();

    MSG_CG_TRADE_PROPOSAL_SYN sendMsg;
    GlobalFunc::SendPacket( CI_GAMESERVERIDX, &sendMsg, sizeof (MSG_CG_TRADE_PROPOSAL_SYN) );
}

//------------------------------------------------------------------------------
/**
*/
void
uiTradeMan::RECV_ProposalNak(DWORD errorCode)
{
    this->m_pUITrade->Unlock();
}

//------------------------------------------------------------------------------
/**
�ŷ� ���� ��Ŷ ���� ����
*/
void
uiTradeMan::RECV_Proposal1()
{
    this->m_pUITrade->Unlock();

    this->m_tradeInfo1.m_status = TRADE_STATUS_PROPOSAL;
}

//------------------------------------------------------------------------------
/**
������ �ŷ� ����
*/
void
uiTradeMan::RECV_Proposal2()
{
    this->m_tradeInfo2.m_status = TRADE_STATUS_PROPOSAL;
}
