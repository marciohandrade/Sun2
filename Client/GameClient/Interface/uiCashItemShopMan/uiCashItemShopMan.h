#pragma once

enum
{
    CASH_ITEM_SHOP_DEFAULT = 1,
    CASH_ITEM_SHOP_HAIM,
    CASH_ITEM_SHOP_MILEAGE,
    CASH_ITEM_SHOP_CCASH,
    CASH_ITEM_SHOP_PCASH,
};


//------------------------------------------------------------------------------ 
#ifndef _NA_0_20110613_GAME_SHOP_RENEWAL //#if'N'def
//------------------------------------------------------------------------------ 


//------------------------------------------------------------------------------
//
//    �۾���: ������
//    uiCashItemShopMan class
//------------------------------------------------------------------------------



#include "uiBaseMan.h"
#if defined(__NA_00387_20070605_SHOP_MILEAGE2)
#include "uiBase.h"
#endif	//	defined(__NA_00387_20070605_SHOP_MILEAGE2)
#include "uiCashItemShopDef.h"

#ifdef __NA_00387_20070605_SHOP_MILEAGE2
class uiCashItemShopBase
{
public:
	uiCashItemShopBase()													{}	
	virtual ~uiCashItemShopBase()							{}
public:
	virtual void	RefreshSingleItemList()					{}
	virtual void	RefreshItemPrice()						{}
	virtual void	SetItem(int index, CODETYPE itemCode)	{}
	virtual int		GetCurPage()							{return 0;}
	virtual int		GetCurMenu()							{return 0;}
	virtual int		GetDlgCheckMenuMax()					{return 0;}
	virtual BYTE	GetCurCategoryType()					{return 0;}
};
#endif	//	__NA_00387_20070605_SHOP_MILEAGE2

class uiCashItemShopBuy;
class uiCashItemShopBuyCheck;
class uiCashItemShopBuyComplete;
class uiCashItemShopCart;
class uiCashItemShopDlg;
class uiCashItemShopNpcPopup;
class uiCashItemShopPresentDlg;
class uiCashItemShopPresentCheck;
class uiCashItemShopPresentComplete;
class uiCashItemShopPackageDlg;
class uiCashItemShopPackageCheck;
class uiCashItemShopPackageComplete;

#ifdef __NA_00387_20070605_SHOP_MILEAGE2
class uiCashItemShopMileageDlg;
class uiCashItemShopMileageBuy;
class uiCashItemShopMileageCheck;
class uiCashItemShopMileageComplete;
#endif	//	__NA_00387_20070605_SHOP_MILEAGE2


class uiCashItemShopExtensionBuy;
class uiCashItemShopExtensionComplete;

class uiCashItemPurchaseDlg;

struct stColorStr
{
	TCHAR	szStr[INTERFACE_STRING_LENGTH];
	DWORD	txtColor;
};

enum { ePURCHASE_DEFAULT = 1, ePURCHASE_PP_CASH = 2 };


class uiCashItemShopMan : public uiBaseMan
{
public:
    enum ShopStringCode
    {
        //�ڵ�	��Ʈ��	�з�
        //2500	��ü����	��������
        //2501	����	��������
        //2502	�̺�Ʈ	��������
        //2503	��ü����	���
        //2504	���	���
        //2505	����	���
        //2506	�尩	���
        //2507	����	���
        //2508	����	���
        //2509	��������	���
        //2510	��Ʈ	���
        //2511	����	���
        //2512	����	���
        //2513	��ü����	��Ų
        //2514	���	��Ų
        //2515	����	��Ų
        //2516	�尩	��Ų
        //2517	����	��Ų
        //2518	����	��Ų
        //2519	��������	��Ų
        //2520	��Ʈ	��Ų
        //2521	����	��Ų
        //2522	����	��Ų
        //2523	��ü����	Ŀ���͸���¡
        //2524	���	Ŀ���͸���¡
        //2525	��	Ŀ���͸���¡
        //2526	��Ÿ	Ŀ���͸���¡
        //2527	��ü����	������ 
        //2528	����	������ 
        //2529	����	������ 
        //2530	����	������ 
        //2531	����Ƿ�	������ 
        //2532	�ɷ�ġ��ȭ��	������ 
        //2533	��ɼ���	������ 
        //2534	���Ǽ��� 	������ 
        //2535	��ü����	��
        //2536	���	��
        //2537	��	��
        //2538	��Ų	��
        //2539	��ü����	Ż��
        //2540	Ż��	Ż��
        //2541	�Ӹ�����	Ż��
        //2542	��������	Ż��
        //2543	�ٸ�����	Ż��
        //2544	��ü Ŭ����	ĳ�û��� ����
        //2545	����Ŀ	ĳ�û��� ����
        //2546	�巡��	ĳ�û��� ����
        //2547	��Ű�� 	ĳ�û��� ����
        //2548	������Ż	ĳ�û��� ����
        //2549	������	ĳ�û��� ����
        //2550	��ü ��Ʈ	��Ű������ ����
        //2551	����Ŀ ��Ʈ	��Ű������ ����
        //2552	�巡�� ��Ʈ	��Ű������ ����
        //2553	��Ű�� ��Ʈ	��Ű������ ����
        //2554	������Ż ��Ʈ	��Ű������ ����
        //2555	������ ��Ʈ	��Ű������ ����
        //2556	��Ÿ ������ ��Ʈ	��Ű������ ����

