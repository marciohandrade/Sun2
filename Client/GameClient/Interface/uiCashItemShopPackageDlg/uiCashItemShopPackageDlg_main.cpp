#include "SunClientPrecompiledHeader.h"

//------------------------------------------------------------------------------ 
#ifndef _NA_0_20110613_GAME_SHOP_RENEWAL //#if'N'def
//------------------------------------------------------------------------------ 


#include "uiCashItemShopPackageDlg.h"
#include "uiCashItemShopMan/uiCashItemShopMan.h"
#include "uiSystemMan/uiSystemMan.h"
#include "Hero.h"
#include "GlobalFunc.h"
#include "interfacemanager.h"
#include "GameFramework.h"
#include "ItemInfoParser.h"
#include "Mouse.h"
#include "uiToolTipMan/uiToolTipMan.h"

//------------------------------------------------------------------------------
WzID uiCashItemShopPackageDlg::m_wzId[eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_MAX] = 
{
	StrToWzID("B999"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_EXIT = 0,			//	X ��ư
	StrToWzID("B001"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_IMG_ITEM1,			//	������ �̹��� 1
	StrToWzID("B002"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_IMG_ITEM2,			//	������ �̹��� 2
	StrToWzID("B003"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_IMG_ITEM3,			//	������ �̹��� 3
    StrToWzID("BT02"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_BTN_MYMILEGE,		//	�ع決�� ���� �׸� ��ư
	StrToWzID("C001"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_CHK_CASHITEM,		//	ĳ�þ����� üũ��ư
	StrToWzID("C002"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_CHK_MYPAGE,			//	���������� üũ��ư
    StrToWzID("C003"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_CHK_PACKAGE,		    //	��Ű�� üũ��ư
    StrToWzID("CT01"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_CHK_MAILAGE,			//	���ϸ��� üũ��ư
	StrToWzID("C010"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_CHK_ALLITEM,			//	�������� üũ��ư
	StrToWzID("C011"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_CHK_NEWITEM,			//	�űԻ�ǰ üũ��ư
	StrToWzID("C012"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_CHK_BESTITEM,		//	�α��ǰ üũ��ư

    StrToWzID("C020"),  // eDIALOG_CASH_ITEM_SHOP_DLG_CHK_EXTENSION,		        //	�Ⱓ����

	StrToWzID("T002"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE_NAME1,	//	��Ű�� �̸�1
	StrToWzID("T003"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE_NAME2,	//	��Ű�� �̸�2
	StrToWzID("T004"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE_NAME3,	//	��Ű�� �̸�3

	StrToWzID("T073"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE_CASH1,	//	��Ű�� ����1
	StrToWzID("T074"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE_CASH2,	//	��Ű�� ����2
	StrToWzID("T075"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE_CASH3,	//	��Ű�� ����3

	StrToWzID("B016"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_BTN_BUY1,			//	��Ű�� ���� ��ư1
	StrToWzID("B018"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_BTN_BUY2,			//	��Ű�� ���� ��ư2
	StrToWzID("B020"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_BTN_BUY3,			//	��Ű�� ���� ��ư2

	StrToWzID("T010"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE1_ITEMNAME1,	//	��Ű��1 ������ �̸�1
	StrToWzID("T011"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE1_ITEMNAME2,	//	��Ű��1 ������ �̸�2
	StrToWzID("T012"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE1_ITEMNAME3,	//	��Ű��1 ������ �̸�3
	StrToWzID("T013"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE1_ITEMNAME4,	//	��Ű��1 ������ �̸�4
	StrToWzID("T014"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE1_ITEMNAME5,	//	��Ű��1 ������ �̸�5
    StrToWzID("S001"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE1_ITEMNAME6,	//	��Ű��1 ������ �̸�4
    StrToWzID("S002"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE1_ITEMNAME7,	//	��Ű��1 ������ �̸�5
	StrToWzID("T020"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE2_ITEMNAME1,	//	��Ű��2 ������ �̸�1
	StrToWzID("T021"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE2_ITEMNAME2,	//	��Ű��2 ������ �̸�2
	StrToWzID("T022"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE2_ITEMNAME3,	//	��Ű��2 ������ �̸�3
	StrToWzID("T023"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE2_ITEMNAME4,	//	��Ű��2 ������ �̸�4
	StrToWzID("T024"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE2_ITEMNAME5,	//	��Ű��2 ������ �̸�5
    StrToWzID("S007"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE2_ITEMNAME6,	//	��Ű��2 ������ �̸�6
    StrToWzID("S010"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE2_ITEMNAME7,	//	��Ű��2 ������ �̸�7
	StrToWzID("T030"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE3_ITEMNAME1,	//	��Ű��3 ������ �̸�1
	StrToWzID("T031"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE3_ITEMNAME2,	//	��Ű��3 ������ �̸�2
	StrToWzID("T032"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE3_ITEMNAME3,	//	��Ű��3 ������ �̸�3
	StrToWzID("T033"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE3_ITEMNAME4,	//	��Ű��3 ������ �̸�4
	StrToWzID("T034"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE3_ITEMNAME5,	//	��Ű��3 ������ �̸�5
    StrToWzID("S013"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE3_ITEMNAME6,	//	��Ű��3 ������ �̸�6
    StrToWzID("S016"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE3_ITEMNAME7,	//	��Ű��3 ������ �̸�7

	StrToWzID("ST01"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE1_ITEMUSETIME1,	//	��Ű��1 ������ ���Ⱓ1
	StrToWzID("ST02"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE1_ITEMUSETIME2,	//	��Ű��1 ������ ���Ⱓ2
	StrToWzID("ST03"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE1_ITEMUSETIME3,	//	��Ű��1 ������ ���Ⱓ3
	StrToWzID("ST04"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE1_ITEMUSETIME4,	//	��Ű��1 ������ ���Ⱓ4
	StrToWzID("ST05"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE1_ITEMUSETIME5,	//	��Ű��1 ������ ���Ⱓ5
    StrToWzID("S006"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE1_ITEMUSETIME6,	//	��Ű��1 ������ ���Ⱓ6
    StrToWzID("S004"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE1_ITEMUSETIME7,	//	��Ű��1 ������ ���Ⱓ7
	StrToWzID("ST06"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE2_ITEMUSETIME1,	//	��Ű��2 ������ ���Ⱓ1
	StrToWzID("ST07"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE2_ITEMUSETIME2,	//	��Ű��2 ������ ���Ⱓ2
	StrToWzID("ST08"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE2_ITEMUSETIME3,	//	��Ű��2 ������ ���Ⱓ3
	StrToWzID("ST09"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE2_ITEMUSETIME4,	//	��Ű��2 ������ ���Ⱓ4
	StrToWzID("ST10"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE2_ITEMUSETIME5,	//	��Ű��2 ������ ���Ⱓ5
    StrToWzID("S009"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE2_ITEMUSETIME6,	//	��Ű��2 ������ ���Ⱓ6
    StrToWzID("S012"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE2_ITEMUSETIME7,	//	��Ű��2 ������ ���Ⱓ7

	StrToWzID("ST11"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE3_ITEMUSETIME1,	//	��Ű��3 ������ ���Ⱓ1
	StrToWzID("ST12"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE3_ITEMUSETIME2,	//	��Ű��3 ������ ���Ⱓ2
	StrToWzID("ST13"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE3_ITEMUSETIME3,	//	��Ű��3 ������ ���Ⱓ3
	StrToWzID("ST14"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE3_ITEMUSETIME4,	//	��Ű��3 ������ ���Ⱓ4
	StrToWzID("ST15"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE3_ITEMUSETIME5,	//	��Ű��3 ������ ���Ⱓ5
    StrToWzID("S015"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE3_ITEMUSETIME6,	//	��Ű��3 ������ ���Ⱓ6
    StrToWzID("S018"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE3_ITEMUSETIME7,	//	��Ű��3 ������ ���Ⱓ7
	StrToWzID("T015"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE1_ITEMAMOUT1,	//	��Ű��1 ������ ����1
	StrToWzID("T016"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE1_ITEMAMOUT2,	//	��Ű��1 ������ ����2
	StrToWzID("T017"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE1_ITEMAMOUT3,	//	��Ű��1 ������ ����3
	StrToWzID("T018"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE1_ITEMAMOUT4,	//	��Ű��1 ������ ����4
	StrToWzID("T019"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE1_ITEMAMOUT5,	//	��Ű��1 ������ ����5
    StrToWzID("S005"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE1_ITEMAMOUT6,	//	��Ű��1 ������ ����6
    StrToWzID("S003"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE1_ITEMAMOUT7,	//	��Ű��1 ������ ����7

	StrToWzID("T025"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE2_ITEMAMOUT1,	//	��Ű��2 ������ ����1
	StrToWzID("T026"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE2_ITEMAMOUT2,	//	��Ű��2 ������ ����2
	StrToWzID("T027"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE2_ITEMAMOUT3,	//	��Ű��2 ������ ����3
	StrToWzID("T028"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE2_ITEMAMOUT4,	//	��Ű��2 ������ ����4
	StrToWzID("T029"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE2_ITEMAMOUT5,	//	��Ű��2 ������ ����5
    StrToWzID("S008"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE2_ITEMAMOUT6,	//	��Ű��2 ������ ����6
    StrToWzID("S011"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE2_ITEMAMOUT7,	//	��Ű��2 ������ ����7
	StrToWzID("T035"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE3_ITEMAMOUT1,	//	��Ű��3 ������ ����1
	StrToWzID("T036"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE3_ITEMAMOUT2,	//	��Ű��3 ������ ����2
	StrToWzID("T037"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE3_ITEMAMOUT3,	//	��Ű��3 ������ ����3
	StrToWzID("T038"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE3_ITEMAMOUT4,	//	��Ű��3 ������ ����4
	StrToWzID("T039"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE3_ITEMAMOUT5,	//	��Ű��3 ������ ����5
    StrToWzID("S014"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE3_ITEMAMOUT6,	//	��Ű��3 ������ ����6
    StrToWzID("S017"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE3_ITEMAMOUT7,	//	��Ű��3 ������ ����7

	StrToWzID("VS00"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_VSCR1,					//	��Ű�� ��ũ��1
	StrToWzID("VS01"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_VSCR2,					//	��Ű�� ��ũ��2
	StrToWzID("VS02"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_VSCR3,					//	��Ű�� ��ũ��3

	StrToWzID("BT00"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_BTN_PREVPAGE,			//	���� ������ ��ư
	StrToWzID("BT01"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_BTN_NEXTPAGE,			//	���� ������ ��ư

	StrToWzID("ST00"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_CURPAGE,				//	���� ������ �ؽ�Ʈ

	StrToWzID("T057"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_MYMONEY,				//	���� �����ݾ�
    StrToWzID("PI13"),  // eDIALOG_CASH_ITEM_SHOP_DLG_BTN_WCOIN_BACK
    StrToWzID("PI11"),  // eDIALOG_CASH_ITEM_SHOP_DLG_IMG_WCOINC_BACK
    StrToWzID("PI12"),  // eDIALOG_CASH_ITEM_SHOP_DLG_IMG_WCOINP_BACK
    StrToWzID("ST21"),  // eDIALOG_CASH_ITEM_SHOP_DLG_STXT_WCOINP
    StrToWzID("ST20"),  // eDIALOG_CASH_ITEM_SHOP_DLG_STXT_WCOINC
    StrToWzID("ST17"),  // eDIALOG_CASH_ITEM_SHOP_DLG_STXT_MILEAGE
    StrToWzID("ST32"),  // eDIALOG_CASH_ITEM_SHOP_DLG_TXT_WCOINP
    StrToWzID("ST19"),  // eDIALOG_CASH_ITEM_SHOP_DLG_TXT_WCOINC
    StrToWzID("ST22"),  // eDIALOG_CASH_ITEM_SHOP_DLG_TXT_MILEGE , eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_MYMILEGE
    StrToWzID("CT02"),  // eDIALOG_CASH_ITEM_SHOP_DLG_BTN_MILEGE , eDIALOG_CASH_ITEM_SHOP_DLG_BTN_BUYWCOIN

    StrToWzID("B000"),  // eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_BTN_SORTCLASS,
    StrToWzID("L001"),  // eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_LIST_SORTCLASS
    StrToWzID("S000"),  // eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TEXT_SORTCLASS

};

//------------------------------------------------------------------------------
/**
*/
uiCashItemShopPackageDlg::uiCashItemShopPackageDlg(InterfaceManager *pUIMan) :
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
	m_iCurMenu = -1;
	m_iCurPage = -1;
	ZeroMemory(&m_PackageItemImg, sizeof(m_PackageItemImg));
	ZeroMemory(&m_ItemCodeForTooltip, sizeof(m_ItemCodeForTooltip));
}

//------------------------------------------------------------------------------
/**
*/
uiCashItemShopPackageDlg::~uiCashItemShopPackageDlg()
{
	this->Release();
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopPackageDlg::Init(CDrawBase* pDrawBase)
{
	uiBase::Init(pDrawBase);

	this->InitControls();
}


//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopPackageDlg::Release()
{

}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopPackageDlg::InitControls()
{
	int i,j;
	for (i=0; i<eCASH_ITEM_SHOP_PACKAGE_DLG_CHK_MAX; ++i)
	{
		m_pBtnCheckTabs[i] = static_cast<CCtrlButtonCheckWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_CHK_CASHITEM+i));
		assert(m_pBtnCheckTabs[i]);
	}
    #ifdef __CN_0_20070507_SHOP_MILEAGE
    #else
    if (m_pBtnCheckTabs[eCASH_ITEM_SHOP_PACKAGE_DLG_CHK_MILEAGE])
    {
        m_pBtnCheckTabs[eCASH_ITEM_SHOP_PACKAGE_DLG_CHK_MILEAGE]->ShowWindowWZ(WZ_HIDE);
    }
    #endif//__CN_0_20070507_SHOP_MILEAGE

#ifdef __CHINA_BLOCK_ITEM_SHOP_PRESENT
	if ( m_pBtnCheckTabs[eCASH_ITEM_SHOP_PACKAGE_DLG_CHK_MY_PAGE] )
	{
		m_pBtnCheckTabs[eCASH_ITEM_SHOP_PACKAGE_DLG_CHK_MY_PAGE]->EnableWindowWZ(FALSE);
		m_pBtnCheckTabs[eCASH_ITEM_SHOP_PACKAGE_DLG_CHK_MY_PAGE]->SetCheckState(FALSE);
	}
#endif //__CHINA_BLOCK_ITEM_SHOP_PRESENT

	for (i=0; i<eCASH_ITEM_SHOP_PACKAGE_DLG_CHK_MENU_MAX; ++i)
	{
		m_pBtnCheckMenu[i] = static_cast<CCtrlButtonCheckWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_CHK_ALLITEM+i));
		assert(m_pBtnCheckMenu[i]);
	}

	int iIndex = 0;
	for (i=0; i<eCASH_ITEM_SHOP_PACKAGE_DLG_ITEM_MAX; ++i)
	{
		m_pImgCtrl[i] = static_cast<CCtrlImageStaticWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_IMG_ITEM1+i));
		assert(m_pImgCtrl[i]);

		m_pTxtCtrlPackageName[i] = static_cast<CCtrlStaticWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE_NAME1+i));
		assert(m_pTxtCtrlPackageName[i]);

		m_pTxtCtrlPackageInfo[i] = static_cast<CCtrlStaticWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE_CASH1+i));
		assert(m_pTxtCtrlPackageInfo[i]);


		m_pScroll[i] = static_cast<CCtrlVScrollWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_VSCR1+i));
		assert(m_pScroll[i]);


		for (j=0; j<eCASH_ITEM_SHOP_PACKAGE_DLG_DETAILITEM_MAX; ++j)
		{
			m_pTxtCtrlItemName[i][j] = static_cast<CCtrlStaticWZ *>(getControl( eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE1_ITEMNAME1 + (i*eCASH_ITEM_SHOP_PACKAGE_DLG_DETAILITEM_MAX) + j));
			assert(m_pTxtCtrlItemName[i][j]);

			m_pTxtCtrlItemUseTime[i][j] = static_cast<CCtrlStaticWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE1_ITEMUSETIME1 + (i*eCASH_ITEM_SHOP_PACKAGE_DLG_DETAILITEM_MAX) + j));
			assert(m_pTxtCtrlItemUseTime[i][j]);

			m_pTxtCtrlItemAmout[i][j] = static_cast<CCtrlStaticWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE1_ITEMAMOUT1 + (i*eCASH_ITEM_SHOP_PACKAGE_DLG_DETAILITEM_MAX) + j));
			assert(m_pTxtCtrlItemAmout[i][j]);
		}
	}

	m_pTxtCtrlCurPage = static_cast<CCtrlStaticWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_CURPAGE));
	assert(m_pTxtCtrlCurPage);

	m_pTxtCtrlMoney = static_cast<CCtrlStaticWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_MYMONEY));
	assert(m_pTxtCtrlMoney);

    CCtrlStaticWZ* control_txt;
    m_pTxtCtrlPPCoin = static_cast<CCtrlStaticWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_DLG_TXT_WCOINP));
    SUN_ASSERT(m_pTxtCtrlPPCoin);
    m_pTxtCtrlCCCoin = static_cast<CCtrlStaticWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_DLG_TXT_WCOINC));
    SUN_ASSERT(m_pTxtCtrlCCCoin);
    m_pTxtCtrlMilege = static_cast<CCtrlStaticWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_MYMILEGE));
	assert(m_pTxtCtrlMilege);

    CCtrlPictureWZ* control_picture = NULL;

    #ifdef _NA00XXXX_090111_CASH_TYPE_SUBDIVIDE_//PPī�� ���� ���
        //����Ǹ� ������ �ʴ´�
    #else
        for (i=0; i<eCASH_ITEM_SHOP_TXT_MAX; ++i)
        {
            control_txt = static_cast<CCtrlStaticWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_DLG_STXT_WCOINP+i));
            SUN_ASSERT(control_txt);
            if(control_txt)
            {
                control_txt->ShowWindowWZ(WZ_HIDE);
            }
        }

        for (i=0; i<eCASH_ITEM_SHOP_IMG_MAX; ++i)
        {
            control_picture = static_cast<CCtrlPictureWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_DLG_BTN_WCOIN_BACK+i));
            SUN_ASSERT(control_picture);
            if(control_picture && (i != eCASH_ITEM_SHOP_BTN_WCOIN_BACK) )
            {
                control_picture->ShowWindowWZ(WZ_HIDE);
            }
        }

        CCtrlButtonWZ* button_of_to_buy_w_coin = static_cast<CCtrlButtonWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_DLG_BTN_BUYWCOIN));
        SUN_ASSERT(button_of_to_buy_w_coin);
        if (button_of_to_buy_w_coin)
        {
            button_of_to_buy_w_coin->ShowWindowWZ(WZ_HIDE);
        }
    #endif//_NA00XXXX_090111_CASH_TYPE_SUBDIVIDE_

    #ifdef __CN_0_20070507_SHOP_MILEAGE
        if (m_pTxtCtrlMilege)
        {
            m_pTxtCtrlMilege->ShowWindowWZ(WZ_SHOW);
        }

        control_txt = static_cast<CCtrlStaticWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_DLG_STXT_MILEAGE));
        SUN_ASSERT(control_txt);
        if( control_txt)
        {
            control_txt->ShowWindowWZ(WZ_SHOW);
        }

        control_picture = static_cast<CCtrlPictureWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_DLG_IMG_WCOINP_BACK));
        SUN_ASSERT(control_picture);
        if (control_picture)
        {
            control_picture->ShowWindowWZ(WZ_SHOW);
        }
    #else
        control_txt = static_cast<CCtrlStaticWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_DLG_STXT_MILEAGE));
        SUN_ASSERT(control_txt);
        if (control_txt)
        {
            control_txt->ShowWindowWZ(WZ_HIDE);
        }

        control_txt = static_cast<CCtrlStaticWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_MYMILEGE));
        SUN_ASSERT(control_txt);
        if (control_txt)
        {
            control_txt->ShowWindowWZ(WZ_HIDE);
        }

        CCtrlButtonWZ* button_of_mymilege = static_cast<CCtrlButtonWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_BTN_MYMILEGE));
        SUN_ASSERT(button_of_mymilege);
        if (button_of_mymilege)
        {
            button_of_mymilege->ShowWindowWZ(WZ_HIDE);
        }
    #endif//__CN_0_20070507_SHOP_MILEAGE
	this->m_bControlsInitialized = true;

    CCtrlListWZ* list_control = static_cast<CCtrlListWZ*>(getControl(eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_LIST_SORTCLASS));
    GetManager()->FillSortCombo(list_control);
    if (list_control != NULL)
    {
        list_control->SetCurSel(0);
        OnSelectedCombo(eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_LIST_SORTCLASS, 
                        eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TEXT_SORTCLASS, 
                        list_control->GetCurSel());
    }

    CControlWZ* check_button = getControl(eDIALOG_CASH_ITEM_SHOP_DLG_CHK_EXTENSION);
    if (check_button != NULL)
    {
        check_button->ShowWindowWZ(WZ_HIDE);
    }

}

//------------------------------------------------------------------------------ 
WORD uiCashItemShopPackageDlg::_getFilter()
{
    WORD filter = SHOP_FILTER_NONE;
    if (m_iCurMenu == 1)
    {
        filter |= SHOP_FILTER_NEWARRIVAL;
    }
    else if (m_iCurMenu == 2)
    {
        filter |= SHOP_FILTER_POPULAR;
    }

    CCtrlListWZ* list_control = static_cast<CCtrlListWZ*>(getControl(eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_LIST_SORTCLASS));

    //2544	��ü Ŭ����	ĳ�û��� ����
    //2545	����Ŀ	ĳ�û��� ����
    //2546	�巡��	ĳ�û��� ����
    //2547	��Ű�� 	ĳ�û��� ����
    //2548	������Ż	ĳ�û��� ����
    //2549	������	ĳ�û��� ����
    switch (list_control->GetCurSel())
    {
    case 0: filter |= SHOP_FILTER_CHAR_ALL; break;
    case 1: filter |= SHOP_FILTER_CHAR_BERSERKER; break;
    case 2: filter |= SHOP_FILTER_CHAR_PALADIN; break;
    case 3: filter |= SHOP_FILTER_CHAR_VALKYRIE; break;
    case 4: filter |= SHOP_FILTER_CHAR_ELEMENTALIST; break;
    case 5: filter |= SHOP_FILTER_CHAR_SHADOW; break;
    }

    return filter;
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopPackageDlg::_setMenu(int iMenu, bool force_change)
{


    if ((m_iCurMenu == iMenu) && (force_change == false))
    {
        return;
    }

    BYTE byCategory = _getCategoryType(iMenu);

    //! �Ⱓ ��������
    if (byCategory == SHOP_CATEGORY_PERIOD_EXTEND)
    {
        GetManager()->OnClickExtensionButton(m_iCurPage);
    }
    else    // if (byCategory == SHOP_CATEGORY_ALL) //��Ű�� �޴����� �����Ѵ�.
    {
        m_iCurMenu = iMenu;
        m_iCurPage = 0;

        WORD filter = _getFilter();

        _updateMenu();
        GetManager()->Send_CG_SHOP_ITEM_LIST_REQ_SYN(SHOP_PARTITION_PACKAGEITEM, byCategory, filter, m_iCurPage);
    }

}

//------------------------------------------------------------------------------
/**
*/
BYTE uiCashItemShopPackageDlg::_getCategoryType(int iMenu)
{
//------------------------------------------------------------------------------ 
//!

    switch(iMenu)
    {
    case eCASH_ITEM_SHOP_DLG_CHK_EXTENSION:     
        return SHOP_CATEGORY_PERIOD_EXTEND;

    case eCASH_ITEM_SHOP_PACKAGE_DLG_CHK_MENU_TOTAL:    
    case eCASH_ITEM_SHOP_PACKAGE_DLG_CHK_MENU_NEW:
    case eCASH_ITEM_SHOP_PACKAGE_DLG_CHK_MENU_BEST:
    default:
        return SHOP_CATEGORY_ALL;
    }
    return SHOP_CATEGORY_ALL;
}
//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopPackageDlg::_updateMenu()
{
	for(int i=0; i<eCASH_ITEM_SHOP_PACKAGE_DLG_CHK_MENU_MAX; ++i)
    {
		m_pBtnCheckMenu[i]->SetCheckState(false);
    }

	if(m_pBtnCheckMenu[m_iCurMenu])
    {
		m_pBtnCheckMenu[m_iCurMenu]->SetCheckState(true);
    }
    CCtrlButtonCheckWZ* button_of_to_buy_w_coin = static_cast<CCtrlButtonCheckWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_DLG_BTN_BUYWCOIN));
    if (button_of_to_buy_w_coin != NULL)
    {
        button_of_to_buy_w_coin->SetCheckState(false);
    }
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopPackageDlg::_setPage(int iPage)
{
	if (m_iCurPage == iPage)
    {
		return;
    }

	//m_iCurPage = iPage;

    BYTE byCategory = _getCategoryType(m_iCurMenu);
    WORD filter = _getFilter();

    GetManager()->Send_CG_SHOP_ITEM_LIST_REQ_SYN(SHOP_PARTITION_PACKAGEITEM, byCategory, filter, iPage);
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopPackageDlg::_prepareShow()
{
	if (!_isEnable())
		return;
	GetManager()->PrepareShow(eCashItemShopDlgState_Package);

	ZeroMemory(&m_ItemCodeForTooltip, sizeof(m_ItemCodeForTooltip));
	_clearAllItem();

	m_iCurMenu = -1;
	m_iCurPage = -1;

	_setMenu(eCASH_ITEM_SHOP_PACKAGE_DLG_CHK_MENU_TOTAL);
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopPackageDlg::_setDetialItem(int index, int iStartIndex )
{
	int i;
	TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szTemp[INTERFACE_STRING_LENGTH] ={0,};
	TCHAR szTemp2[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szTemp3[INTERFACE_STRING_LENGTH] = {0,};


	stPackageItemInfo* pPackageInfo = NULL;
	pPackageInfo = &GetManager()->m_stPackageItemInfo[index];


	stPackageDetailInfo* pPackaDetailIteminfo;
	for(i = 0; i < eCASH_ITEM_SHOP_PACKAGE_DLG_DETAILITEM_MAX; ++i)
	{
		pPackaDetailIteminfo = NULL;
		pPackaDetailIteminfo = &pPackageInfo->stDetailItem[i+iStartIndex];

		if(pPackaDetailIteminfo)
		{
			// �̸�
			if(pPackaDetailIteminfo->dwItemCode)
			{
				g_InterfaceManager.GetItemName(pPackaDetailIteminfo->dwItemCode, szMessage, INTERFACE_STRING_LENGTH);

				if(pPackaDetailIteminfo->byEnchant)
				{
					Snprintf(szTemp, INTERFACE_STRING_LENGTH-1, _T("+%d"), pPackaDetailIteminfo->byEnchant);
					_tcscat(szMessage, szTemp);
				}

				BASE_ITEMINFO * pItemInfo = ItemInfoParser::Instance()->GetItemInfo( pPackaDetailIteminfo->dwItemCode );
				if( pItemInfo && pItemInfo->CanRankUpItem() )
				{
					int iRankIndex = eST_RANK_E_STRING_INDEX  + (pPackaDetailIteminfo->byRank);
					if (iRankIndex > eST_RANK_PS_STRING_INDEX)
						iRankIndex = eST_RANK_PS_STRING_INDEX;
					if (iRankIndex < eST_RANK_E_STRING_INDEX)
						iRankIndex = eST_RANK_E_STRING_INDEX;

					ZeroMemory(szTemp, sizeof(szTemp));
					ZeroMemory(szTemp2, sizeof(szTemp2));
					ZeroMemory(szTemp3, sizeof(szTemp3));
					g_InterfaceManager.GetInterfaceString(eST_RANK_STRING_INDEX,szTemp,INTERFACE_STRING_LENGTH);
					g_InterfaceManager.GetInterfaceString(iRankIndex,szTemp2,INTERFACE_STRING_LENGTH);	
					Snprintf(szTemp3,INTERFACE_STRING_LENGTH-1, szTemp, szTemp2);
					_tcscat(szMessage, _T("   "));
					_tcscat(szMessage, szTemp3);
				}
				

				GlobalFunc::SetCaption(m_pTxtCtrlItemName[index][i], szMessage);
				ZeroMemory(szMessage, sizeof(szMessage));
			}
			// ��� �Ⱓ
			if(pPackaDetailIteminfo->dwUseTime)
			{
				int day = 0;
				int hour = 0;
				int min = 0;
				GlobalFunc::CalcTime(pPackaDetailIteminfo->dwUseTime, day, hour, min, szMessage);
				GlobalFunc::SetCaption(m_pTxtCtrlItemUseTime[index][i], szMessage);
				ZeroMemory(szMessage, sizeof(szMessage));
			}
			// ��� ����
			if(pPackaDetailIteminfo->Aoumt)
			{
				//155376	%d��
				g_InterfaceManager.GetInterfaceString( 155376 , szTemp ,INTERFACE_STRING_LENGTH);
				Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, szTemp , pPackaDetailIteminfo->Aoumt);
				GlobalFunc::SetCaption(m_pTxtCtrlItemAmout[index][i], szMessage);
				ZeroMemory(szMessage, sizeof(szMessage));
			}
		}
	}


	int iScrollRangeSize = ( pPackageInfo->byDetailItemCount < eCASH_ITEM_SHOP_PACKAGE_DLG_DETAILITEM_MAX ) ?
		0 : pPackageInfo->byDetailItemCount - eCASH_ITEM_SHOP_PACKAGE_DLG_DETAILITEM_MAX;
	if(m_pScroll[index])
	{
		m_pScroll[index]->SetScrollRangeWZ(0, iScrollRangeSize);	//��ũ�� ����
		m_pScroll[index]->SetScrollPosWZ(iStartIndex);					// ���� ������
	}

}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopPackageDlg::_setItem(int index, DWORD dwImageCode, TCHAR* szPackageName, int iStartIndex)
{
	TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};

	stPackageItemInfo* pPackageInfo = NULL;
	pPackageInfo = &GetManager()->m_stPackageItemInfo[index];

	if(!pPackageInfo)
		return;

	// ��Ű�� ������
	_addIconImage(index, dwImageCode);

	// ��Ű�� �̸�
	if(pPackageInfo->saleRatio)
	{
		Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, _T("%s  (sale!! %d%)"), szPackageName, pPackageInfo->saleRatio);
	}
	else
	{
		Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, _T("%s"), szPackageName);
	}
	GlobalFunc::SetCaption(m_pTxtCtrlPackageName[index], szMessage);
	ZeroMemory(szMessage, sizeof(szMessage));
	// ��Ű�� ����
	if (pPackageInfo->dwPrice)
	{
		if (LOGINPARAM->IsLiveTestServer() == TRUE)	//�׼�������
		{
			GlobalFunc::SetMoneyText(m_pTxtCtrlPackageInfo[index], pPackageInfo->dwPrice, CASH_ITEM_SHOP_HAIM);
		}
		else
		{
			GlobalFunc::SetMoneyText(m_pTxtCtrlPackageInfo[index], pPackageInfo->dwPrice, CASH_ITEM_SHOP_DEFAULT);
		}
	}
	// ������  �̸� & ���Ⱓ & ��밹��)
	_setDetialItem(index, iStartIndex);
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopPackageDlg::_addIconImage(int iIndex, DWORD dwCode)
{
	m_PackageItemImg[iIndex].dwIconCode = dwCode;
	_removeIconImage(iIndex);

	if (dwCode != 0)
	{
		m_PackageItemImg[iIndex].pImage  = ICONMGR()->Add(dwCode);
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopPackageDlg::_removeIconImage(int iIndex)
{
	if(m_PackageItemImg[iIndex].pImage)
	{
		ICONMGR()->RemoveIcon(m_PackageItemImg[iIndex].pImage);
		ZeroMemory(&(m_PackageItemImg[iIndex]), sizeof(m_PackageItemImg[iIndex]));
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopPackageDlg::_removeAllIconImage()
{
	for(int i=0; i<eCASH_ITEM_SHOP_PACKAGE_DLG_ITEM_MAX;  ++i)
	{
		_removeIconImage(i);
	}
}


//------------------------------------------------------------------------------ 
//! static function
void uiCashItemShopPackageDlg::Callback_ShopIconRender(CControlWZ* control_ptr, void* parameter)
{
    uiCashItemShopPackageDlg* onwer = static_cast<uiCashItemShopPackageDlg*>(parameter);
    if ((control_ptr != NULL) && (onwer != NULL))
    {
        CASH_ITEM_IMG* item_img = (CASH_ITEM_IMG*)(control_ptr->GetUserData());

        if ((item_img->pImage != NULL) && (item_img->dwIconCode > 0))
        {
            RECT rc = control_ptr->GetSizeRect();
            item_img->pImage->Render(&rc);
            onwer->GetManager()->renderCoverImage(*item_img, rc);
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopPackageDlg::_renderIconImage()
{
    for(int i = 0; i < eCASH_ITEM_SHOP_PACKAGE_DLG_ITEM_MAX; ++i)
    {
        if(m_pImgCtrl[i] != NULL)
        {
            m_pImgCtrl[i]->SetUserData((DWORD)&(m_PackageItemImg[i]));
            m_pImgCtrl[i]->SetCallbackDraw(uiCashItemShopPackageDlg::Callback_ShopIconRender, this);
        }
    }
}


//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopPackageDlg::_renderTooltip()
{
#ifdef __CN_0_20070507_SHOP_MILEAGE
	uiToolTipMan* pUIMan = (uiToolTipMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_TOOLTIP);
	if(pUIMan)
	{
		//	155387	�ع決�� ����	//	�ϴ� ���ܻ�Ȳ�� �ϳ��� �ϴ� �ϵ��ڵ� �ϳ� �� �þ�� ���ȭ �ʿ��ؿ�
		if(this->m_ItemCodeForTooltip.dwItemCode[0] == 155387)
		{
			pUIMan->SetTooltipDialogID(GetDialogKey());
			pUIMan->RegisterTooltipWithStringCode(m_ItemCodeForTooltip.dwItemCode[0]);
		}
		else if(this->m_ItemCodeForTooltip.dwItemCode[0] > 0)
		{
			pUIMan->SetTooltipDialogID(GetDialogKey());
			pUIMan->RegisterTooltipItemForPackage(m_ItemCodeForTooltip.szName, &m_ItemCodeForTooltip.dwItemCode[0]);
		}
	}
#else	//	__CN_0_20070507_SHOP_MILEAGE
	if(this->m_ItemCodeForTooltip.dwItemCode[0] > 0)
	{
		uiToolTipMan* pUIMan = (uiToolTipMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_TOOLTIP);
		if(pUIMan)
			pUIMan->RegisterTooltipItemForPackage(m_ItemCodeForTooltip.szName, &m_ItemCodeForTooltip.dwItemCode[0]);
	}
#endif	//	__CN_0_20070507_SHOP_MILEAGE
	int iMouseX = Mouse::Instance()->GetMouseX();
	int iMouseY = Mouse::Instance()->GetMouseY();

	// ��ŵó�� 
	// �������̽����� Dialog ���������� Mouse Over�� ���� �ʴ´�.
	BOOL bMouseOver = FALSE;

#ifdef __CN_0_20070507_SHOP_MILEAGE
	for (POSTYPE a = eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_IMG_ITEM1; a <= eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_BTN_MYMILEGE; a++)
#else	//	__CN_0_20070507_SHOP_MILEAGE
	for (POSTYPE a = eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_IMG_ITEM1; a <= eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_IMG_ITEM3; a++)
#endif	//	__CN_0_20070507_SHOP_MILEAGE
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
void uiCashItemShopPackageDlg::SetItem(int index, DWORD dwImageCode, TCHAR* szPackageName, int iStartIndex)
{
	_setItem(index, dwImageCode, szPackageName, iStartIndex);
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopPackageDlg::SetCurPage()
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
void uiCashItemShopPackageDlg::SetCurMoney( void )
{
    CASH normal_cash_coin = GetManager()->GetMyCash();    // �Ϲ�ĳ��
    CASH pp_card_coin = GetManager()->GetPPCash();        // PPĳ��
    MONEY total_coin = static_cast<MONEY>(normal_cash_coin) + pp_card_coin;       // ĳ������

    GlobalFunc::SetMoneyText(m_pTxtCtrlCCCoin, normal_cash_coin, CASH_ITEM_SHOP_DEFAULT);
    GlobalFunc::SetMoneyText(m_pTxtCtrlPPCoin, pp_card_coin, CASH_ITEM_SHOP_DEFAULT);
	GlobalFunc::SetMoneyText(m_pTxtCtrlMilege, GetManager()->GetMyMileage(), CASH_ITEM_SHOP_MILEAGE);

	if (LOGINPARAM->IsLiveTestServer())	//�׼�������
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
void uiCashItemShopPackageDlg::RefreshPackageItemList()
{
#ifdef _DEBUG
	TCHAR szDebug[INTERFACE_STRING_LENGTH] = {0,};
	Snprintf(szDebug, INTERFACE_STRING_LENGTH-1, _T("RefreshList[%d] %d Page\n"),
		m_iCurMenu, m_iCurPage);
	OutputDebugString(szDebug);
#endif
	_clearAllItem();
	SetCurPage();
	SetCurMoney();
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopPackageDlg::OnShowWindow( BOOL val )
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
		msg.dwType = eDoType_dialog_cash_item_shop_package;
		msg.wParam = InterfaceManager::DIALOG_CASH_ITEM_SHOP_PACKAGE_DLG;
		msg.DoSomething = GlobalFunc::DoShowWindow;
		g_KeyQueueManager.PushBack(msg);

		m_bShow = true;

		_prepareShow();
	}
	else
	{
		g_KeyQueueManager.DeleteMsg(eDoType_dialog_cash_item_shop_package);
		_removeAllIconImage();
        getControl(eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_LIST_SORTCLASS)->ShowWindowWZ(WZ_HIDE);
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopPackageDlg::OnUpdateSolarDialog()
{
	if(!m_bShow)
		this->ShowInterface(FALSE);

	if (!_isEnable())
		return;

	if(m_pBtnCheckTabs[eCASH_ITEM_SHOP_PACKAGE_DLG_CHK_CASHITEM])
		m_pBtnCheckTabs[eCASH_ITEM_SHOP_PACKAGE_DLG_CHK_CASHITEM]->SetCheckState(false);

	if(m_pBtnCheckTabs[eCASH_ITEM_SHOP_PACKAGE_DLG_CHK_MY_PAGE])
		m_pBtnCheckTabs[eCASH_ITEM_SHOP_PACKAGE_DLG_CHK_MY_PAGE]->SetCheckState(false);

	if(m_pBtnCheckTabs[eCASH_ITEM_SHOP_PACKAGE_DLG_CHK_PACKAGE])
		m_pBtnCheckTabs[eCASH_ITEM_SHOP_PACKAGE_DLG_CHK_PACKAGE]->SetCheckState(true);

#ifdef __NA_00387_20070605_SHOP_MILEAGE2
	if(m_pBtnCheckTabs[eCASH_ITEM_SHOP_PACKAGE_DLG_CHK_MILEAGE])
		m_pBtnCheckTabs[eCASH_ITEM_SHOP_PACKAGE_DLG_CHK_MILEAGE]->SetCheckState(false);
#endif	//	__NA_00387_20070605_SHOP_MILEAGE2

	_updateMenu();
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopPackageDlg::OnRenderSolarDialog()
{
	if (!_isEnable())
		return;

	_renderIconImage();
	_renderTooltip();
}


//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopPackageDlg::MessageProc( SI_MESSAGE * pMessage )
{
	if (!_isEnable())
		return;

	int	iCtrlMenu = getCtrlIDToPos( pMessage->dwCtrlID );

	switch( pMessage->eResultMsg ) 
	{
	case RT_MSG_LBUTTONCLICK:
		{
			switch( iCtrlMenu )
			{
			case eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_EXIT:
				{
					m_bShow = false;
				}
				break;

			case eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_CHK_MYPAGE:
				{
					GetManager()->ShowCashItemShopPresentDlg(true);
				}
				break;

			case eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_CHK_CASHITEM:
				{
					GetManager()->ShowCashItemShopDlg(true);
				}
				break;

#ifdef __NA_00387_20070605_SHOP_MILEAGE2
			case eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_CHK_MAILAGE:
				{
					GetManager()->ShowCashItemShopMileageDlg(true);
				}
				break;
#endif	//	__NA_00387_20070605_SHOP_MILEAGE2

			case eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_BTN_PREVPAGE:
				{
					int iPage = m_iCurPage;
					if( iPage > 0)
					{
						iPage--;
						_setPage(iPage);
					}
				}
				break;
			case eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_BTN_NEXTPAGE:
				{
                    _setPage(m_iCurPage + 1);
				}
				break;

			case eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_BTN_BUY1:
			case eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_BTN_BUY2:
			case eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_BTN_BUY3:
				{
					int iIndex = getCtrlIDToPos( pMessage->dwCtrlID ) - eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_BTN_BUY1;
                    if (GetManager()->m_stPackageItemInfo[iIndex].itemguid > 0)
					{
						GetManager()->SelectBuyItem(iIndex);
					}
				}
				break;

            case eDIALOG_CASH_ITEM_SHOP_DLG_BTN_BUYWCOIN:
                {
                    GetManager()->ExecBuyWCoin();
                }
                break;

			case eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_CHK_ALLITEM:
			case eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_CHK_NEWITEM:
			case eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_CHK_BESTITEM:
            case eDIALOG_CASH_ITEM_SHOP_DLG_CHK_EXTENSION:               //�Ⱓ����
				{
					int iMenu = iCtrlMenu - eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_CHK_ALLITEM;
					this->_setMenu(iMenu);
                    if (GetManager())
                    {
                        GetManager()->ShowCashItemPurchaseDlg(false);
                    }
				}
				break;
            case eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_BTN_SORTCLASS:
            case eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_LIST_SORTCLASS:
                OnClickCombo(static_cast<eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG>(iCtrlMenu));
                break;
			}
		}
		break;


	case RT_MSG_SB_PAGETOP:		//. ���� ������ ������.
	case RT_MSG_SB_LINETOP:		//.���� ȭ��ǥ�� ������.
		{
			switch(iCtrlMenu)
			{
			case eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_VSCR1:
			case eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_VSCR2:
			case eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_VSCR3:
				{
					int iMenu = iCtrlMenu - eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_VSCR1;
					int pos = m_pScroll[iMenu]->GetScrollPosWZ();//���� ��ũ�� ���� �޾� �´�.
					int mmin = 0 , mmax = 0;
					m_pScroll[iMenu]->GetScrollRangeWZ( &mmin, &mmax );//��ũ�� ���� �ִ� �ּҸ� ��� �´�.
					if(pos>mmin)// ���� �Էµ� ���μ� ���� ������ 
					{
						pos--;//���� ���� ��Ű�� 
						m_pScroll[iMenu]->SetScrollPosWZ(pos);
						_setDetialItem(iMenu, pos);
					}
				}
				break;
			default:
				break;
			}
		}
		break;

	case RT_MSG_SB_PAGEBOTTOM:	//. ���� ������ ������.
	case RT_MSG_SB_LINEBOTTOM:	//.�Ʒ��� ȭ��ǥ�� ������
		{
			switch(iCtrlMenu)
			{
			case eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_VSCR1:
			case eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_VSCR2:
			case eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_VSCR3:
				{
					int iMenu = iCtrlMenu - eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_VSCR1;
					int pos = m_pScroll[iMenu]->GetScrollPosWZ();//���� ��ũ�� ���� �޾� �´�.
					int mmin = 0 , mmax = 0;
					m_pScroll[iMenu]->GetScrollRangeWZ( &mmin, &mmax );//��ũ�� ���� �ִ� �ּҸ� ��� �´�.

					if(pos<mmax)// ���� �Էµ� ���μ� ���� ������ 
					{
						pos++;//���� ���� ��Ű�� 
						m_pScroll[iMenu]->SetScrollPosWZ(pos);
						_setDetialItem(iMenu, pos);
					}
				}
				break;
			default:
				break;
			}
		}
		break;

	case RT_MSG_SB_THUMBTRACK:				//. ���� �巡�� �ϰ� �ִ�.
	case RT_MSG_SB_THUMBPOSITION:			//. ���� �巡������ ���콺�� ���Ҵ�.
	case RT_MSG_SB_ENDSCROLL:				//. ��ũ���� ���� �Ǿ���.
		{

			switch(iCtrlMenu)
			{
			case eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_VSCR1:
			case eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_VSCR2:
			case eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_VSCR3:
				{

					int iMenu = iCtrlMenu - eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_VSCR1;
					int pos = m_pScroll[iMenu]->GetScrollPosWZ();//���� ��ũ�� ���� �޾� �´�.
			
					m_pScroll[iMenu]->SetScrollPosWZ(pos);
	
					_setDetialItem(iMenu, pos);
				}
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
void uiCashItemShopPackageDlg::MouseOver_MsgProc( SI_MESSAGE * pMessage )
{
	if (!_isEnable())
		return;

	switch( pMessage->eResultMsg ) 
	{
	case RT_MSG_MOUSEOVER:
		{
			POSTYPE pos = getCtrlIDToPos( pMessage->dwCtrlID );
			if (pos >= eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_IMG_ITEM1 && pos <= eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_IMG_ITEM3)
			{
				int iIndex = pos - eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_IMG_ITEM1;
				if (m_PackageItemImg[iIndex].pImage && m_PackageItemImg[iIndex].dwIconCode)
				{
					for(int i = 0; i < GetManager()->m_stPackageItemInfo[iIndex].byDetailItemCount; ++i)
					{
						m_ItemCodeForTooltip.dwItemCode[i] = GetManager()->m_stPackageItemInfo[iIndex].stDetailItem[i].dwItemCode;
					}
					Snprintf(m_ItemCodeForTooltip.szName, INTERFACE_STRING_LENGTH-1, GetManager()->m_stPackageItemInfo[iIndex].szName);
					m_ItemCodeForTooltip.dwCoolTime = GetManager()->m_stPackageItemInfo[iIndex].dwCoolTime;
					m_ItemCodeForTooltip.SellEndDate = GetManager()->m_stPackageItemInfo[iIndex].SellEndDate;
				}
				else
				{
					ZeroMemory(&m_ItemCodeForTooltip, sizeof(m_ItemCodeForTooltip));
				}
			}
    #ifdef __CN_0_20070507_SHOP_MILEAGE
            else if(pos == eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_BTN_MYMILEGE)
			{
				//	155387	�ع決�� ����
				m_ItemCodeForTooltip.dwItemCode[0] = 155387;
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
bool uiCashItemShopPackageDlg::OnSelectedCombo(eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG list_control_index, 
                                        eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG static_control_index, 
                                        int selected_index)
{
    CCtrlListWZ* list_control = static_cast<CCtrlListWZ*>(getControl(list_control_index));
    CControlWZ* staticbox_control = getControl(static_control_index);

    if (selected_index < 0)
    {
        return false;
    }

    if ((list_control != NULL) && (staticbox_control != NULL))
    {
        staticbox_control->SetTextWZ("");

        const char* text = list_control->GetItemText(selected_index, 0);
        if (text != NULL)
        {
            staticbox_control->SetTextWZ((char*)text);
        }

        list_control->ShowWindowWZ(WZ_HIDE);
    }

    return true;
}

//------------------------------------------------------------------------------ 
void uiCashItemShopPackageDlg::OnClickCombo(eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG e)
{
    if (e == eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_BTN_SORTCLASS)
    {
        CControlWZ* list_control = getControl(eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_LIST_SORTCLASS);
        if (list_control != NULL)
        {
            if (list_control->GetState() == WZ_SHOW)
            {
                list_control->ShowWindowWZ(WZ_HIDE);
            }
            else
            {
                list_control->ShowWindowWZ(WZ_SHOW);
            }
        }
    }
    else if (e == eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_LIST_SORTCLASS)
    {
        CCtrlListWZ* list_control = static_cast<CCtrlListWZ*>(getControl(e));

        if (list_control != NULL)
        {
            if (OnSelectedCombo(e, eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TEXT_SORTCLASS, list_control->GetCurSel()) == true)
            {
                _setMenu(m_iCurMenu, true);
            }
        }
    }
}


//------------------------------------------------------------------------------
/**
*/
BOOL uiCashItemShopPackageDlg::_isEnable()
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
void uiCashItemShopPackageDlg::_clearAllItem()
{
	int i;
	for(i=0; i<eCASH_ITEM_SHOP_PACKAGE_DLG_ITEM_MAX; ++i)
	{
		_clearItem(i);
	}
	GlobalFunc::SetCaption(m_pTxtCtrlCurPage, _T(""));
	GlobalFunc::SetCaption(m_pTxtCtrlMoney, _T(""));
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopPackageDlg::_clearItem(int index)
{
	this->_removeIconImage(index);

	GlobalFunc::SetCaption(m_pTxtCtrlPackageName[index], _T(""));
	GlobalFunc::SetCaption(m_pTxtCtrlPackageInfo[index], _T(""));

	for(int i=0; i<eCASH_ITEM_SHOP_PACKAGE_DLG_DETAILITEM_MAX; ++i)
	{
		GlobalFunc::SetCaption(m_pTxtCtrlItemName[index][i], _T(""));
		GlobalFunc::SetCaption(m_pTxtCtrlItemUseTime[index][i], _T(""));
		GlobalFunc::SetCaption(m_pTxtCtrlItemAmout[index][i], _T(""));
	}

}

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------ 
#endif //#ifndef _NA_0_20110613_GAME_SHOP_RENEWAL //#if'N'def
//------------------------------------------------------------------------------ 
