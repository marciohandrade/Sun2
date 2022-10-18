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
uiTradeMan::TEST_Modify()
{
    if (this->m_tradeInfo1.m_status == TRADE_STATUS_PROPOSAL)
    {
		this->SEND_Modify();
        return true;
	}

    return false;
}

//------------------------------------------------------------------------------
/**
*/
void
uiTradeMan::SEND_Modify()
{
    MSG_CG_TRADE_MODIFY_SYN sendMsg;
    GlobalFunc::SendPacket( CI_GAMESERVERIDX, &sendMsg, sizeof(MSG_CG_TRADE_MODIFY_SYN) );

	TCHAR szMessage[INTERFACE_STRING_LENGTH + 1] = {0,};
	//6343	���濡�� ��ǰ������ ��û�ϰ� �ֽ��ϴ�. ��ø� ��ٷ��ֽʽÿ�.
	g_InterfaceManager.GetInterfaceString(6343, szMessage);
	GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );

	//6347	������ ��ǰ���� ��û�� �����ϱ��������� �ŷ��� ����Ͻ� �� �����ϴ�.
	g_InterfaceManager.GetInterfaceString(6347, szMessage);
	GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_TOP_SHOW, szMessage);

    this->m_pUITrade->Lock();
}

//------------------------------------------------------------------------------
/**
*/
void
uiTradeMan::RECV_ModifyNak(DWORD errorCode)
{
    this->m_pUITrade->Unlock();
}

//------------------------------------------------------------------------------
/**
*/
void
uiTradeMan::RECV_Modify1()
{
    this->m_pUITrade->Unlock();

    this->m_tradeInfo1.m_status = TRADE_STATUS_IDLE;

    if (this->m_bModifyPending)
    {
        switch (this->m_uiFunctionIndex)
        {
        case TRADE_MOD_PENDING_UNKNOWN:
            {
                assert ("unknown pending in trade");
            }
            break;

        case TRADE_MOD_PENDING_GET:
            {
                this->SEND_Get(this->m_modpendingGet.pos);
            }
            break;

        case TRADE_MOD_PENDING_PUT:
            {
                this->SEND_Put(this->m_modpendingPut.pos);
            }
            break;

        case TRADE_MOD_PENDING_PUTMONEY:
            {
                this->SEND_PutMoney(this->m_modpendingPutMoney.money);
            }
            break;
        }

        this->m_bModifyPending = false;
    }
}

//------------------------------------------------------------------------------
/**
*/
void
uiTradeMan::RECV_Modify2()
{
	TCHAR	szMessage[INTERFACE_STRING_LENGTH];

    this->m_tradeInfo2.m_status = TRADE_STATUS_IDLE;

    // ������ �ŷ� �������� �����ڰ� �����ϸ� ������ ����� �ŷ� ���÷� �ٲ�
    if (this->m_tradeInfo1.m_status == TRADE_STATUS_ACCEPT)
    {
		//	�ŷ��� �����մϴ�.
		g_InterfaceManager.GetInterfaceString( eST_CHANGE_TRADE, szMessage, INTERFACE_STRING_LENGTH );
        this->m_tradeInfo1.m_status = TRADE_STATUS_PROPOSAL;
        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
    }
}

//------------------------------------------------------------------------------
/**
*/
#ifdef _NA_008678_20160203_ADD_HEIM_TRADINGVOLUME_NOTIFICATION_OPTION
void 
uiTradeMan::TEST_Modify_Mode( const bool& bModifyPending, 
							  const UINT& uiFunctionIndex, 
							  const MONEY& money )
{
	this->m_bModifyPending           = bModifyPending;
	this->m_uiFunctionIndex          = uiFunctionIndex;
	this->m_modpendingPutMoney.money = money;
}
#endif // _NA_008678_20160203_ADD_HEIM_TRADINGVOLUME_NOTIFICATION_OPTION
