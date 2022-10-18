#include "SunClientPrecompiledHeader.h"	 

#include "uiItemCompositeMan.h"

//#include "uiItemComposite/uiItemComposite.h"

#include "ItemComposeListParser.h"
#include "ItemManager.h"
#include "Hero.h"
#include "interfacemanager.h"
#include "globalfunc.h"
#include "ItemTypeList.h"

//-------------------------------------------------------------------------------------------
/**
*/
//BOOL					
//uiItemCompositeMan::IsReadyForCompositeMaterials()
//{
//	if ( !isValidCompositeFlag() )
//		return FALSE;
//
//	sITEM_COMPOSE_LIST * pInfo = ItemComposeListParser::Instance()->GetComposeListInfo( GetCompositeCode() );
//	assert(pInfo);
//
//	if ( !pInfo )
//		return FALSE;
//
//	BOOL bExist = FALSE;
//	for ( int i = 0; i < pInfo->m_byMatCount; ++i )
//	{
//		bExist = 
//			ItemManager::Instance()->IsExistItem(SI_INVENTORY, pInfo->m_pMaterial[i].m_ItemCode, pInfo->m_pMaterial[i].m_byItemNum);
//
//		if (!bExist)
//		{
//			return FALSE;
//		}
//
//	}
//
//	if ( g_pHero && g_pHero->GetMoney() < pInfo->m_Money )
//	{
//		return FALSE;
//	}
//
//	return TRUE;
//}

//-------------------------------------------------------------------------------------------
/**
*/
//BOOL					
//uiItemCompositeMan::isValidCompositeFlag()
//{
//	return BOOL( this->m_eCurItemCompositeCatagory == ITEM_COMPOSITE_CATEGORY_COMPOSITE && 
//				 this->m_eCurItemCompositeCode != eITEMCOMPOSITE_NONE);
//}

//-------------------------------------------------------------------------------------------
/**
*/
//BOOL					
//uiItemCompositeMan::fillCompositeInfo()
//{
//	sITEM_COMPOSE_LIST * pInfo = ItemComposeListParser::Instance()->GetComposeListInfo( GetCompositeCode() );
//	assert(pInfo);
//
//	if ( !pInfo )
//		return FALSE;
//
//	//m_pUIItemComposite->ClearInfo();
//	
//	ItemType * pItemType= NULL;
//
//	for ( int i = 0; i < pInfo->m_byMatCount; ++i )
//	{
//		pItemType = NULL;
//		pItemType = ItemTypeList::Instance()->FindItemType( pInfo->m_pMaterial[i].m_ItemTypeIndex);
//
//		BOOL bExist = ItemManager::Instance()->IsExistItem(SI_INVENTORY, pInfo->m_pMaterial[i].m_ItemCode, pInfo->m_pMaterial[i].m_byItemNum,pItemType);
//		
//		//this->m_pUIItemComposite->AppendMat(i, pInfo->m_pMaterial[i], bExist );
//	}
//
//	//this->m_pUIItemComposite->SetNeedHaim( pInfo->m_Money );
//
//
//
//	return TRUE;
//}

//-------------------------------------------------------------------------------------------
/**
*/
//BOOL					
//uiItemCompositeMan::spendComposeMaterials()
//{
//	TCHAR m_szItemName[INTERFACE_STRING_LENGTH];
//	TCHAR m_szText[INTERFACE_STRING_LENGTH];
//	TCHAR m_szResult[INTERFACE_STRING_LENGTH];
//
//	sITEM_COMPOSE_LIST * pInfo = 
//		ItemComposeListParser::Instance()->GetComposeListInfo( m_ePendingItemCompositeCode );
//	assert(pInfo);
//	if ( !pInfo )
//	{
//		return FALSE;
//	}
//        
//	for ( int i = 0; i < pInfo->m_byMatCount; ++i )
//	{
//		CODETYPE ItemCode = pInfo->m_pMaterial[i].m_ItemCode;
//		int		 iItemNum = pInfo->m_pMaterial[i].m_byItemNum;
//
//		assert(ItemCode);
//		if (ItemCode == 0)
//			continue;
//		
//		BOOL bExist	= ItemManager::Instance()->IsExistItem(SI_INVENTORY, ItemCode, iItemNum);
//		assert(bExist);
//		BOOL bRet = FALSE;
//		bRet = ItemManager::Instance()->SpendItem(SI_INVENTORY, ItemCode, iItemNum);
//		assert(bRet);
//        
//		// %s 아이템을 사용 했습니다.
//		GetUIMan()->GetInterfaceString(5709, m_szText, INTERFACE_STRING_LENGTH);
//		GetUIMan()->GetItemName(pInfo->m_pMaterial[i].m_ItemCode, m_szItemName, INTERFACE_STRING_LENGTH);
//		Sprintf(m_szResult, m_szText, m_szItemName);
//		GlobalFunc::OutputMsg_Notice( eOUTPUT_MSG_CHAT, m_szResult);
//	}
//
//	return TRUE;
//}