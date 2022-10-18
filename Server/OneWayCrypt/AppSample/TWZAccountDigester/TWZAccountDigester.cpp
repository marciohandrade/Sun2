

#include <Windows.h>
#include <shlwapi.h>
#include <tchar.h>
#include <comdef.h>
#include <CRTDBG.H>
#include <atlconv.h>

#include <strsafe.h>
#include "resource.h"
#include "OSVersion.h"

#pragma comment(lib,"shlwapi.lib")

typedef const char* (*lpfnDigest)(BSTR, BSTR);
typedef bool (*lpfnCompare)(BSTR, BSTR, BSTR);
lpfnDigest		XP_MD5_CryptKeyEx	= NULL;
lpfnDigest		XP_SHA2_CryptKeyEx	= NULL;
lpfnCompare		XP_MD5_CompareEx	= NULL;
lpfnCompare		XP_SHA2_CompareEx	= NULL;


void CenterWindow(HWND hwnd);
INT_PTR CALLBACK MainDlgProcedure(HWND hwnd, UINT iMessage, UINT wParam, LPARAM lParam);


void LOGOUT(LPTSTR pFormat, ...)
{
	TCHAR		szBuf[1280];
	va_list		pArg;

	try
	{
		va_start(pArg, pFormat);
		StringCchVPrintf(szBuf, 1280, pFormat, pArg);
		va_end(pArg);

		OutputDebugString(szBuf);
		return;
	}
	catch(...)
	{
	}
}


int APIENTRY _tWinMain(HINSTANCE hInstance,
					   HINSTANCE hPrevInstance,
					   LPTSTR    lpCmdLine,
					   int       nCmdShow)
{
	HMODULE				hModule		= NULL;	

	try
	{
		BOOL bSupportOS = FALSE;
		Greenmaru::COSVersion osv;
		if( osv.IsWindowsXp() && 3 <= osv.ServicePack() )
		{
			bSupportOS = TRUE;
		}
		else
		{
			if( osv.IsWindows2003() || osv.IsWindowsVistaOrLater() )
			{
				bSupportOS = TRUE;
			}
		}

		if( FALSE == bSupportOS )
		{
			MessageBox(NULL, L"Support OS is XP Service pack 3, VISTA, WINDOWS 7 or later and Server 2003, Server 2008 or later", L"WEBZEN Message Digest 테스트", MB_OK|MB_ICONSTOP);
			return 0;
		}


#ifdef _X64_VERIFY_
#ifdef _DEBUG
		hModule = LoadLibrary(L"WZMsgDigest_x64d.dll");
#else
		hModule = LoadLibrary(L"WZMsgDigest_x64.dll");
#endif
#else

#ifdef _DEBUG
		hModule = LoadLibrary(L"WZMsgDigest_x86d.dll");
#else
		hModule = LoadLibrary(L"WZMsgDigest_x86.dll");
#endif

#endif

		if( hModule )
		{
			XP_MD5_CryptKeyEx	= (lpfnDigest)GetProcAddress(hModule, "XP_MD5_CryptKeyEx");
			XP_SHA2_CryptKeyEx	= (lpfnDigest)GetProcAddress(hModule, "XP_SHA2_CryptKeyEx");
			XP_MD5_CompareEx	= (lpfnCompare)GetProcAddress(hModule, "XP_MD5_CompareEx");
			XP_SHA2_CompareEx	= (lpfnCompare)GetProcAddress(hModule, "XP_SHA2_CompareEx");

			if( NULL != XP_MD5_CryptKeyEx && NULL != XP_SHA2_CryptKeyEx && NULL != XP_MD5_CompareEx && NULL != XP_SHA2_CompareEx )
			{
				::DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DLG_FORM), HWND_DESKTOP, (DLGPROC)MainDlgProcedure, NULL);
			}
			else
			{
				MessageBox(NULL, L"Fail to GetProcAddress(XP_MD5_CryptKeyEx, XP_SHA2_CryptKeyEx, XP_MD5_CompareEx, XP_SHA2_CompareEx).", L"WEBZEN Message Digest 테스트", MB_OK|MB_ICONSTOP);
			}
		}
		else
		{
			MessageBox(NULL, L"Fail to LoadLibrary(WZMsgDigest_XXX.dll).실행파일과 같은 폴더내에 WZMsgDigest_XXX.dll 파일이 존재하는지 확인하십시오.", L"WEBZEN Message Digest 테스트", MB_OK|MB_ICONSTOP);
		}

	}
	catch(...)
	{
	}
	
	if( hModule )
	{
		FreeLibrary(hModule);
	}

	return 0;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function		: MainDlgProcedure
