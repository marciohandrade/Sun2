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
			// 5357	�κ��丮�� ���� á���ϴ�.
			g_InterfaceManager.GetInterfaceString( 5357 , szMessage , INTERFACE_STRING_LENGTH );
		}
		break;

	case RC_VENDOR_CANNOT_TRADE_ITEM:
		{
			// 5350	�ŷ��� �������� ���� �������Դϴ�.
			g_InterfaceManager.GetInterfaceString(5350 , szMessage ,INTERFACE_STRING_LENGTH);
		}
		break;
		 
	case RC_VENDOR_NOT_AVAILABLE_SERVICE:
		{
			// 5812	����� �̿��Ͻ� �� ���� �����Դϴ�.
			g_InterfaceManager.GetInterfaceString(5812, szMessage ,INTERFACE_STRING_LENGTH);
		}
		break;

	case RC_VENDOR_BLOCKED_ITEM:
		{
			// 5811	���� �������Դϴ�.
			g_InterfaceManager.GetInterfaceString(5811, szMessage ,INTERFACE_STRING_LENGTH);
		}
		break;

	case RC_VENDOR_INCLUDE_INVALID_ITEM_TO_START:
		{
			// 5810	�ŷ��� �� ���� �������� ���ԵǾ� ���λ����� ������ �� �����ϴ�.
			g_InterfaceManager.GetInterfaceString(5810, szMessage ,INTERFACE_STRING_LENGTH);
		}
		break;

	case RC_VENDOR_UNABLE_FUNCTION_FOR_CHAOSTATE:
		{
			// 5275	ī�� ���¿����� �̿��� �������� �ʽ��ϴ�.
			g_InterfaceManager.GetInterfaceString(5275 , szMessage ,INTERFACE_STRING_LENGTH);
		}
		break;

	case RC_VENDOR_HAVE_NO_MONEY:
		{
			// 551	������ �����մϴ�.
			g_InterfaceManager.GetInterfaceString(551 , szMessage , INTERFACE_STRING_LENGTH);
		}
		break;

	case RC_VENDOR_ALREADY_END:
		{
			// 5809	������ ����Ǿ� �̿��� �� �����ϴ�.
			g_InterfaceManager.GetInterfaceString(5809 , szMessage ,INTERFACE_STRING_LENGTH);
		}
		break;

	case RC_VENDOR_INVALID_STATE:
		{
			//5822	���λ����� �̿��� �� ���� �����Դϴ�.
			g_InterfaceManager.GetInterfaceString(5822 , szMessage ,INTERFACE_STRING_LENGTH);
		}
		break;

	case RC_VENDOR_TITLE_BAN_WORD:
		{
			// �����ܾ�� ����Ҽ� �����ϴ�.
			g_InterfaceManager.GetInterfaceString(70022 , szMessage ,INTERFACE_STRING_LENGTH);
		}
		break;

	case RC_VENDOR_INVENTORY_NEED_PASSWORD:
		{
			//5571 â�� ��� �����Դϴ�.
			g_InterfaceManager.GetInterfaceString(5571,szMessage);
		}
		break;

	default:
		{
			// 5813	������ ������ �� �� ���� �̻� ������ Ȯ�εǾ����ϴ�.
			g_InterfaceManager.GetInterfaceString(5813 ,szMessage ,INTERFACE_STRING_LENGTH);
		}
		break;

	}
	GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT , szMessage , INTERFACE_STRING_LENGTH);
}

//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------