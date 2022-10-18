#include "SunClientPrecompiledHeader.h"

#include "uiItemCompositeMan/uiItemCompositeMan.h"
#include "uiItemTotalComposite.h"
#include "ItemComposeListParser.h"
#include "ItemUnitRenderer.h"
#include "SlotKeyGenerator.h"
#include "SCItemSlot.h"
#include "ItemManager.h"
#include "ItemTypeList.h"
#include "GameFunc.h"

void ChangeItemSlotType(SCItemSlot& item_slot, DWORD item_type)
{
    switch (item_type)
    {
    case 2:
        item_slot.SetSet(1);
        break;
    case 3:
        item_slot.ChangeToEtheriaItem();
        break;
    case 4:
        item_slot.SetSet(1);
        item_slot.ChangeToEtheriaItem();
        break;
    case 5:
        item_slot.ChangeToShellItem();
        break;
    }
}

uiItemCompositeMan*
uiItemTotalcomposite::GetManager()
{
	uiItemCompositeMan* puiItemCompositeMan =
		static_cast<uiItemCompositeMan *>(this->GetUIMan()->GetUserInterfaceManager(UIMAN_ITEM_COMPOSITE));
	assert (puiItemCompositeMan);
	return puiItemCompositeMan;
}


//-------------------------------------------------------------------------------------------
/**
*/
void					
uiItemTotalcomposite::ClearInfo()
{
	for (int i = 0; i < GetMaxSlotNum(); ++i)
	{
		if ( !IsEmpty(i) )
			DeleteSlot(i, NULL);
	}

	
	for ( int i = 0; i < MATERIAL_NAME_SIZE; ++i )
	{
		GlobalFunc::SetCaption( this->m_pMaterialName[i], _T(""));
	}
    GlobalFunc::SetCaption(this->m_pCreateRate, _T(""));
	GlobalFunc::SetCaption(this->m_pStatic[TXT_RESULT_ITEM], _T(""));
	GlobalFunc::SetCaption(m_pStatic[TXT_MANUAL_NUM], _T("0"));
	GlobalFunc::SetCaption(m_pStatic[TXT_CHARGE],_T("0"));
	GlobalFunc::SetCaption(m_pStatic[TXT_RESULT_ITEM],_T(""));
	int curSel = m_pList_Each_Category->GetCurSel();
	if(curSel <0)
	{
		m_iCurSelectedItemCode = 0;
	}
	else
	{
		int * pCode =(int *) m_pList_Each_Category->GetItemData(curSel);
		if(pCode)
			m_iCurSelectedItemCode = *pCode;
		else
			m_iCurSelectedItemCode = 0;
	}

#ifdef _SCJ_LONG_TEXT_REDUCING
    material_name_tooltip_list_.clear();
#endif //_SCJ_LONG_TEXT_REDUCING
}

//-------------------------------------------------------------------------------------------
/**
*/
BOOL					
uiItemTotalcomposite::AppendMat( POSTYPE AtPos, sITEM_COMPOSE_LIST::sITEM_FOR_COMPOSE CompositeInfo, BOOL bExist )
{
	if ( AtPos < 0 && AtPos >= this->GetMaxSlotNum() )
	{
		return FALSE;
	}

	SCItemSlot ItemSlot;
	ItemSlot.SetCode(CompositeInfo.m_ItemCode);
	//assert( !ItemSlot.WithDura() );
	ItemSlot.SetNum(CompositeInfo.m_byItemNum);

    // 인챈트 수치 세팅
    ItemType * pItemType = ItemTypeList::Instance()->FindItemType(CompositeInfo.m_ItemTypeIndex);
    if(pItemType)
    {
        BYTE enchant_number = pItemType->GetEnchant();
        ItemSlot.SetEnchant(enchant_number);
    }

    const BASE_ITEMINFO* item_info = ItemSlot.GetItemInfo();
    if (item_info == NULL)
    {
        return FALSE;
    }

    ChangeItemSlotType(ItemSlot, CompositeInfo.m_ItemTypeIndex);    
    
    GameFunc::GetItemName(&ItemSlot, item_info, m_szText, INTERFACE_STRING_LENGTH);

    GlobalFunc::SetCaption( this->m_pMaterialName[AtPos], this->m_szText);

#ifdef _SCJ_LONG_TEXT_REDUCING
    RECT control_size = m_pMaterialName[AtPos]->GetSize();
    int control_width = control_size.right - 8;
    int text_width = GlobalFunc::GetTextWidth(m_szText, m_pMaterialName[AtPos]->GetFontID());
    if (text_width > control_width)
    {
        DWORD control_id = m_pMaterialName[AtPos]->GetCtrlID();
        std::string material_name = m_szText;
        MaterialNamePair tooltip_value = MaterialNamePair(control_id, material_name);
        material_name_tooltip_list_.push_back(tooltip_value);
    }
#endif //_SCJ_LONG_TEXT_REDUCING

	DWORD dwColor = DEACTIVE_COLOR;
	if ( bExist )
	{
		dwColor = RGBA(255,255,255,255);
	}
	this->m_pMaterialName[AtPos]->SetTextColor( dwColor );

	InsertSlot( AtPos, ItemSlot );
	ItemUnitRenderer::RenderUnit * pRenderUnit = 
		this->m_pTotalItemUnitRenderer->GetItemUnit( GetSlot(AtPos).GetSerial() );
	if ( pRenderUnit )
	{
		pRenderUnit->SetState((bExist)? SLOT_RENDER_STATE_ACTIVATED : SLOT_RENDER_STATE_WARNING);
#ifdef _GS_ADD_COMPOSITE_OVERLAP_NUM
        pRenderUnit->SetForceOverLap(TRUE);
#endif//_GS_ADD_COMPOSITE_OVERLAP_NUM
	}

	return TRUE;
}

