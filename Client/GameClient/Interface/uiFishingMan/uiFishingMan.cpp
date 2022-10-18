 #include "SunClientPrecompiledHeader.h"

#ifdef _YJ_FISHING_SYSTEM_000933

#include "uiFishingMan/uiFishingMan.h"
#include "InterfaceManager.h"
#include "uiFishingShop/uiFishingShop.h"
#include "uiFishingTrade/uiFishingTrade.h"
#include "uiFishingAction/uiFishingAction.h"

const char  c_strDlgFname_Fishing_Shop_Dlg[]      = ("Data\\Interface\\75_1_FishingWindow.iwz");
const DWORD c_dwCompareID_Fishing_Shop_Dlg        = StrToWzID("0751");
const char  c_strDlgFname_Fishing_Trade_Dlg[]     = ("Data\\Interface\\75_2_FishingWindow.iwz");
const DWORD c_dwCompareID_Fishing_Trade_Dlg		  = StrToWzID("0752");
const char  c_strDlgFname_Fishing_Action_Dlg[]	  = ("Data\\Interface\\75_3_FishingWindow.iwz");
const DWORD c_dwCompareID_Fishing_Action_Dlg	  = StrToWzID("0753");

//////////////////////////////////////////////////////////////////////////

uiFishingMan::uiFishingMan( InterfaceManager * pUIMAN) : uiBaseMan(pUIMAN)
{
}

//////////////////////////////////////////////////////////////////////////

void uiFishingMan::OnInitialize()
{
	m_pFishingShop =  CreateUIDialog<uiFishingShop>(
		c_dwCompareID_Fishing_Shop_Dlg, 
		c_strDlgFname_Fishing_Shop_Dlg,
		this);

	m_pFishingTrade = CreateUIDialog<uiFishingTrade>(
		c_dwCompareID_Fishing_Trade_Dlg,
		c_strDlgFname_Fishing_Trade_Dlg,
		this);

	m_pFishingAction = CreateUIDialog<uiFishingAction>(
		c_dwCompareID_Fishing_Action_Dlg,
		c_strDlgFname_Fishing_Action_Dlg,
		this);
}

//////////////////////////////////////////////////////////////////////////

#endif //_YJ_FISHING_SYSTEM_000933