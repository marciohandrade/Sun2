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
	StrToWzID("B999"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_EXIT = 0,			//	X 버튼
	StrToWzID("B001"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_IMG_ITEM1,			//	아이템 이미지 1
	StrToWzID("B002"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_IMG_ITEM2,			//	아이템 이미지 2
	StrToWzID("B003"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_IMG_ITEM3,			//	아이템 이미지 3
    StrToWzID("BT02"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_BTN_MYMILEGE,		//	해방군의 훈장 그림 버튼
	StrToWzID("C001"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_CHK_CASHITEM,		//	캐시아이템 체크버튼
	StrToWzID("C002"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_CHK_MYPAGE,			//	마이페이지 체크버튼
    StrToWzID("C003"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_CHK_PACKAGE,		    //	패키지 체크버튼
    StrToWzID("CT01"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_CHK_MAILAGE,			//	마일리지 체크버튼
	StrToWzID("C010"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_CHK_ALLITEM,			//	모든아이템 체크버튼
	StrToWzID("C011"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_CHK_NEWITEM,			//	신규상품 체크버튼
	StrToWzID("C012"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_CHK_BESTITEM,		//	인기상품 체크버튼

    StrToWzID("C020"),  // eDIALOG_CASH_ITEM_SHOP_DLG_CHK_EXTENSION,		        //	기간연장

	StrToWzID("T002"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE_NAME1,	//	패키지 이름1
	StrToWzID("T003"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE_NAME2,	//	패키지 이름2
	StrToWzID("T004"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE_NAME3,	//	패키지 이름3

	StrToWzID("T073"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE_CASH1,	//	패키지 가격1
	StrToWzID("T074"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE_CASH2,	//	패키지 가격2
	StrToWzID("T075"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE_CASH3,	//	패키지 가격3

	StrToWzID("B016"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_BTN_BUY1,			//	패키지 구입 버튼1
	StrToWzID("B018"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_BTN_BUY2,			//	패키지 구입 버튼2
	StrToWzID("B020"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_BTN_BUY3,			//	패키지 구입 버튼2

	StrToWzID("T010"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE1_ITEMNAME1,	//	패키지1 아이템 이름1
	StrToWzID("T011"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE1_ITEMNAME2,	//	패키지1 아이템 이름2
	StrToWzID("T012"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE1_ITEMNAME3,	//	패키지1 아이템 이름3
	StrToWzID("T013"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE1_ITEMNAME4,	//	패키지1 아이템 이름4
	StrToWzID("T014"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE1_ITEMNAME5,	//	패키지1 아이템 이름5
    StrToWzID("S001"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE1_ITEMNAME6,	//	패키지1 아이템 이름4
    StrToWzID("S002"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE1_ITEMNAME7,	//	패키지1 아이템 이름5
	StrToWzID("T020"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE2_ITEMNAME1,	//	패키지2 아이템 이름1
	StrToWzID("T021"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE2_ITEMNAME2,	//	패키지2 아이템 이름2
	StrToWzID("T022"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE2_ITEMNAME3,	//	패키지2 아이템 이름3
	StrToWzID("T023"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE2_ITEMNAME4,	//	패키지2 아이템 이름4
	StrToWzID("T024"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE2_ITEMNAME5,	//	패키지2 아이템 이름5
    StrToWzID("S007"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE2_ITEMNAME6,	//	패키지2 아이템 이름6
    StrToWzID("S010"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE2_ITEMNAME7,	//	패키지2 아이템 이름7
	StrToWzID("T030"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE3_ITEMNAME1,	//	패키지3 아이템 이름1
	StrToWzID("T031"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE3_ITEMNAME2,	//	패키지3 아이템 이름2
	StrToWzID("T032"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE3_ITEMNAME3,	//	패키지3 아이템 이름3
	StrToWzID("T033"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE3_ITEMNAME4,	//	패키지3 아이템 이름4
	StrToWzID("T034"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE3_ITEMNAME5,	//	패키지3 아이템 이름5
    StrToWzID("S013"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE3_ITEMNAME6,	//	패키지3 아이템 이름6
    StrToWzID("S016"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE3_ITEMNAME7,	//	패키지3 아이템 이름7

	StrToWzID("ST01"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE1_ITEMUSETIME1,	//	패키지1 아이템 사용기간1
	StrToWzID("ST02"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE1_ITEMUSETIME2,	//	패키지1 아이템 사용기간2
	StrToWzID("ST03"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE1_ITEMUSETIME3,	//	패키지1 아이템 사용기간3
	StrToWzID("ST04"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE1_ITEMUSETIME4,	//	패키지1 아이템 사용기간4
	StrToWzID("ST05"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE1_ITEMUSETIME5,	//	패키지1 아이템 사용기간5
    StrToWzID("S006"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE1_ITEMUSETIME6,	//	패키지1 아이템 사용기간6
    StrToWzID("S004"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE1_ITEMUSETIME7,	//	패키지1 아이템 사용기간7
	StrToWzID("ST06"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE2_ITEMUSETIME1,	//	패키지2 아이템 사용기간1
	StrToWzID("ST07"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE2_ITEMUSETIME2,	//	패키지2 아이템 사용기간2
	StrToWzID("ST08"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE2_ITEMUSETIME3,	//	패키지2 아이템 사용기간3
	StrToWzID("ST09"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE2_ITEMUSETIME4,	//	패키지2 아이템 사용기간4
	StrToWzID("ST10"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE2_ITEMUSETIME5,	//	패키지2 아이템 사용기간5
    StrToWzID("S009"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE2_ITEMUSETIME6,	//	패키지2 아이템 사용기간6
    StrToWzID("S012"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE2_ITEMUSETIME7,	//	패키지2 아이템 사용기간7

	StrToWzID("ST11"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE3_ITEMUSETIME1,	//	패키지3 아이템 사용기간1
	StrToWzID("ST12"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE3_ITEMUSETIME2,	//	패키지3 아이템 사용기간2
	StrToWzID("ST13"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE3_ITEMUSETIME3,	//	패키지3 아이템 사용기간3
	StrToWzID("ST14"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE3_ITEMUSETIME4,	//	패키지3 아이템 사용기간4
	StrToWzID("ST15"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE3_ITEMUSETIME5,	//	패키지3 아이템 사용기간5
    StrToWzID("S015"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE3_ITEMUSETIME6,	//	패키지3 아이템 사용기간6
    StrToWzID("S018"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE3_ITEMUSETIME7,	//	패키지3 아이템 사용기간7
	StrToWzID("T015"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE1_ITEMAMOUT1,	//	패키지1 아이템 갯수1
	StrToWzID("T016"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE1_ITEMAMOUT2,	//	패키지1 아이템 갯수2
	StrToWzID("T017"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE1_ITEMAMOUT3,	//	패키지1 아이템 갯수3
	StrToWzID("T018"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE1_ITEMAMOUT4,	//	패키지1 아이템 갯수4
	StrToWzID("T019"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE1_ITEMAMOUT5,	//	패키지1 아이템 갯수5
    StrToWzID("S005"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE1_ITEMAMOUT6,	//	패키지1 아이템 갯수6
    StrToWzID("S003"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE1_ITEMAMOUT7,	//	패키지1 아이템 갯수7

	StrToWzID("T025"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE2_ITEMAMOUT1,	//	패키지2 아이템 갯수1
	StrToWzID("T026"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE2_ITEMAMOUT2,	//	패키지2 아이템 갯수2
	StrToWzID("T027"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE2_ITEMAMOUT3,	//	패키지2 아이템 갯수3
	StrToWzID("T028"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE2_ITEMAMOUT4,	//	패키지2 아이템 갯수4
	StrToWzID("T029"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE2_ITEMAMOUT5,	//	패키지2 아이템 갯수5
    StrToWzID("S008"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE2_ITEMAMOUT6,	//	패키지2 아이템 갯수6
    StrToWzID("S011"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE2_ITEMAMOUT7,	//	패키지2 아이템 갯수7
	StrToWzID("T035"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE3_ITEMAMOUT1,	//	패키지3 아이템 갯수1
	StrToWzID("T036"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE3_ITEMAMOUT2,	//	패키지3 아이템 갯수2
	StrToWzID("T037"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE3_ITEMAMOUT3,	//	패키지3 아이템 갯수3
	StrToWzID("T038"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE3_ITEMAMOUT4,	//	패키지3 아이템 갯수4
	StrToWzID("T039"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE3_ITEMAMOUT5,	//	패키지3 아이템 갯수5
    StrToWzID("S014"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE3_ITEMAMOUT6,	//	패키지3 아이템 갯수6
    StrToWzID("S017"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_PACKAGE3_ITEMAMOUT7,	//	패키지3 아이템 갯수7

	StrToWzID("VS00"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_VSCR1,					//	패키지 스크롤1
	StrToWzID("VS01"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_VSCR2,					//	패키지 스크롤2
	StrToWzID("VS02"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_VSCR3,					//	패키지 스크롤3

	StrToWzID("BT00"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_BTN_PREVPAGE,			//	이전 페이지 버튼
	StrToWzID("BT01"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_BTN_NEXTPAGE,			//	다음 페이지 버튼

	StrToWzID("ST00"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_CURPAGE,				//	현재 페이지 텍스트

	StrToWzID("T057"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_TXT_MYMONEY,				//	현재 보유금액
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
			pSystemMan->Sys_VerifyLock(_T("m_pCashItemShopMan이 없습니다."));
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

    #ifdef _NA00XXXX_090111_CASH_TYPE_SUBDIVIDE_//PP카드 관련 기능
        //적용되면 숨기지 않는다
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

    //2544	전체 클레스	캐시상점 필터
    //2545	버서커	캐시상점 필터
    //2546	드래곤	캐시상점 필터
    //2547	발키리 	캐시상점 필터
    //2548	엘리멘탈	캐시상점 필터
    //2549	새도우	캐시상점 필터
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

    //! 기간 연장인지
    if (byCategory == SHOP_CATEGORY_PERIOD_EXTEND)
    {
        GetManager()->OnClickExtensionButton(m_iCurPage);
    }
    else    // if (byCategory == SHOP_CATEGORY_ALL) //패키지 메뉴인지 구분한다.
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
			// 이름
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
			// 사용 기간
			if(pPackaDetailIteminfo->dwUseTime)
			{
				int day = 0;
				int hour = 0;
				int min = 0;
				GlobalFunc::CalcTime(pPackaDetailIteminfo->dwUseTime, day, hour, min, szMessage);
				GlobalFunc::SetCaption(m_pTxtCtrlItemUseTime[index][i], szMessage);
				ZeroMemory(szMessage, sizeof(szMessage));
			}
			// 사용 갯수
			if(pPackaDetailIteminfo->Aoumt)
			{
				//155376	%d개
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
		m_pScroll[index]->SetScrollRangeWZ(0, iScrollRangeSize);	//스크롤 범위
		m_pScroll[index]->SetScrollPosWZ(iStartIndex);					// 현재 페이지
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

	// 패키지 아이콘
	_addIconImage(index, dwImageCode);

	// 패키지 이름
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
	// 패키지 가격
	if (pPackageInfo->dwPrice)
	{
		if (LOGINPARAM->IsLiveTestServer() == TRUE)	//테섭실행모드
		{
			GlobalFunc::SetMoneyText(m_pTxtCtrlPackageInfo[index], pPackageInfo->dwPrice, CASH_ITEM_SHOP_HAIM);
		}
		else
		{
			GlobalFunc::SetMoneyText(m_pTxtCtrlPackageInfo[index], pPackageInfo->dwPrice, CASH_ITEM_SHOP_DEFAULT);
		}
	}
	// 아이템  이름 & 사용기간 & 사용갯수)
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
		//	155387	해방군의 훈장	//	일단 예외상황이 하나라서 일단 하드코딩 하나 더 늘어나면 모듈화 필요해연
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

	// 스킵처리 
	// 인터페이스에서 Dialog 바탕에서는 Mouse Over가 오지 않는다.
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
	//5796	%d페이지
	g_InterfaceManager.GetInterfaceString( 5796 , szTemp ,INTERFACE_STRING_LENGTH);
	Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, szTemp, m_iCurPage + 1);
	GlobalFunc::SetCaption(m_pTxtCtrlCurPage, szMessage);	// 현재 페이지
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopPackageDlg::SetCurMoney( void )
{
    CASH normal_cash_coin = GetManager()->GetMyCash();    // 일반캐시
    CASH pp_card_coin = GetManager()->GetPPCash();        // PP캐시
    MONEY total_coin = static_cast<MONEY>(normal_cash_coin) + pp_card_coin;       // 캐시총합

    GlobalFunc::SetMoneyText(m_pTxtCtrlCCCoin, normal_cash_coin, CASH_ITEM_SHOP_DEFAULT);
    GlobalFunc::SetMoneyText(m_pTxtCtrlPPCoin, pp_card_coin, CASH_ITEM_SHOP_DEFAULT);
	GlobalFunc::SetMoneyText(m_pTxtCtrlMilege, GetManager()->GetMyMileage(), CASH_ITEM_SHOP_MILEAGE);

	if (LOGINPARAM->IsLiveTestServer())	//테섭실행모드
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
            case eDIALOG_CASH_ITEM_SHOP_DLG_CHK_EXTENSION:               //기간연장
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


	case RT_MSG_SB_PAGETOP:		//. 윗쪽 몸통을 눌렀다.
	case RT_MSG_SB_LINETOP:		//.윗쪽 화살표를 눌렀다.
		{
			switch(iCtrlMenu)
			{
			case eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_VSCR1:
			case eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_VSCR2:
			case eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_VSCR3:
				{
					int iMenu = iCtrlMenu - eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_VSCR1;
					int pos = m_pScroll[iMenu]->GetScrollPosWZ();//현재 스크롤 값을 받아 온다.
					int mmin = 0 , mmax = 0;
					m_pScroll[iMenu]->GetScrollRangeWZ( &mmin, &mmax );//스크롤 값의 최대 최소를 얻어 온다.
					if(pos>mmin)// 현재 입력된 라인수 보다 작으면 
					{
						pos--;//한줄 감소 시키고 
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

	case RT_MSG_SB_PAGEBOTTOM:	//. 윗쪽 몸통을 눌렀다.
	case RT_MSG_SB_LINEBOTTOM:	//.아랫쪽 화살표를 눌렀다
		{
			switch(iCtrlMenu)
			{
			case eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_VSCR1:
			case eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_VSCR2:
			case eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_VSCR3:
				{
					int iMenu = iCtrlMenu - eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_VSCR1;
					int pos = m_pScroll[iMenu]->GetScrollPosWZ();//현재 스크롤 값을 받아 온다.
					int mmin = 0 , mmax = 0;
					m_pScroll[iMenu]->GetScrollRangeWZ( &mmin, &mmax );//스크롤 값의 최대 최소를 얻어 온다.

					if(pos<mmax)// 현재 입력된 라인수 보다 작으면 
					{
						pos++;//한줄 증가 시키고 
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

	case RT_MSG_SB_THUMBTRACK:				//. 썸을 드래그 하고 있다.
	case RT_MSG_SB_THUMBPOSITION:			//. 썸을 드래그한후 마우스를 놓았다.
	case RT_MSG_SB_ENDSCROLL:				//. 스크롤이 종료 되었다.
		{

			switch(iCtrlMenu)
			{
			case eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_VSCR1:
			case eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_VSCR2:
			case eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_VSCR3:
				{

					int iMenu = iCtrlMenu - eDIALOG_CASH_ITEM_SHOP_PACKAGE_DLG_VSCR1;
					int pos = m_pScroll[iMenu]->GetScrollPosWZ();//현재 스크롤 값을 받아 온다.
			
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
				//	155387	해방군의 훈장
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
