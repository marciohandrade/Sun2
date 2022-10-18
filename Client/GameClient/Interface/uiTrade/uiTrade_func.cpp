//------------------------------------------------------------------------------
//  uiTrade_func.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "uiTrade.h"
#include "ItemManager.h"
#include "ItemUnitRenderer.h"
#include "AskReplyTradeDialog.h"
#include "InputNumTradeDialog.h"
#include "InventoryDialog.h"
#include "TargetDialog.h"
#include "Hero.h"
#include "MouseHandler.h"
#include "SCItemSlot.h"
#include "ObjectManager.h"
#include "slotkeygenerator.h"
#include "globalfunc.h"
#include "InterfaceManager.h"
#include "GameFunc.h"

//------------------------------------------------------------------------------
/** ITEM UNIT RENDERER
*/
void
uiTrade::InitItemRender()
{
    SAFE_RELEASENDELETE(m_pItemUnitRenderer);

    if (NULL == m_pItemUnitRenderer )
    {
        m_pItemUnitRenderer = new ItemUnitRenderer;
        m_pItemUnitRenderer->Init( uiBase::GetDrawBase(), GetMaxSlotNum() );
    }
}

//------------------------------------------------------------------------------
/** ITEM UNIT RENDERER
*/
void
uiTrade::ResetItemRenderer()
{
    SAFE_RELEASENDELETE(m_pItemUnitRenderer);

    this->InitItemRender();
}

//------------------------------------------------------------------------------
/** ITEM UNIT RENDERER
*/
ItemUnitRenderer * 
uiTrade::GetItemUnitRender() const
{
    return m_pItemUnitRenderer;
}

//------------------------------------------------------------------------------
/**
    working for slot clean
*/
void
uiTrade::FlushUnitDraw()
{
    SCSlot FromSlot;

    for (int i = 0; i < GetMaxSlotNum(); ++i)
    {
        if( !IsEmpty(i) )
            DeleteSlot(i, NULL);
    }

}

//------------------------------------------------------------------------------
/**
    pSlot for not allocated in heap
*/
RC::eSLOT_INSERT_RESULT		uiTrade::InsertSlot( POSTYPE AtPos, SCSlot & rSlot )
{	
	RC::eSLOT_INSERT_RESULT eInsertSlotResult = SCSlotContainer::InsertSlot( AtPos, rSlot );

    // 클라에서 따로 시리얼을 발급한다.

    DBSERIAL dwSerial;

    if (AtPos >= GetSlotSize1())
    {
        dwSerial = g_SlotKeyGenerator.GetKey();

        SCSlot & rRealSlot = GetSlot(AtPos);
        rRealSlot.SetSerial(dwSerial);

        CControlWZ * pCtrl = getControl(AtPos);
        RECT rect = pCtrl->GetSizeRect();

        SetCheckItem(AtPos, GameFunc::IsCheckItem(rRealSlot));

		if(m_pItemUnitRenderer)
		{
			m_pItemUnitRenderer->AddItemUnit( rRealSlot.GetSerial(), rRealSlot.GetCode(), &rect, &rRealSlot ,pCtrl);
		}
		else
		{
			assert(!"m_pItemUnitRenderer == NULL");
		}
    }
    else
    {
        dwSerial = rSlot.GetSerial();

        SCSlot & rRealSlot = GetSlot(AtPos);
        rRealSlot.SetSerial(dwSerial);

        SCItemSlot & rItemSlot = (SCItemSlot &)rSlot;
        rItemSlot.SetLock(TRUE);
        m_LockedItemPosMap.insert( EXTRA_INFO_PAIR(AtPos, rSlot.GetPos()) );

        CControlWZ * pCtrl = getControl(AtPos);
        RECT rect = pCtrl->GetSizeRect();

		if(m_pItemUnitRenderer)
		{
			m_pItemUnitRenderer->AddItemUnit( rRealSlot.GetSerial(), rRealSlot.GetCode(), &rect, &rRealSlot ,pCtrl);
		}
		else
		{
			assert(!"m_pItemUnitRenderer == NULL");
		}
    }

	return eInsertSlotResult;
}