//-------------------------------------------------------------------------------------------
/**
*/
BOOL uiItemTotalcomposite::AppendResultMat( POSTYPE AtPos, BOOL bExist)
{
	
	sITEM_COMPOSE_LIST * pResultInfo = ItemComposeListParser::Instance()->GetComposeListInfo(m_iCurSelectedItemCode);
	if(!pResultInfo)
	{
		return FALSE;
	}
	if ( AtPos < 0 && AtPos >= this->GetMaxSlotNum() )
	{
		return FALSE;
	}

	bExist = TRUE;
		
    SCItemSlot ItemSlot;
	ItemSlot.SetCode(pResultInfo->m_Result.m_ItemCode);
	//assert( !ItemSlot.WithDura() );
	ItemSlot.SetNum(pResultInfo->m_Result.m_byItemNum);
    
    // 인챈트 수치 세팅
    ItemType* item_type = ItemTypeList::Instance()->FindItemType(pResultInfo->m_Result.m_ItemTypeIndex);
    if (item_type)
    {
        BYTE enchant_number = item_type->GetEnchant();
        ItemSlot.SetEnchant(enchant_number);
    }

    const BASE_ITEMINFO* item_info = ItemSlot.GetItemInfo();
    if (item_info == NULL)
    {
        return FALSE;
    }
    ChangeItemSlotType(ItemSlot, pResultInfo->m_Result.m_ItemTypeIndex);    

	InsertSlot( AtPos, ItemSlot );
	ItemUnitRenderer::RenderUnit * pRenderUnit = 
		this->m_pTotalItemUnitRenderer->GetItemUnit( GetSlot(AtPos).GetSerial() );
	if ( pRenderUnit )
	{
    	pRenderUnit->SetState((bExist)? SLOT_RENDER_STATE_ACTIVATED : SLOT_RENDER_STATE_WARNING);
	}

	TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
    GameFunc::GetItemName(&ItemSlot, item_info, szMessage, INTERFACE_STRING_LENGTH);
	GlobalFunc::SetCaption(m_pStatic[TXT_RESULT_ITEM],szMessage);
	
	DWORD dwColor = RGBA(255,255,255,255);
	m_pStatic[TXT_RESULT_ITEM]->SetTextColor(dwColor);

	return TRUE;
}

//-------------------------------------------------------------------------------------------
/**
*/
ItemUnitRenderer *	
uiItemTotalcomposite::GetItemUnitRender() const
{
	return this->m_pTotalItemUnitRenderer;
}

//-------------------------------------------------------------------------------------------
/**
*/
RC::eSLOT_INSERT_RESULT		uiItemTotalcomposite::InsertSlot( POSTYPE AtPos, SCSlot & rSlot )
{
	/// 슬롯에 넣음
	assert( IsEmpty(AtPos) );

	RC::eSLOT_INSERT_RESULT eInsertSlotResult = SCSlotContainer::InsertSlot( AtPos, rSlot );

	DBSERIAL dwSerial = g_SlotKeyGenerator.GetKey();
	SCSlot & rSetSlot = GetSlot(AtPos);
	rSetSlot.SetSerial(dwSerial);

	/// 인벤토리
	CControlWZ * pCtrl = getControl( AtPos + DIALOG_PIC_SLOT1);
	RECT rect = pCtrl->GetSizeRect();

	m_pTotalItemUnitRenderer->AddItemUnit( dwSerial, rSetSlot.GetCode(), &rect, &rSetSlot, pCtrl );	

	return eInsertSlotResult;
}