        kShopStringCode_AllItem = 2500,
        kShopStringCode_Armor = 2503,  
        kShopStringCode_Skin = 2513,    
        kShopStringCode_Customize = 2523,
        kShopStringCode_Item = 2527,     
        kShopStringCode_Pet = 2535,      
        kShopStringCode_Rider = 2539,     
        kShopStringCode_Filter_Cash = 2544, 
        kShopStringCode_Filter_Package = 2550,
        kShopStringCode_End = 2556
    };

public:
    uiCashItemShopMan(InterfaceManager *pUIMan);
    virtual ~uiCashItemShopMan();

    //--------------------------------------------------------------------------
    // init/release
    //
public:
    virtual void            Release();
    virtual void            Update();
    virtual void            Process(DWORD dwTick);
    virtual void            OnInitialize();

	void					ClearAllDlg( void );
	void                    ClearDlg();
	void					ClearCart();
	void					ClearCashItemInfo()		{ ZeroMemory(&m_stCashItemInfo, sizeof(m_stCashItemInfo));	}
	void					ClearPackageItemInfo()	{ ZeroMemory(&m_stPackageItemInfo, sizeof(m_stPackageItemInfo));	}
	void					ClearPresentItemInfo()	{ ZeroMemory(&m_stPresentItemInfo, sizeof(m_stPresentItemInfo));	}
	void					ClearItemInfo( void );

private:
    void                    _LoadUI();

    //--------------------------------------------------------------------------
    // functions
    //
public:
	void					ShowCashItemShopNpcPopup(bool bShow);	// npc �˾�â Show
	void					ToggleCashItemShopNpcPopup();			// npc �˾�â Toggle

	void					ToggleCashItemShopDlg();				// ���� ����â Toggle

	void					ShowCashItemShopBuyCart(bool bShow);	// ��ٱ��� Show

	void					ShowCashItemShopDlg(bool bShow);		// ���� ����â Show

	void					ShowCashItemShopBuy(int iIndex);
	void					ShowCashItemShopBuy(bool bShow);
	void					ShowCashItemShopBuyCheck(bool bShow);
	void					ShowCashItemShopBuyCheck(stCashItemInfo* pInfo);
	void					ShowCashItemShopBuyComplete(bool bShow);
	void					ShowCashItemShopBuyComplete(stCashItemInfo* pInfo);
	void					ShowCashItemShopPresentDlg(bool bShow);
	void					ShowCashItemShopPresentCheck(bool bShow);
	void					ShowCashItemShopPresentCheck(int iIndex);
	void					ShowCashItemShopPresentCheck(stPresentItemInfo* pInfo);
	void					ShowCashItemShopPresentComplete(bool bShow);
	void					ShowCashItemShopPresentComplete(stPresentItemInfo* pInfo);
	void					ShowCashItemShopPackageDlg(bool bShow);
	void					ShowCashItemShopPackageCheck(bool bShow);
	void					ShowCashItemShopPackageCheck(int iIndex);
	void					ShowCashItemShopPackageComplete(bool bShow);
	void					ShowCashItemShopPackageComplete(stPackageItemInfo* pInfo);

#ifdef __NA_00387_20070605_SHOP_MILEAGE2
	void					ShowCashItemShopMileageDlg(bool bShow);
	void					ShowCashItemShopMileageBuy(int iIndex);
	void					ShowCashItemShopMileageBuy(bool bShow);
	void					ShowCashItemShopMileageCheck(bool bShow);
	void					ShowCashItemShopMileageCheck(stCashItemInfo* pInfo);
	void					ShowCashItemShopMileageComplete(bool bShow);
	void					ShowCashItemShopMileageComplete(stCashItemInfo* pInfo);
#endif	//	__NA_00387_20070605_SHOP_MILEAGE2

