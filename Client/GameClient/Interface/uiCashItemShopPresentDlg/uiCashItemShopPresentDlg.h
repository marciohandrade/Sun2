#pragma once

//------------------------------------------------------------------------------ 
#ifndef _NA_0_20110613_GAME_SHOP_RENEWAL //#if'N'def
//------------------------------------------------------------------------------ 

//------------------------------------------------------------------------------
//
//    작업자: 김태우
//    uiCashItemShopPresentDlg class
//------------------------------------------------------------------------------

#include "uibase.h"

#include "uiCashItemShopMan/uiCashItemShopDef.h"

class uiCashItemShopMan;

class uiCashItemShopPresentDlg: public uiBase
{
	enum eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG
	{
		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_BTN_EXIT = 0,	//	X 버튼

		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_IMG_ITEM1,		//	아이템 이미지1
		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_IMG_ITEM2,		//	아이템 이미지2
		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_IMG_ITEM3,		//	아이템 이미지3
		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_IMG_ITEM4,		//	아이템 이미지4
		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_IMG_ITEM5,		//	아이템 이미지5
		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_IMG_ITEM6,		//	아이템 이미지6
        eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_BTN_MILEGE,

		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_CHK_CASH,		//	캐시아이템 체크버튼
		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_CHK_MYPAGE,		//	마이페이지 체크버튼
		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_CHK_PACKAGE,		//	패키지샾 체크버튼

        eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_CHK_MILEAGE,		//	마일리지 체크버튼
		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_CHK_PRESENT,		//	받은 선물함 체크버튼

		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMNAME1,	//	아이템 이름1
		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMNAME2,	//	아이템 이름2
		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMNAME3,	//	아이템 이름3
		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMNAME4,	//	아이템 이름4
		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMNAME5,	//	아이템 이름5
		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMNAME6,	//	아이템 이름6

		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMDESC1,	//	아이템 간단설명1
		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMDESC2,	//	아이템 간단설명2
		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMDESC3,	//	아이템 간단설명3
		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMDESC4,	//	아이템 간단설명4
		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMDESC5,	//	아이템 간단설명5
		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMDESC6,	//	아이템 간단설명6

		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_PLAYERNAME1,	//	선물한 플레이어 이름1
		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_PLAYERNAME2,	//	선물한 플레이어 이름2
		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_PLAYERNAME3,	//	선물한 플레이어 이름3
		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_PLAYERNAME4,	//	선물한 플레이어 이름4
		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_PLAYERNAME5,	//	선물한 플레이어 이름5
		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_PLAYERNAME6,	//	선물한 플레이어 이름6

		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMSENDDAY1,//	선물한 날짜1
		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMSENDDAY2,//	선물한 날짜2
		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMSENDDAY3,//	선물한 날짜3
		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMSENDDAY4,//	선물한 날짜4
		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMSENDDAY5,//	선물한 날짜5
		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMSENDDAY6,//	선물한 날짜6

		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMUSETIME1,//	선물 사용기간1
		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMUSETIME2,//	선물 사용기간2
		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMUSETIME3,//	선물 사용기간3
		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMUSETIME4,//	선물 사용기간4
		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMUSETIME5,//	선물 사용기간5
		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMUSETIME6,//	선물 사용기간6
		
		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMCOUNT1,	//	선물 갯수1
		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMCOUNT2,	//	선물 갯수2
		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMCOUNT3,	//	선물 갯수3
		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMCOUNT4,	//	선물 갯수4
		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMCOUNT5,	//	선물 갯수5
		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMCOUNT6,	//	선물 갯수6

		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_BTN_PRESENTLISTDEL1,	//	목록 삭제 버튼1
		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_BTN_PRESENTLISTDEL2,	//	목록 삭제 버튼2
		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_BTN_PRESENTLISTDEL3,	//	목록 삭제 버튼3
		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_BTN_PRESENTLISTDEL4,	//	목록 삭제 버튼4
		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_BTN_PRESENTLISTDEL5,	//	목록 삭제 버튼5
		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_BTN_PRESENTLISTDEL6,	//	목록 삭제 버튼6

		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_BTN_PRESENTLISTCHECK1,	//	선물 확인 버튼1
		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_BTN_PRESENTLISTCHECK2,	//	선물 확인 버튼2
		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_BTN_PRESENTLISTCHECK3,	//	선물 확인 버튼3
		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_BTN_PRESENTLISTCHECK4,	//	선물 확인 버튼4
		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_BTN_PRESENTLISTCHECK5,	//	선물 확인 버튼5
		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_BTN_PRESENTLISTCHECK6,	//	선물 확인 버튼6

		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_MONEY,				//	소지금

		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_BTN_PREV_PAGE,			//	이전 페이지 버튼
		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_BTN_NEXT_PAGE,			//	다음 페이지 버튼

		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_CURPAGE,				//	현재 페이지
        eDIALOG_CASH_ITEM_SHOP_PRESENT_IMG_WCOIN_BACK,
        eDIALOG_CASH_ITEM_SHOP_PRESENT_IMG_WCOINC_BACK,
        eDIALOG_CASH_ITEM_SHOP_PRESENT_IMG_WCOINP_BACK,
        eDIALOG_CASH_ITEM_SHOP_PRESENT_STXT_WCOINC,
        eDIALOG_CASH_ITEM_SHOP_PRESENT_STXT_WCOINP,
        eDIALOG_CASH_ITEM_SHOP_PRESENT_STXT_MILEAGE,
        eDIALOG_CASH_ITEM_SHOP_PRESENT_TXT_WCOINC,
        eDIALOG_CASH_ITEM_SHOP_PRESENT_TXT_WCOINP,
        eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_MILEGE,
        eDIALOG_CASH_ITEM_SHOP_PRESENT_BTN_BUY_WCOIN,
        eDIALOG_CASH_ITEM_SHOP_PRESENT_BTN_WCOIN,

        eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_CHK_SEND,		//	보낸 선물함 체크버튼
		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_MAX,
	};

	enum eCASH_ITEM_SHOP_PRESENT_DLG_CHK
	{
		eCASH_ITEM_SHOP_PRESENT_DLG_CHK_CASH_ITEM,
		eCASH_ITEM_SHOP_PRESENT_DLG_CHK_MY_PAGE,	
		eCASH_ITEM_SHOP_PRESENT_DLG_CHK_PACKAGE,
        eCASH_ITEM_SHOP_PRESENT_DLG_CHK_MILEAGE,
		eCASH_ITEM_SHOP_PRESENT_DLG_CHK_MAX,
	};

	enum eCASH_ITEM_SHOP_PRESENT_DLG_CHK_MENU
	{
        eCASH_ITEM_SHOP_PRESENT_DLG_CHK_PRESENT,
		eCASH_ITEM_SHOP_PRESENT_DLG_CHK_MENU_MAX,  
	};

	enum eCASH_ITEM_SHOP_PRESENT_DLG_ITEM
	{
		eCASH_ITEM_SHOP_PRESENT_DLG_ITEM1,
		eCASH_ITEM_SHOP_PRESENT_DLG_ITEM2,
		eCASH_ITEM_SHOP_PRESENT_DLG_ITEM3,
		eCASH_ITEM_SHOP_PRESENT_DLG_ITEM4,
		eCASH_ITEM_SHOP_PRESENT_DLG_ITEM5,
		eCASH_ITEM_SHOP_PRESENT_DLG_ITEM6,

		eCASH_ITEM_SHOP_PRESENT_DLG_ITEM_MAX,
	};

    enum eCASH_ITEM_SHOP_PACKAGE_TXT
	{
        eCASH_ITEM_SHOP_PRESENT_STXT_WCOINP,
        eCASH_ITEM_SHOP_PRESENT_STXT_WCOINC,
        eCASH_ITEM_SHOP_PRESENT_STXT_MILEAGE,
        eCASH_ITEM_SHOP_PRESENT_TXT_WCOINP,
        eCASH_ITEM_SHOP_PRESENT_TXT_WCOINC,
        eCASH_ITEM_SHOP_PRESENT_TXT_MILEAGE,
        eCASH_ITEM_SHOP_PRESENT_TXT_MAX,
	};

    enum eCASH_ITEM_SHOP_PACKAGE_IMG
	{
        eCASH_ITEM_SHOP_PRESENT_IMG_WCOIN_BACK, 
        eCASH_ITEM_SHOP_PRESENT_IMG_WCOINC_BACK,
        eCASH_ITEM_SHOP_PRESENT_IMG_WCOINP_BACK,
        eCASH_ITEM_SHOP_PRESENT_IMG_MAX,
	};

public:
	uiCashItemShopPresentDlg(InterfaceManager *pUIMan);
	virtual ~uiCashItemShopPresentDlg();

	//--------------------------------------------------------------------------
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
	uiCashItemShopMan*		GetManager();

	//--------------------------------------------------------------------------
	// functions
	//
private:
	BOOL					_isEnable();
    void					_updateMenu(eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG e);

	void					_setIndex(int iIndex);
	void					_setPage(int iPage = 0);

	void					_reSetChkBtn();

	void					_prepareShow();

	void					_clearItem(int index);
	void					_clearAllItem();
	void					_setItem(int index, CODETYPE itemCode);

	void					_addIconImage(int iIndex, DWORD dwCode, CODETYPE itemCode);
	void					_removeIconImage(int iIndex);
	void					_removeAllIconImage();

