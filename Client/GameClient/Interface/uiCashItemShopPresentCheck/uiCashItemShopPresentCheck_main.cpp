#include "SunClientPrecompiledHeader.h"

//------------------------------------------------------------------------------ 
#ifndef _NA_0_20110613_GAME_SHOP_RENEWAL //#if'N'def
//------------------------------------------------------------------------------ 


#include "uiCashItemShopPresentCheck.h"
#include "uiCashItemShopMan/uiCashItemShopMan.h"
#include "uiSystemMan/uiSystemMan.h"
#include "Hero.h"
#include "GlobalFunc.h"
#include "interfacemanager.h"
#include "GameFramework.h"
#include "Mouse.h"
#include "ItemInfoParser.h"
#include "uiToolTipMan/uiToolTipMan.h"
#include "GameConst_Color.h"


WzID uiCashItemShopPresentCheck::m_wzId[eDIALOG_CASH_ITEM_SHOP_PRESENT_CHECK_MAX] = 
{
	StrToWzID("B999"),	//eDIALOG_CASH_ITEM_SHOP_PRESENT_CHECK_BTN_EXIT,			// X ��ư

	StrToWzID("BT00"),	//eDIALOG_CASH_ITEM_SHOP_PRESENT_CHECK_IMG_ITEM,			// ���� �̹���

	StrToWzID("T000"),	//eDIALOG_CASH_ITEM_SHOP_PRESENT_CHECK_TXT_PRESENT_SEND,	// ���������� �ؽ�Ʈ
	StrToWzID("ST00"),	//eDIALOG_CASH_ITEM_SHOP_PRESENT_CHECK_TXT_PRESENT_REV,		// ����Ȯ�� �ؽ�Ʈ

	StrToWzID("T001"),	//eDIALOG_CASH_ITEM_SHOP_PRESENT_CHECK_TXT_PRESENT_NAME,	// �����̸� �ؽ�Ʈ

	StrToWzID("T002"),	//eDIALOG_CASH_ITEM_SHOP_PRESENT_CHECK_TXT_USETIME,			// ���� ���Ⱓ
	StrToWzID("T003"),	//eDIALOG_CASH_ITEM_SHOP_PRESENT_CHECK_TXT_MONEY,			// ���� ����
	StrToWzID("T004"),	//eDIALOG_CASH_ITEM_SHOP_PRESENT_CHECK_TXT_COUNT,			// ���� ����

	StrToWzID("E001"),	//eDIALOG_CASH_ITEM_SHOP_PRESENT_CHECK_EDIT_PLAYERNAME,		// ������ �÷��̾��̸�
	StrToWzID("ST01"),	//eDIALOG_CASH_ITEM_SHOP_PRESENT_CHECK_TXT_PLAYERNAME,		// ������ �÷��̾��̸�

	StrToWzID("T007"),	//eDIALOG_CASH_ITEM_SHOP_PRESENT_CHECK_TXT_NAME_MESSAGE1,	// "�Կ��� ������ �����ϴ�" �޼���
	StrToWzID("ST03"),	//eDIALOG_CASH_ITEM_SHOP_PRESENT_CHECK_TXT_NAME_MESSAGE1,		// "���� ���� �����Դϴ�" �޽���

	StrToWzID("T008"),	//eDIALOG_CASH_ITEM_SHOP_PRESENT_CHECK_TXT_EDIT_MESSAGE1,	// "���� �п��� ���� �޽���" �޽���
	StrToWzID("ST02"),	//eDIALOG_CASH_ITEM_SHOP_PRESENT_CHECK_TXT_EDIT_MESSAGE2,	// "�޽���" �޽���

	StrToWzID("B003"),	//eDIALOG_CASH_ITEM_SHOP_PRESENT_CHECK_BTN_OK,				// Ȯ�� ��ư
	StrToWzID("BT01"),	//eDIALOG_CASH_ITEM_SHOP_PRESENT_CHECK_BTN_RETURN,			// ���������� ��ư

	StrToWzID("B002"),	//eDIALOG_CASH_ITEM_SHOP_PRESENT_CHECK_BTN_CANCLE,			// ��� ��ư
	StrToWzID("BT02"),	//eDIALOG_CASH_ITEM_SHOP_PRESENT_CHECK_BTN_REV,				// �����ޱ� ��ư

	StrToWzID("E000"),	//eDIALOG_CASH_ITEM_SHOP_PRESENT_CHECK_MULEDIT_MSG,			// �޽��� �Է� ��Ƽ����Ʈ

	StrToWzID("LS00"),	//eDIALOG_CASH_ITEM_SHOP_PRESENT_CHECK_LIST,				// �޽��� ��� ����Ʈ ��Ʈ��
	StrToWzID("VS00"),	//eDIALOG_CASH_ITEM_SHOP_PRESENT_CHECK_SCROLL,			// �޽��� ��� ��ũ�� ��Ʈ��
	StrToWzID("PI00"),	//eDIALOG_CASH_ITEM_SHOP_PRESENT_CHECK_PICTURE,			// �޽��� ��� ���� ��Ʈ��

};



