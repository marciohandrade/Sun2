#include "SunClientPrecompiledHeader.h"
#include "uiItemCompositeMan.h"
#include "ItemComposeListParser.h"
#include "ItemCrystalizeListParser.h"
//#include "uiItemComposite/uiItemComposite.h"
#include "uiItemTotalComposite/uiItemTotalComposite.h"


//-------------------------------------------------------------------------------------------
/**
*/
uiItemCompositeMan::uiItemCompositeMan(InterfaceManager *pUIMan) : 
	uiBaseMan(pUIMan), 
	m_eCurItemCompositeCatagory(ITEM_COMPOSITE_CATEGORY_NONE),
	m_eCurItemCompositeCode(eITEMCOMPOSITE_NONE),
	m_ePendingItemCompositeCode(eITEMCOMPOSITE_NONE), 
	m_ManualCompositeNum(0),
	m_maxCompositeNum(0),
	m_iCompositeResultType(0),
	m_PendingManualCompositeNum(0)
{

}

//-------------------------------------------------------------------------------------------
/**
*/
void
uiItemCompositeMan::OnInitialize()
{
    _LoadUI();

    m_eCurItemCompositeCatagory = ITEM_COMPOSITE_CATEGORY_NONE;
    m_eCurItemCompositeCode = eITEMCOMPOSITE_NONE;
    m_ePendingItemCompositeCode = eITEMCOMPOSITE_NONE;
}

//-------------------------------------------------------------------------------------------
/**
*/
SolarDialog * 
uiItemCompositeMan::ShowItemComposite( 
						eItemCompositeCatagory eCompositeCategoryType, 
						eITEMCOMPOSITECODE ItemCompositeCode)
{
	this->m_eCurItemCompositeCatagory = eCompositeCategoryType;
	this->m_eCurItemCompositeCode = ItemCompositeCode;

	switch (eCompositeCategoryType) 
	{

	case ITEM_COMPOSITE_CATEGORY_TOTAL_COMPOSITE:
		{
			if( isValidTotalCompositeFlag())
			{
				this->m_pUIItemTotalComposite->ShowInterfaceWithSound(TRUE);
				if(m_pUIItemTotalComposite->GetCurrentSelectItemCode())
				{
					fillTotalCompositeInfo(m_pUIItemTotalComposite->GetCurrentSelectItemCode());
					m_pUIItemTotalComposite->updateInfo();
				}
			}
		}
		break;

	//case ITEM_COMPOSITE_CATEGORY_COMPOSITE:
	//	{
	//		if (isValidCompositeFlag())
	//		{
	//			this->m_pUIItemComposite->ShowInterfaceWithSound(TRUE);
	//			fillCompositeInfo();
	//		}

	//		return (SolarDialog *)this->m_pUIItemComposite;
	//	}
	//	break;

	}


	return (SolarDialog *)NULL;
}