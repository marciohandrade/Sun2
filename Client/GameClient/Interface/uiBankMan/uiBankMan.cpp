#include "SunClientPrecompiledHeader.h"
#include "uibankman.h"
#include "interfacemanager.h"
#ifdef _NA_003923_20120130_GUILD_RENEWAL_MOD_WAREHOUSE_SLOT_NUM
    #include "uiBankMan/uiBankDialog.h"
#else
    #include "BankDialog.h"
#endif //_NA_003923_20120130_GUILD_RENEWAL_MOD_WAREHOUSE_SLOT_NUM
#include "globalfunc.h"
#include "HeroActionInput.h"

#ifdef _NA_003923_20120130_GUILD_RENEWAL_MOD_WAREHOUSE_SLOT_NUM
    const DWORD IM_BANK_MANAGER::BANK_DIALOG = g_IDGenerator.GenerateID();
#else
    const DWORD IM_BANK_MANAGER::BANK_DIALOG = StrToWzID("0600");
#endif //_NA_003923_20120130_GUILD_RENEWAL_MOD_WAREHOUSE_SLOT_NUM


uiBankMan::uiBankMan(InterfaceManager *pUIMan):
uiBaseMan(pUIMan)
{
    m_pBankDialog = NULL;

    ZeroMemory(m_strPassword, sizeof(m_strPassword));
    m_bWaitPacket	= FALSE;

}

void uiBankMan::OnInitialize()
{
#ifdef _NA_003923_20120130_GUILD_RENEWAL_MOD_WAREHOUSE_SLOT_NUM
    CreateUIDialog<uiBankDialog>(
        IM_BANK_MANAGER::BANK_DIALOG, 
        "Data\\Interface\\73_8_Guild_ware_house.iwz", 
        this, TRUE, TRUE);
#else
    m_pBankDialog = CreateUIDialog<BankDialog>(
        IM_BANK_MANAGER::BANK_DIALOG, 
        "Data\\Interface\\60_Warehouse.iwz", 
        this, TRUE);
#endif //_NA_003923_20120130_GUILD_RENEWAL_MOD_WAREHOUSE_SLOT_NUM

    SetPasswordState(eBankDlgState_None);
}

//------------------------------------------------------------------------------
/**
*/

void uiBankMan::Send_CG_WAREHOUSE_START_SYN( void )
{
	if(m_bWaitPacket)
		return;
	MSG_CG_WAREHOUSE_START_SYN			SendPacket;
	SendPacket.m_NPCCode = g_HeroInput.GetClickedNpcCode();
	GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket));
	m_bWaitPacket	= TRUE;
}

//------------------------------------------------------------------------------
/**
*/
void uiBankMan::Send_CG_WAREHOUSE_PWD_LOCKING_SYN( void )
{
	if(m_bWaitPacket)
		return;
	MSG_CG_WAREHOUSE_PWD_LOCKING_SYN		SendPacket;
	GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket));
	m_bWaitPacket	= TRUE;
}

//------------------------------------------------------------------------------
/**
*/
void uiBankMan::Send_CG_WAREHOUSE_PWD_CHECK_SYN( BYTE ReqCode, TCHAR* szPassword )
{
	if(m_bWaitPacket)
		return;
	MSG_CG_WAREHOUSE_PWD_CHECK_SYN			SendPacket;
	SendPacket.m_ReqCode = ReqCode;
	if(ReqCode == MSG_CG_WAREHOUSE_PWD_CHECK_SYN::SYNC_WITH_PWD)
		SendPacket.SetPassword( szPassword );
	else if(ReqCode == MSG_CG_WAREHOUSE_PWD_SETTING_SYN::SYNC_WITH_SSN)
		SendPacket.SetSSN( szPassword );
	GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendPacket, SendPacket.GetSize());
	m_bWaitPacket	= TRUE;
}

//------------------------------------------------------------------------------
/**
*/
void uiBankMan::Send_CG_WAREHOUSE_PWD_SETTING_SYN( BYTE ReqCode, TCHAR* szPassword )
{
	if(m_bWaitPacket)
		return;
	MSG_CG_WAREHOUSE_PWD_SETTING_SYN		SendPacket;
	SendPacket.m_ReqCode = ReqCode;
	if(ReqCode == MSG_CG_WAREHOUSE_PWD_SETTING_SYN::SYNC_WITH_PWD)
		SendPacket.SetPassword( szPassword );
	else if(ReqCode == MSG_CG_WAREHOUSE_PWD_SETTING_SYN::SYNC_WITH_SSN)
		SendPacket.SetSSN( szPassword );
	GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendPacket, SendPacket.GetSize());
	m_bWaitPacket	= TRUE;
}
//------------------------------------------------------------------------------
/**
*/
void uiBankMan::ClearPassword( void )
{
	SetPasswordState(eBankDlgState_None);
	ZeroMemory(m_strPassword, sizeof(m_strPassword));
}

//------------------------------------------------------------------------------
/**
*/
void uiBankMan::SetPasswordStr(TCHAR* strPassword)
{
	Snprintf(m_strPassword, MAX_WAREHOUSE_INVENTORY_PWD_LENGTH, _T("%s"), strPassword);
}

