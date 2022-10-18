#include "SunClientPrecompiledHeader.h"


//------------------------------------------------------------------------------ 
#ifndef _NA_0_20110613_GAME_SHOP_RENEWAL //#if'N'def
//------------------------------------------------------------------------------ 



#include "uiCashItemShopCart.h"
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
WzID uiCashItemShopCart::m_wzId[eDIALOG_CASH_ITEM_SHOP_CART_MAX] = 
{
        StrToWzID("B999"), //eDIALOG_CASH_ITEM_SHOP_CART_BTN_EXIT = 0,	

		StrToWzID("C001"), //eDIALOG_CASH_ITEM_SHOP_CART_CHK_CASH_ITEM,	
		StrToWzID("C002"), //eDIALOG_CASH_ITEM_SHOP_CART_CHK_MY_PAGE,	

		StrToWzID("C010"), //eDIALOG_CASH_ITEM_SHOP_CART_CHK_CART,	
		StrToWzID("C011"), //eDIALOG_CASH_ITEM_SHOP_CART_CHK_GIFT,		
		
		StrToWzID("B016"), //eDIALOG_CASH_ITEM_SHOP_CART_BTN_BUY1,	
		StrToWzID("B018"), //eDIALOG_CASH_ITEM_SHOP_CART_BTN_BUY2,	
		StrToWzID("B020"), //eDIALOG_CASH_ITEM_SHOP_CART_BTN_BUY3,	

		StrToWzID("B008"), //eDIALOG_CASH_ITEM_SHOP_CART_BTN_DEL1,	
		StrToWzID("B009"), //eDIALOG_CASH_ITEM_SHOP_CART_BTN_DEL2,	
		StrToWzID("B012"), //eDIALOG_CASH_ITEM_SHOP_CART_BTN_DEL3,	

		StrToWzID("B010"), //eDIALOG_CASH_ITEM_SHOP_CART_BTN_DEL_ALL,	
		StrToWzID("B013"), //eDIALOG_CASH_ITEM_SHOP_CART_BTN_BUY_SEL,	
		StrToWzID("B011"), //eDIALOG_CASH_ITEM_SHOP_CART_BTN_BUY_ALL,	

		StrToWzID("C003"), //eDIALOG_CASH_ITEM_SHOP_CART_CHK_SEL1,	
		StrToWzID("C004"), //eDIALOG_CASH_ITEM_SHOP_CART_CHK_SEL2,	
		StrToWzID("C005"), //eDIALOG_CASH_ITEM_SHOP_CART_CHK_SEL3,	

		StrToWzID("B001"), //eDIALOG_CASH_ITEM_SHOP_CART_IMG_ITEM1,	
		StrToWzID("B002"), //eDIALOG_CASH_ITEM_SHOP_CART_IMG_ITEM2,	
		StrToWzID("B003"), //eDIALOG_CASH_ITEM_SHOP_CART_IMG_ITEM3,	

		StrToWzID("T002"), //eDIALOG_CASH_ITEM_SHOP_CART_TXT_ITEM1,	
		StrToWzID("T003"), //eDIALOG_CASH_ITEM_SHOP_CART_TXT_ITEM2,	
		StrToWzID("T004"), //eDIALOG_CASH_ITEM_SHOP_CART_TXT_ITEM3,	

		StrToWzID("T080"), //eDIALOG_CASH_ITEM_SHOP_CART_TXT_MEMO_ITEM1,	
		StrToWzID("T081"), //eDIALOG_CASH_ITEM_SHOP_CART_TXT_MEMO_ITEM2,	
		StrToWzID("T082"), //eDIALOG_CASH_ITEM_SHOP_CART_TXT_MEMO_ITEM3,	
	      
		StrToWzID("T005"), //eDIALOG_CASH_ITEM_SHOP_CART_TXT_1_DESC1,	
		StrToWzID("T017"), //eDIALOG_CASH_ITEM_SHOP_CART_TXT_1_DESC2,	
		StrToWzID("T029"), //eDIALOG_CASH_ITEM_SHOP_CART_TXT_1_DESC3,	
		StrToWzID("T042"), //eDIALOG_CASH_ITEM_SHOP_CART_TXT_1_DESC4,	
		
		StrToWzID("T009"), //eDIALOG_CASH_ITEM_SHOP_CART_TXT_2_DESC1,	
		StrToWzID("T021"), //eDIALOG_CASH_ITEM_SHOP_CART_TXT_2_DESC2,	
		StrToWzID("T034"), //eDIALOG_CASH_ITEM_SHOP_CART_TXT_2_DESC3,	
		StrToWzID("T046"), //eDIALOG_CASH_ITEM_SHOP_CART_TXT_2_DESC4,	
		
		StrToWzID("T013"), //eDIALOG_CASH_ITEM_SHOP_CART_TXT_3_DESC1,
		StrToWzID("T025"), //eDIALOG_CASH_ITEM_SHOP_CART_TXT_3_DESC2,
		StrToWzID("T038"), //eDIALOG_CASH_ITEM_SHOP_CART_TXT_3_DESC3,
		StrToWzID("T050"), //eDIALOG_CASH_ITEM_SHOP_CART_TXT_3_DESC4,
		
		StrToWzID("T055"), //eDIALOG_CASH_ITEM_SHOP_CART_TXT_HEIM,	
		StrToWzID("T057"), //eDIALOG_CASH_ITEM_SHOP_CART_TXT_CASH,	

		StrToWzID("V000"), //eDIALOG_CASH_ITEM_SHOP_CART_VSCR,	
};

