#include "stdafx.h"
#include <ClientDialog.h>
#include <ClientDialogDlg.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CClientDialogApp, CWinApp)
#ifdef VERSION_VS60
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
#else
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
#endif
END_MESSAGE_MAP()

CClientDialogApp::CClientDialogApp() {}

CClientDialogApp theApp;

BOOL CClientDialogApp::InitInstance()
{
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();
	AfxEnableControlContainer();
	SetRegistryKey(_T("로컬 응용 프로그램 마법사에서 생성된 응용 프로그램"));

	CClientDialogDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK) {
	} else if (nResponse == IDCANCEL) {
	}

	return FALSE;
}
