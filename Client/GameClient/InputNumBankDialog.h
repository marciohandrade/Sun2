#pragma once
#ifdef _NA_003923_20120130_GUILD_RENEWAL_MOD_WAREHOUSE_SLOT_NUM

#else

#include "inputnumdialog.h"

class BankDialog;
class uiGuildWareHouse;

class InputNumBankDialog :
	public InputNumDialog
{
public:

	enum ePacketStatus
	{
		NONE					 = 0,
		WAREHOUSEMONEY_SEND		 = 1,
		GUILDWAREHOUSEMONEY_SEND = 2,
	};

	InputNumBankDialog(void);
	virtual ~InputNumBankDialog(void);

	virtual VOID    Init( CDrawBase * pDrawBase );
    virtual VOID    Release();

public:
    virtual VOID    MessageProc( SI_MESSAGE * pMessage );

public:
	void			SetWareHouseType(eWAREHOUSE_TYPE eType){m_eWareHouseType = eType;}
	VOID			SendWareHouseMoney(MONEY money);
	void            Exit();
	VOID			ClearPacketFlag();

protected:
    virtual VOID    OnShowWindow( BOOL val );

    void            MoneyProcess();
    bool            checkDigit(const TCHAR* str);

	MSG_CG_WAREHOUSE_MONEY_SYN			m_WarehouseMoneySyn;
	MSG_CG_GUILD_WAREHOUSE_MONEY_SYN	m_GuildWareHouseMoneySyn;

	MONEY				m_CurMoney;

	eWAREHOUSE_TYPE		m_eWareHouseType;
	DWORD				m_dwPacketStatus;
};

#endif //_NA_003923_20120130_GUILD_RENEWAL_MOD_WAREHOUSE_SLOT_NUM