//-------------------------------------------------------------------------------------------
/**
*/
BOOL				
uiItemTotalcomposite::DeleteSlot( POSTYPE AtPos, SCSlot * pSlotOut )
{
	SCSlot & rSlot = GetSlot(AtPos);
	g_SlotKeyGenerator.Restore((DWORD)rSlot.GetSerial());

    if (m_pTotalItemUnitRenderer)
    {
	    m_pTotalItemUnitRenderer->RemoveItemUnit( rSlot.GetSerial() );
    }
	return SCSlotContainer::DeleteSlot(AtPos, pSlotOut);
}

//-------------------------------------------------------------------------------------------
/**
*/
void					
uiItemTotalcomposite::updateInfo(BOOL bInitCompositeNum )
{
	DWORD dwStringID = 0;
	eITEMCOMPOSITECODE Code = this->GetManager()->GetCompositeCode();
	
	GetManager()->UpdateInfo(bInitCompositeNum);
	updateMatInfo();

	TCHAR szMessage[64] = {0,};
	Sprintf(szMessage,_T("%d"),GetManager()->GetManualCompositeNum());
	GlobalFunc::SetCaption(m_pStatic[TXT_MANUAL_NUM],szMessage);

}

//-------------------------------------------------------------------------------------------
/**
*/
void					
uiItemTotalcomposite::updateMatInfo()
{
	BOOL bExist = FALSE;
	BOOL isExistAllItem = TRUE;

	ItemUnitRenderer::RenderUnit * pRenderUnit = NULL;
	ItemType * pItemType = NULL;

    sITEM_COMPOSE_LIST * pInfo = ItemComposeListParser::Instance()->GetComposeListInfo( m_iCurSelectedItemCode );
    if (pInfo == NULL)
    {
        return;
    }
    m_byCreateRate = pInfo->m_byCreateRatio;

    TCHAR final_message[INTERFACE_STRING_LENGTH] = {0,};
    TCHAR message[INTERFACE_STRING_LENGTH] = {0,};
    TCHAR rate_string[INTERFACE_STRING_LENGTH] = {0,};

    g_InterfaceManager.GetInterfaceString(5729, message);   // 5729 (조합 성공률: %s)
    if (m_byCreateRate <= 15)
    {
        g_InterfaceManager.GetInterfaceString(5730, rate_string);   // 5730 매우 낮음
        m_pCreateRate->SetTextColor(DEACTIVE_COLOR);
    }
    else if (m_byCreateRate > 15 && m_byCreateRate <= 35)
    {
        g_InterfaceManager.GetInterfaceString(5731, rate_string);   // 5731 낮음
        m_pCreateRate->SetTextColor(DEACTIVE_COLOR);
    }
    else if (m_byCreateRate > 35 && m_byCreateRate <= 65)
    {
        g_InterfaceManager.GetInterfaceString(5732, rate_string);   // 5732 보통
        m_pCreateRate->SetTextColor(RGBA(255, 255, 255, 255));
    }
    else if (m_byCreateRate > 65 && m_byCreateRate <= 85)
    {
        g_InterfaceManager.GetInterfaceString(5733, rate_string);   // 5733 높음
        m_pCreateRate->SetTextColor(ACTIVE_COLOR);
    }
    else if (m_byCreateRate > 85 && m_byCreateRate <= 99)
    {
        g_InterfaceManager.GetInterfaceString(5734, rate_string);   // 5734 매우 높음
        m_pCreateRate->SetTextColor(ACTIVE_COLOR);
    }
    else // 100% 일 때 확실
    {
        g_InterfaceManager.GetInterfaceString(5735, rate_string);   // 5735 확실
        m_pCreateRate->SetTextColor(RGBA(0, 255, 0, 255));
    }
    Sprintf(final_message, message, rate_string);
    GlobalFunc::SetCaption(m_pCreateRate, final_message);

	for ( int i = 0; i < MATRIAL_MAX; ++i )
	{
		if ( IsEmpty(i) )
		{	
			break;
		}

		SCItemSlot & rItemSlot = (SCItemSlot &)GetSlot(i);
		
		pItemType = NULL;
		pItemType = ItemTypeList::Instance()->FindItemType( pInfo->m_pMaterial[i].m_ItemTypeIndex);
		
#ifdef _NA_007435_20140718_MODIFY_ITEMCOMPOSE_MATERIAL_CONDITION
        bExist = GetManager()->IsExistItemForComposite(
            rItemSlot.GetCode(), 
            rItemSlot.GetNum(), 
            pItemType,
            pInfo->m_pMaterial[i].m_ItemTypeIndex);
#else
		bExist = ItemManager::Instance()->IsExistItem(
			SI_INVENTORY, 
			rItemSlot.GetCode(), 
			rItemSlot.GetNum(), 
			pItemType,
            pInfo->m_pMaterial[i].m_ItemTypeIndex);
#endif //_NA_007435_20140718_MODIFY_ITEMCOMPOSE_MATERIAL_CONDITION

		if(!bExist)  isExistAllItem = FALSE;

		pRenderUnit = this->m_pTotalItemUnitRenderer->GetItemUnit(rItemSlot.GetSerial());
		if (pRenderUnit)
		{
			pRenderUnit->SetState( (bExist)? SLOT_RENDER_STATE_ACTIVATED : SLOT_RENDER_STATE_WARNING );
		}

		DWORD dwColor = DEACTIVE_COLOR;
		if ( bExist )
		{
			dwColor = ACTIVE_COLOR;
		}
		this->m_pMaterialName[i]->SetTextColor(dwColor);
	}

}


