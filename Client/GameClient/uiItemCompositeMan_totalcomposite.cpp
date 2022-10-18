#include "SunClientPrecompiledHeader.h"	 

#include "uiitemcompositeman/uiItemCompositeMan.h"
#include "uiItemTotalComposite/uiItemTotalComposite.h"
#include "ItemComposeListParser.h"
#include "ItemManager.h"
#include "Hero.h"
#include "interfacemanager.h"
#include "globalfunc.h"
#include "ItemInfoParser.h"
#include "uiSystemMan/uiSystemMan.h"
#include "ItemTypeList.h"

//-------------------------------------------------------------------------------------------
/**
*/
BOOL					
uiItemCompositeMan::IsReadyForTotalCompositeMaterials()
{
	TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
	if(m_pUIItemTotalComposite->GetCurrentSelectItemCode() ==0)
	{
		// 잘못된 정보입니다.
		g_InterfaceManager.GetInterfaceString( 5346, szMessage, INTERFACE_STRING_LENGTH );	
		uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
		if(pSystemMan)
		{
			pSystemMan->Sys_Verify(szMessage);
		}

		return FALSE;
	}

	sITEM_COMPOSE_LIST * pInfo = ItemComposeListParser::Instance()->GetComposeListInfo( 
								m_pUIItemTotalComposite->GetCurrentSelectItemCode() );
	assert(pInfo);

	if ( !pInfo )
		return FALSE;

	BOOL bExist = FALSE;
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    BOOL bEnoughMoney = FALSE;
    MONEY total_money = 0;
    int ratio = g_pHero->GetPlayerAttribute()->GetAttrValue(eATTR_CRAFT_COST_RATIO);
    if (ratio != 0 && pInfo->m_Money > 0)
    {
        MONEY craft_cost_decrease = 0;
        bool is_plus = false;
        if (ratio > 0)
        {
            is_plus = true;
        }
        else
        {
            ratio = abs(ratio);
        }
        craft_cost_decrease = (pInfo->m_Money / 100 * ratio) * m_ManualCompositeNum;
        if (is_plus)
        {
            total_money = (pInfo->m_Money * m_ManualCompositeNum) + craft_cost_decrease;
        }
        else
        {
            total_money = (pInfo->m_Money * m_ManualCompositeNum) - craft_cost_decrease;
        }
    }
    else
    {
        total_money = pInfo->m_Money * m_ManualCompositeNum;
    }
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
	if(m_ManualCompositeNum == 0)
	{
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
        bEnoughMoney = g_pHero->GetMoney() >= total_money;
#else
		BOOL bEnoughMoney = g_pHero && g_pHero->GetMoney() >= pInfo->m_Money * m_ManualCompositeNum;
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
		if ( ! bEnoughMoney )
		{
			//하임이 부족합니다.
			g_InterfaceManager.GetInterfaceString( eST_NOT_ENOUGH_MONEY, szMessage, INTERFACE_STRING_LENGTH );
			uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
			if(pSystemMan)
			{
				pSystemMan->Sys_Verify(szMessage);
			}
			return FALSE;
		}
		else
		{
			//재료가 부족합니다.
			GetUIMan()->GetInterfaceString(5702, szMessage, INTERFACE_STRING_LENGTH);
			uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
			if(pSystemMan)
			{
				pSystemMan->Sys_Verify(szMessage);
			}
			return FALSE;
		}
	}
	else
	{
		for ( int i = 0; i < pInfo->m_byMatCount; ++i )
		{
			int maxAvalibelNum =GetAvalibleCompositeNum(pInfo->m_pMaterial[i].m_ItemCode, pInfo->m_pMaterial[i].m_byItemNum,i);
			if(maxAvalibelNum < m_ManualCompositeNum)
			{
				return FALSE;
			}
		}
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
        bEnoughMoney = g_pHero->GetMoney() >= total_money;
#else
		BOOL bEnoughMoney = g_pHero && g_pHero->GetMoney() >= pInfo->m_Money * m_ManualCompositeNum;
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
		if ( ! bEnoughMoney )
		{
			//하임이 부족합니다.
			g_InterfaceManager.GetInterfaceString( eST_NOT_ENOUGH_MONEY, szMessage, INTERFACE_STRING_LENGTH );
			uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
			if(pSystemMan)
				pSystemMan->Sys_Verify(szMessage);
			return FALSE;
		}
	}
	return TRUE;
}