//------------------------------------------------------------------------------
/**
*/
uiCashItemShopPresentCheck::uiCashItemShopPresentCheck(InterfaceManager *pUIMan) :
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
			pSystemMan->Sys_VerifyLock(_T("m_pCashItemShopMan�� �����ϴ�."));
#endif
		this->ShowInterface(FALSE);
		return;
	}

	ZeroMemory(&m_PresentItemImg, sizeof(m_PresentItemImg));
	ZeroMemory(&m_PresentItemInfo, sizeof(m_PresentItemInfo));
	ZeroMemory(&m_ItemCodeForTooltip, sizeof(m_ItemCodeForTooltip));
	
	m_bShow = false;
}

//------------------------------------------------------------------------------
/**
*/
uiCashItemShopPresentCheck::~uiCashItemShopPresentCheck()
{
	this->Release();
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopPresentCheck::Init(CDrawBase* pDrawBase)
{
	uiBase::Init(pDrawBase);

	this->InitControls();
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopPresentCheck::Release()
{
	this->_removeIconImage();
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopPresentCheck::InitControls()
{
	int i;

	for(i = 0; i < eCASH_ITEM_SHOP_PRESENT_CHECK_DLG_STATE_MAX; ++i)
	{
		m_pTxtCtrlTitle[i] = static_cast<CCtrlStaticWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_PRESENT_CHECK_TXT_PRESENT_SEND+i));
		assert(m_pTxtCtrlTitle[i]);

		m_pTxtCtrlNameMessage[i] = static_cast<CCtrlStaticWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_PRESENT_CHECK_TXT_NAME_MESSAGE1+i));
		assert(m_pTxtCtrlNameMessage[i]);

		m_pTxtCtrlEditMessage[i] = static_cast<CCtrlStaticWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_PRESENT_CHECK_TXT_EDIT_MESSAGE1+i));
		assert(m_pTxtCtrlEditMessage[i]);

		m_pBtnCtrlOk[i] = static_cast<CCtrlButtonWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_PRESENT_CHECK_BTN_OK+i));
		assert(m_pBtnCtrlOk[i]);

		m_pBtnCtrlCancle[i] = static_cast<CCtrlButtonWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_PRESENT_CHECK_BTN_CANCLE+i));
		assert(m_pBtnCtrlCancle[i]);
	}
	
	m_pTxtCtrlPlayerName = static_cast<CCtrlStaticWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_PRESENT_CHECK_TXT_PLAYERNAME));
	assert(m_pTxtCtrlPlayerName);

	m_pTxtCtrlPresentName = static_cast<CCtrlStaticWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_PRESENT_CHECK_TXT_PRESENT_NAME));
	assert(m_pTxtCtrlPresentName);

	m_pTxtCtrlPresentUsetime = static_cast<CCtrlStaticWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_PRESENT_CHECK_TXT_USETIME));
	assert(m_pTxtCtrlPresentUsetime);

	m_pTxtCtrlPresentCount = static_cast<CCtrlStaticWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_PRESENT_CHECK_TXT_COUNT));
	assert(m_pTxtCtrlPresentCount);

	m_pTxtCtrlPresentMoney = static_cast<CCtrlStaticWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_PRESENT_CHECK_TXT_MONEY));
	assert(m_pTxtCtrlPresentMoney);

	m_pImgCtrl = static_cast<CCtrlButtonWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_PRESENT_CHECK_IMG_ITEM));
	assert(m_pImgCtrl);

	m_pMultiEditCtrl = static_cast<CCtrlMultilineEditWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_PRESENT_CHECK_MULTIEDIT_MSG));
	assert(m_pMultiEditCtrl);

	m_pMultiEditCtrl->SetFontID(c_wiMessageFont);

	m_pEditCtrlPlayerName = static_cast<CCtrlDxEditBox *>(getControl(eDIALOG_CASH_ITEM_SHOP_PRESENT_CHECK_EDIT_PLAYERNAME));
	assert(m_pEditCtrlPlayerName);


	m_pListCtrl = static_cast<CCtrlListWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_PRESENT_CHECK_LIST));
	assert(m_pListCtrl);

	m_pScroll = static_cast<CCtrlVScrollWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_PRESENT_CHECK_SCROLL));
	assert(m_pScroll);

	m_pPicture = static_cast<CCtrlPictureWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_PRESENT_CHECK_PICTURE));
	assert(m_pPicture);

#if defined(_JAPAN) || defined(_CHINA) || defined(_RUSSIA)
    // �Ϻ��� ȯ�Ұ��� �ȳ��޽����� ������� ����
    CControlWZ* refund_text = GetControlWZ(StrToWzID("S000"));
    if (refund_text != NULL)
    {
        refund_text->ShowWindowWZ(WZ_HIDE);
    }
