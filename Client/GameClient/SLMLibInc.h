#pragma once

#if  _MSC_VER >= 1500 // vs2008
	#ifdef _UNICODE
		#ifdef _DEBUG
			#pragma  comment(lib, "../ShopListManager/lib/ShopListManager_VS2008_d.lib")
		#else
			#pragma  comment(lib, "../ShopListManager/lib/ShopListManager_VS2008.lib")
		#endif	
	#else
		#ifdef _DEBUG
            #pragma  comment(lib, "../ShopListManager/lib/ShopListManager_VS2008_MB_MT_d.lib")
		#else
            #pragma  comment(lib, "../ShopListManager/lib/ShopListManager_VS2008_MB_MT.lib")
		#endif	
	#endif
#elif _MSC_VER >= 1310 // vs2003
	#ifdef _UNICODE
		#ifdef _DEBUG
			#pragma  comment(lib, "../ShopListManager/lib/ShopListManager_VS2003_d.lib")
		#else
			#pragma  comment(lib, "../ShopListManager/lib/ShopListManager_VS2003.lib")
		#endif	
	#else
		#ifdef _DEBUG
			#pragma  comment(lib, "../ShopListManager/lib/ShopListManager_VS2003_MB_d.lib")
		#else
			#pragma  comment(lib, "../ShopListManager/lib/ShopListManager_VS2003_MB.lib")
		#endif	
	#endif

#endif //#elif _MSC_VER >= 1310 // vs2003