//------------------------------------------------------------------------------
/**
*/
uiCashItemShopCart::uiCashItemShopCart(InterfaceManager *pUIMan) :
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
    ZeroMemory(&m_CartItemImg, sizeof(m_CartItemImg));
	this->m_iCurMenu = eCASH_ITEM_SHOP_CART_CHK_CART;
	ZeroMemory(&m_ItemCodeForTooltip, sizeof(m_ItemCodeForTooltip));
}

//------------------------------------------------------------------------------
/**
*/
uiCashItemShopCart::~uiCashItemShopCart()
{
    this->Release();
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopCart::Init(CDrawBase* pDrawBase)
{
    uiBase::Init(pDrawBase);

    this->InitControls();
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopCart::Release()
{
	this->_removeAllIconImage();
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopCart::InitControls()
{
    int i=0;
	for(i=0; i<eCASH_ITEM_SHOP_CART_CHK_MAX; ++i)
	{
		m_pBtnCheckTabs[i] = static_cast<CCtrlButtonCheckWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_CART_CHK_CASH_ITEM+i));
		assert(m_pBtnCheckTabs[i]);
	}

	for(i=0; i<eCASH_ITEM_SHOP_CART_CHK_MENU_MAX; ++i)
	{
		m_pBtnCheckMenu[i] = static_cast<CCtrlButtonCheckWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_CART_CHK_CART+i));
		assert(m_pBtnCheckMenu[i]);
	}

	for(i=0; i<eCASH_ITEM_SHOP_CART_ITEM_MAX; ++i)
	{
		m_pImgCtrl[i] = static_cast<CCtrlImageStaticWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_CART_IMG_ITEM1+i));
		assert(m_pImgCtrl[i]);

		m_pBtnCheckSel[i] = static_cast<CCtrlButtonCheckWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_CART_CHK_SEL1+i));
		assert(m_pBtnCheckSel[i]);

		m_pTxtCtrlName[i] = static_cast<CCtrlStaticWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_CART_TXT_ITEM1+i));
		assert(m_pTxtCtrlName[i]);

		m_pTxtCtrlMemo[i] = static_cast<CCtrlStaticWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_CART_TXT_MEMO_ITEM1+i));
		assert(m_pTxtCtrlMemo[i]);
	}

	
	for(i=0; i<eCASH_ITEM_SHOP_CART_TXT_DESC_MAX; ++i)
	{
		m_pTxtCtrlDesc1[i] = static_cast<CCtrlStaticWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_CART_TXT_1_DESC1+i));
		assert(m_pTxtCtrlDesc1[i]);

		m_pTxtCtrlDesc2[i] = static_cast<CCtrlStaticWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_CART_TXT_2_DESC1+i));
		assert(m_pTxtCtrlDesc2[i]);

		m_pTxtCtrlDesc3[i] = static_cast<CCtrlStaticWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_CART_TXT_3_DESC1+i));
		assert(m_pTxtCtrlDesc3[i]);
	}

	int iIndex = 0;
	for(i=0; i<eCASH_ITEM_SHOP_CART_ITEM_MAX; ++i)
	{
		iIndex = i * _MAX_PRICE_INFO;
		m_pTxtCtrlSale[i] = static_cast<CCtrlStaticWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_CART_TXT_1_DESC4+iIndex));
		assert(m_pTxtCtrlSale[i]);
	}
	
	m_pTxtCtrlHeim = static_cast<CCtrlStaticWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_CART_TXT_HEIM));
	assert(m_pTxtCtrlHeim);

	m_pTxtCtrlCash = static_cast<CCtrlStaticWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_CART_TXT_CASH));
	assert(m_pTxtCtrlCash);
	
	m_pScroll = static_cast<CCtrlVScrollWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_CART_VSCR));
	assert(m_pScroll);

	//TODO: 현재는 장바구니 기능이 없으므로 버튼을 숨긴다
    if(m_pBtnCheckMenu[eCASH_ITEM_SHOP_CART_CHK_GIFT])
		m_pBtnCheckMenu[eCASH_ITEM_SHOP_CART_CHK_GIFT]->ShowWindowWZ(WZ_HIDE);

    this->m_bControlsInitialized = true;
}