//------------------------------------------------------------------------------
/**
*/
BOOL
uiTrade::DeleteSlot( POSTYPE AtPos, SCSlot * pSlotOut )
{
    SCSlot& slot = GetSlot(AtPos);

    if (AtPos < GetSlotSize1())
    {
        EXTRA_INFO_ITR itr = m_LockedItemPosMap.find( AtPos );
        if (itr != m_LockedItemPosMap.end()) 
        {
            POSTYPE FromPos = (POSTYPE)itr->second;
            SCSlotContainer * pContainer = ItemManager::Instance()->GetContainer(SI_INVENTORY);
            SCItemSlot & rItemSlot = (SCItemSlot &)pContainer->GetSlot(FromPos);
            rItemSlot.SetLock(FALSE);
            m_LockedItemPosMap.erase(itr);			
        }
    }
	else
    {
		g_SlotKeyGenerator.Restore((DWORD)slot.GetSerial());
        SetCheckItem(AtPos, false);
        
    }

    if (m_pItemUnitRenderer)
	{
        m_pItemUnitRenderer->RemoveItemUnit( slot.GetSerial() );
	}
	else
	{
		assert(!"m_pItemUnitRenderer == NULL");
	}

    return SCSlotContainer::DeleteSlot( AtPos, pSlotOut );
}

//------------------------------------------------------------------------------
/**
*/
bool
uiTrade::IsInInventoryItem(POSTYPE pos)
{
	TCHAR	szMessage[INTERFACE_STRING_LENGTH];

    InventoryDialog *pInven = GET_CAST_DIALOG( InventoryDialog, IM_INVENTORY_MANAGER::INVEN_DLG );
        
    SCSlot& invenSlot = pInven->GetSlot(pos);

    for (int i=0; i<POS_1_SLOT_SIZE; ++i)
    {
        SCSlot& rSlot = this->GetSlot(i);

        if (rSlot.GetCode())
        {
            // 거래창에 아이템이 있슴
            // 인벤창을 조사해서 올려놓은 아이템을 삭제

            if (rSlot.GetSerial() == invenSlot.GetSerial())
            {
				//	이미 올려져 있는 아이템입니다.
				g_InterfaceManager.GetInterfaceString( eST_ALREADY_INVENTORY_ITEM, szMessage, INTERFACE_STRING_LENGTH );
                GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
                MouseHandler::Instance()->RollbackItemAtHand();
                return true;
            }
        }
    }

    return false;
}

//------------------------------------------------------------------------------
/**
*/
void
uiTrade::TradeEnd(TRADE_TOTAL_INFO tradeInfo)
{
    int i;

	InventoryDialog *pInven = GET_CAST_DIALOG( InventoryDialog, IM_INVENTORY_MANAGER::INVEN_DLG );
	assert (pInven);

    for (i=0; i<tradeInfo.m_InvenCount; ++i)
    {
		ITEMSLOTEX slot = tradeInfo.m_Slot[i];
		SCItemSlot scSlot(slot.m_Stream);
        pInven->InsertSlot(slot.m_Pos, (SCSlot &)scSlot);
    }

    for (i=0; i<POS_1_SLOT_SIZE; ++i)
    {
        SCSlot& rSlot = this->GetSlot(i);

        if (rSlot.GetCode())
        {
            // 거래창에 아이템이 있슴
            // 인벤창을 조사해서 올려놓은 아이템을 삭제

            InventoryDialog *pInven = GET_CAST_DIALOG( InventoryDialog, IM_INVENTORY_MANAGER::INVEN_DLG );

            POSTYPE atPosIter;

            for (atPosIter=0;
                atPosIter<pInven->GetMaxSlotNum();
                ++ atPosIter)
            {
                SCSlot& invenSlot = pInven->GetSlot(atPosIter);

                if (invenSlot.GetCode())
                {
                    if (rSlot.GetSerial() == invenSlot.GetSerial())
                    {
                        pInven->DeleteSlot(atPosIter, NULL);
                        break;
                    }
                }
            }
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void
uiTrade::PutOneStart()
{
    CControlWZ * pControl = getControl(DLGPOS_SLOT00);
    if (pControl)
    {
        RECT rcSlot = pControl->GetSizeRect();
        MouseHandler::Instance()->ItemTransaction(SI_TRADE, DLGPOS_SLOT00, &rcSlot);
    }
}

//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
