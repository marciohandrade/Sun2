#pragma once

//------------------------------------------------------------------------------ 
#ifndef _NA_0_20110613_GAME_SHOP_RENEWAL //#if'N'def
//------------------------------------------------------------------------------ 

//------------------------------------------------------------------------------
//
//    작업자: 김태우
//    uiCashItemShopPackageDlg class
//------------------------------------------------------------------------------

#include "uibase.h"

#include "uiCashItemShopMan/uiCashItemShopDef.h"

class uiCashItemShopMan;

class uiCashItemShopPackageDlg: public uiBase
{
	enum eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG
	{
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_EXIT = 0,			//	X 버튼

		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_IMG_ITEM1,			//	아이템 이미지 1
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_IMG_ITEM2,			//	아이템 이미지 2
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_IMG_ITEM3,			//	아이템 이미지 3
        eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_BTN_MYMILEGE,		//	해방군의 훈장 그림 버튼
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_CHK_CASHITEM,		//	캐시아이템 체크버튼
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_CHK_MYPAGE,			//	마이페이지 체크버튼
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_CHK_PACKAGE,			//	패키지 체크버튼
        eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_CHK_MAILAGE,			//	마일리지 체크버튼
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_CHK_ALLITEM,			//	모든아이템 체크버튼
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_CHK_NEWITEM,			//	신규상품 체크버튼
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_CHK_BESTITEM,		//	인기상품 체크버튼

