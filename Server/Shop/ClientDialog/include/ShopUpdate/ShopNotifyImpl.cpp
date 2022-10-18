#include "StdAfx.h"
#include "ShopNotifyImpl.h"

CShopNotifyImpl::CShopNotifyImpl(void)
{
	HRESULT hr = ::CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
	if (FAILED(hr)) { 
		PrintError(hr); 
	}
}

CShopNotifyImpl::~CShopNotifyImpl(void)
{
	::CoUninitialize();
}

void CShopNotifyImpl::PrintError(HRESULT hr)
{
	TCHAR* lpOSMsg;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
				  NULL,
				  hr,
				  MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				  (LPTSTR)&lpOSMsg,
				  0,
				  NULL);
	CString msg;
	msg.Format(_T("[%ul, 0x%x] %s\n"), hr, hr, lpOSMsg);
	AfxMessageBox(msg);

	LocalFree(lpOSMsg);
}

bool CShopNotifyImpl::SendSalesScriptVersionUpdate(wchar_t* szIP, unsigned short port, long GameCode, unsigned short SalesZone, unsigned short Year, unsigned short YearIdentity)
{
	IShopNotifyPtr ShopNotifyPtr; 
	HRESULT hr = ShopNotifyPtr.CreateInstance(__uuidof(ShopNotify));
	if (FAILED(hr)) { 
		PrintError(hr); 
		return false;
	}

	if (ShopNotifyPtr) {
		if (ShopNotifyPtr->SendSalesScriptVersionUpdate(_bstr_t(szIP), port, GameCode, SalesZone , Year , YearIdentity)) {
			return true;
		}
	}
	return false;
}

bool CShopNotifyImpl::SendBannerScriptVersionUpdate(wchar_t* szIP, unsigned short port, long GameCode, unsigned short BannerZone, unsigned short Year, unsigned short YearIdentity)
{
	IShopNotifyPtr ShopNotifyPtr; 
	HRESULT hr = ShopNotifyPtr.CreateInstance(__uuidof(ShopNotify));
	if (FAILED(hr)) { 
		PrintError(hr); 
		return false;
	}

	if (ShopNotifyPtr) {
		if (ShopNotifyPtr->SendBannerScriptVersionUpdate(_bstr_t(szIP), port, GameCode, BannerZone , Year , YearIdentity)) {
			return true;
		}
	}
	return false;
}

bool CShopNotifyImpl::SendPCBangIPAddressUpdate(wchar_t* szIP, unsigned short port, long GameCode, unsigned long PCRoomGUID)
{
	IShopNotifyPtr ShopNotifyPtr; 
	HRESULT hr = ShopNotifyPtr.CreateInstance(__uuidof(ShopNotify));
	if (FAILED(hr)) { 
		PrintError(hr); 
		return false;
	}

	if(ShopNotifyPtr) {
		if (ShopNotifyPtr->SendPCBangIPAddressUpdate(_bstr_t(szIP), port, GameCode, PCRoomGUID)) {
			return true;
		}
	}
	return false;
}