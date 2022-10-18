#pragma once

//------------------------------------------------------------------------------ 
#ifndef _NA_0_20110613_GAME_SHOP_RENEWAL //#if'N'def
//------------------------------------------------------------------------------ 

//------------------------------------------------------------------------------
//
//    작업자: 김태우
//    uiCashItemShopPresentComplete class
//------------------------------------------------------------------------------

#include "uibase.h"

#include "uiCashItemShopMan/uiCashItemShopDef.h"

class uiCashItemShopMan;

class uiCashItemShopPresentComplete: public uiBase
{
	enum eDIALOG_CASH_ITEM_SHOP_PRESENT_COMPLETE
	{
		eDIALOG_CASH_ITEM_SHOP_PRESENT_COMPLETE_BTN_EXIT,			// X 버튼

		eDIALOG_CASH_ITEM_SHOP_PRESENT_COMPLETE_IMG_ITEM,			// 선물 이미지

		eDIALOG_CASH_ITEM_SHOP_PRESENT_COMPLETE_TXT_SEND,			// 선물확인 텍스트
		eDIALOG_CASH_ITEM_SHOP_PRESENT_COMPLETE_TXT_RCV,			// 선물받기 텍스트
		eDIALOG_CASH_ITEM_SHOP_PRESENT_COMPLETE_TXT_RETURN,		// 선물돌려보내기 텍스트

		eDIALOG_CASH_ITEM_SHOP_PRESENT_COMPLETE_TXT_NAME,			// 선물이름

		eDIALOG_CASH_ITEM_SHOP_PRESENT_COMPLETE_TXT_MESSAGE1,		// "%d님께 선물했습니다." 메시지
		eDIALOG_CASH_ITEM_SHOP_PRESENT_COMPLETE_TXT_MESSAGE2,		// "위 아이템을 선물로 받았습니다" 메시지
		eDIALOG_CASH_ITEM_SHOP_PRESENT_COMPLETE_TXT_MESSAGE3,		// "선물을 돌려 보냈습니다." 메시지

		eDIALOG_CASH_ITEM_SHOP_PRESENT_COMPLETE_BTN_OK,			// 확인 버튼

		eDIALOG_CASH_ITEM_SHOP_PRESENT_COMPLETE_MAX,
	};

	enum eCASH_ITEM_SHOP_PRESENT_COMPLETE_DLG_STATE
	{
		eCASH_ITEM_SHOP_PRESENT_COMPLETE_DLG_STATE_SEND,
		eCASH_ITEM_SHOP_PRESENT_COMPLETE_DLG_STATE_RCV,
		eCASH_ITEM_SHOP_PRESENT_COMPLETE_DLG_STATE_RETURN,

		eCASH_ITEM_SHOP_PRESENT_COMPLETE_DLG_STATE_MAX,
	};

public:
	uiCashItemShopPresentComplete(InterfaceManager *pUIMan);
	virtual ~uiCashItemShopPresentComplete();

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
#ifdef _GS_MOD_GSP_ITEMSHOP_TXT
	void					_renderItemName();
	void					_spliteItemName(TCHAR *pItemName);
#endif//_GS_MOD_GSP_ITEMSHOP_TXT
	//--------------------------------------------------------------------------
	// dialog functions
	//
public:
	void					ShowPresentItemComplete(stPresentItemInfo* pInfo);

protected:
	virtual void	        OnShowWindow( BOOL val );
	virtual void            OnUpdateSolarDialog();
	virtual void            OnRenderSolarDialog();

	CControlWZ *	        getControl( POSTYPE AtPos );
	POSTYPE			        getCtrlIDToPos( WzID wzId );
	static WzID		        m_wzId[eDIALOG_CASH_ITEM_SHOP_PRESENT_COMPLETE_MAX];

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

	CCtrlStaticWZ*			m_pTxtCtrlPresentName;
	CCtrlStaticWZ*			m_pTxtCtrlTitle[eCASH_ITEM_SHOP_PRESENT_COMPLETE_DLG_STATE_MAX];
	CCtrlStaticWZ*			m_pTxtCtrlMessage[eCASH_ITEM_SHOP_PRESENT_COMPLETE_DLG_STATE_MAX];

	CCtrlButtonWZ*			m_pImgCtrl;

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
uiCashItemShopPresentComplete::getCtrlIDToPos( WzID wzId )
{
	for( POSTYPE i = 0 ; i < eDIALOG_CASH_ITEM_SHOP_PRESENT_COMPLETE_MAX ; ++i )
	{
		if( m_wzId[i] == wzId )
		{
			return i;
		}
	}
	return eDIALOG_CASH_ITEM_SHOP_PRESENT_COMPLETE_MAX;
}

//------------------------------------------------------------------------------
/**
*/
inline
CControlWZ *
uiCashItemShopPresentComplete::getControl( POSTYPE AtPos )
{
	ASSERT( AtPos < eDIALOG_CASH_ITEM_SHOP_PRESENT_COMPLETE_MAX );

	return GetControlWZ( m_wzId[AtPos] );
}

//------------------------------------------------------------------------------
/**
*/
inline
uiCashItemShopMan*
uiCashItemShopPresentComplete::GetManager()
{
	return this->m_pCashItemShopMan;
}

//------------------------------------------------------------------------------ 
#endif //#ifndef _NA_0_20110613_GAME_SHOP_RENEWAL //#if'N'def
//------------------------------------------------------------------------------ 
