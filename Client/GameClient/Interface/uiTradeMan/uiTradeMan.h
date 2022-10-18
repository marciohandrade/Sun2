#ifndef     __USER_INTERFACE_MANAGER_TRADE_H__
#define     __USER_INTERFACE_MANAGER_TRADE_H__
//------------------------------------------------------------------------------
/**
    @class uiTradeMan

    interface manager for a trade

    (C) 2005 ckbang
*/
#include "uiBaseMan.h"
#include "uiTradeMan_def.h"
#ifdef _NA_008678_20160203_ADD_HEIM_TRADINGVOLUME_NOTIFICATION_OPTION
#include "uiTrade/uiTrade.h"
#else

class uiTrade;
#endif // _NA_008678_20160203_ADD_HEIM_TRADINGVOLUME_NOTIFICATION_OPTION
#ifdef	_K2P_SOFTWARE_NUMBER_KEYBOARD_000913
class uiInPutNumTradeUIDialog;
#endif

class uiTradeMan :
    public uiBaseMan
{
public:
    uiTradeMan(InterfaceManager *pUIMan);

    //--------------------------------------------------------------------------
    // init/release
    //
public:
	virtual void			OnInitialize( void );

private:
    BOOL                    _LoadUI();

    //--------------------------------------------------------------------------
    // functions
    //
public:
    void                    Clear();    // �ŷ��� ������ �ʱ�ȭ

    bool                    Ask(const TCHAR* pname);    // �ŷ� ���� �˻� �� ��û
    bool                    PopupTradeReply(DWORD dwPlayerKey); // �ŷ� ��û ������ ����â �˾�
    void                    SendTradeReply(bool bDo);   // ���� ������
    void                    PopupTradeMoney();          // �� �Է�â �˾�

    // �ŷ� ���� / ��
    void                    TradeStart();
    void                    TradeEnd(MONEY money, TRADE_TOTAL_INFO tradeInfo);
    void                    TradeClose();

    // �ŷ� ���
    void                    SendTradeCancel();

    //
    // �Լ��� �ڿ� �ٴ� ������ �ǹ�
    // 1. �ŷ� ����
    // 2. �ŷ� ����
    //

    // �ŷ� ����
    // uiTradeMan_proposal.cpp
    bool                    TEST_Proposal();      // ���� �˻�
    void                    SEND_Proposal();
    void                    RECV_ProposalNak(DWORD errorCode);
    void                    RECV_Proposal1();
    void                    RECV_Proposal2();

    // �ŷ� ����
    // uiTradeMan_accept.cpp
    bool                    TEST_Accept();    // ���� �˻�
    void                    SEND_Accept();
    void                    RECV_AcceptNak(DWORD errorCode);
    void                    RECV_Accept1();
    void                    RECV_Accept2();

    // �ŷ�â���� �� �ø���
    // uiTradeMan_putmoney.cpp
    bool                    TEST_PutMoney(MONEY money);
    void                    SEND_PutMoney(MONEY money);
    void                    RECV_PutMoneyNak(DWORD errorCode);
    void                    RECV_PutMoney1(MONEY money);
    void                    RECV_PutMoney2(MONEY money);

    // �ŷ�â���� �� ������
    // uiTradeMan_getmoney.cpp
    bool                    TEST_GetMoney(MONEY money);
    void                    SEND_GetMoney(MONEY money);
    void                    RECV_GetMoneyNak(DWORD errorCode);
    void                    RECV_GetMoney1(MONEY money);
    void                    RECV_GetMoney2(MONEY money);

    // �ŷ� ����
    // uiTradeMan_modify.cpp
    bool                    TEST_Modify();
    void                    SEND_Modify();
    void                    RECV_ModifyNak(DWORD errorCode);
    void                    RECV_Modify1();
    void                    RECV_Modify2();

    // �ŷ�â���� ������ �ø���
    // uiTradeMan_put.cpp
    bool                    TEST_Put(POSTYPE pos);
    void                    SEND_Put(POSTYPE pos);
    void                    RECV_PutNak(DWORD errorCode);
    void                    RECV_Put1(POSTYPE tradePos, POSTYPE invenPos);
	void					RECV_Put2( POSTYPE tradePos, RENDER_ITEMSTREAMEX itemStream );


    // �ŷ�â���� ������ ������
    // uiTradeMan_get.cpp
    bool                    TEST_Get(POSTYPE pos);
    void                    SEND_Get(POSTYPE pos);
    void                    RECV_GetNak(DWORD errorCode);
    void                    RECV_Get1(POSTYPE tradePos);
    void                    RECV_Get2(POSTYPE tradePos);

    // �ŷ��ϱ� ���� �ʿ���� ���� �˻�
    bool                    CheckCanTrading(Player* pPlayer);
    bool                    CheckCanTradingIsBusy(Player* pPlayer);
    bool                    CheckCanTradingIsInRanged(Player* pPlayer);
    bool                    CheckEndTradingCauseRanged();

    // �ŷ��� ����
    const uicTradeInfo&     GetTradeInfo1();    // �ŷ� ����
    const uicTradeInfo&     GetTradeInfo2();    // �ŷ� ����

    void                    SetPutOneStarting();

	void					ShowNumTradeUIDialog( BOOL show );

	DWORD					GetTradeModifyDlgKey(); //��ǰ��ȯ ���� �ý��� ����â�� DlgKey

#ifdef _NA_008678_20160203_ADD_HEIM_TRADINGVOLUME_NOTIFICATION_OPTION
	DWORD                   GetTradeDlgKey();
	void                    TEST_Modify_Mode(const bool& bModifyPending, const UINT& uiFunctionIndex, const MONEY& money);
#endif // _NA_008678_20160203_ADD_HEIM_TRADINGVOLUME_NOTIFICATION_OPTION

private:
    bool                    SetName(DWORD dwKey);   // ���� ����
    bool                    SetNameTo(DWORD dwKey); // ���� ����
	void					ErrorCode(DWORD dwErrorCode);

    //--------------------------------------------------------------------------
    // procedure
    //
public:
    virtual void	        NetworkProc( MSG_BASE * pMsg );         // Pure

    //--------------------------------------------------------------------------
    // member vars
    //
private:
    uiTrade*                m_pUITrade;             // Trade User Interface
#ifdef	_K2P_SOFTWARE_NUMBER_KEYBOARD_000913
	uiInPutNumTradeUIDialog * m_pUINumTradeUIDialog;
#endif

    uicTradeInfo            m_tradeInfo1;           // �ŷ� ���� ����
    uicTradeInfo            m_tradeInfo2;           // �ŷ� ���� ����

    bool                    m_bModifyPending;
    UINT                    m_uiFunctionIndex;

    modPending_Put_t        m_modpendingPut;
    modPending_Get_t        m_modpendingGet;
    modPending_PutMoney_t   m_modpendingPutMoney;

    bool                    m_bPutOneStaring;

	DWORD					m_dwTradeModifyDlgKey;
};

//////////////////////////////////////////////////////////////////////////
//
inline
DWORD
uiTradeMan::GetTradeModifyDlgKey()
{
	return m_dwTradeModifyDlgKey;
}

#ifdef _NA_008678_20160203_ADD_HEIM_TRADINGVOLUME_NOTIFICATION_OPTION
inline
DWORD 
uiTradeMan::GetTradeDlgKey()
{
	if (m_pUITrade)
	{
		return m_pUITrade->GetDialogKey();
	}

	return 0;
}
#endif // _NA_008678_20160203_ADD_HEIM_TRADINGVOLUME_NOTIFICATION_OPTION

//------------------------------------------------------------------------------
/**
    �ŷ� ���� ���� ���
*/
inline
const uicTradeInfo&
uiTradeMan::GetTradeInfo1()
{
    return m_tradeInfo1;
}

//------------------------------------------------------------------------------
/**
    �ŷ� ���� ���� ���
*/
inline
const uicTradeInfo&
uiTradeMan::GetTradeInfo2()
{
    return m_tradeInfo2;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
uiTradeMan::SetPutOneStarting()
{
    this->m_bPutOneStaring = true;
}

//------------------------------------------------------------------------------
#endif  //  __USER_INTERFACE_MANAGER_TRADE_H__
