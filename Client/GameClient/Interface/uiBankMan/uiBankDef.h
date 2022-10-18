#pragma once

struct IM_BANK_MANAGER 
{
	static const DWORD BANK_DIALOG;
};

enum eBankDlgState
{
	eBankDlgState_None = 0,
	eBankDlgState_Setting_SSN,
	eBankDlgState_Setting_PasswordCmp,
	eBankDlgState_Setting_PasswordSend,
	eBankDlgState_Check_Unlock,
	eBankDlgState_Check_Clear,
	eBankDlgState_Lock,

};