        eDIALOG_CASH_ITEM_SHOP_DLG_CHK_EXTENSION,               //기간연장

		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE_NAME1,	//	패키지 이름1
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE_NAME2,	//	패키지 이름2
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE_NAME3,	//	패키지 이름3

		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE_CASH1,	//	패키지 가격1
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE_CASH2,	//	패키지 가격2
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE_CASH3,	//	패키지 가격3

		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_BTN_BUY1,			//	패키지 구입 버튼1
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_BTN_BUY2,			//	패키지 구입 버튼2
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_BTN_BUY3,			//	패키지 구입 버튼2

		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE1_ITEMNAME1,	//	패키지1 아이템 이름1
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE1_ITEMNAME2,	//	패키지1 아이템 이름2
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE1_ITEMNAME3,	//	패키지1 아이템 이름3
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE1_ITEMNAME4,	//	패키지1 아이템 이름4
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE1_ITEMNAME5,	//	패키지1 아이템 이름5
        eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE1_ITEMNAME6,	//	패키지1 아이템 이름6
        eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE1_ITEMNAME7,	//	패키지1 아이템 이름7

		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE2_ITEMNAME1,	//	패키지2 아이템 이름1
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE2_ITEMNAME2,	//	패키지2 아이템 이름2
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE2_ITEMNAME3,	//	패키지2 아이템 이름3
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE2_ITEMNAME4,	//	패키지2 아이템 이름4
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE2_ITEMNAME5,	//	패키지2 아이템 이름5
        eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE2_ITEMNAME6,	//	패키지2 아이템 이름6
        eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE2_ITEMNAME7,	//	패키지2 아이템 이름7

		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE3_ITEMNAME1,	//	패키지3 아이템 이름1
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE3_ITEMNAME2,	//	패키지3 아이템 이름2
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE3_ITEMNAME3,	//	패키지3 아이템 이름3
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE3_ITEMNAME4,	//	패키지3 아이템 이름4
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE3_ITEMNAME5,	//	패키지3 아이템 이름5
        eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE3_ITEMNAME6,	//	패키지3 아이템 이름6
        eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE3_ITEMNAME7,	//	패키지3 아이템 이름7

		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE1_ITEMUSETIME1,	//	패키지1 아이템 사용기간1
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE1_ITEMUSETIME2,	//	패키지1 아이템 사용기간2
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE1_ITEMUSETIME3,	//	패키지1 아이템 사용기간3
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE1_ITEMUSETIME4,	//	패키지1 아이템 사용기간4
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE1_ITEMUSETIME5,	//	패키지1 아이템 사용기간5
        eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE1_ITEMUSETIME6,	//	패키지1 아이템 사용기간6
        eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE1_ITEMUSETIME7,	//	패키지1 아이템 사용기간7

		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE2_ITEMUSETIME1,	//	패키지2 아이템 사용기간1
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE2_ITEMUSETIME2,	//	패키지2 아이템 사용기간2
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE2_ITEMUSETIME3,	//	패키지2 아이템 사용기간3
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE2_ITEMUSETIME4,	//	패키지2 아이템 사용기간4
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE2_ITEMUSETIME5,	//	패키지2 아이템 사용기간5
        eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE2_ITEMUSETIME6,	//	패키지2 아이템 사용기간6
        eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE2_ITEMUSETIME7,	//	패키지2 아이템 사용기간7

		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE3_ITEMUSETIME1,	//	패키지3 아이템 사용기간1
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE3_ITEMUSETIME2,	//	패키지3 아이템 사용기간2
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE3_ITEMUSETIME3,	//	패키지3 아이템 사용기간3
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE3_ITEMUSETIME4,	//	패키지3 아이템 사용기간4
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE3_ITEMUSETIME5,	//	패키지3 아이템 사용기간5
        eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE3_ITEMUSETIME6,	//	패키지3 아이템 사용기간6
        eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE3_ITEMUSETIME7,	//	패키지3 아이템 사용기간7

		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE1_ITEMAMOUT1,	//	패키지1 아이템 갯수1
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE1_ITEMAMOUT2,	//	패키지1 아이템 갯수2
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE1_ITEMAMOUT3,	//	패키지1 아이템 갯수3
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE1_ITEMAMOUT4,	//	패키지1 아이템 갯수4
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE1_ITEMAMOUT5,	//	패키지1 아이템 갯수5
        eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE1_ITEMAMOUT6,	//	패키지1 아이템 갯수6
        eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE1_ITEMAMOUT7,	//	패키지1 아이템 갯수7
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE2_ITEMAMOUT1,	//	패키지2 아이템 갯수1
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE2_ITEMAMOUT2,	//	패키지2 아이템 갯수2
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE2_ITEMAMOUT3,	//	패키지2 아이템 갯수3
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE2_ITEMAMOUT4,	//	패키지2 아이템 갯수4
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE2_ITEMAMOUT5,	//	패키지2 아이템 갯수5
        eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE2_ITEMAMOUT6,	//	패키지2 아이템 갯수6
        eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE2_ITEMAMOUT7,	//	패키지2 아이템 갯수7

		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE3_ITEMAMOUT1,	//	패키지3 아이템 갯수1
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE3_ITEMAMOUT2,	//	패키지3 아이템 갯수2
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE3_ITEMAMOUT3,	//	패키지3 아이템 갯수3
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE3_ITEMAMOUT4,	//	패키지3 아이템 갯수4
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE3_ITEMAMOUT5,	//	패키지3 아이템 갯수5
        eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE3_ITEMAMOUT6,	//	패키지3 아이템 갯수6
        eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE3_ITEMAMOUT7,	//	패키지3 아이템 갯수7
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_VSCR1,					//	패키지 스크롤1
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_VSCR2,					//	패키지 스크롤2
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_VSCR3,					//	패키지 스크롤3

		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_BTN_PREVPAGE,			//	이전 페이지 버튼
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_BTN_NEXTPAGE,			//	다음 페이지 버튼

		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_CURPAGE,				//	현재 페이지 텍스트

		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_MYMONEY,				//	현재 보유금액
        eDIALOG_CASH_ITEM_SHOP_DLG_BTN_WCOIN_BACK,                  //  Buy W Coin 배경
        eDIALOG_CASH_ITEM_SHOP_DLG_IMG_WCOINC_BACK,                 //  W Coin(c) 배경
        eDIALOG_CASH_ITEM_SHOP_DLG_IMG_WCOINP_BACK,                 //  W Coin(p) 배경
        eDIALOG_CASH_ITEM_SHOP_DLG_STXT_WCOINP,                     //  "W Coin(c)"
        eDIALOG_CASH_ITEM_SHOP_DLG_STXT_WCOINC,                     //  "W Coin(p)"
        eDIALOG_CASH_ITEM_SHOP_DLG_STXT_MILEAGE,                    //  "해방군의 훈장"
        eDIALOG_CASH_ITEM_SHOP_DLG_TXT_WCOINP,                      //  W Coin(p) "%d"
        eDIALOG_CASH_ITEM_SHOP_DLG_TXT_WCOINC,                      //  W Coin(c) "%d"
        eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_MYMILEGE,            //  해방군의 훈장 "%d"
        eDIALOG_CASH_ITEM_SHOP_DLG_BTN_BUYWCOIN,                    //  Buy W Coin 버튼

        eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_BTN_SORTCLASS,
        eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_LIST_SORTCLASS,
        eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TEXT_SORTCLASS,


		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_MAX,
	};