	void					ShowCashItemShopExtensionBuy(int iIndex);
	void					ShowCashItemShopExtensionBuy(bool bShow);
	void					ShowCashItemShopExtensionComplete(bool bShow);
	void					ShowCashItemShopExtensionComplete(stCashItemInfo* pInfo);
    void                    ShowCashItemPurchaseDlg(bool bShow);
	void					Send_CG_SHOP_CASH_REQ_SYN();	// �� ���½� ���ϸ��� ����
	void					Send_CG_SHOP_ITEM_REQ_SYN(int iItemIndex, BOOL bExtension = FALSE);

    //------------------------------------------------------------------------------
    //! Parameter: BYTE partition : �޴��� : eITEMSHOP_PARTITION
    //! Parameter: BYTE category īŸ�� : eITEMSHOP_CATEGORY
    //! Parameter: WORD filter ���� �ɼ� eITEMSHOP_FILTER (eSHOP_FILTER_CHAR_VALKYRIE | eITEMSHOP_FILTER_DAY_DESCENDING)
    //! Parameter: BYTE page ��û ������
    //------------------------------------------------------------------------------
    void Send_CG_SHOP_ITEM_LIST_REQ_SYN(BYTE partition, BYTE category, WORD filter, BYTE page);
    void OnClickExtensionButton(int page);

	void					Send_CG_SHOP_ITEM_BUY_SYN(SLOTIDX slotIndex = 0, POSTYPE slotPos = 0);

	void					Send_CG_SHOP_IS_RECV_GIFT_SYN(void);
	void					Send_CG_SHOP_PRESENT_LIST_REQ_SYN( void );
	void					Send_CG_SHOP_SEND_GIFT_SYN( void );
	void					Send_CG_SHOP_ACTION_RECV_GIFT_SYN( int actionType );

	CASH					GetMyCash()						{ return m_Cash;	}
	void					SetCash(CASH cash)				{ m_Cash = cash;	}
	DWORD					GetMyMileage()					{ return m_Mileage;	}
	void					SetMyMileage(DWORD dwMileage)	{ m_Mileage = dwMileage;}

	CASH					GetPPCash()					    { return pp_cash_;	}
	void					SetPPCash(CASH pp_cash)	        { pp_cash_ = pp_cash;}
    void                    SetUseCashType(BYTE use_cash_type){ use_cash_type_ =  use_cash_type; }
    BYTE                    GetUseCashType(){ return use_cash_type_; }
    void                    SetBuyIndex(int selected_index){ selected_index_ = selected_index; }
    int                     GetBuyIndex(){ return selected_index_; }
    void                    SelectBuyItem(int selected_index);
    void                    OpenBuyWindowFromCashType();
    int                     pp_category() { return pp_category_; }
    void                    set_pp_category(int pp_category) { pp_category_ = pp_category; }
    int                     cc_category() { return cc_category_; }
    void                    set_cc_category(int cc_category) { cc_category_ = cc_category; }

	BYTE					GetCategory()					{ return m_byCategory;	}
	void					SetCategory(BYTE byCategory)	{ m_byCategory = byCategory;	}

	BYTE					GetPage()						{ return m_byPage;		}
	void					SetPage(BYTE byPage)			{ m_byPage = byPage;	}

	int						GetNewPresentRcvCount()			{return m_iNewPresentRcvCount;}
	void					ClearNewPresentRcvCount()		{m_iNewPresentRcvCount = 0;}

	bool					IsRcvCashShopInfo(BYTE byCategory, BYTE byPage);
	bool					IsRcvPackageInfo(BYTE byCategory, BYTE byPage);
#ifdef __NA_00387_20070605_SHOP_MILEAGE2	
	BOOL					IsRcvMileageShopInfo(BYTE byCategory, BYTE byPage);
#endif	//	__NA_00387_20070605_SHOP_MILEAGE2
	BOOL					IsRcvExtensionShopInfo(BYTE byCategory, BYTE byPage);

	int						GetCashShopTotalPage(BYTE byCategory);
	int						GetPackageTotalPage(BYTE byCategory);
#ifdef __NA_00387_20070605_SHOP_MILEAGE2
	int						GetMileageTotalPage(BYTE byCategory);
#endif	//	__NA_00387_20070605_SHOP_MILEAGE2
	int						GetExtensionTotalPage(BYTE byCategory);