#endif

	this->m_bControlsInitialized = true;
}

//------------------------------------------------------------------------------
void uiCashItemShopPresentCheck::ShowPresentItemCheck(stPresentItemInfo* pInfo)
{
	if(!pInfo)
		return;
	ZeroMemory(&m_ItemCodeForTooltip, sizeof(m_ItemCodeForTooltip));
	CopyMemory(&m_PresentItemInfo, pInfo, sizeof(m_PresentItemInfo));
	ShowInterface(TRUE);

	_refresh();

}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopPresentCheck::_initDlg()
{
	int iPresentState = GetManager()->GetPresentState();
	
	for(int i = 0; i < eCASH_ITEM_SHOP_PRESENT_CHECK_DLG_STATE_MAX; ++i)
	{
		if( i == iPresentState)
		{
			if(m_pTxtCtrlTitle[i])
				m_pTxtCtrlTitle[i]->ShowWindowWZ(WZ_SHOW);
			if(m_pTxtCtrlNameMessage[i])
				m_pTxtCtrlNameMessage[i]->ShowWindowWZ(WZ_SHOW);
			if(m_pTxtCtrlEditMessage[i])
				m_pTxtCtrlEditMessage[i]->ShowWindowWZ(WZ_SHOW);
			if(m_pBtnCtrlOk[i])
				m_pBtnCtrlOk[i]->ShowWindowWZ(WZ_SHOW);
			if(m_pBtnCtrlCancle[i])
				m_pBtnCtrlCancle[i]->ShowWindowWZ(WZ_SHOW);
		}
		else
		{
			if(m_pTxtCtrlTitle[i])
				m_pTxtCtrlTitle[i]->ShowWindowWZ(WZ_HIDE);
			if(m_pTxtCtrlNameMessage[i])
				m_pTxtCtrlNameMessage[i]->ShowWindowWZ(WZ_HIDE);
			if(m_pTxtCtrlEditMessage[i])
				m_pTxtCtrlEditMessage[i]->ShowWindowWZ(WZ_HIDE);
			if(m_pBtnCtrlOk[i])
				m_pBtnCtrlOk[i]->ShowWindowWZ(WZ_HIDE);
			if(m_pBtnCtrlCancle[i])
				m_pBtnCtrlCancle[i]->ShowWindowWZ(WZ_HIDE);
		}
	}

	if(iPresentState == ePresentState_Send)
	{
		if(m_pEditCtrlPlayerName)
			m_pEditCtrlPlayerName->ShowWindowWZ(WZ_SHOW);

		if(m_pTxtCtrlPlayerName)
			m_pTxtCtrlPlayerName->ShowWindowWZ(WZ_HIDE);


		if(m_pMultiEditCtrl)
			m_pMultiEditCtrl->ShowWindowWZ(WZ_SHOW);

		if(m_pListCtrl)
			m_pListCtrl->ShowWindowWZ(WZ_HIDE);

		if(m_pScroll)
			m_pScroll->ShowWindowWZ(WZ_HIDE);

		if(m_pPicture)
			m_pPicture->ShowWindowWZ(WZ_HIDE);


	}
	else if(iPresentState == ePresentState_Rcv)
	{
		if(m_pEditCtrlPlayerName)
			m_pEditCtrlPlayerName->ShowWindowWZ(WZ_HIDE);

		if(m_pTxtCtrlPlayerName)
			m_pTxtCtrlPlayerName->ShowWindowWZ(WZ_SHOW);

		if(m_pListCtrl)
			m_pListCtrl->ShowWindowWZ(WZ_SHOW);

		if(m_pScroll)
			m_pScroll->ShowWindowWZ(WZ_SHOW);

		if(m_pPicture)
			m_pPicture->ShowWindowWZ(WZ_SHOW);

		if(m_pMultiEditCtrl)
			m_pMultiEditCtrl->ShowWindowWZ(WZ_HIDE);

		if(m_PresentItemInfo.strSendMessage)
		{
			if(m_pListCtrl && m_pScroll)
			{
				m_pListCtrl->DeleteItemAll();
				m_pListCtrl->SetUseReport(false);
				m_pListCtrl->SetSellHeight(22);
				m_pListCtrl->SetFontID(c_wiMessageFont);
				m_pListCtrl->SetColorSelBar(RGBA(255,255,255,0));

				RECT rc = m_pListCtrl->GetSizeRect();
#ifdef __LTJ_TEXT_OUTPUT_AT_LIST_MOD
				int iListWidth = rc.right - rc.left - 15;
#else
				int iListWidth = rc.right - rc.left - 5;
#endif	

				std::vector<util::_tstring> vecStr;
				TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};

				vecStr.clear();
				GlobalFunc::ProcessWordWrapByFontSize( &vecStr, const_cast<TCHAR *>(m_PresentItemInfo.strSendMessage), c_wiMessageFont, _tcslen(m_PresentItemInfo.strSendMessage), iListWidth );

				int index = 0;

				for( unsigned int i =0; i < vecStr.size(); ++i)
				{
					ZeroMemory(szMessage, sizeof(szMessage));

					Snprintf (szMessage, INTERFACE_STRING_LENGTH-1, const_cast<TCHAR *>(vecStr[i].c_str()));
					int iSize = _tcslen(szMessage);

					if(_tcslen(szMessage) != 0)
					{
						index = m_pListCtrl->InsertItemBack();

						GlobalFunc::SetItem(m_pListCtrl,index,0,szMessage,c_wiMessageFont, 
							DT_LEFT | DT_VCENTER,
							RGBA(255,255,255,0), 
							RGBA(255,255,255,255), 
							RGBA(255,255,255,255));
					}
				}

				m_pScroll->SetScrollPosWZ(0);
				int iPage = ((index - SHOW_DEFAULT_READ_PRESENT_MESSAGE_LINE) <= 0) ? 0 : (index - SHOW_DEFAULT_READ_PRESENT_MESSAGE_LINE);
				if(m_pScroll)
					m_pScroll->SetScrollRangeWZ(0,iPage);	//��ũ�� ����


			}
		}


		if(m_PresentItemInfo.strPlayerName)
			GlobalFunc::SetCaption(m_pTxtCtrlPlayerName, m_PresentItemInfo.strPlayerName);

		if(m_PresentItemInfo.byPresentState == PresentRecvBox::PresentInfo::ePS_WAIT)
		{
			if(m_pBtnCtrlOk[eCASH_ITEM_SHOP_PRESENT_CHECK_DLG_STATE_RCV])
				m_pBtnCtrlOk[eCASH_ITEM_SHOP_PRESENT_CHECK_DLG_STATE_RCV]->EnableWindowWZ(true);
			if(m_pBtnCtrlCancle[eCASH_ITEM_SHOP_PRESENT_CHECK_DLG_STATE_RCV])
				m_pBtnCtrlCancle[eCASH_ITEM_SHOP_PRESENT_CHECK_DLG_STATE_RCV]->EnableWindowWZ(true);
		}
		else
		{
			if(m_pBtnCtrlOk[eCASH_ITEM_SHOP_PRESENT_CHECK_DLG_STATE_RCV])
				m_pBtnCtrlOk[eCASH_ITEM_SHOP_PRESENT_CHECK_DLG_STATE_RCV]->EnableWindowWZ(false);
			if(m_pBtnCtrlCancle[eCASH_ITEM_SHOP_PRESENT_CHECK_DLG_STATE_RCV])
				m_pBtnCtrlCancle[eCASH_ITEM_SHOP_PRESENT_CHECK_DLG_STATE_RCV]->EnableWindowWZ(false);
		}
	}
}

