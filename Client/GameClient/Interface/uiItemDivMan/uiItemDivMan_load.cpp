#include "SunClientPrecompiledHeader.h"
#include "interfacemanager.h"

#include "uiItemDivMan.h"
#include "uiItemDiv/uiItemDiv.h"
#include "uiItemShopDiv/uiItemShopDiv.h"

const char  c_strDlgFname_ItemDiv[]             = ("Data\\Interface\\72_1_Trade_input.iwz");
const DWORD IM_ITEMDIV_MANAGER::ITEMDIV = StrToWzID("0721");

const char  c_strDlgFname_ItemShopDiv[]             = ("Data\\Interface\\72_2_Package_purchase.iwz");
const DWORD IM_ITEMDIV_MANAGER::ITEMSHOPDIV = StrToWzID("0722");

BOOL uiItemDivMan::_LoadUI()
{
	m_pUIItemDiv = CreateUIDialog<uiItemDiv>(
					IM_ITEMDIV_MANAGER::ITEMDIV, 
					"Data\\Interface\\72_1_Trade_input.iwz",
					this);
	assert(m_pUIItemDiv);

	m_pUIItemShopDiv = CreateUIDialog<uiItemShopDiv>(
					IM_ITEMDIV_MANAGER::ITEMSHOPDIV, 
					"Data\\Interface\\72_2_Package_purchase.iwz",
					this);
	assert(m_pUIItemShopDiv);
	return TRUE;
}