	enum eCASH_ITEM_SHOP_PACKAGE_DLG_CHK
	{
		eCASH_ITEM_SHOP_PACKAGE_DLG_CHK_CASHITEM,
		eCASH_ITEM_SHOP_PACKAGE_DLG_CHK_MY_PAGE,	
		eCASH_ITEM_SHOP_PACKAGE_DLG_CHK_PACKAGE,   
        eCASH_ITEM_SHOP_PACKAGE_DLG_CHK_MILEAGE,
		eCASH_ITEM_SHOP_PACKAGE_DLG_CHK_MAX,
	};

    enum eCASH_ITEM_SHOP_PACKAGE_TXT
	{
        eCASH_ITEM_SHOP_STXT_WCOINP,
        eCASH_ITEM_SHOP_STXT_WCOINC,
        eCASH_ITEM_SHOP_STXT_MILEAGE,
        eCASH_ITEM_SHOP_TXT_WCOINP,
        eCASH_ITEM_SHOP_TXT_WCOINC,
        eCASH_ITEM_SHOP_TXT_MILEAGE,
        eCASH_ITEM_SHOP_TXT_MAX,
	};
    enum eCASH_ITEM_SHOP_PACKAGE_IMG
	{
        eCASH_ITEM_SHOP_BTN_WCOIN_BACK, 
        eCASH_ITEM_SHOP_IMG_WCOINC_BACK,
        eCASH_ITEM_SHOP_IMG_WCOINP_BACK,
        eCASH_ITEM_SHOP_IMG_MAX,
	};

public:
	enum eCASH_ITEM_SHOP_PACKAGE_DLG_CHK_MENU
	{
		eCASH_ITEM_SHOP_PACKAGE_DLG_CHK_MENU_TOTAL,	
		eCASH_ITEM_SHOP_PACKAGE_DLG_CHK_MENU_NEW,		
		eCASH_ITEM_SHOP_PACKAGE_DLG_CHK_MENU_BEST,	
        eCASH_ITEM_SHOP_DLG_CHK_EXTENSION,
		eCASH_ITEM_SHOP_PACKAGE_DLG_CHK_MENU_MAX,  
	};

private:
	enum eCASH_ITEM_SHOP_PACKAGE_DLG_ITEM
	{
		eCASH_ITEM_SHOP_PACKAGE_DLG_ITEM1,
		eCASH_ITEM_SHOP_PACKAGE_DLG_ITEM2,
		eCASH_ITEM_SHOP_PACKAGE_DLG_ITEM3,

		eCASH_ITEM_SHOP_PACKAGE_DLG_ITEM_MAX,
	};

	enum eCASH_ITEM_SHOP_PACKAGE_DLG_DETAILITEM
	{
		eCASH_ITEM_SHOP_PACKAGE_DLG_DETAILITEM1,
		eCASH_ITEM_SHOP_PACKAGE_DLG_DETAILITEM2,
		eCASH_ITEM_SHOP_PACKAGE_DLG_DETAILITEM3,
		eCASH_ITEM_SHOP_PACKAGE_DLG_DETAILITEM4,
		eCASH_ITEM_SHOP_PACKAGE_DLG_DETAILITEM5,
        eCASH_ITEM_SHOP_PACKAGE_DLG_DETAILITEM6,
        eCASH_ITEM_SHOP_PACKAGE_DLG_DETAILITEM7,

		eCASH_ITEM_SHOP_PACKAGE_DLG_DETAILITEM_MAX,
	};

public:
	uiCashItemShopPackageDlg(InterfaceManager *pUIMan);
	virtual ~uiCashItemShopPackageDlg();

    static void Callback_ShopIconRender(CControlWZ* control_ptr, void* parameter);

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

	void					_setMenu(int iMenu, bool force_change = false);
	int						_getMenu()				{ return m_iCurMenu;	}
	void					_updateMenu();

	void					_setPage(int iPage);
	void					_setIndex(int iIndex);
	void					_prepareShow();

	void					_clearItem(int index);
	void					_clearAllItem();
	void					_setItem(int index, DWORD dwImageCode, TCHAR* szPackageName, int iStartIndex );
	void					_setDetialItem(int index, int iStartIndex = 0 );

	void					_addIconImage(int iIndex, DWORD dwCode);
	void					_removeIconImage(int iIndex);
	void					_removeAllIconImage();

	void					_renderTooltip();
	void					_renderIconImage();
public:
    WORD                    _getFilter();
private:
	BYTE					_getCategoryType(int iMenu);
public:
	void					SetItem(int index, DWORD dwImageCode, TCHAR* szPackageName, int iStartIndex = 0);
	void					SetCurPage( void );
	void					RefreshPackageItemList();
	void					SetCurMoney( void );

	//--------------------------------------------------------------------------
	// dialog functions
	//
public:

