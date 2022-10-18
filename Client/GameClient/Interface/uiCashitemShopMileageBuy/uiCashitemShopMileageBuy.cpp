#include "SunClientPrecompiledHeader.h"


//------------------------------------------------------------------------------ 
#ifndef _NA_0_20110613_GAME_SHOP_RENEWAL //#if'N'def
//------------------------------------------------------------------------------ 



#ifdef __NA_00387_20070605_SHOP_MILEAGE2

#include "uiCashItemShopMileageBuy.h"
#include "uiCashItemShopMan/uiCashItemShopMan.h"
#include "uiSystemMan/uiSystemMan.h"
#include "Hero.h"
#include "GlobalFunc.h"

#include "interfacemanager.h"
#include "ItemInfoParser.h"
#include "Mouse.h"
#include "uiToolTipMan/uiToolTipMan.h"
#include "ItemManager.h"
#include "SolarDateTime.h"
#include "GameFunc.h"

//------------------------------------------------------------------------------
WzID uiCashItemShopMileageBuy::m_wzId[eDIALOG_CASH_ITEM_SHOP_MILEAGE_BUY_MAX] = 
{
	StrToWzID("B999"),	//eDIALOG_CASH_ITEM_SHOP_MILEAGE_BUY_BTN_EXIT = 0,

	StrToWzID("B001"),	//eDIALOG_CASH_ITEM_SHOP_MILEAGE_BUY_IMG_ITEM,

	StrToWzID("LS00"),	//eDIALOG_CASH_ITEM_SHOP_MILEAGE_BUY_LST_DESC,
	StrToWzID("L001"),	//eDIALOG_CASH_ITEM_SHOP_MILEAGE_BUY_TIP_LST_DESC,

	StrToWzID("T004"),	//eDIALOG_CASH_ITEM_SHOP_MILEAGE_BUY_TXT_DESC1,	
	StrToWzID("T012"),	//eDIALOG_CASH_ITEM_SHOP_MILEAGE_BUY_TXT_DESC2,	
	StrToWzID("T008"),	//eDIALOG_CASH_ITEM_SHOP_MILEAGE_BUY_TXT_DESC3,	
	StrToWzID("T005"),	//eDIALOG_CASH_ITEM_SHOP_MILEAGE_BUY_TXT_DESC4,	
	StrToWzID("T013"),	//eDIALOG_CASH_ITEM_SHOP_MILEAGE_BUY_TXT_DESC5,	
	StrToWzID("T009"),	//eDIALOG_CASH_ITEM_SHOP_MILEAGE_BUY_TXT_DESC6,	
	StrToWzID("T006"),	//eDIALOG_CASH_ITEM_SHOP_MILEAGE_BUY_TXT_DESC7,	
	StrToWzID("T014"),	//eDIALOG_CASH_ITEM_SHOP_MILEAGE_BUY_TXT_DESC8,
	StrToWzID("T010"),	//eDIALOG_CASH_ITEM_SHOP_MILEAGE_BUY_TXT_DESC9,
	StrToWzID("T007"),	//eDIALOG_CASH_ITEM_SHOP_MILEAGE_BUY_TXT_DESC10,
	StrToWzID("T015"),	//eDIALOG_CASH_ITEM_SHOP_MILEAGE_BUY_TXT_DESC11,
	StrToWzID("T011"),	//eDIALOG_CASH_ITEM_SHOP_MILEAGE_BUY_TXT_DESC12,

	StrToWzID("T016"),	//eDIALOG_CASH_ITEM_SHOP_MILEAGE_BUY_TXT_SALE1,	
	StrToWzID("T017"),	//eDIALOG_CASH_ITEM_SHOP_MILEAGE_BUY_TXT_SALE2,	
	StrToWzID("T018"),	//eDIALOG_CASH_ITEM_SHOP_MILEAGE_BUY_TXT_SALE3,
	StrToWzID("T019"),	//eDIALOG_CASH_ITEM_SHOP_MILEAGE_BUY_TXT_SALE4,

	StrToWzID("T002"),	//eDIALOG_CASH_ITEM_SHOP_MILEAGE_BUY_TXT_NAME,
	StrToWzID("T023"),	//eDIALOG_CASH_ITEM_SHOP_MILEAGE_BUY_TXT_ITEM_COST_MONEY,
	StrToWzID("T024"),	//eDIALOG_CASH_ITEM_SHOP_MILEAGE_BUY_TXT_MONEY,

	StrToWzID("C001"),	//eDIALOG_CASH_ITEM_SHOP_MILEAGE_BUY_CHK_SEL1,
	StrToWzID("C002"),	//eDIALOG_CASH_ITEM_SHOP_MILEAGE_BUY_CHK_SEL2,
	StrToWzID("CT02"),	//eDIALOG_CASH_ITEM_SHOP_MILEAGE_BUY_CHK_SEL3,
	StrToWzID("CT03"),	//eDIALOG_CASH_ITEM_SHOP_MILEAGE_BUY_CHK_SEL4,

	StrToWzID("B004"),	//eDIALOG_CASH_ITEM_SHOP_MILEAGE_BUY_BTN_GIFT,
	StrToWzID("B002"),	//eDIALOG_CASH_ITEM_SHOP_MILEAGE_BUY_BTN_CART,
	StrToWzID("B003"),	//eDIALOG_CASH_ITEM_SHOP_MILEAGE_BUY_BTN_BUY,
	StrToWzID("VS00"),	//eDIALOG_CASH_ITEM_SHOP_MILEAGE_BUY_VSCR
};