	void					SetCashShopTotalPage(BYTE byCategory, int iTotalPage);
	void					SetPackageTotalPage(BYTE byCategory, int iTotalPage);
#ifdef __NA_00387_20070605_SHOP_MILEAGE2
	void					SetMileageTotalPage(BYTE byCategory, int iTotalPage);
#endif	//	__NA_00387_20070605_SHOP_MILEAGE2
	void					SetExtensionTotalPage(BYTE byCategory, int iTotalPage);

    void renderCoverImage(CASH_ITEM_IMG& info, RECT& rect);

	void					ClearPendingBuyList()
	{
		ZeroMemory(&m_pendingBuyList, sizeof(m_pendingBuyList));
		ZeroMemory(&m_pendingBuyItemInfo, sizeof(m_pendingBuyItemInfo));
	}

	void					ClearPendingBuyPresentItemInfo()	{ZeroMemory(&m_pendingBuyPresentItemInfo, sizeof(m_pendingBuyPresentItemInfo));}

	void					ClearPendingBuyPackageItemInfo()	
	{
		ZeroMemory(&m_pendingBuyPackageItemInfo, sizeof(m_pendingBuyPackageItemInfo));
		m_pendingBuyPackageIndex = 0;
	}

	void					PrepareShow(int iCashShopDlgState);
	void					RefreshPresentItemList();
	void					RefreshItemList(MSG_CG_SHOP_ITEM_LIST_REQ_ACK *pPacket);
	void					RefreshSingleItemList();
	void					RefreshPackageItemList();
	void					RefreshItemPrice();
	void					RefreshExtensionItemList();

	int						GetCashShopDlgState()						{return m_iCashShopDlgState;}
	void					SetCashShopDlgState(int iCashShopDlgState)	{m_iCashShopDlgState = iCashShopDlgState;}

	int						GetPresentState()							{return m_iPresentState;}
	void					SetPresentState(int iPresentState)			{m_iPresentState = iPresentState;}

	int						GetRqstState();
	void					SetRqstState(int iRqstState)				{m_iRqstState = iRqstState;}
	PresentRecvBox*			GetPresentRcvBox();


	void					SetItemTipListDesc(BASE_ITEMINFO * pItemInfo, CCtrlListWZ* pTipList, int iTipListCurLine, DWORD	dwCoolTime);
	void					SetItemTipListPCBangDesc(BASE_ITEMINFO * pItemInfo);
	void					SetItemTipListEqLimitDesc(BASE_ITEMINFO * pItemInfo);
	void					SetItemTipListBuyCoolTimeDesc(DWORD	dwCoolTime);
	void					SetItemtipListTradeInDesc(BASE_ITEMINFO * pItemInfo);
	void					ClearColorStr();
	void					SetSystemDialog(bool bShow)	{ m_bShowSystemDialog = bShow;	}

    void ExecBuyWCoin();
    void FillSubMenu(CCtrlListWZ* list_control, int start_sub_menu, int end_sub_menu);
    void FillSortCombo(CCtrlListWZ* list_control);
    WORD GetFilter(int main_Menu, int sub_menu, CCtrlListWZ* list_control);

private:
	void					_processErrorCode(BYTE byErrorCode, eOUTPUT_MSG_TYPE type, DWORD dwPriceGuid = 0);
    

    //--------------------------------------------------------------------------
    // procedure
    //
public:
    virtual void	        NetworkProc( MSG_BASE * pMsg );


    
    //--------------------------------------------------------------------------
    // member vars
    //
private:
	uiCashItemShopDlg*				m_pUICashItemShopDlg;
    uiCashItemShopBuy*				m_pUICashItemShopBuy;
	uiCashItemShopBuyCheck*			m_pUICashItemShopBuyCheck;
	uiCashItemShopBuyComplete*		m_pUICashItemShopBuyComplete;
	uiCashItemShopCart*				m_pUICashItemShopCart;
	uiCashItemShopNpcPopup*			m_pUICashItemShopNpcPopup;
	uiCashItemShopPresentDlg*		m_pUiCashItemShopPresentDlg;
	uiCashItemShopPresentCheck*		m_pUICashItemShopPresentCheck;
	uiCashItemShopPresentComplete*	m_pUICashItemShopPresentComplete;
	uiCashItemShopPackageDlg*		m_pUiCashItemShopPackageDlg;
	uiCashItemShopPackageCheck*		m_pUiCashItemShopPackageCheck;
	uiCashItemShopPackageComplete*	m_pUiCashItemShopPackageComplete;
#ifdef __NA_00387_20070605_SHOP_MILEAGE2
	uiCashItemShopMileageDlg*		m_pUICashItemShopMileageDlg;
	uiCashItemShopMileageBuy*		m_pUICashItemShopMileageBuy;
	uiCashItemShopMileageCheck*		m_pUICashItemShopMileageCheck;
	uiCashItemShopMileageComplete*	m_pUICashItemShopMileageComplete;
#endif	//	__NA_00387_20070605_SHOP_MILEAGE2