//------------------------------------------------------------------------------
void uiCashItemShopCart::OnShowWindow( BOOL val )
{
    if (val)
    {
		 this->MoveDefaultWindowPos();

        if (!this->m_bControlsInitialized)
        {
            this->ShowInterface(FALSE);
            return;
        }
    
        keyMsg msg;
        ZeroMemory(&msg,sizeof(msg));
        msg.dwType = eDoType_dialog_cash_item_shop_cart;
        msg.wParam = InterfaceManager::DIALOG_CASH_ITEM_SHOP_CART;
        msg.DoSomething = GlobalFunc::DoShowWindow;
        g_KeyQueueManager.PushBack(msg);

		m_bShow = true;

		_prepareShow();
	}
    else
    {
        g_KeyQueueManager.DeleteMsg(eDoType_dialog_cash_item_shop_cart);
		_removeAllIconImage();

    }
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopCart::_setMenu(int iMenu)
{
	m_iCurMenu = iMenu;
	_updateMenu();
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopCart::_updateMenu()
{
	for(int i=0; i<eCASH_ITEM_SHOP_CART_CHK_MENU_MAX; ++i)
		m_pBtnCheckMenu[i]->SetCheckState(false);

	if(m_pBtnCheckMenu[m_iCurMenu])
		m_pBtnCheckMenu[m_iCurMenu]->SetCheckState(true);
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopCart::_prepareShow()
{
	GetManager()->ShowCashItemShopNpcPopup(false);
	GetManager()->ShowCashItemShopDlg(false);

	if (!_isEnable())
		return;

	if(m_pScroll)
		m_pScroll->SetScrollPosWZ(0);

	if(m_pTxtCtrlHeim)
	{
		GlobalFunc::SetMoneyText(m_pTxtCtrlHeim, g_pHero->GetMoney());
	}

	if(m_pTxtCtrlCash)
	{
		// TODO: 캐쉬 정보 세팅
		GlobalFunc::SetMoneyText(m_pTxtCtrlCash, 0);
	}

	_clearAllItem();
	_setMenu(eCASH_ITEM_SHOP_CART_CHK_CART);

	ZeroMemory(&m_ItemCodeForTooltip, sizeof(m_ItemCodeForTooltip));
}


//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopCart::_clearItem(int index)
{
	this->_removeIconImage(index);
	
	GlobalFunc::SetCaption(m_pTxtCtrlName[index], _T(""));	// 아이템 이름
	GlobalFunc::SetCaption(m_pTxtCtrlSale[index], _T(""));	// 세일!! %d
	GlobalFunc::SetCaption(m_pTxtCtrlMemo[index], _T(""));

	for(int k=0; k<eCASH_ITEM_SHOP_CART_TXT_DESC_MAX; ++k)
	{
		if(index == 0)
		{
			GlobalFunc::SetCaption(m_pTxtCtrlDesc1[k], _T(""));
		}
		else if(index == 1)
		{
			GlobalFunc::SetCaption(m_pTxtCtrlDesc2[k], _T(""));
		}
		else
		{
			GlobalFunc::SetCaption(m_pTxtCtrlDesc3[k], _T(""));
		}
	}
}


//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopCart::_clearAllItem()
{
	for(int i=0; i<eCASH_ITEM_SHOP_CART_ITEM_MAX; ++i)
	{
		_clearItem(i);
	}
}


//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopCart::_setItem(int index, CODETYPE itemCode)
{
	BASE_ITEMINFO * pItemInfo = ItemInfoParser::Instance()->GetItemInfo( itemCode );
	if ( !pItemInfo ) 
	{
		return;
	}
    m_CartItemImg[index].type = GetManager()->m_stCartInfo[index].byRank;
	_addIconImage(index, pItemInfo->m_wVICode, itemCode);

	TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szTemp2[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szTemp3[INTERFACE_STRING_LENGTH] = {0,};

	g_InterfaceManager.GetItemName(itemCode, szMessage, INTERFACE_STRING_LENGTH);
	GlobalFunc::SetCaption(m_pTxtCtrlName[index], szMessage);	// 아이템 이름


	if(GetManager()->m_stCartInfo[index].byEnchant)
	{
		Snprintf(szTemp, INTERFACE_STRING_LENGTH-1, _T("+%d"), GetManager()->m_stCartInfo[index].byEnchant);
		_tcscat(szMessage, szTemp);
	}

    // 캐시샵아이템의 Rank 값을 용도 변경하여 Rank를 출력 하지 않도록 수정
    // 이전부터 Rank자체는 사용되고 있지 않았음.
	//if(pItemInfo->CanRankUpItem())
	//{
	//	int iRankIndex = eST_RANK_E_STRING_INDEX  + (GetManager()->m_stCartInfo[index].byRank);
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
}


//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopCart::_addIconImage(int iIndex, DWORD dwCode, CODETYPE itemCode)
{
	m_CartItemImg[iIndex].dwIconCode	= dwCode;
	m_CartItemImg[iIndex].itemCode		= itemCode;
	
	_removeIconImage(iIndex);
	
	if (dwCode != 0)
	{
		m_CartItemImg[iIndex].pImage  = ICONMGR()->Add(dwCode);
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopCart::_removeIconImage(int iIndex)
{
	if(m_CartItemImg[iIndex].pImage)
	{
		ICONMGR()->RemoveIcon(m_CartItemImg[iIndex].pImage);
		m_CartItemImg[iIndex].pImage = NULL;
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopCart::_removeAllIconImage()
{
	for(int i=0; i<eCASH_ITEM_SHOP_CART_ITEM_MAX;  ++i)
	{
		_removeIconImage(i);
	}
}

//------------------------------------------------------------------------------
/**
*/
BOOL uiCashItemShopCart::_isEnable()
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
void uiCashItemShopCart::MessageProc( SI_MESSAGE * pMessage )
{
    if (!_isEnable())
        return;

	switch( pMessage->eResultMsg ) 
    {
    case RT_MSG_LBUTTONCLICK:
        {
            switch( getCtrlIDToPos( pMessage->dwCtrlID ) )
            {
            case eDIALOG_CASH_ITEM_SHOP_CART_BTN_EXIT:
                {
                    m_bShow = false;

                }
                break;

			case eDIALOG_CASH_ITEM_SHOP_CART_CHK_CASH_ITEM:
				{
					GetManager()->ShowCashItemShopDlg(true);
				}
				break;
            }
        }
        break;

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


				}
			}
		}
		break;

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
				}
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
void uiCashItemShopCart::OnUpdateSolarDialog()
{
	if(!m_bShow)
		this->ShowInterface(FALSE);

    if (!_isEnable())
        return;

	if(m_pBtnCheckTabs[eCASH_ITEM_SHOP_CART_CHK_CASH_ITEM])
		m_pBtnCheckTabs[eCASH_ITEM_SHOP_CART_CHK_CASH_ITEM]->SetCheckState(FALSE);

	if(m_pBtnCheckTabs[eCASH_ITEM_SHOP_CART_CHK_MY_PAGE])
		m_pBtnCheckTabs[eCASH_ITEM_SHOP_CART_CHK_MY_PAGE]->SetCheckState(TRUE);

	_updateMenu();
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopCart::OnRenderSolarDialog()
{
	if (!_isEnable())
        return;

	_renderIconImage();
	_renderTooltip();
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopCart::_renderIconImage()
{
	for(int iIndex=0; iIndex<eCASH_ITEM_SHOP_CART_ITEM_MAX;  ++iIndex)
	{
		if(m_pImgCtrl[iIndex])
		{
			if (m_CartItemImg[iIndex].pImage && m_CartItemImg[iIndex].dwIconCode)
			{
				RECT rc = m_pImgCtrl[iIndex]->GetSizeRect();
				m_CartItemImg[iIndex].pImage->Render(&rc);
                GetManager()->renderCoverImage(m_CartItemImg[iIndex], rc);
			}
		}
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopCart::_renderTooltip()
{
	if( m_ItemCodeForTooltip.dwItemCode > 0)
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
	for (POSTYPE a = eDIALOG_CASH_ITEM_SHOP_CART_IMG_ITEM1; a <= eDIALOG_CASH_ITEM_SHOP_CART_IMG_ITEM3; a++)
	{
		CControlWZ * pControl = getControl(a);

		if(pControl)
		{
			RECT rc = pControl->GetSizeRect();

			if (rc.left <= iMouseX && rc.right >= iMouseX)
			{
				if (rc.top <= iMouseY && rc.bottom >= iMouseY)
				{
					bMouseOver = TRUE;
				}
			}
		}
	}

	if ( !bMouseOver )
	{
		ZeroMemory(&m_ItemCodeForTooltip, sizeof(m_ItemCodeForTooltip));
	}
}

//----------------------------------------------------------------------------
/**
*/
void uiCashItemShopCart::MouseOver_MsgProc( SI_MESSAGE * pMessage )
{
	switch( pMessage->eResultMsg ) 
	{
	case RT_MSG_MOUSEOVER:
		{
			POSTYPE pos = getCtrlIDToPos( pMessage->dwCtrlID );
			
			if (pos >= eDIALOG_CASH_ITEM_SHOP_CART_IMG_ITEM1 && pos <= eDIALOG_CASH_ITEM_SHOP_CART_IMG_ITEM3)
			{
				int iIndex = pos - eDIALOG_CASH_ITEM_SHOP_CART_IMG_ITEM1;
				if (m_CartItemImg[iIndex].pImage && m_CartItemImg[iIndex].dwIconCode)
				{
					m_ItemCodeForTooltip.dwItemCode = m_CartItemImg[iIndex].itemCode;
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


//------------------------------------------------------------------------------ 
#endif //#ifndef _NA_0_20110613_GAME_SHOP_RENEWAL //#if'N'def
//------------------------------------------------------------------------------ 

