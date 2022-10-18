#pragma once

//------------------------------------------------------------------------------ 
#ifndef _NA_0_20110613_GAME_SHOP_RENEWAL //#if'N'def
//------------------------------------------------------------------------------ 

//------------------------------------------------------------------------------
//
//    작업자: 김태우
//    uiCashItemShopPresentCheck class
//------------------------------------------------------------------------------

#include "uibase.h"

#include "uiCashItemShopMan/uiCashItemShopDef.h"

class uiCashItemShopMan;

class uiCashItemShopPresentCheck: public uiBase
{
	enum eDIALOG_CASH_ITEM_SHOP_PRESENT_CHECK
	{
		eDIALOG_CASH_ITEM_SHOP_PRESENT_CHECK_BTN_EXIT,			// X 버튼

		eDIALOG_CASH_ITEM_SHOP_PRESENT_CHECK_IMG_ITEM,			// 선물 이미지

		eDIALOG_CASH_ITEM_SHOP_PRESENT_CHECK_TXT_PRESENT_SEND,	// 선물보내기 텍스트
		eDIALOG_CASH_ITEM_SHOP_PRESENT_CHECK_TXT_PRESENT_REV,	// 선물확인 텍스트

		eDIALOG_CASH_ITEM_SHOP_PRESENT_CHECK_TXT_PRESENT_NAME,	// 선물이름 텍스트

		eDIALOG_CASH_ITEM_SHOP_PRESENT_CHECK_TXT_USETIME,		// 선물 사용기간
		eDIALOG_CASH_ITEM_SHOP_PRESENT_CHECK_TXT_MONEY,			// 선물 가격
		eDIALOG_CASH_ITEM_SHOP_PRESENT_CHECK_TXT_COUNT,			// 선물 갯수
		
		eDIALOG_CASH_ITEM_SHOP_PRESENT_CHECK_EDIT_PLAYERNAME,	// 선물할 플레이어이름
		
		eDIALOG_CASH_ITEM_SHOP_PRESENT_CHECK_TXT_PLAYERNAME,	// 선물한 플레이어이름

		eDIALOG_CASH_ITEM_SHOP_PRESENT_CHECK_TXT_NAME_MESSAGE1,	// "님에게 선물로 보냅니다" 텍스트
		eDIALOG_CASH_ITEM_SHOP_PRESENT_CHECK_TXT_NAME_MESSAGE2,	// "님이 보낸 선물입니다" 텍스트

		eDIALOG_CASH_ITEM_SHOP_PRESENT_CHECK_TXT_EDIT_MESSAGE1,	// "받을 분에게 보낼 메시지" 텍스트
		eDIALOG_CASH_ITEM_SHOP_PRESENT_CHECK_TXT_EDIT_MESSAGE2,	// "메시지" 텍스트

		eDIALOG_CASH_ITEM_SHOP_PRESENT_CHECK_BTN_OK,			// 확인 버튼
		eDIALOG_CASH_ITEM_SHOP_PRESENT_CHECK_BTN_RETURN,		// 돌려보내기 버튼

		eDIALOG_CASH_ITEM_SHOP_PRESENT_CHECK_BTN_CANCLE,		// 취소 버튼
		eDIALOG_CASH_ITEM_SHOP_PRESENT_CHECK_BTN_REV,			// 선물받기 버튼

		eDIALOG_CASH_ITEM_SHOP_PRESENT_CHECK_MULTIEDIT_MSG,		// 메시지 입력 멀티에디트

		eDIALOG_CASH_ITEM_SHOP_PRESENT_CHECK_LIST,				// 메시지 출력 리스트 컨트롤
		eDIALOG_CASH_ITEM_SHOP_PRESENT_CHECK_SCROLL,			// 메시지 출력 스크롤 컨트롤
		eDIALOG_CASH_ITEM_SHOP_PRESENT_CHECK_PICTURE,			// 메시지 출력 픽쳐 컨트롤

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