//------------------------------------------------------------------------------
/**
*/
uiCashItemShopMileageBuy::uiCashItemShopMileageBuy(InterfaceManager *pUIMan) :
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
	ZeroMemory(&m_BuyItemImg, sizeof(m_BuyItemImg));
	ZeroMemory(&m_BuyMileageItemInfo, sizeof(m_BuyMileageItemInfo));
	ZeroMemory(&m_BuyMileageItemCheckInfo, sizeof(m_BuyMileageItemCheckInfo));
	ZeroMemory(&m_ItemCodeForTooltip, sizeof(m_ItemCodeForTooltip));

	m_iSelectIndex = -1;
	m_iTipListCurLine = 0;
}

//------------------------------------------------------------------------------
/**
*/
uiCashItemShopMileageBuy::~uiCashItemShopMileageBuy()
{
	this->Release();
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopMileageBuy::Init(CDrawBase* pDrawBase)
{
	uiBase::Init(pDrawBase);

	this->InitControls();
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopMileageBuy::Release()
{
	this->_removeIconImage();
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopMileageBuy::InitControls()
{
	int i=0;
	int iCount = 0;

	for(i = 0; i < _MAX_PRICE_INFO; ++i)
	{
		for(int k = 0; k < eItemDescMax; ++k)
		{
			m_pTxtCtrlDesc[i][k] = static_cast<CCtrlStaticWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_MILEAGE_BUY_TXT_DESC1+iCount));
			assert(m_pTxtCtrlDesc[i][k]);
			++iCount;
		}
	}

	for(i=0; i< _MAX_PRICE_INFO; ++i)
	{
		m_pTxtCtrlSale[i] = static_cast<CCtrlStaticWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_MILEAGE_BUY_TXT_SALE1+i));
		assert(m_pTxtCtrlSale[i]);
	}


	for(i=0; i < eCASH_ITEM_SHOP_MILEAGE_BUY_TXT_MAX; ++i)
	{
		m_pTxtCtrl[i] = static_cast<CCtrlStaticWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_MILEAGE_BUY_TXT_NAME+i));
		assert(m_pTxtCtrl[i]);
	}

	for(i=0; i < eCASH_ITEM_SHOP_MILEAGE_BUY_CHK_MAX; ++i)
	{
		m_pBtnCheckTabs[i] = static_cast<CCtrlButtonCheckWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_MILEAGE_BUY_CHK_SEL1+i));
		assert(m_pBtnCheckTabs[i]);
	}

	m_pImgCtrl = static_cast<CCtrlButtonWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_MILEAGE_BUY_IMG_ITEM));
	assert(m_pImgCtrl);

	m_pListDesc = static_cast<CCtrlListWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_MILEAGE_BUY_LST_DESC));
	assert(m_pListDesc);

	m_pTipListDesc = static_cast<CCtrlListWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_MILEAGE_BUY_TIP_LST_DESC));
	assert(m_pTipListDesc);

	m_pScroll = static_cast<CCtrlVScrollWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_MILEAGE_BUY_VSCR));
	assert(m_pScroll);

	this->m_bControlsInitialized = true;
}


