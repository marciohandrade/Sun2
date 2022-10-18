//------------------------------------------------------------------------------
//  uiVendorMan_main.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "uiVendorMan.h"
#include "uiVendorBuy/uiVendorBuy.h"
#include "uiVendorSell/uiVendorSell.h"
#include "uiVendorPopup/uiVendorPopup.h"
#include "uiVendorTitlePopup/uiVendorTitlePopup.h"
#include "InterfaceManager.h"
#include "ResultCode.h"
#include "GlobalFunc.h"

using namespace RC;

//------------------------------------------------------------------------------
/**
*/
uiVendorMan::uiVendorMan(InterfaceManager *pUIMan) :
    uiBaseMan(pUIMan)
{
    m_pUIVendorBuy = NULL;
    m_pUIVendorSell = NULL;
    m_pUIVendorPopup = NULL;
    m_pUIVendorTitlePopup = NULL;
}


void
uiVendorMan::OnInitialize()
{
    _LoadUI();
}

void uiVendorMan::__errorcode(int iError)
{
	TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};

	switch ( iError) 
	{	
	case RC_VENDOR_NOSPACEININVENTORY:
		{	
			// 5357	인벤토리가 가득 찼습니다.
			g_InterfaceManager.GetInterfaceString( 5357 , szMessage , INTERFACE_STRING_LENGTH );
		}
		break;

	case RC_VENDOR_CANNOT_TRADE_ITEM:
		{
			// 5350	거래가 가능하지 않은 아이템입니다.
			g_InterfaceManager.GetInterfaceString(5350 , szMessage ,INTERFACE_STRING_LENGTH);
		}
		break;
		 
	case RC_VENDOR_NOT_AVAILABLE_SERVICE:
		{
			// 5812	현재는 이용하실 수 없는 서비스입니다.
			g_InterfaceManager.GetInterfaceString(5812, szMessage ,INTERFACE_STRING_LENGTH);
		}
		break;

	case RC_VENDOR_BLOCKED_ITEM:
		{
			// 5811	블럭된 아이템입니다.
			g_InterfaceManager.GetInterfaceString(5811, szMessage ,INTERFACE_STRING_LENGTH);
		}
		break;

	case RC_VENDOR_INCLUDE_INVALID_ITEM_TO_START:
		{
			// 5810	거래할 수 없는 아이템이 포함되어 개인상점을 시작할 수 없습니다.
			g_InterfaceManager.GetInterfaceString(5810, szMessage ,INTERFACE_STRING_LENGTH);
		}
		break;

	case RC_VENDOR_UNABLE_FUNCTION_FOR_CHAOSTATE:
		{
			// 5275	카오 상태에서는 이용이 가능하지 않습니다.
			g_InterfaceManager.GetInterfaceString(5275 , szMessage ,INTERFACE_STRING_LENGTH);
		}
		break;

	case RC_VENDOR_HAVE_NO_MONEY:
		{
			// 551	하임이 부족합니다.
			g_InterfaceManager.GetInterfaceString(551 , szMessage , INTERFACE_STRING_LENGTH);
		}
		break;

	case RC_VENDOR_ALREADY_END:
		{
			// 5809	상점이 종료되어 이용할 수 없습니다.
			g_InterfaceManager.GetInterfaceString(5809 , szMessage ,INTERFACE_STRING_LENGTH);
		}
		break;

	case RC_VENDOR_INVALID_STATE:
		{
			//5822	개인상점을 이용할 수 없는 상태입니다.
			g_InterfaceManager.GetInterfaceString(5822 , szMessage ,INTERFACE_STRING_LENGTH);
		}
		break;

	case RC_VENDOR_TITLE_BAN_WORD:
		{
			// 금지단어라 사용할수 없습니다.
			g_InterfaceManager.GetInterfaceString(70022 , szMessage ,INTERFACE_STRING_LENGTH);
		}
		break;

	case RC_VENDOR_INVENTORY_NEED_PASSWORD:
		{
			//5571 창고가 잠금 상태입니다.
			g_InterfaceManager.GetInterfaceString(5571,szMessage);
		}
		break;

	default:
		{
			// 5813	상점과 관련한 알 수 없는 이상 현상이 확인되었습니다.
			g_InterfaceManager.GetInterfaceString(5813 ,szMessage ,INTERFACE_STRING_LENGTH);
		}
		break;

	}
	GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT , szMessage , INTERFACE_STRING_LENGTH);
}

//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------