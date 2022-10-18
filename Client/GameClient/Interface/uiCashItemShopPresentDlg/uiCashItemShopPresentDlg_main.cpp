#include "SunClientPrecompiledHeader.h"

//------------------------------------------------------------------------------ 
#ifndef _NA_0_20110613_GAME_SHOP_RENEWAL //#if'N'def
//------------------------------------------------------------------------------ 


#include "uiCashItemShopPresentDlg.h"
#include "uiCashItemShopMan/uiCashItemShopMan.h"
#include "uiSystemMan/uiSystemMan.h"
#include "Hero.h"
#include "GlobalFunc.h"
#include "interfacemanager.h"
#include "GameFramework.h"
#include "ItemInfoParser.h"
#include "Mouse.h"
#include "uiToolTipMan/uiToolTipMan.h"
#include "GameFunc.h"

//------------------------------------------------------------------------------
WzID uiCashItemShopPresentDlg::m_wzId[eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_MAX] = 
{
	StrToWzID("B999"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_BTN_EXIT = 0,	//	X ��ư

	StrToWzID("B001"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_IMG_ITEM1,		//	������ �̹���1
	StrToWzID("TI02"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_IMG_ITEM2,		//	������ �̹���2
	StrToWzID("B002"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_IMG_ITEM3,		//	������ �̹���3
	StrToWzID("TI01"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_IMG_ITEM4,		//	������ �̹���4
	StrToWzID("B003"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_IMG_ITEM5,		//	������ �̹���5
	StrToWzID("TI00"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_IMG_ITEM6,		//	������ �̹���6
    StrToWzID("BT02"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_BTN_MILEGE,		//	���ϸ���
	StrToWzID("C001"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_CHK_CASH,		//	ĳ�þ����� üũ��ư
	StrToWzID("C002"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_CHK_MYPAGE,		//	���������� üũ��ư
    StrToWzID("CT00"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_CHK_PACKAGE,		//	��Ű���� üũ��ư
	StrToWzID("CT01"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_CHK_MILEAGE,		//	���ϸ��� üũ��ư
    StrToWzID("C006"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_CHK_PRESENT,		//	���� ������ üũ��ư
	StrToWzID("T002"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMNAME1,	//	������ �̸�1
	StrToWzID("ST00"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMNAME2,	//	������ �̸�2
	StrToWzID("T003"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMNAME3,	//	������ �̸�3
	StrToWzID("ST02"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMNAME4,	//	������ �̸�4
	StrToWzID("T004"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMNAME5,	//	������ �̸�5
	StrToWzID("ST17"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMNAME6,	//	������ �̸�6

	StrToWzID("T080"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMDESC1,	//	������ ���ܼ���1
	StrToWzID("ST01"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMDESC2,	//	������ ���ܼ���2
	StrToWzID("T081"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMDESC3,	//	������ ���ܼ���3
	StrToWzID("ST03"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMDESC4,	//	������ ���ܼ���4
	StrToWzID("T082"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMDESC5,	//	������ ���ܼ���5
	StrToWzID("ST18"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMDESC6,	//	������ ���ܼ���6

	StrToWzID("T006"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_PLAYERNAME1,	//	������ �÷��̾� �̸�1
	StrToWzID("ST22"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_PLAYERNAME2,	//	������ �÷��̾� �̸�2
	StrToWzID("T014"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_PLAYERNAME3,	//	������ �÷��̾� �̸�3
	StrToWzID("ST23"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_PLAYERNAME4,	//	������ �÷��̾� �̸�4
	StrToWzID("T022"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_PLAYERNAME5,	//	������ �÷��̾� �̸�5
	StrToWzID("ST27"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_PLAYERNAME6,	//	������ �÷��̾� �̸�6
	

	StrToWzID("T008"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMSENDDAY1,//	������ ��¥1
	StrToWzID("ST21"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMSENDDAY2,//	������ ��¥2
	StrToWzID("T016"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMSENDDAY3,//	������ ��¥3
	StrToWzID("ST24"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMSENDDAY4,//	������ ��¥4
	StrToWzID("T024"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMSENDDAY5,//	������ ��¥5
	StrToWzID("ST28"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMSENDDAY6,//	������ ��¥6

	StrToWzID("T010"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMUSETIME1,//	���� ���Ⱓ1
	StrToWzID("ST20"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMUSETIME2,//	���� ���Ⱓ2
	StrToWzID("T018"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMUSETIME3,//	���� ���Ⱓ3
	StrToWzID("ST25"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMUSETIME4,//	���� ���Ⱓ4
	StrToWzID("T026"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMUSETIME5,//	���� ���Ⱓ5
	StrToWzID("ST29"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMUSETIME6,//	���� ���Ⱓ6

	StrToWzID("T012"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMCOUNT1,	//	���� ����1
	StrToWzID("ST19"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMCOUNT1,	//	���� ����1
	StrToWzID("ST04"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMCOUNT2,	//	���� ����2
	StrToWzID("ST26"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMCOUNT2,	//	���� ����2
	StrToWzID("T028"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMCOUNT3,	//	���� ����3
	StrToWzID("ST30"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMCOUNT3,	//	���� ����3

	StrToWzID("B008"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_BTN_PRESENTLISTDEL1,	//	��� ���� ��ư1
	StrToWzID("TI09"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_BTN_PRESENTLISTDEL2,	//	��� ���� ��ư2
	StrToWzID("B009"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_BTN_PRESENTLISTDEL3,	//	��� ���� ��ư3
	StrToWzID("TI10"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_BTN_PRESENTLISTDEL4,	//	��� ���� ��ư4
	StrToWzID("B010"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_BTN_PRESENTLISTDEL5,	//	��� ���� ��ư5
	StrToWzID("TI11"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_BTN_PRESENTLISTDEL6,	//	��� ���� ��ư6

	StrToWzID("B016"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_BTN_PRESENTLISTCHECK1,//	���� Ȯ�� ��ư1
	StrToWzID("TI03"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_BTN_PRESENTLISTCHECK2,//	���� Ȯ�� ��ư2
	StrToWzID("B018"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_BTN_PRESENTLISTCHECK3,//	���� Ȯ�� ��ư3
	StrToWzID("TI06"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_BTN_PRESENTLISTCHECK4,//	���� Ȯ�� ��ư4
	StrToWzID("B020"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_BTN_PRESENTLISTCHECK5,//	���� Ȯ�� ��ư5
	StrToWzID("TI07"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_BTN_PRESENTLISTCHECK6,//	���� Ȯ�� ��ư6
	StrToWzID("T057"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_MONEY,			//	������
	StrToWzID("BT01"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_BTN_PREV_PAGE,		//	���� ������ ��ư	
	StrToWzID("BT00"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_BTN_NEXT_PAGE,		//	���� ������ ��ư
	StrToWzID("ST31"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_CURPAGE,			//	����������
    StrToWzID("PI13"),  //  eDIALOG_CASH_ITEM_SHOP_PRESENT_IMG_WCOIN_BACK    

    StrToWzID("PI11"),  //  eDIALOG_CASH_ITEM_SHOP_PRESENT_IMG_WCOINC_BACK
    StrToWzID("PI12"),  //  eDIALOG_CASH_ITEM_SHOP_PRESENT_IMG_WCOINP_BACK
    StrToWzID("ST37"),  //  eDIALOG_CASH_ITEM_SHOP_PRESENT_STXT_WCOINC
    StrToWzID("ST34"),  //  eDIALOG_CASH_ITEM_SHOP_PRESENT_STXT_WCOINP
    StrToWzID("ST33"),  //  eDIALOG_CASH_ITEM_SHOP_PRESENT_STXT_MILEAGE
    StrToWzID("ST36"),  //  eDIALOG_CASH_ITEM_SHOP_PRESENT_TXT_WCOINC
    StrToWzID("ST32"),  //  eDIALOG_CASH_ITEM_SHOP_PRESENT_TXT_WCOINP
    StrToWzID("ST35"),  //  eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_MILEGE
    StrToWzID("BT02"),  //  eDIALOG_CASH_ITEM_SHOP_PRESENT_BTN_BUY_WCOIN
    StrToWzID("CT02"),  //  eDIALOG_CASH_ITEM_SHOP_PRESENT_BTN_WCOIN

    StrToWzID("C000"),	//	eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_CHK_SEND,		//	���� ������ üũ��ư

};


//------------------------------------------------------------------------------
/**
*/
uiCashItemShopPresentDlg::uiCashItemShopPresentDlg(InterfaceManager *pUIMan) :
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

	m_bShow = false;
	ZeroMemory(&m_CashItemImg, sizeof(m_CashItemImg));
	m_iTotalPage = 0;
	m_iCurPage = -1;
	m_iCurIndex = -1;

	m_iTotalPage	= 0;
	m_iPresentCount = 0;

	ZeroMemory(&m_ItemCodeForTooltip, sizeof(m_ItemCodeForTooltip));
}

//------------------------------------------------------------------------------
/**
*/
uiCashItemShopPresentDlg::~uiCashItemShopPresentDlg()
{
	this->Release();
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopPresentDlg::Init(CDrawBase* pDrawBase)
{
	uiBase::Init(pDrawBase);

	this->InitControls();
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopPresentDlg::Release()
{
	_removeAllIconImage();
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopPresentDlg::InitControls()
{
	int i;
	for(i=0; i<eCASH_ITEM_SHOP_PRESENT_DLG_CHK_MAX; ++i)
	{
		m_pBtnCheckTabs[i] = static_cast<CCtrlButtonCheckWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_CHK_CASH+i));
		assert(m_pBtnCheckTabs[i]);
	}

	for(i=0; i<eCASH_ITEM_SHOP_PRESENT_DLG_CHK_MENU_MAX; ++i)
	{
        m_pBtnCheckMenu[i] = static_cast<CCtrlButtonCheckWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_CHK_PRESENT+i));
		assert(m_pBtnCheckMenu[i]);
	}

    #ifdef __CN_0_20070507_SHOP_MILEAGE
    #else
    if(m_pBtnCheckTabs[eCASH_ITEM_SHOP_PRESENT_DLG_CHK_MILEAGE])
    {
       m_pBtnCheckTabs[eCASH_ITEM_SHOP_PRESENT_DLG_CHK_MILEAGE]->ShowWindowWZ(WZ_HIDE);
    }
    #endif//__CN_0_20070507_SHOP_MILEAGE

	for(i=0; i<eCASH_ITEM_SHOP_PRESENT_DLG_ITEM_MAX; ++i)
	{
		m_pImgCtrl[i] = static_cast<CCtrlImageStaticWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_IMG_ITEM1+i));
		assert(m_pImgCtrl[i]);

		m_pTxtCtrlName[i] = static_cast<CCtrlStaticWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMNAME1+i));
		assert(m_pTxtCtrlName[i]);

		m_pTxtCtrlMemo[i] = static_cast<CCtrlStaticWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMDESC1+i));
		assert(m_pTxtCtrlMemo[i]);

		m_pTxtCtrlPlayerName[i] = static_cast<CCtrlStaticWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_PLAYERNAME1+i));
		assert(m_pTxtCtrlPlayerName[i]);

		m_pTxtCtrlSendDay[i] = static_cast<CCtrlStaticWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMSENDDAY1+i));
		assert(m_pTxtCtrlSendDay[i]);

		m_pTxtCtrlUseTime[i] = static_cast<CCtrlStaticWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMUSETIME1+i));
		assert(m_pTxtCtrlUseTime[i]);

		m_pTxtCtrlItemCount[i] = static_cast<CCtrlStaticWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_ITEMCOUNT1+i));
		assert(m_pTxtCtrlItemCount[i]);

		m_pBtnPresentListDel[i] = static_cast<CCtrlButtonWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_BTN_PRESENTLISTDEL1+i));
		assert(m_pBtnPresentListDel[i]);

		m_pBtnPresentListCheck[i] = static_cast<CCtrlButtonWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_BTN_PRESENTLISTCHECK1+i));
		assert(m_pBtnPresentListCheck[i]);

	}

	m_pTxtCtrlMoney = static_cast<CCtrlStaticWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_MONEY));
	assert(m_pTxtCtrlMoney);
    m_pTxtCtrlMilege = static_cast<CCtrlStaticWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_MILEGE));
	assert(m_pTxtCtrlMilege);
	m_pTxtCtrlCurPage = static_cast<CCtrlStaticWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_CURPAGE));
	assert(m_pTxtCtrlCurPage);

    m_pTxtCtrlPPCoin = static_cast<CCtrlStaticWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_PRESENT_TXT_WCOINP));
    SUN_ASSERT(m_pTxtCtrlPPCoin);
    m_pTxtCtrlCCCoin = static_cast<CCtrlStaticWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_PRESENT_TXT_WCOINC));
    SUN_ASSERT(m_pTxtCtrlCCCoin);
    #ifdef _NA00XXXX_090111_CASH_TYPE_SUBDIVIDE_//PPī�� ���� ���
    //����Ǹ� ������ �ʴ´�
    CCtrlStaticWZ* control_txts;
    control_txts = static_cast<CCtrlStaticWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_PRESENT_STXT_MILEAGE));
    SUN_ASSERT(control_txts);
    if(control_txts)
    {
        control_txts->ShowWindowWZ(WZ_HIDE);
    }
    #else
    CCtrlStaticWZ* control_txts;
    for(i=0; i<eCASH_ITEM_SHOP_PRESENT_TXT_MAX; ++i)
    {
        control_txts = static_cast<CCtrlStaticWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_PRESENT_STXT_WCOINC+i));
        SUN_ASSERT(control_txts);
        if(control_txts)
        {
            control_txts->ShowWindowWZ(WZ_HIDE);
        }
    }

    CCtrlPictureWZ* control_pictures;
    for(i=0; i<eCASH_ITEM_SHOP_PRESENT_IMG_MAX; ++i)
    {
        control_pictures = static_cast<CCtrlPictureWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_PRESENT_IMG_WCOIN_BACK+i));
        SUN_ASSERT(control_pictures);
        if(control_pictures && (i != eCASH_ITEM_SHOP_PRESENT_IMG_WCOIN_BACK) )
        {
            control_pictures->ShowWindowWZ(WZ_HIDE);
        }
    }

    CCtrlButtonWZ* button_of_to_buy_w_coin = static_cast<CCtrlButtonWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_PRESENT_BTN_WCOIN));
    SUN_ASSERT(button_of_to_buy_w_coin);
    if(button_of_to_buy_w_coin)
    {
        button_of_to_buy_w_coin->ShowWindowWZ(WZ_HIDE);
    }
    #endif//_NA00XXXX_090111_CASH_TYPE_SUBDIVIDE_

    #ifdef __CN_0_20070507_SHOP_MILEAGE
    if(m_pTxtCtrlMilege)
    {
        m_pTxtCtrlMilege->ShowWindowWZ(WZ_SHOW);
    }
    CCtrlStaticWZ* control_txt = static_cast<CCtrlStaticWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_PRESENT_STXT_MILEAGE));
    SUN_ASSERT(control_txts);
    if(control_txt)
    {
        control_txt->ShowWindowWZ(WZ_SHOW);
    }
    CCtrlPictureWZ* control_picture = static_cast<CCtrlPictureWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_PRESENT_IMG_WCOINP_BACK));
    SUN_ASSERT(control_picture);
    if(control_picture)
    {
        control_picture->ShowWindowWZ(WZ_SHOW);
    }
    #else
    CCtrlStaticWZ* control_txt = static_cast<CCtrlStaticWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_TXT_MILEGE));
    SUN_ASSERT(control_txt);
    if(control_txt)
    {
        control_txt->ShowWindowWZ(WZ_HIDE);
    }

    CCtrlButtonWZ* button_of_mymilege = static_cast<CCtrlButtonWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_BTN_MILEGE));
    SUN_ASSERT(button_of_mymilege);
    if(button_of_mymilege)
    {
        button_of_mymilege->ShowWindowWZ(WZ_HIDE);
    }
    #endif//__CN_0_20070507_SHOP_MILEAGE


    getControl(eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_CHK_SEND)->EnableWindowWZ(false);

	this->m_bControlsInitialized = true;
}

//------------------------------------------------------------------------------
/**
*/
BOOL uiCashItemShopPresentDlg::_isEnable()
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
void uiCashItemShopPresentDlg::_updateMenu(eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG e)
{
    if (e == eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_CHK_PRESENT)
    {
        static_cast<CCtrlButtonCheckWZ*>(getControl(eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_CHK_PRESENT))->SetCheckState(true);
        static_cast<CCtrlButtonCheckWZ*>(getControl(eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_CHK_SEND))->SetCheckState(false);
    }
    else if (e == eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_CHK_SEND)
    {
        static_cast<CCtrlButtonCheckWZ*>(getControl(eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_CHK_PRESENT))->SetCheckState(false);
        static_cast<CCtrlButtonCheckWZ*>(getControl(eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_CHK_SEND))->SetCheckState(true);
    }

}

//------------------------------------------------------------------------------
/** 
*/
void uiCashItemShopPresentDlg::_setIndex( int iIndex )
{
	m_iCurIndex = iIndex;
	if(GetManager()->GetPresentState() != ePresentState_None)
		GetManager()->ShowCashItemShopPresentCheck(m_iCurIndex);
}

//------------------------------------------------------------------------------
/** ������ ������ ���� ����
*/
void uiCashItemShopPresentDlg::_setPage(int iPage)
{
	if(m_iCurPage == iPage)
		return;

	m_iCurPage = iPage;

	if(!GetPresentCount())
	{
		GetManager()->Send_CG_SHOP_PRESENT_LIST_REQ_SYN();
	}
	else
	{
		GetManager()->RefreshPresentItemList();
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopPresentDlg::_prepareShow()
{
	if (!_isEnable())
		return;

	GetManager()->PrepareShow(eCashItemShopDlgState_Present);

	ZeroMemory(&m_ItemCodeForTooltip, sizeof(m_ItemCodeForTooltip));
	_clearAllItem();

	m_iCurPage = -1;
	m_iCurIndex = -1;

	_setPage();
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopPresentDlg::_clearItem(int index)
{
	this->_removeIconImage(index);

	GlobalFunc::SetCaption(m_pTxtCtrlName[index], _T(""));			// ������ �̸�
	GlobalFunc::SetCaption(m_pTxtCtrlMemo[index], _T(""));			// ���� ����
	GlobalFunc::SetCaption(m_pTxtCtrlPlayerName[index], _T(""));	// ������ �÷��̾� �̸�
	GlobalFunc::SetCaption(m_pTxtCtrlSendDay[index], _T(""));		// ���� ���� �Ͻ�
	GlobalFunc::SetCaption(m_pTxtCtrlUseTime[index], _T(""));		// ���� ��� �Ⱓ
	GlobalFunc::SetCaption(m_pTxtCtrlItemCount[index], _T(""));		// ���� ����

	m_pBtnPresentListDel[index]->EnableWindowWZ(false);
	m_pBtnPresentListCheck[index]->EnableWindowWZ(false);
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopPresentDlg::_clearAllItem()
{
	for(int i=0; i<eCASH_ITEM_SHOP_PRESENT_DLG_ITEM_MAX; ++i)
	{
		_clearItem(i);
	}
	GlobalFunc::SetCaption(m_pTxtCtrlMoney, _T(""));	// ���� ���� money
	GlobalFunc::SetCaption(m_pTxtCtrlCurPage, _T(""));  // ���� ������
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopPresentDlg::SetItem(int index, CODETYPE itemCode)
{
	_setItem(index, itemCode);
}

void uiCashItemShopPresentDlg::_setItem(int index, CODETYPE itemCode)
{
	BASE_ITEMINFO * pItemInfo = ItemInfoParser::Instance()->GetItemInfo( itemCode );
	if ( !pItemInfo ) 
	{
		return;
	}

    m_CashItemImg[index].type = GetManager()->m_stPresentItemInfo[index].stPiece[0].byRank;
	_addIconImage(index, pItemInfo->m_wVICode, itemCode);

	TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szTemp2[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szTemp3[INTERFACE_STRING_LENGTH] = {0,};

	// ������ �̸�
	g_InterfaceManager.GetItemName(itemCode, szMessage, INTERFACE_STRING_LENGTH);

	if(GetManager()->m_stPresentItemInfo[index].stPiece[0].byEnchant)
	{
		Snprintf(szTemp, INTERFACE_STRING_LENGTH-1, _T("+%d"), GetManager()->m_stPresentItemInfo[index].stPiece[0].byEnchant);
		_tcscat(szMessage, szTemp);
	}

    // ĳ�ü��������� Rank ���� �뵵 �����Ͽ� Rank�� ��� ���� �ʵ��� ����
    // �������� Rank��ü�� ���ǰ� ���� �ʾ���.
    //if(pItemInfo->CanRankUpItem())
	//{
	//	int iRankIndex = eST_RANK_E_STRING_INDEX  + (GetManager()->m_stPresentItemInfo[index].stPiece[0].byRank);
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

	GlobalFunc::SetCaption(m_pTxtCtrlName[index], szMessage);	
	ZeroMemory(szMessage, sizeof(szMessage));

	// ���� ���� 
	if(pItemInfo->m_TDNCode)
	{
        GameFunc::GetItemDescription(szMessage, NULL, pItemInfo);
		GlobalFunc::SetCaptionSimple(m_pTxtCtrlMemo[index], szMessage);
		ZeroMemory(szMessage, sizeof(szMessage));
	}

	// ������ �÷��̾� �̸�
	if(GetManager()->m_stPresentItemInfo[index].strPlayerName)
	{
		Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, _T("%s"), GetManager()->m_stPresentItemInfo[index].strPlayerName);
		GlobalFunc::SetCaption(m_pTxtCtrlPlayerName[index], szMessage);
		ZeroMemory(szMessage, sizeof(szMessage));
	}

	// ���� �����Ͻ�
	if(GetManager()->m_stPresentItemInfo[index].SendDateTime)
	{
		GlobalFunc::CalcTime(GetManager()->m_stPresentItemInfo[index].SendDateTime, szMessage);
		GlobalFunc::SetCaption(m_pTxtCtrlSendDay[index], szMessage);
		ZeroMemory(szMessage, sizeof(szMessage));
	}
	
	// ���� ���Ⱓ
	if(GetManager()->m_stPresentItemInfo[index].stPiece[0].dwUseTime)	// %d ��
	{
		int day = 0;
		int hour = 0;
		int min = 0;
		GlobalFunc::CalcTime(GetManager()->m_stPresentItemInfo[index].stPiece[0].dwUseTime, day, hour, min, szMessage);
		GlobalFunc::SetCaption(m_pTxtCtrlUseTime[index], szMessage);	
		ZeroMemory(szMessage, sizeof(szMessage));
	}

	// ���� ����
	if(GetManager()->m_stPresentItemInfo[index].stPiece[0].dwAmount)	// %d ��
	{
		TCHAR szTemp[INTERFACE_STRING_LENGTH] ={0,};
		// 155376	%d��
		g_InterfaceManager.GetInterfaceString( 155376 , szTemp ,INTERFACE_STRING_LENGTH);
		Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, szTemp , GetManager()->m_stPresentItemInfo[index].stPiece[0].dwAmount);
		GlobalFunc::SetCaption(m_pTxtCtrlItemCount[index], szMessage);	
		ZeroMemory(szMessage, sizeof(szMessage));
	}

	// ��� ���� ��ư ����
	if(GetManager()->m_stPresentItemInfo[index].byPresentState == PresentRecvBox::PresentInfo::ePS_ACCEPT)
		m_pBtnPresentListDel[index]->EnableWindowWZ(true);
	else
		m_pBtnPresentListDel[index]->EnableWindowWZ(false);

	// ���� Ȯ�� ��ư
	m_pBtnPresentListCheck[index]->EnableWindowWZ(true);
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopPresentDlg::_removeAllIconImage()
{
	for(int i=0; i<eCASH_ITEM_SHOP_PRESENT_DLG_ITEM_MAX;  ++i)
	{
		_removeIconImage(i);
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopPresentDlg::_addIconImage(int iIndex, DWORD dwCode, CODETYPE itemCode)
{
	m_CashItemImg[iIndex].dwIconCode = dwCode;
	m_CashItemImg[iIndex].itemCode = itemCode;
	_removeIconImage(iIndex);

	if (dwCode != 0)
	{
		m_CashItemImg[iIndex].pImage  = ICONMGR()->Add(dwCode);
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopPresentDlg::_removeIconImage(int iIndex)
{
	if(m_CashItemImg[iIndex].pImage)
	{
		ICONMGR()->RemoveIcon(m_CashItemImg[iIndex].pImage);
		ZeroMemory(&(m_CashItemImg[iIndex]), sizeof(m_CashItemImg[iIndex]));
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopPresentDlg::_renderIconImage()
{
	for(int iIndex=0; iIndex < eCASH_ITEM_SHOP_PRESENT_DLG_ITEM_MAX;  ++iIndex)
	{
		if(m_pImgCtrl[iIndex])
		{
			if (m_CashItemImg[iIndex].pImage && m_CashItemImg[iIndex].dwIconCode)
			{
				RECT rc = m_pImgCtrl[iIndex]->GetSizeRect();
				m_CashItemImg[iIndex].pImage->Render(&rc);
                GetManager()->renderCoverImage(m_CashItemImg[iIndex], rc);
			}
		}
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopPresentDlg::_renderTooltip()
{
#ifdef __CN_0_20070507_SHOP_MILEAGE
	uiToolTipMan* pUIMan = (uiToolTipMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_TOOLTIP);
	if(pUIMan)
	{
		//	155387	�ع決�� ����	//	�ϴ� ���ܻ�Ȳ�� �ϳ��� �ϴ� �ϵ��ڵ� �ϳ� �� �þ�� ���ȭ �ʿ��ؿ�
		if(this->m_ItemCodeForTooltip.dwItemCode == 155387)
		{
			pUIMan->SetTooltipDialogID(GetDialogKey());
			pUIMan->RegisterTooltipWithStringCode(m_ItemCodeForTooltip.dwItemCode);
		}
		else if(this->m_ItemCodeForTooltip.dwItemCode > 0)
		{
			pUIMan->SetTooltipDialogID(GetDialogKey());
			pUIMan->RegisterTooltipItemForCashShop(m_ItemCodeForTooltip.dwItemCode, m_ItemCodeForTooltip.dwCoolTime, m_ItemCodeForTooltip.SellEndDate,
				m_ItemCodeForTooltip.byEnchant, m_ItemCodeForTooltip.byRank);
		}
	}
#else	//	__CN_0_20070507_SHOP_MILEAGE
	if(this->m_ItemCodeForTooltip.dwItemCode > 0)
	{
		uiToolTipMan* pUIMan = (uiToolTipMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_TOOLTIP);
		if(pUIMan)
		{
			pUIMan->SetTooltipDialogID(GetDialogKey());
			pUIMan->RegisterTooltipItemForCashShop(m_ItemCodeForTooltip.dwItemCode, m_ItemCodeForTooltip.dwCoolTime, m_ItemCodeForTooltip.SellEndDate,
				m_ItemCodeForTooltip.byEnchant, m_ItemCodeForTooltip.byRank);

		}
	}
#endif	//	__CN_0_20070507_SHOP_MILEAGE
	int iMouseX = Mouse::Instance()->GetMouseX();
	int iMouseY = Mouse::Instance()->GetMouseY();
	// ��ŵó�� 
	// �������̽����� Dialog ���������� Mouse Over�� ���� �ʴ´�.
	BOOL bMouseOver = FALSE;
    for (POSTYPE a = eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_IMG_ITEM1; a <= eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_IMG_ITEM6; ++a)
	{
		CControlWZ * pControl = getControl(a);

		if(pControl)
		{
			RECT rc = pControl->GetSizeRect();

			if (rc.left <= iMouseX && rc.right >= iMouseX)
			{
				if (rc.top <= iMouseY && rc.bottom >= iMouseY)
				{
					bMouseOver = TRUE;
				}
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
void uiCashItemShopPresentDlg::SetCurPage()
{
	TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szTemp[INTERFACE_STRING_LENGTH] ={0,};
	//5796	%d������
	g_InterfaceManager.GetInterfaceString( 5796 , szTemp ,INTERFACE_STRING_LENGTH);
	Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, szTemp, m_iCurPage + 1);
	GlobalFunc::SetCaption(m_pTxtCtrlCurPage, szMessage);	// ���� ������
}


//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopPresentDlg::OnShowWindow( BOOL val )
{
	if (val)
	{
		//this->MoveDefaultWindowPos();

		if (!this->m_bControlsInitialized)
		{
			this->ShowInterface(FALSE);
			return;
		}

		keyMsg msg;
		ZeroMemory(&msg,sizeof(msg));
		msg.dwType = eDoType_dialog_cash_item_shop_present;
		msg.wParam = InterfaceManager::DIALOG_CASH_ITEM_SHOP_PRESENT_DLG;
		msg.DoSomething = GlobalFunc::DoShowWindow;
		g_KeyQueueManager.PushBack(msg);

		m_bShow = true;

		_prepareShow();
	}
	else
	{
		g_KeyQueueManager.DeleteMsg(eDoType_dialog_cash_item_shop_present);
		_removeAllIconImage();
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopPresentDlg::_reSetChkBtn( void )
{
	if(m_pBtnCheckTabs[eCASH_ITEM_SHOP_PRESENT_DLG_CHK_CASH_ITEM])
		m_pBtnCheckTabs[eCASH_ITEM_SHOP_PRESENT_DLG_CHK_CASH_ITEM]->SetCheckState(false);

	if(m_pBtnCheckTabs[eCASH_ITEM_SHOP_PRESENT_DLG_CHK_MY_PAGE])
		m_pBtnCheckTabs[eCASH_ITEM_SHOP_PRESENT_DLG_CHK_MY_PAGE]->SetCheckState(true);

	if(m_pBtnCheckTabs[eCASH_ITEM_SHOP_PRESENT_DLG_CHK_PACKAGE])
		m_pBtnCheckTabs[eCASH_ITEM_SHOP_PRESENT_DLG_CHK_PACKAGE]->SetCheckState(false);

	if(m_pBtnCheckMenu[eCASH_ITEM_SHOP_PRESENT_DLG_CHK_PRESENT])
		m_pBtnCheckMenu[eCASH_ITEM_SHOP_PRESENT_DLG_CHK_PRESENT]->SetCheckState(true);

#ifdef __NA_00387_20070605_SHOP_MILEAGE2
	if(m_pBtnCheckTabs[eCASH_ITEM_SHOP_PRESENT_DLG_CHK_MILEAGE])
		m_pBtnCheckTabs[eCASH_ITEM_SHOP_PRESENT_DLG_CHK_MILEAGE]->SetCheckState(false);
#endif	//	__NA_00387_20070605_SHOP_MILEAGE2

}
//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopPresentDlg::OnUpdateSolarDialog( void )
{
	if(!m_bShow)
		this->ShowInterface(FALSE);

	if (!_isEnable())
		return;

	_reSetChkBtn();

    CCtrlButtonCheckWZ* button_of_to_buy_w_coin = static_cast<CCtrlButtonCheckWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_PRESENT_BTN_WCOIN));
    if (button_of_to_buy_w_coin != NULL)
    {
        button_of_to_buy_w_coin->SetCheckState(false);
    }
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopPresentDlg::OnRenderSolarDialog()
{
	if (!_isEnable())
		return;

	_renderIconImage();
	_renderTooltip();
}


//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopPresentDlg::MessageProc( SI_MESSAGE * pMessage )
{
	if (!_isEnable())
		return;

	int iIndex;

	switch( pMessage->eResultMsg ) 
	{
	case RT_MSG_LBUTTONCLICK:
		{
			int	iCtrlID = getCtrlIDToPos( pMessage->dwCtrlID );
			switch( iCtrlID )
			{
            case eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_CHK_PRESENT:
            case eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_CHK_SEND:
                _updateMenu(static_cast<eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG>(iCtrlID));
                break;

			case eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_BTN_EXIT:
				{
					m_bShow = false;
				}
				break;

			case eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_BTN_PRESENTLISTCHECK1:
			case eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_BTN_PRESENTLISTCHECK2:
			case eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_BTN_PRESENTLISTCHECK3:
			case eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_BTN_PRESENTLISTCHECK4:
			case eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_BTN_PRESENTLISTCHECK5:
			case eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_BTN_PRESENTLISTCHECK6:
				{
					iIndex = iCtrlID - eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_BTN_PRESENTLISTCHECK1;

					if(GetManager()->m_stPresentItemInfo[iIndex].giftGuid > 0)
					{
						GetManager()->SetPresentState(ePresentState_Rcv);
						_setIndex( iIndex );
					}
				}
				break;

			case eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_BTN_PRESENTLISTDEL1:
			case eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_BTN_PRESENTLISTDEL2:
			case eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_BTN_PRESENTLISTDEL3:
			case eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_BTN_PRESENTLISTDEL4:
			case eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_BTN_PRESENTLISTDEL5:
			case eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_BTN_PRESENTLISTDEL6:
				{
					iIndex = iCtrlID - eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_BTN_PRESENTLISTDEL1;

					if(GetManager()->m_stPresentItemInfo[iIndex].giftGuid > 0)
					{
						GetManager()->SetPresentState(ePresentState_None);
						_setIndex( iIndex );
						CopyMemory(&(GetManager()->m_pendingBuyPresentItemInfo), &GetManager()->m_stPresentItemInfo[iIndex], sizeof(GetManager()->m_stPresentItemInfo[iIndex]));
						GetManager()->Send_CG_SHOP_ACTION_RECV_GIFT_SYN(MSG_CG_SHOP_ACTION_RECV_GIFT_SYN::DISCARD);
					}
				}
				break;

			case eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_CHK_CASH:
				{
					GetManager()->ShowCashItemShopDlg(true);
				}
				break;

			case eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_CHK_PACKAGE:
				{
					GetManager()->ShowCashItemShopPackageDlg(true);
				}
				break;

#ifdef __NA_00387_20070605_SHOP_MILEAGE2
			case eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_CHK_MILEAGE:
				{
					GetManager()->ShowCashItemShopMileageDlg(true);
				}
				break;
#endif	//	__NA_00387_20070605_SHOP_MILEAGE2


            case eDIALOG_CASH_ITEM_SHOP_PRESENT_BTN_WCOIN:
                {
                    GetManager()->ExecBuyWCoin();
                }
                break;

			case eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_BTN_PREV_PAGE:
				{
					int iPage = m_iCurPage;
					if( iPage > 0)
					{
						iPage--;
						_setPage(iPage);
					}
				}
				break;
			case eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_BTN_NEXT_PAGE:
				{
					int iPage = m_iCurPage;
					if(iPage < GetTotalPage())
					{
						iPage++;
						_setPage(iPage);
					}
				}
				break;

			}
		}
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
void uiCashItemShopPresentDlg::MouseOver_MsgProc( SI_MESSAGE * pMessage )
{
	if (!_isEnable())
		return;

	switch( pMessage->eResultMsg ) 
	{
	case RT_MSG_MOUSEOVER:
		{
			POSTYPE pos = getCtrlIDToPos( pMessage->dwCtrlID );

			if (pos >= eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_IMG_ITEM1 && pos <= eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_IMG_ITEM6)
			{
				int iIndex = pos - eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_IMG_ITEM1;
				if (m_CashItemImg[iIndex].pImage && m_CashItemImg[iIndex].dwIconCode)
				{
					m_ItemCodeForTooltip.dwItemCode = m_CashItemImg[iIndex].itemCode;
					m_ItemCodeForTooltip.byEnchant	= GetManager()->m_stPresentItemInfo[iIndex].stPiece[0].byEnchant;
					m_ItemCodeForTooltip.byRank		= GetManager()->m_stPresentItemInfo[iIndex].stPiece[0].byRank;
				}
				else
				{
					ZeroMemory(&m_ItemCodeForTooltip, sizeof(m_ItemCodeForTooltip));
				}
			}
#ifdef __CN_0_20070507_SHOP_MILEAGE
		    else if(pos == eDIALOG_CASH_ITEM_SHOP_PRESENT_DLG_BTN_MILEGE)
		    {
			    //	155387	�ع決�� ����
			    m_ItemCodeForTooltip.dwItemCode = 155387;
		    }
#endif	//	__CN_0_20070507_SHOP_MILEAGE
			else
			{
				ZeroMemory(&m_ItemCodeForTooltip, sizeof(m_ItemCodeForTooltip));
			}			
		}
		break;
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopPresentDlg::SetCurMoney( void )
{
    CASH normal_cash_coin = GetManager()->GetMyCash();    // �Ϲ�ĳ��
    CASH pp_card_coin = GetManager()->GetPPCash();        // PPĳ��
    MONEY total_coin = static_cast<MONEY>(normal_cash_coin) + pp_card_coin;       // ĳ������

    GlobalFunc::SetMoneyText(m_pTxtCtrlCCCoin, normal_cash_coin, CASH_ITEM_SHOP_DEFAULT);
    GlobalFunc::SetMoneyText(m_pTxtCtrlPPCoin, pp_card_coin, CASH_ITEM_SHOP_DEFAULT);
	GlobalFunc::SetMoneyText(m_pTxtCtrlMilege, GetManager()->GetMyMileage(), CASH_ITEM_SHOP_MILEAGE);

	if(LOGINPARAM->IsLiveTestServer())	//�׼�������
	{	
		GlobalFunc::SetMoneyText(m_pTxtCtrlMoney, g_pHero->GetMoney(), CASH_ITEM_SHOP_HAIM);
	}
	else
	{
		GlobalFunc::SetMoneyText(m_pTxtCtrlMoney, total_coin, CASH_ITEM_SHOP_DEFAULT);
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopPresentDlg::RefreshPresentItemList()
{
#ifdef _DEBUG
	TCHAR szDebug[INTERFACE_STRING_LENGTH] = {0,};
	Snprintf(szDebug, INTERFACE_STRING_LENGTH-1, _T("%d Page\n"), m_iCurPage);
	OutputDebugString(szDebug);
#endif
	_clearAllItem();
	SetCurPage();
	SetCurMoney();
}

//------------------------------------------------------------------------------ 
#endif //#ifndef _NA_0_20110613_GAME_SHOP_RENEWAL //#if'N'def
//------------------------------------------------------------------------------ 
