#include "SunClientPrecompiledHeader.h"


//------------------------------------------------------------------------------ 
#ifndef _NA_0_20110613_GAME_SHOP_RENEWAL //#if'N'def
//------------------------------------------------------------------------------ 



#include "uiCashItemShopExtensionBuy.h"
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
#include "SCItemSlotContainer.h"
#include "SCItemSlot.h"
#include "MouseHandler.h"
#include "ItemUnitRenderer.h"
#include "SlotKeyGenerator.h"
#include "CursorChangeTransact.h"

//------------------------------------------------------------------------------
WzID uiCashItemShopExtensionBuy::m_wzId[eDIALOG_CASH_ITEM_SHOP_EXTENSION_BUY_MAX] = 
{
	StrToWzID("B001"),	//eDIALOG_CASH_ITEM_SHOP_EXTENSION_BUY_IMG_ITEM = 0,

	StrToWzID("B999"),	//eDIALOG_CASH_ITEM_SHOP_EXTENSION_BUY_BTN_EXIT,

	StrToWzID("T004"),	//eDIALOG_CASH_ITEM_SHOP_EXTENSION_BUY_TXT_DESC1,	
	StrToWzID("T012"),	//eDIALOG_CASH_ITEM_SHOP_EXTENSION_BUY_TXT_DESC2,	
	StrToWzID("T005"),	//eDIALOG_CASH_ITEM_SHOP_EXTENSION_BUY_TXT_DESC3,	
	StrToWzID("T013"),	//eDIALOG_CASH_ITEM_SHOP_EXTENSION_BUY_TXT_DESC4,	
	StrToWzID("T006"),	//eDIALOG_CASH_ITEM_SHOP_EXTENSION_BUY_TXT_DESC5,	
	StrToWzID("T014"),	//eDIALOG_CASH_ITEM_SHOP_EXTENSION_BUY_TXT_DESC6,	
	StrToWzID("ST00"),	//eDIALOG_CASH_ITEM_SHOP_EXTENSION_BUY_TXT_DESC7,	
	StrToWzID("ST01"),	//eDIALOG_CASH_ITEM_SHOP_EXTENSION_BUY_TXT_DESC8,

	StrToWzID("T016"),	//eDIALOG_CASH_ITEM_SHOP_EXTENSION_BUY_TXT_SALE1,	
	StrToWzID("T017"),	//eDIALOG_CASH_ITEM_SHOP_EXTENSION_BUY_TXT_SALE2,	
	StrToWzID("T018"),	//eDIALOG_CASH_ITEM_SHOP_EXTENSION_BUY_TXT_SALE3,
	StrToWzID("ST02"),	//eDIALOG_CASH_ITEM_SHOP_EXTENSION_BUY_TXT_SALE4,

	StrToWzID("T002"),	//eDIALOG_CASH_ITEM_SHOP_EXTENSION_BUY_TXT_NAME,
	StrToWzID("T023"),	//eDIALOG_CASH_ITEM_SHOP_EXTENSION_BUY_TXT_ITEM_COST_MONEY,
	StrToWzID("T024"),	//eDIALOG_CASH_ITEM_SHOP_EXTENSION_BUY_TXT_MONEY,

	StrToWzID("C001"),	//eDIALOG_CASH_ITEM_SHOP_EXTENSION_BUY_CHK_SEL1,
	StrToWzID("C002"),	//eDIALOG_CASH_ITEM_SHOP_EXTENSION_BUY_CHK_SEL2,
	StrToWzID("CT02"),	//eDIALOG_CASH_ITEM_SHOP_EXTENSION_BUY_CHK_SEL3,
	StrToWzID("CT03"),	//eDIALOG_CASH_ITEM_SHOP_EXTENSION_BUY_CHK_SEL4,

	StrToWzID("B003"),	//eDIALOG_CASH_ITEM_SHOP_EXTENSION_BUY_BTN_EXTENSION,
};