BOOL uiItemCompositeMan::fillTotalCompositeInfo(int itemCode)
{
	sITEM_COMPOSE_LIST * pInfo = ItemComposeListParser::Instance()->GetComposeListInfo( itemCode );
	assert(pInfo);
	if ( !pInfo )
	{
		return FALSE;
	}
	BOOL isExistAllItem = TRUE;
	ItemType * pItemType = NULL;
	for ( int i = 0; i < pInfo->m_byMatCount; ++i )
	{

		pItemType = NULL;

		pItemType = ItemTypeList::Instance()->FindItemType( pInfo->m_pMaterial[i].m_ItemTypeIndex);

#ifdef _NA_007435_20140718_MODIFY_ITEMCOMPOSE_MATERIAL_CONDITION
        BOOL bExist = IsExistItemForComposite(
            pInfo->m_pMaterial[i].m_ItemCode, 
            pInfo->m_pMaterial[i].m_byItemNum,
            pItemType, 
            pInfo->m_pMaterial[i].m_ItemTypeIndex);
#else
		BOOL bExist = ItemManager::Instance()->IsExistItem(
			SI_INVENTORY, 
			pInfo->m_pMaterial[i].m_ItemCode, 
			pInfo->m_pMaterial[i].m_byItemNum,
			pItemType, 
            pInfo->m_pMaterial[i].m_ItemTypeIndex);
#endif //_NA_007435_20140718_MODIFY_ITEMCOMPOSE_MATERIAL_CONDITION


		if(!bExist) isExistAllItem = FALSE;

		m_pUIItemTotalComposite->AppendMat(i, pInfo->m_pMaterial[i], bExist);
	}
	// 결과물 출력
	m_pUIItemTotalComposite->AppendResultMat(6,isExistAllItem);
	UpdateInfo();
	m_ManualCompositeNum = m_maxCompositeNum;
	return TRUE;
}

int	uiItemCompositeMan::GetAvalibleCompositeNum(CODETYPE ItemCode, int iItemNum, int iFirst)
{
	if(iFirst == 0)
	{
		m_maxCompositeNum = 255;
	}
	SCSlotContainer * pContainer = ItemManager::Instance()->GetContainer( SI_INVENTORY );
	if ( !pContainer )
	{
		return FALSE;
	}

	if ( iItemNum <= 0 )
	{
		return FALSE;
	}

	BASE_ITEMINFO * pItemInfo = ItemInfoParser::Instance()->GetItemInfo( ItemCode );
	if ( !pItemInfo ) 
	{
		return FALSE;
	}

	int totalNum = 0 ;

    for (int i = 0; i < pContainer->GetMaxSlotNum(); ++i)
    {
        if (pContainer->IsEmpty(i))
            continue;
        
        SCItemSlot& item_slot =  static_cast<SCItemSlot&>(pContainer->GetSlot(i));
        if (item_slot.GetCode() == ItemCode)
        {
            if ((item_slot.IsEtheriaItem() == false) && 
                (item_slot.IsShellItem() == false))
            {
                totalNum += item_slot.GetNum();
            }
        }
    }
    
	int numCase = totalNum / iItemNum;
	if(numCase <= m_maxCompositeNum)
	{
		m_maxCompositeNum = numCase;
	}

	return numCase;
}

BOOL uiItemCompositeMan::isValidTotalCompositeFlag()
{
	return BOOL( this->m_eCurItemCompositeCatagory == ITEM_COMPOSITE_CATEGORY_TOTAL_COMPOSITE);

}

