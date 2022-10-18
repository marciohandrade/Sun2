#include "SunClientPrecompiledHeader.h"	 

#include "uiItemCompositeMan.h"
#include "uiItemTotalComposite/uiItemTotalComposite.h"
//#include "uiItemComposite/uiItemComposite.h"

#include "uiItemCube.h"

#include "interfacemanager.h"
#include "globalfunc.h"

const DWORD IM_ITEM_COMPOSITE_MANAGER::ITEM_TOTAL_COMPOSITE = StrToWzID("0718");
const DWORD IM_ITEM_COMPOSITE_MANAGER::ITEM_CUBE		    = StrToWzID("0769");
//-------------------------------------------------------------------------------------------
/**
*/

BOOL uiItemCompositeMan::_LoadUI()
{
	m_pUIItemTotalComposite = CreateUIDialog<uiItemTotalcomposite>( IM_ITEM_COMPOSITE_MANAGER::ITEM_TOTAL_COMPOSITE,
							  "Data\\interface\\71_8_Item_create.iwz", this );
							  	
    ui_item_cube_ = CreateUIDialog<uiItemCube>(IM_ITEM_COMPOSITE_MANAGER::ITEM_CUBE, "Data\\Interface\\76_9_Cube.iwz", this);	

	return TRUE;
}