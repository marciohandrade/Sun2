#pragma once
#include "uibaseman.h"

class MainDialog;
class MainMenuOptionDlg;

class SystemDialog;

class CreditDialog;

#ifdef _NA_000000_20130114_RENEWER_UI
class MainDialog2;
#endif

struct IM_MENU_MANAGER
{
	static const DWORD MAIN_DLG;
	static const DWORD MAIN_DLG_2;
	static const DWORD MAIN_MENU_OPTION_DLG;

	static const DWORD SYSTEM_DLG;	
	static const DWORD CREDIT_DLG;	// Å©·¡µ÷ ¿µ»ó
};

class uiMenuMan :
	public uiBaseMan
{
public:
	uiMenuMan(InterfaceManager *pUIMan);

public:
    virtual void            OnInitialize();

	void					Send_CG_ZONE_EX_TUTORIAL_FIELD_LEAVE_SYN();
	void					Send_CG_ZONE_EX_TUTORIAL_MISSION_LEAVE_SYN();
#ifdef _NA_000000_20130114_RENEWER_UI
	void					KeyModifyChange();
#endif
private:
	MainDialog *			m_pMainDlg;
	MainMenuOptionDlg *		m_pMainOptionDlg;
	SystemDialog *			m_pSystemDlg;

	CreditDialog *			m_pCreditDlg;
#ifdef _NA_000000_20130114_RENEWER_UI
	MainDialog2 * 				m_pMaindialog2;
#endif
};
