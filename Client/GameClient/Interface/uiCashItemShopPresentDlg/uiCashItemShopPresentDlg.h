#pragma once

//------------------------------------------------------------------------------ 
#ifndef _NA_0_20110613_GAME_SHOP_RENEWAL //#if'N'def
//------------------------------------------------------------------------------ 

//------------------------------------------------------------------------------
//
//    �۾���: ���¿�
//    uiCashItemShopPresentDlg class
//------------------------------------------------------------------------------

#include "uibase.h"

#include "uiCashItemShopMan/uiCashItemShopDef.h"

class uiCashItemShopMan;

class uiCashItemShopPresentDlg: public uiBase
{
	enum eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG
	{
		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_BTN_EXIT = 0,	//	X ��ư

		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_IMG_ITEM1,		//	������ �̹���1
		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_IMG_ITEM2,		//	������ �̹���2
		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_IMG_ITEM3,		//	������ �̹���3
		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_IMG_ITEM4,		//	������ �̹���4
		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_IMG_ITEM5,		//	������ �̹���5
		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_IMG_ITEM6,		//	������ �̹���6
        eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_BTN_MILEGE,

		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_CHK_CASH,		//	ĳ�þ����� üũ��ư
		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_CHK_MYPAGE,		//	���������� üũ��ư
		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_CHK_PACKAGE,		//	��Ű���� üũ��ư

        eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_CHK_MILEAGE,		//	���ϸ��� üũ��ư
		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_CHK_PRESENT,		//	���� ������ üũ��ư

		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMNAME1,	//	������ �̸�1
		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMNAME2,	//	������ �̸�2
		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMNAME3,	//	������ �̸�3
		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMNAME4,	//	������ �̸�4
		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMNAME5,	//	������ �̸�5
		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMNAME6,	//	������ �̸�6

		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMDESC1,	//	������ ���ܼ���1
		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMDESC2,	//	������ ���ܼ���2
		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMDESC3,	//	������ ���ܼ���3
		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMDESC4,	//	������ ���ܼ���4
		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMDESC5,	//	������ ���ܼ���5
		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMDESC6,	//	������ ���ܼ���6

		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_PLAYERNAME1,	//	������ �÷��̾� �̸�1
		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_PLAYERNAME2,	//	������ �÷��̾� �̸�2
		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_PLAYERNAME3,	//	������ �÷��̾� �̸�3
		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_PLAYERNAME4,	//	������ �÷��̾� �̸�4
		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_PLAYERNAME5,	//	������ �÷��̾� �̸�5
		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_PLAYERNAME6,	//	������ �÷��̾� �̸�6

		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMSENDDAY1,//	������ ��¥1
		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMSENDDAY2,//	������ ��¥2
		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMSENDDAY3,//	������ ��¥3
		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMSENDDAY4,//	������ ��¥4
		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMSENDDAY5,//	������ ��¥5
		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMSENDDAY6,//	������ ��¥6

		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMUSETIME1,//	���� ���Ⱓ1
		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMUSETIME2,//	���� ���Ⱓ2
		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMUSETIME3,//	���� ���Ⱓ3
		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMUSETIME4,//	���� ���Ⱓ4
		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMUSETIME5,//	���� ���Ⱓ5
		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMUSETIME6,//	���� ���Ⱓ6
		
		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMCOUNT1,	//	���� ����1
		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMCOUNT2,	//	���� ����2
		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMCOUNT3,	//	���� ����3
		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMCOUNT4,	//	���� ����4
		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMCOUNT5,	//	���� ����5
		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMCOUNT6,	//	���� ����6

		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_BTN_PRESENTLISTDEL1,	//	��� ���� ��ư1
		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_BTN_PRESENTLISTDEL2,	//	��� ���� ��ư2
		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_BTN_PRESENTLISTDEL3,	//	��� ���� ��ư3
		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_BTN_PRESENTLISTDEL4,	//	��� ���� ��ư4
		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_BTN_PRESENTLISTDEL5,	//	��� ���� ��ư5
		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_BTN_PRESENTLISTDEL6,	//	��� ���� ��ư6

		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_BTN_PRESENTLISTCHECK1,	//	���� Ȯ�� ��ư1
		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_BTN_PRESENTLISTCHECK2,	//	���� Ȯ�� ��ư2
		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_BTN_PRESENTLISTCHECK3,	//	���� Ȯ�� ��ư3
		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_BTN_PRESENTLISTCHECK4,	//	���� Ȯ�� ��ư4
		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_BTN_PRESENTLISTCHECK5,	//	���� Ȯ�� ��ư5
		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_BTN_PRESENTLISTCHECK6,	//	���� Ȯ�� ��ư6

		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_MONEY,				//	������

		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_BTN_PREV_PAGE,			//	���� ������ ��ư
		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_BTN_NEXT_PAGE,			//	���� ������ ��ư

		eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_CURPAGE,				//	���� ������
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

        eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_CHK_SEND,		//	���� ������ üũ��ư
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
    CCtrlStaticWZ*			m_pTxtCtrlMoney;	// �׼����� ��������, �������� ĳ�÷�..
    CCtrlStaticWZ*			m_pTxtCtrlPPCoin;	// PPĳ��
    CCtrlStaticWZ*			m_pTxtCtrlCCCoin;	// �Ϲ�ĳ��
    CCtrlStaticWZ*			m_pTxtCtrlMilege;	// ���ϸ���

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