	void					_renderTooltip();
	void					_renderIconImage();

public:
	void					SetItem(int index, CODETYPE itemCode);
	void					SetCurPage( void );
	void					RefreshPresentItemList();
	void					SetCurMoney( void );


	//--------------------------------------------------------------------------
	// dialog functions
	//
public:
	int						GetCurIdex()						{return m_iCurIndex;}
	int						GetCurPage()						{return m_iCurPage;}
	int						GetTotalPage()						{return m_iTotalPage;}
	int						GetPresentCount()					{return m_iPresentCount;}
	void					SetPresentCount(int iPresentCount)
	{
		m_iPresentCount = iPresentCount;
		m_iTotalPage = ((iPresentCount-1) / _PRESENT_ITEM_PER_PAGE);
	}

	virtual void	        OnShowWindow( BOOL val );
	virtual void            OnUpdateSolarDialog();
	virtual void            OnRenderSolarDialog();

protected:
	CControlWZ *			getControl( POSTYPE AtPos );
	POSTYPE					getCtrlIDToPos( WzID wzId );
	static WzID		        m_wzId[eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_MAX];

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
	CCtrlButtonCheckWZ*		m_pBtnCheckMenu[eCASH_ITEM_SHOP_PRESENT_DLG_CHK_MENU_MAX];
	CCtrlButtonCheckWZ*		m_pBtnCheckTabs[eCASH_ITEM_SHOP_PRESENT_DLG_CHK_MAX];
	CCtrlButtonWZ*			m_pBtnPresentListDel[eCASH_ITEM_SHOP_PRESENT_DLG_ITEM_MAX];
	CCtrlButtonWZ*			m_pBtnPresentListCheck[eCASH_ITEM_SHOP_PRESENT_DLG_ITEM_MAX];
	CCtrlImageStaticWZ*		m_pImgCtrl[eCASH_ITEM_SHOP_PRESENT_DLG_ITEM_MAX];
	CCtrlStaticWZ*			m_pTxtCtrlName[eCASH_ITEM_SHOP_PRESENT_DLG_ITEM_MAX];
	CCtrlStaticWZ*			m_pTxtCtrlMemo[eCASH_ITEM_SHOP_PRESENT_DLG_ITEM_MAX];
	CCtrlStaticWZ*			m_pTxtCtrlPlayerName[eCASH_ITEM_SHOP_PRESENT_DLG_ITEM_MAX];
	CCtrlStaticWZ*			m_pTxtCtrlSendDay[eCASH_ITEM_SHOP_PRESENT_DLG_ITEM_MAX];
	CCtrlStaticWZ*			m_pTxtCtrlUseTime[eCASH_ITEM_SHOP_PRESENT_DLG_ITEM_MAX];
	CCtrlStaticWZ*			m_pTxtCtrlItemCount[eCASH_ITEM_SHOP_PRESENT_DLG_ITEM_MAX];
	CASH_ITEM_IMG			m_CashItemImg[eCASH_ITEM_SHOP_PRESENT_DLG_ITEM_MAX];
	CCtrlStaticWZ*			m_pTxtCtrlCurPage;
    CCtrlStaticWZ*			m_pTxtCtrlMoney;	// 테섭에선 하임으로, 나머지는 캐시로..
    CCtrlStaticWZ*			m_pTxtCtrlPPCoin;	// PP캐시
    CCtrlStaticWZ*			m_pTxtCtrlCCCoin;	// 일반캐시
    CCtrlStaticWZ*			m_pTxtCtrlMilege;	// 마일리지

	bool					m_bShow;
	bool                    m_bControlsInitialized;

	stRenderTooltipInfo		m_ItemCodeForTooltip;

	int						m_iTotalPage;
	int						m_iPresentCount;
	int						m_iCurPage;
	int						m_iCurIndex;

};

//------------------------------------------------------------------------------
/**
*/
inline
POSTYPE
uiCashItemShopPresentDlg::getCtrlIDToPos( WzID wzId )
{
	for( POSTYPE i = 0 ; i < eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_MAX; ++i )
	{
		if( m_wzId[i] == wzId )
		{
			return i;
		}
	}
	return eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_MAX;
}

//------------------------------------------------------------------------------
/**
*/
inline
CControlWZ *
uiCashItemShopPresentDlg::getControl( POSTYPE AtPos )
{
	ASSERT( AtPos < eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_MAX);

	return GetControlWZ( m_wzId[AtPos] );
}

//------------------------------------------------------------------------------
/**
*/
inline
uiCashItemShopMan*
uiCashItemShopPresentDlg::GetManager()
{
	return this->m_pCashItemShopMan;
}

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------ 
#endif //#ifndef _NA_0_20110613_GAME_SHOP_RENEWAL //#if'N'def
//------------------------------------------------------------------------------ 