//------------------------------------------------------------------------------
/**
*/
uiCashItemShopExtensionBuy::uiCashItemShopExtensionBuy(InterfaceManager *pUIMan) :
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
	ZeroMemory(&m_BuyCashItemInfo, sizeof(m_BuyCashItemInfo));
	ZeroMemory(&m_BuyCashItemCheckInfo, sizeof(m_BuyCashItemCheckInfo));
	ZeroMemory(&m_ItemCodeForTooltip, sizeof(m_ItemCodeForTooltip));

	m_iSelectIndex		= -1;
	m_iTipListCurLine	= 0;
	m_iCurIndex			= -1;
	m_fromSlotIdx		= 0;
	m_fromPos			= 0;

	m_pItemUnitRenderer	= NULL;

	SCItemSlotContainer::Init(1, SI_EXTENSION );
}

//------------------------------------------------------------------------------
/**
*/
uiCashItemShopExtensionBuy::~uiCashItemShopExtensionBuy()
{
	SCItemSlotContainer::Release();
	this->Release();
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopExtensionBuy::Init(CDrawBase* pDrawBase)
{
	uiBase::Init(pDrawBase);

	this->InitControls();
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopExtensionBuy::Release()
{
	SAFE_RELEASENDELETE( m_pItemUnitRenderer );
	this->_removeIconImage();
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopExtensionBuy::InitControls()
{
	int i=0;
	int iCount = 0;

	for(i = 0; i < _MAX_PRICE_INFO; ++i)
	{
		for(int k = 0; k < eExtensionItemDescMax; ++k)
		{
			m_pTxtCtrlDesc[i][k] = static_cast<CCtrlStaticWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_EXTENSION_BUY_TXT_DESC1+iCount));
			assert(m_pTxtCtrlDesc[i][k]);
			++iCount;
		}
	}

	for(i=0; i< _MAX_PRICE_INFO; ++i)
	{
		m_pTxtCtrlSale[i] = static_cast<CCtrlStaticWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_EXTENSION_BUY_TXT_SALE1+i));
		assert(m_pTxtCtrlSale[i]);
	}

	for(i=0; i < eCASH_ITEM_SHOP_EXTENSION_BUY_TXT_MAX; ++i)
	{
		m_pTxtCtrl[i] = static_cast<CCtrlStaticWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_EXTENSION_BUY_TXT_NAME+i));
		assert(m_pTxtCtrl[i]);
	}

	for(i=0; i < eCASH_ITEM_SHOP_EXTENSION_BUY_CHK_MAX; ++i)
	{
		m_pBtnCheckTabs[i] = static_cast<CCtrlButtonCheckWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_EXTENSION_BUY_CHK_SEL1+i));
		assert(m_pBtnCheckTabs[i]);
	}

	m_pImgCtrl = static_cast<CCtrlButtonWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_EXTENSION_BUY_IMG_ITEM));
	assert(m_pImgCtrl);

	assert( !m_pItemUnitRenderer );

	m_pItemUnitRenderer = new ItemUnitRenderer;
	m_pItemUnitRenderer->Init( g_pSunRenderer, SCSlotContainer::GetMaxSlotNum() );

	this->m_bControlsInitialized = true;
}


//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopExtensionBuy::_clearItem()
{
	if(!m_bControlsInitialized)	return;

	this->_removeIconImage();

	int i=0;

	for(i=0; i<_MAX_PRICE_INFO; ++i)
	{
		if(m_pBtnCheckTabs[i])
			m_pBtnCheckTabs[i]->SetCheckState(false);
	}

	for(i=0; i<_MAX_PRICE_INFO; ++i)
	{
		for(int k=0; k<eExtensionItemDescMax; ++k)
		{
			if(m_pTxtCtrlDesc[i][k])
				GlobalFunc::SetCaption(m_pTxtCtrlDesc[i][k], _T(""));
		}
	}

	for(i=0; i<_MAX_PRICE_INFO; ++i)
	{
		if(m_pTxtCtrlSale[i])
			GlobalFunc::SetCaption(m_pTxtCtrlSale[i], _T(""));
	}


	for(i=0; i<eCASH_ITEM_SHOP_EXTENSION_BUY_TXT_MAX; ++i)
	{
		if(m_pTxtCtrl[i])
			GlobalFunc::SetCaption(m_pTxtCtrl[i], _T(""));
	}

	ZeroMemory(&m_BuyCashItemCheckInfo, sizeof(m_BuyCashItemCheckInfo));
	m_iSelectIndex = -1;
	m_FromTargetPos = -1;
}

