#pragma once

//------------------------------------------------------------------------------ 
#ifndef _NA_0_20110613_GAME_SHOP_RENEWAL //#if'N'def
//------------------------------------------------------------------------------ 

//------------------------------------------------------------------------------
//
//    �۾���: ���¿�
//    uiCashItemShopPackageDlg class
//------------------------------------------------------------------------------

#include "uibase.h"

#include "uiCashItemShopMan/uiCashItemShopDef.h"

class uiCashItemShopMan;

class uiCashItemShopPackageDlg: public uiBase
{
	enum eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG
	{
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_EXIT = 0,			//	X ��ư

		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_IMG_ITEM1,			//	������ �̹��� 1
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_IMG_ITEM2,			//	������ �̹��� 2
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_IMG_ITEM3,			//	������ �̹��� 3
        eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_BTN_MYMILEGE,		//	�ع決�� ���� �׸� ��ư
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_CHK_CASHITEM,		//	ĳ�þ����� üũ��ư
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_CHK_MYPAGE,			//	���������� üũ��ư
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_CHK_PACKAGE,			//	��Ű�� üũ��ư
        eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_CHK_MAILAGE,			//	���ϸ��� üũ��ư
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_CHK_ALLITEM,			//	�������� üũ��ư
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_CHK_NEWITEM,			//	�űԻ�ǰ üũ��ư
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_CHK_BESTITEM,		//	�α��ǰ üũ��ư

        eDIALOG_CASH_ITEM_SHOP_DLG_CHK_EXTENSION,               //�Ⱓ����

		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE_NAME1,	//	��Ű�� �̸�1
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE_NAME2,	//	��Ű�� �̸�2
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE_NAME3,	//	��Ű�� �̸�3

		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE_CASH1,	//	��Ű�� ����1
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE_CASH2,	//	��Ű�� ����2
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE_CASH3,	//	��Ű�� ����3

		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_BTN_BUY1,			//	��Ű�� ���� ��ư1
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_BTN_BUY2,			//	��Ű�� ���� ��ư2
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_BTN_BUY3,			//	��Ű�� ���� ��ư2

		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE1_ITEMNAME1,	//	��Ű��1 ������ �̸�1
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE1_ITEMNAME2,	//	��Ű��1 ������ �̸�2
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE1_ITEMNAME3,	//	��Ű��1 ������ �̸�3
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE1_ITEMNAME4,	//	��Ű��1 ������ �̸�4
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE1_ITEMNAME5,	//	��Ű��1 ������ �̸�5
        eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE1_ITEMNAME6,	//	��Ű��1 ������ �̸�6
        eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE1_ITEMNAME7,	//	��Ű��1 ������ �̸�7

		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE2_ITEMNAME1,	//	��Ű��2 ������ �̸�1
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE2_ITEMNAME2,	//	��Ű��2 ������ �̸�2
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE2_ITEMNAME3,	//	��Ű��2 ������ �̸�3
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE2_ITEMNAME4,	//	��Ű��2 ������ �̸�4
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE2_ITEMNAME5,	//	��Ű��2 ������ �̸�5
        eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE2_ITEMNAME6,	//	��Ű��2 ������ �̸�6
        eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE2_ITEMNAME7,	//	��Ű��2 ������ �̸�7

		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE3_ITEMNAME1,	//	��Ű��3 ������ �̸�1
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE3_ITEMNAME2,	//	��Ű��3 ������ �̸�2
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE3_ITEMNAME3,	//	��Ű��3 ������ �̸�3
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE3_ITEMNAME4,	//	��Ű��3 ������ �̸�4
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE3_ITEMNAME5,	//	��Ű��3 ������ �̸�5
        eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE3_ITEMNAME6,	//	��Ű��3 ������ �̸�6
        eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE3_ITEMNAME7,	//	��Ű��3 ������ �̸�7

		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE1_ITEMUSETIME1,	//	��Ű��1 ������ ���Ⱓ1
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE1_ITEMUSETIME2,	//	��Ű��1 ������ ���Ⱓ2
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE1_ITEMUSETIME3,	//	��Ű��1 ������ ���Ⱓ3
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE1_ITEMUSETIME4,	//	��Ű��1 ������ ���Ⱓ4
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE1_ITEMUSETIME5,	//	��Ű��1 ������ ���Ⱓ5
        eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE1_ITEMUSETIME6,	//	��Ű��1 ������ ���Ⱓ6
        eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE1_ITEMUSETIME7,	//	��Ű��1 ������ ���Ⱓ7

		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE2_ITEMUSETIME1,	//	��Ű��2 ������ ���Ⱓ1
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE2_ITEMUSETIME2,	//	��Ű��2 ������ ���Ⱓ2
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE2_ITEMUSETIME3,	//	��Ű��2 ������ ���Ⱓ3
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE2_ITEMUSETIME4,	//	��Ű��2 ������ ���Ⱓ4
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE2_ITEMUSETIME5,	//	��Ű��2 ������ ���Ⱓ5
        eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE2_ITEMUSETIME6,	//	��Ű��2 ������ ���Ⱓ6
        eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE2_ITEMUSETIME7,	//	��Ű��2 ������ ���Ⱓ7

		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE3_ITEMUSETIME1,	//	��Ű��3 ������ ���Ⱓ1
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE3_ITEMUSETIME2,	//	��Ű��3 ������ ���Ⱓ2
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE3_ITEMUSETIME3,	//	��Ű��3 ������ ���Ⱓ3
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE3_ITEMUSETIME4,	//	��Ű��3 ������ ���Ⱓ4
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE3_ITEMUSETIME5,	//	��Ű��3 ������ ���Ⱓ5
        eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE3_ITEMUSETIME6,	//	��Ű��3 ������ ���Ⱓ6
        eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE3_ITEMUSETIME7,	//	��Ű��3 ������ ���Ⱓ7

		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE1_ITEMAMOUT1,	//	��Ű��1 ������ ����1
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE1_ITEMAMOUT2,	//	��Ű��1 ������ ����2
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE1_ITEMAMOUT3,	//	��Ű��1 ������ ����3
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE1_ITEMAMOUT4,	//	��Ű��1 ������ ����4
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE1_ITEMAMOUT5,	//	��Ű��1 ������ ����5
        eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE1_ITEMAMOUT6,	//	��Ű��1 ������ ����6
        eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE1_ITEMAMOUT7,	//	��Ű��1 ������ ����7
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE2_ITEMAMOUT1,	//	��Ű��2 ������ ����1
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE2_ITEMAMOUT2,	//	��Ű��2 ������ ����2
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE2_ITEMAMOUT3,	//	��Ű��2 ������ ����3
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE2_ITEMAMOUT4,	//	��Ű��2 ������ ����4
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE2_ITEMAMOUT5,	//	��Ű��2 ������ ����5
        eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE2_ITEMAMOUT6,	//	��Ű��2 ������ ����6
        eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE2_ITEMAMOUT7,	//	��Ű��2 ������ ����7

		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE3_ITEMAMOUT1,	//	��Ű��3 ������ ����1
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE3_ITEMAMOUT2,	//	��Ű��3 ������ ����2
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE3_ITEMAMOUT3,	//	��Ű��3 ������ ����3
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE3_ITEMAMOUT4,	//	��Ű��3 ������ ����4
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE3_ITEMAMOUT5,	//	��Ű��3 ������ ����5
        eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE3_ITEMAMOUT6,	//	��Ű��3 ������ ����6
        eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE3_ITEMAMOUT7,	//	��Ű��3 ������ ����7
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_VSCR1,					//	��Ű�� ��ũ��1
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_VSCR2,					//	��Ű�� ��ũ��2
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_VSCR3,					//	��Ű�� ��ũ��3

		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_BTN_PREVPAGE,			//	���� ������ ��ư
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_BTN_NEXTPAGE,			//	���� ������ ��ư

		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_CURPAGE,				//	���� ������ �ؽ�Ʈ

		eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_MYMONEY,				//	���� �����ݾ�
        eDIALOG_CASH_ITEM_SHOP_DLG_BTN_WCOIN_BACK,                  //  Buy W Coin ���
        eDIALOG_CASH_ITEM_SHOP_DLG_IMG_WCOINC_BACK,                 //  W Coin(c) ���
        eDIALOG_CASH_ITEM_SHOP_DLG_IMG_WCOINP_BACK,                 //  W Coin(p) ���
        eDIALOG_CASH_ITEM_SHOP_DLG_STXT_WCOINP,                     //  "W Coin(c)"
        eDIALOG_CASH_ITEM_SHOP_DLG_STXT_WCOINC,                     //  "W Coin(p)"
        eDIALOG_CASH_ITEM_SHOP_DLG_STXT_MILEAGE,                    //  "�ع決�� ����"
        eDIALOG_CASH_ITEM_SHOP_DLG_TXT_WCOINP,                      //  W Coin(p) "%d"
        eDIALOG_CASH_ITEM_SHOP_DLG_TXT_WCOINC,                      //  W Coin(c) "%d"
        eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_MYMILEGE,            //  �ع決�� ���� "%d"
        eDIALOG_CASH_ITEM_SHOP_DLG_BTN_BUYWCOIN,                    //  Buy W Coin ��ư

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

    CCtrlStaticWZ*			m_pTxtCtrlPPCoin;	// PPĳ��
    CCtrlStaticWZ*			m_pTxtCtrlCCCoin;	// �Ϲ�ĳ��
    CCtrlStaticWZ*			m_pTxtCtrlMilege;	// ���ϸ���

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