BOOL uiItemCompositeMan::spendTotalComposeMaterials()
{
	TCHAR m_szItemName[INTERFACE_STRING_LENGTH];
	TCHAR m_szText[INTERFACE_STRING_LENGTH];
	TCHAR m_szResult[INTERFACE_STRING_LENGTH];

	sITEM_COMPOSE_LIST * pInfo = 
		ItemComposeListParser::Instance()->GetComposeListInfo( m_ePendingItemCompositeCode );
	assert(pInfo);
	if ( !pInfo )
	{
		return FALSE;
	}
	ItemType * pItemType = NULL;
	for ( int i = 0; i < pInfo->m_byMatCount; ++i )
	{
		CODETYPE ItemCode = pInfo->m_pMaterial[i].m_ItemCode;
		int		 iItemNum = pInfo->m_pMaterial[i].m_byItemNum * m_PendingManualCompositeNum;

		assert(ItemCode);
		if (ItemCode == 0)
        {
			continue;
        }

		pItemType = NULL;

		pItemType = ItemTypeList::Instance()->FindItemType( pInfo->m_pMaterial[i].m_ItemTypeIndex);
		BOOL bExist	= ItemManager::Instance()->IsExistItem(SI_INVENTORY, 
                                                           ItemCode, 
                                                           iItemNum,pItemType, 
                                                           pInfo->m_pMaterial[i].m_ItemTypeIndex);

		
		assert(bExist);
		BOOL bRet = FALSE;
		bRet = ItemManager::Instance()->SpendItem(SI_INVENTORY, 
                                                  ItemCode, 
                                                  iItemNum,pItemType, 
                                                  pInfo->m_pMaterial[i].m_ItemTypeIndex);
		assert(bRet);

		// %s 아이템을 사용 했습니다.
		GetUIMan()->GetInterfaceString(5709, m_szText, INTERFACE_STRING_LENGTH);
		GetUIMan()->GetItemName(pInfo->m_pMaterial[i].m_ItemCode, m_szItemName, INTERFACE_STRING_LENGTH);
		Sprintf(m_szResult, m_szText, m_szItemName);
		GlobalFunc::OutputMsg_Notice( eOUTPUT_MSG_CHAT, m_szResult);
	}

	return TRUE;
}


void uiItemCompositeMan::UpdateInfo(BOOL bInitCompositeNum)
{
	sITEM_COMPOSE_LIST * pInfo = ItemComposeListParser::Instance()->GetComposeListInfo( 
		m_pUIItemTotalComposite->GetCurrentSelectItemCode() );	

	if ( !pInfo )
		return;

	m_iCompositeResultType = pInfo->m_byResultType;
	for ( int i = 0; i < pInfo->m_byMatCount; ++i )
	{
		GetAvalibleCompositeNum(pInfo->m_pMaterial[i].m_ItemCode, pInfo->m_pMaterial[i].m_byItemNum,i);
	}
	if(m_iCompositeResultType == sITEM_COMPOSE_LIST::RESULTTYPE_REWARDCODE )
	{
		if(m_maxCompositeNum > 1) m_maxCompositeNum = 1;
	}
	if(bInitCompositeNum)
	{
		if(m_maxCompositeNum >= 1)
		{
			m_ManualCompositeNum = 1;
		}
		else
		{
			m_ManualCompositeNum = 0;
		}
	}
	
	// 아이템 가격은 무조건 기본 값이 1개 값 이상이다.
	int iManualCompositeNum = GetManualCompositeNum();
	if(iManualCompositeNum  == 0)
	{
		iManualCompositeNum = 1;
	}
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    MONEY total_money = 0;
    int ratio = g_pHero->GetPlayerAttribute()->GetAttrValue(eATTR_CRAFT_COST_RATIO);
    if (ratio != 0 && pInfo->m_Money > 0)
    {
        MONEY craft_cost_decrease = 0;
        bool is_plus = false;
        if (ratio > 0)
        {
            is_plus = true;
        }
        else
        {
            ratio = abs(ratio);
        }
        craft_cost_decrease = (pInfo->m_Money / 100 * ratio) * iManualCompositeNum;
        if (is_plus)
        {
            total_money = (pInfo->m_Money * iManualCompositeNum) + craft_cost_decrease;
        }
        else
        {
            total_money = (pInfo->m_Money * iManualCompositeNum) - craft_cost_decrease;
        }
    }
    else
    {
        total_money = pInfo->m_Money * iManualCompositeNum;        
    }
    m_pUIItemTotalComposite->SetNeedHaim( total_money );
#else
	m_pUIItemTotalComposite->SetNeedHaim( pInfo->m_Money * iManualCompositeNum);
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
	
}