//------------------------------------------------------------------------------
/** 
*/
void uiCashItemShopExtensionBuy::_setIndex( )	
{
	SCItemSlot & rSlot = (SCItemSlot &)GetSlot(0);
	if ( rSlot.GetItemInfo() ) 
	{
        if( rSlot.GetItemInfo()->IsRiderPartsItem()||
            rSlot.GetItemInfo()->IsWeapon() || 
            rSlot.GetItemInfo()->IsArmor() )
		{
			for (int i = 0; i < _ITEM_NUM_PER_PAGE; ++i)
			{
				BASE_ITEMINFO * pItemInfo = ItemInfoParser::Instance()->GetItemInfo(GetManager()->m_stExtensionItemInfo[i].dwItemCode);

    			if (pItemInfo != NULL)
				{
                    //gs_bug_fix: 추후 기간연장타입이 추가될때 이부분에서 슬롯에올라온아이템과 m_stExtensionItemInfo배열안에
                    //            들어있는 아이템코드(m_wType)과비교해서 올바른 인덱스 걸러줘야한다 현재 두아이템간의 연결정보가 없다
                    //             
                    //            todo:추후 아이템관련 m_stExtensionItemInfo정보가 늘어난다면 조건문의 분기가 불가피하다
                    //                 거의 늘어날일이 있을까 싶지만 늘어난다면 타입체크를 구문을 깔끔하게 바꿀필요가있다
                    if(rSlot.GetItemInfo()->IsRiderPartsItem() && pItemInfo->m_wType!=eITEMTYPE_EXTEND_DATE_RIDER)
                    {
                         continue;
                    }
                    
                    if((rSlot.GetItemInfo()->IsWeapon() || rSlot.GetItemInfo()->IsArmor() ) && pItemInfo->m_wType==eITEMTYPE_EXTEND_DATE_RIDER)
                    {
                        continue;
                    }


                    // 해당 아이템 가격 정보가 있으면 Show, 없으면 프로토콜 요청
					if(GetManager()->m_stExtensionItemInfo[i].stPrice[0].priceGuid > 0)
						GetManager()->ShowCashItemShopExtensionBuy(i);
					else
						GetManager()->Send_CG_SHOP_ITEM_REQ_SYN(i, TRUE);

					m_iCurIndex = i;
					return;
				}
			}
		}
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopExtensionBuy::_setItem()
{
	BASE_ITEMINFO * pItemInfo = ItemInfoParser::Instance()->GetItemInfo( m_BuyCashItemInfo.dwItemCode );
	if ( !pItemInfo ) 
	{
		return;
	}

	TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szTemp2[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szTemp3[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szTemp4[INTERFACE_STRING_LENGTH] = {0,};

	SCItemSlot & rSlot = (SCItemSlot &)GetSlot(0);
	if(rSlot.GetItemInfo())
	{
        m_BuyItemImg.type = m_BuyCashItemInfo.byRank;
        _addIconImage(rSlot.GetItemInfo()->m_wVICode, rSlot.GetItemInfo()->m_Code);
		
        g_InterfaceManager.GetItemName(rSlot.GetItemInfo()->m_Code, szMessage, INTERFACE_STRING_LENGTH);

		Snprintf(szTemp, INTERFACE_STRING_LENGTH-1, _T("+%d"), rSlot.GetEnchant());
		_tcscat(szMessage, szTemp);

        // 캐시샵아이템의 Rank 값을 용도 변경하여 Rank를 출력 하지 않도록 수정
        // 이전부터 Rank자체는 사용되고 있지 않았음.
        //if( pItemInfo->CanRankUpItem())
		//{
		//	int iRankIndex = eST_RANK_E_STRING_INDEX  + (rSlot.GetRank());
		//	if (iRankIndex > eST_RANK_PS_STRING_INDEX)
		//		iRankIndex = eST_RANK_PS_STRING_INDEX;
		//	if (iRankIndex < eST_RANK_E_STRING_INDEX)
		//		iRankIndex = eST_RANK_E_STRING_INDEX;

		//	g_InterfaceManager.GetInterfaceString(eST_RANK_STRING_INDEX,szTemp,INTERFACE_STRING_LENGTH);
		//	g_InterfaceManager.GetInterfaceString(iRankIndex,szTemp2,INTERFACE_STRING_LENGTH);	
		//	Snprintf(szTemp3,INTERFACE_STRING_LENGTH-1, szTemp, szTemp2);
		//	_tcscat(szMessage, _T("   "));
		//	_tcscat(szMessage, szTemp3);

		//	ZeroMemory(szTemp, sizeof(szTemp));
		//	ZeroMemory(szTemp2, sizeof(szTemp2));
		//	ZeroMemory(szTemp3, sizeof(szTemp3));
		//}

		if(rSlot.GetItemInfo()->IsArmor())
		{
			//	155418	방어구
			g_InterfaceManager.GetInterfaceString(155418, szTemp, INTERFACE_STRING_LENGTH);
		}
		else if(rSlot.GetItemInfo()->IsWeapon())
		{
			//	155417	무기
			g_InterfaceManager.GetInterfaceString(155417, szTemp, INTERFACE_STRING_LENGTH);
		}
        else if(rSlot.GetItemInfo()->IsRiderPartsItem())
        {
            //	158383 탈것파츠
            g_InterfaceManager.GetInterfaceString(158383, szTemp, INTERFACE_STRING_LENGTH);
        }

		Snprintf(szTemp2, INTERFACE_STRING_LENGTH-1, _T(" (%s)"), szTemp);
		_tcscat( szMessage, szTemp2);
		GlobalFunc::SetCaption(m_pTxtCtrl[eCASH_ITEM_SHOP_EXTENSION_BUY_TXT_NAME], szMessage);	// 아이템 이름
	}

	int day = 0;
	int hour = 0;
	int min = 0;

	for(int i=0; i<_MAX_PRICE_INFO; ++i)
	{
		if(m_BuyCashItemInfo.stPrice[i].dwUseTime)	// %d 일
		{
			ZeroMemory(szMessage, sizeof(szMessage));
			GlobalFunc::CalcTime(m_BuyCashItemInfo.stPrice[i].dwUseTime, day, hour, min, szMessage);

			GlobalFunc::SetCaption(m_pTxtCtrlDesc[i][eExtensionItemDescDay], szMessage);	
		}

		if(m_BuyCashItemInfo.stPrice[i].dwPrice)
		{
			ZeroMemory(szMessage, sizeof(szMessage));
#ifdef __CN_0_20070507_SHOP_MILEAGE
			GlobalFunc::SetMoneyText(m_pTxtCtrlDesc[i][eExtensionItemDescMoney], m_BuyCashItemInfo.stPrice[i].dwPrice, m_BuyCashItemInfo.moneyType);
#else
	#ifdef _NA00XXXX_090111_CASH_TYPE_SUBDIVIDE_		// 기간연장은 cc로만					
			GlobalFunc::SetMoneyText(m_pTxtCtrlDesc[i][eExtensionItemDescMoney], m_BuyCashItemInfo.stPrice[i].dwPrice, CASH_ITEM_SHOP_CCASH);
	#else
			GlobalFunc::SetMoneyText(m_pTxtCtrlDesc[i][eExtensionItemDescMoney], m_BuyCashItemInfo.stPrice[i].dwPrice, (FALSE!=m_BuyCashItemInfo.moneyType));
	#endif//_NA00XXXX_090111_CASH_TYPE_SUBDIVIDE_
#endif
		}

		TCHAR szTemp[INTERFACE_STRING_LENGTH] ={0,};

		if(m_BuyCashItemInfo.stPrice[i].bySaleRatio)
		{
			ZeroMemory(szMessage, sizeof(szMessage));
			//155377	%d%% 세일가격
			g_InterfaceManager.GetInterfaceString(155377 , szTemp ,INTERFACE_STRING_LENGTH );
			Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, szTemp, m_BuyCashItemInfo.stPrice[i].bySaleRatio);
			GlobalFunc::SetCaption(m_pTxtCtrlSale[i], szMessage);	
		}

	}
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopExtensionBuy::_addIconImage(DWORD dwCode, CODETYPE itemCode)
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
void uiCashItemShopExtensionBuy::_removeIconImage()
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
void uiCashItemShopExtensionBuy::_renderTooltip()
{
	if ( this->m_MouseOverObject.eMouseOverType != eMOUSE_OVER_MAX)
	{
		if ( eMOUSE_OVER_SLOT == m_MouseOverObject.eMouseOverType )
		{
			uiToolTipMan* pUIMan = (uiToolTipMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_TOOLTIP);
			if(pUIMan)
            {
                pUIMan->RegisterTooltipItem(m_MouseOverObject.pSlot, FALSE, kLimitStatCompositeEnchant);
				pUIMan->SetTooltipDialogID(GetDialogKey());
            }

			CursorChangeTransact::Instance()->ChangeTranaction( MouseCursor::eMouseCursor_Cursor_Cancle );
		}
	}

	int iMouseX = Mouse::Instance()->GetMouseX();
	int iMouseY = Mouse::Instance()->GetMouseY();

	// 스킵처리 
	// 인터페이스에서 Dialog 바탕에서는 Mouse Over가 오지 않는다.
	BOOL bMouseOver = FALSE;
	for (POSTYPE a = 0; a < GetMaxSlotNum(); a++)
	{
		CControlWZ * pControl = getControl(a);

		RECT rc = pControl->GetSizeRect();

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
		m_MouseOverObject.eMouseOverType = eMOUSE_OVER_MAX;
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopExtensionBuy::_renderIconImage()
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
void uiCashItemShopExtensionBuy::ShowBuyItem(stCashItemInfo* pInfo)
{
	if( !pInfo )
		return;

	ZeroMemory(&m_ItemCodeForTooltip, sizeof(m_ItemCodeForTooltip));

	CopyMemory(&m_BuyCashItemInfo, pInfo, sizeof(m_BuyCashItemInfo));

	ShowInterface(TRUE);

	_refresh();
}

//------------------------------------------------------------------------------
void uiCashItemShopExtensionBuy::_refresh()
{
	_clearItem();

	_setItem();

}

//------------------------------------------------------------------------------
void uiCashItemShopExtensionBuy::OnShowWindow( BOOL val )
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
		msg.dwType = eDoType_dialog_cash_item_shop_extension_buy;
		msg.wParam = InterfaceManager::DIALOG_CASH_ITEM_SHOP_EXTENSION_BUY;
		msg.DoSomething = GlobalFunc::DoShowWindow;
		g_KeyQueueManager.PushBack(msg);

		m_bShow = true;
	}
	else
	{
		g_KeyQueueManager.DeleteMsg(eDoType_dialog_cash_item_shop_extension_buy);
		for (int i = 0; i < GetMaxSlotNum(); ++i)
		{
			if (IsEmpty(i))
			{
				continue;
			}
			DeleteSlot(i, NULL);
		}
		m_bShow = false;
	}
}

//------------------------------------------------------------------------------
/**
*/
BOOL uiCashItemShopExtensionBuy::_isEnable()
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
void uiCashItemShopExtensionBuy::MessageProc( SI_MESSAGE * pMessage )
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
			case eDIALOG_CASH_ITEM_SHOP_EXTENSION_BUY_BTN_EXIT:
				{
					m_bShow = false;
				}
				break;

			case eDIALOG_CASH_ITEM_SHOP_EXTENSION_BUY_CHK_SEL1:
			case eDIALOG_CASH_ITEM_SHOP_EXTENSION_BUY_CHK_SEL2:
			case eDIALOG_CASH_ITEM_SHOP_EXTENSION_BUY_CHK_SEL3:
			case eDIALOG_CASH_ITEM_SHOP_EXTENSION_BUY_CHK_SEL4:
				{
					for(int i=0; i<eCASH_ITEM_SHOP_EXTENSION_BUY_CHK_MAX; ++i)
						ZeroMemory(&m_BuyCashItemCheckInfo.stPrice[i], sizeof(m_BuyCashItemCheckInfo.stPrice[i]));

					int iIndex = posType - eDIALOG_CASH_ITEM_SHOP_EXTENSION_BUY_CHK_SEL1;
					if((iIndex < eCASH_ITEM_SHOP_EXTENSION_BUY_CHK_SEL1) && (iIndex > eCASH_ITEM_SHOP_EXTENSION_BUY_CHK_SEL4))
						this->m_iSelectIndex = -1;
					else
					{
						if(m_BuyCashItemInfo.stPrice[iIndex].priceGuid > 0)
						{
							this->m_iSelectIndex = iIndex;
							m_pBtnCheckTabs[m_iSelectIndex]->SetCheckState(true);
						}
						else
						{
							ZeroMemory(&m_BuyCashItemCheckInfo.stPrice[iIndex], sizeof(m_BuyCashItemCheckInfo.stPrice[iIndex]));
							m_pBtnCheckTabs[iIndex]->SetCheckState(false);
						}
						CopyMemory(&m_BuyCashItemCheckInfo.stPrice[m_iSelectIndex], &m_BuyCashItemInfo.stPrice[m_iSelectIndex], sizeof(m_BuyCashItemInfo.stPrice[m_iSelectIndex]));
					}
				}
				break;

			case eDIALOG_CASH_ITEM_SHOP_EXTENSION_BUY_BTN_EXTENSION:
				{
					if(!GetManager()->GetRqstState())
					{
						if(m_iSelectIndex >= 0)
						{
							// 구매확인 팝업창
							m_BuyCashItemCheckInfo.dwCoolTime = m_BuyCashItemInfo.dwCoolTime;
							m_BuyCashItemCheckInfo.dwItemCode = m_BuyCashItemInfo.dwItemCode;
							m_BuyCashItemCheckInfo.itemguid = m_BuyCashItemInfo.itemguid;
							m_BuyCashItemCheckInfo.moneyType = m_BuyCashItemInfo.moneyType;
							m_BuyCashItemCheckInfo.SellEndDate = m_BuyCashItemInfo.SellEndDate;

							GetManager()->ClearPendingBuyList();
							int iIndex = 0;
							for(int i=0; i<_MAX_PRICE_INFO; ++i)
							{
								if(this->m_BuyCashItemCheckInfo.stPrice[i].priceGuid > 0)
								{
									GetManager()->m_pendingBuyList[iIndex] = m_BuyCashItemCheckInfo.stPrice[i].priceGuid;
									++iIndex;
								}
							}

							CopyMemory(&(GetManager()->m_pendingBuyItemInfo), &m_BuyCashItemCheckInfo, sizeof(this->m_BuyCashItemCheckInfo));
							SCItemSlot & rSlot = (SCItemSlot &)GetSlot(0);

							GetManager()->m_dwPendingExtensionItemCode = rSlot.GetItemInfo()->m_Code;
#ifdef _NA00XXXX_090111_CASH_TYPE_SUBDIVIDE_		// 기간연장은 cc로만					
							GetManager()->SetUseCashType(CASH_ITEM_SHOP_CCASH);
#endif//_NA00XXXX_090111_CASH_TYPE_SUBDIVIDE_
							GetManager()->Send_CG_SHOP_ITEM_BUY_SYN( m_fromSlotIdx, m_fromPos);
						}
						else
						{
							// 85203	아이템을 선택하세요.
							g_InterfaceManager.GetInterfaceString(85203, szMessage, INTERFACE_STRING_LENGTH);
							GlobalFunc::OutputMsg_SysMsg(szMessage);
						}
					}
				}
				break;

			case eDIALOG_CASH_ITEM_SHOP_EXTENSION_BUY_IMG_ITEM:
				{
					CControlWZ * pControl = getControl(eDIALOG_CASH_ITEM_SHOP_EXTENSION_BUY_IMG_ITEM);
					if (!pControl)
						break;

					RECT rcSlot = pControl->GetSizeRect();
					BOOL bRet	= MouseHandler::Instance()->ItemExtension(SI_EXTENSION, eDIALOG_CASH_ITEM_SHOP_EXTENSION_BUY_IMG_ITEM, &rcSlot);

					if (bRet)
					{
						m_fromSlotIdx = MouseHandler::Instance()->GetMouseItem()->m_fromSlotIdx;
						m_fromPos = MouseHandler::Instance()->GetMouseItem()->m_fromPos;
						
						_setIndex();
					}
					
					MouseHandler::Instance()->RollbackItemAtHand();
				}
				break;
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
void uiCashItemShopExtensionBuy::OnUpdateSolarDialog()
{
	if(!m_bShow)
		this->ShowInterface(FALSE);

	if (!_isEnable())
		return;

	total_cash_ = 0;

	for(int k=0; k< _MAX_PRICE_INFO; ++k)
	{
		m_pBtnCheckTabs[k]->SetCheckState(false);
	}

	if((m_iSelectIndex >= eCASH_ITEM_SHOP_EXTENSION_BUY_CHK_SEL1) && (m_iSelectIndex < eCASH_ITEM_SHOP_EXTENSION_BUY_CHK_MAX))
	{
		m_pBtnCheckTabs[m_iSelectIndex]->SetCheckState(true);

		total_cash_ += static_cast<MONEY>(this->m_BuyCashItemCheckInfo.stPrice[m_iSelectIndex].dwPrice);
	}

#ifdef __CN_0_20070507_SHOP_MILEAGE

	GlobalFunc::SetMoneyText(m_pTxtCtrl[eCASH_ITEM_SHOP_EXTENSION_BUY_TXT_ITEM_COST_MONEY], total_cash_, m_BuyCashItemInfo.moneyType);

	if(LOGINPARAM->IsLiveTestServer())	//테섭실행모드
	{
		GlobalFunc::SetMoneyText(m_pTxtCtrl[eCASH_ITEM_SHOP_EXTENSION_BUY_TXT_MONEY], g_pHero->GetMoney(), m_BuyCashItemInfo.moneyType);
	}
	else
	{
		GlobalFunc::SetMoneyText(m_pTxtCtrl[eCASH_ITEM_SHOP_EXTENSION_BUY_TXT_MONEY], GetManager()->GetMyCash(), m_BuyCashItemInfo.moneyType);
	}

#else	//	__CN_0_20070507_SHOP_MILEAGE

#ifdef _NA00XXXX_090111_CASH_TYPE_SUBDIVIDE_
	// 비용
	GlobalFunc::SetMoneyText(m_pTxtCtrl[eCASH_ITEM_SHOP_EXTENSION_BUY_TXT_ITEM_COST_MONEY], total_cash_, CASH_ITEM_SHOP_CCASH);
	// 소지금액
	GlobalFunc::SetMoneyText(m_pTxtCtrl[eCASH_ITEM_SHOP_EXTENSION_BUY_TXT_MONEY], GetManager()->GetMyCash(), CASH_ITEM_SHOP_CCASH);
#else
	if(LOGINPARAM->IsLiveTestServer())	//테섭실행모드
	{
		// 비용
		GlobalFunc::SetMoneyText(m_pTxtCtrl[eCASH_ITEM_SHOP_EXTENSION_BUY_TXT_ITEM_COST_MONEY], total_heim_, false);

		// 소지금액
		GlobalFunc::SetMoneyText(m_pTxtCtrl[eCASH_ITEM_SHOP_EXTENSION_BUY_TXT_MONEY], g_pHero->GetMoney(), false);
	}
	else
	{
		// 비용
		GlobalFunc::SetMoneyText(m_pTxtCtrl[eCASH_ITEM_SHOP_EXTENSION_BUY_TXT_ITEM_COST_MONEY], total_cash_, true);

		// 소지금액
		GlobalFunc::SetMoneyText(m_pTxtCtrl[eCASH_ITEM_SHOP_EXTENSION_BUY_TXT_MONEY], GetManager()->GetMyCash(), true);
	}
#endif//_NA00XXXX_090111_CASH_TYPE_SUBDIVIDE_

#endif	//	__CN_0_20070507_SHOP_MILEAGE

	if ( this->m_pItemUnitRenderer )
		this->m_pItemUnitRenderer->Update();
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopExtensionBuy::OnRenderSolarDialog()
{
	if (!_isEnable())
		return;

	if (m_pItemUnitRenderer)
		m_pItemUnitRenderer->Render();

	this->_renderIconImage();
	this->_renderTooltip();
}

//----------------------------------------------------------------------------
/**
*/
void uiCashItemShopExtensionBuy::MouseOver_MsgProc( SI_MESSAGE * pMessage )
{
	if (!_isEnable())
		return;

	switch( pMessage->eResultMsg ) 
	{
	case RT_MSG_MOUSEOVER:
		{
			POSTYPE pos = getCtrlIDToPos( pMessage->dwCtrlID );

			if (pos >= 0 && pos < GetMaxSlotNum())
			{
				if (!IsEmpty( pos  ))
				{
					SCSlot & rSlot = GetSlot( pos );
					m_MouseOverObject.eMouseOverType = eMOUSE_OVER_SLOT;
					m_MouseOverObject.pSlot = &rSlot;
				}
				else
				{
					m_MouseOverObject.eMouseOverType = eMOUSE_OVER_MAX;
				}
			}
			else
			{
				m_MouseOverObject.eMouseOverType = eMOUSE_OVER_MAX;
			}	
		}
		break;
	}
}

//----------------------------------------------------------------------------
/**
*/
RC::eSLOT_INSERT_RESULT		uiCashItemShopExtensionBuy::InsertSlot( POSTYPE AtPos, SCSlot & rSlot )
{
	assert(IsEmpty(AtPos));

	RC::eSLOT_INSERT_RESULT eInsertSlotResult = SCSlotContainer::InsertSlot( AtPos, rSlot );

	DBSERIAL dwSerial = g_SlotKeyGenerator.GetKey();
	SCSlot & rSetSlot = GetSlot(AtPos);
	rSetSlot.SetSerial(dwSerial);

	return eInsertSlotResult;
}

//----------------------------------------------------------------------------
/**
*/
BOOL	
uiCashItemShopExtensionBuy::DeleteSlot( POSTYPE AtPos, SCSlot * pSlotOut )
{
	if (IsEmpty(AtPos))
	{
		return FALSE;
	}

	SCItemSlot & rItemSlot = (SCItemSlot & )GetSlot(AtPos);

	if(m_pItemUnitRenderer)
	{
		m_pItemUnitRenderer->RemoveItemUnit( rItemSlot.GetSerial() );
	}
	else
	{
		assert(m_pItemUnitRenderer);
	}

	g_SlotKeyGenerator.Restore((DWORD)rItemSlot.GetSerial());

	return SCSlotContainer::DeleteSlot(AtPos, pSlotOut );
}

//----------------------------------------------------------------------------
/**
*/
void uiCashItemShopExtensionBuy::SetTargetInfo(SLOTIDX fromSlotIdx, POSTYPE fromPos)
{
	this->m_FromTargetPos = fromPos;
}


//------------------------------------------------------------------------------ 
#endif //#ifndef _NA_0_20110613_GAME_SHOP_RENEWAL //#if'N'def
//------------------------------------------------------------------------------ 