	uiCashItemShopExtensionBuy*		m_pUICashItemShopExtensionBuy;
	uiCashItemShopExtensionComplete* m_pUICashItemShopExtensionComplete;
    uiCashItemPurchaseDlg*          m_pUICashItemPurchaseDlg;

	bool						m_bWaitPacket;

	BYTE						m_byCategory;
	BYTE						m_byPage;
	CASH						m_Cash;
	DWORD						m_Mileage;
	CASH						pp_cash_;
    BYTE                        use_cash_type_;
    BYTE                        cash_type_;
    BYTE                        buy_window_type_;
    int                         selected_index_;
    int                         pp_category_;
    int                         cc_category_;

	std::vector<stColorStr> m_vstrColorStr;

	std::vector<stCashShopRcvInfo>	m_vecCashShopRcvInfo;
	std::vector<stCashShopRcvInfo>	m_vecPackageRcvInfo;

#ifdef __NA_00387_20070605_SHOP_MILEAGE2
	std::vector<stCashShopRcvInfo>	m_vecMileageShopRcvInfo;
#endif	//	__NA_00387_20070605_SHOP_MILEAGE2

	std::vector<stCashShopRcvInfo>	m_vecExtensionShopRcvInfo;

	
	int							m_iCashShopDlgState;		//	���� ���� DLG ���°�
	int							m_iPresentState;			//	���������� ���� ���º���
	bool						m_bShowSystemDialog;		// �ý��� Ȯ��â ���� (���� ������Ȯ�θ� ���)
public:
	ShopManager					m_ShopManager;
	stCashItemInfo				m_stCashItemInfo[_ITEM_NUM_PER_PAGE];
	stCashItemInfo				m_stCartInfo[MSG_CG_SHOP_ITEM_BUY_SYN::_MAX_BUY_LIST_NUM];
	stPresentItemInfo			m_stPresentItemInfo[_PRESENT_ITEM_PER_PAGE];
	stPackageItemInfo			m_stPackageItemInfo[_PACKAGE_ITEM_PER_PAGE];
	stCashItemInfo				m_stExtensionItemInfo[_ITEM_NUM_PER_PAGE];

	BYTE						m_TotalPageNumByCart;
	BYTE						m_CurPageNumByCart;

	// pending data
	int							m_pendingBuyList[MSG_CG_SHOP_ITEM_BUY_SYN::_MAX_BUY_LIST_NUM];

	int							m_pendingBuyPackageIndex;
	int							m_iRqstState;
	stCashItemInfo				m_pendingBuyItemInfo;
	stPresentItemInfo			m_pendingBuyPresentItemInfo;
	stPackageItemInfo			m_pendingBuyPackageItemInfo;
	int							m_iNewPresentRcvCount;

	BYTE						m_byPendingCategory;
	BYTE						m_byPendingPage;
    WORD                        m_wPendingFilter;

	BYTE						m_byPendingSeperate;
	ITEMGUID					m_iPendingItemGuid;
	int							m_iPendingItemIndex;

	DWORD						m_dwPendingExtensionItemCode;

	int							m_CurSceneType;

public:
    void ChangeTab(uiBase* ui_base);
    void ShowTab();

    uiBase* active_tab() const { return active_tab_; }
    void set_active_tab(uiBase* val) { active_tab_ = val; }

    MSG_CG_SHOP_ITEM_LIST_REQ_ACK* current_show_item_list() { return &current_show_item_list_; }
private:
    uiBase* active_tab_;
    MSG_CG_SHOP_ITEM_LIST_REQ_ACK current_show_item_list_;

};
//------------------------------------------------------------------------------ 
#endif //#ifndef _NA_0_20110613_GAME_SHOP_RENEWAL //#if'N'def
//------------------------------------------------------------------------------ 
