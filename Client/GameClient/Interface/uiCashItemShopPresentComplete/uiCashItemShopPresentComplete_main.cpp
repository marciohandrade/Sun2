#include "SunClientPrecompiledHeader.h"

//------------------------------------------------------------------------------ 
#ifndef _NA_0_20110613_GAME_SHOP_RENEWAL //#if'N'def
//------------------------------------------------------------------------------ 


#include "uiCashItemShopPresentComplete.h"
#include "uiCashItemShopMan/uiCashItemShopMan.h"
#include "uiSystemMan/uiSystemMan.h"
#include "Hero.h"
#include "GlobalFunc.h"
#include "interfacemanager.h"
#include "GameFramework.h"
#include "Mouse.h"
#include "ItemInfoParser.h"
#include "uiToolTipMan/uiToolTipMan.h"

WzID uiCashItemShopPresentComplete::m_wzId[eDIALOG_CASH_ITEM_SHOP_PRESENT_COMPLETE_MAX] = 
{
	StrToWzID("BT00"),	//eDIALOG_CASH_ITEM_SHOP_PRESENT_COMPLETE_BTN_EXIT,			// X ��ư

	StrToWzID("B001"),	//eDIALOG_CASH_ITEM_SHOP_PRESENT_COMPLETE_IMG_ITEM,			// ���� �̹���

	StrToWzID("T001"),	//eDIALOG_CASH_ITEM_SHOP_PRESENT_COMPLETE_TXT_SEND,			// ����Ȯ�� �ؽ�Ʈ
	StrToWzID("ST01"),	//eDIALOG_CASH_ITEM_SHOP_PRESENT_COMPLETE_TXT_RCV,			// �����ޱ� �ؽ�Ʈ
	StrToWzID("ST02"),	//eDIALOG_CASH_ITEM_SHOP_PRESENT_COMPLETE_TXT_RETURN,		// �������������� �ؽ�Ʈ

	StrToWzID("T002"),	//eDIALOG_CASH_ITEM_SHOP_PRESENT_COMPLETE_TXT_NAME,			// �����̸�
	
	StrToWzID("T005"),	//eDIALOG_CASH_ITEM_SHOP_PRESENT_COMPLETE_TXT_MESSAGE1,		// "%d�Բ� �����߽��ϴ�." �޽���
	StrToWzID("ST03"),	//eDIALOG_CASH_ITEM_SHOP_PRESENT_COMPLETE_TXT_MESSAGE2,		// "�� �������� ������ �޾ҽ��ϴ�" �޽���
	StrToWzID("ST04"),	//eDIALOG_CASH_ITEM_SHOP_PRESENT_COMPLETE_TXT_MESSAGE3,		// "������ ���� ���½��ϴ�." �޽���

	StrToWzID("B003"),	//eDIALOG_CASH_ITEM_SHOP_PRESENT_COMPLETE_BTN_OK,			// Ȯ�� ��ư
};


//------------------------------------------------------------------------------
/**
*/
uiCashItemShopPresentComplete::uiCashItemShopPresentComplete(InterfaceManager *pUIMan) :
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
uiCashItemShopPresentComplete::~uiCashItemShopPresentComplete()
{
	this->Release();
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopPresentComplete::Init(CDrawBase* pDrawBase)
{
	uiBase::Init(pDrawBase);

	this->InitControls();
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopPresentComplete::Release()
{
	_removeIconImage();
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopPresentComplete::InitControls()
{
	int i;
	for(i = 0; i < eCASH_ITEM_SHOP_PRESENT_COMPLETE_DLG_STATE_MAX; ++i)
	{
		m_pTxtCtrlTitle[i] = static_cast<CCtrlStaticWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_PRESENT_COMPLETE_TXT_SEND+i));
		assert(m_pTxtCtrlTitle[i]);

		m_pTxtCtrlMessage[i] = static_cast<CCtrlStaticWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_PRESENT_COMPLETE_TXT_MESSAGE1+i));
		assert(m_pTxtCtrlMessage[i]);
	}

	m_pTxtCtrlPresentName = static_cast<CCtrlStaticWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_PRESENT_COMPLETE_TXT_NAME));
	assert(m_pTxtCtrlPresentName);

	m_pImgCtrl = static_cast<CCtrlButtonWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_PRESENT_COMPLETE_IMG_ITEM));
	assert(m_pImgCtrl);

	this->m_bControlsInitialized = true;
}

