#pragma once

//------------------------------------------------------------------------------ 
#ifndef _NA_0_20110613_GAME_SHOP_RENEWAL //#if'N'def
//------------------------------------------------------------------------------ 

//------------------------------------------------------------------------------
//
//    �۾���: ���¿�
//    uiCashItemShopPresentCheck class
//------------------------------------------------------------------------------

#include "uibase.h"

#include "uiCashItemShopMan/uiCashItemShopDef.h"

class uiCashItemShopMan;

class uiCashItemShopPresentCheck: public uiBase
{
	enum eDIALOG_CASH_ITEM_SHOP_PRESENT_CHECK
	{
		eDIALOG_CASH_ITEM_SHOP_PRESENT_CHECK_BTN_EXIT,			// X ��ư

		eDIALOG_CASH_ITEM_SHOP_PRESENT_CHECK_IMG_ITEM,			// ���� �̹���

		eDIALOG_CASH_ITEM_SHOP_PRESENT_CHECK_TXT_PRESENT_SEND,	// ���������� �ؽ�Ʈ
		eDIALOG_CASH_ITEM_SHOP_PRESENT_CHECK_TXT_PRESENT_REV,	// ����Ȯ�� �ؽ�Ʈ

		eDIALOG_CASH_ITEM_SHOP_PRESENT_CHECK_TXT_PRESENT_NAME,	// �����̸� �ؽ�Ʈ

		eDIALOG_CASH_ITEM_SHOP_PRESENT_CHECK_TXT_USETIME,		// ���� ���Ⱓ
		eDIALOG_CASH_ITEM_SHOP_PRESENT_CHECK_TXT_MONEY,			// ���� ����
		eDIALOG_CASH_ITEM_SHOP_PRESENT_CHECK_TXT_COUNT,			// ���� ����
		
		eDIALOG_CASH_ITEM_SHOP_PRESENT_CHECK_EDIT_PLAYERNAME,	// ������ �÷��̾��̸�
		
		eDIALOG_CASH_ITEM_SHOP_PRESENT_CHECK_TXT_PLAYERNAME,	// ������ �÷��̾��̸�

		eDIALOG_CASH_ITEM_SHOP_PRESENT_CHECK_TXT_NAME_MESSAGE1,	// "�Կ��� ������ �����ϴ�" �ؽ�Ʈ
		eDIALOG_CASH_ITEM_SHOP_PRESENT_CHECK_TXT_NAME_MESSAGE2,	// "���� ���� �����Դϴ�" �ؽ�Ʈ

		eDIALOG_CASH_ITEM_SHOP_PRESENT_CHECK_TXT_EDIT_MESSAGE1,	// "���� �п��� ���� �޽���" �ؽ�Ʈ
		eDIALOG_CASH_ITEM_SHOP_PRESENT_CHECK_TXT_EDIT_MESSAGE2,	// "�޽���" �ؽ�Ʈ

		eDIALOG_CASH_ITEM_SHOP_PRESENT_CHECK_BTN_OK,			// Ȯ�� ��ư
		eDIALOG_CASH_ITEM_SHOP_PRESENT_CHECK_BTN_RETURN,		// ���������� ��ư

		eDIALOG_CASH_ITEM_SHOP_PRESENT_CHECK_BTN_CANCLE,		// ��� ��ư
		eDIALOG_CASH_ITEM_SHOP_PRESENT_CHECK_BTN_REV,			// �����ޱ� ��ư

		eDIALOG_CASH_ITEM_SHOP_PRESENT_CHECK_MULTIEDIT_MSG,		// �޽��� �Է� ��Ƽ����Ʈ

		eDIALOG_CASH_ITEM_SHOP_PRESENT_CHECK_LIST,				// �޽��� ��� ����Ʈ ��Ʈ��
		eDIALOG_CASH_ITEM_SHOP_PRESENT_CHECK_SCROLL,			// �޽��� ��� ��ũ�� ��Ʈ��
		eDIALOG_CASH_ITEM_SHOP_PRESENT_CHECK_PICTURE,			// �޽��� ��� ���� ��Ʈ��

		eDIALOG_CASH_ITEM_SHOP_PRESENT_CHECK_MAX,
	};

	enum eCASH_ITEM_SHOP_PRESENT_CHECK_DLG_STATE
	{
		eCASH_ITEM_SHOP_PRESENT_CHECK_DLG_STATE_SEND,
		eCASH_ITEM_SHOP_PRESENT_CHECK_DLG_STATE_RCV,

		eCASH_ITEM_SHOP_PRESENT_CHECK_DLG_STATE_MAX,
	};

public:
	uiCashItemShopPresentCheck(InterfaceManager *pUIMan);
	virtual ~uiCashItemShopPresentCheck();



	//--------------------------------	------------------------------------------
	// init/release
	//
public:
	virtual void            Init(CDrawBase* pDrawBase);
	virtual void            Release();

protected:
	void                    InitControls();

	//--------------------------------------------------------------------------
	// helper/comms.
	//
public:
	uiCashItemShopMan*      GetManager();