//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopMileageBuy::_clearItem()
{
	this->_removeIconImage();

	int i=0;

	for(i=0; i<_MAX_PRICE_INFO; ++i)
	{
		m_pBtnCheckTabs[i]->SetCheckState(false);
	}

	for(i=0; i<_MAX_PRICE_INFO; ++i)
	{
		for(int k=0; k<eItemDescMax; ++k)
		{
			GlobalFunc::SetCaption(m_pTxtCtrlDesc[i][k], _T(""));
		}
	}

	for(i=0; i<_MAX_PRICE_INFO; ++i)
	{
		GlobalFunc::SetCaption(m_pTxtCtrlSale[i], _T(""));
	}


	for(i=0; i<eCASH_ITEM_SHOP_MILEAGE_BUY_TXT_MAX; ++i)
	{
		GlobalFunc::SetCaption(m_pTxtCtrl[i], _T(""));
	}

	if(m_pListDesc)
	{
		m_pListDesc->SetUseReport(false);
		m_pListDesc->DeleteItemAll();
	}

	if(m_pScroll)
	{
		if(m_pScroll)
		{
			m_pScroll->SetScrollRangeWZ(0,0);	//스크롤 범위
			m_pScroll->SetScrollPosWZ(0);			// 현재 페이지
		}
	}
	if(m_pTipListDesc)
	{
		m_pTipListDesc->SetUseReport(false);
		m_pTipListDesc->DeleteItemAll();
	}
	m_iTipListCurLine = 0;

	ZeroMemory(&m_BuyMileageItemCheckInfo, sizeof(m_BuyMileageItemCheckInfo));
	m_iSelectIndex = -1;
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopMileageBuy::_setItem()
{
	BASE_ITEMINFO * pItemInfo = ItemInfoParser::Instance()->GetItemInfo( m_BuyMileageItemInfo.dwItemCode );
	if ( !pItemInfo ) 
	{
		return;
	}

	TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szTemp2[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szTemp3[INTERFACE_STRING_LENGTH] = {0,};

	g_InterfaceManager.GetItemName(m_BuyMileageItemInfo.dwItemCode, szMessage, INTERFACE_STRING_LENGTH);


	if(m_BuyMileageItemInfo.byEnchant)
	{
		Snprintf(szTemp, INTERFACE_STRING_LENGTH-1, _T("+%d"), m_BuyMileageItemInfo.byEnchant);
		_tcscat(szMessage, szTemp);
	}

    // 캐시샵아이템의 Rank 값을 용도 변경하여 Rank를 출력 하지 않도록 수정
    // 이전부터 Rank자체는 사용되고 있지 않았음.
    //if( pItemInfo->CanRankUpItem())
	//{
	//	int iRankIndex = eST_RANK_E_STRING_INDEX  + (m_BuyMileageItemInfo.byRank);
	//	if (iRankIndex > eST_RANK_PS_STRING_INDEX)
	//		iRankIndex = eST_RANK_PS_STRING_INDEX;
	//	if (iRankIndex < eST_RANK_E_STRING_INDEX)
	//		iRankIndex = eST_RANK_E_STRING_INDEX;

	//	ZeroMemory(szTemp, sizeof(szTemp));
	//	ZeroMemory(szTemp2, sizeof(szTemp2));
	//	ZeroMemory(szTemp3, sizeof(szTemp3));
	//	g_InterfaceManager.GetInterfaceString(eST_RANK_STRING_INDEX,szTemp,INTERFACE_STRING_LENGTH);
	//	g_InterfaceManager.GetInterfaceString(iRankIndex,szTemp2,INTERFACE_STRING_LENGTH);	
	//	Snprintf(szTemp3,INTERFACE_STRING_LENGTH-1, szTemp, szTemp2);
	//	_tcscat(szMessage, _T("   "));
	//	_tcscat(szMessage, szTemp3);
	//}

	GlobalFunc::SetCaption(m_pTxtCtrl[eCASH_ITEM_SHOP_MILEAGE_BUY_TXT_NAME], szMessage);	// 아이템 이름

	// 간단 설명
	ZeroMemory(szMessage, sizeof(szMessage));
	if(pItemInfo->m_TDNCode)
	{
        GameFunc::GetItemDescription(szMessage, NULL, pItemInfo);
		if(m_pListDesc)
		{
			std::vector<util::_tstring> vstrMsg;
			vstrMsg.clear();

			if(!_tcslen(szMessage))
				return;

			RECT rc = m_pListDesc->GetSizeRect();
#ifdef __LTJ_TEXT_OUTPUT_AT_LIST_MOD
			int iListWidth = rc.right - rc.left - 20;
#else
			int iListWidth = rc.right - rc.left - 10;
#endif	
			GlobalFunc::ProcessWordWrapByFontSize( &vstrMsg, szMessage, c_wiCashItemShopFont, _tcslen(szMessage), iListWidth );

			m_pListDesc->DeleteItemAll();
			m_pListDesc->SetColorSelBar(c_CashItemShopListSelBarColor);
			m_pListDesc->SetUseReport(false);
			int iListIndex = -1;

			for (size_t i=0; i<vstrMsg.size(); ++i)
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
		}
	}

	GetManager()->SetItemTipListDesc(pItemInfo, m_pTipListDesc, m_iTipListCurLine, m_BuyMileageItemInfo.dwCoolTime);
	if(m_pScroll)
	{
		m_pScroll->SetScrollRangeWZ(0,m_pTipListDesc->GetListRowCnt());	// 스크롤 범위
		m_pScroll->SetScrollPosWZ(m_iTipListCurLine);			// 현재 라인 위치
	}

    m_BuyItemImg.type = m_BuyMileageItemInfo.byRank;
	_addIconImage(pItemInfo->m_wVICode, m_BuyMileageItemInfo.dwItemCode);

	int day = 0;
	int hour = 0;
	int min = 0;

	for(int i=0; i<_MAX_PRICE_INFO; ++i)
	{
		if(m_BuyMileageItemInfo.stPrice[i].dwUseTime)	// %d 일
		{
			ZeroMemory(szMessage, sizeof(szMessage));
			GlobalFunc::CalcTime(m_BuyMileageItemInfo.stPrice[i].dwUseTime, day, hour, min, szMessage);

			GlobalFunc::SetCaption(m_pTxtCtrlDesc[i][0], szMessage);	
		}

		if(m_BuyMileageItemInfo.stPrice[i].dwPrice)
		{
			ZeroMemory(szMessage, sizeof(szMessage));
			GlobalFunc::SetMoneyText(m_pTxtCtrlDesc[i][1], m_BuyMileageItemInfo.stPrice[i].dwPrice, m_BuyMileageItemInfo.moneyType);
		}

		TCHAR szTemp[INTERFACE_STRING_LENGTH] ={0,};
		if(m_BuyMileageItemInfo.stPrice[i].dwAmount)	// %d 개
		{
			ZeroMemory(szMessage, sizeof(szMessage));
			// 155376	%d개
			g_InterfaceManager.GetInterfaceString(155376 , szTemp ,INTERFACE_STRING_LENGTH);
			Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, szTemp , m_BuyMileageItemInfo.stPrice[i].dwAmount);
			GlobalFunc::SetCaption(m_pTxtCtrlDesc[i][2], szMessage);	
		}

		if(m_BuyMileageItemInfo.stPrice[i].bySaleRatio)
		{
			ZeroMemory(szMessage, sizeof(szMessage));
			//155377	%d%% 세일가격
			g_InterfaceManager.GetInterfaceString(155377 , szTemp ,INTERFACE_STRING_LENGTH );
			Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, szTemp, m_BuyMileageItemInfo.stPrice[i].bySaleRatio);
			GlobalFunc::SetCaption(m_pTxtCtrlSale[i], szMessage);	
		}

	}
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopMileageBuy::_addIconImage(DWORD dwCode, CODETYPE itemCode)
{
	m_BuyItemImg.dwIconCode = dwCode;
	m_BuyItemImg.itemCode = itemCode;
	_removeIconImage();

	if (dwCode != 0)
	{
		m_BuyItemImg.pImage  = ICONMGR()->Add(dwCode);
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopMileageBuy::_removeIconImage()
{
	if(m_BuyItemImg.pImage)
	{
		ICONMGR()->RemoveIcon(m_BuyItemImg.pImage);
		m_BuyItemImg.pImage = NULL;
	}

}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopMileageBuy::_renderTooltip()
{
	if(this->m_ItemCodeForTooltip.dwItemCode > 0)
	{
		uiToolTipMan* pUIMan = (uiToolTipMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_TOOLTIP);
		if(pUIMan)
		{
			pUIMan->RegisterTooltipItemForCashShop(m_ItemCodeForTooltip.dwItemCode, m_ItemCodeForTooltip.dwCoolTime, m_ItemCodeForTooltip.SellEndDate,
				m_ItemCodeForTooltip.byEnchant, m_ItemCodeForTooltip.byRank);
			pUIMan->SetTooltipDialogID(GetDialogKey());
		}
	}

	int iMouseX = Mouse::Instance()->GetMouseX();
	int iMouseY = Mouse::Instance()->GetMouseY();

	// 스킵처리 
	// 인터페이스에서 Dialog 바탕에서는 Mouse Over가 오지 않는다.
	BOOL bMouseOver = FALSE;
	if(m_pImgCtrl)
	{
		RECT rc = m_pImgCtrl->GetSizeRect();

		if (rc.left <= iMouseX && rc.right >= iMouseX)
		{
			if (rc.top <= iMouseY && rc.bottom >= iMouseY)
			{
				bMouseOver = TRUE;
			}
		}
	}

	if ( !bMouseOver )
	{
		ZeroMemory(&m_ItemCodeForTooltip, sizeof(m_ItemCodeForTooltip));
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopMileageBuy::_renderIconImage()
{
	if(m_pImgCtrl)
	{
		if (m_BuyItemImg.pImage && m_BuyItemImg.dwIconCode)
		{
			RECT rc = m_pImgCtrl->GetSizeRect();
			m_BuyItemImg.pImage->Render(&rc);
            GetManager()->renderCoverImage(m_BuyItemImg, rc);
		}
	}
}

//------------------------------------------------------------------------------
void uiCashItemShopMileageBuy::ShowBuyItem(stCashItemInfo* pInfo)
{
	if( !pInfo )
		return;

	ZeroMemory(&m_ItemCodeForTooltip, sizeof(m_ItemCodeForTooltip));

	CopyMemory(&m_BuyMileageItemInfo, pInfo, sizeof(m_BuyMileageItemInfo));

	ShowInterface(TRUE);

	_refresh();
}

//------------------------------------------------------------------------------
void uiCashItemShopMileageBuy::_refresh()
{
	_clearItem();

	_setItem();

}
//------------------------------------------------------------------------------
void uiCashItemShopMileageBuy::OnShowWindow( BOOL val )
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
		msg.dwType = eDoType_dialog_cash_item_shop_mileage_buy;
		msg.wParam = InterfaceManager::DIALOG_CASH_ITEM_SHOP_MILEAGE_BUY;
		msg.DoSomething = GlobalFunc::DoShowWindow;
		g_KeyQueueManager.PushBack(msg);

		m_bShow = true;
	}
	else
	{
		g_KeyQueueManager.DeleteMsg(eDoType_dialog_cash_item_shop_mileage_buy);
		m_bShow = false;
	}
}

//------------------------------------------------------------------------------
/**
*/
BOOL uiCashItemShopMileageBuy::_isEnable()
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
void uiCashItemShopMileageBuy::MessageProc( SI_MESSAGE * pMessage )
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
			case eDIALOG_CASH_ITEM_SHOP_MILEAGE_BUY_BTN_EXIT:
				{
					m_bShow = false;
				}
				break;

			case eDIALOG_CASH_ITEM_SHOP_MILEAGE_BUY_CHK_SEL1:
			case eDIALOG_CASH_ITEM_SHOP_MILEAGE_BUY_CHK_SEL2:
			case eDIALOG_CASH_ITEM_SHOP_MILEAGE_BUY_CHK_SEL3:
			case eDIALOG_CASH_ITEM_SHOP_MILEAGE_BUY_CHK_SEL4:
				{
					for(int i=0; i<eCASH_ITEM_SHOP_MILEAGE_BUY_CHK_MAX; ++i)
					{
						ZeroMemory(&m_BuyMileageItemCheckInfo.stPrice[i], sizeof(m_BuyMileageItemCheckInfo.stPrice[i]));
					}

					int iIndex = posType - eDIALOG_CASH_ITEM_SHOP_MILEAGE_BUY_CHK_SEL1;
					if((iIndex < eCASH_ITEM_SHOP_MILEAGE_BUY_CHK_SEL1) && (iIndex > eCASH_ITEM_SHOP_MILEAGE_BUY_CHK_SEL4))
						this->m_iSelectIndex = -1;
					else
					{
						if(m_BuyMileageItemInfo.stPrice[iIndex].priceGuid > 0)
						{
							this->m_iSelectIndex = iIndex;
							m_pBtnCheckTabs[m_iSelectIndex]->SetCheckState(true);
						}
						else
						{
							ZeroMemory(&m_BuyMileageItemCheckInfo.stPrice[iIndex], sizeof(m_BuyMileageItemCheckInfo.stPrice[iIndex]));
							m_pBtnCheckTabs[iIndex]->SetCheckState(false);
						}

						CopyMemory(&m_BuyMileageItemCheckInfo.stPrice[m_iSelectIndex], &m_BuyMileageItemInfo.stPrice[m_iSelectIndex], sizeof(m_BuyMileageItemInfo.stPrice[m_iSelectIndex]));
					}
				}
				break;

			case eDIALOG_CASH_ITEM_SHOP_MILEAGE_BUY_BTN_BUY:
				{
					if(m_iSelectIndex >= 0)
					{
						// 구매확인 팝업창
						m_BuyMileageItemCheckInfo.dwCoolTime = m_BuyMileageItemInfo.dwCoolTime;
						m_BuyMileageItemCheckInfo.dwItemCode = m_BuyMileageItemInfo.dwItemCode;
						m_BuyMileageItemCheckInfo.itemguid = m_BuyMileageItemInfo.itemguid;
						m_BuyMileageItemCheckInfo.moneyType = m_BuyMileageItemInfo.moneyType;
						m_BuyMileageItemCheckInfo.SellEndDate = m_BuyMileageItemInfo.SellEndDate;
						GetManager()->ShowCashItemShopMileageCheck(&m_BuyMileageItemCheckInfo);
					}
					else
					{
						// 85203	아이템을 선택하세요.
						g_InterfaceManager.GetInterfaceString(85203, szMessage, INTERFACE_STRING_LENGTH);
						GlobalFunc::OutputMsg_SysMsg(szMessage);
					}
				}
				break;
			}
		}
		break;

	case RT_MSG_SB_PAGETOP:		//. 윗쪽 몸통을 눌렀다.
	case RT_MSG_SB_LINETOP:		//.윗쪽 화살표를 눌렀다.
		{
			if(m_pScroll)
			{
				int pos = m_pScroll->GetScrollPosWZ();//현재 스크롤 값을 받아 온다.
				int mmin = 0 , mmax = 0;
				m_pScroll->GetScrollRangeWZ( &mmin, &mmax );//스크롤 값의 최대 최소를 얻어 온다.

				if(pos>mmin)// 현재 입력된 라인수 보다 작으면 
				{
					pos--;//한줄 감소 시키고 
					m_pScroll->SetScrollPosWZ(pos);

					m_iTipListCurLine = pos;
				}
			}
		}
		break;
	case RT_MSG_SB_PAGEBOTTOM:	//. 윗쪽 몸통을 눌렀다.
	case RT_MSG_SB_LINEBOTTOM:	//.아랫쪽 화살표를 눌렀다
		{
			if(m_pScroll)
			{
				int pos = m_pScroll->GetScrollPosWZ();//현재 스크롤 값을 받아 온다.
				int mmin = 0 , mmax = 0;
				m_pScroll->GetScrollRangeWZ( &mmin, &mmax );//스크롤 값의 최대 최소를 얻어 온다.

				if(pos<mmax)// 현재 입력된 라인수 보다 작으면 
				{
					pos++;//한줄 증가 시키고 
					m_pScroll->SetScrollPosWZ(pos);

					m_iTipListCurLine = pos;
				}
			}

		}
		break;

	case RT_MSG_SB_THUMBTRACK:				//. 썸을 드래그 하고 있다.
	case RT_MSG_SB_THUMBPOSITION:			//. 썸을 드래그한후 마우스를 놓았다.
	case RT_MSG_SB_ENDSCROLL:				//. 스크롤이 종료 되었다.
		{
			if(m_pScroll)
			{
				int pos = m_pScroll->GetScrollPosWZ();//현재 스크롤 값을 받아 온다.
				m_pScroll->SetScrollPosWZ(pos);

				m_iTipListCurLine = pos;
			}
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
/**
*/
void uiCashItemShopMileageBuy::OnUpdateSolarDialog()
{
	if(!m_bShow)
		this->ShowInterface(FALSE);

	if (!_isEnable())
		return;

    total_cash_	= 0;

	for(int k=0; k< _MAX_PRICE_INFO; ++k)
	{
		m_pBtnCheckTabs[k]->SetCheckState(false);
	}

	if((m_iSelectIndex >= eCASH_ITEM_SHOP_MILEAGE_BUY_CHK_SEL1) && (m_iSelectIndex < eCASH_ITEM_SHOP_MILEAGE_BUY_CHK_MAX))
	{
		m_pBtnCheckTabs[m_iSelectIndex]->SetCheckState(true);

		total_cash_ += static_cast<MONEY>(this->m_BuyMileageItemInfo.stPrice[m_iSelectIndex].dwPrice);
	}

	GlobalFunc::SetMoneyText(m_pTxtCtrl[eCASH_ITEM_SHOP_MILEAGE_BUY_TXT_ITEM_COST_MONEY], total_cash_, m_BuyMileageItemInfo.moneyType);
	GlobalFunc::SetMoneyText(m_pTxtCtrl[eCASH_ITEM_SHOP_MILEAGE_BUY_TXT_MONEY], GetManager()->GetMyMileage(), m_BuyMileageItemInfo.moneyType);
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopMileageBuy::OnRenderSolarDialog()
{
	if (!_isEnable())
		return;

	this->_renderIconImage();
	this->_renderTooltip();
}

//----------------------------------------------------------------------------
/**
*/
void uiCashItemShopMileageBuy::MouseOver_MsgProc( SI_MESSAGE * pMessage )
{
	if (!_isEnable())
		return;

	switch( pMessage->eResultMsg ) 
	{
	case RT_MSG_MOUSEOVER:
		{
			POSTYPE pos = getCtrlIDToPos( pMessage->dwCtrlID );

			if (pos == eDIALOG_CASH_ITEM_SHOP_MILEAGE_BUY_IMG_ITEM)
			{
				if (m_BuyItemImg.pImage && m_BuyItemImg.dwIconCode)
				{
					m_ItemCodeForTooltip.dwItemCode = m_BuyItemImg.itemCode;
					m_ItemCodeForTooltip.dwCoolTime = m_BuyMileageItemInfo.dwCoolTime;
					m_ItemCodeForTooltip.SellEndDate = m_BuyMileageItemInfo.SellEndDate;
					m_ItemCodeForTooltip.byEnchant		= m_BuyMileageItemInfo.byEnchant;
					m_ItemCodeForTooltip.byRank			= m_BuyMileageItemInfo.byRank;
				}
				else
				{
					ZeroMemory(&m_ItemCodeForTooltip, sizeof(m_ItemCodeForTooltip));
				}
			}
			else
			{
				ZeroMemory(&m_ItemCodeForTooltip, sizeof(m_ItemCodeForTooltip));
			}			
		}
		break;
	}
}

#endif 

//------------------------------------------------------------------------------ 
#endif //#ifndef _NA_0_20110613_GAME_SHOP_RENEWAL //#if'N'def
//------------------------------------------------------------------------------ 