// Description	: 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
INT_PTR CALLBACK MainDlgProcedure( HWND hwnd, UINT iMessage, UINT wParam, LPARAM lParam )
{
	switch(iMessage)
	{
	case WM_INITDIALOG:

		CenterWindow(hwnd);
		CheckRadioButton(hwnd, IDC_RADIO_MD5, IDC_RADIO_SHA, IDC_RADIO_SHA);

		return TRUE;

	case WM_COMMAND:

		switch(LOWORD(wParam))
		{
		case IDOK:
			{
				USES_CONVERSION;

				TCHAR szID[32], szPW[32], szRet[65];

				GetDlgItemText(hwnd, IDC_EDIT_ID, szID, 32*sizeof(TCHAR));
				GetDlgItemText(hwnd, IDC_EDIT_PW, szPW, 32*sizeof(TCHAR));

				LPCSTR pszRet = NULL;

				BSTR bstrID, bstrPW;
				bstrID = ::SysAllocString(T2CW(szID));
				bstrPW = ::SysAllocString(T2CW(szPW));

				if( SendMessage(GetDlgItem(hwnd, IDC_RADIO_SHA), BM_GETCHECK, 0, 0) == BST_CHECKED )
				{
					pszRet = XP_SHA2_CryptKeyEx(bstrID, bstrPW);
				}
				else
				{
					pszRet = XP_MD5_CryptKeyEx(bstrID, bstrPW);
				}

				if( NULL == pszRet || 0 == lstrlenA(pszRet) )
				{
					lstrcpy(szRet, _T("Fail to get Digest"));
				}
				else
				{
					lstrcpyn(szRet, A2CT(pszRet), 65);
				}

				SetDlgItemText(hwnd, IDC_EDIT_DIGEST, szRet);
				
				::SysFreeString(bstrID);
				::SysFreeString(bstrPW);

				return TRUE;
			}

		case IDC_BTN_VERIFY:
			{
				TCHAR szID[32], szPW[32], szDigest[65];
				BSTR bstrID, bstrPW, bstrDigest;

				GetDlgItemText(hwnd, IDC_EDIT_ID, szID, 32*sizeof(TCHAR));
				GetDlgItemText(hwnd, IDC_EDIT_PW, szPW, 32*sizeof(TCHAR));
				GetDlgItemText(hwnd, IDC_EDIT_DIGEST, szDigest, 65*sizeof(TCHAR));

				if( 0 < lstrlen(szID) && 0 < lstrlen(szPW) && 0 < lstrlen(szDigest) )
				{
					USES_CONVERSION;

					bstrID = ::SysAllocString(T2CW(szID));
					bstrPW = ::SysAllocString(T2CW(szPW));
					bstrDigest = ::SysAllocString(T2CW(szDigest));
					bool bRet = false;
					if( SendMessage(GetDlgItem(hwnd, IDC_RADIO_SHA), BM_GETCHECK, 0, 0) == BST_CHECKED )
					{
						bRet = XP_SHA2_CompareEx(bstrID, bstrDigest, bstrPW);
					}
					else
					{
						bRet = XP_MD5_CompareEx(bstrID, bstrDigest, bstrPW);
					}

					if( true == bRet )
					{
						MessageBox(NULL, _T("Verify successfully"), _T("TWZAccountDigester"), MB_OK|MB_ICONINFORMATION);
					}
					else
					{
						MessageBox(NULL, _T("Verify unsuccessfully"), _T("TWZAccountDigester"), MB_OK|MB_ICONSTOP);
					}

					::SysFreeString(bstrID);
					::SysFreeString(bstrPW);
					::SysFreeString(bstrDigest);
				}

				return TRUE;
			}

		case IDCANCEL:
			EndDialog(hwnd, IDCANCEL);
			return TRUE;
		}

		break;

	case WM_DESTROY:

		return TRUE;
	}

	return FALSE;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function		: CenterWindow
// Description	: 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CenterWindow(HWND hwnd)
{
	RECT wrt;
	LONG iX, iY, iWidth, iHeight;
	
	GetWindowRect(hwnd, &wrt);

	iWidth = wrt.right - wrt.left;
	iHeight = wrt.bottom - wrt.top;
	iX = LONG( (GetSystemMetrics(SM_CXSCREEN) - iWidth) / 2 );
	iY = LONG( (GetSystemMetrics(SM_CYSCREEN) - iHeight) / 2 );
	
	MoveWindow(hwnd, iX, iY, iWidth, iHeight, TRUE);
}