	//--------------------------------------------------------------------------
	// functions
	//
private:
	BOOL					_isEnable();
	void					_refresh();

	void					_clearItem();
	void					_setItem();

	void					_addIconImage(DWORD dwCode, CODETYPE itemCode);
	void					_removeIconImage();

	void					_renderTooltip();
	void					_renderIconImage();
	void					_initDlg();
	void					_refreshList();

#ifdef _GS_MOD_GSP_ITEMSHOP_TXT
	void					_renderItemName();
	void					_spliteItemName(TCHAR *pItemName);
#endif//_GS_MOD_GSP_ITEMSHOP_TXT

	//--------------------------------------------------------------------------
	// dialog functions
	//
private:
	
public:
	void					ShowPresentItemCheck(stPresentItemInfo* pInfo);

protected:
	virtual void	        OnShowWindow( BOOL val );
	virtual void            OnUpdateSolarDialog();
	virtual void            OnRenderSolarDialog();

protected:
	CControlWZ *	        getControl( POSTYPE AtPos );
	POSTYPE			        getCtrlIDToPos( WzID wzId );
	static WzID		        m_wzId[eDIALOG_CASH_ITEM_SHOP_PRESENT_CHECK_MAX];

	//--------------------------------------------------------------------------
	// procedure
	//
public:
	virtual void	        MessageProc( SI_MESSAGE * pMessage );   // Pure
	virtual void			MouseOver_MsgProc( SI_MESSAGE * pMessage );

	//--------------------------------------------------------------------------
	// member vars
	//
private:
	uiCashItemShopMan*		m_pCashItemShopMan;


	CCtrlStaticWZ*			m_pTxtCtrlTitle[eCASH_ITEM_SHOP_PRESENT_CHECK_DLG_STATE_MAX];
	CCtrlStaticWZ*			m_pTxtCtrlPlayerName;
	CCtrlStaticWZ*			m_pTxtCtrlPresentName;
	CCtrlStaticWZ*			m_pTxtCtrlPresentUsetime;
	CCtrlStaticWZ*			m_pTxtCtrlPresentCount;
	CCtrlStaticWZ*			m_pTxtCtrlPresentMoney;
	CCtrlStaticWZ*			m_pTxtCtrlNameMessage[eCASH_ITEM_SHOP_PRESENT_CHECK_DLG_STATE_MAX];
	CCtrlStaticWZ*			m_pTxtCtrlEditMessage[eCASH_ITEM_SHOP_PRESENT_CHECK_DLG_STATE_MAX];

	CCtrlDxEditBox*			m_pEditCtrlPlayerName;

	CCtrlButtonWZ*			m_pImgCtrl;

	CCtrlMultilineEditWZ*	m_pMultiEditCtrl;

	CCtrlButtonWZ*			m_pBtnCtrlOk[eCASH_ITEM_SHOP_PRESENT_CHECK_DLG_STATE_MAX];
	CCtrlButtonWZ*			m_pBtnCtrlCancle[eCASH_ITEM_SHOP_PRESENT_CHECK_DLG_STATE_MAX];


	CCtrlListWZ*			m_pListCtrl;
	CCtrlVScrollWZ*			m_pScroll;
	CCtrlPictureWZ*			m_pPicture;


	bool					m_bShow;
	bool                    m_bControlsInitialized;

	stRenderTooltipInfo		m_ItemCodeForTooltip;
	CASH_ITEM_IMG			m_PresentItemImg;

	stPresentItemInfo		m_PresentItemInfo;

#ifdef _GS_MOD_GSP_ITEMSHOP_TXT
	std::vector<util::_tstring> m_vecItemName;
#endif//_GS_MOD_GSP_ITEMSHOP_TXT

};


//------------------------------------------------------------------------------
/**
*/
inline
POSTYPE
uiCashItemShopPresentCheck::getCtrlIDToPos( WzID wzId )
{
	for( POSTYPE i = 0 ; i < eDIALOG_CASH_ITEM_SHOP_PRESENT_CHECK_MAX ; ++i )
	{
		if( m_wzId[i] == wzId )
		{
			return i;
		}
	}
	return eDIALOG_CASH_ITEM_SHOP_PRESENT_CHECK_MAX;
}

//------------------------------------------------------------------------------
/**
*/
inline
CControlWZ *
uiCashItemShopPresentCheck::getControl( POSTYPE AtPos )
{
	ASSERT( AtPos < eDIALOG_CASH_ITEM_SHOP_PRESENT_CHECK_MAX );

	return GetControlWZ( m_wzId[AtPos] );
}


//------------------------------------------------------------------------------
/**
*/
inline
uiCashItemShopMan*
uiCashItemShopPresentCheck::GetManager()
{
	return this->m_pCashItemShopMan;
}

//------------------------------------------------------------------------------ 
#endif //#ifndef _NA_0_20110613_GAME_SHOP_RENEWAL //#if'N'def
//------------------------------------------------------------------------------ 