//------------------------------------------------------------------------------
/**
*/
BOOL uiCashItemShopPresentCheck::_isEnable()
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
void uiCashItemShopPresentCheck::_refresh()
{
	_clearItem();
	_setItem();
}


//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopPresentCheck::_clearItem()
{
	GlobalFunc::SetCaption(m_pTxtCtrlPlayerName, _T(""));
	GlobalFunc::SetCaption(m_pTxtCtrlPresentName, _T(""));
#ifdef _GS_MOD_GSP_ITEMSHOP_TXT
	m_vecItemName.clear();
#endif//_GS_MOD_GSP_ITEMSHOP_TXT
	GlobalFunc::SetCaption(m_pTxtCtrlPresentUsetime, _T(""));
	GlobalFunc::SetCaption(m_pTxtCtrlPresentCount, _T(""));
	GlobalFunc::SetCaption(m_pTxtCtrlPresentMoney, _T("")); 

	_removeIconImage();
	if(m_pEditCtrlPlayerName)
	{
		m_pEditCtrlPlayerName->ClearText();
	}

	if(m_pMultiEditCtrl)
	{
		m_pMultiEditCtrl->ClearText();
		m_pMultiEditCtrl->SetLimitText(MAX_GIFT_MESSAGE_LENGTH/2);
		m_pMultiEditCtrl->SetColorText( RGBA( 255, 255, 255, 255));
		m_pMultiEditCtrl->SetColorCaret(c_InterfaceCaretColor);
		m_pMultiEditCtrl->SetColorSelText( RGBA( 255, 255, 255, 255) );
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopPresentCheck::_setItem()
{
	TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szTemp2[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szTemp3[INTERFACE_STRING_LENGTH] = {0,};

	BASE_ITEMINFO * pItemInfo = ItemInfoParser::Instance()->GetItemInfo(m_PresentItemInfo.stPiece[0].dwItemCode);
	if ( !pItemInfo ) 
	{
		return;
	}

	g_InterfaceManager.GetItemName(m_PresentItemInfo.stPiece[0].dwItemCode, szMessage, INTERFACE_STRING_LENGTH);


	if(m_PresentItemInfo.stPiece[0].byEnchant)
	{
		Snprintf(szTemp2, INTERFACE_STRING_LENGTH-1, _T("+%d"), m_PresentItemInfo.stPiece[0].byEnchant);
		_tcscat(szMessage, szTemp2);
	}

    // ĳ�ü��������� Rank ���� �뵵 �����Ͽ� Rank�� ��� ���� �ʵ��� ����
    // �������� Rank��ü�� ���ǰ� ���� �ʾ���.
    //if(pItemInfo->CanRankUpItem())
	//{
	//	int iRankIndex = eST_RANK_E_STRING_INDEX  + (m_PresentItemInfo.stPiece[0].byRank);
	//	if (iRankIndex > eST_RANK_PS_STRING_INDEX)
	//		iRankIndex = eST_RANK_PS_STRING_INDEX;
	//	if (iRankIndex < eST_RANK_E_STRING_INDEX)
	//		iRankIndex = eST_RANK_E_STRING_INDEX;

	//	ZeroMemory(szTemp2, sizeof(szTemp2));
	//	ZeroMemory(szTemp3, sizeof(szTemp3));
	//	g_InterfaceManager.GetInterfaceString(eST_RANK_STRING_INDEX,szTemp,INTERFACE_STRING_LENGTH);
	//	g_InterfaceManager.GetInterfaceString(iRankIndex,szTemp2,INTERFACE_STRING_LENGTH);	
	//	Snprintf(szTemp3,INTERFACE_STRING_LENGTH-1, szTemp, szTemp2);
	//	_tcscat(szMessage, _T("   "));
	//	_tcscat(szMessage, szTemp3);
	//}

#ifdef _GS_MOD_GSP_ITEMSHOP_TXT
	_spliteItemName(szMessage);
#else
	GlobalFunc::SetCaption(m_pTxtCtrlPresentName, szMessage);	// ������ �̸�
#endif//_GS_MOD_GSP_ITEMSHOP_TXT

	// ������ ������
    m_PresentItemImg.type = m_PresentItemInfo.stPiece[0].byRank;
	_addIconImage(pItemInfo->m_wVICode, m_PresentItemInfo.stPiece[0].dwItemCode);

	int day = 0;
	int hour = 0;
	int min = 0;

	if(m_PresentItemInfo.stPiece[0].dwUseTime)	// %d ��
	{
		ZeroMemory(szMessage, sizeof(szMessage));
		GlobalFunc::CalcTime(m_PresentItemInfo.stPiece[0].dwUseTime, day, hour, min, szMessage);

		GlobalFunc::SetCaption(m_pTxtCtrlPresentUsetime, szMessage);	
	}

	if(m_PresentItemInfo.dwPrice)
	{
		ZeroMemory(szMessage, sizeof(szMessage));
		if(LOGINPARAM->IsLiveTestServer() == FALSE) 
		{
			// %d Cash
			GlobalFunc::SetMoneyText(m_pTxtCtrlPresentMoney, m_PresentItemInfo.dwPrice, true);
		}
		else	
		{
			// %d Heim
			GlobalFunc::SetMoneyText(m_pTxtCtrlPresentMoney, m_PresentItemInfo.dwPrice, false);	
		}
	}

	if(m_PresentItemInfo.stPiece[0].dwAmount)	// %d ��
	{
		ZeroMemory(szMessage, sizeof(szMessage));
		TCHAR szTemp[INTERFACE_STRING_LENGTH] ={0,};
		// 155376	%d��
		g_InterfaceManager.GetInterfaceString( 155376 , szTemp ,INTERFACE_STRING_LENGTH);
		Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, szTemp, m_PresentItemInfo.stPiece[0].dwAmount);
		GlobalFunc::SetCaption(m_pTxtCtrlPresentCount, szMessage);	
	}

	_initDlg();
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopPresentCheck::_addIconImage(DWORD dwCode, CODETYPE itemCode)
{
	m_PresentItemImg.dwIconCode = dwCode;
	m_PresentItemImg.itemCode = itemCode;
	_removeIconImage();

	if (dwCode != 0)
	{
		m_PresentItemImg.pImage  = ICONMGR()->Add(dwCode);
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopPresentCheck::_removeIconImage()
{
	if(m_PresentItemImg.pImage)
	{
		ICONMGR()->RemoveIcon(m_PresentItemImg.pImage);
		m_PresentItemImg.pImage = NULL;
	}
}


//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopPresentCheck::_renderTooltip()
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

	// ��ŵó�� 
	// �������̽����� Dialog ���������� Mouse Over�� ���� �ʴ´�.
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
void uiCashItemShopPresentCheck::_renderIconImage()
{
	if(m_pImgCtrl)
	{
		if (m_PresentItemImg.pImage && m_PresentItemImg.dwIconCode)
		{
			RECT rc = m_pImgCtrl->GetSizeRect();
			m_PresentItemImg.pImage->Render(&rc);
            GetManager()->renderCoverImage(m_PresentItemImg, rc);
		}
	}
}


//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopPresentCheck::OnShowWindow( BOOL val )
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
		msg.dwType = eDoType_dialog_cash_item_shop_present_check;
		msg.wParam = InterfaceManager::DIALOG_CASH_ITEM_SHOP_PRESENT_CHECK;
		msg.DoSomething = GlobalFunc::DoShowWindow;
		g_KeyQueueManager.PushBack(msg);

		m_bShow = true;
	}
	else
	{
		g_KeyQueueManager.DeleteMsg(eDoType_dialog_cash_item_shop_present_check);
		m_bShow = false;
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopPresentCheck::OnUpdateSolarDialog()
{
	if(!m_bShow)
		this->ShowInterface(FALSE);
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopPresentCheck::OnRenderSolarDialog()
{
	if (!_isEnable())
		return;

	this->_renderIconImage();
	this->_renderTooltip();

#ifdef _GS_MOD_GSP_ITEMSHOP_TXT
	_renderItemName();
#endif//_GS_MOD_GSP_ITEMSHOP_TXT
}

//------------------------------------------------------------------------------
/**
	���� ������ ��Ȯ�� 
*/
void _CallBack_GiftConfirm(bool bYes)
{
	uiCashItemShopMan* pCashShopMan = static_cast<uiCashItemShopMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_CASH_ITEM_SHOP));

//	uiCashItemShopPresentCheck* pCashShopPresent = static_cast<uiCashItemShopPresentCheck *>(g_InterfaceManager.GetUserInterface(UI_CASH_ITEM_SHOP_PRESENT_CHECK));

//	CopyMemory(&(pCashShopMan->m_pendingBuyPresentItemInfo), &pCashShopPresent->m_PresentItemInfo, sizeof(pCashShopPresent->m_PresentItemInfo));

	if(!pCashShopMan)
		return;

	if (bYes)
	{	
		pCashShopMan->Send_CG_SHOP_SEND_GIFT_SYN();
	}
	pCashShopMan->SetSystemDialog(false);
}
//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopPresentCheck::MessageProc( SI_MESSAGE * pMessage )
{
	if (!_isEnable())
		return;

	TCHAR	szMessage[INTERFACE_STRING_LENGTH] = {0,};

	POSTYPE CtrlPos = getCtrlIDToPos( pMessage->dwCtrlID );

	switch( pMessage->eResultMsg ) 
	{
	case RT_MSG_LBUTTONCLICK:
		{
			switch(CtrlPos)
			{
			case eDIALOG_CASH_ITEM_SHOP_PRESENT_CHECK_BTN_CANCLE:
			case eDIALOG_CASH_ITEM_SHOP_PRESENT_CHECK_BTN_EXIT:
				{
					m_bShow = false;
				}
				break;

			case eDIALOG_CASH_ITEM_SHOP_PRESENT_CHECK_BTN_OK:	// ���� ������!
				{
					if(!GetManager()->GetRqstState())
					{
						GetManager()->ClearPendingBuyPresentItemInfo();

						if(m_PresentItemInfo.priceGuid > 0)
						{
							if(m_pMultiEditCtrl)
							{
								WCHAR wszMemo[INTERFACE_STRING_LENGTH] = {0,};
								m_pMultiEditCtrl->GetTextWZW(wszMemo, INTERFACE_STRING_LENGTH);

								TCHAR szMemo[INTERFACE_STRING_LENGTH] = {0,};
								GlobalFunc::ChangeMultiEditInput(wszMemo, szMemo);

								Snprintf(m_PresentItemInfo.strSendMessage, MAX_GIFT_MESSAGE_LENGTH, szMemo);
							}

							if(m_pEditCtrlPlayerName)
								m_pEditCtrlPlayerName->GetTextWZ(m_PresentItemInfo.strPlayerName, MAX_CHARNAME_LENGTH);

							if(m_PresentItemInfo.strPlayerName[0] == NULL)
							{
								//155226	������ ������ ĳ���͸��� �Է��� �ֽʽÿ�.
								g_InterfaceManager.GetInterfaceString(155226, szMessage, INTERFACE_STRING_LENGTH);
								GlobalFunc::OutputMsg_SysMsg(szMessage);
							}
							else
							{
								CopyMemory(&(GetManager()->m_pendingBuyPresentItemInfo), &m_PresentItemInfo, sizeof(m_PresentItemInfo));
								TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};
								// (155428: %s �Բ� �����Ͻðڽ��ϱ�?)

								g_InterfaceManager.GetInterfaceString( 155428, szTemp, INTERFACE_STRING_LENGTH );
								Snprintf(szMessage, INTERFACE_STRING_LENGTH, szTemp, m_PresentItemInfo.strPlayerName);
				
								uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
								if(pSystemMan)
								{
									pSystemMan->Sys_ConfirmUnLock(szMessage,_CallBack_GiftConfirm);
								}
								GetManager()->SetSystemDialog(true);
							}
						}
						else
						{
							//85198	������ �������� �����ϴ�.
							g_InterfaceManager.GetInterfaceString(85198, szMessage, INTERFACE_STRING_LENGTH);
							GlobalFunc::OutputMsg_SysMsg(szMessage);
						}
					}
				}
				break;

				case eDIALOG_CASH_ITEM_SHOP_PRESENT_CHECK_BTN_RETURN:		// ���������� ��ư
				{
					if(!GetManager()->GetRqstState())
					{
						GetManager()->ClearPendingBuyPresentItemInfo();

						if(m_PresentItemInfo.priceGuid > 0)
						{
							CopyMemory(&(GetManager()->m_pendingBuyPresentItemInfo), &m_PresentItemInfo, sizeof(m_PresentItemInfo));
							GetManager()->SetPresentState(ePresentState_Return);
							GetManager()->Send_CG_SHOP_ACTION_RECV_GIFT_SYN(MSG_CG_SHOP_ACTION_RECV_GIFT_SYN::REJECT);
						}
						else
						{
							//85198	������ �������� �����ϴ�.
							g_InterfaceManager.GetInterfaceString(85198, szMessage, INTERFACE_STRING_LENGTH);
							GlobalFunc::OutputMsg_SysMsg(szMessage);
						}
					}
				}
				break;

				case eDIALOG_CASH_ITEM_SHOP_PRESENT_CHECK_BTN_REV:		// �����ޱ� ��ư
					{
						if(!GetManager()->GetRqstState())
						{
							GetManager()->ClearPendingBuyPresentItemInfo();

							if(m_PresentItemInfo.priceGuid > 0)
							{
								CopyMemory(&(GetManager()->m_pendingBuyPresentItemInfo), &m_PresentItemInfo, sizeof(m_PresentItemInfo));
								GetManager()->Send_CG_SHOP_ACTION_RECV_GIFT_SYN(MSG_CG_SHOP_ACTION_RECV_GIFT_SYN::OBTAIN);
							}
							else
							{
								//85198	������ �������� �����ϴ�.
								g_InterfaceManager.GetInterfaceString(85198, szMessage, INTERFACE_STRING_LENGTH);
								GlobalFunc::OutputMsg_SysMsg(szMessage);
							}
						}
					}
					break;
			}
		}
		break;



	case RT_MSG_SB_PAGETOP:		//. ���� ������ ������.
	case RT_MSG_SB_LINETOP:		//.���� ȭ��ǥ�� ������.
		{
			switch(CtrlPos)
			{
			case eDIALOG_CASH_ITEM_SHOP_PRESENT_CHECK_SCROLL:
				{
					int pos = m_pScroll->GetScrollPosWZ();//���� ��ũ�� ���� �޾� �´�.
					int mmin = 0 , mmax = 0;
					m_pScroll->GetScrollRangeWZ( &mmin, &mmax );//��ũ�� ���� �ִ� �ּҸ� ��� �´�.
					if(pos>mmin)// ���� �Էµ� ���μ� ���� ������ 
					{
						pos--;//���� ���� ��Ű�� 
						m_pScroll->SetScrollPosWZ(pos);
						_refreshList();
					}
				}
				break;
			default:
				break;
			}
		}
		break;

	case RT_MSG_SB_PAGEBOTTOM:	//. ���� ������ ������.
	case RT_MSG_SB_LINEBOTTOM:	//.�Ʒ��� ȭ��ǥ�� ������
		{
			switch(CtrlPos)
			{
			case eDIALOG_CASH_ITEM_SHOP_PRESENT_CHECK_SCROLL:
				{
					int pos = m_pScroll->GetScrollPosWZ();//���� ��ũ�� ���� �޾� �´�.
					int mmin = 0 , mmax = 0;
					m_pScroll->GetScrollRangeWZ( &mmin, &mmax );//��ũ�� ���� �ִ� �ּҸ� ��� �´�.

					if(pos<mmax)// ���� �Էµ� ���μ� ���� ������ 
					{
						pos++;//���� ���� ��Ű�� 
						m_pScroll->SetScrollPosWZ(pos);
						_refreshList();
					}
				}
				break;
			default:
				break;
			}
		}
		break;

	case RT_MSG_SB_THUMBTRACK:				//. ���� �巡�� �ϰ� �ִ�.
	case RT_MSG_SB_THUMBPOSITION:			//. ���� �巡������ ���콺�� ���Ҵ�.
	case RT_MSG_SB_ENDSCROLL:				//. ��ũ���� ���� �Ǿ���.
		{
			if(m_pScroll)
			{
				int pos = m_pScroll->GetScrollPosWZ();//���� ��ũ�� ���� �޾� �´�.
				m_pScroll->SetScrollPosWZ(pos);
				_refreshList();
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
//----------------------------------------------------------------------------
/**
*/
void uiCashItemShopPresentCheck::_refreshList()
{
	if(m_pListCtrl && m_pScroll)
	{
		int iCurScrollPos = m_pScroll->GetScrollPosWZ();//���� ��ũ�� ���� �޾� �´�
		int iStartDrawIndex = (iCurScrollPos);

		m_pListCtrl->SetBeginDrawIndex( iStartDrawIndex );
	}
}

//----------------------------------------------------------------------------
/**
*/
void uiCashItemShopPresentCheck::MouseOver_MsgProc( SI_MESSAGE * pMessage )
{
	if (!_isEnable())
		return;

	switch( pMessage->eResultMsg ) 
	{
	case RT_MSG_MOUSEOVER:
		{
			POSTYPE pos = getCtrlIDToPos( pMessage->dwCtrlID );

			if (pos == eDIALOG_CASH_ITEM_SHOP_PRESENT_CHECK_IMG_ITEM)
			{
				if (m_PresentItemImg.pImage && m_PresentItemImg.dwIconCode)
				{
					m_ItemCodeForTooltip.dwItemCode		= m_PresentItemImg.itemCode;
					m_ItemCodeForTooltip.dwCoolTime		= m_PresentItemInfo.dwCoolTime;
					m_ItemCodeForTooltip.SellEndDate	= m_PresentItemInfo.SellEndDate;
					m_ItemCodeForTooltip.byEnchant		= m_PresentItemInfo.stPiece[0].byEnchant;
					m_ItemCodeForTooltip.byRank			= m_PresentItemInfo.stPiece[0].byRank;
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


#ifdef _GS_MOD_GSP_ITEMSHOP_TXT
//------------------------------------------------------------------------------
/** �������� �����۸��� ©��������(���� ��Ʈ�ѷ��� �ٲܼ����� ��¿�����̷�����)
*/
void uiCashItemShopPresentCheck::_renderItemName()
{
	if(!m_vecItemName.empty() && m_pTxtCtrlPresentName)
	{
		g_pSunRenderer->x_pManagerTextOut->StartToUseFont( m_pTxtCtrlPresentName->GetFontID() );

		//�����̶�� ���Ϳ� ����ش�.
		if(m_vecItemName.size()<2)
		{
			RECT rcName = m_pTxtCtrlPresentName->GetSizeRect();

			TCHAR *pStr = const_cast<TCHAR *>(m_vecItemName[0].c_str());

			g_pSunRenderer->x_pManagerTextOut->DrawText(pStr,
				&rcName,
				WzColor_RGB( 199, 179, 137 ),
				WzColor_RGBA(  0,  0,  0,   0),
				TP_VCENTER);
		}
		else
		{
			//�����̶�� �����Ѵ�
			for (size_t i=0; i<m_vecItemName.size(); ++i)
			{
				RECT rcName = m_pTxtCtrlPresentName->GetSizeRect();

				TCHAR *pStr = const_cast<TCHAR *>(m_vecItemName[i].c_str());

				DWORD dwFontHeight = GlobalFunc::GetTextHeight(pStr,m_pTxtCtrlPresentName->GetFontID());

				//����ó��
				rcName.top+=(i*dwFontHeight+2);
				rcName.bottom+=(i*dwFontHeight+2);

				g_pSunRenderer->x_pManagerTextOut->DrawText(
					pStr,
					&rcName,
					WzColor_RGB( 199, 179, 137 ),
					WzColor_RGBA(0,0,0,0),
					TP_VTOP|TP_HLEFT);

			}
		}
		g_pSunRenderer->x_pManagerTextOut->EndUseFont();
	}
}
//------------------------------------------------------------------------------
/** 
*/
void uiCashItemShopPresentCheck::_spliteItemName(TCHAR *pItemName)
{
	if(m_pTxtCtrlPresentName)
	{
		//��Ʈ�ѷ��� �����ʴ´�.
		m_pTxtCtrlPresentName->ShowWindowWZ(WZ_HIDE);

		//step:�̸��׸������� ������ ���Ϳ� ��Ƴ���.
		RECT rcName = m_pTxtCtrlPresentName->GetSizeRect();

		GlobalFunc::ProcessWordWrapByFontSize( &m_vecItemName, pItemName, m_pTxtCtrlPresentName->GetFontID(), _tcslen(pItemName),rcName.right - rcName.left - 10 );
	}
}
#endif//_GS_MOD_GSP_ITEMSHOP_TXT

//------------------------------------------------------------------------------ 
#endif //#ifndef _NA_0_20110613_GAME_SHOP_RENEWAL //#if'N'def
//------------------------------------------------------------------------------ 