BOOL					
uiItemTotalcomposite::SetNeedHaim( MONEY Money )
{
	NUMBERFMT nFmt = { 0, 0, 3, _T("."), _T(","), 1 };
	// 돈을 셋팅한다.
	Sprintf(this->m_szText, _T("%I64u"), Money);

	::GetNumberFormat(
		LOCALE_SYSTEM_DEFAULT, 
		NULL, 
		m_szText, 
		&nFmt, 
		m_szResult, 
		sizeof(m_szResult));

	GlobalFunc::SetCaption(m_pStatic[TXT_CHARGE], m_szResult);

	return TRUE;
}

BOOL uiItemTotalcomposite::SetCurrentSelectedItemCode()
{
	int curSel = m_pList_Each_Category->GetCurSel();
	int * pCode =(int *) m_pList_Each_Category->GetItemData(curSel);
	if(pCode)
	{
		m_iCurSelectedItemCode = * pCode;
		if(m_iCurSelectedItemCode != 0)
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	else
	{
		return FALSE;
	}
}

void uiItemTotalcomposite::updateAllInfomation()
{
	GetManager()->fillTotalCompositeInfo(m_iCurSelectedItemCode);
	GetManager()->SetManualCompositeNum(GetManager()->GetMaxCompositeNum());
	updateInfo();
}

void uiItemTotalcomposite::updateMouseWheel()
{
	if( !m_pList_Each_Category || !m_pVScroll)
		return;

	RECT rc = m_pList_Each_Category->GetSizeRect();
	switch (GetMouseWheelStatus(&rc))
	{
	default:
		break;
	case MOUSE_WHEEL_UP:
		{
			if(m_pList_Category->GetState() == WZ_SHOW)
			{
				if( m_pList_Category->GetListRowCnt() > MAX_LIST_DATA)
				{
					// 위로 올릴수 있는 상태인가?
					if ( m_iDropdownCurScrollValue > 0 )
					{
						m_iDropdownCurScrollValue -= 1;
						m_pVScrollDrapDown->SetScrollPosWZ( m_iDropdownCurScrollValue );
						m_pList_Category->SetBeginDrawIndex(m_iDropdownCurScrollValue);
					}
				}
			}
			else
			{
				DecreaseListCategoryScroll();
			}
			
			
		}
		break;

	case MOUSE_WHEEL_DOWN:
		{
			if(m_pList_Category->GetState() == WZ_SHOW)
			{
				if( m_pList_Category->GetListRowCnt() > MAX_LIST_DATA)
				{
					// 아래로 내릴수 있는 상태인가?
					if ( m_iDropdownCurScrollValue < m_iDropdownMaxScrollValue )
					{
						m_iDropdownCurScrollValue += 1;
						m_pVScrollDrapDown->SetScrollPosWZ( m_iDropdownCurScrollValue );
						m_pList_Category->SetBeginDrawIndex(m_iDropdownCurScrollValue);
					}
				}
			}
			else
			{
				IncreaseListCategoryScroll();
			}
		}
		break;
	}
}

void uiItemTotalcomposite::IncreaseListCategoryScroll()
{
	if( m_pList_Each_Category->GetListRowCnt() > MAX_LIST_DATA)
	{
		// 아래로 내릴수 있는 상태인가?
		if ( m_iCurScrollValue < m_iMaxScrollValue )
		{
			m_iCurScrollValue += 1;
			m_pVScroll->SetScrollPosWZ( m_iCurScrollValue );
			m_pList_Each_Category->SetBeginDrawIndex(m_iCurScrollValue);
		}
	}
}
void uiItemTotalcomposite::DecreaseListCategoryScroll()
{
	if( m_pList_Each_Category->GetListRowCnt() > MAX_LIST_DATA)
	{
		// 위로 올릴수 있는 상태인가?
		if ( m_iCurScrollValue > 0 )
		{
			m_iCurScrollValue -= 1;
			m_pVScroll->SetScrollPosWZ( m_iCurScrollValue );
			m_pList_Each_Category->SetBeginDrawIndex(m_iCurScrollValue);
		}
	}
}