#include "SunClientPrecompiledHeader.h"

#ifdef _KTW_CASH_ITEM_SHOP_PRESENT

#include "uiCashItemShopPresentDlg.h"
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
WzID uiCashItemShopPresentDlg::m_wzId[eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_MAX] = 
{
	StrToWzID("B999"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_BTN_EXIT = 0,	//	X ��ư

	StrToWzID("B001"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_IMG_ITEM1,		//	������ �̹���1
	StrToWzID("TI02"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_IMG_ITEM2,		//	������ �̹���2
	StrToWzID("B002"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_IMG_ITEM3,		//	������ �̹���3
	StrToWzID("TI01"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_IMG_ITEM4,		//	������ �̹���4
	StrToWzID("B003"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_IMG_ITEM5,		//	������ �̹���5
	StrToWzID("TI00"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_IMG_ITEM6,		//	������ �̹���6

	StrToWzID("C001"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_CHK_CASH,		//	ĳ�þ����� üũ��ư
	StrToWzID("C002"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_CHK_MYPAGE,		//	���������� üũ��ư
	StrToWzID("CT00"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_CHK_PACKAGE,		//	��Ű���� üũ��ư

	StrToWzID("C005"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_CHK_BASKET,		//	��ٱ��� üũ��ư
	StrToWzID("C006"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_CHK_PRESENT,		//	���� ������ üũ��ư

	StrToWzID("T002"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMNAME1,	//	������ �̸�1
	StrToWzID("ST00"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMNAME2,	//	������ �̸�2
	StrToWzID("T003"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMNAME3,	//	������ �̸�3
	StrToWzID("ST02"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMNAME4,	//	������ �̸�4
	StrToWzID("T004"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMNAME5,	//	������ �̸�5
	StrToWzID("ST17"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMNAME6,	//	������ �̸�6

	StrToWzID("T080"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMDESC1,	//	������ ���ܼ���1
	StrToWzID("ST01"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMDESC2,	//	������ ���ܼ���2
	StrToWzID("T081"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMDESC3,	//	������ ���ܼ���3
	StrToWzID("ST03"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMDESC4,	//	������ ���ܼ���4
	StrToWzID("T082"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMDESC5,	//	������ ���ܼ���5
	StrToWzID("ST18"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMDESC6,	//	������ ���ܼ���6

	StrToWzID("T006"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_PLAYERNAME1,	//	������ �÷��̾� �̸�1
	StrToWzID("ST22"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_PLAYERNAME2,	//	������ �÷��̾� �̸�2
	StrToWzID("T014"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_PLAYERNAME3,	//	������ �÷��̾� �̸�3
	StrToWzID("ST23"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_PLAYERNAME4,	//	������ �÷��̾� �̸�4
	StrToWzID("T022"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_PLAYERNAME5,	//	������ �÷��̾� �̸�5
	StrToWzID("ST27"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_PLAYERNAME6,	//	������ �÷��̾� �̸�6
	

	StrToWzID("T008"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMSENDDAY1,//	������ ��¥1
	StrToWzID("ST21"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMSENDDAY2,//	������ ��¥2
	StrToWzID("T016"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMSENDDAY3,//	������ ��¥3
	StrToWzID("ST24"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMSENDDAY4,//	������ ��¥4
	StrToWzID("T024"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMSENDDAY5,//	������ ��¥5
	StrToWzID("ST28"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMSENDDAY6,//	������ ��¥6

	StrToWzID("T010"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMUSETIME1,//	���� ���Ⱓ1
	StrToWzID("ST20"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMUSETIME2,//	���� ���Ⱓ2
	StrToWzID("T018"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMUSETIME3,//	���� ���Ⱓ3
	StrToWzID("ST25"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMUSETIME4,//	���� ���Ⱓ4
	StrToWzID("T026"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMUSETIME5,//	���� ���Ⱓ5
	StrToWzID("ST29"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMUSETIME6,//	���� ���Ⱓ6

	StrToWzID("T012"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMCOUNT1,	//	���� ����1
	StrToWzID("ST19"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMCOUNT1,	//	���� ����1
	StrToWzID("ST04"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMCOUNT2,	//	���� ����2
	StrToWzID("ST26"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMCOUNT2,	//	���� ����2
	StrToWzID("T028"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMCOUNT3,	//	���� ����3
	StrToWzID("ST30"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMCOUNT3,	//	���� ����3

	StrToWzID("B008"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_BTN_PRESENTLISTDEL1,	//	��� ���� ��ư1
	StrToWzID("TI09"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_BTN_PRESENTLISTDEL2,	//	��� ���� ��ư2
	StrToWzID("B009"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_BTN_PRESENTLISTDEL3,	//	��� ���� ��ư3
	StrToWzID("TI10"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_BTN_PRESENTLISTDEL4,	//	��� ���� ��ư4
	StrToWzID("B010"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_BTN_PRESENTLISTDEL5,	//	��� ���� ��ư5
	StrToWzID("TI11"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_BTN_PRESENTLISTDEL6,	//	��� ���� ��ư6

	StrToWzID("B016"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_BTN_PRESENTLISTCHECK1,//	���� Ȯ�� ��ư1
	StrToWzID("TI03"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_BTN_PRESENTLISTCHECK2,//	���� Ȯ�� ��ư2
	StrToWzID("B018"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_BTN_PRESENTLISTCHECK3,//	���� Ȯ�� ��ư3
	StrToWzID("TI06"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_BTN_PRESENTLISTCHECK4,//	���� Ȯ�� ��ư4
	StrToWzID("B020"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_BTN_PRESENTLISTCHECK5,//	���� Ȯ�� ��ư5
	StrToWzID("TI07"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_BTN_PRESENTLISTCHECK6,//	���� Ȯ�� ��ư6

	StrToWzID("T057"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_MONEY,			//	������

	StrToWzID("BT01"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_BTN_PREV_PAGE,		//	���� ������ ��ư	
	StrToWzID("BT00"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_BTN_NEXT_PAGE,		//	���� ������ ��ư

	StrToWzID("ST31"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_CURPAGE,			//	����������
};


//------------------------------------------------------------------------------
/**
*/
uiCashItemShopPresentDlg::uiCashItemShopPresentDlg(InterfaceManager *pUIMan) :
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
			pSystemMan->Sys_VerifyLock("m_pCashItemShopMan�� �����ϴ�.", FALSE);
#endif
		this->ShowInterface(FALSE);
		return;
	}

	m_bShow = false;
	ZeroMemory(&m_CashItemImg, sizeof(m_CashItemImg));
	m_iTotalPage = 0;
//	m_iCurMenu = -1;
	m_iCurPage = -1;
	m_iCurIndex = -1;

	ZeroMemory(&m_ItemCodeForTooltip, sizeof(m_ItemCodeForTooltip));
}

//------------------------------------------------------------------------------
/**
*/
uiCashItemShopPresentDlg::~uiCashItemShopPresentDlg()
{
	this->Release();
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopPresentDlg::Init(CDrawBase* pDrawBase)
{
	uiBase::Init(pDrawBase);

	this->InitControls();
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopPresentDlg::Release()
{
	_removeAllIconImage();
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopPresentDlg::InitControls()
{
	int i;
	for(i=0; i<eCASH_ITEM_SHOP_PRESENT_DLG_CHK_MAX; ++i)
	{
		m_pBtnCheckTabs[i] = static_cast<CCtrlButtonCheckWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_IMG_ITEM1+i));
		assert(m_pBtnCheckTabs[i]);
	}

	if(m_pBtnCheckTabs[eCASH_ITEM_SHOP_PRESENT_DLG_CHK_PACKAGE])
		m_pBtnCheckTabs[eCASH_ITEM_SHOP_PRESENT_DLG_CHK_PACKAGE]->ShowWindowWZ(WZ_HIDE);

	for(i=0; i<eCASH_ITEM_SHOP_PRESENT_DLG_CHK_MENU_MAX; ++i)
	{
		m_pBtnCheckMenu[i] = static_cast<CCtrlButtonCheckWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_CHK_BASKET+i));
		assert(m_pBtnCheckMenu[i]);
	}

	for(i=0; i<eCASH_ITEM_SHOP_PRESENT_DLG_ITEM_MAX; ++i)
	{
		m_pImgCtrl[i] = static_cast<CCtrlImageStaticWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_IMG_ITEM1+i));
		assert(m_pImgCtrl[i]);

		m_pTxtCtrlName[i] = static_cast<CCtrlStaticWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMNAME1+i));
		assert(m_pTxtCtrlName[i]);

		m_pTxtCtrlMemo[i] = static_cast<CCtrlStaticWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMDESC1+i));
		assert(m_pTxtCtrlMemo[i]);

		m_pTxtCtrlPlayerName[i] = static_cast<CCtrlStaticWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_PLAYERNAME1+i));
		assert(m_pTxtCtrlPlayerName[i]);

		m_pTxtCtrlSendDay[i] = static_cast<CCtrlStaticWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMSENDDAY1+i));
		assert(m_pTxtCtrlSendDay[i]);

		m_pTxtCtrlUseTime[i] = static_cast<CCtrlStaticWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMUSETIME1+i));
		assert(m_pTxtCtrlUseTime[i]);

		m_pTxtCtrlItemCount[i] = static_cast<CCtrlStaticWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMCOUNT1+i));
		assert(m_pTxtCtrlItemCount[i]);

		m_pBtnPresentListDel[i] = static_cast<CCtrlButtonWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_BTN_PRESENTLISTDEL1+i));
		assert(m_pBtnPresentListDel[i]);

		m_pBtnPresentListCheck[i] = static_cast<CCtrlButtonWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_BTN_PRESENTLISTCHECK1+i));
		assert(m_pBtnPresentListCheck[i]);
	}

	m_pTxtCtrlMoney = static_cast<CCtrlStaticWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_MONEY));
	assert(m_pTxtCtrlMoney);

	m_pTxtCtrlCurPage = static_cast<CCtrlStaticWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_CURPAGE));
	assert(m_pTxtCtrlCurPage);

	this->m_bControlsInitialized = true;
}


//------------------------------------------------------------------------------
/**
*/
BOOL uiCashItemShopPresentDlg::_isEnable()
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
void uiCashItemShopPresentDlg::_setIndex(int iIndex)
{
	m_iCurIndex = iIndex;

	// �ش� ������ ���� ������ ������ Show, ������ �������� ��û
	/*

	if(GetManager()->m_stPresentItemInfo[m_iCurIndex].stPrice[0].priceGuid > 0)
		GetManager()->ShowCashItemShopBuy(m_iCurIndex);							
	else
		GetManager()->Send_CG_SHOP_ITEM_REQ_SYN(m_iCurIndex);

	*/
}

//------------------------------------------------------------------------------
/** ������ ������ ���� ����
*/
void uiCashItemShopPresentDlg::_setPage(int iPage)
{
	if(m_iCurPage == iPage)
		return;

	m_iCurPage = iPage;

//	if(!GetManager()->IsRcvShopInfo(m_iCurPage))
	{
//		GetManager()->Send_CG_SHOP_ITEM_LIST_REQ_SYN(byCategory, m_iCurPage);
	}
//	else
	{
//		GetManager()->RefreshPresentList();
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopPresentDlg::_prepareShow()
{
	if (!_isEnable())
		return;

	GetManager()->PrepareShow(eCashItemShopDlgState_Present);

	ZeroMemory(&m_ItemCodeForTooltip, sizeof(m_ItemCodeForTooltip));
	_clearAllItem();

	m_iCurPage = -1;
	m_iCurIndex = -1;

	_setPage();
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopPresentDlg::_clearItem(int index)
{
	this->_removeIconImage(index);

	GlobalFunc::SetCaption(m_pTxtCtrlName[index], _T(""));			// ������ �̸�
	GlobalFunc::SetCaption(m_pTxtCtrlMemo[index], _T(""));			// ���� ����
	GlobalFunc::SetCaption(m_pTxtCtrlPlayerName[index], _T(""));	// ������ �÷��̾� �̸�
	GlobalFunc::SetCaption(m_pTxtCtrlSendDay[index], _T(""));		// ���� ���� �Ͻ�
	GlobalFunc::SetCaption(m_pTxtCtrlUseTime[index], _T(""));		// ���� ��� �Ⱓ
	GlobalFunc::SetCaption(m_pTxtCtrlItemCount[index], _T(""));		// ���� ����
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopPresentDlg::_clearAllItem()
{
	for(int i=0; i<eCASH_ITEM_SHOP_PRESENT_DLG_ITEM_MAX; ++i)
	{
		_clearItem(i);
	}
	GlobalFunc::SetCaption(m_pTxtCtrlMoney, _T(""));	// ���� ���� money
	GlobalFunc::SetCaption(m_pTxtCtrlCurPage, _T(""));  // ���� ������
	m_iTotalPage = 0;
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopPresentDlg::SetItem(int index, CODETYPE itemCode)
{
	_setItem(index, itemCode);
}

void uiCashItemShopPresentDlg::_setItem(int index, CODETYPE itemCode)
{
	BASE_ITEMINFO * pItemInfo = ItemInfoParser::Instance()->GetItemInfo( itemCode );
	if ( !pItemInfo ) 
	{
		return;
	}

	_addIconImage(index, pItemInfo->m_wVICode, itemCode);

	TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
	g_InterfaceManager.GetItemName(itemCode, szMessage, INTERFACE_STRING_LENGTH);
	GlobalFunc::SetCaption(m_pTxtCtrlName[index], szMessage);	// ������ �̸�

	// ���� ���� 
	if(pItemInfo->m_TDNCode)
	{
		g_InterfaceManager.GetInterfaceString(pItemInfo->m_TDNCode,szMessage,INTERFACE_STRING_LENGTH);
		GlobalFunc::SetCaptionSimple(m_pTxtCtrlMemo[index], szMessage);
	}

	// ������ �÷��̾� �̸�
	if(GetManager()->m_stPresentItemInfo[index].strPlayerName)
	{
		ZeroMemory(szMessage, sizeof(szMessage));
		Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, _T("�� �� �� : %s"), GetManager()->m_stPresentItemInfo[index].strPlayerName);
		GlobalFunc::SetCaption(m_pTxtCtrlPlayerName[index], szMessage);
	}

	// ���� �����Ͻ�
	// ���� ���Ⱓ
	// ���� ��    ��
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopPresentDlg::_removeAllIconImage()
{
	for(int i=0; i<eCASH_ITEM_SHOP_PRESENT_DLG_ITEM_MAX;  ++i)
	{
		_removeIconImage(i);
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopPresentDlg::_addIconImage(int iIndex, DWORD dwCode, CODETYPE itemCode)
{
	m_CashItemImg[iIndex].dwIconCode = dwCode;
	m_CashItemImg[iIndex].itemCode = itemCode;
	_removeIconImage(iIndex);

	if (dwCode != 0)
	{
		m_CashItemImg[iIndex].pImage  = ICONMGR()->Add(dwCode);
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopPresentDlg::_removeIconImage(int iIndex)
{
	if(m_CashItemImg[iIndex].pImage)
	{
		ICONMGR()->RemoveIcon(m_CashItemImg[iIndex].pImage);
		ZeroMemory(&m_CashItemImg, sizeof(m_CashItemImg));
	}
}


//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopPresentDlg::_renderIconImage()
{
	for(int iIndex=0; iIndex < eCASH_ITEM_SHOP_PRESENT_DLG_ITEM_MAX;  ++iIndex)
	{
		if(m_pImgCtrl[iIndex])
		{
			if (m_CashItemImg[iIndex].pImage && m_CashItemImg[iIndex].dwIconCode)
			{
				RECT rc = m_pImgCtrl[iIndex]->GetSizeRect();
				m_CashItemImg[iIndex].pImage->Render(&rc);
			}
		}
	}
}


//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopPresentDlg::_renderTooltip()
{
	if(this->m_ItemCodeForTooltip.dwItemCode >= 0)
	{
		uiToolTipMan* pUIMan = (uiToolTipMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_TOOLTIP);
		if(pUIMan)
			pUIMan->RegisterTooltipItemForCashShop(m_ItemCodeForTooltip.dwItemCode, m_ItemCodeForTooltip.dwCoolTime, m_ItemCodeForTooltip.SellEndDate);
	}

	int iMouseX = Mouse::Instance()->GetMouseX();
	int iMouseY = Mouse::Instance()->GetMouseY();

	// ��ŵó�� 
	// �������̽����� Dialog ���������� Mouse Over�� ���� �ʴ´�.
	BOOL bMouseOver = FALSE;
	for (POSTYPE a = eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_IMG_ITEM1; a <= eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_IMG_ITEM3; a++)
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

void uiCashItemShopPresentDlg::SetCurPage()
{
	TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
	Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, _T("%d ������"), m_iCurPage + 1);
	GlobalFunc::SetCaption(m_pTxtCtrlCurPage, szMessage);	// ���� ������
}


//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopPresentDlg::showWindow( BOOL val )
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
		msg.dwType = eDoType_dialog_cash_item_shop_present;
		msg.wParam = InterfaceManager::DIALOG_CASH_ITEM_SHOP_PRESENT_DLG;
		msg.DoSomething = GlobalFunc::DoShowWindow;
		g_KeyQueueManager.PushBack(msg);

		m_bShow = true;

		_prepareShow();
	}
	else
	{
		g_KeyQueueManager.DeleteMsg(eDoType_dialog_cash_item_shop_present);
		_removeAllIconImage();
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopPresentDlg::update()
{
	if(!m_bShow)
		this->ShowInterface(FALSE);

	if (!_isEnable())
		return;

	if(m_pBtnCheckTabs[eCASH_ITEM_SHOP_PRESENT_DLG_CHK_CASH_ITEM])
		m_pBtnCheckTabs[eCASH_ITEM_SHOP_PRESENT_DLG_CHK_CASH_ITEM]->SetCheckState(TRUE);

	if(m_pBtnCheckTabs[eCASH_ITEM_SHOP_PRESENT_DLG_CHK_MY_PAGE])
		m_pBtnCheckTabs[eCASH_ITEM_SHOP_PRESENT_DLG_CHK_MY_PAGE]->SetCheckState(FALSE);

	if(m_pBtnCheckTabs[eCASH_ITEM_SHOP_PRESENT_DLG_CHK_PACKAGE])
		m_pBtnCheckTabs[eCASH_ITEM_SHOP_PRESENT_DLG_CHK_PACKAGE]->SetCheckState(FALSE);


#ifdef _SHOW_HEIM_IN_TESTLIVE_CASH_ITEM_SHOP
	GlobalFunc::SetMoneyText(m_pTxtCtrlMoney, g_pHero->GetMoney(), false);
#else
	GlobalFunc::SetMoneyText(m_pTxtCtrlMoney, GetManager()->GetMyCash(), true);
#endif
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopPresentDlg::render()
{
	if (!_isEnable())
		return;

	_renderIconImage();
	_renderTooltip();
}


//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopPresentDlg::MessageProc( SI_MESSAGE * pMessage )
{
	if (!_isEnable())
		return;

	switch( pMessage->eResultMsg ) 
	{
	case RT_MSG_LBUTTONDOWN:
		{
			int	iCtrlMenu = getCtrlIDToPos( pMessage->dwCtrlID );
			switch( iCtrlMenu )
			{
			case eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_BTN_EXIT:
				{
					m_bShow = false;
				}
				break;

			case eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_CHK_CASH:
				{
					GetManager()->ShowCashItemShopDlg(true);
				}
				break;

			case eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_BTN_PREV_PAGE:
				{
					int iPage = m_iCurPage;
					if( iPage > 0)
					{
						iPage--;
						_setPage(iPage);
					}
				}
				break;
			case eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_BTN_NEXT_PAGE:
				{
					int iPage = m_iCurPage;
					if(iPage < m_iTotalPage)
					{
						iPage++;
						_setPage(iPage);
					}
				}
				break;

			}
		}
	default:
		{
			SolarDialog::MessageProc( pMessage );
		}
		break;
	}
}

//----------------------------------------------------------------------------
/**
*/
void uiCashItemShopPresentDlg::MouseOver_MsgProc( SI_MESSAGE * pMessage )
{
	if (!_isEnable())
		return;

	switch( pMessage->eResultMsg ) 
	{
	case RT_MSG_MOUSEOVER:
		{
			POSTYPE pos = getCtrlIDToPos( pMessage->dwCtrlID );

			if (pos >= eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_IMG_ITEM1 && pos <= eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_IMG_ITEM3)
			{
				int iIndex = pos - eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_IMG_ITEM1;
				if (m_CashItemImg[iIndex].pImage && m_CashItemImg[iIndex].dwIconCode)
				{
					m_ItemCodeForTooltip.dwItemCode = m_CashItemImg[iIndex].itemCode;
//					m_ItemCodeForTooltip.dwCoolTime = GetManager()->m_stCashItemInfo[iIndex].dwCoolTime;
//					m_ItemCodeForTooltip.SellEndDate = GetManager()->m_stCashItemInfo[iIndex].SellEndDate;
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


#endif	//	_KTW_CASH_ITEM_SHOP_PRESENT