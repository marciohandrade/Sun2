#pragma once
#ifdef _NA_0_20110613_GAME_SHOP_RENEWAL


#ifndef GAMESHOP_HEADER_H_
#define GAMESHOP_HEADER_H_

#include <public/ShopDefine.h>
#include <public/protocol_shop.h>
#include <WBANetwork/Common.h>
#include <WBANetwork/WBANetwork.h>


#ifdef _DEBUG
    #define TAIL_TAG_FOR_GAMESHOP_LIB_LINKAGE "_d"
#else
    #define TAIL_TAG_FOR_GAMESHOP_LIB_LINKAGE ""
#endif


#if _MSC_VER >= 1600 // vs2010
    #pragma comment(lib, "ClientStub_VS2010_MB"TAIL_TAG_FOR_GAMESHOP_LIB_LINKAGE".lib")
#elif _MSC_VER >= 1500 // vs2008
    #pragma comment(lib, "ClientStub_VS2008_MB"TAIL_TAG_FOR_GAMESHOP_LIB_LINKAGE".lib")
#elif _MSC_VER >= 1310 // vs2003
    #pragma comment(lib, "ClientStub_VS2003_MB"TAIL_TAG_FOR_GAMESHOP_LIB_LINKAGE".lib")
#else
    #pragma message("not tested on a low version complier");
#endif



#endif // GAMESHOP_HEADER_H_

#endif //_NA_0_20110613_GAME_SHOP_RENEWAL