	int						GetCurPage()	 {return m_iCurPage;}
	int						GetCurMenu()	 {return m_iCurMenu;}
	virtual BYTE			GetCurCategoryType()	{return _getCategoryType(m_iCurMenu);}

protected:
	virtual void	        OnShowWindow( BOOL val );
	virtual void            OnUpdateSolarDialog();
	virtual void            OnRenderSolarDialog();

protected:
	CControlWZ *	        getControl( POSTYPE AtPos );
	POSTYPE			        getCtrlIDToPos( WzID wzId );
	static WzID		        m_wzId[eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_MAX];

	//--------------------------------------------------------------------------
	// procedure
	//
public:
	virtual void	        MessageProc( SI_MESSAGE * pMessage );   // Pure
	virtual void			MouseOver_MsgProc( SI_MESSAGE * pMessage );

private:
    void OnClickCombo(eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG e);
    bool OnSelectedCombo(eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG list_control_index, 
                        eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG static_control_index, 
                        int selected_index);

	//--------------------------------------------------------------------------
	// member vars
	//
private:
	uiCashItemShopMan*		m_pCashItemShopMan;
	CCtrlButtonCheckWZ*		m_pBtnCheckMenu[eCASH_ITEM_SHOP_PACKAGE_DLG_CHK_MENU_MAX];
	CCtrlButtonCheckWZ*		m_pBtnCheckTabs[eCASH_ITEM_SHOP_PACKAGE_DLG_CHK_MAX];
	CCtrlImageStaticWZ*		m_pImgCtrl[eCASH_ITEM_SHOP_PACKAGE_DLG_ITEM_MAX];
	CCtrlStaticWZ*			m_pTxtCtrlPackageName[eCASH_ITEM_SHOP_PACKAGE_DLG_ITEM_MAX];
	CCtrlStaticWZ*			m_pTxtCtrlPackageInfo[eCASH_ITEM_SHOP_PACKAGE_DLG_ITEM_MAX];
	CCtrlStaticWZ*			m_pTxtCtrlItemName[eCASH_ITEM_SHOP_PACKAGE_DLG_ITEM_MAX][eCASH_ITEM_SHOP_PACKAGE_DLG_DETAILITEM_MAX];
	CCtrlStaticWZ*			m_pTxtCtrlItemUseTime[eCASH_ITEM_SHOP_PACKAGE_DLG_ITEM_MAX][eCASH_ITEM_SHOP_PACKAGE_DLG_DETAILITEM_MAX];
	CCtrlStaticWZ*			m_pTxtCtrlItemAmout[eCASH_ITEM_SHOP_PACKAGE_DLG_ITEM_MAX][eCASH_ITEM_SHOP_PACKAGE_DLG_DETAILITEM_MAX];
	CCtrlStaticWZ*			m_pTxtCtrlMoney;

    CCtrlStaticWZ*			m_pTxtCtrlPPCoin;	// PP캐시
    CCtrlStaticWZ*			m_pTxtCtrlCCCoin;	// 일반캐시
    CCtrlStaticWZ*			m_pTxtCtrlMilege;	// 마일리지

	CCtrlStaticWZ*			m_pTxtCtrlCurPage;
	CCtrlVScrollWZ*			m_pScroll[eCASH_ITEM_SHOP_PACKAGE_DLG_ITEM_MAX];
	CASH_ITEM_IMG			m_PackageItemImg[eCASH_ITEM_SHOP_PACKAGE_DLG_ITEM_MAX];
	bool					m_bShow;
	bool                    m_bControlsInitialized;
	stRenderTooltipInfoforPackage m_ItemCodeForTooltip;
public:
	int						m_iCurMenu;
	int						m_iCurPage;
};

//------------------------------------------------------------------------------
/**
*/
inline
POSTYPE
uiCashItemShopPackageDlg::getCtrlIDToPos( WzID wzId )
{
	for( POSTYPE i = 0 ; i < eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_MAX ; ++i )
	{
		if( m_wzId[i] == wzId )
		{
			return i;
		}
	}
	return eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_MAX;
}

//------------------------------------------------------------------------------
/**
*/
inline
CControlWZ *
uiCashItemShopPackageDlg::getControl( POSTYPE AtPos )
{
	ASSERT( AtPos < eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_MAX );

	return GetControlWZ( m_wzId[AtPos] );
}

//------------------------------------------------------------------------------
/**
*/
inline
uiCashItemShopMan*
uiCashItemShopPackageDlg::GetManager()
{
	return this->m_pCashItemShopMan;
}

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------ 
#endif //#ifndef _NA_0_20110613_GAME_SHOP_RENEWAL //#if'N'def
//------------------------------------------------------------------------------ 
