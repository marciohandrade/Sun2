#pragma once
#include "uiBaseMan.h"
#include "uiBankMan/uiBankDef.h"


class BankDialog;
class uiBankMan :
	public uiBaseMan
{
public:
	uiBankMan(InterfaceManager *pUIMan);


public:
    virtual void            OnInitialize();


	void					Send_CG_WAREHOUSE_START_SYN( void );
	void					Send_CG_WAREHOUSE_PWD_SETTING_SYN( BYTE ReqCode, TCHAR* szPassword = NULL);
	void					Send_CG_WAREHOUSE_PWD_LOCKING_SYN( void );
	void					Send_CG_WAREHOUSE_PWD_CHECK_SYN( BYTE ReqCode, TCHAR* szPassword  = NULL);

	void					SetPasswordState( int iPasswordDlgState )		{m_iPasswordDlgState = iPasswordDlgState;}
	int						GetPasswordState( void )						{return m_iPasswordDlgState;}

	void					SetBankLockState(int iBankLockState)			{m_iBankLockState = iBankLockState;}
	int						GetBankLockState()								{return m_iBankLockState;}

	void					ClearPassword( void );

	void					SetPasswordStr(TCHAR* strPassword);
	TCHAR*					GetPAsswordStr( void )							{return m_strPassword;}

	void					ClearWaitPacket( void )							{m_bWaitPacket = FALSE;}


private:
	BankDialog *			m_pBankDialog;

	TCHAR					m_strPassword[PASSWD_MAX_LEN + 1];
	int						m_iBankLockState;
	BOOL					m_bWaitPacket;

	int						m_iPasswordDlgState;

};