//------------------------------------------------------------------------------
void uiCashItemShopPresentComplete::ShowPresentItemComplete(stPresentItemInfo* pInfo)
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
void uiCashItemShopPresentComplete::_initDlg()
{
	int iPresentState = GetManager()->GetPresentState();
	int i;
	TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};
	for(i = 0; i < eCASH_ITEM_SHOP_PRESENT_COMPLETE_DLG_STATE_MAX; ++i)
	{
		if(iPresentState == i)
		{
			if(m_pTxtCtrlTitle[i])
				m_pTxtCtrlTitle[i]->ShowWindowWZ(WZ_SHOW);
			if(m_pTxtCtrlMessage[i])
				m_pTxtCtrlMessage[i]->ShowWindowWZ(WZ_SHOW);
		}
		else
		{
			if(m_pTxtCtrlTitle[i])
				m_pTxtCtrlTitle[i]->ShowWindowWZ(WZ_HIDE);
			if(m_pTxtCtrlMessage[i])
				m_pTxtCtrlMessage[i]->ShowWindowWZ(WZ_HIDE);
		}
	}
	if(iPresentState == ePresentState_Send)
	{
		//155121 �Բ� �����Ͽ����ϴ�.
		g_InterfaceManager.GetInterfaceString(155121, szTemp, INTERFACE_STRING_LENGTH );

		Snprintf(szMessage, INTERFACE_STRING_LENGTH -1, _T("%s%s"),m_PresentItemInfo.strPlayerName, szTemp);
		GlobalFunc::SetCaption(m_pTxtCtrlMessage[eCASH_ITEM_SHOP_PRESENT_COMPLETE_DLG_STATE_SEND], szMessage);
	}
}


//------------------------------------------------------------------------------
/**
*/
BOOL uiCashItemShopPresentComplete::_isEnable()
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
void uiCashItemShopPresentComplete::_refresh()
{
	_clearItem();
	_setItem();
}


//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopPresentComplete::_clearItem()
{
	GlobalFunc::SetCaption(m_pTxtCtrlPresentName, _T(""));
#ifdef _GS_MOD_GSP_ITEMSHOP_TXT
	m_vecItemName.clear();
#endif//_GS_MOD_GSP_ITEMSHOP_TXT

	_removeIconImage();
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopPresentComplete::_setItem()
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

	_initDlg();

	g_InterfaceManager.GetItemName(m_PresentItemInfo.stPiece[0].dwItemCode, szMessage, INTERFACE_STRING_LENGTH);


	if(m_PresentItemInfo.stPiece[0].byEnchant)
	{
		Snprintf(szTemp, INTERFACE_STRING_LENGTH-1, _T("+%d"), m_PresentItemInfo.stPiece[0].byEnchant);
		_tcscat(szMessage, szTemp);
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

	//	ZeroMemory(szTemp, sizeof(szTemp));
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
}


//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopPresentComplete::_addIconImage(DWORD dwCode, CODETYPE itemCode)
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
void uiCashItemShopPresentComplete::_removeIconImage()
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
void uiCashItemShopPresentComplete::_renderTooltip()
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
void uiCashItemShopPresentComplete::_renderIconImage()
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
void uiCashItemShopPresentComplete::OnShowWindow( BOOL val )
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
		msg.dwType = eDoType_dialog_cash_item_shop_present_complete;
		msg.wParam = InterfaceManager::DIALOG_CASH_ITEM_SHOP_PRESENT_COMPLETE;
		msg.DoSomething = GlobalFunc::DoShowWindow;
		g_KeyQueueManager.PushBack(msg);

		m_bShow = true;
	}
	else
	{
		g_KeyQueueManager.DeleteMsg(eDoType_dialog_cash_item_shop_present_complete);
		m_bShow = false;
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopPresentComplete::OnUpdateSolarDialog()
{
	if(!m_bShow)
		this->ShowInterface(FALSE);
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopPresentComplete::OnRenderSolarDialog()
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
*/
void uiCashItemShopPresentComplete::MessageProc( SI_MESSAGE * pMessage )
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
			case eDIALOG_CASH_ITEM_SHOP_PRESENT_COMPLETE_BTN_EXIT:
			case eDIALOG_CASH_ITEM_SHOP_PRESENT_COMPLETE_BTN_OK:
				{
					m_bShow = false;
					GetManager()->SetPresentState(ePresentState_None);
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

//----------------------------------------------------------------------------
/**
*/
void uiCashItemShopPresentComplete::MouseOver_MsgProc( SI_MESSAGE * pMessage )
{
	if (!_isEnable())
		return;

	switch( pMessage->eResultMsg ) 
	{
	case RT_MSG_MOUSEOVER:
		{
			POSTYPE pos = getCtrlIDToPos( pMessage->dwCtrlID );

			if (pos == eDIALOG_CASH_ITEM_SHOP_PRESENT_COMPLETE_IMG_ITEM)
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
void uiCashItemShopPresentComplete::_renderItemName()
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
void uiCashItemShopPresentComplete::_spliteItemName(TCHAR *pItemName)
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