void uiItemCompositeMan::ClearInfo()
{
	m_maxCompositeNum = 0;
	m_ManualCompositeNum = 0;
}

#ifdef _NA_007435_20140718_MODIFY_ITEMCOMPOSE_MATERIAL_CONDITION
BOOL uiItemCompositeMan::IsExistItemForComposite(
    SLOTCODE material_code,
    BYTE material_number,
    ItemType* item_type_ptr /*= NULL*/,
    DWORD item_type_index /*= 0*/)
{
    //ItemManager::Instance()->IsExistItem() 와 다른점은 인챈트 수치가 높아도 TRUE 라는 점.

    SCSlotContainer* container_ptr = ItemManager::Instance()->GetContainer(SI_INVENTORY);
    if (container_ptr == NULL)
    {
        return FALSE;
    }

    BASE_ITEMINFO* item_info_ptr = ItemInfoParser::Instance()->GetItemInfo(material_code);
    if (item_info_ptr == NULL) 
    {
        return FALSE;
    }

    BOOL is_exist = FALSE;

    //슬롯을 다돌면서 동일한아이템코드및 갯수를 체크해서 존재여부확인
    int max_slot_num = container_ptr->GetMaxSlotNum();
    bool is_found_item = false;
    for (int slot_pos = 0; slot_pos < max_slot_num; ++slot_pos)
    {
        if (container_ptr->IsEmpty(slot_pos))
        {
            continue;
        }

        is_found_item = false;
        SCItemSlot& item_slot_ref =  static_cast<SCItemSlot&>(container_ptr->GetSlot(slot_pos));
        if (item_slot_ref.GetCode() == material_code)
        {
            //슬롯추가정보(인첸,랭크,디바) 없을땐 기본만체크
            if(item_type_ptr != NULL)
            {
                if((item_slot_ref.GetEnchant() >= item_type_ptr->GetEnchant()) &&
                    (item_slot_ref.GetRank() == item_type_ptr->GetRank()) &&
                    (nsSlot::ItemTypeChanger::IsEqualItemType(item_slot_ref, item_type_ptr) == true))
                {
                    is_found_item = true;
                }
            }
            else
            {
                if (item_type_index == 0)
                {
                    if ((item_slot_ref.IsEtheriaItem() == false) && 
                        (item_slot_ref.IsShellItem() == false))
                    {
                        is_found_item = true;
                    }
                }
                else
                {
                    DWORD item_type_value = nsSlot::ItemTypeChanger::GetKindOfItemType(item_slot_ref);
                    if ((item_type_index - 1) == item_type_value)
                    {
                        is_found_item = true;
                    }
                }
            }

            if (is_found_item == true)
            {
                if (item_slot_ref.GetNum() >= material_number)
                {
                    is_exist = TRUE;
                    break;
                }
                else
                {
                    material_number -= item_slot_ref.GetNum();
                }
            }
        }
    }

    return is_exist;
}
#endif //_NA_007435_20140718_MODIFY_ITEMCOMPOSE_MATERIAL_CONDITION