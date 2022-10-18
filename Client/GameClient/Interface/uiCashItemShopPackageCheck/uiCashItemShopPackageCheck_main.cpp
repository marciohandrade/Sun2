#include "SunClientPrecompiledHeader.h"

//------------------------------------------------------------------------------ 
#ifndef _NA_0_20110613_GAME_SHOP_RENEWAL //#if'N'def
//------------------------------------------------------------------------------ 


#include "uiCashItemShopPackageCheck.h"
#include "uiCashItemShopMan/uiCashItemShopMan.h"
#include "uiSystemMan/uiSystemMan.h"
#include "Hero.h"
#include "GlobalFunc.h"
#include "interfacemanager.h"
#include "GameFramework.h"
#include "ItemInfoParser.h"
#include "Mouse.h"
#include "uiToolTipMan/uiToolTipMan.h"

//------------------------------------------------------------------------------
WzID uiCashItemShopPackageCheck::m_wzId[eDIALOG_CASH_ITEM_SHOP_PACKAGE_CHECK_MAX] = 
{
	StrToWzID("BT00"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_CHECK_EXIT = 0,			//	X 버튼

	StrToWzID("LS00"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_CHECK_LST_DESC,			//	패키지 목록 리스트

	StrToWzID("VS00"), //	eDIALOG_CASH_ITEM_SHOP_PACKAGE_CHECK_VSCR,				//  패키지 목록 스크롤

	StrToWzID("ST04"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_CHECK_TXT_PRICE,			//	패키지 가격

	StrToWzID("BT01"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_CHECK_BTN_OK,			//	확인 버튼
	StrToWzID("BT02"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_CHECK_BTN_CANCLE,		//	취소 버튼
};

//------------------------------------------------------------------------------
/**
*/
uiCashItemShopPackageCheck::uiCashItemShopPackageCheck(InterfaceManager *pUIMan) :
	uiBase(pUIMan),
	m_bControlsInitialized(false)
{
	this->m_pCashItemShopMan =
		static_cast<uiCashItemShopMan *>(this->GetUIMan()->GetUserInterfaceManager(UIMAN_CASH_ITEM_SHOP));
	assert (m_pCashItemShopMan);

	if (!this->m_pCashItemShopMan)
	{
#ifdef _DEBUG
		uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
		if(pSystemMan)
			pSystemMan->Sys_VerifyLock(_T("m_pCashItemShopMan이 없습니다."));
#endif
		this->ShowInterface(FALSE);
		return;
	}

	m_bShow = false;
}

//------------------------------------------------------------------------------
/**
*/
uiCashItemShopPackageCheck::~uiCashItemShopPackageCheck()
{
	ZeroMemory(&m_PackageItemInfo, sizeof(m_PackageItemInfo));
	this->Release();
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopPackageCheck::Init(CDrawBase* pDrawBase)
{
	uiBase::Init(pDrawBase);

	this->InitControls();
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopPackageCheck::Release()
{
}
//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopPackageCheck::InitControls()
{
	int i=0;

	m_pListDesc = static_cast<CCtrlListWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_PACKAGE_CHECK_LST_DESC));
	assert(m_pListDesc);

	m_pScroll = static_cast<CCtrlVScrollWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_PACKAGE_CHECK_VSCR));
	assert(m_pScroll);

	m_pTxtCtrlPrice = static_cast<CCtrlStaticWZ*>(getControl(eDIALOG_CASH_ITEM_SHOP_PACKAGE_CHECK_TXT_PRICE));
	assert(m_pTxtCtrlPrice);

	this->m_bControlsInitialized = true;
}

//------------------------------------------------------------------------------
/**
*/
BOOL uiCashItemShopPackageCheck::_isEnable()
{
	if(GENERALPARAM->IsNet())
	{
		if (this->IsLock())
			return FALSE;

		if(!g_pHero)
			return FALSE;

		if(g_pHero && g_pHero->IsShowFullMap())
			return FALSE;

		if(!GetManager())
			return FALSE;

		return TRUE;
	}
	else
	{
		return TRUE;
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopPackageCheck::_refresh()
{
	_clearItem();
	_setItem();
}


//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopPackageCheck::_clearItem()
{
	GlobalFunc::SetCaption(m_pTxtCtrlPrice, _T(""));

	if(m_pListDesc)
	{
		m_pListDesc->SetUseReport(false);
		m_pListDesc->DeleteItemAll();
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopPackageCheck::_setItem()
{
	TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szTemp2[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szTemp3[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szTemp4[INTERFACE_STRING_LENGTH] = {0,};


	size_t i;

	if(m_pListDesc)
	{
		std::vector<util::_tstring> vstrMsg;
		vstrMsg.clear();

		RECT rc = m_pListDesc->GetSizeRect();
#ifdef __LTJ_TEXT_OUTPUT_AT_LIST_MOD
		int iListWidth = rc.right - rc.left - 20;
#else
		int iListWidth = rc.right - rc.left - 10;
#endif	

		m_pListDesc->DeleteItemAll();
		m_pListDesc->SetColorSelBar(c_CashItemShopListSelBarColor);
		m_pListDesc->SetUseReport(false);
		int iListIndex = -1;

		// 패키지 이름
		GlobalFunc::ProcessWordWrapByFontSize( &vstrMsg, m_PackageItemInfo.szName, c_wiCashItemShopFont, _tcslen(m_PackageItemInfo.szName), iListWidth );
		// 아이템 정보
		stPackageDetailInfo* pPackaIteminfo;

		for (i = 0; i < m_PackageItemInfo.byDetailItemCount; ++i)
		{
			pPackaIteminfo = NULL;
			pPackaIteminfo = &m_PackageItemInfo.stDetailItem[i];

			if(pPackaIteminfo)
			{
				TCHAR szCharNumber[INTERFACE_STRING_LENGTH];
				// 이름
				if(pPackaIteminfo->dwItemCode)
				{
					g_InterfaceManager.GetItemName(pPackaIteminfo->dwItemCode, szTemp2, INTERFACE_STRING_LENGTH);

					if(pPackaIteminfo->byEnchant)
					{
						Snprintf(szTemp3, INTERFACE_STRING_LENGTH-1, _T("+%d"), pPackaIteminfo->byEnchant);
						_tcscat(szTemp2, szTemp3);
					}
					
					BASE_ITEMINFO * pItemInfo = ItemInfoParser::Instance()->GetItemInfo( pPackaIteminfo->dwItemCode );
					if( pItemInfo && pItemInfo->CanRankUpItem())
					{
						int iRankIndex = eST_RANK_E_STRING_INDEX  + (pPackaIteminfo->byRank);
						if (iRankIndex > eST_RANK_PS_STRING_INDEX)
							iRankIndex = eST_RANK_PS_STRING_INDEX;
						if (iRankIndex < eST_RANK_E_STRING_INDEX)
							iRankIndex = eST_RANK_E_STRING_INDEX;

						ZeroMemory(szTemp3, sizeof(szTemp3));
						ZeroMemory(szTemp4, sizeof(szTemp4));
						g_InterfaceManager.GetInterfaceString(eST_RANK_STRING_INDEX,szTemp3,INTERFACE_STRING_LENGTH);
						g_InterfaceManager.GetInterfaceString(iRankIndex,szTemp4,INTERFACE_STRING_LENGTH);	
						Snprintf(szMessage,INTERFACE_STRING_LENGTH-1, szTemp3, szTemp4);
						_tcscat(szTemp2, _T("   "));
						_tcscat(szTemp2, szMessage);
						ZeroMemory(szMessage, sizeof(szMessage));
					}

				}
				// 사용갯수
				if(pPackaIteminfo->Aoumt)
				{
					//155378	%s %d개
					g_InterfaceManager.GetInterfaceString(155378 , szCharNumber ,INTERFACE_STRING_LENGTH );
					Snprintf(szTemp, INTERFACE_STRING_LENGTH-1, szCharNumber, szTemp2, pPackaIteminfo->Aoumt);
				}
				else
				{
					Snprintf(szTemp, INTERFACE_STRING_LENGTH-1, szTemp2);
				}
				// 사용기간
				if(pPackaIteminfo->dwUseTime)
				{
					int day = 0;
					int hour = 0;
					int min = 0;
					GlobalFunc::CalcTime(pPackaIteminfo->dwUseTime, day, hour, min, szTemp2);
					//155379	%s %s
					g_InterfaceManager.GetInterfaceString(155379 , szCharNumber ,INTERFACE_STRING_LENGTH );
					Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, szCharNumber, szTemp, szTemp2);
				}
				else
				{
					Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, szTemp);
				}

				if(szMessage)
				{
					GlobalFunc::ProcessWordWrapByFontSize( &vstrMsg, szMessage, c_wiCashItemShopFont, _tcslen(szMessage), iListWidth );
					ZeroMemory(szMessage, sizeof(szMessage));
					ZeroMemory(szTemp, sizeof(szTemp));
					ZeroMemory(szTemp2, sizeof(szTemp2));
				}
			}
		}

		for (i=0; i<vstrMsg.size(); ++i)
		{
			iListIndex = m_pListDesc->InsertItemBack();

			TCHAR *pStr = const_cast<TCHAR *>(vstrMsg[i].c_str());
			GlobalFunc::SetItem(
				m_pListDesc, 
				iListIndex, 0, pStr, 
				c_wiCashItemShopFont,
				DT_VCENTER | DT_LEFT, 
				c_CashItemShopListBKColor, 
				c_CashItemShopListTextColor, 
				c_CashItemShopListSelTextColor);
		}   

		int iScrollRangeSize = ( vstrMsg.size() < _PACKAGE_CHECK_LIST_LINE_NUM ) ? 0 : vstrMsg.size() - _PACKAGE_CHECK_LIST_LINE_NUM;

		if(m_pScroll)
		{
			m_pScroll->SetScrollRangeWZ( 0, iScrollRangeSize );	//스크롤 범위
			m_pScroll->SetScrollPosWZ(0);		// 현재 페이지
		}
	}

    BYTE cash_type = GetManager()->GetUseCashType();
    if(m_PackageItemInfo.dwPrice)
	{
    	GlobalFunc::SetMoneyText(m_pTxtCtrlPrice, m_PackageItemInfo.dwPrice, cash_type);
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopPackageCheck::ShowPackageCheck(stPackageItemInfo* pInfo)
{
	if(!pInfo)
		return;

	CopyMemory(&m_PackageItemInfo, pInfo, sizeof(m_PackageItemInfo));
	ShowInterface(TRUE);

	_refresh();

}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopPackageCheck::OnShowWindow( BOOL val )
{
	if (val)
	{
		//uiBase::MoveCenter();

		if (!this->m_bControlsInitialized)
		{
			this->ShowInterface(FALSE);
			return;
		}

		keyMsg msg;
		ZeroMemory(&msg,sizeof(msg));
		msg.dwType = eDoType_dialog_cash_item_shop_package_check;
		msg.wParam = InterfaceManager::DIALOG_CASH_ITEM_SHOP_PACKAGE_CHECK;
		msg.DoSomething = GlobalFunc::DoShowWindow;
		g_KeyQueueManager.PushBack(msg);

		m_bShow = true;
	}
	else
	{
		g_KeyQueueManager.DeleteMsg(eDoType_dialog_cash_item_shop_package_check);
		m_bShow = false;
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopPackageCheck::OnUpdateSolarDialog()
{
	if(!m_bShow)
		this->ShowInterface(FALSE);
}


//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopPackageCheck::MessageProc( SI_MESSAGE * pMessage )
{
	if (!_isEnable())
		return;

	TCHAR	szMessage[INTERFACE_STRING_LENGTH] = {0,};

	switch( pMessage->eResultMsg ) 
	{
	case RT_MSG_LBUTTONCLICK:
		{
			POSTYPE posType = getCtrlIDToPos( pMessage->dwCtrlID );
			switch( posType )
			{
			case eDIALOG_CASH_ITEM_SHOP_PACKAGE_CHECK_EXIT:
			case eDIALOG_CASH_ITEM_SHOP_PACKAGE_CHECK_BTN_CANCLE:
				{
					m_bShow = false;
				}
				break;

			case eDIALOG_CASH_ITEM_SHOP_PACKAGE_CHECK_BTN_OK:
				{
					if(!GetManager()->GetRqstState())	// 구매요청후 기다리는 중
					{
						GetManager()->ClearPendingBuyPackageItemInfo();

						if(m_PackageItemInfo.priceGuid > 0)
						{
							GetManager()->m_pendingBuyPackageIndex = m_PackageItemInfo.priceGuid;
							CopyMemory(&(GetManager()->m_pendingBuyPackageItemInfo), &m_PackageItemInfo, sizeof(m_PackageItemInfo));
							GetManager()->Send_CG_SHOP_ITEM_BUY_SYN();
						}
						else
						{
							//85198	선택한 아이템이 없습니다.
							g_InterfaceManager.GetInterfaceString(85198, szMessage, INTERFACE_STRING_LENGTH);
							GlobalFunc::OutputMsg_SysMsg(szMessage);
						}
					}
				}
				break;
			}
		}
		break;

	case RT_MSG_SB_PAGETOP:		//. 윗쪽 몸통을 눌렀다.
	case RT_MSG_SB_LINETOP:		//.윗쪽 화살표를 눌렀다.
		{
			int pos = m_pScroll->GetScrollPosWZ();//현재 스크롤 값을 받아 온다.
			int mmin = 0 , mmax = 0;
			m_pScroll->GetScrollRangeWZ( &mmin, &mmax );//스크롤 값의 최대 최소를 얻어 온다.

			if(pos>mmin)// 현재 입력된 라인수 보다 작으면 
			{
				pos--;//한줄 감소 시키고 
				m_pScroll->SetScrollPosWZ(pos);

				m_pListDesc->SetBeginDrawIndex(pos);
			}
		}
		break;

	case RT_MSG_SB_PAGEBOTTOM:	//. 윗쪽 몸통을 눌렀다.
	case RT_MSG_SB_LINEBOTTOM:	//.아랫쪽 화살표를 눌렀다
		{
			int pos = m_pScroll->GetScrollPosWZ();//현재 스크롤 값을 받아 온다.
			int mmin = 0 , mmax = 0;
			m_pScroll->GetScrollRangeWZ( &mmin, &mmax );//스크롤 값의 최대 최소를 얻어 온다.

			if(pos<mmax)// 현재 입력된 라인수 보다 작으면 
			{
				pos++;//한줄 증가 시키고 
				m_pScroll->SetScrollPosWZ(pos);

				m_pListDesc->SetBeginDrawIndex(pos);
			}
		}
		break;

	case RT_MSG_SB_THUMBTRACK:				//. 썸을 드래그 하고 있다.
	case RT_MSG_SB_THUMBPOSITION:			//. 썸을 드래그한후 마우스를 놓았다.
	case RT_MSG_SB_ENDSCROLL:				//. 스크롤이 종료 되었다.
		{
			int pos = m_pScroll->GetScrollPosWZ();//현재 스크롤 값을 받아 온다.
			m_pScroll->SetScrollPosWZ(pos);

			m_pListDesc->SetBeginDrawIndex(pos);
		}
		break;

	default:
		{
			SolarDialog::MessageProc( pMessage );
		}
		break;
	}
}


//------------------------------------------------------------------------------ 
#endif //#ifndef _NA_0_20110613_GAME_SHOP_RENEWAL //#if'N'def
//------------------------------------------------------------------------------ 
