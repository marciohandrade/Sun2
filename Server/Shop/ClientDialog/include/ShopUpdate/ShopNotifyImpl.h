#pragma once

// Backward Compatibility
#if defined( _MSC_VER )
	#if(_MSC_VER < 1310)	// vs6
		#pragma comment(lib, "comsupp.lib")
	#elif(_MSC_VER < 1400)	// vs2003 ÀÌÈÄ
		#ifdef _NATIVE_WCHAR_T_DEFINED
			#ifdef _DEBUG
				#pragma comment(lib, "comsuppwd.lib")
			#else
				#pragma comment(lib, "comsuppw.lib")
			#endif
		#else
			#ifdef _DEBUG
				#pragma comment(lib, "comsuppd.lib")
			#else
				#pragma comment(lib, "comsupp.lib")
			#endif
		#endif
	#endif
#endif

#ifdef _DEBUG
	#import "../../ScriptVersionUpdateNotifyd.dll" no_namespace
#else
	#import "../../ScriptVersionUpdateNotify.dll" no_namespace
	//..
#endif


class CShopNotifyImpl
{
public:
	CShopNotifyImpl(void);
	~CShopNotifyImpl(void);

	void PrintError(HRESULT hr = 0);

	bool SendSalesScriptVersionUpdate(wchar_t* szIP, unsigned short port, long GameCode, unsigned short SalesZone, unsigned short Year, unsigned short YearIdentity);
	bool SendBannerScriptVersionUpdate(wchar_t* szIP, unsigned short port, long GameCode, unsigned short BannerZone, unsigned short Year, unsigned short YearIdentity);
	bool SendPCBangIPAddressUpdate(wchar_t* szIP, unsigned short port, long GameCode, unsigned long PCRoomGUID);
};
