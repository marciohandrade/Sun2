#include "StdAfx.h"
#include "ISolarConsole_Guid.h"
#include "SolarConsoleFactory.h"
#include "SolarConsole.h"



void StringFromCLSIDCustom(const CLSID& clsid, char * szCLSID, int length);
BOOL SetRegistryKeyCustom(const char * szKey, const char * szSubKey, const char * szvalueName, const char * szValue);
BOOL DeleteRegistryKeyCustom(HKEY hKey, const char * pKeyName);

HINSTANCE g_hInstance			= NULL;
const int CLSID_STRING_SIZE		= 39;
const char * szFriendlyName		= "SolarConsole Components by taiyo";
const char * szProgID			= "SolarConsole.Component";
const char * szVerIndProgID		= "SolarConsole.Component.1";
const char * szThreadingModel	= "Both";

LONG g_cComponents				= 0;
LONG g_cLocks					= 0;


BOOL APIENTRY DllMain(HINSTANCE hModule, DWORD dwReason, LPVOID lpReserved)
{
	__UNUSED(lpReserved);

	switch(dwReason)
	{
	case DLL_PROCESS_ATTACH:
		g_hInstance = hModule;
		break;
	}


	return TRUE;
}






STDAPI DllRegisterServer()
{
	char szModule[512];
	char szCLSID[CLSID_STRING_SIZE];
	char szKey[64];

	DWORD dwResult = ::GetModuleFileName( g_hInstance, szModule, sizeof(szModule)/sizeof(char) );
	if(!dwResult) return E_FAIL;

	StringFromCLSIDCustom( CLSID_SOLAR_CONSOLE, szCLSID, sizeof(szCLSID) );
	strcpy(szKey, "CLSID\\");
	strcat(szKey, szCLSID);

	SetRegistryKeyCustom(szKey, NULL, NULL, szFriendlyName);
	SetRegistryKeyCustom(szKey, "InprocServer32", NULL, szModule);
	SetRegistryKeyCustom(szKey, "InprocServer32", "ThreadingModel", szThreadingModel);
	SetRegistryKeyCustom(szKey, "ProgID", NULL, szProgID);
	SetRegistryKeyCustom(szKey, "VersionIndependentProgID", NULL, szVerIndProgID);


	SetRegistryKeyCustom(szVerIndProgID, NULL, NULL, szFriendlyName);
	SetRegistryKeyCustom(szVerIndProgID, "CLSID", NULL, szCLSID);
	SetRegistryKeyCustom(szVerIndProgID, "CurVer", NULL, szProgID);

	SetRegistryKeyCustom(szProgID, NULL, NULL, szFriendlyName);
	SetRegistryKeyCustom(szProgID, "CLSID", NULL, szCLSID);

	return S_OK;
}

STDAPI DllUnregisterServer()
{
	char szCLSID[CLSID_STRING_SIZE];
	char szKey[64];

	StringFromCLSIDCustom( CLSID_SOLAR_CONSOLE, szCLSID, sizeof(szCLSID) );
	strcpy(szKey, "CLSID\\");
	strcpy(szKey, szCLSID);

	long lResult = DeleteRegistryKeyCustom(HKEY_CLASSES_ROOT, szKey);
	lResult = DeleteRegistryKeyCustom(HKEY_CLASSES_ROOT, szVerIndProgID);
	lResult = DeleteRegistryKeyCustom(HKEY_CLASSES_ROOT, szProgID);

	return S_OK;
}


STDAPI DllGetClassObject( REFCLSID rclsid, REFIID riid, LPVOID* ppv )
{
	HRESULT hr = CLASS_E_CLASSNOTAVAILABLE;

	if(CLSID_SOLAR_CONSOLE != rclsid) return hr;

	
	IUnknown* pIFactory = new SolarConsoleFactory;
	if (!pIFactory) return E_OUTOFMEMORY;

	hr = pIFactory->QueryInterface(riid, ppv);
	pIFactory->Release();
	

	return hr;
}

STDAPI DllCanUnloadNow()
{
	if( 0 == g_cComponents && 0 == g_cLocks )
	{
		return S_OK;
	}
	else
	{
		return S_FALSE;
	}

}



//====================================================================================
// 레지스트리 관련 전역 함수
//====================================================================================
void StringFromCLSIDCustom(const CLSID& clsid, char * szCLSID, int length)
{
	LPOLESTR wszCLSID = NULL;

	HRESULT hr = StringFromCLSID(clsid, &wszCLSID);
	__UNUSED(hr);

	wcstombs(szCLSID, wszCLSID, length);

	CoTaskMemFree(wszCLSID);
}

BOOL SetRegistryKeyCustom(const char * szKey, const char * szSubKey, const char * szvalueName, const char * szValue)
{
	HKEY hKey;
	char szkeyPath[1024];

	strcpy(szkeyPath, szKey);
	if(szSubKey != NULL)
	{
		strcat(szkeyPath, "\\");
		strcat(szkeyPath, szSubKey);
	}

	long lResult = RegCreateKeyEx(HKEY_CLASSES_ROOT, szkeyPath, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL);
	if(lResult != ERROR_SUCCESS) return FALSE;

	lResult = RegSetValueEx(hKey, szvalueName, 0, REG_SZ, (BYTE *)szValue, (DWORD)(strlen(szValue)+1) );
	if(lResult != ERROR_SUCCESS) return FALSE;

	lResult = RegCloseKey(hKey);
	if(lResult != ERROR_SUCCESS) return FALSE;

	return TRUE;
}

BOOL DeleteRegistryKeyCustom(HKEY hKey, const char * pKeyName)
{
	HKEY hRKey;
	DWORD dwSubkeyLength;
	DWORD dwRtn;
	TCHAR szSubKey[256];

	if(pKeyName && lstrlen(pKeyName))
	{
		if((dwRtn = RegOpenKeyEx(hKey, pKeyName, 0, KEY_ENUMERATE_SUB_KEYS | DELETE, &hRKey)) == ERROR_SUCCESS)
		{
			while(dwRtn == ERROR_SUCCESS)
			{
				dwSubkeyLength = 256;
				dwRtn = RegEnumKeyEx(hRKey, 0, szSubKey, &dwSubkeyLength, NULL, NULL, NULL, NULL);
				if(dwRtn == ERROR_NO_MORE_ITEMS)
				{
					dwRtn = RegDeleteKey(hKey, pKeyName);
					break;
				}
				else if(dwRtn == ERROR_SUCCESS)
				{
					dwRtn = DeleteRegistryKeyCustom(hRKey, szSubKey);
				}
			}
			RegCloseKey(hRKey);
		}
	}
	else
		dwRtn = ERROR_BADKEY;

	return dwRtn;
}